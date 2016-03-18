//
//  LYBaseWall.h
//  TheGodWill
//
//  Created by user on 15/1/22.
//
//

#ifndef __TheGodWill__LYBaseWall__
#define __TheGodWill__LYBaseWall__

#include <stdio.h>

#include "LYBaseBuilding.h"


class CBaseWall : public CBaseBuilding {
    
public:
    
    CBaseWall();
    
    ~CBaseWall();
    
    static CBaseWall *create(FighterInfo *theInfo,FightGroup theGroup=eFightGroup_Freind);
    
    virtual bool initWithInfo(FighterInfo *theInfo,FightGroup theGroup);
    
    virtual void collidUpdate(float time);//
    virtual void stateUpdate(float time);
    
    
};

#endif /* defined(__TheGodWill__LYBaseWall__) */
