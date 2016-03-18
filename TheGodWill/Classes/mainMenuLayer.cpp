//
//  mainMenuLayer.cpp
//  TheGodWill
//
//  Created by user on 14/12/22.
//
//

#include "mainMenuLayer.h"
#include "LYFightData.h"
#include "levelInfoLayer.h"
#include "DataManager.h"
#include "LYPersonalData.h"
#include "commonPropLayer.h"
#include "PropLayer.h"
#include "LvUpLayer.h"
#include "PhysicalSystem.h"
#include "sysTime.h"
#include "AllDefine.h"
#include "DBMgr.h"
#include "guiderManagerLayer.h"
#include "getNewHeroLayer.h"
#include "CSharedPlist.h"
#include "LYFightManager.h"
#include "shop.h"
#include "zzbNetControlModule.h"
#include "PublicDefine.h"
#include "NetWorkDataSingle.h"

#include "zzbNewNetStruct.h"
#include "BeName.h"
#include "Common.h"
#include "TDCCVirtualCurrency.h"

#define MY_PERSION CPersonalData::getInstance()

#define LEVEL_MAX 15//最大关卡数
#define MAP_POINT_MAX 101 //地图连线数
const int line[LEVEL_MAX-1] = {1,2,2,2,1,3};

CMainMenuLayer::CMainMenuLayer()
{
    m_pScrollView = NULL;
    m_pColor = NULL;
    m_pMenu = NULL;
    m_progress = NULL;
    m_pStaminaLabel = NULL;
    m_pStaminaMaxLabel = NULL;
    m_pCoin = NULL;
    m_pStone = NULL;
    m_pMenu = NULL;
    m_buffer = 0;
    m_bottomBtn.clear();
    m_midLevel = 1;
    m_map = NULL;
    m_userLevel = NULL;
    m_pTime = NULL;
    m_bufferIime = 0;
    m_lineNum = 0;
    m_isEqlite = false;
    m_bShowTeam = false;
    m_pListener = NULL;
    m_towerMenu = NULL;
    m_pCollegeItem = NULL;
    m_winSize = Director::getInstance()->getVisibleSize();
    m_midLevelInfoVec.clear();
    m_levelSum = 0;
    m_arrow = NULL;
    m_reLevel = 0;
    m_circle = NULL;
    m_cloudTime = 0;
    m_pTaskMenu = NULL;
    m_staminaBg = NULL;
    m_pRedPoint = NULL;
    m_pDrawCardRedPt = NULL;
    m_pDrawCardItem = NULL;
    
    m_pSuggesListener = NULL;
    SuggestionInputText = NULL;
    m_pSuggestionBG = NULL;
    m_pUserName = NULL;
    m_compoundTip=  NULL;
    
    m_sSuggestionText="";
    
    m_bSkillStart = true;
    
    m_bGetCard = false;
    
    m_fTimeAll = 0;
    m_iDrawCnt = 0;
    
    CSharedPlist::getInstance()->pushPlist("plist/mainMenu.plist");
    CSharedPlist::getInstance()->pushPlist("plist/map.plist");
    CSharedPlist::getInstance()->pushPlist("plist/suggestion.plist");
    
    CSharedPlist::getInstance()->pushPlist("plist/skillUp.plist");
    CSharedPlist::getInstance()->pushPlist("plist/common.plist");
    CSharedPlist::getInstance()->pushPlist("plist/common2.plist");
    CSharedPlist::getInstance()->pushPlist("plist/common3.plist");
    CSharedPlist::getInstance()->pushPlist("plist/login.plist");
    CPersonalData::getInstance()->setPvpMainLayer(false);
    
}

CMainMenuLayer::~CMainMenuLayer()
{
    unschedule(schedule_selector(CMainMenuLayer::updateTime));
    unscheduleUpdate();
    CSharedPlist::getInstance()->erasePlist("plist/mainMenu.plist");
    CSharedPlist::getInstance()->erasePlist("plist/map.plist");
    CSharedPlist::getInstance()->erasePlist("plist/skillUp.plist");
    CSharedPlist::getInstance()->erasePlist("plist/suggestion.plist");
}


bool CMainMenuLayer::IsSkillFinish()
{
    int iPreTime = 0;
    std::string strImg;
    
    vector<CDBMgr::MonsterSkillTime_s> pInfo = CDBMgr::getInstance()->GetSkillUpgradeInfoVec();
    std::vector<heroSkill*> pHeroSkillInfo = CDBMgr::getInstance()->GetHeroSkillInfo();
    vector<CDBMgr::MonsterSkillTime_s>::iterator it = pInfo.begin();
    
    std::string strImg1;
    std::string strImg2;
    std::string strImg3;
    
    std::string strMonsterSKill1;
    std::string strMonsterSKill2;
    std::string strMonsterSKill3;
    
    bool bSkill1 = false;
    bool bSkill2 = false;
    bool bSkill3 = false;
    
    int m_iSKillId1 = 0;
    int m_iSkillId2 = 0;
    int m_iSKillId3 = 0;
    
    
    int iNowTime = CGameTime::getNowTimeInSecond();
    
    if(pInfo.size()>=1)
    {
        iPreTime = pInfo[0].m_iTime;
        int iMonsterId  = it->m_iMonsterId;
        int iSkillId = it->m_iSkillId;
        bool bHero = false;
        
        if(iMonsterId>1000)
        {
            bHero = true;
        }
        
        char strMonsterSkillId[50] = {0};
        sprintf(strMonsterSkillId,"%d_%d",iMonsterId,iSkillId);
        
        int iTimeRemain = iNowTime -iPreTime;
        
        //获取技能升级需要时间
        
        SkillUpgradeInfo*  pInfoMonster = NULL;
        heroSkillUpgradeInfo* pHeroSkillUpgradeInfo = NULL;
        
        int iNeedTime = 0;
        
        if(bHero)
        {
            int iLv = CDBMgr::getInstance()->GetHeroSkillLvById(iSkillId);
            pHeroSkillUpgradeInfo = CDBMgr::getInstance()->GetHeroSkillWithId(iSkillId,iLv);
            iNeedTime = pHeroSkillUpgradeInfo->m_iNeedTime;
            strImg = pHeroSkillInfo[0]->skillIcon;
        }
        else
        {
            skillInfo* pInfo = CDataManager::getTheSkillInfoWithId(iSkillId);
            int iLv = CDBMgr::getInstance()->GetMonsterSKillLv(iMonsterId,iSkillId);
            monsterInfo* pMonsterInfo = CDataManager::getMonsterInfoByMonsterId(iMonsterId);
            pInfoMonster = CDataManager::getSkillUpgradeInfowithId(pMonsterInfo->monsterType,iLv);
            strImg = pInfo->skillIcon;
            iNeedTime = pInfoMonster->m_iNeedTime;
        }
        
        int iSec = iTimeRemain - iNeedTime;
        
        if(iSec>=0)
        {
            iSec = 0;
            log("升级完成加技能点");
            m_iSKillId1  = iSkillId;
            bSkill1 = true;
            strImg1 = strImg;
            char strMonsterSkill[50] = {0};
            sprintf(strMonsterSkill,"%d_%d",iMonsterId,iSkillId);
            strMonsterSKill1 =  strMonsterSkill;
        }
        it++;
        
    }
    if(pInfo.size()>=2)
    {
        iPreTime = pInfo[1].m_iTime;
        int id = it->m_iMonsterId;
        int iSkillId = it->m_iSkillId;
        int iMonsterId = id;
        bool bHero = false;
        
        if(iMonsterId>1000)
        {
            bHero = true;
        }
        
        char strMonsterSkillId[50] = {0};
        sprintf(strMonsterSkillId,"%d_%d",iMonsterId,iSkillId);
        
        int iTimeRemain = iNowTime -iPreTime;
        
        //获取技能升级需要时间
        
        SkillUpgradeInfo*  pInfo = NULL;
        heroSkillUpgradeInfo* pHeroSkillUpgradeInfo = NULL;
        
        int iNeedTime = 0;
        
        if(bHero)
        {
            int iLv = CDBMgr::getInstance()->GetHeroSkillLvById(iSkillId);
            
            pHeroSkillUpgradeInfo = CDBMgr::getInstance()->GetHeroSkillWithId(iSkillId,iLv);
            iNeedTime = pHeroSkillUpgradeInfo->m_iNeedTime;
            int iHeroSkillId = pHeroSkillUpgradeInfo->m_heroSkillId;
            strImg = pHeroSkillInfo[1]->skillIcon;
        }
        else
        {
            skillInfo* pInfo = CDataManager::getTheSkillInfoWithId(iSkillId);
            
            int iLv = CDBMgr::getInstance()->GetMonsterSKillLv(iMonsterId,iSkillId);
            
            monsterInfo* pMonsterInfo = CDataManager::getMonsterInfoByMonsterId(iMonsterId);
            SkillUpgradeInfo* pInfoMonster = CDataManager::getSkillUpgradeInfowithId(pMonsterInfo->monsterType,iLv);
            strImg = pInfo->skillIcon;
            iNeedTime = pInfoMonster->m_iNeedTime;
            
        }
        int iSec = iTimeRemain -iNeedTime;
        
        if(iSec>=0)
        {
            bSkill2 = true;
            iSec = 0;
            m_iSkillId2 = iSkillId;
            strImg2 = strImg;
            char strMonsterSkill[50] = {0};
            sprintf(strMonsterSkill,"%d_%d",iMonsterId,iSkillId);
            strMonsterSKill2 = strMonsterSkill;
        }
        it++;
    }
    
    if(pInfo.size()>=3)
    {
        iPreTime = pInfo[2].m_iTime;
        int id = it->m_iMonsterId;
        int iSkillId = it->m_iSkillId;
        int iMonsterId = id;
        bool bHero = false;
        
        if(iMonsterId>1000)
        {
            bHero = true;
        }
        
        char strMonsterSkillId[50] = {0};
        sprintf(strMonsterSkillId,"%d_%d",iMonsterId,iSkillId);
        
        int iTimeRemain = iNowTime -iPreTime;
        
        //获取技能升级需要时间
        heroSkillUpgradeInfo* pHeroSkillUpgradeInfo = NULL;
        
        int iNeedTime = 0;
        
        if(bHero)
        {
            int iLv = CDBMgr::getInstance()->GetHeroSkillLvById(iSkillId);
            
            pHeroSkillUpgradeInfo = CDBMgr::getInstance()->GetHeroSkillWithId(iSkillId,iLv);
            iNeedTime = pHeroSkillUpgradeInfo->m_iNeedTime;
            int iHeroSkillId = pHeroSkillUpgradeInfo->m_heroSkillId;
            strImg = pHeroSkillInfo[2]->skillIcon;
        }
        else
        {
            skillInfo* pInfo = CDataManager::getTheSkillInfoWithId(iSkillId);
            monsterInfo* pMonsterInfo = CDataManager::getMonsterInfoByMonsterId(iMonsterId);
            
            int iLv = CDBMgr::getInstance()->GetMonsterSKillLv(iMonsterId,iSkillId);
            SkillUpgradeInfo* pInfoMonster = CDataManager::getSkillUpgradeInfowithId(pMonsterInfo->monsterType,iLv);
            
            strImg = pInfo->skillIcon;
            iNeedTime = pInfoMonster->m_iNeedTime;
        }
        
        int iSec = iTimeRemain - iNeedTime;
        if(iSec>=0)
        {
            bSkill3 = true;
            m_iSKillId3 = iSkillId;
            iSec = 0;
            char strMonsterSkill[50] = {0};
            sprintf(strMonsterSkill,"%d_%d",iMonsterId,iSkillId);
            strImg3 = strImg;
            strMonsterSKill3 = strMonsterSkill;
        }
    }
    
    std::string str;
    
    if(bSkill1 || bSkill2 || bSkill3)
    {
        SimpleAudioEngine::getInstance()->playEffect("music/lvlSucess.mp3");
    }
    
    if(bSkill1&&bSkill2 &&bSkill3)
    {
        
        //缺少英雄
        
        addSKillFinishEffect(strMonsterSKill1,strMonsterSKill2,strMonsterSKill3);
        
        CDBMgr::getInstance()->AddSkillIdTostr(strMonsterSKill1);
        CDBMgr::getInstance()->DelSkillUpgradeToStr(strMonsterSKill1);
        
        CDBMgr::getInstance()->AddSkillIdTostr(strMonsterSKill2);
        
        CDBMgr::getInstance()->DelSkillUpgradeToStr(strMonsterSKill2);
        
        CDBMgr::getInstance()->AddSkillIdTostr(strMonsterSKill3);
        
        CDBMgr::getInstance()->DelSkillUpgradeToStr(strMonsterSKill3);
    }
    
    else if(bSkill1&&bSkill2&&!bSkill3)
    {
        addSKillFinishEffect(strMonsterSKill1,strMonsterSKill2,strMonsterSKill3);
        CDBMgr::getInstance()->AddSkillIdTostr(strMonsterSKill1);
        CDBMgr::getInstance()->DelSkillUpgradeToStr(strMonsterSKill1);
        CDBMgr::getInstance()->AddSkillIdTostr(strMonsterSKill2);
        CDBMgr::getInstance()->DelSkillUpgradeToStr(strMonsterSKill2);
    }
    else if(bSkill1&&!bSkill2&&bSkill3)
    {
        addSKillFinishEffect(strMonsterSKill1,strMonsterSKill2,strMonsterSKill3);
        CDBMgr::getInstance()->AddSkillIdTostr(strMonsterSKill1);
        CDBMgr::getInstance()->DelSkillUpgradeToStr(strMonsterSKill1);
        CDBMgr::getInstance()->AddSkillIdTostr(strMonsterSKill3);
        CDBMgr::getInstance()->DelSkillUpgradeToStr(strMonsterSKill3);
    }
    else if(!bSkill1&&bSkill2&&bSkill3)
    {
        addSKillFinishEffect(strMonsterSKill1,strMonsterSKill2,strMonsterSKill3);
        CDBMgr::getInstance()->AddSkillIdTostr(strMonsterSKill2);
        CDBMgr::getInstance()->DelSkillUpgradeToStr(strMonsterSKill2);
        CDBMgr::getInstance()->AddSkillIdTostr(strMonsterSKill3);
        CDBMgr::getInstance()->DelSkillUpgradeToStr(strMonsterSKill3);
    }
    else if(bSkill1&&!bSkill2&&!bSkill3)
    {
        addSKillFinishEffect(strMonsterSKill1,strMonsterSKill2,strMonsterSKill3);
        CDBMgr::getInstance()->AddSkillIdTostr(strMonsterSKill1);
        CDBMgr::getInstance()->DelSkillUpgradeToStr(strMonsterSKill1);
    }
    else if(!bSkill1&&bSkill2&&!bSkill3)
    {
        addSKillFinishEffect(strMonsterSKill1,strMonsterSKill2,strMonsterSKill3);
        CDBMgr::getInstance()->AddSkillIdTostr(strMonsterSKill2);
        CDBMgr::getInstance()->DelSkillUpgradeToStr(strMonsterSKill2);
    }
    else if(!bSkill1&&!bSkill2&&bSkill3)
    {
        //等级提升
        addSKillFinishEffect(strMonsterSKill1,strMonsterSKill2,strMonsterSKill3);
        CDBMgr::getInstance()->AddSkillIdTostr(strMonsterSKill3);
        CDBMgr::getInstance()->DelSkillUpgradeToStr(strMonsterSKill3);
    }
    
    return true;
}

void CMainMenuLayer::detectCompound()
{
    bool bCompound = false;
    std::vector<monsterInfo*> theModleInfo = CDataManager::getAllMonsterInfo();
    std::vector<int> allMonsterId;
    for(int i = 0; i < theModleInfo.size(); i++){
        if(theModleInfo.at(i)->isHaveDebris == 0)//代表显示碎片
        {
            allMonsterId.push_back(theModleInfo.at(i)->monsterId);
        }
    }
    for(int i = 0 ; i < allMonsterId.size(); i++)
    {
        int avaliableNum = CDataManager::getDebrisNumById(allMonsterId[i]);
        int rare = CDataManager::getMonsterStarById(allMonsterId[i]);
        if((rare < 5) && avaliableNum >= CDataManager::getMonsterEvolutionById(rare + 1)->m_number)
        {
            bCompound = true;
            break;
        }
    }
    if(bCompound)
    {
        if(m_compoundTip)
        {
            m_compoundTip->removeFromParent();
            m_compoundTip = NULL;
        }
        LYMenuItemSprite* temp = (LYMenuItemSprite*)p_Menu->getChildByTag(MAP_BTN_HERO);
        if(temp)
        {
            m_compoundTip = Sprite::createWithSpriteFrameName("RedPoint.png");
            temp->addChild(m_compoundTip);
            m_compoundTip->setPosition(Vec2(temp->getNormalImage()->getContentSize().width * 0.91f,
                                            temp->getNormalImage()->getContentSize().height * 0.6f));
        }
        
    }
    else
    {
        if(m_compoundTip)
        {
            m_compoundTip->removeFromParent();
            m_compoundTip = NULL;
        }
    }
}


