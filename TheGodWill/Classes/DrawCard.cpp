//
//  DrawCard.cpp
//  WarOfGod
//
//  Created by prefer_shan on 15/6/4.
//
//

#include "DrawCard.h"
#include "DBMgr.h"
#include "sysTime.h"
#include "DrawCardSucess.h"
#include "commonPropLayer.h"
#include "NetWorkDataSingle.h"
#include "CHitDialog.h"
#include "MainMenuLayer.h"
#include "guiderManagerLayer.h"
#include "zzbNetControlModule.h"
#include "CSharedPlist.h"


int g_iTime[5] = {60,120,60*3,60*4,60*5};

CDrawCard::CDrawCard()
{
    m_pListener = NULL;
    m_bClose = false;
 
    m_bDrawCardOne1 =false;
    m_bDrawCardOne2 = false;
    m_bDrawCardTen1 = false;
    m_bDrawCardTen2 = false;
    
    m_pMenuClose = NULL;
    m_pDrawCard2 = NULL;
    m_pDrawCard1 = NULL;
    m_pDrawOneMenu2 = NULL;
    m_pDrawOneMenu1 = NULL;
    m_pDrawTenMenu1 = NULL;
    m_pDrawTenMenu2 = NULL;
    m_pFreeTime1 = NULL;
    m_pFreeTime2 = NULL;
    
    m_pFreeTime11 = NULL;
    m_pFreeTime22 = NULL;
    
    m_pCoin = NULL;
    m_pStone = NULL;
  
    CSharedPlist::getInstance()->pushPlist("plist/drawcard.plist");
    CSharedPlist::getInstance()->pushPlist("plist/soliderUp.plist");
    m_pMenuItemClose = NULL;
    
    m_iTime1 = 0;
    m_iTime2 = 0;
    m_iMagic = 0;
    m_pDrawInfoCoin= NULL;
    m_pDrawInfoStone = NULL;
}

CDrawCard::~CDrawCard()
{
    CSharedPlist::getInstance()->erasePlist("plist/drawcard.plist");

}

void CDrawCard::onExit()
{

    CMainMenuLayer *pLayer = (CMainMenuLayer*)getParent();
    
    if(pLayer)
    {
        pLayer->m_bSkillStart = true;
        
        {
             CPersonalData::getInstance()->setUpdateDrawCardInfo(true);
            
            //pLayer->updateUsetInfo();
        }
    }
    
    CBaseLayer::onExit();
    _eventDispatcher->removeEventListener(m_pListener);
}



bool CDrawCard::onTouchBegan(Touch *pTouch, Event *pEvent)
{
    
    if(m_pMenuClose)
    {
        m_bClose = m_pMenuClose->onTouchBegan(pTouch,pEvent);
    }
    if(m_pDrawOneMenu1)
    {
        m_bDrawCardOne1 = m_pDrawOneMenu1->onTouchBegan(pTouch,pEvent);
    }
    
    if(m_pDrawOneMenu2)
    {
        m_bDrawCardOne2 = m_pDrawOneMenu2->onTouchBegan(pTouch,pEvent);
    }

    if(m_pDrawTenMenu1)
    {
        m_bDrawCardTen1 = m_pDrawTenMenu1->onTouchBegan(pTouch,pEvent);
    }

    if(m_pDrawTenMenu2)
    {
        m_bDrawCardTen2 = m_pDrawTenMenu2->onTouchBegan(pTouch,pEvent);
    }
    
    
    return true;
}


void CDrawCard::onTouchMoved(Touch *pTouch, Event *pEvent)
{
    if(m_bClose)
    {
        m_pMenuClose->onTouchMoved(pTouch,pEvent);
    }
    else if(m_bDrawCardOne1)
    {
        m_pDrawOneMenu1->onTouchMoved(pTouch,pEvent);
    }
    else if(m_bDrawCardOne2)
    {
        m_pDrawOneMenu2->onTouchMoved(pTouch,pEvent);
    }
    else if(m_bDrawCardTen1)
    {
        m_pDrawTenMenu1->onTouchMoved(pTouch,pEvent);
    }
    else if(m_bDrawCardTen2)
    {
        m_pDrawTenMenu2->onTouchMoved(pTouch,pEvent);
    }
}

void CDrawCard::onTouchEnded(Touch *pTouch, Event *pEvent)
{
    if(m_bClose)
    {
        m_pMenuClose->onTouchEnded(pTouch,pEvent);
        m_bClose = false;
    }
    else if(m_bDrawCardOne1)
    {
        m_pDrawOneMenu1->onTouchEnded(pTouch,pEvent);
        m_bDrawCardOne1 = false;
    }
    else if(m_bDrawCardOne2)
    {
        m_pDrawOneMenu2->onTouchEnded(pTouch,pEvent);
        m_bDrawCardOne2 = false;

    }
    else if(m_bDrawCardTen1)
    {
        m_pDrawTenMenu1->onTouchEnded(pTouch,pEvent);
        m_bDrawCardTen1 = false;
    }
    else if(m_bDrawCardTen2)
    {
        m_pDrawTenMenu2->onTouchEnded(pTouch,pEvent);
        m_bDrawCardTen2 = false;
    
    }

}

void CDrawCard::onTouchCancelled(Touch *pTouch, Event *pEvent)
{
    
    if(m_bClose)
    {
        m_pMenuClose->onTouchCancelled(pTouch,pEvent);
        m_bClose = false;
    }
    
    else if(m_bDrawCardOne1)
    {
        m_pDrawOneMenu1->onTouchCancelled(pTouch,pEvent);
        m_bDrawCardOne1 = false;
    }
    else if(m_bDrawCardOne2)
    {
        m_pDrawOneMenu2->onTouchCancelled(pTouch,pEvent);
        m_bDrawCardOne2 = false;
        
    }
    else if(m_bDrawCardTen1)
    {
        m_pDrawTenMenu1->onTouchCancelled(pTouch,pEvent);
        m_bDrawCardTen1 = false;
    }
    else if(m_bDrawCardTen2)
    {
        m_pDrawTenMenu2->onTouchCancelled(pTouch,pEvent);
        m_bDrawCardTen2 = false;
    }
    
}


void CDrawCard::OnCloseMenu(Ref *pSender)
{
    SimpleAudioEngine::getInstance()->playEffect("music/click_btn.mp3");
    //NotificationCenter::getInstance()->postNotification("drawCardAfter");
    ((CMainMenuLayer*)getParent())->showDrawCardResult();
    removeFromParent();
}


