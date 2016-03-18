//
//  HeroLvUpLayer.cpp
//  WarOfGod
//
//  Created by prefer_shan on 15/3/18.
//
//

#include "HeroLvUpLayer.h"
#include "LYPersonalData.h"
#include "DBMgr.h"
#include "DataManager.h"
#include "ArmLvUpLayer.h"
#include "Common.h"
#include "LYFightManager.h"
#include "LvIntroduce.h"
USING_NS_CC;
NS_CC_EXT_BEGIN

CHeroLvUpLayer::CHeroLvUpLayer()
{
    m_iCurIdx = 0;
    m_tableView = NULL;
}
CHeroLvUpLayer::~CHeroLvUpLayer()
{

}

void CHeroLvUpLayer::tableCellTouched(TableView* table,TableViewCell* cell)
{
    
}

void CHeroLvUpLayer::tableCellWillRecycle(TableView* table, TableViewCell* cell)
{

}

void CHeroLvUpLayer::tableCellHighlight(TableView* table,TableViewCell* cell)
{
    
}

void CHeroLvUpLayer::tableCellUnhighlight(TableView* table,TableViewCell* cell)
{


}

cocos2d::Size CHeroLvUpLayer::cellSizeForTable(TableView* table)
{
    Size visibSize = Director::getInstance()->getVisibleSize();
    return Size(visibSize.width, visibSize.height/4.327);
}


bool CHeroLvUpLayer::init()
{
    CBaseLayer::init();

    
    std::vector<FighterInfo*>  pAll;
    
    pAll = CPersonalData::getInstance()->getAllSoldierInfo();
    
    bool bGril =   CommonData::GetSingletonPtr()->IsGirlHeroVisable();
    
    
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
            
            if(pAll[i]->monsterId == 1004)
            {
                continue;
            }

            m_pSoldierVec.push_back(pAll[i]);
        }
    }
    
    addTableView();
    return true;
}


void CHeroLvUpLayer::OnRemoveNotice()
{
    Label *  p =  (Label *)getChildByTag(1000);
    
    if(p)
    {
        p->removeFromParent();
    }
}

void CHeroLvUpLayer::OnMenuOpen(Ref *pSender)
{
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
    if((CPersonalData::getInstance()->getUserInfo()->m_iSmallLevel >= 23) && (!CDataManager::getEndedBy(GUIDE_GIRL_HERO)))
    {
        CDataManager::saveGuideInfoToDB(GUIDE_GIRL_HERO);
    }
    CArmLvUpLayer *pLayer = CArmLvUpLayer::create(m_pSoldierVec[idx],(int)idx);
    getParent()->addChild(pLayer,2);
    
}


