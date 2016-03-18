
//  SoliderSelectLayer.cpp
//  TheGodWill
//
//  Created by user on 14/12/1.
//
//

#include "SoliderSelectLayer.h"
#include "FightPrepareLayer.h"

#include "DataManager.h"
#include "LYFightData.h"
#include "DBMgr.h"
#include "guiderManagerLayer.h"


#define TIME 5
#define COIN_WIDTH 308 //coin 图片宽度
#define COIN_GAP 100    //间隔
#define COIN_COUNT 11   //个数

SoliderSelectLayer::SoliderSelectLayer()
{
    m_pNode = NULL;
    pMenuItemBack = NULL;
    pMenuItemNext = NULL;
    m_scrollView = NULL;
    m_pColor = NULL;
    m_pBg = NULL;
    m_pLabel = NULL;
    pMenu = NULL;
    m_isMoving = false;
    m_myFighterInfo.clear();
    m_fightMenu = NULL;
    //m_iFightId = 0;
    m_bmoveStart = false;
    m_bMoveEnd = false;
    m_time = 0;
    m_bgColor = NULL;
    soliderVec.clear();
}

SoliderSelectLayer::~SoliderSelectLayer()
{
    m_myFighterInfo.clear();
}

bool SoliderSelectLayer::onTouchBegan(Touch *touch, Event *unused_event)
{
    m_startPos = touch->getLocation();
    m_bmoveStart = true;
    return true;
}

void SoliderSelectLayer::onTouchMoved(Touch *touch, Event *unused_event)
{
    if(!m_isMoving && (m_myFighterInfo.size() > 3))
    {
        m_isMoving = true;
        scheduleUpdate();
    }
}

void SoliderSelectLayer::onTouchEnded(Touch *touch, Event *unused_event)
{
    m_endPos = touch->getLocation();
    
    
    m_speed = (m_endPos.x - m_startPos.x)/m_time;

    m_bmoveStart = false;
    m_bMoveEnd = true;
}

void SoliderSelectLayer::scrollViewDidScroll(cocos2d::extension::ScrollView *view){
    
    auto offSetX = view->getContentOffset().x;
    
    for(auto sp : soliderVec)
    {
        auto pointX = sp->getPositionX() + offSetX;
        if((pointX < 100) || (pointX > 890))
        {
            sp->setVisible(false);
        }
        else{
            sp->setVisible(true);
        }
    }
}


void SoliderSelectLayer::update(float dt)
{
    CCLOG("Updatecontent: %f",m_scrollView->getContentOffset().x);
    if(m_bmoveStart)
    {
        m_time++;
    }
    if(m_bMoveEnd)//m_myFighterInfo
    {
        if(fabsf(m_speed) < 3)
        {
            m_bMoveEnd = false;
            m_speed = 0;
            adjustScrollView();
        }
        else
        {
            float maxDistance = (m_speed/fabsf(m_speed))*(m_myFighterInfo.size() - 3) * 300;
            
            if ((m_speed < 0) && (m_pColor->getPositionX() < maxDistance)) {
                m_pColor->setPositionX(maxDistance);
                m_bMoveEnd = false;
                adjustScrollView();
            }
            else if((m_speed > 0) && (m_pColor->getPositionX() > 0))
            {
                m_pColor->setPositionX(0);
                m_bMoveEnd = false;
                adjustScrollView();
            }
            else{
                m_pColor->setPositionX(m_pColor->getPositionX() + m_speed * dt);
                m_speed-= 1.8 * (m_speed/fabsf(m_speed));
            }
        }
    }
}


void SoliderSelectLayer::onExit()
{
    _eventDispatcher->removeEventListener(listen);
    LayerColor::onExit();
}

