//
//  CLevelInfoLayer.h
//  TheGodWill
//
//  Created by user on 14/12/23.
//
//

#ifndef __TheGodWill__CLevelInfoLayer__
#define __TheGodWill__CLevelInfoLayer__

#include <stdio.h>
#include "cocos2d.h"
#include "BaseLayer.h"


enum {
    LEVELINFO_EXIT = 201,
    LEVELINFO_ACQUIRE,
    LEVELINFO_START,
    LEVELINFO_LOCK,
    LEVELINFO_EQLITE,
    LEVELINFO_COMMON,
    LEVELINFO_TYPE,
    
    
    
    LEVELINFO_STAR,
};

USING_NS_CC;

class CLevelInfoLayer: public CBaseLayer
{
public:
    CLevelInfoLayer();
    ~CLevelInfoLayer();
    CREATE_FUNC(CLevelInfoLayer);
    bool init();
    void initLevelInfo();
    void addSmallLevel(int smallLevel, Node* parent, cocos2d::Point pos);
    void levelPanel();
    void btnCallBack(Ref* pSender);
    void reFreshLevelReawrd();
    void testEffect();
    void setSelectedState();
    void setNewLevelFlag();//设置新关卡开启标识
    void addStarEffect();
    
    void initTowerLevel();//塔防模式关卡
    void addMenu();
    void towerBtnCallBack(Ref* pSender);
    
    bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
    void addGetAwardGuide(Ref* pSender);
    void OnOpenBx(Ref *pSender);
    void OnStartFight(Ref *pSender);
    void addExitGuide(Ref* pSender);
    void onExit();
    void backMainMenu();
    void OnUpdateCoin(Ref *pSender);
    void OnUpdateStone(Ref *pSender);
    Sprite* m_awardBg;
    EventListenerTouchOneByOne* listener;
private:

    cocos2d::Size m_winSize;
    Menu* _pMenu;
    int m_curMidLevel;//当前选中大关卡
    int m_smallLevel;//当前已通关关卡数
    int m_curSmallLevel;//当前选中小关卡
    bool m_bEqlite;//标识是否是精英关卡
    
    int m_iGotNum;
    int m_iReceiveNum;
    
    LYMenuItemSprite* m_pEqliteItem;
    LYMenuItemSprite* m_pComItem;
    Sprite* m_levelBg;
    Menu* m_pAcquireMenu;
    Node* m_node;//存储奖励信息
    LayerColor* m_pAwardLayer;//塔防模式
    LayerColor* m_pLayer;
    Menu* m_pStartMenu;
    
    Sprite *m_pJianliBg;
    
    MenuItemSprite * m_pMenuItemBx1;
    MenuItemSprite * m_pMenuItemBx2;
    MenuItemSprite * m_pMenuItemBx3;
    
};

#endif /* defined(__TheGodWill__CLevelInfoLayer__) */
