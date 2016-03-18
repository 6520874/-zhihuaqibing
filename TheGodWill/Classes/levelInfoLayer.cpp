//
//  CLevelInfoLayer.cpp
//  TheGodWill
//
//  Created by user on 14/12/23.
//
//

#include "levelInfoLayer.h"
#include "LYFightData.h"
#include "LayerManager.h"
#include "LYPersonalData.h"
#include "DataManager.h"
#include "DBMgr.h"
#include "guiderManagerLayer.h"
#include "CSharedPlist.h"
#include "AllDefine.h"
#include "AtkMonsterInfo.h"
#include "NetWorkDataSingle.h"
#include "Common.h"
#include "DBMgr.h"
#include "TDCCVirtualCurrency.h"

#define MY_PERSION CPersonalData::getInstance()

#define SMALL_LEVEL_MAX 10 //每关卡的小关卡数
#define MID_LEVEL_MAX 7//中关卡数目

CLevelInfoLayer::CLevelInfoLayer()
{
    m_awardBg = NULL;
    _pMenu = NULL;
    m_pAcquireMenu = NULL;
    m_levelBg = NULL;
    m_curSmallLevel = 0;
    m_curMidLevel = 0;
    m_bEqlite = false;
    m_pEqliteItem = NULL;
    m_pComItem = NULL;
    m_node = NULL;
    m_pAwardLayer = NULL;
    m_pLayer = NULL;
    m_pStartMenu = NULL;
    m_pMenuItemBx1 = NULL;
    m_pMenuItemBx2 = NULL;
    m_pMenuItemBx3 = NULL;
    m_pJianliBg = NULL;
    listener = NULL;
    
    CSharedPlist::getInstance()->pushPlist("plist/level.plist");
    CSharedPlist::getInstance()->pushPlist("plist/propIcon_1.plist");
}

CLevelInfoLayer::~CLevelInfoLayer()
{
    CSharedPlist::getInstance()->erasePlist("plist/level.plist");
    CSharedPlist::getInstance()->erasePlist("plist/propIcon_1.plist");
    _eventDispatcher->removeEventListener(listener);
}

void CLevelInfoLayer::onExit()
{
    CBaseLayer::onExit();
    NotificationCenter::getInstance()->removeObserver(this, "getAward");
    NotificationCenter::getInstance()->removeObserver(this, "exitGuide");
}

bool CLevelInfoLayer::init()
{
    if (!CBaseLayer::initWithColor(Color4B(0, 0, 0, 200))) {
        return false;
    }
    m_winSize = Director::getInstance()->getWinSize();
    //触摸初始化
    listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(CLevelInfoLayer::onTouchBegan, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    if(CFightData::getInstance()->getNowMidLevelId() >= TOWER_LEVEL_INDEX)
    {
        m_levelBg = Sprite::createWithSpriteFrameName("common_bg.png");
        addChild(m_levelBg, 5);
        m_levelBg->setScaleX(1.02);
        m_levelBg->setScaleY(1.12f);
        m_levelBg->setPosition(Vec2(m_winSize.width / 2, m_winSize.height * 0.605));
        initTowerLevel();
    }
    else{
        if(MY_PERSION->getlevelMode() == 1)
        {
            m_bEqlite = true;
        }
        
        initLevelInfo();
        if(!CPersonalData::getInstance()->getisWinManual()){
            CPersonalData::getInstance()->setisWinManual(true);
            testEffect();
        }
        else{
            //检测大关卡一知否有未领取的任务
            if((CFightData::getInstance()->getNowMidLevelId() == 1) && (CPersonalData::getInstance()->getUserInfo()->m_iSmallLevel >= (CDataManager::getAllLevelById(1))) && (CDataManager::getMidLevelAwardById(1) == 0) && !(CDataManager::getEndedBy(GUIDE_GET_AWARD)))
            {
                CPersonalData::getInstance()->setisWinManual(true);
                CPersonalData::getInstance()->setisFailManual(true);
                CPersonalData::getInstance()->setisOpenNewSmallLevel(false);
                CPersonalData::getInstance()->setisOpenNewLevel(false);
                //引导士兵升级
                guiderManagerLayer::getInstance()->setGuideType(GUIDE_GET_AWARD);
                guiderManagerLayer::getInstance()->setStepNum(1);
                guiderManagerLayer::getInstance()->createLayer(this);
                NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(CLevelInfoLayer::addGetAwardGuide), "getAward", NULL);
            }
        }
    }
    bool bExistedTask = false;//判断是否存在未领取的日常任务
    std::vector<TaskBase *>tempVec = CDataManager::GetAllTaskInfo();
    for(int i = 0; i < tempVec.size(); i++)
    {
        if((tempVec.at(i)->taskProp == 1) && (tempVec.at(i)->m_eTaskState == Task_Get))
        {
            bExistedTask = true;
            break;
        }
    }
    if(bExistedTask && !(CDataManager::getEndedBy(GUIDE_LACK_STAMINA)) && (CPersonalData::getInstance()->GetPhysicalNum() < 6))
    {
        NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(CLevelInfoLayer::addExitGuide), "exitGuide", NULL);
        guiderManagerLayer::getInstance()->setGuideType(GUIDE_LACK_STAMINA);
        guiderManagerLayer::getInstance()->setStepNum(1);
        guiderManagerLayer::getInstance()->createLayer(this);
    }
    return true;

}

void CLevelInfoLayer::addExitGuide(Ref* pSender)
{
    NotificationCenter::getInstance()->removeObserver(this, "exitGuide");
    Menu* pMenu = (Menu*)m_pLayer->getChildByTag(LEVELINFO_EXIT);
    Vec2 pos = pMenu->convertToWorldSpace(Vec2::ZERO);
    guiderManagerLayer::getInstance()->createLayer(this, GUIDE_SHOW_FINGER, pos);
}

void CLevelInfoLayer::addGetAwardGuide(Ref* pSender)
{
    NotificationCenter::getInstance()->removeObserver(this, "getAward");
    Vec2 pos = m_pAcquireMenu->convertToWorldSpace(Vec2::ZERO);//m_pAcquireMenu->getChildByTag(LEVELINFO_ACQUIRE);
    guiderManagerLayer::getInstance()->createLayer(this, GUIDE_SHOW_FINGER, pos);
}

