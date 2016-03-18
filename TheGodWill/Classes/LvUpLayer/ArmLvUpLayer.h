//
//  ArmLvUpLayer.h
//  TheGodWill
//
//  Created by prefer_shan on 14/12/17.
//
//

#ifndef __TheGodWill__ArmLvUpLayer__
#define __TheGodWill__ArmLvUpLayer__

#include <stdio.h>
#include "cocos2d.h"
#include "AllDefine.h"
#include "SoldierLvUpLayer.h"
#include "HeroLvUpLayer.h"
#include "BaseLayer.h"
USING_NS_CC;

//兵种升级
class CArmLvUpLayer:public CBaseLayer
{
public:
    static CArmLvUpLayer* create(FighterInfo *pInfo,int index) \
    { \
        CArmLvUpLayer *pRet = new CArmLvUpLayer(pInfo,index); \
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
    
private:
    CArmLvUpLayer(FighterInfo *pInfo,int index);
    ~CArmLvUpLayer();
    int  GetCostCoin(int ToLv);
    
    void OnRemoveNotice();
    void  addChildCont();
    bool init();
    bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
    void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event) ;
    void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event);
    void onTouchCancelled(Touch *touch, Event *event);
    void OnCloseMenu(Ref *pSender);
    void OnMinus(Ref *pSender);
    void OnPlus(Ref *pSender);
    void OnLvUp(Ref *pSender);
    
    void OnUpdateCoin(Ref *pSender);
    
    void soliderCallBack(Ref * sender, cocos2d::extension::Control::EventType controlEvent);
    
    void onExit();
    void addGuide();//添加引导
    void update(float dt);//控制士兵引导
    bool m_bFlag;//保证消息只发一次
    
    Menu * m_pMenuClose;
    Menu *m_pMenuMinus;
    Menu *m_pMenuPlus;
    Menu *m_pMenuLvUp;
    bool  m_bMenuLvUp;
    bool  m_bMenuCloSe;
    bool  m_bMenuMinus;
    bool  m_bMenuPlus;
    bool   m_bSoldier;
    
    bool m_bPlus;
    bool m_bMinus;
    cocos2d::extension::ControlSlider* m_solider;

    Label * m_pCostMoneyLabel;
    float  m_iCostMoney;
    int  m_iToLV;
    int  m_iCurValue;
    Sprite * m_pJindu;
    Label * m_pUpLvNum;
    
    Sprite *m_pLvArrow;
    
    Sprite * m_pCurPlace;
    EventListenerTouchOneByOne * m_pListener;
    
    Label *m_pAddAtk;
    Label *m_pAddHp;
    
    int m_iAddAtk;
    int m_iAddHp;
    
    int m_index;
    monsterInfo* m_pMonsterInfo;
    FighterInfo * m_pFighterInfo;
    Sprite *m_pBg;
    Size m_winSize;
    

    std::vector<FighterInfo*> m_pSoldierVec;
    
    Sprite* m_grayBg;//升级背景
    
};
#endif /* defined(__TheGodWill__ArmLvUpLayer__) */
