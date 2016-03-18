//
//  LvUpLayer.h
//  TheGodWill
//
//  Created by prefer_shan on 14/11/25.
//
//

#ifndef __TheGodWill__LvUpLayer__
#define __TheGodWill__LvUpLayer__

#include <stdio.h>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "HeroLvUpLayer.h"
#include "EnergyLvUpLayer.h"
#include "SoldierLvUpLayer.h"
#include "BaseLayer.h"

USING_NS_CC;
USING_NS_CC_EXT;


class CLvUpLayer:public CBaseLayer
{
public:    
    CLvUpLayer();
    ~CLvUpLayer();
    CREATE_FUNC(CLvUpLayer);
    bool init();
private:
    enum  LvUpTag
    {
        LvUpTagSoldier = 100,
        LvUpTagNengliang,
        LVUptagBuilding,
    };
    void  OnMenuClose(Ref *pSender);
    void  OnExitLvLayer(Ref *pSender);
    bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
    void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event) ;
    void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event);
    void onTouchCancelled(Touch *touch, Event *event);
    
    void onExit();
    void addSoliderCloseGuide(Ref* pSender);
    void update(float time);
    
    LYMenuItemSprite * m_pMenuItemSoldier;
    LYMenuItemSprite * m_pMenuItemEnegy;
    LYMenuItemSprite *m_pMenuItemBuilding;
    cocos2d::Size m_winSize;
    EventListenerTouchOneByOne * m_pListener;
    CSoldierLvUpLayer * m_pLayer1;
    CEnergyLvUpLayer *  m_pLayer2;
    CHeroLvUpLayer* m_pLayer3;
    Menu *m_pMenuCose;
};
#endif /* defined(__TheGodWill__LvUpLayer__) */
