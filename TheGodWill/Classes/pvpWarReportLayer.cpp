//
//  pvpWarReportLayer.cpp
//  WarOfGod
//
//  Created by user on 15/6/17.
//
//

#include "pvpWarReportLayer.h"
#include "DataManager.h"
#include "DBMgr.h"
#include "zzbNetControlModule.h"
#include "zzbNewNetStruct.h"
#include "fightScene.h"
#include "LYFightData.h"
#include "LYPersonalData.h"
#include "Common.h"
#include "pvpMainLayer.h"

std::string headIcon[5]={"white_bg.png","green_bg.png","blue_bg.png", "purple_bg.png","gold_bg.png"};


pvpWarReportLayer::pvpWarReportLayer()
{
    m_spBg = NULL;
    m_pLayer = NULL;
    m_tableView = NULL;
    m_allVideoInfo.clear();
    m_stone = 0;
    m_idx = 0;
    m_layer = NULL;
    m_iCurVideoId = 0;
    
}

pvpWarReportLayer::~pvpWarReportLayer()
{
    _eventDispatcher->removeEventListener(m_listener);
    unschedule(schedule_selector(pvpWarReportLayer::updateTime));
}

bool pvpWarReportLayer::onTouchBegan(Touch* pTouch, Event* pEvent)
{
    return true;
}

void pvpWarReportLayer::onTouchMoved(Touch* pTouch, Event* pEvent)
{
    
}

void pvpWarReportLayer::onTouchEnded(Touch* pTouch, Event* pEvent)
{
    
}


bool pvpWarReportLayer::init()
{
    if(!CBaseLayer::initWithColor(Color4B(0, 0, 0, 200)))
    {
        return false;
    }
    m_winSize = Director::getInstance()->getWinSize();
    
    m_listener = EventListenerTouchOneByOne::create();
    m_listener->setSwallowTouches(true);
    m_listener->onTouchBegan = CC_CALLBACK_2(pvpWarReportLayer::onTouchBegan, this);
    m_listener->onTouchMoved = CC_CALLBACK_2(pvpWarReportLayer::onTouchMoved, this);
    m_listener->onTouchEnded = CC_CALLBACK_2(pvpWarReportLayer::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(m_listener, this);
    m_allVideoInfo = CDataManager::getVideoListInfo();
    
    CNetControlModule::sharedNetControlModule()->getServerTime(this, menu_selector(pvpWarReportLayer::getServerTime));
    
    return true;
}

void pvpWarReportLayer::getServerTime(Ref* pSender)
{
    if(CPersonalData::getInstance()->getState() != 0)
    {
        log("请求服务器时间失败！");
    }
    CPersonalData::getInstance()->setState(1);
    showUI();
}

void pvpWarReportLayer::showUI()
{
    m_spBg = Sprite::createWithSpriteFrameName("pvp_infoBg.png");
    addChild(m_spBg);
    m_spBg->setPosition(Vec2(m_winSize.width * 0.5f, m_winSize.height * 0.48f));
    m_spBg->setScaleX(2.33f);
    m_spBg->setScaleY(2.16f);
    Size bgSize = m_spBg->getBoundingBox().size;
    m_pLayer = LayerColor::create(Color4B(0, 0, 0, 0), bgSize.width, bgSize.height);
    addChild(m_pLayer);
    m_pLayer->setPosition(Vec2(m_spBg->getPositionX() - bgSize.width * 0.5f, m_spBg->getPositionY() - bgSize.height * 0.5f));
    
    //标题框
    Sprite* pTitleBg = Sprite::createWithSpriteFrameName("pvp_infoTitileBg.png");
    m_pLayer->addChild(pTitleBg);
    pTitleBg->setScale(1.5f);
    pTitleBg->setPosition(Vec2(bgSize.width * 0.5f, bgSize.height * 0.985f));
    
    Label* pTitle = Label::createWithTTF("防守记录", "fonts/cuti.ttf", 34);
    m_pLayer->addChild(pTitle);
    pTitle->enableOutline(Color4B(0, 0, 0, 255), 3);
    pTitle->setPosition(Vec2(bgSize.width * 0.5f, bgSize.height * 0.98f));
    
    Sprite* light = Sprite::createWithSpriteFrameName("pvp_video_light.png");
    m_pLayer->addChild(light, 1);
    light->setPosition(Vec2(bgSize.width * 0.5f, bgSize.height * 0.06f));
    
    m_tableView = TableView::create(this, Size(bgSize.width * 0.98f,bgSize.height*0.87));
    m_tableView->setDirection(ScrollView::Direction::VERTICAL);
    m_tableView->setPosition(Vec2(6,32));
    m_tableView->setDelegate(this);
    m_tableView->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    m_pLayer->addChild(m_tableView);
    m_tableView->reloadData();
    
    Sprite* normal = Sprite::createWithSpriteFrameName("closeBtn1.png");
    Sprite* select = Sprite::createWithSpriteFrameName("closeBtn1.png");
    select->setColor(Color3B::GRAY);
    LYMenuItemSprite* pCloseItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(pvpWarReportLayer::btnCallBack, this));
    pCloseItem->setTag(WAR_BTN_EXIT);
    CommonData::enlargeItemSprite(pCloseItem, 2);
    Menu* pMenu = Menu::create(pCloseItem, NULL);
    m_pLayer->addChild(pMenu);
    pMenu->setPosition(Vec2(bgSize.width * 0.98f, bgSize.height * 0.96f));
    
    schedule(schedule_selector(pvpWarReportLayer::updateTime), 1.0f);
    
    if(m_allVideoInfo.size() == 0)
    {
        Sprite* theBG = Sprite::createWithSpriteFrameName("zhdt.png");
        addChild(theBG,100);
        theBG->setPosition(Vec2(m_winSize.width * 0.5f, m_winSize.height * 0.5f));
        Label* thetarget = Label::createWithTTF("暂无录像记录","fonts/cuti.ttf", 35);
        theBG->addChild(thetarget);
        thetarget->setColor(Color3B(253,161,63));
        thetarget->setPosition(Vec2(theBG->getBoundingBox().size.width*0.5f, theBG->getBoundingBox().size.height*0.5f));
    }
    //保存玩家当前所有的录像已被查看标识
    CDataManager::saveNewVideoInfo();
}

