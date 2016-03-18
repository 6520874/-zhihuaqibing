//
//  EnergyLvUpLayer.cpp
//  TheGodWill
//
//  Created by prefer_shan on 14/11/25.
//
//

#include "EnergyLvUpLayer.h"
#include "DBMgr.h"
#include "guiderManagerLayer.h"
#include "LYFightManager.h"
#include "NetWorkDataSingle.h"

NS_CC_EXT_BEGIN

CEnergyLvUpLayer:: CEnergyLvUpLayer()
{
    m_VisibleSize = Director::getInstance()->getVisibleSize();
    m_tableView = NULL;
    m_iIndex = 0;
    m_pUpEffect = NULL;
    m_pGuideMenu = NULL;
    m_iIdx = 0;
    

}

CEnergyLvUpLayer::~ CEnergyLvUpLayer()
{
    m_pEnergyUpgradeInfo.clear();
}

void CEnergyLvUpLayer::onExit()
{
    CBaseLayer::onExit();
}


bool CEnergyLvUpLayer::init()
{
    CBaseLayer::init();
    
    m_pEnergyUpgradeInfo = CDBMgr::getInstance()->GetEnergyUpGradeInfo();
    
    
    map<int,int> pEnergyIdVec;
    
    
    
    for(int i=0;i< m_pEnergyUpgradeInfo.size();i++)
    {
        pEnergyIdVec[m_pEnergyUpgradeInfo[i]->m_iEnergyUpgradeId] =1;
    }
    
 
    map<int,int>::iterator it = pEnergyIdVec.begin();
    

    for(;it!=pEnergyIdVec.end();it++)
    {
        
         int iLv =  CDBMgr::getInstance()->GetEnergyLvById(it->first);
           EnergyUpgradeInfo* pInfo1 = CDBMgr::getInstance()->getEnergyUpgradeInfo(it->first,iLv+1);
        
        if(pInfo1)
        {

            m_pShowEnergyUpgradeInfo.push_back(pInfo1);
        }
        else
        {
            pInfo1 = CDBMgr::getInstance()->getEnergyUpgradeInfo(it->first,iLv);
            m_pShowEnergyUpgradeInfo.push_back(pInfo1);
        }

        
    }
    
    
    log("%zd",m_pEnergyUpgradeInfo.size());
    
    addTableView();
    return true;
}


void  CEnergyLvUpLayer::addTableView()
{
    m_tableView = TableView::create(this, Size(m_winSize.width,m_winSize.height*0.7));
    
    Size winsize = Director::getInstance()->getWinSize();
    
    m_tableView->setDirection(ScrollView::Direction::VERTICAL);
    
    m_tableView->setPosition(Point(0,100));
    
    m_tableView->setDelegate(this);
    
    m_tableView->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    
    this->addChild(m_tableView,1);
}


void CEnergyLvUpLayer::tableCellTouched(TableView* table,TableViewCell* cell)
{

}

void CEnergyLvUpLayer::tableCellWillRecycle(TableView* table, TableViewCell* cell)
{

}

void CEnergyLvUpLayer::tableCellHighlight(TableView* table,TableViewCell* cell)
{

}

void CEnergyLvUpLayer::tableCellUnhighlight(TableView* table,TableViewCell* cell)
{

}

Size CEnergyLvUpLayer::cellSizeForTable(TableView* table)
{
    Size visibSize = Director::getInstance()->getVisibleSize();
    return Size(visibSize.width, visibSize.height/4.327);
}


void CEnergyLvUpLayer::OnRemoveNotice()
{
    Label *pBuildingUp =  (Label*)getChildByTag(1000);
    pBuildingUp->removeFromParent();
}


void CEnergyLvUpLayer::ReoloadData()
{
    for(int i=0;i<m_pShowEnergyUpgradeInfo.size();i++)
    {
        //log(m_pShowEnergyUpgradeInfo[i]->m_id;
    }
}

