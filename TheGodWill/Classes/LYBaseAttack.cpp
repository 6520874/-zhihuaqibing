//
//  LYBaseAttack.cpp
//  TheGodWill
//
//  Created by user on 14/12/2.
//
//

#include "LYBaseAttack.h"
#include "guiderManagerLayer.h"
#include "LYFightData.h"

#include "PublicDefine.h"

#include "LYBaseBuff.h"

#include "LYBaseFighter.h"

#include "Common.h"

USING_NS_CC;


static int m_count =0;

CBaseAttack::CBaseAttack(){
    
    m_iHurtedNumber=0;
    m_pBulletSprite = NULL;
    
    m_iNowHurtTimes=0;
    m_fNowLifeTime=0;
    
    
    
    m_count++;
    CCLOG("总共技能%d",m_count);
    
    m_pTheskillInfo = NULL;
    
    
    m_pBulletSprite = NULL;
    
    m_pBulletAnimate = NULL;
    
    m_pAttacker = NULL;
    
    m_pTarget = NULL;
    
    m_fAcceleratedSpeedY=0;
    
    ifDestroyed = false;
}

CBaseAttack::~CBaseAttack(){
    
    m_count--;
    CCLOG("总共技能%d",m_count);
    
//    CCLOG("释放技能名%s",m_pTheskillInfo->theSkillInfo->skillName.c_str());
}



void CBaseAttack::setSkillIdAndStart(FighterSkillInfo* theInfo,CBaseFighter* attacker,CBaseFighter* targetAttacker,Vec3 targetPos,int damage,FightGroup attackerGroup){
    
    
    if (!theInfo||!theInfo->theSkillInfo) {//判断是否有技能
        onDestroy();
        return;
    }
    
    
//    if(!attacker){
//        return;
//    }
    
    //此处根据skillid 和技能等级， 获取相关的技能信息。
    m_pTheskillInfo = theInfo;
    
//    CCLOG("生成技能名%s",m_pTheskillInfo->theSkillInfo->skillName.c_str());
    
    
    if (m_pTheskillInfo->theSkillInfo->skillName=="") {
        
        CCASSERT(0, "skillInfo error");
        
    }


    m_pAttacker = attacker;
    m_pTarget = targetAttacker;
    
    if (!m_pAttacker) {
        m_eAttackerGroup = attackerGroup;
        m_iDamage = damage;
    }else{
        m_eAttackerGroup = m_pAttacker->getFightGroup();
        //根据技能提升算技能能够造成的伤害
        m_iDamage = m_pTheskillInfo->damageRatio* m_pAttacker->getNowAttack();
    }
    
    
    if (targetAttacker) {
        m_v3TargetPos = targetAttacker->getPhysicalPosition();
    }else{
        m_v3TargetPos = targetPos;
    }
    

    
    
    
    if (theInfo->theSkillInfo->moveSpeed==0) { //移动速度大于0,则特效初始点在施法者，然后从施法者飞到目标点，移动速度为0表示，直接出现在目标点位置上
        setPosition3D(m_v3TargetPos+m_pTheskillInfo->theSkillInfo->offset);
    }else{
        if (m_pAttacker) {
            setPosition3D(m_pAttacker->getPosition3D()+m_pTheskillInfo->theSkillInfo->offset);
        }else{
            CCLOG("LYASSERT:技能无攻击者，且选择从攻击者飞到目标");
        }
        
    }
    
    if(m_pTheskillInfo)
    {
        __String * temp = __String::createWithFormat("music/%s", m_pTheskillInfo->theSkillInfo->skillEffect.c_str());
        SimpleAudioEngine::getInstance()->playEffect(temp->getCString());
    }
    
    
//    //此处测试用
//    if(skillId!=-1){
//        
//        m_stAttackInfo.MaxlifeTime = 0;
//        m_stAttackInfo.theSkillType = eSkillType_static;
//        m_stAttackInfo.attackMoveSpeed=0;
//        m_stAttackInfo.damageRange = 30;
//        m_stAttackInfo.hurtNum = 5;
//        
//    }else{
//        
//        m_stAttackInfo.MaxlifeTime = -1;
//        m_stAttackInfo.theSkillType = eSkillType_Move;
//        m_stAttackInfo.hurtNum = 99;
//        m_stAttackInfo.damageRange = 15;
//        m_stAttackInfo.attackMoveSpeed=30;
//        
//    }
    
    
    
    
//    m_stAttackInfo.bulletName = "0";
//    m_stAttackInfo.damageRatio = 3;
    
    
    
    
    
    
    
    
    
    
    
    
    animateStart();
    
    
    
    
    m_fNowLifeTime=0;
    
    
    //计算x轴，y轴，z轴速度
    
    if (m_pTheskillInfo->theSkillInfo->moveSpeed==0) {
        m_v3BulletSpeed=Vec3::ZERO;
    }else{
        
        Vec3 startPoint = getPosition3D();
        
        Vec3 endPoint = m_v3TargetPos;
        
        
        float moveDistance = startPoint.distance(endPoint);
        
        //总位移实际那
        float moveTime = moveDistance/m_pTheskillInfo->theSkillInfo->moveSpeed;
        
        //各个方向上每秒位移
        float speedX = (endPoint.x-startPoint.x)/moveTime;
        float speedY = (endPoint.y-startPoint.y)/moveTime;
        float speedZ = (endPoint.z-startPoint.z)/moveTime;
        
        
        //如果有高度上的位移，代表是抛物线，y轴速度需要修改
        if(m_pTheskillInfo->theSkillInfo->moveSpeedY>0){
            
            
            speedY = m_pTheskillInfo->theSkillInfo->moveSpeedY;
            
            
//            //测试用，看到就删
//            speedY = 300;
            
            m_fAcceleratedSpeedY =  -2*(m_pTheskillInfo->theSkillInfo->offset.y+speedY*moveTime)/(moveTime*moveTime);

        }
        
        
        
        m_v3BulletSpeed = Vec3(speedX, speedY, speedZ);
        
        m_v3OriginPos = startPoint;
        
        m_v3OriginBulletSpeed = m_v3BulletSpeed;
        
    }
    
    
    
    //如果是火球术，就加一个火粒子
    if (m_pTheskillInfo->theSkillEffectInfo&&m_pTheskillInfo->theSkillEffectInfo->skillEffectId==2) {
        ParticleSystem *particle = ParticleSystemQuad::create("particleSystem/fireBall.plist");
        CFightManager::addCamera(particle);
        particle->setScale(1);
        particle->setPosition3D(Vec3(0, 0, 0));
        addChild(particle,10);
        particle->setGlobalZOrder(1001);
    }

   //相关的技能的差异性初始化
    
    // m_iDamage = m_pTheskillInfo->damageRatio* attacker->getNowAttack();
    
    switch (m_pTheskillInfo->theSkillInfo->skillId)
    {
        case eAllSkills_huoyanpifu :
            
            
            break;
            
        default:
            break;
    }
    
    
}

