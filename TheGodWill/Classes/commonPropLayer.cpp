//
//  CPropCommonLayer.cpp
//  TheGodWill
//
//  Created by user on 14/12/25.
//
//

#include "commonPropLayer.h"
#include "DataManager.h"
#include "heroCompoundLayer.h"
#include "CHitDialog.h"
#include <LYPersonalData.h>
#include "LYFightData.h"
#include "DataManager.h"
#include "FightWinLayer.h"
#include "guiderManagerLayer.h"
#include "CSharedPlist.h"
#include "NetWorkDataSingle.h"
#include "Common.h"

#include "PublicDefine.h"

std::string propIconBg[5]={"whiteBg.png","greenBg.png","blueBg.png","purpleBg.png", "orangeBg.png"};
#define MY_PERSION CPersonalData::getInstance()

CommonPropLayer::CommonPropLayer()
{
    //m_progress = NULL;
    m_pSliderValue = NULL;
    m_arrow = NULL;
    m_buyNum = NULL;
    m_costNum = NULL;
    m_pBg = NULL;
    m_menu = NULL;
    m_monster = NULL;
    m_type = 0;
    m_availNum = 0;
    m_rare = 0;
    monsterId = 0;
    m_beforeLevel = 0;
    m_afterUpgrade = 0;
    m_pTarget = NULL;
    m_pHandler = NULL;
    
    m_pCancelTarget = NULL;
    m_pCancelHandler=NULL;
    
    m_isLevelEnd = false;
    m_strText = "";
    m_iPropNum = 0;
    m_bg = NULL;
    listen = NULL;
    m_pLayer = NULL;
    m_stone = 0;
    m_addCoin = 0;
}

CommonPropLayer::~CommonPropLayer()
{
    
}




CommonPropLayer* CommonPropLayer::create(int type,int levDown, int levUp)
{
    CommonPropLayer* pRef = new CommonPropLayer();
    pRef->m_type = type;
    pRef->m_beforeLevel = levDown;
    pRef->m_afterUpgrade = levUp;
    if (pRef && pRef->init()) {
        pRef->autorelease();
        return pRef;
    }
    else{
        CC_SAFE_DELETE(pRef);
        return NULL;
    }
}

void CommonPropLayer::setScucessCallBack(cocos2d::Ref * pTarget,cocos2d::SEL_MenuHandler pHandler)
{
    m_pTarget = pTarget;
    m_pHandler = pHandler;
    
}

void CommonPropLayer::setFailCallBack(cocos2d::Ref *pTarget, cocos2d::SEL_MenuHandler pHandler){
    
    m_pCancelTarget = pTarget;
    m_pCancelHandler = pHandler;
    
}


CommonPropLayer* CommonPropLayer::create(int type,std::string strText)
{
    CommonPropLayer* pRef = new CommonPropLayer();
    pRef->m_type = type;
    pRef->m_strText = strText;
    if (pRef && pRef->init()) {
        pRef->autorelease();
        return pRef;
    }
    else{
        CC_SAFE_DELETE(pRef);
        return NULL;
    }
}

CommonPropLayer* CommonPropLayer::create(int type, bool isLevelEnd)
{
    CommonPropLayer* pRef = new CommonPropLayer();
    if(pRef)
    {
        pRef->m_type = type;
        pRef->m_isLevelEnd = isLevelEnd;
    }
    if (pRef && pRef->init()) {
        pRef->autorelease();
        return pRef;
    }
    else{
        CC_SAFE_DELETE(pRef);
        return NULL;
    }
}

CommonPropLayer* CommonPropLayer::create(int type, int monsterId,bool bProp,int iNum)
{
    CommonPropLayer* pRef = new CommonPropLayer();
    pRef->m_type = type;
    pRef->monsterId = monsterId;
    pRef->m_iPropNum = iNum;
    
    if (pRef && pRef->init()) {
        pRef->autorelease();
        return pRef;
    }
    else{
        CC_SAFE_DELETE(pRef);
        return NULL;
    }
}

bool CommonPropLayer::init()
{
    float opcity = 125;
    if(m_type == LEVEL_UPGRADE_TYPE)
    {
        opcity = 180;
    }
    
    else if((m_type == USER_CHEST_TYPE) || (m_type == STONE_EXCHANGE_COIN))
    {
        opcity  = 255*0.8;
    }
    else if(GAME_RMV_SOLIDER == m_type)
    {
        opcity  = 50;
    }
    if (!CBaseLayer::initWithColor(Color4B(0, 0, 0, opcity)))
    {
        return false;
    }
    
    m_winSize = Director::getInstance()->getWinSize();
    
    listen = EventListenerTouchOneByOne::create();
    listen->setSwallowTouches(true);
    listen->onTouchBegan = CC_CALLBACK_2(CommonPropLayer::onTouchBegan, this);
    listen->onTouchEnded = CC_CALLBACK_2(CommonPropLayer::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listen, this);
    
    initUI();
    
    return true;
}

void CommonPropLayer::onExit()
{
    _eventDispatcher->removeEventListener(listen);
    CBaseLayer::onExit();
  
}

void CommonPropLayer::rmvSolider(cocos2d::Ref * pTarget,cocos2d::SEL_MenuHandler pHandler,int num)
{
    char buf[128];
    m_pTarget = pTarget;
    m_pHandler = pHandler;
    m_bg = Sprite::createWithSpriteFrameName("fight_bg.png");
    addChild(m_bg);
    m_bg->setOpacity(180);
    m_bg->setScaleX(0.75f);
    m_bg->setScaleY(0.62f);
    Size bgSize = m_bg->getBoundingBox().size;
    m_bg->setPosition(Vec2(m_winSize.width / 2, m_winSize.height * 0.5));
    
    LayerColor* pLayer = LayerColor::create(Color4B(0, 0, 0, 0), bgSize.width, bgSize.height);
    addChild(pLayer);
    pLayer->setPosition(Vec2(m_winSize.width / 2 - bgSize.width * 0.5f, m_winSize.height / 2 - bgSize.height * 0.5f));
    
    Label* pTitle = Label::createWithSystemFont("确定移除该士兵？", "Arial", 32);
    pLayer->addChild(pTitle);
    pTitle->setPosition(Vec2(bgSize.width / 2, bgSize.height * 0.81));
    
    Label* pEnemyCostWord = Label::createWithTTF("获得能量 ","fonts/cuti.ttf",28);
    pLayer->addChild(pEnemyCostWord);
    pEnemyCostWord->setPosition(Vec2(bgSize.width * 0.4f, bgSize.height * 0.6));
    
    sprintf(buf, "%d", num);
    Label* pEnemyCost = Label::createWithSystemFont(buf, "Arial", 28);
    pLayer->addChild(pEnemyCost);
    pEnemyCost->setColor(Color3B(34, 221, 238));
    pEnemyCost->setAnchorPoint(Vec2(0, 0.5));
    pEnemyCost->setPosition(Vec2(pEnemyCostWord->getPositionX() + pEnemyCostWord->getBoundingBox().size.width * 0.55f, bgSize.height * 0.6));
    
    //yes按钮
    Sprite *pYes = Sprite::createWithSpriteFrameName("greenbtn.png");
    
    Sprite *pYesSelect = Sprite::createWithSpriteFrameName("greenbtn.png");
    pYesSelect->setColor(Color3B::GRAY);
    
    LYMenuItemSprite *pYesSelectItem  = LYMenuItemSprite::create(pYes,pYesSelect,CC_CALLBACK_1(CommonPropLayer::OnSure, this));
    pYesSelectItem->setScale(2.25);
    
    Menu* pSuerMenu = Menu::create(pYesSelectItem, NULL);
    pSuerMenu->setPosition(bgSize.width*0.68,bgSize.height*0.18);
    pLayer->addChild(pSuerMenu);
    
    Size tYesItem  = pYesSelectItem->getContentSize();
    
    Label *pLabelYes = Label::createWithSystemFont("确定","Aril",30);
    pLabelYes->setScale(0.4);
    pLabelYes->setPosition(tYesItem.width*0.5,tYesItem.height*0.5);
    pYesSelectItem->addChild(pLabelYes);
    
    Sprite *pNo  = Sprite::createWithSpriteFrameName("redbtn.png");
    Sprite *pNoSelect = Sprite::createWithSpriteFrameName("redbtn.png");
    pNoSelect->setColor(Color3B::GRAY);
    
    LYMenuItemSprite *pNoItemSprite = LYMenuItemSprite::create(pNo,pNoSelect,CC_CALLBACK_1(CommonPropLayer::OnCancel, this));
    pNoItemSprite->setScale(2.25);
    Size tNoItem  = pNoItemSprite->getContentSize();
    Menu* pCancelMenu = Menu::create(pNoItemSprite, NULL);
    pCancelMenu->setPosition(bgSize.width*0.32,bgSize.height*0.18);
    pLayer->addChild(pCancelMenu);
    
    Label *pLabelNo = Label::createWithSystemFont("取消","Aril",30);
    pLabelNo->setScale(0.4);
    pLabelNo->setPosition(tNoItem.width*0.5,tNoItem.height*0.5);
    pNoItemSprite->addChild(pLabelNo);
}

