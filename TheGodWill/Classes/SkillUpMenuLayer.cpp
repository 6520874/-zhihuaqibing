//
//  SkillUpMenuLayer.cpp
//  TheGodWill
//
//  Created by prefer_shan on 14/12/25.
//
//

#include "SkillUpMenuLayer.h"
#include "SoldierSkillUpLayer.h"
#include "LYPersonalData.h"
#include "mainMenuLayer.h"
#include "CSharedPlist.h"
#include "Common.h"

CSKillMenuLayer *CSKillMenuLayer::t = NULL;

CSKillMenuLayer::CSKillMenuLayer()
{
    m_pMenuItemSoldier = NULL;
    m_pMenuItemHero = NULL;
    m_winSize = Director::getInstance()->getWinSize();
    m_pLayer1 = NULL;
    m_pLayer2 = NULL;
    m_pMenu = NULL;
    CSharedPlist::getInstance()->pushPlist("plist/skillUp.plist");
    CSharedPlist::getInstance()->pushPlist("plist/soliderUp.plist");
    CSharedPlist::getInstance()->pushPlist("plist/propIcon_1.plist");
    m_pMenuCose = NULL;
    m_pBg = NULL;
}


CSKillMenuLayer::~CSKillMenuLayer()
{
    CSharedPlist::getInstance()->erasePlist("plist/skillUp.plist");
    CSharedPlist::getInstance()->erasePlist("plist/soliderUp.plist");
    CSharedPlist::getInstance()->erasePlist("plist/propIcon_1.plist");
    
}

void CSKillMenuLayer::onExit()
{
    _eventDispatcher->removeEventListener(m_pListener);
    CBaseLayer::onExit();
    NotificationCenter::getInstance()->removeObserver(this,"CloseSkillGuide");
    unscheduleUpdate();
}

void CSKillMenuLayer::OnExitLvLayer(Ref *pSender)
{
    if((guiderManagerLayer::getInstance()->getStepNum() == 11) && (guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SKILL_UPGRADE))
    {
        guiderManagerLayer::getInstance()->removeGuiderLayer();
        NotificationCenter::getInstance()->postNotification("upgradeAfter");
    }
    
    CMainMenuLayer *pLayer = (CMainMenuLayer*)getParent();
    
    if(pLayer)
    {
        pLayer->m_bSkillStart = true;
  
         {
         pLayer->updateUsetInfo();
         }
    }
    
    
    
    CPersonalData::getInstance()->setrestartPlayMusic(false);
    removeFromParent();
}


bool CSKillMenuLayer::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{    
    return true;
}

void CSKillMenuLayer::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event)
{
    
}

void CSKillMenuLayer::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event)
{

}

void CSKillMenuLayer::onTouchCancelled(Touch *touch, Event *event)
{

}

void CSKillMenuLayer::StartHeroLayer()
{
    addbg();
    m_pLayer2 =  cocos2d::extension::CHeroSkillUpLayer::create(m_pBg);
    addChild(m_pLayer2,0,101);
    
    Sprite* sp = NULL;
    if(m_pMenuItemHero)
    {
        sp = (Sprite*)m_pMenuItemHero->getChildByTag(50);
    }
    
    if((CDataManager::getEndedBy(GUIDE_BOY_HERO_SKILL)) && (CDataManager::getEndedBy(GUIDE_GIRL_HERO_SKILL)) && sp)
    {
        sp->removeFromParent();
        sp = NULL;
    }
}

void CSKillMenuLayer::StartSoldierLayer()
{
    addbg();
  
    m_pLayer1 =  cocos2d::extension::CSoldierSkillUpLayer::create(m_pBg);
    addChild(m_pLayer1,0,100);
}

void CSKillMenuLayer::addCloseSkillGuide(Ref *pSender)
{
    NotificationCenter::getInstance()->removeObserver(this,"CloseSkillGuide");
    if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SKILL_UPGRADE) && (guiderManagerLayer::getInstance()->getStepNum() == 11))
    {
        Vec2 pos = m_pMenuCose->convertToWorldSpace(Vec2::ZERO);
        guiderManagerLayer::getInstance()->createLayer(getParent(), GUIDE_SHOW_FINGER, pos);
    }
}

