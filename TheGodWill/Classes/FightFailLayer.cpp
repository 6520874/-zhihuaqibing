//
//  FightFailLayer.cpp
//  TheGodWill
//
//  Created by prefer_shan on 14/12/18.
//
//

#include "FightFailLayer.h"

#include "LayerManager.h"
#include "DataManager.h"
#include "LYPersonalData.h"
#include "LYFightData.h"
#include "guiderManagerLayer.h"
#include "CSharedPlist.h"
#include "mainMenuLayer.h"
#include "zzbNetControlModule.h"
#include "zzbNewNetStruct.h"

#define MY_PERSION CPersonalData::getInstance()

CFightFailLayer::CFightFailLayer()
{
    m_winSize =  Director::getInstance()->getWinSize();
    m_pLabelGo1 = NULL;
    m_pLabelGo2 = NULL;
    m_pMenuItemGoArrow1 = NULL;
    m_pMenuItemGoArrow2 = NULL;
    m_bGoArrow1 = false;
    m_bGoArrow2 = false;
    m_pMenuGo1 = NULL;
    m_pMenuGo2 = NULL;
    CSharedPlist::getInstance()->pushPlist("plist/battleResult.plist");
}

CFightFailLayer::~CFightFailLayer()
{
    CSharedPlist::getInstance()->erasePlist("plist/battleResult.plist");
}

bool CFightFailLayer::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    log("on TouchBegan");
    
     m_bGoArrow1 = m_pMenuGo1->onTouchBegan(touch,event);
    
     m_bGoArrow2 = m_pMenuGo2->onTouchBegan(touch,event);

    return true;
}

void CFightFailLayer::onExit()
{
    _eventDispatcher->removeEventListener(listen);
    NotificationCenter::getInstance()->removeObserver(this, "failGuide");
    CBaseLayer::onExit();
}


void CFightFailLayer::OnGoFirst(Ref *pSender)
{
    Director::getInstance()->resume();
    
    if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_FAIL_TIP) && (guiderManagerLayer::getInstance()->getStepNum() == 2))
    {
        CDataManager::saveGuideInfoToDB(GUIDE_FAIL_TIP);
        guiderManagerLayer::getInstance()->removeGuiderLayer();
    }
    if(CPersonalData::getInstance()->getJumpFromSkill())
    {
        CPersonalData::getInstance()->setJumpFromSkill(false);
        CPersonalData::getInstance()->setJumpInfoNull();
    }
    Node*parent =  CLayerManager::GetSingletonPtr()->createLayer(eLayer_MainMenu,NULL,false);
    CLayerManager::GetSingletonPtr()->createLayer(eLayer_LvUp,parent,false);
    
}

void CFightFailLayer::OnGoSecond(Ref *pSender)
{
    Director::getInstance()->resume();
    
    if(CPersonalData::getInstance()->getJumpFromSkill())
    {
        CPersonalData::getInstance()->setJumpFromSkill(false);
        CPersonalData::getInstance()->setJumpInfoNull();
    }
    Node *parent =  CLayerManager::GetSingletonPtr()->createLayer(eLayer_MainMenu,NULL,false);
    CLayerManager::GetSingletonPtr()->createLayer(eLayer_LvSkill,parent,false);
}

