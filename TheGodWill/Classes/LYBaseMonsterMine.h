//
//  LYBaseMonsterMine.h
//  TheGodWill
//
//  Created by user on 15/1/19.
//
//

#ifndef __TheGodWill__LYBaseMonsterMine__
#define __TheGodWill__LYBaseMonsterMine__

#include <stdio.h>

#include "LYBaseResourceMine.h"


class CBaseMonsterMine : public CBaseResourceMine {
    
public:
    
    CBaseMonsterMine();
    ~CBaseMonsterMine();
    
    static CBaseMonsterMine *create(FighterInfo *theInfo,FightGroup theGroup=eFightGroup_Freind);
    
    virtual void onChangeOwner(FightGroup target);
    
    virtual bool initWithInfo(FighterInfo *theInfo,FightGroup theGroup);
    
    virtual void stateUpdate(float time);
    
    virtual void reborn();
    
protected:
    
    FighterInfo m_stTheFighterInfo;
    
    float m_fMaxProudceTime;//生产周期
    
    float m_fNowProudceTime;//当前时间点
    
    
    int m_iMaxSoldierNum; //最大士兵数量
    
    int m_iNowSoldierNum; //当前士兵数量
    
    
    cocos2d::Vector<CBaseFighter*> allSoldier;
    
    cocos2d::Label* m_pSolderNumLabel;
    
    
    
};









#endif /* defined(__TheGodWill__LYBaseMonsterMine__) */
