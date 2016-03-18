//
//  SoldierLvUpLayer.cpp
//  TheGodWill
//
//  Created by prefer_shan on 14/11/25.
//
//

#include "SoldierLvUpLayer.h"
#include "LvUpDetail.h"
#include "LYPersonalData.h"
#include "DBMgr.h"
#include "DataManager.h"
#include "SkillUpLayer.h"
#include "ArmLvUpLayer.h"
#include "guiderManagerLayer.h"
#include "LvIntroduce.h"
#include "LYFightManager.h"
#include "LYFightData.h"
USING_NS_CC;
NS_CC_EXT_BEGIN

bool SortByM1(const FighterInfo *v1,const FighterInfo *v2)//注意：本函数的参数的类型一定要与vector中元素的类型一致
{
    
    if(v1->rare == v2->rare)
    {
        return v1->level>v2->level;
        
    }
    return v1->rare>v2->rare;
    
}


CSoldierLvUpLayer::CSoldierLvUpLayer()
{
    m_iCurIdx = -1;
    m_pLvUpTime  = NULL;
    m_iHour = 1;
    m_iSec  = 0;
    m_pLabelLvUpTime = NULL;
    m_winSize = Director::getInstance()->getWinSize();
    m_pGuideMenu = NULL;
    m_pListener =  NULL;
}

CSoldierLvUpLayer::~CSoldierLvUpLayer()
{
    
}

void CSoldierLvUpLayer::onExit()
{
    Layer::onExit();
    NotificationCenter::getInstance()->removeObserver(this, "soliderGuide");
}


bool CSoldierLvUpLayer::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    log("dssddsdsds\n");
    
    return true;
}

void CSoldierLvUpLayer::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event)
{

}

void CSoldierLvUpLayer::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event)
{

}

void CSoldierLvUpLayer::onTouchCancelled(Touch *touch, Event *event)
{

}


void  CSoldierLvUpLayer::addTableView()
{
    m_tableView = TableView::create(this, Size(m_winSize.width,m_winSize.height*0.7));
    
    Size winsize = Director::getInstance()->getWinSize();
    
    m_tableView->setDirection(ScrollView::Direction::VERTICAL);
    
    m_tableView->setPosition(Point(0,100));
    
    m_tableView->setDelegate(this);
    
    m_tableView->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    
    this->addChild(m_tableView,1);
}


void CSoldierLvUpLayer::OnSoldierLvup(Ref *pSender)
{
    log("兵种升级");
}

void CSoldierLvUpLayer::addSoliderGuide(Ref* pSender)
{
    //保存引导进度
    if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_UPGRADE_SOLIDER) && (guiderManagerLayer::getInstance()->getStepNum() == 5))
    {
        guiderManagerLayer::getInstance()->setStepNum(guiderManagerLayer::getInstance()->getStepNum() + 1);
        guiderManagerLayer::getInstance()->createLayer(this);
    }
}




bool CSoldierLvUpLayer::init()
{
    Layer::init();

    
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(CSoldierLvUpLayer::addSoliderGuide), "soliderGuide", NULL);
    m_pAll = CPersonalData::getInstance()->getAllSoldierInfo();
    

    for(int i=0;i<m_pAll.size();i++)
    {
        //1 英雄  0怪物
        if(m_pAll[i]->theType == 0&&m_pAll[i]->rare>0)
        {
            m_pSoldierVec.push_back(m_pAll[i]);
        }
    }
    
    std::sort(m_pSoldierVec.begin(),m_pSoldierVec.end(),SortByM1);
    
    
    //士兵等级介绍
    
    Sprite *pSl = Sprite::createWithSpriteFrameName("wh.png");
   
    Sprite *pSlSelect = Sprite::createWithSpriteFrameName("wh.png");
    pSlSelect->setColor(Color3B::GRAY);
    
    MenuItemSprite *pMenuItemSprite  = MenuItemSprite::create(pSl,pSlSelect,CC_CALLBACK_1(CSoldierLvUpLayer::OnMenuQuestion,this));
    
    Menu  * pMenu = Menu::create(pMenuItemSprite,NULL);
    pMenu->setPosition(m_winSize.width*0.15,m_winSize.height*0.85);
    addChild(pMenu,111);
    

    addTableView();
    if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_UPGRADE_SOLIDER) && (guiderManagerLayer::getInstance()->getStepNum() == 2))
    {
        Vec2 m_guidePos = m_pGuideMenu->convertToWorldSpace(Vec2::ZERO);
        guiderManagerLayer::getInstance()->setStepNum(guiderManagerLayer::getInstance()->getStepNum() + 1);
        guiderManagerLayer::getInstance()->createLayer(this, GUIDE_SHOW_FINGER, m_guidePos);
    }
    return true;
}


