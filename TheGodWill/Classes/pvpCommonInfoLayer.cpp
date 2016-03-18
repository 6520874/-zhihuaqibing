//
//  pvpCommonInfoLayer.cpp
//  WarOfGod
//
//  Created by user on 15/6/15.
//
//

#include "pvpCommonInfoLayer.h"
#include "pvpMainLayer.h"
#include "DataManager.h"
#include "LYFightData.h"
#include "LYFightManager.h"
#include "LYBaseTower.h"
#include "LYPersonalData.h"
#include "Common.h"
#include "NetWorkDataSingle.h"
#include "zzbNetControlModule.h"
#include "zzbNewNetStruct.h"
#include "guiderManagerLayer.h"

pvpCommonInfoLayer::pvpCommonInfoLayer()
{
    m_listener = NULL;
    m_comBg = NULL;
    m_buildInfo = NULL;
    m_pNextBuildInfo = NULL;
    m_pTarget = NULL;
    m_pCancelTarget = NULL;
    m_pHandler = NULL;
    m_pCancelHandler = NULL;
    m_arrow = NULL;
    m_pCost = NULL;
    m_pGet = NULL;
    m_pSliderValue = NULL;
    m_pKuangshi = NULL;
    m_pStone = NULL;
    m_menu = NULL;
    m_node = NULL;
    m_pTime = NULL;
    m_sliderValue = 0;
    m_remainTime = 0;
    m_time = 0;
    m_productNum = 0;
    m_needTime = 0;
    m_flag = false;
    m_progress = NULL;
    m_pNum = NULL;
    m_pLimit = NULL;
    m_pNeedTime = NULL;
    m_productMenu = NULL;
    m_pDinshiTime = NULL;
    m_pClock = NULL;
    m_iDinshiVaule = 0;
    
}


pvpCommonInfoLayer::~pvpCommonInfoLayer()
{
    
}

void pvpCommonInfoLayer::onExit()
{
    _eventDispatcher->removeEventListener(m_listener);
    CBaseLayer::onExit();
}

pvpCommonInfoLayer* pvpCommonInfoLayer::create(int buildingId, int userUuildingId, MSGTYPE type)
{
    pvpCommonInfoLayer* pRef = new pvpCommonInfoLayer;
    pRef->m_buildingId = buildingId;
    pRef->m_msgType = type;
    pRef->m_userBuildingId = userUuildingId;
    if(pRef && pRef->init())
    {
        pRef->autorelease();
        return pRef;
    }
    else
    {
        delete pRef;
        pRef = NULL;
        return  NULL;
    }
}

pvpCommonInfoLayer* pvpCommonInfoLayer::create(int buildingId, MSGTYPE type, std::string content)
{
    pvpCommonInfoLayer* pRef = new pvpCommonInfoLayer;
    pRef->m_buildingId = buildingId;
    pRef->m_msgType = type;
    pRef->m_content = content;
    if(pRef && pRef->init())
    {
        pRef->autorelease();
        return pRef;
    }
    else
    {
        delete pRef;
        pRef = NULL;
        return  NULL;
    }
}

pvpCommonInfoLayer* pvpCommonInfoLayer::create(MSGTYPE type)
{
    pvpCommonInfoLayer* pRef = new pvpCommonInfoLayer;
    pRef->m_msgType = type;
    if(pRef && pRef->init())
    {
        pRef->autorelease();
        return pRef;
    }
    else
    {
        delete pRef;
        pRef = NULL;
        return  NULL;
    }
}


bool pvpCommonInfoLayer::onTouchBegan(Touch* pTouch, Event* pEvent)
{
    CPersonalData::getInstance()->setTouchRmvFlag(true);
    return true;
}

void pvpCommonInfoLayer::onTouchMoved(Touch* pTouch, Event* pEvent)
{
    
}

void pvpCommonInfoLayer::onTouchEnded(Touch* pTouch, Event* pEvent)
{
    if(!m_pLayer->getBoundingBox().containsPoint(pTouch->getLocationInView()))
    {
        if(PVP_COMMSG_UPGRADE_TIP == m_msgType)
        {
            ((pvpMainLayer*)getParent())->rmvNewTower();
        }
        else
        {
            ((pvpMainLayer*)getParent())->rmvTowerInfoLayer();
        }
    }
}