void CLevelInfoLayer::initTowerLevel()
{
    Size bgSize = m_levelBg->getBoundingBox().size;
    LayerColor* pLayer = LayerColor::create(Color4B(200, 0, 0, 0), bgSize.width, bgSize.height);
    addChild(pLayer, 5);
    pLayer->setPosition(Vec2(m_levelBg->getPositionX() - bgSize.width / 2, m_levelBg->getPositionY() - bgSize.height / 2));
    
    m_curSmallLevel = m_curMidLevel = CFightData::getInstance()->getNowMidLevelId();
    midLevelInfo* levelInfo = CDataManager::getMidLevelInfoById(m_curMidLevel);//中关卡信息
    
    //退出按钮
    Sprite* normal = Sprite::createWithSpriteFrameName("closeBtn.png");
    Sprite* select = Sprite::createWithSpriteFrameName("closeBtn.png");
    select->setColor(Color3B::GRAY);
    LYMenuItemSprite* exitItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(CLevelInfoLayer::towerBtnCallBack, this));
    exitItem->setTag(LEVELINFO_EXIT);
    CommonData::enlargeItemSprite(exitItem, 2);
    Menu* pMenu = Menu::create(exitItem, NULL);
    pMenu->setPosition(Vec2(bgSize.width * 0.96, bgSize.height * 0.87f));
    pLayer->addChild(pMenu);

    
    Label* pTitle = Label::createWithSystemFont(levelInfo->name.c_str(), "Arial", 32);
    pLayer->addChild(pTitle);
    pTitle->setPosition(Vec2(bgSize.width * 0.5f, bgSize.height * 0.895f));
    
    Label* pIntroduce = Label::createWithSystemFont("介绍:", "Arial", 30);
    pLayer->addChild(pIntroduce);
    pIntroduce->setPosition(Vec2(bgSize.width * 0.092f, bgSize.height * 0.81f));
    
    Sprite* leftTop = Sprite::createWithSpriteFrameName("tower_introduce_1.png");
    pLayer->addChild(leftTop);
    leftTop->setPosition(Vec2(bgSize.width * 0.29f, bgSize.height * 0.645f));
    
    Sprite* leftTopPoint = Sprite::createWithSpriteFrameName("levle_point.png");
    pLayer->addChild(leftTopPoint);
    leftTopPoint->setPosition(Vec2(bgSize.width * 0.105f, bgSize.height * 0.485f));
    
    Label* leftIntroduce_1 = Label::createWithSystemFont("画 \"", "Aria", 24);
    pLayer->addChild(leftIntroduce_1);
    leftIntroduce_1->setAnchorPoint(Vec2(0, 0.5f));
    leftIntroduce_1->setPosition(Vec2(bgSize.width * 0.125f, bgSize.height * 0.485f));
    
    Sprite* check = Sprite::createWithSpriteFrameName("level_check.png");
    pLayer->addChild(check);
    check->setAnchorPoint(Vec2(0, 0.5f));
    check->setPosition(Vec2(leftIntroduce_1->getPositionX() + leftIntroduce_1->getContentSize().width , bgSize.height * 0.48f));
    
    Label* leftIntroduce_2 = Label::createWithSystemFont(" \"来建造防御建筑", "Aria", 24);
    pLayer->addChild(leftIntroduce_2);
    leftIntroduce_2->setAnchorPoint(Vec2(0, 0.5f));
    leftIntroduce_2->setPosition(Vec2(check->getPositionX() + check->getContentSize().width , bgSize.height * 0.485f));
    
    Sprite* rightTop = Sprite::createWithSpriteFrameName("tower_introduce_2.png");
    pLayer->addChild(rightTop);
    rightTop->setPosition(Vec2(bgSize.width * 0.71f, bgSize.height * 0.645f));
    
    Sprite* rightTopPoint = Sprite::createWithSpriteFrameName("levle_point.png");
    pLayer->addChild(rightTopPoint);
    rightTopPoint->setPosition(Vec2(bgSize.width * 0.525f, bgSize.height * 0.485f));
    
    Label* rightTopIntroduce = Label::createWithSystemFont("选择想要升级的建筑类型", "Aria", 24);
    pLayer->addChild(rightTopIntroduce);
    rightTopIntroduce->setAnchorPoint(Vec2(0, 0.5f));
    rightTopIntroduce->setPosition(Vec2(bgSize.width * 0.545f, bgSize.height * 0.485f));
    
    Sprite* leftBottom = Sprite::createWithSpriteFrameName("tower_introduce_3.png");
    pLayer->addChild(leftBottom);
    leftBottom->setPosition(Vec2(bgSize.width * 0.29f, bgSize.height * 0.31f));
    
    Sprite* leftBottomPoint = Sprite::createWithSpriteFrameName("levle_point.png");
    pLayer->addChild(leftBottomPoint);
    leftBottomPoint->setPosition(Vec2(bgSize.width * 0.105f, bgSize.height * 0.15f));
    
    Label* leftBottomIntrd_1 = Label::createWithSystemFont("通过击杀怪来获得建造建筑的能量", "Aria", 24, Size(leftBottom->getContentSize().width, 80));
    pLayer->addChild(leftBottomIntrd_1);
    leftBottomIntrd_1->setAnchorPoint(Vec2(0, 0.5f));
    leftBottomIntrd_1->setPosition(Vec2(bgSize.width * 0.125f, bgSize.height * 0.11f));
    
    Sprite* rightBottom = Sprite::createWithSpriteFrameName("tower_introduce_4.png");
    pLayer->addChild(rightBottom);
    rightBottom->setPosition(Vec2(bgSize.width * 0.71f, bgSize.height * 0.31f));
    
    Sprite* rightBottomPoint = Sprite::createWithSpriteFrameName("levle_point.png");
    pLayer->addChild(rightBottomPoint);
    rightBottomPoint->setPosition(Vec2(bgSize.width * 0.525f, bgSize.height * 0.15f));
    
    Label* rightBottomIntrd = Label::createWithSystemFont("逃脱一定数量的士兵会失败", "Aria", 24, Size(leftBottom->getContentSize().width, 80));
    pLayer->addChild(rightBottomIntrd);
    rightBottomIntrd->setAnchorPoint(Vec2(0, 0.5f));
    rightBottomIntrd->setPosition(Vec2(bgSize.width * 0.545f, bgSize.height * 0.11f));
    
    //奖励
    Sprite* awardBg = Sprite::createWithSpriteFrameName("dikuang.png");
    addChild(awardBg, 4);
    awardBg->setScaleX(2.0f);
    awardBg->setScaleY(1.6f);
    awardBg->setPosition(Vec2(m_winSize.width * 0.5f, m_winSize.height * 0.115f));
    Size AwardSize = awardBg->getBoundingBox().size;
    
    m_pAwardLayer = LayerColor::create(Color4B(100, 0, 0, 0), AwardSize.width, AwardSize.height);
    addChild(m_pAwardLayer, 4);
    m_pAwardLayer->setPosition(Vec2(awardBg->getPositionX() - AwardSize.width / 2, awardBg->getPositionY() - AwardSize.height / 2));
    
    Label* award = Label::createWithSystemFont("奖励 : ", "Arial", 30);
    m_pAwardLayer->addChild(award);
    award->setColor(Color3B(200,79,15));
    award->setAnchorPoint(Vec2(0, 0.5));
    award->setPosition(Vec2(AwardSize.width * 0.05f, AwardSize.height * 0.65f));
    
    Sprite* coin = Sprite::createWithSpriteFrameName("single_coin.png");
    m_pAwardLayer->addChild(coin);
    coin->setScale(0.65);
    coin->setAnchorPoint(Vec2(0, 0.5));
    coin->setPosition(Vec2(award->getPositionX() + award->getContentSize().width + 10, award->getPositionY()));
    
    char buf[128];
    struct levelInfo* pSmallLevelInfo = CDataManager::getTheLevelInfoWithId(m_curMidLevel);
    sprintf(buf, "%d", pSmallLevelInfo->goldNum);
    Label* coinNum = Label::createWithSystemFont(buf, "Arial", 30);
    m_pAwardLayer->addChild(coinNum);
    coinNum->setColor(Color3B::RED);
    coinNum->setAnchorPoint(Vec2(0, 0.5));
    coinNum->setPosition(Vec2(coin->getPositionX() + coin->getContentSize().width * 0.65f + 10, award->getPositionY()));
    
    Label* propAward = Label::createWithSystemFont("有几率获得 : ", "Arial", 30);
    m_pAwardLayer->addChild(propAward);
    propAward->setColor(Color3B(200,79,15));
    propAward->setAnchorPoint(Vec2(0, 0.5));
    propAward->setPosition(Vec2(award->getPositionX(), AwardSize.height * 0.3f));
    
    std::vector<smallAwardInfo> itemsVec = pSmallLevelInfo->itemsVec;
    if(itemsVec.size() > 0){
        float xPos = propAward->getPositionX() + propAward->getContentSize().width + 10;
        for (int i = 0; i < itemsVec.size(); i++)
        {
            if (itemsVec.at(i).type == 0)
            {//物品
                propInfo* prop = CDataManager::getPropInfoById(itemsVec.at(i).Id);
                Sprite* sp = Sprite::createWithSpriteFrameName(prop->m_sItemIcon.c_str());
                m_pAwardLayer->addChild(sp);
                sp->setScale(0.7f);
                sp->setAnchorPoint(Vec2(0, 0.5));
                sp->setPosition(xPos, propAward->getPositionY());
                xPos += sp->getBoundingBox().size.width + 15;
            }
            else
            {
                monsterInfo* monster = CDataManager::getMonsterInfoByMonsterId(itemsVec.at(i).Id);
                Sprite* sp = Sprite::createWithSpriteFrameName(monster->monsterIcon.c_str());
                m_pAwardLayer->addChild(sp);
                sp->setScale(0.5f);
                sp->setAnchorPoint(Vec2(0, 0.5));
                sp->setPosition(xPos, propAward->getPositionY());
                xPos += sp->getBoundingBox().size.width + 15;
            }
        }
    }
    addMenu();
}

void CLevelInfoLayer::addMenu()
{
    char buf[128];
    Size awardSize = m_pAwardLayer->getContentSize();
    levelInfo* pSmallLevelInfo = CDataManager::getTheLevelInfoWithId(m_curMidLevel);
    Sprite* normal = Sprite::createWithSpriteFrameName("yellowan.png");
    Sprite* select = Sprite::createWithSpriteFrameName("yellowan.png");
    select->setColor(Color3B::GRAY);
    LYMenuItemSprite* startItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(CLevelInfoLayer::towerBtnCallBack, this),"music/fight.mp3");
    startItem->setTag(LEVELINFO_START);
    startItem->setScale(1.85f);
    m_pStartMenu = Menu::create(startItem, NULL);
    m_pAwardLayer->addChild(m_pStartMenu);
    m_pStartMenu->setPosition(Vec2(awardSize.width * 0.82f, awardSize.height * 0.48));
    
    sprintf(buf,"%d",pSmallLevelInfo->stamina);
    Label* staminaNum = Label::createWithSystemFont(buf, "Arial", 30);
    startItem->addChild(staminaNum);
    staminaNum->setScale(0.55f);
    staminaNum->setColor(Color3B::BLUE);
    staminaNum->setPosition(Vec2(normal->getContentSize().width * 0.18f, normal->getContentSize().height * 0.585));
    
    Sprite* stamina = Sprite::createWithSpriteFrameName("stamina.png");
    stamina->setScale(0.42f);
    startItem->addChild(stamina);
    stamina->setPosition(Vec2(normal->getContentSize().width * 0.34f, staminaNum->getPositionY()));
    
    //分割线
    Sprite* line = Sprite::createWithSpriteFrameName("btn_line.png");
    startItem->addChild(line);
    line->setScale(0.5f);
    line->setPosition(Vec2(normal->getContentSize().width * 0.47f, staminaNum->getPositionY()));
    
    Label* fightLabel = Label::createWithSystemFont("出战", "Arial", 35);
    startItem->addChild(fightLabel);
    fightLabel->setScale(0.5f);
    fightLabel->setColor(Color3B::BLUE);
    fightLabel->setPosition(Vec2(normal->getContentSize().width * 0.68f, line->getPositionY()));
}

void CLevelInfoLayer::towerBtnCallBack(Ref* pSender)
{
    int tag = ((Node*)pSender)->getTag();
    if (tag == LEVELINFO_EXIT)
    {
        removeFromParent();
    }
    else{
        
         //spf
        levelInfo* pSmallLevelInfo = CDataManager::getTheLevelInfoWithId(m_curSmallLevel);
        //判断体力是否足够
        int needStaina = pSmallLevelInfo->stamina;
        if(needStaina > CPersonalData::getInstance()->GetPhysicalNum())
        {
            SimpleAudioEngine::getInstance()->playEffect("music/noBtn.mp3");
            Sprite * thetarget = CFightManager::getTheFlyWord("体力不足","fonts/cuti.ttf");
            thetarget->setPosition(Vec2(m_winSize.width / 2, m_winSize.height / 2));
            addChild(thetarget, 10);
        }
        else{
            CDataManager::saveUserTowerLevelInfoToDB(m_curSmallLevel);
            CFightData::getInstance()->newLevelStart(m_curSmallLevel);
            CFightData::getInstance()->setNowLevelId(m_curSmallLevel);
            CFightData::getInstance()->setFightType(FIGHT_TOWERDENFESE_TYPE);
            CLayerManager::GetSingletonPtr()->createLayerWithLoading(eLayer_Fight, NULL,false);
        }
    }
}

bool CLevelInfoLayer::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    if(m_pJianliBg)
    {
        m_pJianliBg->removeFromParent();
        m_pJianliBg = NULL;
    }

    return true;
}

