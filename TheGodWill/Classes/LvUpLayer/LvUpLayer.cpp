//
//  LvUpLayer.cpp
//  TheGodWill
//
//  Created by prefer_shan on 14/11/25.
//
//

#include "LvUpLayer.h"


#include "EnergyLvUpLayer.h"
#include "SoldierLvUpLayer.h"
#include "HeroLvUpLayer.h"
#include "LYPersonalData.h"
#include "guiderManagerLayer.h"
#include "CSharedPlist.h"
#include "Common.h"

CLvUpLayer::CLvUpLayer()
{
    m_winSize = Director::getInstance()->getWinSize();
    m_pLayer1 = NULL;
    m_pLayer2 = NULL;
    m_pLayer3 = NULL;
    
    m_pMenuItemSoldier = NULL;
    m_pMenuItemEnegy = NULL;
    m_pMenuItemBuilding = NULL;
    m_pMenuCose = NULL;
    CSharedPlist::getInstance()->pushPlist("plist/soliderUp.plist");
}

CLvUpLayer::~CLvUpLayer()
{
    CSharedPlist::getInstance()->erasePlist("plist/soliderUp.plist");
    
}

void CLvUpLayer::onExit()
{
    _eventDispatcher->removeEventListener(m_pListener);
    
    NotificationCenter::getInstance()->removeObserver(this, "SoliderCloseGuide");
    CBaseLayer::onExit();
}

void CLvUpLayer::OnMenuClose(Ref *pSender)
{
    int itag = ((LYMenuItemSprite*)pSender)->getTag();

    if(m_pLayer1)
    {
        m_pLayer1->removeFromParent();
    }
    
    if(m_pLayer2)
    {
        m_pLayer2->removeFromParent();
    }
    
    if(m_pLayer3)
    {
        m_pLayer3->removeFromParent();
    }
    
    if(itag == LvUpTagSoldier)
    {
        m_pLayer1 = CSoldierLvUpLayer::create();
        addChild(m_pLayer1,1,100);
        
        m_pMenuItemSoldier->setEnabled(false);
        m_pMenuItemEnegy->setEnabled(true);
        m_pMenuItemBuilding->setEnabled(true);
        
        m_pLayer2  = NULL;
        m_pLayer3  = NULL;
    }
    else if(itag == LvUpTagNengliang)
    {
        m_pLayer2 = CEnergyLvUpLayer::create();
        addChild(m_pLayer2,1,101);
     
        m_pMenuItemSoldier->setEnabled(true);
        m_pMenuItemEnegy->setEnabled(false);
        m_pMenuItemBuilding->setEnabled(true);
        
        m_pLayer1  = NULL;
        m_pLayer3  = NULL;
    }
    else if(itag ==  LVUptagBuilding)
    {
        m_pLayer3 = CHeroLvUpLayer::create();
        addChild(m_pLayer3,1,102);
     
        m_pMenuItemSoldier->setEnabled(true);
        m_pMenuItemEnegy->setEnabled(true);
        m_pMenuItemBuilding->setEnabled(false);

        m_pLayer1  = NULL;
        m_pLayer2  = NULL;
    }
    if((guiderManagerLayer::getInstance()->getStepNum() == 3) && (guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SOLIDER_NUM))
    {
        guiderManagerLayer::getInstance()->setStepNum(guiderManagerLayer::getInstance()->getStepNum() + 1);
        Vec2 pos = m_pLayer2->m_pGuideMenu->convertToWorldSpace(Vec2::ZERO);
        guiderManagerLayer::getInstance()->createLayer(this, GUIDE_SHOW_BOTH, pos);
    }
}

void CLvUpLayer::OnExitLvLayer(Ref *pSender)
{
    if(((guiderManagerLayer::getInstance()->getStepNum() == 8) && (guiderManagerLayer::getInstance()->getGuideType() == GUIDE_UPGRADE_SOLIDER))
       || ((guiderManagerLayer::getInstance()->getStepNum() == 5) && (guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SOLIDER_NUM)))
    {
        guiderManagerLayer::getInstance()->removeGuiderLayer();
        NotificationCenter::getInstance()->postNotification("upgradeAfter");
    }
    
    
    CPersonalData::getInstance()->setrestartPlayMusic(false);
    removeFromParent();
}