bool SoliderSelectLayer::init()
{
    if (!LayerColor::initWithColor(Color4B(0, 0, 0, 125)))
    {
        return false;
    }
    m_winSize = Director::getInstance()->getWinSize();
    
    listen = EventListenerTouchOneByOne::create();
    listen->setSwallowTouches(true);
    listen->onTouchBegan = CC_CALLBACK_2(SoliderSelectLayer::onTouchBegan, this);
    listen->onTouchMoved = CC_CALLBACK_2(SoliderSelectLayer::onTouchMoved, this);
    listen->onTouchEnded = CC_CALLBACK_2(SoliderSelectLayer::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listen,this);
   // _eventDispatcher->addEventListenerWithFixedPriority(listen, -150);
    m_pBg = Sprite::createWithSpriteFrameName("select_solider_bg.png");
    addChild(m_pBg);
    m_pBg->setScale(2.0f);
    m_pBg->setPosition(Vec2(m_winSize.width / 2, m_winSize.height / 2));
    Size bgSize = m_pBg->getBoundingBox().size;
    
    
    m_bgColor = LayerColor::create(Color4B(255, 0, 0, 0), bgSize.width, bgSize.height);
    addChild(m_bgColor);
    m_bgColor->setPosition(Vec2(m_pBg->getPositionX() - m_pBg->getBoundingBox().size.width * 0.5,
                                m_pBg->getPositionY() - m_pBg->getBoundingBox().size.height * 0.5));

    Label* pTitle = Label::createWithSystemFont("士兵选择", "Arial", 30);
    m_bgColor->addChild(pTitle);
    pTitle->setPosition(Point(bgSize.width / 2, bgSize.height * 0.93f));
    
//    Label* pBack = Label::createWithSystemFont("返回", "Arial", 30);
//    MenuItemLabel* pBackLabel = MenuItemLabel::create(pBack, CC_CALLBACK_1(SoliderSelectLayer::menuCallBack, this));
//    Menu* pBackMenu = Menu::create(pBackLabel, NULL);
//    m_bgColor->addChild(pBackMenu);
//    pBackLabel->setTag(10);
//    pBackMenu->setPosition(Point(50, m_winSize.height / 4 - 15));
    m_curPage = 1;//默认显示第一页兵士
    pMenu = Menu::create();
    m_bgColor->addChild(pMenu);
    pMenu->setPosition(Vec2::ZERO);
    
    //获取玩家所选类型的所有英雄
    int type = CPersonalData::getInstance()->getSoliderType();
    m_myFighterInfo = CPersonalData::getInstance()->getSoliderInfoByType(type);
    
    char buf[128];
    Sprite* pNumBg = Sprite::createWithSpriteFrameName("select_kuang.png");
    m_bgColor->addChild(pNumBg);
    pNumBg->setScale(2.0f);
    pNumBg->setPosition(Point(bgSize.width / 2, bgSize.height * 0.08f));
    
    int allPage = int(m_myFighterInfo.size() - 2);
    if (allPage <= 0) {
        allPage = 1;
    }
    sprintf(buf, "%d/%d", m_curPage, allPage);
    m_pLabel = Label::createWithSystemFont(buf, "Arial", 28);
    m_bgColor->addChild(m_pLabel);
    m_pLabel->setPosition(bgSize.width * 0.5f, bgSize.height * 0.08f);
    
    Sprite* backNormal = Sprite::createWithSpriteFrameName("arrow.png");
    backNormal->setFlippedX(true);
    Sprite* backSelect = Sprite::createWithSpriteFrameName("arrow.png");
    backSelect->setFlippedX(true);
    backSelect->setColor(Color3B::GRAY);
    pMenuItemBack =  LYMenuItemSprite::create(backNormal, backSelect, CC_CALLBACK_1(SoliderSelectLayer::menuCallBack, this), "music/back.mp3");
    pMenu->addChild(pMenuItemBack);
    if (allPage == 1) {
        pMenuItemBack->setEnabled(false);
        pMenuItemBack->setColor(Color3B::GRAY);
    }
    pMenuItemBack->setTag(SOLIDER_SECLECT_BACK_TAG);
    pMenuItemBack->setPosition(Point(bgSize.width * 0.33, bgSize.height * 0.08f));
    
    Sprite* nextNormal = Sprite::createWithSpriteFrameName("arrow.png");
    Sprite* nextSelect = Sprite::createWithSpriteFrameName("arrow.png");
    nextSelect->setColor(Color3B::GRAY);
    pMenuItemNext =  LYMenuItemSprite::create(nextNormal, nextSelect, CC_CALLBACK_1(SoliderSelectLayer::menuCallBack, this), "music/next.mp3");
    pMenu->addChild(pMenuItemNext);
    if (allPage == 1) {
        pMenuItemNext->setEnabled(false);
        pMenuItemNext->setColor(Color3B::GRAY);
    }
    pMenuItemNext->setTag(SOLIDER_SECLECT_NECT_TAG);
    pMenuItemNext->setPosition(Point(bgSize.width * 0.67, bgSize.height * 0.08f));
    
    showSolider();
    
    Sprite* spNormal = Sprite::createWithSpriteFrameName("closeBtn.png");
    Sprite* spSelect = Sprite::createWithSpriteFrameName("closeBtn.png");
    spSelect->setColor(Color3B::GRAY);
    LYMenuItemSprite* pItemExit = LYMenuItemSprite::create(spNormal, spSelect, CC_CALLBACK_1(SoliderSelectLayer::menuCallBack, this));
    pItemExit->setPosition(Point(bgSize.width * 0.948f, bgSize.height * 0.936f));
    pItemExit->setTag(SOLIDER_SECLECT_EXIT_TAG);
    pMenu->addChild(pItemExit);
    
    if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SOLIDER_TEAM) && (guiderManagerLayer::getInstance()->getStepNum() == 3))
    {
        guiderManagerLayer::getInstance()->setStepNum(guiderManagerLayer::getInstance()->getStepNum() + 1);
        guiderManagerLayer::getInstance()->createLayer(this, GUIDE_SHOW_FINGER, m_guidePos);
    }
    
    return true;
    
}

