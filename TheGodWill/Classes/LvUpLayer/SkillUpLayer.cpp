//
//  SkillUpLayer.cpp
//  TheGodWill
//
//  Created by prefer_shan on 14/12/22.
//
//

#include "SkillUpLayer.h"
#include "LYPersonalData.h"
#include "DataManager.h"
#include "DBMgr.h"
#include "CSKillLVUpQueen.h"
#include "LvUpDetail.h"
#include "SoldierSkillUpLayer.h"
#include "DataManager.h"
#include "SkillLvLayerQue.h"
#include "sysTime.h"
#include "LYFightManager.h"
#include "Common.h"
#include "CSharedPlist.h"
#include "propGetPathLayer.h"
#include "NetWorkDataSingle.h"
#include "costStoneWndLayer.h"
#include "CHitDialog.h"
#include "DBMgr.h"

CSkillUpLayer::CSkillUpLayer(int index,int iTag,int SkillIndex,bool bHero)
{
    m_winSize = Director::getInstance()->getWinSize();
    m_pMenuLvUp = NULL;
    m_bMenuLvUp = false;
    m_bMenuClose = false;
    m_pMenuClose= NULL;
    m_index = index;
    m_iSkillIdIndex = iTag;
    
   
    m_iThreeOfInex = SkillIndex;
    m_iNeedNum1 = 0;
    m_iHaveNum1 = 0;
    m_iNeedNum2 = 0;
    m_iHaveNum2 = 0;
    m_iNeedNum3 = 0;
    m_iHaveNum3 = 0;
    m_iSkillId = 0;
    m_pSkillInfo = NULL;
    m_iNeedTime = 0;
    m_bHero = bHero;
    m_iLv = 0;
    m_pListener = NULL;
    m_pSkillName = NULL;
    m_needStoneNum = 0;
    m_node = NULL;
    for(int i = 0; i < sizeof(m_strNeedTime)/sizeof(char); i++)
    {
        m_strNeedTime[i] = '0';
    }
    CSharedPlist::getInstance()->pushPlist("plist/propIcon_2.plist");
}

CSkillUpLayer::~CSkillUpLayer()
{
    CSharedPlist::getInstance()->erasePlist("plist/propIcon_2.plist");
}

CSkillUpLayer*  CSkillUpLayer::create(int index,int iTag,int SkillIndex,bool bHero)
{ \
    CSkillUpLayer *pRet = new CSkillUpLayer(index,iTag,SkillIndex,bHero); \
    if (pRet && pRet->init()) \
    { \
        pRet->autorelease(); \
        return pRet; \
    } \
    else \
    { \
        delete pRet; \
        pRet = NULL; \
        return NULL; \
    } \
}


void CSKillLvLayerQue::StartTouch()
{
    m_plistener = EventListenerTouchOneByOne::create();//创建一个触摸监听
    m_plistener->setSwallowTouches(true);//设置不想向下传递触摸  true是不想 默认为false
    
    m_plistener->onTouchBegan = CC_CALLBACK_2(CSKillLvLayerQue::onTouchBegan, this);
    m_plistener->onTouchMoved = CC_CALLBACK_2(CSKillLvLayerQue::onTouchMoved, this);
    m_plistener->onTouchEnded = CC_CALLBACK_2(CSKillLvLayerQue::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(m_plistener, this);
}

void CSKillLvLayerQue::EndTouch()
{
    if(m_plistener)
    {
        _eventDispatcher->removeEventListener(m_plistener);
    }
    NotificationCenter::getInstance()->postNotification("upgradeGuide");
}


void CSkillUpLayer::OnRemoveNotice()
{
    Label *  p =  (Label *)getChildByTag(1000);
    
    if(p)
    {
        p->removeFromParent();
    }
}


void CSkillUpLayer::OnSkillUpMenu(Ref *pSender)
{
    LvUpDetail*pLvDetail  = (LvUpDetail*)getParent();
    
    int iMonsterId = m_pSoldierVec[m_index]->monsterId;
                     
    bool bHaveSkill =  CDBMgr::getInstance()->HaveSkillUpgradeInfo(iMonsterId,m_iSkillId);
    
    bool bEnough = true;
    
    if(m_iHaveNum1<m_iNeedNum1)
    {
        bEnough = false;
    }
    if(m_iHaveNum2<m_iNeedNum2)
    {
       bEnough = false;
    }
    
    if(m_iHaveNum3<m_iNeedNum3)
    {
        bEnough = false;
    }
    
    if(!bEnough)
    {
        Sprite * thetarget = CFightManager::getTheFlyWord("材料不足!","fonts/cuti.ttf");
        thetarget->setPosition(Vec2(m_winSize.width / 2, m_winSize.height / 2));
        addChild(thetarget, 100);
        return;
    }
    
    if(bHaveSkill)
    {
        Sprite * thetarget = CFightManager::getTheFlyWord("该技能已经在升级队列中!","fonts/cuti.ttf");
        thetarget->setPosition(Vec2(m_winSize.width / 2, m_winSize.height / 2));
        addChild(thetarget, 100);
        
        return;
    }
    
    if(CDBMgr::getInstance()->GetSkillUpgradeInfoVec().size()>=3)
    {
        SimpleAudioEngine::getInstance()->playEffect("music/noBtn.mp3");
        
        Sprite * thetarget = CFightManager::getTheFlyWord("最多同时升级3个技能!","fonts/cuti.ttf");
        thetarget->setPosition(Vec2(m_winSize.width / 2, m_winSize.height / 2));
        addChild(thetarget, 100);
        
        return;
    }
    
    char strPropNum[20] = {0};
    
    CommonData::GetSingletonPtr()->m_PropMap.clear();
    
    if(m_iHaveNum1>0)
    {
        m_iHaveNum1 -= m_iNeedNum1;
        sprintf(strPropNum,"%d",m_iHaveNum1);
        m_pLabel1Have->setString(strPropNum);
        //CDBMgr::getInstance()->DelPropIdByNum(m_iProp1,m_iNeedNum1);
        CDataManager::SaveUserPropToDB(m_iProp1,m_iHaveNum1);
        CommonData::GetSingletonPtr()->m_PropMap[m_iProp1] = m_iNeedNum1;
        
    }
     if(m_iHaveNum2>0)
     {
         m_iHaveNum2 -= m_iNeedNum2;
        sprintf(strPropNum,"%d",m_iHaveNum2);
        m_pLabel2Have->setString(strPropNum);
        ///CDBMgr::getInstance()->DelPropIdByNum(m_iProp2,m_iNeedNum2);
        CDataManager::SaveUserPropToDB(m_iProp2,m_iHaveNum2);
        CommonData::GetSingletonPtr()->m_PropMap[m_iProp2] = m_iNeedNum2;
         
    }
    if(m_iHaveNum3>0)
    {
        m_iHaveNum3 -= m_iNeedNum3;
        sprintf(strPropNum,"%d",m_iHaveNum3);
        m_pLabel3Have->setString(strPropNum);
        //CDBMgr::getInstance()->DelPropIdByNum(m_iProp3,m_iNeedNum3);
        CDataManager::SaveUserPropToDB(m_iProp3,m_iHaveNum3);
        CommonData::GetSingletonPtr()->m_PropMap[m_iProp3] = m_iNeedNum3;
    }
    
    //保存消耗的道具到数据库
    //技能id
    
    int iNowTime = CGameTime::getNowTimeInSecond();
    
    //新手引导加入
//    if(!((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SKILL_UPGRADE) && (guiderManagerLayer::getInstance()->getStepNum() == 7)))
//    {
//        CDBMgr::getInstance()->AddSkillUpgradeToStr(iMonsterId,m_iSkillId,iNowTime,m_iNeedTime);
//    }
    CDBMgr::getInstance()->AddSkillUpgradeToStr(iMonsterId,m_iSkillId,iNowTime,m_iNeedTime);
    
    LvUpDetail* pLayer = (LvUpDetail*)getParent();
    if(pLayer)
    {
        pLayer->addSkillItem();
    }
    
    if(m_bHero)
    {
        CSKillLvLayerQue *pQue =  (CSKillLvLayerQue*)pLvDetail->getChildByTag(9000);
        pQue->OnShowHeroSkillLv(m_pHeroSkillInfo,m_iNeedTime);
    
    }
    else
    {
        CSKillLvLayerQue *pQue =  (CSKillLvLayerQue*)pLvDetail->getChildByTag(9000);
        pQue->OnShowSkillLV(m_pSkillInfo,m_iNeedTime);
    }

    
    if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SKILL_UPGRADE) && (guiderManagerLayer::getInstance()->getStepNum() == 7))
    {
        NotificationCenter::getInstance()->postNotification("clickUpgrade");
        LvUpDetail *pLayer = (LvUpDetail*)getParent();
        if(pLayer)
        {
            pLayer->m_pSprite->setVisible(false);
        }
        CDataManager::saveGuideInfoToDB(GUIDE_SKILL_UPGRADE);
    }
    else
    {
        LvUpDetail *pLayer = (LvUpDetail*)getParent();
        if(pLayer)
        {
            pLayer->m_pSprite->setVisible(true);
        }
    }
    
    removeFromParent();
}


