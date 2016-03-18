//
//  LYBaseWalker.h
//  TheGodWill
//
//  Created by user on 14/12/18.
//
//

#ifndef __TheGodWill__LYBaseWalker__
#define __TheGodWill__LYBaseWalker__

#include <stdio.h>

#include "cocos2d.h"

#include "LYBaseFighter.h"

class CBaseWalker : public CBaseFighter {
    
public:
    
    
    CBaseWalker();
    ~CBaseWalker();
    
    static CBaseWalker *create(FighterInfo *theInfo,FightGroup theGroup=eFightGroup_Freind);
    
    virtual bool initWithInfo(FighterInfo *theInfo,FightGroup theGroup);
    
    virtual void collidUpdate(float time);//
    virtual void stateUpdate(float time);
    
    virtual void onDied(DamageType type = eDamageType_GeneralAttack);
    
    
    
protected:
    
    CC_SYNTHESIZE(int, m_iTargetX, TargetX);
    
    //    CC_SYNTHESIZE(FightGroup, m_iRebornGroup, RebornGroup);
    
    
};


#endif /* defined(__TheGodWill__LYBaseWalker__) */