bool SortByM1( const  energyUpgradeInfo*v1, const  energyUpgradeInfo*v2)//注意：本函数的参数的类型一定要与vector中元素的类型一致
{
    return v1->m_id < v2->m_id;//升序排列
}


void CEnergyLvUpLayer::OnRemoveEffect()
{
    
    Sprite *pbg = (Sprite*)m_ppPic->getChildByTag(101);
    if(pbg)
    {
        pbg->removeFromParent();
    }
    
    
   }

void CEnergyLvUpLayer::OnUpdateCoin(Ref *pSender)
{
    log("on uPdateCOin");
    log("pipi =%zi\n",m_iIdx);
    
    long idx = m_iIdx;
     SimpleAudioEngine::getInstance()->playEffect("music/lvlSucess.mp3");
    
    energyUpgradeInfo* p = NULL;
    
    p = GetEnergyLvUplayer(idx+1,m_pShowEnergyUpgradeInfo[idx]->m_iUpgradeLevel+1);
    
    if(p == NULL)
    {
        CDBMgr::getInstance()->SaveEnergyLvTostr(m_pShowEnergyUpgradeInfo[idx]->m_iEnergyUpgradeId,m_pShowEnergyUpgradeInfo[idx]->m_iUpgradeLevel);
        m_tableView->updateCellAtIndex(idx);
     
        
        return;
    }
    std::vector<energyUpgradeInfo*>::iterator it = m_pShowEnergyUpgradeInfo.begin();
    for(;it!=m_pShowEnergyUpgradeInfo.end();)
    {
        if((*it)->m_iEnergyUpgradeId==(idx+1))
        {
            m_pShowEnergyUpgradeInfo.erase(it);
            break;
        }
        else
        {
            it++;
        }
    }
    
    
    
    m_pShowEnergyUpgradeInfo.push_back(p);
    
    
    std::sort(m_pShowEnergyUpgradeInfo.begin(),m_pShowEnergyUpgradeInfo.end(),SortByM1);

    
    //升级成功
    CDBMgr::getInstance()->SaveEnergyLvTostr(m_pShowEnergyUpgradeInfo[idx]->m_iEnergyUpgradeId,m_pShowEnergyUpgradeInfo[idx]->m_iUpgradeLevel-1);
    
    m_tableView->updateCellAtIndex(idx);
    
    //OnRemoveEffect();
    
    
    Sprite *pbg = (Sprite*)m_ppPic->getChildByTag(101);
    if(pbg)
    {
    pbg->removeFromParent();
    }
    
    
    m_pUpEffect = Sprite::create();
    Animate* theAnimate = CFightManager::getAnimateWithPath("lvupeff",1,14);
    
    m_pUpEffect->setScale(2.8);
    m_pUpEffect->setPosition(m_tPic.width*0.48,m_tPic.height*0.48);
    
    theAnimate->setTag(100);
    
    m_ppPic->addChild(m_pUpEffect,111,101);
    
    //CallFunc* getOff = CallFunc::create(CC_CALLBACK_0(CEnergyLvUpLayer::OnRemoveEffect, this));
    
    m_pUpEffect->runAction(Sequence::create(theAnimate,DelayTime::create(0.1f), RemoveSelf::create(),NULL));
    
    
    if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SOLIDER_NUM) && (guiderManagerLayer::getInstance()->getStepNum() == 4))
    {
        guiderManagerLayer::getInstance()->removeGuiderLayer();
        CDataManager::saveGuideInfoToDB(guiderManagerLayer::getInstance()->getGuideType());
        guiderManagerLayer::getInstance()->setStepNum(guiderManagerLayer::getInstance()->getStepNum() + 1);
        NotificationCenter::getInstance()->postNotification("SoliderCloseGuide");
    }
    

}


