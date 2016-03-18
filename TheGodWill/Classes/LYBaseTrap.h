//
//  LYBaseTrap.h
//  WarOfGod
//
//  Created by user on 15/6/29.
//
//

#ifndef __WarOfGod__LYBaseTrap__
#define __WarOfGod__LYBaseTrap__

#include <stdio.h>

#include "LYBaseBuilding.h"


class CBaseTrap : public CBaseBuilding {
    
public:
    
    CBaseTrap();
    
    ~CBaseTrap();
    
    static CBaseTrap *create(FighterInfo *theInfo,FightGroup theGroup=eFightGroup_Freind);
    
    virtual bool initWithInfo(FighterInfo *theInfo,FightGroup theGroup);
    
    virtual void stateUpdate(float time);
    
    
    void TrapActive();//激活陷阱
    
    void TrapSkillStart(float time);//陷阱爆炸
    
private:
    void update(float time);
    
    bool bDingshiBorn;

    
    int m_iTrapState;//0表示隐藏，1表示触发了
    CC_SYNTHESIZE(bool,m_bUpdating,Updating);
};

#endif /* defined(__WarOfGod__LYBaseTrap__) */
