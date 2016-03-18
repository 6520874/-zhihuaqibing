//
//  LYBaseBuilding.h
//  TheGodWill
//
//  Created by user on 15/2/4.
//
//

#ifndef __TheGodWill__LYBaseBuilding__
#define __TheGodWill__LYBaseBuilding__

#include <stdio.h>

#include "cocos2d.h"

#include "LYBaseFighter.h"



class CBaseBuilding : public CBaseFighter {
    
public:
    
    
    CBaseBuilding();
    ~CBaseBuilding();
    
    static CBaseBuilding *create(FighterInfo *theInfo,FightGroup theGroup=eFightGroup_Freind);
    
    virtual bool initWithInfo(FighterInfo *theInfo,FightGroup theGroup);
    
    virtual void collidUpdate(float time);//
    virtual void stateUpdate(float time);
    
    virtual void beAttacked(CBaseFighter* target,int attack=0,float damagePercent=1,FightGroup attackerGroup=eFightGroup_Freind,
                            DamageType type = eDamageType_GeneralAttack);
    
    virtual void reborn();
    
    void onDied(DamageType type = eDamageType_GeneralAttack);
    
    void onDestroy(FightGroup target);
    
    virtual void refreshTheOwner(FightGroup target =efightGroup_err);//根据阵营初始化模型
    
    
    void buildingSelect();
    void buildingUnselect();
    
    
    bool ifCoverSomeOne();//是否遮住了别人
    void virtualAction(bool ifVirtual=false);//虚化效果
    
    std::string getTheModelPathByGroup(); //获取可变色建筑的模型路径
    
    
    void ChangeModelWithPVPBuildingId(int thePVPBuildingId);//根据pvpbuildingid来改变建筑的模型
    
    
    virtual void resetHPPosition();
    
protected:
    
    CC_SYNTHESIZE(cocos2d::Sprite*, m_pMenuLayer, MenuLayer);
    
    cocos2d::Sprite3D* m_pAtkRangeCircle;
    
    float m_fScale;//pvp中用于显示攻击塔的缩放范围
    
    cocos2d::Sprite3D* m_pTowerUp;
    
    cocos2d::Vec3 m_vecBaseRotation;

    cocos2d::Sprite3D* m_pRuinsSprite;//废墟
    
    cocos2d::ParticleSystem *m_pRuinsParticle;// 废墟粒子特效
    
    //    CC_SYNTHESIZE(FightGroup, m_iRebornGroup, RebornGroup);
    
    
};

#endif /* defined(__TheGodWill__LYBaseBuilding__) */