void CLevelInfoLayer::initLevelInfo()
{
    if(m_levelBg)
    {
        m_levelBg->removeFromParent();
        m_levelBg = NULL;
    }
    m_levelBg = Sprite::createWithSpriteFrameName("common_bg.png");
    if(m_bEqlite)
    {
        m_levelBg = Sprite::createWithSpriteFrameName("eqliten_bg.png");
    }
    addChild(m_levelBg, 5);
    m_levelBg->setScaleX(1.02);
    m_levelBg->setScaleY(1.05f);
    m_levelBg->setPosition(Vec2(m_winSize.width / 2, m_winSize.height * 0.6));
    if (m_pLayer) {
        m_pLayer->removeAllChildren();
    }
    
    
    Size bgSize = m_levelBg->getBoundingBox().size;
    m_pLayer = LayerColor::create(Color4B(200, 0, 0, 0), bgSize.width, bgSize.height);
    addChild(m_pLayer, 5);
    m_pLayer->setPosition(Vec2(m_levelBg->getPositionX() - bgSize.width / 2, m_levelBg->getPositionY() - bgSize.height / 2));
    
    m_smallLevel = CPersonalData::getInstance()->getUserInfo()->m_iSmallLevel;
    if(m_bEqlite){
        m_smallLevel = CPersonalData::getInstance()->getUserInfo()->m_iEqliteSmallLevel;
    }
    m_curMidLevel = CFightData::getInstance()->getNowMidLevelId();
    
    m_curSmallLevel = m_smallLevel + 1;//待测
    //关卡名字
    midLevelInfo* levelInfo = CDataManager::getMidLevelInfoById(m_curMidLevel);//中关卡信息
    Label* name = Label::createWithTTF(levelInfo->name.c_str(), "fonts/cuti.ttf", 30);
    m_pLayer->addChild(name);
    name->setPosition(Vec2(bgSize.width / 2, bgSize.height * 0.892));
    _pMenu = Menu::create();
    m_pLayer->addChild(_pMenu);
    _pMenu->setPosition(Vec2::ZERO);
    //小关卡_pMenu
    int index = 0;
    for(int i = levelInfo->firstLevel - 1; i < levelInfo->lastLevel; i++,index++)
    {
        //关卡背景
        Point pos = Vec2(bgSize.width * 0.16f + 145 * (index%5), bgSize.height * 0.625 - (index/5) * 193);
        addSmallLevel(i + 1, _pMenu, pos);
    }
    if(m_curSmallLevel > levelInfo->lastLevel)
    {
        m_curSmallLevel = levelInfo->lastLevel;
        LYMenuItemSprite* tempItem = (LYMenuItemSprite*)(_pMenu->getChildByTag(m_curSmallLevel));
        if (tempItem) {
            Sprite* sp = Sprite::createWithSpriteFrameName("level_block_2.png");
            tempItem->setNormalImage(sp);
        }
    }
    //领取按钮
    Sprite* normal = Sprite::createWithSpriteFrameName("greenbtn.png");
    Sprite* select = Sprite::createWithSpriteFrameName("greenbtn.png");
    select->setColor(Color3B::GRAY);
    LYMenuItemSprite* acquireItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(CLevelInfoLayer::btnCallBack, this), "music/task.mp3");
    acquireItem->setScale(1.85f);
    acquireItem->setTag(LEVELINFO_ACQUIRE);
    m_pAcquireMenu = Menu::create(acquireItem, NULL);
    m_pAcquireMenu->setPosition(Vec2(bgSize.width * 0.83f, bgSize.height * 0.115));
    m_pLayer->addChild(m_pAcquireMenu);
    //m_pAcquireMenu->setEnabled(false);
    m_pAcquireMenu->setColor(Color3B::GRAY);
    Label* word = Label::createWithSystemFont("领 取", "Arial", 40);
    acquireItem->addChild(word);
    word->setScale(0.3f);
    word->setPosition(Vec2(normal->getContentSize().width / 2, normal->getContentSize().height / 2));
    
    //退出按钮
    normal = Sprite::createWithSpriteFrameName("closeBtn.png");
    select = Sprite::createWithSpriteFrameName("closeBtn.png");
    select->setColor(Color3B::GRAY);
    LYMenuItemSprite* exitItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(CLevelInfoLayer::btnCallBack, this));
    CommonData::enlargeItemSprite(exitItem, 2);
    exitItem->setTag(LEVELINFO_EXIT);
    Menu* pExitMenu = Menu::create(exitItem, NULL);
    pExitMenu->setPosition(Vec2(bgSize.width * 0.97f, bgSize.height * 0.87f));
    m_pLayer->addChild(pExitMenu, 1, LEVELINFO_EXIT);
    
    normal = Sprite::createWithSpriteFrameName("commonBtn1.png");
    select = Sprite::createWithSpriteFrameName("commonBtn1.png");
    
    m_pComItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(CLevelInfoLayer::btnCallBack, this));
    
    Menu* pComMenu = Menu::create(m_pComItem, NULL);
    pComMenu->setPosition(Vec2::ZERO);
    m_pComItem->setTag(LEVELINFO_COMMON);
    m_pLayer->addChild(pComMenu);
    m_pComItem->setPosition(Vec2(bgSize.width * 0.15, bgSize.height * 0.885));
    m_pComItem->setEnabled(false);
    
    normal = Sprite::createWithSpriteFrameName("eqliteBtn1.png");
    select = Sprite::createWithSpriteFrameName("eqliteBtn1.png");
    m_pEqliteItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(CLevelInfoLayer::btnCallBack, this));
    select->setAnchorPoint(Vec2(0.5f, 0.5f));
    Menu* pEqliteMenu= Menu::create(m_pEqliteItem, NULL);
    m_pEqliteItem->setTag(LEVELINFO_EQLITE);
    pEqliteMenu->setPosition(Vec2::ZERO);
    m_pLayer->addChild(pEqliteMenu);
    m_pEqliteItem->setPosition(Vec2(bgSize.width * 0.82, bgSize.height * 0.885));
    m_pEqliteItem->setEnabled(false);
    
    
    if (!m_bEqlite) {
        m_pEqliteItem->setEnabled(true);
        Sprite* normal = Sprite::createWithSpriteFrameName("eqliteBtn1.png");
        Sprite* select = Sprite::createWithSpriteFrameName("eqliteBtn.png");
        m_pEqliteItem->setNormalImage(normal);
        m_pEqliteItem->setSelectedImage(select);
        
        normal = Sprite::createWithSpriteFrameName("commonBtn.png");
        select = Sprite::createWithSpriteFrameName("commonBtn1.png");
        m_pComItem->setNormalImage(normal);
        m_pComItem->setSelectedImage(select);
    }
    else{
        m_pComItem->setEnabled(true);
        Sprite* normal = Sprite::createWithSpriteFrameName("eqliteBtn.png");
        Sprite* select = Sprite::createWithSpriteFrameName("eqliteBtn1.png");
        m_pEqliteItem->setNormalImage(normal);
        m_pEqliteItem->setSelectedImage(select);
        
        normal = Sprite::createWithSpriteFrameName("commonBtn1.png");
        select = Sprite::createWithSpriteFrameName("commonBtn.png");
        m_pComItem->setNormalImage(normal);
        m_pComItem->setSelectedImage(select);
    }
    reFreshLevelReawrd();
    if(m_curSmallLevel > 0){
        levelPanel();
    }
    
    if(((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_UPGRADE_SOLIDER) && (guiderManagerLayer::getInstance()->getStepNum() == 9))
       || ((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SKILL_UPGRADE) && (guiderManagerLayer::getInstance()->getStepNum() == 12)) || ((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_GET_TASK) && (guiderManagerLayer::getInstance()->getStepNum() == 5))
       || ((guiderManagerLayer::getInstance()->getStepNum() == 6) && (guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SOLIDER_NUM)))
    {
        //显示指引关卡
        guiderManagerLayer::getInstance()->setStepNum(guiderManagerLayer::getInstance()->getStepNum() + 1);
        Vec2 pos = m_pStartMenu->convertToWorldSpace(Vec2::ZERO);
        guiderManagerLayer::getInstance()->createLayer(this, GUIDE_SHOW_FINGER, pos);
    }
}

void CLevelInfoLayer::OnOpenBx(Ref *pSender)
{
    if(m_pJianliBg)
    {
        m_pJianliBg->removeFromParent();
        m_pJianliBg = NULL;
    }
    
    int iReceiveNum = CDataManager::getMidLevelAwardById(m_curMidLevel, m_bEqlite);
    
    Size bgSize = m_levelBg->getBoundingBox().size;
    
    MenuItemSprite *pItem = (MenuItemSprite*)pSender;

    int iNum =  pItem->getTag();
    std::string strText;
    
    if(iReceiveNum>=(iNum+1))
    {
        Sprite * thetarget = CFightManager::getTheFlyWord("已领取","fonts/cuti.ttf");
        thetarget->setPosition(m_winSize.width*0.5,m_winSize.height*0.5);
        addChild(thetarget,99);
        return;
    }
    
    midLevelInfo* levelInfo = CDataManager::getMidLevelInfoById(m_curMidLevel);//中关卡信息
    std::vector<awardInfo> awardVec = levelInfo->awardVec;//当前关卡
    
    if(m_bEqlite)
    {
        awardVec = levelInfo->heroAwardVec;
    }
    
    Sprite *pRwewardSp = NULL;

    char buf[128] = {0};

    Label *pPropDesc = NULL;
    std::string strDesc;
    
    m_pJianliBg = Sprite::createWithSpriteFrameName("detail.png");
    m_pJianliBg->cocos2d::Node::setScale(2);
    m_pJianliBg->setOpacity(255*0.9);
    m_node->addChild(m_pJianliBg);
    
    Size tJianliBg = m_pJianliBg->getBoundingBox().size;
    
    if(awardVec.at(iNum).type == 0)
    {//道具
        //根绝道具id获取道具信息
        propInfo* pInfo = CDataManager::getPropInfoById(awardVec.at(iNum).PropId);
        strDesc = pInfo->m_sItemName;
        pRwewardSp = Sprite::createWithSpriteFrameName(pInfo->m_sItemIcon.c_str());
        pRwewardSp->setAnchorPoint(Vec2(1, 0.5));
        m_pJianliBg->addChild(pRwewardSp,1);
        pRwewardSp->cocos2d::Node::setScale(0.5*0.8*2);
        pRwewardSp->setPosition(tJianliBg.width*0.3,tJianliBg.height*0.3);
        
        sprintf(buf, "X %d", awardVec.at(iNum).num);
        strDesc += buf;
        
        pPropDesc = Label::createWithTTF(strDesc,"fonts/cuti.ttf",20);
       
        pPropDesc->setPosition(tJianliBg.width*0.25,tJianliBg.height*0.1);
        m_pJianliBg->addChild(pPropDesc,1);
    }
    else if(awardVec.at(iNum).type == 1)
    {//金币
        
        pRwewardSp = Sprite::createWithSpriteFrameName("coin.png");
        pRwewardSp->setScale(0.9*0.5*2);
        pRwewardSp->setAnchorPoint(Vec2(1, 0.5));
        pRwewardSp->setPosition(tJianliBg.width*0.3,tJianliBg.height*0.3);
        m_pJianliBg->addChild(pRwewardSp,1);
        
        strDesc = "金币";
        sprintf(buf, "X %d", awardVec.at(iNum).num);
        strDesc += buf;
        
        pPropDesc = Label::createWithTTF(strDesc,"fonts/cuti.ttf",20);
        pPropDesc->setPosition(tJianliBg.width*0.25,tJianliBg.height*0.1);
        m_pJianliBg->addChild(pPropDesc,1);
    
    }
    else if(awardVec.at(iNum).type == 2)
    {//魔法石
        pRwewardSp = Sprite::createWithSpriteFrameName("stone.png");
        pRwewardSp->setScale(0.58f*0.5*1.5);
        pRwewardSp->setAnchorPoint(Vec2(1, 0.5));
        pRwewardSp->setPosition(tJianliBg.width*0.3,tJianliBg.height*0.3);
        m_pJianliBg->addChild(pRwewardSp,1);
        
        strDesc = "魔法石";
        sprintf(buf, "X %d", awardVec.at(iNum).num);
        strDesc += buf;

        pPropDesc = Label::createWithTTF(strDesc,"fonts/cuti.ttf",20);
        pPropDesc->setPosition(tJianliBg.width*0.25,tJianliBg.height*0.1);
        m_pJianliBg->addChild(pPropDesc,1);
        
    }
     pPropDesc->setScale(0.75);
    
    float fy = 0.3f;
    
    if(iNum==0)
    {
        m_pJianliBg->setPosition(bgSize.width*0.45,bgSize.height*fy);
    }
    else if(iNum==1)
    {
        m_pJianliBg->setPosition(bgSize.width*0.6,bgSize.height*fy);
    }
    else if(iNum == 2)
    {
       m_pJianliBg->setPosition(bgSize.width*0.7,bgSize.height*fy);
    }
    
}


