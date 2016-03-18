;//
//  CHeroCompoundLayer.cpp
//  TheGodWill
//
//  Created by user on 14/12/24.
//
//

#include "heroCompoundLayer.h"
#include "commonPropLayer.h"
#include "DataManager.h"
#include "LYPersonalData.h"
#include "mainMenuLayer.h"
#include "CSharedPlist.h"
#include "LYFightManager.h"
#include "DBMgr.h"
#include "SkillTouchDetail.h"
#include "NetWorkDataSingle.h"
#include "Common.h"
#include "guiderManagerLayer.h"

#define HERO_NUM 12//每页显示英雄数目

CHeroCompoundLayer::CHeroCompoundLayer()
{
    m_node = NULL;
    m_bg = NULL;
    m_pMenu = NULL;
    m_pageLabel = NULL;
    m_leftItem = NULL;
    m_rightItem = NULL;
    m_layer = NULL;
    m_hero = NULL;
    m_otherMenu = NULL;
    m_page = 1;
    m_maxPage = 1;
    m_selectedHeroId = 0;
    m_skill.clear();
    m_bisLackMaterial = false;
    m_starPos = Vec2::ZERO;
    m_sortVec.clear();
    m_allMonster.clear();
    m_curShow = 0;
    m_InfoBg = NULL;
    m_bSkillMenu = false;
    m_pSKillMenu = NULL;
    m_exitItem = NULL;
    m_validMonsterNum = 0;
    CSharedPlist::getInstance()->pushPlist("plist/heroCompound.plist");
}

CHeroCompoundLayer::~CHeroCompoundLayer()
{
    CSharedPlist::getInstance()->erasePlist("plist/heroCompound.plist");
}

bool CHeroCompoundLayer::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    if(!m_curShow && (m_hero->getBoundingBox().containsPoint(touch->getLocationInView())))
    {
        m_starPos = touch->getLocation();
    }

    
     CSKillTouchDetail *pNode = ( CSKillTouchDetail *)getChildByTag(997);
    
    if(pNode)
    {
        pNode->removeFromParent();
    }
    

    return true;
}

void CHeroCompoundLayer::onTouchMoved(Touch *touch, Event *unused_event)
{
    if(!m_curShow)
    {
        Point movePos = touch->getLocationInView();
        Point prePos = touch->getPreviousLocationInView();
        if(movePos.operator!=(m_starPos))
        {
            float xPos = movePos.x - prePos.x;
            m_hero->setRotation3D(Vec3(0, (m_hero->getRotation3D().y + xPos), 0));
        }
    }
 
}

void CHeroCompoundLayer::onTouchEnded(Touch *touch, Event *unused_event)
{
    m_starPos = Vec2::ZERO;
 
}

bool CHeroCompoundLayer::init()
{
    if (!CBaseLayer::initWithColor(Color4B(0, 0, 0, 200))) {
        return false;
    }
    
    m_winSize = Director::getInstance()->getWinSize();
    
    auto listen = EventListenerTouchOneByOne::create();
    listen->setSwallowTouches(true);
    listen->onTouchBegan = CC_CALLBACK_2(CHeroCompoundLayer::onTouchBegan, this);
    listen->onTouchMoved = CC_CALLBACK_2(CHeroCompoundLayer::onTouchMoved, this);
    listen->onTouchEnded = CC_CALLBACK_2(CHeroCompoundLayer::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listen, this);
    //添加背景
    initUI();
    
    if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SOLIDER_COMPOUND) && (guiderManagerLayer::getInstance()->getStepNum() == 1))
    {
        guiderManagerLayer::getInstance()->setStepNum(guiderManagerLayer::getInstance()->getStepNum() + 1);
        guiderManagerLayer::getInstance()->createLayer(this);
        m_hero->setVisible(false);
    }
    return true;
}

void CHeroCompoundLayer::initUI()
{
    char buf[128];
    m_bg = Sprite::createWithSpriteFrameName("hero_bg.png");
    addChild(m_bg);
    m_bg->setPosition(Vec2(m_winSize.width / 2, m_winSize.height / 2));
    Size bgSize = m_bg->getContentSize();
    
    Label* name = Label::createWithTTF("英雄合成", "fonts/cuti.ttf", 32);
    m_bg->addChild(name);
    name->enableOutline(Color4B(0, 0, 0, 255), 3);
    name->setPosition(Vec2(bgSize.width / 2, bgSize.height - name->getContentSize().height));
    
    //切换
    Sprite* kuang = Sprite::createWithSpriteFrameName("select_kuang.png");
    m_bg->addChild(kuang, 1);
    kuang->setScale(1.5f);
    kuang->setPosition(Vec2(bgSize.width * 0.3, bgSize.height * 0.10));
    //计算当前总页数
    std::vector<monsterInfo*> theModleInfo = CDataManager::getAllMonsterInfo();
    for(int i = 0; i < theModleInfo.size(); i++){
        if(theModleInfo.at(i)->isHaveDebris == 0)//代表显示碎片
        {
            m_allMonster.push_back(theModleInfo.at(i)->monsterId);
        }
    }
    //手动加入未解锁的怪物,先找出当前已有怪物的最大id
    for(int m = 0; m < (int)m_allMonster.size(); m++)
    {
        if(m_allMonster[m] > m_validMonsterNum)
        {
            m_validMonsterNum = m_allMonster[m];
        }
    }
    for(int i = (int)(theModleInfo.size() + 1); i < (int)(theModleInfo.size() + 36); i++)
    {
        if(m_allMonster.size() < 36)
        {
            m_allMonster.push_back(i);
        }
        else{
            break;
        }
    }
    
    int temp = (int)m_allMonster.size() / HERO_NUM;
    if(m_allMonster.size() % HERO_NUM == 0){
        if(temp == 0){
            m_maxPage = 1;
        }
        else{
            m_maxPage = temp;
        }
    }
    else{
        m_maxPage = temp + 1;
    }
    
    //当前页/总数
    sprintf(buf, "%d/%d", m_page, m_maxPage);
    m_pageLabel = Label::createWithSystemFont(buf, "Arial", 25);
    m_bg->addChild(m_pageLabel, 1);
    m_pageLabel->setPosition(kuang->getPosition());
    
    Sprite* normal = Sprite::createWithSpriteFrameName("arrow.png");
    Sprite* select = Sprite::createWithSpriteFrameName("arrow.png");
    select->setColor(Color3B::GRAY);
    normal->setFlippedX(true);
    select->setFlippedX(true);
    m_leftItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(CHeroCompoundLayer::btnCallBack, this), "music/back.mp3");
    Menu* leftMenu = Menu::create(m_leftItem, NULL);
    m_leftItem->setTag(LEFT_ARROW_TAG);
    leftMenu->setPosition(Vec2(kuang->getPositionX() - kuang->getBoundingBox().size.width * 0.7f, kuang->getPositionY()));
    m_bg->addChild(leftMenu, 1);
    
    normal = Sprite::createWithSpriteFrameName("arrow.png");
    select = Sprite::createWithSpriteFrameName("arrow.png");
    select->setColor(Color3B::GRAY);
    m_rightItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(CHeroCompoundLayer::btnCallBack, this), "music/next.mp3");
    Menu* rightMenu = Menu::create(m_rightItem, NULL);
    m_rightItem->setTag(RIGHT_ARROW_TAG);
    rightMenu->setPosition(Vec2(kuang->getPositionX() + kuang->getBoundingBox().size.width * 0.7f, kuang->getPositionY()));
    m_bg->addChild(rightMenu, 1);
    
    //先排序
    sortHero();
    updateHeroShow();
    
    //退出按钮
    normal = Sprite::createWithSpriteFrameName("closeBtn.png");
    select = Sprite::createWithSpriteFrameName("closeBtn.png");
    select->setColor(Color3B::GRAY);
    m_exitItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(CHeroCompoundLayer::btnCallBack, this));
    CommonData::enlargeItemSprite(m_exitItem, 2);
    Menu* exitMenu = Menu::create(m_exitItem, NULL);
    m_exitItem->setTag(EXIT_TAG);
    exitMenu->setPosition(Vec2(bgSize.width * 0.95, bgSize.height* 0.93));
    m_bg->addChild(exitMenu);
    
}