bool pvpCommonInfoLayer::init()
{
    if(!CBaseLayer::initWithColor(Color4B(0, 0, 0, 150)))
    {
        return false;
    }
    m_winSize = Director::getInstance()->getWinSize();
    m_listener = EventListenerTouchOneByOne::create();
    m_listener->setSwallowTouches(true);
    m_listener->onTouchBegan = CC_CALLBACK_2(pvpCommonInfoLayer::onTouchBegan, this);
    m_listener->onTouchMoved = CC_CALLBACK_2(pvpCommonInfoLayer::onTouchMoved, this);
    m_listener->onTouchEnded = CC_CALLBACK_2(pvpCommonInfoLayer::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(m_listener, this);
    
    initUI();
    return true;
}



void pvpCommonInfoLayer::soliderCallBackDinshi(Ref * sender, cocos2d::extension::Control::EventType controlEvent)
{
    //m_DinshiTime->setValue(10);

    auto slide_control = (cocos2d::extension::ControlSlider*)sender;//通过回调参数sender 获得ControlSlider‘
    
    int iCurrent_value = slide_control->getValue();//获取slide当前的
    
    
    //int iValue = iCurrent_value - m_iDinshiVaule;
    
    m_iDinshiVaule = iCurrent_value;
    
   
    int  iTime =  m_iDinshiVaule;
    
    char buf[50] = {0};
    
    sprintf(buf,"%d秒",iTime);
    
    m_pDinshiTime->setString(buf);
    
    
    //m_iDinshiVaule
    
}

void pvpCommonInfoLayer::initUI()
{
    if((m_msgType == PVP_COMMSG_DETAIL) || (m_msgType == PVP_COMMSG_UPGRADE) || (m_msgType == PVP_COMMSG_NOT_UPGRADE))
    {
        detailAndUpgradeUI();//升级和详情界面
    }
    else if(m_msgType == PVP_COMMSG_PRORUCT)
    {
        showProductInfo();
    }
    else if(m_msgType == PVP_COMMSG_EVOLUTION)
    {
        addEvolutionInfo();
    }
    else if(m_msgType == PVP_COMMSG_DINGSHI)
    {
        showDingshiInfo();
        
    }
}


void pvpCommonInfoLayer::OnOkDinshi(Ref *pSender)
{
    log("okOKDIngshi");
    
    
    CPersonalData::getInstance()->setTouchRmvFlag(true);
    
    int iBornTime = m_DinshiTime->getValue();
    
    
    if(iBornTime ==0)
    {
        iBornTime  = 60;
    }
    
    
    
    CFightData::getInstance()->setTheBornTime(iBornTime);
    
    
    
    CNetControlModule::sharedNetControlModule()->SaveDinShiTime(NULL,NULL, 1, iBornTime);
    
    
    ((pvpMainLayer*)getParent())->m_layer = NULL;
    removeFromParent();
    
   // int iBornTime = CFightData::getInstance()->getTheBornTime();
    
    
}

void pvpCommonInfoLayer::showDingshiInfo()
{
    m_comBg = Sprite::createWithSpriteFrameName("pvp_infoBg.png");
    addChild(m_comBg);
    m_comBg->setPosition(Vec2(m_winSize.width * 0.5f, m_winSize.height * 0.45f));
    m_comBg->setScaleX(2.0f);
    m_comBg->setScaleY(1.85f);
    
     char buf[50] = {0};
    
    
    Size bgSize = m_comBg->getBoundingBox().size;
    m_pLayer = LayerColor::create(Color4B(0, 0, 0, 0), bgSize.width, bgSize.height);
    addChild(m_pLayer);
    m_pLayer->setPosition(Vec2(m_comBg->getPositionX() - bgSize.width * 0.5f, m_comBg->getPositionY() - bgSize.height * 0.5f));
    
    //标题框
    Sprite* pTitleBg = Sprite::createWithSpriteFrameName("pvp_infoTitileBg.png");
    m_pLayer->addChild(pTitleBg);
    pTitleBg->setScale(1.2f);
    pTitleBg->setPosition(Vec2(bgSize.width * 0.5f, bgSize.height * 0.98f));
    m_buildInfo = CDataManager::getBuildInfo(m_buildingId);
    
    
    Label* pTitle = Label::createWithTTF("陷阱定时", "fonts/cuti.ttf", 34);
    m_pLayer->addChild(pTitle);
    pTitle->enableOutline(Color4B(0, 0, 0, 255), 3);
    pTitle->setPosition(Vec2(bgSize.width * 0.5f, bgSize.height * 0.98f));

    Sprite *pDilei = Sprite::createWithSpriteFrameName("pvp_dingshilei.png");
    pDilei->setPosition(bgSize.width*0.2,bgSize.height*0.7);
    m_pLayer->addChild(pDilei);
    
    
    Label *pHurtNum = Label::createWithTTF("伤害:","fonts/cuti.ttf",30);
    pHurtNum->setAnchorPoint(Vec2(0,0.5));
    
    pHurtNum->enableOutline(Color4B(0, 0, 0, 255), 3);
    pHurtNum->setPosition(Vec2(bgSize.width*0.55,bgSize.height*0.7));
    m_pLayer->addChild(pHurtNum);
    
    Sprite *pBing = Sprite::createWithSpriteFrameName("pvp_buildAttack.png");
    
    pBing->setPosition(Vec2(pHurtNum->getPositionX()+pHurtNum->getContentSize().width+pBing->getContentSize().width,bgSize.height*0.7));
    m_pLayer->addChild(pBing);

    
     m_buildInfo = CDataManager::getBuildInfo(m_buildingId);
    
    sprintf(buf,"%d",m_buildInfo->attack);
    
    Label *pNum = Label ::createWithTTF(buf,"fonts/cuti.ttf",30);
    pNum->setPosition(Vec2(pBing->getPositionX()+pBing->getContentSize().width+pNum->getContentSize().width,bgSize.height*0.7));
    pNum->enableOutline(Color4B(0, 0, 0, 255), 3);
    m_pLayer->addChild(pNum);

    
    Label  *pBorn = Label::createWithTTF("当前爆炸时间:","fonts/cuti.ttf",30);
    pBorn->enableOutline(Color4B(0, 0, 0, 255), 3);
    pBorn->setAnchorPoint(Vec2(0,0.5));
    pBorn->setPosition(bgSize.width*0.55,bgSize.height*0.6);
    m_pLayer->addChild(pBorn);


    int iBornTime = CFightData::getInstance()->getTheBornTime();
    
   
    sprintf(buf,"%d秒",iBornTime);
    
    
    Label *pBaoTime = Label::createWithTTF(buf,"fonts/cuti.ttf",30);

    pBaoTime->enableOutline(Color4B(0, 0, 0, 255), 3);
    pBaoTime->setPosition(pBorn->getPositionX()+pBorn->getContentSize().width+pBaoTime->getContentSize().width*0.5,bgSize.height*0.6);

    m_pLayer->addChild(pBaoTime);
    

    Sprite* backgroundSprite = Sprite::createWithSpriteFrameName("jindu.png");
    Sprite* pogressSprite = Sprite::createWithSpriteFrameName("processyellow.png");
    Sprite* thumbSprite = Sprite::createWithSpriteFrameName("ladong.png");
    
    m_DinshiTime = cocos2d::extension::ControlSlider::create(backgroundSprite,pogressSprite,thumbSprite);
    m_DinshiTime->setPosition(Vec2(bgSize.width*0.4,bgSize.height * 0.2f));
    m_pLayer->addChild(m_DinshiTime);
    m_DinshiTime->setMaximumValue(120);
    m_DinshiTime->setMinimumValue(1);
    
    
    m_pDinshiTime = Label::createWithTTF(buf,"fonts/cuti.ttf",30);
    m_pDinshiTime->enableOutline(Color4B(0, 0, 0, 255), 3);
    m_pDinshiTime->setPosition(m_DinshiTime->getPositionX()+m_pDinshiTime->getContentSize().width*0.5,bgSize.height*0.3);
    m_pLayer->addChild(m_pDinshiTime);
    
    
    Sprite *pOk = Sprite::createWithSpriteFrameName("fight_blueBtn.png");
    Sprite *pOkSelect = Sprite::createWithSpriteFrameName("fight_blueBtn.png");
    pOkSelect->setColor(Color3B::GRAY);
    
    LYMenuItemSprite *pOkItem = LYMenuItemSprite::create(pOk,pOkSelect,CC_CALLBACK_1(pvpCommonInfoLayer::OnOkDinshi, this));
    
    Label * pLabelReturn = Label::createWithTTF("确定","fonts/cuti.ttf",30);
    Size tBlueSize = pOkItem->getBoundingBox().size;
    pLabelReturn->setPosition(tBlueSize.width*0.5,tBlueSize.height*0.5);
    pOkItem->addChild(pLabelReturn);
    
    Menu *pMenuOk = Menu::create(pOkItem,NULL);
    pMenuOk->setPosition(bgSize.width*0.85,bgSize.height*0.2);
    
    m_pLayer->addChild(pMenuOk);

    
    m_DinshiTime->setValue(iBornTime);
    
    m_iDinshiVaule =  m_DinshiTime->getValue();
    
    m_DinshiTime->addTargetWithActionForControlEvents(this, cccontrol_selector(pvpCommonInfoLayer::soliderCallBackDinshi),cocos2d::extension::Control::EventType::VALUE_CHANGED);
}

void pvpCommonInfoLayer::addEvolutionInfo()
{
    m_comBg = Sprite::createWithSpriteFrameName("pvp_infoBg.png");
    addChild(m_comBg);
    m_comBg->setPosition(Vec2(m_winSize.width * 0.5f, m_winSize.height * 0.45f));
    m_comBg->setScaleX(2.0f);
    m_comBg->setScaleY(1.85f);
    
    Size bgSize = m_comBg->getBoundingBox().size;
    m_pLayer = LayerColor::create(Color4B(0, 0, 0, 0), bgSize.width, bgSize.height);
    addChild(m_pLayer);
    m_pLayer->setPosition(Vec2(m_comBg->getPositionX() - bgSize.width * 0.5f, m_comBg->getPositionY() - bgSize.height * 0.5f));
    
    //标题框
    Sprite* pTitleBg = Sprite::createWithSpriteFrameName("pvp_infoTitileBg.png");
    m_pLayer->addChild(pTitleBg);
    pTitleBg->setScale(1.2f);
    pTitleBg->setPosition(Vec2(bgSize.width * 0.5f, bgSize.height * 0.98f));
    m_buildInfo = CDataManager::getBuildInfo(m_buildingId);
    
    Label* pTitle = Label::createWithTTF("进化", "fonts/cuti.ttf", 34);
    m_pLayer->addChild(pTitle);
    pTitle->enableOutline(Color4B(0, 0, 0, 255), 3);
    pTitle->setPosition(Vec2(bgSize.width * 0.5f, bgSize.height * 0.98f));
    
    pvpBuildInfo* pBuildInfo = CDataManager::getBuildInfo(m_buildingId);
    //当前巡逻怪物
    Sprite* leftLightBg = Sprite::createWithSpriteFrameName("pvp_build_di.png");
    m_pLayer->addChild(leftLightBg);
    leftLightBg->setPosition(Vec2(bgSize.width * 0.2f, bgSize.height * 0.55f));
    
    
    std::vector<partolInfo> tempVec = pBuildInfo->allEvolutionVec;
    monsterInfo* pMonster = CDataManager::getMonsterInfoByMonsterId(tempVec[0].monsterId);
    modelInfo* pModelInfo = CDataManager::getModelInfoByModelId(pMonster->modelId);
    Sprite3D* pLeftHero = Sprite3D::create(pModelInfo->modelName.getCString());
    m_pLayer->addChild(pLeftHero);
    pLeftHero->setPosition(Vec2(bgSize.width * 0.21f, bgSize.height * 0.55f));
    pLeftHero->setGlobalZOrder(100);
    Size modeSize = pLeftHero->getBoundingBox().size;
    float scale;
    
    if(modeSize.width * modeSize.height < 30)
    {
        scale = 830/(modeSize.width * modeSize.height);
        pLeftHero->setScale(scale);
    }
    else if(modeSize.width * modeSize.height < 50)
    {
        pLeftHero->setScale(1250/(modeSize.width * modeSize.height));
    }
    else if(modeSize.width * modeSize.height < 100)
    {
        pLeftHero->setScale(1450/(modeSize.width * modeSize.height));
    }
    else if(modeSize.width * modeSize.height < 150){
        pLeftHero->setScale(1650/(modeSize.width * modeSize.height));
    }
    
    else{
        pLeftHero->setScale(28*8.5/modeSize.width);;
    }
    pLeftHero->setRotation3D(Vec3(0, 20, 0));

    //属性
    Label* pLeftLife = Label::createWithTTF("生命值:", "fonts/cuti.ttf", 26);
    m_pLayer->addChild(pLeftLife);
    pLeftLife->setAnchorPoint(Vec2(0, 0.5f));
    pLeftLife->enableOutline(Color4B(0, 0, 0, 255), 2);
    pLeftLife->setPosition(Vec2(bgSize.width * 0.08f, bgSize.height * 0.425f));
    
    Sprite* pLeftIcon = Sprite::createWithSpriteFrameName("pvp_blood.png");
    m_pLayer->addChild(pLeftIcon);
    pLeftIcon->setAnchorPoint(Vec2(0, 0.5f));
    pLeftIcon->setPosition(Vec2(pLeftLife->getPositionX() + pLeftLife->getBoundingBox().size.width + 8, pLeftLife->getPositionY()));
    
    __String* pBuf = __String::createWithFormat("%d", tempVec[0].life);
    Label* pLeftValue = Label::createWithTTF(pBuf->getCString(), "fonts/cuti.ttf", 30);
    m_pLayer->addChild(pLeftValue);
    pLeftValue->setAnchorPoint(Vec2(0, 0.5f));
    pLeftValue->enableOutline(Color4B(0, 0, 0, 255), 2);
    pLeftValue->setPosition(Vec2(pLeftIcon->getPositionX() + pLeftIcon->getBoundingBox().size.width + 8, pLeftLife->getPositionY()));
    
    Label* pLeftAttack = Label::createWithTTF("伤害:", "fonts/cuti.ttf", 24);
    m_pLayer->addChild(pLeftAttack);
    pLeftAttack->setAnchorPoint(Vec2(0, 0.5f));
    pLeftAttack->enableOutline(Color4B(0, 0, 0, 255), 2);
    pLeftAttack->setPosition(Vec2(bgSize.width * 0.1f, bgSize.height * 0.35f));
    
    Sprite* pLeftAttackIcon = Sprite::createWithSpriteFrameName("pvp_buildAttack.png");
    m_pLayer->addChild(pLeftAttackIcon);
    pLeftAttackIcon->setAnchorPoint(Vec2(0, 0.5f));
    pLeftAttackIcon->setPosition(Vec2(pLeftAttack->getPositionX() + pLeftAttack->getBoundingBox().size.width + 8, pLeftAttack->getPositionY()));
    
    pBuf = __String::createWithFormat("%d", tempVec[0].attack);
    Label* pLeftAttackValue = Label::createWithTTF(pBuf->getCString(), "fonts/cuti.ttf", 30);
    m_pLayer->addChild(pLeftAttackValue);
    pLeftAttackValue->setAnchorPoint(Vec2(0, 0.5f));
    pLeftAttackValue->enableOutline(Color4B(0, 0, 0, 255), 2);
    pLeftAttackValue->setPosition(Vec2(pLeftAttackIcon->getPositionX() + pLeftAttackIcon->getBoundingBox().size.width + 8,
                                       pLeftAttackIcon->getPositionY()));
    
    Sprite* pArrow = Sprite::createWithSpriteFrameName("pvp_evolutionArrow.png");
    m_pLayer->addChild(pArrow);
    pArrow->setPosition(Vec2(bgSize.width * 0.5f, bgSize.height * 0.69f));
    
    //进化后的怪物
    Sprite* rightLightBg = Sprite::createWithSpriteFrameName("pvp_build_di.png");
    m_pLayer->addChild(rightLightBg);
    rightLightBg->setPosition(Vec2(bgSize.width * 0.8f, bgSize.height * 0.55f));

    if(tempVec[1].monsterId == 14)
    {
        Sprite* pRightHero = Sprite::createWithSpriteFrameName("pvp_redlong.png");
        m_pLayer->addChild(pRightHero);
        pRightHero->setPosition(Vec2(bgSize.width * 0.79f, bgSize.height * 0.69f));
    }
    
    //属性
    Label* pRightLife = Label::createWithTTF("生命值:", "fonts/cuti.ttf", 26);
    m_pLayer->addChild(pRightLife);
    pRightLife->setAnchorPoint(Vec2(0, 0.5f));
    pRightLife->enableOutline(Color4B(0, 0, 0, 255), 2);
    pRightLife->setPosition(Vec2(bgSize.width * 0.69f, bgSize.height * 0.425f));
    
    Sprite* pRightIcon = Sprite::createWithSpriteFrameName("pvp_blood.png");
    m_pLayer->addChild(pRightIcon);
    pRightIcon->setAnchorPoint(Vec2(0, 0.5f));
    pRightIcon->setPosition(Vec2(pRightLife->getPositionX() + pRightLife->getBoundingBox().size.width + 8, pRightLife->getPositionY()));
    
    pBuf = __String::createWithFormat("%d", tempVec[1].life);
    Label* pRightValue = Label::createWithTTF(pBuf->getCString(), "fonts/cuti.ttf", 30);
    m_pLayer->addChild(pRightValue);
    pRightValue->setAnchorPoint(Vec2(0, 0.5f));
    pRightValue->enableOutline(Color4B(0, 0, 0, 255), 2);
    pRightValue->setPosition(Vec2(pRightIcon->getPositionX() + pRightIcon->getBoundingBox().size.width + 8, pRightLife->getPositionY()));
    
    Label* pRightAttack = Label::createWithTTF("伤害:", "fonts/cuti.ttf", 24);
    m_pLayer->addChild(pRightAttack);
    pRightAttack->setAnchorPoint(Vec2(0, 0.5f));
    pRightAttack->enableOutline(Color4B(0, 0, 0, 255), 2);
    pRightAttack->setPosition(Vec2(bgSize.width * 0.69f, bgSize.height * 0.35f));
    
    Sprite* pRightAttackIcon = Sprite::createWithSpriteFrameName("pvp_buildAttack.png");
    m_pLayer->addChild(pRightAttackIcon);
    pRightAttackIcon->setAnchorPoint(Vec2(0, 0.5f));
    pRightAttackIcon->setPosition(Vec2(pRightAttack->getPositionX() + pRightAttack->getBoundingBox().size.width + 8, pRightAttack->getPositionY()));
    
    pBuf = __String::createWithFormat("%d", tempVec[1].attack);
    Label* pRightAttackValue = Label::createWithTTF(pBuf->getCString(), "fonts/cuti.ttf", 30);
    m_pLayer->addChild(pRightAttackValue);
    pRightAttackValue->setAnchorPoint(Vec2(0, 0.5f));
    pRightAttackValue->enableOutline(Color4B(0, 0, 0, 255), 2);
    pRightAttackValue->setPosition(Vec2(pRightAttackIcon->getPositionX() + pRightAttackIcon->getBoundingBox().size.width + 8,
                                       pRightAttackIcon->getPositionY()));
    
    //进化花费魔法石
    Sprite* stone = Sprite::createWithSpriteFrameName("pvp_stoneIcon.png");
    m_pLayer->addChild(stone);
    stone->setScale(1.2f);
    stone->setPosition(Vec2(bgSize.width * 0.45f, bgSize.height * 0.23f));
    Label* pCostNum = Label::createWithTTF(":1000", "fonts/cuti.ttf", 30);
    pCostNum->setAnchorPoint(Vec2(0, 0.5f));
    pCostNum->setPosition(Vec2(stone->getPositionX() + stone->getBoundingBox().size.width * 0.5f + 5, stone->getPositionY()));
    m_pLayer->addChild(pCostNum);
    pCostNum->enableOutline(Color4B(0, 0, 0, 255), 2);
    
    //进化按钮
    Sprite* normal = Sprite::createWithSpriteFrameName("pvp_evolutionBtn.png");
    Sprite* select = Sprite::createWithSpriteFrameName("pvp_evolutionBtn.png");
    select->setColor(Color3B::GRAY);
    LYMenuItemSprite* pEvolutionItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(pvpCommonInfoLayer::btnCallBack, this));
    pEvolutionItem->setTag(BTN_EVOLUTION_TAG);
    Menu* pEvolutionMenu = Menu::create(pEvolutionItem, NULL);
    m_pLayer->addChild(pEvolutionMenu);
    pEvolutionItem->setScale(0.9f);
    pEvolutionMenu->setPosition(Vec2(bgSize.width * 0.5f, bgSize.height * 0.12f));
    
    normal = Sprite::createWithSpriteFrameName("closeBtn.png");
    select = Sprite::createWithSpriteFrameName("closeBtn.png");
    select->setColor(Color3B::GRAY);
    LYMenuItemSprite* pCloseItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(pvpCommonInfoLayer::btnCallBack, this));
    pCloseItem->setTag(BTN_EXIT_TAG);
    Menu* pMenu = Menu::create(pCloseItem, NULL);
    m_pLayer->addChild(pMenu);
    pMenu->setPosition(Vec2(bgSize.width * 0.98f, bgSize.height * 0.96f));
}

void pvpCommonInfoLayer::detailAndUpgradeUI()
{
    m_comBg = Sprite::createWithSpriteFrameName("pvp_infoBg.png");
    addChild(m_comBg);
    m_comBg->setPosition(Vec2(m_winSize.width * 0.5f, m_winSize.height * 0.45f));
    m_comBg->setScaleX(2.0f);
    m_comBg->setScaleY(1.85f);
    
    Size bgSize = m_comBg->getBoundingBox().size;
    m_pLayer = LayerColor::create(Color4B(0, 0, 0, 0), bgSize.width, bgSize.height);
    addChild(m_pLayer);
    m_pLayer->setPosition(Vec2(m_comBg->getPositionX() - bgSize.width * 0.5f, m_comBg->getPositionY() - bgSize.height * 0.5f));
    
    //标题框
    Sprite* pTitleBg = Sprite::createWithSpriteFrameName("pvp_infoTitileBg.png");
    m_pLayer->addChild(pTitleBg);
    pTitleBg->setScale(1.2f);
    pTitleBg->setPosition(Vec2(bgSize.width * 0.5f, bgSize.height * 0.98f));
    m_buildInfo = CDataManager::getBuildInfo(m_buildingId);
    char buf[256];
    
    monsterInfo* pMonsterInfo = CDataManager::getMonsterInfoByMonsterId(m_buildInfo->monsterId);
    std::string title = pMonsterInfo->monsterName;
    //下一级的建筑信息
    m_pNextBuildInfo = m_buildInfo;
    int nextBuildId = m_buildingId;
    if((m_msgType == PVP_COMMSG_UPGRADE) || (m_msgType == PVP_COMMSG_NOT_UPGRADE))
    {
        sprintf(buf, "将%s升至%d级", title.c_str(), m_buildInfo->level + 1);
        title = buf;
        nextBuildId = m_buildInfo->nextBuildingID;
        m_pNextBuildInfo = CDataManager::getBuildInfo(nextBuildId);
    }
    Label* pTitle = Label::createWithTTF(title, "fonts/cuti.ttf", 34);
    m_pLayer->addChild(pTitle);
    pTitle->enableOutline(Color4B(0, 0, 0, 255), 3);
    pTitle->setPosition(Vec2(bgSize.width * 0.5f, bgSize.height * 0.98f));
    
    pvpBuildInfo* pNextBuild = m_buildInfo;
    if(m_msgType == PVP_COMMSG_DETAIL)
    {
        sprintf(buf, "%d级", m_buildInfo->level);
        
        Label* pTowerLevel = Label::createWithSystemFont(buf, "Arial", 28);
        m_pLayer->addChild(pTowerLevel);
        pTowerLevel->setColor(Color3B::BLACK);
        pTowerLevel->setPosition(Vec2(bgSize.width * 0.5f, bgSize.height * 0.9f));
    }
    else{
        pNextBuild = CDataManager::getBuildInfo(m_buildInfo->nextBuildingID);
    }
    //阴影
    //名字根据monsterId来获取
    Sprite* towerBg = Sprite::createWithSpriteFrameName("pvp_build_di.png");
    m_pLayer->addChild(towerBg);
    towerBg->setPosition(Vec2(bgSize.width * 0.2f, bgSize.height * 0.62f));
    
    Sprite* pTowerBase = NULL;
    if(m_buildInfo->modelImgVec.size() == 2)
    {
        pTowerBase = Sprite::createWithSpriteFrameName(pNextBuild->modelImgVec[1].c_str());
        m_pLayer->addChild(pTowerBase);
        pTowerBase->setPosition(Vec2(bgSize.width * 0.2f, bgSize.height * 0.7f));
        
        Sprite* pTowerUp = Sprite::createWithSpriteFrameName(pNextBuild->modelImgVec[0].c_str());
        m_pLayer->addChild(pTowerUp);
        pTowerUp->setPosition(Vec2(bgSize.width * 0.2f, bgSize.height * 0.8f));
    }
    else if(m_buildInfo->modelImgVec.size() == 1)
    {
        pTowerBase = Sprite::createWithSpriteFrameName(pNextBuild->modelImgVec[0].c_str());
        m_pLayer->addChild(pTowerBase);
        pTowerBase->setPosition(Vec2(bgSize.width * 0.2f, bgSize.height * 0.7f));
    }
    else{
        pTowerBase = Sprite::createWithSpriteFrameName(pNextBuild->icon.c_str());
        m_pLayer->addChild(pTowerBase);
        pTowerBase->setPosition(Vec2(bgSize.width * 0.2f, bgSize.height * 0.7f));
    }
    if(m_buildInfo->buildingType == 0)
    {
        pTowerBase->setPosition(Vec2(bgSize.width * 0.2f, bgSize.height * 0.76f));;
    }
    Sprite* normal = Sprite::createWithSpriteFrameName("closeBtn.png");
    Sprite* select = Sprite::createWithSpriteFrameName("closeBtn.png");
    select->setColor(Color3B::GRAY);
    LYMenuItemSprite* pCloseItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(pvpCommonInfoLayer::btnCallBack, this));
    pCloseItem->setTag(BTN_EXIT_TAG);
    Menu* pMenu = Menu::create(pCloseItem, NULL);
    m_pLayer->addChild(pMenu);
    pMenu->setPosition(Vec2(bgSize.width * 0.98f, bgSize.height * 0.96f));
    
    showTowerDetail();
}

