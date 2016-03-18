//
//  DrawCard.h
//  WarOfGod
//
//  Created by spf on 15/6/4.
//
//

#ifndef __WarOfGod__DrawCard__
#define __WarOfGod__DrawCard__

#include <stdio.h>
#include "BaseLayer.h"
#include "AllDefine.h"
#include "cocos2d.h"
#include "cocos-ext.h"
#include "LYFightData.h"
USING_NS_CC;

class CDrawCard:public CBaseLayer
{
public:
    CDrawCard();
    ~CDrawCard();
    CREATE_FUNC(CDrawCard);

    void AddCloseMenu();
    void OnCloseMenu(Ref *pSender);
    
    void InitUI();
    void StartInfo();
    void addGuideInfo();//开始高级抽卡引导
    
    DrawCardResultInfo* m_pDrawInfoCoin;
    DrawCardResultInfo* m_pDrawInfoStone;
private:
    bool init();
    
    void  OnGetCardInfo(Ref *pSender);
    void onExit();
    bool onTouchBegan(Touch *pTouch, Event *pEvent);
    void onTouchMoved(Touch *pTouch, Event *pEvent);
    void onTouchEnded(Touch *pTouch, Event *pEvent);
    void onTouchCancelled(Touch *Touch, Event *pEvent);
    void addAdvanceDrawCard();
    void addCommonDrawCard();
    void gotochongzhi(Ref* pSender);
    void onAtCommonTime(Ref *pSender);
    void onAdvanceTime(Ref *pSender);
    
    void OnDrawCardSucess();

    void RunActionCommon();
    void RunAtionAdvance();

    
    void UpdateCommon(float time);
    void UpdateAdvance(float time);
    
    void CallOnceCommon(Ref *pSender);
    void CallOnceAdvance(Ref *pSender);
    void  OnCallTenCommon(Ref *pSender);
    void  OnCallTenAdvance(Ref *pSender);
    
    void  OnSaveCard1(Ref *pSender);
    void  OnSaveCard2(Ref *pSender);
    
    void  OnSaveStone(Ref *pSender);
    void  onSaveCoin(Ref *pSender);
    
    int m_iTime1;
    int m_iTime2;
    
    int m_iMin1;
    int m_iSec1;
    int m_iMin2;
    int m_iSec2;
    int m_iHour2;
    
    
    Sprite * m_pText1;
    Sprite * m_pText2;
    
    EventListenerTouchOneByOne *m_pListener;
    
    Label *m_pFreeTime1;
    
    Label *m_pFreeTime11;//后免费
    
    Label *m_pFreeTime2;
    
    Label *m_pFreeTime22;//后免费

    Label * m_pCoinNum1;
    Label * m_pCoinNum2;
    
    Label * m_pStoneNum1;
    Label * m_pStoneNUm2;
    
    Sprite * m_pDrawCard1;
    Sprite * m_pDrawCard2;



    Sprite *m_pCoin;
    Sprite *m_pStone;
    
    Size  m_tMoneyBg;
    
    Size  m_tMoneyBg2;
    
    Menu *m_pDrawOneMenu1;
    Menu *m_pDrawOneMenu2;
    Menu *m_pDrawTenMenu1;
    Menu *m_pDrawTenMenu2;
    Menu * m_pMenuClose;
    
    MenuItemSprite *m_pMenuItemClose;
    MenuItemSprite *m_pChouIte1;
    MenuItemSprite *m_pChouIte2;
    
    
    bool m_bClose;
    bool m_bDrawCardOne1;
    bool m_bDrawCardOne2;
    bool m_bDrawCardTen1;
    bool m_bDrawCardTen2;
    int  m_iMagic;
    
    int m_iDrawOneAllNum;
    int m_iDrawTwoAllNum;
    int m_iDrawOneNum; //金币抽卡次数
    int m_iDrawTwoNum; //魔法石抽卡次数
    
    DrawCardInfo m_CardInfos;
    


    //add by zxh 2015-07-24 11:04:40
    Vec2 m_guidePos;//记录抽卡一次按钮的位置
};
#endif /* defined(__WarOfGod__DrawCard__) */
