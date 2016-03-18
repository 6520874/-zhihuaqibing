//
//  LYBaseWall.cpp
//  TheGodWill
//
//  Created by user on 15/1/22.
//
//

#include "LYBaseWall.h"

USING_NS_CC;


CBaseWall::CBaseWall(){
    
    
}

CBaseWall::~CBaseWall(){
    
    
}

CBaseWall *CBaseWall::create(FighterInfo *theInfo,FightGroup theGroup){
    
    
    CBaseWall * temp = new CBaseWall();
    
    temp->m_iAttackType = eAttackType_wall;
    
    if (temp && temp->initWithInfo(theInfo,theGroup)) {
        temp->autorelease();
        return temp;
    }else{
        
        delete temp;
        temp = NULL;
        return NULL;
    }
    
}

bool CBaseWall::initWithInfo(FighterInfo *theInfo,FightGroup theGroup){
    
    
    
    //此处测试用
    //    SkillInfo temp ;
    //    temp.skillId=-1;
    //    temp.damageRatio=1;
    //    temp.triggerRatio=1;
    //
    //    m_VecSkillInfo.push_back(temp);
    
    
    
    if (!CBaseBuilding::initWithInfo(theInfo,theGroup)) {
        return false;
    };
    
    
    refreshTheOwner(m_iFightGroup);
    
    
//    m_fSpriteScale = 0.6;
//    m_pTheSrite->setScale(m_fSpriteScale);

    
    
    
    
//    m_pTheHpBar->setPosition(0, m_pTheSrite->getBoundingBox().size.height);

    
//    m_vecBaseRotation=Vec3(-90,90,0);
//    m_pTheSrite->setRotation3D(m_vecBaseRotation);
    
    
    return true;
    
}

void CBaseWall::collidUpdate(float time){
    
    //重写此方法，什么都不做。因为资源点不会位移
    
    
    
}


void CBaseWall::stateUpdate(float time){
    
    
  CBaseBuilding::stateUpdate(time);
    
}