void CMainMenuLayer::addSKillFinishEffect(std::string  strMonsterSKill1,std::string  strMonsterSKill2,std::string strMonsterSKill3)
{
    
    int id1 = 0;
    int iSkillId1 = 0;
    
    int id2 = 0;
    int iSkillId2 = 0;
    
    int id3 = 0;
    int iSkillId3 = 0;
    
    CDBMgr::getInstance()->FengeStr(strMonsterSKill1,id1,iSkillId1);
    
    
    Sprite *pHero = NULL;
    
    char strIcoPath[50] = {0};
    Sprite *pMid = Sprite::createWithSpriteFrameName("dilight.png");
    pMid->setPosition(Point(m_winSize.width/2,m_winSize.height*0.5));
    pMid->setScale(0);
    addChild(pMid,1304,902);
    
    
    pMid->runAction(Sequence::create(CCScaleTo::create(0.5f,4),NULL));
    
    Size tMid = pMid->getContentSize();
    
    if(iSkillId1 != 0)
    {
        
        skillInfo* pInfo1 = CDataManager::getTheSkillInfoWithId(iSkillId1);
        Label *pSkillName = NULL;
        
        if(id1>1000)
        {
            //为英雄图标
            vector<heroSkill*> pHeroSkillInfo = CDBMgr::getInstance()->getHeroSkillByMonsterId(id1);
            int   iSkillInex = CDBMgr::getInstance()->GetHeroSkillIndex(iSkillId1);
            sprintf(strIcoPath,"%s",pHeroSkillInfo[iSkillInex]->skillIcon.c_str());
            pHero = Sprite::createWithSpriteFrameName(strIcoPath);
            pSkillName =  Label::createWithSystemFont(pHeroSkillInfo[iSkillInex]->heroSkillName,"Aril",30);
        }
        else
        {
            sprintf(strIcoPath,"%s",pInfo1->skillIcon.c_str());
            pHero =  CDataManager::getSkillIcoWithId(strIcoPath,0.5);
            pSkillName =  Label::createWithSystemFont(pInfo1->skillName,"Aril",30);
        }
        
        pSkillName->setColor(Color3B::YELLOW);
        pSkillName->setPosition(tMid.width*0.5,tMid.height*0.05);
        pMid->addChild(pSkillName);
        pSkillName->cocos2d::Node::setScale(0.25);
        pHero->setPosition(tMid.width*0.5,tMid.height*0.5);
        pHero->setScale(0.7);
        pMid->addChild(pHero);
        
        LayerColor *pgrayBg = LayerColor::create();
        pgrayBg->setColor(Color3B::BLACK);
        pgrayBg->setOpacity(255*0.8);
        pgrayBg->setScale(6);
        addChild(pgrayBg,1300,900);
    
        
        pgrayBg->setPosition(Vec2(m_winSize.width / 2, m_winSize.height / 2));
      

        Sprite *pLvUpSucessLayer = Sprite::createWithSpriteFrameName("skillupsucess1.png");
        
        
        pLvUpSucessLayer->setScale(3);
        Animate* theAnimate = CFightManager::getAnimateWithPath("skillupsucess", 1, 12);
        
        Animate* theAnimate1 = CFightManager::getAnimateWithPath("skillupsucess", 5, 8);
        
        pLvUpSucessLayer->runAction(Sequence::create(theAnimate ,theAnimate1, NULL));
        pLvUpSucessLayer->setPosition(Point(m_winSize.width /2, m_winSize.height*0.8));
        //pLvUpSucessLayer->setScale(2);
        addChild(pLvUpSucessLayer,1302,901);
    }
    
    CDBMgr::getInstance()->FengeStr(strMonsterSKill2,id2,iSkillId2);
    
    if(iSkillId2 != 0)
    {
        Sprite *pHero = NULL;
        
        skillInfo* pInfo2 = CDataManager::getTheSkillInfoWithId(iSkillId2);
        char strIcoPath[50] = {0};
        
        Label *pSkillName = NULL;
        if(id2>1000)
        {
            //为英雄图标
            vector<heroSkill*> pHeroSkillInfo = CDBMgr::getInstance()->getHeroSkillByMonsterId(id2);
            int   iSkillInex = CDBMgr::getInstance()->GetHeroSkillIndex(iSkillId2);
            sprintf(strIcoPath,"%s",pHeroSkillInfo[iSkillInex]->skillIcon.c_str());
            pHero = Sprite::createWithSpriteFrameName(strIcoPath);
            pSkillName =  Label::createWithSystemFont(pHeroSkillInfo[iSkillInex]->heroSkillName,"Aril",30);
        }
        else
        {
            sprintf(strIcoPath,"%s",pInfo2->skillIcon.c_str());
            pHero =  CDataManager::getSkillIcoWithId(strIcoPath,0.5);
            pSkillName =  Label::createWithSystemFont(pInfo2->skillName,"Aril",30);
        }
        
        pHero->setPosition(tMid.width*0.3,tMid.height*0.5);
        pHero->setScale(0.7);
        pMid->addChild(pHero);
        
        //第二个金额
        pSkillName->setColor(Color3B::YELLOW);
        pSkillName->setScale(0.25);
        pSkillName->setPosition(tMid.width*0.3,tMid.height*0.05);
        pMid->addChild(pSkillName);
    }
    
    CDBMgr::getInstance()->FengeStr(strMonsterSKill3,id3,iSkillId3);
    
    if(iSkillId3 != 0)
    {
        skillInfo* pInfo3 = CDataManager::getTheSkillInfoWithId(iSkillId3);
        char strIcoPath[50] = {0};
        Label *pSkillName = NULL;
        
        if(id3>1000)
        {
            //为英雄图标
            vector<heroSkill*> pHeroSkillInfo = CDBMgr::getInstance()->getHeroSkillByMonsterId(id3);
            int   iSkillInex = CDBMgr::getInstance()->GetHeroSkillIndex(iSkillId3);
            sprintf(strIcoPath,"%s",pHeroSkillInfo[iSkillInex]->skillIcon.c_str());
            pHero = Sprite::createWithSpriteFrameName(strIcoPath);
            pSkillName =  Label::createWithSystemFont(pHeroSkillInfo[iSkillInex]->heroSkillName,"Aril",30);
        }
        else
        {
            sprintf(strIcoPath,"%s",pInfo3->skillIcon.c_str());
            pHero =  CDataManager::getSkillIcoWithId(strIcoPath,0.5);
            pSkillName =  Label::createWithSystemFont(pInfo3->skillName,"Aril",30);
        }
        
        pHero->setPosition(tMid.width*0.7,tMid.height*0.5);
        pHero->setScale(0.7);
        pMid->addChild(pHero);
        
        
        //第三个技能
        pSkillName->setColor(Color3B::YELLOW);
        pSkillName->setScale(0.25);
        pSkillName->setPosition(tMid.width*0.7,tMid.height*0.05);
        pMid->addChild(pSkillName);
        
    }
    
    CallFunc* getOff = CallFunc::create(CC_CALLBACK_0(CMainMenuLayer::OnRemoveNotice, this));
    runAction(Sequence::create(DelayTime::create(2.5f),getOff, NULL));
}


void CMainMenuLayer::OnRemoveNotice()
{
    Label *p4 = (Label*)getChildByTag(1000);
    
    if(p4)
    {
        p4->removeFromParent();
    }
    
    Sprite *p =  (Sprite *)getChildByTag(901);
    if(p)
    {
        p->removeFromParent();
    }
    
    Sprite * p1 =  (Sprite *)getChildByTag(902);
    if(p1)
    {
        p1->removeFromParent();
    }
    
    
    Sprite * p2 =  (Sprite *)getChildByTag(903);
    if(p2)
    {
        p2->removeFromParent();
    }
    
    Sprite *p3 = (Sprite*)getChildByTag(900);
    if(p3)
    {
        p3->removeFromParent();
    }
    
}

void CMainMenuLayer::addMidLevelLine()
{
    char buf[128];
    //添加路线 获取所有点得坐标
    std::vector<cocos2d::Vec2> pointVec;
    m_midLevelInfoVec = CDataManager::getMidLevelInfoFromDB();
    for(int i = 0; i < m_midLevelInfoVec.size(); i++)
    {
        if(m_midLevelInfoVec[i].pointPocVec.size() > 0)
        {
            for(int j = 0; j < m_midLevelInfoVec[i].pointPocVec.size(); j++)
            {
                Vec2 tempPos;
                tempPos.x = m_midLevelInfoVec[i].pointPocVec.at(j).x;
                tempPos.y = m_midLevelInfoVec[i].pointPocVec.at(j).y;
                pointVec.push_back(tempPos);
            }
        }
        if(m_midLevelInfoVec[i].midLevelId >= TOWER_LEVEL_INDEX)
        {
            m_levelSum++;
        }
    }
    
    std::vector<levelAndStar> levelInfo = CPersonalData::getInstance()->getUserInfo()->m_userLevelVec;
    int sum = 0;
    m_reLevel = 1;
    for(int i = 1; i < LEVEL_MAX; i++)
    {
        sum += CDataManager::getAllLevelById(i);
        if(sum >= levelInfo.size())
        {
            m_reLevel = i;
            if(sum == levelInfo.size())
            {
                m_reLevel = i + 1;
            }
            break;
        }
    }
    if((m_reLevel == 2) && (!CDataManager::getEndedBy(GUIDE_ENTER_STORAGE)))//如果第一大关卡已完成,但仓库的引导未完成则只显示第一大关卡
    {
        m_reLevel--;
    }
    Size bgSize = m_map->getBoundingBox().size;
    int index = 0;
    int midLevel = 0;
    for(int i = 1; i < LEVEL_MAX; i++)
    {
        sprintf(buf, "map_point.png");
        for(int j = 0; j < pointVec.size(); j++)
        {
            int opcity = 0;
            Vec2 pos = Vec2(bgSize.width * pointVec[j + index].x, bgSize.height * pointVec[j + index].y);
            int size = int(CDataManager::getMidLevelInfoById(i)->pointPocVec.size());
            if(j < size)
            {
                if(i < m_reLevel)
                {
                    if((i == m_reLevel - 1) && (!CPersonalData::getInstance()->getisWinManual())
                       && (CFightData::getInstance()->getNowLevelId() == CPersonalData::getInstance()->getUserInfo()->m_iSmallLevel))
                    {
                        opcity = 0;
                    }
                    else{
                        opcity = 255;
                    }
                }
                addMapLine(buf, pos, LEVEL_LINE_ONE + j + index, opcity);
            }
            else{
                index += int(CDataManager::getMidLevelInfoById(i)->pointPocVec.size());
                break;
            }
        }
    }
}


void CMainMenuLayer::UpdateCheckSkill(float ft)
{
    if(m_bSkillStart)
    {
        IsSkillFinish();
    }
}

bool CMainMenuLayer::init()
{
    if (!CBaseLayer::init())
    {
        return false;
    }
    
    IsSkillFinish();
    
    m_pListener = EventListenerTouchOneByOne::create();
    m_pListener->setSwallowTouches(true);
    
    m_pListener->onTouchBegan = CC_CALLBACK_2(CMainMenuLayer::onTouchBegan, this);
    m_pListener->onTouchMoved = CC_CALLBACK_2(CMainMenuLayer::onTouchMoved, this);
    m_pListener->onTouchEnded = CC_CALLBACK_2(CMainMenuLayer::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(m_pListener, this);
    
    
    //判断技能是否升级成功
    
    schedule(schedule_selector(CMainMenuLayer::UpdateCheckSkill), 2.0f);
    
    //添加随机背景音乐
    char buf[128];
    int randNum = CCRANDOM_0_1()*2+1;
    sprintf(buf, "music/backGround%d.mp3", randNum);
    if(UserDefault::getInstance()->getBoolForKey("bgMusic", true) && CPersonalData::getInstance()->getrestartPlayMusic())
    {
        SimpleAudioEngine::getInstance()->playBackgroundMusic(buf, true);
        UserDefault::getInstance()->setBoolForKey("bgMusic", true);
        SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(1.5f);
    }
    
    m_winSize = Director::getInstance()->getWinSize();
    
    if(MY_PERSION->getlevelMode() == 1)
    {
        m_isEqlite = true;
    }
    
    
    std::vector<midLevelInfo> midLevelInfo = CDataManager::getMidLevelInfoFromDB();
    for(int i = 1; i < midLevelInfo.size(); i++)
    {
        int allSmallLevel = 0;
        for(int j = 1; j <= i; j++)
        {
            allSmallLevel += CDataManager::getAllLevelById(j);
        }
        //        if((i == 1) && (CPersonalData::getInstance()->getUserInfo()->m_iSmallLevel == allSmallLevel))
        //        {
        //            CPersonalData::getInstance()->setNewMidLevel(true);
        //        }
    }
    //遍历获取已通关大关卡数
    std::vector<levelAndStar> levelInfo = CPersonalData::getInstance()->getUserInfo()->m_userLevelVec;
    int sum = 0;
    for(int i = 1; i < LEVEL_MAX; i++)
    {
        sum += CDataManager::getAllLevelById(i);
        if(sum > levelInfo.size()){
            m_midLevel = i;
            break;
        }
        else if(sum == levelInfo.size()){
            if (CPersonalData::getInstance()->getNewMidLevel()) {
                m_midLevel = i;
            }
            else{
                m_midLevel = i + 1;
            }
            break;
        }
    }
    if((m_midLevel == 2) && (!CDataManager::getEndedBy(GUIDE_ENTER_STORAGE)))//如果第一大关卡已完成,但仓库的引导未完成则只显示第一大关卡
    {
        m_midLevel--;
    }
    initMap();
    
    Sprite* tempSprite = Sprite::createWithSpriteFrameName("suggestion_icon.png");
    Sprite* tempSprite2 = Sprite::createWithSpriteFrameName("suggestion_icon.png");
    LYMenuItemSprite* temp = LYMenuItemSprite::create(tempSprite, tempSprite2,CC_CALLBACK_1(CMainMenuLayer::callBackSugges, this));
    
    Menu* theMenu = Menu::create(temp, NULL);
    addChild(theMenu,7);
    theMenu->setPosition(Vec2::ZERO);
    temp->setPosition(Vec2(m_winSize.width-tempSprite->getBoundingBox().size.width*0.7f, m_winSize.height*0.8f));
    
    //建议按钮动画
    Sprite* tempCircle = Sprite::createWithSpriteFrameName("message_1.png");
    addChild(tempCircle, 6);
    tempCircle->setPosition(temp->getPosition());
    Animation* pAnimation = Animation::create();
    for(int i = 1; i < 6; i++)
    {
        sprintf(buf, "message_%d.png", i);
        //pAnimation->addSpriteFrameWithFile();
    
        
        pAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(buf));
    }
    pAnimation->setDelayPerUnit(0.15f);
    Animate* pAct = Animate::create(pAnimation);
    tempCircle->runAction(RepeatForever::create(pAct));
    
    if(CPersonalData::getInstance()->getJumpFromSkill())
    {
        CLayerManager::GetSingletonPtr()->createLayer(eLayer_LvSkill,this,false);
    }
    
   //CPersonalData::getInstance()->setUpdateUserInfo(true);
    
    CPersonalData::getInstance()->setUpdateDrawCardInfo(true);
    
    
    return true;
}

