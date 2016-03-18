//
//  SoldierLvUpLayer.cpp
//  TheGodWill
//
//  Created by prefer_shan on 14/11/25.
//
//

#include "SoldierSkillUpLayer.h"
#include "LvUpDetail.h"
#include "LYPersonalData.h"
#include "DBMgr.h"
#include "DataManager.h"
#include "SkillUpLayer.h"
#include "ArmLvUpLayer.h"
#include "LvUpDetail.h"
#include "sysTime.h"
#include "CHitDialog.h"
#include "DBMgr.h"
#include "SkillLvLayerQue.h"
#include "SkillUpMenuLayer.h"
#include "LYMenuItemSprite.h"
#include "SkillTouchDetail.h"


USING_NS_CC;
NS_CC_EXT_BEGIN

CSoldierSkillUpLayer::CSoldierSkillUpLayer(Sprite *pBg)
{
    m_iCurIdx = -1;
    m_pLvUpTime  = NULL;
    m_tableView = NULL;
    m_iIndex = 0;
    m_iYanjiuNum = 0;
    m_pListener = NULL;
    m_winSize = Director::getInstance()->getWinSize();
    m_pGuideMenu = NULL;
    m_pBg = pBg;
}

CSoldierSkillUpLayer::~CSoldierSkillUpLayer()
{
    
}


bool SortByRareLv(const FighterInfo *v1,const FighterInfo *v2)//注意：本函数的参数的类型一定要与vector中元素的类型一致
{
    
    if(v1->rare == v2->rare)
    {
        return v1->level>v2->level;
        
    }
    return v1->rare>v2->rare;
}

void CSoldierSkillUpLayer::onExit()
{
    _eventDispatcher->removeEventListener(m_pListener);
    CBaseLayer::onExit();
}

void CSoldierSkillUpLayer::addTableView()
{
    m_tableView = TableView::create(this, Size(m_winSize.width,m_winSize.height*0.7));
    Size winsize = Director::getInstance()->getWinSize();
    
    m_tableView->setDirection(ScrollView::Direction::VERTICAL);
    
    m_tableView->setPosition(Point(0,100));

    m_tableView->setDelegate(this);
    
    m_tableView->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    
    this->addChild(m_tableView,1);
}

bool CSoldierSkillUpLayer::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{

    log("touch = %f,%f\n",touch->getLocation().x,touch->getLocation().y);
    
    
    m_VnowLocation = touch->getLocation();
    
    CSKillMenuLayer *pLayer = (CSKillMenuLayer *)getParent();

    LYMenuItemSprite *p = (LYMenuItemSprite*)pLayer->m_pMenu->getChildByTag(CSKillMenuLayer::LvSkillTagSoldier);
    
    auto location = touch->getLocation();

    cocos2d::Rect rect = p->boundingBox();
    
    if(rect.containsPoint(location))
    {
        return false;
    }
    
    LYMenuItemSprite *p1 = (LYMenuItemSprite*)pLayer->m_pMenu->getChildByTag(CSKillMenuLayer::LVSkillTagHero);
    rect = p1->boundingBox();
    
    if(rect.containsPoint(location))
    {
        return false;
    }
    
    Sprite *pBg1 = (Sprite*)pLayer->getChildByTag(CSKillMenuLayer::LVBgTag);
    Menu *pMenClose  = (Menu*)pBg1->getChildByTag(CSKillMenuLayer::LvCloseTag);
    
    rect = pMenClose->boundingBox();
    
    if(rect.containsPoint(location))
    {
        return false;
    }
    

    Sprite *pNode = (Sprite*)getChildByTag(997);
    
    if(pNode)
    {
        pNode->removeFromParent();
    }
    
    
    return true;
}

void CSoldierSkillUpLayer::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event)
{

    
}

void CSoldierSkillUpLayer::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event)
{
    
}

void CSoldierSkillUpLayer::onTouchCancelled(Touch *touch, Event *event)
{


}

void CSoldierSkillUpLayer::OnSoldierLvup(Ref *pSender)
{
    log("兵种升级");
}