void CBaseAttack::update(float time){
    
    if (!m_pTheskillInfo||!m_pTheskillInfo->theSkillInfo) {
        unscheduleUpdate();
        onDestroy();
        return;
    }
    
    //取巧的做法。此处暂时这样做。
//    if (m_pAttacker->getReferenceCount()>1000) {
//        unscheduleUpdate();
//        removeFromParentAndCleanup(true);
//        return;
//    }

    m_fNowLifeTime+=time;
    
    if (m_pTheskillInfo->theSkillInfo->lifeTime!=0&&m_pTheskillInfo->theSkillInfo->lifeTime!=0&&m_pTheskillInfo->theSkillInfo->lifeTime!=-1&&m_pTheskillInfo->theSkillInfo->lifeTime!=-2) {//如果有生命周期限制，则判断
        
        
        if (m_fNowLifeTime>m_pTheskillInfo->theSkillInfo->lifeTime) {
            onDestroy();
            return;
        }

    }
    
    
    //超越边界后自毁
    if (getPositionY()<-30||getPositionX()<(Start3DPOINT-ONESIDE*2)||getPositionX()>(END3DPOINT+ONESIDE*2)) {
        onDamage();
        onDestroy();
        CPersonalData::getInstance()->setCatapult(false);
        CPersonalData::getInstance()->setShotted(false);
        NotificationCenter::getInstance()->postNotification("damageGuide");
        return;
    }
    
//    //伤害数达标后自毁
//    if (m_iHurtedNumber>=m_stAttackInfo.hurtNum) {
//        onDestroy();
//        return;
//    }
//
    
    //造成过伤害后就自毁
    if (m_pTheskillInfo->theSkillInfo->lifeTime==-2&&m_iHurtedNumber>0) {
        onDestroy();
        return;
    }
    

    
    
    //如果满足伤害时间点，则造成伤害
    if (m_iNowHurtTimes<m_pTheskillInfo->theSkillInfo->hurtTime.size()&& m_fNowLifeTime>=m_pTheskillInfo->theSkillInfo->hurtTime[m_iNowHurtTimes]
        &&m_pTheskillInfo->theSkillInfo->lifeTime!=-1) {//如果是到达目标点后造成伤害，则跳过
        
        
        m_iNowHurtTimes++;
        onDamage();
        
    }
    
    
    
    //如果当前存活时间已经超过最后一次造成伤害的时间，则自毁
    long maxHurtTime = m_pTheskillInfo->theSkillInfo->hurtTime.size();
    if (maxHurtTime>0&&m_fNowLifeTime>m_pTheskillInfo->theSkillInfo->hurtTime[maxHurtTime-1]&&m_pTheskillInfo->theSkillInfo->lifeTime==-2) {
        
        onDestroy();
        return;
        
    }
    

    
    
    //如果是锁定状态，则子弹永远跟着目标走。
    if(m_pTheskillInfo->theSkillInfo->ifLocked&&m_pTheskillInfo->theSkillInfo->moveSpeed!=0){
        
        if (m_pTarget&&!m_pTarget->getIfDied()) {
            m_v3TargetPos = m_pTarget->getPhysicalPosition();
            
            Vec3 startPoint = getPosition3D();
            
            Vec3 endPoint = m_v3TargetPos;
            
            
            float moveDistance = startPoint.distance(endPoint);
            
            //总位移实际那
            float moveTime = moveDistance/m_pTheskillInfo->theSkillInfo->moveSpeed;
            
            //各个方向上每秒位移
            float speedX = (endPoint.x-startPoint.x)/moveTime;
            float speedY = (endPoint.y-startPoint.y)/moveTime;
            float speedZ = (endPoint.z-startPoint.z)/moveTime;
            
            
            m_v3BulletSpeed = Vec3(speedX, speedY, speedZ);
            
        }else{
            onDestroy();
            return;
        }
        
    }
    
    
//    if (int(m_fAcceleratedSpeedY)==0) {  //如果有y轴加速度，则改变y轴速度
//        m_v3BulletSpeed.y += m_fAcceleratedSpeedY*time;
//    }
    
    
    if(ifDestroyed){
        return;
    }
    
    
    Vec3 startPoint = getPosition3D();
    
    Vec3 endPoint = m_v3TargetPos;
    
    
    float moveDistance = startPoint.distance(endPoint);
    
    
    //到达目标点后自毁
    if (moveDistance<=m_pTheskillInfo->theSkillInfo->moveSpeed*time&&m_pTheskillInfo->theSkillInfo->lifeTime==-1) {//如果剩余距离小于一帧里能移动的距离，则判断到了位置了
        
        if(m_pTheskillInfo->theSkillInfo->hurtType==1){//如果伤害类型是到达目标点后造成伤害
            
            onDamage();
        }
        
        onDestroy();
        return;
    }
    
    
    //如果hurttime为-1，则每帧都判断造成伤害，造成伤害后就自毁
    if (m_pTheskillInfo->theSkillInfo->hurtTime[0]==-1) {
        onDamage();
    }
    
    
    
    rotationUpdate();

    
    
    Vec3 nowPos;
    
    if (int(m_pTheskillInfo->theSkillInfo->moveSpeedY)!=0) {  //如果有y轴加速度，则改变y轴速度
        nowPos.x = getPositionX()+m_v3BulletSpeed.x*time;
        nowPos.z = getPositionZ()+ m_v3BulletSpeed.z*time;
        
        nowPos.y = getPositionY()+ 0.5*m_fAcceleratedSpeedY*time*time + m_v3BulletSpeed.y*time;
        
        m_v3BulletSpeed.y += m_fAcceleratedSpeedY*time;
    }else{
        nowPos = getPosition3D()+m_v3BulletSpeed*time;
    }

    setPosition3D(nowPos);
    

}

