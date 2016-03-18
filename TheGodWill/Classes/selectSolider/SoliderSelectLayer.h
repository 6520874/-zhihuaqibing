//
//  SoliderSelectLayer.h
//  TheGodWill
//
//  Created by user on 14/12/1.
//
//

#ifndef __TheGodWill__SoliderSelectLayer__
#define __TheGodWill__SoliderSelectLayer__

#include <stdio.h>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "BaseLayer.h"
#include "LYPersonalData.h"

USING_NS_CC;
USING_NS_CC_EXT;

enum{
    SOLIDER_SECLECT_NECT_TAG = 100,
    SOLIDER_SECLECT_BACK_TAG,
    SOLIDER_SECLECT_EXIT_TAG,
};

class SoliderSelectLayer:public CBaseLayer, public ScrollViewDelegate
{
public:
    SoliderSelectLayer();
    ~SoliderSelectLayer();
    virtual bool init();
    void menuCallBack(Ref* pSender);
    void showSolider();
    void removeSelf();
    
    void updateMonsterInfo();//更新英雄信息
    void onExit();
    virtual bool onTouchBegan(Touch *touch, Event *unused_event);
    virtual void onTouchMoved(Touch *touch, Event *unused_event);
    virtual void onTouchEnded(Touch *touch, Event *unused_event);
    void scrollViewDidScroll(ScrollView* view);
    void scrollViewDidZoom(ScrollView* view) {}
    void adjustScrollView();
    void update(float dt);
    void updateNum();
    void setMoveValid();
    CREATE_FUNC(SoliderSelectLayer);
   // void addBtn(const char* normal, const char* selected, Point &pos, Node* parent, int tag);
private:
    Size m_winSize;
    EventListenerTouchOneByOne* listen;
    Node* m_pNode;
    int m_curPage;
    ScrollView* m_scrollView;
    Menu* pMenu;
    LayerColor* m_bgColor;
    LayerColor* m_pColor;
    LYMenuItemSprite* pMenuItemBack;
    LYMenuItemSprite* pMenuItemNext;
    Sprite* m_pBg;
    Label* m_pLabel;
    bool m_isMoving;
    Point m_startPos;
    Point m_endPos;
    float m_speed;//每一帧移动的速度
    
    std::vector<FighterInfo>m_myFighterInfo;
    Menu* m_fightMenu;
    bool m_bmoveStart;
    bool m_bMoveEnd;
    float m_time;
    
    std::vector<Sprite3D*> soliderVec;
    Vec2 m_guidePos;
};
#endif /* defined(__TheGodWill__SoliderSelectLayer__) */
