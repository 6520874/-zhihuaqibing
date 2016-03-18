//
//  LYBaseAttack.h
//  TheGodWill
//
//  Created by user on 14/12/2.
//
//

#ifndef __TheGodWill__LYBaseAttack__
#define __TheGodWill__LYBaseAttack__

#include <stdio.h>

#include "cocos2d.h"

#include "AllDefine.h"

#include "LYBaseFighter.h"

#include "LYFightManager.h"

class CBaseAttack: public cocos2d::Node {
    
    
    
public:
    
    CBaseAttack();
    ~CBaseAttack();
    
    
    CREATE_FUNC(CBaseAttack);
    
    virtual void onExit();
    
    void setSkillIdAndStart(FighterSkillInfo* theInfo,CBaseFighter* attacker,CBaseFighter* targetAttacker,Vec3 targetPos=Vec3::ZERO,int damage=0,FightGroup attackerGroup=eFightGroup_Freind);
    
    void animateStart();
    
    void update(float time);
    virtual void onEnterTransitionDidFinish();
    
    void rotationUpdate();
    
    
    void onDestroy(bool ifRemoveSelf=false); //如果参数为true，则代表是父类删除的自己，直接删除，不做任何操作
    

    void onDamage();//造成伤害的相关逻辑
    
    void onEffect(CBaseFighter* target);//技能特殊效果的相关逻辑
    
protected:
    
//    CC_SYNTHESIZE(AttackCommandInfo, m_stAttackInfo, AttackInfo);
    CC_SYNTHESIZE(FightGroup,m_iAttackerGroup, AttackerGroup); //攻击者阵营
    CC_SYNTHESIZE(int,m_iDamage, Damage); //攻击者阵营
    CC_SYNTHESIZE(int,m_iHurtedNumber, HurtedNumber); //攻击者阵营
    CC_SYNTHESIZE(cocos2d::Sprite3D*, m_pBulletSprite, BulletSprite);//子弹
    
    CC_SYNTHESIZE(cocos2d::Sprite*, m_pBulletAnimate, BulletAnimate);//帧动画
    
    
    CC_SYNTHESIZE(cocos2d::Vec3, m_v3BulletSpeed, BulletSpeed);
    
    CC_SYNTHESIZE(cocos2d::Vec3, m_v3OriginBulletSpeed, OriginBulletSpeed);
    
    CC_SYNTHESIZE(cocos2d::Vec3, m_v3OriginPos, OriginPos);
    
    
    CC_SYNTHESIZE(float, m_fAcceleratedSpeedY, BulletSpeedY);   //高度轴y上的加速度
    
    
    CC_SYNTHESIZE(CBaseFighter*, m_pAttacker, Attacker);
    CC_SYNTHESIZE(CBaseFighter*, m_pTarget, Target);
    
    
    FighterSkillInfo* m_pTheskillInfo;
    
    FightGroup m_eAttackerGroup;
    
    //临时数据
    CC_SYNTHESIZE(float, m_fNowLifeTime, NowLifeTime);//已经存在了多久
    CC_SYNTHESIZE(int, m_iNowHurtTimes, NowHurtTimes);//已经造成了几次伤害
    
    
    
    
    
    CC_SYNTHESIZE(cocos2d::Vec3, m_v3TargetPos, TargetPos);
    
private:
    
    bool ifDestroyed;
    
    
};














#endif /* defined(__TheGodWill__LYBaseAttack__) */