void CEnergyLvUpLayer::OnMenuUpgrade(Ref *psender)
{
    LYMenuItemSprite *pSprite = (LYMenuItemSprite*)psender;
    long idx =  (long)pSprite->getUserData();

    int iCoin = CPersonalData::getInstance()->getUserCoin();
    int iCostGold = m_pShowEnergyUpgradeInfo[idx]->m_iCostGold;
    if(!((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SOLIDER_NUM) && (guiderManagerLayer::getInstance()->getStepNum() == 4)))
    {
        if(iCoin<iCostGold)
        {
            SimpleAudioEngine::getInstance()->playEffect("music/noBtn.mp3");
            
            Sprite * thetarget = CFightManager::getTheFlyWord("金币不够耶!","fonts/cuti.ttf");
            thetarget->setPosition(Vec2(m_winSize.width / 2, m_winSize.height / 2));
            addChild(thetarget, 100);
            return ;
        }
        else
        {
            CNetWorkDataSingle::GetSingletonPtr()->SaveUserMCF(this, menu_selector(CEnergyLvUpLayer::OnUpdateCoin),-iCostGold,0,0);

        }
    }
    else{
         CNetWorkDataSingle::GetSingletonPtr()->SaveUserMCF(this, menu_selector(CEnergyLvUpLayer::OnUpdateCoin),0,0,0);
         //OnUpdateCoin(this);
        
    }
    
  
    {
        Size tHead;
        Size tPic;
        Sprite *pp = NULL;
        
        //Sprite *ppPic = NULL;
        
        
        TableViewCell* cell = m_tableView->cellAtIndex(idx);
        Sprite *p = (Sprite*)cell->getChildByTag(100);

        if(p)
        {
            pp = (Sprite *)p->getChildByTag(100);
            if(pp)
            {
                tHead = pp->getContentSize();
                m_ppPic = (Sprite*)pp->getChildByTag(101);
                tPic = m_ppPic->getContentSize();
            }
        }
        
        Size tp  = p->getContentSize();
        
                //CDataManager::saveUserStoneAndCoinToDB(iCostGold,0,0);
        
        //spf
        m_iIdx = idx;
    
        m_tPic = tPic;
    }
    
}

//生成cell

energyUpgradeInfo* CEnergyLvUpLayer::GetEnergyLvUplayer(int iEnergyUpid,int iUpgradeLevel)
{
    
    for(int i=0;i<m_pEnergyUpgradeInfo.size();i++)
    {
        if(m_pEnergyUpgradeInfo[i]->m_iEnergyUpgradeId == iEnergyUpid && m_pEnergyUpgradeInfo[i]->m_iUpgradeLevel ==iUpgradeLevel)
        {
            return m_pEnergyUpgradeInfo[i];
        }
    }
    return  NULL;
}

extension::TableViewCell* extension::CEnergyLvUpLayer::tableCellAtIndex(TableView *table,ssize_t idx)
{
    TableViewCell *cell = table->dequeueCell();
    