void CHeroCompoundLayer::adjustHeroInfo()
{
    stopAllActions();
    if(m_selectedHeroId > m_validMonsterNum)
    {
        return;
    }
    char buf[128];
    if(m_layer){
        m_layer->removeFromParent();
        m_layer = NULL;
    }
    if(m_InfoBg)
    {
        m_InfoBg->setVisible(false);
    }
    m_curShow = 0;
    m_layer = LayerColor::create(Color4B(255, 0, 0, 0), 405, 560);
    m_bg->addChild(m_layer, 2);
    Size layerSize = m_layer->getContentSize();
    m_layer->setPosition(Vec2(m_bg->getContentSize().width * 0.6, 55));
    
    //获取选中英雄的基本信息
    monsterInfo* pMonsterInfo = CDataManager::getMonsterInfoByMonsterId(m_selectedHeroId);
    
    
    if(pMonsterInfo)
    {
        //英雄名称
        Label* heroName = Label::createWithSystemFont(pMonsterInfo->monsterName.c_str(), "Arial", 30);
        m_layer->addChild(heroName, 5);
        heroName->setPosition(Vec2(layerSize.width / 2, layerSize.height - heroName->getContentSize().height));
    }
    
    m_otherMenu = Menu::create();
    m_layer->addChild(m_otherMenu);
    m_otherMenu->setPosition(Vec2::ZERO);
    Sprite* normal = Sprite::createWithSpriteFrameName("bluean.png");
    Sprite* select = Sprite::createWithSpriteFrameName("bluean.png");
    
    select->setColor(Color3B::GRAY);
    LYMenuItemSprite* pInfoItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(CHeroCompoundLayer::btnCallBack, this));
    pInfoItem->setTag(HREO_INFO_TAG);
    m_otherMenu->addChild(pInfoItem);
    pInfoItem->setScale(1.1f);
    pInfoItem->setPosition(Vec2(layerSize.width * 0.23, layerSize.height * 0.05));
    
    
    Label* infoLabel = Label::createWithSystemFont("详情", "Arial", 22);
    pInfoItem->addChild(infoLabel);
    infoLabel->setPosition(Vec2(normal->getBoundingBox().size.width / 2, normal->getBoundingBox().size.height / 2));
    
    //fazhen
    Sprite* fazhen = Sprite::createWithSpriteFrameName("fazhen.png");
    m_layer->addChild(fazhen);
    fazhen->setScaleX(2.0f);
    fazhen->setScaleY(0.5);
    fazhen->setPosition(Vec2(layerSize.width / 2, layerSize.height * 0.45f));
    
    //获取英雄模型
    
    modelInfo* pModelInfo = CDataManager::getModelInfoByModelId(pMonsterInfo->modelId);
    m_hero = Sprite3D::create(pModelInfo->modelName.getCString());
    Size modeSize = m_hero->getBoundingBox().size;
    float scale;
    
    if(modeSize.width * modeSize.height < 30)
    {
        scale = 830/(modeSize.width * modeSize.height);
        m_hero->setScale(scale);
    }
    else if(modeSize.width * modeSize.height < 50)
    {
        m_hero->setScale(1250/(modeSize.width * modeSize.height));
    }
    else if(modeSize.width * modeSize.height < 100)
    {
        m_hero->setScale(1450/(modeSize.width * modeSize.height));
    }
    else if(modeSize.width * modeSize.height < 150){
        m_hero->setScale(1650/(modeSize.width * modeSize.height));
    }
    
    else{
        m_hero->setScale(28*8.5/modeSize.width);;
    }
    
    m_hero->setGlobalZOrder(100);
    m_layer->addChild(m_hero);
    m_hero->setPosition(Vec2(layerSize.width / 2, layerSize.height * 0.45f));
    
    if((pMonsterInfo->monsterId == 9) || (pMonsterInfo->monsterId == 12) || (pMonsterInfo->monsterId == 15))
    {
        m_hero->setRotation3D(Vec3(0, 40, 0));
    }
    else
    {
        m_hero->setRotation3D(Vec3(0, -25, 0));
    }
    auto animation = Animation3D::create(pModelInfo->modelName.getCString());
    if (animation)
    {
        auto animate = Animate3D::create(animation, pModelInfo->idleFrame.start/60, pModelInfo->idleFrame.duration/60);
        
        if(pMonsterInfo->monsterId ==  14|| pMonsterInfo->monsterId ==  15)
        {
            animate->setSpeed(0.3f);//测试用，换模型了就删掉
        }
        
        cocos2d::Action* _swim = RepeatForever::create(animate);
        m_hero->runAction(_swim);
    }
    
    
    //获取当前用户拥有的碎片
    int avaliableNum = CDataManager::getDebrisNumById(m_selectedHeroId);
    int rare = CDataManager::getMonsterStarById(m_selectedHeroId);
    
    //根据星级获取碎片数
    if(rare < 5)
    {
        sprintf(buf, "所需材料 : %d/%d", avaliableNum, CDataManager::getMonsterEvolutionById(rare + 1)->m_number);
        Label* material = Label::createWithSystemFont(buf, "Arial", 26);
        m_layer->addChild(material);
        material->setColor(Color3B::YELLOW);
        material->setAnchorPoint(Vec2(0, 0.5));
        material->setPosition(Vec2(5, layerSize.height * 0.3f));
        
        //购买碎片按钮
        normal = Sprite::createWithSpriteFrameName("organge.png");
        select = Sprite::createWithSpriteFrameName("organge.png");
        select->setColor(Color3B::GRAY);
        LYMenuItemSprite* pBuyItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(CHeroCompoundLayer::btnCallBack, this));
        pBuyItem->setTag(BUY_DEBRIS_TAG);
        pBuyItem->setScale(1.4f);
        m_otherMenu->addChild(pBuyItem);
        pBuyItem->setPosition(Vec2(layerSize.width * 0.73f, material->getPositionY()));
        
        Label* debrisLabel = Label::createWithSystemFont("购买碎片", "Arial", 26);
        pBuyItem->addChild(debrisLabel);
        debrisLabel->setScale(0.6f);
        debrisLabel->setPosition(Vec2(normal->getContentSize().width / 2, normal->getContentSize().height / 2));
        pBuyItem->setEnabled(false);
        pBuyItem->setColor(Color3B::GRAY);
        
        //进阶按钮
        normal = Sprite::createWithSpriteFrameName("yellowan.png");
        select = Sprite::createWithSpriteFrameName("yellowan.png");
        select->setColor(Color3B::GRAY);
        LYMenuItemSprite* pAdvanceItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(CHeroCompoundLayer::btnCallBack, this));
        pAdvanceItem->setTag(ADVANCE_START_TAG);
        pAdvanceItem->setScale(1.15f);
        m_otherMenu->addChild(pAdvanceItem);
        pAdvanceItem->setPosition(Vec2(layerSize.width * 0.715f, layerSize.height * 0.05f));
        Label* advaceLabel = Label::createWithSystemFont("进 阶", "Arial", 28);
        if(rare == 0)
        {
            advaceLabel = Label::createWithSystemFont("合 成", "Arial", 28);
        }
        pAdvanceItem->addChild(advaceLabel);
        advaceLabel->setColor(Color3B::BLUE);
        advaceLabel->setScale(0.8f);
        advaceLabel->setPosition(Vec2(normal->getContentSize().width / 2, normal->getContentSize().height * 0.6f));
        
        if(avaliableNum < CDataManager::getMonsterEvolutionById(rare + 1)->m_number)
        {
            pBuyItem->setEnabled(true);
            pBuyItem->setColor(Color3B::WHITE);
            m_bisLackMaterial = true;
            pAdvanceItem->setColor(Color3B::GRAY);
        }
        else
        {
            pBuyItem->setEnabled(false);
            pBuyItem->setColor(Color3B::GRAY);
            m_bisLackMaterial = false;
            pAdvanceItem->setColor(Color3B::WHITE);
        }
        
        //合成费用
        Label* pHecheng = Label::createWithSystemFont("合成费用 : ", "Arial", 26);
        m_layer->addChild(pHecheng);
        pHecheng->setAnchorPoint(Vec2(0, 0.5));
        pHecheng->setColor(Color3B::YELLOW);
        pHecheng->setPosition(Vec2(5, layerSize.height * 0.175f));
        
        Sprite* coin = Sprite::createWithSpriteFrameName("coin.png");
        m_layer->addChild(coin);
        coin->setScale(0.9f);
        coin->setAnchorPoint(Vec2(0, 0.5));
        coin->setPosition(Vec2(pHecheng->getPositionX() + pHecheng->getContentSize().width + 10, pHecheng->getPositionY()));
        
        sprintf(buf, "%d", CDataManager::getMonsterEvolutionById(rare + 1)->m_coinNum);
        Label* cost = Label::createWithSystemFont(buf, "Arial", 26);
        m_layer->addChild(cost);
        cost->setColor(Color3B::YELLOW);
        cost->setAnchorPoint(Vec2(0, 0.5));
        cost->setPosition(Vec2(coin->getPositionX() + coin->getBoundingBox().size.width + 10, pHecheng->getPositionY()));
    }
}