void CSKillMenuLayer::OnSelectTouch(Ref *pSender)
{
    LYMenuItemSprite *p =  (LYMenuItemSprite*)pSender;
    int iTag =  p->getTag();
    
    if(m_pLayer1)
    {
        m_pLayer1->removeFromParent();
    }
    
    if(m_pLayer2)
    {
        m_pLayer2->removeFromParent();
    }
    
    if(iTag == LvSkillTagSoldier)
    {
       
        addbg();
        m_pLayer1 = cocos2d::extension::CSoldierSkillUpLayer::create(m_pBg);
        addChild(m_pLayer1,0,100);
        m_pLayer2 = NULL;
        m_pMenuItemSoldier->setEnabled(false);
        m_pMenuItemHero->setEnabled(true);
        
    }
    else if(iTag == LVSkillTagHero)
    {
         addbg();
        m_pLayer2 =  cocos2d::extension::CHeroSkillUpLayer::create(m_pBg);
        addChild(m_pLayer2,0,101);
        m_pLayer1  = NULL;
        m_pMenuItemSoldier->setEnabled(true);
        m_pMenuItemHero->setEnabled(false);
    }
    
}


void CSKillMenuLayer::addbg()
{
    if(m_pBg)
    {
        m_pBg->removeFromParent();
        m_pBg = NULL;
    }
    
    m_pBg = Sprite::createWithSpriteFrameName("soldierUp_bg.png");
    m_pBg->setPosition(Vec2(m_winSize.width*0.5, m_winSize.height*0.5));
    m_pBg->setScale(2);
    addChild(m_pBg,0,LVBgTag);
    

    Size tBg = m_pBg->getContentSize();
    
    Sprite *pCloseSprite = Sprite::createWithSpriteFrameName("closeBtn.png");
    Sprite *pCloseSpriteSelc  = Sprite::createWithSpriteFrameName("closeBtn.png");
    pCloseSpriteSelc->setColor(Color3B::GRAY);
    LYMenuItemSprite *pCloseItem = LYMenuItemSprite::create(pCloseSprite,pCloseSpriteSelc,CC_CALLBACK_1(CSKillMenuLayer::OnExitLvLayer, this));
    CommonData::enlargeItemSprite(pCloseItem, 2);
    pCloseItem->setTag(LvCloseTag);
    pCloseItem->setScale(0.5);
    
    m_pMenuCose = Menu::create(pCloseItem,NULL);
    m_pMenuCose->setPosition(tBg.width*0.95,tBg.height*0.934);
    m_pBg->addChild(m_pMenuCose,1,LvCloseTag);
    
}