    if (!cell)
    {
        cell = new TableViewCell();
        cell->autorelease();
        
        {
            int iHeight = 85;
            Sprite *pBg =  Sprite::createWithSpriteFrameName("choosebg.png");
            pBg->setPosition(m_winSize.width*0.5,iHeight);
            cell->addChild(pBg,0,100);
            
            Size tBg = pBg->getContentSize();

            Sprite *pNorSoldier = Sprite::createWithSpriteFrameName("headbg.png");
            pNorSoldier->setPosition(tBg.width*0.12,tBg.height*0.5);
            pBg->addChild(pNorSoldier,0,100);
            
            char strHeadPic[100] = {0};
            sprintf(strHeadPic,"%s",m_pShowEnergyUpgradeInfo[idx]->m_sIco.c_str());

            Sprite *pHeadPic = Sprite::createWithSpriteFrameName(strHeadPic);
            pHeadPic->setScale(1);
            pHeadPic->setPosition(pNorSoldier->getContentSize().width*0.5,pNorSoldier->getContentSize().height*0.5);
            pNorSoldier->addChild(pHeadPic,0,101);
            
            //姓名
            Label *pLvName = Label::createWithTTF(m_pShowEnergyUpgradeInfo[idx]->m_sName,"fonts/cuti.ttf",30);
            pLvName->setColor(Color3B(209,108,36));
            pLvName->setAnchorPoint(Vec2(0,0.5));
            pLvName->setPosition(tBg.width*0.2,tBg.height*0.8);
            pBg->addChild(pLvName,0,102);
            
            char str[100] = {0};
            sprintf(str,"LV%d",m_pShowEnergyUpgradeInfo[idx]->m_iUpgradeLevel);
                
            //等级
            Label *pLv = Label::createWithTTF(str,"fonts/cuti.ttf",30);
            pLv->setAnchorPoint(Vec2(0,0.5));
            pLv->setColor(Color3B(245,154,81));
            pLv->setPosition(tBg.width*0.6,tBg.height*0.8);
            pBg->addChild(pLv,0,103);
            
            //描述
            sprintf(str,"%s",m_pShowEnergyUpgradeInfo[idx]->m_sDesc.c_str());
            
            
            Label *pDesc = Label::createWithTTF(str,"fonts/cuti.ttf",20);
            pDesc->setColor(Color3B(245,154,81));
            pDesc->setAnchorPoint(Vec2(0,0.5));
            pDesc->setPosition(tBg.width*0.2,tBg.height*0.5);
            pBg->addChild(pDesc,0,104);
            
            char strPreAffectNum[50] = {0};
            int iLv =  CDBMgr::getInstance()->GetEnergyLvById((int)idx+1);
            EnergyUpgradeInfo* pInfo = CDBMgr::getInstance()->getEnergyUpgradeInfo((int)idx+1,iLv);
            if(pInfo)
            {
                sprintf(strPreAffectNum,"%d",pInfo->m_iAffectNum);
            }
            Label *pPreAffectNum = Label::createWithSystemFont(strPreAffectNum,"Aril",30);
            pPreAffectNum->setPosition(Vec2(tBg.width*0.2,tBg.height*0.2));
            pPreAffectNum->setColor(Color3B::BLACK);
            pPreAffectNum->setAnchorPoint(Vec2(0,0.5));
            pBg->addChild(pPreAffectNum,0,201);
                
                
            Sprite *pLvArrow = Sprite::createWithSpriteFrameName("upArrow.png");
            pLvArrow->setRotation(90);
            pLvArrow->setAnchorPoint(Vec2(0,0.5));
            pLvArrow->setPosition(pPreAffectNum->getPositionX()+pPreAffectNum->getContentSize().width*1.5,tBg.height*0.3);
            pBg->addChild(pLvArrow,0,200);

            //提示多少
            char strAffectNum[50] = {0};
            sprintf(strAffectNum,"%d",m_pShowEnergyUpgradeInfo[idx]->m_iAffectNum);
                
            Label *pLvUpMuch = Label::createWithSystemFont(strAffectNum,"Aril",30);
            pLvUpMuch->setAnchorPoint(Vec2(0,0.5));
            pLvUpMuch->setColor(Color3B::BLACK);
            pLvUpMuch->setPosition(pLvArrow->getPositionX()+pLvArrow->getContentSize().width,tBg.height*0.2);
            pBg->addChild(pLvUpMuch,0,105);
                
            Sprite *pLvUp = Sprite::createWithSpriteFrameName("greenbtn.png");
            Sprite *pLvUpSelect = Sprite::createWithSpriteFrameName("greenbtn.png");
            pLvUpSelect->setColor(Color3B::GRAY);
            
            LYMenuItemSprite *pItemSprite = LYMenuItemSprite::create(pLvUp,pLvUpSelect,CC_CALLBACK_1(CEnergyLvUpLayer::OnMenuUpgrade,this));
            pItemSprite->setTag(100);
            pItemSprite->setScale(3);
            
            pItemSprite->setUserData((void*)idx);
            
            Menu *pMenuSoldier = Menu::create(pItemSprite,NULL);
            pMenuSoldier->setPosition(tBg.width*0.8,tBg.height*0.5);
            pBg->addChild(pMenuSoldier,0,106);
            
            if(m_pShowEnergyUpgradeInfo[idx]->m_iEnergyUpgradeId  == 1)//记录士兵上限的升级按钮位置
            {
                m_pGuideMenu = pMenuSoldier;
            }
            
            Size tLvUp = pItemSprite->getContentSize();
            
            
            Label *pLvUpLabel = Label::createWithTTF("升级","fonts/cuti.ttf",30);
            pLvUpLabel->enableOutline(Color4B(0,0,0,255),3);
            pLvUpLabel->setPosition(tLvUp.width*0.5,tLvUp.height*0.7);
            pItemSprite->addChild(pLvUpLabel,0,100);
                
            Sprite *pCostCoin = Sprite::createWithSpriteFrameName("coin.png");
            pCostCoin->setScale(0.25);
            pCostCoin->setPosition(tLvUp.width*0.45,tLvUp.height*0.3);
            pItemSprite->addChild(pCostCoin,0,107);
            sprintf(str,"%d",m_pShowEnergyUpgradeInfo[idx]->m_iCostGold);
                
            Label *pCostLable = Label::createWithSystemFont(str,"Aril",30);
            pCostLable->setColor(Color3B::YELLOW);
            pCostLable->setAnchorPoint(Vec2(0,0.5));
            pCostLable->setPosition(pCostCoin->getBoundingBox().size.width*0.5+pCostCoin->getPositionX(),tLvUp.height*0.3);
            pItemSprite->addChild(pCostLable,0,108);
        }
    }
    else
    {
         //log("idx = %zd\n",idx);
         {
         
         Sprite *pBg =  (Sprite *)cell->getChildByTag(100);
         pBg->setScale(1.86);
             
         pBg->setSpriteFrame("choosebg.png");
         
         Size tBg = pBg->getContentSize();
             
         //headbg.png
         Sprite *pHeadBg  = (Sprite*)pBg->getChildByTag(100);
         pHeadBg->setSpriteFrame("headbg.png");
    
         //图标
         char strHeadPic[100] = {0};
         sprintf(strHeadPic,"%s",m_pShowEnergyUpgradeInfo[idx]->m_sIco.c_str());
        
         Sprite *pSkillPic = (Sprite*)pHeadBg->getChildByTag(101);
         pSkillPic->setScale(0.5);
         pSkillPic->setSpriteFrame(strHeadPic);
          
             
         //姓名
         std::string strName = m_pShowEnergyUpgradeInfo[idx]->m_sName;
         Label *pName = (Label*)pBg->getChildByTag(102);
         pName->setString(strName.c_str());
         pName->setScale(0.5);
         
    
         //描述
         Label *pDesc = (Label*)pBg->getChildByTag(104);
         pDesc->setScale(0.75);
                char str[100] = {0};
         sprintf(str,"%s",m_pShowEnergyUpgradeInfo[idx]->m_sDesc.c_str());
         pDesc->setString(str);
         
         //先前影响值
         char strPreAffectNum[50] = {0};
         int iLv =  CDBMgr::getInstance()->GetEnergyLvById((int)idx+1);
             
         EnergyUpgradeInfo* pInfo = CDBMgr::getInstance()->getEnergyUpgradeInfo((int)idx+1,iLv);

         /*if(pInfo== NULL)
         {
             pInfo = CDBMgr::getInstance()->getEnergyUpgradeInfo((int)idx+1,iLv-1);
         }*/
         
        //等级
        sprintf(str,"LV%d", pInfo->m_iUpgradeLevel);
        Label *pLvLable =  (Label*)pBg->getChildByTag(103);
        pLvLable->setScale(0.5);
        pLvLable->setString(str);
             
         //先前影响的值
         Label *pPreAffectNum = (Label*)pBg->getChildByTag(201);
         pPreAffectNum->setScale(0.5);
         sprintf(strPreAffectNum,"%d",pInfo->m_iAffectNum);
         pPreAffectNum->setString(strPreAffectNum);
         
             
         Sprite *pLvArrow = (Sprite*)pBg->getChildByTag(200);
         pLvArrow->setScale(0.5);
         pLvArrow->setSpriteFrame("upArrow.png");
         pLvArrow->setRotation(90);
         pLvArrow->setAnchorPoint(Vec2(0,0.5));
         pLvArrow->setPosition(pPreAffectNum->getPositionX()+pPreAffectNum->getBoundingBox().size.width+pLvArrow->getBoundingBox().size.width*0.5,tBg.height*0.3);
             
         pInfo = CDBMgr::getInstance()->getEnergyUpgradeInfo((int)idx+1,iLv+1);
         
        if(pInfo == NULL)
        {
            log("112121212121");
            
            
        }
         Menu *pMenu = (Menu*)pBg->getChildByTag(106);
         LYMenuItemSprite * pMenuItem = (LYMenuItemSprite *)pMenu->getChildByTag(100);
         pMenuItem->setUserData((void*)idx);
         pMenuItem->setScale(2);
            
         sprintf(str,"%d",m_pShowEnergyUpgradeInfo[idx]->m_iCostGold);
         
             
         Label *pCost = (Label*)pMenuItem->getChildByTag(108);
         if(pCost)
         {
             pCost->setScale(0.2);
             pCost->setString(str);
         }
         Label* pLabel = (Label*)pMenuItem->getChildByTag(100);
         pLabel->setScale(0.25);
         
        Sprite* pCoin = (Sprite*)pMenuItem->getChildByTag(107);
        Label* pLabel2  =(Label*)pMenuItem->getChildByTag(108);
           
        Size tLvUp = pMenuItem->getContentSize();
             
             
         if(pInfo == NULL)
         {
             pInfo = CDBMgr::getInstance()->getEnergyUpgradeInfo((int)idx+1,iLv);
           
            
             pCoin->setVisible(false);


             pLabel2->setVisible(false);
             
             pMenuItem->setEnabled(false);
             pLabel->setPosition(tLvUp.width*0.5,tLvUp.height*0.5);
             
             pLabel->setString("满级");

             Sprite *pbg = (Sprite*)pSkillPic->getChildByTag(101);
             if(pbg)
             {
                 pbg->removeFromParent();
             }
            
         }
         else
         {
              pLabel->setString("升级");
               pLabel->setPosition(tLvUp.width*0.5,tLvUp.height*0.7);

              pMenuItem->setEnabled(true);
              pCoin->setSpriteFrame("coin.png");
              pCoin->setVisible(true);
             
             sprintf(str,"%d",m_pShowEnergyUpgradeInfo[idx]->m_iCostGold);
             pLabel2->setString(str);
              pLabel2->setVisible(true);
         }
             
         Label *pAffectNum = (Label*)pBg->getChildByTag(105);
         pAffectNum->setScale(0.5);
             
         pAffectNum->setPositionX(pLvArrow->getPositionX()+pLvArrow->getBoundingBox().size.width);
             
         char strAffectNum[50] = {0};
         sprintf(strAffectNum,"%d",pInfo->m_iAffectNum);
         pAffectNum->setString(strAffectNum);
         
         
         }
    }
    return cell;
   
}

//cell的数量
ssize_t extension::CEnergyLvUpLayer::numberOfCellsInTableView(TableView *table)
{

    return m_pShowEnergyUpgradeInfo.size();
}

NS_CC_EXT_END