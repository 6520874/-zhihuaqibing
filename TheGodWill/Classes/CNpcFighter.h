//
//  CNpcFighter.h
//  WarOfGod
//
//  Created by prefer_shan on 15/8/3.
//
//

#ifndef __WarOfGod__CNpcFighter__
#define __WarOfGod__CNpcFighter__

#include <stdio.h>
#include "AllDefine.h"
#include "cocos2d.h"
#include "LYBaseFighter.h"




class pvpMainLayer;

class CNpcFighter:public CBaseFighter
{
public:
    CNpcFighter();
    ~CNpcFighter();
    
    static CNpcFighter *create(FighterInfo *theInfo,int iMonsterId);
    
    void  setPvpMainLayer(pvpMainLayer*p);
    
    
      void SetDest(cocos2d::Vec3 dst);
    
    /**
     动画相关
     **/
    cocos2d::ActionInterval* m_pAction_IDle;
    cocos2d::ActionInterval* m_pAction_Walk;
    cocos2d::ActionInterval* m_pAction_Attack;
    cocos2d::ActionInterval* m_pAction_specialAttack;
    cocos2d::ActionInterval* m_pAction_Hurt;
    cocos2d::ActionInterval* m_pAction_Die;
    cocos2d::ActionInterval* m_pAction_Dizzy;

    void setAddMoveSpeed(float var);
private:
    
    virtual bool initWithInfo(FighterInfo *theInfo,int iMonsterId);
  
  
    void SetRunAway();
    
    
    void initModelWithPath(std::string path);
    
    void  initAnimate();

    
    
    void  asyncLoad_Callback(cocos2d::Sprite3D *sprite, void *param);
    
    //0 平常模式  1 逃跑模式
    void setFightMode(int iMode);
    
    
    void update(float time);
    void positionUpdate(float time);
    
    void roatationUpdate(float time);
    
    CC_SYNTHESIZE(cocos2d::Sprite3D*, m_pShadow, TheShadow);
    CC_SYNTHESIZE(cocos2d::Sprite3D*, m_pShadow2, TheShadow2);
    
    CC_SYNTHESIZE(int, m_iMode, TheMode);
    
    modelInfo* m_pInfo ;
    cocos2d::Vec3 m_DstVec3;

    
    pvpMainLayer *m_parentLayer;
    

};



#endif /* defined(__WarOfGod__CNpcFighter__) */
