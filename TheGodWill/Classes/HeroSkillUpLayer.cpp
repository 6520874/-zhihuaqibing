//
//  HeroSkillUpLayer.cpp
//  TheGodWill
//
//  Created by prefer_shan on 14/12/29.
//
//

#include "HeroSkillUpLayer.h"
#include "LvUpDetail.h"
#include "LYPersonalData.h"
#include "DBMgr.h"
#include "DataManager.h"
#include "SkillUpLayer.h"
#include "ArmLvUpLayer.h"
#include "LvUpDetail.h"
#include "sysTime.h"
#include "CHitDialog.h"
#include "SkillLvLayerQue.h"
#include "Common.h"
#include "SkillTouchDetail.h"

USING_NS_CC;
NS_CC_EXT_BEGIN

CHeroSkillUpLayer::CHeroSkillUpLayer(Sprite *pBg)
{
    m_pBg = pBg;
    m_iCurIdx = -1;
    m_iYanjiuNum = 0;
    m_pLvUpTime  = NULL;
    m_iHour = 1;
    m_iSec  = 0;
    m_bHero = true;
    m_iHour2 = 0;
    m_iSec2 = 0;
    m_pLabelLvUpTime = NULL;
    m_tableView = NULL;
    m_iSkillInex = 0;
    m_iIndex = 0;
    m_winSize = Director::getInstance()->getWinSize();
}

CHeroSkillUpLayer::~CHeroSkillUpLayer()
{
    
}

void CHeroSkillUpLayer::onExit()
{
    _eventDispatcher->removeEventListener(m_pListener);
    
    CBaseLayer::onExit();
}


bool CHeroSkillUpLayer::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    Sprite *pBg = (Sprite*)getChildByTag(999);
    
    if(pBg)
    {
        pBg->removeFromParent();
    }
    
    Sprite *pNode = (Sprite*)getChildByTag(998);
    
    if(pNode)
    {
        pNode->removeFromParent();
    }

    
    pNode = (Sprite*)getChildByTag(997);
    
    if(pNode)
    {
        pNode->removeFromParent();
    }
    
    CSKillMenuLayer *pLayer = (CSKillMenuLayer *)getParent();
    auto location = touch->getLocation();
    
    Sprite *pBg1 = (Sprite*)pLayer->getChildByTag(CSKillMenuLayer::LVBgTag);
    Menu *pMenClose  = (Menu*)pBg1->getChildByTag(CSKillMenuLayer::LvCloseTag);
    
    Rect rect = pMenClose->boundingBox();
    
    if(rect.containsPoint(location))
    {
        return false;
    }

    
    
    return true;
}

void CHeroSkillUpLayer::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event)
{

}

void CHeroSkillUpLayer::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event)
{


}

void CHeroSkillUpLayer::onTouchCancelled(Touch *touch, Event *event)
{


}



void  CHeroSkillUpLayer::addTableView()
{
    m_tableView = TableView::create(this, Size(m_winSize.width,m_winSize.height*0.7));
    
    Size winsize = Director::getInstance()->getWinSize();
    
    m_tableView->setDirection(ScrollView::Direction::VERTICAL);
    
    m_tableView->setPosition(Point(0,100));
    
    m_tableView->setDelegate(this);
    
    m_tableView->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    
    this->addChild(m_tableView,1);
}


void CHeroSkillUpLayer::OnSoldierLvup(Ref *pSender)
{
    log("兵种升级");
}


bool SortLv(const FighterInfo *v1,const FighterInfo *v2)//注意：本函数的参数的类型一定要与vector中元素的类型一致
{
    
    if(v1->rare == v2->rare)
    {
        return v1->level>v2->level;
        
    }
    return v1->rare>v2->rare;
}