bool CSKillMenuLayer::init()
{
    
    CBaseLayer::initWithColor(Color4B(0, 0, 0, 200));
    
    m_pListener = EventListenerTouchOneByOne::create();
    m_pListener->setSwallowTouches(true);
    m_pListener->onTouchBegan = CC_CALLBACK_2(CSKillMenuLayer::onTouchBegan, this);
    m_pListener->onTouchMoved = CC_CALLBACK_2(CSKillMenuLayer::onTouchMoved, this);
    m_pListener->onTouchEnded = CC_CALLBACK_2(CSKillMenuLayer::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(m_pListener,this);

 
    addbg();
    
    
    //士兵
    Sprite *pSoldierbtn = Sprite::createWithSpriteFrameName("bluebtn.png");
    pSoldierbtn->setScale(0.95);
    
    Sprite *pSoldierbtnSelect = Sprite::createWithSpriteFrameName("organge.png");
    pSoldierbtnSelect->setScale(0.95);
    
    Sprite *pSoldierbtnEnable = Sprite::createWithSpriteFrameName("organge.png");
    pSoldierbtnEnable->setScale(0.95);
    
    Label *pLabelSoldier = Label::createWithTTF("士兵","fonts/cuti.ttf",30);
    pLabelSoldier->setScale(0.55);
    Size tLable1 = pSoldierbtn->getContentSize();
    pLabelSoldier->setPosition(Point(tLable1.width*0.5,tLable1.height*0.5));
    pSoldierbtn->addChild(pLabelSoldier);
    
    Label *pLabelSoldierSelc = Label::createWithTTF("士兵","fonts/cuti.ttf",30);
    pLabelSoldierSelc->setScale(0.55);
    pLabelSoldierSelc->setPosition(Point(tLable1.width*0.5,tLable1.height*0.5));
    pSoldierbtnEnable->addChild(pLabelSoldierSelc);

    
    m_pMenuItemSoldier  = LYMenuItemSprite::create(pSoldierbtn,pSoldierbtnSelect,pSoldierbtnEnable,CC_CALLBACK_1(CSKillMenuLayer::OnSelectTouch, this));
    m_pMenuItemSoldier->setTag(LvSkillTagSoldier);
    m_pMenuItemSoldier->setScale(2);
    m_pMenuItemSoldier->setEnabled(false);
    
    m_pMenuItemSoldier->setPosition(Point(m_winSize.width*0.3,m_winSize.height*0.915));

    
    //英雄
    
    Sprite *pHerobtn = Sprite::createWithSpriteFrameName("bluebtn.png");
    pHerobtn->setScale(0.95);
    
    Sprite *pHerobtnSelect = Sprite::createWithSpriteFrameName("organge.png");
    pHerobtnSelect->setScale(0.95);
    
    Sprite *pHerobtnEnable = Sprite::createWithSpriteFrameName("organge.png");
    pHerobtnEnable->setScale(0.95);
    
    bool bMan =  CommonData::GetSingletonPtr()->IsManHeroVisable();
    
    Label *pLabelHero = Label::createWithTTF("英雄","fonts/cuti.ttf",30);
    pLabelHero->setScale(0.55);
    
    Label *pLabelHeroEnable = Label::createWithTTF("英雄","fonts/cuti.ttf",30);
    pLabelHeroEnable->setScale(0.55);
    Size tHeroBtn = pHerobtn->getContentSize();
    
    pLabelHero->setPosition(tHeroBtn.width*0.5,tHeroBtn.height*0.5);
    pHerobtn->addChild(pLabelHero);
    
    pLabelHeroEnable ->setPosition(tHeroBtn.width*0.5,tHeroBtn.height*0.5);
   
    pHerobtnEnable->addChild(pLabelHeroEnable);
    
    m_pMenuItemHero  = LYMenuItemSprite::create(pHerobtn,pHerobtnSelect,pHerobtnEnable,CC_CALLBACK_1(CSKillMenuLayer::OnSelectTouch, this));
    
    m_pMenuItemHero->setPosition(Point(m_winSize.width*0.7,m_winSize.height*0.915));
    
    m_pMenuItemHero->setTag(LVSkillTagHero);

    m_pMenuItemHero->setScale(2);
    
    if(!bMan)
    {
        m_pMenuItemHero->setVisible(false);
        m_pMenuItemSoldier->setPosition(Point(m_winSize.width*0.5,m_winSize.height*0.915));
    }
    
    m_pMenu  = Menu::create(m_pMenuItemSoldier,m_pMenuItemHero,NULL);
    m_pMenu->setPosition(Vec2::ZERO);
    addChild(m_pMenu,1);
    
    if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SKILL_UPGRADE) && (guiderManagerLayer::getInstance()->getStepNum() == 3))
    {
        NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(CSKillMenuLayer::addCloseSkillGuide), "CloseSkillGuide", NULL);
    }
    
    if(((CPersonalData::getInstance()->getUserInfo()->m_iSmallLevel >= 9) && (!CDataManager::getEndedBy(GUIDE_BOY_HERO_SKILL)))
        || ((CPersonalData::getInstance()->getUserInfo()->m_iSmallLevel >= 23) && (!CDataManager::getEndedBy(GUIDE_GIRL_HERO_SKILL))))
    {
        //m_pMenuItemBuilding
        Sprite *pRedPoint = Sprite::createWithSpriteFrameName("newTip.png");
        m_pMenuItemHero->addChild(pRedPoint, 1, 50);
        pRedPoint->setScale(0.5f);
        Vec2 pos = Vec2(m_pMenuItemHero->getContentSize().width * 0.68f,
                        m_pMenuItemHero->getContentSize().height * 0.65f);
        pRedPoint->setPosition(pos);
        scheduleUpdate();
    }
    
    if((CPersonalData::getInstance()->getJumpFromSkill() && (CPersonalData::getInstance()->m_jumpInfo.type == 0))
       ||(!CPersonalData::getInstance()->getJumpFromSkill()))
    {
        m_pLayer1 =  cocos2d::extension::CSoldierSkillUpLayer::create(m_pBg);
        addChild(m_pLayer1,1,100);
        m_pLayer1->showSkillDetail();
    }
    else if(CPersonalData::getInstance()->getJumpFromSkill())
    {
        if(m_pLayer1)
        {
            m_pLayer1->removeFromParent();
        }

        addbg();
        m_pLayer2 =  cocos2d::extension::CHeroSkillUpLayer::create(m_pBg);
        addChild(m_pLayer2,0,101);
        m_pLayer1  = NULL;
        m_pMenuItemSoldier->setEnabled(true);
        m_pMenuItemHero->setEnabled(false);
        m_pLayer2->showSkillDetail();
    }
    
    return true;
}

void CSKillMenuLayer::update(float time)
{
    if(CPersonalData::getInstance()->getUserInfo()->m_iSmallLevel >= 23)
    {
        if(!(CDataManager::getEndedBy(GUIDE_GIRL_HERO_SKILL)) || !(CDataManager::getEndedBy(GUIDE_BOY_HERO_SKILL)))
        {
            return;
        }
        else{
            Sprite* sp = (Sprite*)m_pMenuItemHero->getChildByTag(50);
            if(sp)
            {
                sp->removeFromParent();
                sp = NULL;
            }
        }
    }
}