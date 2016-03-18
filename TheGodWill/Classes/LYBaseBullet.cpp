//
//  LYBaseBullet.cpp
//  TheGodWill
//
//  Created by user on 14/12/23.
//
//

#include "LYBaseBullet.h"

#include "LYBaseFighter.h"

#include "LYFightData.h"

#include "LYFightManager.h"
#include "DataManager.h"

#include "Common.h"


USING_NS_CC;


static int bulletNumTest = 0;

CBaseBullet::CBaseBullet(){
    
    m_fStoppedTime=-0.2f;
    m_bIfOver = false;
    m_fDamagePercent = 1;
    
    m_fDamageLifePercent=0;
    
    m_fNowLifeTime=0;
    m_fMaxLifeTime=0;
    
    bulletNumTest++;
    
    CCLOG("bulletNum:%d",bulletNumTest);
    
    m_pAttacker = NULL;
    
    m_pThebulletModel = NULL;
    
    
}


CBaseBullet::~CBaseBullet(){
    
    bulletNumTest--;
    
    CCLOG("bulletNum:%d",bulletNumTest);
}


void CBaseBullet::setAttackerAndPositionAndRun(CBaseFighter *attacker, cocos2d::Vec3 targetPos ,int bulletId){
    
    
    m_pAttacker = attacker;
    
    m_v3TargetPos = targetPos;
    
    
    bulletInfoDB* theBulletInfo=NULL;
    
    if (bulletId!=0) {
        theBulletInfo = CDataManager::getBulletInfoByBulletId(bulletId);
    }else{
        theBulletInfo = attacker->getTheInfo()->theBulletInfo;
    }
    
    
    
    
    //计算平面方向速度
    Vec3 startPoint = getPosition3D();
    
    Vec3 endPoint = m_v3TargetPos;
    
    
    float moveDistance = startPoint.distance(endPoint);
    
    //总位移实际那
    float moveTime = moveDistance/theBulletInfo->bulletSpeed;
    
    //子弹的生命周期是实际位移时间的3倍，防止漏算。
    m_fMaxLifeTime = moveTime*3;
    
    
    
    //各个方向上每秒位移
    float speedX = (endPoint.x-startPoint.x)/moveTime;
    float speedZ = (endPoint.z-startPoint.z)/moveTime;
    
    
    
    
    /*
     y轴上速度计算
     */
    float speedY = (endPoint.y-startPoint.y)/moveTime;
    
    if (theBulletInfo->bulletType==0) {
        
        if(targetPos.y<100){//只有在抛物线的目标不是空军的时候，才走抛物线
            
            float time = moveTime*0.5f;
            int distance = (rand()%BULLET_HIGHT)*0.3 + BULLET_HIGHT*0.7;//随机最高高度
            
            //        speedY = (-0.5* BULLET_ACCELERATE *time*time+distance)/time;
            speedY = BULLET_ACCELERATE*time;
            
        }else{ //否则所有速度加倍
            
            speedX = speedX*2;
            speedY = speedY*2;
            speedZ = speedZ*2;
            
        }
        
        
    }
    
    
    
    
    
    
    m_v3Speed = Vec3(speedX, speedY, speedZ);
    
    
    
    
    //模型初始化
    m_pThebulletModel = CFightManager::getTheModelWithPath(theBulletInfo->filePath);
    addChild(m_pThebulletModel);
    CFightManager::addCamera(m_pThebulletModel);
    
    m_pThebulletModel->setScale(theBulletInfo->bulletScale);
    
    
    if (theBulletInfo->filePath.find(".png")!=-1) {
        bulletModelType=1;
    }else{
        bulletModelType = 0;
    }
    
    
    
    if (theBulletInfo->particlePath!="0") {
        ParticleSystem *particle = ParticleSystemQuad::create(theBulletInfo->particlePath);
        CFightManager::addCamera(particle);
        particle->setScale(1);
        particle->setPosition3D(Vec3(0, 0, 0));
        m_pThebulletModel->addChild(particle,10);
        particle->setGlobalZOrder(1001);
    }
    
    
    
    //测试用
//    m_pThebulletModel = Sprite3D::create("bulletModels/arrow2/arrow2.c3b");
//    addChild(m_pThebulletModel);
//    CFightManager::addCamera(m_pThebulletModel);
    
    
    
    //大小修改
    int TempbulletId = theBulletInfo->bulletId;
    if (TempbulletId==1||TempbulletId==2||TempbulletId==3||TempbulletId==9) {
        char buf[256];
        sprintf(buf, "music/%s", CDataManager::getBulletInfoByBulletId(TempbulletId)->hitEffect.c_str());
        SimpleAudioEngine::getInstance()->playEffect(buf);//弓箭手射箭音效
//        setScale(0.4f);
    }
    
    
    
    //动作
//    m_pThebulletModel->runAction(RepeatForever::create(RotateBy::create(0.5f, Vec3(0, 360, 0))));
    
    
    
    //测试用
//    m_pThebulletModel->setScale(50);
//    m_pThebulletModel->setScaleY(50);
    
  
    scheduleUpdate();
    

}

