//
//  LYBaseBuff.h
//  TheGodWill
//
//  Created by user on 15/1/7.
//
//

#ifndef __TheGodWill__LYBaseBuff__
#define __TheGodWill__LYBaseBuff__

#include <stdio.h>


#include "cocos2d.h"

#include "AllDefine.h"

#include "LYBaseFighter.h"

class CBaseBuff: public cocos2d::Node {
    
public:
    
    CBaseBuff();
    ~CBaseBuff();
    
    CREATE_FUNC(CBaseBuff);
    
    virtual void onExit();
    
    
    void setBuffIdAndStart(AllBuffes theType,float theTime=3);
    
    virtual void update(float time);
    
    
    void onStart();
    void onEnd();
    
    void onDestroy(bool ifRemoveSelf=true); //如果参数为true，则删除自己。如果为false，则不删除，以免重复删除
    
    
protected:
    
    
    CC_SYNTHESIZE(cocos2d::Sprite3D*, m_pTheBuffSprite, TheBuffSprite);
    
    bool ifNeedRotate;//buffsprite是否需要跟随士兵进行转向
    cocos2d::Vec3 RotateOffset;//和士兵面向的偏移
    
    
    
    CC_SYNTHESIZE(CBaseFighter*, m_pTheTarget, TheTarget);//buff生效的人物
    
    CC_SYNTHESIZE(float, m_fdamageBase, DamageBase);
    
    
    CC_SYNTHESIZE(float, m_fNowLifeTime, NowLifeTime);
    CC_SYNTHESIZE(float, m_fMaxLifeTime, MaxLifeTime);
    
    CC_SYNTHESIZE(AllBuffes, m_iBuffType, BuffType);

    
    int m_iDuWave;
    
};






#endif /* defined(__TheGodWill__LYBaseBuff__) */
