//
//  propGetPathLayer.cpp
//  WarOfGod
//
//  Created by user on 15/5/27.
//
//

#include "propGetPathLayer.h"
#include "DataManager.h"
#include "CSharedPlist.h"
#include "Common.h"
#include "LYPersonalData.h"
#include "LYFightData.h"
#include "AtkMonsterInfo.h"

#define MY_PERSION CPersonalData::getInstance()

propGetPathLayer::propGetPathLayer()
{
    m_iPropId = 0;
    listener = NULL;
    p_menu = NULL;
    m_levelBg = NULL;
    m_levelVec.clear();
    m_selectedLevel = 0;
    m_comNum = 0;
    m_noSelectedLevel = false;
    CSharedPlist::getInstance()->pushPlist("plist/level.plist");
}

propGetPathLayer::~propGetPathLayer()
{
    CSharedPlist::getInstance()->erasePlist("plist/level.plist");
    _eventDispatcher->removeEventListener(listener);
}

propGetPathLayer* propGetPathLayer::create(int iPropId)
{
    propGetPathLayer* pRef = new propGetPathLayer;
    pRef->m_iPropId = iPropId;
    if(pRef && pRef->init())
    {
        pRef->autorelease();
        return pRef;
    }
    else{
        delete pRef;
        pRef = NULL;
        return NULL;
    }
}

bool propGetPathLayer::onTouchBegan(Touch* pTouch, Event* pEvent)
{
    return true;
}

bool propGetPathLayer::init()
{
    if(!CBaseLayer::initWithColor(Color4B(0, 0, 0, 200)))
    {
        return false;
    }
    
    listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(propGetPathLayer::onTouchBegan, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    m_winSize = Director::getInstance()->getWinSize();
    initLevellInfo();
    return true;
}

void propGetPathLayer::initLevellInfo()
{
    m_levelBg = Sprite::createWithSpriteFrameName("common_bg.png");
    addChild(m_levelBg);
    m_levelBg->setPosition(Vec2(m_winSize.width * 0.5f, m_winSize.height * 0.55f));
    Size bgSize = m_levelBg->getBoundingBox().size;
    
    Label* pLabel = Label::createWithTTF("获得途径", "fonts/cuti.ttf", 30);
    m_levelBg->addChild(pLabel);
    pLabel->setPosition(Vec2(bgSize.width * 0.5f, bgSize.height * 0.895f));
    
    m_levelVec = CDataManager::getItemPathById(m_iPropId);
    m_comNum = m_levelVec.size();
    //根据关卡id排序
    int tempvalue= 0;
    for(int i = 0; i < (m_levelVec.size() - 1) && (m_levelVec.size() > 0); i++)
    {
        for(int j = 0; j < m_levelVec.size() - 1 - i; j++)
        {
            if(m_levelVec[j] > m_levelVec[j + 1])
            {
                tempvalue = m_levelVec[j];
                m_levelVec[j] = m_levelVec[j + 1];
                m_levelVec[j + 1] = tempvalue;
            }
        }
    }
    tempvalue= 0;
    std::vector<int> eqliteVec = CDataManager::getItemPathById(m_iPropId, true);
    for(int i = 0; i < (eqliteVec.size() - 1) && (eqliteVec.size() > 0); i++)
    {
        for(int j = 0; j < eqliteVec.size() - 1 - i; j++)
        {
            if(eqliteVec[j] > eqliteVec[j + 1])
            {
                tempvalue = eqliteVec[j];
                eqliteVec[j] = eqliteVec[j + 1];
                eqliteVec[j + 1] = tempvalue;
            }
        }
    }
    if(((m_levelVec.size() ==0) || (m_levelVec[0] > CPersonalData::getInstance()->getUserInfo()->m_iSmallLevel))
       && ((eqliteVec.size() ==0) || (eqliteVec[0] > CPersonalData::getInstance()->getUserInfo()->m_iEqliteSmallLevel)))
    {
        m_noSelectedLevel = true;
    }
    for(int k = 0; k < eqliteVec.size(); k++)
    {
        m_levelVec.push_back(eqliteVec[k]);
    }
    
    p_menu = Menu::create();
    m_levelBg->addChild(p_menu);
    p_menu->setPosition(Vec2::ZERO);
    if(m_levelVec.size() > 0)
    {
        m_selectedLevel = m_levelVec[0];
    }
    
    for(int i = 0; i < m_levelVec.size(); i++)
    {
        Point pos = Vec2(bgSize.width * 0.16f + 145 * (i%5), bgSize.height * 0.635 - (i/5) * 180);
        addLevelBtn(i, p_menu, pos);
    }
    
    refreshLevelInfo();
    
    Sprite* normal = Sprite::createWithSpriteFrameName("closeBtn.png");
    Sprite* select = Sprite::createWithSpriteFrameName("closeBtn.png");
    select->setColor(Color3B::GRAY);
    LYMenuItemSprite* pCloseItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(propGetPathLayer::btnCallBack, this));
    CommonData::enlargeItemSprite(pCloseItem, 2);
    pCloseItem->setTag(PATH_EXIT_TAG);
    Menu* pCloseMenu = Menu::create(pCloseItem, NULL);
    m_levelBg->addChild(pCloseMenu);
    pCloseMenu->setPosition(Vec2(bgSize.width * 0.96, bgSize.height * 0.875f));
}