void CSoldierLvUpLayer::OnMenuQuestion(Ref *pSender)
{
    log("onMenuQuestion\n");
    
    getParent()->addChild(CLvIntroduce::create(),111);
    
    
  //  CLvIntroduce::create();
    
    
}

void CSoldierLvUpLayer::UpdateTime(float ft)
{
    char strTime[50] = {0};
    
    if(m_iSec>0)
    {
        m_iSec -=1;
    }
    else if(m_iHour>0)
    {
        m_iHour -=1;
        m_iSec = 59;
    }
    
    sprintf(strTime,"(%.2d:%.2d)",m_iHour,m_iSec);
    m_pLabelLvUpTime->setString(strTime);
    
    float  iTime = m_iHour*60+m_iSec;
    
    float fHave = iTime/60*100;
    
    float iPer = 100-fHave;
    
    m_pProgress->setPercentage(iPer);
}


void CSoldierLvUpLayer::tableCellTouched(TableView* table,TableViewCell* cell)
{
    log("touch");
    
}

void CSoldierLvUpLayer::tableCellWillRecycle(TableView* table, TableViewCell* cell)
{
    
}

void CSoldierLvUpLayer::tableCellHighlight(TableView* table,TableViewCell* cell)
{
    
}

void CSoldierLvUpLayer::tableCellUnhighlight(TableView* table,TableViewCell* cell)
{
    
}


Size CSoldierLvUpLayer::cellSizeForTable(TableView* table)
{
    Size visibSize = Director::getInstance()->getVisibleSize();
    return Size(visibSize.width, visibSize.height/4.327);
}


void CSoldierLvUpLayer::OnRemoveNotice()
{
    Label *pBuildingUp =  (Label*)getChildByTag(1000);
    pBuildingUp->removeFromParent();
}

void  CSoldierLvUpLayer::OnMenuOpen(Ref *pSender)
{
//    if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_UPGRADE_SOLIDER) && (guiderManagerLayer::getInstance()->getStepNum() == 8))
//    {
//        CCLOG("你妹!!!!!!!!");
//        return;
//    }
//    else
//    if((CFightData::getInstance()->getNowLevelId() >= 6) && (guiderManagerLayer::getInstance()->getGuideType() == GUIDE_FAIL_TIP)
//            && (guiderManagerLayer::getInstance()->getStepNum() == 3))
//    {
//        CFightData::getInstance()->setNowLevelId(0);//初始化避免重复引导
//        guiderManagerLayer::getInstance()->removeGuiderLayer();
//        guiderManagerLayer::getInstance()->setStepNum(0);
//    }
    LYMenuItemSprite *pSprite = (LYMenuItemSprite*)pSender;
    long  idx= (long)pSprite->getUserData();

    int iUserLv = CPersonalData::getInstance()->getUserInfo()->m_iUserLevel;
    
    FighterInfo *fighterInfo = m_pSoldierVec[idx];

    if((fighterInfo->level+1)>= iUserLv*2)
    {

        Sprite *pRedManji = Sprite::createWithSpriteFrameName("redbtn.png");
        pSprite->setNormalImage(pRedManji);
        pRedManji->setAnchorPoint(Vec2(0,0));
        
        Sprite *pRedSelect = Sprite::createWithSpriteFrameName("redbtn.png");
        pSprite->setSelectedImage(pRedSelect);
        pRedSelect->setAnchorPoint(Vec2(0,0));
        pRedSelect->setColor(Color3B::GRAY);
        
        Label *pLabelManji = Label::createWithTTF("满级","fonts/cuti.ttf",30);
        pLabelManji->enableOutline(Color4B(0,0,0,255),3);
         pLabelManji->setScale(0.4);
         Size tSprite = pSprite->getContentSize();
         pLabelManji->setPosition(tSprite.width*0.5,tSprite.height*0.5);
         pSprite->addChild(pLabelManji);
    }
    
    if(fighterInfo->level>= iUserLv*2)
    {
        SimpleAudioEngine::getInstance()->playEffect("music/noBtn.mp3");
        
        CLvIntroduce *pLayer = CLvIntroduce::create();
        
        Sprite *Pbg = (Sprite *)pLayer->getChildByTag(100);
        
        Pbg->setPosition(m_winSize.width*0.15,m_winSize.height*0.8);
        
        Pbg->setScale(0);
        
        addChild(pLayer,100);
        
       
        Pbg->runAction(Spawn::create(
                                         MoveTo::create(0.3,Vec2(m_winSize.width*0.5,m_winSize.height*0.5)),ScaleTo::create(0.3,1),NULL));
        
        
        return;
    }
    
    CArmLvUpLayer *pLayer = CArmLvUpLayer::create(m_pSoldierVec[idx],(int)idx);
    getParent()->addChild(pLayer,2);
    
}