bool CFightFailLayer::init()
{
    //CBaseLayer::init();
    CBaseLayer::initWithColor(Color4B(0, 0, 0, 125));
    
    listen = EventListenerTouchOneByOne::create();
    listen->setSwallowTouches(true);
    listen->onTouchBegan = CC_CALLBACK_2(CFightFailLayer::onTouchBegan, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listen, this);
    Director::getInstance()->resume();
    SimpleAudioEngine::getInstance()->playEffect("music/fail.mp3");
    //战斗胜利
    Sprite *pBg = Sprite::createWithSpriteFrameName("common_bg.png");
    pBg->setPosition(m_winSize.width*0.5,m_winSize.height*0.5);
    addChild(pBg);
    
    Size szBg = pBg->getBoundingBox().size;
    
    Sprite *pBian = Sprite::createWithSpriteFrameName("result_starBg.png");
    pBian->setPosition(szBg.width*0.5,szBg.height*0.9);
    pBian->setScale(1.5);
    pBg->addChild(pBian);

    Sprite *pText = Sprite::createWithSpriteFrameName("fightfail.png");
    pText->setScale(2);
    pText->setPosition(szBg.width*0.5,szBg.height*0.9);
    pBg->addChild(pText);
    

    
    //战斗失败提示
    Sprite *pFightLoseTip1 = Sprite::createWithSpriteFrameName("losesLip.png");
    pFightLoseTip1->setScale(6);
    pFightLoseTip1->setPosition(szBg.width*0.5,szBg.height*0.675);
    pBg->addChild(pFightLoseTip1);
    
    Sprite *pFightLoseTip2 = Sprite::createWithSpriteFrameName("losesLip.png");
    pFightLoseTip2->setScale(6);
    pFightLoseTip2->setPosition(szBg.width*0.5,szBg.height*0.4);
    pBg->addChild(pFightLoseTip2);
    
    Size tTip1Bg =   pFightLoseTip1->getContentSize();//pFightLoseTip1->getBoundingBox().size;
    Size tTip2Bg =  pFightLoseTip2->getContentSize();  //pFightLoseTip2->getBoundingBox().size;
    
    
    Sprite *pPic1 = Sprite::createWithSpriteFrameName("fightlose1.png");
    pPic1->setScale(0.225);
    pPic1->setPosition(tTip1Bg.width*0.25,tTip1Bg.height*0.5);
    pFightLoseTip1->addChild(pPic1);

    Sprite *pPic2 = Sprite::createWithSpriteFrameName("fightlose2.png");
    pPic2->setScale(0.225);
    pPic2->setPosition(tTip2Bg.width*0.25,tTip2Bg.height*0.5);
    pFightLoseTip2->addChild(pPic2);

    //升级士兵
    
     float fScale = 0.15;
    
    Label *pLabelSoldier1 = Label::createWithTTF("升级士兵等级可使战斗力提升","fonts/cuti.ttf",25);
    pLabelSoldier1->setAnchorPoint(Vec2::ZERO);
    pLabelSoldier1->setScale(fScale);
    pLabelSoldier1->setPosition(tTip1Bg.width*0.35,tTip1Bg.height*0.55);
    pFightLoseTip1->addChild(pLabelSoldier1);
    
    Label *pLabelSoldier2 = Label::createWithTTF("升级士兵技能可使战斗力提升","fonts/cuti.ttf",25);
    pLabelSoldier2->setAnchorPoint(Vec2::ZERO);
    pLabelSoldier2->setScale(fScale);
    pLabelSoldier2->setPosition(tTip2Bg.width*0.35,tTip2Bg.height*0.55);
    pFightLoseTip2->addChild(pLabelSoldier2);
    
    Sprite *pGoSoldier1 = Sprite::createWithSpriteFrameName("goatonce.png");
    Sprite *pGoSoldier2 = Sprite::createWithSpriteFrameName("goatonce.png");
    pGoSoldier2->setColor(Color3B::GRAY);
    
    
    Sprite *pGo1Skill1 = Sprite::createWithSpriteFrameName("goatonce.png");
    Sprite *pGo2Skill2 = Sprite::createWithSpriteFrameName("goatonce.png");
    pGo2Skill2->setColor(Color3B::GRAY);
    
    MenuItemSprite *pItem1 = MenuItemSprite::create(pGoSoldier1,pGoSoldier2,CC_CALLBACK_1(CFightFailLayer::OnGoFirst,this));
    
    pItem1->setScale(0.3);
    
    MenuItemSprite *pItem2 = MenuItemSprite::create(pGo1Skill1,pGo2Skill2,CC_CALLBACK_1(CFightFailLayer::OnGoSecond,this));
    pItem2->setScale(0.3);
    
    
    m_pMenuGo1 = Menu::create(pItem1,NULL);
    m_pMenuGo1->setAnchorPoint(Vec2::ZERO);
    m_pMenuGo1->setPosition(tTip1Bg.width*0.625,tTip1Bg.height*0.2);
    pFightLoseTip1->addChild(m_pMenuGo1);
    
    
    Sprite *pThree1 = Sprite::createWithSpriteFrameName("gothree.png");
    pThree1->setScale(fScale);
    pThree1->setPosition(tTip1Bg.width*0.75,tTip1Bg.height*0.15);
    pFightLoseTip1->addChild(pThree1);
    
    
    Sprite *pThree2 = Sprite::createWithSpriteFrameName("gothree.png");
    pThree2->setScale(fScale);
    pThree2->setPosition(tTip2Bg.width*0.75,tTip2Bg.height*0.15  );
    pFightLoseTip2->addChild(pThree2);
    
    m_pMenuGo2 = Menu::create(pItem2,NULL);
    m_pMenuGo2->setAnchorPoint(Vec2::ZERO);
    m_pMenuGo2->setPosition(tTip2Bg.width*0.625,tTip2Bg.height*0.2);
    pFightLoseTip2->addChild(m_pMenuGo2);
    
    
    Sprite *pGoArrow1 = Sprite::createWithSpriteFrameName("gothree.png");
    
    Sprite *pGoArrow2 = Sprite::createWithSpriteFrameName("gothree.png");

    m_pMenuItemGoArrow1 = MenuItemSprite::create(pGoArrow1,NULL,CC_CALLBACK_1(CFightFailLayer::OnGoFirst, this));
    m_pMenuItemGoArrow1->setPosition(tTip1Bg.width*0.725,tTip1Bg.height*0.15);
    m_pMenuItemGoArrow1->setScale(fScale);
    
    pFightLoseTip1->addChild(m_pMenuItemGoArrow1);
    
    m_pMenuItemGoArrow2 = MenuItemSprite::create(pGoArrow2,NULL,CC_CALLBACK_1(CFightFailLayer::OnGoSecond, this));
    m_pMenuItemGoArrow2->setPosition(tTip2Bg.width*0.725,tTip2Bg.height*0.15);
    m_pMenuItemGoArrow2->setScale(fScale);
    pFightLoseTip2->addChild(m_pMenuItemGoArrow2);
    
    //返回
    //bluean.png  yellowan.png
    Sprite *pBule =  Sprite::createWithSpriteFrameName("redan.png");
    Sprite *pBuleSlect =  Sprite::createWithSpriteFrameName("redan.png");
    pBuleSlect->setColor(Color3B::GRAY);
    
    
    LYMenuItemSprite *pBackItem = LYMenuItemSprite::create(pBule,pBuleSlect,CC_CALLBACK_1(CFightFailLayer::OnCallBack, this));
    pBackItem->setScale(1.5);
    
    
    Sprite *pAgain=  Sprite::createWithSpriteFrameName("bluean.png");
    Sprite *pAgainSlect =  Sprite::createWithSpriteFrameName("bluean.png");
    pAgainSlect ->setColor(Color3B::GRAY);
    
    LYMenuItemSprite *pAgainItem = LYMenuItemSprite::create(pAgain,pAgainSlect,CC_CALLBACK_1(CFightFailLayer::OnFightAgain, this));
    pAgainItem->setScale(1.5);
    

    std::string str ="返回";
    
    
    if(CFightData::getInstance()->getFightType() == FIGHT_PVP_TYPE)
    {
        str = "确定";
    }
    

    Label  *pBackLable = Label::createWithSystemFont(str,"Aril",30);
    pBackLable->setScale(0.7);
    pBackLable->setPosition(pBackItem->getContentSize().width*0.5,pBackItem->getContentSize().height*0.5);
    pBackItem->addChild(pBackLable);

    
    
    Label  *pAgainLable = Label::createWithSystemFont("再来一次","Aril",30);
    pAgainLable->setScale(0.7);
    pAgainLable->setPosition(pAgainItem->getContentSize().width*0.5,pAgainItem->getContentSize().height*0.5);
    pAgainItem->addChild(pAgainLable);
    
    Menu *pMenu = Menu::create(pBackItem,pAgainItem,NULL);
    pMenu->alignItemsHorizontallyWithPadding(170);
    pMenu->setPosition(szBg.width*0.5,szBg.height*0.145);
    pBg->addChild(pMenu);
    
    if(CFightData::getInstance()->getFightType() == FIGHT_PVP_TYPE)
    {
        pAgainItem->setVisible(false);
        pMenu->setPosition(szBg.width*0.7,szBg.height*0.145);
    }
    
    
    
    
    
    //talkingdata
    int nowMidLevel = CFightData::getInstance()->getNowLevelId();
    int nowSmallLevel = CFightData::getInstance()->getNowMidLevelId();
    
    __String* tempStr = __String::createWithFormat("%d-%d",nowMidLevel,nowSmallLevel);
    TDCCMission::onFailed(tempStr->getCString(),"failed");
    
    //如果是首次第六关失败则提示升级士兵
    if(!(CDataManager::getEndedBy(GUIDE_FAIL_TIP)) && (CPersonalData::getInstance()->getlevelMode() == 0))
    {
//        if((CFightData::getInstance()->getNowLevelId() >= 6) && (CFightData::getInstance()->getNowLevelId() <= 10))//引导升级皇家卫士
//        {
//            //判断是否已有皇家卫士
//            bool bExisted = false;
//            std::vector<FighterInfo*> m_pAll = CPersonalData::getInstance()->getAllSoldierInfo();
//            for(int i=0;i<m_pAll.size();i++)
//            {
//                //1 英雄  0怪物
//                if((m_pAll[i]->theType == 0) && (m_pAll[i]->rare>0))
//                {
//                    if(m_pAll[i]->monsterId == 13)
//                    {
//                        bExisted = true;
//                    }
//                }
//            }
//            if(bExisted)
//            {
//                guiderManagerLayer::getInstance()->setGuideType(GUIDE_FAIL_TIP);
//                guiderManagerLayer::getInstance()->setStepNum(1);
//                guiderManagerLayer::getInstance()->createLayer(this);
//                NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(CFightFailLayer::showUpgradeGuide), "failGuide", NULL);
//            }
//        }
//        else
        if((CFightData::getInstance()->getNowLevelId() >=6) && (CFightData::getInstance()->getNowLevelId() <= 10))
        {
            guiderManagerLayer::getInstance()->setGuideType(GUIDE_FAIL_TIP);
            guiderManagerLayer::getInstance()->setStepNum(1);
            guiderManagerLayer::getInstance()->createLayer(this);
            NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(CFightFailLayer::showUpgradeGuide), "failGuide", NULL);
        }
        else if(CPersonalData::getInstance()->getUserInfo()->m_iSmallLevel >= 10){
            CDataManager::saveGuideInfoToDB(GUIDE_FAIL_TIP);//如果前十关一直没有输过则不再显示该提示
        }
    }
    return true;
}