void SoliderSelectLayer::showSolider()
{
    char buf[128];
    float width = 320*m_myFighterInfo.size();
    float scorllWidth = m_pBg->getBoundingBox().size.width * 0.9f;
    if(m_myFighterInfo.size() <= 3)
    {
        width = 320*3;
        scorllWidth = width;
    }
    
    Size bgSize = Size(width, 500);
    m_pColor = LayerColor::create(Color4B(255,0,0,0), bgSize.width, bgSize.height);

    m_scrollView = ScrollView::create(Size(scorllWidth, 500), m_pColor);
    m_scrollView->setPosition(Point(50,m_pBg->getBoundingBox().size.height*0.18));
    m_scrollView->setDirection(ScrollView::Direction::HORIZONTAL);
    m_scrollView->setBounceable(false);
    m_scrollView->setDelegate(this);
    m_bgColor->addChild(m_scrollView);
    
    m_fightMenu = Menu::create();
    m_pColor->addChild(m_fightMenu);
    m_fightMenu->setPosition(Vec2::ZERO);
    
    for (int i = 0; i < m_myFighterInfo.size(); i++) {
        Point pos = Point(180 + i*300, bgSize.height * 0.4);
        Sprite3D* hero = Sprite3D::create(m_myFighterInfo.at(i).theModelInfo->modelName.getCString());
        hero->setPosition(pos);
        hero->setGlobalZOrder(100);
        soliderVec.push_back(hero);
        
        Size modeSize = hero->getBoundingBox().size;
        if( i > 2)
        {
            hero->setVisible(false);
        }
        if(modeSize.width > 7)
        {
            hero->setScaleX(28*8.5/modeSize.width);
            hero->setScaleY(28*8.5/modeSize.height);
        }
        else if(modeSize.width < 5)
        {
            hero->setScale(28*4.5/modeSize.width);
            hero->setScale(28*9/modeSize.height);
        }
        else{
            hero->setScale(30);
        }
        m_pColor->addChild(hero, 1, 50 + i);
        auto animation = Animation3D::create(m_myFighterInfo.at(i).theModelInfo->modelName.getCString());
        if (animation)
        {
            auto animate = Animate3D::create(animation, m_myFighterInfo.at(i).theModelInfo->idleFrame.start/60,
                                             m_myFighterInfo.at(i).theModelInfo->idleFrame.duration/60);
            
            //animate->setQuality(<#cocos2d::Animate3DQuality quality#>)
            
            if(m_myFighterInfo[i]. monsterId ==  14|| m_myFighterInfo[i]. monsterId ==  15)
            {
                animate->setSpeed(0.3f);//测试用，换模型了就删掉
            }
            cocos2d::Action* _swim = RepeatForever::create(animate);
            hero->runAction(_swim);
        }
        
        Node* m_node = Node::create();
        m_pColor->addChild(m_node, 1, m_myFighterInfo.at(i).monsterId);
        m_node->setPosition(Vec2::ZERO);
        Sprite* levelBg = Sprite::createWithSpriteFrameName("select_level.png");
        levelBg->setPosition(pos.x - 70, pos.y - 55);
        m_node->addChild(levelBg, 5);
        
        sprintf(buf, "%d", m_myFighterInfo.at(i).level);
        Label* pLabel = Label::createWithSystemFont(buf, "Arial", 28);
        m_node->addChild(pLabel, 5);
        pLabel->setPosition(Vec2(pos.x - 70, pos.y - 55));
        
        Sprite* nameSp = Sprite::createWithSpriteFrameName("select_name.png");
        m_node->addChild(nameSp);
        nameSp->setScaleX(0.9f);
        nameSp->setAnchorPoint(Vec2(0, 0.5));
        nameSp->setPosition(Vec2(levelBg->getPositionX(), levelBg->getPositionY()));
        
        monsterInfo* info = CDataManager::getMonsterInfoByMonsterId(m_myFighterInfo.at(i).monsterId);
        Label* pName = Label::createWithSystemFont(info->monsterName.c_str(), "Arial", 28);
        nameSp->addChild(pName);
        pName->setAnchorPoint(Vec2(0, 0.5));
        pName->setPosition(nameSp->getContentSize().width * 0.2, nameSp->getContentSize().height / 2);
        
        Sprite* normal = Sprite::createWithSpriteFrameName("greenbtn.png");
        Sprite* select = Sprite::createWithSpriteFrameName("greenbtn.png");
        select->setColor(Color3B::GRAY);
        LYMenuItemSprite* fightItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(SoliderSelectLayer::menuCallBack, this));
        fightItem->setTag(m_myFighterInfo.at(i).monsterId);
        m_fightMenu->addChild(fightItem);
        fightItem->setScale(2.5f);
        fightItem->setPosition(Vec2(pos.x, bgSize.height * 0.13f));
        
        if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SOLIDER_TEAM) && (m_myFighterInfo.at(i).monsterId == 12))//记录小飞龙的位置
        {
            m_guidePos = fightItem->convertToWorldSpaceAR(Vec2::ZERO);
        }
        
        Label* pFight = Label::createWithSystemFont("上阵", "Arial", 30);
        fightItem->addChild(pFight);
        pFight->setScale(0.4f);
        pFight->setPosition(Vec2(normal->getContentSize().width / 2, normal->getContentSize().height / 2));
        
        int type = CPersonalData::getInstance()->getSoliderType();
        FighterInfo* pInfo = CPersonalData::getInstance()->getTeamSolderByType(type);
        if (pInfo && (pInfo->monsterId == m_myFighterInfo.at(i).monsterId)) {
            fightItem->setEnabled(false);
            fightItem->setColor(Color3B::GRAY);
        }
    }
    //updateMonsterInfo();
}