void CMainMenuLayer::initMap()
{
//    为什么要加？ by ly
//    CDataManager::insertIntoMonsterInfo(2, 0, 1, 1);
//    CDataManager::insertIntoMonsterInfo(8, 0, 1, 1);
//    CDataManager::insertIntoMonsterInfo(13, 0, 1, 1);
    m_map = Sprite::createWithSpriteFrameName("map.png");
    m_map->setScale(2.0f);
    m_map->setAnchorPoint(Vec2(0, 0.5));
    Size mapSize = m_map->getBoundingBox().size;
    m_map->setPosition(Vec2(0, m_winSize.height * 0.469f));
    
    Size bgSize = m_map->getBoundingBox().size;
    
    m_pColor = LayerColor::create(Color4B(255, 0, 0, 0), mapSize.width, mapSize.height);
    m_pColor->addChild(m_map);
    m_pScrollView = ScrollView::create(m_winSize, m_pColor);
    m_pScrollView->setPosition(Vec2(0, 25));
    m_pScrollView->setDirection(ScrollView::Direction::HORIZONTAL);
    m_pScrollView->setViewSize(Size(m_winSize.width, mapSize.height));
    m_pScrollView->setBounceable(false);
    addChild(m_pScrollView);
    
    
    if (!CDataManager::getEndedBy(GUIDE_GET_TASK)) {  //by ly 防止拖动时引导手指位置不正确
        m_pScrollView->setTouchEnabled(false);
    }
    
    
    p_Menu = CCMenu::create();
    addChild(p_Menu,5);
    p_Menu->setPosition(Vec2::ZERO);
    
    addUserIcon();
    
    addMidLevelLine();
    //初始化地图位置
    m_pMenu = Menu::create();
    m_pColor->addChild(m_pMenu);
    m_pMenu->setPosition(Vec2::ZERO);
    //        Point level_pos[LEVEL_MAX] = {Point(m_winSize.width * 0.15, m_winSize.height * 0.645),
    //            Point(m_winSize.width * 0.215, m_winSize.height * 0.48),
    //            Point(m_winSize.width * 0.225, m_winSize.height * 0.275),
    //            Point(m_winSize.width * 0.405, m_winSize.height * 0.335),
    //            Point(m_winSize.width * 0.475, m_winSize.height * 0.548),//5
    //            Point(m_winSize.width * 0.565, m_winSize.height * 0.44),
    //            Point(m_winSize.width * 0.692, m_winSize.height * 0.585),
    //            Point(m_winSize.width * 0.84, m_winSize.height * 0.595),
    //            Point(m_winSize.width * 0.95, m_winSize.height * 0.5),
    //            Point(m_winSize.width * 1.04, m_winSize.height * 0.33),//10
    //            Point(m_winSize.width * 1.089, m_winSize.height * 0.625),
    //            Point(m_winSize.width * 1.212, m_winSize.height * 0.69),
    //            Point(m_winSize.width * 1.16, m_winSize.height * 0.44),
    //            Point(m_winSize.width * 1.24, m_winSize.height * 0.3),
    //            Point(m_winSize.width * 1.36, m_winSize.height * 0.365)}; //15
    for (int i = 0; i < LEVEL_MAX; i++)//i < midLevelInfoVec.size(); i++)
    {
        std::string icon = m_midLevelInfoVec.at(i).icon;
        Point pos = Vec2(bgSize.width * m_midLevelInfoVec.at(i).xpos, bgSize.height * m_midLevelInfoVec.at(i).ypos);
        if(i + 1 == m_midLevel)
        {
            icon = "cur_level_btn.png";
            m_arrow = Sprite::createWithSpriteFrameName("main_arrow.png");
            m_pColor->addChild(m_arrow, 10);
            m_arrow->setScale(0.7f);
            m_arrow->setPosition(Vec2(pos.x, pos.y + 70));
            ActionInterval* pAction = Sequence::create(MoveBy::create(1.0, Vec2(0, 15)), MoveBy::create(1.0, Vec2(0, -15)), NULL);
            m_arrow->runAction(RepeatForever::create(pAction));
            
            
            char buf[256];
            m_circle = Sprite::createWithSpriteFrameName("main_circle_1.png");
            m_pColor->addChild(m_circle, 10);
            m_circle->setPosition(pos);
            Animation* pAnimation = Animation::create();
            for(int i = 1; i < 9; i++)
            {
                sprintf(buf, "main_circle_%d.png", i);
                pAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(buf));
            }
            pAnimation->setDelayPerUnit(0.15f);
            Animate* pAct = Animate::create(pAnimation);
            m_circle->runAction(RepeatForever::create(Sequence::create(pAct, DelayTime::create(0.15f), NULL)));
        }
        addLevelBtn(icon.c_str(), icon.c_str(), m_pMenu, pos, i+1);
    }
    
    //显示塔防模式关卡
    m_towerMenu = Menu::create();
    m_pColor->addChild(m_towerMenu, 2);
    m_towerMenu->setPosition(Vec2::ZERO);
    
    //    Point towerPos[LEVEL_TOWER_MAX] = {Point(m_winSize.width * 0.347f, m_winSize.height * 0.61),
    //        Point(m_winSize.width * 0.555f, m_winSize.height * 0.21),
    //        Point(m_winSize.width * 0.605f, m_winSize.height * 0.655),
    //        Point(m_winSize.width * 0.69f, m_winSize.height * 0.26),
    //        Point(m_winSize.width * 0.9f, m_winSize.height * 0.27),//5
    //        Point(m_winSize.width * 0.935f, m_winSize.height * 0.79),
    //        Point(m_winSize.width * 1.13f, m_winSize.height * 0.23)};
    
    for(int k = TOWER_LEVEL_INDEX; k <= m_levelSum + TOWER_LEVEL_INDEX - 1; k++)
    {
        midLevelInfo* pInfo = CDataManager::getMidLevelInfoById(k);
        std::string icon = pInfo->icon;
        Point pos = Vec2(bgSize.width * pInfo->xpos, bgSize.height * pInfo->ypos);
        addLevelBtn(icon.c_str(), icon.c_str(), m_towerMenu, pos, k, true);
    }
    
    addBottomMenus();
    
    
    if(!(CDataManager::getEndedBy(GUIDE_MAP_INTRODUCE)) && (CPersonalData::getInstance()->getUserInfo()->m_iSmallLevel >= 2))
    {
        
        CPersonalData::getInstance()->setisWinManual(true);
        CPersonalData::getInstance()->setisFailManual(true);
        CPersonalData::getInstance()->setisOpenNewSmallLevel(false);
        CPersonalData::getInstance()->setisOpenNewLevel(false);

        guiderManagerLayer::getInstance()->setGuideType(GUIDE_MAP_INTRODUCE);
        guiderManagerLayer::getInstance()->setStepNum(1);
//        Vec2 pos = Vec2(m_winSize.width * 0.059f, m_winSize.height * 0.8f);
        guiderManagerLayer::getInstance()->createLayer(this, GUIDE_SHOW_DIALOG,Vec2::ZERO,false);

    
    }else if(!(CDataManager::getEndedBy(GUIDE_UPGRADE_SOLIDER))&& (CPersonalData::getInstance()->getUserInfo()->m_iSmallLevel == 3))
    {
        CPersonalData::getInstance()->setisWinManual(true);
        CPersonalData::getInstance()->setisFailManual(true);
        CPersonalData::getInstance()->setisOpenNewSmallLevel(false);
        CPersonalData::getInstance()->setisOpenNewLevel(false);
        //引导士兵升级
        NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(CMainMenuLayer::upgradeAfter), "upgradeAfter", NULL);
        
        guiderManagerLayer::getInstance()->setGuideType(GUIDE_UPGRADE_SOLIDER);
        guiderManagerLayer::getInstance()->setStepNum(1);
        guiderManagerLayer::getInstance()->createLayer(this);
    }
    else if(!(CDataManager::getEndedBy(GUIDE_SKILL_UPGRADE)) && (CPersonalData::getInstance()->getUserInfo()->m_iSmallLevel == 4))
    {
        CPersonalData::getInstance()->setisWinManual(true);
        CPersonalData::getInstance()->setisFailManual(true);
        CPersonalData::getInstance()->setisOpenNewSmallLevel(false);
        CPersonalData::getInstance()->setisOpenNewLevel(false);
        //引导士兵技能升级
        NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(CMainMenuLayer::upgradeAfter), "upgradeAfter", NULL);
        
        guiderManagerLayer::getInstance()->setGuideType(GUIDE_SKILL_UPGRADE);
        guiderManagerLayer::getInstance()->setStepNum(1);
        guiderManagerLayer::getInstance()->createLayer(this);
    }
    else if(!(CDataManager::getEndedBy(GUIDE_SOLIDER_NUM)) && (CPersonalData::getInstance()->getUserInfo()->m_iSmallLevel == 6))
    {
        CPersonalData::getInstance()->setisWinManual(true);
        CPersonalData::getInstance()->setisFailManual(true);
        CPersonalData::getInstance()->setisOpenNewSmallLevel(false);
        CPersonalData::getInstance()->setisOpenNewLevel(false);
        //引导士兵技能升级
        NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(CMainMenuLayer::upgradeAfter), "upgradeAfter", NULL);
        guiderManagerLayer::getInstance()->setGuideType(GUIDE_SOLIDER_NUM);
        guiderManagerLayer::getInstance()->setStepNum(1);
        guiderManagerLayer::getInstance()->createLayer(this);
    }
    else if(CPersonalData::getInstance()->getisWinManual() && !(CDataManager::getEndedBy(GUIDE_ENTER_STORAGE)) && (CPersonalData::getInstance()->getUserInfo()->m_iSmallLevel == 10))
    {
        CPersonalData::getInstance()->setisWinManual(true);
        CPersonalData::getInstance()->setisFailManual(true);
        CPersonalData::getInstance()->setisOpenNewSmallLevel(false);
        CPersonalData::getInstance()->setisOpenNewLevel(false);
        //引导士兵技能升级
        guiderManagerLayer::getInstance()->setGuideType(GUIDE_ENTER_STORAGE);
        guiderManagerLayer::getInstance()->setStepNum(1);
        Vec2 pos = Vec2(m_winSize.width * 0.5f, m_winSize.height * 0.09);
        guiderManagerLayer::getInstance()->createLayer(this, GUIDE_SHOW_BOTH, pos);
    }
    else if(!(CDataManager::getEndedBy(GUIDE_COMMON_CARD)) && (CPersonalData::getInstance()->getUserInfo()->m_iSmallLevel == 15))
    {
        CPersonalData::getInstance()->setisWinManual(true);
        CPersonalData::getInstance()->setisFailManual(true);
        CPersonalData::getInstance()->setisOpenNewSmallLevel(false);
        CPersonalData::getInstance()->setisOpenNewLevel(false);
        //引导抽卡
        guiderManagerLayer::getInstance()->setGuideType(GUIDE_COMMON_CARD);
        guiderManagerLayer::getInstance()->setStepNum(1);
        Vec2 pos = Vec2(m_winSize.width * 0.5f, m_winSize.height * 0.09);
        guiderManagerLayer::getInstance()->createLayer(this);
        NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(CMainMenuLayer::drawCardAfter), "drawCardAfter", NULL);
    }
    else if(!(CDataManager::getEndedBy(GUIDE_SENIOR_CARD)) && (CPersonalData::getInstance()->getUserInfo()->m_iSmallLevel == 15))
    {
        CPersonalData::getInstance()->setisWinManual(true);
        CPersonalData::getInstance()->setisFailManual(true);
        CPersonalData::getInstance()->setisOpenNewSmallLevel(false);
        CPersonalData::getInstance()->setisOpenNewLevel(false);
        //引导抽卡
        guiderManagerLayer::getInstance()->setGuideType(GUIDE_SENIOR_CARD);
        guiderManagerLayer::getInstance()->setStepNum(1);
        Vec2 pos = Vec2(m_winSize.width*0.06f,m_winSize.height*0.46f);
        guiderManagerLayer::getInstance()->createLayer(this, GUIDE_SHOW_BOTH, pos);
        NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(CMainMenuLayer::drawCardAfter), "drawCardAfter", NULL);
    }
    else if((CDataManager::getEndedBy(GUIDE_SENIOR_CARD)) && !(CDataManager::getEndedBy(GUIDE_SOLIDER_COMPOUND)) && (CPersonalData::getInstance()->getUserInfo()->m_iSmallLevel == 15))
    {
        guiderManagerLayer::getInstance()->setGuideType(GUIDE_SOLIDER_COMPOUND);
        guiderManagerLayer::getInstance()->setStepNum(1);
        LYMenuItemSprite* temp = (LYMenuItemSprite*)p_Menu->getChildByTag(MAP_BTN_HERO);
        Vec2 pos = temp->convertToWorldSpaceAR(Vec2::ZERO);
        guiderManagerLayer::getInstance()->createLayer(this, GUIDE_SHOW_FINGER, pos);
        NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(CMainMenuLayer::drawCardAfter), "drawCardAfter", NULL);
    }
    else if((CDataManager::getEndedBy(GUIDE_SOLIDER_COMPOUND)) && !(CDataManager::getEndedBy(GUIDE_SOLIDER_TEAM)) && (CPersonalData::getInstance()->getUserInfo()->m_iSmallLevel == 15))
    {
        guiderManagerLayer::getInstance()->setGuideType(GUIDE_SOLIDER_TEAM);
        guiderManagerLayer::getInstance()->setStepNum(1);
        LYMenuItemSprite* temp = (LYMenuItemSprite*)p_Menu->getChildByTag(MAP_BTN_TEAM);
        Vec2 pos = temp->convertToWorldSpaceAR(Vec2::ZERO);
        guiderManagerLayer::getInstance()->createLayer(this, GUIDE_SHOW_FINGER, pos);
    }

    if(!CPersonalData::getInstance()->getisWinManual())
    {//如果是从结算进入的
        std::vector<levelAndStar> levelInfo = CPersonalData::getInstance()->getUserInfo()->m_userLevelVec;
        int sum = 0;
        for(int i = 1; i < LEVEL_MAX; i++)
        {
            sum += CDataManager::getAllLevelById(i);
            if(sum >= levelInfo.size()){
                m_midLevel = i;
                break;
            }
        }
        if((CPersonalData::getInstance()->getlevelMode() == 0) && (!CPersonalData::getInstance()->getFinishedLevel()))
        {
            CFightData::getInstance()->setNowMidLevelId(m_midLevel);
        }
        enterLevelInfo(m_midLevel);
    }
    else{
        if(!(CDataManager::getEndedBy(GUIDE_PVP_BUILDING)) && (CPersonalData::getInstance()->getUserInfo()->m_iSmallLevel == 20))
        {
            CPersonalData::getInstance()->setisWinManual(true);
            CPersonalData::getInstance()->setisFailManual(true);
            CPersonalData::getInstance()->setisOpenNewSmallLevel(false);
            CPersonalData::getInstance()->setisOpenNewLevel(false);
            guiderManagerLayer::getInstance()->setGuideType(GUIDE_PVP_BUILDING);
            guiderManagerLayer::getInstance()->setStepNum(1);
            LYMenuItemSprite* temp = (LYMenuItemSprite*)m_pTaskMenu->getChildByTag(MAP_BTN_PVP);
            Vec2 pos = temp->convertToWorldSpaceAR(Vec2::ZERO);
            guiderManagerLayer::getInstance()->createLayer(this, GUIDE_SHOW_BOTH, pos);
        }
    }
    if(!CPersonalData::getInstance()->getisFailManual())
    {
        CPersonalData::getInstance()->setisFailManual(true);
        enterLevelInfo(m_midLevel);
    }
    
    CallFunc* cloud = CallFunc::create(CC_CALLBACK_0(CMainMenuLayer::addCloud, this));
    runAction(Sequence::create(DelayTime::create(0.5f), cloud, NULL));
    
    if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_UPGRADE_SOLIDER) && (guiderManagerLayer::getInstance()->getStepNum() == 8))
    {
        guiderManagerLayer::getInstance()->setStepNum(guiderManagerLayer::getInstance()->getStepNum() + 1);
        Vec2 pos = Vec2(mapSize.width * 0.11, mapSize.height * 0.645);
        guiderManagerLayer::getInstance()->createLayer(this, GUIDE_SHOW_FINGER, pos);
    }
    //引导
    else if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SKILL_UPGRADE) && (guiderManagerLayer::getInstance()->getStepNum() == 10))
    {
        //显示指引关卡
        guiderManagerLayer::getInstance()->setStepNum(guiderManagerLayer::getInstance()->getStepNum() + 1);
        Vec2 pos = Vec2(mapSize.width * 0.11, mapSize.height * 0.645);
        guiderManagerLayer::getInstance()->createLayer(this, GUIDE_SHOW_FINGER, pos);
    }
    else if((guiderManagerLayer::getInstance()->getStepNum() == 5) && (guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SOLIDER_NUM))
    {
        guiderManagerLayer::getInstance()->setStepNum(guiderManagerLayer::getInstance()->getStepNum() + 1);
        Vec2 pos = Vec2(mapSize.width * 0.11, mapSize.height * 0.645);
        guiderManagerLayer::getInstance()->createLayer(this, GUIDE_SHOW_FINGER, pos);
    }
}

void CMainMenuLayer::showDrawCardResult()
{
    if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SENIOR_CARD) && (guiderManagerLayer::getInstance()->getStepNum() == 5))
    {
        CDataManager::saveGuideInfoToDB(GUIDE_SENIOR_CARD);
        guiderManagerLayer::getInstance()->createLayer(this);
    }
}

void CMainMenuLayer::drawCardAfter(Ref* pSender)
{
    //开始引导英雄合成
    if(!CDataManager::getEndedBy(GUIDE_SOLIDER_COMPOUND))
    {
        guiderManagerLayer::getInstance()->setGuideType(GUIDE_SOLIDER_COMPOUND);
        guiderManagerLayer::getInstance()->setStepNum(1);
        LYMenuItemSprite* temp = (LYMenuItemSprite*)p_Menu->getChildByTag(MAP_BTN_HERO);
        Vec2 pos = temp->convertToWorldSpaceAR(Vec2::ZERO);
        guiderManagerLayer::getInstance()->createLayer(this, GUIDE_SHOW_FINGER, pos);
    }
    else if(!CDataManager::getEndedBy(GUIDE_SOLIDER_TEAM))//编队
    {
        guiderManagerLayer::getInstance()->setGuideType(GUIDE_SOLIDER_TEAM);
        guiderManagerLayer::getInstance()->setStepNum(1);
        LYMenuItemSprite* temp = (LYMenuItemSprite*)p_Menu->getChildByTag(MAP_BTN_TEAM);
        Vec2 pos = temp->convertToWorldSpaceAR(Vec2::ZERO);
        guiderManagerLayer::getInstance()->createLayer(this, GUIDE_SHOW_FINGER, pos);
        NotificationCenter::getInstance()->removeObserver(this, "drawCardAfter");
    }
}