bool CSkillUpLayer::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    if(m_pMenuLvUp)
    {
        m_bMenuLvUp = m_pMenuLvUp->onTouchBegan(touch,event);
    }
    
    if(m_pMenuClose)
    {
        m_bMenuClose = m_pMenuClose->onTouchBegan(touch,event);
    }
    
    return true;
}

void CSkillUpLayer::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event)
{
    if(m_bMenuLvUp)
    {
        m_pMenuLvUp->onTouchMoved(touch,event);
    }
    else if(m_bMenuClose)
    {
        m_pMenuClose->onTouchMoved(touch,event);
    }
}

void CSkillUpLayer::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event)
{
    if(m_bMenuLvUp)
    {
        m_pMenuLvUp->onTouchEnded(touch,event);
        m_bMenuLvUp = false;
    }
    else if(m_bMenuClose)
    {
        m_pMenuClose->onTouchEnded(touch,event);
        m_bMenuClose = false;
    }
}

void CSkillUpLayer::onTouchCancelled(Touch *touch, Event *event)
{
    if(m_bMenuLvUp)
    {
        m_pMenuLvUp->onTouchCancelled(touch,event);
        m_bMenuLvUp = false;
    }
    else if(m_bMenuClose)
    {
        m_pMenuClose->onTouchCancelled(touch,event);
        m_bMenuClose = false;
    }
}



bool SortByLv(const FighterInfo *v1,const FighterInfo *v2)//注意：本函数的参数的类型一定要与vector中元素的类型一致
{
    
    if(v1->rare == v2->rare)
    {
        return v1->level>v2->level;
        
    }
    return v1->rare>v2->rare;
}



bool CSkillUpLayer::init()
{
    //CBaseLayer::init();
    CBaseLayer::initWithColor(Color4B(0, 0, 0, 125));
    
    m_pListener = EventListenerTouchOneByOne::create();
    m_pListener->setSwallowTouches(true);
    
    m_pListener->onTouchBegan = CC_CALLBACK_2(CSkillUpLayer::onTouchBegan, this);
    m_pListener->onTouchMoved = CC_CALLBACK_2(CSkillUpLayer::onTouchMoved, this);
    m_pListener->onTouchEnded = CC_CALLBACK_2(CSkillUpLayer::onTouchEnded, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(m_pListener, this);
    std::vector<FighterInfo*>  pAll;
    
    pAll = CPersonalData::getInstance()->getAllSoldierInfo();
    
    for(int i=0;i<pAll.size();i++)
    {
        //1 英雄  0怪物
        if(m_bHero)
        {
            if(pAll[i]->theType == 1)
            {
                m_pSoldierVec.push_back(pAll[i]);
            }
        }
        else
        {
            if(pAll[i]->rare>0)
            {
                m_pSoldierVec.push_back(pAll[i]);
            }
        }
    }

    std::sort(m_pSoldierVec.begin(),m_pSoldierVec.end(),SortByLv);
    
    
    m_pBg = Sprite::createWithSpriteFrameName("storageBg.png");
    
    m_pBg->setPosition(Vec2(m_winSize.width*0.5, m_winSize.height*0.5));
    addChild(m_pBg);
    
    Size tBg = m_pBg->getContentSize();
    
    //技能升级
    Label *pSkillUpLable = Label::createWithTTF("技能升级", "fonts/cuti.ttf", 30);
    pSkillUpLable->setColor(Color3B(255, 227, 75));
    pSkillUpLable->setPosition(tBg.width*0.5,tBg.height*0.935);
    m_pBg->addChild(pSkillUpLable);
    
    
    initUI();
    
    //关闭按钮
    Sprite *pCloseBtn = Sprite::createWithSpriteFrameName("closeBtn.png");
    Sprite *pCloseBtnSelect = Sprite::createWithSpriteFrameName("closeBtn.png");
    pCloseBtnSelect->setColor(Color3B::GRAY);
    
    LYMenuItemSprite *pCloseItem = LYMenuItemSprite::create(pCloseBtn,pCloseBtnSelect,CC_CALLBACK_1(CSkillUpLayer::OnCloseMenu,this));
    CommonData::enlargeItemSprite(pCloseItem, 2);
    
    m_pMenuClose = Menu::create(pCloseItem,NULL);
    m_pMenuClose->setPosition(tBg.width*0.945,tBg.height*0.935);
    m_pBg->addChild(m_pMenuClose);
   
    if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SKILL_UPGRADE) && (guiderManagerLayer::getInstance()->getStepNum() == 6))
    {
        Vec2 pos = m_pMenuLvUp->convertToWorldSpace(Vec2::ZERO);
        guiderManagerLayer::getInstance()->setStepNum(guiderManagerLayer::getInstance()->getStepNum() + 1);
        guiderManagerLayer::getInstance()->createLayer(this, GUIDE_SHOW_FINGER, pos);
    }
    
    return true;
}

void CSkillUpLayer::initUI()
{
  
    if(m_bHero)
    {
        heroSkillInfo();
    }
    else{
        soliderSkillInfo();
    }
    skillPropInfo();
}

void CSkillUpLayer::RefreshUI()
{
    
    m_pBg->removeAllChildren();
    
    m_node = NULL;
    m_pMenuLvUp = NULL;
    m_pMenuClose = NULL;
    

    if(m_bHero)
    {
        heroSkillInfo();
    }
    else{
        soliderSkillInfo();
    }
    skillPropInfo();
    
}

