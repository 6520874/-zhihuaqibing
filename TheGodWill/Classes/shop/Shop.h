//
//  Shop.h
//  WarOfGod
//
//  Created by prefer_shan on 15/5/4.
//
//

#ifndef __WarOfGod__Shop__
#define __WarOfGod__Shop__

#include <stdio.h>
#include "BaseLayer.h"
#include "CSharedPlist.h"
#include "AllDefine.h"
#include "cocos-ext.h"
#include "BaseLayer.h"
USING_NS_CC;
USING_NS_CC_EXT;


class CShopLayer:public CBaseLayer, public ScrollViewDelegate
{
public:
    CREATE_FUNC(CShopLayer);
    bool init();
    
private:
    //物品
    void OnGreenTouch(Ref *pSender);
    std::string getShopIcoBg(int iPropLevel);
    void OnCloseMenu(Ref *psender);
    void OnWupin(Ref *pSender);
    void onExit();
    bool onTouchBegan(Touch *pTouch, Event *pEvent);
    void onTouchMoved(Touch *pTouch, Event *pEvent);
    void onTouchEnded(Touch *pTouch, Event *pEvent);
    void onTouchCancelled(Touch *touch, Event *pEvent);
    void ReloadData();
    void UpdateMoney(Ref *pSender);
    
    void OnCoinPlus(Ref *pSender);
    void OnSetVisible(Ref *pSender);
    
    void OnStonePlus(Ref *pSender);
    //宝箱
    void OnChest(Ref *pSender);
    void OnMenuPropIcoTouch(Ref *pSender);
    
    int m_index;
    EventListenerTouchOneByOne * m_pListener;
    ScrollView* m_scrollView;
    bool m_bScrollView;
    MenuItemSprite * m_pWupinItem;
    MenuItemSprite * m_pBoxItem;
    std::map<int,std::vector<ShopItemInfo*>> m_pPropMap;
    Label * m_pCoinLabel;
    Label * m_pStoneLabel;
    
    Menu * m_pPropMenu;
    Menu * m_pGreenMenu;
    Menu * m_pMenuItem;
    Menu * m_pPlusItem;
    Menu * m_pMinusItem;
    Menu * m_pCloseItem;
    Menu * m_pCoinItem;
    Menu * m_pStoneItem;

    bool  m_bCoinMenu;
    bool  m_bStoneMenu;
    bool  m_bCloseMenu;
    bool  m_bPropMenu;
    bool  m_bGreenMenu;
    bool  m_bItemMenu;
    bool  m_bTouch;
    
    CShopLayer();
    ~CShopLayer();
    bool m_isMoved;
    bool m_bProp;//0 道具  1宝箱
     Vec2 m_start;
    
    
    std::vector<Sprite*> m_pSpriteVec;
};
#endif /* defined(__WarOfGod__Shop__) */