void CFightFailLayer::showUpgradeGuide(Ref* pSender)
{
    NotificationCenter::getInstance()->removeObserver(this, "failGuide");
    Vec2 pos = m_pMenuGo1->convertToWorldSpace(Vec2::ZERO);
    guiderManagerLayer::getInstance()->createLayer(this, GUIDE_SHOW_FINGER, pos);
}

void CFightFailLayer::backPvpMainLayer(Ref *pSender)
{
    if(CPersonalData::getInstance()->getState() == 0)
    {
        CLayerManager::GetSingletonPtr()->createLayerWithLoading(eLayer_PVPLayer, NULL,false);
//        CLayerManager::GetSingletonPtr()->createLayer(eLayer_PVPLayer, NULL,false);
    }
}

void CFightFailLayer::OnCallBack(Ref *pSender)
{
    
    Director::getInstance()->resume();
    if(CFightData::getInstance()->getFightType() == FIGHT_PVP_TYPE)
    {
         CNetControlModule::sharedNetControlModule()->getUserTeamInfo(this, menu_selector(CFightScene::backPvpMainLayer));
    }
    else
    {
        if(!CPersonalData::getInstance()->getJumpFromSkill())
        {
            CPersonalData::getInstance()->setisFailManual(false);
        }
        
        CLayerManager::GetSingletonPtr()->createLayerWithLoading(eLayer_MainMenu,NULL,false);
        
//        CLayerManager::GetSingletonPtr()->createLayer(eLayer_MainMenu, NULL,false);
    }
}

void CFightFailLayer::OnFightAgain(Ref *pSender)
{
        Director::getInstance()->resume();
    
    levelInfo* pSmallLevelInfo = CDataManager::getTheLevelInfoWithId(CFightData::getInstance()->getNowLevelId());
    //判断体力是否足够
    int needStaina = pSmallLevelInfo->stamina;
    if (MY_PERSION->getlevelMode() == 1) {
        needStaina = pSmallLevelInfo->stamina * 2;
    }
    if(needStaina > CPersonalData::getInstance()->GetPhysicalNum())//测试用false，看到就删
    {
        SimpleAudioEngine::getInstance()->playEffect("music/noBtn.mp3");
        Sprite * thetarget = CFightManager::getTheFlyWord("体力不足","fonts/cuti.ttf");
        thetarget->setPosition(Vec2(m_winSize.width / 2, m_winSize.height / 2));
        addChild(thetarget, 10);
    }
    else{
        CFightData::getInstance()->newLevelStart(CFightData::getInstance()->getNowLevelId());
        CLayerManager::GetSingletonPtr()->createLayerWithLoading(eLayer_Fight, NULL,false);
    }
}
