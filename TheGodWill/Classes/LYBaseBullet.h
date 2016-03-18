//
//  LYBaseBullet.h
//  TheGodWill
//
//  Created by user on 14/12/23.
//
//

#ifndef __TheGodWill__LYBaseBullet__
#define __TheGodWill__LYBaseBullet__

#include <stdio.h>

#include "cocos2d.h"


#define BULLET_HIGHT   1000   //抛射时子弹射多高。

#define BULLET_ACCELERATE  1000  //抛射子弹的重力加速度


class CBaseFighter;

class CBaseBullet: public cocos2d::Node {
    
    //子弹类
    
public:
    
    CBaseBullet();
    ~CBaseBullet();
    
    
    
    CREATE_FUNC(CBaseBullet);
    
    virtual void onExit();
    
    
    void setAttackerAndPositionAndRun(CBaseFighter* attacker, cocos2d::Vec3 targetPos ,int bulletId=0 );
    
    
    
    
    
    void update(float time);
    
    void rotationUpdate();
    
    
    void onDestroy(bool ifRemoveSelf=true); //如果参数为true，则删除自己。如果为false，则不删除，以免重复删除
    
    
    void onDamage();//造成伤害的相关逻辑
    
protected:
   
    CBaseFighter* m_pAttacker;
    
    cocos2d::Vec3 m_v3TargetPos;
    
    cocos2d::Vec3 m_v3Speed;
    
    cocos2d::Sprite3D* m_pThebulletModel;//子弹模型
    
    
    int bulletModelType;//子弹模型的类型 0为3d模型，1为图片。如果是图片就不能进行rotationupdate
    
    //已停止时间，用于生命周期计算
    float m_fStoppedTime;
    
    cocos2d::Vec3 m_v3OriginPos;//初始位置
    
    CC_SYNTHESIZE(float, m_fDamagePercent, DamagePercent);//造成攻击力百分比的伤害
    
    CC_SYNTHESIZE(float, m_fDamageLifePercent, DamageLifePercent);//造成当前生命值百分比伤害。
    
    float m_fMaxLifeTime;
    float m_fNowLifeTime;
    
    bool m_bIfOver;
    
};




#endif /* defined(__TheGodWill__LYBaseBullet__) */