bool CLvUpLayer::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    
    return  true;
}
void CLvUpLayer::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event)
{

}
void CLvUpLayer::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event)
{

}
void CLvUpLayer::onTouchCancelled(Touch *touch, Event *event)
{

}

bool CLvUpLayer::init()
{
    //CBaseLayer::init();
    CBaseLayer::initWithColor(Color4B(0, 0, 0, 200));
    
    m_pListener = EventListenerTouchOneByOne::create();
    m_pListener->setSwallowTouches(true);
    m_pListener->onTouchBegan = CC_CALLBACK_2(CLvUpLayer::onTouchBegan, this);
    m_pListener->onTouchMoved = CC_CALLBACK_2(CLvUpLayer::onTouchMoved, this);
    m_pListener->onTouchEnded = CC_CALLBACK_2(CLvUpLayer::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(m_pListener,this);

    Sprite* m_pSoldierBg = Sprite::createWithSpriteFrameName("hero_bg.png");
    m_pSoldierBg->setPosition(Vec2(m_winSize.width*0.5, m_winSize.height*0.5));
    addChild(m_pSoldierBg);
    
    
    Size tBg = m_pSoldierBg->getContentSize();
    
    Sprite *pCloseSprite = Sprite::createWithSpriteFrameName("closeBtn.png");
    
    Sprite *pCloseSpriteSelc  = Sprite::createWithSpriteFrameName("closeBtn.png");
    pCloseSpriteSelc->setColor(Color3B::GRAY);
    LYMenuItemSprite *pCloseItem = LYMenuItemSprite::create(pCloseSprite,pCloseSpriteSelc,CC_CALLBACK_1(CLvUpLayer::OnExitLvLayer, this));
    CommonData::enlargeItemSprite(pCloseItem, 2);
    m_pMenuCose =  Menu::create(pCloseItem,NULL);
    
    m_pMenuCose->setPosition(tBg.width*0.95,tBg.height*0.94);
    m_pSoldierBg->addChild(m_pMenuCose,1, 100);
    
    //士兵
    
    float fbtScale = 0.95f;
    float fLabelScale = 0.55f;
    
    Sprite *pSoldierbtn = Sprite::createWithSpriteFrameName("bluebtn.png");
    pSoldierbtn->setScale(fbtScale);
    Sprite *pSoldierbtnSelect = Sprite::createWithSpriteFrameName("organge.png");
    pSoldierbtnSelect->setScale(fbtScale);
    Sprite *pSoldierbtnEnable = Sprite::createWithSpriteFrameName("organge.png");
    pSoldierbtnEnable->setScale(fbtScale);
    
    Label *pLabelSoldier = Label::createWithTTF("士兵","fonts/cuti.ttf",30);
    pLabelSoldier->setScale(fLabelScale);
    
    Size tLable1 = pSoldierbtn->getContentSize();
    pLabelSoldier->setPosition(Point(tLable1.width*0.5,tLable1.height*0.5));
    pSoldierbtn->addChild(pLabelSoldier);
    
    Label *pLabelSoldierSelc = Label::createWithTTF("士兵","fonts/cuti.ttf",30);
    pLabelSoldierSelc->setScale(fLabelScale);
    pLabelSoldierSelc->setPosition(Point(tLable1.width*0.5,tLable1.height*0.5));
    pSoldierbtnEnable->addChild(pLabelSoldierSelc);
    
    Label *pLabelSoldier2 = Label::createWithTTF("士兵","fonts/cuti.ttf",30);
    pLabelSoldier2->setScale(fLabelScale);
    pLabelSoldier2->setPosition(Point(tLable1.width*0.5,tLable1.height*0.5));
    pSoldierbtnSelect->addChild(pLabelSoldier2);
    
    m_pMenuItemSoldier  = LYMenuItemSprite::create(pSoldierbtn,pSoldierbtnSelect,pSoldierbtnEnable,CC_CALLBACK_1(CLvUpLayer::OnMenuClose, this));
    m_pMenuItemSoldier->setScale(2);
    
    m_pMenuItemSoldier->setEnabled(false);
    m_pMenuItemSoldier->setTag(LvUpTagSoldier);
    m_pMenuItemSoldier->setPosition(Point(m_winSize.width*0.3,m_winSize.height*0.9));

    
    //能量
    Sprite *pEnergybtn = Sprite::createWithSpriteFrameName("bluebtn.png");
    pEnergybtn->setScale(fbtScale);
    Sprite *pEnergybtnSelect = Sprite::createWithSpriteFrameName("organge.png");
    pEnergybtnSelect->setScale(fbtScale);
    Sprite *pEnergybtnEnable = Sprite::createWithSpriteFrameName("organge.png");
    pEnergybtnEnable->setScale(fbtScale);
    
    Label *pLabelEnergy = Label::createWithTTF("能量","fonts/cuti.ttf",30);
    pLabelEnergy->setScale(fLabelScale);
    Size tLable2 = pSoldierbtn->getContentSize();
    pLabelEnergy->setPosition(Point(tLable2.width*0.5,tLable2.height*0.5));
    pEnergybtn->addChild(pLabelEnergy);
    
    Label *pLabelEnergy2 = Label::createWithTTF("能量","fonts/cuti.ttf",30);
    pLabelEnergy2->setScale(fLabelScale);
    pLabelEnergy2->setPosition(Point(tLable2.width*0.5,tLable2.height*0.5));
    pEnergybtnSelect->addChild(pLabelEnergy2);
    
    Label *pLabelEnergyDis = Label::createWithTTF("能量","fonts/cuti.ttf",30);
    pLabelEnergyDis->setScale(fLabelScale);
    pLabelEnergyDis->setPosition(Point(tLable2.width*0.5,tLable2.height*0.5));
    pEnergybtnEnable->addChild(pLabelEnergyDis);
    
    m_pMenuItemEnegy  = LYMenuItemSprite::create(pEnergybtn,pEnergybtnSelect,pEnergybtnEnable,CC_CALLBACK_1(CLvUpLayer::OnMenuClose, this));
    
    m_pMenuItemEnegy->setPosition(Point(m_winSize.width*0.5,m_winSize.height*0.9));
    
    m_pMenuItemEnegy->setTag(LvUpTagNengliang);
    m_pMenuItemEnegy->setScale(2);

    //建筑
    Sprite *pBuildbtn = Sprite::createWithSpriteFrameName("bluebtn.png");
    pBuildbtn->setScale(fbtScale);
    
    Sprite *pBuildbtnSelect = Sprite::createWithSpriteFrameName("organge.png");
    pBuildbtnSelect->setScale(fbtScale);
    
    
    Sprite *pBuildbtnEnable = Sprite::createWithSpriteFrameName("organge.png");
    pBuildbtnEnable->setScale(fbtScale);
    
    Label *pLabelBuilding = Label::createWithTTF("英雄","fonts/cuti.ttf",30);
    
    //Label::createWithSystemFont("英雄","Arial",30);
    pLabelBuilding->setScale(fLabelScale);
    Size tLable3 = pSoldierbtn->getContentSize();
    pLabelBuilding->setPosition(Point(tLable3.width*0.5,tLable3.height*0.5));
    pBuildbtn->addChild(pLabelBuilding);
    
    Label *pLabeBuilding2 = Label::createWithTTF("英雄","fonts/cuti.ttf",30);
    pLabeBuilding2->setScale(fLabelScale);
    pLabeBuilding2->setPosition(Point(tLable3.width*0.5,tLable3.height*0.5));
    pBuildbtnEnable->addChild(pLabeBuilding2);
    
    Label *pLabeBuildingDisc = Label::createWithTTF("英雄","fonts/cuti.ttf",30);
    pLabeBuildingDisc->setScale(fLabelScale);
    pLabeBuildingDisc->setPosition(Point(tLable3.width*0.5,tLable3.height*0.5));
    pBuildbtnSelect->addChild(pLabeBuildingDisc);
    
    
    
    bool bGril =  CommonData::GetSingletonPtr()->IsGirlHeroVisable();
    
   
    m_pMenuItemBuilding  = LYMenuItemSprite::create(pBuildbtn,pBuildbtnSelect,pBuildbtnEnable,CC_CALLBACK_1(CLvUpLayer::OnMenuClose, this));

    m_pMenuItemBuilding->setPosition(Point(m_winSize.width*0.7,m_winSize.height*0.9));
    
    m_pMenuItemBuilding->setTag(LVUptagBuilding);
    
    m_pMenuItemBuilding->setScale(2);
    
  
    if(!bGril)
    {
        m_pMenuItemBuilding->setVisible(false);
        
        m_pMenuItemEnegy->setPosition(m_winSize.width*0.7,m_winSize.height*0.9);
    }
    
    
    Menu  * pMenu  = Menu::create(m_pMenuItemSoldier,m_pMenuItemEnegy,m_pMenuItemBuilding,NULL);
    
    pMenu->setPosition(Vec2::ZERO);
    addChild(pMenu,2);
    
    if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_UPGRADE_SOLIDER) && (guiderManagerLayer::getInstance()->getStepNum() == 2))
    {
        NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(CLvUpLayer::addSoliderCloseGuide), "SoliderCloseGuide", NULL);
    }
    m_pLayer1 = CSoldierLvUpLayer::create();
    
    addChild(m_pLayer1,1,100);
    
    if(!CDataManager::getEndedBy(GUIDE_SOLIDER_NUM))
    {
        m_pMenuItemEnegy->setVisible(false);
    }
    
    
    //debug模式下显示