void CLevelInfoLayer::reFreshLevelReawrd()
{
    if (m_node && m_pLayer)
    {
        m_pLayer->removeChildByTag(100);
        m_node = NULL;
    }
    
    m_node = Node::create();
    m_pLayer->addChild(m_node, 1, 100);
    m_node->setPosition(Vec2::ZERO);
    char buf[128];
    int starLimit[3] = {12, 24, 30};
    
    Sprite* normal= Sprite::createWithSpriteFrameName("greenbtn.png");
    normal->setScale(0.65);
    Size bgSize = m_levelBg->getBoundingBox().size;
    midLevelInfo* levelInfo = CDataManager::getMidLevelInfoById(m_curMidLevel);//中关卡信息
    
    //获取用户当前关卡的奖励领取次数
    int receiveNum = CDataManager::getMidLevelAwardById(m_curMidLevel, m_bEqlite);
    
    std::vector<awardInfo> awardVec = levelInfo->awardVec;//当前关卡

    if(m_bEqlite)
    {
        awardVec = levelInfo->heroAwardVec;
    }
    
    Sprite* rewardSp = NULL;
    Label* rewardNum = NULL;
    
    if ((receiveNum < 3) && (receiveNum >= 0))
    {
        sprintf(buf, "X %d", awardVec.at(receiveNum).num);
        rewardNum = Label::createWithSystemFont(buf, "Arial", 30);
        
        rewardNum->setAnchorPoint(Vec2(1, 0.5));
        m_node->addChild(rewardNum);
        rewardNum->setColor(Color3B::YELLOW);
        rewardNum->setPosition(Vec2(m_pAcquireMenu->getPositionX() - normal->getContentSize().width, bgSize.height * 0.12f));
        
        if(awardVec.at(receiveNum).type == 0)
        {//道具
            //根绝道具id获取道具信息
            propInfo* pInfo = CDataManager::getPropInfoById(awardVec.at(receiveNum).PropId);
            rewardSp = Sprite::createWithSpriteFrameName(pInfo->m_sItemIcon.c_str());
            m_node->addChild(rewardSp);
            rewardSp->setAnchorPoint(Vec2(1, 0.5));
            rewardSp->setPosition(Vec2(rewardNum->getPositionX() - rewardNum->getBoundingBox().size.width , rewardNum->getPositionY()));
        }
        else if(awardVec.at(receiveNum).type == 1)
        {//金币
            
            rewardSp = Sprite::createWithSpriteFrameName("coin.png");
            m_node->addChild(rewardSp);
            rewardSp->setScale(0.9);
            rewardSp->setAnchorPoint(Vec2(1, 0.5));
            rewardSp->setPosition(Vec2(rewardNum->getPositionX() - rewardNum->getContentSize().width - 10, rewardNum->getPositionY() - 1));
        }
        else if(awardVec.at(receiveNum).type == 2)
        {//魔法石
            
            rewardSp = Sprite::createWithSpriteFrameName("stone.png");
            m_node->addChild(rewardSp);
            rewardSp->setScale(0.58f);
            rewardSp->setAnchorPoint(Vec2(1, 0.5));
            rewardNum->setVisible(false);
            }
        
        rewardSp->setVisible(false);
        rewardNum->setVisible(false);
        
    
        Sprite * pAward = Sprite::createWithSpriteFrameName("bx_awd.png");
        m_node->addChild(pAward);
        pAward->setPosition(Vec2(150, rewardNum->getPositionY()));
        
        
        Sprite *pKuang = Sprite::createWithSpriteFrameName("bx_kuang.png");
        pKuang->setPosition(Vec2(pAward->getPositionX()+pAward->getContentSize().width*0.6+pKuang->getContentSize().width*0.5,rewardNum->getPositionY()));
        m_node->addChild(pKuang,0,100);
        
        Size tKuangBg =  pKuang->getBoundingBox().size;
        
        //进度条底
        Sprite * pProgressBg = Sprite::createWithSpriteFrameName("bx_tiao1.png");
        pProgressBg->setPosition(tKuangBg.width*0.5,tKuangBg.height*0.5);
        pKuang->addChild(pProgressBg,-2);
        
        Size tProgressBg =  pKuang->getBoundingBox().size;
        Sprite *progress = Sprite::createWithSpriteFrameName("bx_tiao2.png");
        
        ProgressTimer * m_pProgress2 = NULL;
        m_pProgress2 = ProgressTimer::create(progress);
        m_pProgress2->setType( ProgressTimer::Type::BAR);
        m_pProgress2->setPosition(Vec2(tKuangBg.width*0.5,tKuangBg.height*0.5));
        
        //进度动画运动方向，可以多试几个值，看看效果
        m_pProgress2->setMidpoint(Point(0,0));

        //进度条宽高变化
        m_pProgress2->setBarChangeRate(Point(1, 0));
        
        int iStarALl = CDBMgr::getInstance()->getAllLevelStarById(m_curMidLevel,m_bEqlite);

        float fPer = float(iStarALl)/30*100;

        m_pProgress2->setPercentage(fPer);
        pKuang->addChild(m_pProgress2,-1);
        
        char strStar[10] = {0};
        sprintf(strStar,"%d",iStarALl);
    
        Label *pStarInfo = Label::createWithTTF(strStar,"fonts/cuti.ttf",25);
        pStarInfo->setPosition(tKuangBg.width*0.1,tKuangBg.height*0.5);
        pKuang->addChild(pStarInfo,2);
        
        Sprite *pStar = Sprite::createWithSpriteFrameName("star.png");
        pStar->setScale(0.7);
        pStar->setPosition(tKuangBg.width*0.15,tKuangBg.height*0.5);
        pKuang->addChild(pStar,2);
        
        //进度条

        int sum = CDataManager::getAllLevelStarById(m_curMidLevel,m_bEqlite);
        
        
        for(int i=0;i<3;i++)
        {
            std::string strBx = "bx_close.png";
    
            if(receiveNum >i)
            {
                strBx = "bx_open.png";
            }
         
            Sprite *pBxNormal = Sprite::createWithSpriteFrameName(strBx);
            Sprite *pBxSelect  = Sprite::createWithSpriteFrameName(strBx);
            pBxSelect->setColor(Color3B::GRAY);
            
            
            float fjianGe = 0.8f;

            if(i==0)
            {
                m_pMenuItemBx1 = MenuItemSprite::create(pBxNormal,pBxSelect,CC_CALLBACK_1(CLevelInfoLayer::OnOpenBx, this));
 
               if(receiveNum <=i&&sum>=12)
               {
                m_pMenuItemBx1->runAction(RepeatForever::create(Sequence::create(RotateBy::create(0.1f,-10),RotateBy::create(0.1f,20),RotateBy::create(0.1f,-10)
                                                                                 ,RotateBy::create(0.1f,-10),RotateBy::create(0.1f,20),RotateBy::create(0.1f,-10),DelayTime::create(fjianGe),NULL)));
               }
                m_pMenuItemBx1->setTag(i);
                Menu *pMenuBx = Menu::create(m_pMenuItemBx1,NULL);
                pMenuBx->setAnchorPoint(Vec2(0.5,0));
                
                pMenuBx->setPosition(150+m_pMenuItemBx1->getBoundingBox().size.width*2*i,tKuangBg.height*0.7);
                pKuang->addChild(pMenuBx,2,100);
                
                Label *pNeedStar = Label::createWithTTF("12","fonts/cuti.ttf",25);
                pNeedStar->setPosition(150+m_pMenuItemBx1->getBoundingBox().size.width*2*i,-tKuangBg.height*0.19);
                pKuang->addChild(pNeedStar,2);
                
                Sprite *pStar = Sprite::createWithSpriteFrameName("star.png");
                pStar->cocos2d::Node::setScale(0.7);
                pStar->setPosition(pNeedStar->getPositionX()+pNeedStar->getBoundingBox().size.width*1.1,-tKuangBg.height*0.19);
                pKuang->addChild(pStar,2);
                
                
            }
            else if(i==1)
            {
                m_pMenuItemBx2 = MenuItemSprite::create(pBxNormal,pBxSelect,CC_CALLBACK_1(CLevelInfoLayer::OnOpenBx, this));
                m_pMenuItemBx2->setTag(i);
                
                if(receiveNum <=i&&sum>=24)
                {
                 m_pMenuItemBx2->runAction(RepeatForever::create(Sequence::create(RotateBy::create(0.1f,-10),RotateBy::create(0.1f,20),RotateBy::create(0.1f,-10),RotateBy::create(0.1f,-10),RotateBy::create(0.1f,20),RotateBy::create(0.1f,-10),DelayTime::create(fjianGe),NULL)));
                }
                
                Menu *pMenuBx = Menu::create(m_pMenuItemBx2,NULL);
                pMenuBx->setAnchorPoint(Vec2(0.5,0));
                pMenuBx->setPosition(150+m_pMenuItemBx2->getBoundingBox().size.width*2.5*i,tKuangBg.height*0.675);
                pKuang->addChild(pMenuBx,2,100);
                
                Label *pNeedStar = Label::createWithTTF("24","fonts/cuti.ttf",25);
                pNeedStar->setPosition(150+m_pMenuItemBx2->getBoundingBox().size.width*2.5*i,-tKuangBg.height*0.19);
                pKuang->addChild(pNeedStar,2);
                
                Sprite *pStar = Sprite::createWithSpriteFrameName("star.png");
                pStar->cocos2d::Node::setScale(0.7);
                pStar->setPosition(pNeedStar->getPositionX()+pNeedStar->getBoundingBox().size.width,-tKuangBg.height*0.19);
                pKuang->addChild(pStar,2);
                
                
            }
            else if(i==2)
            {
                m_pMenuItemBx3 = MenuItemSprite::create(pBxNormal,pBxSelect,CC_CALLBACK_1(CLevelInfoLayer::OnOpenBx, this));
                m_pMenuItemBx3->setTag(i);
                if(receiveNum <=i&&sum>=30)
                {
                 m_pMenuItemBx3->runAction(RepeatForever::create(Sequence::create(RotateBy::create(0.1f,-10),RotateBy::create(0.1f,20),RotateBy::create(0.1f,-10),RotateBy::create(0.1f,-10),RotateBy::create(0.1f,20),RotateBy::create(0.1f,-10),DelayTime::create(fjianGe),NULL)));
                }
                
                Menu *pMenuBx = Menu::create(m_pMenuItemBx3,NULL);
                pMenuBx->setAnchorPoint(Vec2(0.5,0));
                pMenuBx->setPosition(150+m_pMenuItemBx3->getBoundingBox().size.width*2*i,tKuangBg.height*0.675);
                pKuang->addChild(pMenuBx,2,100);
                
                Label *pNeedStar = Label::createWithTTF("30","fonts/cuti.ttf",25);
                pNeedStar->setPosition(150+m_pMenuItemBx3->getBoundingBox().size.width*2*i,-tKuangBg.height*0.19);
                pKuang->addChild(pNeedStar,2);
                
                Sprite *pStar = Sprite::createWithSpriteFrameName("star.png");
                pStar->cocos2d::Node::setScale(0.7);
                pStar->setPosition(pNeedStar->getPositionX()+pNeedStar->getBoundingBox().size.width,-tKuangBg.height*0.19);
                pKuang->addChild(pStar,2);
                
            }
        }
        
        //判断是否达到领取的条件
    
        if(sum >= starLimit[receiveNum])
        {
            m_pAcquireMenu->setEnabled(true);
            m_pAcquireMenu->setColor(Color3B::WHITE);
        }
        else
        {
            //m_pAcquireMenu->setEnabled(false);
            m_pAcquireMenu->setColor(Color3B::GRAY);
        }
    }
    else{
        
        m_pAcquireMenu->setVisible(false);
        Sprite  *pbg  = Sprite::createWithSpriteFrameName("rewardBg.png");
        pbg->setPosition(bgSize.width*0.5,bgSize.height*0.12);
        m_node->addChild(pbg);
        pbg->setOpacity(255*0.6);
        Size  tBg = pbg->getContentSize();

        
        Label* chestNum = Label::createWithTTF("已领取所有奖励", "fonts/cuti.ttf",30);
        pbg->addChild(chestNum);
        chestNum->setColor(Color3B::YELLOW);
        chestNum->setPosition(Vec2(tBg.width * 0.5, tBg.height * 0.5f));
    }
    if (m_awardBg) {
        levelPanel();
    }
}