void propGetPathLayer::refreshLevelInfo()
{
    Size bgSize = m_levelBg->getBoundingBox().size;
    Sprite* normal = Sprite::createWithSpriteFrameName("yellowan.png");
    Sprite* select = Sprite::createWithSpriteFrameName("yellowan.png");
    select->setColor(Color3B::GRAY);
    LYMenuItemSprite* pStartItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(propGetPathLayer::btnCallBack, this));
    pStartItem->setTag(PATH_GAME_START);
    pStartItem->setScale(1.5f);
    m_pStartMenu = Menu::create(pStartItem, NULL);
    m_levelBg->addChild(m_pStartMenu);
    m_pStartMenu->setPosition(Vec2(bgSize.width * 0.5, bgSize.height * 0.13f));
    if(m_noSelectedLevel)
    {
        m_pStartMenu->setEnabled(false);
        m_pStartMenu->setColor(Color3B::GRAY);
    }
    
    char buf[256];
    levelInfo* pSmallLevelInfo = CDataManager::getTheLevelInfoWithId(m_selectedLevel);
    if (CPersonalData::getInstance()->getlevelMode() == 1) {
        sprintf(buf, "%d", pSmallLevelInfo->stamina * 2);
    }
    else{
        sprintf(buf, "%d", pSmallLevelInfo->stamina);
    }
    Label* staminaNum = Label::createWithSystemFont(buf, "Arial", 30);
    pStartItem->addChild(staminaNum);
    staminaNum->setScale(0.55f);
    staminaNum->setColor(Color3B::BLUE);
    staminaNum->setPosition(Vec2(normal->getContentSize().width * 0.18f, normal->getContentSize().height * 0.585));
    
    Sprite* stamina = Sprite::createWithSpriteFrameName("stamina.png");
    stamina->setScale(0.42f);
    pStartItem->addChild(stamina);
    stamina->setPosition(Vec2(normal->getContentSize().width * 0.34f, staminaNum->getPositionY()));
    
    //分割线
    Sprite* line = Sprite::createWithSpriteFrameName("btn_line.png");
    pStartItem->addChild(line);
    line->setScale(0.5f);
    line->setPosition(Vec2(normal->getContentSize().width * 0.47f, staminaNum->getPositionY()));
    
    Label* fightLabel = Label::createWithSystemFont("出战", "Arial", 35);
    pStartItem->addChild(fightLabel);
    fightLabel->setScale(0.5f);
    fightLabel->setColor(Color3B::BLUE);
    fightLabel->setPosition(Vec2(normal->getContentSize().width * 0.68f, line->getPositionY()));
}

