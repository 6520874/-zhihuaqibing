//
//  TaskLayer.cpp
//  TheGodWill
//
//  Created by prefer_shan on 14/12/15.
//
//

#include "TaskLayer.h"
#include "TaskMgr.h"
#include "AllDefine.h"
#include "DataManager.h"
#include "LYFightData.h"
#include "guiderManagerLayer.h"
#include "CSharedPlist.h"
#include "NetWorkDataSingle.h"
#include "DBMgr.h"
#include "Common.h"
#include "TDCCVirtualCurrency.h"
#include "zzbNetControlModule.h"

USING_NS_CC;
NS_CC_EXT_BEGIN

#define TABLEVIEW_WIDTH 957

CTaskLayer::CTaskLayer(bool bMainLine)
{
    m_winSize = Director::getInstance()->getWinSize();
    m_pTaskGetLayer = NULL;
    m_bMainLayer = bMainLine;
    listen = NULL;
    CSharedPlist::getInstance()->pushPlist("plist/task.plist");
    CSharedPlist::getInstance()->pushPlist("plist/propIcon_1.plist");
    m_pMainTaks = NULL;
    m_pMenuCose = NULL;
    m_GuidepMenu = NULL;
    m_pTableViewVec.clear();
    m_bValid = true;
}


CTaskLayer::~CTaskLayer()
{
    CSharedPlist::getInstance()->erasePlist("plist/task.plist");
    CSharedPlist::getInstance()->erasePlist("plist/propIcon_1.plist");
}

void CTaskLayer::tableCellTouched(TableView* table,TableViewCell* cell)
{
    CCLOG("点击到了！%d", int(cell->getIdx()));
}

void CTaskLayer::tableCellWillRecycle(TableView* table, TableViewCell* cell)
{
    
}

void CTaskLayer::tableCellHighlight(TableView* table,TableViewCell* cell)
{
    
}

void CTaskLayer::tableCellUnhighlight(TableView* table,TableViewCell* cell)
{
    
}



cocos2d::Size CTaskLayer::cellSizeForTable(TableView* table)
{
    Size visibSize = Director::getInstance()->getVisibleSize();
    return Size(visibSize.width, visibSize.height/4.327);
}


void CTaskLayer::addTableView()
{
    if(!m_bMainLayer)
    {
        m_tableView = TableView::create(this, Size(TABLEVIEW_WIDTH,m_pMainTaks->getBoundingBox().size.height*0.72));
        m_tableView->setPosition(Point(m_pMainTaks->getBoundingBox().size.width * 0.05f,m_pMainTaks->getBoundingBox().size.height * 0.115));
    }
    else
    {   m_tableView = TableView::create(this, Size(TABLEVIEW_WIDTH,m_pMainTaks->getBoundingBox().size.height*0.73));
        m_tableView->setPosition(Point(m_pMainTaks->getBoundingBox().size.width * 0.03f,m_pMainTaks->getBoundingBox().size.height * 0.135));
    }

    m_tableView->setDirection(ScrollView::Direction::VERTICAL);
    m_tableView->setDelegate(this);
    m_tableView->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    m_pMainTaks->addChild(m_tableView,1);
    m_tableView->reloadData();
}