void CommonPropLayer::salePropDialog(cocos2d::Ref * pTarget,cocos2d::SEL_MenuHandler pHandler,std::string sContent, int num)
{
    char buf[128];
    m_pTarget = pTarget;
    m_pHandler = pHandler;
    
    
    //Sprite *pbg = Sprite::createWithSpriteFrameName(<#const std::string &spriteFrameName#>);
    
    Sprite* bg = Sprite::createWithSpriteFrameName("name_bg2.png");
    addChild(bg);
    Size bgSize = bg->getBoundingBox().size;
    bg->setPosition(Vec2(m_winSize.width / 2, m_winSize.height * 0.5));
    
    Label* pTitle = Label::createWithSystemFont("获得物品", "Arial", 32);
    bg->addChild(pTitle);
    pTitle->setPosition(Vec2(bgSize.width / 2, bgSize.height * 0.895));
    
    propInfo* pInfo = CDataManager::getPropInfoById(monsterId);
    
    Label* propName = Label::createWithSystemFont(sContent.c_str(), "Arial", 30);
    bg->addChild(propName);
    propName->setPosition(bgSize.width / 2, bgSize.height * 0.65f);
    
    //卖出价格
    Label* pSalePrice = Label::createWithSystemFont("卖出价格: ", "Aria", 30);
    pSalePrice->setAnchorPoint(Vec2(0,0.5));
    bg->addChild(pSalePrice);
    pSalePrice->setPosition(propName->getPositionX() - propName->getContentSize().width / 2, bgSize.height * 0.46f);
    
    Sprite* coin = Sprite::createWithSpriteFrameName("coin.png");
    coin->setAnchorPoint(Vec2(0,0.5));
    bg->addChild(coin);
    coin->setPosition(pSalePrice->getPositionX() + pSalePrice->getContentSize().width + 10, bgSize.height * 0.46f);
    
    sprintf(buf, "%d", num * pInfo->m_iSalePrize);
    Label* pPriceLabel = Label::createWithSystemFont(buf, "Arial", 30);
    pPriceLabel->setAnchorPoint(Vec2(0,0.5));
    bg->addChild(pPriceLabel);
    pPriceLabel->setPosition(coin->getPositionX() + coin->getContentSize().width + 10, bgSize.height * 0.46f);
    

    //yes按钮
    Sprite *pYes = Sprite::createWithSpriteFrameName("greenbtn.png");
    
    Sprite *pYesSelect = Sprite::createWithSpriteFrameName("greenbtn.png");
    pYesSelect->setColor(Color3B::GRAY);
    
    LYMenuItemSprite *pYesSelectItem  = LYMenuItemSprite::create(pYes,pYesSelect,CC_CALLBACK_1(CommonPropLayer::OnSure, this));
    pYesSelectItem->setScale(2.5);
    
    Menu* pSuerMenu = Menu::create(pYesSelectItem, NULL);
    pSuerMenu->setPosition(bgSize.width*0.3,bgSize.height*0.14);
    bg->addChild(pSuerMenu);
    
    Size tYesItem  = pYesSelectItem->getContentSize();
    
    Label *pLabelYes = Label::createWithSystemFont("确定","Aril",30);
    pLabelYes->setScale(0.5);
    pLabelYes->setPosition(tYesItem.width*0.5,tYesItem.height*0.5);
    pYesSelectItem->addChild(pLabelYes);
    
    Sprite *pNo  = Sprite::createWithSpriteFrameName("redbtn.png");
    Sprite *pNoSelect = Sprite::createWithSpriteFrameName("redbtn.png");
    pNoSelect->setColor(Color3B::GRAY);
    
    LYMenuItemSprite *pNoItemSprite = LYMenuItemSprite::create(pNo,pNoSelect,CC_CALLBACK_1(CommonPropLayer::OnCancel, this));
    pNoItemSprite->setScale(2.5);
    Size tNoItem  = pNoItemSprite->getContentSize();
    Menu* pCancelMenu = Menu::create(pNoItemSprite, NULL);
    pCancelMenu->setPosition(bgSize.width*0.7,bgSize.height*0.14);
    bg->addChild(pCancelMenu);
    
    Label *pLabelNo = Label::createWithSystemFont("取消","Aril",30);
    pLabelNo->setScale(0.5);
    pLabelNo->setPosition(tNoItem.width*0.5,tNoItem.height*0.5);
    pNoItemSprite->addChild(pLabelNo);
    
    
}

bool CommonPropLayer::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    
    if(USER_CHEST_TYPE ==  m_type)
    {
        postGuideInfo();
        //removeFromParent();
    }
    return true;
}

void CommonPropLayer::onTouchEnded(Touch *touch, Event *unused_event)
{
    if(m_bg)
    {
        if((m_type == HERO_ADVANCED_TYPE) && (m_bg->getBoundingBox().containsPoint(touch->getLocation())))
        {
            remove();
        }
        else if((m_type == GAME_RMV_SOLIDER) && !(m_bg->getBoundingBox().containsPoint(touch->getLocation())))
        {
            if(typeid(*getParent()) == typeid(CFightScene))
            {
                ((CFightScene*)getParent())->resetRmvSolider();
            }
            removeFromParent();
        }
    }
}

void CommonPropLayer::remove()
{
    ((CHeroCompoundLayer*)getParent())->showHero();
    removeFromParent();
}

void CommonPropLayer::initUI()
{
    if ((DEBRIS_BUY_TYPE == m_type) || (HERO_ADVANCED_TYPE == m_type)) {
        
        m_availNum = CDataManager::getDebrisNumById(monsterId);
        m_rare = CDataManager::getMonsterStarById(monsterId);
        m_monster = CDataManager::getMonsterEvolutionById(m_rare + 1);
        if ((DEBRIS_BUY_TYPE == m_type) ) {
            initBuyDebris();
        }
        else{
            //判断材料是否足够
            initHeroAdvance();
        }
    }
    else if(LEVEL_UPGRADE_TYPE == m_type){
        upgradeLevel();
    }
    else if(USER_CHEST_TYPE == m_type){
        initUserChestUI();
    }
    else if(GAME_PAUSE_TYPE == m_type)
    {
        pauseGame();
    }
    else if(LEVEL_STAR_TYPE == m_type)
    {
        initLevelStarInfo();
    }
    else if((GAME_SKILL_BLOW == m_type) || (GAME_SKILL_CUT == m_type))
    {
        addSkillEffect();
    }
    else if(TOWER_MODE_INTRODUCE == m_type)
    {
        showTowerLevelInfo();
    }
    else if(STONE_EXCHANGE_COIN == m_type)
    {
        showExchangeInfo();
    }
    else if(STONE_EXCHANGE_STAMINA == m_type)
    {
        showExchangeStamina();
    }
}

void CommonPropLayer::showExchangeStamina()
{
    m_bg = Sprite::createWithSpriteFrameName("fight_bg.png");
    addChild(m_bg);
    m_bg->setPosition(Vec2(m_winSize.width * 0.5f, m_winSize.height * 0.5f));
    Size bgSize = m_bg->getBoundingBox().size;
    Label* pTitle = Label::createWithSystemFont("体力兑换", "Arial", 32);
    m_bg->addChild(pTitle);
    pTitle->setColor(Color3B(233, 188, 143));
    pTitle->setPosition(Vec2(bgSize.width * 0.5f, bgSize.height * 0.9f));
    
    char buf[256];
    userNum* pUserInfo = CDataManager::getUserInfoNum();
    sprintf(buf, "今日剩余次数%d次",  3 - pUserInfo->exchangeStaminaNum);
    Label* pBuyLabel = Label::createWithSystemFont(buf, "Arial", 26);
    m_bg->addChild(pBuyLabel);
    pBuyLabel->setAnchorPoint(Vec2(0, 0.5f));
    pBuyLabel->setColor(Color3B(247, 151, 26));
    pBuyLabel->setPosition(Vec2(bgSize.width * 0.1f, bgSize.height * 0.72f));
    
    //根据购买次数决定所需魔法石
    userNum* pInfo = CDataManager::getUserInfoNum();
    if(pInfo->exchangeStaminaNum == 0)
    {
        m_stone = 10;
        m_addStamina = 40;
    }
    else if(pInfo->exchangeStaminaNum == 1){
        m_stone = 20;
        m_addStamina = 80;
    }
    else if(pInfo->exchangeStaminaNum >= 2){
        m_stone = 40;
        m_addStamina = 120;
    }
    
    sprintf(buf, "是否花费 %d", m_stone);
    Label* pCostLabel = Label::createWithTTF(buf, "fonts/cuti.ttf", 28);
    m_bg->addChild(pCostLabel);
    pCostLabel->setAnchorPoint(Vec2(0, 0.5f));
    pCostLabel->setPosition(Vec2(bgSize.width * 0.1f, bgSize.height * 0.5f));
    
    Sprite* pStone = Sprite::createWithSpriteFrameName("stone.png");
    m_bg->addChild(pStone);
    pStone->setScale(0.65f);
    pStone->setAnchorPoint(Vec2(0, 0.5f));
    pStone->setPosition(Vec2(pCostLabel->getPositionX() + pCostLabel->getBoundingBox().size.width + 10, pCostLabel->getPositionY()));
    
    sprintf(buf, "兑换 %d", m_addStamina);
    Label* pExchange = Label::createWithTTF(buf, "fonts/cuti.ttf", 28);
    m_bg->addChild(pExchange);
    pExchange->setAnchorPoint(Vec2(0, 0.5f));
    pExchange->setPosition(Vec2(pStone->getPositionX() + pStone->getBoundingBox().size.width + 12, pStone->getPositionY()));
    
    Sprite* pCoin = Sprite::createWithSpriteFrameName("stamina.png");
    m_bg->addChild(pCoin);
    pCoin->setScale(0.85f);
    pCoin->setAnchorPoint(Vec2(0, 0.5f));
    pCoin->setPosition(Vec2(pExchange->getPositionX() + pExchange->getBoundingBox().size.width + 10, pExchange->getPositionY()));
    
    Label* pMark = Label::createWithTTF("?", "fonts/cuti.ttf", 28);
    m_bg->addChild(pMark);
    pMark->setAnchorPoint(Vec2(0, 0.5f));
    pMark->setPosition(Vec2(pCoin->getPositionX() + pCoin->getBoundingBox().size.width + 10, pCoin->getPositionY()));
    
    m_menu = Menu::create();
    m_bg->addChild(m_menu);
    m_menu->setPosition(Vec2::ZERO);
    Sprite* normal = Sprite::createWithSpriteFrameName("redbtn.png");
    Sprite* select = Sprite::createWithSpriteFrameName("redbtn.png");
    select->setColor(Color3B::GRAY);
    LYMenuItemSprite* cancelItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(CommonPropLayer::btnCallBack, this));
    cancelItem->setScale(2.6f);
    cancelItem->setPosition(Vec2(bgSize.width * 0.25, bgSize.height * 0.18));
    m_menu->addChild(cancelItem, 0, EXCHANGE_CANCEL_TAG);
    Label* cancel = Label::createWithSystemFont("取消", "Arial", 32);
    cancelItem->addChild(cancel);
    cancel->setScale(0.35f);
    cancel->setPosition(Vec2(normal->getContentSize().width / 2, normal->getContentSize().height / 2));
    
    normal = Sprite::createWithSpriteFrameName("greenbtn.png");
    select = Sprite::createWithSpriteFrameName("greenbtn.png");
    select->setColor(Color3B::GRAY);
    LYMenuItemSprite* sureItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(CommonPropLayer::btnCallBack, this));
    m_menu->addChild(sureItem, 0, EXCHANGE_STAMINA_TAG);
    sureItem->setScale(2.6f);
    sureItem->setPosition(Vec2(bgSize.width * 0.75, bgSize.height * 0.18));
    Label* sure = Label::createWithSystemFont("确定", "Arial", 32);
    sureItem->addChild(sure);
    sure->setScale(0.35f);
    sure->setPosition(Vec2(normal->getContentSize().width / 2, normal->getContentSize().height / 2));
}