void SoliderSelectLayer::updateMonsterInfo()
{
    m_fightMenu = Menu::create();
    m_pBg->addChild(m_fightMenu);
    m_fightMenu->setPosition(Vec2::ZERO);
    //获取出战士兵的信息
    int type = CPersonalData::getInstance()->getSoliderType();
    FighterInfo* pInfo = CPersonalData::getInstance()->getTeamSolderByType(type);
    Size bgSize = m_pBg->getContentSize();
    int index = 0;
    for (int i = 0; i < m_myFighterInfo.size(); i++,index++) {
        Point pos = Point(220 + index*300, bgSize.height * 0.32 );
        Sprite* normal = Sprite::createWithSpriteFrameName("greenbtn.png");
        Sprite* select = Sprite::createWithSpriteFrameName("greenbtn.png");
        select->setColor(Color3B::GRAY);
        LYMenuItemSprite* fightItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(SoliderSelectLayer::menuCallBack, this));
        fightItem->setTag(m_myFighterInfo.at(i).monsterId);
        m_fightMenu->addChild(fightItem);
        fightItem->setScale(0.7f);
        fightItem->setPosition(Vec2(pos.x + 73, pos.y - 65));
        
        Label* pFight = Label::createWithSystemFont("上阵", "Arial", 30);
        fightItem->addChild(pFight);
        pFight->setScale(1.5f);
        pFight->setPosition(Vec2(normal->getContentSize().width / 2, normal->getContentSize().height / 2));
        
        if (pInfo && (pInfo->monsterId == m_myFighterInfo.at(i).monsterId)) {
            fightItem->setEnabled(false);
            fightItem->setColor(Color3B::GRAY);
        }
    }
//    for (int i = 0; i < m_myFighterInfo.size(); i++) {
//        if((i < (m_curPage - 1)) || (i >= (m_curPage + 2)))
//        {
//            Sprite3D* tempSp = (Sprite3D*)(m_pColor->getChildByTag(50 + i));
//            if(tempSp)
//            {
//                tempSp->setVisible(false);
//            }
//            Node* m_node = (Node*)(m_pColor->getChildByTag(m_myFighterInfo.at(i).monsterId));
//            if(m_node)
//            {
//                m_node->setVisible(false);
//            }
//        }
//    }
}

