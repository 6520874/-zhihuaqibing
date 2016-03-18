    //
//  fightScene.cpp
//  TheGodWill
//
//  Created by 刘勇 on 14-11-25.
//
//

#include "fightScene.h"

#include "SmallMap.h"
#include "PaoWuxian.h"
#include "Common.h"
#include "LYFightManager.h"
#include "LYBaseResourceMine.h"
#include "LYBaseTower.h"
#include "commonPropLayer.h"
#include "LYFightData.h"
#include "LYBaseMonsterMine.h"
#include "CSharedPlist.h"
#include "LYBaseWall.h"

#include "LYBaseLifeMine.h"

#include "LYBaseProtector.h"
#include "getNewHeroLayer.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "MicBlowViewController.h"
#endif

#include "LYBaseCity.h"

#include "BaseLayer.h"

#include "LYBaseHero.h"

#include "DataManager.h"


#include "renderer/CCRenderer.h"
#include "renderer/CCCustomCommand.h"
std::string templateName[]={"Arrow","Caret","Pigtail","QuestionMark","Star","Spiral"};


#include "FightWinLayer.h"
#include "FightFailLayer.h"

#include "guiderManagerLayer.h"
#include "storyManagerLayer.h"


#include "LYBaseTrap.h"
#include "zzbNetControlModule.h"
#include "zzbNewNetStruct.h"

#define ADD_SOLIDER_HEIGHT 240//创建士兵的y轴高度


#include "LYBaseWalker.h"
#include "NetWorkDataSingle.h"
#include "DBMgr.h"
#include "pvpWarReportLayer.h"
#include "PhysicalSystem.h"
#include "thePartolFight.h"
#include "CNpcFighter.h"
#include "sysTime.h"
#include "pvpCommonInfoLayer.h"

USING_NS_CC;

#define BLOW_CAMERA_ANGEL 110
#define MY_PERSION CPersonalData::getInstance()

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
static MicBlowViewController *micBlow=NULL;
#endif


//测试用
static Animation* testAnimate = NULL;

Scene* CFightScene::createScene(){
    
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = CFightScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}


CFightScene::CFightScene()
{
    
    m_eFightSceneType = eFightSceneType_Normal;
    
    m_pXian =  NULL;
    m_pCurView = NULL;
    m_blowLabel = NULL;
    m_cutLabel = NULL;
    m_pBlowMenu = NULL;
    m_pCutMenu = NULL;
    
    m_bJishiStart = false;
    
    CFightData::getInstance()->setIsPvpStart(0);
    
    
    m_bPvpAllMapMove = true;
    
    m_ptaskBg = NULL;
    
    m_pTaskLable = NULL;
    
    g_rGemertricRecognizer = NULL;
    
    m_touchStar = NULL;
    
    m_layer = NULL;
    
    m_pToushiActive = NULL;
    
    m_pSwitchItem1 = NULL;
    
    m_pSwitchItem2 = NULL;
    
    m_pAmbientLight = NULL;
    
    iDle=NULL;
    attack1 = NULL;
    hurt = NULL;
    walk = NULL;
    
    m_pTarget = NULL;
    layer3D = NULL;
    m_pParticle = NULL;
    
    m_combo = NULL;
    m_chop = NULL;
    m_blood = NULL;
    
    m_pTouMiaozhunItem = NULL;
    m_pTouFasheItem = NULL;
    m_guiderArrow = NULL;
    m_GuideNode = NULL;
    m_pLaGan = NULL;
    m_pLanGanDian = NULL;
    //m_pMenuTouShiPao = NULL;
    m_pMenuTouShiMove = NULL;
    m_pvpCloseMenu = NULL;
    
    m_paoWuLayer = NULL;
    m_pCurViewItem = NULL;
    m_pMoveRange = NULL;
    m_pTheEnergyBar = NULL;
    m_pSoldierNum = NULL;
    
    theBombTime = NULL;
    
    m_pPreparedBuilding = NULL;
    
    m_pBuildingControll = NULL;
    
    m_pTheClock = NULL;
    
    m_pClockButton = NULL;
    
    m_pStoneLable = NULL;
    
    m_pLifeLable = NULL;
    
    m_pTrunLable = NULL;
    
    m_pListener = NULL;
    
    
    m_skillTime = 0;
    m_iToushiCDTime = 60;
    m_pToushiActive = NULL;
    m_bToushiMove = false;
    m_skill = NULL;
    m_pLaGan = NULL;
    m_pTouMiaozhunItem = NULL;
    m_pTouFasheItem= NULL;
    m_bMoveToushiche = false;
    m_bToushicheStart = false;
    m_pMoveRange = NULL;
    m_pSoldierNum = NULL;
    m_pTheEnergyBar = NULL;
    m_pTheClock = NULL;
    m_pStoneLable = NULL;
    m_pLifeLable = NULL;
    m_pTrunLable = NULL;
    m_bToushiActive = false;
    
    m_bFightPvpStart = false;
    
    m_pFightItem = NULL;
    m_searchItem = NULL;
    
    m_pBackItem = NULL;
    m_pAllStone = NULL;
    m_progress = NULL;
    
    m_pEnemyBg = NULL;
    m_iKnifeNum = 0;
    m_iKnifeMax = 1;
    
    m_pLableTime = NULL;
    
    m_iAllTime = 120;
    CFightData::getInstance()->setThePvpTime(m_iAllTime);
    
    CFightData::getInstance()->setThePvpAllTime(m_iAllTime);
    
    m_iRecEndTime = 15;
    
    m_iHour = 0;
    m_iSec = 0;
    
    m_iMin = 0;
    
    m_iMyTime = 0;
    m_iCnt = 0;
    
    m_bHuitan= false;
    
    m_allMarkSolider.clear();
    
    m_ePVPMode=EPVPMode_NOPVP;
    
    m_strBGEffect="";
    
    m_iTouchExitState = 0;
    
    _accelerationDiedY=0;
    
    m_pBossHpProcess = NULL;
    
    m_pWhiteBossBg = NULL;
    
    m_pBackMenu = NULL;
    
    m_pNpc1 = NULL;
    m_pNpc2 = NULL;
    m_pNpc3 = NULL;

    CFightData::getInstance()->setNowMonsterNum(0);
    m_vec2BuildPos = Vec2::ZERO;
    
    
    //纹理在loading界面加载过一次了，就不用再加载了
    
    if(CLayerManager::GetSingletonPtr()->getCurLayerType() != eLayer_Loading)
    {
        CSharedPlist::getInstance()->pushPlist("plist/shape.plist");
        CSharedPlist::getInstance()->pushPlist("plist/towerDefense.plist");
        CSharedPlist::getInstance()->pushPlist("plist/guide_1.plist");
        CSharedPlist::getInstance()->pushPlist("plist/guide_2.plist");
        CSharedPlist::getInstance()->pushPlist("plist/fight.plist");
        CSharedPlist::getInstance()->pushPlist("plist/fightAnimation.plist");
        CSharedPlist::getInstance()->pushPlist("plist/effect.plist");
        CSharedPlist::getInstance()->pushPlist("plist/skill.plist");
        
        CSharedPlist::getInstance()->pushPlist("plist/fightTalkIcon.plist");
        CSharedPlist::getInstance()->pushPlist("plist/skillEffect.plist");
    }
}

CFightScene::~CFightScene()
{
    delete g_rGemertricRecognizer;
    NotificationCenter::getInstance()->removeObserver(this, "StartBlow");
    NotificationCenter::getInstance()->removeObserver(this, "StartCut");
    NotificationCenter::getInstance()->removeObserver(this, "guideCut");
    NotificationCenter::getInstance()->removeObserver(this, "guideBlow");
    CSharedPlist::getInstance()->erasePlist("plist/shape.plist");
    CSharedPlist::getInstance()->erasePlist("plist/towerDefense.plist");
    CSharedPlist::getInstance()->erasePlist("plist/guide_1.plist");
    CSharedPlist::getInstance()->erasePlist("plist/guide_2.plist");
    CSharedPlist::getInstance()->erasePlist("plist/fight.plist");
    CSharedPlist::getInstance()->erasePlist("plist/fightAnimation.plist");
    CSharedPlist::getInstance()->erasePlist("plist/effect.plist");
    CSharedPlist::getInstance()->erasePlist("plist/skill.plist");
    
    
    CSharedPlist::getInstance()->erasePlist("plist/fightTalkIcon.plist");
    
    CSharedPlist::getInstance()->erasePlist("plist/skillEffect.plist");
}

void CFightScene::onExit(){
    
    
    _eventDispatcher->removeEventListener(m_pListener);
    
    CBaseLayer::onExit();
    
    Director::getInstance()->getScheduler()->setTimeScale(1);
    
    NotificationCenter::getInstance()->removeObserver(this, "StartBlow");
    NotificationCenter::getInstance()->removeObserver(this, "StartCut");
    NotificationCenter::getInstance()->removeObserver(this, "guideCut");
    NotificationCenter::getInstance()->removeObserver(this, "guideBlow");
    NotificationCenter::getInstance()->removeObserver(this, "gameData");
    NotificationCenter::getInstance()->removeObserver(this, "fightResult");
    NotificationCenter::getInstance()->removeObserver(this, "removeGuide");
    NotificationCenter::getInstance()->removeObserver(this, "damageGuide");
    //将自己放置到fightdata中,便于获取
    CFightData::getInstance()->setTheFightScene(NULL);
    
}

void CFightScene::initData(){
    
    attackerY = 0;
    
    //    CFightData::getInstance()->setaddEnergy(10);
    
    //    CFightData::getInstance()->setAddEnergy_Enemy(1);
    
    m_skillTime = 0;
    //m_offSet = 0;
    //m_MoveTime = 0;
    m_skill = NULL;
    m_isFlying = false;
    m_volume = 0;
    m_isTouch = false;
    m_bTouched = false;
    m_Index = 0;
    m_obb.clear();
    _intersetList.clear();
    m_hurtEnemyVec.clear();
    m_combo = NULL;
    m_comboNum = 0;
    m_directX = 0;
    m_directY = 0;
    m_adjustAngle = 0;
    m_showCombo = 0;
    m_fshowComboInteval = 0;
    m_isblow = false;
    m_bifBlowEnd = false;
    m_chop = NULL;
    m_blood = NULL;
    m_layer = NULL;
    //m_isDraw = true;
    m_opcity = 250;
    m_isValid = true;
    m_touchStar = NULL;
    m_bIsshowResult = false;
    yHeight = 0;
    m_isShowBtn = false;
    theBombTime = &CFightData::getInstance()->getTheLevelInfo()->theBombInfo;
    
    m_bSmallMapMoveStart = false;
    m_bifNorAddSolider = false;
    
    m_iNextBuildingId = 0;
    m_bIfBuildingTower = false;
    m_pPreparedBuilding = NULL;
    
    //m_bLaganStart = false;
    //计算最需要能量的怪物的能量值
    std::vector<FighterInfo>* targetVec = CFightData::getInstance()->getTheMonsterInfo();
    
    theMostEnemyCost=0;
    m_soundId = 0;
    for (std::vector<FighterInfo>::iterator ite = targetVec->begin(); ite!=targetVec->end(); ite++) {
        if ((*ite).energyCost>theMostEnemyCost) {
            theMostEnemyCost =(*ite).energyCost;
        }
    }
    m_time = 0;
    m_HeroSkill.clear();
    blowCD = 0;
    cutCD = 0;
    m_bValid = false;
    m_blowLabel = NULL;
    m_cutLabel = NULL;
    m_pBlowMenu = NULL;
    m_pCutMenu = NULL;
    m_blowTime = 0;
    m_cutTime = 0;
    m_pSwitchItem1 = NULL;
    m_pSwitchItem2 = NULL;
    monsterNum = 0;
    m_dtTime = 0;
    CFightData::getInstance()->setAddCoin(0);
    //初始化游戏中的数据
    CFightData::getInstance()->m_starInfoVec.clear();
    m_ptaskBg = NULL;
    m_bTouchStar = false;
    m_starVec.clear();
    m_bGuideBlow = false;
    m_bTouchMap = false;
    /*
     塔防模式相关初始化
     */
    
    m_fNowWaveTime = TD_WAVE_TIME;
    m_soliderNum = 0;
    m_guiderArrow = NULL;
    m_GuideNode = NULL;
    bArrow = true;
    m_bSmallMapAutoMove = true;
    m_bEndGuide = false;
    m_bCutGuide = false;
    m_storyStartTime = -1;
    m_bHaveStory = false;
    m_bStartStory = false;
    m_bBlowSolider = false;
    m_pauseMenu = NULL;
    m_pRmvItem = NULL;
    m_addEnergy = 0;
    m_rmvMenuItem = NULL;
    m_pRmvBackItem = NULL;
    m_bRmvFlag = false;
    m_bUpdateTime = true;
    m_bPvpMove = false;
    m_bMenuValid = false;
    char buf[257];
    Animation* pTaskAnimation = Animation::create();
    for (int i = 1; i < 8; i++)
    {
        sprintf(buf, "task_%d.png", i);
        pTaskAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(buf));
    }
    pTaskAnimation->setDelayPerUnit(0.1f);
    AnimationCache::getInstance()->addAnimation(pTaskAnimation, "taskAnimation");
    
    CFightData::getInstance()->setBornPath(END3DPOINT);
    CFightData::getInstance()->setClearSolider(false);
    
    //talkingdata
    int nowMidLevel = CFightData::getInstance()->getNowLevelId();
    int nowSmallLevel = CFightData::getInstance()->getNowMidLevelId();
    
    __String* tempStr = __String::createWithFormat("%d-%d",nowMidLevel,nowSmallLevel);
    TDCCMission::onBegin(tempStr->getCString());
    
    CFightData::getInstance()->setStopAddEnemy(false);
}


void CFightScene::closeClound()
{
    
      m_pBackMenu->setEnabled(true);
    
        m_searchItem->setEnabled(true);
        
        m_pFightItem->setEnabled(true);
        
        if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_PVP_ROB) && (guiderManagerLayer::getInstance()->getStepNum() == 3))
        {
            guiderManagerLayer::getInstance()->setStepNum(guiderManagerLayer::getInstance()->getStepNum() + 1);
            guiderManagerLayer::getInstance()->createLayer(this);
        }
   
}


void CFightScene::startSearchEnemy()
{
    
    runAction(Sequence::create(CallFunc::create([=]{
        
        
        
    }),DelayTime::create(1.5),CallFunc::create([=]{
        
        if(m_pLayer)
        {
            Sprite *pCloud1=  (Sprite*)m_pLayer->getChildByTag(ePvp_Cloud1Tag);
            Sprite *pCloud2=  (Sprite*)m_pLayer->getChildByTag(ePvp_Cloud2Tag);
            Sprite *pCloud3=  (Sprite*)m_pLayer->getChildByTag(ePvp_Cloud3Tag);
            Sprite *pCloud4=  (Sprite*)m_pLayer->getChildByTag(ePvp_Cloud4Tag);
            Sprite *pCloud5=  (Sprite*)m_pLayer->getChildByTag(ePvp_Cloud5Tag);
            Sprite *pCloud6=  (Sprite*)m_pLayer->getChildByTag(ePvp_Cloud6Tag);
            
            
            float fTime1 = 2;
            
            
            pCloud1->runAction(Sequence::create(DelayTime::create(fTime1*0.2),Spawn::create(EaseSineOut::create(MoveTo::create(fTime1*0.8,Vec2(-pCloud1->getBoundingBox().size.width*0.5,pCloud1->getBoundingBox().size.height*0.5))),FadeTo::create(fTime1*0.8,255*0.5),NULL),NULL));
            
            pCloud3->runAction(Sequence::create(DelayTime::create(fTime1*0.3),Spawn::create(EaseSineOut::create(MoveTo::create(fTime1/2,Vec2(-pCloud3->getBoundingBox().size.width*0.5,pCloud3->getBoundingBox().size.height*0.5))),FadeTo::create(fTime1/2,255*0.7),NULL),NULL));
            
            pCloud5->runAction(Sequence::create(Spawn::create(EaseSineOut::create(MoveTo::create(fTime1*3/4,Vec2(-pCloud5->getBoundingBox().size.width*0.5,pCloud5->getBoundingBox().size.height*0.5))),FadeTo::create(fTime1/4,255*0.9),NULL),NULL));
            
            
            pCloud2->runAction(Sequence::create(DelayTime::create(fTime1*0.2),Spawn::create(EaseSineOut::create(MoveTo::create(fTime1*0.8,Vec2(m_WinSize.width+pCloud2->getBoundingBox().size.width*0.5,m_WinSize.height-pCloud2->getBoundingBox().size.height*0.5))),FadeTo::create(fTime1*0.8,255*0.5),NULL),NULL));
            
            pCloud4->runAction(Sequence::create(DelayTime::create(fTime1*0.3),Spawn::create(EaseSineOut::create(MoveTo::create(fTime1/2,Vec2(m_WinSize.width+pCloud4->getBoundingBox().size.width*0.5,m_WinSize.height- pCloud4->getBoundingBox().size.height*0.5))),FadeTo::create(fTime1/2,255*0.7),NULL),NULL));
            
            pCloud6->runAction(Sequence::create(Spawn::create(EaseSineOut::create(MoveTo::create(fTime1*3/4,Vec2(m_WinSize.width+pCloud6->getBoundingBox().size.width*0.5,m_WinSize.height-pCloud6->getBoundingBox().size.height*0.5))),FadeTo::create(fTime1/4,255*0.9),NULL),CallFunc::create(CC_CALLBACK_0(CFightScene::closeClound, this)),NULL));
        }}), NULL));
    
    
}


void CFightScene::addGuideInfo()
{
    if(guiderManagerLayer::getInstance()->getGuideType() == GUIDE_USE_MANGONEL)
    {
        guiderManagerLayer::getInstance()->removeGuiderLayer();
        if(guiderManagerLayer::getInstance()->getStepNum() == 4)
        {
            m_guiderArrow = Sprite::createWithSpriteFrameName("finger_1.png");
            addChild(m_guiderArrow, 1000);
            m_guiderArrow->setOpacity(0);
            m_guiderArrow->setPosition(Vec2(m_winSize.width * 0.5f, m_winSize.height * 0.45));
            ActionInterval* action1 = MoveBy::create(1.25f, Vec2(0, 150));
            ActionInterval* action2 = MoveBy::create(0, Vec2(0, -150));
            ActionInterval* seq = RepeatForever::create(Sequence::create(action1, action2, NULL));
            seq->setTag(10);
            m_guiderArrow->runAction(seq);
            m_guiderArrow->runAction(FadeIn::create(0.35f));
        }
        else if(guiderManagerLayer::getInstance()->getStepNum() >= 5)
        {
            guiderManagerLayer::getInstance()->setStepNum(5);
            if(m_guiderArrow)
            {
                m_guiderArrow->removeFromParent();
                m_guiderArrow = NULL;
            }
            m_guiderArrow = Sprite::createWithSpriteFrameName("finger_1.png");
            addChild(m_guiderArrow, 1000);
            m_guiderArrow->setPosition(Vec2(m_winSize.width * 0.33f, m_winSize.height * 0.58));
            ActionInterval* action1 = MoveBy::create(1.25f, Vec2(-100, 0));
            ActionInterval* action2 = MoveBy::create(0, Vec2(100, 0));
            ActionInterval* seq = RepeatForever::create(Sequence::create(action1, action2, NULL));
            seq->setTag(10);
            m_guiderArrow->runAction(seq);
        }
    }
    else if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_PVP_ROB) && (guiderManagerLayer::getInstance()->getStepNum() == 6))
    {
        Vec2 pos = m_pFightItem->convertToWorldSpaceAR(Vec2::ZERO);
        guiderManagerLayer::getInstance()->createLayer(this, GUIDE_SHOW_FINGER, pos);
    }
}

void CFightScene::searchCallBack(Ref* pSender)
{
    CPersonalData::getInstance()->setPvpMainLayer(true);
    CLayerManager::GetSingletonPtr()->createLayerWithLoading(eLayer_PVPLayer, NULL,false);
}

void CFightScene::getEnemyTeamInfoBack(Ref* pSender)
{
    
    _netRecvEventGetUserTeamInfo* pInfo = (_netRecvEventGetUserTeamInfo*)pSender;
    int state = 1;
    for(std::vector<std::string>::iterator it = pInfo->m_paramsStr.begin(); it != pInfo->m_paramsStr.end(); it++)
    {
        string str = *it;
        string name = CDataManager::getStrFromBackStr(str);
        
        if(name == "state")
        {
            state = atoi(CDataManager::getDigitFromBackStr(*it).c_str());
            if(state != 0)
            {
                log("获取玩家阵营信息失败");
                pvpCommonInfoLayer* pLayer = pvpCommonInfoLayer::create(PVP_COMMSG_NOFIND);
                addChild(pLayer, 1000);
                pLayer->showSearchResult(this, menu_selector(CFightScene::searchCallBack));
                return;
            }
        }
        else if(name == "userId")
        {
            m_userId = atoi(CDataManager::getDigitFromBackStr(*it).c_str());
        }
        else if(name == "name")
        {
            m_name = CDataManager::getDigitFromBackStr(*it);
            
            if(m_name == "null")
            {
                m_name = "无名氏";
            }
        }
        else if(name == "level")
        {
            m_level = atoi(CDataManager::getDigitFromBackStr(*it).c_str());
        }
        else if (name == "defenseMap")
        {
            m_enemyMaps = CDataManager::getDigitFromBackStr(*it);
        }
        else if(name == "robStone")
        {
            m_stone = atoi(CDataManager::getDigitFromBackStr(*it).c_str());
            std::string StrEnemyMaps	= m_enemyMaps;
            
            pvpEnemyInfo d;
            
            d.m_iLevel = m_level;
            d.m_iUserId = m_userId;
            d.m_sName = m_name;
            d.m_iRobStone = m_stone;
            
            CFightData::getInstance()->setThePvpEnemyInfo(d);
            
            char buf[256];
            std::vector<std::string> tempValue;
            sprintf(buf, "%d", EPVPMode_PLAY);
            tempValue.push_back(buf);
            sprintf(buf, "%d", int(m_stone * 0.1f));
            tempValue.push_back(buf);
            tempValue.push_back(StrEnemyMaps);
            
            CPersonalData::getInstance()->setPvpMainLayer(false);
            CFightData::getInstance()->setFightType(FIGHT_PVP_TYPE);
            CDBMgr::getInstance()->saveTempUserInfo(m_userId);
            CLayerManager::GetSingletonPtr()->createLayer(eLayer_FightPVP, NULL,false, true,&tempValue);
            
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
    
}

void CFightScene::EnemyTo(Ref *pSender)
{
    std::string content = CDBMgr::getInstance()->getTempAllUser();
    int level = CPersonalData::getInstance()->getUserInfo()->m_mainBaseLevel;
    CNetControlModule::sharedNetControlModule()->getRandomEnemyTeamInfo(this, menu_selector(CFightScene::getEnemyTeamInfoBack), level, content);
}

void CFightScene::getEnemyTeamInfo()
{
    //先找出玩家主基地的等级，然后搜索实力相当的玩家
    //为了测试，这里先传玩家id
    
    m_pLayer = LayerColor::create(Color4B(0, 0, 0, 0));
    addChild(m_pLayer, 900);
    m_pLayer->setPosition(Vec2::ZERO);
    
    CFightManager::addCloud(this,menu_selector(CFightScene::EnemyTo),m_pLayer);
    
}


void CFightScene::OnRebo(Ref *pSender)
{
    Director::getInstance()->resume();
    
    int iVideoId = CPersonalData::getInstance()->getVideoId();
    
    CNetControlModule::sharedNetControlModule()->getDefenseVideoByVideoId(this, menu_selector(CFightScene::getVideoInfo),
                                                                          iVideoId);
    
}


void CFightScene::getVideoInfo(Ref* pSender)
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
    }
    if(state == 1)
    {
        /*Sprite * thetarget = CFightManager::getTheFlyWord("录像不存在","fonts/cuti.ttf");
         thetarget->setPosition(m_winSize.width*0.5,m_winSize.height*0.5);
         addChild(thetarget,100);
         
         m_allVideoInfo[m_idx].bValid = 1;
         CDataManager::saveVideoById(m_allVideoInfo[m_idx].videoId);
         ((pvpMainLayer*)getParent())->getVideoListInfo();
         */
        
        return;
    }
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
    //CPersonalData::getInstance()->setPvpMainLayer(false);
    CLayerManager::GetSingletonPtr()->createLayer(eLayer_FightPVP, NULL,false, true, &tempValue);
    
}



void CFightScene::OnbtnCancle(Ref *pSender)
{
    
    Director::getInstance()->resume();

    

}

void CFightScene::backPvpMainLayer(Ref* pSender)
{
    if(CPersonalData::getInstance()->getState() == 0)
    {
//        CLayerManager::GetSingletonPtr()->createLayer(eLayer_PVPLayer, NULL,false);
        CLayerManager::GetSingletonPtr()->createLayerWithLoading(eLayer_PVPLayer,NULL,false);
    }
}

void CFightScene::OnbtnOk(Ref *pSender)
{
    Director::getInstance()->resume();
    if(guiderManagerLayer::getInstance()->getGuideType() == GUIDE_PVP_ROB)
    {
        guiderManagerLayer::getInstance()->setGuideType(GUIDE_ERROR_STATE);
    }
    
    if(CFightData::getInstance()->getIsPvpStart() == 0)
    {
        CNetControlModule::sharedNetControlModule()->getUserTeamInfo(this, menu_selector(CFightScene::backPvpMainLayer));
    }
    else {
        CFightData::getInstance()->setIsPvpStart(2);
        
        if(CPersonalData::getInstance()->getEnemyType() != SPECIFIED_ENEMY)
        {
            uploadInfo();
        }
        else
        {
            CNetControlModule::sharedNetControlModule()->getUserTeamInfo(this, menu_selector(CFightScene::backPvpMainLayer));
        }
    }
    
    if(m_pvpCloseMenu)
    {
        m_pvpCloseMenu->setEnabled(true);
    }
    
    if(m_pBackMenu)
    {
        m_pBackMenu->setEnabled(true);
        
    }
}

void CFightScene::resetOriginPos()
{
   
}

void CFightScene::rmvNameAndLevel()
{
    rmvNameAndLevel();
}

void CFightScene::btnCallBack(Ref *pSender)
{
    
    ///LYMenuItemSprite * pCloseItem = (LYMenuItemSprite*)pSender;
    
    int tag = ((Node*)pSender)->getTag();
    if(tag == ePVP_BACK_TAG)
    {
        
//        if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_PVP_ROB) && (guiderManagerLayer::getInstance()->getStepNum() >= 5)){
//            return;
//        }
      
    
        CHitDialog *pDlg = CHitDialog::create();
        
        pDlg->CreateFightExitDialog(this,menu_selector(CFightScene::OnbtnOk),menu_selector(CFightScene::OnbtnCancle));
        addChild(pDlg,999);
        
        
        
    }
    else if(tag == ePVP_RESEARCH_TAG)
    {
        //判断金币是否足够 //花费金币数量待定
        if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_PVP_ROB) && (guiderManagerLayer::getInstance()->getStepNum() <= 6))
        {
            return;
        }
        
        int iLevle =  CPersonalData::getInstance()->getUserInfo()->m_iUserLevel;
        if(CPersonalData::getInstance()->getUserCoin() < iLevle*10)
        {
            Sprite * thetarget = CFightManager::getTheFlyWord("金币不足","fonts/cuti.ttf");
            thetarget->setPosition(m_winSize.width*0.5,m_winSize.height*0.5);
            addChild(thetarget,100);
            return;
        }
        else
        {
            CNetWorkDataSingle::GetSingletonPtr()->SaveUserMCF(this, NULL, -iLevle*10,0,0);
            m_searchItem->setEnabled(false);
            m_pBackMenu->setEnabled(false);
            
            getEnemyTeamInfo();
        }
    }
    else if(tag == ePVP_FIGHT_TAG)
    {
        m_bPvpAllMapMove = false;
        rmvNameAndLevel();
        if(!(CDataManager::getEndedBy(GUIDE_PVP_ROB)) && (guiderManagerLayer::getInstance()->getGuideType() == GUIDE_PVP_ROB))
        {
            if(guiderManagerLayer::getInstance()->getStepNum() >= 6)
            {
                guiderManagerLayer::getInstance()->removeGuiderLayer();
                CDataManager::saveGuideInfoToDB(GUIDE_PVP_ROB);
            }
            else{
                return;
            }
        }
        //消耗体力
        if(6 > CPersonalData::getInstance()->GetPhysicalNum())
        {
            SimpleAudioEngine::getInstance()->playEffect("music/noBtn.mp3");
            Sprite * thetarget = CFightManager::getTheFlyWord("体力不足","fonts/cuti.ttf");
            thetarget->setPosition(Vec2(m_winSize.width / 2, m_winSize.height / 2));
            addChild(thetarget, 10);
            
            return;
        }
        int stamina = CPersonalData::getInstance()->GetPhysicalNum() - 6;
        if((stamina < CPersonalData::getInstance()->GetPhysicalMaxNum()) && (CPersonalData::getInstance()->getIfStaminaFull()))
        {
            CPersonalData::getInstance()->setIfStaminaFull(false);
            //保存当前使用体力的时间
            CPhysicalSystem::GetSingletonPtr()->SavePhysicalTime();
        }
        CNetWorkDataSingle::GetSingletonPtr()->SaveUserMCF(this,NULL, 0, 0, -6);
        if(CPersonalData::getInstance()->getEnemyType() == SPECIFIED_ENEMY)
        {
            int videoId = CPersonalData::getInstance()->getVideoId();
            CDataManager::saveRevenueFlag(videoId);
        }
        CFightData::getInstance()->setFightType(FIGHT_PVP_TYPE);
        
        m_searchItem->runAction(MoveTo::create(1,Vec2(m_searchItem->getPositionX(),-1000)));
        
        Label *pLabel = (Label*)m_searchItem->getChildByTag(100);
        
        if(pLabel)
        {
            pLabel->setVisible(false);
        }
        pLabel =  (Label*)m_pFightItem->getChildByTag(101);
        if(pLabel)
        {
            pLabel->setVisible(false);
        }
        m_pFightItem->runAction(Sequence::create(MoveTo::create(1,Vec2(-1000,-1000)),CallFunc::create([=]{
            
            m_bFightPvpStart = true;
            
            CBaseFighter* pCity  =  CFightData::getInstance()->getPVPTheCity();
            if(pCity)
            {
                Vec3 tZhu  = pCity->getPosition3D();
                
                
                m_pNpc1->setTheMode(2);
                m_pNpc1->SetDest(tZhu);
                
                m_pNpc1->setAddMoveSpeed(1.5);
                m_pNpc2->setTheMode(2);
                m_pNpc2->SetDest(tZhu);
                
                m_pNpc2->setAddMoveSpeed(1.5);
                m_pNpc3->setTheMode(2);
                m_pNpc3->SetDest(tZhu);
                
                m_pNpc3->setAddMoveSpeed(1.5);
                
            }
            CFightData::getInstance()->setIsPvpStart(1);
            scheduleUpdate();

        }),NULL));
        
        pLabel =  (Label*)m_pFightItem->getChildByTag(100);
        if(pLabel)
        {
            pLabel->setVisible(false);
        }
        //所有按钮开始移动
        //战斗场景飞入
        
        m_pBackItem->runAction(MoveTo::create(1,Vec2(-1000,1000)));
        m_progress->runAction(MoveTo::create(1,Vec2(1000,1000)));
        m_pAllStone->runAction(MoveTo::create(1,Vec2(1000,1000)));
        m_pEnemyBg->runAction(MoveTo::create(1,Vec2(1000,1000)));
        
        Label *pText = (Label*)m_pEnemyBg->getChildByTag(100);
        
        pText->runAction(MoveTo::create(1,Vec2(1000,1000)));
        
        Label *pLabelMagic = (Label*)getChildByTag(998);
        if(pLabelMagic)
        {
            ///pLabelMagic->runAction(MoveTo::create(3,Vec2(pLabelMagic->getPositionX(),-1000)));
        }
        
        Sprite *pg = (Sprite*)getChildByTag(999);
        if(pg)
        {
            //pg->runAction(MoveTo::create(3,Vec2(pg->getPositionX(),-1000)));
        }
    }
}

bool CFightScene::InitReadyFightPvpUI()
{
    initBG();
    
    _camera->setPositionX(END3DPOINT);
    
    char buf[50];
    Sprite* normal = Sprite::createWithSpriteFrameName("pvpBack.png");
    Sprite* select = Sprite::createWithSpriteFrameName("pvpBack.png");
    select->setColor(Color3B::GRAY);
    m_pBackItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(CFightScene::btnCallBack, this));
    m_pBackMenu = Menu::create(m_pBackItem,NULL);
    
    m_pBackItem->setTag(ePVP_BACK_TAG);
    addChild(m_pBackMenu, 100);
    m_pBackMenu->setEnabled(false);
    
    m_pBackMenu->setPosition(Vec2(m_winSize.width * 0.04f, m_winSize.height * 0.94f));
    
    //能量框
    m_pEnemyBg = Sprite::createWithSpriteFrameName("pvpStoneProgressBg.png");
    addChild(m_pEnemyBg, 100);
    m_pEnemyBg->setPosition(Vec2(m_winSize.width * 0.9f, m_winSize.height * 0.96));
    
    Sprite* sp = Sprite::createWithSpriteFrameName("pvpStoneProgress.png");
    m_progress = ProgressTimer::create(sp);
    m_pEnemyBg->addChild(m_progress, 101);
    m_progress->setType(ProgressTimer::Type::BAR);
    m_progress->setMidpoint(Vec2(0,0.5));
    m_progress->setBarChangeRate(Vec2(1, 0));
    m_progress->setPosition(m_pEnemyBg->getContentSize().width*0.5,m_pEnemyBg->getContentSize().height*0.5);
    
    
    pvpEnemyInfo EnemyInfo_s = CFightData::getInstance()->getThePvpEnemyInfo();
    m_userId = EnemyInfo_s.m_iUserId;
    
    int iStone  = EnemyInfo_s.m_iRobStone;
    int iMyStone =  CPersonalData::getInstance()->getUserKuangStone();
    
    int iMaxStone = CPersonalData::getInstance()->getUserMaxKuangStone();
    
    
    CFightData::getInstance()->setTheRobStone(iStone);
    
    sprintf(buf,"%d/%d",iMyStone,iMaxStone);
    
    
    float fPrecent = iMyStone*1.0/ iMaxStone*100;
    
    m_progress->setPercentage(fPrecent);
    
    
    Label* pcurStoneNum = Label::createWithSystemFont(buf, "Arial", 26);
    m_pEnemyBg->addChild(pcurStoneNum, 9999,100);
    pcurStoneNum->setPosition(Vec2(m_pEnemyBg->getBoundingBox().size.width * 0.5f, m_pEnemyBg->getBoundingBox().size.height * 0.5f));
    
    
    
    m_pAllStone =  Sprite::createWithSpriteFrameName("pvp_ore.png");
    
    m_pAllStone->setPosition(Vec2(m_winSize.width * 0.82f, m_winSize.height * 0.95));
    addChild(m_pAllStone, 100);
    
    normal = Sprite::createWithSpriteFrameName("pvp_fight.png");
    select = Sprite::createWithSpriteFrameName("pvp_fight.png");
    select->setColor(Color3B::GRAY);
    m_pFightItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(CFightScene::btnCallBack, this));
    Menu* fightMenu = Menu::create(m_pFightItem , NULL);
    m_pFightItem ->setTag(ePVP_FIGHT_TAG);
    m_pFightItem->setEnabled(false);
    
    addChild(fightMenu, 100);
    fightMenu->setPosition(Vec2(m_winSize.width * 0.1f, m_winSize.height * 0.08f));
    
    Label* pLabel = Label::createWithTTF("开始战斗", "fonts/cuti.ttf", 30);
    m_pFightItem ->addChild(pLabel,0,100);
    pLabel->setPosition(Vec2(normal->getContentSize().width * 0.5f, normal->getContentSize().height * 0.7f));
    
    
    int iLevle =  CPersonalData::getInstance()->getUserInfo()->m_iUserLevel;
    

    Label* pStamina = Label::createWithTTF("6", "fonts/cuti.ttf", 30);
    m_pFightItem->addChild(pStamina, 0, 101);
    pStamina->enableOutline(Color4B(0, 0, 0, 255), 2);
    pStamina->setPosition(Vec2(normal->getContentSize().width * 0.4f, normal->getContentSize().height * 0.26f));
    
    Sprite* pStaminaIcon = Sprite::createWithSpriteFrameName("stamina.png");
    m_pFightItem->addChild(pStaminaIcon);
    pStaminaIcon->setScale(0.58f);
    pStaminaIcon->setPosition(Vec2(normal->getContentSize().width * 0.63f, normal->getContentSize().height * 0.26f));
    
    normal = Sprite::createWithSpriteFrameName("reSearch.png");
    select = Sprite::createWithSpriteFrameName("reSearch.png");
    
    select->setColor(Color3B::GRAY);
    m_searchItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(CFightScene::btnCallBack, this));
    Menu* searchMenu = Menu::create(m_searchItem, NULL);
    m_searchItem->setTag(ePVP_RESEARCH_TAG);
    addChild(searchMenu, 100);
    searchMenu->setPosition(Vec2(m_winSize.width * 0.9f, m_winSize.height * 0.08f));
    m_searchItem->setEnabled(false);
    
    //搜索下一个目标需要的金币
    Sprite* coin = Sprite::createWithSpriteFrameName("coin.png");
    coin->setAnchorPoint(Vec2(1, 0.5f));
    m_searchItem->addChild(coin);
    coin->setScale(0.65f);
    coin->setPosition(Vec2(normal->getContentSize().width * 0.95f, normal->getContentSize().height * 0.25f));
    
    sprintf(buf,"%d",iLevle*10);
    
    Label* pCostCoinNum = Label::createWithSystemFont(buf, "Arial", 30);
    m_searchItem->addChild(pCostCoinNum,0,100);
    pCostCoinNum->setPosition(Vec2(normal->getContentSize().width * 0.51f, normal->getContentSize().height * 0.25f));
    
    
    Label *pGetStone = Label::createWithTTF("可获取矿石:","fonts/cuti.ttf",30);
    pGetStone->setPosition(m_winSize.width*0.5,m_winSize.height*0.1);
    pGetStone->enableOutline(Color4B(0,0,0,255),3);
    addChild(pGetStone,0,998);
    
    //pvp_stoneIcon.png
    //pvpStoneBg.png
    Sprite *pbg = Sprite::createWithSpriteFrameName("pvpStoneBg.png");
    pbg->setPosition(m_winSize.width*0.5,m_winSize.height*0.05);
    addChild(pbg,0,999);
    
    cocos2d::Size tStoneBg  = pbg->getContentSize();
    
    
    sprintf(buf,"%d",EnemyInfo_s.m_iRobStone);
    
    Label *pStoneNum = Label::createWithTTF(buf,"fonts/cuti.ttf",30);
    pStoneNum->setPosition(tStoneBg.width*0.6,tStoneBg.height*0.5);
    pbg->addChild(pStoneNum,0,999);
    
    Sprite *pStone = Sprite::createWithSpriteFrameName("pvp_ore.png");
    pStone->cocos2d::Node::setScale(0.8);
    
    pStone->setPosition(tStoneBg.width*0.2,tStoneBg.height*0.5);
    pbg->addChild(pStone);
    
    
    sprintf(buf, "%s的窝 Lv:%d", EnemyInfo_s.m_sName.c_str(), EnemyInfo_s.m_iLevel);
    Label* pEnemy = Label::createWithTTF(buf, "fonts/cuti.ttf", 30);
    pEnemy->setPosition(tStoneBg.width*0.5,tStoneBg.height*2.5);
    ///pEnemy->setPosition(Vec2(m_winSize.width * 0.5f, m_winSize.height * 0.15f));
    
    pEnemy->enableOutline(Color4B(0, 0, 0, 255), 3);
    pbg->addChild(pEnemy, 9999);
    
    //pEnemy->setPosition(Vec2(m_winSize.width * 0.5f, m_winSize.height * 0.15f));
    
    m_pSwitchItem1->setVisible(false);
    
    return true;
}


bool CFightScene::init(){
    
    if (!CBaseLayer::init())
    {
        return false;
    }
    
    
    
    
    char buf[128];
    int randNum = CCRANDOM_0_1()*2+1;
    sprintf(buf, "music/battle%d.mp3", randNum);
    
    //如果是塔防模式下，使用塔防战斗音乐
    if (CFightData::getInstance()->getTheLevelCondition()==eLevelCondition_TowerDefense) {
        sprintf(buf, "music/towerFight.mp3");
    }
    
    
    //    //测试用暂停背景音乐
    //    SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    
    if(UserDefault::getInstance()->getBoolForKey("bgMusic", true))
    {
        SimpleAudioEngine::getInstance()->playBackgroundMusic(buf, true);
        SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(1.5f);
    }
    
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("plist/FX.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("plist/attack.plist");
    
    initData();
    m_layer = Layer::create();
    addChild(m_layer, 100);
    m_layer->setCascadeColorEnabled(true);
    
    layer3D = Layer::create();
    addChild(layer3D,0,FIGHT_LAYER3DTAG);
    
    layer3D->setCascadeColorEnabled(true);
    //    std::string fileName = "changing.c3b";
    
    //    std::string fileName = "models/gameScene/theScene2.c3b";
    
    //    m_spriteBg = Sprite3D::create(fileName);
    
    //
    //    m_spriteBg->setPosition3D(Vec3(s.width*0.85, -79,-400));
    //    m_spriteBg->setScale(0);
    //    layer3D->addChild(m_spriteBg);
    
    //m_spriteBg->setRotation3D(Vec3(-90,90,0));
    
    auto s = Director::getInstance()->getWinSize();
    m_WinSize  = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    //    //拉杆 上下左右
    //    m_pLaGan  = Sprite::createWithSpriteFrameName("lagan.png");
    //
    //    m_pLaGan->setPosition(m_WinSize.width*0.1,m_WinSize.height*0.26);
    //
    //    m_layer->addChild(m_pLaGan);
    //
    //    m_pLanGanDian = Sprite::createWithSpriteFrameName("laganpoint.png");
    //
    //
    //    m_pLanGanDian->setPosition(m_WinSize.width*0.1,m_WinSize.height*0.26);
    //    m_layer->addChild(m_pLanGanDian);
    
    _camera = Camera::createPerspective(60, (GLfloat)s.width/s.height, 1, 2000);
    
    //    _camera = Camera::createOrthographic(s.width, s.height, 1, 4000);
    
    _camera->setCameraFlag(CameraFlag::USER1);
    
    
    if(CFightData::getInstance()->getTheLevelCondition()!=eLevelCondition_TowerDefense){
        
        Vec3  VCamera(Start3DPOINT,CAMERA_POSITION_Y,CAMERA_POSITION_Z); //350
        
        //        _camera->setPosition3D(VCamera);
        setOriginCameraPos(VCamera);
        
        _camera->lookAt(Vec3(Start3DPOINT,CAMERA_LOOK_POSITION_Y,CAMERA_LOOK_POSITION_Z), Vec3(0,1,0) );
        
    }else{ //塔防模式下，镜头做特殊处理
        
        Vec3  VCamera(Start3DPOINT,CAMERA_TOWER_DEFFENSE_POSITION_Y,CAMERA_TOWER_DEFFENSE_POSITION_Z); //350
        //
        //        _camera->setPosition3D(VCamera);
        setOriginCameraPos(VCamera);
        
        _camera->lookAt(Vec3(Start3DPOINT,CAMERA_LOOK_POSITION_Y,CAMERA_LOOK_TOWER_DEFFENSE_POSITION_Z), Vec3(0,1,0));
        
    }
    
    //_camera->setPosition3D(Vec3(0, 200, 350) + m_pTarget->getPosition3D());
    //    _camera->lookAt(m_pTarget->getPositiƒo3D(), Vec3(0,1,0) );
    
    layer3D->addChild(_camera);
    layer3D->setCameraMask((unsigned short )CameraFlag::USER1);
    
    //触摸初始化
    m_pListener = EventListenerTouchOneByOne::create();
    m_pListener->setSwallowTouches(true);
    
    m_pListener->onTouchBegan = CC_CALLBACK_2(CFightScene::onTouchBegan, this);
    m_pListener->onTouchMoved = CC_CALLBACK_2(CFightScene::onTouchMoved, this);
    m_pListener->onTouchEnded = CC_CALLBACK_2(CFightScene::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(m_pListener, this);

    
    CFightData::getInstance()->m_pSmallMapLayer = CSmallMap::create(this);
    
    addChild( CFightData::getInstance()->m_pSmallMapLayer,100);
    
    
    //投石车标识按钮
    Menu* pMenu = Menu::create();
    m_layer->addChild(pMenu, 10);
    pMenu->setPosition(Vec2::ZERO);
    pMenu->setVisible(false);
    if(CFightData::getInstance()->getNowMidLevelId() < TOWER_LEVEL_INDEX)
    {
        pMenu->setVisible(true);
    }
    

    
    Sprite* normal = Sprite::createWithSpriteFrameName("toushiji.png");
    Sprite* select = Sprite::createWithSpriteFrameName("toushiji.png");
    Sprite* pDisable = Sprite::createWithSpriteFrameName("toushiji.png");
    
    pDisable->setColor(Color3B::GRAY);
    select->setColor(Color3B::GRAY);
    m_pSwitchItem1 = LYMenuItemSprite::create(normal, select,pDisable, CC_CALLBACK_1(CFightScene::startBlowCallBack, this));
    m_pSwitchItem1->setTag(FIGHT_TOUSHICHE_TAG);
    pMenu->addChild(m_pSwitchItem1);
    m_pSwitchItem1->setVisible(false);
    
    normal = Sprite::createWithSpriteFrameName("rmv_1.png");
    select = Sprite::createWithSpriteFrameName("rmv_1.png");
    select->setColor(Color3B::GRAY);
    m_pRmvItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(CFightScene::startBlowCallBack, this));
    m_pRmvItem->setTag(FIGHT_DEL_SOLIDER);
    m_pRmvItem->setVisible(false);
    m_pRmvItem->setPosition(Vec2(m_WinSize.width * 0.06, m_WinSize.height * 0.74f));
    pMenu->addChild(m_pRmvItem);
    
    if(CPersonalData::getInstance()->getUserInfo()->m_iSmallLevel >= 5&&m_eFightSceneType!=eFightSceneType_PVP)
    {
        m_pRmvItem->setVisible(true);
    }
    
    normal = Sprite::createWithSpriteFrameName("rmv_2.png");
    select = Sprite::createWithSpriteFrameName("rmv_2.png");
    select->setColor(Color3B::GRAY);
    m_pRmvBackItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(CFightScene::startBlowCallBack, this));
    m_pRmvBackItem->setTag(FIGHT_DEL_SOLIDER);
    m_pRmvBackItem->setVisible(false);
    m_pRmvBackItem->setPosition(Vec2(m_WinSize.width * 0.06, m_WinSize.height * 0.74f));
    pMenu->addChild(m_pRmvBackItem);
    
    m_pSwitchItem1->setPosition(Vec2(m_WinSize.width * 0.05, m_WinSize.height * 0.42f));
  
    
    normal = Sprite::createWithSpriteFrameName("hero_switch.png");
    select = Sprite::createWithSpriteFrameName("hero_switch.png");
    select->setColor(Color3B::GRAY);
    m_pSwitchItem2 = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(CFightScene::startBlowCallBack, this));
    m_pSwitchItem2->setTag(FIGHT_LAGAN_TAG);
    pMenu->addChild(m_pSwitchItem2);
    m_pSwitchItem2->setPosition(Vec2(m_WinSize.width * 0.04, m_WinSize.height * 0.42f));
    m_pSwitchItem2->setVisible(false);
    m_pSwitchItem2->setEnabled(false);
    
    m_pToushiActive = Sprite::createWithSpriteFrameName("toushichenormal.png");
    m_pToushiActive->setVisible(false);
    m_pToushiActive->setScale(1);
    m_pToushiActive->setPosition(m_WinSize.width*0.30,m_WinSize.height*0.63);
    addChild(m_pToushiActive);
    
    
    //注册消息
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(CFightScene::startMonitorBlow), "StartBlow", NULL);
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(CFightScene::startMonitorCut), "StartCut", NULL);
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(CFightScene::startCutSolider), "guideCut", NULL);
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(CFightScene::startBlowSolider), "guideBlow", NULL);
    //小地图当前位置
    m_pCurView = Sprite::createWithSpriteFrameName("pvp_smallmap.png");
    
    //float fScaleX =  m_WinSize.width*(m_WinSize.width/(BIGMAPLEN+2*ONESIDE))/m_pCurView->getContentSize().width;
    
    
    float fScaleX = m_WinSize.width*ONESIDE/BIGMAPLEN*2 / m_pCurView->getBoundingBox().size.width;
    
    float fScaleY = m_WinSize.height*0.12/m_pCurView->getContentSize().height;
    
    m_pCurView->setScaleX(fScaleX);
    m_pCurView->setScaleY(fScaleY);
    
    m_pCurView->setAnchorPoint(cocos2d::Point(0.5,0.5));
    m_pCurView->setOpacity(125);
    m_pCurView->setPosition(cocos2d::Point(m_pCurView->boundingBox().size.width*0.5,m_WinSize.height*0.065));
    addChild(m_pCurView, 2);

    
    initAddSolider();
    
    //if(CFightData::getInstance()->getFightType() == FIGHT_PVP_TYPE)
    {
        //smallmapdi.png
        
        Sprite *pBg = Sprite::createWithSpriteFrameName("smallmapdi.png");
        pBg->setPosition(pBg->getBoundingBox().size.width*0.5,m_WinSize.height*0.08);
        addChild(pBg,0,FIGHT_SMALLMAP);
        
        
        //bgLayer->setVisible(false);
        
        m_pCurView->setSpriteFrame("pvp_smallmap.png");
        m_pCurView->setScale(0.9);
        
        
        
        if(CFightData::getInstance()->getFightType() == FIGHT_PVP_TYPE)
        {
            pBg->setVisible(false);
            m_pCurView->setVisible(false);
            CFightData::getInstance()->m_pSmallMapLayer->setVisible(false);
            
        }
        m_pCurView->setPosition(cocos2d::Point(m_pCurView->boundingBox().size.width*0.5,m_WinSize.height*0.08));
        
    }
    
    if(CFightData::getInstance()->getFightType() == FIGHT_PVP_TYPE)
    {
        //添加地板
        for (int x = 0; x < 24; x++)
        {
            for (int z = -9; z <= 0; z++)
            {
                Sprite3D* pFloor = CFightManager::getTheModelWithPath("pvp_MapBlock.png");
                pFloor->setPosition3D(Vec3(x * LENGTH_BLOCK_PER + MAP_START_XPOS, 0, z * LENGTH_BLOCK_PER + STARTZPOINT));
                pFloor->setRotation3D(Vec3(90, 0, 0));
                pFloor->getChildByTag(100)->setScale(0.6f);
                float opcity = 150;
                if(z < -6)
                {
                    opcity = ((z + 9) * 0.05f + 0.5f)*255;
                }
                pFloor->getChildByTag(100)->setOpacity(opcity);
                
                layer3D->addChild(pFloor);
                pFloor->setCameraMask((unsigned short )CameraFlag::USER1);
                
                Sprite3D* sprite = CFightManager::getTheModelWithPath("pvpKuang.png");
                sprite->setPosition3D(Vec3(x * LENGTH_BLOCK_PER + MAP_START_XPOS, 1, z * LENGTH_BLOCK_PER + STARTZPOINT));
                sprite->setRotation3D(Vec3(90, 0, 0));
                sprite->setScale(0.6f);
                layer3D->addChild(sprite);
                sprite->setVisible(false);
                sprite->setCameraMask((unsigned short )CameraFlag::USER1);
            }
        }
        
        
        if(CPersonalData::getInstance()->getEnemyType() == FIGHT_BACK_VIDEO)
        {
            addCloud();
            //closeClound();
            
            CFightManager::closeCloud(NULL,NULL,m_pLayer);
            
            m_iAllTime = m_iRecEndTime;
            
            CFightData::getInstance()->setThePvpTime(m_iAllTime);
            
            CFightData::getInstance()->setThePvpAllTime(m_iAllTime);
            
            
            initUI();
            fightStartAnimateInit();
            scheduleUpdate();
            m_bJishiStart = true;
            
            
            schedule(schedule_selector(CFightScene::updateAnimateGuide), 0.5f);
            m_pCurView->setVisible(true);
            
            Sprite *pSmallMap = (Sprite*)getChildByTag(FIGHT_SMALLMAP);
            pSmallMap->setVisible(true);
            CFightData::getInstance()->m_pSmallMapLayer->setVisible(true);
        }
        else
        {
            
            InitReadyFightPvpUI();
            addCloud();
            startSearchEnemy();
            
            
            //添加逃跑路人
       
              char strMode[] = {16,18,19,20,22,23,25};
            
            int iRandom =   sizeof(strMode)/sizeof(char);
            
             int iNum =  random(0,iRandom-1);
            
            
            
            CDataManager::CalcFighterInfoWith(&m_theInfo,14, 1, 1, 0, false, 1, 0, 0, 0);
            m_pNpc1  = CNpcFighter::create(&m_theInfo,strMode[iNum]);
            int iDescY =   -450;
            int  ix = random(1900,2200);
            int  iz  =  random(-650,-450);
            m_pNpc1->SetDest(Vec3(ix ,0,iz));
            m_pNpc1->setPosition3D(Vec3(2000,0,iDescY));
            m_pNpc1->setTheMode(1);
            m_pNpc1->scheduleUpdate();
        
            addChild(m_pNpc1,9999);


             iNum =  random(0,iRandom-1);
             CDataManager::CalcFighterInfoWith(&m_theInfo,14, 1, 1, 0, false, 1, 0, 0, 0);
            m_pNpc2  = CNpcFighter::create(&m_theInfo,strMode[iNum]);

            ix = random(1900,2200);
            iz  =  random(-650,-450);
            m_pNpc2->setPosition3D(Vec3(ix,0,iz));

            ix = random(1900,2200);
            iz  =  random(-650,-450);

            m_pNpc2->SetDest(Vec3(ix ,0,iz));
            m_pNpc2->setTheMode(1);
            m_pNpc2->scheduleUpdate();
            addChild(m_pNpc2,9999);


            iNum =  random(0,iRandom-1);
            CDataManager::CalcFighterInfoWith(&m_theInfo,14, 1, 1, 0, false, 1, 0, 0, 0);
           
            m_pNpc3  = CNpcFighter::create(&m_theInfo,strMode[iNum]);
            ix = random(1900,2200);
            iz  =  random(-650,-450);
            m_pNpc3->setPosition3D(Vec3(ix,0,iz));
            ix = random(1900,2200);
            iz  =  random(-650,-450);
            m_pNpc3->SetDest(Vec3(ix ,0,iz));
            m_pNpc3->setTheMode(1);
            m_pNpc3->scheduleUpdate();
            addChild(m_pNpc3,9999);

        
            
            m_pNpc1->setCameraMask((unsigned short )CameraFlag::USER1);
            m_pNpc2->setCameraMask((unsigned short )CameraFlag::USER1);
            m_pNpc3->setCameraMask((unsigned short )CameraFlag::USER1);
        }
    }
    
    else
    {
        initUI();
        fightStartAnimateInit();
        schedule(schedule_selector(CFightScene::updateAnimateGuide), 0.5f);
    }
    //    fighterInit();
    animateInit();
    levelInit();
    
    initLight();
    
    CFightManager::addSprite3dAnimationBeforeFight();//预加载所有动画
    

    if(CFightData::getInstance()->getFightType() == FIGHT_PVP_TYPE||CFightData::getInstance()->getFightType() == FIGHT_TOWERDENFESE_TYPE)
    {
        m_pXian = Sprite3D::create();
        
    
        m_pXian->setPosition3D(Vec3(1680, 0,-203));
        
        if(CFightData::getInstance()->getFightType() == FIGHT_TOWERDENFESE_TYPE)
        {
        
            m_pXian->setPosition3D(Vec3(END3DPOINT,0,-103));
        }
        
        m_pXian->setRotation3D(Vec3(-90,0,0));
        layer3D->addChild(m_pXian);
    
        
        Color3B defaultColor = Color3B(255,200,0);

        
        float changeTime= 0.2f;
        
        for(int i=0;i<14;i++)
        {
            Sprite *pChubinXian = Sprite::createWithSpriteFrameName("chubingxian.png");
            pChubinXian->cocos2d::Node::setScale(0.5);
            
            pChubinXian->setPositionY(-30+40*i);
            
            m_pXian->addChild(pChubinXian);
        
            pChubinXian->setColor(defaultColor);
            
            pChubinXian->runAction(Sequence::create(DelayTime::create(changeTime*i),CallFunc::create([=](){
                
            auto* repeatAction = RepeatForever::create(Sequence::create(TintTo::create(0, 100, 100, 100),DelayTime::create(changeTime),CallFunc::create([=]{pChubinXian->setColor(Color3B::GRAY);}),DelayTime::create(19*changeTime), NULL));
                pChubinXian->runAction(repeatAction);
            }), NULL));

            
            
        }
        
          m_pXian->setCameraMask((unsigned short )CameraFlag::USER1);
    }

    return true;

}

void CFightScene::addTaskBgAnimation()
{
    Animation* pTaskAction = AnimationCache::getInstance()->getAnimation("taskAnimation");
    Sprite* sp = Sprite::createWithSpriteFrameName("task_1.png");
    
    
    m_ptaskBg->addChild(sp,0,100);
    sp->setPosition(Vec2(m_ptaskBg->getContentSize().width * 0.5f + 5, m_ptaskBg->getContentSize().height * 0.5f));
    sp->runAction(RepeatForever::create(Animate::create(pTaskAction)));
}


void CFightScene::StartToushiCD()
{
    
    if(m_ePVPMode==EPVPMode_REC){
        return;
    }
    
    if(m_pXian)
    {
        m_pXian->setVisible(false);
    }
    
    
    m_bToushicheStart = true;
    
    
    CDataManager::saveTaskInfoToDB(7, 1);
    
    m_pToushiActive->setVisible(false);
    if(m_paoWuLayer)
    {
        m_paoWuLayer->getTheDragBall()->setVisible(false);
      
        m_paoWuLayer->m_theBall->setVisible(false);
    }
    m_pSwitchItem1->setEnabled(true);
    m_pSwitchItem1->setVisible(true);
    m_pSwitchItem2->setEnabled(false);
    m_pSwitchItem2->setVisible(false);
    
    if(CFightData::getInstance()->getFightType() != FIGHT_PVP_TYPE)
    {
        m_layer->getChildByTag(SKILL_START_TAG)->setVisible(true);
        m_layer->getChildByTag(SKILL_CUT_FRUIT_TAG)->setVisible(true);
    }
    m_bifNorAddSolider = true;
    m_bToushiMove = false;
    m_pSwitchItem1->setEnabled(false);
    cocos2d::Size  tSize  = m_pSwitchItem1->getContentSize();
    
    char strTime[50] = {0};
    sprintf(strTime,"%d",m_iToushiCDTime);
    
    Label *pTime = (Label *)m_pSwitchItem1->getChildByTag(100);
    if(pTime)
    {
        pTime->removeFromParent();
    }
    
    Label *pLabelTime = Label::createWithTTF(strTime,"fonts/cuti.ttf",30);
    pLabelTime->enableOutline(Color4B(0,0,0,255),3);
    
    pLabelTime->setPosition(tSize.width*0.5,tSize.height*0.5);
    m_pSwitchItem1->addChild(pLabelTime,0,100);
    
    
     
}


void CFightScene::EndToushijiCD()
{
    if(m_pXian){
        m_pXian->setVisible(true);
    }
    
    m_bToushicheStart = false;
    
    m_iToushiCDTime = 60;
    
    
    Label * pLabelTime = (Label*)m_pSwitchItem1->getChildByTag(100);
    if(pLabelTime)
    {
        pLabelTime->removeFromParent();
    }
    if(!((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_USE_MANGONEL) && (guiderManagerLayer::getInstance()->getStepNum() == 3)))
    {
        m_pSwitchItem1->setEnabled(true);
    }
}




void CFightScene::initLight(){
    
    
    //    //使用光照实在是太卡
    //    m_pAmbientLight = DirectionLight::create(Vec3(0, -1.0f, -1.0f), Color3B(200, 200, 200));
    //    m_pAmbientLight->retain();
    //    m_pAmbientLight->setEnabled(true);
    //    addChild(m_pAmbientLight);
    //    m_pAmbientLight->setPosition3D(Vec3(Start3DPOINT,CAMERA_TOWER_DEFFENSE_POSITION_Y,CAMERA_TOWER_DEFFENSE_POSITION_Z));
    //
    //    CFightManager::addCamera(m_pAmbientLight);
    //    m_pAmbientLight->setCameraMask(2);
    
    
    //        m_pAmbientLight = DirectionLight::create(Vec3(0, -1.0f, 0), Color3B(200, 200, 200));
    //        m_pAmbientLight->retain();
    //        m_pAmbientLight->setEnabled(true);//光源关闭
    //        m_pAmbientLight->setPosition3D(_camera->getPosition3D());
    //        addChild(m_pAmbientLight);
    //        CFightManager::addCamera(m_pAmbientLight);
    
    //    auto ambient = AmbientLight::create(Color3B(255,255,255));
    //    ambient->setPosition3D(Vec3(Start3DPOINT,100,CAMERA_TOWER_DEFFENSE_POSITION_Z));
    //
    //    addChild(ambient);
    
    
    
}


void CFightScene::remvGuideDraw()
{
    
    bool ifRemove = true;
    
    //如果当前引导是划线并且该引导没完成。则删除
    if ((guiderManagerLayer::getInstance()->getGuideType()==GUIDE_ADD_CIRCLE&&!CDataManager::getEndedBy(GUIDE_ADD_CIRCLE))
        ||(guiderManagerLayer::getInstance()->getGuideType()==GUIDE_ADD_LINE&&!CDataManager::getEndedBy(GUIDE_ADD_LINE))
        ||(guiderManagerLayer::getInstance()->getGuideType()==GUIDE_ADD_RECTANGLE&&!CDataManager::getEndedBy(GUIDE_ADD_RECTANGLE))
        ||(guiderManagerLayer::getInstance()->getGuideType()==GUIDE_ADD_V&&!CDataManager::getEndedBy(GUIDE_ADD_V))) {
        ifRemove = false;
    }
    
    
    
    
    
    
    
    if (ifRemove) {
        for(int i = 0; i < 8; i++)
        {
            Sprite* sp = (Sprite*)(m_layer->getChildByTag(GUIDER_DRAW_LINE + i));
            if(sp)
            {
                sp->removeFromParent();
                sp = NULL;
            }
        }
        
    }else{    //不移除动画，只是颜色变淡
        
        for(int i = 0; i < 8; i++)
        {
            Sprite* sp = (Sprite*)(m_layer->getChildByTag(GUIDER_DRAW_LINE + i));
            if(sp)
            {
                sp->setOpacity(180);
                sp = NULL;
            }
        }
        
    }
    
    
    
}

void CFightScene::startNextGuide()
{//    if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_ADD_LINE) && (guiderManagerLayer::getInstance()->getStepNum() == 4))
    //    {
    //        Vec2 pos = Vec2(m_WinSize.width * 0.22f, m_WinSize.height - 45);
    //        guiderManagerLayer::getInstance()->createLayer(this, GUIDE_SHOW_BOTH, pos);
    //    }
    //    else
    //    {
    //        scheduleUpdate();
    //    }
}

void CFightScene::updateAnimateGuide(float dt)
{
    if(guiderManagerLayer::getInstance()->getstart())
    {
        char buf[257];
        if(guiderManagerLayer::getInstance()->getGuideType() > 36)//引导类型数量
        {
            unschedule(schedule_selector(CFightScene::updateAnimateGuide));
        }
        guiderManagerLayer::getInstance()->setstart(false);
        //根绝type值播放动画
        if(guiderManagerLayer::getInstance()->getGuideType() == GUIDE_ADD_LINE)
        {
            if(guiderManagerLayer::getInstance()->getStepNum() == 3)
            {
                
                //如果有动作了，则不初始化了
                Node* target = m_layer->getChildByTag(GUIDER_DRAW_LINE);
                
                if (target) {
                    target->setOpacity(255);
                }else{
                    Animation* pLineAnimation = Animation::create();
                    for (int i = 1; i < 5; i++)
                    {
                        sprintf(buf, "line_guide_%d.png", i);
                        pLineAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(buf));
                    }
                    pLineAnimation->setDelayPerUnit(0.35f);
                    Animate* pAction = Animate::create(pLineAnimation);
                    Sprite* line = Sprite::createWithSpriteFrameName("line_guide_1.png");
                    m_layer->addChild(line, 100, GUIDER_DRAW_LINE);
                    
                    
                    line->setScale(1.65f);
                    line->setPosition(Vec2(m_WinSize.width / 2, m_WinSize.height / 2));
                    line->runAction(RepeatForever::create(pAction));
                }
                
            }
        }
        else if(guiderManagerLayer::getInstance()->getGuideType() == GUIDE_ADD_CIRCLE)
        {
            //圆形动画
            
            //如果有动作了，则不初始化了
            Node* target = m_layer->getChildByTag(GUIDER_DRAW_CIRCLE);
            
            if (target) {
                target->setOpacity(255);
            }else{
                Animation* pCircleAnimation = Animation::create();
                for (int i = 1; i < 10; i++)
                {
                    sprintf(buf, "circle_guide_%d.png", i);
                    pCircleAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(buf));
                }
                pCircleAnimation->setDelayPerUnit(0.15f);
                Animate* pAction = Animate::create(pCircleAnimation);
                Sprite* circle = Sprite::createWithSpriteFrameName("circle_guide_1.png");
                circle->setScale(1.65f);
                m_layer->addChild(circle, 100, GUIDER_DRAW_CIRCLE);
                circle->setPosition(Vec2(m_WinSize.width / 2, m_WinSize.height / 2));
                circle->runAction(RepeatForever::create(pAction));
            }
        }
        else if(guiderManagerLayer::getInstance()->getGuideType() == GUIDE_ADD_RECTANGLE)
        {
            //矩形动画
            
            //如果有动作了，则不初始化了
            Node* target = m_layer->getChildByTag(GUIDER_DRAW_RECTANGLE);
            
            if (target) {
                target->setOpacity(255);
            }else{
                Animation* pRectAnimation = Animation::create();
                for (int i = 1; i < 6; i++)
                {
                    sprintf(buf, "rectangle_guide_%d.png", i);
                    pRectAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(buf));
                }
                pRectAnimation->setDelayPerUnit(0.25f);
                Animate* pAction = Animate::create(pRectAnimation);
                Sprite* pRect = Sprite::createWithSpriteFrameName("rectangle_guide_1.png");
                pRect->setScale(1.65f);
                m_layer->addChild(pRect, 100, GUIDER_DRAW_RECTANGLE);
                pRect->setPosition(Vec2(m_WinSize.width / 2, m_WinSize.height / 2));
                pRect->runAction(RepeatForever::create(pAction));
            }
        }
        else if(guiderManagerLayer::getInstance()->getGuideType() == GUIDE_ADD_V)
        {
            //V形动画
            
            //如果有动作了，则不初始化了
            Node* target = m_layer->getChildByTag(GUIDER_DRAW_V);
            
            if (target) {
                target->setOpacity(255);
            }else{
                Animation* pVAnimation = Animation::create();
                for (int i = 1; i < 6; i++)
                {
                    sprintf(buf, "V_guide_%d.png", i);
                    pVAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(buf));
                }
                pVAnimation->setDelayPerUnit(0.25f);
                Animate* pAction = Animate::create(pVAnimation);
                Sprite* pV = Sprite::createWithSpriteFrameName("V_guide_1.png");
                pV->setScale(1.65f);
                m_layer->addChild(pV, 100, GUIDER_DRAW_V);
                pV->setPosition(Vec2(m_WinSize.width / 2, m_WinSize.height / 2));
                pV->runAction(RepeatForever::create(pAction));
            }
        }
        else if(!(CDataManager::getEndedBy(GUIDE_TOWER_LEVEL)) && (guiderManagerLayer::getInstance()->getGuideType() == GUIDE_TOWER_LEVEL))
        {
            //形动画
            
            //如果有动作了，则不初始化了
            Node* target = m_layer->getChildByTag(GUIDER_DRAW_V);
            
            if (target) {
                target->setOpacity(255);
            }else{
                Animation* pVAnimation = Animation::create();
                for (int i = 1; i < 5; i++)
                {
                    sprintf(buf, "check_%d.png", i);
                    pVAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(buf));
                }
                pVAnimation->setDelayPerUnit(0.35f);
                Animate* pAction = Animate::create(pVAnimation);
                Sprite* pV = Sprite::createWithSpriteFrameName("check_1.png");
                m_layer->addChild(pV, 100, GUIDER_CHECK);
                pV->setPosition(Vec2(m_WinSize.width / 2, m_WinSize.height / 2));
                pV->runAction(RepeatForever::create(pAction));
                
                //显示位置提示
                Sprite* pWord = Sprite::createWithSpriteFrameName("TD_GuideWord.png");
                m_layer->addChild(pWord, 100, GUIDER_WORD_TIP);
                pWord->setScale(1.2f);
                pWord->setPosition(Vec2(m_WinSize.width * 0.5f, m_WinSize.height * 0.8f));
                
                //显示光圈
                Sprite* sp = Sprite::createWithSpriteFrameName("buildingBg.png");
                m_layer->addChild(sp, 100, GUIDER_LIGHT);
                sp->setScale(0);
                sp->setPosition(Vec2(pV->getPositionX() - pV->getBoundingBox().size.width * 0.42f, pV->getPositionY() + 22));
                sp->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.6f, 1.0), ScaleTo::create(0.6f, 0.3), NULL)));
            }
        }
        scheduleUpdate();
        m_bGuideBlow = true;
        CFightData::getInstance()->setStopAddEnemy(true);
    }
    if(guiderManagerLayer::getInstance()->getNextGuide())
    {
        guiderManagerLayer::getInstance()->setNextGuide(false);
        scheduleUpdate();
    }
    
}

void CFightScene::detectSdtory()
{
    //先保存游戏中的引导
    int storyType = CDataManager::getStoryTypeByLevelId(CFightData::getInstance()->getNowLevelId(), 1);
    std::vector<gameStory> tempStory = CDataManager::getStoryInfoByLevelId((storyState)storyType);
    for(int i = 0; i < tempStory.size(); i++)
    {
        if(tempStory[i].startTime > 0)
        {
            m_storyStartTime = tempStory[i].startTime ;
            m_bHaveStory = true;
            break;
        }
    }
    
    //然后判断是否有游戏开始的剧情
    int startStoryType = CDataManager::getStoryTypeByLevelId(CFightData::getInstance()->getNowLevelId(), 0);
    std::vector<gameStory> startStory = CDataManager::getStoryInfoByLevelId((storyState)startStoryType);
    for(int i = 0; i < startStory.size(); i++)
    {
        if((startStory[i].startTime == 0) && (CPersonalData::getInstance()->getlevelMode() == 0))
        {
            Director::getInstance()->pause();
            m_bStartStory = true;
            storyManagerLayer::getInstance()->setstoryType((storyState)startStoryType);
            storyManagerLayer::getInstance()->setStepNum(1);
            storyManagerLayer::getInstance()->createStoryLayer(this);
            NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(CFightScene::initGameDate), "gameData", NULL);
            break;
        }
    }
    if((!m_bStartStory) || (!m_bStartStory && !m_bHaveStory) || m_bHaveStory)
    {
        initGameDate(NULL);
    }
}

void CFightScene::initGameDate(Ref *pSender)
{
    if(m_bStartStory)
    {
        m_bStartStory = false;
        NotificationCenter::getInstance()->removeObserver(this, "gameData");
    }
    //addMonsterWithIdAndGroupAndPosition(3, eFightGroup_midNotFight, Vec2((END3DPOINT-Start3DPOINT)/2+Start3DPOINT, -200),eAttackType_min);
    
    //测试用，增加一个中立塔
    
    //addMonsterWithIdAndGroupAndPosition(3, eFightGroup_midFight, Vec2((END3DPOINT-Start3DPOINT)/4+Start3DPOINT, -200),eAttackType_tower);
    
    int type = CDataManager::getGuideStepNum();
    int num = CDataManager::getGuideInfoSum();
    if((type <= CDataManager::getGuideInfoSum()) && (CPersonalData::getInstance()->getlevelMode() == 0))
    {
        if(CFightData::getInstance()->getFightType() != FIGHT_PVP_TYPE)
        {
            guiderManagerLayer::getInstance()->setStepNum(0);
        }
        if(CFightData::getInstance()->getNowLevelId() == 1)
        {
            guiderManagerLayer::getInstance()->setGuideType(GUIDE_ADD_LINE);
            if(!(CDataManager::getEndedBy(GUIDE_ADD_LINE)))
            {
                guiderManagerLayer::getInstance()->setStepNum(1);
                guiderManagerLayer::getInstance()->createLayer(this);
            }
            else{
                m_bifNorAddSolider = true;
                scheduleUpdate();
            }
            
        }
        else if(!(CDataManager::getEndedBy(GUIDE_ADD_CIRCLE)) && (CFightData::getInstance()->getNowLevelId() == 2))
        {
            guiderManagerLayer::getInstance()->setGuideType(GUIDE_ADD_CIRCLE);
            guiderManagerLayer::getInstance()->setStepNum(2);
            guiderManagerLayer::getInstance()->createLayer(this);
        }
        else if(!(CDataManager::getEndedBy(GUIDE_ESCORT_TASK)) && (CFightData::getInstance()->getNowLevelId() == 4))
        {
            m_bifNorAddSolider = true;
            guiderManagerLayer::getInstance()->setGuideType(GUIDE_ESCORT_TASK);
            guiderManagerLayer::getInstance()->setStepNum(1);
            guiderManagerLayer::getInstance()->createLayer(this);
        }
        else if(!(CDataManager::getEndedBy(GUIDE_ADD_RECTANGLE)) && (CFightData::getInstance()->getNowLevelId() == 5))
        {
            guiderManagerLayer::getInstance()->setGuideType(GUIDE_ADD_RECTANGLE);
            guiderManagerLayer::getInstance()->setStepNum(1);
            guiderManagerLayer::getInstance()->createLayer(this);
        }
        else if(!(CDataManager::getEndedBy(GUIDE_ADD_V)) && (CFightData::getInstance()->getNowLevelId() == 8))
        {
            guiderManagerLayer::getInstance()->setGuideType(GUIDE_ADD_V);
            guiderManagerLayer::getInstance()->setStepNum(1);
            guiderManagerLayer::getInstance()->createLayer(this);
        }
        //        else if(!(CDataManager::getEndedBy(GUIDE_GET_FLAG)) && (CFightData::getInstance()->getNowLevelId() == 11))
        //        {
        //            m_bifNorAddSolider = true;
        //            guiderManagerLayer::getInstance()->setGuideType(GUIDE_GET_FLAG);
        //            guiderManagerLayer::getInstance()->setStepNum(1);
        //            guiderManagerLayer::getInstance()->createLayer(this);
        //        }
        else if(!(CDataManager::getEndedBy(GUIDE_USE_MANGONEL)) && (CFightData::getInstance()->getNowLevelId() == 15))//投石机引导
        {
            guiderManagerLayer::getInstance()->setGuideType(GUIDE_USE_MANGONEL);
            m_bifNorAddSolider = false;
            NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(CFightScene::removeGuide), "removeGuide", NULL);
            guiderManagerLayer::getInstance()->setStepNum(1);
            guiderManagerLayer::getInstance()->createLayer(this);
        }
        else if(!(CDataManager::getEndedBy(GUIDE_SHOW_DRAGON)) && (CFightData::getInstance()->getNowLevelId() == 18))
        {
            m_bifNorAddSolider = true;
            guiderManagerLayer::getInstance()->setGuideType(GUIDE_SHOW_DRAGON);
            guiderManagerLayer::getInstance()->setStepNum(1);
            guiderManagerLayer::getInstance()->createLayer(this);
        }
        else if(!(CDataManager::getEndedBy(GUIDE_GET_TOWER)) && (CFightData::getInstance()->getNowLevelId() == 21))
        {
            m_bifNorAddSolider = true;
            guiderManagerLayer::getInstance()->setGuideType(GUIDE_GET_TOWER);
            guiderManagerLayer::getInstance()->setStepNum(1);
            guiderManagerLayer::getInstance()->createLayer(this);
        }
        else if(!(CDataManager::getEndedBy(GUIDE_SHOW_SWORDSMEN)) && (CFightData::getInstance()->getNowLevelId() == 23))
        {
            m_bifNorAddSolider = true;
            guiderManagerLayer::getInstance()->setGuideType(GUIDE_SHOW_SWORDSMEN);
            guiderManagerLayer::getInstance()->setStepNum(1);
            guiderManagerLayer::getInstance()->createLayer(this);
        }
        else if(!(CDataManager::getEndedBy(GUIDE_KILL_BOSS)) && (CFightData::getInstance()->getNowLevelId() == 24))
        {
            m_bifNorAddSolider = true;
            guiderManagerLayer::getInstance()->setGuideType(GUIDE_KILL_BOSS);
            guiderManagerLayer::getInstance()->setStepNum(1);
            guiderManagerLayer::getInstance()->createLayer(this);
        }
        else if(!(CDataManager::getEndedBy(GUIDE_SPRING_WATER)) && (CFightData::getInstance()->getNowLevelId() == 28))
        {
            m_bifNorAddSolider = true;
            guiderManagerLayer::getInstance()->setGuideType(GUIDE_SPRING_WATER);
            guiderManagerLayer::getInstance()->setStepNum(1);
            guiderManagerLayer::getInstance()->createLayer(this);
        }
        else if(!(CDataManager::getEndedBy(GUIDE_TOWER_LEVEL)) && (CFightData::getInstance()->getNowLevelId() == 1001))
        {
            guiderManagerLayer::getInstance()->setGuideType(GUIDE_TOWER_LEVEL);
            guiderManagerLayer::getInstance()->setStepNum(1);
            guiderManagerLayer::getInstance()->createLayer(this);
            m_bUpdateTime = false;
        }
        else
        {
            m_bifNorAddSolider = true;
            if(CFightData::getInstance()->getNowLevelId() == 3)//如果是第三关为防止剧情出现之前可以造兵，在这里加了限制
            {
                if(m_bHaveStory)
                {
                    m_bifNorAddSolider = false;
                }
            }
            scheduleUpdate();
        }
    }
    else{
        m_bifNorAddSolider = true;
        scheduleUpdate();
    }
}

void CFightScene::initAddSolider()
{
    //加载模板
    g_rGemertricRecognizer = new GeometricRecognizer();
    g_rGemertricRecognizer->loadTemplates();
    
    m_touchStar = ParticleSystemQuad::create("plist/star.plist") ;
    this->addChild(m_touchStar, 10);
    m_touchStar->setVisible(false);
}



void CFightScene::addCloud()
{
    //CSharedPlist::getInstance()->pushPlist("plist/mainMenu.plist");
    
    
    m_pLayer = LayerColor::create(Color4B(0, 0, 0, 0));
    addChild(m_pLayer, 900);
    m_pLayer->setPosition(Vec2::ZERO);
    
    
    Sprite *pCloud5 = Sprite::createWithSpriteFrameName("pvpcloud5.png");
       pCloud5->cocos2d::Node::setScale(1/0.4);
    pCloud5->setPosition(pCloud5->getBoundingBox().size.width*0.5,pCloud5->getBoundingBox().size.height*0.5);
    
    pCloud5->setOpacity(255);
    m_pLayer->addChild(pCloud5,3,ePvp_Cloud5Tag);
    
    
    Sprite *pCloud3 = Sprite::createWithSpriteFrameName("pvpcloud3.png");
    pCloud3->setScale(1.7/0.4);
    pCloud3->setPosition(pCloud3->getBoundingBox().size.width*0.5,pCloud3->getBoundingBox().size.height*0.5);
    pCloud3->setOpacity(255*0.3);
    m_pLayer->addChild(pCloud3,1,ePvp_Cloud3Tag);
    
    
    Sprite *pCloud1 =  Sprite::createWithSpriteFrameName("pvpcloud1.png");
    pCloud1->setScale(2/0.4);
    pCloud1->setPosition(pCloud1->getBoundingBox().size.width*0.5,pCloud1->getBoundingBox().size.height*0.5);
    m_pLayer->addChild(pCloud1,0,ePvp_Cloud1Tag);
    pCloud1->setOpacity(255*0.1);
    
    //上边
    
    Sprite *pCloud6 = Sprite::createWithSpriteFrameName("pvpcloud6.png");
      pCloud6->cocos2d::Node::setScale(1/0.4);
    pCloud6->setPosition(m_WinSize.width-pCloud6->getBoundingBox().size.width*0.5,m_WinSize.height-pCloud6->getBoundingBox().size.height*0.5);
    m_pLayer->addChild(pCloud6,2,ePvp_Cloud6Tag);
  
    
    pCloud6->setOpacity(255);
    
    
    Sprite *pCloud4 = Sprite::createWithSpriteFrameName("pvpcloud4.png");
    pCloud4->setScale(1.7/0.4);
    pCloud4->setPosition(Vec2(m_WinSize.width-pCloud4->getBoundingBox().size.width*0.5,m_WinSize.height-pCloud4->getBoundingBox().size.height*0.5));
    pCloud4->setOpacity(255*0.3);
    
    m_pLayer->addChild(pCloud4,1,ePvp_Cloud4Tag);
    
    
    Sprite *pCloud2 = Sprite::createWithSpriteFrameName("pvpcloud2.png");
    pCloud2->setScale(2/0.4);
    pCloud2->setPosition(m_WinSize.width-pCloud2->getBoundingBox().size.width*0.5,m_WinSize.height-pCloud2->getBoundingBox().size.height*0.5);
    pCloud2->setOpacity(255*0.1);
    m_pLayer->addChild(pCloud2,0,ePvp_Cloud2Tag);
    
    
}


void CFightScene::initUI(){
    
    
    //初始化背景
    initBG();
    
    
    
    //测试用
    //    Director::getInst7nce()->getScheduler()->setTimeScale(0.5f);
    
    //    m_pSceneParticle = ParticleSystemQuad::create("particleSystem/scene_Snow.plist");
    //    addChild(m_pSceneParticle,1000);
    //    m_pSceneParticle->setPosition(Vec2(m_WinSize.width*0.5f, m_WinSize.height));
    
    
    
    char buf[128];
    cocos2d::Size winsize = Director::getInstance()->getWinSize();
    
    m_bPvpAllMapMove= false;
    
    
    //任务指标
    m_ptaskBg = Sprite::createWithSpriteFrameName("taskbag.png");
    m_ptaskBg->setPosition(m_WinSize.width*0.5,m_WinSize.height*0.5);
    m_layer->addChild(m_ptaskBg, 11);
    m_ptaskBg->setScale(4.0f);
    //m_ptaskBg->runAction(Hide::create());
    m_ptaskBg->setVisible(false);
    
    
    Sprite *ptaskPic = Sprite::createWithSpriteFrameName("taskpic.png");
    ptaskPic->setAnchorPoint(Vec2(0, 0.5));
    ptaskPic->setPosition(0, m_ptaskBg->getContentSize().height / 2);
    
    m_ptaskBg->addChild(ptaskPic, 5,101);
    
    //任务内容
    m_pTaskLable = Label::createWithTTF("收集碎片：1/4","fonts/cuti.ttf",24);
    m_pTaskLable->setAnchorPoint(Vec2(0, 0.5));
    m_pTaskLable->setPosition(ptaskPic->getPositionX() + ptaskPic->getContentSize().width + 10, ptaskPic->getPositionY());
    m_ptaskBg->addChild(m_pTaskLable);
    
    //m_pTaskLable->setPosition(m_WinSize.width*0.5,m_WinSize.height*0.5);
    // addChild(m_pTaskLable,10);
    
    Sprite* normal = Sprite::createWithSpriteFrameName("pause.png");
    Sprite* select = Sprite::createWithSpriteFrameName("pause.png");
    LYMenuItemSprite* pPauseItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(CFightScene::startBlowCallBack, this));
    m_pauseMenu = Menu::create(pPauseItem, NULL);
    
    
    //pvp模式
    if(CFightData::getInstance()->getFightType() == FIGHT_PVP_TYPE)
    {
        m_ptaskBg->setVisible(false);
        m_pauseMenu ->setVisible(false);
    }
    
    
    CommonData::enlargeItemSprite(pPauseItem, 2);
    pPauseItem->setTag(GAME_PAUSE_TAG);
    m_pauseMenu->setPosition(m_WinSize.width-pPauseItem->getContentSize().width*0.5,m_WinSize.height*0.94);
    m_layer->addChild(m_pauseMenu);
    
    if(((!(CDataManager::getEndedBy(GUIDE_ADD_LINE)) && (CFightData::getInstance()->getNowLevelId() == 1)) ||
        (!(CDataManager::getEndedBy(GUIDE_ADD_CIRCLE)) && (CFightData::getInstance()->getNowLevelId() == 2)) ||
        (!(CDataManager::getEndedBy(GUIDE_ADD_RECTANGLE)) && (CFightData::getInstance()->getNowLevelId() == 5)) ||
        (!(CDataManager::getEndedBy(GUIDE_ADD_V)) && (CFightData::getInstance()->getNowLevelId() == 8)))
       && (CPersonalData::getInstance()->getlevelMode() == 0))
    {
        m_pauseMenu->setEnabled(false);
    }
    
    if(CFightData::getInstance()->getNowMidLevelId() < TOWER_LEVEL_INDEX&& (CFightData::getInstance()->getFightType() != FIGHT_PVP_TYPE))
    {
        //完成度
        normal = Sprite::createWithSpriteFrameName("finish_word.png");
        select = Sprite::createWithSpriteFrameName("finish_word.png");
        select->setColor(Color3B::GRAY);
        LYMenuItemSprite* pFinishItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(CFightScene::startBlowCallBack, this));
        Menu* finishMenu = Menu::create(pFinishItem, NULL);
        pFinishItem->setTag(LEVEL_STAR_TAG);
        finishMenu->setPosition(m_WinSize.width*0.93,m_WinSize.height*0.84);
        m_layer->addChild(finishMenu);
        
        //获取当前关卡的星级
        //int star = CDataManager::getStarBySmallLevelId(CFightData::getInstance()->getNowLevelId(), MY_PERSION->getlevelMode());
        for(int i = 0; i < 3; i++)
        {
            starShow infp;
            Sprite* sp = Sprite::createWithSpriteFrameName("start_gray.png");
            m_layer->addChild(sp, 100, FIGHT_STAR_ONE + i);
            sp->setPosition(Vec2(m_WinSize.width*0.9 + i * 43, m_WinSize.height*0.78));
            infp.tag = FIGHT_STAR_ONE + i;
            infp.star = sp;
            m_starVec.push_back(infp);
        }
    }
    
    //敌我双方标识

    
    if (CFightData::getInstance()->getFightType()!=FIGHT_PVP_TYPE) {   //非pvp战斗才初始化 任务目标
        CFightData::getInstance()->setLevelConditionLab(m_pTaskLable);   //设置任务目标
        CFightData::getInstance()->refreshTheFightCondition();
        
        
    }
    
    
    //添加缩放框
    
    {
        Sprite *ZoomBG = Sprite::createWithSpriteFrameName("fight_ZoomBG.png");
        Sprite *ZoomBlue = Sprite::createWithSpriteFrameName("fight_ZoomBlue.png");
        Sprite *ZoomButton = Sprite::createWithSpriteFrameName("fight_ZoomButton.png");
        
        m_pZommControl = cocos2d::extension::ControlSlider::create(ZoomBG, ZoomBlue, ZoomButton);
        m_pZommControl->setMaximumValue(MAX_CAMERA_Y-MIN_CAMERA_Y);
        m_pZommControl->setMinimumValue(0);
        
        m_pZommControl->setValue(_camera->getPositionY()-MIN_CAMERA_Y);
        m_pZommControl->setPosition(Vec2(m_WinSize.width*0.97f, m_WinSize.height*0.5f));
        addChild(m_pZommControl,10,FIGHT_SHENGSUOTIAOTAG);
        
        m_pZommControl->setRotation(-90);
        
        m_pZommControl->addTargetWithActionForControlEvents(this, cccontrol_selector(CFightScene::soliderCallBack), cocos2d::extension::Control::EventType::VALUE_CHANGED);
        
        //缩放按钮
        Sprite* zoomIn = Sprite::createWithSpriteFrameName("fight_ZoomIn.png");
        Sprite* zoominDown =Sprite::createWithSpriteFrameName("fight_ZoomIn.png");
        zoominDown->setColor(Color3B::GRAY);
        
        Sprite* zoomOut = Sprite::createWithSpriteFrameName("fight_ZoomOut.png");
        Sprite* zoomOutDown = Sprite::createWithSpriteFrameName("fight_ZoomOut.png");
        zoomOutDown->setColor(Color3B::GRAY);
        
        LYMenuItemSprite* zoomInButton = LYMenuItemSprite::create(zoomIn, zoominDown, CC_CALLBACK_1(CFightScene::callBackZoomIN, this));
        zoomInButton->setPosition(m_pZommControl->getPosition()+Vec2(0,-m_pZommControl->getBoundingBox().size.height*0.5f-zoomInButton->getBoundingBox().size.height*0.8f));
        
        LYMenuItemSprite* zoomOutButton = LYMenuItemSprite::create(zoomOut, zoomOutDown, CC_CALLBACK_1(CFightScene::callBackZoomOut, this));
        zoomOutButton->setPosition(m_pZommControl->getPosition()+Vec2(0,m_pZommControl->getBoundingBox().size.height*0.5f+zoomOutButton->getBoundingBox().size.height*0.8f));
        
        Menu* tempMenu = Menu::create(zoomInButton,zoomOutButton, NULL);
        tempMenu->setPosition(Vec2::ZERO);
        tempMenu->setAnchorPoint(Vec2::ZERO);
        
        addChild(tempMenu,10,FIGHT_SHENGSUOMENUTAG);
        
    }
    //根据数据初始化最大能量值，此处测试用
    //    CFightData::getInstance()->setMaxEnergy(100);
    //    CFightData::getInstance()->setNowEnergy(100);
    
    
    if (CFightData::getInstance()->getTheLevelCondition()==eLevelCondition_TowerDefense) {
        initTDUI();
        
        
    }else if(m_ePVPMode!=EPVPMode_REC) {
        initNormalUI();
    }
    
    
    //战斗开始动画调整
    
    if(CFightData::getInstance()->getFightType() == FIGHT_PVP_TYPE)
    {
        
        char str[50] = {0};
        
        m_iHour =  m_iAllTime/3600;
        m_iMin = m_iAllTime%3600/60;
        m_iSec = m_iAllTime%60;
        
        sprintf(str,"倒计时  %02d:%02d",m_iMin,m_iSec);
        
        m_pLableTime = Label::createWithTTF(str,"fonts/cuti.ttf",35);
        
        m_pLableTime->setPosition(m_winSize.width*0.7,m_winSize.height*0.95);
        
        addChild(m_pLableTime,99,FIGHT_DAOJISHTAG);
        
        Sprite *PvpClose = Sprite::createWithSpriteFrameName("closepvp.png");
        
        Sprite *pvpCloseSelect = Sprite::createWithSpriteFrameName("closepvp.png");
        pvpCloseSelect->setColor(Color3B::GRAY);
        
        Sprite *pvpCloseDisabel  = Sprite::createWithSpriteFrameName("closepvp.png");
        pvpCloseDisabel->setColor(Color3B::GRAY);
        
        MenuItemSprite *pVpCloseItem = MenuItemSprite::create(PvpClose,pvpCloseSelect,pvpCloseDisabel,CC_CALLBACK_1(CFightScene::btnCallBack, this));
        pVpCloseItem->setTag(ePVP_BACK_TAG);
        
        pVpCloseItem->setPosition(m_winSize.width*0.9,m_winSize.height*0.95);
        
        m_pvpCloseMenu = Menu::create(pVpCloseItem,NULL);
        
        m_pvpCloseMenu->setPosition(Vec2(0,0));
        
        addChild(m_pvpCloseMenu,99,FIGHT_GUANBITAG);
        
        if(CPersonalData::getInstance()->getFromVideoList()==false)
        {
            RunEnergyAction((Node*)m_pLableTime,1000,1000);
            
            RunEnergyAction((Node*)m_pvpCloseMenu,1000,1000);
            
            Sprite *pEnergy = (Sprite *)m_layer->getChildByTag(FIGHT_ENERGY_BGTAG);
            Sprite *pQiu = (Sprite*)m_layer->getChildByTag(FIGHT_ENERGY_QIUTAG);
            Sprite *pEnergyCao = (Sprite*)m_layer->getChildByTag(FIGHT_ENERGY_CAOTAG);
            Sprite *pEnergyLine =  (Sprite*)getChildByTag(FIGHT_ENERGY_LINETAG);
            
            
            
            Sprite *pDrawLINE = (Sprite*)m_layer->getChildByTag(200 + eFighterType_line);
            Sprite *pTriangle = (Sprite*)m_layer->getChildByTag(200 + eFighterType_triangle);
            Sprite *pRectangle = (Sprite *)m_layer->getChildByTag(200 + eFighterType_square);
            Sprite *pCricle = (Sprite*)m_layer->getChildByTag(200 + eFighterType_circle);
            Sprite *pV = (Sprite*)m_layer->getChildByTag(200 + eFighterType_v);
            
            Sprite *pSoldierNum = (Sprite*)m_layer->getChildByTag(FIGHT_SOLDIER_NUMTAG);
            
            Sprite *pSmallMap = (Sprite*)getChildByTag(FIGHT_SMALLMAP);
            
            Sprite *pShengsuo = (Sprite*)getChildByTag(FIGHT_SHENGSUOTIAOTAG);
            
            Menu *pShengsuoMenu  = (Menu*)getChildByTag(FIGHT_SHENGSUOMENUTAG);
            
            
            
            if(pShengsuo)
            {
                
                RunEnergyAction(pShengsuo,pShengsuo->getPositionX()*2,pShengsuo->getPositionY());
    
            }
            
            
            if(pShengsuoMenu)
            {
                
                RunEnergyAction(pShengsuoMenu,pShengsuo->getPositionX()*2,pShengsuo->getPositionY());
                
            }
            
            if(pSoldierNum)
            {
                pSoldierNum->setVisible(true);
                
                RunEnergyAction(pSoldierNum,pSoldierNum->getPositionX(),1000);
            }
            
            Sprite *pSoldierNUmBg =  (Sprite*)m_layer->getChildByTag(FIGHT_SOLDIER_NUMBGTAG);
            
            if(pSoldierNUmBg)
            {
                Sprite * p= (Sprite*)pSoldierNUmBg->getChildByTag(100);
                RunEnergyAction(p,p->getPositionX(),1000);
                RunEnergyAction(pSoldierNUmBg,pSoldierNUmBg->getPositionX(),1000);
            }
            
            if(pEnergyLine)
            {
                RunEnergyAction(pEnergyLine,-1000,1000);
            }
            
            if(pDrawLINE)
            {
                
                RunEnergyAction(pDrawLINE,-1000,1000);
            }
            
            if(pTriangle)
            {
    
                RunEnergyAction(pTriangle,-1000,1000);
                
            }
            
            if(pSmallMap)
            {
                pSmallMap->setVisible(true);
                pSmallMap->cocos2d::Node::setPosition(pSmallMap->getBoundingBox().size.width*0.5,m_WinSize.height*0.08);
                
                RunEnergyAction(pSmallMap,-1000,-1000);
            }
            
            m_pCurView->setVisible(true);
            RunEnergyAction(m_pCurView,-1000,-1000);
            
            if(pRectangle)
            {
                RunEnergyAction(pRectangle);
            }
            
            if(pCricle)
            {
                RunEnergyAction(pCricle);
            }
            
            if(pV)
            {
                RunEnergyAction(pV);
            }
            
            if(pEnergy)
            {
               // m_pSwitchItem1->setVisible(true);

                
                int x = pEnergy->getPositionX();
                int y = pEnergy->getPositionY();
                pEnergy->setPosition(-1000,1000);
                pEnergy->runAction(Sequence::create(MoveTo::create(1,Vec2(x,y)),CallFunc::create([=]{
                    fightStartAnimateInit();
                    schedule(schedule_selector(CFightScene::updateAnimateGuide), 0.5f);
                    
                    
                    
                }),NULL));
            }
            
            if(pQiu)
            {
                int x=  pQiu->getPositionX();
                int y = pQiu->getPositionY();
                pQiu->setPosition(-1000,1000);
                pQiu->runAction(MoveTo::create(1,Vec2(x,y)));
                
            }
            
            if(pEnergyCao)
            {
                int x=  pEnergyCao->getPositionX();
                int y = pEnergyCao->getPositionY();
                pEnergyCao->setPosition(-1000,1000);
                pEnergyCao->runAction(MoveTo::create(1,Vec2(x,y)));
            }
            
            
            Label *pLabelMagic = (Label*)getChildByTag(998);
            if(pLabelMagic)
            {
                RunEnergyAction(pLabelMagic,m_winSize.width*0.5,m_winSize.height*0.1);
                
            }
            
            Sprite *pg = (Sprite*)getChildByTag(999);
            if(pg)
            {
                RunEnergyAction(pg ,m_winSize.width*0.5,m_winSize.height*0.05);
            }
            
            
            RunEnergyAction(m_pSwitchItem1,m_pSwitchItem1->getPositionX(),m_pSwitchItem1->getPositionY());
            Vec2 t = CFightData::getInstance()->m_pSmallMapLayer->getPosition();
            

            
            CFightData::getInstance()->m_pSmallMapLayer->runAction(Sequence::create(MoveTo::create(1,Vec2(t.x,t.y)),Show::create(),CallFunc::create(CC_CALLBACK_0(CFightScene::OnToushicheShow, this)),NULL));

        
            

               // CFightData::getInstance()->m_pSmallMapLayer->setVisible(true);
        }
      
        RunEnergyAction(m_pSwitchItem1,m_pSwitchItem1->getPositionX(),m_pSwitchItem1->getPositionY());

      }
    

}


void CFightScene::OnToushicheShow()
{
    //m_pSwitchItem1->setVisible(true);
    
}
void CFightScene::RunEnergyAction(Node *p,int iTox,int iToY)
{
    int x = p->getPositionX();
    int y = p->getPositionY();
    
    p->setPosition(iTox,iToY);
    
    p->runAction(MoveTo::create(1,Vec2(x,y)));
}

void CFightScene::initBG(){
    
    
    int level = CFightData::getInstance()->getNowMidLevelId();
    
    
    int whichOne = level%eFightBG_error;
    
    
    __String BGName="";  //相关控制参数
    __String GroundName="";
    __String particleName="";
    __String pngName="";//非粒子的一些特效
    
    
    std::vector<std::string> allParticle;
    
    std::vector<std::string> allPng;
    
    //测试用
    //    whichOne = eFightBG_Snow1;
    
    
    //如果是回放界面，则一定是冰雪地图
    if (m_ePVPMode==EPVPMode_REC) {
        whichOne = eFightBG_Snow1;
    }
    
    switch (whichOne) {
        case eFightBG_Snow1:
            BGName="models/gameScene/fightBG_Snow1_1.png";
            GroundName="models/gameScene/fightBG_Snow1_2.png";
            
            allParticle.push_back("particleSystem/scene_Snow.plist");  //小雪
            allParticle.push_back("");
            
            //            particleName="particleSystem/scene_Snow.plist";
            break;
            
        case eFightBG_Snow1_night:
            BGName="models/gameScene/fightBG_Snow1Night_1.png";
            GroundName="models/gameScene/fightBG_Snow1Night_2.png";
            
            allParticle.push_back("particleSystem/scene_Snow.plist");  //小雪
            allParticle.push_back("");
            
            break;
            
        case eFightBG_Forest1:
            BGName="models/gameScene/fightBG_Forest1_1.png";
            GroundName="models/gameScene/fightBG_Forest1_2.png";
            
            allParticle.push_back("particleSystem/rain.plist");  //小雨
            allParticle.push_back("");
            
            allPng.push_back("models/gameScene/fightBGEffect_fog.png");
            //            allPng.push_back("");
            
            
            break;
            
        case eFightBG_Forest2:
            BGName="models/gameScene/fightBG_Forest2_1.png";
            GroundName="models/gameScene/fightBG_Forest2_2.png";
            
            allParticle.push_back("particleSystem/rain.plist");  //小雪
            allParticle.push_back("");
            
            break;
            
        case eFightBG_Wall1:
            BGName="models/gameScene/fightBG_Wal1_1.png";
            GroundName="models/gameScene/fightBG_Wal1_2.png";
            
            allParticle.push_back("particleSystem/rain.plist");  //小雪
            allParticle.push_back("");
            
            break;
            
//        case eFightBG_Desert:
//            BGName="models/gameScene/fightBG_Desert1_1.png";
//            GroundName="models/gameScene/fightBG_Desert1_2.png";
//            
//            allParticle.push_back("particleSystem/rain.plist");  //小雪
//            allParticle.push_back("");
//            
//            break;
            
            
        default:
            BGName="models/gameScene/fightBG_Forest1_1.png";
            GroundName="models/gameScene/fightBG_Forest1_2.png";
            
            allParticle.push_back("particleSystem/rain.plist");  //小雪
            allParticle.push_back("");
            
            
            break;
    }
    
    if (allParticle.size()>0) { //随机一种场景效果
        
        int random = rand()%allParticle.size();
        
        particleName = allParticle.at(random);
        
    }
    
    if(allPng.size()>0){
        
        int random = rand()%allPng.size();
        
        pngName = allPng.at(random);
        
        //赋值，具体操作由update完成
        
        m_strBGEffect=pngName.getCString();
    }
    
    
    
    
    //初始化背景
    Sprite3D* tempTarget = Sprite3D::create();
    Sprite* temp = Sprite::create(BGName.getCString());
    temp->getTexture()->setAliasTexParameters();
    temp->setAnchorPoint(Vec2(0.5f, 0));
    temp->setScale(2);
    
    tempTarget->addChild(temp);
    
    
    
    CFightManager::addCamera(tempTarget);
    
    tempTarget->setRotation3D(Vec3(-60, 0, 0));
    tempTarget->setPosition3D(Vec3(temp->getBoundingBox().size.width*0.71,-1,-700));
    
    layer3D->addChild(tempTarget);
    
    tempTarget->setForceDepthWrite(true);
    
    
    Sprite3D* tempTarget2 = Sprite3D::create();
    Sprite* temp2 = Sprite::create(GroundName.getCString());
    temp2->getTexture()->setAliasTexParameters();
    temp2->setAnchorPoint(Vec2(0.5f, 0));
    temp2->setScale(2);
    
    tempTarget2->addChild(temp2);
    
    CFightManager::addCamera(tempTarget2);
    
    tempTarget2->setRotation3D(Vec3(90, 0, 0));  //依然是那个坑，如果旋转角度为-90度，就会出现渲染错误
    tempTarget2->setPosition3D(Vec3(tempTarget->getPositionX(), tempTarget->getPositionY(), tempTarget->getPosition3D().z));
    layer3D->addChild(tempTarget2,0,FIGHT_BIGMAPTAG);
    
    //初始化场景粒子
    if(particleName.compare("")){
        
        for (int i=0; i<3; i++) {  //总共层次
            
            Layer* target = Layer::create();
            //        target.Zorder=i;
            //
            //        target.particleLayer = Layer::create();
            addChild(target,700-i);  //i越高，则离屏幕越远
            
            //按层次来初始化粒子，i越低，则需要粒子越多
            for (int num=0; num<7-i; num++) {    //最少需要两个粒子才能覆盖屏幕，因此最底层，也需要能初始化两个粒子
                
                ParticleSystemQuad *m_pSceneParticle = ParticleSystemQuad::create(particleName.getCString());
                target->addChild(m_pSceneParticle);
                m_pSceneParticle->setPosition(Vec2(m_WinSize.width*0.3f*(num+1), m_WinSize.height));
                
                m_pSceneParticle->setPositionType(cocos2d::ParticleSystem::PositionType::RELATIVE);
                
            }
            
            m_VecSceneParticle.push_back(target);
            
        }
        
    }
    
    //初始化场景png
    if (m_strBGEffect!="") {
        
        for (int i=0; i<3; i++) {
            
            Layer* target = Layer::create();
            addChild(target,1000-i);
            
            m_vecScenePng.push_back(target);
            
        }
    }
    
}



void CFightScene::initNormalUI(){
    
    char buf[128];
    
    
    //左上边能量槽
    
    Sprite *pEnemyLine = Sprite::createWithSpriteFrameName("enemy_bottom.png");
    pEnemyLine->setPosition(m_WinSize.width*0.19,m_WinSize.height*0.94);
    addChild(pEnemyLine,0,FIGHT_ENERGY_LINETAG);
    
    
    //能量球
    Sprite *pEnemy = Sprite::createWithSpriteFrameName("enemgypic.png");
    pEnemy->setPosition(pEnemyLine->getPositionX()-pEnemyLine->getBoundingBox().size.width*0.42f,m_WinSize.height*0.935);
    m_layer->addChild(pEnemy, 1,FIGHT_ENERGY_QIUTAG);
    pEnemy->setScale(1.5f);
    auto animation = Animation::create();
    for (int i = 1; i < 11; ++i) {
        char strName[128];
        sprintf(strName, "enemgy_%d.png", i);
        animation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(strName));
    }
    animation->setDelayPerUnit(0.15f);
    auto action = Animate::create(animation);
    pEnemy->runAction(RepeatForever::create(action));
    
    
    //能量槽
    Sprite *temp  = Sprite::createWithSpriteFrameName("enemgyline.png");
    m_pTheEnergyBar = ProgressTimer::create(temp);
    cocos2d::Size barSize = m_pTheEnergyBar->getBoundingBox().size;
    m_pTheEnergyBar->setPosition(cocos2d::Point(m_WinSize.width*0.188f,m_WinSize.height*0.94f));
    m_pTheEnergyBar->setType(ProgressTimer::Type::BAR);
    m_pTheEnergyBar->setMidpoint(cocos2d::Point(0, 0.5));
    m_pTheEnergyBar->setBarChangeRate(cocos2d::Point(1, 0));
    m_layer->addChild(m_pTheEnergyBar,0,FIGHT_ENERGY_CAOTAG);
    //    addMonsterWithIdAndGroupAndPosition(1, eFightGroup_Freind, Vec2(Start3DPOINT, 0));
    //    addMonsterWithIdAndGroupAndPosition(1, eFightGroup_Enemy, Vec2(END3DPOINT+100, 0));
    
    
    //enemgyline.png
    Sprite *pEnemyProgrss = Sprite::createWithSpriteFrameName("enemgybg.png");
    pEnemyProgrss->setPosition(m_WinSize.width*0.19,m_WinSize.height*0.94);
    m_layer->addChild(pEnemyProgrss,0,FIGHT_ENERGY_BGTAG);
    
    float startPos = m_pTheEnergyBar->getPositionX() - m_pTheEnergyBar->getContentSize().width*0.5;//记录进度条的起始位置
    float width = m_pTheEnergyBar->getBoundingBox().size.width;//进度条宽度
    //各兵种cd
    Sprite* m_line = Sprite::createWithSpriteFrameName("enemy_line.png");
    m_layer->addChild(m_line, 1 , 200 + eFighterType_line);
    float enemgyCost = CPersonalData::getInstance()->getTeamSolderByType(eFighterType_line)->energyCost;
    float scale = enemgyCost / CFightData::getInstance()->getMaxEnergy();
    m_line->setPosition(Vec2(startPos + width * scale, m_WinSize.height * 0.905f));
    
    std::vector<FighterInfo*> teamVec = CPersonalData::getInstance()->m_vecMyTeamInfo;
    bool circle = false,triang = false, rec = false, vx=  false;
    for(int i = 0; i < teamVec.size(); i++)
    {
        if(teamVec.at(i)->monsterType == eFighterType_triangle)
        {
            triang = true;
        }
        else if(teamVec.at(i)->monsterType == eFighterType_circle)
        {
            circle = true;
        }
        else if(teamVec.at(i)->monsterType == eFighterType_square)
        {
            rec = true;
        }
        else if(teamVec.at(i)->monsterType == eFighterType_v)
        {
            vx = true;
        }
    }
    if(triang)
    {
        Sprite* m_triangle = Sprite::createWithSpriteFrameName("enemy_triangle.png");
        m_layer->addChild(m_triangle, 1 , 200 + eFighterType_triangle);
        m_triangle->getTexture()->setAntiAliasTexParameters();
        float enemgyCost = CPersonalData::getInstance()->getTeamSolderByType(eFighterType_triangle)->energyCost;
        float scale = enemgyCost / CFightData::getInstance()->getMaxEnergy();
        m_triangle->setPosition(Vec2(startPos + width * scale, m_WinSize.height * 0.905f));
    }
    if(circle)
    {
        Sprite* m_circle = Sprite::createWithSpriteFrameName("enemy_circle.png");
        m_circle->getTexture()->setAntiAliasTexParameters();
        m_layer->addChild(m_circle, 1 , 200 + eFighterType_circle);
        float enemgyCost = CPersonalData::getInstance()->getTeamSolderByType(eFighterType_circle)->energyCost;
        float scale = enemgyCost / CFightData::getInstance()->getMaxEnergy();
        m_circle->setPosition(startPos + width * scale, m_WinSize.height * 0.905f);
    }
    if(rec)
    {
        Sprite* m_rect = Sprite::createWithSpriteFrameName("enemy_rect.png");
        m_rect->getTexture()->setAntiAliasTexParameters();
        m_layer->addChild(m_rect, 1 , 200 + eFighterType_square);
        float enemgyCost = CPersonalData::getInstance()->getTeamSolderByType(eFighterType_square)->energyCost;
        float scale = enemgyCost / CFightData::getInstance()->getMaxEnergy();
        m_rect->setPosition(Vec2(startPos + width * scale, m_WinSize.height * 0.905f));
    }
    
    if(vx)
    {
        Sprite* m_vSp = Sprite::createWithSpriteFrameName("enemy_V.png");
        m_layer->addChild(m_vSp, 1 , 200 + eFighterType_v);
        m_vSp->getTexture()->setAntiAliasTexParameters();
        float enemgyCost = CPersonalData::getInstance()->getTeamSolderByType(eFighterType_v)->energyCost;
        float scale = enemgyCost / CFightData::getInstance()->getMaxEnergy();
        m_vSp->setPosition(Vec2(startPos + width * scale, m_WinSize.height * 0.905f));
    }
    
    
    //初始化人数限制
    
    Sprite* soliderNumBg = Sprite::createWithSpriteFrameName("soliderNumBg.png");
    soliderNumBg->setPosition(Vec2(m_WinSize.width*0.5f,m_WinSize.height*0.94f));
    m_layer->addChild(soliderNumBg,0,FIGHT_SOLDIER_NUMBGTAG);
    
    Sprite* soliderNumIcon = Sprite::createWithSpriteFrameName("soliderNum.png");
    soliderNumIcon->cocos2d::Node::setAnchorPoint(Vec2(0, 0.5));
    soliderNumIcon->setPosition(Vec2(m_WinSize.width*0.44f,m_WinSize.height*0.94f));
    m_layer->addChild(soliderNumIcon,0,FIGHT_SOLDIER_NUMTAG);
    
    __String * num = __String::createWithFormat("%d/%d",CFightData::getInstance()->getNowMonsterNum(),CFightData::getInstance()->getMaxMonsterNum());
    
    //fonts/cuti.ttf
    m_pSoldierNum = Label::createWithTTF(num->getCString(), "fonts/cuti.ttf", 32);
    m_pSoldierNum->setAnchorPoint(Vec2(1, 0.5));
    m_pSoldierNum->setPosition(soliderNumBg->getBoundingBox().size.width* 0.85f,soliderNumBg->getBoundingBox().size.height * 0.5f);
    soliderNumBg->addChild(m_pSoldierNum,0,100);
    
    
    //技能开关
    m_HeroSkill = CPersonalData::getInstance()->getHeroSkillVec();
    
    sprintf(buf, "%s", m_HeroSkill.at(0).theSkill->skillIcon.c_str());
    
    if(CFightData::getInstance()->getFightType() != FIGHT_PVP_TYPE)
    {
        Sprite* normal = Sprite::createWithSpriteFrameName(buf);
        Sprite* select = Sprite::createWithSpriteFrameName(buf);
//        select->setColor(Color3B::GRAY);
//        LYMenuItemSprite* pBlowItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(CFightScene::startBlowCallBack, this));
//        pBlowItem->setScale(3.0f);
//        m_pBlowMenu = Menu::create(pBlowItem, NULL);
//        pBlowItem->setTag(SKILL_BLOW_TAG);
//        m_pBlowMenu->setPosition(m_WinSize.width*0.915,m_WinSize.height*0.2);
//        m_layer->addChild(m_pBlowMenu,1,SKILL_BLOW_TAG);
//        //pBlowItem->setContentSize(cocos2d::Size(normal->getBoundingBox().size.width * 1.5f, normal->getBoundingBox().size.height * 1.5f));
//        m_pBlowMenu->setOpacity(0);
//        m_pBlowMenu->setVisible(false);
        
        sprintf(buf, "%s", m_HeroSkill.at(1).theSkill->skillIcon.c_str());
        normal = Sprite::createWithSpriteFrameName(buf);
        select = Sprite::createWithSpriteFrameName(buf);
        select->setColor(Color3B::GRAY);
        LYMenuItemSprite* pCutItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(CFightScene::startBlowCallBack, this));
        m_pCutMenu = Menu::create(pCutItem, NULL);
        pCutItem->setScale(3.0f);
        pCutItem->setTag(SKILL_CUT_FRUIT_TAG);
        m_pCutMenu->setPosition(m_WinSize.width*0.915,m_WinSize.height*0.2);
        m_layer->addChild(m_pCutMenu,1,SKILL_CUT_FRUIT_TAG);
        m_pCutMenu->setOpacity(0);
        m_pCutMenu->setVisible(false);
        
        normal = Sprite::createWithSpriteFrameName("atkpic.png");
        select = Sprite::createWithSpriteFrameName("atkpic.png");
        select->setColor(Color3B::GRAY);
        LYMenuItemSprite* pSwitchItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(CFightScene::startBlowCallBack, this), "music/skillUI.mp3");
        pSwitchItem->setScale(0.8f);
        Menu* pSwitchMenu = Menu::create(pSwitchItem, NULL);
        pSwitchItem->setTag(SKILL_START_TAG);
        pSwitchMenu->setPosition(m_WinSize.width*0.915,m_WinSize.height*0.2);
        m_layer->addChild(pSwitchMenu, 1, SKILL_START_TAG);
        pSwitchMenu->setVisible(false);
        if((CDataManager::getEndedBy(GUIDE_SKILL_CUT)) || (CFightData::getInstance()->getNowLevelId() >= 9))
        {
            pSwitchMenu->setVisible(true);
            m_pCutMenu->setVisible(true);
        }
//        if(CDataManager::getEndedBy(GUIDE_SKILL_BLOW) || (CFightData::getInstance()->getNowLevelId() >= 18))//屏蔽吹风
//        {
//            pSwitchMenu->setVisible(true);
//            //m_pBlowMenu->setVisible(true);
//        }
    }
    
}

void CFightScene::initTDUI(){
    
    m_pBuildingControll = CBuildingControllLayer::create();
    addChild(m_pBuildingControll,1000);
    m_pBuildingControll->setTheBuildingInfo(NULL);
    
    
    Sprite* stoneBG = Sprite::createWithSpriteFrameName("TD_StoneBG.png");
    stoneBG->setPosition(Vec2(m_WinSize.width*0.1f, m_WinSize.height*0.95f));
    m_layer->addChild(stoneBG);
    
    Sprite* stone = Sprite::createWithSpriteFrameName("TD_Stone.png");
    stone->setPosition(Vec2(stoneBG->getBoundingBox().size.width*0.05f, stoneBG->getBoundingBox().size.height*0.5f));
    stoneBG->addChild(stone);
    
    m_pStoneLable = Label::createWithSystemFont("0", "Arial", 30);
    m_pStoneLable->setPosition(Vec2(stoneBG->boundingBox().size.width*0.5f, stoneBG->getBoundingBox().size.height*0.5f));
    stoneBG->addChild(m_pStoneLable);
    
    
    
    Sprite* lifeBG = Sprite::createWithSpriteFrameName("TD_LifeBG.png");
    lifeBG->setPosition(Vec2(m_WinSize.width*0.28f, m_WinSize.height*0.95f));
    m_layer->addChild(lifeBG);
    
    Sprite* life = Sprite::createWithSpriteFrameName("TD_Life.png");
    life->setPosition(Vec2(lifeBG->getBoundingBox().size.width*0.05f, lifeBG->getBoundingBox().size.height*0.5f));
    lifeBG->addChild(life);
    
    m_pLifeLable = Label::createWithSystemFont("10", "Arial", 30);
    m_pLifeLable->setPosition(Vec2(lifeBG->boundingBox().size.width*0.5f, lifeBG->getBoundingBox().size.height*0.5f));
    lifeBG->addChild(m_pLifeLable);
    
    
    Sprite* help = Sprite::createWithSpriteFrameName("TD_Help.png");
    Sprite* help_down = Sprite::createWithSpriteFrameName("TD_Help.png");
    help_down->setColor(Color3B::GRAY);
    LYMenuItemSprite* pSwitchItem = LYMenuItemSprite::create(help, help_down, CC_CALLBACK_1(CFightScene::OnCallBackTDHelp, this));
    Menu* pSwitchMenu = Menu::create(pSwitchItem, NULL);
    pSwitchMenu->setPosition(Vec2::ZERO);
    pSwitchMenu->setAnchorPoint(Vec2::ZERO);
    pSwitchItem->setPosition(Vec2(m_WinSize.width*0.03f, m_WinSize.height*0.85f));
    m_layer->addChild(pSwitchMenu);
    
    
    
    Sprite* turnBG = Sprite::createWithSpriteFrameName("TD_StoneBG.png");
    turnBG->setPosition(Vec2(m_WinSize.width*0.45f, m_WinSize.height*0.95f));
    m_layer->addChild(turnBG);
    
    m_pTrunLable = Label::createWithSystemFont("波数:1/10", "Arial", 30);
    m_pTrunLable->setPosition(Vec2(turnBG->boundingBox().size.width*0.5f, turnBG->getBoundingBox().size.height*0.5f));
    turnBG->addChild(m_pTrunLable);
    
    
    
    //    Sprite* clockBG = Sprite::createWithSpriteFrameName("TD_ClockBG.png");
    //    clockBG->setPosition(Vec2(m_WinSize.width*0.45f, m_WinSize.height*0.9f));
    //    m_layer->addChild(clockBG);
    
    
    
    Sprite* clock = Sprite::createWithSpriteFrameName("TD_Clock.png");
    Sprite* clock_down = Sprite::createWithSpriteFrameName("TD_Clock.png");
    clock_down->setColor(Color3B::GRAY);
    m_pClockButton = LYMenuItemSprite::create(clock, clock_down, CC_CALLBACK_1(CFightScene::OnCallBackClockOver, this));
    Menu* pClockMenu = Menu::create(m_pClockButton, NULL);
    pClockMenu->setPosition(Vec2::ZERO);
    pClockMenu->setAnchorPoint(Vec2::ZERO);
    m_pClockButton->setPosition(Vec2(m_WinSize.width*0.85f, m_WinSize.height*0.95f));
    m_layer->addChild(pClockMenu,2);
    
    Sprite* clockBG = Sprite::createWithSpriteFrameName("TD_ClockTimeBG.png");
    clockBG->setPosition(m_pClockButton->getPosition());
    m_layer->addChild(clockBG,0);
    
    //能量槽
    Sprite *temp  = Sprite::createWithSpriteFrameName("TD_ClockTime.png");
    
    m_pTheClock = ProgressTimer::create(temp);
    m_pTheClock->setPosition(m_pClockButton->getPosition());
    m_pTheClock->setType(ProgressTimer::Type::RADIAL);
    m_pTheClock->setPercentage(100);
    m_pTheClock->setReverseProgress(true);
    //m_pTheEnergyBar->setAnchorPoint(Vec2(0.44, 0.5));
    //    m_pTheEnergyBar->setMidpoint(cocos2d::Point(0.5f, 0.5));
    //    m_pTheEnergyBar->setBarChangeRate(cocos2d::Point(1, 0));
    m_layer->addChild(m_pTheClock,1);
    
    
    //初始化怪物终点
    
    Sprite3D* target = CFightManager::getTheModelWithPath("fight_CityCircle.png");
    target->setRotation3D(Vec3(90, 0, 0));
    target->getChildByTag(100)->runAction(RepeatForever::create(RotateBy::create(3, 150)));
    CFightManager::addCamera(target);
    layer3D->addChild(target);

    pathPos* tempPath = CFightData::getInstance()->getTheHead();
    Vec3 thePos = CFightData::getInstance()->convertPathPosToPos(tempPath);
    target->setPosition3D( thePos);
    target->setPositionY(12);
    
    
    
}

void CFightScene::stopAllAttack()
{
    //所有敌我双方士兵停止动作
    cocos2d::Vector<CBaseFighter*>allSolider = CFightData::getInstance()->getAllAttacker();
    for (cocos2d::Vector<CBaseFighter*>::iterator it = allSolider.begin(); it != allSolider.end(); it++)
    {
        CBaseFighter* sp = *it;
        if (sp) {
            if ((sp->getAttackType() == eAttackType_monster) && (sp->getFightGroup() == eFightGroup_Enemy)) {
                addNewOBBWithCoords(sp->getPhysicalPosition(), sp);
            }
            
            sp->animateChange(eFighteState_Idle);
            sp->gettheSprite()->stopAllActions();//暂停士兵的所有动作
            
            sp->unscheduleUpdate();
            sp->unscheduleAllCallbacks();
            sp->stopAllActions();
        }
    }
    //所有塔防都停止攻击
    cocos2d::Vector<CBaseFighter*>allTower = CFightData::getInstance()->getAllTowers();
    for (cocos2d::Vector<CBaseFighter*>::iterator it = allTower.begin(); it != allTower.end(); it++)
    {
        CBaseFighter* sp = *it;
        if (sp && (sp->getAttackType() == eAttackType_tower)) {
            sp->animateChange(eFighteState_Idle);
            sp->gettheSprite()->stopAllActions();
            sp->unscheduleUpdate();
        }
    }
}

void CFightScene::beforeCut()
{
    if (CFightData::getInstance()->getNowEnergy()<m_HeroSkill.at(1).theSkill->costEnergy) {
        
        //如果能量值不够。会提示声音。并且弹字
        
        Sprite * thetarget = CFightManager::getTheFlyWord("能量值不足","fonts/cuti.ttf");
        
        thetarget->setPosition(Vec2(m_WinSize.width*0.5f,m_WinSize.height*0.7f));
        addChild(thetarget);
        return;
    }
    m_bGuideBlow = true;
    m_bSmallMapMoveStart = false;
    //所有敌我双方士兵停止动作
    stopAllAttack();
    char buf[257];
    Animation* pLineAnimation = Animation::create();
    for (int i = 1; i < 11; i++)
    {
        sprintf(buf, "CrazyMode_QuesSelect_%d.png", i);
        pLineAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(buf));
    }
    pLineAnimation->setDelayPerUnit(0.1f);
    AnimationCache::getInstance()->addAnimation(pLineAnimation, "skillAct");
    
    CDataManager::saveTaskInfoToDB(6);
    CFightData::getInstance()->addEnergyWith(-m_HeroSkill.at(1).theSkill->costEnergy,eFightGroup_Freind);
    CommonPropLayer* layer = CommonPropLayer::create(GAME_SKILL_CUT);
    m_layer->addChild(layer, 100);
    
}

void CFightScene::beforeBlow()
{
    m_bSmallMapMoveStart = false;
    //判断能量是否足够
    if (CFightData::getInstance()->getNowEnergy()<m_HeroSkill.at(0).theSkill->costEnergy) {
        
        //如果能量值不够。会提示声音。并且弹字
        
        Sprite * thetarget = CFightManager::getTheFlyWord("能量值不足","fonts/cuti.ttf");
        
        thetarget->setPosition(Vec2(m_WinSize.width*0.5f,m_WinSize.height*0.7f));
        addChild(thetarget);
        return;
    }
    m_bGuideBlow = true;
    stopAllAttack();
    CDataManager::saveTaskInfoToDB(6);
    CFightData::getInstance()->addEnergyWith(-m_HeroSkill.at(0).theSkill->costEnergy,eFightGroup_Freind);
    CommonPropLayer* layer = CommonPropLayer::create(GAME_SKILL_BLOW);
    m_layer->addChild(layer, 100);
}

void CFightScene::onEnterTransitionDidFinish(){
    
    //父类不调用onentertransition的话，子类也不会调用
    
    //    CCLOG("pvpNowFightTime--fightStart::%f",CFightData::getInstance()->getAllFightTime());
    
    CBaseLayer::onEnterTransitionDidFinish();
    
    //    CLayerManager::GetSingletonPtr()->removeLoadingLayer();
    
    
    CFightData::getInstance()->setNowMonsterNum(0);
    
    //将自己放置到fightdata中,便于获取
    CFightData::getInstance()->setTheFightScene(this);
    
    
    //测试用招怪
    //    addLevelMonster();
    
    //初始化塔防地图
    if (CFightData::getInstance()->getTheLevelCondition()==eLevelCondition_TowerDefense) {
        CFightData::getInstance()->calcAllPathWeight(true);
    }
    
    //初始化抛物线
    m_paoWuLayer = CPaoWuxian::create(Vec3(m_WinSize.width*0.1,m_WinSize.height*0.8,0),this);
    addChild(m_paoWuLayer);
    
    
}





void CFightScene::resetEnemy(float dt)//针对侦测过程中出现的士兵，避免发生位移
{
    cocos2d::Size winSize = Director::getInstance()->getWinSize();
    cocos2d::Point midPoint = Vec2(winSize.width * 0.5f, winSize.height * 0.5f);
    Vec2 tempPos = CommonData::GetSingletonPtr()->getPosOnLayer3D(_camera, midPoint);//获取的屏幕中心点的3D坐标
    Vec3 Mid3DPos = Vec3(tempPos.x, 0, tempPos.y);
    cocos2d::Vector<CBaseFighter*>* enemyVec = CFightData::getInstance()->getAllEnemy();
    for (cocos2d::Vector<CBaseFighter*>::iterator it = enemyVec->begin() ; it != enemyVec->end(); it++)
    {
        CBaseFighter* enemy = *it;
        if(Mid3DPos.distance(enemy->getPosition3D()) <= CR_SCREENMID_RANGE && (enemy->getAttackType() == eAttackType_monster))
        {
            enemy->roatationUpdate(dt);//更新刚出来士兵的面向朝左
            if ((enemy->getAnimateState()!=eFighteState_Dizzy) && (!enemy->getIfDied())) {
                MonsterDebuff* temp = enemy->getMonsterDebuff();
                temp->ifShake = true;
                enemy->stateChange(eFighteState_Dizzy);
                //                enemy->animateChange(eFighteState_Idle);
            }
        }
        
    }
}



void CFightScene::draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags)
{
    if(m_isTouch)
    {
        long dt = time(NULL) ;
        
        if ( m_timer > 10 )
        {
            m_timer = 0 ;
            erasureNail() ;
        }
        else {
            m_timer += dt ;
        }
    }
    LayerColor::draw(renderer, transform, flags);
    _customCommand.init(_globalZOrder);
    _customCommand.func = CC_CALLBACK_0(CFightScene::onDraw, this, transform, flags);
    renderer->addCommand(&_customCommand);
}

void CFightScene::onDraw(const cocos2d::Mat4 &transform, bool transformUpdated)
{
    Director* director = Director::getInstance();
    director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    director->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, transform);
    
    CHECK_GL_ERROR_DEBUG();
    if(m_isTouch)
    {
        drawKnifeTrace();
    }
    else{
        //暂时代替为图片
        //        drawLine();
    }
    CHECK_GL_ERROR_DEBUG();
    director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
}

void CFightScene::drawLine()
{
    glLineWidth( 15 );
    DrawPrimitives::setDrawColor4B(144,190,216,m_opcity);
    
    if (m_pointArr.size() > 0) {
        for (int i = 0; i < m_pointArr.size(); i++){
            if (i == 0) {
                DrawPrimitives::drawPoint(cocos2d::Point(m_pointArr.at(0).x, m_pointArr.at(0).y));
            }
            else
            {
                cocos2d::Point pstart = m_pointArr.at(i - 1);
                DrawPrimitives::drawLine(cocos2d::Point(m_pointArr.at(i).x, m_pointArr.at(i).y ),cocos2d::Point(pstart.x, pstart.y));
            }
        }
    }
}

void CFightScene::erasureNail()
{
    if ( m_Index < 3 || m_Index > 1024)
        return ;
    
    cocos2d::Point*p = m_pointPath ;
    m_Index-- ;
    memmove(p, &(p[1]), sizeof(cocos2d::Point)*(m_Index)) ;
}

void CFightScene::drawKnifeTrace()
{
    if (m_Index<3 || m_Index > 1024) {
        return;
    }
    cocos2d::Point *p = m_pointPath ;
    cocos2d::Point*vertexMiddle = new cocos2d::Point[m_Index] ;
    memcpy(vertexMiddle, p, sizeof(cocos2d::Point)*m_Index);
    GLfloat angle = p[m_Index-1].getAngle(p[m_Index-2]);//计算最后两个点之间的角度
    vertexMiddle[m_Index-1].x += cosf(angle)*10*CC_CONTENT_SCALE_FACTOR();
    vertexMiddle[m_Index-1].y += sinf(angle)*10*CC_CONTENT_SCALE_FACTOR();
    
    cocos2d::Point vertexTop[1024]={Vec2(0.0, 0.0)};
    
    vertexTop[0].x = p[m_Index-1].x + cosf(angle)*10*CC_CONTENT_SCALE_FACTOR();
    vertexTop[0].y = p[m_Index-1].y + sinf(angle)*10*CC_CONTENT_SCALE_FACTOR();
    
    GLint count1 = 1;
    
    for (int i = (m_Index - 2); i>0; --i) {
        float w = i*0.5f ;
        if (w < 3)w = 3 ;
        else if(w > 8)w = 8 ;
        triangle(&(vertexTop[count1]),p[i],p[i-1], w);
        count1++;
    }
    vertexTop[count1++] = p[0];
    
    cocos2d::Point vertexBottom[1024]={Vec2(0.0, 0.0)};
    
    vertexBottom[0].x = p[m_Index-1].x + cosf(angle)*10*CC_CONTENT_SCALE_FACTOR();
    vertexBottom[0].y = p[m_Index-1].y + sinf(angle)*10*CC_CONTENT_SCALE_FACTOR();
    
    GLint count2 = 1;
    
    for (int i = (m_Index - 2); i>0; --i) {
        float w = i*0.5f ;
        if (w < 3)w = 3 ;
        else if(w > 8)w =8 ;
        triangle(&(vertexBottom[count2]),p[i],p[i-1], -w);
        count2++;
    }
    vertexBottom[count2++] = p[0];
    
    cocos2d::Point vertexTriangle[512] ={Vec2(0.0, 0.0)} ;
    cocos2d::Point*vertexCpy = vertexTriangle;
    GLubyte lineColors[1024] = {0};
    GLubyte* lineCpy = lineColors ;
    
    //1
    *vertexCpy = vertexMiddle[m_Index-1] ;
    *lineCpy = MIDDLE_POINT_R; lineCpy++ ;
    *lineCpy = MIDDLE_POINT_G; lineCpy++ ;
    *lineCpy = MIDDLE_POINT_B; lineCpy++ ;
    *lineCpy = MIDDLE_POINT_A; lineCpy++ ;
    vertexCpy++ ;
    
    *vertexCpy = vertexMiddle[m_Index-2] ;
    *lineCpy = MIDDLE_POINT_R; lineCpy++ ;
    *lineCpy = MIDDLE_POINT_G; lineCpy++ ;
    *lineCpy = MIDDLE_POINT_B; lineCpy++ ;
    *lineCpy = MIDDLE_POINT_A; lineCpy++ ;
    vertexCpy++ ;
    
    *vertexCpy = vertexTop[1] ;
    *lineCpy = TOP_POINT_R; lineCpy++ ;
    *lineCpy = TOP_POINT_G; lineCpy++ ;
    *lineCpy = TOP_POINT_B; lineCpy++ ;
    *lineCpy = TOP_POINT_A; lineCpy++ ;
    vertexCpy++ ;
    
    *vertexCpy = vertexMiddle[m_Index-1] ;
    *lineCpy = MIDDLE_POINT_R; lineCpy++ ;
    *lineCpy = MIDDLE_POINT_G; lineCpy++ ;
    *lineCpy = MIDDLE_POINT_B; lineCpy++ ;
    *lineCpy = MIDDLE_POINT_A; lineCpy++ ;
    vertexCpy++ ;
    *vertexCpy = vertexBottom[1] ;
    *lineCpy = BOTTOM_POINT_R; lineCpy++ ;
    *lineCpy = BOTTOM_POINT_G; lineCpy++ ;
    *lineCpy = BOTTOM_POINT_B; lineCpy++ ;
    *lineCpy = BOTTOM_POINT_A; lineCpy++ ;
    vertexCpy++ ;
    *vertexCpy = vertexMiddle[m_Index-2] ;
    *lineCpy = MIDDLE_POINT_R; lineCpy++ ;
    *lineCpy = MIDDLE_POINT_G; lineCpy++ ;
    *lineCpy = MIDDLE_POINT_B; lineCpy++ ;
    *lineCpy = MIDDLE_POINT_A; lineCpy++ ;
    vertexCpy++ ;
    
    for (int i = 0; i < m_Index-2; i++) {
        //上半个四边形
        *vertexCpy = vertexMiddle[m_Index-(i+2)] ;
        *lineCpy = MIDDLE_POINT_R; lineCpy++ ;
        *lineCpy = MIDDLE_POINT_G; lineCpy++ ;
        *lineCpy = MIDDLE_POINT_B; lineCpy++ ;
        *lineCpy = MIDDLE_POINT_A; lineCpy++ ;
        vertexCpy++ ;
        *vertexCpy = vertexMiddle[m_Index-(i+3)] ;
        *lineCpy = MIDDLE_POINT_R; lineCpy++ ;
        *lineCpy = MIDDLE_POINT_G; lineCpy++ ;
        *lineCpy = MIDDLE_POINT_B; lineCpy++ ;
        *lineCpy = MIDDLE_POINT_A; lineCpy++ ;
        vertexCpy++ ;
        *vertexCpy = vertexTop[i+1] ;
        *lineCpy = TOP_POINT_R; lineCpy++ ;
        *lineCpy = TOP_POINT_G; lineCpy++ ;
        *lineCpy = TOP_POINT_B; lineCpy++ ;
        *lineCpy = TOP_POINT_A; lineCpy++ ;
        vertexCpy++ ;
        
        *vertexCpy = vertexTop[i+1] ;
        *lineCpy = TOP_POINT_R; lineCpy++ ;
        *lineCpy = TOP_POINT_G; lineCpy++ ;
        *lineCpy = TOP_POINT_B; lineCpy++ ;
        *lineCpy = TOP_POINT_A; lineCpy++ ;
        vertexCpy++ ;
        *vertexCpy = vertexMiddle[m_Index-(i+3)] ;
        *lineCpy = MIDDLE_POINT_R; lineCpy++ ;
        *lineCpy = MIDDLE_POINT_G; lineCpy++ ;
        *lineCpy = MIDDLE_POINT_B; lineCpy++ ;
        *lineCpy = MIDDLE_POINT_A; lineCpy++ ;
        vertexCpy++ ;
        *vertexCpy = vertexTop[i+2] ;
        *lineCpy = TOP_POINT_R; lineCpy++ ;
        *lineCpy = TOP_POINT_G; lineCpy++ ;
        *lineCpy = TOP_POINT_B; lineCpy++ ;
        *lineCpy = TOP_POINT_A; lineCpy++ ;
        vertexCpy++ ;
        
        //下半个四边形
        *vertexCpy = vertexBottom[i+1] ;
        *lineCpy = BOTTOM_POINT_R; lineCpy++ ;
        *lineCpy = BOTTOM_POINT_G; lineCpy++ ;
        *lineCpy = BOTTOM_POINT_B; lineCpy++ ;
        *lineCpy = BOTTOM_POINT_A; lineCpy++ ;
        vertexCpy++ ;
        *vertexCpy = vertexBottom[i+2] ;
        *lineCpy = BOTTOM_POINT_R; lineCpy++ ;
        *lineCpy = BOTTOM_POINT_G; lineCpy++ ;
        *lineCpy = BOTTOM_POINT_B; lineCpy++ ;
        *lineCpy = BOTTOM_POINT_A; lineCpy++ ;
        vertexCpy++ ;
        *vertexCpy = vertexMiddle[m_Index-(i+2)] ;
        *lineCpy = MIDDLE_POINT_R; lineCpy++ ;
        *lineCpy = MIDDLE_POINT_G; lineCpy++ ;
        *lineCpy = MIDDLE_POINT_B; lineCpy++ ;
        *lineCpy = MIDDLE_POINT_A; lineCpy++ ;
        vertexCpy++ ;
        
        *vertexCpy = vertexMiddle[m_Index-(i+2)] ;
        *lineCpy = MIDDLE_POINT_R; lineCpy++ ;
        *lineCpy = MIDDLE_POINT_G; lineCpy++ ;
        *lineCpy = MIDDLE_POINT_B; lineCpy++ ;
        *lineCpy = MIDDLE_POINT_A; lineCpy++ ;
        vertexCpy++ ;
        *vertexCpy = vertexBottom[i+2] ;
        *lineCpy = BOTTOM_POINT_R; lineCpy++ ;
        *lineCpy = BOTTOM_POINT_G; lineCpy++ ;
        *lineCpy = BOTTOM_POINT_B; lineCpy++ ;
        *lineCpy = BOTTOM_POINT_A; lineCpy++ ;
        vertexCpy++ ;
        *vertexCpy = vertexMiddle[m_Index-(i+3)] ;
        *lineCpy = MIDDLE_POINT_R; lineCpy++ ;
        *lineCpy = MIDDLE_POINT_G; lineCpy++ ;
        *lineCpy = MIDDLE_POINT_B; lineCpy++ ;
        *lineCpy = MIDDLE_POINT_A; lineCpy++ ;
        vertexCpy++ ;
    }
    glDisable(GL_TEXTURE_2D);
    GL::enableVertexAttribs( GL::VERTEX_ATTRIB_FLAG_POSITION | GL::VERTEX_ATTRIB_FLAG_COLOR );
    // Tell OpenGL ES 2.0 to use the shader program assigned in the init of this node.
    this->getGLProgram()->use();
    this->getGLProgram()->setUniformsForBuiltins();
    
    
    Vec2 vetexes[512]={Vec2(0, 0)};
    for (int i=0; i<512; i++) {
        vetexes[i].x=vertexTriangle[i].x;
        vetexes[i].y=vertexTriangle[i].y;
    }
    // Pass the verticies to draw to OpenGL
    glEnableVertexAttribArray(GL::VERTEX_ATTRIB_FLAG_POSITION);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_TRUE, 0,vetexes);
    
    // Pass the colors of the vertices to draw to OpenGL
    glEnableVertexAttribArray(GL::VERTEX_ATTRIB_FLAG_COLOR);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, lineColors);
    
    GLint numberTo=(GLint)((vertexCpy-vertexTriangle-1));
    glDrawArrays(GL_TRIANGLES, 0, numberTo );
    
    glEnable(GL_TEXTURE_2D);
    delete [] vertexMiddle ;
}

void CFightScene::triangle(cocos2d::Point* vertex, cocos2d::Point p1, cocos2d::Point p2, GLfloat w)
{
    GLfloat angle = p1.getAngle(p2);
    GLfloat x = sinf(angle) * w;
    GLfloat y = cosf(angle) * w;
    vertex->x = p1.x+x;
    vertex->y = p1.y-y;
}


void CFightScene::setColorLight(Color3B color)
{
    cocos2d::Vector<CBaseFighter*>* soliderVec = CFightData::getInstance()->getAllSoldier();
    for (cocos2d::Vector<CBaseFighter*>::iterator it = soliderVec->begin(); it != soliderVec->end(); it++) {
        CBaseFighter* sp = *it;
        sp->gettheSprite()->setColor(color);
    }
    
    cocos2d::Vector<CBaseFighter*> TowerVec = CFightData::getInstance()->getAllTowers();
    for (cocos2d::Vector<CBaseFighter*>::iterator it = TowerVec.begin(); it != TowerVec.end(); it++) {
        CBaseFighter* sp = *it;
        sp->gettheSprite()->setColor(color);
    }
    cocos2d::Vector<CBaseFighter*> groupVec = CFightData::getInstance()->getAllMines();
    for (cocos2d::Vector<CBaseFighter*>::iterator it = groupVec.begin(); it != groupVec.end(); it++) {
        CBaseFighter* sp = *it;
        sp->gettheSprite()->setColor(color);
    }
    //difangjidi
    CBaseCity* Athena = CFightData::getInstance()->getTheEnemyCity();
    if (Athena) {
        Athena->gettheSprite()->setColor(color);
    }
    
}

void CFightScene::updateCameraAngle(float dt)
{
    
}

void CFightScene::startMonitorCut(Ref* pSender)
{
    if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SKILL_CUT) && (guiderManagerLayer::getInstance()->getStepNum() == 3))
    {
        CDataManager::saveGuideInfoToDB(guiderManagerLayer::getInstance()->getGuideType());
        guiderManagerLayer::getInstance()->setStepNum(guiderManagerLayer::getInstance()->getStepNum() + 1);
        guiderManagerLayer::getInstance()->createLayer(this);
        m_bCutGuide = true;
        
    }
    else
    {
        if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SKILL_CUT) && (guiderManagerLayer::getInstance()->getStepNum() == 4))
        {
            m_guiderArrow = Sprite::createWithSpriteFrameName("finger_1.png");
            addChild(m_guiderArrow, 1000);
            m_guiderArrow->setPosition(Vec2(m_winSize.width * 0.4f, m_winSize.height * 0.4));
            ActionInterval* action1 = MoveBy::create(1.25f, Vec2(200, 150));
            ActionInterval* action2 = MoveBy::create(0, Vec2(-200, -150));
            ActionInterval* seq = RepeatForever::create(Sequence::create(action1, action2, NULL));
            m_guiderArrow->runAction(seq);
        }
        else{
            if (m_isTouch)
            {
                m_bTouched = false;
                m_pParticle->stopSystem();
            }
        }
        
        char buf[256];
        m_skillTime = m_HeroSkill.at(1).lastTime;
        setBtnEnable();
        m_isTouch = true;
        layer3D->setColor(Color3B(125, 125, 125));
        m_layer->setColor(Color3B(125, 125, 125));
        setColorLight(Color3B::GRAY);//敌方英雄变亮
        
        GLProgram * glShaderProgram = CCShaderCache::getInstance()->getGLProgram(GLProgram::SHADER_NAME_POSITION_COLOR);
        this->setGLProgram(glShaderProgram);
        m_pParticle = ParticleSystemQuad::create("plist/star.plist") ;
        this->addChild(m_pParticle, 10);
        m_pParticle->setVisible(false);
        memset(m_pointPath, 0, sizeof(m_pointPath));
        m_timer = 0 ;
        m_touchBeganPoint = m_touchEndPoint = Vec2(0, 0);
        cutCD = m_HeroSkill.at(1).theSkill->skillCD;//初始化技能cd
        
        if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SKILL_CUT) && (guiderManagerLayer::getInstance()->getStepNum() == 4))
        {
            guiderManagerLayer::getInstance()->setStepNum(guiderManagerLayer::getInstance()->getStepNum() + 1);
        }
        else{
            
            LYMenuItemSprite* pCutItem = (LYMenuItemSprite*)(m_pCutMenu->getChildByTag(SKILL_CUT_FRUIT_TAG));//添加技能cd显示
            sprintf(buf, "%d", m_HeroSkill.at(1).theSkill->skillCD);
            m_cutLabel = Label::createWithSystemFont(buf, "Arial", 30);
            pCutItem->addChild(m_cutLabel);
            m_cutLabel->setScale(0.33f);
            m_cutLabel->enableOutline(Color4B(0,0,0,255),3);
            m_cutLabel->setPosition(pCutItem->getNormalImage()->getContentSize().width / 2, pCutItem->getNormalImage()->getContentSize().height / 2);
            CFightData::getInstance()->setFightData(6);
            if(!m_bCutGuide)
            {
                //动画
                char buf[257];
                Animation* pLineAnimation = Animation::create();
                for (int i = 1; i < 11; i++)
                {
                    sprintf(buf, "CrazyMode_QuesSelect_%d.png", i);
                    pLineAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(buf));
                }
                pLineAnimation->setDelayPerUnit(0.1f);
                AnimationCache::getInstance()->addAnimation(pLineAnimation, "skillAct");
                
                Animation* pAnimation = AnimationCache::getInstance()->getAnimation("skillAct");
                Animate* pAction = Animate::create(pAnimation);
                Sprite* skillSp = Sprite::createWithSpriteFrameName("CrazyMode_QuesSelect_1.png");
                addChild(skillSp, 100);
                skillSp->setPosition(m_winSize.width * 0.5f, m_winSize.height * 0.8f + skillSp->getBoundingBox().size.height * 0.46f);
                skillSp->runAction(RepeatForever::create(pAction));
                skillSp->runAction(Sequence::create(DelayTime::create(m_skillTime), RemoveSelf::create(), NULL));
                
                Sprite* skillTimeBg = Sprite::createWithSpriteFrameName("cutSkillBg.png");
                addChild(skillTimeBg, 100);
                skillTimeBg->setPosition(Vec2(m_winSize.width * 0.5f, m_winSize.height * 0.8f));
                
                Sprite* sp = Sprite::createWithSpriteFrameName("cutSkillProgress.png");
                ProgressTimer* skillProgress = ProgressTimer::create(sp);
                addChild(skillProgress, 100);
                skillProgress->setPosition(Vec2(skillTimeBg->getPositionX() + skillTimeBg->getBoundingBox().size.width * 0.09f,
                                                skillTimeBg->getPositionY()));
                skillProgress->setBarChangeRate(Vec2(1,0));
                skillProgress->setType(ProgressTimer::Type::BAR);
                skillProgress->setMidpoint(Vec2(1, 0));
                skillProgress->setReverseProgress(true);
                auto act = Sequence::createWithTwoActions(ProgressTo::create(m_skillTime, 100), ProgressTo::create(0, 0));
                skillProgress->runAction(Sequence::create(act, RemoveSelf::create(), NULL));
                skillTimeBg->runAction(Sequence::create(DelayTime::create(m_skillTime), RemoveSelf::create(), NULL));
            }
        }
        CFightData::getInstance()->setUseSkill(true);
        schedule(schedule_selector(CFightScene::updateCutSkill));//开启切兵技能的持续时间调度
    }
}


void CFightScene::startMonitorBlow(Ref *pSender)
{
    
    if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SKILL_BLOW) && (guiderManagerLayer::getInstance()->getStepNum() == 5))
    {
        CDataManager::saveGuideInfoToDB(guiderManagerLayer::getInstance()->getGuideType());
        guiderManagerLayer::getInstance()->setStepNum(guiderManagerLayer::getInstance()->getStepNum() + 1);
        guiderManagerLayer::getInstance()->createLayer(this);
        m_bCutGuide = true;
    }
    else{
        char buf[256];
        m_skillTime = m_HeroSkill.at(0).lastTime;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        
        if (micBlow==NULL) {
            micBlow = [[MicBlowViewController alloc] init];
        }
        [micBlow setThe:this];
        [micBlow viewDidLoad];
#else
        JniMethodInfo minfo; //定义Jni函数信息结构体
        bool isHave = JniHelper::getStaticMethodInfo(minfo,"cn.xinpin.view.activity/GameActivity", "getInstance","()Ljava/lang/Object;");
        if (isHave)
        {
            CCLog("jni:此函数存在");
            //调用此函数
            jobject obj= minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
            bool isHaveUser=JniHelper::getMethodInfo(minfo,"cn.xinpin.view.activity/GameActivity", "startVolume","()V");
            if(isHaveUser)
            {
                CCLog("jni:此函数存在.");
                minfo.env->CallVoidMethod(obj, minfo.methodID);
            }
        }
        
#endif
        setBtnEnable();
        m_isblow = true;
        cocos2d::Vector<CBaseFighter*>* enemyVec = CFightData::getInstance()->getAllEnemy();
        
        for (cocos2d::Vector<CBaseFighter*>::iterator it = enemyVec->begin() ; it != enemyVec->end(); it++)
        {
            CBaseFighter* enemy = *it;
            
            if (enemy->getIfDied()) {
                continue;
            }
            MonsterDebuff* temp = enemy->getMonsterDebuff();
            if (!temp->ifShake&&!enemy->getIfDied() && (enemy->getAttackType() == eAttackType_monster)) {
                enemy->blowShake(true);
                enemy->m_offSet = 0;
                enemy->m_MoveTime=0;
                enemy->m_ifValid=false;
                enemy->stateChange(eFighteState_Dizzy);
                //                if (!enemy->getIfDied()) {
                //                    enemy->animateChange(eFighteState_Idle);
                //                }
                temp->ifShake = true;
            }
        }
        if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SKILL_BLOW) && (guiderManagerLayer::getInstance()->getStepNum() == 6))
        {
            guiderManagerLayer::getInstance()->setStepNum(guiderManagerLayer::getInstance()->getStepNum() + 1);
        }
        else{
            blowCD = m_HeroSkill.at(0).theSkill->skillCD ;//初始化技能cd
            LYMenuItemSprite* pBlowItem = (LYMenuItemSprite*)(m_pBlowMenu->getChildByTag(SKILL_BLOW_TAG));//添加技能cd显示
            sprintf(buf, "%d", m_HeroSkill.at(0).theSkill->skillCD);
            m_blowLabel = Label::createWithSystemFont(buf, "Arial", 30);
            pBlowItem->addChild(m_blowLabel);
            m_blowLabel->setScale(0.33f);
            m_blowLabel->setColor(Color3B::RED);
            m_blowLabel->setPosition(pBlowItem->getNormalImage()->getContentSize().width / 2, pBlowItem->getNormalImage()->getContentSize().height / 2);
        }
        schedule(schedule_selector(CFightScene::moveEnemy));//开始移动抖动的士兵
        schedule(schedule_selector(CFightScene::updateBlowingSkill), 1/6.0);
    }
}

void CFightScene::startBlowCallBack(Ref* pSender)
{
    if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_USE_MANGONEL) && ((guiderManagerLayer::getInstance()->getStepNum() == 4)
                                                                                     || (guiderManagerLayer::getInstance()->getStepNum() == 5)))
    {
        return;
    }
    //技能图标隐藏
    //屏蔽画线
    if(!m_bMenuValid)
    {
        return;
    }
    
    int tag = ((Node*)pSender)->getTag();
    if ((tag == SKILL_CUT_FRUIT_TAG) && (!m_bRmvFlag))//切水果技能
    {
        if(guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SKILL_CUT)
        {
            if(guiderManagerLayer::getInstance()->getStepNum() == 3)
            {
                return;
            }
            else if(guiderManagerLayer::getInstance()->getStepNum() == 4)
            {
                guiderManagerLayer::getInstance()->setStepNum(3);
                guiderManagerLayer::getInstance()->removeGuiderLayer();
            }
        }
        else if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SKILL_BLOW) && (guiderManagerLayer::getInstance()->getStepNum() == 5))
        {
            return;
        }
        beforeCut();
    }
//    else if((tag == SKILL_BLOW_TAG) && (!m_bRmvFlag))//吹风技能
//    {
//        if(guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SKILL_BLOW)
//        {
//            if(guiderManagerLayer::getInstance()->getStepNum() == 5)
//            {
//                return;
//            }
//            else if(guiderManagerLayer::getInstance()->getStepNum() == 6)
//            {
//                guiderManagerLayer::getInstance()->setStepNum(5);
//                guiderManagerLayer::getInstance()->removeGuiderLayer();
//            }
//            
//        }
//        beforeBlow();
//    }
    else if((tag == SKILL_START_TAG) && (!m_bRmvFlag))
    {
        if((!(CDataManager::getEndedBy(GUIDE_SKILL_BLOW)) && (CFightData::getInstance()->getNowLevelId() == 18)
            && ((guiderManagerLayer::getInstance()->getStepNum() < 4) || (guiderManagerLayer::getInstance()->getStepNum() == 5)))
           || (!(CDataManager::getEndedBy(GUIDE_SKILL_CUT)) && (CFightData::getInstance()->getNowLevelId() == 9)
               && ((guiderManagerLayer::getInstance()->getStepNum() < 2) || (guiderManagerLayer::getInstance()->getStepNum() == 3)) ))
        {
            return;
        }
        m_bSmallMapAutoMove = false;
        if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SKILL_BLOW) && (guiderManagerLayer::getInstance()->getStepNum() == 4))
        {
            guiderManagerLayer::getInstance()->setStepNum(guiderManagerLayer::getInstance()->getStepNum() + 1);
            guiderManagerLayer::getInstance()->removeGuiderLayer();
            //            Vec2 pos = Vec2(m_winSize.width * 0.3f, m_winSize.height * 0.02);
            //            guiderManagerLayer::getInstance()->createLayer(this, GUIDE_SHOW_BOTH, pos);
        }
        else if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SKILL_CUT) && (guiderManagerLayer::getInstance()->getStepNum() == 2))
        {
            guiderManagerLayer::getInstance()->setStepNum(guiderManagerLayer::getInstance()->getStepNum() + 1);
            guiderManagerLayer::getInstance()->removeGuiderLayer();
        }
        
        UserInfo* pInfo = CPersonalData::getInstance()->getUserInfo();
        //判断英雄是否达到技能的等级限制
        if (!m_isShowBtn) {
            m_bSmallMapAutoMove = false;
            m_isShowBtn = true;
            if (((CDataManager::getEndedBy(GUIDE_SKILL_BLOW)) || (CFightData::getInstance()->getNowLevelId() >= 18))
                && m_pBlowMenu && (pInfo->m_iUserLevel >= m_HeroSkill.at(0).theSkill->levelLimit))
            {
                ActionInterval* pFade = FadeTo::create(0.3, 255);
                ActionInterval* move = MoveBy::create(0.3, Vec2(-130, 25));
                ActionInterval* spwn = Spawn::create(pFade, move, NULL);
                m_pBlowMenu->runAction(EaseSineIn::create(spwn));
            }
            if (((CDataManager::getEndedBy(GUIDE_SKILL_CUT)) || (CFightData::getInstance()->getNowLevelId() >= 9))
                && m_pCutMenu && (pInfo->m_iUserLevel >= m_HeroSkill.at(1).theSkill->levelLimit)) {
                ActionInterval* scale = FadeTo::create(0.3, 255);
                ActionInterval* move = MoveBy::create(0.3, Vec2(-40, 120));
                ActionInterval* spwn = Spawn::create(scale, move, NULL);
                m_pCutMenu->runAction(EaseSineIn::create(spwn));
            }
        }
        else{
            m_isShowBtn = false;
            if (m_pBlowMenu) {
                ActionInterval* fade = FadeOut::create(0.3);
                ActionInterval* move = MoveBy::create(0.3, Vec2(130, -25));
                ActionInterval* spwn = Spawn::create(fade, move, NULL);
                m_pBlowMenu->runAction(EaseSineOut::create(spwn));
            }
            //pItemMenu = (LYMenuItemSprite*)(pSkillMenu->getChildByTag(SKILL_CUT_FRUIT_TAG));
            if (m_pCutMenu) {
                ActionInterval* fade = FadeOut::create(0.3);
                ActionInterval* move = MoveBy::create(0.3, Vec2(40, -120));
                ActionInterval* spwn = Spawn::create(fade, move, NULL);
                m_pCutMenu->runAction(EaseSineOut::create(spwn));
            }
            m_bSmallMapAutoMove = true;
        }
    }
    else if(tag == GAME_PAUSE_TAG)
    {
        if(!m_bRmvFlag)
        {
            m_bifNorAddSolider = true;
        }
        
        Director::getInstance()->pause();
        CommonPropLayer* layer = CommonPropLayer::create(GAME_PAUSE_TYPE);
        addChild(layer, 9999);
    }
    else if(tag == LEVEL_STAR_TAG)
    {
        Director::getInstance()->pause();
        if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_LEVEL_STAR) && (guiderManagerLayer::getInstance()->getStepNum() == 1) && (CFightData::getInstance()->getNowLevelId() == 3))
        {
            CDataManager::saveGuideInfoToDB(GUIDE_LEVEL_STAR);
            guiderManagerLayer::getInstance()->setStepNum(0);
            guiderManagerLayer::getInstance()->removeGuiderLayer();
        }
        CommonPropLayer* layer = CommonPropLayer::create(LEVEL_STAR_TYPE);
        addChild(layer, 999);
    }
    else if(tag == FIGHT_TOUSHICHE_TAG)
    {
        if(m_isblow)
        {
            return;
        }
        
        
        m_paoWuLayer->m_startBall->runAction(RepeatForever::create(Sequence::create(TintTo::create(1.1, 150, 150, 150), TintTo::create(1.1, 255, 255, 255), NULL)));
        if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_USE_MANGONEL) && (guiderManagerLayer::getInstance()->getStepNum() == 2))
        {
            //guiderManagerLayer::getInstance()->removeGuiderLayer();
            guiderManagerLayer::getInstance()->setStepNum(guiderManagerLayer::getInstance()->getStepNum() + 1);
            guiderManagerLayer::getInstance()->createLayer(this);
            //m_GuideNode->runAction(FadeIn::create(0.35f));
        }
        
        m_pSwitchItem1->setEnabled(false);
        m_pSwitchItem1->setVisible(false);
        m_pSwitchItem2->setEnabled(true);
        m_pSwitchItem2->setVisible(true);
        m_pToushiActive->setVisible(true);
        m_bifNorAddSolider = false;
        m_bToushiMove = true;
        
        m_paoWuLayer->getTheDragBall()->setVisible(true);
        m_paoWuLayer->getTheDragBall()->setPosition(m_pToushiActive->getPosition());
        
        
        if(m_pXian)
        {
            m_pXian->setVisible(false);
        }
        if(m_paoWuLayer)
        {
            m_paoWuLayer->m_startBall->setVisible(true);
        }
        
        //        Vec3  VCamera(Start3DPOINT,_camera->getPositionY(),_camera->getPositionZ());
        CameraRunWithSmallMap(Start3DPOINT);
        //        _camera->setPosition3D(VCamera);
        moveCameraPos(Start3DPOINT,_camera->getPositionY(),_camera->getPositionZ());
        
        if(CFightData::getInstance()->getFightType() != FIGHT_PVP_TYPE)
        {
            //m_layer->getChildByTag(SKILL_BLOW_TAG)->setVisible(false);
            m_layer->getChildByTag(SKILL_CUT_FRUIT_TAG)->setVisible(false);
            m_layer->getChildByTag(SKILL_START_TAG)->setVisible(false);
        }
    }
    else if(tag == FIGHT_LAGAN_TAG)
    {
        m_pToushiActive->setVisible(false);
        m_paoWuLayer->getTheDragBall()->setVisible(false);
        
        if(m_paoWuLayer)
        {
            m_paoWuLayer->m_startBall->setVisible(false);
            m_paoWuLayer->m_theBall->setVisible(false);
        }
        m_bifNorAddSolider = true;
        m_bToushiMove = false;
        m_pSwitchItem1->setEnabled(true);
        m_pSwitchItem1->setVisible(true);
        m_pSwitchItem2->setEnabled(false);
        m_pSwitchItem2->setVisible(false);
        
        if(CFightData::getInstance()->getFightType() != FIGHT_PVP_TYPE)
        {
            m_layer->getChildByTag(SKILL_START_TAG)->setVisible(true);
           // m_layer->getChildByTag(SKILL_BLOW_TAG)->setVisible(true);
            m_layer->getChildByTag(SKILL_CUT_FRUIT_TAG)->setVisible(true);
        }
    }
    else if(tag == FIGHT_DEL_SOLIDER)
    {
        if(m_bRmvFlag)
        {
            m_bRmvFlag = false;
            if(m_pRmvItem && m_pRmvBackItem)
            {
                m_pRmvItem->setVisible(true);
                m_pRmvBackItem->setVisible(false);
            }
            rmvAllRmvSolider(NULL);
        }
        else{
            m_bRmvFlag = true;
            if(m_pRmvItem && m_pRmvBackItem)
            {
                m_pRmvItem->setVisible(false);
                m_pRmvBackItem->setVisible(true);
            }
            CFightData::getInstance()->setClearSolider(true);
            showAllRmvSolider();
        }
        
    }
    else if(tag == FIGHT_RMV_BTN)
    {
        m_rmvMenuItem = (LYMenuItemSprite*)pSender;
        CBaseFighter* sp = (CBaseFighter*)m_rmvMenuItem->getUserData();
        if(sp && !sp->getIfDied())
        {
            sp->gettheSprite()->setColor(Color3B::RED);
            //计算删除后获取的能量
            float scale = (float)sp->getNowHP()/sp->getMaxHP();
            m_addEnergy = scale * sp->getTheInfo()->energyCost * 0.8;
            CommonPropLayer *pDlg = CommonPropLayer::create(GAME_RMV_SOLIDER);
            pDlg->rmvSolider(this, menu_selector(CFightScene::rmvAllRmvSolider), m_addEnergy);
            addChild(pDlg, 999);
        }
    }
}

void CFightScene::resetRmvSolider()
{
    cocos2d::Vector<CBaseFighter*>allSolider = CFightData::getInstance()->getAllAttacker();
    for (cocos2d::Vector<CBaseFighter*>::iterator it = allSolider.begin(); it != allSolider.end(); it++)
    {
        CBaseFighter* sp = *it;
        if (sp) {
            sp->gettheSprite()->setColor(Color3B::WHITE);
            if(sp->getNowHP() <= 0)
            {
                sp->onRemoved(true);
            }
        }
    }
    m_rmvMenuItem = NULL;
}

void CFightScene::rmvAllRmvSolider(Ref* pSender)
{
    if(m_rmvMenuItem)
    {
        m_pRmvItem->setVisible(true);
        m_pRmvBackItem->setVisible(false);
        CBaseFighter* sp = (CBaseFighter*)m_rmvMenuItem->getUserData();
        char buf[256];
        sprintf(buf, "获得%d能量", m_addEnergy);
        Sprite * thetarget = CFightManager::getTheFlyWord(buf,"fonts/cuti.ttf");
        Vec3 pos = sp->getPosition3D();
        float height = sp->gettheSprite()->getBoundingBox().size.height * 0.5f;
        Vec2 targetPos = CommonData::GetSingletonPtr()->getPosOnScreen(_camera, pos);
        thetarget->setPosition(Vec2(targetPos.x, targetPos.y + height));
        m_layer->addChild(thetarget, 999);
        
        CFightData::getInstance()->addEnergyWith(m_addEnergy, sp->getFightGroup());
        sp->onRemoved(true);
        CFightData::getInstance()->setNowMonsterNum(CFightData::getInstance()->getNowMonsterNum() - 1);
    }
    for(int i = 0;i < m_allMarkSolider.size(); i++)
    {
        m_allMarkSolider[i]->removeFromParent();
    }
    m_allMarkSolider.clear();
    m_bRmvFlag = false;
    m_bifNorAddSolider = true;
    m_bGuideBlow = false;
    m_bSmallMapAutoMove = true;
    m_pZommControl->setEnabled(true);
    CFightData::getInstance()->setStopAddEnemy(false);
    CFightData::getInstance()->setClearSolider(false);
    cocos2d::Vector<CBaseFighter*>allSolider = CFightData::getInstance()->getAllAttacker();
    for (cocos2d::Vector<CBaseFighter*>::iterator it = allSolider.begin(); it != allSolider.end(); it++)
    {
        CBaseFighter* sp = *it;
        if (sp) {
            sp->scheduleUpdate();//技能时间结束后判断士兵的血量
            
            sp->animateChange(eFighteState_Idle,true);
            //            sp->setAnimateState(eFighteState_Idle);//防止怪物技能卡死
            //            sp->stateChange(eFighteState_Idle);
            sp->gettheSprite()->setColor(Color3B::WHITE);
            if(sp->getNowHP() <= 0)
            {
                sp->onRemoved(true);
            }
        }
    }
    cocos2d::Vector<CBaseFighter*>allTower = CFightData::getInstance()->getAllTowers();
    for (cocos2d::Vector<CBaseFighter*>::iterator it = allTower.begin(); it != allTower.end(); it++)
    {
        CBaseFighter* sp = *it;
        if (sp && (sp->getAttackType() == eAttackType_tower)) {
            sp->scheduleUpdate();
        }
    }
    m_rmvMenuItem = NULL;
}

void CFightScene::showAllRmvSolider()
{
    m_bifNorAddSolider = false;
    m_bSmallMapAutoMove = false;
    m_pZommControl->setEnabled(false);
    m_bGuideBlow = true;
    CFightData::getInstance()->setStopAddEnemy(true);
    //所有敌我双方士兵停止动作
    cocos2d::Vector<CBaseFighter*>allSolider = CFightData::getInstance()->getAllAttacker();
    for (cocos2d::Vector<CBaseFighter*>::iterator it = allSolider.begin(); it != allSolider.end(); it++)
    {
        CBaseFighter* sp = *it;
        if (sp && (sp->getTheInfo()->monsterType != eFighterType_Tower)) {
            //            sp->animateChange(eFighteState_Idle);
            sp->gettheSprite()->stopAllActions();//暂停士兵的所有动作
            
            sp->unscheduleUpdate();
            sp->unscheduleAllCallbacks();
            sp->stopAllActions();
            if((sp->getFightGroup() == eFightGroup_Freind) && (sp->getAttackType() == eAttackType_monster) && (sp->getNowHP() > 0)
               && (sp->isVisible()) && (!sp->getIfReinforce()))
            {
                Sprite* normal = Sprite::createWithSpriteFrameName("rmvBtn.png");
                Sprite* select = Sprite::createWithSpriteFrameName("rmvBtn.png");
                select->setColor(Color3B::GRAY);
                LYMenuItemSprite* pItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(CFightScene::startBlowCallBack, this));
                pItem->setUserData(sp);
                pItem->setTag(FIGHT_RMV_BTN);
                pItem->setScale(0.75f);
                Menu* rvMenu = Menu::create(pItem, NULL);
                m_layer->addChild(rvMenu,9);
                Vec3 pos = sp->getPosition3D();
                float height = sp->gettheSprite()->getBoundingBox().size.height * 0.8f;
                Vec2 targetPos = CommonData::GetSingletonPtr()->getPosOnScreen(_camera, pos);
                rvMenu->setPosition(Vec2(targetPos.x, targetPos.y + height));
                m_allMarkSolider.push_back(rvMenu);
            }
        }
    }
    //所有塔防都停止攻击
    cocos2d::Vector<CBaseFighter*>allTower = CFightData::getInstance()->getAllTowers();
    for (cocos2d::Vector<CBaseFighter*>::iterator it = allTower.begin(); it != allTower.end(); it++)
    {
        CBaseFighter* sp = *it;
        if (sp && (sp->getAttackType() == eAttackType_tower)) {
            sp->animateChange(eFighteState_Idle);
            sp->gettheSprite()->stopAllActions();
            sp->unscheduleUpdate();
        }
    }
}



void CFightScene::updateEnemyPos(float dt)
{
    cocos2d::Vector<CBaseFighter*>* enemyVec = CFightData::getInstance()->getAllEnemy();
    for (cocos2d::Vector<CBaseFighter*>::iterator it = enemyVec->begin() ; it != enemyVec->end(); it++)
    {
        CBaseFighter* enemy = *it;
        Vec3 enemyPos = enemy->getPosition3D();
        float posZ = enemyPos.z -= CR_SCREENMID_RANGE;
        if (enemy->getMonsterDebuff()->ifFlyOut) {
            enemy->setPosition3D(Vec3(enemyPos.x, enemyPos.y, posZ));
        }
    }
}


void CFightScene::addNewOBBWithCoords(Vec3 p,CBaseFighter* target)
{
    float height = target->gettheSprite()->getBoundingBox().size.height;
    
    p.y = p.y+height*0.5f;
    
    Vec3 extents = Vec3(height*0.35f, height*0.35f, height*0.35f);
    AABB aabb(-extents, extents);
    auto obb = OBB(aabb);
    obb._center = p;
    enemyInfo pInfo;
    pInfo.obb = obb;
    pInfo.enemy = target;
    m_obb.push_back(pInfo);
}

void CFightScene::removeOBBWithFighter(CBaseFighter *target){
    
    if (m_obb.size()<=0) {
        return;
    }
    
    
    for (std::vector<enemyInfo>::iterator ite = m_obb.begin(); ite!=m_obb.end();) {
        
        if ((*ite).enemy==target) {
            m_obb.erase(ite);
        }else{
            ite++;
        }
    }
    
}

//切兵技能
void CFightScene::updateCutSkill(float dt)
{
    if (!m_bCutGuide && (m_skillTime == 0))
    {
        m_bGuideBlow = false;
        m_bSmallMapMoveStart = true;
        m_bSmallMapAutoMove = true;
        m_time = 0;
        m_isTouch = false;
        layer3D->setColor(Color3B(255, 255, 255));
        m_layer->setColor(Color3B(255, 255, 255));
        setColorLight(Color3B::WHITE);
        m_obb.clear();
        m_hurtEnemyVec.clear();
        CFightData::getInstance()->setUseSkill(false);
        CFightData::getInstance()->m_isIfGetVolume = false;
        unschedule(schedule_selector(CFightScene::updateCutSkill));
        m_pParticle->setVisible(false);
        //setBtnEnable(true);
        
        cocos2d::Vector<CBaseFighter*>allSolider = CFightData::getInstance()->getAllAttacker();
        for (cocos2d::Vector<CBaseFighter*>::iterator it = allSolider.begin(); it != allSolider.end(); it++)
        {
            CBaseFighter* sp = *it;
            if (sp) {
                sp->m_isCut = false;
                sp->scheduleUpdate();//技能时间结束后判断士兵的血量
                
                sp->setAnimateState(eFighteState_Idle);//防止怪物技能卡死
                sp->stateChange(eFighteState_Idle);
                
                
                if(sp->getIfDied()==true){ //如果是本来就死掉得怪，则直接死掉
                    
                    sp->removeFromParentAndCleanup(true);
                    
                }
                
                if ((sp->getNowHP() <= 0) && (sp->getIfDied() == false)) {
                    ParticleSystem*explodePar = ParticleSystemQuad::create("plist/blood2.plist") ;
                    explodePar->setScale(1.5f);
                    explodePar->setPosition3D(Vec3(0, sp->getPosition3D().y / 2, 0) + sp->getPhysicalPosition()) ;
                    explodePar->setCameraMask((unsigned short )CameraFlag::USER1);
                    explodePar->setAutoRemoveOnFinish(true);
                    addChild(explodePar, 11, 101) ;
                    explodePar->setGlobalZOrder(999);
                    sp->onDied(eDamageType_HeroSkill);
                }
                
                
                
                
            }
        }
        cocos2d::Vector<CBaseFighter*>allTower = CFightData::getInstance()->getAllTowers();
        for (cocos2d::Vector<CBaseFighter*>::iterator it = allTower.begin(); it != allTower.end(); it++)
        {
            CBaseFighter* sp = *it;
            if (sp && (sp->getAttackType() == eAttackType_tower)) {
                sp->scheduleUpdate();
            }
        }
        if(m_blood)//清空连击数
        {
            CallFunc* combo = CallFunc::create(CC_CALLBACK_0(CFightScene::resetCombo, this));//延迟重置连击数
            runAction(Sequence::create(DelayTime::create(0.5f), combo, NULL));
        }
        
        for (cocos2d::Vector<CBaseFighter*>::iterator it = allSolider.begin(); it != allSolider.end(); it++)
        {
            CBaseFighter* sp = *it;
            if (sp) {
                sp->scheduleUpdate();//技能时间结束后判断士兵的血量
            }
        }
        if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SKILL_CUT) && (guiderManagerLayer::getInstance()->getStepNum() == 5))
        {
            CallFunc* guide = CallFunc::create(CC_CALLBACK_0(CFightScene::showCutGuide, this));
            runAction(Sequence::create(DelayTime::create(1.2f), guide , NULL));
        }
        if(m_isShowBtn)
        {
            ActionInterval* fade = FadeOut::create(0.3);
            ActionInterval* move = MoveBy::create(0.3, Vec2(40, -120));
            ActionInterval* spwn = Spawn::create(fade, move, NULL);
            m_pCutMenu->runAction(EaseSineOut::create(spwn));
            m_isShowBtn = false;
        }
        CallFunc* set = CallFunc::create(CC_CALLBACK_0(CFightScene::setDrawLine, this));
        runAction(Sequence::create(DelayTime::create(1.0f), set, NULL));
    }
    else
    {
        m_time += dt;
        if(m_time > 1)
        {
            m_time = 0;
            m_skillTime--;
        }
        //检测敌方士兵是否受到伤害
    }
}

void CFightScene::setDrawLine()
{
    m_bifNorAddSolider = true;
}

void CFightScene::startBlowSolider(Ref* pSender)
{
    unschedule(schedule_selector(CFightScene::updateBlowingSkill));
    m_bCutGuide = false;
    NotificationCenter::getInstance()->postNotification("StartBlow");
}

void CFightScene::startCutSolider(Ref* pSender)
{
    unschedule(schedule_selector(CFightScene::updateCutSkill));
    m_bCutGuide = false;
    NotificationCenter::getInstance()->postNotification("StartCut");
    
}

void CFightScene::showCutGuide()
{
    guiderManagerLayer::getInstance()->createLayer(this);
    m_bTouchMap = false;
}

void CFightScene::resetCombo()
{
    m_chop->removeFromParent();
    m_blood->removeFromParent();
    m_combo->removeFromParent();
    m_blood = NULL;
    m_combo = NULL;
    m_chop = NULL;
    m_comboNum = 0;
    m_showCombo = 0;
    m_fshowComboInteval = 0;
}

void CFightScene::addCutEffect(cocos2d::Vec3 pos, CBaseFighter* sprite)//士兵受伤效果接口
{
    std::vector<CBaseFighter*>::iterator result = find( m_hurtEnemyVec.begin(), m_hurtEnemyVec.end(), sprite ); //查找sprite
    if(result == m_hurtEnemyVec.end())
    {
        
        //添加士兵受伤动作
        sprite->beAttacked(NULL, m_HeroSkill.at(1).damage, 1, eFightGroup_Freind, eDamageType_HeroSkill);
        
        sprite->animateChange(eFighteState_Hurt,true);
        
        m_hurtEnemyVec.push_back(sprite);
        m_comboNum++;
        //        CallFunc* combo = CallFunc::create(CC_CALLBACK_0(CFightScene::updateCombo, this));
        //        runAction(Sequence::create(DelayTime::create(0.2f), combo, NULL));
        //添加掉血效果
        ParticleSystem*explodePar = ParticleSystemQuad::create("plist/blood1.plist") ;
        explodePar->setScale(1.5f);
        explodePar->setPosition3D(Vec3(0, sprite->gettheSprite()->getBoundingBox().size.height / 2 + sprite->getPhysicalPosition().y, 0)) ;
        explodePar->setCameraMask((unsigned short )CameraFlag::USER1);
        explodePar->setAutoRemoveOnFinish(true);
        sprite->addChild(explodePar, 11, 100) ;
        explodePar->setGlobalZOrder(1001);
    }
}
//连击数
void CFightScene::updateCombo()
{
    char buf[32]={0};
    sprintf(buf, "%d", m_showCombo);
    ActionInterval* action = Sequence::create(ScaleTo::create(0.1, 1.5), ScaleTo::create(0.1, 1), NULL);
    action->setTag(100);
    if(!m_blood)
    {
        m_blood = Sprite::createWithSpriteFrameName("blood.png");
        addChild(m_blood);
        m_blood->setScale(0);
        m_blood->setPosition(Vec2(m_WinSize.width * 0.2f, m_WinSize.height * 0.8));
        m_blood->runAction(ScaleTo::create(0.2f, 1.5));
        
        m_chop = Sprite::createWithSpriteFrameName("continuate attack.png");
        addChild(m_chop);
        m_chop->setPosition(Vec2(m_WinSize.width *0.23f, m_WinSize.height*0.82));
        m_chop->runAction(ScaleTo::create(0.2f, 1.0));
        
        m_combo = Label::createWithCharMap("fonts/figure.png", 35,53,'0');
        addChild(m_combo, 10);
        m_combo->setAnchorPoint(Vec2(0.5f, 0.5));
        m_combo->setString(buf);
        m_combo->setPosition(Vec2(m_chop->getPositionX() - m_chop->getContentSize().width / 2 - m_combo->getBoundingBox().size.width*0.7f, m_WinSize.height * 0.82));
        //        m_combo->updateContent();
        m_combo->runAction(action);
    }
    else
    {
        m_combo->setScale(1);
        m_combo->setString(buf);
        m_combo->updateContent();
        m_combo->setPosition(Vec2(m_chop->getPositionX() - m_chop->getContentSize().width / 2 - m_combo->getBoundingBox().size.width*0.7f, m_WinSize.height * 0.82));
        m_combo->stopActionByTag(100);
        m_combo->runAction(action);
    }
}

void CFightScene::switchBtnValid(float dt)
{
    char buf[128];
    if(blowCD != 0)//更新技能cd时间
    {
        m_blowTime += dt;
        if(m_blowTime > 1)
        {
            blowCD--;
            m_blowTime = 0;
            sprintf(buf,"%d", blowCD);
            m_blowLabel->setString(buf);
        }
    }
    else
    {
        if(m_blowLabel)
        {
            m_blowLabel->removeFromParent();
            m_blowLabel = NULL;
        }
        //LYMenuItemSprite* pItemSprite = (LYMenuItemSprite*)(pSkillMenu->getChildByTag(SKILL_BLOW_TAG));
        if (!m_isblow && m_pBlowMenu && !(m_pBlowMenu->isEnabled() && (m_pBlowMenu->getOpacity() == 255)))
        {
            m_pBlowMenu->setEnabled(true);
            m_pBlowMenu->setColor(Color3B::WHITE);
        }
        else{
            if(m_pBlowMenu)
            {//如果能量不足则按钮变灰，但可点击
                if(CFightData::getInstance()->getNowEnergy()<m_HeroSkill.at(0).theSkill->costEnergy)
                {
                    m_pBlowMenu->setColor(Color3B::GRAY);
                }
                else{
                    m_pBlowMenu->setColor(Color3B::WHITE);
                }
            }
            if(m_pBlowMenu && (m_pBlowMenu->getOpacity() == 0))
                m_pBlowMenu->setEnabled(false);
        }
    }
    if(cutCD != 0)
    {
        if(CFightData::getInstance()->getClearSolider())
        {
            return;
        }
        m_cutTime += dt;
        if(m_cutTime > 1)
        {
            cutCD--;
            m_cutTime = 0;
            sprintf(buf,"%d", cutCD);
            if(m_cutLabel)
            {
                m_cutLabel->setString(buf);
            }
        }
    }
    else
    {
        if(m_cutLabel)
        {
            m_cutLabel->removeFromParent();
            m_cutLabel = NULL;
        }
        if((m_pCutMenu && !(m_pCutMenu->isEnabled()) && !m_isblow && !CDataManager::getEndedBy(GUIDE_SKILL_BLOW))
           || (guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SKILL_CUT))
        {
            m_pCutMenu->setEnabled(true);
            m_pCutMenu->setColor(Color3B::WHITE);
        }
//        else if ((m_pCutMenu && !(m_pCutMenu->isEnabled()) && (m_pBlowMenu && (m_pBlowMenu->getOpacity() == 255)) && !m_isblow)
//                 || ((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SKILL_CUT)))//如果加吹风技能则用当前条件
        else if ((m_pCutMenu && !(m_pCutMenu->isEnabled()) && !m_isblow)
                 || ((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SKILL_CUT)))
        {
            m_pCutMenu->setEnabled(true);
            m_pCutMenu->setColor(Color3B::WHITE);
        }
        else{
            if(m_pBlowMenu)
            {//如果能量不足则按钮变灰，但可点击
                if(CFightData::getInstance()->getNowEnergy()<m_HeroSkill.at(1).theSkill->costEnergy)
                {
                    m_pCutMenu->setColor(Color3B::GRAY);
                }
                else{
                    m_pCutMenu->setColor(Color3B::WHITE);
                }
            }
            
            if(m_pCutMenu && (m_pCutMenu->getOpacity() == 0))
                m_pCutMenu->setEnabled(false);
        }
    }
    
}

void CFightScene::setBtnEnable(bool ifValid)
{
    //LYMenuItemSprite* pItemMenu = (LYMenuItemSprite*)(pSkillMenu->getChildByTag(SKILL_BLOW_TAG));
    if (m_pBlowMenu)
    {
        m_pBlowMenu->setEnabled(ifValid);
        if(ifValid)
        {
            m_pBlowMenu->setColor(Color3B::WHITE);
        }
        else{
            m_pBlowMenu->setColor(Color3B::GRAY);
        }
    }
    //pItemMenu = (LYMenuItemSprite*)(pSkillMenu->getChildByTag(SKILL_CUT_FRUIT_TAG));
    if (m_pCutMenu) {
        m_pCutMenu->setEnabled(ifValid);
        if(ifValid)
        {
            m_pCutMenu->setColor(Color3B::WHITE);
        }
        else{
            m_pCutMenu->setColor(Color3B::GRAY);
        }
    }
    //    m_pMenuTouShiPao->setEnabled(ifValid);
}

void CFightScene::updateBlowingSkill(float dt)
{
    if (m_adjustAngle >= BLOW_CAMERA_ANGEL) {//视角调整结束后
        
        char buf[128];
        if(!CFightData::getInstance()->getUseSkill() && !m_bCutGuide)
        {
            CFightData::getInstance()->setUseSkill(true);
            CFightData::getInstance()->setFightData(6);
            //动画
            Animation* pLineAnimation = Animation::create();
            for (int i = 1; i < 11; i++)
            {
                sprintf(buf, "CrazyMode_QuesSelect_%d.png", i);
                pLineAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(buf));
            }
            pLineAnimation->setDelayPerUnit(0.1f);
            AnimationCache::getInstance()->addAnimation(pLineAnimation, "skillAct");
            
            Animation* pAnimation = AnimationCache::getInstance()->getAnimation("skillAct");
            Animate* pAction = Animate::create(pAnimation);
            Sprite* skillSp = Sprite::createWithSpriteFrameName("CrazyMode_QuesSelect_1.png");
            addChild(skillSp, 100);
            skillSp->setPosition(m_winSize.width * 0.5f, m_winSize.height * 0.8f + skillSp->getBoundingBox().size.height * 0.46f);
            skillSp->runAction(RepeatForever::create(pAction));
            skillSp->runAction(Sequence::create(DelayTime::create(m_skillTime), RemoveSelf::create(), NULL));
            
            Sprite* skillTimeBg = Sprite::createWithSpriteFrameName("cutSkillBg.png");
            addChild(skillTimeBg, 100);
            skillTimeBg->setPosition(Vec2(m_winSize.width * 0.5f, m_winSize.height * 0.8f));
            
            Sprite* sp = Sprite::createWithSpriteFrameName("cutSkillProgress.png");
            ProgressTimer* skillProgress = ProgressTimer::create(sp);
            addChild(skillProgress, 100);
            skillProgress->setPosition(Vec2(skillTimeBg->getPositionX() + skillTimeBg->getBoundingBox().size.width * 0.09f,
                                            skillTimeBg->getPositionY()));
            skillProgress->setBarChangeRate(Vec2(1,0));
            skillProgress->setType(ProgressTimer::Type::BAR);
            skillProgress->setMidpoint(Vec2(1, 0));
            skillProgress->setReverseProgress(true);
            auto act = Sequence::createWithTwoActions(ProgressTo::create(m_skillTime, 100), ProgressTo::create(0, 0));
            skillProgress->runAction(Sequence::create(act, RemoveSelf::create(), NULL));
            skillTimeBg->runAction(Sequence::create(DelayTime::create(m_skillTime), RemoveSelf::create(), NULL));
        }
        cocos2d::Vector<CBaseFighter*>* enemyVec = CFightData::getInstance()->getAllEnemy();
        if (!m_bCutGuide && (m_skillTime == 0))
        {
            m_bSmallMapMoveStart = true;
            m_bSmallMapAutoMove = true;
            CFightData::getInstance()->setUseSkill(false);
            m_volume = 0;
            //停止吹风
            for(int i = 0; i < m_soundVec.size(); i++)
            {
                SimpleAudioEngine::getInstance()->stopEffect(m_soundVec[i]);
            }
            log("停止吹风音效 = %d", m_soundId);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            
            [micBlow RecordOverCallBack];
            
#else
            JniMethodInfo minfo; //定义Jni函数信息结构体
            bool isHave = JniHelper::getStaticMethodInfo(minfo,"cn.xinpin.view.activity/GameActivity", "getInstance","()Ljava/lang/Object;");
            if (isHave)
            {
                CCLog("jni:此函数存在");
                //调用此函数
                jobject obj= minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
                bool isHaveUser=JniHelper::getMethodInfo(minfo,"cn.xinpin.view.activity/GameActivity", "stopVolume","()V");
                if(isHaveUser)
                {
                    CCLog("jni:此函数存在.");
                    minfo.env->CallVoidMethod(obj, minfo.methodID);
                }
            }
#endif
            
            
            m_bGuideBlow = false;
            m_bTouchMap = false;
            unschedule(schedule_selector(CFightScene::updateBlowingSkill));
            unschedule(schedule_selector(CFightScene::moveEnemy));
            m_isblow = false;
            m_bifNorAddSolider = true;
            CFightData::getInstance()->m_isIfGetVolume = false;
            //将那些移动了但是没有飞出去的士兵的状态设置回来?????这一块是不是应该将士兵原来的状态给记住，在吹风结束后如果士兵还活着，则设置回来
            for (cocos2d::Vector<CBaseFighter*>::iterator it = enemyVec->begin() ; it != enemyVec->end(); )
            {
                CBaseFighter* enemy = *it;
                if(enemy->getAttackType() == eAttackType_monster){
                    enemy->blowShake(false);
                    
                    enemy->stateChange(eFighteState_Idle);
                    
                    MonsterDebuff* temp = enemy->getMonsterDebuff();
                    if((temp->ifFlyOut) && (enemy->m_MoveTime > 3))
                    {
                        enemy->onRemoved(true);
                        //奇坑，如果在此处删掉vector，一定不能it++
                    }
                    else{
                        temp->ifFlyOut = false;
                        temp->ifShake = false;
                        enemy->scheduleUpdate();
                        it++;
                    }
                }else{
                    it++;
                }
            }
            m_bifBlowEnd = true;
            m_adjustAngle = 0;
            
            //吹风技能使用引导
            cocos2d::Vector<CBaseFighter*>allSolider = CFightData::getInstance()->getAllAttacker();
            for (cocos2d::Vector<CBaseFighter*>::iterator it = allSolider.begin(); it != allSolider.end(); it++)
            {
                CBaseFighter* sp = *it;
                if (sp) {
                    sp->scheduleUpdate();//技能时间结束后判断士兵的血量
                }
            }
            if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SKILL_BLOW) && (guiderManagerLayer::getInstance()->getStepNum() == 5))
            {
                guiderManagerLayer::getInstance()->setStepNum(guiderManagerLayer::getInstance()->getStepNum() + 1);
                guiderManagerLayer::getInstance()->createLayer(this);
            }
            if(m_isShowBtn)
            {
                ActionInterval* fade1 = FadeOut::create(0.3);
                ActionInterval* move1 = MoveBy::create(0.3, Vec2(130, -25));
                ActionInterval* spwn1 = Spawn::create(fade1, move1, NULL);
                m_pBlowMenu->runAction(EaseSineOut::create(spwn1));
                ActionInterval* fade2 = FadeOut::create(0.3);
                ActionInterval* move2 = MoveBy::create(0.3, Vec2(40, -120));
                ActionInterval* spwn2 = Spawn::create(fade2, move2, NULL);
                m_pCutMenu->runAction(EaseSineOut::create(spwn2));
                m_isShowBtn = false;
            }
        }
        else
        {
            float prob = CR_BASE_PROBILITY;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            JniMethodInfo minfo; //定义Jni函数信息结构体
            bool isHave = JniHelper::getStaticMethodInfo(minfo,"cn.xinpin.view.activity/GameActivity", "getInstance","()Ljava/lang/Object;");
            if (isHave)
            {
                CCLog("jni:此函数存在");
                //调用此函数
                jobject obj= minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
                bool isHaveUser=JniHelper::getMethodInfo(minfo,"cn.xinpin.view.activity/GameActivity", "getVolume","()Ljava/lang/String;");
                if(isHaveUser)
                {
                    CCLog("jni:此函数存在.");
                    jstring volume = (jstring)minfo.env->CallObjectMethod(obj, minfo.methodID);
                    const char* javavolume = minfo.env->GetStringUTFChars(volume, 0);
                    CCLog("jni:getVolume此函数存在%d",volume);
                    m_volume = (float)atof(javavolume);
                }
            }
#endif
            if (m_volume >= prob)
            {
                m_soundId = SimpleAudioEngine::getInstance()->playEffect("music/blow.mp3");
                m_soundVec.push_back(m_soundId);
                log("开始吹风音效 = %d", m_soundId);
                traverseSolider();
            }
            m_time += dt;
            if(m_time > 1)
            {
                m_time = 0;
                m_skillTime--;
            }
        }
        
    }
}



void CFightScene::moveEnemy(float dt)
{
    if (m_adjustAngle >= BLOW_CAMERA_ANGEL)
    {
        cocos2d::Vector<CBaseFighter*>* enemyVec = CFightData::getInstance()->getAllEnemy();
        for (cocos2d::Vector<CBaseFighter*>::iterator it = enemyVec->begin() ; it != enemyVec->end(); )
        {
            
            bool ifDelete = false;
            
            CBaseFighter* enemy = *it;
            Vec3 enemyPos = enemy->getPosition3D();
            if (enemy->m_ifValid&&(!enemy->getIfDied()) && (enemy->getAttackType() == eAttackType_monster)) {
                enemy->m_offSet++;
                //            enemy->stateChange(eFighteState_Idle);//设置士兵状态为空闲状态
                float posZ = enemyPos.z -= CR_SHAKE_EACHFRAME*(enemy->m_MoveTime+1);//受风力影响的位移每帧移动三个像素
                enemy->setPosition3D(Vec3(enemyPos.x, enemyPos.y, posZ));
                
                
                enemy->blowShake(false);
                
                if(posZ < ENDZPOINT)//如果抖动位移小于最小Z轴则直接让士兵飞出去
                {
                    enemy->m_offSet = 5;    //不用最小位移了，直接飞出
                    enemy->m_MoveTime = 2;
                }
                
                if (enemy->m_offSet == 5) {
                    enemy->m_MoveTime++;
                    enemy->m_offSet = 0;
                    enemy->m_ifValid = false;
                    enemy->blowShake(true);
                    if(enemy->m_MoveTime>=3)
                    {
                        MonsterDebuff* temp = enemy->getMonsterDebuff();
                        temp->ifFlyOut = true;
                        //temp->ifShake = false;
                    }
                }
            }
            
            if (enemy->m_MoveTime >= 3)
            {
                enemy->blowShake(false);
                
                enemy->m_offSet = 0;
                enemy->m_ifValid = false;
                //enemy->setIfDied(true);
                Vec3 enemyPos = enemy->getPosition3D();
                float posZ = enemyPos.z -= CR_FLY_EACHFRAME;//士兵被吹起时每帧45个像素
                if (enemy->getMonsterDebuff()->ifFlyOut)
                {
                    enemy->m_MoveTime++;
                    enemy->setPosition3D(Vec3(enemyPos.x, enemyPos.y+6, posZ));
                    
                    if (enemy->getActionByTag(SOLDIER_FLY_ROTATE_TAG) == NULL)
                    {
                        ActionInterval* scale = RepeatForever::create(RotateBy::create(1, Vec3(-720, 0, 0)));
                        enemy->runAction(scale);
                        scale->setTag(SOLDIER_FLY_ROTATE_TAG);
                    }
                    if (enemyPos.z < CR_MOVE_MAXDISTANCE) {//z轴移动的最远距离
                        MonsterDebuff* temp = enemy->getMonsterDebuff();
                        temp->ifFlyOut = false;
                        enemy->stopAllActions();
                        
                        
                        Sprite* star = Sprite::createWithSpriteFrameName("ShineStar.png");
                        star->setColor(Color3B(255, 255, 0));
                        layer3D->addChild(star);
                        star->setCameraMask((unsigned short )CameraFlag::USER1);
                        //预留，3d坐标转2d坐标逻辑，
                        star->setPosition3D(enemy->getPosition3D());
                        star->setScale(0);
                        star->runAction(Sequence::create(DelayTime::create(0.5f),ScaleTo::create(0.4, 0.4), ScaleTo::create(0.4, 0), RemoveSelf::create(), NULL));
                        
                        //enemy->onRemoved(true);//如果超过这个距离则删除士兵
                        enemy->onDied(eDamageType_HeroSkill);
                        ifDelete = true;
                        if(!m_bBlowSolider && ((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SKILL_BLOW)
                                               && (guiderManagerLayer::getInstance()->getStepNum() == 7)))
                        {
                            m_bBlowSolider = true;
                            CPersonalData::getInstance()->setCutGuide(true);
                            NotificationCenter::getInstance()->postNotification("guideBlow");
                        }
                        
                    }
                }
            }
            
            
            //如果没有删除士兵，则移动指针++
            if (!ifDelete) {
                it++;
            }
            
            
        }
        resetEnemy(dt);
    }
}

void CFightScene::traverseSolider()
{
    CFightData::getInstance()->m_isIfGetVolume = true;
    cocos2d::Size winSize = Director::getInstance()->getWinSize();
    cocos2d::Point midPoint = Vec2(winSize.width * 0.5f, winSize.height * 0.5f);
    Vec2 tempPos = CommonData::GetSingletonPtr()->getPosOnLayer3D(_camera, midPoint);//获取的屏幕中心点的3D坐标
    Vec3 Mid3DPos = Vec3(tempPos.x, 0, tempPos.y);
    cocos2d::Vector<CBaseFighter*>* enemyVec = CFightData::getInstance()->getAllEnemy();
    float Prob = (m_volume - CR_BASE_PROBILITY);//吹走一个士兵的概率
    for (cocos2d::Vector<CBaseFighter*>::iterator it = enemyVec->begin() ; it != enemyVec->end(); it++)
    {
        CBaseFighter* enemy = *it;
        
        //        if (enemy->getAnimateState()!=eFighteState_Idle&&!enemy->getIfDied()) {
        //            enemy->stateChange(eFighteState_Idle);
        //            if(!enemy->getIfDied()){
        //                enemy->animateChange(eFighteState_Idle);
        //            }
        //        }
        
        if (enemy&&!enemy->m_ifValid&&Mid3DPos.distance(enemy->getPosition3D()) <= CR_SCREENMID_RANGE)//技能对于屏幕两侧600码以内的敌方士兵有效
        {
            Vec3 enemyPos = enemy->getPosition3D();
            int min = int(100*Prob);
            int randProb = 1 + rand() % 100;//计算士兵被吹到的概率
            //            enemy->stateChange(eFighteState_Idle);
            if(enemy->getTheInfo()->monsterId == 12)//修改小飞龙被风吹走的概率
            {
                randProb = randProb *0.5f;
            }
            
            MonsterDebuff* temp = enemy->getMonsterDebuff();
            temp->ifShake = true;
            if ((randProb >= 1) && (randProb <= min))
            {
                if(enemy->getAttackType() == eAttackType_monster){
                    enemy->unscheduleUpdate();
                    //enemy->unscheduleAllCallbacks();
                    //enemy->stopAllActions();
                    enemy->m_ifValid = true;
                    CCLOG("概率为%d", randProb);
                    //                    if(!m_bBlowSolider && ((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SKILL_BLOW)
                    //                                           && (guiderManagerLayer::getInstance()->getStepNum() == 7)))
                    //                    {
                    //                        m_bBlowSolider = true;
                    //                        CPersonalData::getInstance()->setCutGuide(true);
                    //                        NotificationCenter::getInstance()->postNotification("guideBlow");
                    //                    }
                }
                
            }
        }
    }
    
}


void CFightScene::onSkillActive(cocos2d::Ref *pSender){
    
//    if (!pSender) {
//        return;
//    }
//    
//    int id =(((Node*)pSender)->getTag());
//    
//    Vector<CBaseFighter*> theVec = CFightData::getInstance()->getSoldierById(id);
//    
//    
//    for (int i=0; i<theVec.size(); i++) {
//        
//        CBaseFighter* temp = theVec.at(i);
//        
//        if(temp&&!temp->getIfDied()){
//            
//            temp->skillPrepare();
//        }
//        
//        
//    }
    
}


void  CFightScene::OnToushicheNormal(Ref *pSender)
{
    //Sprite *pEnable = Sprite::create("toushichenormal.png");
    //m_pToushicheItem->setNormalImage(pEnable);
    
    //m_pTouMiaozhunItem->setEnabled(true);
    //m_pTouFasheItem->setEnabled(true);
}




void CFightScene::onBtnCreateBuilding(cocos2d::Ref *pSender){
    
    
    m_iNextBuildingId = (long)((Node*)pSender)->getUserData();
    
    
    //    if (CPersonalData::getInstance()->getBuildingInfoById(m_iNextBuildingId)) {
    //
    //
    //
    //    }
    
    
    
    m_bIfBuildingTower = true;
    
}





void CFightScene::fighterInit(){
    
    
    //初始化英雄
    float randZ = (ENDZPOINT-STARTZPOINT)/2+STARTZPOINT;
    
    //    FighterInfo* heroInfo = CPersonalData::getInstance()->getHero();
    //    CBaseFighter* theHero = addMonsterWithIdAndGroupAndPosition(heroInfo, eFightGroup_Freind, Vec2(Start3DPOINT-0.5*ONESIDE, randZ),eAttackType_hero);
    
    
    FighterInfo* cityInfo = CFightData::getInstance()->getTheFriendCityInfo();
    CBaseFighter* city = addMonsterWithIdAndGroupAndPosition(cityInfo, eFightGroup_Freind, Vec2(Start3DPOINT-0.3*ONESIDE, randZ),eAttackType_hero);
    
    //    theHero->stateChange(eFighteState_Idle);
    //    theHero->animateChange(eFighteState_Idle);
    
    
    //添加守护者
    int levelId = CFightData::getInstance()->getNowLevelId();
    
    if (levelId>=LEVEL_START_PROTECT && levelId < 1001&&levelId!=54)
    {//从23关开始，有美女助战
        addMonsterWithIdAndGroupAndPosition(CFightData::getInstance()->getTheProtectorInfo(), eFightGroup_Freind, Vec2(Start3DPOINT-0.1*ONESIDE, randZ),eAttackType_protector);
    }
}



void CFightScene::StartLaganActive()
{
    log("OnTouchLagan");
    
    //    m_pLaGan->setColor(Color3B::GRAY);
}

void CFightScene::unproject(const Mat4& viewProjection, const cocos2d::Size* viewport, Vec3* src, Vec3* dst)
{
    assert(dst);
    Vec4 screen(src->x / viewport->width, ((viewport->height - src->y)) / viewport->height, src->z, 1.0f);
    screen.x = screen.x * 2.0f - 1.0f;
    screen.y = screen.y * 2.0f - 1.0f;
    screen.z = screen.z * 2.0f - 1.0f;
    
    viewProjection.getInversed().transformVector(screen, &screen);
    
    if (screen.w != 0.0f)
    {
        screen.x /= screen.w;
        screen.y /= screen.w;
        screen.z /= screen.w;
    }
    
    dst->set(screen.x, screen.y, screen.z);
}


void CFightScene::calculateRayByLocationInView(Ray* ray,Camera* theCamera, const Vec2& location)
{
    //    auto dir = Director::getInstance();
    //    auto view = dir->getWinSize();
    //    auto mat = dir->getMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);
    //
    //    Vec3 src = Vec3(location.x, location.y, -1);
    //    Vec3 nearPoint;
    //    unproject(mat, &view, &src, &nearPoint);
    //
    //    src = Vec3(location.x, location.y, 1);
    //    Vec3 farPoint;
    //    unproject(mat, &view, &src, &farPoint);
    
    auto size = Director::getInstance()->getWinSize();
    
    
    //测试用，模型动画
    //        CFightData::getInstance()->getTheFriendHero()->testmode();
    Vec3 targetNear;
    Vec3 targetFar;
    Vec3 target;
    
    targetNear.x = location.x;
    targetNear.y = location.y;
    targetNear.z = -1;
    
    
    targetFar.x = location.x;
    targetFar.y = location.y;
    targetFar.z = 1;
    
    
    _camera->unproject(size, &targetNear, &targetNear);
    _camera->unproject(size, &targetFar, &targetFar);
    
    Vec3 farPoint = targetFar;
    Vec3 nearPoint = targetNear;
    
    Vec3 direction;
    Vec3::subtract(farPoint, nearPoint, &direction);
    direction.normalize();
    
    ray->_origin = nearPoint;
    ray->_direction = direction;
}


bool CFightScene::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event){
    
    
    if (m_iTouchExitState==1) {  //如果已经战斗结束了。点击则会直接跳出战斗胜利
        ifGameOver(0);
    }
    
    
    if(m_bFightPvpStart)
    {
        
        m_bSmallMapMoveStart = false;
        return true;
        
    }
    
    if(m_bPvpAllMapMove)
    {
        
        m_bSmallMapMoveStart = true;
        /*if(m_bFightPvpStart)
         {
         m_bSmallMapMoveStart = false;
         }*/
        
        
        return true;
    }
    
    //测试用，模型动画
    //    CFightData::getInstance()->getTheFriendHero()->testmode();
    
    //    addSoldierByType(eFighterType_line);
    
    m_vec2BuildPos = touch->getLocationInView();
    
    //    //如果是造建筑的话，就不继续走以下引导了
    //    if (addbuildingVirtual(touch)) {
    //        return true;
    //    }
    //
    
    auto s = Director::getInstance()->getWinSize();
    
    auto location = touch->getLocation();
    
    auto UILocation = touch->getLocationInView();
    
    cocos2d::Rect rect = m_pToushiActive->boundingBox();
    
    if(rect.containsPoint(location)&&m_pToushiActive->isVisible())
    {
        if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_USE_MANGONEL) && (guiderManagerLayer::getInstance()->getStepNum() == 4))
        {
            return false;
        }
        m_bToushiActive = true;
        m_bifNorAddSolider = false;
        
        m_paoWuLayer->m_startBall->runAction(RepeatForever::create(Sequence::create(TintTo::create(1.1, 150, 150, 150), TintTo::create(1.1, 255, 255, 255), NULL)));
        
        return false;
    }
    
    
    for(int i =0 ;i < m_starVec.size(); i++)
    {
        if(m_starVec.at(i).star->getBoundingBox().containsPoint(location))
        {
            m_bTouchStar = true;
            break;
        }
    }
    
    if (m_isTouch && !m_bTouchStar) {
        m_bifNorAddSolider = false;
        cocos2d::Point touchPoint = touch->getLocationInView() ;
        m_curPoint = touchPoint;
        touchPoint = Director::getInstance()->convertToGL(touchPoint) ;
        m_touchEndPoint = m_touchBeganPoint = touchPoint ;
        m_bTouched = true ;
        m_Index = 0 ;
        m_pointPath[m_Index++] = touchPoint * CC_CONTENT_SCALE_FACTOR();//起始点乘以特定因子
        m_pParticle->resetSystem() ;//清除所有的粒子
        m_pParticle->setPosition(touchPoint) ;
        
    }
    else
    {
        if(!m_isblow && !m_bTouchStar && !m_bTouchMap && !m_bRmvFlag){
            //            if (!m_isDraw) {
            //                return false;
            //            }
            if(
               //               !(m_pLaGan->boundingBox().containsPoint(touch->getLocation())) &&
               !(m_pCurView->boundingBox().containsPoint(touch->getLocation()))){
                m_startPos = Director::getInstance()->convertToGL(touch->getLocationInView());
                m_touchStar->resetSystem() ;//清除所有的粒子
                m_touchStar->setPosition(m_startPos) ;
            }
        }
        
        
        Vec3 v3Camera =  _camera->getPosition3D();
        m_bMovedView = false;
        
        if(!m_isblow && (location .y<BIGMAPHIGH||m_bPvpAllMapMove) && !m_bRmvFlag)
        {
            Sprite *pSmallMap = (Sprite*)getChildByTag(FIGHT_SMALLMAP);
            
            if(CFightData::getInstance()->getFightType() == FIGHT_PVP_TYPE)
            {
                if(m_bPvpAllMapMove)
                {
                    if(location.x<m_WinSize.width)
                    {
                        m_bSmallMapMoveStart = true;
                        MoveCameraWithSmallMapPos(location);
                    }
                }
                else
                {
                    if(location.x<pSmallMap->getBoundingBox().size.width)
                    {
                        m_bSmallMapMoveStart = true;
                        MoveCameraWithSmallMapPos(location);
                    }
                }
            }
            else
            {
                if(location.x<pSmallMap->getBoundingBox().size.width)
                {
                    m_bSmallMapMoveStart = true;
                    MoveCameraWithSmallMapPos(location);
                }
            }
            
            
        }else{
            m_bSmallMapMoveStart = false;
        }
        
    }
    
    return true;
}

void CFightScene::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event)
{
    
    if (m_iTouchExitState>=1) {  //如果已经战斗结束了。移动就没有反应了
        onTouchCancelled(touch, event);
    }
    
    m_bPvpMove = true;
    auto location = touch->getLocation();
    
    auto UILocation = touch->getLocationInView();
    
    if(m_bToushiMove)
    {
        if(m_guiderArrow && m_guiderArrow->isVisible())
        {
            m_guiderArrow->stopActionByTag(10);
            m_guiderArrow->setVisible(false);
            if(m_GuideNode)
            {
                m_GuideNode->removeFromParent();
                m_GuideNode = NULL;
            }
        }
        
       // float x = m_paoWuLayer->m_startBall->getPositionX();
        
        Vec2 tempPos = CommonData::GetSingletonPtr()->getPosOnLayer3D(_camera, UILocation);//获取的屏幕中心点的3D坐标
        

        
       if(tempPos.y>=ENDZPOINT &&tempPos.y<=(STARTZPOINT))
        {
            
            Vec3 VPaowu = m_paoWuLayer->m_startBall->getPosition3D();
            
            m_paoWuLayer->m_startBall->setPosition3D(Vec3(VPaowu.x,VPaowu.y,tempPos.y));
            m_paoWuLayer->m_theBall->setPosition3D(Vec3(VPaowu.x,VPaowu.y,tempPos.y));
            m_paoWuLayer->setUpdateBallStarVec3(Vec3(VPaowu.x,VPaowu.y,tempPos.y)-m_paoWuLayer->getCarOffsetVec3());
        }
        return ;
    }
    
    if (m_isTouch && !m_bTouchStar)
    {
        if(m_guiderArrow && (guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SKILL_CUT)
           && (guiderManagerLayer::getInstance()->getStepNum() == 5))
        {
            m_guiderArrow->removeFromParent();
            m_guiderArrow = NULL;
            //NotificationCenter::getInstance()->postNotification("StartCut");
        }
        
        m_curPoint = touch->getLocationInView();
        m_touchBeganPoint = Director::getInstance()->convertToGL(m_curPoint) ;
        cocos2d::Point prePos = touch->getPreviousLocationInView();
        m_bTouched = true ;
        m_pParticle->setVisible(true) ;
        m_pParticle->setPosition(m_touchBeganPoint) ;
        
        cocos2d::Point pointTmp = m_touchBeganPoint*CC_CONTENT_SCALE_FACTOR();
        //float distance = pointTmp.distance(m_pointPath[m_Index]);
        
        if ( m_Index < POINT_NUM )
        {
            m_pointPath[m_Index++] = pointTmp ;
        }
        else {
            memmove(m_pointPath, &m_pointPath[1], sizeof(cocos2d::Point)*(POINT_NUM-1)) ;
            m_pointPath[m_Index-1] = pointTmp ;
        }
        if(m_isFirst){
            m_isFirst = false;
            m_directX = m_curPoint.x - prePos.x;
            m_directY = m_curPoint.y - prePos.y;
        }
        
        
        if ((m_directX * (m_curPoint.x - prePos.x) < 0)
            || (m_directY * (m_curPoint.y - prePos.y) < 0)) {//如果方向相反则算为一次伤害
            m_directX = 0;
            m_directY = 0;
            m_hurtEnemyVec.clear();
            
            //方向相反则刀光数量也被重置
            m_iKnifeNum=0;
            
            return;
        }
        else{
            m_directX = m_curPoint.x - prePos.x;
            m_directY = m_curPoint.y - prePos.y;
        }
        
        //检测士兵受到的伤害
        auto obbSize = m_obb.size();
        if(obbSize > 0)
        {
            Ray ray;
            calculateRayByLocationInView(&ray,_camera, m_curPoint);
            for(decltype(obbSize) i = 0; i < obbSize; i++)
            {
                if(ray.intersects(m_obb.at(i).obb))
                {
                    CCLOG("啊啊啊啊啊");
                    SimpleAudioEngine::getInstance()->playEffect("music/cut.mp3");
                    Vec3 pos = m_obb.at(i).enemy->getPhysicalPosition();
                    m_obb.at(i).enemy->m_isCut = true;
                    if (!m_obb.at(i).enemy->getIfDied() && (m_obb.at(i).enemy->getNowHP() > 0)) {
                        addCutEffect(pos, m_obb.at(i).enemy);
                        
                        /*
                         如果切到了，需要额外加一个特效
                         */
                        
                        
                        if (m_iKnifeNum<m_iKnifeMax) { //如果刀光数没有到最大值，则显示刀光
                            m_iKnifeNum++;
                            
                            //上一个触摸点
                            cocos2d::Point lastPos = touch->getPreviousLocation();
                            
                            float degree = CFightManager::getTheDegreeWith(lastPos, m_touchBeganPoint, false,Vec2(1, 0));
                            
                            Sprite* tempKnife = Sprite::createWithSpriteFrameName("daoguang.png");
                            tempKnife->setRotation(degree);
                            
                            Vec2 targetPos = CommonData::GetSingletonPtr()->getPosOnScreen(_camera, pos);
                            tempKnife->setPosition(targetPos);
                            addChild(tempKnife,100);
                            
                            tempKnife->setScale(0);
                            tempKnife->runAction(Sequence::create(ScaleTo::create(0.1,5),ScaleTo::create(0.1,0),CallFunc::create([=](){
                                
                                m_iKnifeNum--;
                                
                                m_iKnifeNum = MAX(m_iKnifeNum, 0);
                            }),RemoveSelf::create(), NULL));
                        }
                        
                        
                        
                        
                    }
                }
            }
        }
        if(m_skillTime == 0)
        {
            m_bifNorAddSolider = false;
        }
    }
    else
    {
        //投石车移动
        if(m_bMoveToushiche)
        {
            float x = m_paoWuLayer->m_startBall->getPositionX();
            Vec2 t  = CommonData::GetSingletonPtr()->getPosOnLayer3D(_camera,Vec2(UILocation.x,UILocation.y));
            
            
            if(t.y>=ENDZPOINT &&t.y<=STARTZPOINT&&t.x>x)
            {
                m_paoWuLayer->m_startBall->setPosition3D(Vec3(x,0,t.y));
                m_paoWuLayer->m_theBall->setPosition3D(Vec3(x,0,t.y));
            }
            return;
        }
        else
        {
            if (!m_bTouchMap && !m_isblow && (!m_pCurView->getBoundingBox().containsPoint(touch->getLocation())) && m_bifNorAddSolider)
            {
                remvGuideDraw();
                saveTouchPoint(touch);
            }
        }
        
        
        if(m_bSmallMapMoveStart)
        {
            //小地图移动，则把建造框移除
            if (m_pBuildingControll) {
                m_pBuildingControll->setTheBuildingInfo(NULL);
            }
            
            Sprite *pSmallMap = (Sprite*)getChildByTag(FIGHT_SMALLMAP);
            if(CFightData::getInstance()->getFightType() == FIGHT_PVP_TYPE)
            {
                if(m_bPvpAllMapMove)
                {
                    auto preLocation = touch->getPreviousLocation();
                    auto location = touch->getLocationInView();
                    float xPos = (preLocation.x - location.x);
                    
                    float CameraPosX = _camera->getPositionX() + xPos;
                    
                    _camera->setPosition3D(Vec3(CameraPosX , _camera->getPositionY(),_camera->getPositionZ()));
                    
                    if(_camera->getPosition3D().x<Start3DPOINT)
                    {
                        _camera->setPosition3D(Vec3(Start3DPOINT,_camera->getPositionY(),_camera->getPositionZ()));
                    }
                    
                    else if(_camera->getPosition3D().x>END3DPOINT)
                    {
                        _camera->setPosition3D(Vec3(END3DPOINT,_camera->getPositionY(),_camera->getPositionZ()));
                    }
                    
                    
                }
                else
                {
                    if(location.x<=pSmallMap->getBoundingBox().size.width)
                    {
                        MoveCameraWithSmallMapPos(location);
                    }
                }
            }
            else
            {
                if(location.x<=pSmallMap->getBoundingBox().size.width)
                {
                    MoveCameraWithSmallMapPos(location);
                }
            }
            if(m_guiderArrow && m_GuideNode && m_GuideNode->isVisible())
            {
                m_guiderArrow->stopActionByTag(10);
                m_guiderArrow->setVisible(false);
                if(m_GuideNode)
                {
                    m_GuideNode->removeFromParent();
                    m_GuideNode = NULL;
                }
            }
            if(((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SKILL_CUT)
                && (guiderManagerLayer::getInstance()->getStepNum() == 3))
               ||((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SKILL_BLOW)
                  && (guiderManagerLayer::getInstance()->getStepNum() == 5)))
            {
                adjustMoveMapPos(touch, event);
            }
            
        }
        //小地图view
    }
}

void CFightScene::adjustMoveMapPos(cocos2d::Touch *touch, cocos2d::Event *event)
{
    cocos2d::Size winSize = Director::getInstance()->getWinSize();
    cocos2d::Point midPoint = Vec2(winSize.width / 2, winSize.height / 2);
    Vec2 tempPos = CommonData::GetSingletonPtr()->getPosOnLayer3D(_camera, midPoint);//获取的屏幕中心点的3D坐标
    Vec3 Mid3DPos = Vec3(tempPos.x, 0, tempPos.y);
    float distance = CR_MAP_RANCGE;
    if(guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SKILL_BLOW)
    {
        distance = CR_MAP_BLOW_RANCGE;
    }
    
    cocos2d::Vector<CBaseFighter*>* enemyVec = CFightData::getInstance()->getAllEnemy();
    for (cocos2d::Vector<CBaseFighter*>::iterator it = enemyVec->begin() ; it != enemyVec->end(); it++)
    {
        CBaseFighter* enemy = *it;
        if (enemy && Mid3DPos.distance(enemy->getPosition3D()) <= distance && m_guiderArrow)//技能对于屏幕两侧300码以内的敌方士兵有效
        {
            m_bEndGuide = true;
            bArrow = false;
            m_guiderArrow->removeFromParent();
            m_guiderArrow = NULL;
            Vec2 pos = m_pCutMenu->convertToWorldSpace(Vec2::ZERO);
            if(guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SKILL_BLOW)
            {
                pos = m_pBlowMenu->convertToWorldSpace(Vec2::ZERO);
            }
            CFightScene::onTouchEnded(touch, event);
            guiderManagerLayer::getInstance()->createLayer(this, GUIDE_SHOW_BOTH, pos);
            guiderManagerLayer::getInstance()->setStepNum(guiderManagerLayer::getInstance()->getStepNum() + 1);
            break;
        }
    }
}

void CFightScene::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event)
{
    log("摄像机的当前位置:%f, %f，%f",_camera->getPositionX(), _camera->getPositionY(), _camera->getPositionZ());
    //    //如果是造建筑的话，就不继续走以下引导了
    //    if (addbuildingVirtual(touch)) {
    //        return;
    //    }
    
    
    if(m_bToushiActive)
    {
        m_paoWuLayer->RemoveTouch();
        modelInfo* pModelInfo = CDataManager::getModelInfoByModelId(1016);
        
        //m_bifNorAddSolider = true;
        m_bToushiActive = false;
        //出兵
    }
    
    cocos2d::Point touchPoint = touch->getLocationInView() ;
    
    
    if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SKILL_BLOW) && (guiderManagerLayer::getInstance()->getStepNum() == 5))
    {
        if(!m_bEndGuide)
        {
            if(m_guiderArrow && (!m_guiderArrow->getActionByTag(10)))
            {
                ActionInterval* action1 = MoveBy::create(1.25f, Vec2(300, 0));
                ActionInterval* action2 = MoveBy::create(0, Vec2(-300, 0));
                ActionInterval* seq = RepeatForever::create(Sequence::create(action1, action2, NULL));
                seq->setTag(10);
                m_guiderArrow->runAction(seq);
                m_guiderArrow->setVisible(true);
            }
            
        }
        
        //        if(m_guiderArrow && (m_pCurView->getPositionX() - m_pCurView->boundingBox().size.width*0.5 > 300))
        //        {
        //            bArrow = false;
        //            m_guiderArrow->removeFromParent();
        //            m_guiderArrow = NULL;
        //            Vec2 pos = Vec2(m_WinSize.width * 0.865f, m_WinSize.height * 0.38);
        //            guiderManagerLayer::getInstance()->createLayer(this, GUIDE_SHOW_BOTH, pos);
        //
        //        }
        //        else{
        //            if(m_guiderArrow && (!m_guiderArrow->getActionByTag(10)))
        //            {
        //                ActionInterval* action1 = MoveBy::create(1.25f, Vec2(300, 0));
        //                ActionInterval* action2 = MoveBy::create(0, Vec2(-300, 0));
        //                ActionInterval* seq = RepeatForever::create(Sequence::create(action1, action2, NULL));
        //                seq->setTag(10);
        //                m_guiderArrow->runAction(seq);
        //                m_guiderArrow->setVisible(true);
        //            }
        //        }
    }
    else if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SKILL_CUT) && (guiderManagerLayer::getInstance()->getStepNum() == 3))
    {
        if(!m_bEndGuide)
        {
            if(m_guiderArrow && (!m_guiderArrow->getActionByTag(10)))
            {
                ActionInterval* action1 = MoveBy::create(1.25f, Vec2(300, 0));
                ActionInterval* action2 = MoveBy::create(0, Vec2(-300, 0));
                ActionInterval* seq = RepeatForever::create(Sequence::create(action1, action2, NULL));
                seq->setTag(10);
                m_guiderArrow->runAction(seq);
                m_guiderArrow->setVisible(true);
            }
            
        }
        
        //        if(m_guiderArrow && (m_pCurView->getPositionX() - m_pCurView->boundingBox().size.width*0.5 > 300))
        //        {
        //            bArrow = false;
        //            m_guiderArrow->removeFromParent();
        //            m_guiderArrow = NULL;
        //            Vec2 pos = Vec2(m_WinSize.width * 0.946f, m_WinSize.height * 0.445);
        //            guiderManagerLayer::getInstance()->createLayer(this, GUIDE_SHOW_BOTH, pos);
        //        }
        //        else{
        //            if(m_guiderArrow && (!m_guiderArrow->getActionByTag(10)))
        //            {
        //                ActionInterval* action1 = MoveBy::create(1.25f, Vec2(300, 0));
        //                ActionInterval* action2 = MoveBy::create(0, Vec2(-300, 0));
        //                ActionInterval* seq = RepeatForever::create(Sequence::create(action1, action2, NULL));
        //                seq->setTag(10);
        //                m_guiderArrow->runAction(seq);
        //                m_guiderArrow->setVisible(true);
        //            }
        //        }
    }
    else if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_ADD_LINE) && (guiderManagerLayer::getInstance()->getStepNum() == 7))
    {
        if(m_bSmallMapMoveStart && m_guiderArrow && (m_pCurView->getPositionX() - m_pCurView->boundingBox().size.width*0.5 > 50))
        {
            m_guiderArrow->removeFromParent();
            m_guiderArrow = NULL;
            bArrow = false;
            m_bTouchMap = false;
            guiderManagerLayer::getInstance()->createLayer(this);
        }
        else{
            if(m_guiderArrow && (!m_guiderArrow->getActionByTag(10)))
            {
                ActionInterval* action1 = MoveBy::create(1.25f, Vec2(300, 0));
                ActionInterval* action2 = MoveBy::create(0, Vec2(-300, 0));
                ActionInterval* seq = RepeatForever::create(Sequence::create(action1, action2, NULL));
                seq->setTag(10);
                m_guiderArrow->runAction(seq);
                m_guiderArrow->setVisible(true);
            }
        }
    }
    else if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_USE_MANGONEL) && (guiderManagerLayer::getInstance()->getStepNum() == 4))
    {
        Vec3 tempPos = m_paoWuLayer->m_startBall->getPosition3D();
        if(m_bToushiMove && m_guiderArrow)// && fabs(m_paoWuLayer->m_startBall->getPosition3D().z - m_paoWuLayer->m_startPosVec3.z) > 50)
        {
            if(m_guiderArrow)
            {
                m_guiderArrow->removeFromParent();
                m_guiderArrow = NULL;
            }
            bArrow = false;
            m_bToushiMove = false;
            guiderManagerLayer::getInstance()->createLayer(this);
        }
        else{
            if(m_guiderArrow && (!m_guiderArrow->getActionByTag(10)))
            {
                ActionInterval* action1 = MoveBy::create(1.25f, Vec2(0, 150));
                ActionInterval* action2 = MoveBy::create(0, Vec2(0, -150));
                ActionInterval* seq = RepeatForever::create(Sequence::create(action1, action2, NULL));
                seq->setTag(10);
                m_guiderArrow->runAction(seq);
                m_guiderArrow->setVisible(true);
            }
        }
    }
    else if(m_bPvpMove && (guiderManagerLayer::getInstance()->getGuideType() == GUIDE_PVP_ROB)
           && (guiderManagerLayer::getInstance()->getStepNum() == 5))
    {
        guiderManagerLayer::getInstance()->createLayer(this);
    }
    
    
    if(m_bTouchStar&&CFightData::getInstance()->getWinState()==0)
    {
        for(int i =0 ;i < m_starVec.size(); i++)
        {
            if(m_starVec.at(i).star->getBoundingBox().containsPoint(touch->getLocation()))
            {
                m_bTouchStar = false;
                Director::getInstance()->pause();
                if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_LEVEL_STAR) && (guiderManagerLayer::getInstance()->getStepNum() == 1) && (CFightData::getInstance()->getNowLevelId() == 3))
                {
                    CDataManager::saveGuideInfoToDB(GUIDE_LEVEL_STAR);
                    guiderManagerLayer::getInstance()->setStepNum(0);
                    guiderManagerLayer::getInstance()->removeGuiderLayer();
                }
                CommonPropLayer* layer = CommonPropLayer::create(LEVEL_STAR_TYPE);
                addChild(layer, 999);;
                break;
            }
        }
    }
    else if (m_isTouch)
    {
        touchPoint = Director::getInstance()->convertToGL(touchPoint) ;
        m_bTouched = false;
        m_touchEndPoint = touchPoint;
        m_pParticle->stopSystem();
    }
    else
    {
        if(m_bMoveToushiche)
        {
            // m_pTouMiaozhunItem->setVisible(true);
            // m_pTouFasheItem->setVisible(true);
            m_bMoveToushiche = false;
            return;
        }
        
        else if(0)
        {
            //            m_pLaGan->setColor(Color3B::WHITE);
            //            m_bLaganStart = false;
            //            m_pLanGanDian->setPosition(m_WinSize.width*0.1,m_WinSize.height*0.26);
            CBaseHero* pFight  = CFightData::getInstance()->getTheFriendHero();
            if (pFight) {
                pFight->LaganStop();
            }
        }
        else{
            identifyTouch(touch);
        }
    }
    m_bTouchStar = false;
    m_bPvpMove = false;
    //    if(!m_bifNorAddSolider)
    //        m_bifNorAddSolider = true;
    
    
    
    //是否选中建筑的判断。在触摸画图数较少,并且距离较短时判断
    
    if (m_vecDrawSprite.size()<20&&m_vec2BuildPos.distance(touchPoint)<20) {
        
        if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_TOWER_LEVEL) && !(CDataManager::getEndedBy(GUIDE_TOWER_LEVEL)))
        {
            if(guiderManagerLayer::getInstance()->getStepNum() >= 6)
            {
                buildingSelect(touchPoint);
                if(guiderManagerLayer::getInstance()->getStepNum() == 6)
                {
                    guiderManagerLayer::getInstance()->removeGuiderLayer();
                }
            }
        }
        else{
            buildingSelect(touchPoint);
        }
    }
    
    
    
    
    //删除所有画图时的图片
    for (int i=0; i<m_vecDrawSprite.size(); i++) {
        
        Sprite* temp = m_vecDrawSprite.at(i);
        
        temp->removeFromParentAndCleanup(true);
    }
    m_vecDrawSprite.clear();
    
    
    m_bSmallMapMoveStart = false;
}



CBaseFighter *CFightScene::SoliderSelect(cocos2d::Vec2 touchPos)
{
    cocos2d::Vector<CBaseFighter*> soliderVec = CFightData::getInstance()->getAllAttacker();
    
    CBaseFighter* target = NULL;
    
    
    log("soliderVec =%zi\n",soliderVec.size());
    
    if(soliderVec.size()>0)
    {
        
        Ray ray;
        calculateRayByLocationInView(&ray,_camera, touchPos);
        
        
        for(int i=0;i<soliderVec.size();i++)
        {
            CBaseFighter* tempTarget = soliderVec.at(i);
            
            OBB  t(soliderVec.at(i)->gettheSprite()->getAABB());
            
            if(ray.intersects(t))
            {
                if(!target)
                {
                    target = tempTarget;
                }
                else
                {   //选中状态下z轴最大的
                    if (tempTarget->getPositionZ()>target->getPositionZ())
                    {
                        target = tempTarget;
                    }
                }
                
            }
            
        }
    }
    
    return target;
}


CBaseFighter* CFightScene::buildingSelect(cocos2d::Vec2 touchPos){
    
    std::vector<obbInfo>* theObb = CFightData::getInstance()->getAllTowerObb();
    
    CBaseBuilding* target = NULL;
    
    if(theObb->size() > 0)
    {
        Ray ray;
        calculateRayByLocationInView(&ray,_camera, touchPos);
        
        
        bool ifSelect=false;
        
        for(int i = 0; i < theObb->size(); i++)
        {
            
            CBaseBuilding* tempTarget = NULL;
            
            if (theObb->at(i).theFighter->getTheInfo()->monsterType==eFighterType_Tower ) {
                tempTarget = (CBaseBuilding*)theObb->at(i).theFighter;
            }else{
                CCLOG("LYError: building wrong");
                return NULL;
            }
            
            
            if(ray.intersects(theObb->at(i).obb))
            {
                
                //为了防止选中多个，我们总是优先选择z轴大的，这样防止选错
                
                if(!target){
                    
                    target = tempTarget;
                    target->buildingSelect();
                    
                    m_pBuildingControll->setTheBuildingInfo(target);
                    
                    ifSelect = true;
                    
                }else{
                    
                    if (tempTarget->getPositionZ()>target->getPositionZ()) {
                        
                        target->buildingUnselect();
                        
                        tempTarget->buildingSelect();
                        m_pBuildingControll->setTheBuildingInfo(tempTarget);
                        
                        target = tempTarget;
                        
                        ifSelect = true;
                        
                    }else{
                        tempTarget->buildingUnselect();
                    }
                    
                }
                
            }else{
                
                tempTarget->buildingUnselect();
                
            }
            
        }
        
        //如果没有选中任何建筑，则建筑控制圈消失
        if (!ifSelect && (CDataManager::getEndedBy(GUIDE_TOWER_LEVEL))) {
            m_pBuildingControll->setTheBuildingInfo(NULL);
        }
        
    }
    
    
    return target;
    
}




bool CFightScene::addbuildingVirtual(cocos2d::Vec2 buildPos){
    
    if (m_iNextBuildingId>0&&m_bIfBuildingTower==true) {
        
        
        if(!m_pPreparedBuilding){
            
            monsterInfo* temp = CDataManager::getMonsterInfoByMonsterId((int)m_iNextBuildingId);
            
            modelInfo* TheModel = CDataManager::getModelInfoByModelId(temp->modelId);
            
            m_pPreparedBuilding = Sprite3D::create(TheModel->modelName.getCString());
            
            m_pPreparedBuilding->setScale(TheModel->modelScale);
            
            m_pPreparedBuilding->setOpacity(100);
            m_pPreparedBuilding->setColor(Color3B::GREEN);
            
            addChild(m_pPreparedBuilding);
        }
        
        
        //坐标转换为地图3d坐标
        Vec2 thePos = CommonData::GetSingletonPtr()->getPosOnLayer3D(_camera, buildPos);
        
        //坐标转换为可建造建筑的坐标
        thePos = CFightManager::getThePosInMap(thePos);
        
        m_pPreparedBuilding->setPosition3D(Vec3(thePos.x,0,thePos.y));
        
        CFightManager::addCamera(m_pPreparedBuilding);
        
        return true;
    }
    
    
    return false;
}


CBaseFighter* CFightScene::addbuildingReal(cocos2d::Vec2 buildPos){
    
    
    //    m_pPreparedBuilding->removeFromParentAndCleanup(true);
    
    FighterInfo* monsterInfo=CPersonalData::getInstance()->getBuildingInfoById((int)m_iNextBuildingId);
    
    
    //坐标转换为地图3d坐标
    Vec2 thePos = CommonData::GetSingletonPtr()->getPosOnLayer3D(_camera, buildPos);
    
    //坐标转换为可建造建筑的坐标
    thePos = CFightManager::getThePosInMap(thePos);
    
    
    
    
    
    if (!buildingPosSelect(&thePos)) {
        return addMonsterWithIdAndGroupAndPosition(monsterInfo, eFightGroup_Freind, thePos,eAttackType_tower);
    }else{
        return NULL;
    }
    
    
    
    
    
}


void CFightScene::saveTouchPoint(cocos2d::Touch *touch)
{
    
    if (m_ePVPMode==EPVPMode_REC) {
        return;
    }
    
    cocos2d::Point location = touch->getLocationInView();
    m_touchStar->setVisible(true) ;
    m_touchStar->setPosition(Director::getInstance()->convertToGL(location)) ;
    
    m_pointArr.push_back(touch->getLocation());
    Point2D p_Point2DTemp;
    p_Point2DTemp.x=location.x;
    p_Point2DTemp.y=location.y;
    p_2dPath.push_back(p_Point2DTemp);//存储触摸点
    
    
    //绘图
    cocos2d::Point point1 = touch->getPreviousLocation();
    cocos2d::Point point2 = touch->getLocation();
    
    Sprite* temp = Sprite::createWithSpriteFrameName("fight_drawLine.png");
    
    temp->getTexture()->setAliasTexParameters();
    
    float degree = CFightManager::getTheDegreeWith(point1, point2, false,Vec2(1, 0));
    float dist = point1.getDistance(point2);
    float scaleX = dist/temp->getBoundingBox().size.width*1.05f;
    
    temp->setAnchorPoint(Vec2(0,0.5f));
    temp->setScaleX(scaleX);
    
    temp->setPosition(point1);
    temp->setRotation(degree);
    addChild(temp,10000);
    
    temp->runAction(Sequence::create(DelayTime::create(0.5f),ScaleTo::create(2, scaleX,0.2f),DelayTime::create(1),ScaleTo::create(2, scaleX,0), NULL));
    m_vecDrawSprite.pushBack(temp);
    
}



void CFightScene::identifyTouch(cocos2d::Touch *touch)
{
    
    
    cocos2d::Point endPos = touch->getLocationInView();
    m_touchStar->stopSystem();
    if (p_2dPath.size() < 3){
        p_2dPath.clear();
        m_pointArr.clear();
        return ;
    }
    //m_touchStar->setVisible(false);
    //对保存的触摸点顺序逆序都做一次识别，去识别度较高的结果作为判断结果
    RecognitionResult resultOrder = g_rGemertricRecognizer->recognize(p_2dPath);
    
    
    
    Path2D reversePath;
    for (int i = p_2dPath.size() - 1; i >= 0; i--)
    {
        reversePath.push_back(p_2dPath.at(i));
    }
    if((fabs(endPos.x - m_startPos.x) < 60) && (fabs(endPos.y - m_startPos.y) < 50 && (reversePath.size() < 20)))
    {
        m_isValid = false;
    }
    RecognitionResult resultReverse = g_rGemertricRecognizer->recognize(reversePath);
    

    
    if(((resultReverse.name == "RightSquareBracket") && (resultOrder.name == "Circle"))//优化矩形识别准确性
       || ((resultReverse.name == "Circle") && (resultOrder.name == "RightSquareBracket"))){
        m_GraphicsName = "Rectangle";
        if (resultOrder.name == "Circle") {
            m_similarity = resultOrder.score;
        }
        else{
            m_similarity = resultReverse.score;
        }
    }
    else{
        if (resultReverse.score > resultOrder.score)
        {
            m_GraphicsName = resultReverse.name;
            m_similarity = resultReverse.score;
        }
        else
        {
            m_GraphicsName = resultOrder.name;
            m_similarity = resultOrder.score;
        }
    }
    
    
    for (int i = 0; i < 6; i++)
    {
        if(m_GraphicsName == templateName[i])
        {
            CCLOG("没有对应的兵种！");
            p_2dPath.clear();
            m_pointArr.clear();
            //return;
        }
    }
    
    if(m_GraphicsName == "LeftSquareBracket" || m_GraphicsName == "RightSquareBracket"
       || m_GraphicsName == "LeftCurlyBrace" || m_GraphicsName == "RightCurlyBrace"
       || (m_GraphicsName == "Unknown" && m_similarity == 1.000000))
    {
        m_GraphicsName = "LineDownDiagonal";
        CCLOG("直线");
    }
    else if(m_GraphicsName == "X" || m_GraphicsName == "Delete")
    {
        m_GraphicsName = "Triangle";
    }
    else if(m_GraphicsName == "Diamond")
    {
        m_GraphicsName = "Circle";
    }
    if (m_isValid) {
        //m_isDraw = false;
        m_bIsshowResult = true;//开始显示识别结果
    }
    if( ((m_GraphicsName == "LineDownDiagonal") && (fabsf(p_2dPath.at(p_2dPath.size()-1).x - p_2dPath.at(0).x) < 15)
         && (fabsf(p_2dPath.at(p_2dPath.size()-1).y - p_2dPath.at(0).y) < 50))
       || ((m_GraphicsName == "LineDownDiagonal") && (fabsf(p_2dPath.at(p_2dPath.size()-1).x - p_2dPath.at(0).x) < 50)
           && (fabsf(p_2dPath.at(p_2dPath.size()-1).y - p_2dPath.at(0).y < 15))))
    {
        p_2dPath.clear();
        m_pointArr.clear();
        return;
    }

    updateIdentify();
}

void CFightScene::updateIdentify()
{
    
    bool bGuide = false;
    if(CFightData::getInstance()->getNowMonsterNum()>=CFightData::getInstance()->getMaxMonsterNum()&&CFightData::getInstance()->getFightType()!=FIGHT_TOWERDENFESE_TYPE)
    {
        
        Sprite* tipLabel = CFightManager::getTheFlyWord("已经达到士兵数量上限", "fonts/cuti.ttf", 30);
        tipLabel->setPosition(Vec2(m_WinSize.width / 2, m_WinSize.height * 0.7f));
        //        tipLabel->runAction(Sequence::create(MoveBy::create(0.5f, Vec2(0, 100)),DelayTime::create(0.5f),RemoveSelf::create(),NULL));
        addChild(tipLabel);
        p_2dPath.clear();
        m_pointArr.clear();
        m_opcity = 250;
        
        //相关动作
        //m_layer->addChild(soliderNumIcon,0,FIGHT_SOLDIER_NUMTAG);
        
        Sprite *pSoldier = (Sprite *)m_layer->getChildByTag(FIGHT_SOLDIER_NUMTAG);
        
        
        Sprite *pSoldierBg = (Sprite *)m_layer->getChildByTag(FIGHT_SOLDIER_NUMBGTAG);
        
        
        pSoldier->runAction(Sequence::create(MoveBy::create(0.2,Vec2(0,15)),MoveBy::create(0.2,Vec2(0,-15)),MoveBy::create(0.2,Vec2(0,15)),MoveBy::create(0.2,Vec2(0,-15)),NULL));
        
        
        Label *pText = (Label*)pSoldierBg ->getChildByTag(100);
        
        pText->runAction(Sequence::create(TintTo::create(0.2,Color3B(255,-255,0)),TintTo::create(0.2,Color3B::WHITE),TintTo::create(0.2,Color3B(255,-255,0)),TintTo::create(0.2,Color3B::WHITE),NULL));
        
        
        return;
    }
    FighterType theFightType;
    
    if (m_GraphicsName == "LineDownDiagonal") {
        theFightType = eFighterType_line;
    }
    else if(m_GraphicsName == "Circle"){
        theFightType = eFighterType_circle;
    }
    else if(m_GraphicsName == "Triangle"){
        theFightType = eFighterType_triangle;
    }
    else if(m_GraphicsName == "Rectangle"){
        theFightType = eFighterType_square;
    }
    else if(m_GraphicsName == "V"){
        theFightType = eFighterType_v;
    }else if(m_GraphicsName == "CheckMark"){
        theFightType = eFighterType_Tower;
    }
    
    
    FighterInfo* monsterInfo = NULL;
    
    //如果是塔防模式的话，则找塔的信息
    if (CFightData::getInstance()->getTheLevelCondition()==eLevelCondition_TowerDefense) {//如果是塔防模式，则寻找塔的信息
        
        if ((theFightType==eFighterType_Tower) || (theFightType == eFighterType_v)) {
            
            m_GraphicsName = "CheckMark";
            theFightType = eFighterType_Tower;
            m_iNextBuildingId= TD_BASE_TOWER_ID;//暂时写死monsterid，到时候改
            monsterInfo=CPersonalData::getInstance()->getBuildingInfoById((int)m_iNextBuildingId);
        }
    }else
    {
        if(theFightType==eFighterType_Tower)
        {
            m_GraphicsName = "V";
            theFightType = eFighterType_v;
        }
        monsterInfo = CPersonalData::getInstance()->getTeamSolderByType(theFightType);
    }
    
    
    //    FighterInfo* monsterInfo = CPersonalData::getInstance()->getTeamSolderByType(theFightType);
    
    //如果没取到士兵，则提示错误
    bool ifWrong = false;
    if (!monsterInfo) {
        //        Label * thetarget = CFightManager::getTheFlyWord("没有对应士兵");
        //        thetarget->setPosition(Vec2(m_WinSize.width*0.5f,m_WinSize.height*0.5f));
        //        addChild(thetarget);
        ifWrong = true;
        
    }
    
    
    if(monsterInfo )
    {
        Sprite *pShape  = NULL;
        
        if(monsterInfo->monsterType ==eFighterType_line )
        {
            pShape  =  (Sprite*)m_layer->getChildByTag(200 + eFighterType_line);
        }
        else if(monsterInfo->monsterType ==eFighterType_triangle)
        {
            pShape = (Sprite*)m_layer->getChildByTag(200 + eFighterType_triangle);
        }
        else if(monsterInfo->monsterType ==eFighterType_circle)
        {
            pShape = (Sprite *)m_layer->getChildByTag(200 + eFighterType_circle);
        }
        else if(monsterInfo->monsterType == eFighterType_square)
        {
            pShape = (Sprite*)m_layer->getChildByTag(200 + eFighterType_square);
        }
        else if(monsterInfo->monsterType == eFighterType_v)
        {
            pShape = (Sprite*)m_layer->getChildByTag(200 + eFighterType_v);
        }
        
        
        
        if (CFightData::getInstance()->getTheLevelCondition()==eLevelCondition_TowerDefense) {
            
            fightBuildingInfo* target = CDataManager::getTheFightBuildingInfoWithId(monsterInfo->monsterId);
            
            if (target&&CFightData::getInstance()->getStone()<target->buildCost)
            {
                Sprite * thetarget = CFightManager::getTheFlyWord("能量值不足","fonts/cuti.ttf");
                
                thetarget->setPosition(Vec2(m_WinSize.width*0.5f,m_WinSize.height*0.7f));
                addChild(thetarget);
                ifWrong = true;
            }
            
        }else if((CFightData::getInstance()->getNowEnergy() < monsterInfo->energyCost)) {
            
            Sprite * thetarget = CFightManager::getTheFlyWord("能量值不足","fonts/cuti.ttf");
            
            thetarget->setPosition(Vec2(m_WinSize.width*0.5f,m_WinSize.height*0.7f));
            addChild(thetarget);
            ifWrong = true;
            pShape->runAction(Sequence::create(MoveBy::create(0.2,Vec2(10,0)),MoveBy::create(0.2,Vec2(-10,0)),MoveBy::create(0.2,Vec2(10,0)),MoveBy::create(0.2,Vec2(-10,0)),NULL));
            Sprite *pEnemyColor = m_pTheEnergyBar->getSprite();
            
            // p1->setTexture("enemgyline2.png");
            // p1->setScaleY(10);
            
            
            m_pTheEnergyBar->runAction(Sequence::create(DelayTime::create(0.2),CallFunc::create([=]{ pEnemyColor->setSpriteFrame("enemgyline2.png");}),DelayTime::create(0.1),CallFunc::create([=]{ pEnemyColor->setSpriteFrame("enemgyline.png");}),NULL));
            
            
        }
        
    }
    //更新引导状态
    if((CFightData::getInstance()->getNowMonsterNum() >= 0) && (guiderManagerLayer::getInstance()->getGuideType() == GUIDE_ADD_LINE) && (guiderManagerLayer::getInstance()->getStepNum() == 3))
    {
        if(theFightType == eFighterType_line)
        {
            if(m_bGuideBlow)
            {
                m_bGuideBlow = false;
                CFightData::getInstance()->setStopAddEnemy(false);
            }
            
            CDataManager::saveGuideInfoToDB(GUIDE_ADD_LINE);
            remvGuideDraw();
            
            guiderManagerLayer::getInstance()->createLayer(this);
        }
        else{
            if(!guiderManagerLayer::getInstance()->getstart())
            {
                guiderManagerLayer::getInstance()->setstart(true);
                bGuide = true;
                //return;
            }
        }
    }
    //    if((theFightType == eFighterType_line) && (CFightData::getInstance()->getNowMonsterNum() >= 2) && (guiderManagerLayer::getInstance()->getStepNum() == 6))
    //    {
    //        guiderManagerLayer::getInstance()->createLayer(this);
    //    }
    if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_ADD_CIRCLE) && (guiderManagerLayer::getInstance()->getStepNum() == 3))
    {
        if(theFightType == eFighterType_circle)
        {
            if(m_bGuideBlow)//防止连续造兵
            {
                m_bGuideBlow = false;
                CFightData::getInstance()->setStopAddEnemy(false);
            }
            
            CDataManager::saveGuideInfoToDB(GUIDE_ADD_CIRCLE);
            remvGuideDraw();
            
            guiderManagerLayer::getInstance()->createLayer(this);
        }
        else{
            if(!guiderManagerLayer::getInstance()->getstart())
            {
                guiderManagerLayer::getInstance()->setstart(true);
            }
            bGuide = true;
            //return;
        }
    }
    if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_ADD_RECTANGLE) && (guiderManagerLayer::getInstance()->getStepNum() == 2))
    {
        if(theFightType == eFighterType_square)
        {
            if(m_bGuideBlow)
            {
                m_bGuideBlow = false;
                CFightData::getInstance()->setStopAddEnemy(false);
            }
            
            CDataManager::saveGuideInfoToDB(GUIDE_ADD_RECTANGLE);
            remvGuideDraw();
            
            guiderManagerLayer::getInstance()->createLayer(this);
        }
        else{
            if(!guiderManagerLayer::getInstance()->getstart())
            {
                guiderManagerLayer::getInstance()->setstart(true);
            }
            bGuide = true;
            //return;
        }
    }
    if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_ADD_V) && (guiderManagerLayer::getInstance()->getStepNum() == 2))
    {
        if(theFightType == eFighterType_v)
        {
            if(m_bGuideBlow)
            {
                m_bGuideBlow = false;
                CFightData::getInstance()->setStopAddEnemy(false);
            }
            
            CDataManager::saveGuideInfoToDB(GUIDE_ADD_V);
            remvGuideDraw();
            
            guiderManagerLayer::getInstance()->createLayer(this);
        }
        else{
            if(!guiderManagerLayer::getInstance()->getstart())
            {
                guiderManagerLayer::getInstance()->setstart(true);
            }
            bGuide = true;
            //return;
        }
    }
    if((CFightData::getInstance()->getNowLevelId() == 1001) && (guiderManagerLayer::getInstance()->getGuideType() == GUIDE_TOWER_LEVEL) && ((guiderManagerLayer::getInstance()->getStepNum() == 3)
                                                                                    || (guiderManagerLayer::getInstance()->getStepNum() == 5)) && !m_bUpdateTime)
    {
        if(theFightType == eFighterType_Tower)
        {
            if(m_bGuideBlow)
            {
                m_bGuideBlow = false;
                CFightData::getInstance()->setStopAddEnemy(false);
            }
            Vec2 pos;
            if(guiderManagerLayer::getInstance()->getStepNum() == 5)
            {
                pos = m_pClockButton->convertToWorldSpaceAR(Vec2::ZERO);
            }
            else{
                //显示能量的位置
                pos = m_pStoneLable->convertToWorldSpaceAR(Vec2::ZERO);
            }
            guiderManagerLayer::getInstance()->createLayer(this, GUIDE_SHOW_BOTH, pos);
        }
        else{
            if(!guiderManagerLayer::getInstance()->getstart())
            {
                guiderManagerLayer::getInstance()->setstart(true);
            }
            bGuide = true;
        }
        
    }
    if (ifWrong || bGuide) {//如果不能成功召唤士兵的话
        p_2dPath.clear();
        m_pointArr.clear();
        m_opcity = 250;
        return;
    }
    
    
    //此处扣除所需消耗的能量，最多扣为0
    if(monsterInfo)
    {
        CCLOG("士兵数量: %d", CFightData::getInstance()->getNowMonsterNum());
        CFightData::getInstance()->addEnergyWith(-monsterInfo->energyCost,eFightGroup_Freind);
        
        if(theFightType!=eFighterType_Tower)
        {
            CFightData::getInstance()->setNowMonsterNum(CFightData::getInstance()->getNowMonsterNum() + 1);
            
            //CFightData::getInstance()->setNowMonsterNum(10);
            if((CFightData::getInstance()->getNowMonsterNum() == CFightData::getInstance()->getMaxMonsterNum())
               && !(CDataManager::getEndedBy(GUIDE_PERFECT_INTRO)) && (CFightData::getInstance()->getNowLevelId() == 3))
            {
                Director::getInstance()->pause();
                guiderManagerLayer::getInstance()->setGuideType(GUIDE_PERFECT_INTRO);
                guiderManagerLayer::getInstance()->setStepNum(1);
                guiderManagerLayer::getInstance()->createLayer(this, GUIDE_SHOW_WND, Vec2::ZERO);
            }
        }
    }
    //遍历寻找相距最远的两个点,并求出中点
    int start = 0;
    int end = 0;
    float distance = 0;
    for (int i = 0; i < p_2dPath.size() - 1; i++ ){
        for(int j = i + 1; j < p_2dPath.size(); j++)
        {
            if( (p_2dPath.at(i).distance(p_2dPath.at(j))) > distance){
                distance = p_2dPath.at(i).distance(p_2dPath.at(j));
                start = i;
                end = j;
            }
        }
    }
    
    cocos2d::Point midPos = Vec2((p_2dPath.at(start).x + p_2dPath.at(end).x) / 2, (p_2dPath.at(start).y + p_2dPath.at(end).y) / 2);
    midPos = Director::getInstance()->convertToGL(midPos);
    //m_bIsshowResult = false;
    
    //显示结果
    char buf[32];
    //调用对应函数
    CCLOG("图形为:%s, 相似度为:%f", m_GraphicsName.c_str(), m_similarity);
    //显示识别的图形和相似度
    sprintf(buf, "%s.png", m_GraphicsName.c_str());
    
    Sprite* grap = NULL;
    
    if (m_GraphicsName == "CheckMark") {   //如果是checkmark，则用其他图形代替初始化，因为始终不显示
        grap = Sprite::createWithSpriteFrameName("V.png");
    }else{
        grap = Sprite::createWithSpriteFrameName(buf);
    }
    
    grap->setPosition(midPos);
    if (m_GraphicsName == "Triangle") {
        grap->setPosition(Vec2(grap->getPositionX(), grap->getPositionY() + grap->getContentSize().height * 0.55));
    }
    else if(m_GraphicsName == "V"){
        grap->setPosition(Vec2(grap->getPositionX() - grap->getContentSize().width / 4, grap->getPositionY()));
    }else if(m_GraphicsName == "CheckMark"){
        grap->setVisible(false);
    }
    
    addChild(grap,1,80);
    //取x轴，y轴相距最远的一个值
    
    //暂时注释用
    float diameter = p_2dPath.at(start).distance(p_2dPath.at(end));
    grap->setScale(diameter / grap->getContentSize().width *0.9f);
    if(m_GraphicsName == "LineDownDiagonal")
    {
        if(m_similarity > 0.95)
        {
            m_similarity = 1.0;
            //记录完美绘图次数
            CFightData::getInstance()->setFightData(2);
        }
    }
    else if(m_similarity > 0.9)
    {
        m_similarity = 1.0;
        //记录完美绘图次数
        CFightData::getInstance()->setFightData(2);
    }
    else
    {
        m_similarity /= 0.9f;
    }
    
    //相似度图片
    Sprite* pSimilar = Sprite::createWithSpriteFrameName("similarity.png");
    addChild(pSimilar,1,85);
    pSimilar->setAnchorPoint(Vec2(0, 0.5f));
    pSimilar->setPosition(Vec2(grap->getPositionX() - grap->getContentSize().width / 2, grap->getPositionY() + grap->getContentSize().height / 2 + 30));
    
    sprintf(buf, "%d", int(m_similarity*100));
    Label* pSimilarLabel = Label::createWithCharMap("fonts/similarity_figure.png", 42.4,48,'0');
    addChild(pSimilarLabel,1,90);
    pSimilarLabel->setAnchorPoint(Vec2(0, 0.5f));
    pSimilarLabel->setPosition(Vec2(pSimilar->getPositionX() + pSimilar->getContentSize().width + 10, pSimilar->getPositionY()));
    pSimilarLabel->setString(buf);
    
    //百分号
    Sprite* percent = Sprite::createWithSpriteFrameName("percent.png");
    addChild(percent,1,95);
    percent->setAnchorPoint(Vec2(0, 0.5f));
    percent->setPosition(Vec2(pSimilarLabel->getPositionX() + pSimilarLabel->getContentSize().width + 10, pSimilar->getPositionY()));
    
    
    
    ActionInterval*  pActionOut = FadeOut::create(0.3f);
    pSimilar->runAction(Sequence::create(DelayTime::create(0.5), pActionOut, RemoveSelf::create(),NULL));
    pSimilarLabel->runAction(Sequence::create(DelayTime::create(0.5), pActionOut->clone(), RemoveSelf::create(),NULL));
    percent->runAction(Sequence::create(DelayTime::create(0.5), pActionOut->clone(), RemoveSelf::create(),NULL));
    
    
    CallFunc* effect = CallFunc::create(CC_CALLBACK_0(CFightScene::addSoliderEffect, this));
    
    //如果完美绘图，给个表示
    if(m_similarity==1){
        
        
        CallFunc* rmv = CallFunc::create(CC_CALLBACK_0(CFightScene::removeResultPerfect, this));
        
        float moveY = 80;
        float moveTime = 0.2f;
        
        Sprite* perfect = Sprite::createWithSpriteFrameName("draw_perfect.png");
        perfect->getTexture()->setAntiAliasTexParameters();
        //        perfect->setScale(2);
        
        perfect->setPosition(pSimilar->getBoundingBox().size.width*0.5f,perfect->boundingBox().size.height*0.5f+pSimilar->boundingBox().size.height*0.5f+moveY);
        
        pSimilar->addChild(perfect,1);
        
        perfect->setOpacity(0);
        
        perfect->setScale(6);
        
        Action* tempSpawn = Spawn::create(FadeIn::create(moveTime),MoveBy::create(moveTime, Vec2(0, -moveY)),ScaleTo::create(moveTime, 3), NULL);
        
        perfect->runAction(Sequence::create(DelayTime::create(0.1f),tempSpawn,CallFunc::create([=](){
            ParticleSystemQuad *particle = ParticleSystemQuad::create("particleSystem/star2.plist");
            particle->setAutoRemoveOnFinish(true);
            
            Vec2 targetPos = perfect->convertToWorldSpace(Vec2(perfect->getBoundingBox().size.width*0.5f, perfect->getBoundingBox().size.height*0.5f));
            particle->setPosition(targetPos);
            addChild(particle);
        }), NULL));
        
        
        pSimilarLabel->setOpacity(0);
        pSimilar->setOpacity(0);
        percent->setOpacity(0);
        
        grap->runAction(Sequence::create(effect,rmv,DelayTime::create(0.65), pActionOut, RemoveSelf::create(), NULL));
        
        
        
        //        pSimilarLabel->setScale(0.4f);
        //        pSimilar->setScale(0.4f);
        //        percent->setScale(0.4f);
        //        pSimilar->setPosition(Vec2(grap->getPositionX() - grap->getBoundingBox().size.width / 2, grap->getPositionY() + grap->getBoundingBox().size.height / 2 + 30));
        //        pSimilarLabel->setPosition(Vec2(pSimilar->getPositionX() + pSimilar->getBoundingBox().size.width + 10, pSimilar->getPositionY()));
        //        percent->setPosition(Vec2(pSimilarLabel->getPositionX() + pSimilarLabel->getBoundingBox().size.width + 10, pSimilar->getPositionY()));
    }else{
        
        CallFunc* rmv = CallFunc::create(CC_CALLBACK_0(CFightScene::removeResultNormal, this));
        
        float moveY = 80;
        float moveTime = 0.2f;
        ActionInterval* tempSpawn = Spawn::create(FadeIn::create(moveTime),MoveBy::create(moveTime, Vec2(0, -moveY)),ScaleTo::create(moveTime, diameter / grap->getContentSize().width*0.4f),rmv , NULL);
        
        grap->setPositionY(grap->getPositionY()+moveY);
        grap->cocos2d::Node::setScale(diameter / grap->getContentSize().width);
        //        ActionInterval* scale1 = ScaleTo::create(0.15f, diameter / grap->getContentSize().width + 0.35);
        //        ActionInterval* scale2 = ScaleTo::create(0.2f, 0.65);
        
        
        if (CFightData::getInstance()->getTheLevelCondition()==eLevelCondition_TowerDefense) {
            grap->runAction(Sequence::create( effect,rmv, DelayTime::create(0.3), pActionOut, RemoveSelf::create(), NULL));
        }else{
            grap->runAction(Sequence::create(tempSpawn, effect, DelayTime::create(0.3), pActionOut, RemoveSelf::create(), NULL));
            
        }
        
        
    }
    
    
    
    p_2dPath.clear();
    m_pointArr.clear();
    m_opcity = 250;
}

void CFightScene::addSoliderEffect()
{
    SimpleAudioEngine::getInstance()->playEffect("music/zaobing.mp3");
}

void CFightScene::onTouchCancelled(Touch *touch, Event *event)
{
    
}

void CFightScene::clearPoint()
{
    p_2dPath.clear();
    m_pointArr.clear();
}

void CFightScene::addParticleEffect(Ref* pObject)
{
    Sprite* sp1 = (Sprite*)pObject;
    
    ParticleSystemQuad* effect= ParticleSystemQuad::create("plist/Particle.plist");
    layer3D->addChild(effect);
    effect->setPosition(sp1->getPosition());
    
    yHeight = -rand()%(STARTZPOINT-ENDZPOINT) +STARTZPOINT;
    //    if(sp1->getPosition().y > ADD_SOLIDER_HEIGHT){
    //        effect->runAction(Sequence::create(MoveBy::create(0.5f, Vec2(0, 300)), MoveBy::create(0.5f, Vec2(-300, 0)), MoveBy::create(0.5f, Vec2(0, -300)), RemoveSelf::create(),NULL));
    //    }
    //    else if(sp1->getPosition().y == ADD_SOLIDER_HEIGHT){
    //
    //    }
    //    else{
    //
    //    }
    effect->runAction(Sequence::create(MoveBy::create(0.5f, Vec2(300-sp1->getPositionX(), 0)), MoveBy::create(0.5f, Vec2(0, yHeight - sp1->getPositionY())), RemoveSelf::create(),NULL));//Start3DPOINT
}

void CFightScene::removeResult(bool ifPerfect)
{
    //m_isDraw = true;
    FighterType theFightType;
    
    if (m_GraphicsName == "LineDownDiagonal") {
        theFightType = eFighterType_line;
    }
    else if(m_GraphicsName == "Circle"){
        theFightType = eFighterType_circle;
    }
    else if(m_GraphicsName == "Triangle"){
        theFightType = eFighterType_triangle;
    }
    else if(m_GraphicsName == "Rectangle"){
        theFightType = eFighterType_square;
    }
    else if(m_GraphicsName == "V"){
        theFightType = eFighterType_v;
    }else if (m_GraphicsName=="CheckMark"){
        theFightType = eFighterType_Tower;
    }
    
    
    if (CFightData::getInstance()->getTheLevelCondition()==eLevelCondition_TowerDefense) {
        
        //如果是塔防，则根据图形造塔
        if (theFightType==eFighterType_Tower) {
            m_bIfBuildingTower=true;
            
            CBaseFighter* temp = addbuildingReal(m_vec2BuildPos);
            
            if (temp) {
                
                
                fightBuildingInfo* tempInfo = CDataManager::getTheFightBuildingInfoWithId(temp->getTheInfo()->monsterId);
                
                //测试用注释
                CFightData::getInstance()->costStone(tempInfo->buildCost);
            }
            
            
        }
        
        //        addbuildingVirtual(m_vec2BuildPos);
        
    }else{
        
        float posY = -rand()%(STARTZPOINT-ENDZPOINT) +STARTZPOINT;
        addSoldierByType(theFightType, posY,ifPerfect);
        
        
        
    }
    
}

void CFightScene::removeResultPerfect(){
    
    removeResult(true);
    
}

void CFightScene::removeResultNormal(){
    
    removeResult(false);
}



void CFightScene::addSoldierByType(FighterType theFightType, float yPos,bool ifPerfect){
    
    
    FighterInfo* monsterInfo = CPersonalData::getInstance()->getTeamSolderByType(theFightType);
    
    
    
    //    测试用，召唤弓箭手
    //    monsterInfo = CPersonalData::getInstance()->getTeamSolderByType(eFighterType_circle);
    
    
    //如果没取到士兵，则提示错误
    if (!monsterInfo) {
        Sprite * thetarget = CFightManager::getTheFlyWord("没有对应士兵","fonts/cuti.ttf");
        thetarget->setPosition(Vec2(m_WinSize.width*0.5f,m_WinSize.height*0.7f));
        addChild(thetarget);
        return;
    }
    
    
    //    //此处扣除所需消耗的能量，最多扣为0
    //    CFightData::getInstance()->addEnergyWith(-monsterInfo->energyCost,eFightGroup_Freind);
    
    //float randZ = -rand()%(STARTZPOINT-ENDZPOINT) +STARTZPOINT;
    //addMonsterWithIdAndGroupAndPosition(monsterInfo, eFightGroup_Freind, Vec2(Start3DPOINT-ONESIDE*0.5f, yPos));
    m_soliderNum++;
    
    CBaseFighter* target = addMonsterWithIdAndGroupAndPosition(monsterInfo, eFightGroup_Freind, Vec2(Start3DPOINT-ONESIDE*0.2f, yPos),eAttackType_monster,false,false,ifPerfect);
    
    
    
    
    //    if (ifPerfect) { //如果是完美,则设置士兵为精英
    //
    //        target->setElite(true);
    //
    //
    //    }
    
    
    //        addLevelMonster(1,cocos2d::Point( Start3DPOINT+800, -200));
    //    addLevelMonster(cocos2d::Point( Start3DPOINT+100, randZ));
    //    addLevelMonster(cocos2d::Point( Start3DPOINT+100, randZ));
    
}

void CFightScene::addLevelMonster(int thenumbers,cocos2d::Point thePos,EnemyType thetype){
    
    //做个保护，如果怪物数量大于60个，则不继续造兵了
    if (CFightData::getInstance()->getAllEnemy()->size()>60) {
        return;
    }
    
    
    
    //测试用,看到就删
    //    return;
    
    std::vector<FighterInfo>* normalInfo=NULL;
    
    FighterInfo* targetInfo=NULL;
    
    
    if(thetype==eEnemyType_normal){//召唤普通怪物
        
        normalInfo = CFightData::getInstance()->getTheMonsterInfo();
        
    }else if(thetype==eEnemyType_boss) { //召唤boss
        
        targetInfo = CFightData::getInstance()->getTheBossInfo();
        
    }else{
        
        CCASSERT(0, "lyAssert:: add EnemyType wrong");
    }
    
    //    std::vector<FighterInfo> temp;
    
    
    
    
    for (int i=0; i<thenumbers; i++) {
        
        int randZ = random(ENDZPOINT,STARTZPOINT);
        
        
        if (normalInfo&&normalInfo->size()>0) {
            
            int randMonsterId = rand()%normalInfo->size();
            
            std::vector<FighterInfo>::iterator ite = normalInfo->begin();
            ite= ite+randMonsterId;
            
            targetInfo = &(*ite);
        }
        
        
        if (targetInfo->monsterId!=0) {  //只有当怪物id不为0时才初始化
            CBaseFighter* tempFight=NULL;
            if (thePos!=cocos2d::Point::ZERO) {
                tempFight=addMonsterWithIdAndGroupAndPosition(targetInfo, eFightGroup_Enemy, thePos);
            }else{
                tempFight=addMonsterWithIdAndGroupAndPosition(targetInfo, eFightGroup_Enemy, Vec2(END3DPOINT, randZ));
            }
            
            //如果是小boss，则会变红，属性提高2倍
            if(thetype==eEnemyType_boss) {
                float attack = tempFight->getNowAttack()*1.5f;
                float hp = tempFight->getMaxHP()*1.5f;
                
                //            tempFight->gettheSprite()->setColor(Color3B(255, 103, 103));
                tempFight->changeSpriteScale(1.7);
                //            tempFight->gettheSprite()->setScale(tempFight->gettheSprite()->getScale()*1.4f);
                tempFight->setNowAttack(attack);
                tempFight->setNowHP(hp);
                tempFight->setMaxHP(hp);
                
                tempFight->setIfSmallBoss(true);
                
//                tempFight->changeShader(eShadeType_3D_Boss);  //暂时弃用，切屏幕会引起bug  by ly
                
//                tempFight->bossShader= (CSprite3dAnimateShaderWithFile*)CFightManager::addShaderToSprite3D(tempFight->gettheSprite(), eShadeType_3D_Boss,"shaderFade");
                
            }
            
            
            //此处扣除所需消耗的能量，最多扣为0
            CFightData::getInstance()->addEnergyWith(-targetInfo->energyCost,eFightGroup_Enemy);
        }
        
        
    }
    
}



CBaseFighter* CFightScene::addMonsterWithIdAndGroupAndPosition(FighterInfo* theInfo, FightGroup theGroup, cocos2d::Vec2 position,AttackType theType,bool ifMidCamp,bool ifboss,bool ifElite,bool bZhaohuan,bool bUpdating)
{
    
    Vec2 thePos = position;
    
    
    Vec3 curtouch = Vec3(thePos.x,0,thePos.y);
    
    
    CBaseFighter* theMonster=NULL;
    
    if(theType == eAttackType_monster){
        theMonster = CBaseFighter::create(theInfo,theGroup,ifboss,ifElite);
        
        if(bZhaohuan)
        {
            theMonster->setIsZhaohuan(1);
        }
        //小地图的标示
        // theMonster->m_pMapSprite =  CFightData::getInstance()->m_pSmallMapLayer->SetSmallMapPostion(curtouch,theInfo->monsterType, theGroup,eAttackType_monster,ifboss);
        
        
        
        
        
    }else if (theType ==eAttackType_min){
        
        
        theMonster = CBaseResourceMine::create(theInfo,theGroup);
        
        
        
        CFightData::getInstance()->m_pSmallMapLayer->SetSmallMapPostion(curtouch,theMonster->m_pMapSprite); //建筑的话要手动帮他地图精灵置位置。
        //        theMonster->m_pMapSprite =  CFightData::getInstance()->m_pSmallMapLayer->SetSmallMapPostion(curtouch,theInfo->monsterType, theGroup);
        
        
    }else if (theType ==eAttackType_minMonster){
        
        
        theMonster = CBaseMonsterMine::create(theInfo,theGroup);
        
        
    }else if (theType ==eAttackType_lifeWater){
        
        
        theMonster = CBaseLifeMine::create(theInfo,theGroup);
        CFightData::getInstance()->m_pSmallMapLayer->SetSmallMapPostion(curtouch,theMonster->m_pMapSprite);
        //        theMonster->m_pMapSprite =  CFightData::getInstance()->m_pSmallMapLayer->SetSmallMapPostion(curtouch,theInfo->monsterType, theGroup);
        
        
    }else if (theType ==eAttackType_tower){
        
        
        theMonster = CBaseTower::create(theInfo,theGroup);
        CFightData::getInstance()->m_pSmallMapLayer->SetSmallMapPostion(curtouch,theMonster->m_pMapSprite);
        //        theMonster->m_pMapSprite =  CFightData::getInstance()->m_pSmallMapLayer->SetSmallMapPostion(curtouch,theInfo->monsterType, theGroup);
        
        theMonster->setAttackType(theType);
        
        //锤子
        
        if(bUpdating)
        {
            ((CBaseTower*)theMonster)->setUpdating(true);
            
            //未升级的正在建造
            cocos2d::Size bgSize =  theMonster->gettheSprite()->getBoundingBox().size;
            Sprite * m_hammer = Sprite::createWithSpriteFrameName("pvp_hammer_1.png");
            theMonster->addChild(m_hammer, 100);
            m_hammer->setPosition3D(Vec3(bgSize.width * 0.55f, bgSize.height * 0.7f, 0));
            m_hammer->setCameraMask((unsigned short )CameraFlag::USER1);
            Animation* pAction = AnimationCache::getInstance()->getAnimation("hammer");
            
            pAction->setDelayPerUnit(0.1f);
            
            m_hammer->runAction(RepeatForever::create(Sequence::create(Animate::create(pAction),DelayTime::create(0.7),NULL)));
            
            
            
            Sprite* temp=Sprite::createWithSpriteFrameName("tower_die_1.png");
            CFightManager::addCamera(temp);
            temp->setGlobalZOrder(1000);
            
            temp->setPosition3D(getPosition3D());
            theMonster->addChild(temp,111111);
            temp->setScale(1);
            char buf[256];
            Animation* pAnimation = Animation::create();
            for (int i = 1; i < 11; i++)
            {
                sprintf(buf, "tower_die_%d.png", i);
                pAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(buf));
            }
            pAnimation->setDelayPerUnit(0.1f);
            temp->runAction(RepeatForever::create(Sequence::create(DelayTime::create(0.5),Animate::create(pAnimation),NULL)));
            
            
            
            
        }
        
        
    }else if (theType == eAttackType_protector){
        
        
        theMonster = CBaseProtector::create(theInfo,theGroup);
        
    }else if (theType == eAttackType_wall){
        
        
        theMonster = CBaseWall::create(theInfo,theGroup);
        CFightData::getInstance()->m_pSmallMapLayer->SetSmallMapPostion(curtouch,theMonster->m_pMapSprite);
        
        
    }else if (theType == eAttackType_Trap){
        
        
        theMonster = CBaseTrap::create(theInfo,theGroup);
        
        if(bUpdating)
        {
            ((CBaseTrap*)theMonster)->setUpdating(true);
            
            //未升级的正在建造
            cocos2d::Size bgSize =  theMonster->gettheSprite()->getBoundingBox().size;
            Sprite * m_hammer = Sprite::createWithSpriteFrameName("pvp_hammer_1.png");
            theMonster->addChild(m_hammer, 100);
            m_hammer->setPosition3D(Vec3(bgSize.width * 0.55f, bgSize.height * 0.7f, 0));
            m_hammer->setCameraMask((unsigned short )CameraFlag::USER1);
            Animation* pAction = AnimationCache::getInstance()->getAnimation("hammer");
            
            pAction->setDelayPerUnit(0.1f);
            
            m_hammer->runAction(RepeatForever::create(Sequence::create(Animate::create(pAction),DelayTime::create(0.7),NULL)));
            
            
            
            Sprite* temp=Sprite::createWithSpriteFrameName("tower_die_1.png");
            CFightManager::addCamera(temp);
            temp->setGlobalZOrder(1000);
            
            temp->setPosition3D(getPosition3D());
            theMonster->addChild(temp,111111);
            temp->setScale(1);
            char buf[256];
            Animation* pAnimation = Animation::create();
            for (int i = 1; i < 11; i++)
            {
                sprintf(buf, "tower_die_%d.png", i);
                pAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(buf));
            }
            pAnimation->setDelayPerUnit(0.1f);
            temp->runAction(RepeatForever::create(Sequence::create(DelayTime::create(0.5),Animate::create(pAnimation),NULL)));
            
        
        }

        
        
        
    }else if (theType == eAttackType_city){
        
        
        
        theMonster = CBaseCity::create(theInfo,theGroup);
        CFightData::getInstance()->m_pSmallMapLayer->SetSmallMapPostion(curtouch,theMonster->m_pMapSprite);
        
        
    }else if (theType == eAttackType_walker){
        
        theMonster = CBaseWalker::create(theInfo,theGroup);
        
        
        
        //护送者的血量*1.5倍
        float hp = theMonster->getMaxHP();
        hp = hp*1.5f;
        theMonster->setMaxHP(hp);
        theMonster->setNowHP(hp);
        
        
        //如果有护送者，则初始化终点线
        
        int allNum = 5;
        float changeTime = 0.15f;
        
        Sprite* temp = Sprite::createWithSpriteFrameName("fight_TargetLine.png");
        int distOffset = temp->getBoundingBox().size.height*3;   //防止终点线离太近
        
        Color3B defaultColor = Color3B(255,255,255);
        
        for (int i=0; i<allNum; i++) {
            
            Vec3 targetPos;
            
            targetPos.x = ((CBaseWalker*)theMonster)->getTargetX();
            targetPos.y = 0;
            targetPos.z = (STARTZPOINT - ENDZPOINT-distOffset*2)*i/(allNum-1) +ENDZPOINT+distOffset*1.3f;
            
            
            Vec2 mapPos = CFightData::getInstance()->m_pSmallMapLayer->getMapPosBy3dPos(targetPos);
            
            Sprite* temp = Sprite::createWithSpriteFrameName("fight_TargetLine.png");
            
            temp->setPosition(mapPos);
            CFightData::getInstance()->m_pSmallMapLayer->addChild(temp,10);
            
            
            temp->setColor(defaultColor);
            
            temp->runAction(Sequence::create(DelayTime::create(changeTime*i),CallFunc::create([=](){
                
                auto* repeatAction = RepeatForever::create(Sequence::create(TintTo::create(0, 100, 100, 100),DelayTime::create(changeTime),TintTo::create(0,defaultColor),DelayTime::create((allNum-1)*changeTime), NULL));
                temp->runAction(repeatAction);
            }), NULL));
            //            temp->runAction(Sequence::create(DelayTime::create(changeTime*i),repeatAction, NULL));
            
        }
        
        
        
        
        
    }else if (eAttackType_hero==theType){
        
        
        //        test.walkSpeed =3;
        
        theMonster = CBaseHero::create(theInfo,theGroup);
        
        CFightData::getInstance()->setTheFriendHero(theMonster);
        
        //        theMonster->m_pMapSprite =  CFightData::getInstance()->m_pSmallMapLayer->SetSmallMapPostion(curtouch,eFighterType_triangle);
        
    }
    else if(theType == eAttackType_Partol)
    {
        theMonster = CPartolFight::create(theInfo, theGroup);
        //theMonster->gettheSprite()->setScale(0.4f);
    }
    
    //
    //    std::string test1= "zhanshi_wuqi01";
    //    //    _girlUpperBody[0] = "Girl_UpperBody01";
    //    std::string test2 = "zhanshi_tou01";
    //    //    _girlShoes[0]  = "Girl_Shoes01";
    //    std::string test3  = "zhanshi_shenti01";
    //    //    _girlHair[0]= "Girl_Hair01";
    //    std::string test4= "Girl_Hair02";
    //
    //    auto girlPants = theMonster->gettheSprite()->getMeshByName(test1);
    //    if(girlPants)
    //    {
    //        girlPants->setVisible(false);
    //    }
    //    auto girlShoes = theMonster->gettheSprite()->getMeshByName(test3);
    //    if(girlShoes)
    //    {
    //        girlShoes->setVisible(false);
    //    }
    //    auto girlHair = theMonster->gettheSprite()->getMeshByName(test4);
    //    if(girlHair)
    //    {
    //        girlHair->setVisible(false);
    //    }
    //    auto girlUpBody = theMonster->gettheSprite()->getMeshByName(test2);
    //    if(girlUpBody)
    //    {
    //        girlUpBody->setVisible(false);
    //    }
    //
    //
    //
    //
    
    if (theMonster) {
        
        
        theMonster->setIfCampMonster(ifMidCamp);
        
        //        if((theMonster->getAttackType() == eAttackType_monster) && (theMonster->getFightGroup() == eFightGroup_Freind))
        //        {
        //            if(CFightData::getInstance()->getNowMonsterNum() >= CFightData::getInstance()->getMaxMonsterNum())
        //            {
        //                Label* tipLabel = Label::createWithSystemFont("已经达到士兵数量上限", "Arial", 30);
        //                tipLabel->setPosition(Vec2(m_WinSize.width / 2, m_WinSize.height / 2));
        //                tipLabel->runAction(Sequence::create(MoveBy::create(0.5f, Vec2(0, 100)),DelayTime::create(0.5f),RemoveSelf::create(),NULL));
        //                addChild(tipLabel);
        //                return NULL;
        //            }
        //        }
        if(theGroup == eFightGroup_Freind)//记录召唤弓箭手的次数
        {
            CFightData::getInstance()->setFightData(3, theInfo->monsterId);
            CFightData::getInstance()->setFightData(4, theInfo->monsterId);
        }
        
        CFightData::getInstance()->addMonster(theMonster,theType);
        
        layer3D->addChild(theMonster);
        
        

        theMonster->setPosition3D(Vec3(thePos.x, attackerY,thePos.y));
        
        theMonster->setCameraMask((unsigned short )CameraFlag::USER1);
        
        theMonster->setVisible(false);
        theMonster->unscheduleUpdate();
        
        
        //如果是塔防模式下召唤建筑，则添加obb
        if (CFightData::getInstance()->getTheLevelCondition()==eLevelCondition_TowerDefense&&theType==eAttackType_tower) {
            CFightData::getInstance()->changeTowerObb(true, theMonster);
        }
        
        //        //陷阱就先暂时没有出生动画了
        //        if (theType!=eAttackType_Trap) {
        //            addBornAnimate(theMonster);
        //        }else{
        //            theMonster->scheduleUpdate();
        //        }
        
        //如果是pvp模式，并且是建筑或者陷阱，则没有出生动画
        
        if ((CFightData::getInstance()->getFightType()==FIGHT_PVP_TYPE&&theType==eAttackType_tower)||theType==eAttackType_Trap
            || theType==eAttackType_Partol) {
            theMonster->scheduleUpdate();
        }else{
            addBornAnimate(theMonster);
        }
        
        
        
    }
    
    return theMonster;
    
    //    theMonster->setScale(3);
    
}


void CFightScene::addBornAnimate(CBaseFighter *target){
    
    
    Sprite3D* temp = Sprite3D::create();
    
    Sprite* tempSprite = Sprite::createWithSpriteFrameName("fight_bornCircle.png");
    
    Sprite* tempSprite2 = Sprite::createWithSpriteFrameName("fight_bornCircle2.png");
    
    tempSprite->addChild(tempSprite2);
    
    if(target->getIsZhaohuan())
    {
        tempSprite->setVisible(false);
    }
    
    tempSprite2->setPosition(Vec2(tempSprite->getBoundingBox().size.width*0.5f, tempSprite->getBoundingBox().size.height*0.5f));
    
    
    tempSprite->runAction(RepeatForever::create(RotateBy::create(0.5f, 50)));
    
    tempSprite2->runAction(RepeatForever::create(RotateBy::create(0.5f, -100)));
    
    temp->addChild(tempSprite);
    
    layer3D->addChild(temp);
    
    temp->setPosition3D(target->getPosition3D());
    
    CFightManager::addCamera(temp);
    
    
    target->setTheActiveCircle(temp);
    
    if (target->getTheInfo()->monsterType==eFighterType_Tower) { //塔的出生动画
        
        
        
        float startScale = 0.1f;
        
        float targetScale = target->gettheSprite()->getBoundingBox().size.width/tempSprite->getBoundingBox().size.width*2.f;
        
        temp->setScale(startScale);
        
        temp->setRotation3D(Vec3(-90, 0, 0));
        
        target->gettheSprite()->setPosition3D( target->gettheSprite()->getPosition3D()+Vec3(0, -target->gettheSprite()->getBoundingBox().size.height*1.2f, 0));
        temp->setPositionY(5);
        
        target->setVisible(true);
        target->getTheShadow()->setVisible(false);
        
        
        
        
        target->scheduleUpdate();
        
        temp->runAction(Sequence::create(ScaleTo::create(0.2f, targetScale*1.4f),ScaleTo::create(0.1f, targetScale),DelayTime::create(0.15f),CallFunc::create([=](){
            
            temp->setUserData((void*)(1));
            
        }),DelayTime::create(0.4f),ScaleTo::create(0.2f, targetScale*1.4f),ScaleTo::create(0.1f, 0),RemoveSelf::create(), NULL));
        
        
        
        
        
    }else{ //怪物的出生动画
        
        
        float startScale = 0.1f;
        float targetScale = target->gettheSprite()->getBoundingBox().size.height/tempSprite->getBoundingBox().size.height*1.2f;
        
        tempSprite->setScale(targetScale);
        
        temp->setRotation3D(Vec3(0, 90, 0));
        
        temp->setPositionY(tempSprite->getBoundingBox().size.height*0.5f);
        
        tempSprite->setScale(startScale);
        
        target->scheduleUpdate();
        
        tempSprite->runAction(Sequence::create(ScaleTo::create(0.2f, targetScale*1.4f),ScaleTo::create(0.1f, targetScale),DelayTime::create(0.4f),CallFunc::create([=](){
            
            temp->setUserData((void*)(1));
            
        }),DelayTime::create(0.4f),ScaleTo::create(0.2f, targetScale*1.4f),ScaleTo::create(0.1f, 0),RemoveSelf::create(), NULL));
        
        
        
    }
    
    
    
    
}



void CFightScene::animateInit()
{
    Animation * hurtAnimation = Animation::create();
    
    for (int i=1; i<5; i++)
    {
        
        __String *temp = __String::createWithFormat("hit%d.png",i);
        hurtAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(temp->getCString()));
    }
    hurtAnimation->setDelayPerUnit(0.1f);
    AnimationCache::getInstance()->addAnimation(hurtAnimation, "hurtAnimation");
    
}

void CFightScene::updateLevelStar(bool isEnd)
{
    
    //pvp模式则不更新星级
    if (CFightData::getInstance()->getFightType()==FIGHT_PVP_TYPE) {
        return;
    }
    
    
    //更新当前关卡星级
    int levelId = CFightData::getInstance()->getNowLevelId();
    std::vector<int> starIdVec = CDataManager::getStarConditionById(levelId);
    std::vector<levelStarInfo> info = CFightData::getInstance()->m_starInfoVec;
    for(int k =0 ; k < starIdVec.size(); k++)
    {
        bool isExisted = false;
        starInfo* pInfo = CDataManager::getStarInfoById(starIdVec.at(k));
        for (int l = 0; l < info.size(); l++) {
            if((info.at(l).starType == pInfo->starType) && (info.at(l).monsterId == pInfo->monsterId))
            {
                isExisted = true;
                break;
            }
        }
        if(!isExisted)
        {
            levelStarInfo tempInfo;
            tempInfo.starType = pInfo->starType;
            tempInfo.monsterId = pInfo->monsterId;
            tempInfo.num = 0;
            info.push_back(tempInfo);
        }
    }
    float starNum[3] = {0,0,0};//记录三个星级条件的完成情况
    for (int i = 0; i < starIdVec.size(); i++) {
        starInfo* pInfo =  CDataManager::getStarInfoById(starIdVec.at(i));
        if(pInfo)
        {
            for(int j = 0; j < info.size(); j++)
            {
                if((pInfo->starType == info.at(j).starType) && (pInfo->monsterId == info.at(j).monsterId))
                {
                    starNum[i] = info.at(j).num;
                }
            }
        }
    }
    for(int i = 0; i < 3; i++)
    {
        starInfo* pInfo =  CDataManager::getStarInfoById(starIdVec.at(i));
        Sprite* sp = (Sprite*)(m_layer->getChildByTag(FIGHT_STAR_ONE + i));
        if(sp)
        {
            if(pInfo->positive == 1)
            {
                Sprite* star = NULL;
                if(starNum[i] >= pInfo->num)
                {
                    star = Sprite::createWithSpriteFrameName("level_star.png");
                }
                else if(isEnd){
                    star = Sprite::createWithSpriteFrameName("star_fail1.png");
                }
                if(star)
                {
                    m_layer->addChild(star,10);
                    star->setPosition(sp->getPosition());
                    star->setScale(2.5);
                    star->runAction(Hide::create());
                    CallFunc* act = CallFunc::create(CC_CALLBACK_0(CFightScene::addStarEffect, this));
                    Spawn* starAct = Spawn::create(ScaleTo::create(0.4, 1.05), act, NULL);
                    star->runAction(Sequence::create(Show::create(), starAct, NULL));
                    starShow infp;
                    infp.tag = FIGHT_STAR_ONE + i;
                    infp.star = star;
                    for(std::vector<starShow>::iterator it = m_starVec.begin(); it != m_starVec.end(); it++)
                    {
                        if(it->tag == FIGHT_STAR_ONE + i)
                        {
                            m_starVec.erase(it);
                            break;
                        }
                    }
                    m_starVec.push_back(infp);
                    sp->removeFromParent();
                    
                    //                    暂时去掉星级引导
                    //                    if(!(CDataManager::getEndedBy(GUIDE_LEVEL_STAR)) && (guiderManagerLayer::getInstance()->getStepNum() == 0)
                    //                       && (CFightData::getInstance()->getNowLevelId() == 3) && (CFightData::getInstance()->getWinState()!=-1))
                    //                    {
                    //                        guiderManagerLayer::getInstance()->setStepNum(1);
                    //                        Vec2 pos = Vec2(m_WinSize.width*0.93,m_WinSize.height*0.84);
                    //                        guiderManagerLayer::getInstance()->setGuideType(GUIDE_LEVEL_STAR);
                    //                        guiderManagerLayer::getInstance()->createLayer(this, GUIDE_SHOW_BOTH, pos);
                    //                    }
                }
            }
            if(pInfo->positive == 0)
            {
                Sprite* star = NULL;
                if(starNum[i] > pInfo->num)
                {
                    star = Sprite::createWithSpriteFrameName("star_fail1.png");
                }
                else{
                    if(isEnd)
                    {
                        star = Sprite::createWithSpriteFrameName("level_star.png");
                    }
                }
                if(star)
                {
                    m_layer->addChild(star,10);
                    star->setPosition(sp->getPositionX(), sp->getPositionY());
                    star->setScale(2.5);
                    star->runAction(Hide::create());
                    CallFunc* act = CallFunc::create(CC_CALLBACK_0(CFightScene::addStarEffect, this));
                    Spawn* starAct = Spawn::create(ScaleTo::create(0.4, 1.05), act, NULL);
                    star->runAction(Sequence::create(Show::create(), starAct, NULL));
                    starShow infp;
                    infp.tag = FIGHT_STAR_ONE + i;
                    infp.star = star;
                    for(std::vector<starShow>::iterator it = m_starVec.begin(); it != m_starVec.end(); it++)
                    {
                        if(it->tag == FIGHT_STAR_ONE + i)
                        {
                            m_starVec.erase(it);
                            break;
                        }
                    }
                    m_starVec.push_back(infp);
                    sp->removeFromParent();
                }
            }
        }
    }
}


void CFightScene::ResetGlobalZ()
{
    Sprite *p1 =  (Sprite*)m_ptaskBg->getChildByTag(100);
    
    Sprite *p2 = (Sprite*)m_ptaskBg->getChildByTag(101);
    
    if(p1)
        p1->setGlobalZOrder(0);
    
    if(p2)
        p2->setGlobalZOrder(0);
    
    if(m_pTaskLable)
    {
        m_pTaskLable->setGlobalZOrder(0);
    }
    
}
void CFightScene::showFightResult(Ref* pSender)
{
    ResetGlobalZ();
    
  
    NotificationCenter::getInstance()->removeObserver(this, "fightResult");
    addChild(CFightWinLayer::create(),1000);
}


void CFightScene::DefendSucessRecv(std::string str)
{
    

   Director::getInstance()->pause();
    
    CHitDialog *pDlg = CHitDialog::create();
    
    
    pDlg->CreateHuifanDialog(this,menu_selector(CFightScene::OnRebo),menu_selector(CFightScene::OnbtnOk));
    addChild(pDlg,999);
    

    
    
}

void CFightScene::uploadInfo()
{
    
    
}

void CFightScene::ifGameOver(float time){
    
    if (CFightData::getInstance()->getWinState()==0) {
        return;
    }
    
    m_iTouchExitState = 2;
    
    
    Director::getInstance()->getScheduler()->setTimeScale(1);
    
    
    unschedule(schedule_selector(CFightScene::EndCameraUpdate));
    
    unscheduleAllCallbacks();
    unscheduleUpdate();
    stopAllActions();
    
    stopAllBattle();
    stopAllAttack();
    guiderManagerLayer::getInstance()->removeGuiderLayer();
    
    if (CFightData::getInstance()->getWinState()==1) {
        
        if(m_guiderArrow)
        {
            m_guiderArrow->removeFromParent();
            m_guiderArrow = NULL;
        }
        //CFightData::getInstance()->getAllAttacker();
        if(CFightData::getInstance()->getNowMidLevelId() < TOWER_LEVEL_INDEX)
        {
            updateLevelStar(true);
        }
        //更新任务信息
        if(MY_PERSION->getlevelMode() == 1)
        {
            CDataManager::saveTaskInfoToDB(2);
        }
        else
        {
            CDataManager::saveTaskInfoToDB(1);
        }
        
        CDataManager::saveTaskInfoToDB(15);//记录任意关卡的完成次数
        
        //判读是否有剧情
        int storyType = CDataManager::getStoryTypeByLevelId(CFightData::getInstance()->getNowLevelId());
        if((storyType > 0) && (CPersonalData::getInstance()->getlevelMode() == 0))
        {
            storyManagerLayer::getInstance()->setstoryType((storyState)storyType);
            storyManagerLayer::getInstance()->setStepNum(1);
            storyManagerLayer::getInstance()->createStoryLayer(this);
            NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(CFightScene::showFightResult), "fightResult", NULL);
        }
        else{
            
            //            if (CFightData::getInstance()->getFightType()==FIGHT_COMMON_TYPE||CFightData::getInstance()->getFightType()==FIGHT_TOWERDENFESE_TYPE) {  //如果是塔防或者普通模式
            //                addChild(CFightWinLayer::create(),1000);
            //            }else if (CFightData::getInstance()->getFightType()==FIGHT_PVP_TYPE){
            //
            //            }
            
            
            if(!CPersonalData::getInstance()->getFromVideoList())
            {
                
                
                //矿石飞入
                
                Director::getInstance()->pause();
                if(CPersonalData::getInstance()->getEnemyType() != SPECIFIED_ENEMY&&CFightData::getInstance()->getIsPvpStart())
                {
                    uploadInfo();
                }
                
                
    
                
                addChild(CFightWinLayer::create(),1000);
            }
            else
            {
                //if(m_bWin)
                {
                    DefendSucessRecv("防守失败!");
                }
            }
            
            
        }
        return;
        
    }else if(CFightData::getInstance()->getWinState()==-1) {
        
        if(m_guiderArrow)
        {
            m_guiderArrow->removeFromParent();
            m_guiderArrow = NULL;
        }
        if(CFightData::getInstance()->getNowMidLevelId() < TOWER_LEVEL_INDEX)
        {
            updateLevelStar(true);
        }
        
        
        ResetGlobalZ();
        
        addChild(CFightFailLayer::create(),1000);
        return;
    }
    
    
    
    
}



void CFightScene::addResult()
{
    CBaseFighter* target = CFightData::getInstance()->getEndTarget();
    
    float delayTime = 0;
    float time1=0;
    float time2=0;
    
    if (target)
    {
        
        
        //计算摄像头的位移速度
        
        float speadX = 1500;   //每秒速度
        
        if (_camera->getPositionX()>target->getPositionX())
        {
            speadX = -1500;
        }
        
        
        time1 = abs( _camera->getPositionX()-target->getPositionX())/abs(speadX);
        
        time2 = 0.4f;//横向移动摄像机后，再纵向移动摄像机，纵向移动摄像机所需要的时间
        //            float time =   abs( _camera->getPositionX()-target->getPositionX())/abs(speadX);
        
        //            float speadY = -(_camera->getPositionY()-200-target->getPositionY())/time2;
        //
        //            float speadZ = -(_camera->getPositionZ()-200-target->getPositionZ())/time2;
        
        
        //纵向移动是个变速运动，因此需要一个加速度
        _accelerationDiedY = 2*(_camera->getPositionY()-200-target->getPositionY())/time2/time2;  // s=1/2*a*t^2
        float speadY = -_accelerationDiedY*time2; //  v = a t
        
        _accelerationDiedZ = 2*(_camera->getPositionZ()-200-target->getPositionZ())/time2/time2;  // s=1/2*a*t^2
        float speadZ = -_accelerationDiedZ*time2; //  v = a t
        
        
        _vec3DeadSpead = Vec3(speadX, speadY, speadZ);
        
        CCLOG("cameraSpead:%f,%f,%f",speadX,speadY,speadZ);
        
        
        if (CFightData::getInstance()->getTheLevelCondition()==eLevelCondition_destroyCity
            ||CFightData::getInstance()->getTheLevelCondition()==eLevelCondition_fastKill)
            
        {   //如果是摧毁基地类型的，则等播完防御塔爆炸动画后，再继续，否则帧动画会变得太难看
            delayTime = 0.5f;
            schedule(schedule_selector(CFightScene::EndCameraUpdate),0,CC_REPEAT_FOREVER,delayTime);
        }
        else
        {
            schedule(schedule_selector(CFightScene::EndCameraUpdate));
        }
        
    }
    
    
    scheduleOnce(schedule_selector(CFightScene::ifGameOver), time1+time2+0.1+delayTime);
    
    
}

void CFightScene::update(float time)
{
//    float TTTIME = CFightData::getInstance()->getAllFightTime();
//    
//    if (TTTIME>=5) {
//        CFightData::getInstance()->setWinState(1);
//    }
    
    
    
    
    
    
    LevelCondition target = CFightData::getInstance()->getTheLevelCondition();
    
    if(target == eLevelCondition_killBoss &&CFightData::getInstance()->getFightType() != FIGHT_PVP_TYPE)
    {
        CBaseFighter * p = CFightData::getInstance()->getTheBigBoss();
        float fMax= p->getNowHP()*1.0/p->getMaxHP()*100;
        
        float  fNow = m_pWhiteBossBg->getPercentage();
        
        if((fNow-0.5)<=fMax)
        {
            fNow = fMax;
            
        }
        else
        {
            fNow-=0.5;
        }
        
        
        m_pWhiteBossBg->setPercentage(fNow);
        
        
    }
    
    /*if(CFightData::getInstance()->getFightType() != FIGHT_PVP_TYPE)
    {
        CFightData::getInstance()->addTheFightTime(time);
    }*/
    
    if(m_bFightPvpStart)
    {
        m_iMyTime = 0;
        
        int x = _camera->getPositionX();
        
        m_iCnt =0;
        
        if(x>=(Start3DPOINT)&&!m_bHuitan)
        {
            x-=20;
            _camera->setPositionX(x);
            if(x<=(Start3DPOINT))
            {
                m_bHuitan = true;
            }
        }
        else if(m_bHuitan)
        {
            initUI();
            m_bFightPvpStart= false;
            
        }
    }
    
    
    if(!CFightData::getInstance()->getClearSolider() && (m_bJishiStart))
    {
        m_iMyTime+=time;
        CFightData::getInstance()->addTheFightTime(time);
        
        if(m_iMyTime/1>m_iCnt)
        {
            m_iCnt++;
           
            if(m_bToushicheStart)
            {
                m_iToushiCDTime--;
        
                Label * pLabelTime = (Label*)m_pSwitchItem1->getChildByTag(100);
            
                char strTime[50] = {0};
            
                if(pLabelTime){
                    sprintf(strTime,"%d",m_iToushiCDTime);
                    pLabelTime->setString(strTime);
                }

                
                if(m_iToushiCDTime<=0)
                {
                    EndToushijiCD();
                }
            }
            
            if(CFightData::getInstance()->getFightType() == FIGHT_PVP_TYPE)
                
            {
            m_iAllTime--;
            
            m_iMin = m_iAllTime%3600/60;
            m_iSec = m_iAllTime%60;
            
            CFightData::getInstance()->setThePvpTime(m_iAllTime);
            
            if(m_iMin<=0&&m_iSec<0)
            {
                if(CPersonalData::getInstance()->getEnemyType() == FIGHT_BACK_VIDEO)
                {
                    
                    cocos2d::Vector<CBaseFighter*> pInfo =  CFightData::getInstance()->getAllTowers();
                    
                    if(pInfo.at(0)&&m_bWin)
                    {
                        pInfo.at(0)->setMaxHP(0);
                        pInfo.at(0)->setMaxHP(0);
                        pInfo.at(0)->onDied();
                        
                    }
                    
                    if(m_bWin)
                    {
                        DefendSucessRecv("防守失败!");
                    }
                    else
                    {
                        DefendSucessRecv("防守成功!");
                    }
                    
                }
                else
                {
                    stopAllBattle();
                    if(CPersonalData::getInstance()->getEnemyType() != SPECIFIED_ENEMY&&CFightData::getInstance()->getIsPvpStart())
                    {
                        uploadInfo();
                    }
                    
                    Director::getInstance()->pause();
                    
                    addChild(CFightFailLayer::create(),1000);
                }
                m_bJishiStart =false;
                
                return ;
            }
            char str[50] = {0};
            
            m_iHour =  m_iAllTime/3600;
            m_iMin = m_iAllTime%3600/60;
            m_iSec = m_iAllTime%60;
            
            sprintf(str,"倒计时 %02d:%02d",m_iMin,m_iSec);
            if(m_pLableTime)
            {
                m_pLableTime->setString(str);
            }
        }
        }
    }
    
    
    int iLvId = CFightData::getInstance()->getNowMidLevelId();
    
    if((CFightData::getInstance()->getNowMidLevelId() < TOWER_LEVEL_INDEX) && (CFightData::getInstance()->getFightType()!=FIGHT_PVP_TYPE))
    {
        updateLevelStar();
    }
    
    //如果获得了战斗成功或者失败的标志，则打开胜利或者失败界面
    
    //CFightData::getInstance()->setWinState(1);
    
    
    
    
    if (CFightData::getInstance()->getWinState()) {
        unscheduleUpdate();
        
        
        //图标砸下去
        
        std::string str = "taskto.png";
        
        
        if(CFightData::getInstance()->getWinState() == -1)
        {
            str = "taskfail.png";
        }
        
        
        if(CFightData::getInstance()->getFightType() == FIGHT_PVP_TYPE)
        {
            addResult();
        }
        else
        {
            
            Director::getInstance()->getScheduler()->setTimeScale(1);
            
            Sprite *pTaskPic = Sprite::createWithSpriteFrameName(str);
            pTaskPic->setScale(3);
            pTaskPic->setPosition(m_WinSize.width*0.825,m_WinSize.height*0.95);
            
            addChild(pTaskPic,999);
            
          
            
            pTaskPic->runAction(Sequence::create(Spawn::create(ScaleTo::create(0.1,1),
                                                 CallFunc::create([=]{
                
                //SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0);
                SimpleAudioEngine::getInstance()->stopBackgroundMusic();
                
                SimpleAudioEngine::getInstance()->playEffect("music/Swish01.mp3");
                
                
            }),NULL),DelayTime::create(0.5),CallFunc::create([=]{
                
                //SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(1);
                //SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
                
                addResult();
                
            }),NULL));
        }
        
    }
    
    
    if((((CDataManager::getEndedBy(GUIDE_ADD_LINE)) && (CFightData::getInstance()->getNowLevelId() == 1))
        || ((CDataManager::getEndedBy(GUIDE_ADD_CIRCLE)) && (CFightData::getInstance()->getNowLevelId() == 2))
        || ((CDataManager::getEndedBy(GUIDE_ADD_RECTANGLE)) && (CFightData::getInstance()->getNowLevelId() == 5))
        || ((CDataManager::getEndedBy(GUIDE_ADD_V)) && (CFightData::getInstance()->getNowLevelId() == 8))) && (m_pauseMenu&&!m_pauseMenu->isEnabled()))
    {
        m_pauseMenu->setEnabled(true);
    }
    switchBtnValid(time);
    
    //更新最大士兵数量
    if(m_pSoldierNum){
        __String * temp = __String::createWithFormat("%d/%d",CFightData::getInstance()->getNowMonsterNum(),CFightData::getInstance()->getMaxMonsterNum());
        m_pSoldierNum->setString(temp->getCString());
    }
    
    
    CFightData::getInstance()->addTimeEnergy(time);
    
    if(m_isTouch)//切兵技能
    {
        if((m_showCombo < m_comboNum) && (m_fshowComboInteval >=0.15))
        {
            m_showCombo=m_comboNum;
            m_fshowComboInteval = 0;
            updateCombo();
        }
        m_fshowComboInteval += time;
    }else if(m_isblow)//吹风技能前的摄像机视角调整
    {
        //LYMenuItemSprite* pItemMenu = (LYMenuItemSprite*)(pSkillMenu->getChildByTag(SKILL_BLOW_TAG));
        if (m_adjustAngle < BLOW_CAMERA_ANGEL) {
            //            if (pItemMenu)
            //            {
            //                pItemMenu->setEnabled(false);
            //                pItemMenu->setColor(Color3B::GRAY);
            //            }
            moveCameraPos(_camera->getPosition3D().x,CAMERA_POSITION_Y - m_adjustAngle,CAMERA_POSITION_Z);
            //            _camera->setPosition3D(Vec3(_camera->getPosition3D().x,CAMERA_POSITION_Y - m_adjustAngle,CAMERA_POSITION_Z));
            _camera->lookAt(Vec3(_camera->getPosition3D().x,CAMERA_LOOK_POSITION_Y+m_adjustAngle,CAMERA_LOOK_POSITION_Z), Vec3(0,1,0) );
            m_adjustAngle+=3;
            m_reverseAdjust = m_adjustAngle;
        }
    }
    else
    {
        if(m_bifBlowEnd)
        {
            //Menu* pMenu = (Menu*)(m_layer->getChildByTag(50));
            if (m_adjustAngle < m_reverseAdjust) {
                //                if (pMenu)
                //                {
                //                    pMenu->setEnabled(false);
                //                }
                moveCameraPos(_camera->getPosition3D().x,CAMERA_POSITION_Y - m_reverseAdjust+ m_adjustAngle,CAMERA_POSITION_Z);
                //                _camera->setPosition3D(Vec3(_camera->getPosition3D().x,CAMERA_POSITION_Y - m_reverseAdjust+ m_adjustAngle,CAMERA_POSITION_Z));
                _camera->lookAt(Vec3(_camera->getPosition3D().x,m_reverseAdjust+CAMERA_LOOK_POSITION_Y-m_adjustAngle,CAMERA_LOOK_POSITION_Z), Vec3(0,1,0) );
                m_adjustAngle+=3;
            }
            else{
                m_reverseAdjust = 0;
                m_adjustAngle = 0;
                m_bifBlowEnd = false;
            }
        }
        
        //如果英雄需要摄像机跟随，则摄像机跟随
        CBaseHero* theHero= CFightData::getInstance()->getTheFriendHero();
        if (theHero&&theHero->getIfCameraFollow()) {
            
            float offsetX = theHero->getPositionX();
            
            CameraRunWithSmallMap(offsetX);
            
            if (offsetX>Start3DPOINT&&offsetX<END3DPOINT) {
                //                _camera->setPositionX(offsetX);
                moveCameraPos(offsetX);
            }
            
            
            
        }
    }
    float percent = CFightData::getInstance()->getNowEnergy()*1.0/CFightData::getInstance()->getMaxEnergy()*100;
    if (m_pTheEnergyBar) {
        m_pTheEnergyBar->setPercentage(percent);
    }
    
    
    for(int i = eFighterType_line; i <= eFighterType_v; i++)
    {
        Sprite* line = (Sprite*)(m_layer->getChildByTag(200 + i));
        if(line)
        {
            FighterInfo* monsterInfo = CPersonalData::getInstance()->getTeamSolderByType(i);
            if(monsterInfo && (CFightData::getInstance()->getNowEnergy() < monsterInfo->energyCost))
            {
                line->setColor(Color3B::GRAY);
            }
            else if(monsterInfo)
            {
                line->setColor(Color3B::WHITE);
            }
        }
    }
    if(!CFightData::getInstance()->getClearSolider() && (m_bJishiStart) && !CFightData::getInstance()->getUseSkill())
    {
        if(m_dtTime > 1)
        {
            m_dtTime = 0;
            CFightData::getInstance()->setFightData(0);
        }
        else{
            m_dtTime += time;
        }
    }
    
    //如果是塔防模式，则需要刷新路点信息
    if (CFightData::getInstance()->getTheLevelCondition()==eLevelCondition_TowerDefense) {
        CFightData::getInstance()->updateAllPathPosIdle();
        __String* tempStr = __String::create("");
        
        tempStr->initWithFormat("%d",CFightData::getInstance()->getStone());
        if(m_pStoneLable)
        {
            m_pStoneLable->setString(tempStr->getCString());
        }
        if(m_bUpdateTime)
        {
            updateTD(time);
        }
        if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_TOWER_LEVEL) && (guiderManagerLayer::getInstance()->getStepNum() == 5)
           && (CFightData::getInstance()->getStone() >= 150) && m_bUpdateTime)
        {
            //如果没有升级过塔则显示引导
            bool bUpgraded = false;//判断是否升级过塔
            CBaseBuilding* tempTarget = NULL;
            std::vector<obbInfo>* theObb = CFightData::getInstance()->getAllTowerObb();
            for(int i = 0; i < theObb->size(); i++)
            {
                tempTarget = (CBaseBuilding*)theObb->at(i).theFighter;
                if(tempTarget->getTheInfo()->monsterId != TD_BASE_TOWER_ID)
                {
                    bUpgraded = true;
                    break;
                }
            }
            if(!bUpgraded)
            {
                m_bGuideBlow = true;
                m_bUpdateTime = false;
                cocos2d::Vector<CBaseFighter*>allSolider = CFightData::getInstance()->getAllAttacker();
                for (cocos2d::Vector<CBaseFighter*>::iterator it = allSolider.begin(); it != allSolider.end(); it++)
                {
                    CBaseFighter* sp = *it;
                    if((sp->getFightGroup() == eFightGroup_Enemy) && (sp->getAttackType() == eAttackType_monster))
                    {
                        sp->gettheSprite()->stopAllActions();//暂停士兵的所有动作
                        sp->unscheduleUpdate();
                        sp->unscheduleAllCallbacks();
                        sp->stopAllActions();
                    }
                }
                
                tempTarget = (CBaseBuilding*)theObb->at(0).theFighter;
                Vec2 targetPos = CommonData::GetSingletonPtr()->getPosOnScreen(_camera, tempTarget->getPosition3D());
                guiderManagerLayer::getInstance()->setStepNum(guiderManagerLayer::getInstance()->getStepNum() + 1);
                guiderManagerLayer::getInstance()->m_pos = targetPos;
                guiderManagerLayer::getInstance()->createLayer(this);
            }
        }
    }
    //测试用注释掉
    if(!m_bGuideBlow)
    {
        enemyMonsterUpdate(time);
    }
    if (bArrow && !m_guiderArrow && ( ((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SKILL_BLOW) && (guiderManagerLayer::getInstance()->getStepNum() == 5)) || ((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SKILL_CUT) && (guiderManagerLayer::getInstance()->getStepNum() == 3))))  //((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_ADD_LINE) && (guiderManagerLayer::getInstance()->getStepNum() == 7)) ||
    {
        CCLOG("刷新");
        m_bTouchMap = true;
        m_guiderArrow = Sprite::createWithSpriteFrameName("finger_1.png");
        addChild(m_guiderArrow, 1000);
        m_guiderArrow->setPosition(Vec2(m_winSize.width * 0.1f, m_winSize.height * 0.02));
        ActionInterval* action1 = MoveBy::create(1.25f, Vec2(200, 0));
        ActionInterval* action2 = MoveBy::create(0, Vec2(-200, 0));
        ActionInterval* seq = RepeatForever::create(Sequence::create(action1, action2, NULL));
        seq->setTag(10);
        m_guiderArrow->runAction(seq);
        
        //显示对话框
        m_GuideNode = Node::create();
        addChild(m_GuideNode, 1000);
        m_GuideNode->setPosition(Vec2::ZERO);
        std::string str;
        //        if(guiderManagerLayer::getInstance()->getGuideType() == GUIDE_ADD_LINE)
        //        {
        //            str = "拖动您的地图来查看战况~";
        //        }
        //        else
        if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SKILL_BLOW)
           || (guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SKILL_CUT))
        {
            str = "然后，将屏幕移动到需要使用技能的区域";
        }
        Sprite* dialog = Sprite::createWithSpriteFrameName("dialog.png");
        m_GuideNode->addChild(dialog);
        dialog->setPosition(Vec2(m_WinSize.width * 0.53, m_WinSize.height * 0.5));
        Label* pWord = Label::createWithTTF(str, "fonts/cuti.ttf", 30, cocos2d::Size(460, 200));
        dialog->addChild(pWord, 1);
        pWord->setColor(Color3B::BLACK);
        pWord->setAnchorPoint(Vec2(0, 0.5));
        pWord->setPosition(50, dialog->getContentSize().height * 0.4f);
        
        Sprite* girl = Sprite::createWithSpriteFrameName("girl.png");
        m_GuideNode->addChild(girl, 1);
        girl->setScale(0.75f);
        girl->setPosition(Vec2(m_winSize.width * 0.76f, m_winSize.height * 0.5));
    }
    
    if(CFightData::getInstance()->getFirstSolider() && (CFightData::getInstance()->getNowLevelId() == 2))
    {
        int guideType = CDataManager::getGuideStepNum();
        CFightData::getInstance()->setFirstSolider(false);
        guiderManagerLayer::getInstance()->setStepNum(1);
        Vec2 pos = Vec2(m_WinSize.width * 0.75f, m_WinSize.height * 0.94f);
        guiderManagerLayer::getInstance()->setGuideType(GuideState(guideType + 1));
        guiderManagerLayer::getInstance()->createLayer(this, GUIDE_SHOW_BOTH, pos);
    }
    
    //更新战斗对话间隔
    float talkTime = CFightData::getInstance()->getNowTalkTime();
    talkTime+=time;
    CFightData::getInstance()->setNowTalkTime(talkTime);
    
    
    //更新摄像机位置
    updateCamera(time);
    
    //更新场景效果
    updateBGpng(time);
}


static float EffectTime = 0;
void CFightScene::updateBGpng(float time)
{
    
    if (!m_strBGEffect.compare("")) {
        return;
    }
    
    
    EffectTime+= time;
    
    if (EffectTime>=6) {
        EffectTime=0;
        
        Sprite* effectPng = Sprite::create(m_strBGEffect.c_str());
        
        
        float posx = rand()%int(m_WinSize.width*2)+m_WinSize.width*0.1f;
        float posy = rand()%int(m_WinSize.height*0.5f)+m_WinSize.height*0.2f;
        
        float speed = rand()%10+10;
        
        float moveTime = rand()%10+12;
        
        
        int randomLayer = rand()%m_vecScenePng.size();//随机放在一个特效layer上
        
        
        
        effectPng->setPosition(Vec2(posx, posy));
        m_vecScenePng.at(randomLayer)->addChild(effectPng,1000);
        effectPng->setOpacity(0);
        
        auto temp = Spawn::create(MoveBy::create(moveTime, Vec2(speed*moveTime, 0)),Sequence::create(FadeTo::create(moveTime*0.5f, 200),FadeOut::create(moveTime*0.5f), NULL), NULL);
        
        effectPng->runAction(Sequence::create(temp,RemoveSelf::create(), NULL));
    }
}


void CFightScene::addRecIsWin(int iWin)
{
    m_bWin = iWin==1?true:false;
    
}


void CFightScene::addRecEndTime(int fTime)
{
    m_iRecEndTime = fTime;
}

void CFightScene::finishTowerLevelGuide()
{
    m_bGuideBlow = false;
    m_bUpdateTime = true;
    cocos2d::Vector<CBaseFighter*>allSolider = CFightData::getInstance()->getAllAttacker();
    for (cocos2d::Vector<CBaseFighter*>::iterator it = allSolider.begin(); it != allSolider.end(); it++)
    {
        CBaseFighter* sp = *it;
        if((sp->getFightGroup() == eFightGroup_Enemy) && (sp->getAttackType() == eAttackType_monster))
        {
            sp->scheduleUpdate();
            sp->setAnimateState(eFighteState_Idle);//防止怪物技能卡死
            sp->stateChange(eFighteState_Idle);
            
            
            if(sp->getIfDied()==true){ //如果是本来就死掉得怪，则直接死掉
                
                sp->removeFromParentAndCleanup(true);
                
            }
        }
    }
}

void CFightScene::removeGuide(Ref *pSender)
{
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(CFightScene::damageGuide), "damageGuide", NULL);
    if(m_GuideNode)
    {
        m_GuideNode->removeFromParent();
        m_GuideNode = NULL;
    }
    if(m_guiderArrow)
    {
        m_guiderArrow->removeFromParent();
        m_guiderArrow = NULL;
    }
    
}

void CFightScene::addTouishicheGuide()
{
    //显示对话框
    std::string str;
    if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_USE_MANGONEL) && (guiderManagerLayer::getInstance()->getStepNum() == 6))
    {
        guiderManagerLayer::getInstance()->setStepNum(5);
        guiderManagerLayer::getInstance()->createLayer(this);
    }
    
}

void CFightScene::damageGuide(Ref *pSender)
{
    if(CPersonalData::getInstance()->getShotted())
    {
        NotificationCenter::getInstance()->removeObserver(this, "removeGuide");
        NotificationCenter::getInstance()->removeObserver(this, "damageGuide");
        guiderManagerLayer::getInstance()->setStepNum(guiderManagerLayer::getInstance()->getStepNum() + 1);
        guiderManagerLayer::getInstance()->createLayer(this);
        m_bifNorAddSolider = true;
        m_paoWuLayer->m_startBall->setVisible(false);
    }
    else{
        m_pToushiActive->setVisible(true);
        m_paoWuLayer->getTheDragBall()->setVisible(true);
        m_paoWuLayer->getTheDragBall()->setPosition(m_pToushiActive->getPosition());
        
        //        Vec3  VCamera(Start3DPOINT,_camera->getPositionY(),_camera->getPositionZ());
        CameraRunWithSmallMap(Start3DPOINT);
        moveCameraPos(Start3DPOINT,_camera->getPositionY(),_camera->getPositionZ());
        //        _camera->setPosition3D(VCamera);
        addTouishicheGuide();
    }
}


void CFightScene::updateCamera(float time){
    
    
    if(m_bToushiMove)
    {
        return ;
    }
    if (m_bSmallMapMoveStart) {
        return;
    }
    if(!m_bSmallMapAutoMove)
    {
        return;
    }
    
    if (CFightData::getInstance()->getTheLevelCondition()==eLevelCondition_TowerDefense) {  //塔防模式不更新摄像机位置
        return;
    }
    
    
    CBaseFighter* target = CFightManager::getTheFrontSoldierAndBuilding(eFightGroup_Freind);
    
    if(!target){
        return;
    }
    
    
    
    //每帧最大移动X值
    int offsetMaxX = 20;
    int direction = 1;
    
    if (_camera->getPositionX()>target->getPositionX()) {
        direction = -1;
        offsetMaxX = 5;//如果是拉回镜头，速度可以慢很多
    }
    
    
    if (abs(_camera->getPositionX()-target->getPositionX())<offsetMaxX) {
        offsetMaxX=abs(_camera->getPositionX()-target->getPositionX());
    }
    
    
    float cameraPosX = _camera->getPositionX()+offsetMaxX*direction;
    
    
    if(cameraPosX<Start3DPOINT)
    {
        cameraPosX=Start3DPOINT;
    }
    else if(cameraPosX>END3DPOINT) //此处为移动小地图时的边界判断
    {
        cameraPosX=END3DPOINT;
    }
    
    
    
    //    _camera->setPositionX(cameraPosX);
    moveCameraPos(cameraPosX);
    
    CameraRunWithSmallMap(_camera->getPositionX());
    
}








void CFightScene::addStarEffect()
{
    SimpleAudioEngine::getInstance()->playEffect("music/addStar.mp3");
}

void CFightScene::stopAllBattle(){
    
    //暂停背景音乐
    SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    cocos2d::Vector<CBaseFighter*> temp = CFightData::getInstance()->getAllBaseFight();
    
    
    for (int i=0; i<temp.size(); i++) {
        
        CBaseFighter* target = temp.at(i);
        
        if (target&&target->getReferenceCount()>0) {
            
            target->stopAllActions();
            target->unscheduleAllCallbacks();
            target->unscheduleUpdate();
        }
        
    }
    
}




void CFightScene::enemyMonsterUpdate(float time){
    
    
    //战场计时
    
    //如果是塔防模式下，则不走此逻辑
    if (CFightData::getInstance()->getTheLevelCondition()==eLevelCondition_TowerDefense) {
        return;
    }
    
    
    //pvp模式下，不走此逻辑
    if (m_eFightSceneType==eFightSceneType_PVP) {
        return;
    }
    
    
    float nowTime = CFightData::getInstance()->getAllFightTime();
    int nowWave =CFightData::getInstance()->getNowWave();
    
    
    
    
    if(CFightData::getInstance()->getTheLevelCondition()==eLevelCondition_kill){
        
        
        //        if (nowWave>3) {
        //            CFightData::getInstance()->setWinState(1);
        //            return;
        //        }
        
        long num = CFightData::getInstance()->getFriendMonstersByGroup(eFightGroup_Enemy,1).size(); //获得所有活着的怪物数量
        
        //如果活着的怪物数量为0，则出下一波兵
        if (num==0) {
            
            CFightData::getInstance()->addTheFightConditon(eLevelCondition_kill, 1,CFightData::getInstance()->getlastDiedTarget());
            if (CFightData::getInstance()->getWinState()==1) {
                return;
            }
            
            
            nowWave++;
            CFightData::getInstance()->setNowWave(nowWave);
            addLevelMonster(3*nowWave);
            addLevelMonster(nowWave/2,cocos2d::Point::ZERO,eEnemyType_boss);
        }
        
    }else if(CFightData::getInstance()->getTheLevelCondition()==eLevelCondition_defense){
        if ((int(nowTime)/25)>=nowWave) {//防守模式下，15秒出下一波怪,第一秒就出怪
            
            CFightData::getInstance()->setNowWave(nowWave+1);
            
            addLevelMonster(4+nowWave);
            
            addLevelMonster((1+nowWave)/2,cocos2d::Point::ZERO,eEnemyType_boss); //每两波多一个小boss
            
        }
        
        //判断能量，如果能量够了则随机招怪，这里有个小处理，要等能量比最耗能量的怪多得时候才随机。不然的话逻辑有误
        if (CFightData::getInstance()->getNowEnergy_Enemy()>=theMostEnemyCost) {
            
            addLevelMonster();
        }
    }else if(CFightData::getInstance()->getTheLevelCondition()==eLevelCondition_protect){
        
        //护送模式下，每走到一段距离，就出下一波怪
        
        
        if (CFightManager::getTheWalker()) {
            float posX = CFightManager::getTheWalker()->getPositionX()-Start3DPOINT;//当前最靠前的怪物走出的距离
            
            
            float dist = (END3DPOINT-Start3DPOINT+WALKER_TARGET_X-200)/3;//出每波怪所间隔的距离
            
            
            
            if ((posX/dist)>nowWave) {
                
                CFightData::getInstance()->setNowWave(nowWave+1);
                
                addLevelMonster(2*nowWave);
                
                addLevelMonster(nowWave/2,cocos2d::Point::ZERO,eEnemyType_boss); //每两波多一个小boss
                
            }
            
            //判断能量，如果能量够了则随机招怪，这里有个小处理，要等能量比最耗能量的怪多得时候才随机。不然的话逻辑有误
            if (CFightData::getInstance()->getNowEnergy_Enemy()>=theMostEnemyCost) {
                
                addLevelMonster();
            }
        }
        
        
    }else if(CFightData::getInstance()->getTheLevelCondition()==eLevelCondition_destroyCity||CFightData::getInstance()->getTheLevelCondition()==eLevelCondition_killBoss){
        if(nowWave==0){
            
            CFightData::getInstance()->setNowWave(nowWave+1);
            
            addLevelMonster(5);
            addLevelMonster(1,cocos2d::Point::ZERO,eEnemyType_boss); //每两波多一个小boss
        }
        
        //        if ((int(nowTime)/40)>=nowWave) {
        //
        //            CFightData::getInstance()->setNowWave(nowWave+1);
        //
        //            addLevelMonster(3+nowWave);
        //
        //            addLevelMonster((1+nowWave)/2,cocos2d::Point::ZERO,eEnemyType_boss); //每两波多一个小boss
        //
        //        }
        
        //判断能量，如果能量够了则随机招怪，这里有个小处理，要等能量比最耗能量的怪多得时候才随机。不然的话逻辑有误
        if (CFightData::getInstance()->getNowEnergy_Enemy()>=theMostEnemyCost) {
            
            int nowNum = CFightData::getInstance()->getAllEnemy()->size();
            
            int maxNum = CFightData::getInstance()->getMaxMonsterNum();
            
            if (nowNum<7) {  //当在场的怪物数小于玩家最大召唤数的时候
                addLevelMonster();
            }
            
        }
        //    }else if(CFightData::getInstance()->getTheLevelCondition()==eLevelCondition_killBoss){
        //        if ((int(nowTime)/25)>=nowWave) {//防守模式下，15秒出下一波怪,第一秒就出怪
        //
        //            CFightData::getInstance()->setNowWave(nowWave+1);
        //
        //            addLevelMonster(4+nowWave);
        //
        //            addLevelMonster((1+nowWave)/2,cocos2d::Point::ZERO,eEnemyType_boss); //每两波多一个小boss
        //
        //        }
        //
        //        //判断能量，如果能量够了则随机招怪，这里有个小处理，要等能量比最耗能量的怪多得时候才随机。不然的话逻辑有误
        //        if (CFightData::getInstance()->getNowEnergy_Enemy()>=theMostEnemyCost) {
        //
        //            addLevelMonster();
        //        }
    }else{
        if ((int(nowTime)/30)>=nowWave) {
            
            CFightData::getInstance()->setNowWave(nowWave+1);
            
            addLevelMonster(3+nowWave);
            
            addLevelMonster((1+nowWave)/2,cocos2d::Point::ZERO,eEnemyType_boss); //每两波多一个小boss
            
        }
        
        //判断能量，如果能量够了则随机招怪，这里有个小处理，要等能量比最耗能量的怪多得时候才随机。不然的话逻辑有误
        if (CFightData::getInstance()->getNowEnergy_Enemy()>=theMostEnemyCost) {
            
            addLevelMonster();
        }
    }
    
    
    
    if(m_bHaveStory && (nowTime  >= m_storyStartTime) && (CPersonalData::getInstance()->getlevelMode() == 0))
    {
        Director::getInstance()->pause();
        m_bHaveStory = false;
        int storyType = CDataManager::getStoryTypeByLevelId(CFightData::getInstance()->getNowLevelId(), 1);
        storyManagerLayer::getInstance()->setstoryType((storyState)storyType);
        storyManagerLayer::getInstance()->setStepNum(1);
        storyManagerLayer::getInstance()->createStoryLayer(this);
    }
    
    
    //如果是第一波怪过去3秒后
    if((nowTime >3))
    {
        //显示吹风技能指引 屏蔽吹风技能
//        if(!m_bGuideBlow && !(CDataManager::getEndedBy(GUIDE_SKILL_BLOW)) && (guiderManagerLayer::getInstance()->getStepNum() == 0)
//           && (CFightData::getInstance()->getNowLevelId() == 18) && (CPersonalData::getInstance()->getlevelMode() == 0))
//        {
//            //                cocos2d::Vector<CBaseFighter*>allSolider = CFightData::getInstance()->getAllAttacker();
//            //                for (cocos2d::Vector<CBaseFighter*>::iterator it = allSolider.begin(); it != allSolider.end(); it++)
//            //                {
//            //                    CBaseFighter* sp = *it;
//            //                    if (sp) {
//            //                        sp->unscheduleUpdate();//技能时间结束后判断士兵的血量
//            //                    }
//            //                }
//            guiderManagerLayer::getInstance()->setStepNum(guiderManagerLayer::getInstance()->getStepNum() + 1);
//            guiderManagerLayer::getInstance()->setGuideType(GUIDE_SKILL_BLOW);
//            guiderManagerLayer::getInstance()->createLayer(this);
//        }
//        else
        if(!(CDataManager::getEndedBy(GUIDE_SKILL_CUT)) && !m_bGuideBlow && (guiderManagerLayer::getInstance()->getStepNum() == 0)
                && (CFightData::getInstance()->getNowLevelId() == 9) && (CPersonalData::getInstance()->getlevelMode() == 0))
        {
            //                cocos2d::Vector<CBaseFighter*>allSolider = CFightData::getInstance()->getAllAttacker();
            //                for (cocos2d::Vector<CBaseFighter*>::iterator it = allSolider.begin(); it != allSolider.end(); it++)
            //                {
            //                    CBaseFighter* sp = *it;
            //                    if (sp) {
            //                        sp->unscheduleUpdate();//技能时间结束后判断士兵的血量
            //                    }
            //                }
            guiderManagerLayer::getInstance()->setStepNum(guiderManagerLayer::getInstance()->getStepNum() + 1);
            guiderManagerLayer::getInstance()->setGuideType(GUIDE_SKILL_CUT);
            guiderManagerLayer::getInstance()->createLayer(this);
        }
    }
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    //    if (nowWave<theBombTime->size()) {//如果已经出来的怪物波数还没够的话，则判断时间，到了时间则出怪
    //
    //        std::vector<bombTime>::iterator ite = theBombTime->begin();
    //
    //        ite = ite + nowWave;
    //        if(nowTime == ((*ite).theBombTime - 5))
    //        {
    //            SimpleAudioEngine::getInstance()->playEffect("music/monster.mp3");
    //        }
    //        if((nowTime - theBombTime->at(0).theBombTime) > 3)
    //        {
    //            //显示吹风技能指引
    //            if(!m_bGuideBlow && !(CDataManager::getEndedBy(GUIDE_SKILL_BLOW)) && (guiderManagerLayer::getInstance()->getStepNum() == 0)
    //               && (CFightData::getInstance()->getNowLevelId() == 18))
    //            {
    ////                cocos2d::Vector<CBaseFighter*>allSolider = CFightData::getInstance()->getAllAttacker();
    ////                for (cocos2d::Vector<CBaseFighter*>::iterator it = allSolider.begin(); it != allSolider.end(); it++)
    ////                {
    ////                    CBaseFighter* sp = *it;
    ////                    if (sp) {
    ////                        sp->unscheduleUpdate();//技能时间结束后判断士兵的血量
    ////                    }
    ////                }
    //                guiderManagerLayer::getInstance()->setStepNum(guiderManagerLayer::getInstance()->getStepNum() + 1);
    //                guiderManagerLayer::getInstance()->setGuideType(GUIDE_SKILL_BLOW);
    //                guiderManagerLayer::getInstance()->createLayer(this);
    //            }
    //            else if(!(CDataManager::getEndedBy(GUIDE_SKILL_CUT)) && !m_bGuideBlow && (guiderManagerLayer::getInstance()->getStepNum() == 0)
    //                    && (CFightData::getInstance()->getNowLevelId() == 9))
    //            {
    ////                cocos2d::Vector<CBaseFighter*>allSolider = CFightData::getInstance()->getAllAttacker();
    ////                for (cocos2d::Vector<CBaseFighter*>::iterator it = allSolider.begin(); it != allSolider.end(); it++)
    ////                {
    ////                    CBaseFighter* sp = *it;
    ////                    if (sp) {
    ////                        sp->unscheduleUpdate();//技能时间结束后判断士兵的血量
    ////                    }
    ////                }
    //                guiderManagerLayer::getInstance()->setStepNum(guiderManagerLayer::getInstance()->getStepNum() + 1);
    //                guiderManagerLayer::getInstance()->setGuideType(GUIDE_SKILL_CUT);
    //                guiderManagerLayer::getInstance()->createLayer(this);
    //            }
    //        }
    //        if(m_bHaveStory && ((nowTime - theBombTime->at(0).theBombTime) >= m_storyStartTime) && (CPersonalData::getInstance()->getlevelMode() == 0))
    //        {
    //            Director::getInstance()->pause();
    //            m_bHaveStory = false;
    //            int storyType = CDataManager::getStoryTypeByLevelId(CFightData::getInstance()->getNowLevelId());
    //            storyManagerLayer::getInstance()->setstoryType((storyState)storyType);
    //            storyManagerLayer::getInstance()->setStepNum(1);
    //            storyManagerLayer::getInstance()->createStoryLayer(this);
    //        }
    //
    //        if (nowTime>=(*ite).theBombTime) {
    //
    //
    //            CFightData::getInstance()->setNowWave(nowWave+1);
    //
    //            addLevelMonster((*ite).theBombNum);
    //
    //            addLevelMonster((*ite).theBossNum,cocos2d::Point::ZERO,eEnemyType_boss);
    //
    //
    //        }
    //
    //    }else{  //如果已经过了预写的出怪时间，则每过30秒出一次怪
    //
    //        float Overtime = theBombTime->at(theBombTime->size()-1).theBombTime;
    //
    //        if ((int(nowTime-Overtime)/30)>(nowWave-theBombTime->size())) {
    //
    //            CFightData::getInstance()->setNowWave(nowWave+1);
    //
    //            addLevelMonster(5);
    //
    //            addLevelMonster(1,cocos2d::Point::ZERO,eEnemyType_boss);
    //
    //        }
    //
    //
    //
    //
    //    }
    
    
    
    //援军相关判断
    for (std::vector<reinforcementInfo*>::iterator ite = CFightData::getInstance()->theReinforcementInfo.begin(); ite!=CFightData::getInstance()->theReinforcementInfo.end(); ) {
        
        reinforcementInfo* temp = *ite;
        
        if (temp->happenTime<=nowTime&&temp->theMonsterInfo.monsterId!=0) {
            
            //召唤怪物
            
            for (int i=0; i<temp->allPos.size(); i++) {
                
                CBaseFighter* target = addMonsterWithIdAndGroupAndPosition(&temp->theMonsterInfo, (FightGroup)temp->monsterGroup, temp->allPos.at(i));
                target->setIfReinforce(true);
            }
            
            CFightData::getInstance()->theReinforcementInfo.erase(ite);
        }else{
            ite++;
        }
    }
    
    
    
    
    
    
}



void CFightScene::CameraRunWithSmallMap(float CameraPosX)
{
    
    //    float CameraPosX = location.x/m_WinSize.width*BIGMAPLEN + Start3DPOINT-ONESIDE;
    
    float x = m_WinSize.width;
    
    Sprite *pSmallMap = (Sprite*)getChildByTag(FIGHT_SMALLMAP);
    
    //float t = pSmallMap->getBoundingBox().size.width;
    
    
    if(!m_bPvpAllMapMove)
    {
        x = pSmallMap->getBoundingBox().size.width;
    }
    
    float locationX = (CameraPosX+ONESIDE-Start3DPOINT)*x/BIGMAPLEN;
    
    if((locationX)> (x-m_pCurView->boundingBox().size.width*0.5))
    {
        m_pCurView->setPositionX(x-m_pCurView->boundingBox().size.width*0.5);
    }
    else if((locationX)<m_pCurView->boundingBox().size.width*0.5)
    {
        m_pCurView->setPositionX(m_pCurView->boundingBox().size.width*0.5);
    }
    else
    {
        m_pCurView->setPositionX(locationX);
    }
    
}

void CFightScene::MoveCameraWithSmallMapPos(cocos2d::Point location)
{
    
    Sprite *pSmallMap = (Sprite*)getChildByTag(FIGHT_SMALLMAP);
    
    float x  = m_WinSize.width;
    
    if(!m_bPvpAllMapMove)
    {
        x = pSmallMap->getBoundingBox().size.width;
    }
    
    if(location.x<m_pCurView->boundingBox().size.width*0.5)
    {
        location.x = m_pCurView->boundingBox().size.width*0.5;
    }
    else if(location.x>x-m_pCurView->boundingBox().size.width*0.5)
    {
        location.x = x-m_pCurView->boundingBox().size.width*0.5;
    }
    
    
    m_pCurView->setPositionX(location.x);
    
    
    //此处有一坑，正交摄像机和透视摄像机的中心点是不同的。相当于透视摄像机的中心点在中心，正交摄像机的中心点在左边。
    
    //正交摄像机的时间点
    //    float CameraPosX = location.x/m_WinSize.width*BIGMAPLEN + Start3DPOINT-ONESIDE-m_WinSize.width*0.5f;
    
    //透视摄像机的时间点
    
    
    // float CameraPosX = location.x/x*BIGMAPLEN + Start3DPOINT-ONESIDE-x*0.5;
    float CameraPosX = location.x/x*BIGMAPLEN + Start3DPOINT-ONESIDE;
    
    
    //    float CameraPosX = Start3DPOINT;
    
    moveCameraPos(CameraPosX,_camera->getPositionY(),_camera->getPositionZ());
    //    _camera->setPosition3D(Vec3(CameraPosX,_camera->getPositionY(),_camera->getPositionZ()));
    
    
    //正交摄像机的区域判断
    //    if(_camera->getPosition3D().x<Start3DPOINT-m_WinSize.width*0.5f)
    //    {
    //        _camera->setPosition3D(Vec3(Start3DPOINT-m_WinSize.width*0.5f,_camera->getPositionY(),_camera->getPositionZ()));
    //    }
    //
    //    else if(_camera->getPosition3D().x>END3DPOINT-m_WinSize.width*0.5f)
    //    {
    //        _camera->setPosition3D(Vec3(END3DPOINT-m_WinSize.width*0.5f,_camera->getPositionY(),_camera->getPositionZ()));
    //    }
    
    if(_camera->getPosition3D().x<Start3DPOINT)
    {
        moveCameraPos(Start3DPOINT,_camera->getPositionY(),_camera->getPositionZ());
        //        _camera->setPosition3D(Vec3(Start3DPOINT,_camera->getPositionY(),_camera->getPositionZ()));
    }
    
    else if(_camera->getPosition3D().x>END3DPOINT)
    {
        moveCameraPos(END3DPOINT,_camera->getPositionY(),_camera->getPositionZ());
        //        _camera->setPosition3D(Vec3(END3DPOINT,_camera->getPositionY(),_camera->getPositionZ()));
    }
    
}


void CFightScene::levelInit(){
    
    
    
    LevelCondition target = CFightData::getInstance()->getTheLevelCondition();
    
    switch (target) {
        case eLevelCondition_destroyCity://如果目标是摧毁基地，则初始化主基地
        case eLevelCondition_fastKill:
            addMonsterWithIdAndGroupAndPosition(CFightData::getInstance()->getTheEnemyCityInfo(),eFightGroup_Enemy,Vec2(END3DPOINT-ONESIDE*0.2f, (ENDZPOINT-STARTZPOINT)/2+STARTZPOINT),eAttackType_city);//
            break;
            
        case eLevelCondition_killBoss://boss模式，则新增一个大boss
        {
            
            CBaseFighter* target = addMonsterWithIdAndGroupAndPosition(CFightData::getInstance()->getTheLevelBossInfo(),eFightGroup_Enemy,Vec2(END3DPOINT, (ENDZPOINT-STARTZPOINT)/2+STARTZPOINT),eAttackType_monster,false,true);
            target->setIfLevelBoss(true);
            target->changeSpriteScale(2.2);
            
            CFightData::getInstance()->setTheBigBoss(target);
            
            
            //m_ptaskBg
            
            Sprite *pbg = Sprite::createWithSpriteFrameName("bosshp.png");
    
            
            
            pbg->setColor(Color3B::WHITE);
            
            
            m_pBossHpProcess = ProgressTimer::create(pbg);
            
            //m_pBossHpProcess->setScale(2);
            
            m_pBossHpProcess->setPosition(m_WinSize.width*0.75,m_WinSize.height*0.94);
            
            
            
            m_pBossHpProcess->setBarChangeRate(Vec2(1,0));
            
            m_pBossHpProcess->setType(ProgressTimer::Type::BAR);
            
            m_pBossHpProcess->setMidpoint(Vec2(0,0.5));
            
            m_pBossHpProcess->setPercentage(100);
            
            m_pBossHpProcess->setVisible(false);
            m_layer->addChild(m_pBossHpProcess,12);
            
            
            
            
            //白色条目
            Sprite *pBossHp = Sprite::createWithSpriteFrameName("bosshp2.png");
           
            
            m_pWhiteBossBg = ProgressTimer::create(pBossHp);
            
            
            m_pWhiteBossBg->cocos2d::Node::setPosition(m_WinSize.width*0.75,m_WinSize.height*0.94);
            
            //m_pWhiteBossBg->setScale(2);
            
            m_pWhiteBossBg->setBarChangeRate(Vec2(1,0));
            
            m_pWhiteBossBg->setType(ProgressTimer::Type::BAR);
            
            m_pWhiteBossBg->setMidpoint(Vec2(0,0.5));
            
            m_pWhiteBossBg->setPercentage(100);
            
            m_pWhiteBossBg->setVisible(false);
            
            m_layer->addChild(m_pWhiteBossBg,11);
            
            
            m_pWhiteBossBg->setVisible(false);
            
            
            
            break;
        }
            
        case eLevelCondition_protect://如果是护送，则初始化护送怪物
            addMonsterWithIdAndGroupAndPosition(CFightData::getInstance()->getTheBeProtectedInfo(),eFightGroup_Freind,Vec2(Start3DPOINT, (ENDZPOINT-STARTZPOINT)/2+STARTZPOINT),eAttackType_walker);
            break;
            
        default:
            break;
    }
    
    
    if (target!=eLevelCondition_TowerDefense) {
        fighterInit();
    }
    
    
    std::vector<mapFightInfo>* TheMapInfo = CFightData::getInstance()->getTheMapFightInfo();//根据地图信息初始化
    
    for (std::vector<mapFightInfo>::iterator ite = TheMapInfo->begin(); ite!=TheMapInfo->end(); ite++) {
        
        //        FighterInfo temp;
        
        //        CDataManager::CalcFighterInfoWith(&temp, (*ite)->monsterId, (*ite)->monsterLevel, 1, 0,false,1,1,1,1);
        addMonsterWithIdAndGroupAndPosition(&(*ite).theFightInfo, (*ite).TheMapInfo->buildingGroup, (*ite).TheMapInfo->pos,(*ite).TheMapInfo->buildingType);
        
    }
    //初始化装饰物，测试用
    
    float offset = 140;
    
    //    for (int j=0; j<1; j++) {
    //        for (int i=0; i<30; i++) {
    //
    //            Sprite3D *tree = Sprite3D::create("models/gameScene/songshu.c3b");
    //            auto s = Director::getInstance()->getWinSize();
    //
    //            int theflag = i%2;
    //
    //            tree->setPosition3D(Vec3(i*offset+700+j*offset*0.5f, 0,-740-theflag*offset));
    //            tree->setScale(7);
    //
    //            layer3D->addChild(tree);
    //
    //
    //            CFightManager::addCamera(tree);
    //
    //        }
    //    }
}

void CFightScene::fightStartAnimateInit(){
    
    
    if (m_ePVPMode==EPVPMode_REC) {
        
        initGameDate(NULL);
        return;
    }
    
    
    float BGMoveTime = 0.3f;  //背景的移动时间
    
    
    float fTime = 4.7;
    
    CallFunc* gameInit = CallFunc::create(CC_CALLBACK_0(CFightScene::detectSdtory, this));
    
    LayerColor* m_pBgLayer = LayerColor::create(Color4B(0, 0, 0, 125));
    m_layer->addChild(m_pBgLayer, 10);
    m_pBgLayer->setPosition(0,0);
    
    
    if(CFightData::getInstance()->getFightType() == FIGHT_PVP_TYPE)
    {
        m_pBgLayer->runAction(Sequence::create(DelayTime::create(3), FadeOut::create(0.15f),gameInit,RemoveSelf::create(), NULL));
    }
    else
    {
        m_pBgLayer->runAction(Sequence::create(DelayTime::create(fTime), FadeOut::create(0.15f),gameInit,RemoveSelf::create(), NULL));
    }
    
    Sprite* fightLeftBg = Sprite::createWithSpriteFrameName("fight_kuang.png");
    fightLeftBg->setScale(2);
    m_layer->addChild(fightLeftBg,10);
    fightLeftBg->setAnchorPoint(Vec2(1,0.5));
    fightLeftBg->setPosition(Vec2(-m_winSize.width * 0.5, m_winSize.height / 2));
    fightLeftBg->getTexture()->setAliasTexParameters();
    
    EaseSineIn* actionLeft = EaseSineIn::create(MoveTo::create(BGMoveTime, Vec2(m_winSize.width * 0.5f + 1 , m_winSize.height / 2)));
    fightLeftBg->runAction(Sequence::create(actionLeft,  DelayTime::create(1),   RemoveSelf::create(),NULL));
    
    Sprite* fightRightBg = Sprite::createWithSpriteFrameName("fight_kuang.png");
    fightRightBg->setScale(2);
    m_layer->addChild(fightRightBg,10);
    fightRightBg->setAnchorPoint(Vec2(0,0.5));
    fightRightBg->setFlippedX(true);
    fightRightBg->setPosition(Vec2(m_winSize.width * 1.5f, m_winSize.height / 2));
    fightRightBg->getTexture()->setAliasTexParameters();
    EaseSineIn* actionRight = EaseSineIn::create(MoveTo::create(BGMoveTime, Vec2(m_winSize.width * 0.5f - 1 , m_winSize.height / 2)));
    fightRightBg->runAction(Sequence::create(actionRight,  DelayTime::create(1),  RemoveSelf::create(), NULL));
    
    
    if(CFightData::getInstance()->getFightType() == FIGHT_PVP_TYPE)
    {
        fightLeftBg->setVisible(false);
        fightRightBg->setVisible(false);
        
    }
    //任务目标移动到的位置点
    
    Vec2 movePos ;
    Vec2 movePosLable;
    
    if(CFightData::getInstance()->getTheLevelCondition()==eLevelCondition_TowerDefense){
        movePos = Vec2(m_WinSize.width*0.69,m_WinSize.height*0.95);
        movePosLable = Vec2(m_WinSize.width*0.63,m_WinSize.height*0.95);
    }else
    {
        movePos = Vec2(m_WinSize.width*0.75,m_WinSize.height*0.94);
        movePosLable = Vec2(m_WinSize.width*0.71,m_WinSize.height*0.94);
    }
    
    if(CFightData::getInstance()->getFightType() != FIGHT_PVP_TYPE)
    {
        
        CallFunc* pTaskAct = CallFunc::create(CC_CALLBACK_0(CFightScene::addTaskBgAnimation, this));
        Spawn* spnTask = Spawn::create(MoveTo::create(0.2, movePos), ScaleTo::create(0.2f, 1.0), NULL);
        
        m_ptaskBg->setScale(1.5f);
        m_ptaskBg->runAction(Sequence::create(DelayTime::create(0.3f),Show::create(),
                                              DelayTime::create(1), spnTask, pTaskAct,NULL));
        
        m_pTaskLable->setScale(1.5f);
        
        
        m_pTaskLable->runAction(Sequence::create(DelayTime::create(0.3f),Show::create(),
                                                 DelayTime::create(1), ScaleTo::create(0.2f, 1.0),
                                                 
                                                 CallFunc::create([=]{
            
            LevelCondition target = CFightData::getInstance()->getTheLevelCondition();
            
            
            if(target == eLevelCondition_killBoss)
            {
                
                Sprite *p1 =  (Sprite*)m_ptaskBg->getChildByTag(100);
                
                Sprite *p2 = (Sprite*)m_ptaskBg->getChildByTag(101);
                
                
                
                
                p1->setGlobalZOrder(101);
                p2->setGlobalZOrder(102);
                
                
                m_pTaskLable->setGlobalZOrder(101);
                m_pBossHpProcess->setVisible(true);
                m_pWhiteBossBg->setVisible(true);
            }
            
            
        }), NULL));
        
    }
    
    
    Animation * hurtAnimation = Animation::create();
    
    
    
    for (int i=1; i<5; i++)
    {
    
        __String *tempStr = __String::createWithFormat("daojishi%d.png",i);
        
        Sprite* theSprite = Sprite::createWithSpriteFrameName(tempStr->getCString());
        
        theSprite->setScale(2);
        
        theSprite->setPosition(Vec2(m_WinSize.width*0.5f, m_WinSize.height*0.5f));
        
        addChild(theSprite,1000);
        
        //        theSprite->setVisible(false);
        
        if (i<4) { //前3个数字的话
            
            Sprite * goBG = Sprite::createWithSpriteFrameName("daojishiBG.png");  //数字底下的红色底
            
            goBG->setScale(0.6);
            
            goBG->setPosition(Vec2(theSprite->getBoundingBox().size.width*0.25f, theSprite->getBoundingBox().size.height*0.25f));
            theSprite->addChild(goBG,-1);
            
            // SimpleAudioEngine::getInstance()->playEffect("music/BattleStartYesBtn.mp3");
            
            
            theSprite->setScale(0);
            
            float fTime =  0.65f;
            
            if(CFightData::getInstance()->getFightType() == FIGHT_PVP_TYPE)
            {
                m_pvpCloseMenu->setEnabled(false);
                
                if(i==1)
                {
                    fTime = 0.0f;
                }
                theSprite->runAction(Sequence::create(DelayTime::create(fTime*(i-1)),ScaleTo::create(0.15, 2),CallFunc::create([=]{
                    SimpleAudioEngine::getInstance()->playEffect("music/BattleStartYesBtn.mp3");
                    
                }),DelayTime::create(0.5f),RemoveSelf::create(), NULL));
                
            }
            else
            {
                
                
                theSprite->runAction(Sequence::create(DelayTime::create(fTime*i+1.3),ScaleTo::create(0.15, 2),CallFunc::create([=]{
                    SimpleAudioEngine::getInstance()->playEffect("music/BattleStartYesBtn.mp3");
                    
                }),DelayTime::create(0.5f),RemoveSelf::create(), NULL));
            }
            
            
        }else{
            
            ///SimpleAudioEngine::getInstance()->playEffect("music/5375.wav");
            
            theSprite->setVisible(false);
            theSprite->setScale(4);
            
            float ft = 1.3;
            
            
            int t = i;
            if(CFightData::getInstance()->getFightType() == FIGHT_PVP_TYPE)
            {
                ft = 0;
                t = i-1;
            }
            
            
            theSprite->runAction(Sequence::create(DelayTime::create(0.65f*t+ft),Show::create(),ScaleTo::create(0.2, 2),CallFunc::create([=]{
                
                if((CDataManager::getEndedBy(GUIDE_USE_MANGONEL) || (CFightData::getInstance()->getNowLevelId() >= 15)
                    ||CFightData::getInstance()->getFightType() == FIGHT_PVP_TYPE)&&m_ePVPMode!=EPVPMode_REC)
                {
                    m_pSwitchItem1->setVisible(true);
                    if(m_pvpCloseMenu)
                    {
                        m_pvpCloseMenu->setEnabled(true);
                    }
                }
                SimpleAudioEngine::getInstance()->playEffect("music/go.mp3");
                m_bMenuValid = true;
            }),DelayTime::create(0.1f),CallFunc::create([=]{
                
                resetOriginPos();
                
        

        
                
               
                
                m_bJishiStart = true;
                
                //Vec3  tZhu =
                
              if (CFightData::getInstance()->getFightType() == FIGHT_PVP_TYPE)
              {
                
                  
                //CFightData::getInstance()->setIsPvpStart(1);
              }
                
            }),RemoveSelf::create(), NULL));
            
        
        }
        
        
    }
    //    hurtAnimation->setDelayPerUnit(0.5f);
    //
    //    Sprite* fightStart = Sprite::create("fight/1.png");
    //    m_layer->addChild(fightStart,10);
    //    fightStart->setVisible(false);
    //
    //    fightStart->setPosition(Vec2(m_WinSize.width*0.5f, m_WinSize.height*0.5f));
    //
    //    Animate *temp = Animate::create(hurtAnimation);
    //
    //
    //    fightStart->runAction(Sequence::create(DelayTime::create(1),Show::create(), temp,RemoveSelf::create(), nil));
    
    
    //    Sprite* fightStart = Sprite::createWithSpriteFrameName("fight_start.png");
    //    fightStart->setScale(1.8f);
    //    fightStart->setPosition(Vec2(m_WinSize.width * 1.2f, m_WinSize.height * 0.5f));
    //    m_layer->addChild(fightStart, 10);
    //
    //    EaseSineIn* actionStart = EaseSineIn::create(MoveTo::create(0.3f, Vec2(m_WinSize.width / 2, m_WinSize.height * 0.5f)));
    //    fightStart->runAction(Sequence::create(DelayTime::create(1.5f), actionStart,
    //                                           DelayTime::create(0.5f), FadeOut::create(0.3f), RemoveSelf::create(), NULL));
    
    
    
    
    
    
    
    
    
}







// MARK: TDFight

int CFightScene::buildingPosSelect(cocos2d::Vec2 *targetPos){
    
    //建筑放下的位置。一般四舍五入为就近的网格。网格坐标以x轴间隔40，y轴间隔40
    
    
    float posX = (targetPos->x-(Start3DPOINT-0.5*ONESIDE))*1.0/BUILDING_POS_OFFSET_X;
    
    float posY = (targetPos->y-STARTZPOINT)*1.0/BUILDING_POS_OFFSET_Y;
    
    
    int realX = int(posX+0.5f);
    
    int realY = int(-posY+0.5f);
    
    
    targetPos->x = realX*BUILDING_POS_OFFSET_X+(Start3DPOINT-0.5*ONESIDE);
    
    targetPos->y = -realY*BUILDING_POS_OFFSET_Y+STARTZPOINT;
    
    
    
    pathPos* target = CFightData::getInstance()->findPathPosByPos(Vec2(targetPos->x, targetPos->y));
    
    if (target->availible==false) {  //有建筑了则返回1
        
        Sprite* temp = CFightManager::getTheFlyWord("此处已有建筑，请重新挑选位置~","fonts/cuti.ttf");
        temp->setPosition(Vec2(m_WinSize.width*0.5f, m_WinSize.height*0.7f));
        addChild(temp);
        
        return 1;
    }
    
    //如果添加建筑以后，怪物出生地的权重大于了999，则返回2
    CFightData::getInstance()->addPos(Vec2(realX, realY));
    if (CFightData::getInstance()->ifBlockRoad()) {
        CFightData::getInstance()->removePos(Vec2(realX, realY));
        
        Sprite* temp = CFightManager::getTheFlyWord("此处建造建筑将会封闭道路，请重新挑选位置~","fonts/cuti.ttf");
        temp->setPosition(Vec2(m_WinSize.width*0.5f, m_WinSize.height*0.7f));
        addChild(temp);
        return 2;
    }
    
    
    
    
    
    return 0;
}


void CFightScene::OnCallBackTDHelp(cocos2d::Ref *pSender)
{
    Director::getInstance()->pause();
    CommonPropLayer* layer = CommonPropLayer::create(TOWER_MODE_INTRODUCE);
    addChild(layer, 100);
}

void CFightScene::OnCallBackClockOver(cocos2d::Ref *pSender){
    
    if(((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_TOWER_LEVEL)) && (guiderManagerLayer::getInstance()->getStepNum() == 5))
    {
        guiderManagerLayer::getInstance()->removeGuiderLayer();
        m_bUpdateTime = true;
    }
    TDMonsterStart();
    
}

void CFightScene::updateTD(float time){
    
    
    __String* tempStr = __String::create("");
    
    tempStr->initWithFormat("%d",CFightData::getInstance()->getStone());
    m_pStoneLable->setString(tempStr->getCString());
    
    //    tempStr->initWithFormat("%d",CFightData::getInstance()->getNowLife());
    //    m_pLifeLable->setString(tempStr->getCString());
    //
    //    tempStr->initWithFormat("%d",CFightData::getInstance()->getNowLife());
    //    m_pLifeLable->setString(tempStr->getCString());
    
    
    if (m_pBuildingControll->isVisible()) {
        m_pBuildingControll->buttonRefresh();
    }
    
    
    m_fNowWaveTime-=time;
    
    
    float tempWaveTime = TD_WAVE_TIME;
    
    int roundNum = CFightData::getInstance()->getNowRound();
    
    if (roundNum%5==0&&roundNum!=0) {
        tempWaveTime = TD_BOSSWAVE_TIME;
    }
    
    
    
    m_pTheClock->setPercentage(m_fNowWaveTime/tempWaveTime *92);  //因为闹钟的头会挡住一部分进度条，因此稍微处理下,让进度条直接从闹钟头开始
    
    
    if ((m_fNowWaveTime<=0) && (!m_bGuideBlow)) {//出怪的判断
        TDMonsterStart();
    }
    
    
    
    
}

void CFightScene::gotTheSton(int stoneNum,cocos2d::Vec3 thePos){
    
    //获得钱
    Sprite* icon = Sprite::createWithSpriteFrameName("defenseCoin1.png");
    addChild(icon,1000);
    
    
    
    Vec2 theScreenPos = CommonData::GetSingletonPtr()->getPosOnScreen(_camera,thePos)+Vec2(-icon->getBoundingBox().size.width, 0);
    icon->setPosition(theScreenPos);
    
    Sprite* plus = Sprite::createWithSpriteFrameName("TD_Plus.png");
    plus->setPosition(Vec2(icon->getBoundingBox().size.width+plus->getBoundingBox().size.width*0.5f, icon->getBoundingBox().size.height*0.5f));
    icon->setScale(0);
    
    icon->addChild(plus);
    
    
    char buf[50] = {0};
    
    auto* pAnimation = Animation::create();
    for (int i = 1; i <9; i++)
    {
        sprintf(buf, "defenseCoin%d.png", i);
        pAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(buf));
    }
    
    pAnimation->setDelayPerUnit(0.1f);
    Animate* pAction = Animate::create(pAnimation);
    icon->runAction(Sequence::create(Spawn::create(EaseSineOut::create(MoveBy::create(0.5f, Vec2(0, 100))),EaseSineOut::create(ScaleTo::create(0.5,1)),pAction,NULL),NULL));

    
    __String * tempStr = __String::createWithFormat("  %d",stoneNum);
    
    Label* theFly = Label::createWithCharMap("fonts/TD_StoneNum.png", 34, 46, '0');
    theFly->setString(tempStr->getCString());
    theFly->setPosition(Vec2(plus->getPositionX()+theFly->getBoundingBox().size.width*0.3f, 0)+icon->getPosition());
    addChild(theFly,1000);
    theFly->runAction(Sequence::create(EaseSineOut::create(MoveBy::create(0.5f, Vec2(0, 100))),NULL));
    
    runAction(Sequence::create(DelayTime::create(0.7f),CallFunc::create([=](){
    

        icon->removeFromParent();
        
        theFly->removeFromParent();
        
    }),NULL));
    
    
}

void CFightScene::TDMonsterStart(){
    
    
    
    
    
    int nowRound = CFightData::getInstance()->getNowRound();
    
    m_fNowWaveTime = 99999;//出怪时不记录当前波时间
//    resetTDTime();
    

    if (nowRound>=10)
    {
        return;
    }
    
    CFightData::getInstance()->addRound(1);
    nowRound = CFightData::getInstance()->getNowRound();
    
    
    __String* tempStr  = __String::createWithFormat("波数: %d/10",nowRound);
    
    m_pTrunLable->setString(tempStr->getCString());
    
    
    //每回合开始，都会加上30点能量,最后一波怪物将会加2500点能量
    if(nowRound<10){
        
        CFightData::getInstance()->addStone(30);
        
    }else{
        CFightData::getInstance()->addStone(2500);
    }
    
//    CFightData::getInstance()->addStone(50*CFightData::getInstance()->getTheLevelInfo()->monsterLevel);
    
    
    m_iNowMonsterNum = 0;
    
    
    m_pTheClock->setVisible(false);
    m_pClockButton->setVisible(false);
    
    
    if (nowRound%5!=0) {
        schedule(schedule_selector(CFightScene::addTDmonster), 0.8,20,0);
    }else{
        scheduleOnce(schedule_selector(CFightScene::addTDmonster), 1);
    }
    
    
    
    
}

void CFightScene::resetTDTime(){
    
    int nowRound = CFightData::getInstance()->getNowRound();
    
    if(nowRound%5==0&&nowRound!=0){
        
        m_fNowWaveTime = TD_BOSSWAVE_TIME;
        
    }else{
        m_fNowWaveTime = TD_WAVE_TIME;
    }
    
    
}



void CFightScene::addTDmonster(float time){
    
    m_iNowMonsterNum++;
    
    int nowRound  = CFightData::getInstance()->getNowRound();
    
    //测试用
    //nowRound = 4;
    
    FighterInfo* target = CFightData::getInstance()->getTDFighterInfoByTurn(nowRound-1);
    
    int endWave = 20;
    
    if (nowRound%5==0)
    {
        endWave=1;
        addMonsterWithIdAndGroupAndPosition(target,eFightGroup_Enemy,Vec2(END3DPOINT, (ENDZPOINT-STARTZPOINT)/2+STARTZPOINT),eAttackType_monster,false,false);
    }else
    {
        int randZ = random(ENDZPOINT,STARTZPOINT);
        addMonsterWithIdAndGroupAndPosition(target, eFightGroup_Enemy, Vec2(END3DPOINT, randZ));
    }
    
    
    if (m_iNowMonsterNum==endWave&&m_iNowMonsterNum!=10&&nowRound<10)
    {
        m_pTheClock->setVisible(true);
        m_pClockButton->setVisible(true);
        resetTDTime();
    }
    
    
    if (m_iNowMonsterNum==endWave==1&&nowRound==10) {
        CFightData::getInstance()->addRound(1);
    }
    
}

void CFightScene::soliderCallBack(Ref * sender, cocos2d::extension::Control::EventType controlEvent){
    
    
    auto slide_control = (cocos2d::extension::ControlSlider*)sender;//通过回调参数sender 获得ControlSlider‘
    
    int iCurrent_value = slide_control->getValue();//获取slide当前的值
    
    
    float nowY = MIN_CAMERA_Y + iCurrent_value;
    float nowZ = MIN_CAMERA_Z + iCurrent_value;
    
    moveCameraPos(_camera->getPositionX(),nowY,nowZ);
    //    _camera->setPositionY(nowY);
    //    _camera->setPositionZ(nowZ);
    
    
    CCLOG("cameraY::%f",nowY);
    
    CCLOG("cameraZ::%f",nowZ);
    
}

void CFightScene::callBackZoomIN(cocos2d::Ref *Sender){
    
    
    int iCurrent_value = m_pZommControl->getValue();//获取slide当前的值
    
    float change = (MAX_CAMERA_Y-MIN_CAMERA_Y)*0.01f;
    
    m_pZommControl->setValue(iCurrent_value-change);
    
}

void CFightScene::callBackZoomOut(cocos2d::Ref *Sender){
    
    int iCurrent_value = m_pZommControl->getValue();//获取slide当前的值
    
    float change = (MAX_CAMERA_Y-MIN_CAMERA_Y)*0.01f;
    
    m_pZommControl->setValue(iCurrent_value+change);
    
    
}

void CFightScene::setOriginCameraPos(cocos2d::Vec3 originPos){
    
    m_vec3OriginCameraPos = originPos;
    
    moveCameraPos(originPos.x,originPos.y,originPos.z);
    
}


void CFightScene::moveCameraPos(float x,float y,float z){
    
    if (!_camera) {
        return;
    }
    
    if(x!=-999){
        _camera->setPositionX(x);
    }
    if(y!=-999){
        _camera->setPositionY(y);
    }
    if(z!=-999){
        _camera->setPositionZ(z);
    }
    
    
    //移动场景粒子特效
    for (int i=0; i<m_VecSceneParticle.size(); i++) {
        
        Layer* target = m_VecSceneParticle.at(i);
        
        if (target) {
            
            float posX = (m_vec3OriginCameraPos.x-x)*(5-i)/5;     //i越低得，受影响范围越大
            //            float posY = -(m_vec3OriginCameraPos.y-y)*(5-i)/5;
            //            float posZ = -(m_vec3OriginCameraPos.z-z)*(5-i)/5;
            
            target->setPositionX(posX);
        }
        
    }
    
    //移动场景png特效
    for (int i=0; i<m_vecScenePng.size(); i++) {
        
        Layer* target = m_vecScenePng.at(i);
        
        if (target) {
            
            float posX = (m_vec3OriginCameraPos.x-x)*(5-i)/5;     //i越低得，受影响范围越大
            //            float posY = -(m_vec3OriginCameraPos.y-y)*(5-i)/5;
            //            float posZ = -(m_vec3OriginCameraPos.z-z)*(5-i)/5;
            
            target->setPositionX(posX);
        }
        
    }
}

void CFightScene::EndCameraUpdate(float time){
    
    if (Director::getInstance()->getScheduler()->getTimeScale()>=1) {
        Director::getInstance()->getScheduler()->setTimeScale(0.2f);
    }
    
    m_iTouchExitState = 1;
    
    CBaseFighter* endTarget = CFightData::getInstance()->getEndTarget();
    
    if (!endTarget) {
        return;
    }
    
    
    
    
    int offsetY = 200; //摄像机位置和死亡目标位置的间隔
    int offsetZ = 200;
    
    
    
    
    //    if ((_camera->getPositionX()-endTarget->getPositionX())*_vec3DeadSpead.x>=0) {
    //        
    //        _camera->lookAt(endTarget->getPosition3D()+Vec3(0, endTarget->getBoundingBox().size.height*0.5f, 0));
    //        
    //        unschedule(schedule_selector(CFightScene::EndCameraUpdate));
    //    }
    
    //第一个阶段，移动x轴
    float cameraPosX = abs(_camera->getPositionX()-endTarget->getPositionX())<abs(_vec3DeadSpead.x*time)? endTarget->getPositionX(): _camera->getPositionX()+_vec3DeadSpead.x*time;
    moveCameraPos(cameraPosX);
    
    
    if ((_camera->getPositionX()-endTarget->getPositionX())*_vec3DeadSpead.x>=0) { //第二个阶段，移动y轴，z轴
        
        
        _vec3DeadSpead.y += _accelerationDiedY*time;
        _vec3DeadSpead.z += _accelerationDiedZ*time;
        
        _vec3DeadSpead.y = MIN(_vec3DeadSpead.y, 0);  //最大的速度也是负的
        _vec3DeadSpead.z = MIN(_vec3DeadSpead.z, 0);
        
        //移动摄像机
        float cameraPosY = abs(_camera->getPositionY()-offsetY-endTarget->getPositionY())<abs(_vec3DeadSpead.y*time)? endTarget->getPositionY()+offsetY: _camera->getPositionY()+ _vec3DeadSpead.y*time;
        
        float cameraPosZ = abs(_camera->getPositionZ()-offsetZ-endTarget->getPositionZ())<abs(_vec3DeadSpead.z*time)? endTarget->getPositionZ()+offsetZ: _camera->getPositionZ()+ _vec3DeadSpead.z*time;
        
        moveCameraPos(_camera->getPositionX(),cameraPosY,cameraPosZ);
        
        
        //摄像机的视点也移动
        
        //        Vec3 targetDirection = endTarget->getPosition3D()+Vec3(0, endTarget->getBoundingBox().size.height*0.5f, 0) -_camera->getPosition3D();
        
        Vec3 originRotation = _camera->getRotation3D();
        
        //        CommonData::changeRotation3DWithVec(Vec3(0, 1, 0), targetDirection, _camera);
        
        _camera->lookAt(endTarget->getPosition3D()+Vec3(0, endTarget->getBoundingBox().size.height*0.5f, 0));
        
        Vec3 targetRotation = _camera->getRotation3D();
        
        
        Vec3 offsetRotation = (targetRotation-originRotation)*0.1f;
        
        _camera->setRotation3D(originRotation+offsetRotation);
        
    }
    
    if ((_camera->getPositionY()-endTarget->getPositionY())*_vec3DeadSpead.y>=0) {  //第三阶段，结束
        unschedule(schedule_selector(CFightScene::EndCameraUpdate));
    }
    
    
    
    
    
    
    //    CCLOG("cameraPos:%f,%f,%f--------targetPos:%f,%f,%f",cameraPosX,cameraPosY,cameraPosZ,endTarget->getPositionX(),endTarget->getPositionY(),endTarget->getPositionZ());
    
    
    //    float cameraPosX = _camera->getPositionX()+offsetMaxX*direction;
    
    
    //    if(cameraPosX<Start3DPOINT)
    //    {
    //        cameraPosX=Start3DPOINT;
    //    }
    //    else if(cameraPosX>(m_WinSize.width-m_pCurView->boundingBox().size.width*0.5)/m_WinSize.width*BIGMAPLEN + Start3DPOINT-ONESIDE) //此处为移动小地图时的边界判断
    //    {
    //        cameraPosX=(m_WinSize.width-m_pCurView->boundingBox().size.width*0.5)/m_WinSize.width*BIGMAPLEN + Start3DPOINT-ONESIDE;
    //    }
    
    
    
    //    _camera->setPositionX(cameraPosX);
    //    moveCameraPos(cameraPosX);
    
    
}