void CSoldierSkillUpLayer::OnExitLvLayer(Ref *pSender)
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


bool CSoldierSkillUpLayer::init()
{
    CBaseLayer::init();

    m_pListener = EventListenerTouchOneByOne::create();
    m_pListener->setSwallowTouches(true);
    m_pListener->onTouchBegan = CC_CALLBACK_2(CSoldierSkillUpLayer::onTouchBegan, this);
    m_pListener->onTouchMoved = CC_CALLBACK_2(CSoldierSkillUpLayer::onTouchMoved, this);
    m_pListener->onTouchEnded = CC_CALLBACK_2(CSoldierSkillUpLayer::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(m_pListener, this);

    
    std::vector<FighterInfo*>  pAll;
    
    pAll = CPersonalData::getInstance()->getAllSoldierInfo();
    
    for(int i=0;i<pAll.size();i++)
    {
        //1 英雄  0怪物
        if(pAll[i]->theType == 0&&pAll[i]->rare>0)
        {
            m_pSoldierVec.push_back(pAll[i]);
        }
    }
    
    std::sort(m_pSoldierVec.begin(),m_pSoldierVec.end(),SortByRareLv);
    
    Vec2 m_guidePos;
    
    m_tBg = m_pBg->getContentSize();
    
    Sprite *pLvUp = Sprite::createWithSpriteFrameName("lvqueen.png");
    pLvUp->setScale(0.5);
    pLvUp->setPosition(m_tBg.width*0.88,m_tBg.height*0.8);
    m_pBg->addChild(pLvUp,1);
    
    Label *pLvUpQueue = Label::createWithSystemFont("升级队列","Arial",30);
    Size tLv = pLvUpQueue->getContentSize();
    
    pLvUpQueue->setPosition(Vec2(tLv.width*1.2,tLv.height*0.6));
    pLvUp->addChild(pLvUpQueue);
    
    addTableView();

    if(m_pGuideMenu)
    {
        m_guidePos = m_pGuideMenu->convertToWorldSpace(Vec2::ZERO);
    }
    
    addChild(CSKillLvLayerQue::create(m_iIndex,m_pBg),10,9000);
    
    if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SKILL_UPGRADE) && (guiderManagerLayer::getInstance()->getStepNum() == 3))
    {
        guiderManagerLayer::getInstance()->setStepNum(guiderManagerLayer::getInstance()->getStepNum() + 1);
        guiderManagerLayer::getInstance()->createLayer(this);
        guiderManagerLayer::getInstance()->m_pos = m_guidePos;
    }
    return true;
}


void CSoldierSkillUpLayer::showSkillDetail()
{
    for(int i = 0; i < m_pSoldierVec.size(); i++)
    {
        if(CPersonalData::getInstance()->m_jumpInfo.monsterId == m_pSoldierVec[i]->monsterId)
        {
            LvUpDetail *pLayer = LvUpDetail::create((CSKillMenuLayer*)getParent(),i);
            getParent()->addChild(pLayer,1,100);
            pLayer->showSkillInfo();
            removeFromParent();
            break;
        }
    }
}

void CSoldierSkillUpLayer::addSkillDetail(int idx,int iTag)
{
    
    Sprite *pNode = (Sprite*)getChildByTag(997);
    
    if(pNode)
    {
        pNode->removeFromParent();
    }
    
    
    FighterInfo* fighterInfo  =  m_pSoldierVec[idx];
    
    CSKillTouchDetail *pLayer = CSKillTouchDetail::create(fighterInfo, iTag);
    
    addChild(pLayer,997,997);
 }