void CMainMenuLayer::upgradeAfter(Ref* pSender)
{
    NotificationCenter::getInstance()->removeObserver(this, "upgradeAfter");
    if(((guiderManagerLayer::getInstance()->getStepNum() == 11) && (guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SKILL_UPGRADE))
       || ((guiderManagerLayer::getInstance()->getStepNum() == 8) && (guiderManagerLayer::getInstance()->getGuideType() == GUIDE_UPGRADE_SOLIDER))
       || ((guiderManagerLayer::getInstance()->getStepNum() == 5) && (guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SOLIDER_NUM))
       || ((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_GET_TASK) && (guiderManagerLayer::getInstance()->getStepNum() == 4)))
    {
        guiderManagerLayer::getInstance()->setStepNum(guiderManagerLayer::getInstance()->getStepNum() + 1);
        Vec2 pos = Vec2(m_map->getBoundingBox().size.width * 0.11, m_map->getBoundingBox().size.height * 0.645);
        guiderManagerLayer::getInstance()->createLayer(this, GUIDE_SHOW_FINGER, pos);
    }
    
}

void CMainMenuLayer::addCloud()
{
    for(int i = 0; i < 3; i++)
    {
        Sprite* sp = (Sprite*)getChildByTag(100+i);
        if(sp)
        {
            sp->removeFromParent();
            sp = NULL;
        }
    }
    int num = rand() % 3;
    char buf[256];
    float height[3] = {m_winSize.height * 0.76f, m_winSize.height * 0.5f, m_winSize.height * 0.25f};
    float duration[3] = {35, 20, 10};
    for(int i = 0; i < num; i++)
    {
        sprintf(buf, "cloud_%d.png", i + 1);
        Sprite* m_cloud = Sprite::createWithSpriteFrameName(buf);
        addChild(m_cloud, 100 + i);
        m_cloud->setScale(2.0f);
        float interTime = duration[i] * 0.5f + rand() % int(duration[i]);
        m_cloud->setPosition(Vec2(-m_winSize.width * 0.25, height[i]));
        int index = int(CCRANDOM_0_1()*2);
        ActionInterval* pAction = MoveTo::create(duration[i], Vec2(m_winSize.width * 1.25f, height[index]));
        if(rand() % 3 == 1)
        {
            pAction = Spawn::create(pAction, FadeTo::create(duration[i], 210), NULL);
        }
        m_cloud->runAction(Sequence::create(DelayTime::create(i * 0.5f), pAction,
                                            MoveTo::create(0, Vec2(-m_winSize.width * 0.25, height[index])),
                                            DelayTime::create(interTime), NULL));
    }
}

void CMainMenuLayer::addMapLine(const char* img, cocos2d::Point pos, int tag, int opcity)
{
    Sprite* spLine = Sprite::createWithSpriteFrameName(img);
    m_pColor->addChild(spLine, 0, tag);
    spLine->setPosition(pos);
    spLine->setOpacity(opcity);
    //    //先获取当前关卡之前有多少条线
    //    int allLine = 0;
    //    for(int i = 0; i < m_midLevel - 1; i++)
    //    {
    //        allLine += line[i];
    //    }
    //    if(m_lineNum < allLine){
    //        spLine->setOpacity(255);
    //        m_lineNum++;
    //    }
}

void CMainMenuLayer::addMapEffect()
{
    CPersonalData::getInstance()->setisOpenNewLevel(false);
    CPersonalData::getInstance()->setNewMidLevel(false);
    if(m_midLevel >= 15)
    {
        return;
    }
    
    m_arrow->setVisible(false);
    
    int allLine = 0;//到当前关卡线的总数
    int preAllLine = 0;//到前一关卡线的总数
    if(m_midLevel > 2)
    {
        for(int i = 0; i < m_midLevel - 2; i++)
        {
            preAllLine += CDataManager::getMidLevelInfoById(i+1)->pointPocVec.size();
        }
    }
    
    for(int i = 0; i < m_midLevel - 1; i++)
    {
        allLine += CDataManager::getMidLevelInfoById(i+1)->pointPocVec.size();
    }
    
    for (int i = preAllLine; i < allLine; i++) {
        Sprite* spLine = (Sprite*)(m_pColor->getChildByTag(LEVEL_LINE_ONE + i));
        if (spLine) {
            spLine->runAction(Sequence::create(DelayTime::create((i - preAllLine)* 0.24f + 0.4f), FadeTo::create(0.3, 255), NULL));
        }
    }
    float delayTime = (allLine - preAllLine) * 0.24f;
    LYMenuItemSprite* LevelItem = (LYMenuItemSprite*)(m_pMenu->getChildByTag(m_midLevel));
    if (LevelItem) {
        LevelItem->runAction(Sequence::create(DelayTime::create(0.4f + delayTime), FadeTo::create(0.5, 255), NULL));
        LevelItem->setEnabled(true);
    }
    
    CallFunc* arrowAct = CallFunc::create(CC_CALLBACK_0(CMainMenuLayer::updateArrowPos, this));
    
    Sprite* starBg = (Sprite*)(m_pColor->getChildByTag(m_midLevel + 50));
    if(starBg)
    {
        starBg->runAction(Sequence::create(DelayTime::create(0.45f + delayTime), FadeTo::create(0.5, 255), NULL));
    }
    Sprite* star = (Sprite*)(starBg->getChildByTag(1));
    if(star){
        star->runAction(Sequence::create(DelayTime::create(0.45f + delayTime), FadeTo::create(0.5, 255), NULL));
    }
    Label* pLabel = (Label*)(starBg->getChildByTag(2));
    if(pLabel){
        pLabel->runAction(Sequence::create(DelayTime::create(0.45f + delayTime), FadeTo::create(0.5, 255), arrowAct, NULL));
    }
    
    
    
    //    midLevelInfo* pInfo = CDataManager::getMidLevelInfoById(m_midLevel - 1);
    //    //显示前一关卡
    //    sprintf(buf, "%s精英模式已开启", pInfo->name.c_str());
    //    Label* heroLevel = Label::createWithSystemFont(buf, "Arial", 32);
    //    heroLevel->setColor(Color3B::GREEN);
    //    addChild(heroLevel, 10);
    //    heroLevel->setOpacity(0);
    //    heroLevel->setPosition(Vec2(m_winSize.width / 2, m_winSize.height / 2));
    //    heroLevel->runAction(Sequence::create(DelayTime::create(delayTime + 0.2f), FadeIn::create(0), MoveBy::create(1.5f, Vec2(0, 200)), NULL));
    //    heroLevel->runAction(Sequence::create(DelayTime::create(delayTime + 1.1f), FadeOut::create(1.0f), NULL));
    //
    //    pInfo = CDataManager::getMidLevelInfoById(m_midLevel);
    //    sprintf(buf, "%s开启", pInfo->name.c_str());
    //    Label* newLevel = Label::createWithSystemFont(buf, "Arial", 32);
    //    addChild(newLevel, 10);
    //    newLevel->setColor(Color3B::GREEN);
    //    newLevel->setOpacity(0);
    //    newLevel->setPosition(Vec2(m_winSize.width / 2, m_winSize.height / 2));
    //    newLevel->runAction(Sequence::create(DelayTime::create(delayTime + 0.8f), FadeIn::create(0), MoveBy::create(1.5f, Vec2(0, 200)), NULL));
    //    newLevel->runAction(Sequence::create(DelayTime::create(delayTime + 1.2f), FadeOut::create(1.0f), NULL));
    Sprite* tipBg1 = Sprite::createWithSpriteFrameName("ditiao.png");
    addChild(tipBg1,100);
    tipBg1->setCascadeOpacityEnabled(true);
    tipBg1->setOpacity(0);
    tipBg1->setPosition(Vec2(m_winSize.width * 0.5f, m_winSize.height * 0.55f));
    Sprite* newMidLevel1 = Sprite::createWithSpriteFrameName("openNewMidLevel.png");
    tipBg1->addChild(newMidLevel1);
    newMidLevel1->setPosition(tipBg1->getBoundingBox().size.width * 0.5f, tipBg1->getBoundingBox().size.height * 0.5f);
    tipBg1->runAction(Sequence::create(FadeTo::create(0.5f, 255), DelayTime::create(0.8f), FadeTo::create(0.3f, 0), NULL));
    
    Sprite* tipBg2 = Sprite::createWithSpriteFrameName("ditiao.png");
    addChild(tipBg2,100);
    tipBg2->setCascadeOpacityEnabled(true);
    tipBg2->setOpacity(0);
    tipBg2->setPosition(Vec2(m_winSize.width * 0.5f, m_winSize.height * 0.55f));
    Sprite* newMidLevel2 = Sprite::createWithSpriteFrameName("openNewTowerLevel.png");
    tipBg2->addChild(newMidLevel2);
    newMidLevel2->setPosition(tipBg1->getBoundingBox().size.width * 0.5f, tipBg1->getBoundingBox().size.height * 0.5f);
    tipBg2->runAction(Sequence::create(DelayTime::create(1.5f), FadeTo::create(0.8f, 255),
                                       DelayTime::create(0.3f), FadeTo::create(0.5f, 0), NULL));
}

void CMainMenuLayer::updateArrowPos()
{
    m_arrow->setVisible(true);
    for (int i = 0; i < LEVEL_MAX; i++)
    {
        std::string icon = m_midLevelInfoVec.at(i).icon;
        Point pos = Vec2(m_map->getBoundingBox().size.width * m_midLevelInfoVec.at(i).xpos,
                         m_map->getBoundingBox().size.height * m_midLevelInfoVec.at(i).ypos);
        if(i + 1 == m_midLevel)
        {
            m_arrow->setPosition(Vec2(pos.x, pos.y + 70));
            m_circle->setPosition(pos);
            break;
        }
    }
}

void CMainMenuLayer::addLevelBtn(const char* normal, const char* selected, Node* parent, cocos2d::Point pos, int tag, bool bTower)
{
    Sprite* normalSp = Sprite::createWithSpriteFrameName(normal);
    Sprite* selectSp = Sprite::createWithSpriteFrameName(selected);
    
    selectSp->setColor(Color3B::GRAY);
    LYMenuItemSprite* item = LYMenuItemSprite::create(normalSp, selectSp, CC_CALLBACK_1(CMainMenuLayer::btnCallBack, this));
    item->setPosition(pos);
    item->setTag(tag);
    parent->addChild(item);
    
    
    if (bTower) {
        float randTime = CCRANDOM_0_1() * 2.0f;
        
        item->runAction(Sequence::create(DelayTime::create(randTime), CallFuncN::create([](Node* pSender){
            ActionInterval* move = Sequence::create(MoveBy::create(1.25f, Vec2(0, 10)), MoveBy::create(1.25f, Vec2(0, -10)), NULL);
            pSender->runAction(RepeatForever::create(move));
        }), NULL));
        item->setVisible(false);
        //影子
        Sprite* shade = Sprite::createWithSpriteFrameName("level_shade.png");
        item->addChild(shade);
        shade->setPosition(Vec2(normalSp->getContentSize().width * 0.5f, -shade->getContentSize().height));
        shade->runAction(Sequence::create(DelayTime::create(randTime), CallFuncN::create([](Node* pSender){
            ActionInterval* scale = Sequence::create(ScaleTo::create(1.25f, 0.65), ScaleTo::create(1.25f, 1.0),NULL);
            pSender->runAction(RepeatForever::create(scale));
        }), NULL));
        shade->setVisible(false);
        if((m_reLevel > (tag - 1000)) || (CPersonalData::getInstance()->getUserInfo()->m_iSmallLevel >=
                                          CDataManager::getMidLevelInfoById(tag - 1000)->lastLevel))
        {
            item->setVisible(true);
            shade->setVisible(true);
            //判读是否显示新图标
            bool bExisted = false;
            std::vector<int> towerInfo = CPersonalData::getInstance()->getUserInfo()->m_userTowerLevelVec;
            for(int i = 0; i < towerInfo.size(); i++)
            {
                if(towerInfo[i] == tag)
                {
                    bExisted = true;
                    break;
                }
            }
            if(!bExisted)
            {
                Sprite* newIcon = Sprite::createWithSpriteFrameName("newTower.png");
                item->addChild(newIcon);
                newIcon->setPosition(Vec2(normalSp->getContentSize().width * 0.5, normalSp->getContentSize().height * 1.15f));
            }
        }
    }
    else{
        CommonData::enlargeItemSprite(item, 2);
        int starNum = CDataManager::getAllLevelStarById(tag) + CDataManager::getAllLevelStarById(tag, true);
        
        Sprite* starBg = Sprite::createWithSpriteFrameName("main_star_bg.png");
        m_pColor->addChild(starBg, 2, tag + 50);
        starBg->setPosition(Vec2(pos.x, pos.y -55));
        
        Sprite* star = Sprite::createWithSpriteFrameName("star.png");
        starBg->addChild(star, 0, 1);
        star->setAnchorPoint(Vec2(0, 0.5));
        star->setScale(0.85f);
        star->setPosition(Vec2(5, starBg->getContentSize().height * 0.52));
        
        char buf[128];
        sprintf(buf, "%d/%d", starNum, CDataManager::getAllLevelById(tag) * 6);
        Label* pLabel = Label::createWithSystemFont(buf, "Arial", 22);
        starBg->addChild(pLabel, 0, 2);
        pLabel->setPosition(Vec2(starBg->getContentSize().width * 0.58, star->getPositionY() - 3));
        
        if(CPersonalData::getInstance()->getisOpenNewLevel()){
            if(tag > m_midLevel - 1){
                item->setOpacity(0);
                item->setEnabled(false);
                starBg->setOpacity(0);
                star->setOpacity(0);
                pLabel->setOpacity(0);
            }
        }
        else{
            if(tag > m_midLevel){// || ((CPersonalData::getInstance()->getShowMap()) && (m_midLevel >= 4) && (tag == m_midLevel)))
                item->setOpacity(0);
                item->setEnabled(false);
                starBg->setOpacity(0);
                star->setOpacity(0);
                pLabel->setOpacity(0);
            }
        }
    }
}


bool CMainMenuLayer::onTouchBegan(Touch *pTouch, Event *pEvent)
{
    
    Sprite *p  = (Sprite *)getChildByTag(100);
    
    if(p)
    {
        p->removeFromParent();
    }
    
    
    return true;
}

void CMainMenuLayer::onTouchMoved(Touch *pTouch, Event *pEvent)
{
    
}

void CMainMenuLayer::onTouchEnded(Touch *pTouch, Event *pEvent)
{
    
}

void CMainMenuLayer::onTouchCancelled(Touch *touch, Event *pEvent)
{
    
    
}


void CMainMenuLayer::OnLvDetail(Ref *pSender)
{
    //addChild(CBeNameLayer::create(1),99999);
    
    //return ;
    
    Sprite *p  = (Sprite *)getChildByTag(100);
    
    if(p)
    {
        p->removeFromParent();
    }
    
    Sprite *pBg = Sprite::createWithSpriteFrameName("jieshao.png");
    pBg->setPosition(pBg->getBoundingBox().size.width*0.5,m_winSize.height*0.75);
    addChild(pBg,USER_INTRDUCE,100);
    
    
    Size tBg = pBg->boundingBox().size;
    
    UserInfo* pUserInfo = CPersonalData::getInstance()->getUserInfo();
    int iUserLv = pUserInfo->m_iUserLevel;
    
    char strJinyan[100] = {0};
    sprintf(strJinyan,"玩家等级: %d级",iUserLv);
    
    
    
    Label *pLabelJinyan = Label::createWithTTF(strJinyan,"fonts/cuti.ttf",25);
    pLabelJinyan->setAnchorPoint(Vec2(0,0.5));
    pLabelJinyan->setColor(Color3B::BLACK);
    pLabelJinyan->setPosition(tBg.width*0.05,tBg.height*0.75);
    pBg->cocos2d::Node::addChild(pLabelJinyan,0,100);
    
    Label *pUpToNext = Label::createWithTTF("升到下一等级需要:","fonts/cuti.ttf",25);
    pUpToNext->setAnchorPoint(Vec2(0,0.5));
    pUpToNext->setColor(Color3B::BLACK);
    pUpToNext->setPosition(tBg.width*0.05,tBg.height*0.6);
    pBg->cocos2d::Node::addChild(pUpToNext,0,101);
    
    char strSoldierLv[50] = {0};
    sprintf(strSoldierLv,"当前士兵等级最大为:%d级 (玩家等级的两倍)",iUserLv*2);
    
    Label *pMonsterDesc = Label::createWithTTF(strSoldierLv,"fonts/cuti.ttf",22);
    pMonsterDesc->setAnchorPoint(Vec2(0,0.5));
    pMonsterDesc->setColor(Color3B::BLACK);
    pMonsterDesc->setPosition(tBg.width*0.05,tBg.height*0.1);
    pBg->addChild(pMonsterDesc,0.102);
    
    UserInfo* userInfo = CPersonalData::getInstance()->getUserInfo();
    float allLast = CDataManager::getUpgradeInfoByLevel(userInfo->m_iUserLevel + 1)->userExp;
    
    float allpre = CDataManager::getUpgradeInfoByLevel(userInfo->m_iUserLevel)->userExp;
    
    int iCurUserExp = userInfo->m_iUserExp;
    
    int iNowExp = iCurUserExp- (int)allpre;
    
    if(iNowExp<0)
    {
        iNowExp = 0;
    }
    
    char strLvTime[50] = {0};
    sprintf(strLvTime,"%d/%d",iNowExp,(int)(allLast-allpre));
    
    Label *pLvTime  = Label::createWithTTF(strLvTime,"fonts/cuti.ttf",25);
    pLvTime->setAnchorPoint(Vec2(0,0.5));
    pLvTime->setColor(Color3B::BLACK);
    pLvTime->setPosition(pUpToNext->getPositionX()+pUpToNext->getContentSize().width,tBg.height*0.6);
    pBg->addChild(pLvTime,0,102);
    
    Label *pDesc = Label::createWithTTF("可以通过完成日常任务和打关卡获得经验","fonts/cuti.ttf",22);
    pDesc->setAnchorPoint(Vec2(0,0.5));
    pDesc->setColor(Color3B::BLACK);
    pDesc->setPosition(tBg.width*0.05,tBg.height*0.38);
    pBg->cocos2d::Node::addChild(pDesc,0,103);
    //可以通过完成日常任务和打关卡获得经验
    
}