void CDrawCard::AddCloseMenu()
{
    
    Sprite *pCloseNormal = Sprite::createWithSpriteFrameName("closeBtn.png");
    Sprite *pCloseSelect = Sprite::createWithSpriteFrameName("closeBtn.png");
    pCloseSelect->setColor(Color3B::GRAY);
    
    m_pMenuItemClose = LYMenuItemSprite::create(pCloseNormal,pCloseSelect,CC_CALLBACK_1(CDrawCard::OnCloseMenu, this));
    
    m_pMenuItemClose->setPosition(m_winSize.width*0.05,m_winSize.height*0.9);

    
    m_pMenuClose = Menu::create(m_pMenuItemClose,NULL);
    m_pMenuClose->setPosition(Vec2(0,0));
    addChild(m_pMenuClose);
    
    
}


void CDrawCard::OnDrawCardSucess()
{
    addChild(DrawCardSucess::create(m_iMagic,this));
}


void CDrawCard::OnSaveCard1(Ref *pSender)
{
    
    RunActionCommon();
    if(m_CardInfos.m_iCoindrawNum<=0)
    {
        
    }
    else
    {
        schedule(schedule_selector(CDrawCard::UpdateCommon),1);
    }
}

void CDrawCard::OnSaveCard2(Ref *pSender)
{
    
    
    
    RunAtionAdvance();
    schedule(schedule_selector(CDrawCard::UpdateAdvance),1);
}


void CDrawCard::onSaveCoin(Ref *pSender)
{
    
    addCommonDrawCard();
    
}


void CDrawCard::OnSaveStone(Ref *pSender)
{
    
     m_CardInfos.m_istoneDrawNum  =1;
    
    m_CardInfos.m_iStonedrawtime  = 0;
    
    addAdvanceDrawCard();
    CNetWorkDataSingle::GetSingletonPtr()->GetDrawCardInfo(this,NULL);
}



void CDrawCard::CallOnceCommon(Ref *pSender)
{
    log("onCallCommon");
    
    
    SimpleAudioEngine::getInstance()->playEffect("music/click_btn.mp3");
    string str =  m_pFreeTime1->getString();

     int  iPos = str.find("免费次数");
    
    

    if(guiderManagerLayer::getInstance()->getGuideType() == GUIDE_COMMON_CARD)
    {
        guiderManagerLayer::getInstance()->removeGuiderLayer();
    }
    
    
    if(m_iDrawOneNum>=1&&iPos>=0)
    {
        //本次免费
        m_iMagic = 0;
        
        char strTime[50] = {"curTime"};
        UserDefault::getInstance()->setIntegerForKey(strTime,m_iTime1);
        UserDefault::getInstance()->flush();

        m_pDrawOneMenu1->setEnabled(false);
        m_pDrawOneMenu2->setEnabled(false);
        m_pDrawTenMenu1->setEnabled(false);
        m_pDrawTenMenu2->setEnabled(false);
        m_pMenuItemClose->setVisible(false);
        

        CNetWorkDataSingle::GetSingletonPtr()->SaveDrawCardInfo(this,menu_selector(CDrawCard::OnSaveCard1),0,--m_iDrawOneNum);
        
        m_CardInfos.m_iCoindrawNum = m_iDrawOneNum;
        
    }
    else
    {
        //需要的钱
        int iCoin = CPersonalData::getInstance()->getUserCoin();
    
        std::vector<ConfigureInfo*>  pInfo = CDBMgr::getInstance()->GetConfigureInfo();
    
        int iNeeCoin = pInfo[0]->m_iDrawOneCoin;
        
        
        if(iCoin>=iNeeCoin)
        {
            m_iMagic = 0;
            m_pDrawOneMenu1->setEnabled(false);
            m_pDrawOneMenu2->setEnabled(false);
            m_pDrawTenMenu1->setEnabled(false);
            m_pDrawTenMenu2->setEnabled(false);
            m_pMenuItemClose->setVisible(false);
            
            RunActionCommon();
            CNetWorkDataSingle::GetSingletonPtr()->SaveUserMCF(NULL,NULL,-iNeeCoin,0,0);
        }
        else
        {
            //金币不够到魔法石兑换金币界面
            CommonPropLayer* pLayer = CommonPropLayer::create(STONE_EXCHANGE_COIN);
            addChild(pLayer, 999);
        
        }
        
    }
}



void CDrawCard::CallOnceAdvance(Ref *pSender)
{
    SimpleAudioEngine::getInstance()->playEffect("music/click_btn.mp3");
    string str =  m_pFreeTime2->getString();
    
 
    
    int iPos = str.find("免费次数");
    
    if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SENIOR_CARD))
    {
        guiderManagerLayer::getInstance()->removeGuiderLayer();
    }
    
    if(m_iDrawTwoNum>=1&& iPos>=0)
    {
        m_iMagic = 2;
        char strTime[50] = {"curTime2"};
        UserDefault::getInstance()->setIntegerForKey(strTime,m_iTime2);
        UserDefault::getInstance()->flush();
        m_pDrawOneMenu1->setEnabled(false);
        m_pDrawOneMenu2->setEnabled(false);
        m_pDrawTenMenu1->setEnabled(false);
        m_pDrawTenMenu2->setEnabled(false);
        m_pMenuItemClose->setVisible(false);
        

         CNetWorkDataSingle::GetSingletonPtr()->SaveDrawCardInfo(this,menu_selector(CDrawCard::OnSaveCard2),1,0);
    }
    else
    {
        int iStone = CPersonalData::getInstance()->getUserStone();
        
        std::vector<ConfigureInfo*>  pInfo = CDBMgr::getInstance()->GetConfigureInfo();

        int iNeedStone = pInfo[0]->m_iDrawOneStone;
        
        if(iStone>=iNeedStone)
        {
            m_iMagic = 2;
    
            m_pDrawOneMenu1->setEnabled(false);
            m_pDrawOneMenu2->setEnabled(false);
            m_pDrawTenMenu1->setEnabled(false);
            m_pDrawTenMenu2->setEnabled(false);
            m_pMenuItemClose->setVisible(false);
    
            RunAtionAdvance();
        
            CNetWorkDataSingle::GetSingletonPtr()->SaveUserMCF(NULL,NULL,0,-iNeedStone,0);
            TDCCItem::onPurchase("一次抽卡", 1, iNeedStone);
            
        }
        else
        {
            CHitDialog *pDlg = CHitDialog::create();
            pDlg->createYesNO(this, menu_selector(CDrawCard::gotochongzhi),"魔法石不足，是否去充值？");
            addChild(pDlg);
        }
    }
    
}


void CDrawCard::gotochongzhi(Ref* pSender)
{
    CLayerManager::GetSingletonPtr()->createLayer(eLayer_Charge,this,false);

}


