//
//  PropLayer.h
//  TheGodWill
//
//  Created by prefer_shan on 14/12/24.
//
//

#ifndef __TheGodWill__PropLayer__
#define __TheGodWill__PropLayer__

#include <stdio.h>
#include "cocos2d.h"
#include "PublicDefine.h"
#include "AllDefine.h"
#include "BaseLayer.h"
USING_NS_CC;

enum
{
    STORAGE_GOOD_TAG = 100,
    STORAGE_CHEST_TAG,
    STORAGE_COIN_CHARGE,
    STORAGE_STONE_CHARGE,
};


class CPropLayer:public CBaseLayer
{
public:
    CREATE_FUNC(CPropLayer);
private:
    bool init();
    CPropLayer();
    ~CPropLayer();
    
    void OnLeftMenu(Ref *pSender);
    void OnRightMenu(Ref *pSender);
    void OnSaleMenu(Ref *pSender);
    void OnUseMenu(Ref *pSender);
    void OnSaleAllMenu(Ref *pSender);
    void OnExitLvLayer(Ref *pSender);
    void ReloadData();
    void OnSaleCallBack(Ref *pSender);
    void OnSaleAllCallBack(Ref *pSender);
    
    bool onTouchBegan(Touch *pTouch, Event *pEvent);
    void onTouchMoved(Touch *pTouch, Event *pEvent);
    void onTouchEnded(Touch *pTouch, Event *pEvent);
    void onTouchCancelled(Touch *touch, Event *pEvent);
    void OnEraseIt(int iPropId,int iNum=0);
    
    void onExit();
    void OnRemoveNotice();
    void addBtnMenu();//添加按钮
    void OnSaleAllProp(Ref *pSender);
    void OnSaleProp(Ref* pSender);
    void OnUserProp(Ref *pSender);
    
    Menu * m_pMenuUp;
    Menu * m_pMenuNext;
    Menu * m_pMenuSale;
    Menu * m_pMenuUse;
    Menu * m_pMenuSaleAll;
    Menu * m_pMenuClose;
    
    Label *m_pPropNameLabel;
    Label *m_pPropDescLabel;
    
    bool  m_bMenuUse;
    bool  m_bMenuSaleAll;
    bool  m_bMenuSale;
    bool  m_bMenuClose;
    bool  m_bMenuUp;
    bool  m_bMenuNext;
    int m_iSelectIndex;
    int m_iMapIndex;
    EventListenerTouchOneByOne *m_pListner;
    cocos2d::Size m_winSize;
    LYMenuItemSprite * m_pSaleMenuItem;
    LYMenuItemSprite * m_pUseMenuItem;
    Label * m_pLabelSale;
    Label * m_pLabelNeedNum;
    Label * m_pLabelHaveNum;
    Label * m_pSaleNum;
    Label * m_pCurIndex;
    Label * m_pAllIndex;
    Sprite * m_pKey;
    Label * m_pKeyText;
    
    int m_iNeedNum;
    int m_iHaveNum;
    
    Sprite * m_pSpriteNum;
    Sprite * m_pNeedSprite;
    cocos2d::Size  m_tDesc;
    std::vector<MyPropInfo*> m_pMyPropInfo;
    
    std::vector<MyPropInfo*> m_pMyPropAllInfo;
    std::map<int, std::vector<MyPropInfo*>>  m_pPropMap;
    std::vector<Sprite *> m_pPropVecSprite;
    
    std::vector<MyPropInfo*> m_UserChest;//存储用户宝箱和钥匙
    int m_selected;//标识当前显示的是物品还是宝箱0:物品，1:宝箱
    int m_boxIndex;
    int m_iboxSelected;
    Menu* m_pSwitchMenu;
    Label* m_pCoin;
    Label* m_pStone;
    Sprite *m_pMainTaks;
    
    //network
    int m_iSaleAllPropId;
    int m_iSalePropId;
    int m_iSaleIndexId;
    
    int m_iUsePropId;
    
    //
    void onBtnCallBack(Ref* pSender);
    void updateCoinAndStone();
    
    void addCloseGuide(Ref* pSender);//关闭引导
    
};
#endif /* defined(__TheGodWill__PropLayer__) */