void pvpCommonInfoLayer::addCancelUpgrade(Ref* pTarget, SEL_MenuHandler phander)
{
    m_comBg = Sprite::createWithSpriteFrameName("pvp_infoBg.png");
    addChild(m_comBg);
    m_comBg->setPosition(Vec2(m_winSize.width * 0.5f, m_winSize.height * 0.45f));
    m_comBg->setScaleX(1.5f);
    m_comBg->setScaleY(1.4f);
    
    Size bgSize = m_comBg->getBoundingBox().size;
    m_pLayer = LayerColor::create(Color4B(0, 0, 0, 0), bgSize.width, bgSize.height);
    addChild(m_pLayer);
    m_pLayer->setPosition(Vec2(m_comBg->getPositionX() - bgSize.width * 0.5f, m_comBg->getPositionY() - bgSize.height * 0.5f));
    
    //标题框
    Sprite* pTitleBg = Sprite::createWithSpriteFrameName("pvp_infoTitileBg.png");
    m_pLayer->addChild(pTitleBg);
    pTitleBg->setScale(1.2f);
    pTitleBg->setPosition(Vec2(bgSize.width * 0.5f, bgSize.height * 0.98f));
    
    m_pTarget = pTarget;
    m_pHandler = phander;
    m_buildInfo = CDataManager::getBuildInfo(m_buildingId);
    std::string title = "取消升级?";
    Label* pTitle = Label::createWithTTF(title, "fonts/cuti.ttf", 34);
    m_pLayer->addChild(pTitle);
    pTitle->enableOutline(Color4B(0, 0, 0, 255), 3);
    pTitle->setPosition(Vec2(bgSize.width * 0.5f, bgSize.height * 0.98f));
    
    monsterInfo* pInfo = CDataManager::getMonsterInfoByMonsterId(m_buildInfo->monsterId);
    __String* tempStr = __String::createWithFormat("您确定要取消%s的升级吗？如果取消只会返还升级费用的50%%", pInfo->monsterName.c_str());
    
    Label* pContent = Label::createWithSystemFont(tempStr->getCString(), "Arial", 28);
    m_pLayer->addChild(pContent);
    pContent->setColor(Color3B::BLACK);
    pContent->setWidth(bgSize.width * 0.8f);
    pContent->setPosition(Vec2(bgSize.width * 0.5f, bgSize.height * 0.65f));
    
    Sprite* normal = Sprite::createWithSpriteFrameName("redbtn.png");
    Sprite* select = Sprite::createWithSpriteFrameName("redbtn.png");
    select->setColor(Color3B::GRAY);
    LYMenuItemSprite* cancelItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(pvpCommonInfoLayer::btnCallBack, this));
    Menu* pCancelMenu = Menu::create(cancelItem, NULL);
    cancelItem->setScale(2.6f);
    cancelItem->setTag(BTN_CANCEL_UPGRADE);
    m_pLayer->addChild(pCancelMenu);
    pCancelMenu->setPosition(Vec2(bgSize.width * 0.25f, bgSize.height * 0.2f));
    
    Label* pCancelLabel = Label::createWithTTF("取消", "fonts/cuti.ttf", 28);
    pCancelLabel->setPosition(Vec2(normal->getContentSize().width * 0.5f, normal->getContentSize().height * 0.5f));
    cancelItem->addChild(pCancelLabel, 5);
    pCancelLabel->setScale(0.4f);
    pCancelLabel->enableOutline(Color4B(0, 0, 0, 255), 2);
    
    
    normal = Sprite::createWithSpriteFrameName("greenbtn.png");
    select = Sprite::createWithSpriteFrameName("greenbtn.png");
    select->setColor(Color3B::GRAY);
    LYMenuItemSprite* sureItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(pvpCommonInfoLayer::OnSure, this));
    sureItem->setScale(2.6f);
    Menu* pSureMenu = Menu::create(sureItem, NULL);
    sureItem->setTag(BTN_OK_UPGRADE);
    m_pLayer->addChild(pSureMenu);
    pSureMenu->setPosition(Vec2(bgSize.width * 0.75f, bgSize.height * 0.2f));
    
    Label* pSureLabel = Label::createWithTTF("确定", "fonts/cuti.ttf", 28);
    pSureLabel->setPosition(Vec2(normal->getContentSize().width * 0.5f, normal->getContentSize().height * 0.5f));
    sureItem->addChild(pSureLabel, 5);
    pSureLabel->setScale(0.4f);
    pSureLabel->enableOutline(Color4B(0, 0, 0, 255), 2);
    
    normal = Sprite::createWithSpriteFrameName("closeBtn.png");
    select = Sprite::createWithSpriteFrameName("closeBtn.png");
    select->setColor(Color3B::GRAY);
    LYMenuItemSprite* pCloseItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(pvpCommonInfoLayer::btnCallBack, this));
    pCloseItem->setTag(BTN_EXIT_TAG);
    Menu* pMenu = Menu::create(pCloseItem, NULL);
    m_pLayer->addChild(pMenu);
    pMenu->setPosition(Vec2(bgSize.width * 0.98f, bgSize.height * 0.96f));
}

void pvpCommonInfoLayer::showProductInfo()
{
    m_comBg = Sprite::createWithSpriteFrameName("pvp_infoBg.png");
    addChild(m_comBg);
    m_comBg->setPosition(Vec2(m_winSize.width * 0.5f, m_winSize.height * 0.45f));
    m_comBg->setScaleX(2.0f);
    m_comBg->setScaleY(1.85f);
    
    Size bgSize = m_comBg->getBoundingBox().size;
    m_pLayer = LayerColor::create(Color4B(0, 0, 0, 0), bgSize.width, bgSize.height);
    addChild(m_pLayer);
    m_pLayer->setPosition(Vec2(m_comBg->getPositionX() - bgSize.width * 0.5f, m_comBg->getPositionY() - bgSize.height * 0.5f));
    
    //标题框
    Sprite* pTitleBg = Sprite::createWithSpriteFrameName("pvp_infoTitileBg.png");
    m_pLayer->addChild(pTitleBg);
    pTitleBg->setScale(1.2f);
    pTitleBg->setPosition(Vec2(bgSize.width * 0.5f, bgSize.height * 0.98f));
    m_buildInfo = CDataManager::getBuildInfo(m_buildingId);
    char buf[256];
    
    Label* pTitle = Label::createWithTTF("开始熔炼", "fonts/cuti.ttf", 34);
    m_pLayer->addChild(pTitle);
    pTitle->enableOutline(Color4B(0, 0, 0, 255), 3);
    pTitle->setPosition(Vec2(bgSize.width * 0.5f, bgSize.height * 0.98f));
    
    Sprite* productIcon = Sprite::createWithSpriteFrameName("pvp_proructIcon.png");
    m_pLayer->addChild(productIcon);
    productIcon->setPosition(Vec2(bgSize.width * 0.2f, bgSize.height * 0.68f));
    
    Label* pKuangshi = Label::createWithTTF("总矿石:", "fonts/cuti.ttf", 24);
    pKuangshi->setAnchorPoint(Vec2(0, 0.5f));
    m_pLayer->addChild(pKuangshi);
    pKuangshi->enableOutline(Color4B(0, 0, 0, 255), 2);
    pKuangshi->setPosition(Vec2(bgSize.width * 0.4f, bgSize.height * 0.75f));
    
    Sprite* pStone = Sprite::createWithSpriteFrameName("pvp_ore.png");
    pStone->setAnchorPoint(Vec2(0, 0.5f));
    pStone->setScale(0.6f);
    m_pLayer->addChild(pStone);
    pStone->setPosition(Vec2(pKuangshi->getPositionX() + pKuangshi->getBoundingBox().size.width + 10, pKuangshi->getPositionY()));
    
    if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_PVP_RONGLU) && (guiderManagerLayer::getInstance()->getStepNum() == 5))
    {
        guiderManagerLayer::getInstance()->setStepNum(guiderManagerLayer::getInstance()->getStepNum() + 1);
        guiderManagerLayer::getInstance()->createLayer(this);
        
    }
    
    pvpBuildInfo* pInfo = CDataManager::getBuildInfo(m_buildingId);
    
    sprintf(buf, "%d/%d", CPersonalData::getInstance()->getUserKuangStone(), CPersonalData::getInstance()->getUserMaxKuangStone());
    m_pNum = Label::createWithTTF(buf, "fonts/cuti.ttf", 24);
    m_pNum->setAnchorPoint(Vec2(0, 0.5f));
    m_pLayer->addChild(m_pNum);
    m_pNum->enableOutline(Color4B(0, 0, 0, 255), 2);
    m_pNum->setPosition(Vec2(pStone->getPositionX() + pStone->getBoundingBox().size.width + 10, pStone->getPositionY()));
    
    
    sprintf(buf, "最大熔炼个数: %d", pInfo->effectTime);
    m_pLimit = Label::createWithTTF(buf, "fonts/cuti.ttf", 24);
    m_pLimit->setAnchorPoint(Vec2(0, 0.5f));
    m_pLayer->addChild(m_pLimit);
    m_pLimit->enableOutline(Color4B(0, 0, 0, 255), 2);
    m_pLimit->setPosition(Vec2(bgSize.width * 0.4f, bgSize.height * 0.67f));
    
    m_node = Node::create();
    m_pLayer->addChild(m_node);
    m_node->setPosition(Vec2::ZERO);

    __String* pBuf = __String::createWithFormat("%d", m_userBuildingId);
    CNetControlModule::sharedNetControlModule()->isEndProduct(this, menu_selector(pvpCommonInfoLayer::productInfoBack), pBuf->getCString());
    
    Sprite* normal = Sprite::createWithSpriteFrameName("closeBtn.png");
    Sprite* select = Sprite::createWithSpriteFrameName("closeBtn.png");
    select->setColor(Color3B::GRAY);
    LYMenuItemSprite* pCloseItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(pvpCommonInfoLayer::btnCallBack, this));
    pCloseItem->setTag(BTN_EXIT_TAG);
    Menu* pMenu = Menu::create(pCloseItem, NULL);
    m_pLayer->addChild(pMenu);
    pMenu->setPosition(Vec2(bgSize.width * 0.98f, bgSize.height * 0.96f));
    
}

void pvpCommonInfoLayer::showGuideInfo()
{
    if(guiderManagerLayer::getInstance()->getGuideType() == GUIDE_PVP_RONGLU)
    {
        if(guiderManagerLayer::getInstance()->getStepNum() == 8)
        {
            guiderManagerLayer::getInstance()->setStepNum(guiderManagerLayer::getInstance()->getStepNum() + 1);
            guiderManagerLayer::getInstance()->createLayer(this);
        }
        else if(guiderManagerLayer::getInstance()->getStepNum() == 10)
        {
            guiderManagerLayer::getInstance()->createLayer(this, GUIDE_SHOW_FINGER, m_guidePos);
        }
    }
}

void pvpCommonInfoLayer::productInfoBack(Ref* pSender)
{
    _netRecvEventIsEndProduct* pInfo = (_netRecvEventIsEndProduct*)pSender;
    int state = 1;
    int remainTime = 0;
    for(std::vector<std::string>::iterator it = pInfo->m_paramsStr.begin(); it!= pInfo->m_paramsStr.end(); it++)
    {
        string str = *it;
        string name = CDataManager::getStrFromBackStr(str);
        
        if(name == "state")
        {
            state = atoi(CDataManager::getDigitFromBackStr(*it).c_str());
            if(state != 0)
            {
                log("判断熔炼是否结束");
                return;
            }
        }
        else if (name == "retTime")
        {
            remainTime = atoi(CDataManager::getDigitFromBackStr(*it).c_str());
        }
        else if(name == "retStone")
        {
            m_productNum = atoi(CDataManager::getDigitFromBackStr(*it).c_str());
        }
        else if(name == "productTime")
        {
            m_needTime = atoi(CDataManager::getDigitFromBackStr(*it).c_str());
        }
    }
    if(remainTime > 0)
    {
        m_remainTime = remainTime;
        //m_productNum *= 100;
        addProvessInfo();
        m_remainTime--;
        scheduleUpdate();
    }
    else{
        addProductProcess();
    }
}