void CSkillUpLayer::heroSkillInfo()
{
    Size tBg = m_pBg->getContentSize();
    std::string strSkillName;
    vector<heroSkill*> pVec = CDBMgr::getInstance()->getHeroSkillByMonsterId(m_pSoldierVec[m_index]->monsterId);
    
    vector<heroSkill*> pHeroSkillInfo;
    

    for(int i=0;i<pVec.size();i++)
    {
        if(pVec[i]->heroSkillId!=1)
        {
            pHeroSkillInfo.push_back(pVec[i]);
            
        }
    
    }
    
    char strIcoPath[50] = {0};
    sprintf(strIcoPath,"%s",pHeroSkillInfo[m_iSkillIdIndex]->skillIcon.c_str());
    m_pHeroSkillInfo = pHeroSkillInfo[m_iSkillIdIndex];
    
    Sprite *pSkillPic = Sprite::createWithSpriteFrameName(strIcoPath);
    pSkillPic->setScale(3);
    pSkillPic->setPosition(tBg.width*0.1,tBg.height*0.77);
    m_pBg->addChild(pSkillPic);
    
    //技能名字
    strSkillName = pHeroSkillInfo[m_iSkillIdIndex]->heroSkillName;
    m_pSkillName = Label::createWithTTF(strSkillName,"fonts/cuti.ttf",30);
    m_pSkillName->setColor(Color3B::ORANGE);
    
    m_pSkillName->setAnchorPoint(Vec2(0,0.5));
    m_pSkillName->setPosition(tBg.width*0.2,tBg.height*0.815);
    m_pBg->addChild(m_pSkillName);
    
    //技能描述
    std::string strSkillDesc = pHeroSkillInfo[m_iSkillIdIndex]->heroSkillDes;
    
    
    int iLv = CDBMgr::getInstance()->GetHeroSkillLvById(pHeroSkillInfo[m_iSkillIdIndex]->heroSkillId);
    
    int iHeroId = pHeroSkillInfo[m_iSkillIdIndex]->heroSkillId;
    
    float fHurtNum = CDataManager::getTheDamageWithHeroKillIdAndLevel(iHeroId,iLv);
    
    int iHurtNum = 0;
    
    if(iHeroId == 3)
    {
        iHurtNum = fHurtNum*m_pSoldierVec[m_index]->attk;
    }
    else
    {
        iHurtNum = fHurtNum;
    }
    
    float fTime =  CDataManager::getTheLastTimeWithIdAndLevel(iHeroId,iLv);
    
    char strTime[50] = {0};
    
    sprintf(strTime,"%.2f",fTime);
    
    char strDesc[200] = {0};
    
    if(iHeroId==2)
    {
        sprintf(strDesc,strSkillDesc.c_str(),iHurtNum,strTime);
    }
    else if(iHeroId == 3)
    {
        sprintf(strDesc,strSkillDesc.c_str(),iHurtNum);
    }
    else
    {
        sprintf(strDesc,strSkillDesc.c_str(),strTime);
    }
    
    
    int iHeroSkillId = pHeroSkillInfo[m_iSkillIdIndex]->heroSkillId;
    
    userInfoInDB* userInfoDB = CDataManager::getTheUSerInfDB();
    

    std::vector<IdAndNum> HeroSkillVec =  userInfoDB->heroSkills;
    //CPersonalData::getInstance()->getUserInfo()->m_heroSkillsVec;
    
    std::vector<IdAndNum> pSkillVec = userInfoDB->heroSkills;
    //CPersonalData::getInstance()->getUserInfo()->m_heroSkillsVec;
    
    m_iSkillId = iHeroSkillId;
    
    for(int i=0;i<pSkillVec.size();i++)
    {
        if(pSkillVec[i].theId == iHeroSkillId)
        {
            m_iLv = pSkillVec[i].theNumber;
            
            break;
        }
    }
    
    Label *pLabelDesc = Label::createWithTTF(strDesc,"fonts/cuti.ttf",25);
    pLabelDesc->setAnchorPoint(Vec2(0,1));
    pLabelDesc->setWidth(tBg.width * 0.74f);
    pLabelDesc->setPosition(tBg.width*0.2,tBg.height*0.77);
    m_pBg->addChild(pLabelDesc);
    
    //详细属性背景
    Sprite* propBg = Sprite::createWithSpriteFrameName("skillUpgrade_di.png");
    m_pBg->addChild(propBg,0,996);
    propBg->setScale(3.0f);
    propBg->setPosition(Vec2(tBg.width * 0.5f, tBg.height * 0.4f));
    
    float fHurtNum2 = CDataManager::getTheDamageWithHeroKillIdAndLevel(iHeroId,iLv+1);
    
    int  iHurtNum2  = 0;
    
    if(iHeroId == 3)
    {
        iHurtNum2 = fHurtNum2*m_pSoldierVec[m_index]->attk;
    }
    else
    {
        iHurtNum2 = fHurtNum2;
    }
    
    char strHurtNum1[50] = {0};
    sprintf(strHurtNum1,"%d",iHurtNum);
    
    char strHurtNum2[50] = {0};
    sprintf(strHurtNum2,"%d",iHurtNum2);
    
    heroSkillUpgradeInfo* pHeroSkillUpgradeInfo = CDBMgr::getInstance()->GetHeroSkillWithId(iHeroSkillId,iLv);
    
    CDBMgr::getInstance()->AnalyseSkillUpgradeNeedItem(pHeroSkillUpgradeInfo->m_strItems);
    
    m_iNeedTime = pHeroSkillUpgradeInfo->m_iNeedTime;
    
    strSkillDesc = pHeroSkillInfo[m_iSkillIdIndex]->heroSkillDes;
    
    int iPosHurt = strSkillDesc.find("%d");
    
    Label *pHurtLabel = NULL;
    if(iPosHurt!= -1)  //有伤害
    {
        Label *pHurtLabel = Label::createWithTTF("伤害:","fonts/cuti.ttf",26);
        pHurtLabel->setColor(Color3B::GREEN);
        pHurtLabel->setAnchorPoint(Vec2(0, 0.5f));
        pHurtLabel->setPosition(tBg.width*0.51,tBg.height*0.53);
        m_pBg->addChild(pHurtLabel,111);
        
        Label *pHurt1 = Label::createWithTTF(strHurtNum1,"fonts/cuti.ttf",26);
        pHurt1->setColor(Color3B::GREEN);
        pHurt1->setAnchorPoint(Vec2(0,0.5));
        pHurt1->setPosition(pHurtLabel->getPositionX() + pHurtLabel->getBoundingBox().size.width + 15,pHurtLabel->getPositionY());
        m_pBg->addChild(pHurt1,111);
        
        Sprite *pArrow = Sprite::createWithSpriteFrameName("upgrade_arrow.png");
        pArrow->setAnchorPoint(Vec2(0,0.5));
        pArrow->setPosition(Vec2(pHurt1->getPositionX() + pHurt1->getBoundingBox().size.width + 25,pHurtLabel->getPositionY()));
        m_pBg->addChild(pArrow,111);
        
        Label *pHurt2 = Label::createWithTTF(strHurtNum2,"fonts/cuti.ttf",26);
        pHurt2->setColor(Color3B::GREEN);
        pHurt2->setAnchorPoint(Vec2(0,0.5));
        pHurt2->setPosition(pArrow->getPositionX() + pArrow->getBoundingBox().size.width + 25,pHurtLabel->getPositionY());
        m_pBg->addChild(pHurt2,111);
    }
    
    int iPosTime = strSkillDesc.find("%s");
    fTime =  CDataManager::getTheLastTimeWithIdAndLevel(iHeroId,iLv+1);
    
    char strTime2[50] = {0};
    sprintf(strTime2,"%.2f",fTime);
    
    if(iPosTime != -1)
    {
        Label *pDuringLabel = Label::createWithTTF("持续时间:","fonts/cuti.ttf",26);
        pDuringLabel->setColor(Color3B::GREEN);
        pDuringLabel->setAnchorPoint(Vec2(0,0.5));
        m_pBg->addChild(pDuringLabel,111);
        pDuringLabel->setPosition(tBg.width*0.51,tBg.height*0.47);

        Label *pDuringTime1 = Label::createWithTTF(strTime,"fonts/cuti.ttf",26);
        pDuringTime1->setColor(Color3B::GREEN);
        pDuringTime1->setAnchorPoint(Vec2(0,0.5));
        pDuringTime1->setPosition(pDuringLabel->getPositionX()+pDuringLabel->getContentSize().width + 15,pDuringLabel->getPositionY());
        m_pBg->addChild(pDuringTime1,111);
        
        Sprite *pArrow = Sprite::createWithSpriteFrameName("upgrade_arrow.png");
        pArrow->setAnchorPoint(Vec2(0,0.5));
        pArrow->setPosition(Vec2(pDuringTime1->getPositionX()+pDuringTime1->getContentSize().width + 25,pDuringLabel->getPositionY()));
        m_pBg->addChild(pArrow,111);
        
        Label *pDuringTime2 = Label::createWithTTF(strTime2,"fonts/cuti.ttf",26);
        pDuringTime2->setAnchorPoint(Vec2(0,0.5));
        pDuringTime2->setColor(Color3B::GREEN);
        pDuringTime2->setPosition(Vec2(pArrow->getPositionX()+pArrow->getContentSize().width + 25,pDuringLabel->getPositionY()));
        m_pBg->addChild(pDuringTime2,111);
        
    }
    
    if(pHeroSkillUpgradeInfo)
    {
        sprintf(m_strNeedTime,"%d秒",pHeroSkillUpgradeInfo->m_iNeedTime);
    }
}