void CDrawCard::onAtCommonTime(Ref *pSender)
{
    
    char str[50] = {0};
    sprintf(str,"免费次数:%d/%d",m_iDrawOneNum,m_iDrawOneAllNum);
    m_pFreeTime1->setString(str);
    
    
    Size  tBg = m_pDrawCard1->getContentSize();
    
    m_pFreeTime1->setPositionX(tBg.width*0.5);
    m_pFreeTime11->setVisible(false);
    
    if(m_iDrawOneNum<=0)
    {
        m_pFreeTime1->setVisible(false);
    }
    
    std::vector<ConfigureInfo*>  pInfo = CDBMgr::getInstance()->GetConfigureInfo();
    
    if(m_iDrawOneNum==5)
    {
        m_iTime1 = g_iTime[0];
        
    }else if(m_iDrawOneNum ==4)
    {
        m_iTime1 = g_iTime[1];
    }
    else if(m_iDrawOneNum == 3)
    {
        m_iTime1 = g_iTime[2];
    }
    else if(m_iDrawOneNum == 2)
    {
        m_iTime1 = g_iTime[3];
    }
    else if(m_iDrawOneNum ==1)
    {
        m_iTime1 = g_iTime[4];
    }
    
    m_pFreeTime11->setVisible(false);
    
    char strTime[50] = {"curTime"};
    UserDefault::getInstance()->setIntegerForKey(strTime,0);
    UserDefault::getInstance()->flush();
    
    CNetWorkDataSingle::GetSingletonPtr()->GetDrawCardInfo(this,NULL);
    
    
    unschedule(schedule_selector(CDrawCard::UpdateCommon));
    
    addCommonDrawCard();
}

void CDrawCard::onAdvanceTime(Ref *pSender)
{
   

}


void CDrawCard::UpdateCommon(float time)
{
    m_iTime1 -=1;
    
    m_iMin1  = m_iTime1/60;

    m_iSec1 = m_iTime1%60;
    
    if(m_iMin1==0&&m_iSec1==0)
    {
        CNetWorkDataSingle::GetSingletonPtr()->GetDrawCardInfo(this,menu_selector(CDrawCard::onAtCommonTime));
        return;
    }
    char str[20] = {0};
    
    sprintf(str,"%02d:%02d",m_iMin1,m_iSec1);
    Size tBg = m_pDrawCard1->getContentSize();
    
    m_pFreeTime1->setPositionX(tBg.width*0.4);
    
    m_pFreeTime1->setString(str);
    m_pFreeTime11->setPositionX(m_pFreeTime1->getPositionX()+m_pFreeTime1->getContentSize().width*0.5+m_pFreeTime11->getContentSize().width*0.5);
    m_pFreeTime11->setVisible(true);
}


void CDrawCard::UpdateAdvance(float time)
{
    m_iTime2 -=1;
    
    m_iHour2  = m_iTime2/3600;
    m_iMin2 = m_iTime2%3600/60;
    m_iSec2 = m_iTime2%60;
    
    Size tBg = m_pDrawCard2->getContentSize();
    
    if(m_iMin2==0&&m_iSec2==0)
    {
       
        char str[50] = {0};
        
        m_iDrawTwoNum++;
        
        sprintf(str,"免费次数:%d/%d",m_iDrawTwoNum,m_iDrawTwoAllNum);
        
        m_pFreeTime2->setString(str);
        
        m_pFreeTime22->setVisible(false);
        
        
        CNetWorkDataSingle::GetSingletonPtr()->SaveDrawCardInfo(this,menu_selector(CDrawCard::OnSaveStone),1,1);
        
        m_pFreeTime2->setPositionX(tBg.width*0.5);
        
        unschedule(schedule_selector(CDrawCard::UpdateAdvance));
        
        std::vector<ConfigureInfo*>  pInfo = CDBMgr::getInstance()->GetConfigureInfo();
        
        m_iTime2 = pInfo[0]->m_iDrawTenTime;
        
       // m_iTime2 = 10;
        
        char strTime2[50] = {"curTime2"};
        UserDefault::getInstance()->setIntegerForKey(strTime2,0);
        UserDefault::getInstance()->flush();
        return;
        
    }
    char str[20] = {0};
    
    if(m_iHour2<=0)
    {
        sprintf(str,"%02d:%02d",m_iMin2,m_iSec2);
    }
    else
    {
        sprintf(str,"%02d:%02d:%02d",m_iHour2,m_iMin2,m_iSec2);
    }
    
    m_pFreeTime2->setString(str);

    m_pFreeTime2->setPositionX(tBg.width*0.41);
        m_pFreeTime22->setPositionX(m_pFreeTime2->getPositionX()+m_pFreeTime2->getContentSize().width*0.5+m_pFreeTime22->getContentSize().width*0.5);
    m_pFreeTime22->setVisible(true);
   
}



void CDrawCard::RunActionCommon()
{

    
    
    float fRandom = random(0.01,0.9);
    
    float fNowTime = 0.0f;
    
    std::vector<DrawCardResultInfo*> pInfo  = CDBMgr::getInstance()->GetDrawCardResultInfo();
    
    
    std::vector<DrawCardResultInfo*> pCoinInfo;
    
    for(int i=0;i<pInfo.size();i++)
    {
        if(pInfo[i]->m_bCoin)
        {
            pCoinInfo.push_back(pInfo[i]);
        }
        else
        {
            //pStoneInfo.push_back(pInfo[i]);
        }
    }
    
    
    for(int i=0;i<pCoinInfo.size();i++)
    {
        float f = atof(pCoinInfo[i]->m_sRadio.c_str());
        fNowTime+= f;
        
        if(fNowTime>=fRandom)
        {
            m_pDrawInfoCoin = pCoinInfo[i];
            break;
        }
    }
    
    
    //道具直接加入
    if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_COMMON_CARD) && (guiderManagerLayer::getInstance()->getStepNum() == 3))
    {
        // pDebrisIcon = CDataManager::getMonsterInfodebris(9);
        CDBMgr::getInstance()->saveUserDebrisToDB(9,1);
    }
    else
    {
        if(m_CardInfos.m_bfirstDrawCoin)
        {
            m_pDrawInfoCoin->m_iType = 0;
            
        }
        if(m_pDrawInfoCoin->m_iType ==0) //碎片
        {
            int id = random(1,13);
            CDBMgr::getInstance()->saveUserDebrisToDB(id,1);
            
        }
        else if(m_pDrawInfoCoin->m_iType ==1) //道具
        {
            int iNum = CDBMgr::getInstance()->HavePropNumById(m_pDrawInfoCoin->m_iGotItem);
            CDataManager::SaveUserPropToDB(m_pDrawInfoCoin->m_iGotItem,iNum+1);
        }
    }
    
    
    
 
    
    m_pDrawCard1->setCascadeOpacityEnabled(true);
    
    CallFunc* guide = CallFunc::create(CC_CALLBACK_0(CDrawCard::OnDrawCardSucess, this));
    
    m_pDrawCard2->runAction(Sequence::create(MoveTo::create(0.5f,Vec2(m_winSize.width*1.5f,m_winSize.height*0.55f)),NULL));
    
    m_pDrawCard1->runAction(Sequence::create(MoveTo::create(0.5f,Vec2(m_winSize.width*0.5f,m_winSize.height*0.55f)),DelayTime::create(0.5f),Spawn::create(ScaleTo::create(0.5, 4),FadeOut::create(0.5),NULL),guide,NULL));

  
}