void CommonPropLayer::showExchangeInfo()
{
    m_bg = Sprite::createWithSpriteFrameName("fight_bg.png");
    addChild(m_bg);
    m_bg->setPosition(Vec2(m_winSize.width * 0.5f, m_winSize.height * 0.5f));
    Size bgSize = m_bg->getBoundingBox().size;
    Label* pTitle = Label::createWithSystemFont("金币兑换", "Arial", 32);
    m_bg->addChild(pTitle);
    pTitle->setColor(Color3B(233, 188, 143));
    pTitle->setPosition(Vec2(bgSize.width * 0.5f, bgSize.height * 0.9f));
    
    char buf[256];
    userNum* pUserInfo = CDataManager::getUserInfoNum();
    sprintf(buf, "今日剩余次数%d次",  3 - pUserInfo->exchangeCoinNum);
    Label* pBuyLabel = Label::createWithSystemFont(buf, "Arial", 26);
    m_bg->addChild(pBuyLabel);
    pBuyLabel->setAnchorPoint(Vec2(0, 0.5f));
    pBuyLabel->setColor(Color3B(247, 151, 26));
    pBuyLabel->setPosition(Vec2(bgSize.width * 0.1f, bgSize.height * 0.72f));
    
    //根据购买次数决定所需魔法石
    userNum* pInfo = CDataManager::getUserInfoNum();
    if(pInfo->exchangeCoinNum == 0)
    {
        m_stone = 5;
    }
    else if(pInfo->exchangeCoinNum == 1){
        m_stone = 10;
    }
    else if(pInfo->exchangeCoinNum >= 2){
        m_stone = 20;
    }
    m_addCoin = 5000;

    sprintf(buf, "是否花费 %d", m_stone);
    Label* pCostLabel = Label::createWithTTF(buf, "fonts/cuti.ttf", 28);
    m_bg->addChild(pCostLabel);
    pCostLabel->setAnchorPoint(Vec2(0, 0.5f));
    pCostLabel->setPosition(Vec2(bgSize.width * 0.1f, bgSize.height * 0.5f));
    
    Sprite* pStone = Sprite::createWithSpriteFrameName("stone.png");
    m_bg->addChild(pStone);
    pStone->setScale(0.65f);
    pStone->setAnchorPoint(Vec2(0, 0.5f));
    pStone->setPosition(Vec2(pCostLabel->getPositionX() + pCostLabel->getBoundingBox().size.width + 10, pCostLabel->getPositionY()));
    
    sprintf(buf, "兑换 %d", m_addCoin);
    Label* pExchange = Label::createWithTTF(buf, "fonts/cuti.ttf", 28);
    m_bg->addChild(pExchange);
    pExchange->setAnchorPoint(Vec2(0, 0.5f));
    pExchange->setPosition(Vec2(pStone->getPositionX() + pStone->getBoundingBox().size.width + 12, pStone->getPositionY()));
    
    Sprite* pCoin = Sprite::createWithSpriteFrameName("coin.png");
    m_bg->addChild(pCoin);
    pCoin->setScale(0.85f);
    pCoin->setAnchorPoint(Vec2(0, 0.5f));
    pCoin->setPosition(Vec2(pExchange->getPositionX() + pExchange->getBoundingBox().size.width + 10, pExchange->getPositionY()));
    
    Label* pMark = Label::createWithTTF("?", "fonts/cuti.ttf", 28);
    m_bg->addChild(pMark);
    pMark->setAnchorPoint(Vec2(0, 0.5f));
    pMark->setPosition(Vec2(pCoin->getPositionX() + pCoin->getBoundingBox().size.width + 10, pCoin->getPositionY()));
    
    m_menu = Menu::create();
    m_bg->addChild(m_menu);
    m_menu->setPosition(Vec2::ZERO);
    Sprite* normal = Sprite::createWithSpriteFrameName("redbtn.png");
    Sprite* select = Sprite::createWithSpriteFrameName("redbtn.png");
    select->setColor(Color3B::GRAY);
    LYMenuItemSprite* cancelItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(CommonPropLayer::btnCallBack, this));
    cancelItem->setScale(2.6f);
    cancelItem->setPosition(Vec2(bgSize.width * 0.25, bgSize.height * 0.18));
    m_menu->addChild(cancelItem, 0, EXCHANGE_CANCEL_TAG);
    Label* cancel = Label::createWithSystemFont("取消", "Arial", 32);
    cancelItem->addChild(cancel);
    cancel->setScale(0.35f);
    cancel->setPosition(Vec2(normal->getContentSize().width / 2, normal->getContentSize().height / 2));
    
    normal = Sprite::createWithSpriteFrameName("greenbtn.png");
    select = Sprite::createWithSpriteFrameName("greenbtn.png");
    select->setColor(Color3B::GRAY);
    LYMenuItemSprite* sureItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(CommonPropLayer::btnCallBack, this));
    m_menu->addChild(sureItem, 0, EXCHANGE_COIN_TAG);
    sureItem->setScale(2.6f);
    sureItem->setTag(EXCHANGE_COIN_TAG);
    sureItem->setPosition(Vec2(bgSize.width * 0.75, bgSize.height * 0.18));
    Label* sure = Label::createWithSystemFont("确定", "Arial", 32);
    sureItem->addChild(sure);
    sure->setScale(0.35f);
    sure->setPosition(Vec2(normal->getContentSize().width / 2, normal->getContentSize().height / 2));
}

void CommonPropLayer::showTowerLevelInfo()
{
    CSharedPlist::getInstance()->pushPlist("plist/level.plist");
    m_bg = Sprite::createWithSpriteFrameName("common_bg.png");
    addChild(m_bg, 5);
    m_bg->setScaleX(1.03);
    m_bg->setScaleY(1.27f);
    m_bg->setPosition(Vec2(m_winSize.width / 2, m_winSize.height * 0.525));
    cocos2d::Size bgSize = m_bg->getBoundingBox().size;
    LayerColor* pLayer = LayerColor::create(Color4B(200, 0, 0, 0), bgSize.width, bgSize.height);
    addChild(pLayer, 5);
    pLayer->setPosition(Vec2(m_bg->getPositionX() - bgSize.width / 2, m_bg->getPositionY() - bgSize.height / 2));
    
    
    //退出按钮
    Sprite* normal = Sprite::createWithSpriteFrameName("closeBtn.png");
    Sprite* select = Sprite::createWithSpriteFrameName("closeBtn.png");
    select->setColor(Color3B::GRAY);
    LYMenuItemSprite* exitItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(CommonPropLayer::btnCallBack, this));
    exitItem->setTag(GAME_CONTINUE_TAG);
    CommonData::enlargeItemSprite(exitItem, 2);
    Menu* pMenu = Menu::create(exitItem, NULL);
    pMenu->setPosition(Vec2(bgSize.width * 0.96, bgSize.height * 0.87f));
    pLayer->addChild(pMenu);
    
    
    Label* pTitle = Label::createWithSystemFont("塔防模式帮助", "Arial", 32);
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
}

void CommonPropLayer::postMessage()
{
    if(GAME_SKILL_BLOW == m_type)
    {
        NotificationCenter::getInstance()->postNotification("StartBlow");
    }
    else
    {
        NotificationCenter::getInstance()->postNotification("StartCut");
    }
    
}

void CommonPropLayer::addSkillEffect()
{
    CallFunc* message = CallFunc::create(CC_CALLBACK_0(CommonPropLayer::postMessage, this));
    
    Sprite* skillBg = Sprite::createWithSpriteFrameName("skill_bg.png");
    addChild(skillBg);
    skillBg->setScale(4.0f);
    skillBg->setPosition(Vec2(m_winSize.width * 0.6f, m_winSize.height * 0.58f));
    
    Sprite* god = NULL;
    Sprite* word = NULL;
    if(m_type == GAME_SKILL_BLOW)
    {
        god = Sprite::createWithSpriteFrameName("fengshen.png");
        word = Sprite::createWithSpriteFrameName("fengshenzhinu.png");
    }
    else{
        god = Sprite::createWithSpriteFrameName("deathGod.png");
        word = Sprite::createWithSpriteFrameName("deathGodWord.png");
    }
    addChild(god);
    god->setPosition(Vec2(-god->getBoundingBox().size.width / 2, m_winSize.height  * 0.5f));
    addChild(word);
    
    god->runAction(Sequence::create(MoveBy::create(0.3, Vec2(god->getBoundingBox().size.width, 0)), DelayTime::create(1.2f), FadeOut::create(0.5f), NULL));
    word->setScale(7.0f);
    word->runAction(Hide::create());
    word->setPosition(Vec2(m_winSize.width * 0.6f , m_winSize.height  * 0.58f));
    word->runAction(Sequence::create(DelayTime::create(0.5f), Show::create(), ScaleTo::create(0.2f, 2.0f), DelayTime::create(0.8f), FadeOut::create(0.5f), NULL));
    runAction(Sequence::create(DelayTime::create(2.0f),message, RemoveSelf::create(), NULL));
}