void CMainMenuLayer::OnMenuNameTouch(Ref *pSender)
{
    log("Name");
    
  
   addChild( CBeNameLayer::create(1),1111);
  
}

void CMainMenuLayer::OnGetCardInfo(Ref *pSender)
{
    m_CardInfos = CFightData::getInstance()->getDrawCardInfo();
    
    int iTime[5] = {60,120,60*3,60*4,60*5};

    
    int iIndex =  4-m_CardInfos.m_iCoindrawNum;
    
    
    m_iRemainTime = m_CardInfos.m_iServerTime-m_CardInfos.m_iCoindrawtime;
    
    m_iRemainStoneTime = m_CardInfos.m_iServerTime-m_CardInfos.m_iStonedrawtime;


    if(m_CardInfos.m_istoneDrawNum==0)
    {
        m_iRemainStoneTime =  86400-m_iRemainStoneTime;
    }
    
    
    if(m_CardInfos.m_iCoindrawNum<5)
    {
        m_iRemainTime = iTime[iIndex]- m_iRemainTime;
    }
    
    
    if(m_iRemainStoneTime<=0&&m_CardInfos.m_istoneDrawNum<=0)
    {
        m_bGetCard = false;
        m_pDrawCardRedPt->setVisible(true);
        CNetWorkDataSingle::GetSingletonPtr()->SaveDrawCardInfo(this,NULL,1,1);
        char strTime2[50] = {"curTime2"};
        UserDefault::getInstance()->setIntegerForKey(strTime2,0);
        UserDefault::getInstance()->flush();
        
    }
    if(m_CardInfos.m_istoneDrawNum>=1)
    {
        m_bGetCard = false;
        m_pDrawCardRedPt->setVisible(true);
        
    }
    m_bGetCard =true;
    
    
}


bool CMainMenuLayer::IsDrawCardRed()
{
    
    
    
    return  false;
   // return CNetWorkDataSingle::GetSingletonPtr()->GetDrawCardInfo(this,menu_selector(CMainMenuLayer::OnGetCardInfo));
    
}



void CMainMenuLayer::addUserIcon()
{
    Sprite* topBg = Sprite::createWithSpriteFrameName("bottom_1.png");
    addChild(topBg);
    topBg->setPosition(Vec2(m_winSize.width / 2, m_winSize.height - topBg->getBoundingBox().size.height / 2));
    
    UserInfo* userInfo = CPersonalData::getInstance()->getUserInfo();
    char buf[128];
    Sprite* nameBg = Sprite::createWithSpriteFrameName("information_bg.png");
    addChild(nameBg);
    nameBg->setPosition(Vec2(m_winSize.width * 0.105f, m_winSize.height - nameBg->getContentSize().height * 0.55f));
    
    Sprite* progressBg = Sprite::createWithSpriteFrameName("level.png");
    
    MenuItemSprite *pProgressItem = MenuItemSprite::create(progressBg,NULL,CC_CALLBACK_1(CMainMenuLayer::OnLvDetail,this));
    
    Menu *pMenLv =  Menu::create(pProgressItem,NULL);
    pMenLv->setPosition(Vec2(m_winSize.width * 0.04f,nameBg->getPositionY()));
    addChild(pMenLv);
    
    
    Sprite* levelBg = Sprite::createWithSpriteFrameName("useLevel_bg.png");
    addChild(levelBg);
    levelBg->setPosition(pMenLv->getPosition());
    
    Sprite* progress = Sprite::createWithSpriteFrameName("level_progress.png");
    m_progress = ProgressTimer::create(progress);
    addChild(m_progress);
    m_progress->setMidpoint(Vec2(0.5, 0));
    m_progress->setBarChangeRate(Vec2(0, 1));
    m_progress->setType(ProgressTimer::Type::BAR);
    m_progress->setPosition(pMenLv->getPosition());
    
    float allLast = CDataManager::getUpgradeInfoByLevel(userInfo->m_iUserLevel + 1)->userExp;
    
    float allpre = CDataManager::getUpgradeInfoByLevel(userInfo->m_iUserLevel)->userExp;
    
    int iCurUserExp = userInfo->m_iUserExp;
    
    float  iNowExp = (float)iCurUserExp-allpre;
    
    if(iNowExp<0)
    {
        iNowExp = 0;
    }
    
    float percentage = iNowExp /(allLast-allpre);
    
    
    
    m_progress->setPercentage((percentage*100));
    
    Sprite* levelLight = Sprite::createWithSpriteFrameName("level_light.png");
    addChild(levelLight);
    levelLight->setPosition(pMenLv->getPosition());
    
    //用户等级
    sprintf(buf, "%d", userInfo->m_iUserLevel);
    m_userLevel = Label::createWithSystemFont(buf, "Araial", 40);
    addChild(m_userLevel, 5);
    m_userLevel->setPosition(pMenLv->getPosition());
    
    
    //用户名字
    //userInfo->userName.c_str()
    m_pUserName =  Label::createWithTTF(userInfo->userName.c_str(),"fonts/cuti.ttf",30);
    
    m_pUserName->enableOutline(Color4B(0,0,0,255),3);
    m_pUserName->setScale(0.7);
    
    
 


    
    rapidjson::SizeType iUserName = userInfo->userName.length();
    
    if(iUserName>9&&iUserName<18)
    {
        m_pUserName->setScale(0.6);
    }
    else if(iUserName>=18)
    {
        m_pUserName->setScale(0.5);
    }
    
    MenuItemFont *pItem = MenuItemFont::create("       ",CC_CALLBACK_1(CMainMenuLayer::OnMenuNameTouch, this));

   
    
    Menu *pMenu = Menu::create(pItem,NULL);
    
    
    
    pMenu->setPosition(Vec2(nameBg->getContentSize().width * 0.56, nameBg->getContentSize().height / 2));
    nameBg->addChild(pMenu);
    
    nameBg->addChild(m_pUserName,1);
    m_pUserName->setPosition(Vec2(nameBg->getContentSize().width * 0.56, nameBg->getContentSize().height / 2));
    
    //体力
    m_staminaBg = Sprite::createWithSpriteFrameName("information_bg.png");
    addChild(m_staminaBg);
    m_staminaBg->setScaleX(0.9f);
    m_staminaBg->setPosition(Vec2(m_winSize.width * 0.345f, nameBg->getPositionY()));
    Sprite* staminaIcon = Sprite::createWithSpriteFrameName("stamina.png");
    m_staminaBg->addChild(staminaIcon);
    staminaIcon->setScale(0.85f);
    staminaIcon->setAnchorPoint(Vec2(0, 0.5));
    staminaIcon->setPosition(Vec2(12, m_staminaBg->getBoundingBox().size.height * 0.498));
    
    //当前体力
    int maxStamina = CPersonalData::getInstance()->GetPhysicalMaxNum();
    
    sprintf(buf, "/%d", maxStamina);
    m_pStaminaMaxLabel = Label::createWithSystemFont(buf, "Arial", 20);
    m_pStaminaMaxLabel->setAnchorPoint(Vec2(1, 0.5));
    m_pStaminaMaxLabel->setPosition(Vec2(m_staminaBg->getBoundingBox().size.width * 0.98, m_staminaBg->getBoundingBox().size.height * 0.63f));
    m_staminaBg->addChild(m_pStaminaMaxLabel);
    
    sprintf(buf, "%d", CPersonalData::getInstance()->GetPhysicalNum());
    m_pStaminaLabel = Label::createWithSystemFont(buf, "Arial", 20);
    m_pStaminaLabel->setAnchorPoint(Vec2(1, 0.5));
    m_pStaminaLabel->setPosition(Vec2(m_pStaminaMaxLabel->getPositionX() - m_pStaminaMaxLabel->getContentSize().width, m_staminaBg->getBoundingBox().size.height * 0.63f));
    m_staminaBg->addChild(m_pStaminaLabel);
    
    //距下次体力恢复的时间
    m_bufferIime = CPhysicalSystem::GetSingletonPtr()->GetNextUpTime();
    sprintf(buf, "%02ld:%02ld", m_bufferIime/60, m_bufferIime%60);
    m_pTime = Label::createWithSystemFont(buf, "Arial", 20);
  
    
    m_pTime->setAnchorPoint(Vec2(1, 0.5));
    m_staminaBg->addChild(m_pTime);
    
    UserInfo* pInfo = CPersonalData::getInstance()->getUserInfo();
    int iUpperStamin = CDataManager::getUpgradeInfoByLevel(pInfo->m_iUserLevel)->upperStamina;
    
    if(CPersonalData::getInstance()->GetPhysicalNum()>=iUpperStamin)
    {
        m_pStaminaLabel->setSystemFontSize(30);
        m_pStaminaMaxLabel->setSystemFontSize(30);
        m_pStaminaMaxLabel->setPosition(Vec2(m_staminaBg->getBoundingBox().size.width * 0.98, m_staminaBg->getBoundingBox().size.height * 0.5f));
        m_pTime->setVisible(false);
        m_pStaminaLabel->setPosition(Vec2(m_pStaminaMaxLabel->getPositionX() - m_pStaminaMaxLabel->getContentSize().width, m_staminaBg->getBoundingBox().size.height * 0.5f));
        m_pStaminaLabel->setColor(Color3B(239, 146, 66));
    }
//    else
//    {
//        m_pStaminaLabel->setSystemFontSize(20);
//    }
    m_pTime->setPosition(Vec2(m_pStaminaMaxLabel->getPositionX(), m_pStaminaLabel->getPositionY() - m_pStaminaLabel->getContentSize().height + 5));
    
    
    //体力补给
    Sprite* normal = Sprite::createWithSpriteFrameName("plus.png");
    Sprite* select = Sprite::createWithSpriteFrameName("plus.png");
    select->setColor(Color3B::GRAY);
    LYMenuItemSprite* staminaItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(CMainMenuLayer::btnCallBack, this));
    staminaItem->setTag(STAMINA_CHARGE_TAG);
    CommonData::enlargeItemSprite(staminaItem, 3);
    Menu* pStaminaMenu = Menu::create(staminaItem, NULL);
    m_staminaBg->addChild(pStaminaMenu);
    staminaItem->setScale(0.7f);
    pStaminaMenu->setPosition(Vec2(m_staminaBg->getContentSize().width * 0.2f, m_staminaBg->getContentSize().height * 0.36f));
#ifdef NO_PAY
    staminaItem->setVisible(false);
#endif
    
    //金币
    Sprite* coinBg = Sprite::createWithSpriteFrameName("information_bg.png");
    addChild(coinBg);
    coinBg->setScaleX(0.9f);
    coinBg->setPosition(Vec2(m_winSize.width * 0.553f, nameBg->getPositionY()));
    Sprite* coinIcon = Sprite::createWithSpriteFrameName("single_coin.png");
    coinIcon->setAnchorPoint(Vec2(0, 0.5));
    coinIcon->setScale(0.8f);
    coinBg->addChild(coinIcon);
    coinIcon->setPosition(Vec2(12, coinBg->getBoundingBox().size.height * 0.45));
    //金币充值
    sprintf(buf, "%d", CPersonalData::getInstance()->getUserCoin());
    m_pCoin = Label::createWithSystemFont(buf, "Arial", 30);
    m_pCoin->setAnchorPoint(Vec2(1, 0.5));
    coinBg->addChild(m_pCoin);
    m_pCoin->setPosition(Vec2(coinBg->getBoundingBox().size.width * 0.98f, coinBg->getBoundingBox().size.height * 0.51f));
    normal = Sprite::createWithSpriteFrameName("plus.png");
    select = Sprite::createWithSpriteFrameName("plus.png");
    select->setColor(Color3B::GRAY);
    LYMenuItemSprite* coinItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(CMainMenuLayer::btnCallBack, this));
    CommonData::enlargeItemSprite(coinItem, 3);
    coinItem->setTag(COIN_CHARGE_TAG);
    coinItem->setScale(0.7f);
    Menu* pCoinMenu = Menu::create(coinItem, NULL);
    coinBg->addChild(pCoinMenu);
    pCoinMenu->setPosition(Vec2(coinBg->getBoundingBox().size.width * 0.2f, coinBg->getBoundingBox().size.height * 0.36f));
    
#ifdef NO_PAY
    coinItem->setVisible(false);
#endif
    
    
    
    //魔法石
    Sprite* stoneBg = Sprite::createWithSpriteFrameName("information_bg.png");
    addChild(stoneBg);
    stoneBg->setScaleX(0.9f);
    stoneBg->setPosition(Vec2(m_winSize.width * 0.76f, nameBg->getPositionY()));
    Sprite* stoneIcon = Sprite::createWithSpriteFrameName("stone.png");
    stoneIcon->setAnchorPoint(Vec2(0, 0.5));
    stoneBg->addChild(stoneIcon);
    stoneIcon->setScale(0.8f);
    stoneIcon->setPosition(Vec2(12, stoneBg->getBoundingBox().size.height * 0.43));
    //魔法石充值
    sprintf(buf, "%d", CPersonalData::getInstance()->getUserStone());
    m_pStone = Label::createWithSystemFont(buf, "Arial", 30);
    m_pStone->setAnchorPoint(Vec2(1, 0.5));
    stoneBg->addChild(m_pStone);
    m_pStone->setPosition(Vec2(stoneBg->getBoundingBox().size.width * 0.98f, stoneBg->getBoundingBox().size.height * 0.51f));
    
    normal = Sprite::createWithSpriteFrameName("plus.png");
    select = Sprite::createWithSpriteFrameName("plus.png");
    select->setColor(Color3B::GRAY);
    LYMenuItemSprite* stoneItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(CMainMenuLayer::btnCallBack, this));
    stoneItem->setTag(STONE_CHARGE_TAG);
    CommonData::enlargeItemSprite(stoneItem, 3);
    Menu* pStoneMenu = Menu::create(stoneItem, NULL);
    stoneBg->addChild(pStoneMenu);
    stoneItem->setScale(0.7f);
    pStoneMenu->setPosition(Vec2(stoneBg->getBoundingBox().size.width * 0.24f, stoneBg->getBoundingBox().size.height * 0.36f));
    
#ifdef NO_PAY
    stoneItem->setVisible(false);
#endif
    
    //设置
    normal = Sprite::createWithSpriteFrameName("set.png");
    select = Sprite::createWithSpriteFrameName("set.png");
    select->setColor(Color3B::GRAY);
    LYMenuItemSprite* setItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(CMainMenuLayer::btnCallBack, this));
    setItem->setTag(MAP_BTN_SET);
    Menu* pSetMenu = Menu::create(setItem, NULL);
    addChild(pSetMenu);
    pSetMenu->setPosition(Vec2(m_winSize.width * 0.94f, nameBg->getPositionY()));
    
    m_pTaskMenu = Menu::create();
    addChild(m_pTaskMenu, 600);
    m_pTaskMenu->setPosition(Vec2::ZERO);
    //日常图标
    Sprite* taskNormal = Sprite::createWithSpriteFrameName("dailyTask.png");
    Sprite* taskSelect = Sprite::createWithSpriteFrameName("dailyTask.png");
    taskSelect->setColor(Color3B::GRAY);
    LYMenuItemSprite* taskItem = LYMenuItemSprite::create(taskNormal, taskSelect, CC_CALLBACK_1(CMainMenuLayer::btnCallBack, this));
    taskItem->setTag(MAP_BTN_DAILY);
    taskItem->setPosition(Vec2(m_winSize.width * 0.06f, m_winSize.height * 0.8f));
    m_pTaskMenu->addChild(taskItem);
    taskItem->setVisible(false);
    if(CPersonalData::getInstance()->getUserInfo()->m_iSmallLevel >= 2)
    {
        taskItem->setVisible(true);
    }
    
    //任务图标
    normal = Sprite::createWithSpriteFrameName("task.png");
    select = Sprite::createWithSpriteFrameName("task.png");
    select->setColor(Color3B::GRAY);
    LYMenuItemSprite* dailyItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(CMainMenuLayer::btnCallBack, this));
    dailyItem->setTag(MAP_BTN_TASK);
    dailyItem->setPosition(Vec2(m_winSize.width * 0.06f, m_winSize.height * 0.625f));
    dailyItem->setVisible(false);
    m_pTaskMenu->addChild(dailyItem);
    if(CPersonalData::getInstance()->getUserInfo()->m_iSmallLevel >= 3)
    {
        dailyItem->setVisible(true);
    }
    

    normal = Sprite::createWithSpriteFrameName("drawcardico.png");
    select = Sprite::createWithSpriteFrameName("drawcardico.png");
    select->setColor(Color3B::GRAY);

    m_pDrawCardItem = LYMenuItemSprite::create(normal,select,CC_CALLBACK_1(CMainMenuLayer::btnCallBack,this));
    m_pDrawCardItem->setTag(MAP_BTN_DRAWCARD);
    m_pDrawCardItem->setVisible(false);
    m_pDrawCardItem->setPosition(m_winSize.width*0.06f,m_winSize.height*0.45f);
    m_pTaskMenu->addChild(m_pDrawCardItem);
    if(CPersonalData::getInstance()->getUserInfo()->m_iSmallLevel >= 15)
    {
        m_pDrawCardItem->setVisible(true);
    }
    
    //商店图标
    normal = Sprite::createWithSpriteFrameName("shop.png");
    select = Sprite::createWithSpriteFrameName("shop.png");
    select->setColor(Color3B::GRAY);
    LYMenuItemSprite* pShopItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(CMainMenuLayer::btnCallBack, this));
    pShopItem->setTag(MAP_BTN_SHOP);
    
