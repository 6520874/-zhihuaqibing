//
//  FightPapreLayer.h
//  TheGodWill
//
//  Created by zhaixh on 14/12/1.
//
//

#ifndef TheGodWill_FightPapreLayer_h
#define TheGodWill_FightPapreLayer_h

#include "cocos2d.h"
#include "BaseLayer.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;
#define PAGE_NUM 3//每页显示的兵士数量

enum{
    FIGHT_EXIT_TAG = 100,
    FIGHT_SURE_TAG,
};

class FightPrepareLayer : public CBaseLayer
{
public :
    FightPrepareLayer();
    ~FightPrepareLayer();
    virtual bool init();
    void initUI();
    CREATE_FUNC(FightPrepareLayer);
    void menuCallBack(Ref* pSender);
    virtual bool onTouchBegan(Touch *touch, Event *unused_event);
    void showSolider(int curPage, int soliderIndex);
    void addBtn(const char* normal, const char* selected, const char* child, cocos2d::Point &pos, Node* parent, int tag);
    void addGuideInfo();
    void onExit();
private:
    cocos2d::Size m_winSize;
    int m_curSelectedSolider;
    Menu* m_pMenu;
    Sprite* m_pBg;
    EventListenerTouchOneByOne* listen;
};

#endif