void CBaseAttack::rotationUpdate(){
    
    
    if (!m_pBulletSprite) {
        return;
    }
    
    
    Vec3 originPos = getPosition3D();
    
//    if (m_pAttacker) {
//        originPos = m_pAttacker->getPosition3D();
//    }
    
    
    Vec3 tempDirection =  m_v3TargetPos - originPos;
    
    
    CommonData::changeRotation3DWithVec(Vec3(0, 1, 0), tempDirection, this);
    
    
}




void CBaseAttack::animateStart(){
    

    
    if (m_pTheskillInfo->theSkillEffectInfo) {//如果技能特效存在，则初始化
        
        
        if (m_pTheskillInfo->theSkillEffectInfo->effectType==0) {//模型初始化
            m_pBulletSprite = Sprite3D::create(m_pTheskillInfo->theSkillEffectInfo->skillPath);
            
            
            addChild(m_pBulletSprite,1000);
            
//            m_pBulletSprite->runAction(RepeatForever::create(RotateBy::create(0.5f, Vec3(0, 360, 0))));
            
            CFightManager::addCamera(m_pBulletSprite);
            
            CFightManager::addCamera(this);
            
            
            m_pBulletSprite->setScale(m_pTheskillInfo->theSkillEffectInfo->effectScale);
            
           // m_pBulletSprite->setScale(3);

        m_pBulletSprite->setPosition3D(m_pTheskillInfo->theSkillEffectInfo->offset);
            
//            m_pBulletSprite->setColor(Color3B::RED);
            
            
        }else if (m_pTheskillInfo->theSkillEffectInfo->effectType==1){//帧动画初始化
            
            
//            m_pBulletAnimate = BillBoard::create();
            
            
            Animation * hurtAnimation = Animation::create();
            
            for (int i=1; i<=m_pTheskillInfo->theSkillEffectInfo->skillFrameNumber; i++)
            {
                
                __String *temp = __String::createWithFormat("%s%d.png",m_pTheskillInfo->theSkillEffectInfo->skillPath.c_str(),i);

                
                hurtAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(temp->getCString()));
            }
            hurtAnimation->setDelayPerUnit(0.1f);


            //效果
            
            if(!m_pTheskillInfo->theSkillEffectInfo->skillPath.empty())
            {
                auto hurtAction = Animate::create(hurtAnimation);
                
                 __String *tempStr = __String::createWithFormat("%s1.png",m_pTheskillInfo->theSkillEffectInfo->skillPath.c_str());
                m_pBulletAnimate = Sprite::createWithSpriteFrameName(tempStr->getCString());
                
    //            m_pBulletAnimate->setMode(BillBoard::Mode::VIEW_PLANE_ORIENTED);
                
    //            m_pBulletAnimate->setPosition3D(m_v3TargetPos);
                m_pBulletAnimate->setCameraMask((unsigned short )CameraFlag::USER1);
                
                
                if(m_pTheskillInfo->theSkillInfo->skillId == eAllSkills_Feilongdu)
                {
                    m_pBulletAnimate->runAction(RepeatForever::create(Sequence::create(hurtAction,RemoveSelf::create(), NULL)));
                }
                else
                {
                    m_pBulletAnimate->runAction(Sequence::create(hurtAction,RemoveSelf::create(), NULL));
                }
                    
//                Sprite3D* temp = Sprite3D::create();
//                temp->addChild(m_pBulletAnimate);
//                CFightManager::addCamera(temp);
//                CFightManager::addCamera(this);
//                addChild(temp,100);
                
                CFightManager::addCamera(m_pBulletAnimate);
                addChild(m_pBulletAnimate,100);
                
            
                m_pBulletAnimate->setScale(m_pTheskillInfo->theSkillEffectInfo->effectScale);
                
//                m_pBulletAnimate->setScaleX(m_pTheskillInfo->theSkillEffectInfo->effectScale);
//                m_pBulletAnimate->setScaleY(m_pTheskillInfo->theSkillEffectInfo->effectScale);
           
//            m_pBulletAnimate->setScaleX(3.5);
//            m_pBulletAnimate->setScaleY(3.5);
//            float tempScale = theTarget->gettheSprite()->getBoundingBox().size.height/tempSprite->getBoundingBox().size.height*0.5f;
//            tempSprite->setScale(tempScale);
            
                
                m_pBulletAnimate->setPosition3D(m_pTheskillInfo->theSkillEffectInfo->offset);
                               
             }
            
        }
        
    }
    
    
    //测试用