void CLevelInfoLayer::addSmallLevel(int smallLevel, Node* parent, cocos2d::Point pos)
{
    std::vector<levelAndStar> userLevelInfo = CPersonalData::getInstance()->getUserInfo()->m_userLevelVec;
    if (m_bEqlite) {
        userLevelInfo = CPersonalData::getInstance()->getUserInfo()->m_userEqliteLevelVec;
    }
    bool isShowStar = true;
    Sprite* normal = Sprite::createWithSpriteFrameName("level_block_1.png");
    Sprite* select = Sprite::createWithSpriteFrameName("level_block_2.png");
    LYMenuItemSprite* item = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(CLevelInfoLayer::btnCallBack, this));
    item->setTag(smallLevel);
    item->setPosition(pos);
    _pMenu->addChild(item);
    
    //关卡背景
//    Sprite* bg = Sprite::createWithSpriteFrameName("levelName_bg.png");
//    bg->setPosition(Vec2(normal->getContentSize().width / 2, normal->getContentSize().height - bg->getContentSize().height * 0.58));
//    item->addChild(bg, 5);
    struct levelInfo* pSmallLevelInfo = CDataManager::getTheLevelInfoWithId(smallLevel);
    Label* name = Label::createWithTTF(pSmallLevelInfo->levelName.c_str(), "fonts/cuti.ttf", 24);
    name->setColor(Color3B::WHITE);
    m_pLayer->addChild(name, 5);
    name->setPosition(Vec2(pos.x, pos.y + normal->getContentSize().height * 0.63f));
    
    //int index = (m_curMidLevel - 1)*10 + smallLevel;
    bool isShowEqlite =false;
    if(m_bEqlite){
        midLevelInfo* midLevelInfo = CDataManager::getMidLevelInfoById(m_curMidLevel);//中关卡信息
        if (CPersonalData::getInstance()->getUserInfo()->m_iSmallLevel >= midLevelInfo->lastLevel) {//如果本关已全部通关
            isShowEqlite = true;
        }
    }
    if(m_bEqlite && !isShowEqlite){
        if(m_awardBg)
        {
            m_awardBg->removeFromParent();
            m_awardBg = NULL;
        }
    }

    Sprite* levelType = Sprite::createWithSpriteFrameName(CDataManager::getImgByLevelId(smallLevel).c_str());
    item->addChild(levelType, 5, LEVELINFO_TYPE);
    levelType->setPosition(normal->getContentSize().width * 0.5, normal->getContentSize().height * 0.66);
    levelType->setOpacity(0);
    
    if(smallLevel > (m_smallLevel + 1)){
        //枷锁
        Sprite* normal = Sprite::createWithSpriteFrameName("level_block_3.png");
        item->setNormalImage(normal);
        Sprite* lock = Sprite::createWithSpriteFrameName("level_lock.png");
        item->addChild(lock, 5, LEVELINFO_LOCK);
        item->setEnabled(false);
        lock->setPosition(Vec2(normal->getContentSize().width * 0.5, normal->getContentSize().height * 0.4));
        //log("当前关卡 = %d, 完成度 = %d", smallLevel, m_smallLevel);
    }
    else
    {
        //log("<当前关卡 = %d, 完成度 = %d", smallLevel, m_smallLevel);
        if(smallLevel == m_smallLevel + 1)
        {
            if(CPersonalData::getInstance()->getisOpenNewSmallLevel())
            {
                Sprite* sp = Sprite::createWithSpriteFrameName("level_block_3.png");
                item->setNormalImage(sp);
                Sprite* lock = Sprite::createWithSpriteFrameName("level_lock.png");
                item->addChild(lock, 5, LEVELINFO_LOCK);
                lock->setPosition(Vec2(normal->getContentSize().width * 0.5, normal->getContentSize().height * 0.32));
                isShowStar = false;
                
                CPersonalData::getInstance()->setisOpenNewSmallLevel(false);
            }
            else{
                Sprite* sp = Sprite::createWithSpriteFrameName("level_block_2.png");
                item->setNormalImage(sp);
                
            }
            m_curSmallLevel = smallLevel;
            if(CPersonalData::getInstance()->getisWinManual() || CPersonalData::getInstance()->getisOpenNewSmallLevel() || CPersonalData::getInstance()->getFinishedLevel())
            {
                CPersonalData::getInstance()->setFinishedLevel(false);
                levelType->setOpacity(255);
            }
        }
        else{
            levelType->setOpacity(255);
        }
        if(isShowStar){
            //星级
            int curLevelStart = CPersonalData::getInstance()->getStarByLevel(smallLevel, m_bEqlite);//获取当前关卡星级
            if(!CPersonalData::getInstance()->getisWinManual() && (CFightData::getInstance()->getNowLevelId() == smallLevel))
            {
                curLevelStart = CPersonalData::getInstance()->getstar();
            }
            for(int i = 0; i < 3; i++)
            {
                Sprite* star = Sprite::createWithSpriteFrameName("level_star.png");
                if((smallLevel == m_smallLevel) && (CPersonalData::getInstance()->getisOpenNewSmallLevel()))
                {
                    star = Sprite::createWithSpriteFrameName("start_gray.png");
                }
                else if(i >= curLevelStart){
                    star = Sprite::createWithSpriteFrameName("start_gray.png");
                }
                item->addChild(star, 5, LEVELINFO_STAR + i);//增加星星效果后后期可能要删掉原来的灰色星星
                float height = normal->getContentSize().height * 0.25;
//                if (i == 1) {
//                    height = normal->getContentSize().height * 0.55;
//                }
                star->setScale(0.82f);
                star->setPosition(Vec2(normal->getContentSize().width * 0.25 + i*38, height));
                //star->setRotation((i - 1) * 30);
            }
        }
    }
    
}