void CBaseBullet::update(float time){
    
    m_fNowLifeTime+=time;
    
    if (m_bIfOver) {
        onDestroy();
        return;
    }
    
    
    float minHeight = 0;//子弹的最低高度
    
    if (m_pAttacker->getTheInfo()->theBulletInfo&&m_pAttacker->getTheInfo()->theBulletInfo->bulletType==0) { //如果是抛物线，则最低距离提高
        minHeight = 30;
    }
    
    
    
    if ((getPosition3D().y>minHeight||m_v3Speed.y>=0)
        &&(getPosition3D().y<400)) { //子弹不能超过的最高高度
        
        //位置变化
        setPosition3D( getPosition3D()+m_v3Speed*time);
        
        //速度变化,如果是抛物线类型
        if (m_pAttacker->getTheInfo()->theBulletInfo&&m_pAttacker->getTheInfo()->theBulletInfo->bulletType==0&&m_v3TargetPos.y<100) {
            m_v3Speed.y -= BULLET_ACCELERATE*time;
//            CCLOG("%f",m_v3Speed.y);
        }
        
        rotationUpdate();
        
    }else{
        
        m_pThebulletModel->stopAllActions();
        
        
        m_fStoppedTime+=time;
    }
    
    if (m_fStoppedTime>=m_pAttacker->getTheInfo()->theBulletInfo->lifeTime) {//如果超过了存在时间
        onDestroy();
        return;
    }
    
    
    float distance = Vec2(getPositionX(),getPositionZ()).distance(Vec2(m_v3TargetPos.x, m_v3TargetPos.z));
    
    if (m_pAttacker->getTheInfo()->theBulletInfo->bulletType==1&&m_fNowLifeTime>=m_fMaxLifeTime) {//如果是平射，并且到了目标位置
        onDestroy();
        return;
    }
    
    onDamage();
    
    
}


void CBaseBullet::rotationUpdate(){
    
    
    if (bulletModelType==1) {
        return;
    }
    
    
//    return;
    
    CommonData::changeRotation3DWithVec(Vec3(0,1,0), m_v3Speed, this);
    
    
}





void CBaseBullet::onDestroy(bool ifRemoveSelf){
    
    //这里涉及到一个问题，如果怪物死亡时删掉子弹，最好再子弹自身的update中删除，这样能保证不会在走程序逻辑时发现自己被删掉了，从而避免野指针
    
    m_bIfOver = true;
    
    unscheduleUpdate();
    
    if (m_pAttacker) {
        m_pAttacker->getAllBullets()->eraseObject(this);
        m_pAttacker = NULL;
    }
    
    
    if (ifRemoveSelf) {
        removeFromParentAndCleanup(true);
    }
    
}

void CBaseBullet::onExit(){
    
    
    onDestroy(false);
    
    Node::onExit();
    
    
}


void CBaseBullet::onDamage(){
    
    
    if (m_bIfOver) {
        return;
    }
    
    //获取所有敌人
    Vector<CBaseFighter*>targetVec = CFightData::getInstance()->getFightTargetByGroup(m_pAttacker->getFightGroup());
    
    
    for (int i=0; i<targetVec.size(); i++) {
        
        
        CBaseFighter* theTarget = targetVec.at(i);
        
        
        if (theTarget->getIfDied()) {//如果敌人死亡了，则跳过
            continue;
        }
        
        //伤害距离内的造成伤害
        float distance = Vec2(getPositionX(),getPositionZ()).distance(theTarget->getFightPosition());
        
        if (distance<m_pAttacker->getTheInfo()->theBulletInfo->bulletDamageRange) {
            
            
            //如果造成生命值百分比伤害则
            if(m_fDamageLifePercent!=0){
                
                float damage = theTarget->getNowHP()*m_fDamageLifePercent+m_pAttacker->getNowHP();
                
                
                theTarget->beAttacked(m_pAttacker,damage,1,m_pAttacker->getFightGroup(),eDamageType_SoliderSkill);//普通攻击力及额外生命上限攻击力
                
            }else{
                
                
                //如果伤害百分比不为1，则表示是技能，因此伤害类型也是技能
                if (m_fDamagePercent!=1) {
                    
                    theTarget->beAttacked(m_pAttacker,0,m_fDamagePercent,m_pAttacker->getFightGroup(),eDamageType_SoliderSkill);
                    
                }else{
                    theTarget->beAttacked(m_pAttacker,0,m_fDamagePercent,m_pAttacker->getFightGroup());
                }
                
            }
            
            
            
            theTarget->animateChange(eFighteState_Hurt);
            
            if (!m_pAttacker||! m_pAttacker->getTheInfo()) {  //此处大坑，被攻击者反伤杀死攻击者，子弹的所有者被置空。价格判断
                return;
            }
            
            
            
            
            bulletInfoDB* temp = m_pAttacker->getTheInfo()->theBulletInfo;
            //子弹的命中特效
            if (temp->hurtAnimate.animatePath!="0") {
                
                __String* tempStr = __String::createWithFormat("%s1.png",temp->hurtAnimate.animatePath.c_str());
                Sprite* bulletHurt = Sprite::createWithSpriteFrameName(tempStr->getCString());
                
                Animate* theAnimate = CFightManager::getAnimateWithPath(temp->hurtAnimate.animatePath.c_str(), 1, temp->hurtAnimate.animateNumber);
                
                bulletHurt->setScale(2);
                
                bulletHurt->runAction(Sequence::create(theAnimate,RemoveSelf::create(),NULL));
                CFightManager::addCamera(bulletHurt);
                
                bulletHurt->setPosition3D(Vec3(0,theTarget->getPhysicalPosition().y+theTarget->gettheSprite()->getBoundingBox().size.height*0.5f,0));
                
                theTarget->addChild(bulletHurt);
                
                
            }
            
            
            
            
            m_bIfOver = true;
            
            return;
            
        }
        
        
        
    }
    
    
    
    
    
    
    
}
