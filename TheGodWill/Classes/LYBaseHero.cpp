//
//  LYBaseHero.cpp
//  TheGodWill
//
//  Created by user on 14/12/11.
//
//

#include "LYBaseHero.h"

#include "fightScene.h"

#include "LYFightData.h"

CBaseHero::CBaseHero(){
    
    
    
    
    
    m_bIfCameraFollow = false;
}

CBaseHero::~CBaseHero(){
    
}




CBaseHero *CBaseHero::create(FighterInfo *theInfo,FightGroup theGroup){
    
    CBaseHero * temp = new CBaseHero();
    
    temp->setAttackType(eAttackType_hero);
    
//    temp->m_iAttackType = eAttackType_hero;
    
    if (temp && temp->initWithInfo(theInfo,theGroup)) {
        temp->autorelease();
        return temp;
    }else{
        
        delete temp;
        temp = NULL;
        return NULL;
    }
    
}


int lookatOffsetY=0;
int lookatOffsetZ=0;

void CBaseHero::LaganMove(int x,int y,float fAngle)
{
    
//    Vec3 position = ((CFightScene*)(getParent()->getParent()))->_camera->getPosition3D();
//    
////    Vec3 lookPosition = ((CFightScene*)(getParent()->getParent()))->_camera->lookAt(<#const cocos2d::Vec3 &target#>, <#const cocos2d::Vec3 &up#>)();
//    
//    
//    //测试用，查看当前地图角度;
//    if (fabs(x)>fabs(y)) {
//        if (x>0) {
//            
//            ((CFightScene*)(getParent()->getParent()))->_camera->setPosition3D(position+Vec3(0,1,0));
//            
//        }else{
//            
//            ((CFightScene*)(getParent()->getParent()))->_camera->setPosition3D(position+Vec3(0,-1,0));
//        }
//    }else{
//        
//        if (y>0) {
//            
//            ((CFightScene*)(getParent()->getParent()))->_camera->setPosition3D(position+Vec3(0,0,1));
//            
//        }else{
//            
//            ((CFightScene*)(getParent()->getParent()))->_camera->setPosition3D(position+Vec3(0,0,-1));
//        }
//        
//        
//        
//    }
//    
//    CCLOG("x%f,y%f,z%f",position.x,position.y,position.z);
//    
//    return;
    
//    ((CFightScene*)(getParent()->getParent()))->_camera  ;
    
    
    
    
    
    
    
    
    
    
    
    
    
    
   // getPositionX()+x*1000,getPositionY()+y*1000;
    
    m_v2MoveControll = Vec2(x,-y);
    
//    CCLOG("laganVec %d,%d",x,y);
    
    m_bIfCameraFollow = true;
    
    
    
}



void CBaseHero::LaganStop()
{
    m_v2MoveControll = Vec2(0,0);
    
//    CCLOG("laganVec stop");
    
    m_bIfCameraFollow = false;
}

bool CBaseHero::initWithInfo(FighterInfo *theInfo,FightGroup theGroup){
    
    //测试用，有模型了就删掉
//    theInfo->theModelInfo = CDataManager::getModelInfoByModelId(1011);
    
    
    
    if (!CBaseFighter::initWithInfo(theInfo,theGroup)) {
        return false;
    };
    
    
    //测试用，血量提升
//    setNowHP(100000000);
    
    Vec3 rotate =  m_pTheSrite->getRotation3D();
//    m_pTheSrite->setRotation3D(Vec3(0, 0, 0));
//    m_pTheSrite->setScale(40);
    
//    m_pTheSrite->setRotation3D(Vec3(-90,0,0));
    
//    m_pTheSrite->setRotation3D(Vec3(0,0,0));
    
    
    m_pTheHpBar->setPosition(0, m_pTheSrite->getBoundingBox().size.height);
    
    stateChange(eFighteState_Idle);
    animateChange(eFighteState_Idle);
    
    
    //脚底光圈
    
    Sprite3D* theCircle = Sprite3D::create();
    
    Sprite* tempSprite = Sprite::createWithSpriteFrameName("fight_CityCircle2.png");
    
    tempSprite->setScale(m_pTheSrite->getBoundingBox().size.width/tempSprite->getBoundingBox().size.width*2.5);
    theCircle->addChild(tempSprite);
    
    tempSprite->runAction(RepeatForever::create(RotateBy::create(0.5f, 10)));
    
    theCircle->setRotation3D(Vec3(90, 0, 0));
    CFightManager::addCamera(theCircle);
    
    theCircle->setPosition3D(Vec3(0, 1, 0));
    addChild(theCircle);
    
    
    //测试用
    
//    m_pTheSrite->removeFromParentAndCleanup(true);
//    m_pTheSrite = Sprite3D::create("models/newBuilding/kuang.c3b");
////    m_pTheSrite = Sprite3D::create("models/newBuilding/3.c3b","models/newBuilding/tower_base2_2.png");
//    addChild(m_pTheSrite,1);
//    CFightManager::addCamera(m_pTheSrite);
//    m_pTheSrite->setScale(50);
    
    
    return true;
    
}

void CBaseHero::collidUpdate(float time){
    
    //重写此方法，什么都不做。因为资源点不会位移
    
    
}

