//
//  LYBaseCity.h
//  TheGodWill
//
//  Created by user on 14/12/17.
//
//

#ifndef __TheGodWill__LYBaseCity__
#define __TheGodWill__LYBaseCity__

#include <stdio.h>

#include "cocos2d.h"

#include "LYBaseBuilding.h"

class CBaseCity : public CBaseBuilding {
    
public:
    
    
    CBaseCity();
    ~CBaseCity();
    
    static CBaseCity *create(FighterInfo *theInfo,FightGroup theGroup=eFightGroup_Freind);
    
    virtual bool initWithInfo(FighterInfo *theInfo,FightGroup theGroup);
    
    virtual void collidUpdate(float time);//
    virtual void stateUpdate(float time);
    
    virtual void onDied(DamageType type = eDamageType_GeneralAttack);
    
    
protected:
    
    //    CC_SYNTHESIZE(FightGroup, m_iRebornGroup, RebornGroup);
    
    
};


#endif /* defined(__TheGodWill__LYBaseCity__) */