#ifdef NO_PAY
    pShopItem->setVisible(false);
#endif

    
    pShopItem->setPosition(Vec2(m_winSize.width * 0.06f, m_winSize.height * 0.275f));
    //pShopItem->setVisible(false);
    m_pTaskMenu->addChild(pShopItem);
    
    //pvp图标
    normal = Sprite::createWithSpriteFrameName("pvp.png");
    select = Sprite::createWithSpriteFrameName("pvp.png");
    select->setColor(Color3B::GRAY);
    LYMenuItemSprite* pvpItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(CMainMenuLayer::btnCallBack, this));
    pvpItem->setTag(MAP_BTN_PVP);
    pvpItem->setPosition(Vec2(m_winSize.width * 0.938f, m_winSize.height * 0.62f));
    m_pTaskMenu->addChild(pvpItem, 0, MAP_BTN_PVP);

    
    //app不允许上没有开发的功能，因此注释掉
    if(GAME_VERSION=="App Store"){
        pvpItem->setVisible(false);
    }
    
#ifndef  DEBUG_Mode
    pvpItem->setVisible(false);
#endif
    
    if(CPersonalData::getInstance()->getUserInfo()->m_iSmallLevel >= 20)
    {
        pvpItem->setVisible(true);
    }
    
    //活动图标
    normal = Sprite::createWithSpriteFrameName("activity.png");
    select = Sprite::createWithSpriteFrameName("activity.png");
    select->setColor(Color3B::GRAY);
    LYMenuItemSprite* activityItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(CMainMenuLayer::btnCallBack, this));
    activityItem->setTag(MAP_BTN_ACTIVITY);
    activityItem->setPosition(Vec2(m_winSize.width * 0.938f, m_winSize.height * 0.46f));
    m_pTaskMenu->addChild(activityItem);
    activityItem->setColor(Color3B::GRAY);

    //app不允许上没有开发的功能，因此注释掉
    if(GAME_VERSION=="App Store"){
        activityItem->setVisible(false);
    }

    
    //充值图标
    normal = Sprite::createWithSpriteFrameName("stoneCharge.png");
    select = Sprite::createWithSpriteFrameName("stoneCharge.png");
    select->setColor(Color3B::GRAY);
    LYMenuItemSprite* chargeItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(CMainMenuLayer::btnCallBack, this));
    
    chargeItem->setTag(MAP_BTN_CHARGE);
    chargeItem->setPosition(Vec2(m_winSize.width * 0.938f, m_winSize.height * 0.295f));
    m_pTaskMenu->addChild(chargeItem);
    
#ifdef NO_PAY
    chargeItem->setVisible(false);
#endif
    
    
    schedule(schedule_selector(CMainMenuLayer::updateTime));
    scheduleUpdate();
    
}

void CMainMenuLayer::updateUsetInfo()
{
    char buf[128];
    
    m_bGetCard = false;
    std::vector<FighterInfo*>  pFightInfo;
    
    std::vector<FighterInfo*> pAllInfo = CPersonalData::getInstance()->getAllSoldierInfo();
    
    bool bGril =  CommonData::GetSingletonPtr()->IsGirlHeroVisable();
    
    if(!bGril)
    {
        
        for(int i=0;i<pAllInfo.size();i++)
        {
            //1 英雄  0怪物
            if(pAllInfo[i]->theType == 0&&pAllInfo[i]->rare>0)
            {
                pFightInfo.push_back(pAllInfo[i]);
            }
        }
        
    }
    else
    {
        
        pFightInfo = pAllInfo;
        
    }
    
    bool bEnough = false;
    
    
    for(int i=0;i<pFightInfo.size();i++)
    {
        std::vector<skillInfo*>  m_pskill = CDataManager::getAvailableSkillInfo(pFightInfo[i]->monsterId,pFightInfo[i]->rare);
        
        for(int j=0;j<m_pskill.size();j++)
        {
            bool bHero  = false;
            if(pFightInfo[i]->monsterId>1000)
            {
                bHero = true;
            }
            
            if( CDBMgr::getInstance()->IsSkillCanLvUp(pFightInfo[i]->monsterId,m_pskill[j]->skillId,bHero,pFightInfo[i]->theFighterSkillInfo[j].skillLevel))
            {
                bEnough = true;
            }
            
        }
    }
    Size tcollege = m_pCollegeItem->getContentSize();
    
    
    if(m_pRedPoint)
    {
        m_pRedPoint->removeFromParent();
        m_pRedPoint = NULL;
    }
    
  
    if(CDBMgr::getInstance()->GetSkillUpgradeInfoVec().size()<=2&& bEnough )
    {
        m_pRedPoint = Sprite::createWithSpriteFrameName("RedPoint.png");
        m_pRedPoint->setPosition(tcollege.width*0.9,tcollege.height*0.6);
        m_pCollegeItem->addChild(m_pRedPoint);
    }
    
    
    sprintf(buf, "%d", CPersonalData::getInstance()->getUserInfo()->m_iUserLevel);
    m_userLevel->setString(buf);
    
    UserInfo* userInfo = CPersonalData::getInstance()->getUserInfo();

    float allLast = CDataManager::getUpgradeInfoByLevel(userInfo->m_iUserLevel + 1)->userExp;
    
    float allpre = CDataManager::getUpgradeInfoByLevel(userInfo->m_iUserLevel)->userExp;
    
    int iCurUserExp = userInfo->m_iUserExp;
    
    float iNowExp = (float )iCurUserExp-allpre;
    
    if(iNowExp<0)
    {
        iNowExp = 0;
    }
    
    float percentage = iNowExp /(allLast-allpre);
    
    
    m_progress->setPercentage(percentage*100);
    
    sprintf(buf, "%d", CPersonalData::getInstance()->getUserCoin());
    m_pCoin->setString(buf);
    
    sprintf(buf, "%d", CPersonalData::getInstance()->getUserStone());
    m_pStone->setString(buf);
    
    int maxStamina = CPersonalData::getInstance()->GetPhysicalMaxNum();
    sprintf(buf, "%d", CPersonalData::getInstance()->GetPhysicalNum());
    m_pStaminaLabel->setString(buf);
    
    sprintf(buf, "/%d", maxStamina);
    m_pStaminaMaxLabel->setString(buf);
    m_pUserName->setString(userInfo->userName);
}

void CMainMenuLayer::addBottomMenus()
{
    
    Sprite* bottomBg = Sprite::createWithSpriteFrameName("bottom_2.png");
    addChild(bottomBg);
    bottomBg->setPosition(Vec2(m_winSize.width / 2, m_winSize.height * 0.03f));
    
    Sprite* normal = Sprite::createWithSpriteFrameName("upgrade.png");
    Sprite* select = Sprite::createWithSpriteFrameName("upgrade.png");
    select->setColor(Color3B::GRAY);
    
    LYMenuItemSprite* upgradeItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(CMainMenuLayer::btnCallBack, this));
    upgradeItem->setPosition(Vec2(m_winSize.width * 0.095f, m_winSize.height * 0.075f));
    upgradeItem->setScale(0.95f);
    upgradeItem->setTag(MAP_BTN_UPGRADE);
    m_bottomBtn.push_back(upgradeItem);
    p_Menu->addChild(upgradeItem);
#ifndef  DEBUG_Mode
    upgradeItem->setVisible(false);
#endif
    
    if(CPersonalData::getInstance()->getUserInfo()->m_iSmallLevel > 2)
    {
        upgradeItem->setVisible(true);
    }
    
    normal = Sprite::createWithSpriteFrameName("college.png");
    select = Sprite::createWithSpriteFrameName("college.png");
    select->setColor(Color3B::GRAY);
    m_pCollegeItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(CMainMenuLayer::btnCallBack, this));
    m_pCollegeItem->setPosition(Vec2(m_winSize.width * 0.29f, m_winSize.height * 0.075f));
    m_bottomBtn.push_back(m_pCollegeItem);
#ifndef  DEBUG_Mode
    m_pCollegeItem->setVisible(false);
#endif
    
    
    if(CPersonalData::getInstance()->getUserInfo()->m_iSmallLevel >= 4)
    {
        m_pCollegeItem->setVisible(true);
    }
    m_pCollegeItem->setTag(MAP_BTN_COLLEGE);
    m_pCollegeItem->setScale(0.95f);
    p_Menu->addChild(m_pCollegeItem);
    
    normal = Sprite::createWithSpriteFrameName("storage.png");
    select = Sprite::createWithSpriteFrameName("storage.png");
    select->setColor(Color3B::GRAY);
    LYMenuItemSprite* storageItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(CMainMenuLayer::btnCallBack, this));
    storageItem->setPosition(Vec2(m_winSize.width * 0.5f, m_winSize.height * 0.075f));
    m_bottomBtn.push_back(storageItem);
    storageItem->setTag(MAP_BTN_STORAGE);
    storageItem->setScale(0.95f);
    p_Menu->addChild(storageItem);
#ifndef  DEBUG_Mode
    storageItem->setVisible(false);
#endif
    if(CPersonalData::getInstance()->getUserInfo()->m_iSmallLevel >= 10)
    {
        storageItem->setVisible(true);
    }
    
    normal = Sprite::createWithSpriteFrameName("hero.png");
    select = Sprite::createWithSpriteFrameName("hero.png");
    select->setColor(Color3B::GRAY);
    LYMenuItemSprite* heroItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(CMainMenuLayer::btnCallBack, this));
    heroItem->setPosition(Vec2(m_winSize.width * 0.7f, m_winSize.height * 0.075f));
    m_bottomBtn.push_back(heroItem);
    heroItem->setTag(MAP_BTN_HERO);
    heroItem->setScale(0.95f);
    p_Menu->addChild(heroItem);
#ifndef  DEBUG_Mode
    heroItem->setVisible(false);
#endif
    if(CPersonalData::getInstance()->getUserInfo()->m_iSmallLevel >= 11)
    {
        heroItem->setVisible(true);
    }
    
    normal = Sprite::createWithSpriteFrameName("team.png");
    select = Sprite::createWithSpriteFrameName("team.png");
    select->setColor(Color3B::GRAY);
    LYMenuItemSprite* teamItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(CMainMenuLayer::btnCallBack, this));
    teamItem->setPosition(Vec2(m_winSize.width * 0.9f, m_winSize.height * 0.075f));
    m_bottomBtn.push_back(teamItem);
    teamItem->setTag(MAP_BTN_TEAM);
    teamItem->setScale(0.95f);
    p_Menu->addChild(teamItem, 1, MAP_BTN_TEAM);
#ifndef  DEBUG_Mode
    teamItem->setVisible(false);
#endif
}

void CMainMenuLayer::updateMusicState()
{
    if(UserDefault::getInstance()->getBoolForKey("bgMusic", true))
    {
        SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    }
    else{
        SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    }
}

void CMainMenuLayer::detectTask()
{
    //检测每日任务
    bool bTaskRedPoint = false;
    std::vector<TaskBase *>tempVec = CDataManager::GetAllTaskInfo();
    for(int i = 0; i < tempVec.size(); i++)
    {
        if((tempVec.at(i)->taskProp == 1) && (tempVec.at(i)->m_eTaskState == Task_Get))
        {
            bTaskRedPoint = true;
            break;
        }
    }
    LYMenuItemSprite* temp = (LYMenuItemSprite*)m_pTaskMenu->getChildByTag(MAP_BTN_DAILY);
    if(bTaskRedPoint)
    {
        bTaskRedPoint = false;
        if(temp)
        {
            Sprite* point = (Sprite*)temp->getChildByTag(50);
            if(!point)
            {
                Sprite *pRedPoint = Sprite::createWithSpriteFrameName("RedPoint.png");
                temp->addChild(pRedPoint, 1, 50);
                Vec2 pos = Vec2(temp->getNormalImage()->getContentSize().width * 0.8f, temp->getNormalImage()->getContentSize().height * 0.78f);
                pRedPoint->setPosition(pos);
            }
        }
    }
    else
    {
        Sprite* sp = (Sprite*)temp->getChildByTag(50);
        if(temp && sp)
        {
            sp->removeFromParent();
        }
    }
    for(int i = 0; i < tempVec.size(); i++)
    {
        if((tempVec.at(i)->taskProp == 0) && (tempVec.at(i)->m_eTaskState == Task_Get))
        {
            bTaskRedPoint = true;
            break;
        }
    }
    temp = (LYMenuItemSprite*)m_pTaskMenu->getChildByTag(MAP_BTN_TASK);
    if(bTaskRedPoint)
    {
        bTaskRedPoint = false;
        if(temp)
        {
            Sprite* point = (Sprite*)temp->getChildByTag(50);
            if(!point)
            {
                Sprite *pRedPoint = Sprite::createWithSpriteFrameName("RedPoint.png");
                temp->addChild(pRedPoint, 1, 50);
                Vec2 pos = Vec2(temp->getNormalImage()->getContentSize().width * 0.8f, temp->getNormalImage()->getContentSize().height * 0.78f);
                pRedPoint->setPosition(pos);
            }
        }
    }
    else
    {
        Sprite* sp = (Sprite*)temp->getChildByTag(50);
        if(temp && sp)
        {
            sp->removeFromParent();
        }
    }
    
    //检测主线任务
}

void CMainMenuLayer::onExit()
{
    
    if (m_pSuggesListener) {
        _eventDispatcher->removeEventListener(m_pSuggesListener);
    }
    
    _eventDispatcher->removeEventListener(m_pListener);
    NotificationCenter::getInstance()->removeObserver(this, "upgradeAfter");
    CBaseLayer::onExit();
    CPersonalData::getInstance()->setrestartPlayMusic(true);
}

void CMainMenuLayer::onEnterTransitionDidFinish()
{
    CBaseLayer::onEnterTransitionDidFinish();
    
    CFightManager::removeUnusedMemory();//清掉不用内存
    std::vector<string> messageInfo = CDataManager::getMessageInfo();
    if(messageInfo.size() > 0)//暂时这么写
    {
        m_messageInfo = messageInfo[0];
        CNetControlModule::sharedNetControlModule()->sendChargeInfo(this, menu_selector(CMainMenuLayer::getMessgageBack), m_messageInfo);
    }
}

void CMainMenuLayer::getMessgageBack(Ref* pSender)
{
    _netRecvEventGetBuyInfo* pInfo = (_netRecvEventGetBuyInfo*)pSender;
    int info = 1;
    int userStone = 0;//魔法石增量
    for(std::vector<std::string>::iterator it = pInfo->m_paramsStr.begin(); it!= pInfo->m_paramsStr.end(); it++)
    {
        string str = *it;
        string name = getStrFromBackStr(str.c_str());
        
        if(name == "info")
        {
            info = atoi(getDigitFromBackStr(it->c_str()).c_str());
        }
        else if(name == "stone")
        {
            userStone = atoi(getDigitFromBackStr(it->c_str()).c_str());
        }
    }
    if(info == 0)
    {
        CPersonalData::getInstance()->setUserStone(CPersonalData::getInstance()->getUserStone() + userStone);
        __String* tempStr1 = __String::createWithFormat("%d", userStone);
        EventParamMap paramMap;
        
        paramMap.insert(EventParamPair("购买魔法石",tempStr1->getCString()));
        
        TDCCTalkingDataGA::onEvent("充值界面", &paramMap);
        //删除成功的这一条充值信息
        CDataManager::delMeeageInfo(m_messageInfo);
    }
    else{
        log("充值更新服务器数据失败!");
    }
}

void CMainMenuLayer::onEnter()
{
    CBaseLayer::onEnter();
}