void pvpCommonInfoLayer::addProductProcess()
{
    char buf[256];
    Size bgSize = m_comBg->getBoundingBox().size;
    
    Label* pExchange = Label::createWithTTF("每100矿石可熔炼1个魔法石", "fonts/cuti.ttf", 24);
    pExchange->setAnchorPoint(Vec2(0, 0.5f));
    m_node->addChild(pExchange);
    pExchange->enableOutline(Color4B(0, 0, 0, 255), 2);
    pExchange->setColor(Color3B::YELLOW);
    pExchange->setPosition(Vec2(bgSize.width * 0.4f, bgSize.height * 0.59f));
    
    //显示滑块
    Sprite* backgroundSprite = Sprite::createWithSpriteFrameName("bx_tiao1.png");
    Sprite* sliderBg = Sprite::createWithSpriteFrameName("bx_kuang.png");
    backgroundSprite->addChild(sliderBg, 0, 200);
    sliderBg->setPosition(Vec2(backgroundSprite->getBoundingBox().size.width * 0.5f, backgroundSprite->getBoundingBox().size.height * 0.5f));
    
    Sprite* pogressSprite = Sprite::createWithSpriteFrameName("bx_tiao2.png");
    Sprite* thumbSprite = Sprite::createWithSpriteFrameName("ladong.png");
    cocos2d::extension::ControlSlider* m_solider = cocos2d::extension::ControlSlider::create(backgroundSprite,pogressSprite,thumbSprite);
    m_solider->setPosition(Vec2(bgSize.width * 0.37f, bgSize.height * 0.24f));
    m_solider->setScale(1.1f);
    if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_PVP_RONGLU) && (guiderManagerLayer::getInstance()->getStepNum() == 6))
    {
        CPersonalData::getInstance()->setUserKuangStone(CPersonalData::getInstance()->getUserKuangStone() + 100);
    }
    int kuangNum = CPersonalData::getInstance()->getUserKuangStone()/100;
    pvpBuildInfo* pInfo = CDataManager::getBuildInfo(m_buildingId);
    m_solider->setMaximumValue(MIN(kuangNum, pInfo->effectTime/100));
    m_solider->setMinimumValue(0);
    m_solider->setValue(100);
    m_sliderValue = 0;
    m_node->addChild(m_solider, 0, 1000);
    m_solider->addTargetWithActionForControlEvents(this, cccontrol_selector(pvpCommonInfoLayer::soliderCallBack), Control::EventType::VALUE_CHANGED);
    
    m_arrow = Sprite::createWithSpriteFrameName("numPoint.png");
    m_solider->addChild(m_arrow);
    m_arrow->setPosition(Vec2(m_solider->getContentSize().width * m_solider->getValue()/m_solider->getMaximumValue() - 5, m_solider->getContentSize().height * 1.42f));
    
    sprintf(buf, "%d", int(m_solider->getValue() * 100));
    m_pSliderValue = Label::createWithSystemFont(buf, "Arial", 15);
    m_arrow->addChild(m_pSliderValue);
    m_pSliderValue->setPosition(Vec2(m_arrow->getContentSize().width / 2, m_arrow->getContentSize().height * 0.68f));
    
    m_menu = Menu::create();
    m_node->addChild(m_menu);
    m_menu->setPosition(Vec2::ZERO);
    Sprite* normal = Sprite::createWithSpriteFrameName("minus.png");
    Sprite* select = Sprite::createWithSpriteFrameName("minus.png");
    select->setColor(Color3B::GRAY);
    LYMenuItemSprite* minusItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(pvpCommonInfoLayer::btnCallBack, this));
    minusItem->setTag(PRODUCT_MINUS_TAG);
    minusItem->setEnabled(false);
    minusItem->setColor(Color3B::GRAY);
    minusItem->setPosition(Vec2(bgSize.width * 0.075f, bgSize.height * 0.24f));
    m_menu->addChild(minusItem, 0, PRODUCT_MINUS_TAG);
    
    normal = Sprite::createWithSpriteFrameName("plus.png");
    select = Sprite::createWithSpriteFrameName("plus.png");
    select->setColor(Color3B::GRAY);
    LYMenuItemSprite* plusItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(pvpCommonInfoLayer::btnCallBack, this));
    plusItem->setTag(PRODUCT_ADD_TAG);
    plusItem->setPosition(Vec2(bgSize.width * 0.665f, bgSize.height * 0.24f));
    m_menu->addChild(plusItem, 0, PRODUCT_ADD_TAG);
    if(kuangNum < 1)
    {
        plusItem->setEnabled(false);
        plusItem->setColor(Color3B::GRAY);
    }
    
    Sprite* productBg = Sprite::createWithSpriteFrameName("pvp_product_bg.png");
    m_node->addChild(productBg);
    productBg->setScale(2.0f);
    productBg->setPosition(bgSize.width * 0.37f, bgSize.height * 0.13f);
    
    Label* pCostLabel = Label::createWithTTF("消耗: ","fonts/cuti.ttf", 28);
    m_node->addChild(pCostLabel);
    pCostLabel->enableOutline(Color4B(0, 0, 0, 255), 2);
    pCostLabel->setAnchorPoint(Vec2(0, 0.5f));
    pCostLabel->setPosition(Vec2(bgSize.width * 0.085f, bgSize.height * 0.13f));
    
    sprintf(buf, "%d", int(m_solider->getValue() * 100));
    m_pCost = Label::createWithTTF(buf,"fonts/cuti.ttf", 28);
    m_node->addChild(m_pCost);
    m_pCost->enableOutline(Color4B(0, 0, 0, 255), 2);
    m_pCost->setAnchorPoint(Vec2(0, 0.5f));
    m_pCost->setPosition(Vec2(pCostLabel->getPositionX() + pCostLabel->getBoundingBox().size.width, bgSize.height * 0.13f));
    
    m_pKuangshi = Sprite::createWithSpriteFrameName("pvp_ore.png");
    m_node->addChild(m_pKuangshi);
    m_pKuangshi->setScale(0.6);
    m_pKuangshi->setAnchorPoint(Vec2(0, 0.5f));
    m_pKuangshi->setPosition(Vec2(m_pCost->getPositionX() + m_pCost->getBoundingBox().size.width + 10, m_pCost->getPositionY()));
    
    m_pStone = Sprite::createWithSpriteFrameName("pvp_stoneIcon.png");
    m_node->addChild(m_pStone);
    m_pStone->setAnchorPoint(Vec2(1, 0.5f));
    m_pStone->setPosition(Vec2(bgSize.width * 0.665f, bgSize.height * 0.13f));
    
    sprintf(buf, "可获得: %d", m_sliderValue/100);
    m_pGet = Label::createWithTTF(buf,"fonts/cuti.ttf", 28);
    m_node->addChild(m_pGet);
    m_pGet->enableOutline(Color4B(0, 0, 0, 255), 2);
    m_pGet->setAnchorPoint(Vec2(1, 0.5f));
    m_pGet->setPosition(Vec2(m_pStone->getPositionX() - m_pStone->getBoundingBox().size.width - 10, bgSize.height * 0.13f));
    
    m_pClock = Sprite::createWithSpriteFrameName("TD_Clock.png");
    m_node->addChild(m_pClock);
    m_pClock->setRotation(-15);
    
    
    int current_value = (int)m_solider->getValue() * 100;
    m_remainTime = (3600 * 4 * current_value)/pInfo->effectTime;
    
    std::string time;
    int second = m_remainTime%60;
    int minute = m_remainTime/60;
    int hour = 0;
    if(m_remainTime < 60)
    {
        sprintf(buf, "%d秒", m_remainTime);
        time += buf;
    }
    else
    {
        if(minute > 60)
        {
            hour = minute/60;
            minute = minute%60;
            if(hour > 0)
            {
                sprintf(buf, "%d小时", hour);
                time += buf;
            }
        }
        if(minute > 0)
        {
            sprintf(buf, "%d分", minute);
            time += buf;
        }
        if(second > 0)
        {
            sprintf(buf, "%d秒", second);
            time += buf;
        }
    }
    
    m_pNeedTime = Label::createWithTTF(time.c_str(), "fonts/cuti.ttf", 26);
    m_node->addChild(m_pNeedTime);
    m_pNeedTime->enableOutline(Color4B(0, 0, 0, 255), 3);
    
    if(((hour > 0) && (minute > 0)) || ((minute > 0) && (second > 0)))
    {
        m_pNeedTime->setAnchorPoint(Vec2(1, 0.5f));
        m_pNeedTime->setPosition(Vec2(bgSize.width * 0.96f, bgSize.height * 0.34));
        m_pClock->setAnchorPoint(Vec2(1, 0.5f));
        m_pClock->setPosition(Vec2(m_pNeedTime->getPositionX() - m_pNeedTime->getBoundingBox().size.width - 10, bgSize.height * 0.355));
    }
    else{
        m_pClock->setAnchorPoint(Vec2(0, 0.5f));
        m_pClock->setPosition(Vec2(bgSize.width * 0.75f, bgSize.height * 0.33));
        m_pNeedTime->setAnchorPoint(Vec2(0, 0.5f));
        m_pNeedTime->setPosition(Vec2(m_pClock->getPositionX() + m_pClock->getBoundingBox().size.width, bgSize.height * 0.34));
        
    }
    normal= Sprite::createWithSpriteFrameName("fight_greenBtn.png");
    select= Sprite::createWithSpriteFrameName("fight_greenBtn.png");
    select->setColor(Color3B::GRAY);
    LYMenuItemSprite* productItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(pvpCommonInfoLayer::btnCallBack, this));
    m_productMenu = Menu::create(productItem, NULL);
    m_node->addChild(m_productMenu);
    productItem->setTag(PRODUCT_START_TAG);
    m_productMenu->setPosition(Vec2(bgSize.width * 0.85f, bgSize.height * 0.22));
    Label* pRoduct = Label::createWithTTF("开始熔炼", "fonts/cuti.ttf", 28);
    productItem->addChild(pRoduct);
    pRoduct->enableOutline(Color4B(0, 0, 0, 255), 3);
    pRoduct->setPosition(Vec2(normal->getBoundingBox().size.width * 0.5f, normal->getBoundingBox().size.height * 0.51f));
    if(m_solider->getValue() > kuangNum)
    {
        productItem->setColor(Color3B::GRAY);
        productItem->setEnabled(false);
        m_pCost->setColor(Color3B::RED);
    }
    if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_PVP_RONGLU) && (guiderManagerLayer::getInstance()->getStepNum() == 6))
    {
        sprintf(buf, "%d/%d", CPersonalData::getInstance()->getUserKuangStone(), CPersonalData::getInstance()->getUserMaxKuangStone());
        m_pNum->setString(buf);
        m_solider->setValue(kuangNum);
        m_guidePos = productItem->convertToWorldSpaceAR(Vec2::ZERO);
        //CPersonalData::getInstance()->setUpdateUserInfo(true);
        CPersonalData::getInstance()->setUpdateKuanshiInfo(true);
    }
}

std::string pvpCommonInfoLayer::calculateTime(int allTime)
{
    char buf[256];
    int hour = 0;
    int minute = 0;
    int second = 0;
    if(allTime < 60)
    {
        second = allTime;
    }
    else
    {
        second = allTime%60;
        minute = allTime/60;
        if(minute > 60)
        {
            hour = minute/60;
            minute = minute%60;
        }
    }
    sprintf(buf, "%02d:%02d:%02d", hour, minute, second);
    return buf;
}

std::string pvpCommonInfoLayer::calculateNeedTime(int allTime)
{
    char buf[256];
    std::string time;
    if(allTime < 60)
    {
        sprintf(buf, "%d秒", allTime);
        time += buf;
    }
    else
    {
        int second = allTime%60;
        int minute = allTime/60;
        int hour = 0;
        if(minute > 60)
        {
            hour = minute/60;
            minute = minute%60;
            if(hour > 0)
            {
                sprintf(buf, "%d时", hour);
                time += buf;
            }
        }
        if(minute > 0)
        {
            sprintf(buf, "%d分", minute);
            time += buf;
        }
        if(second > 0)
        {
            sprintf(buf, "%d秒", second);
            time += buf;
        }
    }
    return time;
}


void pvpCommonInfoLayer::update(float dt)
{
    m_time += dt;
    
    if(m_time >= 1)
    {
        m_time -= 1;
        if(m_remainTime > 0)
        {
            float percent = (float)(m_needTime - m_remainTime)/m_needTime*100;
            m_progress->setPercentage(percent);
            std::string temp = calculateTime(m_remainTime);
            m_pTime->setString(temp.c_str());
            m_remainTime -= 1;
        }
        else{
            if(!m_flag)
            {
                m_flag=  true;
                //发送消息验证是否真的结束
                __String* buf = __String::createWithFormat("%d", m_userBuildingId);
                CNetControlModule::sharedNetControlModule()->isEndProduct(this, menu_selector(pvpCommonInfoLayer::isEndProductBack), buf->getCString());
            }
        }
    }
}

void pvpCommonInfoLayer::isEndProductBack(Ref *pSender)
{
    _netRecvEventIsEndProduct* pInfo = (_netRecvEventIsEndProduct*)pSender;
    int state = 1;
    int remainTime = 0;
    int stone = 0;
    for(std::vector<std::string>::iterator it = pInfo->m_paramsStr.begin(); it!= pInfo->m_paramsStr.end(); it++)
    {
        string str = *it;
        string name = CDataManager::getStrFromBackStr(str);
        
        if(name == "state")
        {
            state = atoi(CDataManager::getDigitFromBackStr(*it).c_str());
            if(state != 0)
            {
                log("判断熔炼是否结束");
                return;
            }
        }
        else if (name == "remainTime")
        {
            remainTime = atoi(CDataManager::getDigitFromBackStr(*it).c_str());
        }
        else if(name == "retStone")
        {
            stone = atoi(CDataManager::getDigitFromBackStr(*it).c_str());
        }
        else if(name == "productTime")
        {
            m_needTime = atoi(CDataManager::getDigitFromBackStr(*it).c_str());
        }
    }
    if(remainTime > 0)
    {
        m_remainTime = remainTime;
        m_flag = false;
    }
    else{
        m_pTime->removeFromParent();
        m_pTime = NULL;
        CPersonalData::getInstance()->setUserStone(CPersonalData::getInstance()->getUserStone() + stone);
        CPersonalData::getInstance()->setUpdateUserInfo(true);
        ((pvpMainLayer*)getParent())->saveRongluState(m_userBuildingId, true);
        ((pvpMainLayer*)getParent())->rmvTowerInfoLayer();
    }
}

