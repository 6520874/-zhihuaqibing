//
//  FightFailLayer.h
//  TheGodWill
//
//  Created by prefer_shan on 14/12/18.
//
//

#ifndef __TheGodWill__FightFailLayer__
#define __TheGodWill__FightFailLayer__

#include <stdio.h>

#include "cocos2d.h"
#include "BaseLayer.h"

USING_NS_CC;

class CFightFailLayer:public CBaseLayer
{
public:
    CREATE_FUNC(CFightFailLayer);
private:
    CFightFailLayer();
    ~CFightFailLayer();
    bool init();
    void OnCallBack(Ref *pSender);
    void backPvpMainLayer(Ref *pSender);
    virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
    void OnFightAgain(Ref *pSender);
    void onExit();
    void OnGoFirst(Ref *pSender);
    void OnGoSecond(Ref *pSender);
    
    void showUpgradeGuide(Ref* pSender);
    
    MenuItemFont * m_pLabelGo1;
    Menu *m_pMenuGo1;
    Menu *m_pMenuGo2;
    
    MenuItemFont * m_pLabelGo2;
    MenuItemSprite * m_pMenuItemGoArrow1;
    MenuItemSprite * m_pMenuItemGoArrow2;
    bool m_bGoArrow1;
    bool m_bGoArrow2;
    cocos2d::Size m_winSize;
    EventListenerTouchOneByOne* listen;
private:

};

#endif /* defined(__TheGodWill__FightFailLayer__) */