void CLevelInfoLayer::levelPanel()
{
    if (m_awardBg) {
        m_awardBg->removeFromParent();
        m_awardBg = NULL;
    }
    if(m_pAwardLayer)
    {
        m_pAwardLayer->removeFromParent();
        m_pAwardLayer = NULL;
    }
    char buf[128];
    m_awardBg = Sprite::createWithSpriteFrameName("dikuang.png");
    addChild(m_awardBg);
    m_awardBg->setPosition(m_winSize.width * 0.5, m_winSize.height * 0.115f);
    m_awardBg->setScaleX(2.0f);
    m_awardBg->setScaleY(1.8f);
    Size bgSize = m_awardBg->getBoundingBox().size;
    
    //midLevelInfo* levelInfo = CDataManager::getMidLevelInfoById(m_curMidLevel);//中关卡信息
    m_pAwardLayer = LayerColor::create(Color4B(100, 0, 0, 0), bgSize.width, bgSize.height);
    addChild(m_pAwardLayer, 4);
    m_pAwardLayer->setPosition(Vec2(m_awardBg->getPositionX() - bgSize.width / 2, m_awardBg->getPositionY() - bgSize.height / 2));
    
    levelInfo* pSmallLevelInfo = CDataManager::getTheLevelInfoWithId(m_curSmallLevel);//获取当前选中小关卡信息
    if(!CPersonalData::getInstance()->getisWinManual()){
        pSmallLevelInfo = CDataManager::getTheLevelInfoWithId(m_smallLevel);
    }
    
    Label* award = Label::createWithSystemFont("奖励 : ", "Arial", 30);
    m_pAwardLayer->addChild(award);
    award->setColor(Color3B(200,79,15));
    award->setAnchorPoint(Vec2(0, 0.5));
    award->setPosition(Vec2(bgSize.width * 0.05f, bgSize.height * 0.72f));
    
    Sprite* coin = Sprite::createWithSpriteFrameName("single_coin.png");
    m_pAwardLayer->addChild(coin);
    coin->setScale(0.65);
    coin->setAnchorPoint(Vec2(0, 0.5));
    coin->setPosition(Vec2(award->getPositionX() + award->getContentSize().width + 10, award->getPositionY()));
    
    if (m_bEqlite) {
        sprintf(buf, "%d", pSmallLevelInfo->goldNum * 2);
    }
    else{
        sprintf(buf, "%d", pSmallLevelInfo->goldNum);
    }
    
    Label* coinNum = Label::createWithSystemFont(buf, "Arial", 30);
    m_pAwardLayer->addChild(coinNum);
    coinNum->setColor(Color3B::RED);
    coinNum->setAnchorPoint(Vec2(0, 0.5));
    coinNum->setPosition(Vec2(coin->getPositionX() + coin->getContentSize().width * 0.65f + 10, award->getPositionY()));
    
    Sprite* exp = Sprite::createWithSpriteFrameName("level_EXP.png");
    m_pAwardLayer->addChild(exp);
    exp->setAnchorPoint(Vec2(0, 0.5));
    exp->setPosition(Vec2(coinNum->getPositionX() + coinNum->getContentSize().width + 65, award->getPositionY()));
    
    if (m_bEqlite) {
        sprintf(buf, "%d", pSmallLevelInfo->playerExp * 2);
    }
    else{
        sprintf(buf, "%d", pSmallLevelInfo->playerExp);
    }
    
    Label* expNum = Label::createWithSystemFont(buf, "Arial", 30);
    m_pAwardLayer->addChild(expNum);
    expNum->setColor(Color3B::RED);
    expNum->setAnchorPoint(Vec2(0, 0.5));
    expNum->setPosition(Vec2(exp->getPositionX() + exp->getContentSize().width + 10, award->getPositionY()));
    
    Label* propAward = Label::createWithSystemFont("有几率获得 : ", "Arial", 30);
    m_pAwardLayer->addChild(propAward);
    propAward->setColor(Color3B(200,79,15));
    propAward->setAnchorPoint(Vec2(0, 0.5));
    propAward->setPosition(Vec2(award->getPositionX(), bgSize.height * 0.32f));
    
    std::vector<smallAwardInfo> itemsVec = pSmallLevelInfo->itemsVec;
    if (m_bEqlite) {
        itemsVec = pSmallLevelInfo->heroItemsVec;
    }
    if(itemsVec.size() > 0){
        float xPos = propAward->getPositionX() + propAward->getContentSize().width + 10;
        for (int i = 0; i < itemsVec.size(); i++)
        {
            if (itemsVec.at(i).type == 0)
            {//物品
                propInfo* prop = CDataManager::getPropInfoById(itemsVec.at(i).Id);
                Sprite* sp = Sprite::createWithSpriteFrameName(prop->m_sItemIcon.c_str());
                m_pAwardLayer->addChild(sp);
                sp->setScale(1.5f);
                sp->setAnchorPoint(Vec2(0, 0.5));
                sp->setPosition(xPos, propAward->getPositionY());
                xPos += sp->getBoundingBox().size.width + 15;
            }
            else
            {
                monsterInfo* monster = CDataManager::getMonsterInfoByMonsterId(itemsVec.at(i).Id);
                Sprite* sp = Sprite::createWithSpriteFrameName(monster->monsterIcon.c_str());
                m_pAwardLayer->addChild(sp);
                sp->setScale(0.55f);
                sp->setAnchorPoint(Vec2(0, 0.5));
                sp->setPosition(xPos, propAward->getPositionY());
                xPos += sp->getBoundingBox().size.width + 15;
                
                Sprite* debrisBg = Sprite::createWithSpriteFrameName("debrisBg.png");
                m_pAwardLayer->addChild(debrisBg);
                debrisBg->setAnchorPoint(Vec2(0, 0.5));
                debrisBg->setScale(1.8f);
                debrisBg->setPosition(sp->getPosition());
            }
        }
    }
    
    //开始按钮
    Sprite* normal = Sprite::createWithSpriteFrameName("yellowan.png");
    Sprite* select = Sprite::createWithSpriteFrameName("yellowan.png");
    select->setColor(Color3B::GRAY);
    LYMenuItemSprite* startItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(CLevelInfoLayer::btnCallBack, this),"music/fight.mp3");
    startItem->setTag(LEVELINFO_START);
    startItem->setScale(1.85f);
    m_pStartMenu = Menu::create(startItem, NULL);
    m_pAwardLayer->addChild(m_pStartMenu);
    m_pStartMenu->setPosition(Vec2(bgSize.width * 0.82f, bgSize.height * 0.5));
    
    if (m_bEqlite) {
        sprintf(buf, "%d", pSmallLevelInfo->stamina * 2);
    }
    else{
        sprintf(buf, "%d", pSmallLevelInfo->stamina);
    }
    
    Label* staminaNum = Label::createWithSystemFont(buf, "Arial", 30);
    startItem->addChild(staminaNum);
    staminaNum->setScale(0.55f);
    staminaNum->setColor(Color3B::BLUE);
    staminaNum->setPosition(Vec2(normal->getContentSize().width * 0.18f, normal->getContentSize().height * 0.585));

    Sprite* stamina = Sprite::createWithSpriteFrameName("stamina.png");
    stamina->setScale(0.42f);
    startItem->addChild(stamina);
    stamina->setPosition(Vec2(normal->getContentSize().width * 0.34f, staminaNum->getPositionY()));
    
    //分割线
    Sprite* line = Sprite::createWithSpriteFrameName("btn_line.png");
    startItem->addChild(line);
    line->setScale(0.5f);
    line->setPosition(Vec2(normal->getContentSize().width * 0.47f, staminaNum->getPositionY()));

    Label* fightLabel = Label::createWithSystemFont("出战", "Arial", 35);
    startItem->addChild(fightLabel);
    fightLabel->setScale(0.5f);
    fightLabel->setColor(Color3B::BLUE);
    fightLabel->setPosition(Vec2(normal->getContentSize().width * 0.68f, line->getPositionY()));
}

void CLevelInfoLayer::setNewLevelFlag()
{
    if(CPersonalData::getInstance()->getUserInfo()->m_iSmallLevel == (CDataManager::getAllLevelById(1)))
    {
        if(!(CDataManager::getEndedBy(GUIDE_GET_AWARD)))
        {
            CPersonalData::getInstance()->setisWinManual(true);
            CPersonalData::getInstance()->setisFailManual(true);
            CPersonalData::getInstance()->setisOpenNewSmallLevel(false);
            CPersonalData::getInstance()->setisOpenNewLevel(false);
            if(CDataManager::getMidLevelAwardById(1) <= 0)
            {
                //引导士兵升级
                guiderManagerLayer::getInstance()->setGuideType(GUIDE_GET_AWARD);
                guiderManagerLayer::getInstance()->setStepNum(1);
                guiderManagerLayer::getInstance()->createLayer(this);
                NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(CLevelInfoLayer::addGetAwardGuide), "getAward", NULL);
            }
            else{
                CDataManager::saveGuideInfoToDB(GUIDE_GET_AWARD);
            }
        }
    }
    else{
        CallFunc* back = CallFunc::create(CC_CALLBACK_0(CLevelInfoLayer::backMainMenu, this));
        runAction(Sequence::create(DelayTime::create(0.85f), back, NULL));
    }
}

void CLevelInfoLayer::backMainMenu()
{
    if(!(CDataManager::getEndedBy(GUIDE_PVP_BUILDING)) && (CPersonalData::getInstance()->getUserInfo()->m_iSmallLevel == 20))
    {
        CPersonalData::getInstance()->setisWinManual(true);
        CPersonalData::getInstance()->setisFailManual(true);
        CPersonalData::getInstance()->setisOpenNewSmallLevel(false);
        guiderManagerLayer::getInstance()->setGuideType(GUIDE_PVP_BUILDING);
        guiderManagerLayer::getInstance()->setStepNum(1);
        Vec2 pos = Vec2(m_winSize.width * 0.938f, m_winSize.height * 0.62f);
        guiderManagerLayer::getInstance()->createLayer(getParent(), GUIDE_SHOW_BOTH, pos);
    }
    removeFromParent();
    CPersonalData::getInstance()->setisOpenNewLevel(true);
}

void CLevelInfoLayer::setSelectedState()
{
    midLevelInfo* levelInfo = CDataManager::getMidLevelInfoById(m_curMidLevel);//中关卡信息
    for (int i = levelInfo->firstLevel - 1; i < levelInfo->lastLevel; i++) {
        LYMenuItemSprite* tempItem = (LYMenuItemSprite*)(_pMenu->getChildByTag(i + 1));
        if (i == m_smallLevel) {
            Sprite* sp = Sprite::createWithSpriteFrameName("level_block_2.png");
            tempItem->setNormalImage(sp);
            m_curSmallLevel = m_smallLevel + 1;
            CFightData::getInstance()->setNowLevelId(m_curSmallLevel);
        }
        else{
            if(i <= m_smallLevel){
                Sprite* sp = Sprite::createWithSpriteFrameName("level_block_1.png");
                tempItem->setNormalImage(sp);
            }
        }
    }
    SimpleAudioEngine::getInstance()->playEffect("music/unlock.mp3");
    levelPanel();//更新选中状态后在更新奖励信息
}

void CLevelInfoLayer::addStarEffect()
{
    SimpleAudioEngine::getInstance()->playEffect("music/addStar.mp3");
}