void CommonPropLayer::initLevelStarInfo()
{
    Scale9Sprite* pBg = Scale9Sprite::createWithSpriteFrameName("common_bg.png");
    addChild(pBg);
    pBg->setContentSize(Size(pBg->getContentSize().width * 1.15, pBg->getContentSize().height * 1.15));
    Size bgSize = pBg->getBoundingBox().size;
    pBg->setPosition(Vec2(m_winSize.width / 2, m_winSize.height * 0.5));
    
//    LayerColor* pLayer = LayerColor::create(Color4B(255, 0, 0, 0), bgSize.width, bgSize.height);
//    addChild(pLayer);
//    pLayer->setPosition(Vec2(m_winSize.width * 0.13, m_winSize.height * 0.06));
    
    float star[3] = {0,0,0};//记录三个星级条件的完成情况
    Label* pTitle = Label::createWithTTF("完成度","fonts/cuti.ttf",32);
    
    
    pBg->addChild(pTitle);
    pTitle->setPosition(Vec2(bgSize.width / 2, bgSize.height * 0.91f));
    std::vector<levelStarInfo> info = CFightData::getInstance()->m_starInfoVec;
    int levelId = CFightData::getInstance()->getNowLevelId();
    std::vector<int> starIdVec = CDataManager::getStarConditionById(levelId);
    for (int i = 0; i < starIdVec.size(); i++) {
        starInfo* pInfo =  CDataManager::getStarInfoById(starIdVec.at(i));
        if(pInfo)
        {
            for(int j = 0; j < info.size(); j++)
            {
                if((pInfo->starType == info.at(j).starType) && (pInfo->monsterId == info.at(j).monsterId))
                {
                    star[i] = info.at(j).num;
                }
            }
        }
    }

    
    for(int i = 0; i < 3; i++)
    {
        starInfo* pInfo =  CDataManager::getStarInfoById(starIdVec.at(i));
        Sprite* sp = Sprite::createWithSpriteFrameName("darkstar.png");
        sp->setScale(4);
        Sprite* check = NULL;
        if(pInfo->positive == 1)
        {
            if(star[i] >= pInfo->num)
            {
                sp = Sprite::createWithSpriteFrameName("battle_star.png");
                check = Sprite::createWithSpriteFrameName("finish.png");
                pBg->addChild(check);
                check->setPosition(Vec2(bgSize.width*0.22 + i * 270, bgSize.height*0.22));
                sp->setPosition(Vec2(bgSize.width*0.22 + i * 270, bgSize.height*0.7));
            }
            else{
                if(m_isLevelEnd)
                {
                    sp = Sprite::createWithSpriteFrameName("star_fail.png");
                    check = Sprite::createWithSpriteFrameName("fail.png");
                    pBg->addChild(check);
                    check->setPosition(Vec2(bgSize.width*0.22 + i * 260, bgSize.height*0.22));
                }
                sp->setPosition(Vec2(bgSize.width*0.22 + i * 270, bgSize.height*0.69));
            }
        }
        else{
            if(star[i] > pInfo->num)
            {
                sp = Sprite::createWithSpriteFrameName("star_fail.png");
                check = Sprite::createWithSpriteFrameName("fail.png");
                pBg->addChild(check);
                sp->setScale(1);
                check->setPosition(Vec2(bgSize.width*0.22 + i * 260, bgSize.height*0.22));
                sp->setPosition(Vec2(bgSize.width*0.22 + i * 270, bgSize.height*0.69));
            }
            else{
                sp = Sprite::createWithSpriteFrameName("darkstar.png");
                sp->setScale(4.1);
                if(m_isLevelEnd)
                {
                    sp = Sprite::createWithSpriteFrameName("battle_star.png");
                    sp->setScale(0.9f);
                }
                check = Sprite::createWithSpriteFrameName("finish.png");
                pBg->addChild(check);
                
                check->setPosition(Vec2(bgSize.width*0.22 + i * 260, bgSize.height*0.22));
                sp->setPosition(Vec2(bgSize.width*0.22 + i * 270, bgSize.height*0.69));
            }
            
        }
        //sp->setScale(2.5f);
        pBg->addChild(sp);
        
        Label::createWithSystemFont;
        
        Label* pLabel = Label::createWithTTF(pInfo->starDesc.c_str(),"fonts/cuti.ttf", 26, Size(240,100));
        pBg->addChild(pLabel);
        pLabel->setPosition(Vec2(bgSize.width*0.22 + i * 270, bgSize.height*0.495));
        
        //任务进度
        Sprite* starBg = Sprite::createWithSpriteFrameName("star_bg.png");
        pBg->addChild(starBg);
        starBg->setPosition(Vec2(bgSize.width*0.22 + i * 275, bgSize.height*0.4));
        
        Sprite* starSp = Sprite::createWithSpriteFrameName("star_progress.png");
        ProgressTimer* progress = ProgressTimer::create(starSp);
        starBg->addChild(progress);
        progress->setPosition(Vec2(starBg->getContentSize().width / 2, starBg->getContentSize().height / 2));
        progress->setMidpoint(Vec2(0,1));
        progress->setType(ProgressTimer::Type::BAR);
        progress->setBarChangeRate(Vec2(1,0));
        
        __String * temp = NULL;
        Label* progressLabel = NULL;
        float nowNum = star[i];
        if((pInfo->positive == 1) && (nowNum > pInfo->num))
        {
            nowNum = pInfo->num;
        }
        temp = __String::createWithFormat("%d/%d",int(nowNum), pInfo->num);
        float percent = (nowNum/pInfo->num)*100;
        progress->setPercentage(percent);
        progressLabel = Label::createWithSystemFont(temp->getCString(), "粗体", 28);
        starBg->addChild(progressLabel);
        progressLabel->setPosition(starBg->getContentSize().width / 2, starBg->getContentSize().height / 2);
        if(pInfo->positive == 0)
        {
            if(star[i] <= pInfo->num && !m_isLevelEnd)
            {
                check->setVisible(false);
            }
        }
    }
    
    Sprite* normal = Sprite::createWithSpriteFrameName("closeBtn.png");
    Sprite* select = Sprite::createWithSpriteFrameName("closeBtn.png");
    select->setColor(Color3B::GRAY);
    LYMenuItemSprite* exitItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(CommonPropLayer::btnCallBack, this));
    CommonData::enlargeItemSprite(exitItem, 2);
    exitItem->setTag(CLICK_EXIT_TAG);
    Menu* pMenu = Menu::create(exitItem, NULL);
    pMenu->setPosition(Vec2(bgSize.width * 0.965, bgSize.height * 0.895));
    pBg->addChild(pMenu);
}

void CommonPropLayer::pauseGame()
{
    Scale9Sprite* pBg = Scale9Sprite::createWithSpriteFrameName("common_bg.png");
    addChild(pBg);
    pBg->setContentSize(Size(pBg->getContentSize().width * 0.95f, pBg->getContentSize().height * 0.8f));
    
    Size bgSize = pBg->getBoundingBox().size;
    pBg->setPosition(Vec2(m_winSize.width / 2, m_winSize.height * 0.5));
    
    Label* pTitle = Label::createWithSystemFont("暂停", "Arial", 30);
    pBg->addChild(pTitle);
    pTitle->setPosition(Vec2(bgSize.width / 2, bgSize.height * 0.87f));
    
    Sprite* normal = Sprite::createWithSpriteFrameName("redan.png");
    Sprite* select = Sprite::createWithSpriteFrameName("redan.png");
//    normal->setScale(1.5);
//    select->setScale(1.5);

    select->setColor(Color3B::GRAY);
    LYMenuItemSprite* plItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(CommonPropLayer::btnCallBack, this));
    plItem->setScale(1.5f);
    plItem->setTag(GAME_EXIT_TAG);
    Label* cancel = Label::createWithSystemFont("退出战斗", "Arial", 20);
    plItem->addChild(cancel);
    cancel->setPosition(Vec2(normal->getBoundingBox().size.width / 2, normal->getBoundingBox().size.height / 2));
    
    normal = Sprite::createWithSpriteFrameName("bluean.png");
    select = Sprite::createWithSpriteFrameName("bluean.png");
//    normal->setScale(1.5);
//    select->setScale(1.5);
    select->setColor(Color3B::GRAY);
    LYMenuItemSprite* pContinueItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(CommonPropLayer::btnCallBack, this));
    pContinueItem->setScale(1.5f);
    pContinueItem->setTag(GAME_CONTINUE_TAG);
    Label* sure = Label::createWithSystemFont("继续游戏", "Arial", 20);
    pContinueItem->addChild(sure);
    sure->setPosition(Vec2(normal->getBoundingBox().size.width / 2, normal->getBoundingBox().size.height / 2));

    normal = Sprite::createWithSpriteFrameName("greenan.png");
    select = Sprite::createWithSpriteFrameName("greenan.png");
//    normal->setScale(1.5);
//    select->setScale(1.5);
    select->setColor(Color3B::GRAY);
    LYMenuItemSprite* pRestartItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(CommonPropLayer::btnCallBack, this));
    pRestartItem->setScale(1.5f);
    pRestartItem->setTag(GAME_RESTART_TAG);
    Label* restart = Label::createWithSystemFont("重新开始", "Arial", 20);
    pRestartItem->addChild(restart);
    restart->setPosition(Vec2(normal->getBoundingBox().size.width / 2, normal->getBoundingBox().size.height / 2));
    
    Menu* pMenu = Menu::create(plItem, pContinueItem, pRestartItem, NULL);
    pBg->addChild(pMenu);
    pMenu->alignItemsHorizontallyWithPadding(20);
    pMenu->setPosition(Vec2(bgSize.width / 2, bgSize.height * 0.34));
    
    Label* word = Label::createWithSystemFont("游戏暂停中", "Arial", 32);
    pBg->addChild(word);
    word->setPosition(Vec2(bgSize.width / 2, bgSize.height *  0.62f));
}

void CommonPropLayer::OnSure(Ref *pSender)
{
    if (m_pTarget && m_pHandler)
    {
        (m_pTarget->*m_pHandler)(pSender);
    }

    
    removeFromParent();
}

void CommonPropLayer::OnCancel(Ref *pSender)
{
    if(m_type == GAME_RMV_SOLIDER)
    {
        if(typeid(*getParent()) == typeid(CFightScene))
        {
            ((CFightScene*)getParent())->resetRmvSolider();
        }
    }

    removeFromParent();
}


