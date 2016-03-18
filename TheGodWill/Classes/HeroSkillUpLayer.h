//
//  HeroSkillUpLayer.h
//  TheGodWill
//
//  Created by prefer_shan on 14/12/29.
//
//

#ifndef __TheGodWill__HeroSkillUpLayer__
#define __TheGodWill__HeroSkillUpLayer__

#include <stdio.h>
#include "cocos-ext.h"
#include "AllDefine.h"
#include "BaseLayer.h"

USING_NS_CC;
NS_CC_EXT_BEGIN

class CHeroSkillUpLayer:public CBaseLayer,public TableViewDelegate,public TableViewDataSource
{
public:
    CHeroSkillUpLayer(Sprite *pBg);
    ~CHeroSkillUpLayer();
    
    //CREATE_FUNC(CHeroSkillUpLayer);
    
    static CHeroSkillUpLayer* create(Sprite *pBg) \
    { \
        CHeroSkillUpLayer *pRet = new(std::nothrow) CHeroSkillUpLayer(pBg); \
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
    
    void  InitYanjiuNum();
    void showSkillDetail();
    TableView  *m_tableView;
    
private:
    bool init();
    int  GetSkillId(int iSkillId);

    
    virtual void tableCellTouched(TableView* table,TableViewCell* cell);
    
    virtual void tableCellWillRecycle(TableView* table, TableViewCell* cell);
    
    virtual void tableCellHighlight(TableView* table,TableViewCell* cell);
    
    virtual void tableCellUnhighlight(TableView* table,TableViewCell* cell);

    virtual cocos2d::Size cellSizeForTable(TableView* table);
    
    bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
    void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event) ;
    void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event);
    void onTouchCancelled(Touch *touch, Event *event);
    
    
    
    void addSkillDetail(int idx,int iTag);
    
    //生成cell
    virtual TableViewCell* tableCellAtIndex(TableView *table,ssize_t idx);
    
    //cell的数量
    virtual ssize_t numberOfCellsInTableView(TableView *table);
    
    void scrollBar(TableView* table){};
    
    virtual void scrollViewDidScroll(ScrollView* view){};
    
    virtual void scrollViewDidZoom(ScrollView* view){};
    
    void addTableView();
    
    void onExit();
    
    void OnMenuOpen(Ref *pSender);
    
    void OnSoldierLvup(Ref *pSender);
    
    void OnSoldierExit(Ref *pSender);
    
    void OnSoldierSKillUp(Ref *pSender);
    
    
    cocos2d::Size m_winSize;
    
    Sprite * m_pBg;
    
    std::string m_strMonsSkill1Id;
    std::string m_strMonsSkill2Id;
    std::string m_strMonsSkill3Id;
    
    int m_iAllSec1;
    int m_iAllSec2;
    int m_iAllSec3;
    
    int m_iYanjiuNum;
    
    Label *m_pYanjiuNum;
    Label *m_pLabelLvUpTime;
    Label *m_pLabelLvUpTime2;
    Label *m_pLabelLvUpTime3;
    
    int m_iCurIdx;
    int m_iIndex;
    
    bool m_bHero;
    
    int m_pLvUpTime;
    int m_iHour;
    int m_iSec;
    int m_iHour2;
    int m_iSec2;
    int m_iHour3;
    int m_iSec3;
    
    int m_iSkillInex;

    ProgressTimer* m_pProgress2;
    ProgressTimer* m_pProgress3;
    ProgressTimer * m_pProgress1;
    EventListenerTouchOneByOne * m_pListener;

    std::vector<FighterInfo*> m_pSoldierVec;
};
NS_CC_EXT_END

#endif /* defined(__TheGodWill__HeroSkillUpLayer__) */