std::string pvpWarReportLayer::calculateTime(int seconds)
{
    char buf[256];
    int serverTime = int(CPersonalData::getInstance()->getUserInfo()->serverLogintime/1000);
    int allTime = serverTime - seconds;
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
        int days = 0;
        if(minute > 60)
        {
            hour = minute/60;
            minute = minute%60;
            if(hour > 24)
            {
                days = hour/24;
                hour = hour%24;
                if(days > 0)
                {
                    sprintf(buf, "%d天", days);
                    time += buf;
                }
            }
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
        if((days == 0) && (hour == 0) && (minute == 0))
        {
            if(second > 0)
            {
                sprintf(buf, "%d秒", second);
                time += buf;
            }
        }
    }
    time += "前";
    return time;
}

void pvpWarReportLayer::tableCellTouched(TableView* table,TableViewCell* cell)
{
    
}

void pvpWarReportLayer::tableCellHighlight(TableView* table,TableViewCell* cell)
{
    
}

void pvpWarReportLayer::tableCellUnhighlight(TableView* table,TableViewCell* cell)
{
    
}

cocos2d::Size pvpWarReportLayer::cellSizeForTable(TableView* table)
{
    Size bgSize = m_spBg->getBoundingBox().size;
    return Size(978, 226);
}

TableViewCell* pvpWarReportLayer::tableCellAtIndex(TableView *table,ssize_t idx)
{
    TableViewCell *cell = table->dequeueCell();
    char buf[256];
    if(!cell)
    {
        cell = new TableViewCell();
        cell->autorelease();
    }
    cell->removeAllChildren();
    Sprite* cellBg = Sprite::createWithSpriteFrameName("pvp_video_bg.png");
    if(m_allVideoInfo[idx].iWin == 1)
    {
        cellBg = Sprite::createWithSpriteFrameName("pvp_video_redBg.png");
    }
    cellBg->setAnchorPoint(Vec2::ZERO);
    cellBg->setPosition(Vec2(15, 0));
    cellBg->setScaleX(2.0f);
    cellBg->setScaleY(2.1f);
    cell->addChild(cellBg);
    Size bgSize = cellBg->getBoundingBox().size;
    
    //玩家名字
    Label* enenmtName = Label::createWithTTF(m_allVideoInfo[idx].enemyName.c_str(), "fonts/cuti.ttf", 26);
    cell->addChild(enenmtName);
    enenmtName->enableOutline(Color4B(0, 0, 0, 255), 2);
    enenmtName->setAnchorPoint(Vec2(0, 0.5f));
    enenmtName->setPosition(Vec2(bgSize.width * 0.04f, bgSize.height * 0.85));
    
    //等级
    sprintf(buf, "LV:%d", m_allVideoInfo[idx].enemyLevel);
    Label* pLevel = Label::createWithTTF(buf, "fonts/cuti.ttf", 26);
    cell->addChild(pLevel);
    pLevel->setAnchorPoint(Vec2(0, 0.5f));
    pLevel->enableOutline(Color4B(0, 0, 0, 255), 2);
    pLevel->setPosition(Vec2(enenmtName->getPositionX() + enenmtName->getBoundingBox().size.width + 20, enenmtName->getPositionY()));
    
    //阵营显示
    std::vector<IdAndTwoNum> enemyInfo = m_allVideoInfo[idx].enemyInfo;
    for(int i = 0; i < enemyInfo.size(); i++)
    {
        Vec2 pos = Vec2(bgSize.width * 0.09f + 110 * i, bgSize.height * 0.515f);
        int rare = CDataManager::getMonsterStarById(enemyInfo[i].theId);
        int index = 0;
        if(rare > 0)
        {
            index = rare - 1;
        }
        
        Sprite* soliderBg = Sprite::createWithSpriteFrameName("pvp_solider_bg.png");
        cell->addChild(soliderBg);
        soliderBg->setScaleX(1.1f);
        soliderBg->setScaleY(1.1f);
        soliderBg->setPosition(pos.x, bgSize.height * 0.47);
        
        Sprite* icon = Sprite::createWithSpriteFrameName("pvp_yixiu.png");
        cell->addChild(icon);
        icon->setPosition(Vec2(pos.x - 20, bgSize.height * 0.242f));
        
        sprintf(buf, "X %d", enemyInfo[i].number);
        Label* pNumLabel = Label::createWithTTF(buf, "fonts/cuti.ttf", 20);
        cell->addChild(pNumLabel);
        pNumLabel->enableOutline(Color4B(0, 0, 0, 255), 1);
        pNumLabel->setAnchorPoint(Vec2(0, 0.5f));
        pNumLabel->setPosition(icon->getPositionX() + 20 ,bgSize.height * 0.242f);
        
        Sprite* monsterBg = Sprite::createWithSpriteFrameName(headIcon[index].c_str());
        monsterBg->setScale(3.0f);
        cell->addChild(monsterBg);
        monsterBg->setPosition(pos);
        
        monsterInfo* pInfo= CDataManager::getMonsterInfoByMonsterId(enemyInfo[i].theId);
        Sprite* monster = Sprite::createWithSpriteFrameName(pInfo->monsterIcon.c_str());
        monsterBg->addChild(monster);
        monster->setScale(0.22);
        monster->setPosition(Vec2(monsterBg->getContentSize().width / 2, monsterBg->getContentSize().height / 2));
        //星级
        for(int i = 0;i < rare; i++)
        {
            Sprite* star = Sprite::createWithSpriteFrameName("advance_star.png");
            monsterBg->addChild(star, 5);
            star->setScale(0.3);
            if(rare == 1){
                star->setPosition(Vec2(monsterBg->getContentSize().width / 2, monsterBg->getContentSize().height * 0.96f));
            }
            else if(rare == 2){
                star->setPosition(Vec2(monsterBg->getContentSize().width * 0.38f + i*8 , monsterBg->getContentSize().height * 0.96f));
            }
            else if(rare == 3){
                star->setPosition(Vec2(monsterBg->getContentSize().width * 0.32f + i*6 , monsterBg->getContentSize().height * 0.96f));
            }
            else if(rare == 4){
                star->setPosition(Vec2(monsterBg->getContentSize().width * 0.235 + i*6 , monsterBg->getContentSize().height * 0.96f));
            }
            else if(rare == 5){
                star->setPosition(Vec2(monsterBg->getContentSize().width * 0.2 + i*5 , monsterBg->getContentSize().height * 0.96f));
            }
        }
        Sprite * pType = CDBMgr::getInstance()->GetShareSpriteByFigtherType(pInfo->monsterType);
        pType->setPosition(monster->getContentSize().width*0.2,monster->getContentSize().height*0.9);
        monster->addChild(pType);
        
        Sprite* monsterNumBg = Sprite::createWithSpriteFrameName("detail.png");
        monster->addChild(monsterNumBg);
        monsterNumBg->setScaleX(0.9f);
        monsterNumBg->setScaleY(0.4f);
        monsterNumBg->setPosition(monster->getContentSize().width*0.5,monster->getContentSize().height*0.06);
        
        sprintf(buf, "Lv:%d", enemyInfo[i].level);
        Label* pSoliderLevel = Label::createWithSystemFont(buf, "Arial", 24);
        monster->addChild(pSoliderLevel);
        pSoliderLevel->setAnchorPoint(Vec2(0, 0.5f));
        pSoliderLevel->setPosition(5,monster->getContentSize().height*0.06);
    }
    
    //数量
    sprintf(buf, "%d", m_allVideoInfo[idx].robStone);
    Label* plunderNum = Label::createWithTTF(buf, "fonts/cuti.ttf", 24);
    plunderNum->setAnchorPoint(Vec2(1, 0.5f));
    cell->addChild(plunderNum);
    plunderNum->enableOutline(Color4B(0, 0, 0, 255), 2);
    plunderNum->setPosition(Vec2(bgSize.width * 0.68f, bgSize.height * 0.85));
    
    Sprite* stone = Sprite::createWithSpriteFrameName("pvp_ore.png");
    cell->addChild(stone);
    stone->setAnchorPoint(Vec2(1, 0.5f));
    stone->setScale(0.55f);
    stone->setPosition(Vec2(plunderNum->getPositionX() - plunderNum->getBoundingBox().size.width - 8, bgSize.height * 0.85f));
    
    //掠夺的资源
    sprintf(buf, "被%s掠夺了", m_allVideoInfo[idx].enemyName.c_str());
    Label* plunder = Label::createWithTTF(buf, "fonts/cuti.ttf", 24);
    cell->addChild(plunder);
    plunder->setAnchorPoint(Vec2(1, 0.5f));
    plunder->enableOutline(Color4B(0, 0, 0, 255), 2);
    plunder->setPosition(Vec2(stone->getPositionX() - stone->getBoundingBox().size.width - 8, bgSize.height * 0.85));
    
    //距当前时间 先做好示意图
    std::string time = calculateTime(m_allVideoInfo[idx].uploadTime);
    Label* pTime = Label::createWithSystemFont(time.c_str(), "Arial", 26);
    cell->addChild(pTime);
    pTime->setColor(Color3B::BLACK);
    pTime->setAnchorPoint(Vec2(1, 0.5f));
    pTime->setPosition(Vec2(bgSize.width * 0.66f, bgSize.height * 0.15f));
    //分割线
    Sprite* line= Sprite::createWithSpriteFrameName("split_line.png");
    cell->addChild(line);
    line->setScaleY(0.38f);
    line->setPosition(Vec2(bgSize.width * 0.7f, bgSize.height * 0.48f));
    //右边
    Sprite* descSp = Sprite::createWithSpriteFrameName("pvp_video_tipBg.png");
    cell->addChild(descSp);
    descSp->setPosition(Vec2(bgSize.width * 0.865f, bgSize.height * 0.84f));
    
    std::string rest = "您的主基地被攻陷";
    if(m_allVideoInfo[idx].iWin == 0)
    {
        rest = "您的主基地防御成功";
    }
    Label* descLabel = Label::createWithTTF(rest.c_str(), "fonts/cuti.ttf", 24);
    descSp->addChild(descLabel);
    descLabel->setColor(Color3B::BLACK);
    descLabel->setPosition(Vec2(descSp->getBoundingBox().size.width * 0.5f, descSp->getBoundingBox().size.height * 0.5f));
    
    
    if(m_allVideoInfo[idx].bValid == 0)
    {
        Sprite* normal = Sprite::createWithSpriteFrameName("greenbtn.png");
        Sprite* select = Sprite::createWithSpriteFrameName("greenbtn.png");
        select->setColor(Color3B::GRAY);
        LYMenuItemSprite* backItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(pvpWarReportLayer::btnCallBack, this));
        Menu* pbackMenu = Menu::create(backItem, NULL);
        cell->addChild(pbackMenu);
        backItem->setTag(WAR_BACK_PLAY);
        backItem->setUserData((void*)idx);
        backItem->setScale(2.5f);
        pbackMenu->setPosition(Vec2(bgSize.width * 0.92f, bgSize.height * 0.56f));
        Label* pBackLabel = Label::createWithTTF("回放", "fonts/cuti.ttf", 28);
        backItem->addChild(pBackLabel);
        pBackLabel->enableOutline(Color4B(0, 0, 0, 255), 3);
        pBackLabel->setScale(0.36f);
        pBackLabel->setPosition(Vec2(normal->getBoundingBox().size.width * 0.5f, normal->getBoundingBox().size.height * 0.5f));
    }
    else{
        Label* pVideoLabel = Label::createWithTTF("录像不可用", "fonts/cuti.ttf", 24);
        cell->addChild(pVideoLabel);
        pVideoLabel->enableOutline(Color4B(0, 0, 0, 255), 3);
        pVideoLabel->setPosition(Vec2(bgSize.width * 0.92f, bgSize.height * 0.55f));
    }
    
    if(m_allVideoInfo[idx].iWin == 1)
    {
        //星级
        __String* pBuf = __String::createWithFormat("%d%%", m_allVideoInfo[idx].destroyDegree);
        Label* pStarLevel = Label::createWithSystemFont(pBuf->getCString(), "Arial", 26);
        cell->addChild(pStarLevel);
        pStarLevel->setColor(Color3B::BLACK);
        pStarLevel->setPosition(Vec2(bgSize.width * 0.78f, bgSize.height * 0.65f));
        for(int i = 0; i < 3; i++)
        {
            Sprite* sp = Sprite::createWithSpriteFrameName("start_gray.png");
            if(i == 0)
            {
                sp = Sprite::createWithSpriteFrameName("level_star.png");
            }
            if((i == 1) && (m_allVideoInfo[idx].destroyDegree > 50))
            {
                sp = Sprite::createWithSpriteFrameName("level_star.png");
            }
            if((i == 2) && (m_allVideoInfo[idx].destroyDegree == 100))
            {
                sp = Sprite::createWithSpriteFrameName("level_star.png");
            }
            cell->addChild(sp);
            sp->setPosition(Vec2(bgSize.width * 0.74f + 35*i, bgSize.height * 0.49f));
        }
        
        Sprite* normal = Sprite::createWithSpriteFrameName("redbtn.png");
        Sprite* select = Sprite::createWithSpriteFrameName("redbtn.png");
        select->setColor(Color3B::GRAY);
        LYMenuItemSprite* pRevengeItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(pvpWarReportLayer::btnCallBack, this));
        Menu* pRevengeMenu = Menu::create(pRevengeItem, NULL);
        cell->addChild(pRevengeMenu);
        pRevengeItem->setTag(WAR_BTN_REVENAGE);
        pRevengeItem->setUserData((void*)idx);
        pRevengeItem->setScale(2.5f);
        pRevengeMenu->setPosition(Vec2(bgSize.width * 0.92f, bgSize.height * 0.21f));
        Label* pRevengeLabel = Label::createWithTTF("复仇", "fonts/cuti.ttf", 28);
        pRevengeItem->addChild(pRevengeLabel);
        pRevengeLabel->enableOutline(Color4B(0, 0, 0, 255), 3);
        pRevengeLabel->setScale(0.36f);
        pRevengeLabel->setPosition(Vec2(normal->getBoundingBox().size.width * 0.5f, normal->getBoundingBox().size.height * 0.5f));
        if(m_allVideoInfo[idx].iAttack == 1)
        {
            pRevengeMenu->setVisible(false);
        }
    }
    else
    {
        //星级
        for(int i = 0; i < 3; i++)
        {
            Sprite* sp = Sprite::createWithSpriteFrameName("start_gray.png");
            cell->addChild(sp);
            sp->setPosition(Vec2(bgSize.width * 0.74f + 35*i, bgSize.height * 0.52f));
        }
        stone->setVisible(false);
        plunderNum->setVisible(false);
        plunder->setVisible(false);
        if(m_allVideoInfo[idx].iGetWard == 0)
        {
            Sprite* normal = Sprite::createWithSpriteFrameName("greenbtn.png");
            Sprite* select = Sprite::createWithSpriteFrameName("greenbtn.png");
            select->setColor(Color3B::GRAY);
            LYMenuItemSprite* pAwardItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(pvpWarReportLayer::btnCallBack, this));
            Menu* pRAwardMenu = Menu::create(pAwardItem, NULL);
            cell->addChild(pRAwardMenu);
            pAwardItem->setTag(WAR_ACQUIRE_AWARD);
            pAwardItem->setUserData((void*)idx);
            pAwardItem->setScale(2.5f);
            pRAwardMenu->setPosition(Vec2(bgSize.width * 0.92f, bgSize.height * 0.21f));
            Label* pAwardLabel = Label::createWithTTF("领取奖励", "fonts/cuti.ttf", 28);
            pAwardItem->addChild(pAwardLabel);
            pAwardLabel->enableOutline(Color4B(0, 0, 0, 255), 3);
            pAwardLabel->setScale(0.36f);
            pAwardLabel->setPosition(Vec2(normal->getBoundingBox().size.width * 0.5f, normal->getBoundingBox().size.height * 0.5f));
            
            //奖励
            Sprite* awardBg = Sprite::createWithSpriteFrameName("twobei.png");
            cell->addChild(awardBg);
            awardBg->setRotation(90);
            awardBg->setScaleX(0.1f);
            awardBg->setScaleY(0.2f);
            awardBg->setPosition(Vec2(bgSize.width * 0.78f, bgSize.height * 0.2f));
            
            Label* pAwardNum = Label::createWithSystemFont("1", "Arial", 26);
            cell->addChild(pAwardNum);
            pAwardNum->setAnchorPoint(Vec2(0, 0.5f));
            pAwardNum->setPosition(Vec2(bgSize.width * 0.74f, awardBg->getPositionY()));
            
            Sprite* stamina = Sprite::createWithSpriteFrameName("stamina.png");
            cell->addChild(stamina);
            stamina->setScale(0.6f);
            stamina->setPosition(Vec2(bgSize.width * 0.805f, pAwardNum->getPositionY()));
        }
    }
    
    return cell;
}