#ifdef DEBUG_Mode
    m_pMenuItemEnegy->setVisible(true);
#endif
    
    
    

    if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SOLIDER_NUM) && (guiderManagerLayer::getInstance()->getStepNum() == 2))
    {
        m_pMenuItemBuilding->setVisible(false);
        m_pMenuItemEnegy->setVisible(true);
        NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(CLvUpLayer::addSoliderCloseGuide), "SoliderCloseGuide", NULL);
        Vec2 pos = Vec2(m_winSize.width * 0.67f, m_winSize.height * 0.905f);
        guiderManagerLayer::getInstance()->setStepNum(guiderManagerLayer::getInstance()->getStepNum() + 1);
        guiderManagerLayer::getInstance()->createLayer(this, GUIDE_SHOW_FINGER, pos);
    }
    if((CPersonalData::getInstance()->getUserInfo()->m_iSmallLevel >= 23) && (!CDataManager::getEndedBy(GUIDE_GIRL_HERO)))
    {
        //m_pMenuItemBuilding
        Sprite *pRedPoint = Sprite::createWithSpriteFrameName("newTip.png");
        m_pMenuItemBuilding->addChild(pRedPoint, 1, 50);
        pRedPoint->setScale(0.5f);
        Vec2 pos = Vec2(m_pMenuItemBuilding->getNormalImage()->getContentSize().width * 0.58f,
                        m_pMenuItemBuilding->getNormalImage()->getContentSize().height * 0.65f);
        pRedPoint->setPosition(pos);
        scheduleUpdate();
    }
    return true;
}