//生成cell
TableViewCell* CTaskLayer::tableCellAtIndex(TableView *table,ssize_t idx)
{
    TableViewCell *cell = table->dequeueCell();
    

    
    char buf[256];
    if (!cell)
    {
        cell = new TableViewCell();
        cell->autorelease();
        m_pTableViewVec.clear();
        
       // m_pTableViewVec.push_back(cell);
        
        
    }
  
    
    {
        cell->removeAllChildren();
    
    Sprite *pBg =  Sprite::createWithSpriteFrameName("choosebg.png");
    pBg->setAnchorPoint(Vec2::ZERO);
    pBg->setScaleX(1.75f);
    pBg->setScaleY(1.98f);
    pBg->setPosition(Vec2(10, 0));
    cell->addChild(pBg);
    if(m_bMainLayer)
    {
        pBg->setScaleX(1.8f);
        pBg->setScaleY(1.98f);
        pBg->setPosition(Vec2(15, 0));
    }
    Size tBg = pBg->getBoundingBox().size;
    
    //任务图标
    sprintf(buf, "%s", m_pTaskInfoVec[idx]->m_taskIcon.c_str());
    Sprite *pTaskPic = Sprite::createWithSpriteFrameName(buf);
    pTaskPic->setPosition(tBg.width*0.102f,tBg.height*0.505);
    cell->addChild(pTaskPic, 0, TASK_ICON);
    pTaskPic->setScale(2.05f);
    
    
    //任务名称
    Label*pLabeTaksName = Label::createWithSystemFont(m_pTaskInfoVec[idx]->m_sDesc,"Arial", 28);
    pLabeTaksName->setAnchorPoint(Vec2(0,0.5));
    pLabeTaksName->setColor(Color3B(91,45,2));
    pLabeTaksName->setPosition(pTaskPic->getPositionX()+pTaskPic->getBoundingBox().size.width*0.6,tBg.height*0.76);
    cell->addChild(pLabeTaksName, 0, TASK_NAME);
    
    //任务完成进度
    Sprite* progressBg = Sprite::createWithSpriteFrameName("task_progressBg.png");
    cell->addChild(progressBg);
    progressBg->setScaleX(1.85f);
    progressBg->setScaleY(2.0f);
    progressBg->setAnchorPoint(Vec2(0, 0.5));
    progressBg->setPosition(Vec2(pLabeTaksName->getPositionX() - 10, tBg.height * 0.49f));
    
    Sprite* temp = Sprite::createWithSpriteFrameName("task_progress.png");
    ProgressTimer* progress = ProgressTimer::create(temp);
    progress->setScaleX(1.85f);
    progress->setScaleY(2.0f);
    cell->addChild(progress, 0, TASK_PROGRESS);
    progress->setAnchorPoint(Vec2(0, 0.5));
    progress->setBarChangeRate(Point(1,0));
    progress->setType(ProgressTimer::Type::BAR);
    progress->setMidpoint(Point(0,1));
    progress->setPosition(progressBg->getPosition());
    int num = CDataManager::getTaskProByType(m_pTaskInfoVec[idx]->m_iTaskType);
    float percent = 100*(float)num/(m_pTaskInfoVec[idx]->m_iNum);
    progress->setPercentage(percent);
    
    if(num > m_pTaskInfoVec[idx]->m_iNum)
    {
        num = m_pTaskInfoVec[idx]->m_iNum;
    }
    sprintf(buf,"%d/%d",num, m_pTaskInfoVec[idx]->m_iNum);
    Label *pLabelFinishNum = Label::createWithSystemFont(buf,"Arial",30);
    pLabelFinishNum->setPosition(Vec2(progressBg->getPositionX() + progressBg->getBoundingBox().size.width / 2, progressBg->getPositionY()));
    cell->addChild(pLabelFinishNum, 0, TASK_NUM);
    
    //获得奖励
    Label *pLabelGetAward = Label::createWithSystemFont("获得奖励:","Arial",26);
    pLabelGetAward->setAnchorPoint(Vec2(0,0.5));
    pLabelGetAward->setColor(Color3B(130,77,30));
    pLabelGetAward->setPosition(pLabeTaksName->getPositionX(), tBg.height*0.22);
    cell->addChild(pLabelGetAward);
    
    //金币图标
    Sprite *pSpriteCoin = Sprite::createWithSpriteFrameName("coin.png");
    pSpriteCoin->setPosition(tBg.width*0.34,tBg.height*0.21);
    pSpriteCoin->setScale(0.5);
    cell->addChild(pSpriteCoin);
    pSpriteCoin->setVisible(false);
    
    int showAwardNum = 0;
    //金币数
    char strCoinNum[100] = {0};
    sprintf(strCoinNum,"%d",m_pTaskInfoVec[idx]->m_iMoney);
    Label *pLabelCoin = Label::createWithSystemFont(strCoinNum,"Arial",26);
    pLabelCoin->setColor(Color3B(130,77,30));
    pLabelCoin->setAnchorPoint(Vec2(0, 0.5));
    pLabelCoin->setPosition(pSpriteCoin->getPositionX() + pSpriteCoin->getBoundingBox().size.width / 2 + 5,tBg.height*0.21);
    cell->addChild(pLabelCoin, 0, TASK_COIN);
    pLabelCoin->setVisible(false);
    if(m_pTaskInfoVec[idx]->m_iMoney > 0)
    {
        pLabelCoin->setVisible(true);
        pSpriteCoin->setVisible(true);
        showAwardNum++;
    }
    
    //经验图标
    Sprite *pSpriteExp = Sprite::createWithSpriteFrameName("EXP.png");
    pSpriteExp->setScale(0.5);
    pSpriteExp->setAnchorPoint(Vec2(0, 0.5));
    pSpriteExp->setPosition(pLabelCoin->getPositionX() + pLabelCoin->getContentSize().width + 15, tBg.height*0.215);
    cell->addChild(pSpriteExp);
    pSpriteExp->setVisible(false);
    
    //经验数
    char strExp[100] = {0};
    sprintf(strExp,"%d",m_pTaskInfoVec[idx]->m_iExp);
    Label *pLabelExp = Label::createWithSystemFont(strExp,"Arial",22);
    pLabelExp->setAnchorPoint(Vec2(0, 0.5));
    cell->addChild(pLabelExp, 0, TASK_EXP);
    pLabelExp->setColor(Color3B(130,77,30));
    pLabelExp->setPosition(pSpriteExp->getPositionX() + pSpriteExp->getBoundingBox().size.width + 5,tBg.height*0.21);
    pLabelExp->setVisible(false);
    if(m_pTaskInfoVec[idx]->m_iExp > 0)
    {
        pSpriteExp->setVisible(true);
        pLabelExp->setVisible(true);
        if(showAwardNum == 0)
        {
            pSpriteExp->setPosition(tBg.width*0.325,tBg.height*0.215);
            pLabelExp->setPosition(pSpriteExp->getPositionX() + pSpriteExp->getBoundingBox().size.width + 5,tBg.height*0.21);
        }
        showAwardNum++;
    }
    
    //魔法石
    Sprite* pStone = Sprite::createWithSpriteFrameName("stone.png");
    pStone->setScale(0.5);
    pStone->setAnchorPoint(Vec2(0, 0.5));
    pStone->setPosition(pLabelExp->getPositionX() + pLabelExp->getContentSize().width + 15, tBg.height*0.21);
    cell->addChild(pStone);
    
    sprintf(strExp, "%d", m_pTaskInfoVec[idx]->m_stone);
    Label *pLabelStone = Label::createWithSystemFont(strExp,"Arial",22);
    pLabelStone->setAnchorPoint(Vec2(0, 0.5));
    cell->addChild(pLabelStone, 0, TASK_STONE);
    pLabelStone->setColor(Color3B(130,77,30));
    pLabelStone->setPosition(pStone->getPositionX() + pStone->getBoundingBox().size.width + 5,tBg.height*0.21);
    if(m_pTaskInfoVec[idx]->m_stone > 0)
    {
        if(showAwardNum == 0)
        {
            pStone->setPosition(tBg.width*0.322,tBg.height*0.215);
            pLabelStone->setPosition(pStone->getPositionX() + pStone->getBoundingBox().size.width + 5, pStone->getPositionY());
        }
        else if(showAwardNum == 1)
        {
            if(m_pTaskInfoVec[idx]->m_iExp == 0)
            {
                pStone->setPosition(pSpriteExp->getPositionX(), tBg.height*0.215);
                pLabelStone->setPosition(pStone->getPositionX() + pStone->getBoundingBox().size.width + 5, pStone->getPositionY());
            }
            else{
                pStone->setPosition(pLabelExp->getPositionX() + pLabelExp->getContentSize().width + 15,tBg.height*0.215);
                pLabelStone->setPosition(pStone->getPositionX() + pStone->getBoundingBox().size.width + 8, pStone->getPositionY());
            }
        }
    }
    else{
        pStone->setVisible(false);
        pLabelStone->setVisible(false);
    }
    
    //掉落道具和英雄碎片
    if (m_pTaskInfoVec[idx]->itemVec.size() > 0)
    {
        for(int i = 0; i < m_pTaskInfoVec[idx]->itemVec.size(); i++)
        {
            Sprite* prop = CDataManager::getPropSpriteById(m_pTaskInfoVec[idx]->itemVec.at(i).theId, true);
            prop->setScale(1.2f);
            
            
            if(i <= 1)
            {
                if ((i == 0) || ((i == 1) && (m_pTaskInfoVec[idx]->heroItemVec.size() == 0)))
                {
                    prop->setPosition(tBg.width*0.63 + i*90,tBg.height*0.4);
                    cell->addChild(prop, 0, TASK_PROP + i);
                  
                    
                    Size tProp = prop->getContentSize();
                    Sprite *pPropNum = Sprite::createWithSpriteFrameName("numbg.png");
                    pPropNum->setPosition(tProp.width*0.78,tProp.height*0.2);
                    pPropNum->setScale(0.4);
                    prop->addChild(pPropNum,99);
                    
                    char strNum[50] = {0};
                    sprintf(strNum,"%d",m_pTaskInfoVec[idx]->itemVec.at(0).theNumber);
                    
                    Size tPropNum = pPropNum->getContentSize();
                    
                    Label *pNum = Label::createWithSystemFont(strNum,"Aril",30);
                    pNum->setPosition(tPropNum.width*0.5,tPropNum.height*0.5);
                    pPropNum->addChild(pNum,999);

            
                    
                }
            }
        }
        
    }
    
    if(m_pTaskInfoVec[idx]->heroItemVec.size() > 0)
    {
        Sprite* prop = NULL;
        if((m_pTaskInfoVec[idx]->itemVec.size() > 0) && (m_pTaskInfoVec[idx]->itemVec.size() < 2))
        {
            prop = CDataManager::getMonsterIconAndBg(m_pTaskInfoVec[idx]->heroItemVec.at(0).theId);
            prop->setScale(0.6f);
            prop->setPosition(tBg.width*0.73,tBg.height*0.4);
            cell->addChild(prop, 0, TASK_DEBRIS );
            //添加碎片背景
            Sprite* debrisBg = Sprite::createWithSpriteFrameName("debrisBg.png");
            cell->addChild(debrisBg);
            debrisBg->setScale(2.0f);
            debrisBg->setPosition(prop->getPosition());
            
            Size tProp = debrisBg->getContentSize();
            Sprite *pPropNum = Sprite::createWithSpriteFrameName("numbg.png");
            pPropNum->setPosition(tProp.width*0.78,tProp.height*0.15);
            pPropNum->setScale(0.25);
            debrisBg->addChild(pPropNum,99);
            
            char strNum[50] = {0};
            sprintf(strNum,"%d",m_pTaskInfoVec[idx]->itemVec.at(0).theNumber);
            
            Size tPropNum = pPropNum->getContentSize();
            
            Label *pNum = Label::createWithSystemFont(strNum,"Aril",30);
            pNum->setPosition(tPropNum.width*0.5,tPropNum.height*0.5);
            pPropNum->addChild(pNum,999);
            
            
        }
        else{
            for(int i = 0; i < m_pTaskInfoVec[idx]->heroItemVec.size(); i++)
            {
                prop = CDataManager::getMonsterIconAndBg(m_pTaskInfoVec[idx]->heroItemVec.at(i).theId);
                prop->setScale(2.3f);
                prop->setPosition(tBg.width*0.625 + i*90,tBg.height*0.5);
                cell->addChild(prop, 0, TASK_DEBRIS + i);
                //添加碎片背景
                Sprite* debrisBg = Sprite::createWithSpriteFrameName("debrisBg.png");
                cell->addChild(debrisBg);
                debrisBg->setScale(2.0f);
                debrisBg->setPosition(prop->getPosition());

            
                Size tProp = debrisBg->getContentSize();
                Sprite *pPropNum = Sprite::createWithSpriteFrameName("numbg.png");
                pPropNum->setPosition(tProp.width*0.78,tProp.height*0.15);
                pPropNum->setScale(0.25);
                debrisBg->addChild(pPropNum,99);
                
                char strNum[50] = {0};
                sprintf(strNum,"%d",m_pTaskInfoVec[idx]->heroItemVec.at(i).theNumber);
                
                Size tPropNum = pPropNum->getContentSize();
                
                Label *pNum = Label::createWithSystemFont(strNum,"Aril",30);
                pNum->setPosition(tPropNum.width*0.5,tPropNum.height*0.5);
                pPropNum->addChild(pNum,999);
                
            }
        }
        
        
    
    }
    //领取按钮
    std::string strPicName = "yellowbtn.png";
    std::string musicName = "music/click_btn.mp3";
    if(m_pTaskInfoVec[idx]->m_eTaskState == Task_Fininsh)
    {
        //看不见了，这条
        
    }
    else if(m_pTaskInfoVec[idx]->m_eTaskState == Task_Go)
    {
        strPicName = "yellowbtn.png";
    }
    else if(m_pTaskInfoVec[idx]->m_eTaskState == Task_Get)
    {
        strPicName = "greenbtn.png";
        musicName = "music/task.mp3";
    }
    
    Sprite *pYellowBtn = Sprite::createWithSpriteFrameName(strPicName.c_str());
    
    Sprite *pYellowBtnSelc = Sprite::createWithSpriteFrameName(strPicName.c_str());
    pYellowBtnSelc->setColor(Color3B::GRAY);
    
    m_iCurIdx = idx;
    
    LYMenuItemSprite *pYellowItem = LYMenuItemSprite::create(pYellowBtn,pYellowBtnSelc,CC_CALLBACK_1(CTaskLayer::OnUpOrGetAward, this), musicName.c_str());
    pYellowItem->setScale(2.6f);
    pYellowItem->setTag(int(m_iCurIdx));
   pYellowItem->setUserData((void*)idx);
    
    Menu *pMenuYellow = Menu::create(pYellowItem,NULL);
    pMenuYellow->setPosition(tBg.width*0.89,tBg.height*0.46);
    cell->addChild(pMenuYellow);
    
    if(m_pTaskInfoVec[idx]->m_iTaskType == 5)
    {
        m_GuidepMenu = pMenuYellow;
    }
    
    
    Size  tYellowBt = pYellowItem->getContentSize();
    
    std::string strText = "领取";
    
    if(m_pTaskInfoVec[idx]->m_eTaskState == Task_Fininsh)
    {
        
    }
    else if(m_pTaskInfoVec[idx]->m_eTaskState == Task_Get)
    {
        strText = "领取";
    }
    else if(m_pTaskInfoVec[idx]->m_eTaskState == Task_Go)
    {
        strText = "前往";
    }
    
    Label *pQianWanLabel =  Label::createWithTTF(strText.c_str(),"fonts/cuti.ttf",30);
    pQianWanLabel->enableOutline(Color4B(0,0,0,255),2);
    pQianWanLabel->setPosition(tYellowBt.width*0.5,tYellowBt.height*0.5);
    pYellowItem->addChild(pQianWanLabel);
    pQianWanLabel->setScale(0.35f);
  
    m_pTableViewVec.push_back(cell);
    
    }
    
    return cell;
}