void pvpWarReportLayer::updateTime(float dt)
{
    for(int i = 0; i < m_allVideoInfo.size(); i++)
    {
        m_allVideoInfo[i].uploadTime -= 1;
        m_tableView->updateCellAtIndex(i);
    }
}

ssize_t pvpWarReportLayer::numberOfCellsInTableView(TableView *table)
{
    return m_allVideoInfo.size();
}


void pvpWarReportLayer::Enemy(Ref *pSedner)
{
    CPersonalData::getInstance()->setFromVideoList(true);
    CPersonalData::getInstance()->setEnemyType(SPECIFIED_ENEMY);
    CFightData::getInstance()->setPvpReporterThis(this);
    
}

void  pvpWarReportLayer::Rebo()
{
    
    
    
    
    
}
void pvpWarReportLayer::btnCallBack(Ref* pSender)
{
    int tag = ((Node*)pSender)->getTag();
    
    
    LYMenuItemSprite* pBackItem = (LYMenuItemSprite*)pSender;
    
    
    if(WAR_BTN_EXIT == tag)
    {
        CPersonalData::getInstance()->setTouchRmvFlag(true);
        this->removeFromParent();
    }
    else
    {
        LYMenuItemSprite* temp = (LYMenuItemSprite*)pSender;
        long idx = (long)(temp->getUserData());
        
        m_idx = idx;
        
        if(WAR_ACQUIRE_AWARD == tag)
        {
            temp->setEnabled(false);
            temp->setColor(Color3B::GRAY);
            m_layer = pvpCommonInfoLayer::create(PVP_COMMSG_ACQUIRE_AWARD);
            m_layer->addAcquireAward(this, menu_selector(pvpWarReportLayer::acquireQward), 1);
            addChild(m_layer, 100);
        }
        else if(WAR_BACK_PLAY == tag)
        {
            m_pLayer = LayerColor::create(Color4B(0, 0, 0, 0));
            addChild(m_pLayer, 900);
            m_pLayer->setPosition(Vec2::ZERO);
            
            //CFightManager::addCloud(this,menu_selector(pvpWarReportLayer::Enemy),m_pLayer);
            
            m_iCurVideoId = m_allVideoInfo[idx].videoId;
            
            CPersonalData::getInstance()->setVideoId(m_iCurVideoId);
            
            CNetControlModule::sharedNetControlModule()->getDefenseVideoByVideoId(this, menu_selector(pvpWarReportLayer::getVideoInfo),
                                                                                  m_iCurVideoId);
        }
        else if(WAR_BTN_REVENAGE == tag)
        {
            
            pBackItem->setEnabled(false);
            
            CNetControlModule::sharedNetControlModule()->getEnemyTeamInfo(this, menu_selector(pvpWarReportLayer::getEnemyTeamInfoBack), m_allVideoInfo[idx].enemyUserId);
        }
    }
}