void CDrawCard::RunAtionAdvance()
{
    
    std::vector<DrawCardResultInfo*> pInfo  = CDBMgr::getInstance()->GetDrawCardResultInfo();
    
    
    std::vector<DrawCardResultInfo*> pStoneInfo;
    if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SENIOR_CARD) && (guiderManagerLayer::getInstance()->getStepNum() == 3))
    {
        CDataManager::saveGuideInfoToDB(GUIDE_SENIOR_CARD);
    }

    
    for(int i=0;i<pInfo.size();i++)
    {
        if(pInfo[i]->m_bCoin)
        {
            //pCoinInfo.push_back(pInfo[i]);
        }
        else
        {
            pStoneInfo.push_back(pInfo[i]);
        }
    }
    
    int iRandom = random(1,100);
    
    DrawCardResultInfo*  pDrawInfo = NULL;
    
    
    if(iRandom<=40)
    {
        pDrawInfo = pStoneInfo[0];
    }
    else if (iRandom>40&&iRandom<=70)
    {
        pDrawInfo = pStoneInfo[1];
    }
    else if(iRandom>70&&iRandom<=90)
    {
        pDrawInfo = pStoneInfo[2];
    }
    else if(iRandom>90&&iRandom<=95)
    {
        pDrawInfo = pStoneInfo[3];
    }
    else if(iRandom>95&&iRandom<=98)
    {
        pDrawInfo = pStoneInfo[4];
    }
    else if(iRandom==99)
    {
        pDrawInfo = pStoneInfo[5];
    }
    else if(iRandom == 100)
    {
        pDrawInfo = pStoneInfo[6];
    }
    else
    {
        pDrawInfo = pStoneInfo[0];
    }
    
    
    
    int iNum = pDrawInfo->m_iGotNum;
    
    
    int id = random(1,13);
    
    if(m_CardInfos.m_bfirstDrawStone)
    {
        iNum = 10;
    }
    
    
    //获取怪物的个数
    
    if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SENIOR_CARD) && (guiderManagerLayer::getInstance()->getStepNum() == 3))
    {
        id = 12;
        iNum = 10;
    }
     CDBMgr::getInstance()->saveUserDebrisToDB(id,iNum);
    m_pDrawInfoStone = pDrawInfo;
    
    
    m_pDrawOneMenu1->setEnabled(false);
    
    m_pDrawOneMenu2->setEnabled(false);
    m_pDrawTenMenu1->setEnabled(false);
    m_pDrawTenMenu2->setEnabled(false);
    
    CallFunc* guide = CallFunc::create(CC_CALLBACK_0(CDrawCard::OnDrawCardSucess, this));
    
    m_pDrawCard1->runAction(Sequence::create(MoveTo::create
                                             (0.5f,Vec2(-m_winSize.width*0.5f,m_winSize.height*0.55f)),NULL));
    m_pDrawCard2->setCascadeOpacityEnabled(true);
    
    m_pDrawCard2->runAction(Sequence::create(MoveTo::create(0.5f,Vec2(m_winSize.width*0.5f,m_winSize.height*0.55f)),DelayTime::create(0.5f),Spawn::create(ScaleTo::create(0.5, 8),FadeOut::create(0.5),NULL),guide,NULL));

    m_pMenuItemClose->runAction(Sequence::create(MoveTo::create(0.5f,Vec2(-m_winSize.width*0.5f,m_winSize.height*0.9f)), NULL));
}


void CDrawCard::OnCallTenCommon(Ref *pSender)
{
   
    //需要的钱
    
  
     SimpleAudioEngine::getInstance()->playEffect("music/click_btn.mp3");
    int iCoin = CPersonalData::getInstance()->getUserCoin();
    
    std::vector<ConfigureInfo*>  pInfo = CDBMgr::getInstance()->GetConfigureInfo();
    
    int iNeeCoin = pInfo[0]->m_iDrawTenCoin;
    if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_COMMON_CARD))
    {
        guiderManagerLayer::getInstance()->removeGuiderLayer();
    }
    if(iCoin>=iNeeCoin)
    {
        
        m_iMagic = 1;

        RunActionCommon();
        m_pDrawOneMenu1->setEnabled(false);
        m_pDrawOneMenu2->setEnabled(false);
        m_pDrawTenMenu1->setEnabled(false);
        m_pDrawTenMenu2->setEnabled(false);
        m_pMenuItemClose->setVisible(false);
        
    
        CNetWorkDataSingle::GetSingletonPtr()->SaveUserMCF(NULL,NULL,-iNeeCoin,0,0);
    }
    else
    {
        //金币不够到魔法石兑换金币界面
        CommonPropLayer* pLayer = CommonPropLayer::create(STONE_EXCHANGE_COIN);
        addChild(pLayer, 999);
        
    }
}


void CDrawCard::OnCallTenAdvance(Ref *pSender)
{
    
 
    
    SimpleAudioEngine::getInstance()->playEffect("music/click_btn.mp3");
    int iStone = CPersonalData::getInstance()->getUserStone();

    m_iMagic = 3;

    std::vector<ConfigureInfo*>  pInfo = CDBMgr::getInstance()->GetConfigureInfo();
    
    int iNeedStone = pInfo[0]->m_iDrawTenStone;
    if(guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SENIOR_CARD)
    {
        guiderManagerLayer::getInstance()->removeGuiderLayer();
    }
    if(iStone>=iNeedStone)
    {

        CNetWorkDataSingle::GetSingletonPtr()->SaveUserMCF(NULL,NULL,0,-iNeedStone,0);
        TDCCItem::onPurchase("十连抽", 1, iNeedStone);

        m_pDrawOneMenu1->setEnabled(false);
        m_pDrawOneMenu2->setEnabled(false);
        m_pDrawTenMenu1->setEnabled(false);
        m_pDrawTenMenu2->setEnabled(false);
        m_pMenuItemClose->setVisible(false);
        
        RunAtionAdvance();
        
    }
    else
    {
        CHitDialog *pDlg = CHitDialog::create();
        pDlg->createYesNO(this, menu_selector(CDrawCard::gotochongzhi),"魔法石不足，是否去充值？");
        addChild(pDlg);
    }
}