void CTaskLayer::addCloseGuide(Ref* pSender)
{
    NotificationCenter::getInstance()->removeObserver(this, "guideClose");
    Vec2 pos = m_pMenuCose->convertToWorldSpace(Vec2::ZERO);
    guiderManagerLayer::getInstance()->createLayer(this, GUIDE_SHOW_FINGER, pos);
}


void CTaskLayer::OnUpdateCoinAndStone(Ref *pSender)
{
    
 
    
    
}


void CTaskLayer::OnRemoveEffect()
{
    OnCloseAwardLayer();
    log("sadda");
    
}


void CTaskLayer::OnUpOrGetAward(Ref *pSender)
{
    guiderManagerLayer::getInstance()->removeGuiderLayer();
    LYMenuItemSprite * p = ( LYMenuItemSprite *)pSender;

    long  idx= (long)p->getUserData();
    
    
    m_iCurIdx = (long)p->getUserData();
    log("前往 = %d", int(m_iCurIdx));
    if((m_pTaskInfoVec[m_iCurIdx]->m_eTaskState== Task_Go) && m_bValid)
    {
        //m_pTaskInfoVec[m_iCurIdx]->m_iTaskType ==
        if((m_pTaskInfoVec[m_iCurIdx]->m_iTaskType == 1) || (m_pTaskInfoVec[m_iCurIdx]->m_iTaskType == 2)
           || (m_pTaskInfoVec[m_iCurIdx]->m_iTaskType == 5) || (m_pTaskInfoVec[m_iCurIdx]->m_iTaskType == 6)
           || (m_pTaskInfoVec[m_iCurIdx]->m_iTaskType == 7) || (m_pTaskInfoVec[m_iCurIdx]->m_iTaskType == 8)
           || (m_pTaskInfoVec[m_iCurIdx]->m_iTaskType == 9) || (m_pTaskInfoVec[m_iCurIdx]->m_iTaskType == 10)
           || (m_pTaskInfoVec[m_iCurIdx]->m_iTaskType == 11) || (m_pTaskInfoVec[m_iCurIdx]->m_iTaskType == 15))
        {
            std::vector<levelAndStar> levelInfo = CPersonalData::getInstance()->getUserInfo()->m_userLevelVec;
            if((m_pTaskInfoVec[m_iCurIdx]->m_iTaskType == 10) || (m_pTaskInfoVec[m_iCurIdx]->m_iTaskType == 2))
            {
                CPersonalData::getInstance()->setlevelMode(1);
                levelInfo = CPersonalData::getInstance()->getUserInfo()->m_userEqliteLevelVec;
            }
            int m_midLevel = 1;
            int sum = 0;
            for(int i = 1; i < 15; i++)
            {
                sum += CDataManager::getAllLevelById(i);
                if(sum >= levelInfo.size()){
                    if(sum == levelInfo.size())
                    {
                        m_midLevel = i + 1;
                    }
                    else{
                        m_midLevel = i;
                    }
                    break;
                }
            }
            CFightData::getInstance()->setNowMidLevelId(m_midLevel);
            if((CPersonalData::getInstance()->getUserInfo()->m_iEqliteSmallLevel <= 0) && (CPersonalData::getInstance()->getlevelMode() == 1))
            {
                CPersonalData::getInstance()->setlevelMode(0);//如果精英关卡还未开启则设置回来
                Sprite * thetarget = CFightManager::getTheFlyWord("精英关卡还未开启","fonts/cuti.ttf");
                thetarget->setPosition(m_winSize.width * 0.48f, m_winSize.height / 2);
                addChild(thetarget,99);
            }
            else{
                CLayerManager::GetSingletonPtr()->createLayer(eLayer_LevelInfo, this->getParent(),false);
                removeFromParent();
            }
        }
        else if((m_pTaskInfoVec[m_iCurIdx]->m_iTaskType == 14) || (m_pTaskInfoVec[m_iCurIdx]->m_iTaskType == 13)
                || (m_pTaskInfoVec[m_iCurIdx]->m_iTaskType == 17))
        {
            if(CPersonalData::getInstance()->getUserInfo() ->m_iSmallLevel < 11)
            {
                Sprite * thetarget = CFightManager::getTheFlyWord("'英雄'还未开启","fonts/cuti.ttf");
                thetarget->setPosition(m_winSize.width * 0.48f, m_winSize.height / 2);
                addChild(thetarget,99);
            }
            else{
                CLayerManager::GetSingletonPtr()->createLayer(eLayer_Compound,this->getParent(),false);
                removeFromParent();
            }
        }
        else if((m_pTaskInfoVec[m_iCurIdx]->m_iTaskType == 4) || (m_pTaskInfoVec[m_iCurIdx]->m_iTaskType == 18))
        {
            if(CPersonalData::getInstance()->getUserInfo() ->m_iSmallLevel < 4)
            {
                Sprite * thetarget = CFightManager::getTheFlyWord("'学院'还未开启","fonts/cuti.ttf");
                thetarget->setPosition(m_winSize.width * 0.48f, m_winSize.height / 2);
                addChild(thetarget,99);
            }
            else{
                CLayerManager::GetSingletonPtr()->createLayer(eLayer_LvSkill, this->getParent(),false);
                removeFromParent();
            }
        }
        else if((m_pTaskInfoVec[m_iCurIdx]->m_iTaskType == 3) || (m_pTaskInfoVec[m_iCurIdx]->m_iTaskType == 19))
        {
            if(CPersonalData::getInstance()->getUserInfo() ->m_iSmallLevel < 3)
            {
                Sprite * thetarget = CFightManager::getTheFlyWord("'升级'还未开启","fonts/cuti.ttf");
                thetarget->setPosition(m_winSize.width * 0.48f, m_winSize.height / 2);
                addChild(thetarget,99);
            }
            else{
                CLayerManager::GetSingletonPtr()->createLayer(eLayer_LvUp, this->getParent(),false);
                removeFromParent();
            }
        }
        else if(m_pTaskInfoVec[m_iCurIdx]->m_iTaskType == 12)
        {
            if(CPersonalData::getInstance()->getUserInfo() ->m_iSmallLevel < 10)
            {
                Sprite * thetarget = CFightManager::getTheFlyWord("'仓库'还未开启","fonts/cuti.ttf");
                thetarget->setPosition(m_winSize.width * 0.48f, m_winSize.height / 2);
                addChild(thetarget,99);
            }
            else{
                CLayerManager::GetSingletonPtr()->createLayer(eLayer_PropLayer, this->getParent(),false);
                removeFromParent();
            }
        }
        else if(m_pTaskInfoVec[m_iCurIdx]->m_iTaskType == 16)
        {
            CLayerManager::GetSingletonPtr()->createLayer(eLayer_DailyTaskLayer, this->getParent(),false);
            removeFromParent();
        }
        else if(m_pTaskInfoVec[m_iCurIdx]->m_iTaskType == 20)
        {
            if(CPersonalData::getInstance()->getUserInfo()->m_iSmallLevel >= 20)
            {
                m_bValid = false;
                CNetControlModule::sharedNetControlModule()->getUserTeamInfo(this, menu_selector(CTaskLayer::getUserTeamInfo));
            }
        }
    }
    else
    {
        if(!m_bValid)
        {
            return;
        }
        else{
            m_bValid = false;
            if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_GET_TASK) && (guiderManagerLayer::getInstance()->getStepNum() == 2))
            {
                CDataManager::saveGuideInfoToDB(guiderManagerLayer::getInstance()->getGuideType());
                guiderManagerLayer::getInstance()->removeGuiderLayer();
            }
            int iMoney =  m_pTaskInfoVec[m_iCurIdx]->m_iMoney;
            int iExp = m_pTaskInfoVec[m_iCurIdx]->m_iExp;
            int iStone = m_pTaskInfoVec[m_iCurIdx]->m_stone;
            
            //m_pTaskGetLayer = CTaskGetSucess::create(iMoney,iExp);
            //m_pTaskGetLayer->GetAwardSucess(this,menu_selector(CTaskLayer::OnCloseAwardLayer));
            //addChild(m_pTaskGetLayer,10);
            
            std::string awardStr;
            char buf[128];
            if(iMoney > 0)
            {
                sprintf(buf, "获得%d金币", iMoney);
                awardStr += buf;
                awardStr += "\n";
            }
            if(iStone > 0)
            {
                sprintf(buf, "获得%d魔法石", iStone);
                awardStr += buf;
                awardStr += "\n";
            }
            if(iExp > 0)
            {
                sprintf(buf, "获得%d经验", iExp);
                awardStr += buf;
            }
            if(iMoney || iStone || iExp)
            {
                Sprite * thetarget = CFightManager::getRewardWord(awardStr.c_str());
                addChild(thetarget, 10);
                thetarget->setPosition(m_winSize.width * 0.48f, m_winSize.height / 2);
            }
            else{
                float delay = 0;
                for (int i = 0; i < m_pTaskInfoVec[m_iCurIdx]->itemVec.size(); i++) {
                    std::string name = CDataManager::getPropInfoById(m_pTaskInfoVec[m_iCurIdx]->itemVec.at(i).theId)->m_sItemName;
                    int num = m_pTaskInfoVec[m_iCurIdx]->itemVec.at(i).theNumber;
                    __String * temp = __String::createWithFormat("获得%sX%d", name.c_str(), num);
                    delay += 0.5f;
                    Sprite * thetarget = CFightManager::getRewardWord(temp->getCString(), 0.5*i);
                    addChild(thetarget, 10);
                    thetarget->setPosition(m_winSize.width * 0.48f, m_winSize.height / 2);
                }
                for (int i = 0; i < m_pTaskInfoVec[m_iCurIdx]->heroItemVec.size(); i++) {
                    std::string name = CDataManager::getMonsterInfoByMonsterId(m_pTaskInfoVec[m_iCurIdx]->heroItemVec.at(i).theId)->monsterName;
                    int num = m_pTaskInfoVec[m_iCurIdx]->heroItemVec.at(i).theNumber;
                    __String * temp = __String::createWithFormat("获得%s碎片X%d", name.c_str(), num);
                    Sprite * thetarget = CFightManager::getRewardWord(temp->getCString(), delay + 0.5*i);
                    addChild(thetarget, 10);
                    thetarget->setPosition(m_winSize.width * 0.48f, m_winSize.height / 2);
                }
            }
            
            //显示获取的碎片和道具名字
            
            
            //物品
            std::vector<IdAndNum> propVec = m_pTaskInfoVec[m_iCurIdx]->itemVec;
            
            for(int i = 0; i < propVec.size(); i++)
            {
                int iNum = CDBMgr::getInstance()->HavePropNumById(propVec.at(i).theId);
                CDataManager::SaveUserPropToDB(propVec.at(i).theId, propVec.at(i).theNumber + iNum);//保存道具
            }
            //碎片
            std::vector<IdAndNum> debricVec = m_pTaskInfoVec[m_iCurIdx]->heroItemVec;
            for(int i = 0; i < debricVec.size(); i++)
            {
                CDataManager::saveUserDebrisToDB(debricVec.at(i).theId, debricVec.at(i).theNumber);
            }
            CDataManager::saveTaskState(m_pTaskInfoVec[m_iCurIdx]->m_iTaskId, Task_Fininsh, true);
            
            TableViewCell* cell =  m_tableView->cellAtIndex(idx);
            
            
            Sprite *pp =    (Sprite *)cell->getChildByTag(TASK_ICON);
            
            
            
            Sprite *pic =  Sprite::create();
            pic->setContentSize(pp->getContentSize());
            pic->Node::setScale(2);
            
            pic->setPosition(pp->getContentSize().width*0.5,pp->getContentSize().height*0.5);
            pp->addChild(pic);
            
            
            Animate* theAnimate = CFightManager::getAnimateWithPath("taskEffect",1,5);
            CallFunc* getOff = CallFunc::create(CC_CALLBACK_0(CTaskLayer::OnRemoveEffect, this));
            pic->runAction(Sequence::create(theAnimate,getOff,NULL));
            
            int addStaminaNUm=0;
            CDataManager::judgeUserIfUpgrade(this, iExp,&addStaminaNUm);
            
            //spf
            CNetWorkDataSingle::GetSingletonPtr()->SaveUserMCF(this,menu_selector(CTaskLayer::OnUpdateCoinAndStone),iMoney,iStone,addStaminaNUm);
            TDCCVirtualCurrency::onReward(iStone, "任务奖励");
            
            //CDataManager::saveUserStoneAndCoinToDB(iMoney, 0,true);
            
            //CDataManager::saveUserStoneAndCoinToDB(iStone, 1,true);
        }
    }
}