void CSoldierSkillUpLayer::tableCellTouched(TableView* table,TableViewCell* cell)
{
    ssize_t  idx = cell->getIdx();
    
    log("idx = %zi\n",idx);
    

    Sprite *pNode = (Sprite*)getChildByTag(997);
    
    if(pNode)
    {
        pNode->removeFromParent();
    }
    
    
    Sprite *pBg = (Sprite*)cell->getChildByTag(100);
    
    Sprite *p1 =   (Sprite*)pBg->getChildByTag(104);
    
    Sprite *p2 =  (Sprite*)pBg->getChildByTag(105);
    
    Sprite *p3 = (Sprite*)pBg->getChildByTag(106);
    
    Sprite *p4 = (Sprite*)pBg->getChildByTag(107);
    
    
    if(p1)
    {
        Rect  rect1 =  p1->boundingBox();

        rect1.origin = p1->convertToWorldSpace(Vec2::ZERO);

        if(rect1.containsPoint(m_VnowLocation))
        {
            addSkillDetail(idx,0);
        }
    }
    
    if(p2)
    {
        Rect rect2 = p2->boundingBox();
        rect2.origin = p2->convertToWorldSpace(Vec2::ZERO);
        

        if(rect2.containsPoint(m_VnowLocation))
        {
              addSkillDetail(idx,1);
        }
    }
    
    if(p3)
    {
        Rect rect3 = p3->boundingBox();
        rect3.origin = p3->convertToWorldSpace(Vec2::ZERO);
        if(rect3.containsPoint(m_VnowLocation))
        {
             addSkillDetail(idx,2);
        }
    }
    
    if(p4)
    {
        Rect rect4 = p4->boundingBox();
        rect4.origin = p4->convertToWorldSpace(Vec2::ZERO);
        if(rect4.containsPoint(m_VnowLocation))
        {
            addSkillDetail(idx,3);
        }
        
    }
}

void CSoldierSkillUpLayer::tableCellWillRecycle(TableView* table, TableViewCell* cell)
{
    
}

void CSoldierSkillUpLayer::tableCellHighlight(TableView* table,TableViewCell* cell)
{
    
}

void CSoldierSkillUpLayer::tableCellUnhighlight(TableView* table,TableViewCell* cell)
{
    
}

Size CSoldierSkillUpLayer::cellSizeForTable(TableView* table)
{
    Size visibSize = Director::getInstance()->getVisibleSize();
    return Size(visibSize.width, visibSize.height/4.327);
}


void CSoldierSkillUpLayer::OnMenuOpen(Ref *pSender)
{

    LYMenuItemSprite *pSprite = (LYMenuItemSprite*)pSender;
    long idx = (long)pSprite->getUserData();
    m_iIndex = idx;
    
    LvUpDetail *pLayer = LvUpDetail::create((CSKillMenuLayer*)getParent(), (int)idx);
    getParent()->addChild(pLayer,1,100);
    removeFromParent();
}