void pvpCommonInfoLayer::addProvessInfo()
{
    if(m_node)
    {
        m_node->removeFromParent();
        m_node = NULL;
    }
    
    Size bgSize = m_pLayer->getBoundingBox().size;
    //显示动画
    Sprite* pRong = Sprite::createWithSpriteFrameName("ronglian_1.png");
    m_pLayer->addChild(pRong);
    pRong->setScale(1.5f);
    pRong->setPosition(Vec2(bgSize.width * 0.2f, bgSize.height * 0.7f));
    Animation* pAction = AnimationCache::getInstance()->getAnimation("RongLian");
    pRong->runAction(RepeatForever::create(Animate::create(pAction)));
    
    
    Sprite* backgroundSprite = Sprite::createWithSpriteFrameName("bx_tiao1.png");
    m_pLayer->addChild(backgroundSprite);
    backgroundSprite->setScale(1.1f);
    backgroundSprite->setPosition(Vec2(bgSize.width * 0.5f, bgSize.height * 0.25f));
    
    Sprite* sp = Sprite::createWithSpriteFrameName("bx_tiao3.png");
    m_progress = ProgressTimer::create(sp);
    m_pLayer->addChild(m_progress);
    m_progress->setScale(1.1f);
    m_progress->setType(ProgressTimer::Type::BAR);
    m_progress->setPosition(Vec2(bgSize.width * 0.5f, bgSize.height * 0.25f));
    m_progress->setMidpoint(Vec2(0,0.5));
    m_progress->setBarChangeRate(Vec2(1, 0));
    float percent = (float)(m_needTime - m_remainTime)/m_needTime*100;
    m_progress->setPercentage(percent);
    
    Sprite* sliderBg = Sprite::createWithSpriteFrameName("bx_kuang.png");
    m_pLayer->addChild(sliderBg);
    sliderBg->setScale(1.1f);
    sliderBg->setPosition(Vec2(bgSize.width * 0.5f, bgSize.height * 0.25f));
    
    //时间
    std::string temp = calculateTime(m_remainTime);
    m_pTime = Label::createWithTTF(temp.c_str(), "fonts/cuti.ttf", 28);
    m_pLayer->addChild(m_pTime);
    m_pTime->setPosition(sliderBg->getPosition());
    m_pTime->enableOutline(Color4B(0, 0, 0, 255), 2);
    
    __String* buf = __String::createWithFormat("熔炼完成可获得%d个", m_productNum);
    Label* pLabel = Label::createWithTTF(buf->getCString(), "fonts/cuti.ttf", 30);
    m_pLayer->addChild(pLabel);
    pLabel->setPosition(Vec2(bgSize.width * 0.5f, bgSize.height * 0.15f));
    pLabel->enableOutline(Color4B(0, 0, 0, 255), 2);
    
    Sprite* pStone = Sprite::createWithSpriteFrameName("pvp_stoneIcon.png");
    m_pLayer->addChild(pStone);
    pStone->setAnchorPoint(Vec2(0, 0.5f));
    pStone->setPosition(Vec2(pLabel->getPositionX() + pLabel->getBoundingBox().size.width * 0.5f + 5, pLabel->getPositionY()));
}

void pvpCommonInfoLayer::checkBtnEnabled(ControlSlider* slider)
{
    if(int(slider->getValue()) == int(slider->getMaximumValue()))
    {
        ((LYMenuItemSprite*)(m_menu->getChildByTag(PRODUCT_ADD_TAG)))->setEnabled(false);
        ((LYMenuItemSprite*)(m_menu->getChildByTag(PRODUCT_ADD_TAG)))->setColor(Color3B::GRAY);
    }
    else if (int(slider->getValue()) == int(slider->getMinimumValue()))
    {
        ((LYMenuItemSprite*)(m_menu->getChildByTag(PRODUCT_MINUS_TAG)))->setEnabled(false);
        ((LYMenuItemSprite*)(m_menu->getChildByTag(PRODUCT_MINUS_TAG)))->setColor(Color3B::GRAY);
    }
    else
    {
        ((LYMenuItemSprite*)(m_menu->getChildByTag(PRODUCT_MINUS_TAG)))->setEnabled(true);
        ((LYMenuItemSprite*)(m_menu->getChildByTag(PRODUCT_MINUS_TAG)))->setColor(Color3B::WHITE);
        ((LYMenuItemSprite*)(m_menu->getChildByTag(PRODUCT_ADD_TAG)))->setEnabled(true);
        ((LYMenuItemSprite*)(m_menu->getChildByTag(PRODUCT_ADD_TAG)))->setColor(Color3B::WHITE);
        
    }
    
}

void pvpCommonInfoLayer::soliderCallBack(Ref * sender, Control::EventType controlEvent)
{
    char buf[128];
    
    auto slide_control = (ControlSlider*)sender;//通过回调参数sender 获得ControlSlider
    int current_value = (int)slide_control->getValue() * 100;//获取slide当前的值
    if(current_value != m_sliderValue)
    {
        SimpleAudioEngine::getInstance()->playEffect("music/slider.mp3");
        CCLOG("当前值 = %f", slide_control->getValue());
    }
    sprintf(buf, "%d", current_value);
    m_sliderValue = current_value;
    m_arrow->setPosition(Vec2(slide_control->getContentSize().width * slide_control->getValue()/slide_control->getMaximumValue() - 5, m_arrow->getPositionY()));
    m_pSliderValue->setString(buf);
    m_pSliderValue->updateContent();
    m_pCost->setString(buf);
    int kuangNum = CPersonalData::getInstance()->getUserKuangStone()/100;
    kuangNum *= 100;
    if((current_value > kuangNum) && (current_value > 0))
    {
        log("目前值%d", current_value);
        m_pCost->setColor(Color3B::RED);
    }
    else
    {
        m_pCost->setColor(Color3B::WHITE);
    }
    m_pKuangshi->setPositionX(m_pCost->getPositionX() + m_pCost->getBoundingBox().size.width + 10);
    sprintf(buf, "可获得: %d", current_value / 100);
    m_pGet->setString(buf);
    m_pGet->setPositionX(m_pStone->getPositionX() - m_pStone->getBoundingBox().size.width - 10);
    
    pvpBuildInfo* pInfo = CDataManager::getBuildInfo(m_buildingId);
    m_remainTime = (3600 * 4 * current_value)/pInfo->effectTime;
    //std::string temp = calculateNeedTime(m_remainTime);
    
    std::string time;
    int second = m_remainTime%60;
    int minute = m_remainTime/60;
    int hour = 0;
    if(m_remainTime < 60)
    {
        sprintf(buf, "%d秒", m_remainTime);
        time += buf;
    }
    else
    {
        if(minute > 60)
        {
            hour = minute/60;
            minute = minute%60;
            if(hour > 0)
            {
                sprintf(buf, "%d小时", hour);
                time += buf;
            }
        }
        if(minute > 0)
        {
            sprintf(buf, "%d分", minute);
            time += buf;
        }
        if(second > 0)
        {
            sprintf(buf, "%d秒", second);
            time += buf;
        }
    }
    Size bgSize = m_comBg->getBoundingBox().size;
    m_pNeedTime->removeFromParent();
    m_pClock->removeFromParent();;
    
    m_pClock = Sprite::createWithSpriteFrameName("TD_Clock.png");
    m_node->addChild(m_pClock);
    m_pClock->setRotation(-15);
    m_pClock->setPosition(Vec2(bgSize.width * 0.77f, bgSize.height * 0.33));
    
    m_pNeedTime = Label::createWithTTF(time.c_str(), "fonts/cuti.ttf", 26);
    m_node->addChild(m_pNeedTime);
    m_pNeedTime->enableOutline(Color4B(0, 0, 0, 255), 3);
    
    if(((hour > 0) && (minute > 0)) || ((minute > 0) && (second > 0)))
    {
        m_pNeedTime->setAnchorPoint(Vec2(1, 0.5f));
        m_pNeedTime->setPosition(Vec2(bgSize.width * 0.96f, bgSize.height * 0.34));
        m_pClock->setAnchorPoint(Vec2(1, 0.5f));
        m_pClock->setPosition(Vec2(m_pNeedTime->getPositionX() - m_pNeedTime->getBoundingBox().size.width - 10, bgSize.height * 0.355));
    }
    else{
        m_pClock->setAnchorPoint(Vec2(0, 0.5f));
        m_pClock->setPosition(Vec2(bgSize.width * 0.75f, bgSize.height * 0.33));
        m_pNeedTime->setAnchorPoint(Vec2(0, 0.5f));
        m_pNeedTime->setPosition(Vec2(m_pClock->getPositionX() + m_pClock->getBoundingBox().size.width, bgSize.height * 0.34));
        
    }
    checkBtnEnabled(slide_control);
}

void pvpCommonInfoLayer::addCostStoneTip(Ref* pTarget, SEL_MenuHandler phander, std::string content, int stone)
{
    m_pTarget = pTarget;
    m_pHandler = phander;
    m_comBg = Sprite::createWithSpriteFrameName("pvp_infoBg.png");
    addChild(m_comBg);
    m_comBg->setPosition(Vec2(m_winSize.width * 0.5f, m_winSize.height * 0.45f));
    m_comBg->setScaleX(1.5f);
    m_comBg->setScaleY(1.4f);
    
    Size bgSize = m_comBg->getBoundingBox().size;
    m_pLayer = LayerColor::create(Color4B(0, 0, 0, 0), bgSize.width, bgSize.height);
    addChild(m_pLayer);
    m_pLayer->setPosition(Vec2(m_comBg->getPositionX() - bgSize.width * 0.5f, m_comBg->getPositionY() - bgSize.height * 0.5f));
    
    //标题框
    Sprite* pTitleBg = Sprite::createWithSpriteFrameName("pvp_infoTitileBg.png");
    m_pLayer->addChild(pTitleBg);
    pTitleBg->setScale(1.5f);
    
    pTitleBg->setPosition(Vec2(bgSize.width * 0.5f, bgSize.height * 0.98f));
    
    std::string title = "花费魔法石";
    Label* pTitle = Label::createWithTTF(title, "fonts/cuti.ttf", 34);
    m_pLayer->addChild(pTitle);
    pTitle->enableOutline(Color4B(0, 0, 0, 255), 3);
    pTitle->setPosition(Vec2(bgSize.width * 0.5f, bgSize.height * 0.98f));
    
    Label* pContent = Label::createWithSystemFont(content.c_str(), "Arial", 28);
    m_pLayer->addChild(pContent);
    pContent->setColor(Color3B::BLACK);
    pContent->setWidth(bgSize.width * 0.8f);
    pContent->setPosition(Vec2(bgSize.width * 0.5f, bgSize.height * 0.65f));
    
    Sprite* pStone = Sprite::createWithSpriteFrameName("stone.png");
    m_pLayer->addChild(pStone);
    pStone->setScale(0.8f);
    pStone->setPosition(Vec2(bgSize.width * 0.45f, bgSize.height * 0.41f));
    __String* tempStr = __String::createWithFormat("%d", stone);
    Label* pStoneNum = Label::createWithTTF(tempStr->getCString(), "fonts/cuti.ttf", 30);
    m_pLayer->addChild(pStoneNum);
    pStoneNum->enableOutline(Color4B(0, 0, 0, 255), 2);
    pStoneNum->setAnchorPoint(Vec2(0, 0.5f));
    pStoneNum->setPosition(Vec2(pStone->getPositionX() + pStone->getBoundingBox().size.width * 0.5f + 10, pStone->getPositionY() + 5));
    
    //确定
    Sprite* normal = Sprite::createWithSpriteFrameName("greenbtn.png");
    Sprite* select = Sprite::createWithSpriteFrameName("greenbtn.png");
    select->setColor(Color3B::GRAY);
    LYMenuItemSprite* sureItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(pvpCommonInfoLayer::OnSure, this));
    sureItem->setScale(2.6f);
    Menu* pSureMenu = Menu::create(sureItem, NULL);
    sureItem->setTag(BTN_OK_UPGRADE);
    m_pLayer->addChild(pSureMenu);
    pSureMenu->setPosition(Vec2(bgSize.width * 0.5f, bgSize.height * 0.18f));
    
    Label* pSureLabel = Label::createWithTTF("确定", "fonts/cuti.ttf", 28);
    pSureLabel->setPosition(Vec2(normal->getContentSize().width * 0.5f, normal->getContentSize().height * 0.5f));
    sureItem->addChild(pSureLabel, 5);
    pSureLabel->setScale(0.4f);
    pSureLabel->enableOutline(Color4B(0, 0, 0, 255), 2);
    
    normal = Sprite::createWithSpriteFrameName("closeBtn.png");
    select = Sprite::createWithSpriteFrameName("closeBtn.png");
    select->setColor(Color3B::GRAY);
    LYMenuItemSprite* pCloseItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(pvpCommonInfoLayer::btnCallBack, this));
    pCloseItem->setTag(BTN_EXIT_TAG);
    Menu* pMenu = Menu::create(pCloseItem, NULL);
    m_pLayer->addChild(pMenu);
    pMenu->setPosition(Vec2(bgSize.width * 0.98f, bgSize.height * 0.96f));
    
}

