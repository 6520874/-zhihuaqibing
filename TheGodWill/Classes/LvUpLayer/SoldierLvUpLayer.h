//
//  SoldierLvUpLayer.h
//  TheGodWill
//
//  Created by prefer_shan on 14/11/25.
//
//

#ifndef __TheGodWill__SoldierLvUpLayer__
#define __TheGodWill__SoldierLvUpLayer__

#include <stdio.h>
#include "cocos-ext.h"
#include "AllDefine.h"

USING_NS_CC;
NS_CC_EXT_BEGIN

class CSoldierLvUpLayer:public Layer,public TableViewDelegate,public TableViewDataSource
{
public:
    CSoldierLvUpLayer();
    ~CSoldierLvUpLayer();

    CREATE_FUNC(CSoldierLvUpLayer);
    
    TableView  *m_tableView;
    
private:
    bool init();

    void OnRemoveNotice();
    
    virtual void tableCellTouched(TableView* table,TableViewCell* cell);
    
    virtual void tableCellWillRecycle(TableView* table, TableViewCell* cell);
    
    virtual void tableCellHighlight(TableView* table,TableViewCell* cell);
    
    virtual void tableCellUnhighlight(TableView* table,TableViewCell* cell);
    
    
    bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
    
    void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event);
    
    void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event);
    
    void onTouchCancelled(Touch *touch, Event *event);
    
    virtual cocos2d::Size cellSizeForTable(TableView* table);
    
    void OnMenuQuestion(Ref *pSender);
    
    //生成cell
    virtual TableViewCell* tableCellAtIndex(TableView *table,ssize_t idx);
    
    //cell的数量
    virtual ssize_t numberOfCellsInTableView(TableView *table);
    
    void scrollBar(TableView* table){};
    
    virtual void scrollViewDidScroll(ScrollView* view){};
    
    virtual void scrollViewDidZoom(ScrollView* view){};

    void addTableView();
    
    void UpdateTime(float ft);
    
    void onExit();
    void OnMenuOpen(Ref *pSender);
    
    void OnSoldierLvup(Ref *pSender);
    
    void OnSoldierExit(Ref *pSender);
    
    void addSoliderGuide(Ref* pSender);//添加士兵引导
    
    cocos2d::Size m_winSize;
    
    ssize_t m_iCurIdx;
    
    Label * m_pLvUpTime;
     std::vector<FighterInfo*>  m_pAll;
    
    std::vector<FighterInfo*> m_pSoldierVec;
     EventListenerTouchOneByOne * m_pListener;
    
    int  m_iHour;
    int  m_iSec;
    Label  *m_pLabelLvUpTime;
    ProgressTimer * m_pProgress;
    Menu* m_pGuideMenu;
    
};
NS_CC_EXT_END
#endif /* defined(__TheGodWill__SoldierLvUpLayer__) */
