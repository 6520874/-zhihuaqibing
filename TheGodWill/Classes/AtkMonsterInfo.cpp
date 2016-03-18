//
//  AtkMonsterInfo.cpp
//  WarOfGod
//
//  Created by prefer_shan on 15/4/15.
//
//

#include "AtkMonsterInfo.h"
#include "CSharedPlist.h"
#include "DataManager.h"
#include "LYPersonalData.h"
#include "LYFightData.h"

CAtkMonsterInfo::CAtkMonsterInfo()
{
    m_pListener = NULL;
    m_pTarget = NULL;
    m_pHandler = NULL;
    CSharedPlist::getInstance()->pushPlist("plist/battleResult.plist");
    
    //CSharedPlist::getInstance()->pushPlist("plist/heroCompound.plist");
}

CAtkMonsterInfo::~CAtkMonsterInfo()
{
   // CSharedPlist::getInstance()->erasePlist("plist/heroCompound.plist");
       CSharedPlist::getInstance()->erasePlist("plist/battleResult.plist");
}


void CAtkMonsterInfo::OnRedBtn(Ref *pSender)
{
    log("onRedBtn");
    removeFromParent();
    
}

void CAtkMonsterInfo::OnBlueBtn(Ref *pSender)
{

    if (m_pTarget && m_pHandler)
    {
        (m_pTarget->*m_pHandler)(pSender);
    }

//    removeFromParent();
    
    log("onBlueBtn");
    
}


bool CAtkMonsterInfo::onTouchBegan(Touch *touch, Event *event)
{
    
    return true;
}


void CAtkMonsterInfo::onTouchMoved(Touch *touch, Event *event)
{

}

void CAtkMonsterInfo::onTouchEnded(Touch *touch, Event *event)
{

}

void CAtkMonsterInfo::onTouchCancelled(Touch *touch, Event *event)
{

    
}
void CAtkMonsterInfo::onExit()
{
    _eventDispatcher->removeEventListener(m_pListener);
    
    CBaseLayer::onExit();
    
}


void  CAtkMonsterInfo::CreateActionStart(Ref * pTarget,cocos2d::SEL_MenuHandler pHandler)
{
    m_pTarget = pTarget;
    m_pHandler = pHandler;
}