void CTaskLayer::getUserTeamInfo(Ref* pSender)
{
    if(CPersonalData::getInstance()->getState() == 0)
    {
        CPersonalData::getInstance()->setEnemyType(RANDOM_ENEMY);
        CPersonalData::getInstance()->setrestartPlayMusic(false);
        CLayerManager::GetSingletonPtr()->createLayerWithLoading(eLayer_PVPLayer, NULL,false);
    }
    else
    {
        m_bValid = true;
    }
}


void CTaskLayer::OnCloseAwardLayer()
{
    //    //删除已领取的任务
    //    for(std::vector<TaskBase *>::iterator it = m_pTaskInfoVec.begin(); it != m_pTaskInfoVec.end(); it++)
    //    {
    //        if((*it)->m_iTaskId == m_pTaskInfoVec[m_iCurIdx]->m_iTaskId)
    //        {
    //            CDataManager::eraseTaskById((*it)->m_iTaskId);//删除任务信息，避免下次登入已领取任务还在
    //            m_pTaskInfoVec.erase(it);
    //            break;
    //        }
    //    }
    m_pTaskInfoVec.clear();
    getTask();
    m_tableView->reloadData();
    m_bValid = true;
}

//cell的数量
ssize_t CTaskLayer::numberOfCellsInTableView(TableView *table)
{
    return m_pTaskInfoVec.size();
}


