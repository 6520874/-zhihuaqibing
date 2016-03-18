//
//  LYBaseTower.h
//  TheGodWill
//
//  Created by user on 14/12/8.
//
//

#ifndef __TheGodWill__LYBaseTower__
#define __TheGodWill__LYBaseTower__

#include <stdio.h>


#include "cocos2d.h"

#include "LYBaseBuilding.h"



class CBaseTower : public CBaseBuilding {
    
public:
    
    
    CBaseTower();
    ~CBaseTower();
    
    static CBaseTower *create(FighterInfo *theInfo,FightGroup theGroup=eFightGroup_Freind);
    
    virtual bool initWithInfo(FighterInfo *theInfo,FightGroup theGroup);
    
    virtual void collidUpdate(float time);//
    virtual void stateUpdate(float time);
    
    virtual void onDied(DamageType type = eDamageType_GeneralAttack);
    
//    virtual void reborn();
    
    void onDestroy(FightGroup target);
    
    
    virtual void setAttackTarget(CBaseFighter* target,bool ifRec=false);
    
protected:
    
//    CC_SYNTHESIZE(FightGroup, m_iRebornGroup, RebornGroup);
    
     CC_SYNTHESIZE(bool,m_bUpdating,Updating);
};









#endif /* defined(__TheGodWill__LYBaseTower__) */
