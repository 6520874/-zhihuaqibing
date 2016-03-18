//
//  LYBaseResourceMine.cpp
//  TheGodWill
//
//  Created by user on 14/12/8.
//
//

#include "LYBaseResourceMine.h"

#include "LYFightData.h"

USING_NS_CC;


CBaseResourceMine::CBaseResourceMine(){
    
    m_vecBaseRotation = Vec3(0, 0, 0);
    
}

CBaseResourceMine::~CBaseResourceMine(){
    
    
}


CBaseResourceMine *CBaseResourceMine::create(FighterInfo *theInfo,FightGroup theGroup){
    
    CBaseResourceMine * temp = new CBaseResourceMine();
    
    temp->m_iAttackType = eAttackType_min;
    
    //修改双方的能量增长值
    CFightData::getInstance()->addEnergySpeedWith(ADD_ENERGY_PER_SECOND, theGroup);
    
    if (temp && temp->initWithInfo(theInfo,theGroup)) {
        temp->autorelease();
        return temp;
    }else{
        
        delete temp;
        temp = NULL;
        return NULL;
    }
    
}

bool CBaseResourceMine::initWithInfo(FighterInfo *theInfo,FightGroup theGroup){
    
    //测试用，有模型了就删掉
//    theInfo.theModelInfo->modelName= "model/piglet/piglet.c3b";
//    theInfo.theModelInfo->radius = 50;
    
    if (!CBaseBuilding::initWithInfo(theInfo,theGroup)) {
        return false;
    };
    
    
    
    if (theGroup==eFightGroup_midFight||theGroup==eFightGroup_midNotFight) {
        m_bIfReborn = true;//中立塔死后会复活，变为摧毁者一方
    }
    
    
    refreshTheOwner(m_iFightGroup);
    
    
//    m_fSpriteScale = 0.4;
//    m_pTheSrite->setScale(m_fSpriteScale);
    
    m_pTheHpBar->setPosition(0, m_pTheSrite->getBoundingBox().size.height);
//    m_pTheSrite->setScale();
    
    m_pTheSrite->setRotation3D(m_vecBaseRotation);
    
    
    //测试用
//    m_strTheInfo->theModelInfo->radius=30;
    
    
    return true;
    
}

void CBaseResourceMine::collidUpdate(float time){
    
    //重写此方法，什么都不做。因为资源点不会位移
    
    
}

void CBaseResourceMine::stateUpdate(float time){
    
    
    CBaseBuilding::stateUpdate(time);
    
    
    //重写此方法，因为资源点只判断最后一个经过他的人的属性。
    
    
    
    //判断所有与资源点相近的人，如果双方都有，则维持不变化，如果只有一方，则为相近的这一方
    
//    cocos2d::Vector<CBaseFighter*>* friendVector = CFightData::getInstance()->getAllSoldier();
//    
//    
//    bool ifFriendIn=false;
//    bool ifEnemyIn=false;
//    
//    
//    
//    for (int i=0; i<friendVector->size(); i++) {
//        
//        CBaseFighter* tempOne = friendVector->at(i);
//        
//        if (getFightPosition().distance(tempOne->getFightPosition())<m_strTheInfo->theModelInfo->radius) {
//            
//            ifFriendIn = true;
//            break;
//        }
//
//    }
//
//    
//    cocos2d::Vector<CBaseFighter*>* enemyVector = CFightData::getInstance()->getAllEnemy();
//    
//    for (int i=0; i<enemyVector->size(); i++) {
//        
//        CBaseFighter* tempOne = enemyVector->at(i);
//        
//        if (getFightPosition().distance(tempOne->getFightPosition())<m_strTheInfo->theModelInfo->radius) {
//            
//            ifEnemyIn = true;
//            break;
//        }
//        
//    }
//    
//    
//    if (ifFriendIn&&!ifEnemyIn) {//如果只有友军，没有敌人。
//        
//        if (m_iFightGroup!=eFightGroup_Freind) { //如果原先不是友军，则变为友军
//            onChangeOwner(eFightGroup_Freind);
//        }
//        
//    }
//    
//    if (ifEnemyIn&&!ifFriendIn) {//如果只有友军，没有敌人。
//        
//        if (m_iFightGroup!=eFightGroup_Enemy) { //如果原先不是友军，则变为友军
//            onChangeOwner(eFightGroup_Enemy);
//        }
//        
//    }
    
    
}