//    m_pBulletSprite = Sprite3D::create("model/piglet/piglet.c3b");
////    m_pBulletSprite->setScale(0.2f);
//
//
//    m_pBulletSprite->runAction(Sequence::create(RotateBy::create(0.5f, Vec3(0, 10, 0)), NULL));
//    
//    CFightManager::addCamera(m_pBulletSprite);
//    
//    CFightManager::addCamera(this);
    
    
    

}

void CBaseAttack::onEnterTransitionDidFinish(){
    
    Node::onEnterTransitionDidFinish();
    
    scheduleUpdate();
    
}



void CBaseAttack::onDestroy(bool ifRemoveSelf){
    
    if(ifDestroyed){
        return;
    }
    
    ifDestroyed = true;

    
    
    if(ifRemoveSelf){ //如果是直接删除，则不做多余操作了
        removeFromParentAndCleanup(true);
        return;
    }
    
    
    
    
    //如果是陷阱类，则技能摧毁时，原陷阱也摧毁
    switch (m_pTheskillInfo->theSkillInfo->skillId) {
            
            //如果是陷阱相关技能，则造成伤害后就自杀
        case eAllSkills_Trap_Range:
        case eAllSkills_Trap_Single:
        case eAllskills_Zibao:
        case eAllSkills_Trap_Dinshi:
            
            if (m_pAttacker) {
//                m_pAttacker->getAllAttackes()->eraseObject(this);
                m_pAttacker->onDied();
                m_pAttacker=NULL;
            }
            break;
            
        default:
            break;
    }
    
    if (m_pAttacker) {
        m_pAttacker->getAllAttackes()->eraseObject(this);
        m_pAttacker = NULL;
    }
    
    
    //爆炸特效
    if (m_pTheskillInfo->theSkillEffectInfo&&std::strcmp(m_pTheskillInfo->theSkillEffectInfo->bombAnimate.animatePath.c_str(), "")!=0) {
        
        
        __String *temp = __String::createWithFormat("%s1.png",m_pTheskillInfo->theSkillEffectInfo->bombAnimate.animatePath.c_str());
        
        Sprite* tempSprite = Sprite::createWithSpriteFrameName(temp->getCString());
        
        Animate* targetAnimation = CFightManager::getAnimateWithPath(m_pTheskillInfo->theSkillEffectInfo->bombAnimate.animatePath.c_str(), 1, m_pTheskillInfo->theSkillEffectInfo->bombAnimate.animateNumber);
        
        tempSprite->runAction(Sequence::create(targetAnimation,RemoveSelf::create(), NULL));
        
        tempSprite->setScale(2);
        
        CFightManager::addCamera(tempSprite);
        
        CFightData::getInstance()->getTheFightScene()->addChild(tempSprite);
        
        tempSprite->setPosition3D(getPosition3D());
    }
    
    
    if(m_pTheskillInfo->theSkillInfo->skillId==102&&m_iHurtedNumber==0){
        
        CCLOG("bad Shoot or Error");
        
    }
    
    
    
    
    
    unscheduleUpdate();
    
    
    removeFromParentAndCleanup(true);

}

