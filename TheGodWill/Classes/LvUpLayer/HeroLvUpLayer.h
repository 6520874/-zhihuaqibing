//
//  HeroLvUpLayer.h
//  WarOfGod
//
//  Created by prefer_shan on 15/3/18.
//
//

#ifndef __WarOfGod__HeroLvUpLayer__
#define __WarOfGod__HeroLvUpLayer__

#include <stdio.h>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "AllDefine.h"
#include "BaseLayer.h"

USING_NS_CC;
NS_CC_EXT_BEGIN

class CHeroLvUpLayer:public CBaseLayer,public TableViewDelegate,public TableViewDataSource
{
public:
    CHeroLvUpLayer();
    ~CHeroLvUpLayer();
    CREATE_FUNC(CHeroLvUpLayer);
    bool init();

    
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
    
    void addTableView();
       TableView *m_tableView;
private:
    
    void OnRemoveNotice();
    
    void OnMenuOpen(Ref *pSender);
    int m_iCurIdx;
 
    
    std::vector<FighterInfo*> m_pSoldierVec;
    
};

NS_CC_EXT_END
#endif /* defined(__WarOfGod__HeroLvUpLayer__) */