void CSkillUpLayer::soliderSkillInfo()
{
    SkillUpgradeInfo*  pSkillUpgradeInfo = NULL;
    std::string strSkillName;
    Size tBg = m_pBg->getContentSize();
    
     vector<CDBMgr::MonsterSkillTime_s> pInfo = CDBMgr::getInstance()->GetSkillUpgradeInfoVec();
    
    
    m_iLv = m_pSoldierVec[m_index]->theFighterSkillInfo[m_iSkillIdIndex].skillLevel;
    
    for(int i=0;i<pInfo.size();i++)
    {
        if(pInfo[i].m_iMonsterId ==  m_pSoldierVec[m_index]->monsterId &&pInfo[i].m_iSkillId == m_pSoldierVec[m_index]->theFighterSkillInfo[m_iSkillIdIndex].theSkillInfo->skillId )
        {
            m_iLv +=1;
        }
        break;
        
    }
    
    

    char strLv[100] = {0};
    {
        strSkillName = m_pSoldierVec[m_index]->theFighterSkillInfo[m_iSkillIdIndex].theSkillInfo->skillName;
        sprintf(strLv,"LV:%d",m_iLv);
        
        pSkillUpgradeInfo =  CDataManager::getSkillUpgradeInfowithId(m_pSoldierVec[m_index]->monsterType,m_iLv);
    }
    
    //技能描述
    int iMonsterid = m_pSoldierVec[m_index]->monsterId;
    int iRare = m_pSoldierVec[m_index]->rare;
    std::vector<skillInfo*> m_pskill = CDataManager::getAvailableSkillInfo(iMonsterid,iRare);
    
    int fAtkNum = 0;
    float fRatioPer = 0.0;
    
    monsterInfo* pMonsterInfo = CDataManager::getMonsterInfoByMonsterId(m_pSoldierVec[m_index]->monsterId);
    
    int iSkillId = pMonsterInfo->allSkill[m_iSkillIdIndex];
    int iAddHp = 0;
    int iAddAtk = 0;
    int iposd = 0;
    int iposf = 0;
    
    CDataManager::getTheAddAtrWith(pMonsterInfo,&iAddHp,&iAddAtk);
    
    for(int i=0;i<m_pskill.size();i++)
    {
        if(i == m_iSkillIdIndex)
        {
            Sprite *pSkillPic = CDataManager::getSkillIcoWithId(m_pskill[i]);
            
            pSkillPic->setScale(3);
            m_iSkillId = m_pskill[i]->skillId;
            m_pSkillInfo = m_pskill[i];
            
            pSkillPic->setPosition(tBg.width*0.1,tBg.height*0.77);
            m_pBg->addChild(pSkillPic);
            
            std::string strSkillName = m_pskill[i]->skillName;
            
            m_pSkillName = Label::createWithTTF(strSkillName,"fonts/cuti.ttf",30);
            m_pSkillName->setAnchorPoint(Vec2(0,0.5));
            m_pSkillName->setColor(Color3B::ORANGE);
            m_pSkillName->setPosition(tBg.width*0.2,tBg.height*0.815);
            m_pBg->addChild(m_pSkillName);
            
            //技能描述
            char strLvBefore[200] = {0};
            std::string strBeforeDesc = m_pskill[i]->skillDes+"。";
            int iAtk =  m_pSoldierVec[m_index]->attk;
            
            float fRatio = m_pSoldierVec[m_index]->theFighterSkillInfo[m_iSkillIdIndex].damageRatio;
        
            if(iSkillId == eAllSkills_reborn||iSkillId == eAllSkills_silinqiyue)
            {
                fRatio = m_pSoldierVec[m_index]->theFighterSkillInfo[m_iSkillIdIndex].startRatio;
            }
            
            
            //技能出现几率
            iposd = strBeforeDesc.find("%d");
            iposf  = strBeforeDesc.find("%.2f");
            
            int iAddSkillNum = (iAddAtk)*fRatio;
            
            if(m_iSkillId == eAllSkills_jushipifu)//巨石皮肤
            {
                strcpy(strLvBefore,m_pskill[i]->skillDes.c_str());
            }
            
            if(iposd>0)
            {
                //伤害为数值
                fAtkNum = iAtk*fRatio;
                std::string str = strBeforeDesc.substr(0,iposd+2);
                str += strBeforeDesc.substr(iposd+2,strBeforeDesc.length());
                sprintf(strLvBefore,str.c_str(),fAtkNum);
                sprintf(strLv,"%d",fAtkNum);
            }
            
            if(iposf>0)
            {
                //两个值
                fRatioPer = fRatio*100;
                std::string str = strBeforeDesc.substr(0,iposf+6);
                int iPosf2 = str.find("%.2f");
                
                if(iPosf2>0)
                {
                    str += strBeforeDesc.substr(iposf+6,strBeforeDesc.length());
                    sprintf(strLvBefore,str.c_str(),fRatioPer,fRatioPer);
                    sprintf(strLv,"%.2f%%",fRatioPer);
                }
                else
                {
                    str += strBeforeDesc.substr(iposf+6,strBeforeDesc.length());
                    sprintf(strLvBefore,str.c_str(),fRatioPer);
                    sprintf(strLv,"%.2f%%",fRatioPer);
                }
            }
            
            Label *pLabelDesc = Label::createWithTTF(strLvBefore,"fonts/cuti.ttf",25);
            pLabelDesc->setAnchorPoint(Vec2(0,1));
            pLabelDesc->setWidth(tBg.width * 0.74f);
            pLabelDesc->setPosition(tBg.width*0.2,tBg.height*0.77);
            m_pBg->addChild(pLabelDesc);
        }
    }
    
    //详细属性背景
    Sprite* propBg = Sprite::createWithSpriteFrameName("skillUpgrade_di.png");
    m_pBg->addChild(propBg,0,996);
    propBg->setScale(3.0f);
    propBg->setPosition(Vec2(tBg.width * 0.5f, tBg.height * 0.4f));
    
    
    float fJilv = m_pSoldierVec[m_index]->theFighterSkillInfo[m_iSkillIdIndex].startRatio;
    
    sprintf(strLv,"%.2f%%",fJilv*100);
    
    int iFontSize = 26;
    
    std::string str = "触发几率:";
    
    
    
    //当前技能正在升级
    
     pInfo = CDBMgr::getInstance()->GetSkillUpgradeInfoVec();
    
    

  
    int iSkillLv = m_pSoldierVec[m_index]->theFighterSkillInfo[m_iSkillIdIndex].skillLevel;
    
    for(int i=0;i<pInfo.size();i++)
    {
        if(pInfo[i].m_iMonsterId ==  m_pSoldierVec[m_index]->monsterId &&pInfo[i].m_iSkillId == m_pSoldierVec[m_index]->theFighterSkillInfo[m_iSkillIdIndex].theSkillInfo->skillId )
        {
            iSkillLv +=1;
        }
        break;
        
    }
    
    
    if(iSkillId !=eAllSkills_Qiannenjifa &&fJilv<1)
    {
        Label *pJilv = Label::createWithTTF(str,"fonts/cuti.ttf",iFontSize);
        pJilv->setColor(Color3B::GREEN);
        pJilv->setAnchorPoint(Vec2(0,0.5));
        pJilv->setPosition(tBg.width*0.51,tBg.height*0.53);
        m_pBg->addChild(pJilv,0,798);
        
        Label *pJilvLable = Label::createWithTTF(strLv,"fonts/cuti.ttf",iFontSize);
        pJilvLable->setColor(Color3B::GREEN);
        pJilvLable->setAnchorPoint(Vec2(0,0.5));
        pJilvLable->setPosition( pJilv->getPositionX() + pJilv->getBoundingBox().size.width + 15,tBg.height*0.53);
        m_pBg->addChild(pJilvLable,0,799);
        
        
        Sprite *pJiArrow = Sprite::createWithSpriteFrameName("upgrade_arrow.png");
        
        pJiArrow->setAnchorPoint(Vec2(0,0.5));
        pJiArrow->setPosition(Vec2(pJilvLable->getPositionX() + pJilvLable->getBoundingBox().size.width + 25,tBg.height*0.53));
        m_pBg->addChild(pJiArrow,0,797);
        
        float fJiLvNew =  CDataManager::getTheStartRatioWithIdAndLevel(iSkillId,iSkillLv+1);
        char strJilv[50] = {0};
        sprintf(strJilv,"%.2f%%",fJiLvNew*100);
        
        Label *pJiLvLabelAfter = Label::createWithTTF(strJilv,"fonts/cuti.ttf",iFontSize);
        pJiLvLabelAfter->setColor(Color3B::GREEN);
        pJiLvLabelAfter->setAnchorPoint(Vec2(0,0.5));
        pJiLvLabelAfter->setPosition(pJiArrow->getPositionX() + pJiArrow->getBoundingBox().size.width + 25,tBg.height*0.53);
        m_pBg->addChild(pJiLvLabelAfter,0,796);
    }
    std::string strHurt = "伤害:";
    
    if(iSkillId == eAllSkills_Qiannenjifa)
    {
        strHurt = "提升攻击:";
    }
    else if(iSkillId == eAllSkills_huichunshu||iSkillId == eAllSkills_wushuyanxi||
            iSkillId == eAllSkills_huifushu||iSkillId ==eAllSkills_ziranyuhe)
    {
        strHurt = "治疗效果:";
        
    }else if(iSkillId == eAllSkills_dundang)
    {
        strHurt = "格挡伤害";
    }
    else if(iSkillId == eAllSkills_dunfan)
    {
        strHurt = "反弹伤害";
    }
    else if(iSkillId == eAllSkills_shechenlong)
    {
        strHurt = "射程变长";
    }
    
    Label *pHurtLabel = Label::createWithTTF(strHurt,"fonts/cuti.ttf",iFontSize);
    pHurtLabel->setColor(Color3B::GREEN);
    pHurtLabel->setAnchorPoint(Vec2(0,0.5));
    pHurtLabel->setPosition(tBg.width*0.51,tBg.height*0.47);
    
    
    if(iSkillId== eAllSkills_Qiannenjifa)
    {
        pHurtLabel->setPosition(tBg.width*0.6,tBg.height*0.52);
    }
    else if(iSkillId == eAllSkills_jushipifu||iSkillId == eAllSkills_reborn
            ||iSkillId == eAllSkills_silinqiyue)
    {
        pHurtLabel->setVisible(false);
    }
    
    m_pBg->addChild(pHurtLabel,0,800);
    
    //当前值
    float fDamage = CDataManager::getTheDamageRatioWithIdAndLevel(iSkillId,iSkillLv);
    
    if(iposd>0)
    {
        int iNum = m_pSoldierVec[m_index]->attk*fDamage;
        sprintf(strLv,"%d",iNum);
    }
    if(iposf>0)
    {
        float fNum = 100*fDamage;
        sprintf(strLv,"%.2f%%",fNum);
    }

    {
        float fHeight = tBg.height*0.47;
        Label *pHurt1 = Label::createWithTTF(strLv  ,"fonts/cuti.ttf",iFontSize);
        pHurt1->setColor(Color3B::GREEN);
        pHurt1->setAnchorPoint(Vec2(0,0.5));
        pHurt1->setPosition(pHurtLabel->getPositionX()+pHurtLabel->getContentSize().width + 15,
                            fHeight);
        m_pBg->addChild(pHurt1,0,801);

        
        if(iSkillId== eAllSkills_jushipifu||iSkillId == eAllSkills_reborn||iSkillId == eAllSkills_silinqiyue)
        {
            pHurt1->setVisible(false);
        }
        else if(iSkillId ==  eAllSkills_Qiannenjifa)
        {
            fHeight  = tBg.height*0.52;
            pHurt1->setPosition(pHurtLabel->getPositionX()+pHurtLabel->getContentSize().width,fHeight );
        }
        
        Sprite *pHurtArrow = Sprite::createWithSpriteFrameName("upgrade_arrow.png");
        pHurtArrow->setAnchorPoint(Vec2(0,0.5));
        pHurtArrow->setPosition(Vec2(pHurt1->getPositionX()+pHurt1->getContentSize().width + 25,fHeight));
        m_pBg->addChild(pHurtArrow,0,802);
        
        if(iSkillId== eAllSkills_reborn||iSkillId== eAllSkills_jushipifu||iSkillId == eAllSkills_silinqiyue)
        {
            pHurtArrow->setVisible(false);
        }
        
        else if(iSkillId == eAllSkills_Qiannenjifa)
        {
            pHurtArrow->setPosition(pHurt1->getPositionX()+pHurt1->getContentSize().width*1.3,fHeight);
        }
        
        myMonster * pMyMonster = CDataManager::getuserMonsterById(iMonsterid);
        
        FighterInfo* pFighterInfo = CPersonalData::getInstance()->refreshSoldierInfoWithInfo(pMyMonster);
        
        fDamage = CDataManager::getTheDamageRatioWithIdAndLevel(iSkillId,iSkillLv+1);
        
        if(iposd>0)
        {
            int iNum = m_pSoldierVec[m_index]->attk*fDamage;
            sprintf(strLv,"%d",iNum);
        }
        
        if(iposf>0)
        {
            float fNum = 100*fDamage;
            sprintf(strLv,"%.2f%%",fNum);
        }
        
        Label *pHurt2 = Label::createWithTTF(strLv,"fonts/cuti.ttf",iFontSize);
        pHurt2->setColor(Color3B::GREEN);
        pHurt2->setAnchorPoint(Vec2(0,0.5));
        pHurt2->setPosition(Vec2(pHurtArrow->getPositionX()+pHurtArrow->getContentSize().width + 25,fHeight));
        m_pBg->addChild(pHurt2,0,803);
        
        if(iSkillId== eAllSkills_jushipifu||iSkillId == eAllSkills_reborn||iSkillId == eAllSkills_silinqiyue) //巨石皮肤
        {
            pHurt2->setVisible(false);
        }
        else if(iSkillId ==  eAllSkills_Qiannenjifa)
        {
            pHurt2->setPosition(pHurtArrow->getPositionX()+pHurtArrow->getContentSize().width*1.2,fHeight);
        }
        
        if(iSkillId== eAllSkills_Qiannenjifa)
        {
            Label *pUpHpText = Label::createWithTTF("提升生命:","fonts/cuti.ttf",iFontSize);
            pUpHpText->setColor(Color3B::GREEN);
            pUpHpText->setPosition(tBg.width*0.6,tBg.height*0.47);
            pUpHpText->setAnchorPoint(Vec2(0,0.5));
            m_pBg->addChild(pUpHpText);
            
            char str[10] = {0};
            float fDamage = CDataManager::getTheDamageRatioWithIdAndLevel(iSkillId,iSkillLv);

            float fNum = 100*fDamage;
            sprintf(str,"%.2f%%",fNum);
    
            Label *pHp1 = Label::createWithTTF(str,"fonts/cuti.ttf",iFontSize);
            pHp1->setColor(Color3B::GREEN);
            pHp1->setAnchorPoint(Vec2(0,0.5));
            pHp1->setPosition(Vec2(pUpHpText->getPositionX()+pUpHpText->getContentSize().width+20,tBg.height*0.47));
            m_pBg->addChild(pHp1);
            
            Sprite *pHurtArrow = Sprite::createWithSpriteFrameName("upgrade_arrow.png");
            pHurtArrow->setAnchorPoint(Vec2(0,0.5));
            pHurtArrow->setPosition(Vec2(pHurt1->getPositionX()+pHurt1->getContentSize().width + 25,tBg.height*0.47));
            m_pBg->addChild(pHurtArrow);
            
            fDamage = CDataManager::getTheDamageRatioWithIdAndLevel(iSkillId,iSkillLv+1);
            fNum = 100*fDamage;
            sprintf(str,"%.2f%%",fNum);
            
            Label *pHp2 = Label::createWithTTF(str,"fonts/cuti.ttf",iFontSize);
            pHp2->setColor(Color3B::GREEN);
            pHp2->setAnchorPoint(Vec2(0,0.5));
            pHp2->setPosition(Vec2(pHurtArrow->getPositionX()+pHurtArrow->getContentSize().width+20,tBg.height*0.47));
            m_pBg->addChild(pHp2);
        }
        
        int iSkillMaxLv = 0;
        
        if(m_bHero)
        {
            iSkillMaxLv = m_pHeroSkillInfo->levelMax;
        }
        
        iSkillMaxLv = m_pSoldierVec[m_index]->theFighterSkillInfo[m_iSkillIdIndex].theSkillInfo->maxLevel;
        
        if(iSkillMaxLv<=m_iLv)
        {
            pHurt1->setVisible(false);
            pHurtLabel->setVisible(false);
            pHurt2->setVisible(false);
            pHurtArrow->setVisible(false);
        }
        
        
        
    }
    
    
    
    //所需道具
    CDBMgr::getInstance()->AnalyseSkillUpgradeNeedItem(pSkillUpgradeInfo->m_strItems);
    
    m_iNeedTime = pSkillUpgradeInfo->m_iNeedTime;
    
    
    int iMin = m_iNeedTime%3600/60;
    int iHour = m_iNeedTime/3600;
    int iSec = m_iNeedTime%60;
    
    
    if(pSkillUpgradeInfo)
    {
        if(iMin >= 0 &iHour >0 & iSec >=0)
        {
            sprintf(m_strNeedTime,"%d时%d分%d秒",iHour,iMin,iSec);
        }
        else if(iMin > 0 && iSec >= 0)
        {
            sprintf(m_strNeedTime,"%d分%d秒",iMin,iSec);
        }
        else if(iSec>0)
        {
            sprintf(m_strNeedTime,"%d秒",iSec);
        }
    }
}