void CBaseAttack::onDamage(){
    
//    Vector<CBaseFighter*>targetVec;
//    
//    //如果是伤害类型，则获取敌人，如果是治疗类型，则获取友军
//    if (m_pTheskillInfo->theSkillInfo->skillType==0) {
//        targetVec= CFightData::getInstance()->getFightTargetByGroup(m_iAttackerGroup);
//    }else if (m_pTheskillInfo->theSkillInfo->skillType==1){
//        targetVec= CFightData::getInstance()->getFriendMonstersByGroup(m_iAttackerGroup);
//    }
//    
////    targetVec= CFightData::getInstance()->getFightTargetByGroup(m_iAttackerGroup);
//
    
    
    //log("hurtnumbers = %d\n", m_pTheskillInfo->theSkillInfo->skillId);
  
    if(m_pTheskillInfo->theSkillInfo->skillId == eAllSkills_huoqiu)
    {
        log("huoqiushu\n");
        
    }
    
    //如果是治疗技能，则技能目标先接受治疗，
    if (m_pTheskillInfo->theSkillInfo->skillType==1&&m_pTarget&&!m_pTarget->getIfDied())
    {
        //萨满被动治疗
        //float fDamage =  m_pTheskillInfo->damageRatio;
        
        if(m_pTheskillInfo->theSkillInfo->skillId == eAllSkills_huifushu)
        {
            float fWushuNum  =   m_pTarget->m_fWushuXishu;
        
            m_iDamage *= (1+fWushuNum);
        }

        
        else if(m_pTheskillInfo->theSkillInfo->skillId == eAllSkills_yexinggrow)
        {
            float fXishu  =   m_pTarget->m_fYexinXishu;
            m_iDamage *= (1+fXishu);
            
        }
        else if(m_pTheskillInfo->theSkillInfo->skillId == eAllSkills_huichunshu)
        {
           
             float fXishu  =   m_pTarget->m_fZiranXishu;
            m_iDamage *= (1+fXishu);
        }
        m_pTarget->becured(m_iDamage);
        m_iHurtedNumber++;
    }
    
    
    Vector<CBaseFighter*>targetVec;
    
    
    //根据释放目标来更新面向
    if (m_pTheskillInfo->theSkillInfo->effectGroup==-1)
    {//如果目标点为自己。则选择自己
        if (m_pAttacker) {
            targetVec.pushBack(m_pAttacker);
        }else{
            CCLOG("LYASSERT:技能无攻击者，且选择从目标为攻击者");
        }
        
    }else if (m_pTheskillInfo->theSkillInfo->hurtNumbers==1&&m_pTheskillInfo->theSkillInfo->ifLocked&&m_pTarget){   //如果伤害数量只有一个人，并且是锁定的，就只对该目标造成伤害
        
        targetVec.pushBack(m_pTarget);
        
    }else
    {
//        if (m_pTarget) {
//            targetVec.pushBack(m_pTarget);
//        }
//        
//        //防止二次选中
//        Vector<CBaseFighter*>tempVec =  CFightData::getInstance()->getAttackerByGroupCondition(m_eAttackerGroup, m_pTheskillInfo->theSkillInfo->effectGroup);
//        
//        if (m_pTarget) {
//            tempVec.eraseObject(m_pTarget);
//        }
//        
//        targetVec.pushBack(tempVec);
        
        targetVec= CFightData::getInstance()->getAttackerByGroupCondition(m_eAttackerGroup, m_pTheskillInfo->theSkillInfo->effectGroup);
    }
    
    
    for (int i=0; i<targetVec.size(); i++) {
        
        if (m_iHurtedNumber>=m_pTheskillInfo->theSkillInfo->hurtNumbers) {
            break;
        }
        
        CBaseFighter* theTarget = targetVec.at(i);
        
        
        if (theTarget->getIfDied()) {//如果敌人死亡了，则跳过
            continue;
        }
        
        float selfRadius = 0;
        
        if (m_pAttacker) {
            selfRadius = m_pAttacker->getNowRadius();
        }
        
        //伤害距离内的造成伤害
        float distance =getPosition3D().distance(theTarget->getPhysicalPosition()) - theTarget->getNowRadius()-selfRadius;
        
        
        
        if (distance<m_pTheskillInfo->theSkillInfo->damageRange) {
            
            
            onEffect(theTarget);//一定要在被攻击之前调用，因为怪物死亡时会删掉所有debuff，如果在攻击之后调用，就赶不上这次删除了
            
            if (m_pTheskillInfo->theSkillInfo->skillType==0){//类型为伤害型技能，则造成伤害
                if(m_pTheskillInfo->theSkillInfo->skillId == eAllSkills_bingdong)
                {
                    SimpleAudioEngine::getInstance()->playEffect("music/frozen.mp3");
                }
                
                else if(m_pTheskillInfo->theSkillInfo->skillEffectId == eAllSkills_huoqiu)
                {
                    log("huoqiushu\n");
                }
            
                //火焰皮肤对周围敌人造成持续的%.2f%%生命上限的伤害
                else if(m_pTheskillInfo->theSkillInfo->skillId == eAllSkills_huoyanpifu)
                {
                    m_iDamage =   theTarget->getTheInfo()->maxHP*m_pTheskillInfo->damageRatio;
                    
                     if(theTarget->getTheInfo()->monsterType>eFighterType_enemy)
                     {
                         return ;
                         
                     }
                    
                    
                }
                else if(m_pTheskillInfo->theSkillInfo->skillId == eAllSkills_renShitou) //火巨人扔石头
                {
                    m_iDamage  = m_pAttacker->getNowAttack()*m_pTheskillInfo->damageRatio;
                    
                }
                else if(m_pTheskillInfo->theSkillInfo->skillId ==  eAlSkills_wanzheyiyan) //诅咒
                {
                    m_iDamage =  m_pAttacker->getNowAttack()*m_pTheskillInfo->damageRatio;
                    
                }
                else if(m_pTheskillInfo->theSkillInfo->skillId == eAllSkills_shuanrenjian) //双刃剑
                {
                    int iNowHp = m_pAttacker->getNowHP();
                    float fDamage = m_pTheskillInfo->damageRatio;
                    m_iDamage = fDamage*iNowHp+m_pAttacker->getNowAttack();
                    m_pAttacker->beAttacked(NULL,m_iDamage*0.5);
                }
                
                else if(m_pTheskillInfo->theSkillInfo->skillId == eAllSkills_Xuanfengzhan) //旋风斩
                {
                    m_iDamage = m_pAttacker->getNowAttack()*m_pTheskillInfo->damageRatio;
                    
                }
                else if(m_pTheskillInfo->theSkillInfo->skillId == eAllskills_Zibao)
                {
                       //theTarget
                    
                     m_iDamage = m_pAttacker->getNowAttack()*m_pTheskillInfo->damageRatio;
                    
                    
                   // break;
                    
                    
                    
                }
                
                //塔技能伤害不显示出来
                if (m_pTheskillInfo->theSkillInfo->skillId==100) {
                    theTarget->beAttacked(m_pAttacker,m_iDamage,1,m_iAttackerGroup, eDamageType_GeneralAttack);
                }else if(m_pTheskillInfo->theSkillInfo->skillId==102){//投石攻击时
                    
                    //如果目标是飞行单位则跳过
                    if (theTarget->getTheInfo()->theMonsterInfo->ifFly) {
                        continue;
                    }
                    
                    theTarget->beAttacked(m_pAttacker,m_iDamage,1,m_iAttackerGroup, eDamageType_StoneSkill);
                }else if(m_pTheskillInfo->theSkillInfo->skillId==37){//炸弹人自爆
                    
                    //如果目标是飞行单位则跳过
                    if (theTarget->getTheInfo()->theMonsterInfo->ifFly) {
                        continue;
                    }
                    
                    theTarget->beAttacked(m_pAttacker,m_iDamage,1,m_iAttackerGroup, eDamageType_SoliderSkill);
                }else{
                    theTarget->beAttacked(m_pAttacker,m_iDamage,1,m_iAttackerGroup, eDamageType_SoliderSkill);
                }
                
                //受伤效果
                theTarget->animateChange(eFighteState_Hurt);
                
                
            }else if (m_pTheskillInfo->theSkillInfo->skillType==1)//类型为治疗型技能，则治疗
            {
           
                theTarget->becured(m_iDamage);
            }

            m_iHurtedNumber++;
            
            
            //技能命中特效
            if (m_pTheskillInfo&&m_pTheskillInfo->theSkillEffectInfo&&m_pTheskillInfo->theSkillEffectInfo->hurtAnimate.animatePath!="") {
                
                
                __String *temp = __String::createWithFormat("%s1.png",m_pTheskillInfo->theSkillEffectInfo->hurtAnimate.animatePath.c_str());
                
                Sprite* tempSprite = Sprite::createWithSpriteFrameName(temp->getCString());
                
                
                float tempScale = theTarget->gettheSprite()->getBoundingBox().size.height/tempSprite->getBoundingBox().size.height*0.5f;
                tempSprite->setScale(tempScale);
                
                Animate* targetAnimation = CFightManager::getAnimateWithPath(m_pTheskillInfo->theSkillEffectInfo->hurtAnimate.animatePath.c_str(), 1, m_pTheskillInfo->theSkillEffectInfo->hurtAnimate.animateNumber);
                
                tempSprite->runAction(Sequence::create(targetAnimation,RemoveSelf::create(), NULL));
                
                CFightManager::addCamera(tempSprite);
                
                theTarget->addChild(tempSprite);
                
                if(m_pTheskillInfo->theSkillInfo->skillId == eAllSkills_huoyanpifu)
                {
                    float fScale =  theTarget->gettheSprite()->getBoundingBox().size.height*1;
                    tempSprite->setScale(0.5);
                    tempSprite->setPosition3D(Vec3(0, theTarget->getPhysicalPosition().y+fScale, 0));
                }
            }
            
            
            
        }
  
    }
    if ((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_USE_MANGONEL) && (guiderManagerLayer::getInstance()->getStepNum() == 5))
    {
        if(m_iHurtedNumber > 0)
        {
            CPersonalData::getInstance()->setShotted(true);
            NotificationCenter::getInstance()->postNotification("damageGuide");
        }
        else{
            CPersonalData::getInstance()->setCatapult(false);
            CPersonalData::getInstance()->setShotted(false);
            guiderManagerLayer::getInstance()->setStepNum(guiderManagerLayer::getInstance()->getStepNum() + 1);
            NotificationCenter::getInstance()->postNotification("damageGuide");
        }
    }
    
    
    
    switch (m_pTheskillInfo->theSkillInfo->skillId) {
            
            //如果是陷阱相关技能，则造成伤害后就自杀
            case eAllSkills_Trap_Range:
            case eAllSkills_Trap_Single:
            case eAllskills_Zibao:
            case eAllSkills_Trap_Dinshi:
            
            if (m_pAttacker) {
                m_pAttacker->setVisible(false);
            }
            break;
//                m_pAttacker->onDied();
            
//           case eAllskills_Zibao:
//            {
//                //theTarget
//                
//                
//                m_pAttacker->setSoul(false);
//                
//                m_pAttacker->setNowHP(0);
//                m_pAttacker->setMaxHP(0);
//                m_pAttacker->onDieing();
//                m_pAttacker->gettheSprite()->setVisible(false);
//                m_pAttacker->onDied();
//                
//            }

            default:
                break;
            
    }
    
    
    
    
    
    
    
}


