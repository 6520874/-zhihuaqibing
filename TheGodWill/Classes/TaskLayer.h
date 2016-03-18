//
//  TaskLayer.h
//  TheGodWill
//
//  Created by prefer_shan on 14/12/15.
//
//

#ifndef __TheGodWill__TaskLayer__
#define __TheGodWill__TaskLayer__

#include <stdio.h>
//任务系统界面
#include "cocos2d.h"
#include "cocos-ext.h"
#include "PublicDefine.h"
#include "AllDefine.h"
#include "BaseLayer.h"

class CTaskGetSucess;

USING_NS_CC;
NS_CC_EXT_BEGIN

enum
{
    TASK_ICON = 200,
    TASK_NAME,
    TASK_NUM,
    TASK_PROGRESS,
    TASK_COIN,
    TASK_EXP,
    TASK_STONE,
    TASK_PROP,
};
enum
{
    TASK_DEBRIS = 500,
};

class CTaskLayer:public CBaseLayer,public TableViewDelegate,public TableViewDataSource
{
public:
    
    static CTaskLayer* create(int bMainLine) \
    { \
        CTaskLayer *pRet = new CTaskLayer(bMainLine); \
        if (pRet && pRet->init()) \
        { \
            pRet->autorelease(); \
            return pRet; \
        } \
        else \
        { \
            delete pRet; \
            pRet = NULL; \
            return NULL; \
   
        }
    }
    
private:
    CTaskLayer(bool bMainLine);
    ~CTaskLayer();
    bool init();
    
    
    void  OnRemoveEffect();
    
    virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
//    virtual void onTouchMoved(Touch *touch, Event *unused_event);
//    virtual void onTouchEnded(Touch *touch, Event *unused_event);
    
    virtual void tableCellTouched(TableView* table,TableViewCell* cell);
    
    virtual void tableCellWillRecycle(TableView* table, TableViewCell* cell);
    
    virtual void tableCellHighlight(TableView* table,TableViewCell* cell);
    
    virtual void tableCellUnhighlight(TableView* table,TableViewCell* cell);
    
    virtual cocos2d::Size cellSizeForTable(TableView* table);
    
    //生成cell
    virtual TableViewCell* tableCellAtIndex(TableView *table,ssize_t idx);
    
    //cell的数量
    virtual ssize_t numberOfCellsInTableView(TableView *table);
    
    void scrollBar(TableView* table){};
    
    virtual void scrollViewDidScroll(ScrollView* view){};
    
    virtual void scrollViewDidZoom(ScrollView* view){};
    
    void OnUpOrGetAward(Ref *pSender);
    
    void  addTableView();
    
    void OnExitLvLayer(Ref *psender);
    
    void OnCloseAwardLayer();
    
    void OnUpdateCoinAndStone(Ref *pSender);
    
    void onExit();
    
    void TaskSort();//任务排序
    void getTask();//筛选出任务
    void getUserTeamInfo(Ref* pSender);
    
    void addCloseGuide(Ref* pSender);//添加关闭界面引导
    
    
    cocos2d::Size m_winSize;
    TableView *m_tableView;
    CTaskGetSucess * m_pTaskGetLayer;
    bool m_bMainLayer;
    Scale9Sprite *m_pMainTaks;
    ssize_t m_iCurIdx;
    std::vector<TaskBase *> m_pTaskInfoVec;//任务信息
    Menu *m_pMenuCose;
    
     std::vector<TableViewCell*>  m_pTableViewVec;
    
    EventListenerTouchOneByOne* listen;
    Menu* m_GuidepMenu;
    bool m_bValid;//标识按钮是否可点击
};

NS_CC_EXT_END
#endif /* defined(__TheGodWill__TaskLayer__) */