void CSkillUpLayer::skillPropInfo()
{
    if(m_node)
    {
        m_node->removeFromParent();
        m_node = NULL;
    }
    m_node = Node::create();
    m_pBg->addChild(m_node);
    m_node->setPosition(Vec2::ZERO);
    vector<heroSkill*> pHeroSkillInfo = CDBMgr::getInstance()->getHeroSkillByMonsterId(m_pSoldierVec[m_index]->monsterId);
    Size tBg = m_pBg->getContentSize();
    map<int,int> m_pNeedPropmap = CDBMgr::getInstance()->GetSkillUpgradeItemMap();
    
    map<int,int>::iterator it = m_pNeedPropmap.begin();
    
    int iSkillMaxLv = 0;
    
    if(m_bHero)
    {
        iSkillMaxLv = CDBMgr::getInstance()->GetHeroSkillMaxLvById(pHeroSkillInfo[m_iSkillIdIndex]->m_iMonsterId);
    }
    else
    {
        iSkillMaxLv = m_pSoldierVec[m_index]->theFighterSkillInfo[m_iSkillIdIndex].theSkillInfo->maxLevel;
    }
    
    if(iSkillMaxLv>m_iLv)
    {
        for(int i=0;it!=m_pNeedPropmap.end();i++,it++)
        {
            Sprite *pNeedPro1 =  CDataManager::getPropSpriteById(it->first,true);
            pNeedPro1->setScale(1.6);
            pNeedPro1->setPosition(tBg.width*0.18+tBg.width*0.2*i,tBg.height*0.49);
            m_node->addChild(pNeedPro1);
            
            Sprite* normal = Sprite::createWithSpriteFrameName("orangeBtn.png");
            Sprite* select = Sprite::createWithSpriteFrameName("orangeBtn.png");
            select->setColor(Color3B::GRAY);
            LYMenuItemSprite* getMaterial = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(CSkillUpLayer::btnCallBack, this));
            Menu* pMenu = Menu::create(getMaterial, NULL);
            getMaterial->setUserData((void*)(it->first));
            getMaterial->setScale(2.0f);
            m_node->addChild(pMenu);
            pMenu->setPosition(Vec2(pNeedPro1->getPositionX(), tBg.height * 0.285f));
            
            Label* pLabel = Label::createWithTTF("获取", "fonts/cuti.ttf", 28);
            getMaterial->addChild(pLabel);
            pLabel->setScale(0.5f);
            pLabel->setPosition(Vec2(normal->getContentSize().width * 0.5f, normal->getContentSize().height * 0.5f));
            
            char buf[100] = {0};
            int iHaveNum = CDBMgr::getInstance()->HavePropNumById(it->first);
            pMenu->setVisible(true);
            
            if(i==0)
            {
                sprintf(buf,"%d / %d",iHaveNum, it->second);
                m_pLabel1Have = Label::createWithTTF(buf,"fonts/cuti.ttf",30);
                m_pLabel1Have->setPosition(tBg.width*0.18+tBg.width*0.2*i,tBg.height*0.385);
                m_pLabel1Have->setColor(Color3B(240,158,2));
                m_node->addChild(m_pLabel1Have);
                m_iProp1 = it->first;
                
                m_iNeedNum1 = it->second;
                m_iHaveNum1 = iHaveNum;
                
                if(m_iNeedNum1<=m_iHaveNum1)
                {
                    m_pLabel1Have->setColor(Color3B(93,213,33));
                    //pMenu->setEnabled(false);
                    pMenu->setVisible(false);
                    //pMenu->setColor(Color3B::GRAY);
                }
            }
            else if(i==1)
            {
                sprintf(buf,"%d / %d",iHaveNum, it->second);
                m_pLabel2Have = Label::createWithTTF(buf,"fonts/cuti.ttf",30);
                m_pLabel2Have->setPosition(tBg.width*0.18+tBg.width*0.2*i,tBg.height*0.385);
                m_pLabel2Have->setColor(Color3B(240,158,2));
                m_node->addChild(m_pLabel2Have);
                m_iProp2 = it->first;
                
                m_iNeedNum2 = it->second;
                m_iHaveNum2 = iHaveNum;
                
                if(m_iNeedNum2<=m_iHaveNum2)
                {
                    m_pLabel2Have->setColor(Color3B(93,213,33));
                    //pMenu->setEnabled(false);
                    pMenu->setVisible(false);
                    //pMenu->setColor(Color3B::GRAY);
                }
            }
            else if(i==2)
            {
                sprintf(buf,"%d / %d",iHaveNum, it->second);
                m_iProp3 = it->first;
                m_pLabel3Have = Label::createWithTTF(buf,"fonts/cuti.ttf",30);
                m_pLabel3Have->setPosition(tBg.width*0.18+tBg.width*0.2*i,tBg.height*0.385);
                m_pLabel3Have->setColor(Color3B(240,158,2));
                m_node->addChild(m_pLabel3Have);
                
                m_iNeedNum3 = it->second;
                m_iHaveNum3 = iHaveNum;
                
                if(m_iNeedNum3<=m_iHaveNum3)
                {
                    m_pLabel3Have->setColor(Color3B(93,213,33));
                    pMenu->setVisible(false);

                }
            }
        }
    }
    
    char strLv[100] = {0};
    sprintf(strLv,"Lv:%d",m_iLv);
    
    Label *pSkillToLv = Label::createWithTTF(strLv,"fonts/cuti.ttf",30);
    pSkillToLv->setAnchorPoint(Vec2(0,0.5));
    pSkillToLv->setPosition(m_pSkillName->getPositionX() + m_pSkillName->getBoundingBox().size.width + 30,m_pSkillName->getPositionY());
    pSkillToLv->setColor(Color3B::YELLOW);
    m_node->addChild(pSkillToLv);
    
    Sprite *pLvArrow = Sprite::createWithSpriteFrameName("upArrow.png");
    pLvArrow->setRotation(90);
    pLvArrow->setPosition(pSkillToLv->getPositionX()+pSkillToLv->getContentSize().width*1.5,pSkillToLv->getPositionY());
    m_node->addChild(pLvArrow);
    sprintf(strLv,"LV:%d",m_iLv+1);
    
    Label *pSkill2ToLv = Label::createWithTTF(strLv,"fonts/cuti.ttf",30);
    pSkill2ToLv->setAnchorPoint(Vec2(0,0.5));
    pSkill2ToLv->setPosition(pLvArrow->getPositionX()+pLvArrow->getContentSize().width,pLvArrow->getPositionY());
    pSkill2ToLv->setColor(Color3B::YELLOW);
    m_node->addChild(pSkill2ToLv);
    
    Label *pNeedProp = Label::createWithTTF("所需道具:","fonts/cuti.ttf",30);
    pNeedProp->setPosition(tBg.width*0.12,tBg.height*0.615);
    m_node->addChild(pNeedProp);
    
    //需要时间
    Label *pNeedTimeLable = Label::createWithTTF("所需时间:","fonts/cuti.ttf",28);
    pNeedTimeLable->setAnchorPoint(Vec2(0, 0.5f));
    pNeedTimeLable->setPosition(tBg.width*0.51,tBg.height*0.4);
    m_node->addChild(pNeedTimeLable);
    
    Label *pTimeLable = Label::createWithTTF(m_strNeedTime,"fonts/cuti.ttf",28);
    pTimeLable->setAnchorPoint(Vec2(0, 0.5f));
    pTimeLable->setPosition(pNeedTimeLable->getPositionX()+pNeedTimeLable->getContentSize().width + 15,pNeedTimeLable->getPositionY());
    pTimeLable->setColor(Color3B::YELLOW);
    m_node->addChild(pTimeLable);
    
    //升级按钮呢
    Sprite *pLvUp  = Sprite::createWithSpriteFrameName("skill_greenBtn.png");
    
    Sprite *pLvUpSelect = Sprite::createWithSpriteFrameName("skill_greenBtn.png");
    
    pLvUpSelect->setColor(Color3B::GRAY);
    
    LYMenuItemSprite *pLvUpItem = LYMenuItemSprite::create(pLvUp,pLvUpSelect,CC_CALLBACK_1(CSkillUpLayer::OnSkillUpMenu,this));
    pLvUpItem->setScale(2.45f);
    
    pLvUpItem->setAnchorPoint(Vec2(0.5, 0.5f));
    m_pMenuLvUp = Menu::create(pLvUpItem,NULL);
    m_pMenuLvUp->setPosition(tBg.width*0.58,tBg.height*0.295);
    m_node->addChild(m_pMenuLvUp);
    
    Label *pLvUpLable = Label::createWithTTF("升级", "fonts/cuti.ttf", 30);
    pLvUpLable->setScale(0.4);
    pLvUpLable->setPosition(pLvUp->getContentSize().width*0.5,pLvUp->getContentSize().height*0.5);
    pLvUpItem->addChild(pLvUpLable);
    
    
    if(iSkillMaxLv<=m_iLv)
    {
        Sprite* p = (Sprite *)m_pBg->getChildByTag(996);
        
        if(p)
        {
            p->setVisible(false);
        }
        
        pSkill2ToLv->setVisible(false);
        pLvArrow->setVisible(false);
        pLvUpItem->setVisible(false);
        m_pMenuLvUp->setEnabled(false);
        pNeedProp->setVisible(false);
        pNeedTimeLable->setVisible(false);
        pTimeLable->setVisible(false);
    }
    
    
    //显示需要购买的材料数量以及魔法石
    if((m_iHaveNum1<m_iNeedNum1) || (m_iHaveNum2<m_iNeedNum2) || (m_iHaveNum3<m_iNeedNum3))
    {
        showBuyItem();
    }

}

