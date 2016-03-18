//
//  LYBaseHero.h
//  TheGodWill
//
//  Created by user on 14/12/11.
//
//

#ifndef __TheGodWill__LYBaseHero__
#define __TheGodWill__LYBaseHero__

#include <stdio.h>


#include "cocos2d.h"
#include "LYBaseFighter.h"

#include "AllDefine.h"


class CBaseHero :public CBaseFighter {
    
public:
    
    CBaseHero();
    ~CBaseHero();
    
    static CBaseHero *create(FighterInfo *theInfo,FightGroup theGroup=eFightGroup_Freind);
    virtual bool initWithInfo(FighterInfo *theInfo,FightGroup theGroup);

    
    virtual void collidUpdate(float time);//
    virtual void stateUpdate(float time);
    
    void LaganMove(int x,int y,float fAngle);
    void LaganStop();
    
    virtual void onDied(DamageType type = eDamageType_GeneralAttack);
    
    virtual void beAttacked(CBaseFighter* target,int attack=0,float damagePercent=1,FightGroup attackerGroup=eFightGroup_Freind,
                            DamageType type = eDamageType_GeneralAttack);
    
    virtual void afterDied();
    
    //测试模型用
    void testmode();

protected:
    
    CC_SYNTHESIZE(bool, m_bIfCameraFollow, IfCameraFollow);
    
    
    

    

};




#endif /* defined(__TheGodWill__LYBaseHero__) */