void CTaskLayer::onExit()
{
    _eventDispatcher->removeEventListener(listen);
    CBaseLayer::onExit();
    NotificationCenter::getInstance()->removeObserver(this, "guideClose");
}

bool CTaskLayer::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    return true;
}

void CTaskLayer::TaskSort()
{
    //根据完成度进行排序
    
    std::vector<TaskBase *>sortVec;
    //将待领区的任务排在前面
    TaskState maxValue = Task_Go;
    int taskId = 1000;
    float percent = 0;
    for(int i = 0; i < m_pTaskInfoVec.size(); i++)
    {
        for(int j = 0; j < m_pTaskInfoVec.size(); j++)
        {
            bool isExisted = false;
            for(int k = 0; k < sortVec.size(); k++)
            {
                if(m_pTaskInfoVec[j]->m_iTaskId == sortVec.at(k)->m_iTaskId)
                {
                    isExisted = true;
                    break;
                }
            }
            if(!isExisted)
            {
                if (maxValue < m_pTaskInfoVec.at(j)->m_eTaskState) {
                    maxValue = m_pTaskInfoVec.at(j)->m_eTaskState;
                    taskId = m_pTaskInfoVec.at(j)->m_iTaskId;
                }
                else if(maxValue == m_pTaskInfoVec.at(j)->m_eTaskState)
                {
                    if(maxValue == Task_Get)
                    {
                        if(taskId > m_pTaskInfoVec.at(j)->m_iTaskId)
                        {
                            taskId = m_pTaskInfoVec.at(j)->m_iTaskId;
                        }
                    }
                    else{
                        float temp = CDataManager::getTaskProByType(m_pTaskInfoVec.at(j)->m_iTaskType)/float(m_pTaskInfoVec.at(j)->m_iNum);
                        if(percent < temp)
                        {
                            percent = temp;
                            taskId = m_pTaskInfoVec.at(j)->m_iTaskId;
                        }
                        else if(percent == temp)
                        {
                            if(taskId > m_pTaskInfoVec.at(j)->m_iTaskId)
                            {
                                taskId = m_pTaskInfoVec.at(j)->m_iTaskId;
                            }
                            
                        }
                    }
                }
            }
        }
        sortVec.push_back(CDataManager::getTaskInfoById(taskId));
        maxValue = Task_Go;
        taskId = 1000;
        percent = 0;
    }
    
    m_pTaskInfoVec = sortVec;
}