//生成cell
TableViewCell* CSoldierSkillUpLayer::tableCellAtIndex(TableView *table,ssize_t idx)
{
    FighterInfo *fighterInfo = m_pSoldierVec[idx];
    
    int  iMonsterId = fighterInfo->monsterId;
    int  iRare = fighterInfo->rare;
    
    monsterInfo* pMonsterInfo = CDataManager::getMonsterInfoByMonsterId(iMonsterId);
    
    std::vector<skillInfo*>  m_pskill = CDataManager::getAvailableSkillInfo(iMonsterId,iRare);
    
    TableViewCell *cell = table->dequeueCell();

    if (!cell)
    {
        cell = new TableViewCell();
        cell->autorelease();
    }
    
    {
        cell->removeAllChildren();
        
        Sprite *pBg =  Sprite::createWithSpriteFrameName("choosebg2.png");
        int iHeight = 100;
        
        pBg->setPosition(m_winSize.width*0.45,iHeight);
        cell->addChild(pBg,0,100);
        
        Size tBg = pBg->getContentSize();
        
        Sprite *pNorSoldier = CDataManager::getMonsterIconAndBg(pMonsterInfo->monsterId,true);
        pNorSoldier->setPosition(tBg.width*0.12,tBg.height*0.5);
        pNorSoldier->setScale(3.5);
        pBg->addChild(pNorSoldier,0,101);
        
        Size  tSoldierSize = pNorSoldier->getContentSize();
        Sprite * pType = CDBMgr::getInstance()->GetShareSpriteByFigtherType(pMonsterInfo->monsterType);
        pType->setScale(0.25);
        pType->setPosition(tSoldierSize.width*0.25,tSoldierSize.height*0.2);
        pNorSoldier->addChild(pType);

        
        //姓名
        Label *pLvName = Label::createWithSystemFont(pMonsterInfo->monsterName,"Aril",30);
        pLvName->setAnchorPoint(Vec2(0,0.5));
        pLvName->setColor(Color3B(207,109,34));
        pLvName->setPosition(tBg.width*0.25,tBg.height*0.8);
        pBg->addChild(pLvName,0,102);
        
        char strLv[100] = {0};
        sprintf(strLv,"LV%d",fighterInfo->level);
        
        //等级
        Label *pLv = Label::createWithSystemFont(strLv,"Aril",30);
        pLv->setColor(Color3B(207,109,34));
        pLv->setPosition(tBg.width*0.6,tBg.height*0.8);
        pBg->addChild(pLv,0,103);

        Label *pSKill = Label::createWithSystemFont("技能","Aril",30);
        pSKill->setColor(Color3B(207,109,34));
        pSKill->setAnchorPoint(Vec2(0,0.5));
        pSKill->setPosition(tBg.width*0.25,tBg.height*0.3);
    
        pBg->addChild(pSKill);
        
        bool  bTrue = false;
        
        //技能1
        for(int i=0;i<m_pskill.size();i++)
        {
            Sprite *pSkill1 = CDataManager::getSkillIcoWithId(m_pskill[i]);
            pSkill1->setScale(1.5);
     
            pSkill1->setPosition(tBg.width*0.4+tBg.width*0.08*i,tBg.height*0.3);
            
             pBg->addChild(pSkill1,0,104+i);
            
            bool bSkill = CDBMgr::getInstance()->IsSkillCanLvUp(iMonsterId,m_pskill[i]->skillId,false,m_pSoldierVec[idx]->theFighterSkillInfo[i].skillLevel);
            
            if(bSkill)
            {
                bTrue = true;
                Sprite *pGreenPlus = Sprite::createWithSpriteFrameName("RedPoint.png");
                pGreenPlus->setScale(0.4);
                Size t1 = pSkill1->getContentSize();
                pGreenPlus->setPosition(t1.width*0.8,t1.height*0.8);
                pSkill1->addChild(pGreenPlus);

            }
        }
    
        Sprite *pLvUp = Sprite::createWithSpriteFrameName("greenbtn.png");
        Sprite *pLvUpSelect = Sprite::createWithSpriteFrameName("greenbtn.png");
        pLvUpSelect->setColor(Color3B::GRAY);
        LYMenuItemSprite *pItemSprite = LYMenuItemSprite::create(pLvUp,pLvUpSelect,CC_CALLBACK_1(CSoldierSkillUpLayer::OnMenuOpen,this));
        pItemSprite->setScale(3);
        pItemSprite->setTag(110);
        pItemSprite->setUserData((void *)idx);
        
        

        Menu *pMenuSoldier = Menu::create(pItemSprite,NULL);
        pMenuSoldier->setPosition(tBg.width*0.83,tBg.height*0.5);
        pBg->addChild(pMenuSoldier);
        
        if(pMonsterInfo->monsterId == 1)
        {
            m_pGuideMenu = pMenuSoldier;
            pCell = cell;
        }
        
        Size tLvUp = pItemSprite->getContentSize();
    
        
        Label *pLvUpLabel = Label::createWithTTF("技能升级","fonts/cuti.ttf",25);
        pLvUpLabel->enableOutline(Color4B(0,0,0,255),3);
        pLvUpLabel->setScale(0.4);
        pLvUpLabel->setPosition(tLvUp.width*0.5,tLvUp.height*0.5);
        pItemSprite->addChild(pLvUpLabel);
        
    }
    
    return cell;
}


//cell的数量
ssize_t CSoldierSkillUpLayer::numberOfCellsInTableView(TableView *table)
{
    return m_pSoldierVec.size();
}



NS_CC_EXT_END