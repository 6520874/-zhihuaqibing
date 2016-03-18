//
//  thePartolManLayer.h
//  WarOfGod
//
//  Created by user on 15/8/12.
//
//

#ifndef __WarOfGod__thePartolManLayer__
#define __WarOfGod__thePartolManLayer__

#include <stdio.h>
#include "cocos2d.h"

#include "LYBaseFighter.h"
#include "pvpCommonData.h"

USING_NS_CC;


class CPartolFight: public CBaseFighter {
    
public:
    
    
    CPartolFight();
    ~CPartolFight();
    
    static CPartolFight* create(FighterInfo *theInfo, FightGroup theGroup=eFightGroup_Freind);
    virtual bool initWithInfo(FighterInfo *theInfo,FightGroup theGroup);
    void update(float dt);
    void positionUpdate(float time);
    void roatationUpdate(float time);
    void setDestationPos(Vec3 pos);
    
    float getFightDistance(CBaseFighter* target);
    Vec3 getValidPosition();
    //void updatePartolTower(buildInfo* tower);
private:
    int m_iRadius;//巡逻者的活动半径
    float m_fStopTime;
    float m_fTime;
    CC_SYNTHESIZE(Vec2, m_tagetPos, TargetPos);//祭坛的坐标
    CC_SYNTHESIZE(int, m_iIndex, Index);
    std::vector<Vec3> m_persionPos;
    CC_SYNTHESIZE(int, m_iAddHeight, AddHeight);//是否增加高度
    CC_SYNTHESIZE(int, m_iMonsterId, MonsterId);//是否增加高度
    bool m_bStartStoped;//标识当前巡逻的人是否停止巡逻
    //locationInfo m_allBlock[MAP_X_INDEX][MAP_Y_INDEX];
};


#endif /* defined(__WarOfGod__thePartolManLayer__) */