void pvpCommonInfoLayer::showTowerDetail()
{
    Size bgSize = m_comBg->getBoundingBox().size;
    
    //生命值
    Label* pLifeLabel = Label::createWithTTF("生命值:", "fonts/cuti.ttf", 26);
    m_pLayer->addChild(pLifeLabel);
    pLifeLabel->setAnchorPoint(Vec2(1, 0.5f));
    pLifeLabel->enableOutline(Color4B(0, 0, 0, 255), 2);
    pLifeLabel->setPosition(Vec2(bgSize.width * 0.66f, bgSize.height * 0.74f));
    
    Sprite* pBlood = Sprite::createWithSpriteFrameName("pvp_blood.png");
    pBlood->setAnchorPoint(Vec2(0, 0.5f));
    m_pLayer->addChild(pBlood);
    pBlood->setPosition(Vec2(pLifeLabel->getPositionX() + 10, pLifeLabel->getPositionY()));
    
    char buf[256];
    sprintf(buf, "%d", m_buildInfo->life);
    Label* pLifeValue = Label::createWithTTF(buf, "fonts/cuti.ttf", 26);
    m_pLayer->addChild(pLifeValue);
    pLifeValue->setAnchorPoint(Vec2(0, 0.5f));
    pLifeValue->enableOutline(Color4B(0, 0, 0, 255), 2);
    pLifeValue->setPosition(Vec2(pBlood->getPositionX() + pBlood->getBoundingBox().size.width + 10, pLifeLabel->getPositionY()));
    
    sprintf(buf, "+%d", m_pNextBuildInfo->life - m_buildInfo->life);//如何获取升级后的血量
    Label* addLife = Label::createWithTTF(buf, "fonts/cuti.ttf", 26);
    m_pLayer->addChild(addLife);
    addLife->setVisible(false);
    addLife->setColor(Color3B::GREEN);
    addLife->setAnchorPoint(Vec2(0, 0.5f));
    addLife->enableOutline(Color4B(0, 0, 0, 255), 2);
    addLife->setPosition(Vec2(pLifeValue->getPositionX() + pLifeValue->getBoundingBox().size.width + 10, pLifeLabel->getPositionY()));
    //伤害
    Label* pAttackLabel = Label::createWithTTF("伤害:", "fonts/cuti.ttf", 26);
    m_pLayer->addChild(pAttackLabel);
    pAttackLabel->setAnchorPoint(Vec2(1, 0.5f));
    pAttackLabel->enableOutline(Color4B(0, 0, 0, 255), 2);
    pAttackLabel->setPosition(Vec2(bgSize.width * 0.66f, bgSize.height * 0.66f));
    
    Sprite* pAttack = Sprite::createWithSpriteFrameName("pvp_buildAttack.png");
    pAttack->setAnchorPoint(Vec2(0, 0.5f));
    m_pLayer->addChild(pAttack);
    pAttack->setPosition(Vec2(pAttackLabel->getPositionX() + 10, pAttackLabel->getPositionY()));
    
    sprintf(buf, "%d", m_buildInfo->attack);
    Label* pAttackValue = Label::createWithTTF(buf, "fonts/cuti.ttf", 26);//m_buildInfo->life;
    m_pLayer->addChild(pAttackValue);
    pAttackValue->setAnchorPoint(Vec2(0, 0.5f));
    pAttackValue->enableOutline(Color4B(0, 0, 0, 255), 2);
    pAttackValue->setPosition(Vec2(pAttack->getPositionX() + pAttack->getBoundingBox().size.width + 10, pAttackLabel->getPositionY()));
    
    sprintf(buf, "+%d", m_pNextBuildInfo->attack - m_buildInfo->attack);//如何获取升级后的血量
    Label* addHurt = Label::createWithTTF(buf, "fonts/cuti.ttf", 26);
    m_pLayer->addChild(addHurt);
    addHurt->setVisible(false);
    addHurt->setColor(Color3B::GREEN);
    addHurt->setAnchorPoint(Vec2(0, 0.5f));
    addHurt->enableOutline(Color4B(0, 0, 0, 255), 2);
    addHurt->setPosition(Vec2(pAttackValue->getPositionX() + pAttackValue->getBoundingBox().size.width + 10, pAttackLabel->getPositionY()));
    
    monsterInfo* pMonsterInfo = CDataManager::getMonsterInfoByMonsterId(m_buildInfo->monsterId);
    if((m_msgType == PVP_COMMSG_UPGRADE) || (m_msgType == PVP_COMMSG_NOT_UPGRADE))
    {
        addLife->setVisible(true);
        addHurt->setVisible(true);
        showUpgradeBtn();
    }
    else if(m_msgType == PVP_COMMSG_DETAIL)
    {
        //描述
        Label* pDesc = Label::createWithTTF(pMonsterInfo->monsterIntroduce.c_str(), "fonts/cuti.ttf", 26);
        m_pLayer->addChild(pDesc);
        pDesc->setColor(Color3B::BLACK);
        pDesc->setAnchorPoint(Vec2(0.5, 1));
        pDesc->setWidth(bgSize.width * 0.9f);
        pDesc->setPosition(Vec2(bgSize.width * 0.5f, bgSize.height * 0.3f));
    }
    if(m_buildInfo->life == 1)
    {
        pLifeLabel->setVisible(false);
        pBlood->setVisible(false);
        pLifeValue->setVisible(false);
        addLife->setVisible(false);
    }
    if(m_buildInfo->attack==0)
    {
        if(m_buildInfo->buildingType == 1)//矿石库
        {
            pAttackLabel->setString("矿石容量:");
            SpriteFrame* frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("pvp_ore.png");
            pAttack->setDisplayFrame(frame);
            pAttack->setScale(0.6f);
            pAttack->setPositionX(pAttackLabel->getPositionX() + 10);
            sprintf(buf, "%d", m_buildInfo->effectTime);
            pAttackValue->setString(buf);
            pAttackValue->setPositionX(pAttack->getPositionX() + pAttack->getBoundingBox().size.width + 10);;
            addHurt->setPositionX(pAttackValue->getPositionX() + pAttackValue->getBoundingBox().size.width + 10);
            
            sprintf(buf, "+%d", m_pNextBuildInfo->effectTime - m_buildInfo->effectTime);//如何获取升级后的血量
            addHurt->setString(buf);
            addHurt->setPositionX(pAttackValue->getPositionX() + pAttackValue->getBoundingBox().size.width + 10);
            
            Label* iHaveNum = Label::createWithTTF("当前库存:", "fonts/cuti.ttf", 26);
            m_pLayer->addChild(iHaveNum);
            iHaveNum->setAnchorPoint(Vec2(1, 0.5f));
            iHaveNum->enableOutline(Color4B(0, 0, 0, 255), 2);
            iHaveNum->setPosition(Vec2(bgSize.width * 0.66f, bgSize.height * 0.58f));
            
            Sprite* pStone = Sprite::createWithSpriteFrameName("pvp_ore.png");
            pStone->setAnchorPoint(Vec2(0, 0.5f));
            pStone->setScale(0.6f);
            m_pLayer->addChild(pStone);
            pStone->setPosition(Vec2(iHaveNum->getPositionX() + 10, iHaveNum->getPositionY()));
            
            sprintf(buf, "%d", CPersonalData::getInstance()->getUserKuangStone());
            Label* pSoneNum = Label::createWithTTF(buf, "fonts/cuti.ttf", 26);//m_buildInfo->life;
            m_pLayer->addChild(pSoneNum);
            pSoneNum->setAnchorPoint(Vec2(0, 0.5f));
            pSoneNum->enableOutline(Color4B(0, 0, 0, 255), 2);
            pSoneNum->setPosition(Vec2(pStone->getPositionX() + pStone->getBoundingBox().size.width + 10, pStone->getPositionY()));
        }
        else if(m_buildInfo->buildingType == 2)//熔炉
        {
            pAttackLabel->setString("熔炼量:");
            SpriteFrame* frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("pvp_ore.png");
            pAttack->setDisplayFrame(frame);
            pAttack->setScale(0.6f);
            pAttack->setPositionX(pAttackLabel->getPositionX() + 10);
            sprintf(buf, "%d", m_buildInfo->effectTime);
            pAttackValue->setString(buf);
            pAttackValue->setPositionX(pAttack->getPositionX() + pAttack->getBoundingBox().size.width + 10);;
            addHurt->setPositionX(pAttackValue->getPositionX() + pAttackValue->getBoundingBox().size.width + 10);
            
            sprintf(buf, "+%d", m_pNextBuildInfo->effectTime - m_buildInfo->effectTime);//如何获取升级后的血量
            addHurt->setString(buf);
            addHurt->setPositionX(pAttackValue->getPositionX() + pAttackValue->getBoundingBox().size.width + 10);
        }
        else if(m_buildInfo->buildingType == 10)//矿场
        {
            pAttackLabel->setString("矿石/每小时:");
            SpriteFrame* frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("pvp_ore.png");
            pAttack->setDisplayFrame(frame);
            pAttack->setScale(0.6f);
            pAttack->setPositionX(pAttackLabel->getPositionX() + 10);
            sprintf(buf, "%d", m_buildInfo->effectTime);
            pAttackValue->setString(buf);
            pAttackValue->setPositionX(pAttack->getPositionX() + pAttack->getBoundingBox().size.width + 10);;
            addHurt->setPositionX(pAttackValue->getPositionX() + pAttackValue->getBoundingBox().size.width + 10);
            
            sprintf(buf, "+%d", m_pNextBuildInfo->effectTime - m_buildInfo->effectTime);//如何获取升级后的血量
            addHurt->setString(buf);
            addHurt->setPositionX(pAttackValue->getPositionX() + pAttackValue->getBoundingBox().size.width + 10);
        }
        else
        {
            pAttackLabel->setVisible(false);
            pAttack->setVisible(false);
            pAttackValue->setVisible(false);
            addHurt->setVisible(false);
        }
    }
    if(m_buildInfo->buildingType == 11)//如果是祭坛
    {
        if((m_msgType == PVP_COMMSG_NOT_UPGRADE) || (m_msgType == PVP_COMMSG_UPGRADE) || (m_msgType == PVP_COMMSG_DETAIL))
        {
            pLifeLabel->setPosition(Vec2(pLifeLabel->getPositionX() - 30, pLifeLabel->getPositionY() + 50));
            pBlood->setPosition(Vec2(pBlood->getPositionX() - 30, pBlood->getPositionY() + 50));
            pLifeValue->setPosition(Vec2(pLifeValue->getPositionX() - 30, pLifeValue->getPositionY() + 50));
            addLife->setPosition(Vec2(addLife->getPositionX() - 30, addLife->getPositionY() + 50));
        }
        addJiTanPro();
    }
}

void pvpCommonInfoLayer::addJiTanPro()
{
    Size bgSize = m_comBg->getBoundingBox().size;
    float xPos = 30;
    
    Label* defenseLabel= Label::createWithTTF("守军:", "fonts/cuti.ttf", 26);
    m_pLayer->addChild(defenseLabel);
    defenseLabel->setAnchorPoint(Vec2(1, 0.5f));
    defenseLabel->enableOutline(Color4B(0, 0, 0, 255), 2);
    defenseLabel->setPosition(Vec2(bgSize.width * 0.66f - xPos, bgSize.height * 0.72f));
    
    partolInfo tempInfo;
    if(pvpCommonData::getInstance()->getEvolution() == 0)
    {
        tempInfo = m_buildInfo->allEvolutionVec[0];
    }
    else{
        tempInfo = m_buildInfo->allEvolutionVec[1];
    }
    __String* pBuf = __String::createWithFormat("%s.png", tempInfo.monsterIcon.c_str());
    
    Sprite* pCurXunLuo = Sprite::createWithSpriteFrameName(pBuf->getCString());
    m_pLayer->addChild(pCurXunLuo);
    pCurXunLuo->setAnchorPoint(Vec2(0, 0.5f));
    pCurXunLuo->setPosition(Vec2(defenseLabel->getPositionX() + 15, bgSize.height * 0.72f));
    
    Sprite* pCurAttack = Sprite::createWithSpriteFrameName("pvp_buildAttack.png");
    m_pLayer->addChild(pCurAttack);
    pCurAttack->setAnchorPoint(Vec2(1, 0.5f));
    pCurAttack->setPosition(Vec2(bgSize.width * 0.68f, bgSize.height * 0.61f));
    
    pBuf = __String::createWithFormat("%d", tempInfo.attack);
    Label* pCurAttackLabel = Label::createWithTTF(pBuf->getCString(), "fonts/cuti.ttf", 26);
    m_pLayer->addChild(pCurAttackLabel);
    pCurAttackLabel->enableOutline(Color4B(0, 0, 0, 255), 2);
    pCurAttackLabel->setAnchorPoint(Vec2(0, 0.5f));
    pCurAttackLabel->setPosition(Vec2(pCurAttack->getPositionX() + 8, bgSize.height * 0.61f));

    Sprite* pCurBlood = Sprite::createWithSpriteFrameName("pvp_blood.png");
    m_pLayer->addChild(pCurBlood);
    pCurBlood->setAnchorPoint(Vec2(1, 0.5f));
    pCurBlood->setPosition(Vec2(bgSize.width * 0.68f, bgSize.height * 0.53f));
    
    pBuf = __String::createWithFormat("%d", tempInfo.life);
    Label* pCurBloodLabel = Label::createWithTTF(pBuf->getCString(), "fonts/cuti.ttf", 26);
    m_pLayer->addChild(pCurBloodLabel);
    pCurBloodLabel->setAnchorPoint(Vec2(0, 0.5f));
    pCurBloodLabel->enableOutline(Color4B(0, 0, 0, 255), 2);
    pCurBloodLabel->setPosition(Vec2(pCurBlood->getPositionX() + 8, bgSize.height * 0.53f));
    
    if(m_msgType != PVP_COMMSG_DETAIL)
    {
        pvpBuildInfo* pNextInfo = CDataManager::getBuildInfo(m_buildInfo->nextBuildingID);
        Sprite* pAttackArrow = Sprite::createWithSpriteFrameName("pvp_evolutionArrow.png");
        m_pLayer->addChild(pAttackArrow);
        pAttackArrow->setScale(0.35f);
        pAttackArrow->setPosition(Vec2(bgSize.width * 0.785f, bgSize.height * 0.61f));
        
        Sprite* pBloodArrow = Sprite::createWithSpriteFrameName("pvp_evolutionArrow.png");
        m_pLayer->addChild(pBloodArrow);
        pBloodArrow->setScale(0.35f);
        pBloodArrow->setPosition(Vec2(bgSize.width * 0.785f, bgSize.height * 0.53f));
        
        partolInfo tempNextInfo;
        if(pvpCommonData::getInstance()->getEvolution() == 0)
        {
            tempNextInfo = pNextInfo->allEvolutionVec[0];
        }
        else{
            tempNextInfo = pNextInfo->allEvolutionVec[1];
        }

        
        //进化后属性
        pBuf = __String::createWithFormat("%s.png", tempNextInfo.monsterIcon.c_str());
        Sprite* pNextXunLuo = Sprite::createWithSpriteFrameName(pBuf->getCString());
        m_pLayer->addChild(pNextXunLuo);
        pNextXunLuo->setAnchorPoint(Vec2(0, 0.5f));
        pNextXunLuo->setPosition(Vec2(bgSize.width * 0.84f, bgSize.height * 0.72f));
        
        Sprite* pNextAttack = Sprite::createWithSpriteFrameName("pvp_buildAttack.png");
        m_pLayer->addChild(pNextAttack);
        pNextAttack->setAnchorPoint(Vec2(1, 0.5f));
        pNextAttack->setPosition(Vec2(bgSize.width * 0.88f, bgSize.height * 0.61f));
        
        pBuf = __String::createWithFormat("%d", tempNextInfo.attack);
        Label* pNextAttackLabel = Label::createWithTTF(pBuf->getCString(), "fonts/cuti.ttf", 26);
        m_pLayer->addChild(pNextAttackLabel);
        pNextAttackLabel->enableOutline(Color4B(0, 0, 0, 255), 2);
        pNextAttackLabel->setAnchorPoint(Vec2(0, 0.5f));
        pNextAttackLabel->setPosition(Vec2(pNextAttack->getPositionX() + 8, bgSize.height * 0.61f));
        
        Sprite* pNextBlood = Sprite::createWithSpriteFrameName("pvp_blood.png");
        m_pLayer->addChild(pNextBlood);
        pNextBlood->setAnchorPoint(Vec2(1, 0.5f));
        pNextBlood->setPosition(Vec2(bgSize.width * 0.88f, bgSize.height * 0.53f));
        
        pBuf = __String::createWithFormat("%d", tempNextInfo.life);
        Label* pNextBloodLabel = Label::createWithTTF(pBuf->getCString(), "fonts/cuti.ttf", 26);
        m_pLayer->addChild(pNextBloodLabel);
        pNextBloodLabel->setAnchorPoint(Vec2(0, 0.5f));
        pNextBloodLabel->enableOutline(Color4B(0, 0, 0, 255), 2);
        pNextBloodLabel->setPosition(Vec2(pNextBlood->getPositionX() + 8, bgSize.height * 0.53f));
    }
}

