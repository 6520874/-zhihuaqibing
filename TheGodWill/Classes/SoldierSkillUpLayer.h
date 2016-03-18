//
//  SoldierLvUpLayer.h
//  TheGodWill
//
//  Created by prefer_shan on 14/11/25.
//
//

#ifndef __TheGodWill__SoldierSkillUpLayer__
#define __TheGodWill__SoldierSkillUpLayer__

#include <stdio.h>
#include "BaseLayer.h"
#include "cocos-ext.h"
#include "AllDefine.h"
#include "guiderManagerLayer.h"
#define  SKILLMINUS 0
#define  SKILLSEC   10


USING_NS_CC;
NS_CC_EXT_BEGIN

//类描述: 英雄技能升级tableView

class CSoldierSkillUpLayer:public CBaseLayer,public TableViewDelegate,public TableViewDataSource
{
public:
    CSoldierSkillUpLayer(Sprite *pBg);
    ~CSoldierSkillUpLayer();

    //CREATE_FUNC(CSoldierSkillUpLayer);
    
    static CSoldierSkillUpLayer * create(Sprite *pBg) \
    { \
        CSoldierSkillUpLayer *pRet = new(std::nothrow) CSoldierSkillUpLayer(pBg); \
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
        } \
    }
    
     void InitYanjiuNum();
    void showSkillDetail();
    TableView  *m_tableView;
private:
    bool init();
    
    virtual void tableCellTouched(TableView* table,TableViewCell* cell);
    
    virtual void tableCellWillRecycle(TableView* table, TableViewCell* cell);
    
    virtual void tableCellHighlight(TableView* table,TableViewCell* cell);
    
    virtual void tableCellUnhighlight(TableView* table,TableViewCell* cell);
    
    bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
    void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event) ;
    void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event);
    void onTouchCancelled(Touch *touch, Event *event);
    
    virtual cocos2d::Size cellSizeForTable(TableView* table);
    
    //生成cell
    virtual TableViewCell* tableCellAtIndex(TableView *table,ssize_t idx);
    
    //cell的数量
    virtual ssize_t numberOfCellsInTableView(TableView *table);
    
    void scrollBar(TableView* table){};
    
    virtual void scrollViewDidScroll(ScrollView* view){};
    
    virtual void scrollViewDidZoom(ScrollView* view){};

    void addTableView();
    void addSkillDetail(int idx,int iTag);
    
    void OnExitLvLayer();
    
    void onExit();
    
    void OnMenuOpen(Ref *pSender);
    
    void OnSoldierLvup(Ref *pSender);
    
    void OnSoldierExit(Ref *pSender);
        
    
    void OnExitLvLayer(Ref *pSender);
    
    cocos2d::Size m_winSize;
    
    ssize_t m_iCurIdx;
    
    Label * m_pLvUpTime;
    
    Menu * m_pMenu;
    
    int m_iIndex;
    
    int m_iYanjiuNum;
    
    Label *m_pYanjiuNum;
    
    EventListenerTouchOneByOne * m_pListener;
    ProgressTimer* m_pProgress1;
    ProgressTimer* m_pProgress2;
    ProgressTimer* m_pProgress3;

    
    Size m_tBg;
    Sprite * m_pBg;
    
    Vec2   m_VnowLocation;
    
    std::vector<FighterInfo*> m_pSoldierVec;
    Menu* m_pGuideMenu;
    
    TableViewCell* pCell;
};
NS_CC_EXT_END
#endif /* defined(__TheGodWill__SoldierLvUpLayer__) */