//生成cell
TableViewCell* CSoldierLvUpLayer::tableCellAtIndex(TableView *table,ssize_t idx)
{
    TableViewCell *cell = table->dequeueCell();
    
    FighterInfo *fighterInfo =   m_pSoldierVec[idx];
    monsterInfo* pMonsterInfo = CDataManager::getMonsterInfoByMonsterId(fighterInfo->monsterId);
    
    int  iMonsterId = fighterInfo->monsterId;
    int  iRare =  fighterInfo->rare;
    
    std::vector<skillInfo*>  m_pskill = CDataManager::getAvailableSkillInfo(iMonsterId,iRare);

    
    if (!cell)
    {
        cell = new TableViewCell();
        cell->autorelease();
        
    }
    
    {
        cell->removeAllChildren();
        
        int iHeight = 85;
        
        Sprite *pBg =  Sprite::createWithSpriteFrameName("choosebg.png");
        pBg->setPosition(m_winSize.width*0.5,iHeight);
        pBg->setScale(1.86);
        cell->addChild(pBg,0,100);
        
        Size tBg = pBg->getContentSize();//pBg->boundingBox().size;
        
        log("%d\n",pMonsterInfo->monsterId);
        
        Sprite *pNorSoldier = CDataManager::getMonsterIconAndBg(pMonsterInfo->monsterId,true);
        
        int iCellWidth =  tBg.width*0.12;
        int iCellHeight = tBg.height*0.5;
        
        pNorSoldier->Node::setScale(2);
        pNorSoldier->setPosition(iCellWidth,iCellHeight);
        pBg->addChild(pNorSoldier,0,100);
        
        
        Size  tSoldierSize = pNorSoldier->getContentSize();
        
        Sprite * pType = CDBMgr::getInstance()->GetShareSpriteByFigtherType(pMonsterInfo->monsterType);
        pType->setScale(0.25);
        pType->setPosition(tSoldierSize.width*0.25,tSoldierSize.height*0.2);
        pNorSoldier->addChild(pType);

        
        //姓名
        Label *pLvName = Label::createWithSystemFont(pMonsterInfo->monsterName,"Aril",30);
        pLvName->setAnchorPoint(Vec2(0,0.5));
        pLvName->setScale(0.6);
      
        pLvName->setPosition(tBg.width*0.2,tBg.height*0.745);
        pLvName->setColor(Color3B(209,108,36));
        pBg->addChild(pLvName,0,102);
        
        char strLv[100] = {0};
        sprintf(strLv,"LV%d",fighterInfo->level);
        
        //等级
        Label *pLv = Label::createWithSystemFont(strLv,"Aril",30);
        pLv->setColor(Color3B(245,154,81));
        pLv->setScale(0.6);
        pLv->setAnchorPoint(Vec2(0,0.5));
        pLv->setPosition(pLvName->getPositionX()+pLvName->boundingBox().size.width*1.35,tBg.height*0.745);
        pBg->addChild(pLv,0,103);
        
    

        std::string strPng = "greenbtn.png";

        std::string strLableJi = "升级";

        int iUserLv = CPersonalData::getInstance()->getUserInfo()->m_iUserLevel;
        
        FighterInfo *fighterInfo = m_pSoldierVec[idx];

        if((fighterInfo->level)>= iUserLv*2)
        {
            strPng = "redbtn.png";
            strLableJi = "满级";
        }
        
        
        Sprite *pLvUp = Sprite::createWithSpriteFrameName(strPng);
        Sprite *pLvUpSelect = Sprite::createWithSpriteFrameName(strPng);
        pLvUpSelect->setColor(Color3B::GRAY);
        Sprite *pLvUpDisabel = Sprite::createWithSpriteFrameName(strPng);
        pLvUpDisabel->setColor(Color3B::GRAY);
        
        LYMenuItemSprite *pItemSprite = LYMenuItemSprite::create(pLvUp,pLvUpSelect,pLvUpDisabel,CC_CALLBACK_1(CSoldierLvUpLayer::OnMenuOpen,this));
        pItemSprite->setTag(110);
        pItemSprite->setScale(2);
        pItemSprite->setUserData((void*)idx);
        
        Menu *pMenuSoldier = Menu::create(pItemSprite,NULL);
        pMenuSoldier->setPosition(tBg.width*0.8,tBg.height*0.5);
        pBg->addChild(pMenuSoldier,0,106);
        
        if(pMonsterInfo->monsterId == 2)
        {
            m_pGuideMenu = pMenuSoldier;
        }
        
        Size tLvUp = pItemSprite->getContentSize();
        
        Label *pLvUpLabel = Label::createWithTTF(strLableJi,"fonts/cuti.ttf",30);
        pLvUpLabel->enableOutline(Color4B(0,0,0,255),3);
        pLvUpLabel->setScale(0.4);
        pLvUpLabel->setPosition(tLvUp.width*0.5,tLvUp.height*0.5);
        pItemSprite->addChild(pLvUpLabel,0,100);
        
        Sprite *pHP = Sprite::createWithSpriteFrameName("hppic.png");
        pHP->setAnchorPoint(Vec2(0,0.5));
        pHP->setScale(0.25);
        pHP->setPosition(Vec2(tBg.width*0.225,tBg.height*0.3));
        pBg->addChild(pHP,0,200);
        
        char strHp[50] = {0};
        sprintf(strHp,"%d", fighterInfo->maxHP);
        
        Label *pHpNum = Label::createWithSystemFont(strHp,"Aril",30);
        pHpNum->setScale(0.5);
        pHpNum->setAnchorPoint(Vec2(0,0.5));
        pHpNum->setPosition(Vec2(pHP->getPositionX()+pHP->getBoundingBox().size.width,tBg.height*0.3));
        pHpNum->setColor(Color3B(234,56,189));
        pBg->addChild(pHpNum,0,300);
        
        Sprite *pAtk = Sprite::createWithSpriteFrameName("attk.png");
        pAtk->setAnchorPoint(Vec2(0,0.5));
        pAtk->Node::setScale(0.25);
        pAtk->setPosition(Vec2(pHpNum->getPositionX()+pHpNum->getBoundingBox().size.width*1.5,tBg.height*0.3));
        pBg->addChild(pAtk,0,201);
        
        int iAtk = fighterInfo->attk;
        char strAtk[50] = {0};
        sprintf(strAtk,"%d",iAtk);
        
        Label *pAtkNum = Label::createWithSystemFont(strAtk,"Aril",30);
        pAtkNum->setScale(0.5);
        pAtkNum->setAnchorPoint(Vec2(0,0.5));
        pAtkNum->setPosition(Vec2(pAtk->getPositionX()+pAtk->getBoundingBox().size.width,tBg.height*0.3));
        pAtkNum->setColor(Color3B(200,97,189));
        pBg->addChild(pAtkNum,0,301);
        
        Sprite *pDef = Sprite::createWithSpriteFrameName("defpic.png");
        pDef->setAnchorPoint(Vec2(0,0.5));
        pDef->setScale(0.25);
        pDef->setPosition(Vec2(pAtkNum->getPositionX()+pAtkNum->getBoundingBox().size.width*1.5,tBg.height*0.3));
        pBg->addChild(pDef,0,202);
        
        Label *pDefNum = Label::createWithSystemFont("125","Aril",30);
        pDefNum->setScale(0.5);
        pDefNum->setAnchorPoint(Vec2(0,0.5));
        pDefNum->setColor(Color3B(12,166,190));
        pDefNum->setPosition(Vec2(pDef->getPositionX()+pDef->getBoundingBox().size.width,tBg.height*0.3));
        pBg->addChild(pDefNum,0,302);
    }
    return cell;
}


//cell的数量
ssize_t CSoldierLvUpLayer::numberOfCellsInTableView(TableView *table)
{
    return m_pSoldierVec.size();
}


NS_CC_EXT_END