void pvpCommonInfoLayer::showUpgradeBtn()
{
    Size bgSize = m_comBg->getBoundingBox().size;//显示注意(判断条件待定)
    if(m_msgType == PVP_COMMSG_NOT_UPGRADE)
    {
        Sprite* tipBg = Sprite::createWithSpriteFrameName("pvp_tipBg.png");
        m_pLayer->addChild(tipBg);
        tipBg->setPosition(Vec2(bgSize.width * 0.5f, bgSize.height * 0.25));
        
        Label* pTip = Label::createWithTTF("注意！", "fonts/cuti.ttf", 35);
        tipBg->addChild(pTip);
        pTip->enableOutline(Color4B(0, 0, 0, 255), 3);
        pTip->setPosition(Vec2(tipBg->getBoundingBox().size.width * 0.5f, tipBg->getBoundingBox().size.height * 0.76f));
        
        Label* pDesc = Label::createWithTTF(m_content.c_str(), "fonts/cuti.ttf", 30);
        tipBg->addChild(pDesc);
        pDesc->setPosition(Vec2(tipBg->getBoundingBox().size.width * 0.5f, tipBg->getBoundingBox().size.height * 0.32f));
        return;
    }
    char  buf[256];
    
    Sprite* stoneBg = Sprite::createWithSpriteFrameName("pvpUpgradeBg.png");
    m_pLayer->addChild(stoneBg);
    stoneBg->setPosition(Vec2(bgSize.width * 0.15f, bgSize.height * 0.18f));
    
    Sprite* stone = Sprite::createWithSpriteFrameName("stone.png");
    stoneBg->addChild(stone);
    stone->setScale(0.72f);
    stone->setAnchorPoint(Vec2(0, 0.5f));
    stone->setPosition(Vec2(stoneBg->getBoundingBox().size.width * 0.05f, stoneBg->getBoundingBox().size.height * 0.45f));
    
    //计算立即完成所需魔法石
    pvpBuildInfo* pBuildInfo = CDataManager::getBuildInfo(m_buildInfo->nextBuildingID);
    int stoneNum = pBuildInfo->costTime/60;
    if(stoneNum == 0)
    {
        stoneNum = 1;
    }
    sprintf(buf, "%d", stoneNum);
    Label* pLabelStone = Label::createWithTTF(buf, "fonts/cuti.ttf", 28);
    stoneBg->addChild(pLabelStone);
    pLabelStone->setAnchorPoint(Vec2(0, 0.5f));
    pLabelStone->setPosition(Vec2(stoneBg->getBoundingBox().size.width * 0.5f, stone->getPositionY()));
    
    Sprite* normal= Sprite::createWithSpriteFrameName("fight_blueBtn.png");
    Sprite* select= Sprite::createWithSpriteFrameName("fight_blueBtn.png");
    select->setColor(Color3B::GRAY);
    LYMenuItemSprite* finishItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(pvpCommonInfoLayer::btnCallBack, this));
    Menu* finishMenu = Menu::create(finishItem, NULL);
    finishItem->setTag(BTN_FINISH_TAG);
    finishItem->setUserData((void*)stoneNum);
    m_pLayer->addChild(finishMenu);
    finishMenu->setPosition(Vec2(bgSize.width * 0.38f, bgSize.height * 0.18));
    Label* finishWord = Label::createWithTTF("立即完成", "fonts/cuti.ttf", 30);
    finishItem->addChild(finishWord);
    finishWord->enableOutline(Color4B(0, 0, 0, 255), 3);
    finishWord->setPosition(Vec2(normal->getBoundingBox().size.width * 0.5f, normal->getBoundingBox().size.height * 0.5f));
    
    if(pBuildInfo->costTime <= 0)
    {
        finishMenu->setVisible(false);
        stoneBg->setVisible(false);
    }
    
    //金币
    Sprite* coinBg = Sprite::createWithSpriteFrameName("pvpUpgradeBg.png");
    m_pLayer->addChild(coinBg);
    coinBg->setPosition(Vec2(bgSize.width * 0.63f, bgSize.height * 0.18f));
    
    Sprite* coin = Sprite::createWithSpriteFrameName("coin.png");
    coinBg->addChild(coin);
    coin->setScale(0.95f);
    coin->setAnchorPoint(Vec2(0, 0.5f));
    coin->setPosition(Vec2(coinBg->getBoundingBox().size.width * 0.05f, coinBg->getBoundingBox().size.height * 0.5f));
    
    sprintf(buf, "%d", pBuildInfo->cost);
    Label* coinNum = Label::createWithTTF(buf, "fonts/cuti.ttf", 28);
    coinNum->setAnchorPoint(Vec2(0, 0.5f));
    coinBg->addChild(coinNum);
    coinNum->setPosition(Vec2(coinBg->getBoundingBox().size.width * 0.5f, coin->getPositionY()));
    
    //计算时间
    int allTime = pBuildInfo->costTime;
    std::string time;
    int second = allTime%60;
    int minute = allTime/60;
    int hour = 0;
    if(allTime < 60)
    {
        sprintf(buf, "%d秒", allTime);
        time += buf;
    }
    else
    {
        if(minute > 60)
        {
            hour = minute/60;
            minute = minute%60;
            if(hour > 0)
            {
                sprintf(buf, "%d小时", hour);
                time += buf;
            }
        }
        if(minute > 0)
        {
            sprintf(buf, "%d分", minute);
            time += buf;
        }
        if(second > 0)
        {
            sprintf(buf, "%d秒", second);
            time += buf;
        }
    }

    Label* pTime = Label::createWithTTF(time.c_str(), "fonts/cuti.ttf", 26);
    if(allTime == 0)
    {
        pTime = Label::createWithTTF("无", "fonts/cuti.ttf", 26);
    }
    m_pLayer->addChild(pTime);
    pTime->enableOutline(Color4B(0, 0, 0, 255), 3);
    //时间
    Sprite* clock = Sprite::createWithSpriteFrameName("TD_Clock.png");
    m_pLayer->addChild(clock);
    clock->setRotation(-15);
    
    
    
    if(((hour > 0) && (minute > 0)) || ((minute > 0) && (second > 0)))
    {
        pTime->setAnchorPoint(Vec2(1, 0.5f));
        pTime->setPosition(Vec2(bgSize.width * 0.96f, bgSize.height * 0.3));
        clock->setAnchorPoint(Vec2(1, 0.5f));
        clock->setPosition(Vec2(pTime->getPositionX() - pTime->getBoundingBox().size.width - 5, bgSize.height * 0.315));
    }
    else{
        clock->setAnchorPoint(Vec2(0, 0.5f));
        clock->setPosition(Vec2(bgSize.width * 0.75f, bgSize.height * 0.29));
        pTime->setAnchorPoint(Vec2(0, 0.5f));
        pTime->setPosition(Vec2(clock->getPositionX() + clock->getBoundingBox().size.width, bgSize.height * 0.3));
        
    }
    
    normal= Sprite::createWithSpriteFrameName("fight_greenBtn.png");
    select= Sprite::createWithSpriteFrameName("fight_greenBtn.png");
    select->setColor(Color3B::GRAY);
    LYMenuItemSprite* upgradeItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(pvpCommonInfoLayer::btnCallBack, this));
    Menu* upgradeMenu = Menu::create(upgradeItem, NULL);
    m_pLayer->addChild(upgradeMenu);
    upgradeItem->setTag(BTN_UPGRADE_TAG);
    upgradeMenu->setPosition(Vec2(bgSize.width * 0.86f, bgSize.height * 0.18));
    Label* upgrade = Label::createWithTTF("升级", "fonts/cuti.ttf", 30);
    upgradeItem->addChild(upgrade);
    upgrade->enableOutline(Color4B(0, 0, 0, 255), 3);
    upgrade->setPosition(Vec2(normal->getBoundingBox().size.width * 0.5f, normal->getBoundingBox().size.height * 0.5f));
    
}

void pvpCommonInfoLayer::addAcquireAward(Ref* pTarget, SEL_MenuHandler phander, int staminaNum)
{
    m_comBg = Sprite::createWithSpriteFrameName("pvp_infoBg.png");
    addChild(m_comBg);
    m_comBg->setPosition(Vec2(m_winSize.width * 0.5f, m_winSize.height * 0.45f));
    m_comBg->setScaleX(1.5f);
    m_comBg->setScaleY(1.4f);
    
    Size bgSize = m_comBg->getBoundingBox().size;
    m_pLayer = LayerColor::create(Color4B(0, 0, 0, 0), bgSize.width, bgSize.height);
    addChild(m_pLayer);
    m_pLayer->setPosition(Vec2(m_comBg->getPositionX() - bgSize.width * 0.5f, m_comBg->getPositionY() - bgSize.height * 0.5f));
    
    
    m_pTarget = pTarget;
    m_pHandler = phander;
    
    Label* pContent = Label::createWithTTF("防守成功获得奖励:", "fonts/cuti.ttf", 32);
    pContent->enableOutline(Color4B(0, 0, 0, 255), 3);
    m_pLayer->addChild(pContent);
    pContent->setPosition(bgSize.width * 0.5f, bgSize.height * 0.7f);
    
    Sprite* stamina = Sprite::createWithSpriteFrameName("stamina.png");
    m_pLayer->addChild(stamina);
    stamina->setScale(0.8f);
    stamina->setPosition(bgSize.width * 0.55f, bgSize.height * 0.48f);
    
    __String* tmp = __String::createWithFormat("%d", staminaNum);
    Label* pStamina = Label::createWithTTF(tmp->getCString(), "fonts/cuti.ttf", 34);
    m_pLayer->addChild(pStamina);
    pStamina->enableOutline(Color4B(0, 0, 0, 255), 2);
    pStamina->setAnchorPoint(Vec2(1, 0.5f));
    pStamina->setPosition(stamina->getPositionX() - stamina->getBoundingBox().size.width * 1.2f, stamina->getPositionY());
    
    Sprite* normal= Sprite::createWithSpriteFrameName("pvp_fight.png");
    Sprite* select= Sprite::createWithSpriteFrameName("pvp_fight.png");
    select->setColor(Color3B::GRAY);
    LYMenuItemSprite* pGetItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(pvpCommonInfoLayer::OnSure, this));
    Menu* pGetMenu = Menu::create(pGetItem, NULL);
    m_pLayer->addChild(pGetMenu);
    pGetMenu->setPosition(Vec2(bgSize.width * 0.5f, bgSize.height * 0.18));
    Label* pGetLabel = Label::createWithTTF("领取", "fonts/cuti.ttf", 30);
    pGetItem->addChild(pGetLabel);
    pGetLabel->enableOutline(Color4B(0, 0, 0, 255), 3);
    pGetLabel->setPosition(Vec2(normal->getBoundingBox().size.width * 0.5f, normal->getBoundingBox().size.height * 0.5f));
    
    normal = Sprite::createWithSpriteFrameName("closeBtn.png");
    select = Sprite::createWithSpriteFrameName("closeBtn.png");
    select->setColor(Color3B::GRAY);
    LYMenuItemSprite* pCloseItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(pvpCommonInfoLayer::btnCallBack, this));
    pCloseItem->setTag(BTN_GET_AWARD_EXIT);
    Menu* pMenu = Menu::create(pCloseItem, NULL);
    m_pLayer->addChild(pMenu);
    pMenu->setPosition(Vec2(bgSize.width * 0.98f, bgSize.height * 0.96f));
}