void CSkillUpLayer::showBuyItem()
{
    int needBuyTypeNum = 0;//需要购买的类型数量
    Size tBg = m_pBg->getContentSize();
    Sprite* pBuyBg = Sprite::createWithSpriteFrameName("skillUpgrade_di.png");
    
    
    m_node->addChild(pBuyBg,0,996);
    pBuyBg->setScaleX(2.2f);
    pBuyBg->setScaleY(1.2f);
    pBuyBg->setPosition(Vec2(tBg.width * 0.5f, tBg.height * 0.13f));
    
    Sprite* normal = Sprite::createWithSpriteFrameName("skill_buy_btn.png");
    Sprite* select = Sprite::createWithSpriteFrameName("skill_buy_btn.png");
    select->setColor(Color3B::GRAY);
    LYMenuItemSprite* buyMenuItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(CSkillUpLayer::buyMaterialCallBack, this));
    buyMenuItem->setTag(10);
    Menu* _buyMenu = Menu::create(buyMenuItem, NULL);
    m_node->addChild(_buyMenu);
    _buyMenu->setPosition(Vec2(tBg.width * 0.735f, tBg.height * 0.125f));
    
    char buf[256];
    Label* propNum_1 = NULL;
    Label* propNum_2 = NULL;
    Label* propNum_3 = NULL;
    if(m_iHaveNum1<m_iNeedNum1)
    {
        needBuyTypeNum++;
        propInfo* info = CDataManager::getPropInfoById(m_iProp1);
        int price = CDBMgr::getInstance()->GeShopPropPrize(m_iProp1);
        if(info)
        {
            m_needStoneNum += price*(m_iNeedNum1 - m_iHaveNum1);;
        }
        Sprite* prop_1 = CDataManager::getPropSpriteById(m_iProp1,true);
        m_node->addChild(prop_1);
        prop_1->setPosition(Vec2(tBg.width * 0.23f, tBg.height * 0.13f));
        prop_1->setScale(1.4f);
        
        sprintf(buf, "X%d", m_iNeedNum1 - m_iHaveNum1);
        propNum_1 = Label::createWithSystemFont(buf, "Arial", 26);
        m_node->addChild(propNum_1);
        propNum_1->setAnchorPoint(Vec2(0, 0.5f));
        propNum_1->setPosition(Vec2(prop_1->getPositionX() + prop_1->getBoundingBox().size.width * 0.55, prop_1->getPositionY()));
    }
    if(m_iHaveNum2<m_iNeedNum2)
    {
        if(needBuyTypeNum == 1)
        {
            Label* addMark = Label::createWithTTF("+", "fonts/cuti.ttf", 26);
            m_node->addChild(addMark);
            addMark->setPosition(Vec2(propNum_1->getPositionX() + propNum_1->getBoundingBox().size.width + 18, propNum_1->getPositionY()));
        }
        propInfo* info = CDataManager::getPropInfoById(m_iProp2);
        int price = CDBMgr::getInstance()->GeShopPropPrize(m_iProp2);
        if(info)
        {
            m_needStoneNum += price*(m_iNeedNum2 - m_iHaveNum2);;
        }
        Sprite* prop_2 = CDataManager::getPropSpriteById(m_iProp2,true);
        m_node->addChild(prop_2);
        prop_2->setPosition(Vec2(tBg.width * 0.23f + needBuyTypeNum * 155, tBg.height * 0.13f));
        prop_2->setScale(1.4f);
        
        sprintf(buf, "X%d", m_iNeedNum2 - m_iHaveNum2);
        propNum_2 = Label::createWithSystemFont(buf, "Arial", 26);
        m_node->addChild(propNum_2);
        propNum_2->setAnchorPoint(Vec2(0, 0.5f));
        propNum_2->setPosition(Vec2(prop_2->getPositionX() + prop_2->getBoundingBox().size.width * 0.55, prop_2->getPositionY()));
        needBuyTypeNum++;
    }
    if(m_iHaveNum3<m_iNeedNum3)
    {
        if(needBuyTypeNum == 2)
        {
            Label* addMark = Label::createWithTTF("+", "fonts/cuti.ttf", 26);
            m_node->addChild(addMark);
            addMark->setPosition(Vec2(propNum_2->getPositionX() + propNum_2->getBoundingBox().size.width + 18, propNum_2->getPositionY()));
        }
        propInfo* info = CDataManager::getPropInfoById(m_iProp3);
        int price = CDBMgr::getInstance()->GeShopPropPrize(m_iProp3);
        if(info)
        {
            m_needStoneNum += price*(m_iNeedNum3 - m_iHaveNum3);
        }
        else if(needBuyTypeNum == 1)
        {
            Label* addMark = Label::createWithTTF("+", "fonts/cuti.ttf", 26);
            m_node->addChild(addMark);
            if(propNum_2)
            {
                addMark->setPosition(Vec2(propNum_2->getPositionX() + propNum_2->getBoundingBox().size.width + 18, propNum_2->getPositionY()));
            }
            else{
                addMark->setPosition(Vec2(propNum_1->getPositionX() + propNum_1->getBoundingBox().size.width + 18, propNum_1->getPositionY()));
            }
            
        }
        Sprite* prop_3 = CDataManager::getPropSpriteById(m_iProp3,true);
        m_node->addChild(prop_3);
        prop_3->setPosition(Vec2(tBg.width * 0.23f + needBuyTypeNum * 155, tBg.height * 0.13f));
        prop_3->setScale(1.4f);
        
        sprintf(buf, "X%d", m_iNeedNum3 - m_iHaveNum3);
        propNum_3 = Label::createWithTTF(buf, "fonts/cuti.ttf", 26);
        m_node->addChild(propNum_3);
        propNum_3->setAnchorPoint(Vec2(0, 0.5f));
        propNum_3->setPosition(Vec2(prop_3->getPositionX() + prop_3->getBoundingBox().size.width * 0.6, prop_3->getPositionY()));
        needBuyTypeNum++;
    }
    sprintf(buf, "=  %d", m_needStoneNum);
    Label* price = Label::createWithTTF(buf, "fonts/cuti.ttf", 26);
    m_node->addChild(price);
    price->setAnchorPoint(Vec2(0, 0.5f));
    if(propNum_3)
    {
        price->setPosition(Vec2(propNum_3->getPositionX() + propNum_3->getBoundingBox().size.width + 15, propNum_3->getPositionY()));
    }
    else if(propNum_2)
    {
        price->setPosition(Vec2(propNum_2->getPositionX() + propNum_2->getBoundingBox().size.width + 15, propNum_2->getPositionY()));
    }
    else if(propNum_1)
    {
        price->setPosition(Vec2(propNum_1->getPositionX() + propNum_1->getBoundingBox().size.width + 15, propNum_1->getPositionY()));
    }
    
    Sprite* stone = Sprite::createWithSpriteFrameName("stone.png");
    m_node->addChild(stone);
    stone->setScale(0.65f);
    stone->setPosition(Vec2(price->getPositionX() + price->getBoundingBox().size.width + stone->getBoundingBox().size.width * 0.62f ,
                            tBg.height * 0.125));
}

