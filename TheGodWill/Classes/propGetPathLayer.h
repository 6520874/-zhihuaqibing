//
//  propGetPathLayer.h
//  WarOfGod
//
//  Created by user on 15/5/27.
//
//

#ifndef __WarOfGod__propGetPathLayer__
#define __WarOfGod__propGetPathLayer__

#include <stdio.h>
#include "cocos2d.h"
#include "BaseLayer.h"

enum
{
    PATH_EXIT_TAG = 5000,
    PATH_GAME_START,
};

class propGetPathLayer:public CBaseLayer
{
public:
    propGetPathLayer();
    ~propGetPathLayer();
    
    static propGetPathLayer* create(int iPropId);
    bool init();
    void initLevellInfo();
    void addLevelBtn(int index, Node* parent, Vec2 pos);
    void btnCallBack(Ref* pSender);
    void refreshLevelInfo();//刷新小关卡信息
    
    void OnStartFight(Ref *pSender);
    
    virtual bool onTouchBegan(Touch* pTouch, Event* pEvent);
private:
    EventListenerTouchOneByOne* listener;
    Size m_winSize;
    int m_iPropId;
    Menu* p_menu;
    Menu* m_pStartMenu;
    Sprite* m_levelBg;
    int m_selectedLevel;//当前选中的小关卡
    std::vector<int> m_levelVec;
    int m_comNum;//记录普通关卡的数量
    bool m_noSelectedLevel;//判断是否当前显示的关卡都是未开启的
};

#endif /* defined(__WarOfGod__propGetPathLayer__) */