void CHeroCompoundLayer::OnSkillTouch(Ref *pSender)
{
    MenuItemSprite *p = (MenuItemSprite*)pSender;
    
    int iTag = p->getTag();


     long idx =  (long)p->getUserData();
    

     CSKillTouchDetail *pNode = ( CSKillTouchDetail *)getChildByTag(997);
    
    if(pNode)
    {
        pNode->removeFromParent();
    }

    
    //m_selectedHeroId;
    
      std::vector<FighterInfo*> userMonterVec = CPersonalData::getInstance()->getAllSoldierInfo();
    
    
    
    FighterInfo  fighterInfo; //=  new FighterInfo;
 
    
    
    fighterInfo.monsterId = m_selectedHeroId;
    
    monsterInfo* pInfo = CDataManager::getMonsterInfoByMonsterId(m_selectedHeroId);
    
    fighterInfo.rare =  CDataManager::getMonsterStarById(m_selectedHeroId);
    fighterInfo.attk = pInfo->minAtk;
    
    
    CSKillTouchDetail *pLayer = CSKillTouchDetail::create(&fighterInfo, iTag);
    
    pLayer->setPositionX(-m_winSize.width*0.025+iTag*0.03*m_winSize.width);
    

    if(idx==0)
    {
        
        
        pLayer->setPositionX(m_winSize.width*0.125+iTag*0.04*m_winSize.width);
    }
    addChild(pLayer,977,997);

}