void CommonPropLayer::initUserChestUI()
{
    //透明度70%
    
    SimpleAudioEngine::getInstance()->playEffect("music/openBox.mp3");
    char buf[128] = {0};
    
    Sprite* word = Sprite::createWithSpriteFrameName("gongxi.png");
    word->setScale(0);
    addChild(word,5,GONXI_TAG);
    word->setPosition(Vec2(m_winSize.width * 0.5, m_winSize.height * 0.85f));

    Sprite* light = Sprite::createWithSpriteFrameName("backlight3.png");
    addChild(light,2,BackLight_TAG);
    light->setScale(0);
    light->setPosition(Vec2(m_winSize.width * 0.5, m_winSize.height * 0.5f));
    light->runAction(RepeatForever::create(RotateBy::create(1.0, 90)));
    
    Sprite *pBgKuang  = Sprite::createWithSpriteFrameName("goodsdikuang.png");
    pBgKuang->cocos2d::Node::setScale(1.25*8);
    pBgKuang->setScale(0);
    pBgKuang->setPosition(m_winSize.width*0.5,m_winSize.height*0.5);
    addChild(pBgKuang,1,BgKuang_TAG);
    
    Sprite *bg = Sprite::createWithSpriteFrameName("prop_openk.png");
    bg->setScale(0);
    bg->setPosition(m_winSize.width*0.5,m_winSize.height*0.5);
    addChild(bg,10,SmallBgKauang_TAG);
    
    word->runAction(ScaleTo::create(0.1,2));
    light->runAction(ScaleTo::create(0.1,4));
    pBgKuang->runAction(ScaleTo::create(0.1,1.25*8));
    bg->runAction(ScaleTo::create(0.1, 1));
    
    Size bgSize = bg->getContentSize();
    
    if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_ENTER_STORAGE) && (guiderManagerLayer::getInstance()->getStepNum() == 4))
    {
        monsterId = 0;
        m_iPropNum = 3000;//引导时固定奖励为3000金币
    }
    

    if(monsterId==0)
    {
        
        Sprite* propKuang = Sprite::createWithSpriteFrameName("jbjl.png");
        propKuang->setScale(0.5);
        bg->addChild(propKuang);
        //propKuang->setScale(2);
        propKuang->setPosition(bgSize.width / 2, bgSize.height * 0.45f);
        
        //名字、数量
        sprintf(buf, "金币 x%d",m_iPropNum);
        Label* pLabelNum = Label::createWithTTF(buf, "fonts/cuti.ttf", 30);
        bg->addChild(pLabelNum);
        pLabelNum->setPosition(bgSize.width / 2, -bgSize.height*0.025f);
        
        
        Color3B Rgb = Color3B(255,255,255);
        
        if(m_iPropNum>1000) //绿色
        {
            Rgb = Color3B(18,203,31);
        }
        else if(m_iPropNum>5000) //蓝色
        {
            Rgb = Color3B(16,105,216);
        }
        else if(m_iPropNum>10000) //紫色
        {
            Rgb = Color3B(162,16,216);
        }
        
        pLabelNum->setColor(Rgb);
        
    }
    else if(monsterId ==-1)
    {
        //获取碎片

        std::vector<monsterInfo*>  pInfo = CDataManager::getAllMonsterInfo();
        std::vector<monsterInfo *> pSuiInof;
        
        for(int i=0;i<pInfo.size();i++)
        {
            if(pInfo[i]->isHaveDebris==0)
            {
                pSuiInof.push_back(pInfo[i]);
            }
        }
        
        int iRandom = rand()%pSuiInof.size();
        
        monsterInfo * now = NULL;
        
        for(int i=0;i<iRandom;i++)
        {
            now = pSuiInof[i];
        }
     

         if(now==NULL)
         {
             now = pSuiInof[0];
         }
        
        CDataManager::saveUserDebrisToDB(now->monsterId,1);
        Sprite* propKuang =  CDataManager::getMonsterInfodebris(now->monsterId);
        propKuang->setScale(3);
        propKuang->setPosition(bgSize.width / 2, bgSize.height*0.45);
        bg->addChild(propKuang);
        
        //名字、数量
        sprintf(buf, "%s碎片 x1",now->monsterName.c_str());
        Label* pLabelNum = Label::createWithTTF(buf, "fonts/cuti.ttf", 30);
        bg->addChild(pLabelNum);
        pLabelNum->setColor(Color3B(252,111,7));
        pLabelNum->setPosition(bgSize.width / 2, -bgSize.height*0.025f);
        //保存碎片
    }
    else
    {
        propInfo* pInfo = CDataManager::getPropInfoById(monsterId);
        Sprite* propBg = Sprite::createWithSpriteFrameName(propIconBg[pInfo->m_iItemLevel - 1].c_str());
        bg->addChild(propBg);
        propBg->setScale(1.8);
        propBg->setPosition(bgSize.width / 2, bgSize.height * 0.45f);
        
        Sprite* propIcon = Sprite::createWithSpriteFrameName(pInfo->m_sItemIcon.c_str());
        propIcon->setScale(1.8);
        bg->addChild(propIcon);
        propIcon->setPosition(bgSize.width / 2, bgSize.height * 0.45f);
        sprintf(buf, "%s", pInfo->m_sItemIcon.c_str());
        
        Sprite* propKuang = Sprite::createWithSpriteFrameName(buf);
        //bg->addChild(propKuang);
        propKuang->setPosition(bgSize.width / 2, bgSize.height * 0.45f);

        //名字、数量
        sprintf(buf, "%s x%d", pInfo->m_sItemName.c_str(), 1);
        Label* pLabelNum = Label::createWithTTF(buf, "fonts/cuti.ttf",30);
        bg->addChild(pLabelNum);
        pLabelNum->setPosition(bgSize.width / 2, -bgSize.height*0.025f);
        
        Color3B Rgb = Color3B(255,255,255);
        
         if(pInfo->m_iItemLevel==2)
        {
            Rgb = Color3B(18,203,31);
        }
        else if(pInfo->m_iItemLevel ==3)
        {
            Rgb = Color3B(16,105,216);
        }
        else if(pInfo->m_iItemLevel ==4)
        {
            Rgb = Color3B(162,16,216);
        }
        else if(pInfo->m_iItemLevel == 5)
        {
            Rgb = Color3B(252,111,7);
        }
        pLabelNum->setColor(Rgb);

    }

    if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_ENTER_STORAGE)&& (guiderManagerLayer::getInstance()->getStepNum() == 4))
    {
        guiderManagerLayer::getInstance()->setStepNum(guiderManagerLayer::getInstance()->getStepNum() + 1);
        guiderManagerLayer::getInstance()->removeGuiderLayer();
        CDataManager::saveGuideInfoToDB(guiderManagerLayer::getInstance()->getGuideType());
    }
    
    CallFunc* rmv = CallFunc::create(CC_CALLBACK_0(CommonPropLayer::postGuideInfo, this));
    runAction(Sequence::create(DelayTime::create(3.0f), rmv, NULL));
    
  }

void CommonPropLayer::postGuideInfo()
{
    if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_ENTER_STORAGE)&& (guiderManagerLayer::getInstance()->getStepNum() == 5))
    {
        guiderManagerLayer::getInstance()->setStepNum(guiderManagerLayer::getInstance()->getStepNum() + 1);
        NotificationCenter::getInstance()->postNotification("storageGuide");
    }
    removeFromParent();
}

void CommonPropLayer::lightEffect(Node *pTf)
{
    pTf->runAction(RepeatForever::create(RotateBy::create(1.0, 90)));
}

void CommonPropLayer::addParticleEffect(float dt)
{
    ParticleSystemQuad* effect1 = ParticleSystemQuad::create("particleSystem/hechengBomb.plist") ;
    addChild(effect1);
    effect1->setScale(1.3f);
    effect1->setAutoRemoveOnFinish(true);
    Vec2 pos = Vec2(m_winSize.width * CCRANDOM_0_1(), m_winSize.height * CCRANDOM_0_1());
    effect1->setPosition(pos);
    
//    ParticleSystemQuad* effect2 = ParticleSystemQuad::create("particleSystem/hechengBomb.plist") ;
//    addChild(effect2);
//    effect2->setAutoRemoveOnFinish(true);
//    pos = Vec2(m_winSize.width * CCRANDOM_0_1(), m_winSize.height * CCRANDOM_0_1());
//    effect2->setPosition(pos);
}

void CommonPropLayer::startShowParticle()
{
    schedule(schedule_selector(CommonPropLayer::randTime), 0.4f);
    ParticleSystemQuad* effect1 = ParticleSystemQuad::create("particleSystem/star2.plist") ;
    addChild(effect1);
    effect1->setAutoRemoveOnFinish(true);
    effect1->setScale(1.2f);
    effect1->setPosition(m_winSize.width * 0.5f, m_winSize.height * 0.73f);
}

void CommonPropLayer::randTime(float dt)
{
    float time = CCRANDOM_0_1() * 0.5;
    scheduleOnce(schedule_selector(CommonPropLayer::addParticleEffect), time);
}