void CSkillUpLayer::buyMaterialCallBack(Ref* pSender)
{
    char buf[256];
    sprintf(buf, "是否花费%d个魔法石购买?", m_needStoneNum);
    costStoneCommonLayer* layer = costStoneCommonLayer::create();
    layer->initWndInfo(this, menu_selector(CSkillUpLayer::confirmBuyBack), buf, m_needStoneNum);
    addChild(layer);
}

void CSkillUpLayer::gotoShop(Ref* pSender)
{
    CLayerManager::GetSingletonPtr()->createLayer(eLayer_Charge,this,false);
}

void CSkillUpLayer::confirmBuyBack(Ref* pSender)
{
    __String* tempStr = __String::createWithFormat("%d", m_needStoneNum);
    EventParamMap paramMap;
    
    paramMap.insert(EventParamPair("购买材料花费魔法石",tempStr->getCString()));
    
    TDCCTalkingDataGA::onEvent("购买材料", &paramMap);
    if(CPersonalData::getInstance()->getUserStone() >= m_needStoneNum)
    {
        CNetWorkDataSingle::GetSingletonPtr()->SaveUserMCF(this,menu_selector(CSkillUpLayer::buyAfterCallBack),0,-m_needStoneNum,0);
        TDCCItem::onPurchase("购买材料", 1, m_needStoneNum);
    }
    else{
        CHitDialog *pDlg = CHitDialog::create();
        pDlg->createYesNO(this, menu_selector(CSkillUpLayer::gotoShop),"魔法石不足，是否去充值？");
        addChild(pDlg);
    }
}

