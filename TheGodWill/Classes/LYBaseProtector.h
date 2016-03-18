//
//  LYBaseProtector.h
//  TheGodWill
//
//  Created by user on 15/1/26.
//
//

#ifndef __TheGodWill__LYBaseProtector__
#define __TheGodWill__LYBaseProtector__

#include <stdio.h>

#include "LYBaseFighter.h"


#define MIN_WALK_RANGE 50
#define MAX_WALK_RANGE 150

#define MAX_ALERT_RANGE 350 //预警范围，当怪物和男主角的距离达到一定程度时，保护者就会开始攻击

#define IDLE_WAIT_TIME  3 //空闲时每五秒进行一次位移


class CBaseProtector : public CBaseFighter {
    
public:
    
    CBaseProtector();
    ~CBaseProtector();
    
    static CBaseProtector *create(FighterInfo *theInfo,FightGroup theGroup=eFightGroup_Freind);
    virtual bool initWithInfo(FighterInfo *theInfo,FightGroup theGroup);
    
    
    virtual void collidUpdate(float time);//
    virtual void stateUpdate(float time);
    
    
    virtual bool findTargetLogic();//寻敌逻辑

    
protected:
    
    float m_fNowWaitTime;
    
    
    
    
    
};

#endif /* defined(__TheGodWill__LYBaseProtector__) */
