//
//  SkillUpMenuLayer.h
//  TheGodWill
//
//  Created by prefer_shan on 14/12/25.
//
//

#ifndef __TheGodWill__SkillUpMenuLayer__
#define __TheGodWill__SkillUpMenuLayer__

#include <stdio.h>
#include "cocos2d.h"
#include "BaseLayer.h"
#include "SoldierSkillUpLayer.h"
#include "HeroSkillUpLayer.h"
#include "BaseLayer.h"

USING_NS_CC;


class CSKillMenuLayer:public  CBaseLayer
{
public:
    CREATE_FUNC(CSKillMenuLayer);

    void StartHeroLayer();
    void StartSoldierLayer();
     Menu  * m_pMenu;
    
    enum  LVSkillTag
    {
        LvSkillTagSoldier = 100,
        LVSkillTagHero,
        LvCloseTag,
        LVBgTag
    };
    
private:
    CSKillMenuLayer();
    ~CSKillMenuLayer();
    void OnExitLvLayer(Ref *pSender);
    bool init();
    bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
    void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event) ;
    void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event);
    void onTouchCancelled(Touch *touch, Event *event);
    void OnSelectTouch(Ref *pSender);
    void onExit();
    void update(float time);
   
    void addCloseSkillGuide(Ref *pSender);
    void addbg();
    
    bool m_bMenu;
    static CSKillMenuLayer *t;
    EventListenerTouchOneByOne * m_pListener;
    cocos2d::Size m_winSize;
    LYMenuItemSprite * m_pMenuItemSoldier;
    LYMenuItemSprite * m_pMenuItemHero;

    Sprite *m_pBg;
    
    cocos2d::extension::CSoldierSkillUpLayer * m_pLayer1;
    cocos2d::extension::CHeroSkillUpLayer * m_pLayer2;
    Menu *m_pMenuCose;
};

#endif /* defined(__TheGodWill__SkillUpMenuLayer__) */