void CDrawCard::InitUI()
{
    removeAllChildren();


    AddCloseMenu();
    
    //高级抽卡
    
    m_pDrawCard1 = Sprite::createWithSpriteFrameName("drawcomon.png");
    m_pDrawCard1->setPosition(m_winSize.width*0.25,m_winSize.height*0.55);
    addChild(m_pDrawCard1);
    
    std::vector<ConfigureInfo*>  pInfo = CDBMgr::getInstance()->GetConfigureInfo();
    
    m_iDrawOneNum =m_CardInfos.m_iCoindrawNum;
    
    if(m_iDrawOneNum==5)
    {
        m_iTime1 = g_iTime[0];
        
    }else if(m_iDrawOneNum ==4)
    {
        m_iTime1 = g_iTime[1];
    }
    else if(m_iDrawOneNum == 3)
    {
        m_iTime1 = g_iTime[2];
    }
    else if(m_iDrawOneNum == 2)
    {
        m_iTime1 = g_iTime[3];
    }
    else if(m_iDrawOneNum ==1)
    {
        m_iTime1 = g_iTime[4];
    }

    //m_iTime1 = 10;

    m_iTime2 =  pInfo[0]->m_iDrawTenTime;
    
    //m_iTime2 = 10;
    
    m_iDrawOneAllNum = pInfo[0]->m_iDrawOneNum;
    
    m_iDrawTwoAllNum = pInfo[0]->m_iDrawMagicNum;

    m_pDrawCard2 = Sprite::createWithSpriteFrameName("drawadvance.png");
    m_pDrawCard2->setPosition(m_winSize.width*0.75,m_winSize.height*0.55);
    addChild(m_pDrawCard2);

    addAdvanceDrawCard();
    addCommonDrawCard();
    if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_COMMON_CARD) && (guiderManagerLayer::getInstance()->getStepNum() == 2))
    {
        m_guidePos = m_pChouIte1->convertToWorldSpaceAR(Vec2::ZERO);
        guiderManagerLayer::getInstance()->setStepNum(guiderManagerLayer::getInstance()->getStepNum() + 1);
        guiderManagerLayer::getInstance()->createLayer(this, GUIDE_SHOW_FINGER, m_guidePos);
    }
}



void  CDrawCard::OnGetCardInfo(Ref *pSender)
{
    m_CardInfos = CFightData::getInstance()->getDrawCardInfo();
    

  /* long lastTime =  m_CardInfos.m_iCoindrawtime;
    
    if(lastTime == 0)
    {
        lastTime =  m_CardInfos.m_iServerTime;
        
    }
    CGameTime::my_time_t timeLast = CGameTime::convertTimeToMyTimeformat(lastTime);
    CGameTime::my_time_t timeNow = CGameTime::convertTimeToMyTimeformat(m_CardInfos.m_iServerTime);
    
    if((lastTime == 0)|| (timeNow.year > timeLast.year) || ((timeNow.year == timeLast.year) && (timeNow.month > timeLast.month))
       ||((timeNow.year == timeLast.year) && (timeNow.month == timeLast.month) && (timeNow.day > timeLast.day)
          && (timeNow.hour >= 3)))
    {
        
        m_CardInfos.m_iCoindrawNum = 5;
        CNetWorkDataSingle::GetSingletonPtr()->SaveDrawCardInfo(this,NULL,0,5);
        
    }*/
    
    
    InitUI();
    
}

void CDrawCard::addGuideInfo()
{
    //引导抽卡
    if((CDataManager::getEndedBy(GUIDE_COMMON_CARD)) && (!CDataManager::getEndedBy(GUIDE_SENIOR_CARD))
       && (guiderManagerLayer::getInstance()->getStepNum() == 4))
    {
        guiderManagerLayer::getInstance()->setGuideType(GUIDE_SENIOR_CARD);
        guiderManagerLayer::getInstance()->setStepNum(1);
        guiderManagerLayer::getInstance()->createLayer(this);
    }
    else if(guiderManagerLayer::getInstance()->getStepNum() == 2)
    {
        m_guidePos = m_pChouIte2->convertToWorldSpaceAR(Vec2::ZERO);
        guiderManagerLayer::getInstance()->setStepNum(guiderManagerLayer::getInstance()->getStepNum() + 1);
        guiderManagerLayer::getInstance()->createLayer(this, GUIDE_SHOW_FINGER, m_guidePos);
    }
    else if(guiderManagerLayer::getInstance()->getStepNum() == 4)
    {
        m_guidePos = m_pMenuItemClose->convertToWorldSpaceAR(Vec2::ZERO);
        guiderManagerLayer::getInstance()->setStepNum(guiderManagerLayer::getInstance()->getStepNum() + 1);
        guiderManagerLayer::getInstance()->createLayer(this, GUIDE_SHOW_FINGER, m_guidePos);
    }
}

