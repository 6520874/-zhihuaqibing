//
//  LYBaseResourceMine.h
//  TheGodWill
//
//  Created by user on 14/12/8.
//
//

#ifndef __TheGodWill__LYBaseResourceMine__
#define __TheGodWill__LYBaseResourceMine__

#include <stdio.h>

#include "cocos2d.h"

#include "LYBaseBuilding.h"


#define ADD_ENERGY_PER_SECOND 5


class CBaseResourceMine : public CBaseBuilding {
    
public:
    
    
    CBaseResourceMine();
    ~CBaseResourceMine();
    
    static CBaseResourceMine *create(FighterInfo *theInfo,FightGroup theGroup=eFightGroup_Freind);
    
    virtual bool initWithInfo(FighterInfo *theInfo,FightGroup theGroup);
    
    virtual void collidUpdate(float time);//
    virtual void stateUpdate(float time);
    
    virtual void refreshTheOwner(FightGroup target =efightGroup_err);//继承父类的 ，根据阵营初始化模型
    

    virtual void onChangeOwner(FightGroup target);
    
    virtual void reborn();
    
};  




#endif /* defined(__TheGodWill__LYBaseResourceMine__) */