void pvpWarReportLayer::getEnemyTeamInfoBack(Ref *pSender)
{
    _netRecvEventGetUserTeamInfo* pInfo = (_netRecvEventGetUserTeamInfo*)pSender;
    std::vector<std::string>  p =  pInfo->m_paramsStr;
    int state = 1;
    int iAttack;
    pvpEnemyInfo pEnemyInfo;
    std::string enemyMaps;
    for(std::vector<std::string>::iterator it = pInfo->m_paramsStr.begin(); it != pInfo->m_paramsStr.end(); it++)
    {
        string str = *it;
        string name = CDataManager::getStrFromBackStr(str);
        
        if(name == "info")
        {
            state = atoi(CDataManager::getDigitFromBackStr(*it).c_str());
            if(state != 0)
            {
                log("获取玩家阵营信息失败");
                //CLayerManager::GetSingletonPtr()->createLayer(eLayer_PVPLayer, NULL,false);
                return;
            }
        }
        else if(name == "userId")
        {
            pEnemyInfo.m_iUserId = atoi(CDataManager::getDigitFromBackStr(*it).c_str());
            
        }
        else if(name == "name")
        {
            pEnemyInfo.m_sName = CDataManager::getDigitFromBackStr(*it);
            
            
            if(pEnemyInfo.m_sName == "null")
            {
                pEnemyInfo.m_sName = "无名氏";
            }
            
        }
        else if(name == "level")
        {
            pEnemyInfo.m_iLevel = atoi(CDataManager::getDigitFromBackStr(*it).c_str());
        }
        else if (name == "defenseMap")
        {
            enemyMaps = CDataManager::getDigitFromBackStr(*it);
            
        }
        else if(name == "robStone")
        {
            pEnemyInfo.m_iRobStone = atoi(CDataManager::getDigitFromBackStr(*it).c_str());
        }
        else if(name == "isattacked")
        {
            iAttack = atoi(CDataManager::getDigitFromBackStr(*it).c_str());
        }
        else if(name == "buidingbornTime")
        {
            std::string str = CDataManager::getDigitFromBackStr(*it).c_str();
            int ipos =  str.find(",");
            
            
            int iTime = atoi(str.substr(ipos+1,str.length()).c_str());
            
            if(iTime ==0)
            {
                iTime = 60;
            }
            
            
            CFightData::getInstance()->setTheBornTime(iTime);
            
            
        }
        
    }
    if(iAttack == 0)
    {
        m_allVideoInfo[m_idx].iAttack = 1;
        CPersonalData::getInstance()->setVideoId(m_allVideoInfo[m_idx].videoId);
    
        
        CPersonalData::getInstance()->setFromVideoList(true);
        CFightData::getInstance()->setThePvpEnemyInfo(pEnemyInfo);
        
        char buf[256];
        std::vector<std::string> tempValue;
        sprintf(buf, "%d", EPVPMode_PLAY);
        tempValue.push_back(buf);
        sprintf(buf, "%d", pEnemyInfo.m_iRobStone);
        tempValue.push_back(buf);
        tempValue.push_back(enemyMaps);
        
        CPersonalData::getInstance()->setPvpMainLayer(false);

        CFightData::getInstance()->setFightType(FIGHT_PVP_TYPE);
        
        CLayerManager::GetSingletonPtr()->createLayer(eLayer_FightPVP, NULL,false, true,&tempValue);
    }
    else if(iAttack == 1)
    {
        Sprite * thetarget = CFightManager::getTheFlyWord("该玩家当前在线，无法攻击","fonts/cuti.ttf");
        thetarget->setPosition(m_winSize.width*0.5,m_winSize.height*0.5);
        addChild(thetarget,100);
    }
    else if(iAttack == 2)
    {
        Sprite * thetarget = CFightManager::getTheFlyWord("该玩家正处于保护时间","fonts/cuti.ttf");
        thetarget->setPosition(m_winSize.width*0.5,m_winSize.height*0.5);
        addChild(thetarget,100);
    }
    return;
}