void propGetPathLayer::addLevelBtn(int index, Node *parent, Vec2 pos)
{
    int levelId = m_levelVec[index];
    Sprite* normal = Sprite::createWithSpriteFrameName("level_block_1.png");
    Sprite* select = Sprite::createWithSpriteFrameName("level_block_2.png");
    LYMenuItemSprite* item = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(propGetPathLayer::btnCallBack, this));
    item->setTag(levelId);
    item->setPosition(pos);
    parent->addChild(item);
    
    int m_iSmallLevel = CPersonalData::getInstance()->getUserInfo()->m_iSmallLevel;
    if(index >= m_comNum)//精英关卡的提示
    {
        m_iSmallLevel = CPersonalData::getInstance()->getUserInfo()->m_iEqliteSmallLevel;
        Sprite* tip = Sprite::createWithSpriteFrameName("jingying.png");
        item->addChild(tip,  10);
        tip->setPosition(Vec2(normal->getContentSize().width * 0.32f, normal->getContentSize().height * 0.68f));
    }
    
    levelInfo* pSmallLevelInfo = CDataManager::getTheLevelInfoWithId(levelId);
    Label* name = Label::createWithTTF(pSmallLevelInfo->levelName.c_str(), "fonts/cuti.ttf", 24);
    name->setColor(Color3B::WHITE);
    m_levelBg->addChild(name, 5);
    name->setPosition(Vec2(pos.x, pos.y + normal->getContentSize().height * 0.63f));
    
    Sprite* levelType = Sprite::createWithSpriteFrameName(CDataManager::getImgByLevelId(levelId).c_str());
    item->addChild(levelType, 5);
    levelType->setPosition(normal->getContentSize().width * 0.5, normal->getContentSize().height * 0.66);
    levelType->setOpacity(0);
    
    bool isShowStar = false;
    if(levelId > (m_iSmallLevel + 1))
    {
        //加锁
        Sprite* normal = Sprite::createWithSpriteFrameName("level_block_3.png");
        item->setNormalImage(normal);
        Sprite* lock = Sprite::createWithSpriteFrameName("level_lock.png");
        item->addChild(lock);
        item->setEnabled(false);
        lock->setPosition(Vec2(normal->getContentSize().width * 0.5, normal->getContentSize().height * 0.4));
    }
    else
    {
        isShowStar = true;
        if(levelId == m_selectedLevel)
        {
            Sprite* sp = Sprite::createWithSpriteFrameName("level_block_2.png");
            item->setNormalImage(sp);
        }
        levelType->setOpacity(255);
    }
    if(isShowStar){
        //星级
        int curLevelStart = CPersonalData::getInstance()->getStarByLevel(levelId, CPersonalData::getInstance()->getlevelMode());//获取当前关卡星级
        for(int i = 0; i < 3; i++)
        {
            Sprite* star = Sprite::createWithSpriteFrameName("level_star.png");
            if(i >= curLevelStart){
                star = Sprite::createWithSpriteFrameName("start_gray.png");
            }
            item->addChild(star, 5);//增加星星效果后后期可能要删掉原来的灰色星星
            float height = normal->getContentSize().height * 0.25;
            star->setScale(0.82f);
            star->setPosition(Vec2(normal->getContentSize().width * 0.25 + i*38, height));
        }
    }
    
}

void propGetPathLayer::btnCallBack(Ref *pSender)
{
    int tag = ((Node*)pSender)->getTag();
    if(PATH_EXIT_TAG == tag)
    {
        CPersonalData::getInstance()->setJumpFromSkill(false);
        CPersonalData::getInstance()->setJumpInfoNull();
        removeFromParent();
    }
    else if(PATH_GAME_START == tag)
    {
        CFightData::getInstance()->newLevelStart(m_selectedLevel);//设置当前选中关卡
        CFightData::getInstance()->setNowLevelId(m_selectedLevel);
        CAtkMonsterInfo *pLayer = CAtkMonsterInfo::create();
        
        pLayer->CreateActionStart(this,menu_selector(propGetPathLayer::OnStartFight));
        addChild(pLayer,111);
    }
    else
    {
        m_selectedLevel = tag;
        for (int i = 0; i < m_levelVec.size(); i++) {
            LYMenuItemSprite* tempItem = (LYMenuItemSprite*)(p_menu->getChildByTag(m_levelVec[i]));
            if (m_levelVec[i] == tag) {
                Sprite* sp = Sprite::createWithSpriteFrameName("level_block_2.png");
                tempItem->setNormalImage(sp);
            }
            else
            {
                if(m_levelVec[i] <= (CPersonalData::getInstance()->getUserInfo()->m_iSmallLevel + 1)){
                    Sprite* sp = Sprite::createWithSpriteFrameName("level_block_1.png");
                    tempItem->setNormalImage(sp);
                }
                else{
                    Sprite* sp = Sprite::createWithSpriteFrameName("level_block_3.png");
                    tempItem->setNormalImage(sp);
                }
            }
        }
        refreshLevelInfo();
    }
}

void propGetPathLayer::OnStartFight(Ref *pSender)
{
    
    levelInfo* pSmallLevelInfo = CDataManager::getTheLevelInfoWithId(m_selectedLevel);
    //判断体力是否足够
    int needStaina = pSmallLevelInfo->stamina;
    if (MY_PERSION->getlevelMode() == 1) {
        needStaina = pSmallLevelInfo->stamina * 2;
    }
    if(needStaina > MY_PERSION->GetPhysicalNum())//测试用false，看到就删
    {
        SimpleAudioEngine::getInstance()->playEffect("music/noBtn.mp3");
        Sprite * thetarget = CFightManager::getTheFlyWord("体力不足","fonts/cuti.ttf");
        thetarget->setPosition(Vec2(m_winSize.width / 2, m_winSize.height / 2));
        addChild(thetarget, 1111);
    }
    else
    {
        SimpleAudioEngine::getInstance()->stopBackgroundMusic();
        
//        if (m_bEqlite) {
//            MY_PERSION->setlevelMode(1);
//        }
//        else{
//            MY_PERSION->setlevelMode(0);
//        }
        
        m_pStartMenu->setEnabled(false);
        CFightData::getInstance()->setFightType(FIGHT_COMMON_TYPE);
        CLayerManager::GetSingletonPtr()->createLayerWithLoading(eLayer_Fight, NULL,false);
    }
}