//生成cell
TableViewCell* CHeroLvUpLayer::tableCellAtIndex(TableView *table,ssize_t idx)
{

    TableViewCell *cell = table->dequeueCell();
    
    FighterInfo *fighterInfo =   m_pSoldierVec[idx];
    monsterInfo* pMonsterInfo = CDataManager::getMonsterInfoByMonsterId(fighterInfo->monsterId);

    vector<heroSkill*> pVec = CDBMgr::getInstance()->getHeroSkillByMonsterId(fighterInfo->monsterId);
    
    
    
    
    
    if (!cell)
    {
        cell = new TableViewCell();
        cell->autorelease();
        
    }
    {
        cell->removeAllChildren();
        int iHeight = 85;
    
        Sprite *pBg =  Sprite::createWithSpriteFrameName("choosebg.png");

        pBg->setScale(1.86);
        
        pBg->setPosition(m_winSize.width*0.5,iHeight);
        
        cell->addChild(pBg,0,100);
        
        Size tBg = pBg->getContentSize();
        
        log("%d\n",pMonsterInfo->monsterId);
        
        Sprite *pNorSoldier = CDataManager::getMonsterIconAndBg(pMonsterInfo->monsterId,true);
        pNorSoldier->Node::setScale(2);
        pNorSoldier->setPosition(tBg.width*0.12,tBg.height*0.5);
        pBg->addChild(pNorSoldier,0,100);
        
        //姓名
        Label *pLvName = Label::createWithSystemFont(pMonsterInfo->monsterName,"Aril",30);
        pLvName->setScale(0.6);
        pLvName->setAnchorPoint(Vec2(0,0.5));
        pLvName->setPosition(tBg.width*0.2,tBg.height*0.745);
        pLvName->setColor(Color3B(209,108,36));
        pBg->addChild(pLvName,0,102);
        
        char strLv[100] = {0};
        sprintf(strLv,"LV%d",fighterInfo->level);
        
        //等级
        Label *pLv = Label::createWithSystemFont(strLv,"Aril",30);
        pLv->setScale(0.6);
        pLv->setColor(Color3B(245,154,81));
        pLv->setAnchorPoint(Vec2(0,0.5));
        pLv->setPosition(pLvName->getPositionX()+pLvName->getContentSize().width*1.35,tBg.height*0.745);
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
        
        LYMenuItemSprite *pItemSprite = LYMenuItemSprite::create(pLvUp,pLvUpSelect,pLvUpDisabel,CC_CALLBACK_1(CHeroLvUpLayer::OnMenuOpen,this));
        
        pItemSprite->setTag(110);
        pItemSprite->setScale(2);
        pItemSprite->setUserData((void*)idx);
        
        Menu *pMenuSoldier = Menu::create(pItemSprite,NULL);
        pMenuSoldier->setPosition(tBg.width*0.8,tBg.height*0.5);
        pBg->addChild(pMenuSoldier,0,106);
        
        if((CPersonalData::getInstance()->getUserInfo()->m_iSmallLevel >= 23) && (!CDataManager::getEndedBy(GUIDE_GIRL_HERO)))
        {
            if(pMonsterInfo->monsterId == 1005)
            {
                Sprite *pRedPoint = Sprite::createWithSpriteFrameName("newTip.png");
                pItemSprite->addChild(pRedPoint, 1, 50);
                pRedPoint->setScale(0.45f);
                Vec2 pos = Vec2(pItemSprite->getNormalImage()->getContentSize().width * 0.785f,
                                pItemSprite->getNormalImage()->getContentSize().height * 0.88f);
                pRedPoint->setPosition(pos);
            }
        }
        
        Size tLvUp = pItemSprite->getContentSize();
        
        Label *pLvUpLabel = Label::createWithTTF(strLableJi,"fonts/cuti.ttf",30);
        pLvUpLabel->enableOutline(Color4B(0,0,0,255),3);

        pLvUpLabel->setPosition(tLvUp.width*0.5,tLvUp.height*0.5);
        pLvUpLabel->setScale(0.4);
        pItemSprite->addChild(pLvUpLabel,0,100);
        
        float fHpScale = 0.4;
        
        Sprite *pHP = Sprite::createWithSpriteFrameName("hppic.png");
        pHP->setAnchorPoint(Vec2(0,0.5));
        pHP->setScale(fHpScale);
        pHP->setPosition(Vec2(tBg.width*0.225,tBg.height*0.3));
        pBg->addChild(pHP,0,200);
        
        char strHp[50] = {0};
        sprintf(strHp,"%d", fighterInfo->maxHP);
        
        Label *pHpNum = Label::createWithSystemFont(strHp,"Aril",30);
        pHpNum->setAnchorPoint(Vec2(0,0.5));
        pHpNum->setScale(fHpScale);
        pHpNum->setPosition(Vec2(pHP->getPositionX()+pHP->getBoundingBox().size.width,tBg.height*0.3));
        pHpNum->setColor(Color3B(234,56,189));
        pBg->addChild(pHpNum,0,300);
        
        Sprite *pAtk = Sprite::createWithSpriteFrameName("attk.png");
        pAtk->setAnchorPoint(Vec2(0,0.5));
        pAtk->setScale(fHpScale);
        pAtk->setPosition(Vec2(pHpNum->getPositionX()+pHpNum->getBoundingBox().size.width*1.5,tBg.height*0.3));
        pBg->addChild(pAtk,0,201);
        
        int iAtk = fighterInfo->attk;
        char strAtk[50] = {0};
        sprintf(strAtk,"%d",iAtk);
        
        Label *pAtkNum = Label::createWithSystemFont(strAtk,"Aril",30);
        pAtkNum->setScale(fHpScale);
        pAtkNum->setAnchorPoint(Vec2(0,0.5));
        pAtkNum->setPosition(Vec2(pAtk->getPositionX()+pAtk->getBoundingBox().size.width,tBg.height*0.3));
        pAtkNum->setColor(Color3B(200,97,189));
        pBg->addChild(pAtkNum,0,301);
        
        Sprite *pDef = Sprite::createWithSpriteFrameName("defpic.png");
        pDef->setAnchorPoint(Vec2(0,0.5));
        pDef->setScale(fHpScale);
        pDef->setPosition(Vec2(pAtkNum->getPositionX()+pAtkNum->getBoundingBox().size.width*1.5,tBg.height*0.3));
        pBg->addChild(pDef,0,202);
        
        Label *pDefNum = Label::createWithSystemFont("125","Aril",30);
        pDefNum->setScale(fHpScale);
        pDefNum->setAnchorPoint(Vec2(0,0.5));
        pDefNum->setColor(Color3B(12,166,190));
        pDefNum->setPosition(Vec2(pDef->getPositionX()+pDef->getBoundingBox().size.width,tBg.height*0.3));
        pBg->addChild(pDefNum,0,302);
    }
    return cell;

}

//cell的数量
ssize_t CHeroLvUpLayer::numberOfCellsInTableView(TableView *table)
{

    return m_pSoldierVec.size();
}


void CHeroLvUpLayer::addTableView()
{
    
    m_tableView = TableView::create(this,Size(m_winSize.width,m_winSize.height*0.7));
    
    Size winsize = Director::getInstance()->getWinSize();
    
    m_tableView->setDirection(ScrollView::Direction::VERTICAL);
    
    m_tableView->setPosition(Point(0,100));
    
    m_tableView->setDelegate(this);
    
    m_tableView->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    
    this->addChild(m_tableView,1);
}

NS_CC_EXT_END
