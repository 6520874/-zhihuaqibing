//
//  LYBaseLifeMine.h
//  TheGodWill
//
//  Created by user on 15/1/19.
//
//

#ifndef __TheGodWill__LYBaseLifeMine__
#define __TheGodWill__LYBaseLifeMine__

#include <stdio.h>

#include "LYBaseResourceMine.h"


#define CURE_RANGE 200  //生命之泉加血范围

class CBaseLifeMine : public CBaseResourceMine {
    
public:
    
    
    CBaseLifeMine();
    ~CBaseLifeMine();
    
    static CBaseLifeMine *create(FighterInfo *theInfo,FightGroup theGroup=eFightGroup_Freind);

    
    virtual bool initWithInfo(FighterInfo *theInfo,FightGroup theGroup);
    
    virtual void stateUpdate(float time);
    
    
protected:
    
    int  cureNum; //治疗量
    
    float m_fMaxTime;//生产周期
    
    float m_fNowTime;//当前时间点
    
    
};

#endif /* defined(__TheGodWill__LYBaseLifeMine__) */