void CommonPropLayer::upgradeLevel()
{
    int effectId = CPersonalData::getInstance()->getEffectId();
    SimpleAudioEngine::getInstance()->stopEffect(effectId);
    SimpleAudioEngine::getInstance()->playEffect("music/upgrade.mp3");
    //光
    Sprite* light = Sprite::createWithSpriteFrameName("light.png");
    addChild(light);
    light->setPosition(Vec2(m_winSize.width / 2, m_winSize.height * 0.8));
    CallFuncN* pAction = CallFuncN::create(CC_CALLBACK_1(CommonPropLayer::lightEffect, this));
    light->runAction(Sequence::create(Hide::create(), DelayTime::create(0.6f), Show::create(), ScaleTo::create(0.2, 6.0),
                                      pAction,NULL));
    char buf[128];
    Sprite* bg = Sprite::createWithSpriteFrameName("upgrade_bg.png");
    addChild(bg);
    bg->setScale(7.0f);
    bg->setPosition(Vec2(m_winSize.width * 1.5f, m_winSize.height * 0.45));
    
    Size bgSize = bg->getBoundingBox().size;
    LayerColor* pLayer= LayerColor::create(Color4B(255, 0, 0, 0), bgSize.width, bgSize.height);
    addChild(pLayer);
    pLayer->setPosition(bg->getPositionX() - bg->getBoundingBox().size.width * 0.5f, bg->getPositionY() - bg->getBoundingBox().size.height * 0.5f);
    
    Sprite* leftTop = Sprite::createWithSpriteFrameName("upgrade_decorate.png");
    leftTop->setFlippedX(true);
    leftTop->setFlippedY(true);
    addChild(leftTop);
    leftTop->setPosition(Vec2(m_winSize.width * 0.96, m_winSize.height * 0.72));
    leftTop->runAction(EaseSineInOut::create(MoveTo::create(0.3f, Vec2(m_winSize.width * 0.32f, m_winSize.height * 0.72))));
    
    Sprite* rightTop = Sprite::createWithSpriteFrameName("upgrade_decorate.png");
    rightTop->setFlippedY(true);
    addChild(rightTop);
    rightTop->setPosition(Vec2(m_winSize.width * 2.04f   , m_winSize.height * 0.72));
    rightTop->runAction(EaseSineInOut::create(MoveTo::create(0.3f, Vec2(m_winSize.width * 0.68f, m_winSize.height * 0.72))));

    Sprite* leftBottom = Sprite::createWithSpriteFrameName("upgrade_decorate.png");
    addChild(leftBottom);
    leftBottom->setFlippedX(true);
    leftBottom->setPosition(Vec2(m_winSize.width * 0.96, m_winSize.height * 0.18f));
    leftBottom->runAction(EaseSineInOut::create(MoveTo::create(0.3f, Vec2(m_winSize.width * 0.32f, m_winSize.height * 0.18f))));
    
    Sprite* rightBottom = Sprite::createWithSpriteFrameName("upgrade_decorate.png");
    addChild(rightBottom);
    
    rightBottom->setPosition(Vec2(m_winSize.width * 2.04f, m_winSize.height * 0.18f));
    rightBottom->runAction(EaseSineInOut::create(MoveTo::create(0.3f, Vec2(m_winSize.width * 0.68f, m_winSize.height * 0.18f))));
    //light->runAction();
    
    Sprite* icon = Sprite::createWithSpriteFrameName("upgrade_di.png");
    addChild(icon);
    icon->setScale(1.0f);
    icon->setPosition(Vec2(m_winSize.width / 2, m_winSize.height * 1.5f));
    icon->runAction(EaseSineInOut::create(MoveTo::create(0.4f, Vec2(m_winSize.width / 2, m_winSize.height * 0.745f))));
    
    Sprite* word = Sprite::createWithSpriteFrameName("upgrade_word.png");
    addChild(word, 2);
    word->setPosition(Vec2(m_winSize.width / 2, m_winSize.height * 0.73f));
    word->setScale(4.5f);
    word->runAction(Hide::create());
    CallFunc* effect = CallFunc::create(CC_CALLBACK_0(CommonPropLayer::startShowParticle, this));
    
    word->runAction(Sequence::create(DelayTime::create(0.95f), Show::create(), ScaleTo::create(0.2, 2.4f), effect, NULL));
    
    //属性变化
    upgradeInfo* pInfoBefore = CDataManager::getUpgradeInfoByLevel(m_beforeLevel);
    upgradeInfo* pInfoAfter = CDataManager::getUpgradeInfoByLevel(m_afterUpgrade);
    
    

    
    int addStamina = 0;
    for (int i = m_beforeLevel; i < m_afterUpgrade; i++) {
        addStamina += CDataManager::getUpgradeInfoByLevel(i)->upgradeStamina;
    }
    
//    CPersonalData::getInstance()->SetPhysicalNum(CPersonalData::getInstance()->GetPhysicalNum()+addStamina);
    
    
//    CNetWorkDataSingle::GetSingletonPtr()->SaveUserMCF(this,NULL,0,0,addStamina);
    
    
    //等级
    sprintf(buf, "等级: %d", m_beforeLevel);
    Label* levelLeft = Label::createWithSystemFont(buf, "Arial", 35);
    levelLeft->setPosition(Vec2(bgSize.width * 0.3f, bgSize.height * 0.69f));
    levelLeft->setAnchorPoint(Vec2(0, 0.5));
    pLayer->addChild(levelLeft);
    
    Sprite* arrowLevel = Sprite::createWithSpriteFrameName("upgrade_arrow.png");
    pLayer->addChild(arrowLevel);
    arrowLevel->setAnchorPoint(Vec2(0, 0.5));
    arrowLevel->setPosition(Vec2(levelLeft->getPositionX() + levelLeft->getContentSize().width + 15, levelLeft->getPositionY()));
    
    sprintf(buf, "%d", m_afterUpgrade);
    Label* levelRight = Label::createWithSystemFont(buf, "Arial", 35);
    levelRight->setColor(Color3B(39, 214, 16));
    levelRight->setPosition(Vec2(arrowLevel->getPositionX() + arrowLevel->getContentSize().width + 20, arrowLevel->getPositionY()));
    pLayer->addChild(levelRight);
    
    sprintf(buf, "士兵最大等级: %d", pInfoBefore->upperLevel);
    Label* maxLevel = Label::createWithSystemFont(buf, "Arial", 35);
    maxLevel->setPosition(Vec2(bgSize.width * 0.3f, levelLeft->getPositionY() - 60));
    maxLevel->setAnchorPoint(Vec2(0, 0.5));
    pLayer->addChild(maxLevel);
    Sprite* arrowMaxLevel = Sprite::createWithSpriteFrameName("upgrade_arrow.png");
    pLayer->addChild(arrowMaxLevel);
    arrowMaxLevel->setAnchorPoint(Vec2(0, 0.5));
    arrowMaxLevel->setPosition(Vec2(maxLevel->getPositionX() + maxLevel->getContentSize().width + 15, maxLevel->getPositionY()));
    
    sprintf(buf, "%d", pInfoAfter->upperLevel);
    Label* maxLevelRight = Label::createWithSystemFont(buf, "Arial", 35);
    maxLevelRight->setAnchorPoint(Vec2(0, 0.5));
    maxLevelRight->setColor(Color3B(39, 214, 16));
    maxLevelRight->setPosition(Vec2(arrowMaxLevel->getPositionX() + arrowMaxLevel->getContentSize().width + 20, arrowMaxLevel->getPositionY()));
    pLayer->addChild(maxLevelRight);
    
    sprintf(buf, "当前体力: %d", CPersonalData::getInstance()->GetPhysicalNum() - addStamina);
    Label* userStaminaLeft = Label::createWithSystemFont(buf, "Arial", 35);
    userStaminaLeft->setPosition(Vec2(bgSize.width * 0.3f, maxLevel->getPositionY() - 60));
    userStaminaLeft->setAnchorPoint(Vec2(0, 0.5));
    pLayer->addChild(userStaminaLeft);
    Sprite* arrowStaminaLeft = Sprite::createWithSpriteFrameName("upgrade_arrow.png");
    pLayer->addChild(arrowStaminaLeft);
    arrowStaminaLeft->setAnchorPoint(Vec2(0, 0.5));
    arrowStaminaLeft->setPosition(Vec2(userStaminaLeft->getPositionX() + userStaminaLeft->getContentSize().width + 15, userStaminaLeft->getPositionY()));
    
    sprintf(buf, "%d", CPersonalData::getInstance()->GetPhysicalNum());
    Label* staminaRight = Label::createWithSystemFont(buf, "Arial", 35);
    staminaRight->setAnchorPoint(Vec2(0, 0.5));
    staminaRight->setColor(Color3B(39, 214, 16));
    staminaRight->setPosition(Vec2(arrowStaminaLeft->getPositionX() + arrowStaminaLeft->getContentSize().width + 20, arrowStaminaLeft->getPositionY()));
    pLayer->addChild(staminaRight);
    
    sprintf(buf, "体力上限: %d", pInfoBefore->upperStamina);
    Label* maxStaminaLeft = Label::createWithSystemFont(buf, "Arial", 35);
    maxStaminaLeft->setPosition(Vec2(bgSize.width * 0.3f, userStaminaLeft->getPositionY() - 60));
    maxStaminaLeft->setAnchorPoint(Vec2(0, 0.5));
    pLayer->addChild(maxStaminaLeft);
    
    Sprite* arrowMaxStaminaLeft = Sprite::createWithSpriteFrameName("upgrade_arrow.png");
    pLayer->addChild(arrowMaxStaminaLeft);
    arrowMaxStaminaLeft->setAnchorPoint(Vec2(0, 0.5));
    arrowMaxStaminaLeft->setPosition(Vec2(maxStaminaLeft->getPositionX() + maxStaminaLeft->getContentSize().width + 15, maxStaminaLeft->getPositionY()));
    
    sprintf(buf, "%d", pInfoAfter->upperStamina);
    Label* maxStaminaRight = Label::createWithSystemFont(buf, "Arial", 35);
    maxStaminaRight->setAnchorPoint(Vec2(0, 0.5));
    maxStaminaRight->setColor(Color3B(39, 214, 16));
    maxStaminaRight->setPosition(Vec2(arrowMaxStaminaLeft->getPositionX() + arrowMaxStaminaLeft->getContentSize().width + 20,
                                      arrowMaxStaminaLeft->getPositionY()));
    pLayer->addChild(maxStaminaRight);
    bg->runAction(EaseSineInOut::create(MoveTo::create(0.3f, Vec2(m_winSize.width * 0.5f, m_winSize.height * 0.45f))));
    pLayer->runAction(EaseSineInOut::create(MoveTo::create(0.3f, Vec2(m_winSize.width * 0.5f - bg->getBoundingBox().size.width * 0.5f, m_winSize.height * 0.45f - bg->getBoundingBox().size.height * 0.5f))));
    
    Sprite* normal = Sprite::createWithSpriteFrameName("bluean.png");
    Sprite* select = Sprite::createWithSpriteFrameName("bluean.png");
    
    select->setColor(Color3B::GRAY);
    LYMenuItemSprite* pSureItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(CommonPropLayer::btnCallBack, this));
    pSureItem->setTag(UPGRADE_SUER_TAG);
    pSureItem->setScale(1.25f);
    Menu* pMenu = Menu::create(pSureItem, NULL);
    addChild(pMenu);
    pMenu->setPosition(Vec2(m_winSize.width / 2, m_winSize.height * 0.1));
    
    Label* sureLabel = Label::createWithSystemFont("确定", "Arial", 18);
    pSureItem->addChild(sureLabel);
    //sureLabel->setScale(0.5f);
    sureLabel->setPosition(Vec2(normal->getBoundingBox().size.width / 2, normal->getBoundingBox().size.height / 2));
}

void CommonPropLayer::initHeroAdvance()
{
    SimpleAudioEngine::getInstance()->playEffect("music/lvlSucess.mp3");
    monsterInfo* pInfo = CDataManager::getMonsterInfoByMonsterId(monsterId);
    //光
    Sprite* light = Sprite::createWithSpriteFrameName("backlight3.png");
    addChild(light);
    light->setScale(4.5);
    light->setPosition(Vec2(m_winSize.width / 2, m_winSize.height * 0.5f));
    light->runAction(RepeatForever::create(RotateBy::create(2.5, 90)));
    
    Sprite* word = NULL;
    if (CDataManager::getMonsterStarById(pInfo->monsterId) > 1) {
        word = Sprite::createWithSpriteFrameName("advanceWord.png");
    }
    else{
        word = Sprite::createWithSpriteFrameName("getHero.png");
    }
    
    addChild(word);
    word->setScale(1.5f);
    word->setPosition(Vec2(m_winSize.width / 2, m_winSize.height * 0.86));
    
    Sprite* advanceBg = Sprite::createWithSpriteFrameName("getHero_bg.png");
    addChild(advanceBg);
    advanceBg->setScale(3.0f);
    advanceBg->setPosition(Vec2(m_winSize.width / 2, m_winSize.height / 2));
    Size adSize = Size(advanceBg->getContentSize().width * 0.6f, advanceBg->getContentSize().height * 0.6f);
    
//    //法阵
//    Sprite* fazhen = Sprite::createWithSpriteFrameName("fazhen.png");
//    addChild(fazhen);
//    fazhen->setScaleX(1.8f);
//    fazhen->setScaleY(0.35f);
//    fazhen->setPosition(Vec2(m_winSize.width / 2, m_winSize.height * 0.35f));
    
    //英雄
    modelInfo* pModelInfo = CDataManager::getModelInfoByModelId(pInfo->modelId);
    Sprite3D* m_hero = Sprite3D::create(pModelInfo->modelName.getCString());
    advanceBg->addChild(m_hero);
    m_hero->setGlobalZOrder(100);
    Size modeSize = m_hero->getBoundingBox().size;

    
    float scale = adSize.width/modeSize.width;
    

    if(scale > adSize.height/modeSize.height)
    {
        scale = adSize.height/modeSize.height;
    }
    m_hero->setScale(scale*4);
    if((pInfo->monsterId == 9) || (pInfo->monsterId == 12) || (pInfo->monsterId == 15))
    {
        m_hero->setRotation3D(Vec3(0, 40, 0));
    }

    m_hero->setPosition(Vec2(advanceBg->getContentSize().width / 2, advanceBg->getContentSize().height * 0.23f));
    auto animation = Animation3D::create(pModelInfo->modelName.getCString());
    if (animation)
    {
        auto animate = Animate3D::create(animation, pModelInfo->idleFrame.start/60, 1/60);
        cocos2d::Action* _swim = RepeatForever::create(animate);
        m_hero->runAction(_swim);
    }
    
    //暂停3秒之后自动关闭
    CallFunc* rmv = CallFunc::create(CC_CALLBACK_0(CommonPropLayer::remove, this));
    runAction(Sequence::create(DelayTime::create(3.0f), rmv, NULL));
    
//    Sprite* normal = Sprite::createWithSpriteFrameName("yellowan.png");
//    Sprite* select = Sprite::createWithSpriteFrameName("yellowan.png");
//    select->setColor(Color3B::GRAY);
//    LYMenuItemSprite* pItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(CommonPropLayer::btnCallBack, this));
//    pItem->setTag(ADVANCE_SURE_TAG);
//    Menu* pMenu = Menu::create(pItem, NULL);
//    pMenu->setPosition(Vec2(m_winSize.width / 2, m_winSize.height * 0.135f));
//    addChild(pMenu);
//    
//    Label* pLabel = Label::createWithSystemFont("确定", "Arial", 30);
//    pItem->addChild(pLabel);
//    pLabel->setColor(Color3B::BLUE);
//    pLabel->setPosition(Vec2(normal->getContentSize().width / 2, normal->getContentSize().height * 0.58));
}