void pvpWarReportLayer::acquireQward(Ref* pSender)
{
    if(m_layer)
    {
        m_layer->removeFromParent();
        m_layer = NULL;
    }
    m_allVideoInfo[m_idx].iGetWard = 1;
    ((pvpMainLayer*)getParent())->updateVideoState(m_allVideoInfo[m_idx].videoId);
    CNetWorkDataSingle::GetSingletonPtr()->SaveUserMCF(this,menu_selector(pvpWarReportLayer::updateUserData),0,0,1);
    //CNetControlModule::sharedNetControlModule()->updateDefenseAward(this, NULL, m_allVideoInfo[m_idx].videoId, 5);
    m_tableView->updateCellAtIndex(m_idx);
    Sprite * thetarget = CFightManager::getTheFlyWord("获得1点体力值","fonts/cuti.ttf");
    thetarget->setPosition(Vec2(m_winSize.width / 2, m_winSize.height / 2));
    addChild(thetarget, 10);
}

void pvpWarReportLayer::updateUserData(Ref *pSender)
{
    CPersonalData::getInstance()->setUpdateUserInfo(true);
}

void pvpWarReportLayer::getVideoInfo(Ref* pSender)
{
    _netRecvEventGetDefenseVideoInfo* pInfo = (_netRecvEventGetDefenseVideoInfo*)pSender;
    int state = 0;
    std::string nemyMaps;
    std::string enemyInfo;
    std::string videoInfo;
    std::string strIsWin;
    
    for(std::vector<std::string>::iterator it = pInfo->m_paramsStr.begin(); it!= pInfo->m_paramsStr.end(); it++)
    {
        string str = *it;
        string name = CDataManager::getStrFromBackStr(str);
        if(name == "state")
        {
            state = atoi(CDataManager::getDigitFromBackStr(*it).c_str());
        }
        if(name == "defenseMap")
        {
            nemyMaps = CDataManager::getDigitFromBackStr(*it);
        }
        else if(name == "AttackTeam")
        {
            enemyInfo = CDataManager::getDigitFromBackStr(*it);
        }
        else if(name == "Video")
        {
            videoInfo = CDataManager::getDigitFromBackStr(*it);
        }
        /*else if(name == "isWin")
         {
         strIsWin = CDataManager::getDigitFromBackStr(*it);
         }*/
    }
    if(state == 1)
    {
        Sprite * thetarget = CFightManager::getTheFlyWord("录像不存在","fonts/cuti.ttf");
        thetarget->setPosition(m_winSize.width*0.5,m_winSize.height*0.5);
        addChild(thetarget,100);
        
        m_allVideoInfo[m_idx].bValid = 1;
        CDataManager::saveVideoById(m_allVideoInfo[m_idx].videoId);
        ((pvpMainLayer*)getParent())->getVideoListInfo();
        //更新显示数据
        m_tableView->updateCellAtIndex(m_idx);
        return;
    }
    CPersonalData::getInstance()->setFromVideoList(true);
    CFightData::getInstance()->setPvpReporterThis(this);
    char buf[256];
    
    std::vector<std::string> tempValue;
    sprintf(buf, "%d", EPVPMode_REC);
    tempValue.push_back(buf);
    sprintf(buf, "%d", m_stone);
    tempValue.push_back(buf);
    tempValue.push_back(nemyMaps);
    tempValue.push_back(enemyInfo);
    tempValue.push_back(videoInfo);
    CFightData::getInstance()->setFightType(FIGHT_PVP_TYPE);
    CPersonalData::getInstance()->setEnemyType(FIGHT_BACK_VIDEO);
    CPersonalData::getInstance()->setPvpMainLayer(false);

    CLayerManager::GetSingletonPtr()->createLayer(eLayer_FightPVP, NULL,false, true, &tempValue);
    
}