void CHeroCompoundLayer::heroDetails()
{
    if(m_layer){
        m_layer->removeAllChildren();
        m_skill.clear();
    }
    if(m_InfoBg)
    {
        m_InfoBg->setVisible(true);
    }
    else{
        m_InfoBg = Sprite::createWithSpriteFrameName("onebei.png");
        m_bg->addChild(m_InfoBg);
        m_InfoBg->setPosition(m_bg->getContentSize().width * 0.8, m_bg->getContentSize().height * 0.465);
        m_InfoBg->setScaleX(8.9);
        m_InfoBg->setScaleY(9.1);
    }
    
    
    m_curShow = 1;
    char buf[128];
    int attkNum = 0;
    int hp = 0;
    int walkSpeed = 0;
    int rare= 0;
    std::vector<int> allSkill;//保存怪物拥有的技能
    //获取选中英雄的基本信息
    monsterInfo* pMonsterInfo = CDataManager::getMonsterInfoByMonsterId(m_selectedHeroId);
    //判断当前用户是否已有该英雄
    std::vector<FighterInfo*> userMonterVec = CPersonalData::getInstance()->m_vecMyFighterInfo;
    int i = 0;
    for(;i < userMonterVec.size(); i++)
    {
        if(userMonterVec.at(i)->monsterId == m_selectedHeroId)
        {
            break;
        }
    }
    if(i < userMonterVec.size())//用户拥有该怪物
    {
        attkNum = userMonterVec.at(i)->attk;
        hp = userMonterVec.at(i)->maxHP;
        walkSpeed = userMonterVec.at(i)->walkSpeed * 10;
        rare = userMonterVec.at(i)->rare;
        if(rare > 0)
        {
            MonsterAllEvolutionInfo pEvoInfoBefore = CDataManager::getAllEvolutionInfoByRare(rare);
            attkNum += pEvoInfoBefore.m_allAddAtk;
            hp += pEvoInfoBefore.m_allAddHP;
        }
    }
    else{
        attkNum = pMonsterInfo->minAtk;
        hp = pMonsterInfo->minHp;
        walkSpeed = pMonsterInfo->walkSpeed * 10;
    }
    
    m_skill = CDataManager::getAvailableSkillInfo(m_selectedHeroId, rare);
    allSkill = pMonsterInfo->allSkill;//英雄的全部技能
    
    Size layerSize = m_layer->getContentSize();
    Label* pTitle = Label::createWithSystemFont("英雄介绍", "Arial", 30);
    pTitle->setColor(Color3B::YELLOW);
    m_layer->addChild(pTitle);
    pTitle->setPosition(Vec2(layerSize.width / 2, layerSize.height - pTitle->getContentSize().height));
    
    Sprite* leftDecorte = Sprite::createWithSpriteFrameName("decorate.png");
    leftDecorte->setFlippedX(true);
    leftDecorte->setAnchorPoint(Vec2(1, 0.5));
    m_layer->addChild(leftDecorte);
    leftDecorte->setPosition(Vec2(pTitle->getPositionX() - pTitle->getContentSize().width * 0.55, pTitle->getPositionY()));
    
    Sprite* rightDecorte = Sprite::createWithSpriteFrameName("decorate.png");
    rightDecorte->setAnchorPoint(Vec2(0, 0.5));
    m_layer->addChild(rightDecorte);
    rightDecorte->setPosition(Vec2(pTitle->getPositionX() + pTitle->getContentSize().width * 0.55, pTitle->getPositionY()));
    
    Label* pIntroduce = Label::createWithSystemFont(pMonsterInfo->monsterIntroduce.c_str(), "Arial", 22, Size(340, 100));
    m_layer->addChild(pIntroduce);
    pIntroduce->setPosition(Vec2(layerSize.width / 2, pTitle->getPositionY() - pTitle->getContentSize().height - 45));
    
    //英雄属性
    Label* pAttribute = Label::createWithSystemFont("英雄属性", "Arial", 30);
    pAttribute->setColor(Color3B::YELLOW);
    m_layer->addChild(pAttribute);
    pAttribute->setPosition(Vec2(layerSize.width / 2, layerSize.height * 0.65));
    
    Sprite* leftMidDecorte = Sprite::createWithSpriteFrameName("decorate.png");
    leftMidDecorte->setFlippedX(true);
    leftMidDecorte->setAnchorPoint(Vec2(1, 0.5));
    m_layer->addChild(leftMidDecorte);
    leftMidDecorte->setPosition(Vec2(pAttribute->getPositionX() - pAttribute->getContentSize().width * 0.55, pAttribute->getPositionY()));
    
    Sprite* rightMidDecorte = Sprite::createWithSpriteFrameName("decorate.png");
    rightMidDecorte->setAnchorPoint(Vec2(0, 0.5));
    m_layer->addChild(rightMidDecorte);
    rightMidDecorte->setPosition(Vec2(pAttribute->getPositionX() + pAttribute->getContentSize().width * 0.55, pAttribute->getPositionY()));
    
    
    int m = 0;
    float prop = 0.575f;
    Label* pSkillLeft = NULL;
    
    if(rare == 0 || rare >= 5)
    {
        Sprite* starBg = Sprite::createWithSpriteFrameName("advance_bk.png");
        m_layer->addChild(starBg);
        starBg->setPosition(Vec2(layerSize.width / 2, layerSize.height * 0.57));
        cocos2d::Size bgSize = starBg->getBoundingBox().size;
        for(int i = 0; i < 5; i++)
        {
            Vec2 pos = Vec2(bgSize.width * 0.12f + 25*i, bgSize.height * 0.5f);
            Sprite* star = Sprite::createWithSpriteFrameName("advance_gray_star.png");
            if(((i == 0 && rare == 0)) || (rare == 5))
            {
                star = Sprite::createWithSpriteFrameName("advance_star.png");
            }
            starBg->addChild(star);
            star->setPosition(pos);
        }
        //详细属性
        Label* pAttack1 = Label::createWithSystemFont("攻击力:", "Arial", 26);
        m_layer->addChild(pAttack1);
        pAttack1->setColor(Color3B(234, 215, 162));
        pAttack1->setAnchorPoint(Vec2(0, 0.5));
        pAttack1->setPosition(Vec2(layerSize.width * 0.34f, starBg->getPositionY() - starBg->getContentSize().height - 15));
        sprintf(buf, "%d", attkNum);
        Label* pAttack1Num = Label::createWithSystemFont(buf, "Arial", 26);
        m_layer->addChild(pAttack1Num);
        pAttack1Num->setColor(Color3B(230, 116, 32));
        pAttack1Num->setAnchorPoint(Vec2(0, 0.5));
        pAttack1Num->setPosition(Vec2(layerSize.width * 0.6f, pAttack1->getPositionY()));
        
        Label* pArmor1 = Label::createWithSystemFont("血量:", "Arial", 26);
        m_layer->addChild(pArmor1);
        pArmor1->setColor(Color3B(234, 215, 162));
        pArmor1->setAnchorPoint(Vec2(0, 0.5));
        pArmor1->setPosition(Vec2(layerSize.width * 0.34f, pAttack1->getPositionY() - pAttack1->getContentSize().height - 10));
        sprintf(buf, "%d", hp);
        Label* pArmor1Num = Label::createWithSystemFont(buf, "Arial", 26);
        m_layer->addChild(pArmor1Num);
        pArmor1Num->setColor(Color3B(230, 116, 32));
        pArmor1Num->setAnchorPoint(Vec2(0, 0.5));
        pArmor1Num->setPosition(Vec2(layerSize.width * 0.6f, pArmor1->getPositionY()));
        
        Label* pReply1 = Label::createWithSystemFont("移速:", "Arial", 26);
        m_layer->addChild(pReply1);
        pReply1->setColor(Color3B(234, 215, 162));
        pReply1->setAnchorPoint(Vec2(0, 0.5));
        pReply1->setPosition(Vec2(layerSize.width * 0.34f, pArmor1->getPositionY() - pArmor1->getContentSize().height - 10));
        sprintf(buf, "%d", walkSpeed);
        Label* pReply1Num = Label::createWithSystemFont(buf, "Arial", 26);
        m_layer->addChild(pReply1Num);
        pReply1Num->setColor(Color3B(230, 116, 32));
        pReply1Num->setAnchorPoint(Vec2(0, 0.5));
        pReply1Num->setPosition(Vec2(layerSize.width * 0.6f, pReply1->getPositionY()));
        
        pSkillLeft = Label::createWithSystemFont("技能", "Arial", 26);
        m_layer->addChild(pSkillLeft);
        pSkillLeft->setColor(Color3B(234, 215, 162));
        pSkillLeft->setAnchorPoint(Vec2(0, 0.5));
        pSkillLeft->setPosition(Vec2(layerSize.width * 0.34f, pReply1->getPositionY() - pReply1->getContentSize().height - 10));
        
        //显示技能
        for(m = 0; m < allSkill.size(); m++)
        {
            Sprite* pSkill = NULL;
            Color3B clr = Color3B(80,80,80);
            if(m < m_skill.size()){
                clr = Color3B::WHITE;
                skillInfo* pSkillInfo = CDataManager::getTheSkillInfoWithId(allSkill.at(m));
                pSkill = CDataManager::getSkillIcoWithId(pSkillInfo->skillIcon, 0.6f, clr);
               // m_layer->addChild(pSkill);
            
            }
            else{
                if(m == 0 && rare == 0)
                {
                    clr = Color3B::WHITE;
                }
                skillInfo* pInfo = CDataManager::getTheSkillInfoWithId(allSkill.at(m));
                pSkill = CDataManager::getSkillIcoWithId(pInfo->skillIcon, 0.6f, clr);
               // m_layer->addChild(pSkill);
            }
            //CC_CALLBACK_1(CArmLvUpLayer::OnMinus, this));
            
            MenuItemSprite *pSkillItem = MenuItemSprite::create(pSkill,NULL,CC_CALLBACK_1(CHeroCompoundLayer::OnSkillTouch, this));
            pSkillItem->setTag(m);
            //int i = 1;
            
            //pSkillItem->setUserData((void *)i);
            pSkillItem->setPosition(layerSize.width * 0.375f + 50*m, pSkillLeft->getPositionY() - pSkill->getBoundingBox().size.height * 1.2f);
    

            pSkillItem->setScale(1.3f);
            
            m_pSKillMenu = Menu::create(pSkillItem,NULL);
            m_pSKillMenu->setPosition(Vec2(0,0));
            m_layer->addChild(m_pSKillMenu);
        }
        
    }
    else
    {
        //详细属性
        Sprite* starBg1 = Sprite::createWithSpriteFrameName("advance_bk.png");
        m_layer->addChild(starBg1);
        starBg1->setPosition(Vec2(layerSize.width * 0.22f, layerSize.height * 0.57));
        cocos2d::Size bgSize = starBg1->getBoundingBox().size;
        for(int i = 0; i < 5; i++)
        {
            Vec2 pos = Vec2(bgSize.width * 0.12f + 25*i, bgSize.height * 0.5f);
            Sprite* star = Sprite::createWithSpriteFrameName("advance_gray_star.png");
            if(i < rare)
            {
                star = Sprite::createWithSpriteFrameName("advance_star.png");
            }
            starBg1->addChild(star);
            star->setPosition(pos);
        }
        Label* pAttack1 = Label::createWithSystemFont("攻击力:", "Arial", 24);
        m_layer->addChild(pAttack1);
        pAttack1->setColor(Color3B(234, 215, 162));
        pAttack1->setAnchorPoint(Vec2(0, 0.5));
        pAttack1->setPosition(Vec2(layerSize.width * 0.06f, starBg1->getPositionY() - starBg1->getContentSize().height - 15));
        sprintf(buf, "%d", attkNum);
        Label* pAttack1Num = Label::createWithSystemFont(buf, "Arial", 24);
        m_layer->addChild(pAttack1Num);
        pAttack1Num->setColor(Color3B(234, 215, 162));
        pAttack1Num->setAnchorPoint(Vec2(0, 0.5));
        pAttack1Num->setPosition(Vec2(layerSize.width * 0.31f, pAttack1->getPositionY()));
        Sprite* pAttack1NumArrow = Sprite::createWithSpriteFrameName("advance_prop_arrow.png");
        m_layer->addChild(pAttack1NumArrow);
        pAttack1NumArrow->setPosition(Vec2(layerSize.width * 0.49f, pAttack1Num->getPositionY()));
        pAttack1NumArrow->runAction(RepeatForever::create(Sequence::create(MoveBy::create(0.55f, Vec2(8, 0)),
                                                                           MoveBy::create(0.55f, Vec2(-8, 0)), NULL)));
        
        Label* pArmor1 = Label::createWithSystemFont("血量:", "Arial", 24);
        m_layer->addChild(pArmor1);
        pArmor1->setColor(Color3B(234, 215, 162));
        pArmor1->setAnchorPoint(Vec2(0, 0.5));
        pArmor1->setPosition(Vec2(layerSize.width * 0.06f, pAttack1->getPositionY() - pAttack1->getContentSize().height - 10));
        sprintf(buf, "%d", hp);
        Label* pArmor1Num = Label::createWithSystemFont(buf, "Arial", 24);
        m_layer->addChild(pArmor1Num);
        pArmor1Num->setColor(Color3B(234, 215, 162));
        pArmor1Num->setAnchorPoint(Vec2(0, 0.5));
        pArmor1Num->setPosition(Vec2(layerSize.width * 0.31f, pArmor1->getPositionY()));
        Sprite* pArmor1NumArrow = Sprite::createWithSpriteFrameName("advance_prop_arrow.png");
        m_layer->addChild(pArmor1NumArrow);
        pArmor1NumArrow->setPosition(Vec2(layerSize.width * 0.49f, pArmor1Num->getPositionY()));
        pArmor1NumArrow->runAction(RepeatForever::create(Sequence::create(MoveBy::create(0.55f, Vec2(8, 0)), MoveBy::create(0.55f, Vec2(-8, 0)), NULL)));
        
        Label* pReply1 = Label::createWithSystemFont("移速:", "Arial", 24);
        m_layer->addChild(pReply1);
        pReply1->setColor(Color3B(234, 215, 162));
        pReply1->setAnchorPoint(Vec2(0, 0.5));
        pReply1->setPosition(Vec2(layerSize.width * 0.06f, pArmor1->getPositionY() - pArmor1->getContentSize().height - 10));
        sprintf(buf, "%d", walkSpeed);
        Label* pReply1Num = Label::createWithSystemFont(buf, "Arial", 24);
        m_layer->addChild(pReply1Num);
        pReply1Num->setColor(Color3B(234, 215, 162));
        pReply1Num->setAnchorPoint(Vec2(0, 0.5));
        pReply1Num->setPosition(Vec2(layerSize.width * 0.31f, pReply1->getPositionY()));
        Sprite* pReply1NumArrow = Sprite::createWithSpriteFrameName("advance_prop_arrow.png");
        m_layer->addChild(pReply1NumArrow);
        pReply1NumArrow->setPosition(Vec2(layerSize.width * 0.49f, pReply1Num->getPositionY()));
        pReply1NumArrow->runAction(RepeatForever::create(Sequence::create(MoveBy::create(0.55f, Vec2(8, 0)), MoveBy::create(0.55f, Vec2(-8, 0)), NULL)));
        
        pSkillLeft = Label::createWithSystemFont("技能", "Arial", 24);
        m_layer->addChild(pSkillLeft);
        pSkillLeft->setColor(Color3B(234, 215, 162));
        pSkillLeft->setAnchorPoint(Vec2(0, 0.5));
        pSkillLeft->setPosition(Vec2(layerSize.width * 0.06f, pReply1->getPositionY() - pReply1->getContentSize().height - 10));
        //显示技能
        for(m = 0; m < allSkill.size(); m++)
        {
            Sprite* pSkill = NULL;
            Color3B clr = Color3B(80,80,80);
            if(m < m_skill.size()){
                clr = Color3B::WHITE;
                skillInfo* pSkillInfo = CDataManager::getTheSkillInfoWithId(allSkill.at(m));
                pSkill = CDataManager::getSkillIcoWithId(pSkillInfo->skillIcon, 0.6f, clr);
                //m_layer->addChild(pSkill);
            }
            else{
                skillInfo* pInfo = CDataManager::getTheSkillInfoWithId(allSkill.at(m));
                pSkill = CDataManager::getSkillIcoWithId(pInfo->skillIcon, 0.6f, clr);
                //m_layer->addChild(pSkill);
            }
            //pSkill->setScale(1.3f);
            //pSkill->setPosition(Vec2(layerSize.width * 0.112f + 50*m, pSkillLeft->getPositionY() - pSkill->getBoundingBox().size.height * 1.2f));
            MenuItemSprite *pSkillItem = MenuItemSprite::create(pSkill,NULL,CC_CALLBACK_1(CHeroCompoundLayer::OnSkillTouch, this));
            pSkillItem->setTag(m);
            
            int i = 1;
            pSkillItem->setUserData((void *)i);
            
            //pSkillItem->setPosition(Vec2(layerSize.width * 0.112f + 50*m, pSkillLeft->getPositionY() - pSkill->getBoundingBox().size.height * 1.2f));
            pSkillItem->setPosition(Vec2(layerSize.width * 0.25f + 50*(m%2),
                                         pSkillLeft->getPositionY() - pSkill->getBoundingBox().size.height * 0.4f - 50*(m/2)));
            pSkillItem->setScale(1.3f);
            
            m_pSKillMenu = Menu::create(pSkillItem,NULL);
            m_pSKillMenu->setPosition(Vec2(0,0));
            m_layer->addChild(m_pSKillMenu);
            
        }
        //箭头
        Sprite* advanceArrow = Sprite::createWithSpriteFrameName("advance_star_arrow.png");
        m_layer->addChild(advanceArrow);
        advanceArrow->setPosition(Vec2(layerSize.width * 0.485f, layerSize.height * 0.57));
        
        //进阶后属性
        Sprite* starBg2 = Sprite::createWithSpriteFrameName("advance_bk.png");
        m_layer->addChild(starBg2);
        starBg2->setPosition(Vec2(layerSize.width * 0.76f, layerSize.height * 0.57));
        for(int i = 0; i < 5; i++)
        {
            Vec2 pos = Vec2(bgSize.width * 0.12f + 25*i, bgSize.height * 0.5f);
            Sprite* star = Sprite::createWithSpriteFrameName("advance_gray_star.png");
            if(i < rare + 1)
            {
                star = Sprite::createWithSpriteFrameName("advance_star.png");
            }
            starBg2->addChild(star);
            star->setPosition(pos);
        }
        
        MonsterEvolutionInfo* pEvoInfoArter = CDataManager::getMonsterEvolutionById(rare + 1);
        m_skill = CDataManager::getAvailableSkillInfo(m_selectedHeroId, rare + 1);
        
        Label* pAttack2 = Label::createWithSystemFont("攻击力:", "Arial", 24);
        m_layer->addChild(pAttack2);
        pAttack2->setColor(Color3B(234, 215, 162));
        pAttack2->setAnchorPoint(Vec2(0, 0.5));
        pAttack2->setPosition(Vec2(layerSize.width * 0.59, starBg2->getPositionY() - starBg2->getContentSize().height - 15));
        sprintf(buf, " %d", attkNum + pEvoInfoArter->m_addAtk);
        Label* pAttack2Num = Label::createWithSystemFont(buf, "Arial", 24);
        m_layer->addChild(pAttack2Num);
        pAttack2Num->setColor(Color3B(230, 116, 32));
        pAttack2Num->setAnchorPoint(Vec2(0, 0.5));
        pAttack2Num->setPosition(Vec2(layerSize.width * 0.8f, pAttack1->getPositionY()));
        
        Label* pArmor2 = Label::createWithSystemFont("血量:", "Arial", 24);
        m_layer->addChild(pArmor2);
        pArmor2->setColor(Color3B(234, 215, 162));
        pArmor2->setAnchorPoint(Vec2(0, 0.5));
        pArmor2->setPosition(Vec2(layerSize.width * 0.59, pAttack2->getPositionY() - pAttack2->getContentSize().height - 10));
        sprintf(buf, " %d", hp + pEvoInfoArter->m_addHp);
        Label* pArmor2Num = Label::createWithSystemFont(buf, "Arial", 24);
        m_layer->addChild(pArmor2Num);
        pArmor2Num->setColor(Color3B(230, 116, 32));
        pArmor2Num->setAnchorPoint(Vec2(0, 0.5));
        pArmor2Num->setPosition(Vec2(layerSize.width * 0.8f, pArmor2->getPositionY()));
        
        Label* pReply2 = Label::createWithSystemFont("移速:", "Arial", 24);
        m_layer->addChild(pReply2);
        pReply2->setColor(Color3B(234, 215, 162));
        pReply2->setAnchorPoint(Vec2(0, 0.5));
        pReply2->setPosition(Vec2(layerSize.width * 0.59, pArmor2->getPositionY() - pArmor2->getContentSize().height - 10));
        sprintf(buf, " %d", walkSpeed);
        Label* pReply2Num = Label::createWithSystemFont(buf, "Arial", 24);
        m_layer->addChild(pReply2Num);
        pReply2Num->setColor(Color3B(230, 116, 32));
        pReply2Num->setAnchorPoint(Vec2(0, 0.5));
        pReply2Num->setPosition(Vec2(layerSize.width * 0.8f, pReply2->getPositionY()));
        
        Label* pSkill2 = Label::createWithSystemFont("技能", "Arial", 24);
        m_layer->addChild(pSkill2);
        pSkill2->setColor(Color3B(234, 215, 162));
        pSkill2->setAnchorPoint(Vec2(0, 0.5));
        pSkill2->setPosition(Vec2(layerSize.width * 0.59, pReply2->getPositionY() - pReply2->getContentSize().height - 10));
        
        //显示技能
        for(m = 0; m < allSkill.size(); m++)
        {
            Sprite* pSkill = NULL;
            Color3B clr = Color3B(80,80,80);
            if(m < m_skill.size()){
                clr = Color3B::WHITE;
                skillInfo* pSkillInfo = CDataManager::getTheSkillInfoWithId(allSkill.at(m));
                pSkill = CDataManager::getSkillIcoWithId(pSkillInfo->skillIcon,0.6f, clr);
                //m_layer->addChild(pSkill);
            }
            else{
                skillInfo* pInfo = CDataManager::getTheSkillInfoWithId(allSkill.at(m));
                pSkill = CDataManager::getSkillIcoWithId(pInfo->skillIcon,0.6f, clr);
                //m_layer->addChild(pSkill);
            }
            
            MenuItemSprite *pSkillItem = MenuItemSprite::create(pSkill,NULL,CC_CALLBACK_1(CHeroCompoundLayer::OnSkillTouch, this));
            pSkillItem->setTag(m);
            
            pSkillItem->setPosition(Vec2(layerSize.width * 0.77f + 50*(m%2),
                                         pSkillLeft->getPositionY() - pSkill->getBoundingBox().size.height * 0.4f - 50*(m/2)));
            pSkillItem->setScale(1.3f);
            
            m_pSKillMenu = Menu::create(pSkillItem,NULL);
            m_pSKillMenu->setPosition(Vec2(0,0));
            m_layer->addChild(m_pSKillMenu);
        }
        
        
    }
    //返回
    Sprite* normal = Sprite::createWithSpriteFrameName("bluean.png");
    Sprite* select = Sprite::createWithSpriteFrameName("bluean.png");
    select->setColor(Color3B::GRAY);
    LYMenuItemSprite* backItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(CHeroCompoundLayer::btnCallBack, this));
    Menu* pBackMenu = Menu::create(backItem, NULL);
    backItem->setScale(1.1f);
    backItem->setTag(BACK_HERO_INFO_TAG);
    m_layer->addChild(pBackMenu);
    pBackMenu->setPosition(Vec2(layerSize.width * 0.23, normal->getContentSize().height * 0.25f));
    Label* pBackLabel = Label::createWithSystemFont("返回", "Arial", 22);
    backItem->addChild(pBackLabel);
    pBackLabel->setPosition(Vec2(normal->getContentSize().width / 2, normal->getContentSize().height / 2));
}