void CBaseResourceMine::onChangeOwner(FightGroup target){
    
    
//    m_pTheSrite->removeFromParentAndCleanup(true);
//    
//    __String modelPath = m_strTheInfo->theModelInfo->modelName;
//
//
//    
//    
//    
//    
//    //更换模型
//    if (target==eFightGroup_Freind) {
////        m_pTheSrite->setColor(Color3B::BLUE);
//        modelPath.append("_blue");
//        
//    }else if (target==eFightGroup_Enemy) {
////        m_pTheSrite->setColor(Color3B::RED);
//        modelPath.append("_red");
//        
//    }
//    
//    initModelWithPath(modelPath.getCString());
    
    
//    //修改双方的能量增长值
//    CFightData::getInstance()->addEnergySpeedWith(ADD_ENERGY_PER_SECOND, target);
//    CFightData::getInstance()->addEnergySpeedWith(-ADD_ENERGY_PER_SECOND, m_iFightGroup);

    
    
//    refreshTheOwner(target);
    
    
//    //修改拥有者
//    m_iFightGroup = target;
    
}

void CBaseResourceMine::reborn(){
    
    //更换模型
    
    //    m_pTheSrite->removeFromParentAndCleanup(true);
    //
    //
    //    if (m_pDeadlyTarget) {
    //        m_iFightGroup = m_pDeadlyTarget->getFightGroup();
    //
    //    }else{
    //
    //        m_iFightGroup = m_iDeadlyGroup;
    //    }
    //
    //
    //    __String modelPath = m_strTheInfo->theModelInfo->modelName;
    //
    //    std::string temp = modelPath.getCString();
    //
    //    long offset = temp.find("white");
    //
    //    if (m_iFightGroup == eFightGroup_Freind) {
    //
    //        temp.replace(offset,5,"blue");
    ////        modelPath.append("_blue");
    //    }else if (m_iFightGroup == eFightGroup_Enemy){
    //        temp.replace(offset,5,"red");
    ////        modelPath.append("_red");
    //    }
    //
    //    initModelWithPath(temp);
    
    
    
    //减去原先拥有者的能量增长值
    CFightData::getInstance()->addEnergySpeedWith(-ADD_ENERGY_PER_SECOND, m_iFightGroup);
    
//    refreshTheOwner();
    
    //增加后来拥有者的能量增长值
    FightGroup next;
    CBaseFighter* deadTarget = getDeadlyTarget();
    if (deadTarget) {
        next = deadTarget->getFightGroup();
        
    }else{
        
        next = m_iDeadlyGroup;
    }
    
    CFightData::getInstance()->addEnergySpeedWith(ADD_ENERGY_PER_SECOND, next);
    
    
    CBaseBuilding::reborn();
    
    
    //    m_pTheHpBar->setScale(10);
}

void CBaseResourceMine::refreshTheOwner(FightGroup target){
    
    
    CBaseBuilding::refreshTheOwner(target);
    
    //旗帜会飘动
    
    //动画初始化
//    std::string thePath = getTheModelPathByGroup();
    
//    if (thePath!=""&&m_iAttackType == eAttackType_min) {
//        auto animation = Animation3D::create(thePath);
//        auto animation2 = Animation3D::create(thePath);
//        if (animation)
//        {
//            auto animate = Animate3D::create(animation,5.0f/60,130.0f/60);  //45-85 /30 搞笑转圈
//            //        animate->setSpeed(0.2f);//测试用，换模型了就删掉
//            Animate3D *animate2 = Animate3D::create(animation2,5.0f/60,130.0f/60);
//            animate2->setSpeed(-animate->getSpeed());
//            
//            float offset = animate->getTransitionTime()*2;
//            
//            cocos2d::ActionInterval * temp = RepeatForever::create(Sequence::create(animate,DelayTime::create(offset),animate2, NULL));
//            temp->setTag(199);
//            m_pTheSrite->runAction(temp);
//        }
//
//    }
}


    