bool CAtkMonsterInfo::init()
{
    
    //CBaseLayer::init();
    CBaseLayer::initWithColor(Color4B(0,0,0,255*0.8));
    
    
    m_pListener = EventListenerTouchOneByOne::create();
    m_pListener->setSwallowTouches(true);
    m_pListener->onTouchBegan = CC_CALLBACK_2(CAtkMonsterInfo::onTouchBegan, this);
    m_pListener->onTouchMoved = CC_CALLBACK_2(CAtkMonsterInfo::onTouchMoved, this);
    m_pListener->onTouchEnded = CC_CALLBACK_2(CAtkMonsterInfo::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(m_pListener,this);
    

    Sprite *pBg = Sprite::createWithSpriteFrameName("fight_bg.png");
    pBg->setPosition(m_winSize.width*0.5,m_winSize.height*0.5);
    addChild(pBg);
    
    Size tBg = pBg->getBoundingBox().size;
    //level_hsmb.png


    
    char *strTitle[7] = {"level_tdjd.png","level_hsmb.png","level_jsgw.png","level_jszd.png","level_xsch.png","level_jsboss.png",""};
    

    
    std::string strTitleLevel;
    
    LevelCondition t = CFightData::getInstance()->getTheLevelCondition();
    
    if(t == eLevelCondition_destroyCity)
    {
        strTitleLevel = strTitle[0];
        
    }
    else if(t ==  eLevelCondition_protect)
    {
        strTitleLevel = strTitle[1];
    }
    else if(t == eLevelCondition_kill)
    {
        strTitleLevel  = strTitle[2];
    }
    else if(t == eLevelCondition_defense)
    {
        strTitleLevel = strTitle[3];
    }
    else if(t == eLevelCondition_fastKill)
    {
        strTitleLevel = strTitle[4];
    }
    else if(t == eLevelCondition_killBoss)
    {
        strTitleLevel = strTitle[5];
    }
   
    
    int iLvId = CFightData::getInstance()->getNowLevelId();
   
    std::string  strIco = CDataManager::getImgByLevelId(iLvId);
    

    
    if(!strTitleLevel.empty())
    {
        Sprite * pTitle = Sprite::createWithSpriteFrameName(strTitleLevel);
        pTitle->setPosition(tBg.width*0.5,tBg.height*0.775);
        pBg->addChild(pTitle);
    }
    
    Sprite *pDLeft  = Sprite::createWithSpriteFrameName("level_huawen.png");
    pDLeft->setPosition(tBg.width*0.7,tBg.height*0.9);
    pBg->addChild(pDLeft);
    
    Sprite *pDRight = Sprite::createWithSpriteFrameName("level_huawen.png");
    pDRight->setFlippedX(true);
    pDRight->setPosition(tBg.width*0.3,tBg.height*0.9);
    pBg->addChild(pDRight);

    
    Sprite *pBoss = Sprite ::createWithSpriteFrameName(strIco);
    pBoss->setPosition(tBg.width*0.5,tBg.height*0.9);
    pBg->addChild(pBoss);
    
    Sprite *pRedBtn = Sprite::createWithSpriteFrameName("fight_redBtn.png");
    
    Sprite *pRedSelectBtn = Sprite::createWithSpriteFrameName("fight_redBtn.png");
    pRedSelectBtn->setColor(Color3B::GRAY);
    
    LYMenuItemSprite *pRedItem = LYMenuItemSprite::create(pRedBtn,pRedSelectBtn,CC_CALLBACK_1(CAtkMonsterInfo::OnRedBtn, this));
    
    Label * pLabelOK = Label::createWithTTF("返回","fonts/cuti.ttf",30);
    
    Size tRedSize = pRedItem->getBoundingBox().size;
    
    pLabelOK->setPosition(tRedSize.width*0.5,tRedSize.height*0.52);
    
    pRedItem->addChild(pLabelOK);
    
    
    Menu *pMenuRed = Menu::create(pRedItem,NULL);
    pMenuRed->setPosition(tBg.width*0.3,tBg.height*0.15);
    pBg->addChild(pMenuRed);

    Sprite *pBlueBtn = Sprite::createWithSpriteFrameName("fight_blueBtn.png");

    Sprite *pBlueSelectBtn = Sprite::createWithSpriteFrameName("fight_blueBtn.png");
    
    Sprite *pBuleSelectSec = Sprite::createWithSpriteFrameName("fight_blueBtn.png");
    
    pBuleSelectSec->setColor(Color3B::GRAY);
    pBlueSelectBtn->setColor(Color3B::GRAY);
    
    LYMenuItemSprite *pBlueItem = LYMenuItemSprite::create(pBlueBtn,pBlueSelectBtn,pBuleSelectSec,CC_CALLBACK_1(CAtkMonsterInfo::OnBlueBtn, this));

    Label * pLabelReturn = Label::createWithTTF("确定","fonts/cuti.ttf",30);
    
    Size tBlueSize = pBlueItem->getBoundingBox().size;
    pLabelReturn->setPosition(tBlueSize.width*0.5,tBlueSize.height*0.52);
    pBlueItem->addChild(pLabelReturn);

    Menu *pMenuBlue = Menu::create(pBlueItem,NULL);
    pMenuBlue->setPosition(tBg.width*0.7,tBg.height*0.15);
    pBg->addChild(pMenuBlue);
    
    
    std::vector<int> starIdVec = CDataManager::getStarConditionById(iLvId);
    
    for(int i=0;i<starIdVec.size();i++)
    {
        Sprite *pStar = Sprite::createWithSpriteFrameName("battle_star.png");
        pStar->setScale(0.5);
        pStar->setPosition(tBg.width*0.1,tBg.height*0.65-tBg.height*0.15*i);
        
        pBg->addChild(pStar);
        
        Sprite *pHengxian = Sprite::createWithSpriteFrameName("level_henxian.png");
        pHengxian->setPosition(tBg.width*0.5,tBg.height*0.575-tBg.height*0.15*i);
        pBg->addChild(pHengxian);
        
        starInfo* pInfo =  CDataManager::getStarInfoById(starIdVec.at(i));
        
        Label *pLabel = Label::createWithTTF(pInfo->starDesc,"fonts/cuti.ttf",30);
        
        pLabel->setAnchorPoint(Vec2(0,0.5));
        pLabel->setPosition(tBg.width*0.2,tBg.height*0.65-tBg.height*0.15*i);
        pBg->addChild(pLabel);
        
    }
    
    
   //levelInfo

    return true;
}

