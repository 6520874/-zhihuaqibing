//
//  pvpEnemyLayer.h
//  WarOfGod
//
//  Created by user on 15/6/11.
//
//

#ifndef __WarOfGod__pvpEnemyLayer__
#define __WarOfGod__pvpEnemyLayer__

#include <stdio.h>
#include "cocos2d.h"
#include "BaseLayer.h"
#include "LYBaseFighter.h"
#include "pvpMainLayer.h"

USING_NS_CC;

class pvpEnemyLayer:public CBaseLayer
{
public:
    pvpEnemyLayer();
    ~pvpEnemyLayer();
    
    CREATE_FUNC(pvpEnemyLayer);
    bool init();
    void initUI();
    void adduserInfo();
    void addTower(buildingMessage& messInfo);
//    bool onTouchBegan(Touch* pTouch, Event* pEvent);
//    void onTouchMoved(Touch* pTouch, Event* pEvent);
//    void onTouchEnded(Touch* pTouch, Event* pEvent);
    virtual void onTouchesBegan(const std::vector<Touch*>& touches, Event *unused_event);
    virtual void onTouchesMoved(const std::vector<Touch*>& touches, Event *unused_event);
    virtual void onTouchesEnded(const std::vector<Touch*>& touches, Event *unused_event);
    
    void addBuildObb(CBaseBuilding* tower);
    void getEnemyTeamInfoBack(Ref* pSender);//获取目标阵营信息
    void addCloud();
    void startSearchEnemy();
    void getEnemyTeamInfo();
    
    void btnCallBack(Ref* pSender);
    void calculateRayByLocationInView(Ray* ray,Camera* theCamera, const Vec2& location);
private:
    Size m_winSize;
    EventListenerTouchAllAtOnce* m_listen;
    cocos2d::Camera *_camera;
    CC_SYNTHESIZE_READONLY(cocos2d::Layer*, layer3D, theLayer3D);
    std::vector<towerOBBInfo> m_allBuildObb;
    pvpBuildInfo* m_buildInfo;
    CBaseBuilding* m_curBuild;//当前选中的塔
    Menu* m_btnMenu;
    int m_userId;//玩家id
    std::string m_name;//玩家名字
    int m_level;//玩家等级
    int m_stone;//服务器返回的增长后的用户魔法石总量
    std::string m_enemyMaps;
    ProgressTimer* m_progress;
    Vec2 m_startPos;
    float m_distance;//多点触摸时两个点之间的起始距离
    LayerColor* m_pLayer;
    Sprite* m_searchBg;
    
    LYMenuItemSprite* m_searchItem;
    LYMenuItemSprite* m_pFightItem;
    Sprite* m_pAllStone;
    
    LYMenuItemSprite* m_pBackItem;
};



#endif /* defined(__WarOfGod__pvpEnemyLayer__) */