void CHeroCompoundLayer::updateHeroShow()
{
    
    if (m_node) {
        m_node->removeFromParent();
        m_node = NULL;
    }
    Size bgSize = m_bg->getContentSize();
    m_node = Node::create();
    m_bg->addChild(m_node, 1);
    m_node->setPosition(Vec2::ZERO);
    
    m_pMenu = Menu::create();
    m_node->addChild(m_pMenu, 3);
    m_pMenu->setPosition(Vec2::ZERO);
    //m_sortVec.clear();
    
    Sprite* bgColor = Sprite::createWithSpriteFrameName("zhezhao.png");
    m_bg->addChild(bgColor);
    bgColor->setOpacity(225);
    bgColor->setScaleX(7.6);
    bgColor->setScaleY(10.0);
    bgColor->setPosition(m_bg->getContentSize().width * 0.295, m_bg->getContentSize().height * 0.47f);
    
    char buf[128];
    std::string headIcon[5]={"white_bg.png","green_bg.png","blue_bg.png", "purple_bg.png","gold_bg.png"};
    for(int i =  (m_page - 1)*HERO_NUM; (i < m_page * HERO_NUM)&&(i < m_sortVec.size()) ; i++)
    {
        int index = m_sortVec.at(i);
        int rare = CDataManager::getMonsterStarById(index);
        
        if(rare > 0)
        {
            rare -= 1;
        }
        Point pos = Vec2(bgSize.width * 0.087 + i%HERO_NUM%4*140, bgSize.height*0.792 - i%HERO_NUM/4*165);
        addHeroInfo(headIcon[rare].c_str(), headIcon[rare].c_str(), pos, m_pMenu, index);
        
        monsterInfo* info = CDataManager::getMonsterInfoByMonsterId(index);
        //添加名字
        if(info)
        {
            Label* pName = Label::createWithSystemFont(info->monsterName.c_str(), "Arial", 20);
            m_node->addChild(pName, 5);
            pName->setPosition(Vec2(pos.x, pos.y - 83));
        }
    }
    if (m_page == m_maxPage) {
        if (m_rightItem) {
            m_rightItem->setEnabled(false);
            m_rightItem->setColor(Color3B::GRAY);
        }
        if (m_leftItem) {
            m_leftItem->setEnabled(true);
            m_leftItem->setColor(Color3B::WHITE);
        }
        
    }
    else if(m_page == 1){
        if (m_leftItem) {
            m_leftItem->setEnabled(false);
            m_leftItem->setColor(Color3B::GRAY);
        }
        if (m_rightItem) {
            m_rightItem->setEnabled(true);
            m_rightItem->setColor(Color3B::WHITE);
        }
    }
    else{
        if (m_leftItem && m_rightItem) {
            m_rightItem->setEnabled(true);
            m_rightItem->setColor(Color3B::WHITE);
            m_leftItem->setEnabled(true);
            m_leftItem->setColor(Color3B::WHITE);
        }
    }
    sprintf(buf, "%d/%d", m_page, m_maxPage);
    m_pageLabel->setString(buf);
    
    Sprite *pLine = Sprite::createWithSpriteFrameName("split_line.png");
    pLine->setPosition(Vec2(bgSize.width*0.595,bgSize.height*0.48));
    m_bg->addChild(pLine,1);
    
    if (m_curShow == 1) {
        heroDetails();
    }
    else{
        adjustHeroInfo();
    }
}