void SoliderSelectLayer::adjustScrollView()
{
    unscheduleUpdate();
    m_speed = 0;
    int index = 0;
    float mindistance = 999999;
    int position = -1;
    float m_distance = m_scrollView->getContentOffset().x;
    if(m_endPos.x - m_startPos.x > 0)
    {
        position = 1;
    }
    
    float modePos = 520;
    for (int i = 0; i < m_myFighterInfo.size(); i++) {
        Sprite3D* hero = (Sprite3D*)(m_pColor->getChildByTag(50 + i));
        if(hero)
        {
            float posX = hero->getPosition3D().x;
            posX += m_distance;
            if(fabsf(mindistance) > fabsf(posX - modePos))
            {
                mindistance = modePos - posX ;
                index = i;
            }
        }
    }
    m_pColor->stopAllActions();
    m_curPage = index;
    CallFunc* set = CallFunc::create(CC_CALLBACK_0(SoliderSelectLayer::setMoveValid, this));
    CallFunc* updateMenu = CallFunc::create(CC_CALLBACK_0(SoliderSelectLayer::updateNum, this));
    m_pColor->runAction(Sequence::create(MoveBy::create(0.25f, Vec2(mindistance, 0)), set, updateMenu, NULL));

}


void SoliderSelectLayer::setMoveValid()
{
    m_isMoving = false;
}

void SoliderSelectLayer::updateNum()
{
    int allPage = int(m_myFighterInfo.size() - 2);
    if (allPage <= 0) {
        allPage = 1;
    }
    char buf[32];
    sprintf(buf, "%d/%d", m_curPage, allPage);
    m_pLabel->setString(buf);
    if (m_curPage == 1) {
        pMenuItemBack->setEnabled(false);
        pMenuItemBack->setColor(Color3B::GRAY);
    }
    else
    {
        pMenuItemBack->setEnabled(true);
        pMenuItemBack->setColor(Color3B::WHITE);
    }
    if (m_curPage == allPage) {
        pMenuItemNext->setEnabled(false);
        pMenuItemNext->setColor(Color3B::GRAY);
    }
    else
    {
        pMenuItemNext->setEnabled(true);
        pMenuItemNext->setColor(Color3B::WHITE);
    }
    //CallFunc* updateMenu = CallFunc::create(CC_CALLBACK_0(SoliderSelectLayer::updateMonsterInfo, this));
    //runAction(Sequence::create(DelayTime::create(0.3), updateMenu, NULL));
    //updateMonsterInfo();
}