void CMainMenuLayer::update(float dt)
{
    //m_iRemainTime = m_CardInfos.m_iServerTime-m_CardInfos.m_iCoindrawtime;
    
     if(m_bGetCard&&m_CardInfos.m_iCoindrawtime==0)
     {
         if(m_pDrawCardRedPt)
         {
             m_pDrawCardRedPt->setVisible(true);
         }
     }
     else
     {
         if(m_bGetCard)
         {
             m_fTimeAll +=dt;
             if(m_fTimeAll/1>m_iDrawCnt)
             {
                 m_iDrawCnt++;
                 m_iRemainStoneTime--;
                 m_iRemainTime--;
                 
                 
                 if(m_iRemainTime<=0||m_iRemainStoneTime<=0)
                 {
                     if(m_CardInfos.m_iCoindrawNum>0)
                     {
                         m_pDrawCardRedPt->setVisible(true);
                     }
                     m_bGetCard =false;
                     
                     if(m_iRemainStoneTime<=0)
                     {
                         //CNetWorkDataSingle::GetSingletonPtr()->SaveDrawCardInfo(this,NULL,1,1);
                         char strTime2[50] = {"curTime2"};
                         UserDefault::getInstance()->setIntegerForKey(strTime2,0);
                         UserDefault::getInstance()->flush();
                     }
                     
                 }
             }
         }
     }
    bool flag = CPersonalData::getInstance()->getisOpenNewLevel();
    if(flag)
    {
        CPersonalData::getInstance()->setShowMap(false);
        CPersonalData::getInstance()->setisOpenNewLevel(false);
        LYMenuItemSprite* temp = (LYMenuItemSprite*)m_pMenu->getChildByTag(2);
        if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_ENTER_STORAGE)
           && (guiderManagerLayer::getInstance()->getStepNum() == 6))
        {
            guiderManagerLayer::getInstance()->setStepNum(0);
            if(temp && !temp->isEnabled())
            {
                m_midLevel++;//将中关卡加上去
                addMapEffect();
            }
        }
        else{
            m_midLevel++;//将中关卡加上去
            addMapEffect();
        }
    }
    
    if (CPersonalData::getInstance()->getUpdateUserInfo())
    {
        updateUsetInfo();
        CPersonalData::getInstance()->setUpdateUserInfo(false);
    }
    
    
    if(CPersonalData::getInstance()->getUpdateDrawCardInfo())
    {
        
        if(!m_pDrawCardRedPt)
        {
            m_pDrawCardRedPt = Sprite::createWithSpriteFrameName("RedPoint.png");
              m_pDrawCardItem->addChild(m_pDrawCardRedPt );
        }
        Size tDrawCard = m_pDrawCardItem->getContentSize();
        
        m_pDrawCardRedPt->setPosition(tDrawCard.width*0.9,tDrawCard.height*0.8);
      
        m_pDrawCardRedPt->setVisible(false);

        
        if(IsDrawCardRed())
        {
            CPersonalData::getInstance()->setUpdateDrawCardInfo(false);
        }

        
        
    }
    detectTask();
    std::vector<FighterInfo*> allSoliderInfo = CPersonalData::getInstance()->m_vecMyFighterInfo;
    for(int type = eFighterType_line; (type <= eFighterType_v) && (!m_bShowTeam); type++)
    {
        int num = 0;//当前同一种类型的怪物的最多数量
        for(int i = 0 ;i < allSoliderInfo.size(); i++)
        {
            monsterInfo* pMonsterInfo = CDataManager::getMonsterInfoByMonsterId(allSoliderInfo.at(i)->monsterId);
            if(pMonsterInfo && (pMonsterInfo->monsterType == (FighterType)type) && (allSoliderInfo.at(i)->theType == 0))
            {
                num++;
            }
            if(num >= 2)//横线兵种本来就不止两个，所以编队会显示
            {
                m_bShowTeam = true;
                //显示编队
                LYMenuItemSprite* tempTeam = (LYMenuItemSprite*)(p_Menu->getChildByTag(MAP_BTN_TEAM));
                if(tempTeam)
                {
                    tempTeam->setVisible(true);
                }
                break;
            }
        }
    }
    
    if(CPersonalData::getInstance()->GetPhysicalNum()>= CPersonalData::getInstance()->GetPhysicalMaxNum())
    {
        m_pStaminaMaxLabel->setPosition(Vec2(m_staminaBg->getBoundingBox().size.width * 0.98, m_staminaBg->getBoundingBox().size.height * 0.5f));
        m_pStaminaLabel->setPosition(Vec2(m_pStaminaMaxLabel->getPositionX() - m_pStaminaMaxLabel->getContentSize().width, m_staminaBg->getBoundingBox().size.height * 0.5f));
        m_pTime->setVisible(false);
    }
    else{
        m_pStaminaMaxLabel->setPosition(Vec2(m_staminaBg->getBoundingBox().size.width * 0.98, m_staminaBg->getBoundingBox().size.height * 0.63f));
        m_pStaminaLabel->setPosition(Vec2(m_pStaminaMaxLabel->getPositionX() - m_pStaminaMaxLabel->getContentSize().width, m_staminaBg->getBoundingBox().size.height * 0.63f));
        m_pTime->setVisible(true);
    }
    detectHeroUpgrade();
    detectCompound();
    
    
    if(!(CDataManager::getEndedBy(GUIDE_GET_TASK)) && CDataManager::getEndedBy(GUIDE_MAP_INTRODUCE)&& (guiderManagerLayer::getInstance()->getGuideType()!=GUIDE_GET_TASK) && (CPersonalData::getInstance()->getUserInfo()->m_iSmallLevel >= 2))
    {
        bool bExistedTask = false;//判断是否存在未领取的日常任务  防止引导异常
        std::vector<TaskBase *>tempVec = CDataManager::GetAllTaskInfo();
        for(int i = 0; i < tempVec.size(); i++)
        {//定死三星任务
            if((tempVec.at(i)->m_iTaskType == 5) && (tempVec.at(i)->m_eTaskState == Task_Get))
            {
                bExistedTask = true;
                break;
            }
        }
        if(bExistedTask)
        {
            CPersonalData::getInstance()->setisWinManual(true);
            CPersonalData::getInstance()->setisFailManual(true);
            CPersonalData::getInstance()->setisOpenNewSmallLevel(false);
            CPersonalData::getInstance()->setisOpenNewLevel(false);
            //引导士兵技能升级
            NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(CMainMenuLayer::upgradeAfter), "upgradeAfter", NULL);
            guiderManagerLayer::getInstance()->setGuideType(GUIDE_GET_TASK);
            guiderManagerLayer::getInstance()->setStepNum(1);
            Vec2 pos = Vec2(m_winSize.width * 0.059f, m_winSize.height * 0.8f);
            guiderManagerLayer::getInstance()->createLayer(this, GUIDE_SHOW_BOTH, pos);
        }
    }

    
}

void CMainMenuLayer::detectHeroUpgrade()
{
    //判断是否可以升级男女主角等级和技能
    bool bShowSkillTip = false;
    bool bShowLevelTip = false;
    if((CPersonalData::getInstance()->getUserInfo()->m_iSmallLevel >= 9) && (!CDataManager::getEndedBy(GUIDE_BOY_HERO_SKILL)))
    {
        bShowSkillTip = true;
    }
    if(CPersonalData::getInstance()->getUserInfo()->m_iSmallLevel >= 23)
    {
        if(!CDataManager::getEndedBy(GUIDE_GIRL_HERO))
        {
            bShowLevelTip = true;
        }
        if(!CDataManager::getEndedBy(GUIDE_GIRL_HERO_SKILL) && (!bShowSkillTip))
        {
            bShowSkillTip = true;
        }
    }
    
    if(bShowSkillTip)
    {
        LYMenuItemSprite* temp = (LYMenuItemSprite*)p_Menu->getChildByTag(MAP_BTN_COLLEGE);
        if(temp)
        {
            Sprite* newTip = (Sprite*)temp->getChildByTag(50);
            if(!newTip)
            {
                Sprite *pRedPoint = Sprite::createWithSpriteFrameName("newTip.png");
                temp->addChild(pRedPoint, 1, 50);
                pRedPoint->setScale(1.1f);
                Vec2 pos = Vec2(temp->getNormalImage()->getContentSize().width * 0.31f, temp->getNormalImage()->getContentSize().height * 0.77f);
                pRedPoint->setPosition(pos);
            }
        }
    }
    else{
        LYMenuItemSprite* temp = (LYMenuItemSprite*)p_Menu->getChildByTag(MAP_BTN_COLLEGE);
        if(temp)
        {
            Sprite* newTip = (Sprite*)temp->getChildByTag(50);
            if(newTip)
            {
                newTip->removeFromParent();
                newTip = NULL;
            }
        }
    }
    if(bShowLevelTip)
    {
        LYMenuItemSprite* temp = (LYMenuItemSprite*)p_Menu->getChildByTag(MAP_BTN_UPGRADE);
        if(temp)
        {
            Sprite* newTip = (Sprite*)temp->getChildByTag(50);
            if(!newTip)
            {
                Sprite *pRedPoint = Sprite::createWithSpriteFrameName("newTip.png");
                temp->addChild(pRedPoint, 1, 50);
                pRedPoint->setScale(1.1f);
                Vec2 pos = Vec2(temp->getNormalImage()->getContentSize().width * 0.31f, temp->getNormalImage()->getContentSize().height * 0.77f);
                pRedPoint->setPosition(pos);
            }
        }
    }
    else{
        LYMenuItemSprite* temp = (LYMenuItemSprite*)p_Menu->getChildByTag(MAP_BTN_UPGRADE);
        if(temp)
        {
            Sprite* newTip = (Sprite*)temp->getChildByTag(50);
            if(newTip)
            {
                newTip->removeFromParent();
                newTip = NULL;
            }
        }
    }
}

void CMainMenuLayer::guideEnterGame(float dt)
{
    //引导
    Vec2 pos = Vec2(m_map->getBoundingBox().size.width * 0.11, m_map->getBoundingBox().size.height * 0.645);
    guiderManagerLayer::getInstance()->createLayer(this, GUIDE_SHOW_FINGER, pos);
}

void CMainMenuLayer::updateTime(float dt)
{
    char buf[128] = {0};
    
    int iStamina = CPersonalData::getInstance()->GetPhysicalNum();
    UserInfo* pInfo = CPersonalData::getInstance()->getUserInfo();
    int iUpperStamin = CPersonalData::getInstance()->GetPhysicalMaxNum();
    
    if(iStamina < iUpperStamin)
    {
        m_pTime->setVisible(true);
        
        
        int tempTime = CPhysicalSystem::GetSingletonPtr()->GetNextUpTime();
        
        if (tempTime!=m_bufferIime) {
            m_bufferIime = tempTime;
            sprintf(buf, "%02d:%02d", m_bufferIime/60, m_bufferIime%60);
            m_pTime->setString(buf);
        }
        
        
        
        
        
        
        
        //        if(m_bufferIime == 0)
        //        {
        //            iStamina += 1;
        //
        //            CPersonalData::getInstance()->SetPhysicalNum(iStamina);
        //
        //            //保存体力增加的时间
        //            CPhysicalSystem::GetSingletonPtr()->SavePhysicalTime();
        //            m_bufferIime = CPhysicalSystem::GetSingletonPtr()->GetNextUpTime();
        //            CPersonalData::getInstance()->setUpdateUserInfo(true);
        //        }
        //        else
        //        {
        //            m_bufferIime--;
        //            sprintf(buf, "%ld:%ld", m_bufferIime/60, m_bufferIime%60);
        //
        //            char strKey[10] = {"timet"};
        //
        ////            UserDefault::getInstance()->setIntegerForKey(strKey,m_bufferIime);
        ////            UserDefault::getInstance()->flush();
        //
        //            m_pTime->setString(buf);
        //            CPhysicalSystem::GetSingletonPtr()->SavePhysicalTime();
        //            //m_bufferIime--;
        //        }
        
    }
    else
    {
        m_pTime->setVisible(false);
    }
    if(m_cloudTime >= 60)
    {
        m_cloudTime = 0;
        addCloud();
    }
    else{
        m_cloudTime+=dt;
    }
}

void CMainMenuLayer::addGuideInfo()
{
    
}

void CMainMenuLayer::getUserTeamInfo(Ref* pSender)
{
    if(CPersonalData::getInstance()->getState() == 0)
    {
        guiderManagerLayer::getInstance()->removeGuiderLayer();
        CPersonalData::getInstance()->setEnemyType(RANDOM_ENEMY);
        CPersonalData::getInstance()->setrestartPlayMusic(false);
        CLayerManager::GetSingletonPtr()->createLayerWithLoading(eLayer_PVPLayer, NULL,false);
        //CLayerManager::GetSingletonPtr()->createLayer(eLayer_PVPLayer, NULL,false);

    }
    else
    {
        LYMenuItemSprite* temp = (LYMenuItemSprite*)m_pTaskMenu->getChildByTag(MAP_BTN_PVP);
        if(temp)
        {
            temp->setEnabled(true);
            temp->setColor(Color3B::WHITE);
        }
    }
}

void CMainMenuLayer::btnCallBack(Ref* pSender)
{
    Sprite *p  = (Sprite *)getChildByTag(100);
    
    if(p)
    {
        p->removeFromParent();
    }
    
    
    int tag = ((Node*)pSender)->getTag();
    if(tag == MAP_BTN_ACTIVITY)
    {
        Sprite * thetarget = CFightManager::getTheFlyWord("敬请期待","fonts/cuti.ttf");
        thetarget->setPosition(Vec2(m_winSize.width / 2, m_winSize.height / 2));
        addChild(thetarget, 100);
    }
    else if(tag == MAP_BTN_PVP)
    {
        LYMenuItemSprite* temp = (LYMenuItemSprite*)m_pTaskMenu->getChildByTag(MAP_BTN_PVP);
        if(temp)
        {
            temp->setEnabled(false);
            temp->setColor(Color3B::GRAY);
        }
        CNetControlModule::sharedNetControlModule()->getUserTeamInfo(this, menu_selector(CMainMenuLayer::getUserTeamInfo));
    }
    else if(tag == MAP_BTN_SHOP)
    {
        CLayerManager::GetSingletonPtr()->createLayer(eLayer_ShopLayer,this,false);
    }
    else if(tag == MAP_BTN_CHARGE)
    {
        CLayerManager::GetSingletonPtr()->createLayer(eLayer_Charge,this,false);
    }
    else if(tag == MAP_BTN_UPGRADE)
    {
        if(CPersonalData::getInstance()->getUserInfo()->m_iSmallLevel >= 0)
        {
            
            CLayerManager::GetSingletonPtr()->createLayer(eLayer_LvUp,this,false);
        }
        else
        {
            Label* pLabel = Label::createWithSystemFont("完成第2关后开启", "Arial", 32);
            addChild(pLabel);
            pLabel->setPosition(Vec2(m_winSize.width / 2, m_winSize.height / 2));
            ActionInterval* fade = FadeOut::create(0.6);
            pLabel->runAction(Sequence::create(MoveBy::create(1.2f, Point(0, 200)), fade, RemoveSelf::create(), NULL));
        }
    }
    
    else if(tag == MAP_BTN_TASK)
    {
        CLayerManager::GetSingletonPtr()->createLayer(eLayer_TaskLayer,this,false);
        //talkingData
        __String* tempStr = __String::createWithFormat("1");
        EventParamMap paramMap;
        
        paramMap.insert(EventParamPair("点击次数",tempStr->getCString()));
        
        TDCCTalkingDataGA::onEvent("任务界面", &paramMap);
    }
    else if(tag == COIN_CHARGE_TAG)
    {
        CommonPropLayer* pLayer = CommonPropLayer::create(STONE_EXCHANGE_COIN);
        addChild(pLayer, 999);
        
        CPersonalData::getInstance()->setUserCoin(99990);
        
        CNetWorkDataSingle::GetSingletonPtr()->SaveUserMCF(NULL,NULL,9999999,0,0);
         updateUsetInfo();
        
        //talkingData
        __String* tempStr = __String::createWithFormat("1");
        EventParamMap paramMap;
        
        paramMap.insert(EventParamPair("点击次数",tempStr->getCString()));
        
        TDCCTalkingDataGA::onEvent("充金币", &paramMap);
        
    }
    else if(tag == STONE_CHARGE_TAG)
    {
        //CLayerManager::GetSingletonPtr()->createLayer(eLayer_Charge,this,false);
        //talkingData
        
        CPersonalData::getInstance()->setUserStone(99999);
        
       CNetWorkDataSingle::GetSingletonPtr()->SaveUserMCF(NULL,NULL,0,99999,0);
        updateUsetInfo();
        
        __String* tempStr = __String::createWithFormat("1");
        EventParamMap paramMap;
        
        paramMap.insert(EventParamPair("点击次数",tempStr->getCString()));
        
        TDCCTalkingDataGA::onEvent("充宝石", &paramMap);
        
    }
    else if(tag == STAMINA_CHARGE_TAG)
    {
        CommonPropLayer* pLayer = CommonPropLayer::create(STONE_EXCHANGE_STAMINA);
        addChild(pLayer, 999);
        
        
        //CPersonalData::getInstance()->SetPhysicalNum(10000);
        //updateUsetInfo();
        
        //talkingData
        __String* tempStr = __String::createWithFormat("1");
        EventParamMap paramMap;
        
        paramMap.insert(EventParamPair("点击次数",tempStr->getCString()));
        
        TDCCTalkingDataGA::onEvent("充体力", &paramMap);
        
    }
    else if(tag == MAP_BTN_SET)
    {
        CLayerManager::GetSingletonPtr()->createLayer(eLayer_SetLayer,this,false);
    }
    else if(tag == MAP_BTN_DAILY)
    {
        if((guiderManagerLayer::getInstance()->getStepNum() == 3) && (guiderManagerLayer::getInstance()->getGuideType() == GUIDE_LACK_STAMINA))
        {
            guiderManagerLayer::getInstance()->removeGuiderLayer();
        }
        //请求服务器时间
        CNetControlModule::sharedNetControlModule()->getServerTime(this, menu_selector(CMainMenuLayer::getSerTime));
    }
    else if(tag == MAP_BTN_STORAGE)
    {
        CLayerManager::GetSingletonPtr()->createLayer(eLayer_PropLayer,this,false);
        
        //talkingData
        __String* tempStr = __String::createWithFormat("1");
        EventParamMap paramMap;
        
        paramMap.insert(EventParamPair("点击次数",tempStr->getCString()));
        
        TDCCTalkingDataGA::onEvent("仓库界面", &paramMap);
    }
    else if(tag == MAP_BTN_COLLEGE)
    {
        if(CPersonalData::getInstance()->getUserInfo()->m_iSmallLevel >= 0)
        {
            m_bSkillStart = false;
            CLayerManager::GetSingletonPtr()->createLayer(eLayer_LvSkill,this,false);
            
        }
        else{
            Label* pLabel = Label::createWithSystemFont("完成第4关后开启", "Arial", 32);
            addChild(pLabel);
            pLabel->setPosition(Vec2(m_winSize.width / 2, m_winSize.height / 2));
            ActionInterval* fade = FadeOut::create(0.6);
            pLabel->runAction(Sequence::create(MoveBy::create(1.2f, Point(0, 200)), fade, RemoveSelf::create(), NULL));
        }
        
        //talkingData
        __String* tempStr = __String::createWithFormat("1");
        EventParamMap paramMap;
        
        paramMap.insert(EventParamPair("点击次数",tempStr->getCString()));
        
        TDCCTalkingDataGA::onEvent("技能界面", &paramMap);
    }
    else if(tag == MAP_BTN_HERO)
    {
        if(CPersonalData::getInstance()->getUserInfo()->m_iSmallLevel >= 0)
        {
            CLayerManager::GetSingletonPtr()->createLayer(eLayer_Compound,this,false);
        }
        else{
            Label* pLabel = Label::createWithSystemFont("完成20关后开启", "Arial", 32);
            addChild(pLabel);
            pLabel->setPosition(Vec2(m_winSize.width / 2, m_winSize.height / 2));
            ActionInterval* fade = FadeOut::create(0.6);
            pLabel->runAction(Sequence::create(MoveBy::create(1.2f, Point(0, 200)), fade, RemoveSelf::create(), NULL));
        }
        
        //talkingData
        __String* tempStr = __String::createWithFormat("1");
        EventParamMap paramMap;
        
        paramMap.insert(EventParamPair("点击次数",tempStr->getCString()));
        
        TDCCTalkingDataGA::onEvent("英雄界面", &paramMap);
    }
    else if(tag == MAP_BTN_TEAM)
    {
        if(CPersonalData::getInstance()->getUserInfo()->m_iSmallLevel >= 0)
        {
            CLayerManager::GetSingletonPtr()->createLayer(eLayer_TeamLayer,this,false);
        }
        else{
            Label* pLabel = Label::createWithSystemFont("完成20关后开启", "Arial", 32);
            addChild(pLabel);
            pLabel->setPosition(Vec2(m_winSize.width / 2, m_winSize.height / 2));
            ActionInterval* fade = FadeOut::create(0.6);
            pLabel->runAction(Sequence::create(MoveBy::create(1.2f, Point(0, 200)), fade, RemoveSelf::create(), NULL));
        }
        
        //talkingData
        __String* tempStr = __String::createWithFormat("1");
        EventParamMap paramMap;
        
        paramMap.insert(EventParamPair("点击次数",tempStr->getCString()));
        
        TDCCTalkingDataGA::onEvent("编队界面", &paramMap);
    }
    else if(tag == MAP_BTN_DRAWCARD)
    {
    
        
        CLayerManager::GetSingletonPtr()->createLayer(eLayer_DrawCardLayer,this,false);
        
        __String* tempStr = __String::createWithFormat("1");
        EventParamMap paramMap;
        
        paramMap.insert(EventParamPair("点击次数",tempStr->getCString()));
        
        TDCCTalkingDataGA::onEvent("抽卡界面", &paramMap);
        
    }
    else{
        CFightData::getInstance()->setNowMidLevelId(tag);//保存当前选中的大关卡
        enterLevelInfo(tag);
    }
}

