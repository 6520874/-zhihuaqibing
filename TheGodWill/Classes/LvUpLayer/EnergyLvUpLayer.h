//
//  EnergyLvUpLayer.h
//  TheGodWill
//
//  Created by prefer_shan on 14/11/25.
//
//

#ifndef __TheGodWill__EnergyLvUpLayer__
#define __TheGodWill__EnergyLvUpLayer__

#include <stdio.h>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "BaseLayer.h"
#include "AllDefine.h"

USING_NS_CC;
NS_CC_EXT_BEGIN

class CEnergyLvUpLayer:public CBaseLayer,public TableViewDelegate,public TableViewDataSource
{
  public:  
    CEnergyLvUpLayer();
    ~CEnergyLvUpLayer();

    
   CREATE_FUNC(CEnergyLvUpLayer);
    Menu* m_pGuideMenu;
private:
    virtual void tableCellTouched(TableView* table,TableViewCell* cell);
    
    virtual void tableCellWillRecycle(TableView* table, TableViewCell* cell);
    
    virtual void tableCellHighlight(TableView* table,TableViewCell* cell);
    
    virtual void tableCellUnhighlight(TableView* table,TableViewCell* cell);
    
    virtual Size cellSizeForTable(TableView* table);
    
    //生成cell
    virtual TableViewCell* tableCellAtIndex(TableView *table,ssize_t idx);
    
    //cell的数量
    virtual ssize_t numberOfCellsInTableView(TableView *table);
    
    void scrollBar(TableView* table){};
    
    virtual void scrollViewDidScroll(ScrollView* view){};
    
    virtual void scrollViewDidZoom(ScrollView* view){};
    
    bool init();
    
    void addTableView();
    
    void onExit();
    
    void OnMenuUpgrade(Ref *psender);
    
    void OnRemoveNotice();

    void ReoloadData();
    
    void OnRemoveEffect();
    
    void OnUpdateCoin(Ref *pSender);
    
    energyUpgradeInfo* GetEnergyLvUplayer(int iEnergyUpid,int iUpgradeLevel);
    
    Sprite *m_pUpEffect;
    
    std::vector<energyUpgradeInfo*>  m_pEnergyUpgradeInfo;
    std::vector<energyUpgradeInfo*> m_pShowEnergyUpgradeInfo;
    
 
    
    Size m_VisibleSize;
    long   m_iIdx;
    Size   m_tPic;
    Sprite * m_ppPic;
    int  m_iIndex;
    TableView *m_tableView;
};
NS_CC_EXT_END
#endif /* defined(__TheGodWill__EnergyLvUpLayer__) */