void CBaseAttack::onEffect(CBaseFighter *target){
    
    
    float effectTime =3;
    
    
    //攻击特殊效果
    switch (m_pTheskillInfo->theSkillInfo->skillId) {
        case eAllSkills_jurenhuiji:
            
            effectTime = 1;
            target->beatBackFrom(getPosition3D(), 130, effectTime);
            break;
            
        case eAllSkills_jianta:
        {
            //如果此时敌人的血量已经低于一定值，则伤害值变为剩余生命值“即斩杀”
            
            float hpPercent = target->getNowHP()*1.0/target->getMaxHP();
            
            if (hpPercent<=0.2f) { //生命值小于20%时，斩杀
                target->beAttacked(NULL,target->getNowHP()*10,1,m_iAttackerGroup);//伤害为剩余血量的10倍，保险起见。一定死掉
            }
            break;
        }
            
            
        case eAllSkills_huoyanpifu:
            log("allSkills_huojuren\n");
            
            break;
            
        case eAllSkills_toushi:
        {
            
            //让飞行单位和建筑单位免疫此效果
            if (target->getTheInfo()->theMonsterInfo->ifFly||target->getTheInfo()->walkSpeed==0) {
                break;
            }
            
            float beatBackDist = 200;
            float beatFlyDist = 130;
            float beatTime = 1;
            
            target->beatBackFrom(getPosition3D(), beatBackDist, beatTime,beatFlyDist);
            CFightManager::addTheBuffToFighter(target, (AllBuffes)eAllBuffes_jitui,beatTime);
            CFightManager::addTheBuffToFighter(target, (AllBuffes)eAllBuffes_xuanyun,beatTime*1.5f);
        }

            
        default:
            break;
    }
    
    
    
    //附加buff
    
    if (m_pTheskillInfo->theSkillInfo->buffId>0) {
        
        if(m_pTheskillInfo->theSkillInfo->skillId==eAllSkills_bingdong){//冰冻术和巨石皮肤的持续时间根据技能等级计算。
            
            //冰冻术初始1秒，之后每级+0.1秒。
            effectTime = (m_pTheskillInfo->skillLevel-1)*0.1+1;
            CFightManager::addTheBuffToFighter(target, (AllBuffes)m_pTheskillInfo->theSkillInfo->buffId,effectTime);
        }/*else if (m_pTheskillInfo->theSkillInfo->skillId==eAllSkills_jushipifu){
            //巨石皮肤初始0.05秒，之后每级+0.05秒。
            
            effectTime = m_pTheskillInfo->skillLevel*0.05;
            CFightManager::addTheBuffToFighter(target, (AllBuffes)m_pTheskillInfo->theSkillInfo->buffId,effectTime);

        }*/
        else if(m_pTheskillInfo->theSkillInfo->skillId== eAllSkills_Feilongdu)
        {
            effectTime = 3;
            
            //毒的效果只触发一次
            
        
           cocos2d::Vector<CBaseBuff*>  p =   *target->getAllBuffes();
            
            for(int i=0;i<p.size();i++)
            {
                if(p.at(i)->getBuffType()== eAllBuffes_du)
                {
                    p.erase(i);
                    break;
                }
            }
        
            //target->setOpacity();
        
            if(target->getAttackType()==eAttackType_monster || target->getAttackType()==eAttackType_hero)
            {
                float fDamge = m_pTheskillInfo->damageRatio* m_pAttacker->getNowAttack();
            
                CFightManager::addTheBuffToFighter(target, (AllBuffes)m_pTheskillInfo->theSkillInfo->buffId,effectTime,fDamge);
            }
        }
        else
        {
            CFightManager::addTheBuffToFighter(target, (AllBuffes)m_pTheskillInfo->theSkillInfo->buffId,effectTime);
        }
    }
    
}


void CBaseAttack::onExit(){
    
    
//    onDestroy(false);
    
    Node::onExit();
    
    
}



