bool CDrawCard::init()
{
    CBaseLayer::initWithColor(Color4B(0,0,0, 255*0.9));
    
    m_pListener = EventListenerTouchOneByOne::create();//创建一个触摸监听
    m_pListener->setSwallowTouches(true);//设置不想向下传递触摸  true是不想 默认为false
    m_pListener->onTouchBegan = CC_CALLBACK_2(CDrawCard::onTouchBegan, this);
    m_pListener->onTouchMoved = CC_CALLBACK_2(CDrawCard::onTouchMoved, this);
    m_pListener->onTouchEnded = CC_CALLBACK_2(CDrawCard::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(m_pListener, this);
    
     //   InitUI();
    
     InitUI();
    //StartInfo();
    
     return true;
}


void CDrawCard::StartInfo()
{
    CNetWorkDataSingle::GetSingletonPtr()->GetDrawCardInfo(this,menu_selector(CDrawCard::OnGetCardInfo));
}



void CDrawCard::addCommonDrawCard()
{
    
    /*if(m_pDrawOneMenu1)
    {
        m_pDrawOneMenu1->setEnabled(true);
    }
    
    if(m_pDrawOneMenu2)
    {
        m_pDrawOneMenu2->setEnabled(true);
    }
    
    if(m_pDrawTenMenu1)
    {
        m_pDrawTenMenu1->setEnabled(true);
    }
    
    if(m_pDrawTenMenu2)
    {
        m_pDrawTenMenu2->setEnabled(true);
    }
    if(m_pMenuItemClose)
    {
        m_pMenuItemClose->setVisible(true);
    }*/
    
    m_pDrawCard1->removeAllChildren();
    char str[50] = {"curTime"};
    int ikey = UserDefault::getInstance()->getIntegerForKey(str);
    
    std::vector<ConfigureInfo*>  pInfo = CDBMgr::getInstance()->GetConfigureInfo();
    
    int iLastTime  = m_CardInfos.m_iCoindrawtime;

    int iCoinNum  = m_CardInfos.m_iCoindrawNum;
    m_iDrawOneNum = iCoinNum;

    int iTime =   m_CardInfos.m_iServerTime - iLastTime;
    
    m_pText1 = Sprite::createWithSpriteFrameName("drawtext1.png");
    Size  tBg  = m_pDrawCard1->getContentSize();
    m_pText1->setPosition(tBg.width*0.5,tBg.height*0.65);
    m_pDrawCard1->addChild(m_pText1);
    
    
    if(!m_CardInfos.m_bfirstDrawCoin)
    {
        m_pText1->setScale(0.75);
        m_pText1->setSpriteFrame("drawtext3.png");
        
    }
    
    Sprite *pDan1 = Sprite::createWithSpriteFrameName("drawdan_1.png");
    pDan1->setPosition(tBg.width*0.5,tBg.height*0.5);
    m_pDrawCard1->addChild(pDan1);
    
    
    sprintf(str,"免费次数:%d/%d",m_iDrawOneNum,m_iDrawOneAllNum);
    
    m_pFreeTime1 = Label::createWithTTF(str,"fonts/cuti.ttf",25);
    m_pFreeTime1->setPosition(tBg.width*0.5,tBg.height*0.28);
    m_pFreeTime1->enableOutline(Color4B(0,0,0,255),1);
    m_pDrawCard1->addChild(m_pFreeTime1);
    
    m_pFreeTime11 = Label::createWithTTF("后免费","fonts/cuti.ttf",25);
    m_pFreeTime11->setColor(Color3B(255,228,140));
    m_pFreeTime11->enableOutline(Color4B(0,0,0,255),1);
    m_pFreeTime11->setVisible(false);
    m_pFreeTime11->setPosition(m_pFreeTime1->getPositionX()+m_pFreeTime1->getContentSize().width*0.5
                               +m_pFreeTime11->getContentSize().width*0.5,tBg.height*0.28);
    m_pDrawCard1->addChild(m_pFreeTime11);

    //普通抽卡
    if(ikey!=0)
    {
        if((ikey-iTime)<=0)
        {
            sprintf(str,"免费次数:%d/%d",m_iDrawOneNum,m_iDrawOneAllNum);
            
            m_pFreeTime1->setString(str);
            m_pFreeTime11->setVisible(false);
        }
        else
        {
            m_iTime1 =  ikey-iTime;
            m_iMin1  = m_iTime1/60;
            m_iSec1 = m_iTime1%60;
            
            char str[20] = {0};
            sprintf(str,"%02d:%02d",m_iMin1,m_iSec1);
            
            m_pFreeTime1->setString(str);
            m_pFreeTime1->setPosition(tBg.width*0.4,tBg.height*0.28);
            m_pFreeTime11->setVisible(true);
            m_pFreeTime11->setPosition(m_pFreeTime1->getPositionX()+m_pFreeTime1->getContentSize().width*0.5
                                       +m_pFreeTime11->getContentSize().width*0.5,tBg.height*0.28);
            
             if(m_CardInfos.m_iCoindrawNum<=0)
             {
                 sprintf(str,"免费次数:%d/%d",0,m_iDrawOneAllNum);
                 
                 m_pFreeTime1->setString(str);
                 m_pFreeTime1->setPositionX(tBg.width*0.5);
                 m_pFreeTime11->setVisible(false);
             }
            else
            {
                schedule(schedule_selector(CDrawCard::UpdateCommon),1);
            }
            
        }
    }
    
    
    Sprite *pChouBtn = Sprite::createWithSpriteFrameName("drawbluebg.png");
    Sprite *pChouBtnSelect = Sprite::createWithSpriteFrameName("drawbluebg.png");
    pChouBtnSelect->setColor(Color3B::GRAY);
    
    m_pChouIte1 = MenuItemSprite::create(pChouBtn,pChouBtnSelect,CC_CALLBACK_1(CDrawCard::CallOnceCommon, this));
    m_pChouIte1->setPosition(tBg.width*0.5,tBg.height*0.2);
    m_pDrawOneMenu1 = Menu::create(m_pChouIte1,NULL);
    m_pDrawOneMenu1->setPosition(Vec2(0,0));
    m_pDrawCard1->addChild(m_pDrawOneMenu1);
    
    
    Size tChou1  = m_pChouIte1->getContentSize();
    
    
    sprintf(str,"%d",pInfo[0]->m_iDrawOneCoin);
    
    Label *pChouNum = Label::createWithTTF("抽 次","fonts/cuti.ttf",25);
    pChouNum->setPosition(tChou1.width*0.35,tChou1.height*0.5);
    pChouNum->enableOutline(Color4B(0,0,0,255),1);
    m_pChouIte1->addChild(pChouNum);
    
    
    
    Label *pOneNum = Label::createWithTTF("1","fonts/cuti.ttf",25);
    pOneNum->setPosition(tChou1.width*0.35,tChou1.height*0.5);
    pOneNum->setColor(Color3B(255,228,140));
    pOneNum->enableOutline(Color4B(0,0,0,255),2);
    m_pChouIte1->addChild(pOneNum);
    
    
    
    sprintf(str,"%d",pInfo[0]->m_iDrawOneCoin);
    
    Label *pNum = Label::createWithTTF(str,"fonts/cuti.ttf",25);
    pNum->setPosition(pChouNum->getPositionX()+pNum->getContentSize().width*0.6+pChouNum->getContentSize().width*0.5,tChou1.height*0.5);
    m_pChouIte1->addChild(pNum);
    pNum->enableOutline(Color4B(0,0,0,255),2);
    
    //single_coin.png"
    Sprite *pCoin = Sprite::createWithSpriteFrameName("single_coin.png");
    pCoin->cocos2d::Node::setScale(0.5);
    
    pCoin->setPosition(pNum->getPositionX()+pNum->getContentSize().width*0.5+pCoin->getBoundingBox().size.width*0.5,tChou1.height*0.5);
    m_pChouIte1->addChild(pCoin);
    
    
    std::string strFree = m_pFreeTime1->getString();
    
    int iPos = strFree.find("免费次数");
    
    if(iCoinNum>0&&iPos>=0)
    {
        Sprite *pMianfei  = Sprite::createWithSpriteFrameName("mianfei.png");
        pMianfei->setPosition(tChou1.width*0.35,tChou1.height*0.5);
        m_pChouIte1->addChild(pMianfei);
        
        pChouNum->setString("抽 次");
        pChouNum->setPosition(tChou1.width*0.65,tChou1.height*0.5);
        pCoin->setVisible(false);
        pNum->setVisible(false);
        pOneNum->setVisible(false);
        
        
        Label *pChouOne = Label::createWithTTF("1","fonts/cuti.ttf",25);
        pChouOne->setColor(Color3B(255,228,140));
        pChouOne->enableOutline(Color4B(0,0,0,255),2);
        pChouOne->setPosition(tChou1.width*0.645,tChou1.height*0.5);
        m_pChouIte1->addChild(pChouOne);
    }
    
    Sprite *pChouTen = Sprite::createWithSpriteFrameName("drawbtn1.png");
    Sprite *pChouTenSelect = Sprite::createWithSpriteFrameName("drawbtn1.png");
    pChouTenSelect->setColor(Color3B::GRAY);
    
    MenuItemSprite *pChouTenItem = MenuItemSprite::create(pChouTen,pChouTenSelect,CC_CALLBACK_1(CDrawCard::OnCallTenCommon, this));
    pChouTenItem->setPosition(tBg.width*0.5,0);
    m_pDrawTenMenu1 = Menu::create(pChouTenItem,NULL);
    m_pDrawTenMenu1->setPosition(Vec2(0,0));
    m_pDrawCard1->addChild(m_pDrawTenMenu1);
    
    
    Size  tMSize = pChouTenItem->getContentSize();
    Sprite *pCoinTen = Sprite::createWithSpriteFrameName("single_coin.png");
    
    
    pCoinTen->setScale(0.4);
    pCoinTen->setPosition(tMSize.width*0.6,tMSize.height*0.4);
    pChouTenItem->addChild(pCoinTen);
    
    sprintf(str,"%d",pInfo[0]->m_iDrawTenCoin);
    Label *pCoinNum = Label::createWithTTF(str,"fonts/cuti.ttf",20);
    pCoinNum->setColor(Color3B(255,228,140));
    
    pCoinNum->setPosition(tMSize.width*0.45,tMSize.height*0.4);
    pCoinNum->enableOutline(Color4B(0,0,0,255),1);
    pChouTenItem->addChild(pCoinNum);
    
    if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SENIOR_CARD) && (guiderManagerLayer::getInstance()->getStepNum() == 4))
    {
        addGuideInfo();
    }
}