void CLevelInfoLayer::testEffect()
{
    //获取当前进行的关卡
    int smallLevel = CFightData::getInstance()->getNowLevelId();
    int starNum = CPersonalData::getInstance()->getstar();
    
    if(CPersonalData::getInstance()->getisOpenNewSmallLevel())
    {
        CPersonalData::getInstance()->setisOpenNewSmallLevel(false);
    }

    
    //星星效果
    int num = CDataManager::getStarBySmallLevelId(smallLevel, m_bEqlite);
    float delayTime = 0.7f;
    if (num >= starNum) {
        if(num == starNum)
        {
            delayTime = 0.0f;
        }
        //CDataManager::saveUserLevelInfoToDB(smallLevel, star,MY_PERSION->getlevelMode());
        
        LYMenuItemSprite* tempItem = (LYMenuItemSprite*)(_pMenu->getChildByTag(smallLevel));
        for(int i = starNum; i < num; i++)
        {
            Sprite* star = Sprite::createWithSpriteFrameName("level_star.png");
            if(tempItem)
            {
                tempItem->addChild(star, 5);
                float height = tempItem->getNormalImage()->getContentSize().height * 0.25;
                //            if (i == 1) {
                //                height = tempItem->getNormalImage()->getContentSize().height * 0.55;
                //            }
                star->setPosition(Vec2(tempItem->getNormalImage()->getContentSize().width * 0.25 + i*38, height));
                //star->setRotation((i - 1) * 30);
                star->setScale(2.5);
                star->runAction(Hide::create());
                CallFunc* act = CallFunc::create(CC_CALLBACK_0(CLevelInfoLayer::addStarEffect, this));
                Spawn* starAct = Spawn::create(ScaleTo::create(0.4, 0.82), act, NULL);
                star->runAction(Sequence::create(DelayTime::create((i - starNum)/2.1f), Show::create(), starAct, NULL));

            }
        }
        int starNUm = CDataManager::getStarBySmallLevelId(CDataManager::getMidLevelInfoById(m_curMidLevel)->lastLevel);
        if ((!CPersonalData::getInstance()->getFinishedLevel() ||(CPersonalData::getInstance()->getShowMap())) && !m_bEqlite && (CDataManager::getStarBySmallLevelId(CDataManager::getMidLevelInfoById(m_curMidLevel)->lastLevel) > 0))
        {//完成最后一关并且当前关卡的每一个小关卡都是三星则关闭关卡界面
            CFightData::getInstance()->setNowMidLevelId(m_curMidLevel + 1);
            //CPersonalData::getInstance()->setisOpenNewLevel(true);
            CallFunc* set = CallFunc::create(CC_CALLBACK_0(CLevelInfoLayer::setNewLevelFlag, this));
            runAction(Sequence::create(DelayTime::create(delayTime), set, NULL));
        }
        else{
            if(!CPersonalData::getInstance()->getFinishedLevel())
            {
                //锁抖动效果
                midLevelInfo* pInfo = CDataManager::getMidLevelInfoById(m_curMidLevel);
                if(smallLevel != pInfo->lastLevel)
                {
                    tempItem = (LYMenuItemSprite*)(_pMenu->getChildByTag(smallLevel + 1));
                    Sprite* lock = (Sprite*)(tempItem->getChildByTag(LEVELINFO_LOCK));
                    if (lock) {
                        //显示选中状态
                        CallFunc* select = CallFunc::create(CC_CALLBACK_0(CLevelInfoLayer::setSelectedState, this));
                        ActionInterval* seq = Sequence::create(MoveBy::create(0.08f, Vec2(-8, 0)), MoveBy::create(0.08f, Vec2(8, 0)), NULL);
                        lock->runAction(Sequence::create(DelayTime::create(0.4 + num/2.1f),Repeat::create(seq, 6),FadeOut::create(0.3),
                                                         select, RemoveSelf::create(), NULL));
                    }
                    Sprite* typeIcon = (Sprite*)(tempItem->getChildByTag(LEVELINFO_TYPE));
                    if(typeIcon)
                    {
                        typeIcon->runAction(Sequence::create(DelayTime::create(1.2 + num/2.1f),FadeTo::create(0.5, 255),NULL));
                    }
                }
                for(int i = 0; i < 3; i++)
                {
                    Sprite* star = Sprite::createWithSpriteFrameName("start_gray.png");
                    tempItem->addChild(star, 5);
                    float height = tempItem->getNormalImage()->getContentSize().height * 0.25;
                    //                if (i == 1) {
                    //                    height = tempItem->getNormalImage()->getContentSize().height * 0.55;
                    //                }
                    star->setPosition(Vec2(tempItem->getNormalImage()->getContentSize().width * 0.25 + i*38, height));
                    //star->setRotation((i - 1) * 30);
                    star->runAction(Hide::create());
                    star->setScale(0.82f);
                    star->runAction(Sequence::create(DelayTime::create(0.5 + num/2.1f + 0.96f + 0.3) , Show::create(), FadeIn::create(0.5), NULL));
                }
            }
        }
            
    }
    CPersonalData::getInstance()->setstar(0);
    midLevelInfo* levelInfo = CDataManager::getMidLevelInfoById(m_curMidLevel);//中关卡信息
    m_smallLevel = CPersonalData::getInstance()->getUserInfo()->m_iSmallLevel;
    if (m_bEqlite) {
        m_smallLevel = CPersonalData::getInstance()->getUserInfo()->m_iEqliteSmallLevel;
    }
//    for(int i = levelInfo->firstLevel - 1; i < levelInfo->lastLevel; i++)
//    {
//        if(i < m_smallLevel){
//            MenuItemSprite* item = (MenuItemSprite*)(_pMenu->getChildByTag(i + 1));
//            if (item) {
//                Sprite* sp = Sprite::createWithSpriteFrameName("level_block_1.png");
//                item->setNormalImage(sp);
//            }
//        }
//        else if(i == m_smallLevel)
//        {
//            MenuItemSprite* item = (MenuItemSprite*)(_pMenu->getChildByTag(i + 1));
//            if (item) {
//                Sprite* sp = Sprite::createWithSpriteFrameName("level_block_2.png");
//                item->setNormalImage(sp);
//                m_curSmallLevel = i + 1;
//            }
//        }
//    }
}


void CLevelInfoLayer::OnStartFight(Ref *pSender)
{

    levelInfo* pSmallLevelInfo = CDataManager::getTheLevelInfoWithId(m_curSmallLevel);
    //判断体力是否足够
    int needStaina = pSmallLevelInfo->stamina;
    if (m_bEqlite) {
        needStaina = pSmallLevelInfo->stamina * 2;
    }
    if(needStaina > CPersonalData::getInstance()->GetPhysicalNum())//测试用false，看到就删
    {
        SimpleAudioEngine::getInstance()->playEffect("music/noBtn.mp3");
        Sprite * thetarget = CFightManager::getTheFlyWord("体力不足","fonts/cuti.ttf");
        thetarget->setPosition(Vec2(m_winSize.width / 2, m_winSize.height / 2));
        addChild(thetarget, 1111);
    }
    else{
        SimpleAudioEngine::getInstance()->stopBackgroundMusic();
        
        if (m_bEqlite) {
            MY_PERSION->setlevelMode(1);
        }
        else{
            MY_PERSION->setlevelMode(0);
        }
        
        m_pStartMenu->setEnabled(false);
        CFightData::getInstance()->setFightType(FIGHT_COMMON_TYPE);
        CLayerManager::GetSingletonPtr()->createLayerWithLoading(eLayer_Fight, NULL,false);
    }
}


void CLevelInfoLayer::OnUpdateCoin(Ref *pSender)
{
    char buf[100] = {0};
    
    std::string awardStr;

    sprintf(buf, "%d",m_iGotNum);
    awardStr += "获得金币: ";
    awardStr += buf;
    
    CPersonalData::getInstance()->setUpdateUserInfo(true);
    CDataManager::saveUserAwardInfo(m_curMidLevel, m_iReceiveNum + 1, m_bEqlite);
    reFreshLevelReawrd();//领完奖励刷新奖励界面
    
    Sprite * thetarget = CFightManager::getRewardWord(awardStr.c_str());
    addChild(thetarget, 10);
    thetarget->setPosition(m_winSize.width * 0.5f, m_winSize.height / 2);
    
}

void CLevelInfoLayer::OnUpdateStone(Ref *pSender)
{
    char buf[100] = {0};
    
    std::string awardStr;
    
    sprintf(buf, "%d",m_iGotNum);
    awardStr += "获得魔法石: ";
    awardStr += buf;
    
    CPersonalData::getInstance()->setUpdateUserInfo(true);
    CDataManager::saveUserAwardInfo(m_curMidLevel, m_iReceiveNum + 1, m_bEqlite);
    reFreshLevelReawrd();//领完奖励刷新奖励界面
    
    Sprite * thetarget = CFightManager::getRewardWord(awardStr.c_str());
    addChild(thetarget, 10);
    thetarget->setPosition(m_winSize.width * 0.5f, m_winSize.height / 2);


}