void CBaseHero::stateUpdate(float time){
    
    
    return;
    
    //重写此方法，因为英雄不会自己攻击，不会自己移动。
    
    
    
    if (m_iFightState==eFighteState_Idle&&m_v2MoveControll!=Vec2(0,0)) {
        stateChange(eFighteState_Walk);
    }else if (m_iFightState==eFighteState_Walk&&m_v2MoveControll==Vec2(0,0)) {
        stateChange(eFighteState_Idle);
    }
    
    
    //但是会实时侦测拉杆，并且产生位移。
//    positionUpdate(time);
    roatationUpdate(time);
    
}

void CBaseHero::onDied(DamageType type){
    
    
    if (CFightData::getInstance()->getWinState()==1) { //如果已经战斗胜利了，则英雄就不死了，防止报错
        return;
    }
    
    CBaseFighter::onDied(type);
    //英雄死了就输了
    CFightData::getInstance()->setWinState(-1,this);
    
    Sprite* temp=Sprite::createWithSpriteFrameName("tower_die_1.png");
    CFightManager::addCamera(temp);
    temp->setGlobalZOrder(1005);
    addChild(temp);
    temp->setScale(1.5f);
    char buf[256];
    Animation* pAnimation = Animation::create();
    for (int i = 1; i < 11; i++)
    {
        sprintf(buf, "tower_die_%d.png", i);
        pAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(buf));
    }
    pAnimation->setDelayPerUnit(0.1f);
    temp->runAction(Sequence::create(Animate::create(pAnimation),RemoveSelf::create(), NULL) );

//    int randFlag = rand()%3+1;
//    __String *tempStr = __String::createWithFormat("ruins%d.png",randFlag);
//    
//    Sprite* ruinSprite = Sprite::createWithSpriteFrameName(tempStr->getCString());
//    Sprite3D* pRuinsSprite = Sprite3D::create();
//    pRuinsSprite->addChild(ruinSprite);
//    //    m_pRuinsSprite = CFightManager::getTheModelWithPath("ruins.png");
//    
//    pRuinsSprite->setPosition3D(getPosition3D());
//    pRuinsSprite->setRotation3D(Vec3(90, 0, 0));
//    CFightManager::addCamera(pRuinsSprite);
//    
//    
//    pRuinsSprite->setRotation3D(Vec3(90, 0, 0));
//    
//    pRuinsSprite->setScale(m_pTheSrite->getBoundingBox().size.width/ruinSprite->getBoundingBox().size.width*1.2f);
//    
//    if (getParent()) {
//        getParent()->addChild(pRuinsSprite,100);
//    }
    
    
    //随机给个废墟特效
//    int randParticle = rand()%1+1;
//    __String *tempStr2 = __String::createWithFormat("particleSystem/ruinsSmoke%d.plist",randParticle);
//    
//    ParticleSystem* pRuinsParticle = ParticleSystemQuad::create(tempStr2->getCString());
//    CFightManager::addCamera(pRuinsParticle);
//    pRuinsParticle->setPosition3D(getPosition3D());
//    pRuinsParticle->setScale(m_pTheSrite->getBoundingBox().size.width/ruinSprite->getBoundingBox().size.width*1);
//    if (getParent()) {
//        getParent()->addChild(pRuinsParticle,100);
//    }
//    pRuinsParticle->setGlobalZOrder(1005);
//    

  
}




static int animateTest=0;
void CBaseHero::testmode(){

    
//    if (animateTest==eFighteState_None) {
//        animateTest=eFighteState_Idle;
//    }
//
//    animateChange(eFighteState_Dizzy);
//    m_pTheSrite->runAction(FadeOut::create(m_pAction_Die->getDuration()));
//    return;
    
    
    
    //试试停掉 sequence
//    animateChange(eFighteState_SpecialAttack);
//    return;
    
    
    
//    Sequence *theSequence = Sequence::create(DelayTime::create(m_pAction_Attack->getDuration()),CallFunc::create([=](){
//        
////        if (m_iFightState==eFighteState_Attack&&!m_bIfDied) {//如果动画结束了，依然是攻击状态，并且没死，则进入空闲动画
//            animateChange(eFighteState_Idle);
////        }
//        
//        
//    }) , NULL);
//    
//    m_iAnimateState=eFighteState_Attack;
//    playAniamteAction(m_pAction_Attack);
//    m_pTheSrite->runAction(theSequence);
//    return;
    
    
    
    
//    animateChange(eFighteState_Dizzy);
    
    
    
    animateTest++;
    
    //测试用
//    animateTest = eFighteState_Dizzy;
    
    
    if (animateTest==eFighteState_None) {
        animateTest=eFighteState_Idle;
    }

    
    
    FighteState temp = (FighteState)animateTest;
    
//    m_pAttackTarget = NULL;
    setAttackTarget(NULL);
    
    
    animateChange(temp);
    
    
    CCLOG("%d",temp);
    
}

void CBaseHero::beAttacked(CBaseFighter* target,int attk,float damagePercent,FightGroup attackerGroup ,DamageType type){
    
    CBaseFighter::beAttacked(target, attk, damagePercent, attackerGroup, type);
    
    
    
    //建筑变色
    auto theAction = Sequence::create(TintTo::create(0.35f, 255, 0, 0),TintTo::create(0.35f, 255, 255, 255), NULL);
    theAction->setTag(999);
    
    m_pTheSrite->stopActionByTag(999);
    m_pTheSrite->runAction(theAction);
    
    CFightManager::RunTumblerAction(m_pTheSrite, 20, 0.15, 1,true,0,Vec3(0, 90, 0));
    
}

void CBaseHero::afterDied(){
    
    
    CFightData::getInstance()->setTheFriendHero(NULL);
    CBaseFighter::afterDied();
    
}



