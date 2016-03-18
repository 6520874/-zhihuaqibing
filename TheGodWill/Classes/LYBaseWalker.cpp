//
//  LYBaseWalker.cpp
//  TheGodWill
//
//  Created by user on 14/12/18.
//
//

#include "LYBaseWalker.h"

#include "LYFightData.h"

#include "PublicDefine.h"


USING_NS_CC;


CBaseWalker::CBaseWalker(){
    
    
    
}

CBaseWalker::~CBaseWalker(){
    
    
}


CBaseWalker *CBaseWalker::create(FighterInfo *theInfo,FightGroup theGroup){
    
    CBaseWalker * temp = new CBaseWalker();
    
    temp->m_iAttackType = eAttackType_walker;
    
    if (temp && temp->initWithInfo(theInfo,theGroup)) {
        temp->autorelease();
        return temp;
    }else{
        
        delete temp;
        temp = NULL;
        return NULL;
    }
    
}

bool CBaseWalker::initWithInfo(FighterInfo *theInfo,FightGroup theGroup){
    
    
    
    //此处测试用
//    SkillInfo temp ;
//    temp.skillId=-1;
//    temp.damageRatio=1;
//    temp.triggerRatio=1;
//    
//    m_VecSkillInfo.push_back(temp);
    
    
    
    if (!CBaseFighter::initWithInfo(theInfo,theGroup)) {
        return false;
    };
    
//    m_pTheHpBar->setScale(1);
    
    m_v2AttackPosOffset = Vec3(10, 100, 10);
    
    
    
    Sprite * theTag =  Sprite::createWithSpriteFrameName("map_protect.png");
    theTag->setScale(0.8f);
    CFightManager::addCamera(theTag);
    theTag->setPosition3D(Vec3(0, m_pTheHpBar->getPositionY()+ m_pTheHpBar->getBGHP()->getBoundingBox().size.height+theTag->getBoundingBox().size.height*0.5f, 0));
    addChild(theTag);
    
    
//    m_pTheSrite->setColor(Color3B::GREEN);
    
    stateChange(eFighteState_Walk);
    
    m_iTargetX = END3DPOINT+WALKER_TARGET_X;
    
    
    return true;
    
}

void CBaseWalker::collidUpdate(float time){
    
    //重写此方法，什么都不做。因为资源点不会位移
    
    CBaseFighter::collidUpdate(time);
    
    
}


void CBaseWalker::stateUpdate(float time){
    
    
    CBaseFighter::stateUpdate(time);
    
    //测试用
    //    ifSkillStart();
    //    skillPrepare();
    //    return;
    
//    animateChange(eFighteState_Walk);
//    
//    
//    //默认所有保护模式的怪最终点一致
//    m_v3SearchPos= Vec3(END3DPOINT-300,0,(ENDZPOINT-STARTZPOINT)*0.5f);
//    
//    m_bIfSearchMode = true;
//    
//    //移动
//    if (m_bIfDied) {
//        return;
//    }
//    
//    positionUpdate(time);
//    roatationUpdate(time);
    

    
    //如果位置和目标点接近。则win
//    if (getPosition3D().distance(m_v3SearchPos)<=10) {
//        CFightData::getInstance()->addTheFightConditon(eLevelCondition_protect, 1);
//    }
    
    if(getPositionX()>m_iTargetX){
        
        CFightData::getInstance()->addTheFightConditon(eLevelCondition_protect, 1,this);
        
    }
    

}

void CBaseWalker::onDied(DamageType type){
    
    CBaseFighter::onDied(type);
    
//    CFightData::getInstance()->addTheFightConditon(eLevelCondition_protect, -1);
    
    //英雄死了就输了
    CFightData::getInstance()->setWinState(-1,this);
    
    
    
}
