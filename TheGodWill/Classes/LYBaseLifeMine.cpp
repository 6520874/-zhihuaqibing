//
//  LYBaseLifeMine.cpp
//  TheGodWill
//
//  Created by user on 15/1/19.
//
//

#include "LYBaseLifeMine.h"

#include "LYFightData.h"

#include "DataManager.h"

USING_NS_CC;

CBaseLifeMine::CBaseLifeMine(){
    
    
    
}



CBaseLifeMine::~CBaseLifeMine(){
    
    
    
}

CBaseLifeMine *CBaseLifeMine::create(FighterInfo *theInfo,FightGroup theGroup){
    
    CBaseLifeMine * temp = new CBaseLifeMine();
    
    temp->m_iAttackType = eAttackType_min;
    
    if (temp && temp->initWithInfo(theInfo,theGroup)) {
        temp->autorelease();
        return temp;
    }else{
        
        delete temp;
        temp = NULL;
        return NULL;
    }
    
}

bool CBaseLifeMine::initWithInfo(FighterInfo *theInfo,FightGroup theGroup){
    
    //测试用，有模型了就删掉
    //    theInfo.theModelInfo->modelName= "model/piglet/piglet.c3b";
    //    theInfo.theModelInfo->radius = 50;
    
    if (!CBaseResourceMine::initWithInfo(theInfo,theGroup)) {
        return false;
    };
    
    
//    m_fSpriteScale = 40;
//    m_pTheSrite->setScale(m_fSpriteScale);
    
    m_pTheHpBar->setPosition(0, m_pTheSrite->getBoundingBox().size.height);
    
   
    //数据相关初始化
    
    cureNum = m_strTheInfo->level*20;//每级多回复20点体力
    m_fMaxTime = 3; //每3秒回复一次体力
    m_fNowTime = 0;
    
    
    m_vecBaseRotation=Vec3(-90,0,0);
    m_pTheSrite->setRotation3D(m_vecBaseRotation);
    
    
    
    return true;
    
}

void CBaseLifeMine::stateUpdate(float time){
    
    
//    CBaseResourceMine::stateUpdate(time);
    CBaseBuilding::stateUpdate(time);
    
    //时间增长，判断什么时候治疗
    
    m_fNowTime+=time;
    
    
    if (m_fNowTime>=m_fMaxTime) {
        
        
        m_fNowTime=0;
        
        //治疗所有人
        cocos2d::Vector<CBaseFighter*> targetVec = CFightData::getInstance()->getAllAttacker();
        
        for (int i =0; i<targetVec.size(); i++) {
            
            CBaseFighter* temp = targetVec.at(i);
            
            
            if (temp->getFightDistance(this)<CURE_RANGE) { //生命之泉
                temp->becured(cureNum);
            }

        }
        
    }
    
}