void CommonPropLayer::initBuyDebris()
{
    char buf[128];
    m_pBg = Sprite::createWithSpriteFrameName("common_bg.png");
    addChild(m_pBg);
    m_pBg->setScaleY(1.12f);
    m_pBg->setPosition(Vec2(m_winSize.width / 2, m_winSize.height / 2));
    
    Size bgSize = m_pBg->getBoundingBox().size;
    m_pLayer = LayerColor::create(Color4B(200, 0, 0, 0), bgSize.width, bgSize.height);
    addChild(m_pLayer);
    m_pLayer->setPosition(m_winSize.width / 2 - bgSize.width / 2, m_winSize.height / 2 - bgSize.height / 2);
    
    Label* pTitle = Label::createWithSystemFont("购买碎片", "Arial", 30);
    m_pLayer->addChild(pTitle);
    pTitle->setPosition(Vec2(bgSize.width / 2, bgSize.height * 0.895f));
    
    monsterInfo* pInfo = CDataManager::getMonsterInfoByMonsterId(monsterId);
    //碎片图标
    Sprite* debrisIcon = Sprite::createWithSpriteFrameName(pInfo->monsterIcon);
    m_pLayer->addChild(debrisIcon);
    debrisIcon->setPosition(Vec2(bgSize.width * 0.155, bgSize.height * 0.72f));
    
    //碎片背景
    Sprite* debrisBg = Sprite::createWithSpriteFrameName("debrisBg.png");
    m_pLayer->addChild(debrisBg);
    debrisBg->setScale(3.2f);
    debrisBg->setPosition(Vec2(bgSize.width * 0.155, bgSize.height * 0.72f));
    
    //碎片名字
    sprintf(buf, "%s碎片", pInfo->monsterName.c_str());
    Label* debrisName = Label::createWithSystemFont(buf, "Arial", 28);
    m_pLayer->addChild(debrisName);
    debrisName->setAnchorPoint(Vec2(0, 0.5));
    debrisName->setPosition(Vec2(debrisIcon->getPositionX() + debrisIcon->getContentSize().width * 0.9f, bgSize.height * 0.765));
    //单价
    Label* price = Label::createWithSystemFont("单价: ", "Arial", 28);
    price->setColor(Color3B::YELLOW);
    price->setAnchorPoint(Vec2(0, 0.5));
    m_pLayer->addChild(price);
    price->setPosition(Vec2(debrisName->getPositionX(), bgSize.height * 0.665));
    
    Sprite* coin = Sprite::createWithSpriteFrameName("stone.png");
    coin->setAnchorPoint(Vec2(0, 0.5));
    coin->setScale(0.65f);
    m_pLayer->addChild(coin);
    coin->setPosition(Vec2(price->getPositionX() + price->getContentSize().width, price->getPositionY() - 5));
    
    Label* labelNum = Label::createWithSystemFont("100", "Arial", 28);//暂定为100魔法石
    labelNum->setAnchorPoint(Vec2(0, 0.5));
    labelNum->setColor(Color3B::YELLOW);
    m_pLayer->addChild(labelNum);
    labelNum->setPosition(Vec2(coin->getPositionX() + coin->getBoundingBox().size.width, price->getPositionY()));
    
    Sprite* backgroundSprite = Sprite::createWithSpriteFrameName("jindu.png");
    Sprite* pogressSprite = Sprite::createWithSpriteFrameName("processyellow.png");
    Sprite* thumbSprite = Sprite::createWithSpriteFrameName("ladong.png");
    
    cocos2d::extension::ControlSlider* m_solider = cocos2d::extension::ControlSlider::create(backgroundSprite,pogressSprite,thumbSprite);
    m_solider->setPosition(Vec2(bgSize.width / 2, bgSize.height * 0.42f));
    
    m_solider->setMaximumValue(m_monster->m_number - m_availNum);
    //myslider->setMaximumAllowedValue(50);  //这里，如果启用则只能滑动一半哦
    m_solider->setMinimumValue(0);
    m_solider->setValue(1);
    m_sliderValue = 1;
    m_pLayer->addChild(m_solider, 0, 200);
    m_solider->addTargetWithActionForControlEvents(this, cccontrol_selector(CommonPropLayer::soliderCallBack), Control::EventType::VALUE_CHANGED);
    
    m_arrow = Sprite::createWithSpriteFrameName("numPoint.png");
    m_solider->addChild(m_arrow);
    m_arrow->setPosition(Vec2(m_solider->getContentSize().width * m_solider->getValue()/m_solider->getMaximumValue() - 5, m_solider->getContentSize().height * 1.42f));
    
    sprintf(buf, "%d", int(m_solider->getValue()));
    m_pSliderValue = Label::createWithSystemFont(buf, "Arial", 15);
    m_arrow->addChild(m_pSliderValue);
    m_pSliderValue->setPosition(Vec2(m_arrow->getContentSize().width / 2, m_arrow->getContentSize().height * 0.7f));
    
    sprintf(buf, "购买个数: %d", int(m_solider->getValue()));
    m_buyNum = Label::createWithSystemFont(buf, "Arial", 24);
    m_pLayer->addChild(m_buyNum);
    m_buyNum->setPosition(Vec2(bgSize.width * 0.25, bgSize.height * 0.3f));//m_costNum
    
    Label* cost = Label::createWithSystemFont("花费: ", "Arial", 24);
    m_pLayer->addChild(cost);
    cost->setColor(Color3B::YELLOW);
    cost->setAnchorPoint(Vec2(0, 0.5));
    cost->setPosition(Vec2(bgSize.width * 0.65, bgSize.height * 0.3f));
    
    Sprite* costCoin = Sprite::createWithSpriteFrameName("stone.png");
    costCoin->setAnchorPoint(Vec2(0, 0.5));
    m_pLayer->addChild(costCoin);
    costCoin->setScale(0.65f);
    costCoin->setPosition(Vec2(cost->getPositionX() + cost->getContentSize().width + 10, cost->getPositionY() - 5));
    
    sprintf(buf, "%d", int(m_solider->getValue() * 100));
    m_costNum = Label::createWithSystemFont(buf, "Arial", 24);
    m_costNum->setAnchorPoint(Vec2(0, 0.5));
    m_pLayer->addChild(m_costNum);
    m_costNum->setPosition(Vec2(costCoin->getPositionX() + costCoin->getBoundingBox().size.width + 10, cost->getPositionY()));
    
    
    m_menu = Menu::create();
    m_pLayer->addChild(m_menu);
    m_menu->setPosition(Vec2::ZERO);
    addBtn();
}

void CommonPropLayer::addBtn()
{
    Size bgSize = m_pBg->getBoundingBox().size;
    Sprite* normal = Sprite::createWithSpriteFrameName("minus.png");
    Sprite* select = Sprite::createWithSpriteFrameName("minus.png");
    select->setColor(Color3B::GRAY);
    LYMenuItemSprite* minusItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(CommonPropLayer::btnCallBack, this));
    minusItem->setTag(BUY_NUM_MINUS_TAG);
    minusItem->setPosition(Vec2(bgSize.width * 0.095f, bgSize.height * 0.42f));
    m_menu->addChild(minusItem, 0, BUY_NUM_MINUS_TAG);
    
    normal = Sprite::createWithSpriteFrameName("plus.png");
    select = Sprite::createWithSpriteFrameName("plus.png");
    select->setColor(Color3B::GRAY);
    LYMenuItemSprite* plusItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(CommonPropLayer::btnCallBack, this));
    plusItem->setTag(BUY_NUM_ADD_TAG);
    plusItem->setPosition(Vec2(bgSize.width * 0.905f, bgSize.height * 0.42f));
    m_menu->addChild(plusItem, 0, BUY_NUM_ADD_TAG);

    
    normal = Sprite::createWithSpriteFrameName("redbtn.png");
    select = Sprite::createWithSpriteFrameName("redbtn.png");
    select->setColor(Color3B::GRAY);
    LYMenuItemSprite* cancelItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(CommonPropLayer::btnCallBack, this));
    cancelItem->setTag(BUY_CANCEL_TAG);
    cancelItem->setScale(3.0f);
    cancelItem->setPosition(Vec2(bgSize.width * 0.25, bgSize.height * 0.135));
    m_menu->addChild(cancelItem, 0, BUY_CANCEL_TAG);
    Label* cancel = Label::createWithSystemFont("取消", "Arial", 32);
    cancelItem->addChild(cancel);
    cancel->setScale(0.3f);
    cancel->setPosition(Vec2(normal->getContentSize().width / 2, normal->getContentSize().height / 2));
    
    normal = Sprite::createWithSpriteFrameName("greenbtn.png");
    select = Sprite::createWithSpriteFrameName("greenbtn.png");
    select->setColor(Color3B::GRAY);
    LYMenuItemSprite* sureItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(CommonPropLayer::btnCallBack, this));
    m_menu->addChild(sureItem, 0, BUY_SURE_TAG);
    sureItem->setScale(3.0f);
    sureItem->setTag(BUY_SURE_TAG);
    sureItem->setPosition(Vec2(bgSize.width * 0.75, bgSize.height * 0.135));
    Label* sure = Label::createWithSystemFont("确定", "Arial", 32);
    sureItem->addChild(sure);
    sure->setScale(0.3f);
    sure->setPosition(Vec2(normal->getContentSize().width / 2, normal->getContentSize().height / 2));
}