void CTaskLayer::getTask()
{
    std::vector<TaskBase *>tempVec = CDataManager::GetAllTaskInfo();
    for(int i = 0; i < tempVec.size(); i++)
    {
        if(tempVec.at(i)->m_eTaskState != Task_Fininsh)
        {
            if(m_bMainLayer)
            {
                if(tempVec.at(i)->taskProp == 0)
                {
                    m_pTaskInfoVec.push_back(tempVec.at(i));
                }
            }
            else{
                if(tempVec.at(i)->taskProp == 1)
                {
                    m_pTaskInfoVec.push_back(tempVec.at(i));
                }
            }
        }
    }
    
    
    std::vector<TaskBase *>taskVec;
    for(int i = 0; i < m_pTaskInfoVec.size(); i++)
    {
        bool bExisted = false;//标识是否已有类该类型任务
        for(int j = 0; j < taskVec.size() ;j++)
        {
            if(m_pTaskInfoVec[i]->m_iTaskType == taskVec[j]->m_iTaskType)
            {
                bExisted = true;
                break;
            }
        }
        if(!bExisted)
        {
            taskVec.push_back(m_pTaskInfoVec[i]);
        }
    }
    m_pTaskInfoVec = taskVec;
    TaskSort();
}

bool CTaskLayer::init()
{
    //任务背景
    
    //CBaseLayer::init();
    CBaseLayer::initWithColor(Color4B(0, 0, 0, 200));
    
    listen = EventListenerTouchOneByOne::create();
    listen->setSwallowTouches(true);
    listen->onTouchBegan = CC_CALLBACK_2(CTaskLayer::onTouchBegan, this);
    //    listen->onTouchMoved = CC_CALLBACK_2(CTaskLayer::onTouchMoved, this);
    //    listen->onTouchEnded = CC_CALLBACK_2(CTaskLayer::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listen, this);
    
    getTask();
    
    m_pMainTaks = Scale9Sprite::createWithSpriteFrameName("Soldierbg.png");
    m_pMainTaks->setContentSize(Size(1015, 707));
    std::string strMainText = "主线任务";
    if(!m_bMainLayer)
    {
        m_pMainTaks = Scale9Sprite::createWithSpriteFrameName("common_bg.png");
        m_pMainTaks->setContentSize(Size(1023, 707));
        strMainText = "日常任务";
    }
    
    m_pMainTaks->setPosition(m_winSize.width*0.5,m_winSize.height*0.5);
    addChild(m_pMainTaks);
    Size bgSize = m_pMainTaks->getBoundingBox().size;
    Label *pMainText = Label::createWithSystemFont(strMainText.c_str(),"Aril", 30);
    
    pMainText->setPosition(bgSize.width*0.5, bgSize.height*0.945);
    m_pMainTaks->addChild(pMainText);
    
    Sprite *pCloseSprite = Sprite::createWithSpriteFrameName("closeBtn.png");
    
    Sprite *pCloseSpriteSelc  = Sprite::createWithSpriteFrameName("closeBtn.png");
    pCloseSpriteSelc->setColor(Color3B::GRAY);
    LYMenuItemSprite *pCloseItem = LYMenuItemSprite::create(pCloseSprite,pCloseSpriteSelc,CC_CALLBACK_1(CTaskLayer::OnExitLvLayer, this));
    
    CommonData::enlargeItemSprite(pCloseItem, 2);
//    pCloseItem->setContentSize(pCloseItem->getContentSize()*3);
//    
//    pCloseSprite->setPosition(Vec2(pCloseSprite->getContentSize().width*1, pCloseSprite->getContentSize().height*1));
    
    
    m_pMenuCose =  Menu::create(pCloseItem,NULL);
    
    m_pMenuCose->setPosition(bgSize.width*0.962,bgSize.height*0.95);
    m_pMainTaks->addChild(m_pMenuCose,1);
    if(!m_bMainLayer)
    {
        pMainText->setPosition(bgSize.width*0.5,bgSize.height*0.91);
        m_pMenuCose->setPosition(bgSize.width*0.955,bgSize.height*0.89);
    }
    if(m_pTaskInfoVec.size() == 0)
    {
        Sprite* theBG = Sprite::createWithSpriteFrameName("zhdt.png");
        addChild(theBG,100);
        theBG->setPosition(Vec2(m_winSize.width * 0.5f, m_winSize.height * 0.5f));
        Label* thetarget = Label::createWithTTF("今天的任务已领完，请明天再来吧！","fonts/cuti.ttf", 35);
        theBG->addChild(thetarget);
        thetarget->setColor(Color3B(253,161,63));
        thetarget->setPosition(Vec2(theBG->getBoundingBox().size.width*0.5f, theBG->getBoundingBox().size.height*0.5f));
    }
    addTableView();
    Vec2 m_guidePos;
    if(m_GuidepMenu)
    {
        m_guidePos = m_GuidepMenu->convertToWorldSpace(Vec2::ZERO);
    }
    
    if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_GET_TASK) && (guiderManagerLayer::getInstance()->getStepNum() == 1))
    {
        guiderManagerLayer::getInstance()->setStepNum(guiderManagerLayer::getInstance()->getStepNum() + 1);
        guiderManagerLayer::getInstance()->createLayer(this, GUIDE_SHOW_BOTH, m_guidePos);
        //注册事件
        NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(CTaskLayer::addCloseGuide), "guideClose", NULL);
    }
    return true;
}


void CTaskLayer::OnExitLvLayer(Ref *psender)
{
    if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_GET_TASK) && (guiderManagerLayer::getInstance()->getStepNum() == 4))
    {
        guiderManagerLayer::getInstance()->removeGuiderLayer();
        NotificationCenter::getInstance()->postNotification("upgradeAfter");
    }
    
    removeFromParent();
}
NS_CC_EXT_END