void CHeroCompoundLayer::addSpriteByImg(Sprite* parent, const char* img)
{
    Sprite* sp = Sprite::createWithSpriteFrameName(img);
    parent->addChild(sp);;
    sp->setColor(parent->getColor());
    sp->setPosition(Vec2(parent->getContentSize().width / 2, parent->getContentSize().height / 2));
}


void CHeroCompoundLayer::sortHero()
{
    int avaliableNum = 0;
    int rare = 0;
    int index = 0;
    
    for(int i = 0; i < m_allMonster.size(); i++)
    {
        bool isExisted = false;
        bool isFirst = true;//五星级无材料的英雄只保存遇见的第一个
        for(int j = 0; j < m_allMonster.size() ; j++)
        {
            for(int m = 0; m < m_sortVec.size(); m++)
            {
                if(m_allMonster.at(j) == m_sortVec.at(m))
                {
                    isExisted = true;
                    break;
                }
                
            }
            if(!isExisted)
            {
                if(rare < CDataManager::getMonsterStarById(m_allMonster[j]))
                {
                    rare = CDataManager::getMonsterStarById(m_allMonster[j]);
                    avaliableNum = CDataManager::getDebrisNumById(m_allMonster[j]);
                    index = j;
                }
                else if(rare == CDataManager::getMonsterStarById(m_allMonster[j]))
                {
                    if(avaliableNum < CDataManager::getDebrisNumById(m_allMonster[j]))
                    {
                        avaliableNum = CDataManager::getDebrisNumById(m_allMonster[j]);
                        index = j;
                    }
                    else{
                        if((avaliableNum == 0) && (rare == 0) && isFirst)
                        {
                            isFirst = false;
                            index = j;
                        }
                    }
                }
            }
            else{
                isExisted = false;
            }
        }
        isFirst = true;
        if(!isExisted){
            m_sortVec.push_back(m_allMonster[index]);
            rare = 0;
            avaliableNum = 0;
            index = 0;
        }
        else{
            isExisted = false;
        }
    }
}