bool CHeroSkillUpLayer::init()
{
    CBaseLayer::init();
    
    m_pListener = EventListenerTouchOneByOne::create();
    m_pListener->setSwallowTouches(true);
    m_pListener->onTouchBegan = CC_CALLBACK_2(CHeroSkillUpLayer::onTouchBegan, this);
    m_pListener->onTouchMoved = CC_CALLBACK_2(CHeroSkillUpLayer::onTouchMoved, this);
    m_pListener->onTouchEnded = CC_CALLBACK_2(CHeroSkillUpLayer::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(m_pListener, this);
    
    std::vector<FighterInfo*>  pAll;
    
    pAll = CPersonalData::getInstance()->getAllSoldierInfo();
    
    bool bGril =  CommonData::GetSingletonPtr()->IsGirlHeroVisable();
    
    for(int i=0;i<pAll.size();i++)
    {
        //1 英雄  0怪物
        if(pAll[i]->theType == 1)
        {
            if(pAll[i]->monsterId == 1005)
            {
                if(!bGril)
                {
                    continue;
                }
            }
        
            m_pSoldierVec.push_back(pAll[i]);
        }
    }
    std::sort(m_pSoldierVec.begin(),m_pSoldierVec.end(),SortLv);
    
    
    Size tBg = m_pBg->getContentSize();
    
    Sprite *pLvUp = Sprite::createWithSpriteFrameName("lvqueen.png");
    pLvUp->Node::setScale(0.5);
    pLvUp->setPosition(tBg.width*0.88,tBg.height*0.8);
    m_pBg->addChild(pLvUp,1);
    
    Label *pLvUpQueue = Label::createWithSystemFont("升级队列","Arial",30);
    Size tLv = pLvUpQueue->getContentSize();
    pLvUpQueue->setPosition(Vec2(tLv.width*1.2,tLv.height*0.6));
    pLvUp->addChild(pLvUpQueue);
    
    addTableView();
    
   
    CSKillLvLayerQue *pSkillLayer = CSKillLvLayerQue::create(m_iIndex,m_pBg);
    
    addChild(pSkillLayer,200,9000);
    
    return true;
}

void CHeroSkillUpLayer::showSkillDetail()
{
    for(int i = 0; i < m_pSoldierVec.size(); i++)
    {
        if(CPersonalData::getInstance()->m_jumpInfo.monsterId == m_pSoldierVec[i]->monsterId)
        {
            LvUpDetail *pLayer = LvUpDetail::create((CSKillMenuLayer*)getParent(), i,true);
            getParent()->addChild(pLayer,100,101);
            pLayer->showSkillInfo();
            removeFromParent();
        }
    }
}

void CHeroSkillUpLayer::tableCellTouched(TableView* table,TableViewCell* cell)
{
    Sprite *pBg = (Sprite*)getChildByTag(999);
    
    if(pBg)
    {
        pBg->removeFromParent();
    }
    
    Sprite *pNode = (Sprite*)getChildByTag(998);
    
    if(pNode)
    {
        pNode->removeFromParent();
    }

}


void CHeroSkillUpLayer::tableCellWillRecycle(TableView* table, TableViewCell* cell)
{
    
}

void CHeroSkillUpLayer::tableCellHighlight(TableView* table,TableViewCell* cell)
{
    
}

void CHeroSkillUpLayer::tableCellUnhighlight(TableView* table,TableViewCell* cell)
{
    
}

Size CHeroSkillUpLayer::cellSizeForTable(TableView* table)
{
    Size visibSize = Director::getInstance()->getVisibleSize();
    return Size(visibSize.width, visibSize.height/4.327);
}


void CHeroSkillUpLayer::OnMenuOpen(Ref *pSender)
{
    Sprite *pBg = (Sprite*)getChildByTag(999);
    
    if(pBg)
    {
        pBg->removeFromParent();
    }
    
    Sprite *pNode = (Sprite*)getChildByTag(998);
    
    if(pNode)
    {
        pNode->removeFromParent();
    }

    
    LYMenuItemSprite *pSprite = (LYMenuItemSprite*)pSender;
    
    long idx =  (long)pSprite->getUserData();
    
    if((idx == 0) && (CPersonalData::getInstance()->getUserInfo()->m_iSmallLevel >= 9) && (!CDataManager::getEndedBy(GUIDE_BOY_HERO_SKILL)))
    {
        CDataManager::saveGuideInfoToDB(GUIDE_BOY_HERO_SKILL);
    }
    if((idx == 1) && (CPersonalData::getInstance()->getUserInfo()->m_iSmallLevel >= 23) && (!CDataManager::getEndedBy(GUIDE_GIRL_HERO_SKILL)))
    {
        CDataManager::saveGuideInfoToDB(GUIDE_GIRL_HERO_SKILL);
    }
    
    LvUpDetail *pLayer = LvUpDetail::create((CSKillMenuLayer*)getParent(), idx,true);
    getParent()->addChild(pLayer,100,101);
    removeFromParent();

}


//生成cell
TableViewCell* CHeroSkillUpLayer::tableCellAtIndex(TableView *table,ssize_t idx)
{
    FighterInfo *fighterInfo =   m_pSoldierVec[idx];
    monsterInfo* pMonsterInfo = CDataManager::getMonsterInfoByMonsterId(fighterInfo->monsterId);
    
    vector<heroSkill*> pHero = CDBMgr::getInstance()->getHeroSkillByMonsterId(fighterInfo->monsterId);

    vector<heroSkill*> pVec;
    
    for(int i=0;i<pHero.size();i++)
    {
        
        if(pHero[i]->heroSkillId !=1)
        {
            pVec.push_back(pHero[i]);
            
        }
    }

    
    
    TableViewCell *cell = table->dequeueCell();
    if (!cell)
    {
        cell = new TableViewCell();
        cell->autorelease();
        
    }
    
    {
        
        cell->removeAllChildren();
        
        m_iCurIdx = idx;
        
        int iHeight = 100;
        
        Sprite *pBg =  Sprite::createWithSpriteFrameName("choosebg2.png");
        
        pBg->setPosition(m_winSize.width*0.45,iHeight);
        
        cell->addChild(pBg,0,100);
        
        Size tBg = pBg->getContentSize();
        
        //队列
        Sprite *pNorSoldier =  CDataManager::getMonsterIconAndBg(pMonsterInfo->monsterId,true);
        pNorSoldier->setPosition(tBg.width*0.12,tBg.height*0.5);
          pNorSoldier->setScale(3.5);
        pBg->addChild(pNorSoldier,0,101);
        
        //姓名
        Label *pLvName = Label::createWithSystemFont(pMonsterInfo->monsterName,"Aril",30);
         pLvName->setAnchorPoint(Vec2(0,0.5));
        pLvName->setPosition(tBg.width*0.25,tBg.height*0.8);
        pLvName->setColor(Color3B(207,109,34));
        
        pBg->addChild(pLvName,0,102);
        
        char strLv[100] = {0};
        sprintf(strLv,"LV%d",fighterInfo->level);
        
        //等级
        Label *pLv = Label::createWithSystemFont(strLv,"Aril",30);
        pLv->setColor(Color3B(207,109,34));
        pLv->setAnchorPoint(Vec2(0,0.5));
        pLv->setPosition(tBg.width*0.55,tBg.height*0.8);
        pBg->addChild(pLv,0,103);
        
        //技能1
        for(int i=0;i<pVec.size();i++)
        {
            char strSKill[50] = {0};
            sprintf(strSKill,"%s",pVec[i]->skillIcon.c_str());
        
            Sprite *pSkill1 = Sprite::createWithSpriteFrameName(strSKill);
        
            MenuItemSprite *pSkillMenuItem = MenuItemSprite::create(pSkill1,NULL,CC_CALLBACK_1(CHeroSkillUpLayer::OnSoldierSKillUp, this));
            pSkillMenuItem->setScale(1.5);
            pSkillMenuItem->setTag(i);
            pSkillMenuItem->setUserData((void*)idx);
            
            Menu *pMenu = Menu::create(pSkillMenuItem,NULL);
            pMenu->setPosition(tBg.width*0.45+tBg.width*0.1*i,tBg.height*0.3);
            pBg->addChild(pMenu,104);
            
            int iLv = CDBMgr::getInstance()->GetHeroSkillLvById(pVec[i]->heroSkillId);

            bool bSkill = CDBMgr::getInstance()->IsSkillCanLvUp(pMonsterInfo->monsterId,pVec[i]->heroSkillId,true,iLv);
         
            if(bSkill)
            {
                Sprite *pGreenPlus = Sprite::createWithSpriteFrameName("plus.png");
                pGreenPlus->setScale(0.25);
                Size t1 = pSkill1->getContentSize();
                pGreenPlus->setPosition(t1.width*0.1,t1.height*0.8);
                pSkill1->addChild(pGreenPlus);
            }
        }
    
        Label *pSKill = Label::createWithSystemFont("技能","Aril",30);
        pSKill->setColor(Color3B(207,109,34));
        pSKill->setAnchorPoint(Vec2(0,0.5));
        pSKill->setPosition(tBg.width*0.25,tBg.height*0.3);
        pBg->addChild(pSKill);

        Sprite *pLvUp = Sprite::createWithSpriteFrameName("greenbtn.png");
        Sprite *pLvUpSelect = Sprite::createWithSpriteFrameName("greenbtn.png");
        pLvUpSelect->setColor(Color3B::GRAY);
        
        LYMenuItemSprite *pItemSprite = LYMenuItemSprite::create(pLvUp,pLvUpSelect,CC_CALLBACK_1(CHeroSkillUpLayer::OnMenuOpen,this));
        pItemSprite->setScale(3);
        pItemSprite->setTag(110);
        pItemSprite->setUserData((void *)idx);
        
        Menu *pMenuSoldier = Menu::create(pItemSprite,NULL);
        pMenuSoldier->setPosition(tBg.width*0.83,tBg.height*0.5);
        pBg->addChild(pMenuSoldier,0,106);
        
        Size tLvUp = pLvUp->getContentSize();
        
        Label *pLvUpLabel = Label::createWithTTF("技能升级","fonts/cuti.ttf",25);
        pLvUpLabel->enableOutline(Color4B(0,0,0,255),3);

         pLvUpLabel->setScale(0.4);
        pLvUpLabel->setPosition(pItemSprite->getContentSize().width*0.5,pItemSprite->getContentSize().height*0.5);
        pItemSprite->addChild(pLvUpLabel,0,107);
        if((pMonsterInfo->monsterId == 1004) && (CPersonalData::getInstance()->getUserInfo()->m_iSmallLevel >= 9)
           && (!CDataManager::getEndedBy(GUIDE_BOY_HERO_SKILL)))
        {
            Sprite *pRedPoint = Sprite::createWithSpriteFrameName("newTip.png");
            pItemSprite->addChild(pRedPoint, 1, 50);
            pRedPoint->setScale(0.45f);
            Vec2 pos = Vec2(pItemSprite->getNormalImage()->getContentSize().width * 0.785f,
                            pItemSprite->getNormalImage()->getContentSize().height * 0.88f);
            pRedPoint->setPosition(pos);
        }
        if((pMonsterInfo->monsterId == 1005) && (CPersonalData::getInstance()->getUserInfo()->m_iSmallLevel >= 23)
           && (!CDataManager::getEndedBy(GUIDE_GIRL_HERO_SKILL)))
        {
            Sprite *pRedPoint = Sprite::createWithSpriteFrameName("newTip.png");
            pItemSprite->addChild(pRedPoint, 1, 60);
            pRedPoint->setScale(0.45f);
            Vec2 pos = Vec2(pItemSprite->getNormalImage()->getContentSize().width * 0.785f,
                            pItemSprite->getNormalImage()->getContentSize().height * 0.88f);
            pRedPoint->setPosition(pos);
        }
        
    }
    return cell;
}


void CHeroSkillUpLayer::OnSoldierSKillUp(Ref *pSender)
{
    MenuItemSprite * p   = (MenuItemSprite*)pSender;
    int iTag = p->getTag();
    
    long idx = (long )p->getUserData();
        
    Sprite *pNode = (Sprite*)getChildByTag(997);
    
    if(pNode)
    {
        pNode->removeFromParent();
    }

    
    addSkillDetail(idx,iTag);

}


void CHeroSkillUpLayer::addSkillDetail(int idx,int iTag)
{
    FighterInfo* fighterInfo  =  m_pSoldierVec[idx];
    
    //int  iMonsterId = fighterInfo->monsterId;
    //int  iRare = fighterInfo->rare;

    
    CSKillTouchDetail *pLayer = CSKillTouchDetail::create(fighterInfo, iTag);
    
    addChild(pLayer,997,997);
}



//cell的数量
ssize_t CHeroSkillUpLayer::numberOfCellsInTableView(TableView *table)
{
    return m_pSoldierVec.size();
}

NS_CC_EXT_END