void CDrawCard::addAdvanceDrawCard()
{
    
    /*if(m_pDrawOneMenu1)
    {
        m_pDrawOneMenu1->setEnabled(true);
    }
    
    if(m_pDrawOneMenu2)
    {
        m_pDrawOneMenu2->setEnabled(true);
    }
    
    if(m_pDrawTenMenu1)
    {
        m_pDrawTenMenu1->setEnabled(true);
    }
    
    if(m_pDrawTenMenu2)
    {
        m_pDrawTenMenu2->setEnabled(true);
    }
    if(m_pMenuItemClose)
    {
        m_pMenuItemClose->setVisible(true);
    }*/
    
    std::vector<ConfigureInfo*>  pInfo = CDBMgr::getInstance()->GetConfigureInfo();
    
    char str[50] = {0};
    
    m_iDrawTwoNum = m_CardInfos.m_istoneDrawNum;
    
    m_pDrawCard2->removeAllChildren();
    
    int iTime2 =   m_CardInfos.m_iServerTime - m_CardInfos.m_iStonedrawtime;
    
    char str2[50] = {"curTime2"};
    int iKey2 = UserDefault::getInstance()->getIntegerForKey(str2);
    
    
    int iStoneNum = m_CardInfos.m_istoneDrawNum;
    
    
    if(iKey2-iTime2<=0)
    {
        m_iDrawTwoNum = 1;
        iStoneNum = 1;
        m_CardInfos.m_istoneDrawNum = 1;
        
    }
    
    Size tBg = m_pDrawCard2->getContentSize();

    
    Sprite *pDan2 = Sprite::createWithSpriteFrameName("drawdan_2.png");
    pDan2->setPosition(tBg.width*0.5,tBg.height*0.5);
    m_pDrawCard2->addChild(pDan2);

    std::string strText = "drawtext2.png";
    
    m_pText2 = Sprite::createWithSpriteFrameName(strText);
    m_pText2->setPosition(tBg.width*0.5,tBg.height*0.65);
    m_pDrawCard2->addChild(m_pText2);
    

    if(!m_CardInfos.m_bfirstDrawStone)
    {
        strText = "drawtext4.png";
        m_pText2->setSpriteFrame(strText);
        m_pText2->cocos2d::Node::setScale(0.75);
    }
    
    sprintf(str,"免费次数:%d/%d",1,m_iDrawTwoAllNum);
    m_pFreeTime2 = Label::createWithTTF(str,"fonts/cuti.ttf",25);
    m_pFreeTime2->enableOutline(Color4B(0,0,0,255),1);
    m_pFreeTime2->setPosition(tBg.width*0.5,tBg.height*0.28);
    m_pDrawCard2->addChild(m_pFreeTime2);

    
    m_pFreeTime22 = Label::createWithTTF("后免费","fonts/cuti.ttf",25);
    m_pFreeTime22->enableOutline(Color4B(0,0,0,255),1);
    m_pFreeTime22->setColor(Color3B(255,228,140));
    m_pFreeTime22->setVisible(false);
    m_pFreeTime22->setPosition(m_pFreeTime2->getPositionX()+m_pFreeTime2->getContentSize().width*0.5+m_pFreeTime22->getContentSize().width*0.5,tBg.height*0.28);
    m_pDrawCard2->addChild(m_pFreeTime22);
    

    Sprite *pChouBtn = Sprite::createWithSpriteFrameName("drawbluebg.png");

    Sprite *pChouBtnSelect = Sprite::createWithSpriteFrameName("drawbluebg.png");
    
    pChouBtnSelect->setColor(Color3B::GRAY);
    
    m_pChouIte2 = MenuItemSprite::create(pChouBtn,pChouBtnSelect,CC_CALLBACK_1(CDrawCard::CallOnceAdvance, this));

    m_pChouIte2->setPosition(tBg.width*0.5,tBg.height*0.2);
    
    m_pDrawOneMenu2 = Menu::create(m_pChouIte2,NULL);
    m_pDrawOneMenu2->setPosition(Vec2(0,0));
    
    m_pDrawCard2->addChild(m_pDrawOneMenu2);
    
    
    Size tChou2 = m_pChouIte2->getContentSize();
    
    
    Label *pChouNum = Label::createWithTTF("抽 次","fonts/cuti.ttf",25);
    pChouNum->setPosition(tChou2.width*0.35,tChou2.height*0.5);
    pChouNum->enableOutline(Color4B(0,0,0,255),1);

    
    Label *pOneNum = Label::createWithTTF("1","fonts/cuti.ttf",25);
    pOneNum->setPosition(tChou2.width*0.3475,tChou2.height*0.5);
    pOneNum->setColor(Color3B(255,228,140));
    pOneNum->enableOutline(Color4B(0,0,0,255),2);
    m_pChouIte2->addChild(pOneNum);
    
    m_pChouIte2->addChild(pChouNum);
    
    sprintf(str,"%d",pInfo[0]->m_iDrawOneStone);
    
    Label *pNum = Label::createWithTTF(str,"fonts/cuti.ttf",25);
    pNum->setPosition(pChouNum->getPositionX()+pNum->getContentSize().width*0.7+pChouNum->getContentSize().width*0.5,tChou2.height*0.5);
    m_pChouIte2->addChild(pNum);
    pNum->enableOutline(Color4B(0,0,0,255),2);
    
    //single_coin.png"
    Sprite *pCoin = Sprite::createWithSpriteFrameName("stone.png");
    pCoin->cocos2d::Node::setScale(0.5);
    
    pCoin->setPosition(pNum->getPositionX()+pNum->getContentSize().width*0.5+pCoin->getBoundingBox().size.width*0.5,tChou2.height*0.5);
    m_pChouIte2->addChild(pCoin);
    
    
    //高级抽卡
    if(iKey2!=0)
    {
        if((iKey2-iTime2)<=0)
        {
            m_iDrawTwoNum = 1;
            m_CardInfos.m_istoneDrawNum = 1;
            sprintf(str,"免费次数:%d/%d",m_iDrawTwoNum,m_iDrawTwoAllNum);
            m_pFreeTime2->setString(str);
            m_pFreeTime22->setVisible(false);
            m_pFreeTime2->setPositionX(tBg.width*0.5);
        }
        else
        {
            m_iTime2 =  iKey2-iTime2;
            m_iHour2  = m_iTime2/3600;
            m_iMin2 = m_iTime2%3600/60;
            m_iSec2 = m_iTime2%60;
            
            char str[20] = {0};
            
            if(m_iHour2<=0)
            {
                sprintf(str,"%02d:%02d",m_iMin2,m_iSec2);
            }
            else
            {
                sprintf(str,"%02d:%02d:%02d",m_iHour2,m_iMin2,m_iSec2);
            }
            
            m_pFreeTime2->setString(str);
            m_pFreeTime2->setPositionX(tBg.width*0.41);
            
            m_pFreeTime22->setPosition(m_pFreeTime2->getPositionX()+m_pFreeTime2->getContentSize().width*0.5+m_pFreeTime22->getContentSize().width*0.5,tBg.height*0.28);
            m_pFreeTime22->setVisible(true);
            schedule(schedule_selector(CDrawCard::UpdateAdvance),1);
        }
    }
    

    
    std::string strFree = m_pFreeTime2->getString();
    
    int iPos = strFree.find("免费次数");
    
    if(iStoneNum >0&&iPos>=0)
    {
    
        Sprite *pMianFei = Sprite::createWithSpriteFrameName("mianfei.png");
        pMianFei->setPosition(tChou2.width*0.35,tChou2.height*0.5);
        m_pChouIte2->addChild(pMianFei);
        
        pChouNum->setString("抽 次");
        pChouNum->setPosition(tChou2.width*0.65,tChou2.height*0.5);
        
        
        Label *pChouTwo = Label::createWithTTF("1","fonts/cuti.ttf",25);
        pChouTwo->setColor(Color3B(255,228,140));
        pChouTwo->enableOutline(Color4B(0,0,0,255),2);
        pChouTwo->setPosition(tChou2.width*0.645,tChou2.height*0.5);
        m_pChouIte2->addChild(pChouTwo);
        pOneNum->setVisible(false);
        pCoin->setVisible(false);
        pNum->setVisible(false);
        
    }
    
    //连抽10次
    Sprite *pChouTen = Sprite::createWithSpriteFrameName("drawbtn2.png");
    Sprite *pChouTenSelect = Sprite::createWithSpriteFrameName("drawbtn2.png");
    pChouTenSelect->setColor(Color3B::GRAY);

    MenuItemSprite *pChouTenItem = MenuItemSprite::create(pChouTen,pChouTenSelect,CC_CALLBACK_1(CDrawCard::OnCallTenAdvance, this));
    pChouTenItem->setPosition(tBg.width*0.5,0);
    
    m_pDrawTenMenu2 = Menu::create(pChouTenItem,NULL);
    m_pDrawTenMenu2->setPosition(Vec2(0,0));
    m_pDrawCard2->addChild(m_pDrawTenMenu2);

    
    Size  tMSize = pChouTenItem->getContentSize();
    Sprite *pCoinTen = Sprite::createWithSpriteFrameName("stone.png");

    
    pCoinTen->setScale(0.4);
    pCoinTen->setPosition(tMSize.width*0.625,tMSize.height*0.4);
    pChouTenItem->addChild(pCoinTen);
    
    sprintf(str,"%d",pInfo[0]->m_iDrawTenStone);
    Label *pCoinNum = Label::createWithTTF(str,"fonts/cuti.ttf",20);
    pCoinNum->setColor(Color3B(255,228,140));
    
    pCoinNum->setPosition(tMSize.width*0.5,tMSize.height*0.4);
    pCoinNum->enableOutline(Color4B(0,0,0,255),1);
    pChouTenItem->addChild(pCoinNum);
    if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SENIOR_CARD) && (guiderManagerLayer::getInstance()->getStepNum() == 1))
    {
        m_guidePos = m_pChouIte2->convertToWorldSpaceAR(Vec2::ZERO);
        guiderManagerLayer::getInstance()->setStepNum(3);
        guiderManagerLayer::getInstance()->createLayer(this, GUIDE_SHOW_FINGER, m_guidePos);
    }
    else if((CDataManager::getEndedBy(GUIDE_COMMON_CARD)) && (!CDataManager::getEndedBy(GUIDE_SENIOR_CARD)))
    {
        addGuideInfo();
    }
}