void CHeroCompoundLayer::addHeroInfo(const char* normalImg, const char* selectImg, Point pos, Node* parent, int tag)
{
    char buf[128];
    Sprite* normal = Sprite::createWithSpriteFrameName(normalImg);
    Sprite* select = Sprite::createWithSpriteFrameName(selectImg);
    LYMenuItemSprite* item = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(CHeroCompoundLayer::btnCallBack, this));
    item->setScale(3.75f);
    item->setPosition(pos);
    if (tag == m_sortVec.at(0)) {
        if(m_selectedHeroId  == 0 || m_selectedHeroId == tag){
            Sprite* kuang = Sprite::createWithSpriteFrameName("selected.png");
            m_node->addChild(kuang, 1, HERO_ELECTED_KUANG);
            kuang->setScale(4.15f);
            kuang->setPosition(pos);
            m_selectedHeroId = tag;
        }
    }
    else if((tag == m_selectedHeroId) && (!m_node->getChildByTag(HERO_ELECTED_KUANG))){
        Sprite* kuang = Sprite::createWithSpriteFrameName("selected.png");
        m_node->addChild(kuang, 1, HERO_ELECTED_KUANG);
        kuang->setScale(4.15f);
        kuang->setPosition(pos);
    }
    item->setColor(Color3B(100,100,100));
    item->setTag(tag);
    parent->addChild(item);
    
    if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SOLIDER_COMPOUND) && (tag == 12))
    {
        m_guidePos = item->convertToWorldSpaceAR(Vec2::ZERO);
    }
    //添加英雄头像
    monsterInfo* pInfo= CDataManager::getMonsterInfoByMonsterId(tag);
    Sprite* icon = NULL;
    if(!pInfo)
    {
        icon = Sprite::createWithSpriteFrameName("not_UnLock.png");
    }
    else{
        icon = Sprite::createWithSpriteFrameName(pInfo->monsterIcon.c_str());
    }
    item->addChild(icon);
    icon->setScale(0.26);
    icon->setPosition(Vec2(normal->getContentSize().width / 2, normal->getContentSize().height / 2));
    
    if(pInfo)
    {
        Size  tSoldierSize = icon->getContentSize();
        
        Sprite * pType = CDBMgr::getInstance()->GetShareSpriteByFigtherType(pInfo->monsterType);
        pType->setScale(1);
        pType->setPosition(tSoldierSize.width*0.2,tSoldierSize.height*0.9);
        icon->addChild(pType);
        //获取当前用户拥有的碎片
        int avaliableNum = CDataManager::getDebrisNumById(tag);
        int rare = CDataManager::getMonsterStarById(tag);
        if(rare < 5)
        {
            //添加进度条
            Sprite* progressBg = Sprite::createWithSpriteFrameName("barBg.png");
            m_node->addChild(progressBg, 5);
            progressBg->setPosition(Vec2(pos.x, pos.y - normal->getContentSize().height * item->getScale() * 0.35));
            
            Sprite* progresSp = Sprite::createWithSpriteFrameName("redBar.png");
            ProgressTimer* progress = ProgressTimer::create(progresSp);
            m_node->addChild(progress, 5);
            progress->setBarChangeRate(Point(1,0));
            progress->setType(ProgressTimer::Type::BAR);
            progress->setMidpoint(Point(0,1));
            progress->setPosition(progressBg->getPosition());
            
            int percent = avaliableNum*100/CDataManager::getMonsterEvolutionById(rare + 1)->m_number;
            progress->setPercentage(percent);
            
            if(avaliableNum >= CDataManager::getMonsterEvolutionById(rare + 1)->m_number)
            {
                progress->getSprite()->setSpriteFrame("greenBar.png");
            }
            //根据星级获取碎片数
            sprintf(buf, "%d/%d", avaliableNum, CDataManager::getMonsterEvolutionById(rare + 1)->m_number);
            
            Label* pLabel = Label::createWithSystemFont(buf, "Arial", 18);
            m_node->addChild(pLabel, 5);
            pLabel->setPosition(progressBg->getPosition());
        }
        if(rare > 0){
            item->setColor(Color3B(255,255,255));
        }
        for(int i = 0;i < rare; i++)
        {
            Sprite* star = Sprite::createWithSpriteFrameName("advance_star.png");
            item->addChild(star, 5);
            star->setScale(0.26);
            if(rare == 1){
                star->setPosition(Vec2(normal->getContentSize().width / 2, normal->getContentSize().height * 0.96f));
            }
            else if(rare == 2){
                star->setPosition(Vec2(normal->getContentSize().width * 0.38f + i*8 , normal->getContentSize().height * 0.96f));
            }
            else if(rare == 3){
                star->setPosition(Vec2(normal->getContentSize().width * 0.32f + i*6 , normal->getContentSize().height * 0.96f));
            }
            else if(rare == 4){
                star->setPosition(Vec2(normal->getContentSize().width * 0.235 + i*6 , normal->getContentSize().height * 0.96f));
            }
            else if(rare == 5){
                star->setPosition(Vec2(normal->getContentSize().width * 0.2 + i*5 , normal->getContentSize().height * 0.96f));
            }
        }
    }
    
}

void CHeroCompoundLayer::addCloseGuide()
{
    if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SOLIDER_COMPOUND) && (guiderManagerLayer::getInstance()->getStepNum() == 6))
    {
        Vec2 pos = m_exitItem->convertToWorldSpaceAR(Vec2::ZERO);
        guiderManagerLayer::getInstance()->createLayer(this, GUIDE_SHOW_FINGER, pos);
        if(m_hero)
        {
            m_hero->setVisible(true);
        }
    }
}

void CHeroCompoundLayer::showHero()
{
    
    if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SOLIDER_COMPOUND) && (guiderManagerLayer::getInstance()->getStepNum() == 4))
    {
        guiderManagerLayer::getInstance()->setStepNum(guiderManagerLayer::getInstance()->getStepNum() + 1);
        guiderManagerLayer::getInstance()->createLayer(this);
    }
    else
    {
        if(m_hero)
        {
            m_hero->setVisible(true);
        }
    }
}