void SoliderSelectLayer::menuCallBack(Ref* pSender)
{
    int tag = ((Node*)pSender)->getTag();
    if (tag == SOLIDER_SECLECT_EXIT_TAG)
    {
        CLayerManager::GetSingletonPtr()->createLayer(eLayer_TeamLayer,this->getParent(),false);
        removeFromParent();
    }
    else if(tag == SOLIDER_SECLECT_NECT_TAG)
    {
        int allPage = int(m_myFighterInfo.size() - 2);
        if (allPage <= 0) {
            allPage = 1;
        }
        if (m_curPage == allPage) {
            return;
        }
        
        LYMenuItemSprite* preMenuItem = (LYMenuItemSprite*)pMenu->getChildByTag(m_curPage + 1);
        if (preMenuItem) {
            preMenuItem->runAction(ScaleTo::create(0.3, 1.0));
        }
        
        LYMenuItemSprite* pNextMenuItem = (LYMenuItemSprite*)pMenu->getChildByTag(m_curPage + 2);
        if (pNextMenuItem) {
            pNextMenuItem->runAction(ScaleTo::create(0.3, 1.13));
        }
        m_pColor->runAction(MoveBy::create(0.3f, Vec2(-300, 0)));
        m_curPage++;
        updateNum();
    }
    else if(tag == SOLIDER_SECLECT_BACK_TAG)
    {
        if (m_curPage == 1) {
            return;
        }
        LYMenuItemSprite* pNextMenuItem = (LYMenuItemSprite*)pMenu->getChildByTag(m_curPage + 1);
        if (pNextMenuItem) {
            pNextMenuItem->runAction(ScaleTo::create(0.3, 1.0));
        }
        
        LYMenuItemSprite* preMenuItem = (LYMenuItemSprite*)pMenu->getChildByTag(m_curPage);
        if (preMenuItem) {
            preMenuItem->runAction(ScaleTo::create(0.3, 1.13));
        }
        m_curPage--;
        m_pColor->runAction(MoveBy::create(0.3f, Vec2(300, 0)));
        updateNum();
    }
    else
    {
        guiderManagerLayer::getInstance()->removeGuiderLayer();
        for (int i = m_curPage - 1; (i < m_curPage + 2) && (i < m_myFighterInfo.size()); i++) {
            LYMenuItemSprite* tempItem = (LYMenuItemSprite*)m_fightMenu->getChildByTag(m_myFighterInfo.at(i).monsterId);
            if(m_myFighterInfo.at(i).monsterId == tag)
            {
                //判断创建当前士兵所需的能量是否比当前能量上限大，如果是则不可上阵，因为创建不出来
                monsterInfo*pInfo = CDataManager::getMonsterInfoByMonsterId(tag);
                int maxEnemgy = CDBMgr::getInstance()->getAffectNum(CDBMgr::eAffectType::eEnergyType, 2);
                if(pInfo && (pInfo->energyCost > maxEnemgy))
                {
                    Sprite * thetarget = CFightManager::getTheFlyWord("该兵种所需能量超过您的能量最大值\n请先升级您的能量最大值","fonts/cuti.ttf");
                    thetarget->setPosition(Vec2(m_winSize.width / 2, m_winSize.height / 2));
                    addChild(thetarget, 10);
                    return;
                }
                //m_iFightId = tag;
                std::vector<FighterInfo*> tempVec = CPersonalData::getInstance()->m_vecMyTeamInfo;
                //先删除同类型的怪物
                for(std::vector<FighterInfo*>::iterator it = tempVec.begin(); it != tempVec.end(); it++)
                {
                    if (CPersonalData::getInstance()->getSoliderType() == (*it)->monsterType) {
                        CDataManager::saveUserTeamToDB((*it)->monsterId, 0);
                        tempVec.erase(it);
                        break;
                    }
                }
                CPersonalData::getInstance()->m_vecMyTeamInfo = tempVec;
                
                CDataManager::saveUserTeamToDB(m_myFighterInfo.at(i).monsterId, 1);
                //将m_vecMyFighterInfo中的信息保存到m_vecMyTeamInfo
                for(int j = 0;j < CPersonalData::getInstance()->m_vecMyFighterInfo.size(); j++){
                    if(m_myFighterInfo.at(i).monsterId == CPersonalData::getInstance()->m_vecMyFighterInfo.at(j)->monsterId)
                    {
                        CPersonalData::getInstance()->m_vecMyTeamInfo.push_back(CPersonalData::getInstance()->m_vecMyFighterInfo.at(j));
                        break;
                    }
                }
                tempItem->setEnabled(false);
                tempItem->setColor(Color3B::GRAY);
                //播放特殊动作动画
                int modelId = CDataManager::getMonsterInfoByMonsterId(tag)->modelId;
                modelInfo* pModelInfo = CDataManager::getModelInfoByModelId(modelId);
                Sprite3D* hero = (Sprite3D*)(m_pColor->getChildByTag(50 + i));
                auto animation = Animation3D::create(pModelInfo->modelName.getCString());
                if (animation && hero)
                {
                    hero->unscheduleAllCallbacks();
//                    hero->stopActionByTag(SOLDIER_ANIMATE_TAG);
                    hero->stopAllActions();
                    CallFunc* act = CallFunc::create(CC_CALLBACK_0(SoliderSelectLayer::removeSelf, this));
                    auto animate = Animate3D::create(animation, pModelInfo->specialAttackFrame.start/60, pModelInfo->specialAttackFrame.duration/60);
                    
                    if(m_myFighterInfo[i]. monsterId ==  14|| m_myFighterInfo[i]. monsterId ==  15)
                    {
                        animate->setSpeed(0.3f);//测试用，换模型了就删掉
                    }

                    CCLOG("theTime ::%f",animate->getDuration());
                    
                    hero->runAction(animate);
                    
                    hero->runAction(Sequence::create(DelayTime::create(animate->getDuration()),act, NULL));
                    
                    
//                    hero->runAction(Sequence::create(animate, act, NULL));
                }

            }
            else{
                tempItem->setEnabled(false);
                tempItem->setColor(Color3B::WHITE);
            }
        }
    }
}

void SoliderSelectLayer::removeSelf()
{
    
    CLayerManager::GetSingletonPtr()->createLayer(eLayer_TeamLayer,this->getParent(),false);
    removeFromParent();
}