void CSkillUpLayer::buyAfterCallBack(Ref *pSender)
{
    Sprite * thetarget = CFightManager::getTheFlyWord("购买成功","fonts/cuti.ttf");
    thetarget->setPosition(Vec2(m_winSize.width / 2, m_winSize.height / 2));
    addChild(thetarget, 10);
    CPersonalData::getInstance()->setUpdateUserInfo(true);
    //保存购买的道具
    if(m_iHaveNum1<m_iNeedNum1)
    {
        int iNum = CDBMgr::getInstance()->HavePropNumById(m_iProp1);
        iNum += (m_iNeedNum1 - m_iHaveNum1);
        CDataManager::SaveUserPropToDB(m_iProp1, iNum);//保存道具
    }
    if(m_iHaveNum2<m_iNeedNum2)
    {
        int iNum = CDBMgr::getInstance()->HavePropNumById(m_iProp2);
        iNum += (m_iNeedNum2 - m_iHaveNum2);
        CDataManager::SaveUserPropToDB(m_iProp2, iNum);//保存道具
    }
    if(m_iHaveNum3<m_iNeedNum3)
    {
        int iNum = CDBMgr::getInstance()->HavePropNumById(m_iProp3);
        iNum += (m_iNeedNum3 - m_iHaveNum3);
        CDataManager::SaveUserPropToDB(m_iProp3, iNum);//保存道具
    }
    //刷新界面
    skillPropInfo();
}

void CSkillUpLayer::btnCallBack(Ref* pSender)
{
    LYMenuItemSprite *pSprite = (LYMenuItemSprite*)pSender;
    int iMonsterId = m_pSoldierVec[m_index]->monsterId;
    long iPropId = (long)(pSprite->getUserData());
    CPersonalData::getInstance()->setJumpFromSkill(true);
    CPersonalData::getInstance()->m_jumpInfo.monsterId = iMonsterId;
    CPersonalData::getInstance()->m_jumpInfo.skillId = m_iSkillId;
    if(m_bHero)
    {
        CPersonalData::getInstance()->m_jumpInfo.type = 1;
    }
    else{
        CPersonalData::getInstance()->m_jumpInfo.type = 0;
    }
    propGetPathLayer* layer = propGetPathLayer::create(iPropId);
    addChild(layer);
}

void CSkillUpLayer::onExit()
{
    LvUpDetail *pLayer = (LvUpDetail*)getParent();
    
    if(pLayer)
    {
         if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SKILL_UPGRADE) && (guiderManagerLayer::getInstance()->getStepNum() == 8))
         {
             pLayer->m_pSprite->setVisible(false);
         }
         else{
             pLayer->m_pSprite->setVisible(true);
         }
        
         pLayer->m_pSkillUpLayer = NULL;
    }
    
    _eventDispatcher->removeEventListener(m_pListener);
    CBaseLayer::onExit();
}

void CSkillUpLayer::OnCloseMenu(Ref *pSender)
{
   
    removeFromParent();
}