void CLvUpLayer::update(float time)
{
    if((CPersonalData::getInstance()->getUserInfo()->m_iSmallLevel >= 23) && (CDataManager::getEndedBy(GUIDE_GIRL_HERO)))
    {
        unscheduleUpdate();
        Sprite* sp = (Sprite*)m_pMenuItemBuilding->getChildByTag(50);
        if(sp)
        {
            sp->removeFromParent();
            sp = NULL;
        }
    }
    
}

void CLvUpLayer::addSoliderCloseGuide(Ref* pSender)
{
    NotificationCenter::getInstance()->removeObserver(this, "SoliderCloseGuide");
    if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_UPGRADE_SOLIDER) && (guiderManagerLayer::getInstance()->getStepNum() == 8))
    {
        Vec2 pos = m_pMenuCose->convertToWorldSpace(Vec2::ZERO);
        guiderManagerLayer::getInstance()->createLayer(this, GUIDE_SHOW_FINGER, pos);
    }
    else if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SOLIDER_NUM) && (guiderManagerLayer::getInstance()->getStepNum() == 5))
    {
        Vec2 pos = m_pMenuCose->convertToWorldSpace(Vec2::ZERO);
        guiderManagerLayer::getInstance()->createLayer(this, GUIDE_SHOW_FINGER, pos);
    }
}