void CHeroCompoundLayer::btnCallBack(Ref* pSender)
{
    CSKillTouchDetail *pNode = ( CSKillTouchDetail *)getChildByTag(997);
    
    if(pNode)
    {
        pNode->removeFromParent();
    }
    
    int tag = ((Node*)pSender)->getTag();
    if ((LEFT_ARROW_TAG == tag) || (RIGHT_ARROW_TAG == tag)) {
        if(LEFT_ARROW_TAG == tag)
        {
            m_page--;
            updateHeroShow();
        }
        else{
            m_page++;
            updateHeroShow();
        }
    }
    else{
        if(EXIT_TAG == tag)
        {
            CPersonalData::getInstance()->setrestartPlayMusic(false);
            NotificationCenter::getInstance()->postNotification("drawCardAfter");
            removeFromParent();
        }
        else if(HREO_INFO_TAG == tag)
        {
            heroDetails();
        }
        else if(BACK_HERO_INFO_TAG == tag)
        {
            adjustHeroInfo();
        }
        else if(BUY_DEBRIS_TAG == tag){
            
            CommonPropLayer* layer = CommonPropLayer::create(DEBRIS_BUY_TYPE, m_selectedHeroId,true);
            addChild(layer, 100);
            if(m_hero)
            {
                m_hero->setVisible(false);
            }
            
        }
        else if(ADVANCE_START_TAG == tag){
            int avaliableNum = CDataManager::getDebrisNumById(m_selectedHeroId);
            int rare = CDataManager::getMonsterStarById(m_selectedHeroId);
            if(avaliableNum < CDataManager::getMonsterEvolutionById(rare + 1)->m_number)
            {
                SimpleAudioEngine::getInstance()->playEffect("music/noBtn.mp3");
                LYMenuItemSprite* temp = (LYMenuItemSprite*)(m_otherMenu->getChildByTag(ADVANCE_START_TAG));
                Vec2 pos = m_layer->convertToWorldSpaceAR(temp->getPosition());
                pos.add(Vec2(-m_layer->getContentSize().width / 2, -m_layer->getContentSize().height / 2));
                Sprite * thetarget = CFightManager::getTheFlyWord("碎片不足","fonts/cuti.ttf");
                thetarget->setPosition(pos);
                addChild(thetarget, 10);
                
            }
            else{
                int rare = CDataManager::getMonsterStarById(m_selectedHeroId);
                int coinNum = CDataManager::getMonsterEvolutionById(rare + 1)->m_coinNum;
                if(!((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SOLIDER_COMPOUND) && (guiderManagerLayer::getInstance()->getStepNum() == 4)))
                {
                    if(CPersonalData::getInstance()->getUserCoin() < coinNum){
                        SimpleAudioEngine::getInstance()->playEffect("music/noBtn.mp3");
                        LYMenuItemSprite* temp = (LYMenuItemSprite*)(m_otherMenu->getChildByTag(ADVANCE_START_TAG));
                        
                        Vec2 pos = m_layer->convertToWorldSpaceAR(temp->getPosition());
                        pos.add(Vec2(-m_layer->getContentSize().width / 2, -m_layer->getContentSize().height / 2));
                        Sprite * thetarget = CFightManager::getTheFlyWord("金币不足","fonts/cuti.ttf");
                        thetarget->setPosition(pos);
                        addChild(thetarget, 10);
                        return;
                    }
                }
                if(m_hero)
                {
                    m_hero->setVisible(false);
                }
                //保存玩家金币
                //CDataManager::saveUserStoneAndCoinToDB(coinNum, 0);
                if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SOLIDER_COMPOUND) && (guiderManagerLayer::getInstance()->getStepNum() == 4))
                {
                    guiderManagerLayer::getInstance()->removeGuiderLayer();
                    CDataManager::saveGuideInfoToDB(GUIDE_SOLIDER_COMPOUND);
                    OnUpdateCoinAndStone(NULL);
                }
                else{
                    CNetWorkDataSingle::GetSingletonPtr()->SaveUserMCF(this,menu_selector(CHeroCompoundLayer::OnUpdateCoinAndStone),
                                                                       -coinNum,0,0);
                }
            }
        }
        else{
            //显示选中英雄的信息
            if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SOLIDER_COMPOUND) && (guiderManagerLayer::getInstance()->getStepNum() == 3))
            {
                if(tag != 12)//放置点击其他英雄碎片
                {
                    return;
                }
            }
            m_selectedHeroId = tag;
            if(m_selectedHeroId > m_validMonsterNum)
            {
                Sprite * thetarget = CFightManager::getTheFlyWord("未解锁","fonts/cuti.ttf");
                thetarget->setPosition(Vec2(m_winSize.width * 0.5f, m_winSize.height * 0.5f));
                addChild(thetarget, 10);
                return;
            }
            else
            {
                for(int i = 0; i < m_allMonster.size() ; i++)
                {
                    LYMenuItemSprite* temp = (LYMenuItemSprite*)(m_pMenu->getChildByTag(i + 1));
                    if(temp){
                        if((i + 1) == tag)
                        {
                            m_node->removeChildByTag(HERO_ELECTED_KUANG);
                            Sprite* kuang = Sprite::createWithSpriteFrameName("selected.png");
                            kuang->setColor(Color3B::WHITE);//判断当前英雄是否有星级
                            kuang->setScale(4.15f);
                            m_node->addChild(kuang, 1, HERO_ELECTED_KUANG);
                            kuang->setPosition(temp->getPosition());
                        }
                    }
                }
                if (m_curShow == 1) {
                    heroDetails();
                }
                else{
                    adjustHeroInfo();
                }
                if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SOLIDER_COMPOUND) && (guiderManagerLayer::getInstance()->getStepNum() == 3))
                {
                    guiderManagerLayer::getInstance()->setStepNum(guiderManagerLayer::getInstance()->getStepNum() + 1);
                    LYMenuItemSprite* temp = (LYMenuItemSprite*)m_otherMenu->getChildByTag(ADVANCE_START_TAG);
                    Vec2 pos = temp->convertToWorldSpaceAR(Vec2::ZERO);
                    guiderManagerLayer::getInstance()->createLayer(this, GUIDE_SHOW_FINGER, pos);
                }
            }
        }
    }
}

void CHeroCompoundLayer::addGuideInfo()
{
    if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SOLIDER_COMPOUND) && (guiderManagerLayer::getInstance()->getStepNum() == 3))
    {
        guiderManagerLayer::getInstance()->createLayer(this, GUIDE_SHOW_FINGER, m_guidePos);
        m_hero->setVisible(true);
    }
}


void CHeroCompoundLayer::OnUpdateCoinAndStone(Ref *pSender)
{

    int avaliableNum = CDataManager::getDebrisNumById(m_selectedHeroId);
    int rare = CDataManager::getMonsterStarById(m_selectedHeroId);
    int coinNum = CDataManager::getMonsterEvolutionById(rare + 1)->m_coinNum;
    __String* tempStr = __String::createWithFormat("%d", coinNum);
    EventParamMap paramMap;
    
    paramMap.insert(EventParamPair("合成或者进阶花费魔金币",tempStr->getCString()));
    
    TDCCTalkingDataGA::onEvent("合成或者进阶", &paramMap);
    int debrisNum = CDataManager::getMonsterEvolutionById(rare + 1)->m_number;//获取进阶材料数
    CDataManager::saveUserDebrisToDB(m_selectedHeroId, debrisNum, 1);
    CDataManager::saveMonsterStarToDB(m_selectedHeroId, rare + 1);//更新玩家本地数据库怪物星级

    updateHeroShow();//刷新进阶界面
    CPersonalData::getInstance()->setUpdateUserInfo(true);
    CommonPropLayer* layer = CommonPropLayer::create(HERO_ADVANCED_TYPE, m_selectedHeroId,true,1);
    addChild(layer, 999);
    if(m_hero)
    {
        m_hero->setVisible(false);
    }

    
}


void CHeroCompoundLayer::OnSaveBuyResult(Ref *pSender)
{
    //保存购买的碎片
    CDataManager::saveUserDebrisToDB(m_selectedHeroId,m_dibrisNum);
    
    updateHeroShow();//刷新进阶界面
    CMainMenuLayer* parent = (CMainMenuLayer*)getParent();
    //parent->updateUsetInfo();
    CPersonalData::getInstance()->setUpdateUserInfo(true);
}


void CHeroCompoundLayer::saveBuyResult(int stoneNum,int dibrisNum)
{
    //保存魔法石数
    //CDataManager::saveUserStoneAndCoinToDB(stoneNum, 1);
    monsterInfo* pInfo = CDataManager::getMonsterInfoByMonsterId(m_selectedHeroId);
    __String* temp = __String::createWithFormat("获得%d个%s碎片", dibrisNum, pInfo->monsterName.c_str());
    Sprite* sp = CFightManager::getTheFlyWord(temp->getCString());
    sp->setPosition(m_winSize.width*0.5f,m_winSize.height*0.5f);
    addChild(sp, 100);
    
    __String* tempStr = __String::createWithFormat("%d", stoneNum);
    EventParamMap paramMap;
    
    paramMap.insert(EventParamPair("购买碎片花费魔法石",tempStr->getCString()));
    
    TDCCTalkingDataGA::onEvent("购买碎片", &paramMap);
    m_dibrisNum = dibrisNum;
    
    CNetWorkDataSingle::GetSingletonPtr()->SaveUserMCF(this,menu_selector(CHeroCompoundLayer::OnSaveBuyResult),0,-stoneNum,0);
    TDCCItem::onPurchase("购买碎片", 1, stoneNum);
}
