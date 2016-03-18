//
//  pvpWarReportLayer.h
//  WarOfGod
//
//  Created by user on 15/6/17.
//
//

#ifndef __WarOfGod__pvpWarReportLayer__
#define __WarOfGod__pvpWarReportLayer__

#include <stdio.h>
#include "BaseLayer.h"
#include "cocos-ext.h"
#include "pvpMainLayer.h"

USING_NS_CC_EXT;

enum
{
    WAR_BTN_EXIT = 100,
    WAR_BACK_PLAY,
    WAR_BTN_REVENAGE,
    WAR_ACQUIRE_AWARD,//获取奖励
    
};


class pvpWarReportLayer:public CBaseLayer,public TableViewDelegate,public TableViewDataSource
{
public:
    pvpWarReportLayer();
    ~pvpWarReportLayer();
    //static pvpWarReportLayer* create();
    CREATE_FUNC(pvpWarReportLayer);
    bool init();
    void showUI();
    bool onTouchBegan(Touch* pTouch, Event* pEvent);
    void onTouchMoved(Touch* pTouch, Event* pEvent);
    void onTouchEnded(Touch* pTouch, Event* pEvent);
    
    void getServerTime(Ref* pSender);//获取服务器时间
    void btnCallBack(Ref* pSender);
    std::string calculateTime(int seconds);//计算时间差
    void getVideoInfo(Ref* pSender);//获取指定录像
    void getEnemyTeamInfoBack(Ref *pSender);
    void updateUserData(Ref *pSender);
    virtual void tableCellTouched(TableView* table,TableViewCell* cell);
    
    virtual void tableCellWillRecycle(TableView* table, TableViewCell* cell){}
    
    virtual void tableCellHighlight(TableView* table,TableViewCell* cell);
    
    virtual void tableCellUnhighlight(TableView* table,TableViewCell* cell);
    
    virtual cocos2d::Size cellSizeForTable(TableView* table);
    
    //生成cell
    virtual TableViewCell* tableCellAtIndex(TableView *table,ssize_t idx);
    
    //cell的数量
    virtual ssize_t numberOfCellsInTableView(TableView *table);
    
    virtual void scrollViewDidScroll(ScrollView* view){};
    
    virtual void scrollViewDidZoom(ScrollView* view){};
    
    void updateTime(float dt);
    void acquireQward(Ref* pSender);
    void  Rebo();
    void  Enemy(Ref *pSedner);
    
    int m_iCurVideoId;
    
private:
    //Size m_winSize;
    EventListenerTouchOneByOne* m_listener;
    Sprite* m_spBg;//
    LayerColor* m_pLayer;
    std::vector<videoListInfo> m_allVideoInfo;//所有的录像信息
    TableView* m_tableView;
    int m_stone;//用户魔法石数量
    int m_idx;//当前领取奖励的录像索引
    pvpCommonInfoLayer* m_layer;//奖励界面
};

#endif /* defined(__WarOfGod__pvpWarReportLayer__) */