std::string CMainMenuLayer::getStrFromBackStr(const char* str)
{
    const char* p = str;
    string ret;
    while(*p != '=')
    {
        ret += *p;
        p++;
    }
    return ret;
}

std::string CMainMenuLayer::getDigitFromBackStr(const char* str)
{
    const char* p = str;
    string ret;
    while (*p != '=')
    {
        p++;
    }
    p++;
    while(*p != '\0')
    {
        ret += *p;
        p++;
    }
    return ret;
}

void CMainMenuLayer::getSerTime(Ref* pSender)
{
    _netRecvEventGetSevTime* pInfo = (_netRecvEventGetSevTime*)pSender;
    chargeInfo spInfo;
    int state = 1;
    long time;
    for(std::vector<std::string>::iterator it = pInfo->m_paramsStr.begin(); it!= pInfo->m_paramsStr.end(); it++)
    {
        string str = *it;
        string name = getStrFromBackStr(str.c_str());
        if (name == "state")
        {
            state = atoi(getDigitFromBackStr(it->c_str()).c_str());
            if(state != 0)
            {
                log("获取服务器时间失败");
                return;
            }
        }
        else if(name == "getServerTime")
        {
            time = atol(getDigitFromBackStr(it->c_str()).c_str())/1000;
        }
    }
    CPersonalData::getInstance()->detectTaskRefresh(time);
    CLayerManager::GetSingletonPtr()->createLayer(eLayer_DailyTaskLayer,this,false);
    
    //talkingData
    __String* tempStr = __String::createWithFormat("1");
    EventParamMap paramMap;
    
    paramMap.insert(EventParamPair("点击次数",tempStr->getCString()));
    
    TDCCTalkingDataGA::onEvent("日常任务", &paramMap);
    
}

void CMainMenuLayer::enterLevelInfo(int level)
{
    CLayerManager::GetSingletonPtr()->createLayer(eLayer_LevelInfo, this,false);
}

void CMainMenuLayer::callBackSugges(cocos2d::Ref *pSender){
    
    if (m_pSuggestionBG) {
        return;
    }
    
    
    m_pSuggestionBG = Sprite::createWithSpriteFrameName("dialog.png");
    addChild(m_pSuggestionBG,650);
    m_pSuggestionBG->setPosition(Vec2(m_winSize.width*0.5f, m_winSize.height*0.5f));
    
    
    m_pSuggestionShadow = LayerColor::create(Color4B(0, 0, 0, 200));
    //    tempShadow->setPosition(Vec2(m_pSuggestionBG->getBoundingBox().size.width*0.5f, m_pSuggestionBG->getBoundingBox().size.height*0.5f)-m_pSuggestionBG->getPosition());//把坐标转换
    addChild(m_pSuggestionShadow,600);
    
    
    
    Sprite* tempGirl = Sprite::createWithSpriteFrameName("suggestion_girl.png");
    tempGirl->setScale(2);
    m_pSuggestionBG->addChild(tempGirl,10);
    tempGirl->setPosition(Vec2(m_pSuggestionBG->getBoundingBox().size.width+tempGirl->getBoundingBox().size.width*0.3f, m_pSuggestionBG->getBoundingBox().size.height*0.5f));
    
    Sprite* GirlDialog = Sprite::createWithSpriteFrameName("suggestion_dialog.png");
    GirlDialog->setScale(2.5f);
    m_pSuggestionBG->addChild(GirlDialog,10);
    GirlDialog->setPosition(Vec2(m_pSuggestionBG->getBoundingBox().size.width*0.55f, m_pSuggestionBG->getBoundingBox().size.height+GirlDialog->getBoundingBox().size.height*0.5f));
    
    Size labelSize = Size(GirlDialog->getBoundingBox().size.width*0.9f, GirlDialog->getBoundingBox().size.height*0.9f);
    Label* girlLabel = Label::createWithTTF("首次留言可以获得100魔法石的奖励哦~如果您的意见被我们采纳的话，还有机会获得额外大奖~", "fonts/cuti.ttf", 30,  labelSize);
    m_pSuggestionBG->addChild(girlLabel,11);
    girlLabel->setPosition(GirlDialog->getPosition());
    
    
    Sprite* sale = Sprite::createWithSpriteFrameName("icon_give.png");
    sale->setScale(1.0/2.7);
    sale->setPosition(GirlDialog->getBoundingBox().size.width*0.14f,GirlDialog->getBoundingBox().size.height*0.17f);
    GirlDialog->addChild(sale,12);
    
    Sprite* stone = Sprite::createWithSpriteFrameName("stone.png");
    stone->setScale(1.0/3.8);
    stone->setPosition(sale->getPosition()+Vec2(sale->getBoundingBox().size.width*0.4f+stone->getBoundingBox().size.width*0.5f,-stone->getBoundingBox().size.height*0.15f));
    GirlDialog->addChild(stone,13);
    
    Label* coinNum = Label::createWithCharMap("fonts/Damage_num.png", 24, 36, '0');
    coinNum->setScale(1.0/2.7);
    coinNum->setString("100");
    coinNum->setPosition(stone->getPosition()+Vec2(stone->getBoundingBox().size.width*0.5f+coinNum->getBoundingBox().size.width*0.5f,0));
    GirlDialog->addChild(coinNum,13);
    
    
    
    Sprite* confirmNormal = Sprite::createWithSpriteFrameName("suggestion_sendNormal.png");
    Sprite* confirmDown = Sprite::createWithSpriteFrameName("suggestion_sendDown.png");
    LYMenuItemSprite* confirmButton = LYMenuItemSprite::create(confirmNormal, confirmDown, CC_CALLBACK_1(CMainMenuLayer::onBtnSuggesConfirm, this));
    confirmButton->setPosition(Vec2(m_pSuggestionBG->getBoundingBox().size.width*0.6f,m_pSuggestionBG->getBoundingBox().size.height*0.2f));
    
    
    Sprite* cancelNormal = Sprite::createWithSpriteFrameName("suggestion_CancelNormal.png");
    Sprite* cancelDown = Sprite::createWithSpriteFrameName("suggestion_CancelDown.png");
    LYMenuItemSprite* cancelButton = LYMenuItemSprite::create(cancelNormal, cancelDown, CC_CALLBACK_1(CMainMenuLayer::onBtnSuggesCancel, this));
    cancelButton->setPosition(Vec2(m_pSuggestionBG->getBoundingBox().size.width*0.8f,m_pSuggestionBG->getBoundingBox().size.height*0.2f));
    
    Menu* targetMenu = Menu::create(confirmButton,cancelButton, NULL);
    targetMenu->setPosition(Vec2::ZERO);
    m_pSuggestionBG->addChild(targetMenu,11);
    
    
    m_pSuggesListener = EventListenerTouchOneByOne::create();
    m_pSuggesListener->setSwallowTouches(true);
    
    m_pSuggesListener->onTouchBegan = CC_CALLBACK_2(CMainMenuLayer::onSuggesTouch, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(m_pSuggesListener, m_pSuggestionBG);
    
    
    Size InputSize = m_pSuggestionBG->boundingBox().size;
    InputSize.width -= 55;
    SuggestionInputText = NULL;
    SuggestionInputText = TextFieldTTF::textFieldWithPlaceHolder("", InputSize , TextHAlignment::LEFT ,"Arial", 35);
    SuggestionInputText->setPosition(Vec2(m_pSuggestionBG->getBoundingBox().size.width*0.51f,m_pSuggestionBG->getBoundingBox().size.height*0.43f));
    SuggestionInputText->setAlignment(TextHAlignment::LEFT, TextVAlignment::TOP);
    SuggestionInputText->setColor(Color3B(104, 43, 43));
    m_pSuggestionBG->addChild(SuggestionInputText);
    SuggestionInputText->setDelegate(this);
    //    SuggestionInputText->attachWithIME();
    
    
    
}

bool CMainMenuLayer::onSuggesTouch(cocos2d::Touch *pTouch, cocos2d::Event *pEvent){
    
    
    Vec2 Pos = pTouch->getLocation();
    
    if(m_pSuggestionBG->getBoundingBox().containsPoint(Pos)){
        
        SuggestionInputText->attachWithIME();
        
        return true;
    }
    
    SuggestionInputText->detachWithIME();
    
    return true;
}

void CMainMenuLayer::onBtnSuggesCancel(cocos2d::Ref *pSender){
    
    if (SuggestionInputText) {
        SuggestionInputText->detachWithIME();
    }
    
    
    if (m_pSuggesListener) {
        _eventDispatcher->removeEventListener(m_pSuggesListener);
        m_pSuggesListener = NULL;
    }
    
    
    
    
    m_pSuggestionBG->removeFromParentAndCleanup(true);
    m_pSuggestionBG = NULL;
    m_pSuggestionShadow->removeFromParentAndCleanup(true);
    
    
}


void CMainMenuLayer::OnSuggestion(Ref *pSender)
{
    CPersonalData::getInstance()->setUpdateUserInfo(true);
    
    
    Sprite* temp =  CFightManager::getRewardWord("您获得了100魔法石");
    temp->setPosition(Vec2(m_winSize.width*0.5f, m_winSize.height*0.5f));
    addChild(temp,10001);
}

void CMainMenuLayer::onBtnSuggesConfirm(cocos2d::Ref *pSender){
    
    //如果内容为空，则调用键盘
    if(m_sSuggestionText.length()==0){
        
        SuggestionInputText->attachWithIME();
        
        Node* temp =  CFightManager::getTheFlyWord("请输入您的留言~");
        temp->setPosition(m_winSize.width*0.5f, m_winSize.height*0.5f);
        addChild(temp,10000);
        return;
    }
    
    if (SuggestionInputText) {
        SuggestionInputText->detachWithIME();
    }
    
    if (m_pSuggesListener) {
        _eventDispatcher->removeEventListener(m_pSuggesListener);
        m_pSuggesListener = NULL;
    }
    
    
    m_pSuggestionBG->removeFromParentAndCleanup(true);
    m_pSuggestionBG = NULL;
    m_pSuggestionShadow->removeFromParentAndCleanup(true);
    
    
    
    if (m_sSuggestionText.length()>0) { //意见大于0才提交
        CNetControlModule::sharedNetControlModule()->uploadSuggestion(NULL, NULL, m_sSuggestionText);
        m_sSuggestionText="";
    }
    
    
    UserInfo * target = CPersonalData::getInstance()->getUserInfo();
    if(target->ifSuggesed==0){
        
        target->ifSuggesed = 1;
        CDBMgr::getInstance()->setUserIfSuggessed();
        
        //        int num = CPersonalData::getInstance()->getUserStone();
        //        num+=100;
        //        CPersonalData::getInstance()->setUserStone(num);
        
        //CDataManager::saveUserStoneAndCoinToDB(100,menu_selector(CLevelInfoLayer::OnUpdateStone),0,awardVec.at(receiveNum).num, 0);
        
        
        
        // CDataManager::saveUserStoneAndCoinToDB(100,1,1);
        
        
        CNetWorkDataSingle::GetSingletonPtr()->SaveUserMCF(this, menu_selector(CMainMenuLayer::OnSuggestion), 0, 100,0);
        TDCCVirtualCurrency::onReward(100, "游戏建议");
        //updateUsetInfo();
        
    }
    
    
    
}

//MARK: 输入回调
bool CMainMenuLayer::onTextFieldAttachWithIME(TextFieldTTF * sender)
{
    
    CCLOG("启动输入");
    
    m_pSuggestionBG->setPosition(Vec2(m_winSize.width*0.5f, m_winSize.height*0.75f));
    
    
    return false;
    //    return true;(不启动)
}
//    当用户关闭虚拟键盘的时候回调函数
bool CMainMenuLayer::onTextFieldDetachWithIME(TextFieldTTF * sender)
{
    
    CCLOG("关闭输入");
    
    m_pSuggestionBG->setPosition(Vec2(m_winSize.width*0.5f, m_winSize.height*0.5f));
    
    return false;
    //    return true;(不关闭)
}
//    当用户进行输入 虚拟键盘的时候回调函数
bool  CMainMenuLayer::onTextFieldInsertText(TextFieldTTF * sender, const char * text, size_t nLen)
{
    CCLOG("输入字符");
    std::string str = sender->getString();
    __String *string = __String::createWithFormat("%s%s",str.c_str(),text);
    CCLOG("%d",string->length());
    
    if (string->length()>120)
    {
        SuggestionInputText->detachWithIME();
        return true;
    }
    
    m_sSuggestionText = sender->getString();
    //sender->setString(string->getCString());
    return false;
    //    return true;(不输入)
}
//    当用户进行删除文字 虚拟键盘的时候回调函数
bool CMainMenuLayer::onTextFieldDeleteBackward(TextFieldTTF * sender, const char * delText, size_t nLen)
{
    CCLOG("删除字符");
    return false;
    //    return true;(不删除)
}