void pvpCommonInfoLayer::showSearchResult(Ref* pTarget, SEL_MenuHandler pHander)
{
    m_comBg = Sprite::createWithSpriteFrameName("pvp_infoBg.png");
    addChild(m_comBg);
    m_comBg->setPosition(Vec2(m_winSize.width * 0.5f, m_winSize.height * 0.5f));
    m_comBg->setScaleX(1.5f);
    m_comBg->setScaleY(1.4f);
    
    Size bgSize = m_comBg->getBoundingBox().size;
    m_pLayer = LayerColor::create(Color4B(0, 0, 0, 0), bgSize.width, bgSize.height);
    addChild(m_pLayer);
    m_pLayer->setPosition(Vec2(m_comBg->getPositionX() - bgSize.width * 0.5f, m_comBg->getPositionY() - bgSize.height * 0.5f));
    
    
    m_pTarget = pTarget;
    m_pHandler = pHander;
    
    Label* pContent = Label::createWithSystemFont("目前未搜索到任何玩家，请稍后再试", "Arial", 28);
    m_pLayer->addChild(pContent);
    pContent->setColor(Color3B::BLACK);
    pContent->setWidth(bgSize.width * 0.8f);
    pContent->setPosition(Vec2(bgSize.width * 0.5f, bgSize.height * 0.65f));
    
    
    Sprite* normal = Sprite::createWithSpriteFrameName("greenbtn.png");
    Sprite* select = Sprite::createWithSpriteFrameName("greenbtn.png");
    select->setColor(Color3B::GRAY);
    LYMenuItemSprite* sureItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(pvpCommonInfoLayer::OnSure, this));
    sureItem->setScale(2.6f);
    Menu* pSureMenu = Menu::create(sureItem, NULL);
    sureItem->setTag(BTN_OK_UPGRADE);
    m_pLayer->addChild(pSureMenu);
    pSureMenu->setPosition(Vec2(bgSize.width * 0.5f, bgSize.height * 0.2f));
    
    Label* pSureLabel = Label::createWithTTF("确定", "fonts/cuti.ttf", 28);
    pSureLabel->setPosition(Vec2(normal->getContentSize().width * 0.5f, normal->getContentSize().height * 0.5f));
    sureItem->addChild(pSureLabel, 5);
    pSureLabel->setScale(0.4f);
    pSureLabel->enableOutline(Color4B(0, 0, 0, 255), 2);
}

void pvpCommonInfoLayer::clearProtectTime(Ref* pTarget, SEL_MenuHandler phander)
{
    m_comBg = Sprite::createWithSpriteFrameName("pvp_infoBg.png");
    addChild(m_comBg);
    m_comBg->setPosition(Vec2(m_winSize.width * 0.5f, m_winSize.height * 0.5f));
    m_comBg->setScaleX(1.5f);
    m_comBg->setScaleY(1.4f);
    
    Size bgSize = m_comBg->getBoundingBox().size;
    m_pLayer = LayerColor::create(Color4B(0, 0, 0, 0), bgSize.width, bgSize.height);
    addChild(m_pLayer);
    m_pLayer->setPosition(Vec2(m_comBg->getPositionX() - bgSize.width * 0.5f, m_comBg->getPositionY() - bgSize.height * 0.5f));
    
    
    m_pTarget = pTarget;
    m_pHandler = phander;
    
    Label* pContent = Label::createWithSystemFont("您现在正处于保护时间内，如果进攻玩家，保护时间将归0。是否继续？", "Arial", 28);
    m_pLayer->addChild(pContent);
    pContent->setColor(Color3B::BLACK);
    pContent->setWidth(bgSize.width * 0.8f);
    pContent->setPosition(Vec2(bgSize.width * 0.5f, bgSize.height * 0.65f));
    
    Sprite* normal = Sprite::createWithSpriteFrameName("redbtn.png");
    Sprite* select = Sprite::createWithSpriteFrameName("redbtn.png");
    select->setColor(Color3B::GRAY);
    LYMenuItemSprite* cancelItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(pvpCommonInfoLayer::OnCancel, this));
    Menu* pCancelMenu = Menu::create(cancelItem, NULL);
    cancelItem->setScale(2.6f);
    m_pLayer->addChild(pCancelMenu);
    pCancelMenu->setPosition(Vec2(bgSize.width * 0.25f, bgSize.height * 0.2f));
    
    Label* pCancelLabel = Label::createWithTTF("取消", "fonts/cuti.ttf", 28);
    pCancelLabel->setPosition(Vec2(normal->getContentSize().width * 0.5f, normal->getContentSize().height * 0.5f));
    cancelItem->addChild(pCancelLabel, 5);
    pCancelLabel->setScale(0.4f);
    pCancelLabel->enableOutline(Color4B(0, 0, 0, 255), 2);
    
    
    normal = Sprite::createWithSpriteFrameName("greenbtn.png");
    select = Sprite::createWithSpriteFrameName("greenbtn.png");
    select->setColor(Color3B::GRAY);
    LYMenuItemSprite* sureItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(pvpCommonInfoLayer::OnSure, this));
    sureItem->setScale(2.6f);
    Menu* pSureMenu = Menu::create(sureItem, NULL);
    sureItem->setTag(BTN_OK_UPGRADE);
    m_pLayer->addChild(pSureMenu);
    pSureMenu->setPosition(Vec2(bgSize.width * 0.75f, bgSize.height * 0.2f));
    
    Label* pSureLabel = Label::createWithTTF("确定", "fonts/cuti.ttf", 28);
    pSureLabel->setPosition(Vec2(normal->getContentSize().width * 0.5f, normal->getContentSize().height * 0.5f));
    sureItem->addChild(pSureLabel, 5);
    pSureLabel->setScale(0.4f);
    pSureLabel->enableOutline(Color4B(0, 0, 0, 255), 2);
    
    normal = Sprite::createWithSpriteFrameName("closeBtn.png");
    select = Sprite::createWithSpriteFrameName("closeBtn.png");
    select->setColor(Color3B::GRAY);
    LYMenuItemSprite* pCloseItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(pvpCommonInfoLayer::btnCallBack, this));
    pCloseItem->setTag(BTN_GET_AWARD_EXIT);
    Menu* pMenu = Menu::create(pCloseItem, NULL);
    m_pLayer->addChild(pMenu);
    pMenu->setPosition(Vec2(bgSize.width * 0.98f, bgSize.height * 0.96f));
}

void pvpCommonInfoLayer::gotoShop(Ref* pSender)
{
    CPersonalData::getInstance()->setTouchRmvFlag(true);
    CLayerManager::GetSingletonPtr()->createLayer(eLayer_Charge,getParent(),false);
    String* tempStr = String::createWithFormat("1");
    EventParamMap paramMap;
    
    paramMap.insert(EventParamPair("点击次数",tempStr->getCString()));
    
    TDCCTalkingDataGA::onEvent("英雄宝石充值", &paramMap);
}

void pvpCommonInfoLayer::btnCallBack(Ref* pSender)
{
    int tag = ((Node*)pSender)->getTag();
    if(BTN_FINISH_TAG == tag)
    {
        log("立即完成");
        long stoneNum = (long)((LYMenuItemSprite*)pSender)->getUserData();
        if(CPersonalData::getInstance()->getUserStone() < stoneNum)
        {
//            Sprite * thetarget = CFightManager::getTheFlyWord("魔法石不足","fonts/cuti.ttf");
//            thetarget->setPosition(m_winSize.width*0.5,m_winSize.height*0.5);
//            addChild(thetarget,100);
            CHitDialog *pDlg = CHitDialog::create();
            pDlg->createYesNO(this, menu_selector(pvpCommonInfoLayer::gotoShop),"魔法石不足，是否去充值？");
            addChild(pDlg);
            return;
        }
        pvpBuildInfo* pBuildInfo = CDataManager::getBuildInfo(m_buildInfo->nextBuildingID);
        if(CPersonalData::getInstance()->getUserCoin() < pBuildInfo->cost)
        {
            Sprite * thetarget = CFightManager::getTheFlyWord("金币不足","fonts/cuti.ttf");
            thetarget->setPosition(m_winSize.width*0.5,m_winSize.height*0.5);
            addChild(thetarget,100);
            return;
        }
        __String* tempStr = __String::createWithFormat("%ld", stoneNum);
        EventParamMap paramMap;
        
        paramMap.insert(EventParamPair("升级花费魔法石",tempStr->getCString()));
        
        TDCCTalkingDataGA::onEvent("升级", &paramMap);
        ((pvpMainLayer*)getParent())->directlyFinish();
    }
    else if(BTN_UPGRADE_TAG == tag)
    {
        log("开始升级");
        pvpBuildInfo* pBuildInfo = CDataManager::getBuildInfo(m_buildInfo->nextBuildingID);
        if(CPersonalData::getInstance()->getUserCoin() < pBuildInfo->cost)
        {
            Sprite * thetarget = CFightManager::getTheFlyWord("金币不足","fonts/cuti.ttf");
            thetarget->setPosition(m_winSize.width*0.5,m_winSize.height*0.5);
            addChild(thetarget,100);
            return;
        }
        else
        {
            CNetWorkDataSingle::GetSingletonPtr()->SaveUserMCF(this, menu_selector(pvpCommonInfoLayer::updateUserData), -m_buildInfo->cost,0,0);
        }
    }
    else if(BTN_EVOLUTION_TAG == tag)
    {
        if(CPersonalData::getInstance()->getUserStone() < 1000)
        {
            Sprite * thetarget = CFightManager::getTheFlyWord("魔法石不足","fonts/cuti.ttf");
            thetarget->setPosition(m_winSize.width*0.5,m_winSize.height*0.5);
            addChild(thetarget,100);
            return;
        }
        else
        {
            CNetWorkDataSingle::GetSingletonPtr()->SaveUserMCF(this, menu_selector(pvpCommonInfoLayer::updateUserData), 0,-1000,0);
        }
    }
    else if((BTN_EXIT_TAG == tag) || (BTN_CANCEL_UPGRADE == tag))
    {
        CPersonalData::getInstance()->setTouchRmvFlag(true);
        if(PVP_COMMSG_UPGRADE_TIP == m_msgType)
        {
            ((pvpMainLayer*)getParent())->rmvNewTower();
        }
        else{
            ((pvpMainLayer*)getParent())->rmvTowerInfoLayer();
        }
    }
    else if(BTN_GET_AWARD_EXIT == tag)
    {
        CPersonalData::getInstance()->setTouchRmvFlag(true);
        removeFromParent();
    }
    else if(tag == PRODUCT_MINUS_TAG)
    {
        ControlSlider* solider = (ControlSlider*)m_node->getChildByTag(1000);
        if (solider) {
            solider->setValue(solider->getValue() - 1);
            checkBtnEnabled(solider);
        }
    }
    else if (tag == PRODUCT_ADD_TAG)
    {
        ControlSlider* solider = (ControlSlider*)m_node->getChildByTag(1000);
        if (solider) {
            solider->setValue(solider->getValue() + 1);
            checkBtnEnabled(solider);
        }
    }
    else if(tag == PRODUCT_START_TAG)
    {
        //发送请求开始熔炼
        ControlSlider* solider = (ControlSlider*)m_node->getChildByTag(1000);
        if(solider->getValue() < 1)
        {
            std::string content;
            if(CPersonalData::getInstance()->getUserKuangStone() < 100)
            {
                content = "矿石不足";
            }
            else{
                content = "请选择矿石数量";
            }
            Sprite * thetarget = CFightManager::getTheFlyWord(content.c_str(), "fonts/cuti.ttf");
            thetarget->setPosition(m_winSize.width*0.5,m_winSize.height*0.5);
            addChild(thetarget,100);
            return;
        }
        m_remainTime  = 10;
        
        m_needTime = m_remainTime;
        m_productNum = (int)solider->getValue();
        __String* buf = __String::createWithFormat("%d", m_userBuildingId);

        CNetControlModule::sharedNetControlModule()->startProduct(this, menu_selector(pvpCommonInfoLayer::startProductBack), buf->getCString(), m_productNum, m_remainTime);
        if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_PVP_RONGLU) && (guiderManagerLayer::getInstance()->getStepNum() == 10))
        {
            CDataManager::saveGuideInfoToDB(GUIDE_PVP_RONGLU);
            guiderManagerLayer::getInstance()->setStepNum(guiderManagerLayer::getInstance()->getStepNum() + 1);
            guiderManagerLayer::getInstance()->createLayer(this);
        }
    }
}

void pvpCommonInfoLayer::startProductBack(Ref *pSender)
{
    if(CPersonalData::getInstance()->getState() == 0)
    {
        CPersonalData::getInstance()->setUserKuangStone(CPersonalData::getInstance()->getUserKuangStone() - m_productNum * 100);
        CPersonalData::getInstance()->setUpdateUserInfo(true);
        pvpBuildInfo* pInfo = CDataManager::getBuildInfo(m_buildingId);
        __String* pBuf = __String::createWithFormat("%d/%d", CPersonalData::getInstance()->getUserKuangStone(), CPersonalData::getInstance()->getUserMaxKuangStone());
        m_pNum->setString(pBuf->getCString());

        pBuf = __String::createWithFormat("最大熔炼个数: %d", pInfo->effectTime);
        m_pLimit->setString(pBuf->getCString());
        
        addProvessInfo();
        m_remainTime--;
        scheduleUpdate();
        
        //保存熔炉的状态并调用保存玩家阵营接口
        ((pvpMainLayer*)getParent())->saveRongluState(m_userBuildingId);
        ((pvpMainLayer*)getParent())->startProduct(m_remainTime + 1);
    }
}

void pvpCommonInfoLayer::updateUserData(Ref *pSender)
{
    CPersonalData::getInstance()->setUpdateUserInfo(true);
    if(m_msgType == PVP_COMMSG_EVOLUTION)
    {
        ((pvpMainLayer*)getParent())->evolutionBack();
    }
    else{
        int build = m_buildInfo->nextBuildingID;
        ((pvpMainLayer*)getParent())->startUpgradeTower(build);
    }
    
}

void pvpCommonInfoLayer::OnSure(Ref *pSender)
{
    if(m_pTarget && m_pHandler)
    {
        (m_pTarget->*m_pHandler)(pSender);
    }
    if(m_msgType == PVP_COMMSG_CANCEL_UPGRADE)
    {
        CPersonalData::getInstance()->setTouchRmvFlag(true);
    }
    else if(m_msgType == PVP_COMMSG_NOFIND)
    {
        removeFromParent();
    }
}

void pvpCommonInfoLayer::OnCancel(Ref *pSender)
{
    ((pvpMainLayer*)getParent())->m_layer = NULL;
    removeFromParent();
}

