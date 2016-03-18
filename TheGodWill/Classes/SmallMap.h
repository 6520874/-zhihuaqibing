//
//  SmallMap.h
//  TheGodWill
//
//  Created by prefer_shan on 14/12/1.
//
//

#ifndef __TheGodWill__SmallMap__
#define __TheGodWill__SmallMap__

#include <stdio.h>
#include "cocos2d.h"
#include "PublicDefine.h"
#include "AllDefine.h"
USING_NS_CC;
//小地图


class CFightScene;

class CSmallMap:public  cocos2d::Layer
{
    
    CSmallMap(CFightScene *pScene);
    
    ~CSmallMap();
 
public:
    static CSmallMap* create(CFightScene *pScene = NULL) \
    { \
        CSmallMap *pRet = new CSmallMap(pScene); \
        if (pRet && pRet->init()) \
        { \
            pRet->autorelease(); \
            return pRet; \
        } \
        else \
        { \
            delete pRet; \
            pRet = NULL; \
            return NULL; \
        }
    }
    
    Sprite * SetSmallMapPostion(Vec3 vt,FighterType t, FightGroup theGroup = eFightGroup_Freind, AttackType theAttack = eAttackType_monster,bool ifBoss=false);
    void RemoveSmallMapPostion(Sprite *pSprite);
    void SetSmallMapPostion(Vec3  vt,Sprite *p);
    
    Vec2 getMapPosBy3dPos(Vec3 pos3D);
    
private:
    bool init();
    //void update(float ft);

 
    std::string GetSoldierTypeImg(FighterType t, FightGroup theGroup,AttackType theAttack = eAttackType_monster,bool ifBoss=false);

    std::string  GetSoldierTypeImg();
    
    void OnMenuViewMove(Ref *pSender);
    bool onTouchBegan(Touch *pTouch, Event *pEvent);
    void onTouchMoved(Touch *pTouch, Event *pEvent);
    void onTouchEnded(Touch *pTouch, Event *pEvent);
    void onTouchCancelled(Touch *touch, Event *pEvent);

    Menu * m_pMenuViewMove;
    bool   m_bMenuViewMove;
    Sprite * m_pCurView; //当前视角
   
    cocos2d::Size m_WinSize;
 
    CFightScene *m_pScene;
    
    Vector<Sprite *> m_pSpriteMap;

    static CSmallMap *t;
};

#endif /* defined(__TheGodWill__SmallMap__) */