void CommonPropLayer::btnCallBack(Ref* pSender)
{
    int tag = ((Node*)pSender)->getTag();
    if(tag == BUY_CANCEL_TAG || tag == USERPROP_SURE_TAG || tag == ADVANCE_SURE_TAG || tag == EXCHANGE_CANCEL_TAG)
    {
        if(tag == BUY_CANCEL_TAG)
        {
            ((CHeroCompoundLayer*)getParent())->showHero();
            if (m_pTarget && m_pHandler)
            {
                (m_pTarget->*m_pHandler)(pSender);
            }
        }
        
        //响应商店回调
        if(tag == EXCHANGE_CANCEL_TAG)
        {
            if (m_pCancelTarget &&m_pCancelHandler)
            {
                (m_pCancelTarget->*m_pCancelHandler)(pSender);
            }
        }
        
        removeFromParent();
    }
    else if( tag == UPGRADE_SUER_TAG)
    {
        
        if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_GET_TASK) && (guiderManagerLayer::getInstance()->getStepNum() == 2))
        {
            guiderManagerLayer::getInstance()->setStepNum(guiderManagerLayer::getInstance()->getStepNum() + 1);
            guiderManagerLayer::getInstance()->createLayer(getParent());
        }
        if(CPersonalData::getInstance()->getInsertHero())
        {
            CFightWinLayer* parent = (CFightWinLayer*)getParent();
            if(parent)
            {
                parent->showNewHero();
            }
        }
        unschedule(schedule_selector(CommonPropLayer::addParticleEffect));
        removeFromParent();
    }
    else if(tag == GAME_CONTINUE_TAG)
    {
        if(m_type == TOWER_MODE_INTRODUCE)
        {
            CSharedPlist::getInstance()->erasePlist("plist/level.plist");
        }
        removeFromParent();
        Director::getInstance()->resume();
    }
    else if(tag == CLICK_EXIT_TAG)
    {
        removeFromParent();
        Director::getInstance()->resume();
    }
    else if(tag == GAME_EXIT_TAG)
    {
        removeFromParent();
        Director::getInstance()->resume();
        if(!CPersonalData::getInstance()->getJumpFromSkill())
        {
            CPersonalData::getInstance()->setisFailManual(false);
        }
        
        SimpleAudioEngine::getInstance()->stopBackgroundMusic();
        
        
        if((CFightData::getInstance()->getFightType() == FIGHT_PVP_TYPE) && (CPersonalData::getInstance()->getFromVideoList()))
        {
            CLayerManager::GetSingletonPtr()->createLayer(eLayer_PVPLayer, NULL,false);
        }
        else{
//            CLayerManager::GetSingletonPtr()->createLayer(eLayer_MainMenu, NULL,false);
            CLayerManager::GetSingletonPtr()->createLayerWithLoading(eLayer_MainMenu, NULL,false);
        }
        
        //talkingdata
        int nowMidLevel = CFightData::getInstance()->getNowLevelId();
        int nowSmallLevel = CFightData::getInstance()->getNowMidLevelId();
        
        __String* tempStr = __String::createWithFormat("%d-%d",nowMidLevel,nowSmallLevel);
        TDCCMission::onFailed(tempStr->getCString(),"quit");
        
    }
    else if(tag == GAME_RESTART_TAG)
    {
        Director::getInstance()->resume();
        CFightData::getInstance()->getTheFightScene()->unscheduleUpdate();
        CFightData::getInstance()->getTheFightScene()->stopAllBattle();
        removeFromParent();
        SimpleAudioEngine::getInstance()->stopBackgroundMusic();
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
    else if(tag == EXCHANGE_STAMINA_TAG)
    {
        if(m_stone > CPersonalData::getInstance()->getUserStone())
        {
            CHitDialog *pDlg = CHitDialog::create();
            pDlg->createYesNO(this, menu_selector(CommonPropLayer::gotoShop),"魔法石不足，是否去充值？");
            addChild(pDlg);
        }
        else
        {
            userNum* pInfo= CDataManager::getUserInfoNum();
            if(pInfo && (pInfo->exchangeStaminaNum < 3))
            {
                //发送网络请求保存魔法石、金币
                CNetWorkDataSingle::GetSingletonPtr()->SaveUserMCF(this, menu_selector(CommonPropLayer::exchangeCallBack),0,-m_stone,m_addStamina);
                TDCCItem::onPurchase("兑换体力", 1, m_stone);
            }
            else
            {
                if(pInfo)
                {
                    Sprite * thetarget = CFightManager::getTheFlyWord("今日兑换次数已用尽","fonts/cuti.ttf");
                    thetarget->setPosition(Vec2(m_bg->getBoundingBox().size.width / 2, m_bg->getBoundingBox().size.height / 2));
                    m_bg->addChild(thetarget, 10);
                }
            }
        }
    }
    else if(tag == EXCHANGE_COIN_TAG)
    {
        //每次兑换的标准可能不一样，先这么处理
        if(m_stone > CPersonalData::getInstance()->getUserStone())
        {
            CHitDialog *pDlg = CHitDialog::create();
            pDlg->createYesNO(this, menu_selector(CommonPropLayer::gotoShop),"魔法石不足，是否去充值？");
            addChild(pDlg);
        }
        else
        {
            userNum* pInfo= CDataManager::getUserInfoNum();
            if(pInfo && (pInfo->exchangeCoinNum < 3))
            {
                //发送网络请求保存魔法石、金币
                CNetWorkDataSingle::GetSingletonPtr()->SaveUserMCF(this, menu_selector(CommonPropLayer::exchangeCallBack),m_addCoin,-m_stone,0);
                TDCCItem::onPurchase("兑换金币", 1, m_stone);
            }
            else
            {
                if(pInfo)
                {
                    Sprite * thetarget = CFightManager::getTheFlyWord("今日兑换次数已用尽","fonts/cuti.ttf");
                    thetarget->setPosition(Vec2(m_bg->getBoundingBox().size.width / 2, m_bg->getBoundingBox().size.height / 2));
                    m_bg->addChild(thetarget, 10);
                }
            }
        }
        
      
    }
    else{
        ControlSlider* solider = (ControlSlider*)m_pLayer->getChildByTag(200);
        if ((tag == BUY_SURE_TAG) && solider)
        {
            int stoneNum = solider->getValue();
            
            if (stoneNum>0) {
                //判断魔法石是否足够
                int num = CPersonalData::getInstance()->getUserStone();
                if(num < stoneNum*100)
                {
                    //如果无充值功能，则直接弹一句话
#ifdef NO_PAY
                    Sprite* temp = CFightManager::getTheFlyWord("魔法石不足");
                    temp->setPosition(m_winSize.width*0.5f,m_winSize.height*0.5f);
                    addChild(temp);
#endif
          
#ifndef NO_PAY
                    CHitDialog *pDlg = CHitDialog::create();
                    pDlg->createYesNO(this ,menu_selector(CommonPropLayer::gotoShop),"魔法石不足，是否去充值？");
                    addChild(pDlg);
#endif
                    
                }
                else{
                    CHeroCompoundLayer* parent = (CHeroCompoundLayer*)getParent();
                    parent->saveBuyResult(stoneNum * 100, stoneNum);
                    removeFromParent();
                }
            }
            if (m_pTarget && m_pHandler)
            {
                (m_pTarget->*m_pHandler)(pSender);
            }
        }
        else if(tag == BUY_NUM_MINUS_TAG)
        {
            if (solider) {
                solider->setValue(solider->getValue() - 1);
                checkBtnEnabled(solider);
            }
        }
        else if (tag == BUY_NUM_ADD_TAG)
        {
            if (solider) {
                solider->setValue(solider->getValue() + 1);
                checkBtnEnabled(solider);
            }
        }

    }
}

void CommonPropLayer::exchangeCallBack(Ref* pSender)
{
    if(STONE_EXCHANGE_COIN == m_type)
    {
        CDataManager::saveUpdateExchangeNum();
    }
    else{
        CDataManager::saveUpdateExchangeNum(1);
    }
    CPersonalData::getInstance()->setUpdateUserInfo(true);
    Sprite * thetarget = CFightManager::getTheFlyWord("兑换成功","fonts/cuti.ttf");
    thetarget->setPosition(m_winSize.width*0.5,m_winSize.height*0.5);
    getParent()->addChild(thetarget,99);
    
    if(m_type == STONE_EXCHANGE_COIN)
    {
        if (m_pTarget && m_pHandler)
        {
            (m_pTarget->*m_pHandler)(pSender);
        }
    }
    
    removeFromParent();
}

void CommonPropLayer::gotoShop(Ref* pSender)
{
    CLayerManager::GetSingletonPtr()->createLayer(eLayer_Charge,getParent(),false);
//    removeFromParent();
    String* tempStr = String::createWithFormat("1");
    EventParamMap paramMap;
    
    paramMap.insert(EventParamPair("点击次数",tempStr->getCString()));
    
    TDCCTalkingDataGA::onEvent("英雄宝石充值", &paramMap);
}


void CommonPropLayer::checkBtnEnabled(ControlSlider* slider)
{
    if(int(slider->getValue()) == int(slider->getMaximumValue()))
    {
        ((LYMenuItemSprite*)(m_menu->getChildByTag(BUY_NUM_ADD_TAG)))->setEnabled(false);
        ((LYMenuItemSprite*)(m_menu->getChildByTag(BUY_NUM_ADD_TAG)))->setColor(Color3B::GRAY);
    }
    else if (int(slider->getValue()) == int(slider->getMinimumValue()))
    {
        ((LYMenuItemSprite*)(m_menu->getChildByTag(BUY_NUM_MINUS_TAG)))->setEnabled(false);
        ((LYMenuItemSprite*)(m_menu->getChildByTag(BUY_NUM_MINUS_TAG)))->setColor(Color3B::GRAY);
    }
    else
    {
        ((LYMenuItemSprite*)(m_menu->getChildByTag(BUY_NUM_MINUS_TAG)))->setEnabled(true);
        ((LYMenuItemSprite*)(m_menu->getChildByTag(BUY_NUM_MINUS_TAG)))->setColor(Color3B::WHITE);
        ((LYMenuItemSprite*)(m_menu->getChildByTag(BUY_NUM_ADD_TAG)))->setEnabled(true);
        ((LYMenuItemSprite*)(m_menu->getChildByTag(BUY_NUM_ADD_TAG)))->setColor(Color3B::WHITE);
        
    }

}

void CommonPropLayer::soliderCallBack(Ref * sender, Control::EventType controlEvent)
{
    char buf[128];
    
    auto slide_control = (ControlSlider*)sender;//通过回调参数sender 获得ControlSlider
    int current_value = slide_control->getValue();//获取slide当前的值
    if(current_value != m_sliderValue)
    {
        SimpleAudioEngine::getInstance()->playEffect("music/slider.mp3");
        CCLOG("当前值 = %d", current_value);
    }
    sprintf(buf, "%d", current_value);
    m_sliderValue = current_value;
    m_arrow->setPosition(Vec2(slide_control->getContentSize().width * slide_control->getValue()/slide_control->getMaximumValue() - 5, m_arrow->getPositionY()));
    m_pSliderValue->setString(buf);
    sprintf(buf, "购买个数: %d", current_value);
    m_buyNum->setString(buf);
    sprintf(buf, "%d", current_value * 100);
    m_costNum->setString(buf);
    checkBtnEnabled(slide_control);
}