void CLevelInfoLayer::btnCallBack(Ref* pSender)
{
    if(m_pJianliBg)
    {
        m_pJianliBg->removeFromParent();
        m_pJianliBg = NULL;
    }
        
    int tag = ((Node*)pSender)->getTag();
    
    if (tag == LEVELINFO_EXIT)
    {
        guiderManagerLayer::getInstance()->removeGuiderLayer();
        MY_PERSION->setlevelMode(0);
        if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_GET_AWARD) && (guiderManagerLayer::getInstance()->getStepNum() == 3))
        {
            guiderManagerLayer::getInstance()->setStepNum(0);
        }
        if(!(CDataManager::getEndedBy(GUIDE_ENTER_STORAGE)) && (CPersonalData::getInstance()->getUserInfo()->m_iSmallLevel == 10))
        {
            CPersonalData::getInstance()->setisWinManual(true);
            CPersonalData::getInstance()->setisFailManual(true);
            CPersonalData::getInstance()->setisOpenNewSmallLevel(false);
            CPersonalData::getInstance()->setisOpenNewLevel(false);
            //判断当前是否有木制宝箱和木制钥匙 木制宝箱:36 木制钥匙:39
            if((CDBMgr::getInstance()->HavePropNumById(36) > 0) && (CDBMgr::getInstance()->HavePropNumById(39) > 0))
            {
                guiderManagerLayer::getInstance()->setGuideType(GUIDE_ENTER_STORAGE);
                guiderManagerLayer::getInstance()->setStepNum(1);
                Vec2 pos = Vec2(m_winSize.width * 0.5f, m_winSize.height * 0.09);
                guiderManagerLayer::getInstance()->createLayer(getParent(), GUIDE_SHOW_BOTH, pos);
            }
            else{
                guiderManagerLayer::getInstance()->removeGuiderLayer();
                CDataManager::saveGuideInfoToDB(GUIDE_ENTER_STORAGE);
                CPersonalData::getInstance()->setisOpenNewLevel(true);
            }
        }
        if((guiderManagerLayer::getInstance()->getStepNum() == 2) && (guiderManagerLayer::getInstance()->getGuideType() == GUIDE_LACK_STAMINA))
        {
            guiderManagerLayer::getInstance()->removeGuiderLayer();
            guiderManagerLayer::getInstance()->setStepNum(guiderManagerLayer::getInstance()->getStepNum() + 1);
            Vec2 pos = Vec2(m_winSize.width * 0.059f, m_winSize.height * 0.72f);
            guiderManagerLayer::getInstance()->createLayer(getParent(), GUIDE_SHOW_FINGER, pos);
        }
        removeFromParent();
    }

    else if(tag == LEVELINFO_ACQUIRE)
    {
        if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_GET_AWARD) && (guiderManagerLayer::getInstance()->getStepNum() == 2))
        {
            CDataManager::saveGuideInfoToDB(guiderManagerLayer::getInstance()->getGuideType());
            guiderManagerLayer::getInstance()->setStepNum(guiderManagerLayer::getInstance()->getStepNum() + 1);
            Menu* pMenu = (Menu*)m_pLayer->getChildByTag(LEVELINFO_EXIT);
            Vec2 pos = pMenu->convertToWorldSpace(Vec2::ZERO);
            guiderManagerLayer::getInstance()->createLayer(this, GUIDE_SHOW_FINGER, pos);
        }

        //领取奖励
        midLevelInfo* levelInfo = CDataManager::getMidLevelInfoById(m_curMidLevel);
        int receiveNum = CDataManager::getMidLevelAwardById(m_curMidLevel, m_bEqlite);
        
        int starLimit[3] = {12, 24, 30};
        
        int sum = CDataManager::getAllLevelStarById(m_curMidLevel,m_bEqlite);
        
        if(sum < starLimit[receiveNum ])
        {
            
            int iGeshu  = starLimit[receiveNum ] - sum ;
            
            char strText[100] = {0};
            sprintf(strText,"       您当前获得的星星数不足   \n再获得%d颗星星就可以领取奖励了哦~",iGeshu);
            
            Sprite * thetarget = CFightManager::getTheFlyWord(strText,"fonts/cuti.ttf");
            thetarget->setPosition(m_winSize.width*0.5,m_winSize.height*0.5);
            addChild(thetarget,99);
            return;
        }
        
        
        //打开宝箱
        
        MenuItemSprite *p = NULL;
        
        if(receiveNum == 0)
        {
            p = m_pMenuItemBx1;
        }
        else if(receiveNum == 1)
        {
            p = m_pMenuItemBx2;
        }
        else if(receiveNum == 2)
        {
            p = m_pMenuItemBx3;
        }
        
        Sprite *pOpen = Sprite::createWithSpriteFrameName("bx_open.png");
        

        Sprite *pOpenSelc = Sprite::createWithSpriteFrameName("bx_open.png");
         p->stopAllActions();
        
        p->setNormalImage(pOpen);
        p->setSelectedImage(pOpenSelc);
    
        
        std::vector<awardInfo> awardVec = levelInfo->awardVec;//当前关卡
        if(m_bEqlite)
        {
            awardVec = levelInfo->heroAwardVec;
        }
        std::string awardStr;
        char buf[128];
        if(awardVec.at(receiveNum).type == 0)//道具
        {
            int iNum = CDBMgr::getInstance()->HavePropNumById(awardVec.at(receiveNum).PropId);
            awardStr += "获得 ";
            awardStr +=  CDataManager::getPropInfoById(awardVec.at(receiveNum).PropId)->m_sItemName;
            sprintf(buf, "X%d", awardVec.at(receiveNum).num);
            awardStr += buf;
            CDataManager::SaveUserPropToDB(awardVec.at(receiveNum).PropId, iNum + awardVec.at(receiveNum).num);
            CPersonalData::getInstance()->setUpdateUserInfo(true);
            CDataManager::saveUserAwardInfo(m_curMidLevel, receiveNum + 1, m_bEqlite);
            reFreshLevelReawrd();//领完奖励刷新奖励界面
            
            Sprite * thetarget = CFightManager::getRewardWord(awardStr.c_str());
            addChild(thetarget, 10);
            thetarget->setPosition(m_winSize.width * 0.5f, m_winSize.height / 2);
        }
        else if(awardVec.at(receiveNum).type == 1){
           // CDataManager::saveUserStoneAndCoinToDB(awardVec.at(receiveNum).num, 0, true);
            //spf
            CNetWorkDataSingle::GetSingletonPtr()->SaveUserMCF(this,menu_selector(CLevelInfoLayer::OnUpdateCoin),awardVec.at(receiveNum).num, 0,0);
            
            
            m_iGotNum = awardVec.at(receiveNum).num;
            m_iReceiveNum = receiveNum;
            /*sprintf(buf, "%d", awardVec.at(receiveNum).num);
            awardStr += "获得金币: ";
            awardStr += buf;*/
            
        }
        else{
            //CDataManager::saveUserStoneAndCoinToDB(awardVec.at(receiveNum).num, 1, true);
            CNetWorkDataSingle::GetSingletonPtr()->SaveUserMCF(this,menu_selector(CLevelInfoLayer::OnUpdateStone),0,awardVec.at(receiveNum).num, 0);
            
            m_iGotNum = awardVec.at(receiveNum).num;
            m_iReceiveNum = receiveNum;
            TDCCVirtualCurrency::onReward(m_iGotNum, "关卡任务奖励");
            /*sprintf(buf, "%d", awardVec.at(receiveNum).num);
            awardStr += "获得魔法石: ";
            awardStr += buf;*/
        }
        
       /*
        CPersonalData::getInstance()->setUpdateUserInfo(true);
        CDataManager::saveUserAwardInfo(m_curMidLevel, receiveNum + 1, m_bEqlite);
        reFreshLevelReawrd();//领完奖励刷新奖励界面
        
        Sprite * thetarget = CFightManager::getRewardWord(awardStr.c_str());
        addChild(thetarget, 10);
        thetarget->setPosition(m_winSize.width * 0.5f, m_winSize.height / 2);
        */
    }
    
    else if(tag == LEVELINFO_START)
    {
        if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_GET_TASK) && (guiderManagerLayer::getInstance()->getStepNum() == 6))
        {
            guiderManagerLayer::getInstance()->setStepNum(0);;
            guiderManagerLayer::getInstance()->removeGuiderLayer();
        }
        else if((guiderManagerLayer::getInstance()->getStepNum() == 7) && (guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SOLIDER_NUM))
        {
            guiderManagerLayer::getInstance()->setStepNum(0);;
            guiderManagerLayer::getInstance()->removeGuiderLayer();
        }
        if (((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_UPGRADE_SOLIDER) && (guiderManagerLayer::getInstance()->getStepNum() == 3)) || ((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SKILL_UPGRADE) && (guiderManagerLayer::getInstance()->getStepNum() == 4)) )
        {
            guiderManagerLayer::getInstance()->setStepNum(0);
            CDataManager::saveGuideInfoToDB(guiderManagerLayer::getInstance()->getGuideType());
            guiderManagerLayer::getInstance()->setNextGuide(true);
            guiderManagerLayer::getInstance()->removeGuiderLayer();
        }
        CFightData::getInstance()->newLevelStart(m_curSmallLevel);//设置当前选中关卡
        CFightData::getInstance()->setNowLevelId(m_curSmallLevel);
        CAtkMonsterInfo *pLayer = CAtkMonsterInfo::create();
        
        pLayer->CreateActionStart(this,menu_selector(CLevelInfoLayer::OnStartFight));
    
        
        addChild(pLayer,111);
        
    }
    else if(tag == LEVELINFO_EQLITE)
    {
        if(CDataManager::getAllLevelStarById(m_curMidLevel) < CDataManager::getAllLevelById(m_curMidLevel) * 3)
        {
            SimpleAudioEngine::getInstance()->playEffect("music/noBtn.mp3");
            Sprite * thetarget = CFightManager::getTheFlyWord("普通关卡全部为三星后开启","fonts/cuti.ttf");
            thetarget->setPosition(Vec2(m_levelBg->getBoundingBox().size.width / 2, m_levelBg->getBoundingBox().size.height / 2));
            m_pLayer->addChild(thetarget, 10);
        }
        else{
            if(m_curMidLevel > 1)
            {
                //计算之前一关的精英关卡通关数
                int preLevel = 0;
                for(int i = 1; i < m_curMidLevel - 1; i++)
                {
                    preLevel += CDataManager::getAllLevelById(i);
                }
                int curMidLevel = CPersonalData::getInstance()->getUserInfo()->m_iEqliteSmallLevel - preLevel;//之前大关卡的进度
                if(curMidLevel < CDataManager::getAllLevelById(m_curMidLevel - 1))
                {
                    SimpleAudioEngine::getInstance()->playEffect("music/noBtn.mp3");
                    char buf[256];
                    sprintf(buf, "请先完成第%d关的所有精英关卡", m_curMidLevel - 1);
                    Sprite * thetarget = CFightManager::getTheFlyWord(buf,"fonts/cuti.ttf");
                    thetarget->setPosition(Vec2(m_levelBg->getBoundingBox().size.width / 2, m_levelBg->getBoundingBox().size.height / 2));
                    m_pLayer->addChild(thetarget, 10);
                    return;
                }
            }
            MY_PERSION->setlevelMode(1);
            m_pEqliteItem->setEnabled(false);
            m_pComItem->setEnabled(true);
            
            Sprite* sp = Sprite::createWithSpriteFrameName("eqliteBtn.png");
            m_pEqliteItem->setSelectedImage(sp);
            sp = Sprite::createWithSpriteFrameName("commonBtn.png");
            m_pComItem->setNormalImage(sp);
            
            m_bEqlite = true;
            initLevelInfo();
        }
        
    }
    else if(tag == LEVELINFO_COMMON)
    {
        m_pEqliteItem->setEnabled(true);
        m_pComItem->setEnabled(false);
        Sprite* sp = Sprite::createWithSpriteFrameName("eqliteBtn.png");
        m_pEqliteItem->setNormalImage(sp);
        sp = Sprite::createWithSpriteFrameName("commonBtn.png");
        m_pComItem->setSelectedImage(sp);
        m_bEqlite = false;
        MY_PERSION->setlevelMode(0);
        initLevelInfo();
    }
    else
    {//关卡奖励信息
        m_curSmallLevel = tag;
        CFightData::getInstance()->setNowLevelId(tag);
        midLevelInfo* levelInfo = CDataManager::getMidLevelInfoById(m_curMidLevel);//中关卡信息
        for (int i = levelInfo->firstLevel - 1; i < levelInfo->lastLevel; i++) {
            LYMenuItemSprite* tempItem = (LYMenuItemSprite*)(_pMenu->getChildByTag(i + 1));
            if (i + 1 == tag) {
                Sprite* sp = Sprite::createWithSpriteFrameName("level_block_2.png");
                tempItem->setNormalImage(sp);
            }
            else{
                if(i <= m_smallLevel){
                    Sprite* sp = Sprite::createWithSpriteFrameName("level_block_1.png");
                    tempItem->setNormalImage(sp);
                }
            }
        }
        levelPanel();
    }

}