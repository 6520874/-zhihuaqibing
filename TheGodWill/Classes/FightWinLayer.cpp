//
//  FightWinLayer.cpp
//  TheGodWill
//
//  Created by prefer_shan on 14/12/18.
//
//

#include "FightWinLayer.h"
#include "LayerManager.h"
#include "PhysicalSystem.h"
#include "LYFightData.h"
#include "LYPersonalData.h"
#include "commonPropLayer.h"
#include "DataManager.h"
#include "DBMgr.h"
#include "guiderManagerLayer.h"
#include "getNewHeroLayer.h"
#include "LYBeatNumbers.h"
#include "CSharedPlist.h"
#include "NetWorkDataSingle.h"
#include "TDCCVirtualCurrency.h"
#include "zzbNewNetStruct.h"
#include "zzbNetControlModule.h"

#define MY_PERSION CPersonalData::getInstance()
USING_NS_CC;


CFightWinLayer::CFightWinLayer()
{
    m_winSize = Director::getInstance()->getWinSize();
    m_iGotRare = 0;
    m_iGotGold = 0;
    m_iGotExp = 0;
    m_pMenu = NULL;
    m_bOnce = true;
    m_pSucess = NULL;
    bValid = false;
    m_bWupin = false;
    m_bJiesuan= false;
    before = 0;
    after = 0;
    m_iStartNum = 0;
    m_monsterId = 0;
    m_pExpBeatNum = NULL;
    m_pKillBetaCoin = NULL;
    m_pNumber = NULL;
    m_pNextItem = NULL;
    m_pBackItem = NULL;
    m_pAgainItem = NULL;
    
    m_iTime = 0;
    m_iTime2 = 0;
    m_iTime3 = 0;
    m_reSmallLevel = 0;
    CSharedPlist::getInstance()->pushPlist("plist/propIcon_1.plist");
//    CSharedPlist::getInstance()->pushPlist("plist/fight.plist");
    CSharedPlist::getInstance()->pushPlist("plist/battleResult.plist");
}


CFightWinLayer::~CFightWinLayer()
{
    CSharedPlist::getInstance()->erasePlist("plist/propIcon_1.plist");
    CSharedPlist::getInstance()->erasePlist("plist/battleResult.plist");
}


bool CFightWinLayer::init()
{
    //createWithNumberAndSize
 
    CBaseLayer::initWithColor(Color4B(0, 0, 0, 255*0.8));
    listen = EventListenerTouchOneByOne::create();
    listen->setSwallowTouches(true);
    listen->onTouchBegan = CC_CALLBACK_2(CFightWinLayer::onTouchBegan, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listen, this);

    Director::getInstance()->resume();
    SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    SimpleAudioEngine::getInstance()->playEffect("music/victory.mp3");
    
    m_reSmallLevel = CPersonalData::getInstance()->getUserInfo()->m_iSmallLevel;
    if(CPersonalData::getInstance()->getlevelMode()==1)
    {
        m_reSmallLevel = CPersonalData::getInstance()->getUserInfo()->m_iEqliteSmallLevel;
    }
    if(CFightData::getInstance()->getNowMidLevelId() >= TOWER_LEVEL_INDEX)
    {
        initTowerData();
    }
    else
    {
        initData();
    }
    
    if(CFightData::getInstance()->getFightType() == FIGHT_PVP_TYPE)
    {
        initUI();
        if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_PVP_ROB) && (guiderManagerLayer::getInstance()->getStepNum() == 6))
        {
            guiderManagerLayer::getInstance()->setStepNum(guiderManagerLayer::getInstance()->getStepNum() + 1);
            guiderManagerLayer::getInstance()->createLayer(this, GUIDE_SHOW_FINGER, m_guidePos);
        }

        return true ;
    }
    
    before= CPersonalData::getInstance()->getUserInfo()->m_iUserLevel;
    int userExp = CPersonalData::getInstance()->getUserInfo()->m_iUserExp + m_iGotExp;
    after = before;
    int addStamina = 0;
    for (int i = before; i < 100; i++) {
        int temp = CDataManager::getUpgradeInfoByLevel(i + 1)->userExp;
        if (userExp < temp)
        {
            after = i;
            break;
        }
        else{
            addStamina += CDataManager::getUpgradeInfoByLevel(i)->upgradeStamina;
        }
    }
    judgeIfUpgrade(m_iGotExp);
    if(CPersonalData::getInstance()->getlevelMode()==0)
    {
        m_monsterId = judgeIfGetNewHero();
    }
    
    
    AddSucessDonghua();
    
    //talkingdata
    int nowMidLevel = CFightData::getInstance()->getNowLevelId();
    int nowSmallLevel = CFightData::getInstance()->getNowMidLevelId();
    
    __String* tempStr = __String::createWithFormat("%d-%d",nowMidLevel,nowSmallLevel);
    TDCCMission::onCompleted(tempStr->getCString());
    
    int midLevel = CFightData::getInstance()->getNowMidLevelId();
    midLevelInfo* pInfo = CDataManager::getMidLevelInfoById(midLevel);
    if((CFightData::getInstance()->getNowLevelId() > m_reSmallLevel) && (pInfo->lastLevel == CFightData::getInstance()->getNowLevelId()) && (CPersonalData::getInstance()->getlevelMode()==0))
    {
        CPersonalData::getInstance()->setShowMap(true);
    }
    
    return true;
}

void CFightWinLayer::backPvpMainLayer(Ref* pSender)
{
    if(CPersonalData::getInstance()->getState() == 0)
    {
        CLayerManager::GetSingletonPtr()->createLayerWithLoading(eLayer_PVPLayer, NULL,false);
//        CLayerManager::GetSingletonPtr()->createLayer(eLayer_PVPLayer, NULL,false);
    }
}

void CFightWinLayer::addShowGuide()
{
    LYMenuItemSprite* temp = (LYMenuItemSprite*)m_pMenu->getChildByTag(10);
    if(temp && (CPersonalData::getInstance()->getUserInfo()->m_iSmallLevel == 1) && (CFightData::getInstance()->getNowLevelId() == 1))
    {
        Vec2 pps = temp->convertToWorldSpaceAR(Vec2::ZERO);
        guiderManagerLayer::getInstance()->setStepNum(1);
        guiderManagerLayer::getInstance()->createLayer(this, GUIDE_SHOW_FINGER, pps);
    }
}

void CFightWinLayer::judgeIfUpgrade(int exp)
{
    if(CFightData::getInstance()->getFightType() == FIGHT_PVP_TYPE)
    {
        return ;
    }
    int userLevel= CPersonalData::getInstance()->getUserInfo()->m_iUserLevel;
    int userExp = CPersonalData::getInstance()->getUserInfo()->m_iUserExp + exp;
    int index = userLevel;
    int addStamina = 0;
    for (int i = userLevel; i < 100; i++) {
        int temp = CDataManager::getUpgradeInfoByLevel(i + 1)->userExp;
        if (userExp < temp)
        {
            index = i;
            break;
        }
        else{
            addStamina += CDataManager::getUpgradeInfoByLevel(i)->upgradeStamina;
        }
    }
    
    //根据难度获取扣除体力，
    bool ifHero=false;
    if(CPersonalData::getInstance()->getlevelMode()==1){
        ifHero = true;
    }
    int costStamina = CFightData::getInstance()->getTheLevelInfo()->stamina;
    if (ifHero)
    {
        costStamina=2*costStamina;
    }
    m_iGotGold = CFightData::getInstance()->getTheLevelInfo()->goldNum;
    if(ifHero)
    {
        m_iGotGold= 2*m_iGotGold;
    }
    
    CDataManager::saveUserInfo(CPersonalData::getInstance()->GetPhysicalNum() + addStamina-costStamina, index, userExp);
    CNetWorkDataSingle::GetSingletonPtr()->SaveUserMCF(this,menu_selector(CFightWinLayer::OnUpdateStoneAndCoin),m_iGotGold + CFightData::getInstance()->getAddCoin(),0,addStamina-costStamina);
    
    
    if (index > userLevel)
    {
        CDataManager::saveTaskInfoToDB(11, index, true);
        CPersonalData::getInstance()->setJudgeUpgrade(true);
    }
    else{
        CPersonalData::getInstance()->setJudgeUpgrade(false);
    }

}


void CFightWinLayer::OnPvpOK(Ref *pSender)
{
    Director::getInstance()->resume();
    
    int iStone = CFightData::getInstance()->getTheRobStone();
    CPersonalData::getInstance()->setUserKuangStone(CPersonalData::getInstance()->getUserKuangStone() + iStone);
    //CNetWorkDataSingle::GetSingletonPtr()->SaveUserMCF(this,menu_selector(CFightWinLayer::OnUpdateStoneAndCoin),0,iStone,0);
    //TDCCVirtualCurrency::onReward(iStone, "pvp PK 获得");
    if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_PVP_ROB) && (guiderManagerLayer::getInstance()->getStepNum() == 7))
    {
        guiderManagerLayer::getInstance()->removeGuiderLayer();
    }
    CNetControlModule::sharedNetControlModule()->getUserTeamInfo(this, menu_selector(CFightWinLayer::backPvpMainLayer));
}

void CFightWinLayer::showNewHero()
{
    if((!CPersonalData::getInstance()->getJudgeUpgrade()) || (CPersonalData::getInstance()->getInsertHero()))
    {
        CPersonalData::getInstance()->setInsertHero(false);
        if(m_monsterId > 0)
        {
            //显示新兵种
            CGetNewHeroLayer* pLayer = CGetNewHeroLayer::create(m_monsterId);
            addChild(pLayer,999);
            m_pMenu->setEnabled(true);
        }
    }
    
    bValid = true;
    if(m_pBackItem)
    {
        m_pBackItem->setEnabled(true);
    }
    if(m_pAgainItem)
    {
        m_pAgainItem->setEnabled(true);
    }
    
    if(m_pNextItem)
    {
        m_pNextItem->setEnabled(true);
    }
    
}

void CFightWinLayer::onExit()
{
    _eventDispatcher->removeEventListener(listen);
    CBaseLayer::onExit();
    
}

bool CFightWinLayer::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
   if(m_bWupin)
   {
       Sprite *pMidLan = (Sprite *)getChildByTag(101);
       
       if(pMidLan)
       {
           pMidLan->removeFromParent();
       }
       
       Sprite *p1 = (Sprite*) getChildByTag(102);
       if(p1)
           p1->removeFromParent();
       
       Sprite *p2 = (Sprite*) getChildByTag(103);
       if(p2)
           p2->removeFromParent();
       
       Sprite *p3 = (Sprite*) getChildByTag(104);
       if(p3)
           p3->removeFromParent();
       
       if(m_pSucess)
       {
           m_pSucess->removeFromParent();
           m_pSucess =NULL;
       }
       
       Sprite *p4 = (Sprite*)getChildByTag(105);
       if(p4)
           p4->removeFromParent();
       
       Sprite *p5 = (Sprite*)getChildByTag(106);
       if(p5)
           p5->removeFromParent();
       
       unschedule(schedule_selector(CFightWinLayer::addParticleEffect));
       m_bWupin = false;
       
       AddWupinDonghua();
   }
    
    if(m_bJiesuan)
    {
        Sprite *pMidLan = (Sprite *)getChildByTag(101);
        
        if(pMidLan)
        {
            pMidLan->removeFromParent();
        }
    
        Sprite *pBeiGuang = Sprite::createWithSpriteFrameName("backlight1.png");
        pBeiGuang->setScale(5);
        pBeiGuang->setPosition(m_winSize.width*0.5,m_winSize.height*0.9);
        addChild(pBeiGuang);
        

        //初始化显示
        if(CFightData::getInstance()->getFightType() != FIGHT_PVP_TYPE)
        {
            initUI();
            m_bJiesuan = false;
        }
    }
    return true;
}

void CFightWinLayer::onTouchEnded(Touch *touch, Event *event)
{
    if(m_bWupin)
    {
        m_bWupin = false;
    }
    
    if(m_bJiesuan)
    {
        m_bJiesuan = false;
    }
}

void CFightWinLayer::initTowerData()
{
    //根据难度获取扣除体力，
    int costStamina = CFightData::getInstance()->getTheLevelInfo()->stamina;

    CPhysicalSystem::GetSingletonPtr()->costManual(-costStamina, NULL, NULL, NULL);
    m_iGotExp = CFightData::getInstance()->getTheLevelInfo()->playerExp;
    
    //根据难度获取道具。
    std::vector<smallAwardInfo> itemsVec = CFightData::getInstance()->getTheLevelInfo()->itemsVec;
    for (int i=0; i<itemsVec.size(); i++)
    {
        bool ifDrop = CFightManager::ifDropItem(itemsVec[i].type, itemsVec[i].Id);
        if (ifDrop)
        {
            m_VecGotItems.push_back(itemsVec[i]);
            
            //物品则插入物品表
            if(itemsVec[i].type==0)
            {
                int iNum = CDBMgr::getInstance()->HavePropNumById(itemsVec[i].Id);
                CDataManager::SaveUserPropToDB(itemsVec[i].Id,iNum+1);
            }
            //碎片则插入碎片表
            if(itemsVec[i].type==1){
                CDataManager::insertIntoMonsterInfo(itemsVec[i].Id);
            }
        }
    }
}

void CFightWinLayer::initData()
{
    if(CFightData::getInstance()->getFightType() == FIGHT_PVP_TYPE)
    {
        return ;
    }
    
    bool ifHero=false;
    
    if(CPersonalData::getInstance()->getlevelMode()==1){
        ifHero = true;
    }
    
    //获取星级
    std::vector<levelStarInfo> info = CFightData::getInstance()->m_starInfoVec;
    //如果没有某一个星级id的条件则默认初始化为0
    
    int levelId = CFightData::getInstance()->getNowLevelId();
    std::vector<int> starIdVec = CDataManager::getStarConditionById(levelId);
    
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

    for (int i = 0; i < starIdVec.size(); i++) {
        starInfo* pInfo =  CDataManager::getStarInfoById(starIdVec.at(i));
        if(pInfo)
        {
            for(int j = 0; j < info.size(); j++)
            {
                if((pInfo->starType == info.at(j).starType) && (pInfo->monsterId == info.at(j).monsterId))
                {
                    if((pInfo->positive == 0) && (info.at(j).num <= pInfo->num))
                    {
                        m_iGotRare++;
                        break;
                    }
                    else if((pInfo->positive  == 1) && (info.at(j).num >= pInfo->num))
                    {
                        m_iGotRare++;
                        break;
                    }
                }
            }
        }
    }

//    //根据难度获取扣除体力，
//    int costStamina = CFightData::getInstance()->getTheLevelInfo()->stamina;
//    
//    if (ifHero)
//    {
//        costStamina=2*costStamina;
//    }
//    CPhysicalSystem::GetSingletonPtr()->costManual(-costStamina, NULL, NULL, NULL);
    
    
    //根据难度获取获得金币，

    ///CDataManager::saveUserStoneAndCoinToDB(m_iGotGold + CFightData::getInstance()->getAddCoin(), 0,true);
    //spf
    

    
    
    //更新关卡星级
    int oldRare = CDataManager::getStarBySmallLevelId(CFightData::getInstance()->getNowLevelId(), ifHero);
    CPersonalData::getInstance()->setstar(oldRare);
    if((m_iGotRare >= oldRare) || ((m_iGotRare == 0) && (oldRare == 0)))
    {
        CDataManager::saveUserLevelInfoToDB(CFightData::getInstance()->getTheLevelInfo()->levelId, m_iGotRare,ifHero);
    }
    if(m_iGotRare == 3)
    {
        CDataManager::saveTaskInfoToDB(5);//保存三星通关次数
    }
    //根据难度获取获得经验，
    m_iGotExp = CFightData::getInstance()->getTheLevelInfo()->playerExp;
    
    
    if(ifHero){
        m_iGotExp= 2*m_iGotExp;
    }
    
    //根据难度获取道具。
    std::vector<smallAwardInfo> itemsVec;
    
    if (ifHero) {
        itemsVec = CFightData::getInstance()->getTheLevelInfo()->heroItemsVec;
    }else{
        
        itemsVec = CFightData::getInstance()->getTheLevelInfo()->itemsVec;
    }
    
    for (int i=0; i<itemsVec.size(); i++) {
        
        bool ifDrop =  CFightManager::ifDropItem(itemsVec[i].type, itemsVec[i].Id);
        if (ifDrop) {
            m_VecGotItems.push_back(itemsVec[i]);
            
            //物品则插入物品表
            if(itemsVec[i].type==0)
            {
                //propInfo* pInfo = CDBMgr::getInstance()->getPropInfoById(itemsVec[i].Id);
                int iNum = CDBMgr::getInstance()->HavePropNumById(itemsVec[i].Id);
                
                CDataManager::SaveUserPropToDB(itemsVec[i].Id,iNum+1);
            }
            //碎片则插入碎片表
            if(itemsVec[i].type==1){
                CDataManager::saveUserDebrisToDB(itemsVec[i].Id, 1);
            }
        }
    }

    
//    CPersonalData::getInstance()->SetPhysicalNum(CPersonalData::getInstance()->GetPhysicalNum()-costStamina);
    
    
}


void CFightWinLayer::OnUpdateStoneAndCoin(Ref *pSender)
{

    
}



void CFightWinLayer::addStarEffect()
{
    SimpleAudioEngine::getInstance()->playEffect("music/addStar.mp3");
    ParticleSystemQuad* effect2 = ParticleSystemQuad::create("particleSystem/star2.plist");
    
    effect2->setAutoRemoveOnFinish(true);
    
    //1000
    Sprite *pBg = (Sprite *)getChildByTag(1000);

    Size szBg = pBg->getContentSize();
    

    effect2->setPosition(szBg.width*0.325+m_iStartNum*0.17*szBg.width,szBg.height*0.95);
    
    pBg->addChild(effect2,1);
    m_iStartNum++;
    
    if(m_iGotRare == m_iStartNum)
    {
      
        m_pNumber->BeatDownTo(0);
        m_pNumber->BeatUpTo(m_iGotGold);
        
        ParticleSystemQuad* effect2 = ParticleSystemQuad::create("particleSystem/beatStar.plist") ;
        
        effect2->setAutoRemoveOnFinish(true);
        effect2->setPosition(m_winSize.width*0.65,m_winSize.height*0.58);
        addChild(effect2,111);

    
        schedule(schedule_selector(CFightWinLayer::addBetaStar),0.0f);
    }
 
}

void CFightWinLayer::AddWupinDonghua()
{
    Sprite *pMidLan = Sprite::createWithSpriteFrameName("goodsdikuang.png");
    pMidLan->setPosition(m_winSize.width*0.5,m_winSize.height*0.5);
    
    pMidLan->setScale(0);
    addChild(pMidLan,0,101);
    
    Size tBg = pMidLan->getContentSize();
    
    Sprite *pGetWuPin = Sprite::createWithSpriteFrameName("getwupin.png");
    pGetWuPin->setScale(0.15);
    pGetWuPin->setPosition(tBg.width*0.3,tBg.height*0.5);
    pMidLan->addChild(pGetWuPin,1);

    SimpleAudioEngine::getInstance()->playEffect("music/pushAward.mp3");

                                                 
    for (int i=0; i<m_VecGotItems.size(); i++)
    {
        Sprite* wuping=NULL;
         
         if(m_VecGotItems[i].type==0)
         {
             wuping = CDataManager::getPropSpriteById(m_VecGotItems[i].Id, true);
             wuping->setScale(0.2);
         }
         
         else if(m_VecGotItems[i].type==1)
         {
             int id = m_VecGotItems[i].Id;
             
             wuping = CDataManager::getMonsterInfodebris(id);
             wuping->setScale(0.2*1.5);

         }
        
         if(wuping)
         {
             wuping->setPosition(tBg.width*0.5,tBg.height*0.5);
             
             pMidLan->addChild(wuping,1);
             
             CallFunc* getOff = CallFunc::create(CC_CALLBACK_0(CFightWinLayer::OnRemoveWupin, this));
             pMidLan->runAction(Sequence::create(ScaleTo::create(0.2f,10),ScaleTo::create(0.2f,12),NULL));
             runAction(Sequence::create(DelayTime::create(0.5f),getOff, NULL));
             //m_bJiesuan = true;
         }
    }
    
    if(m_VecGotItems.size()<=0)
    {
        //初始化显示
        initUI();
    }
}

void CFightWinLayer::OnRemoveSucessLayer()
{
    m_bWupin = true;
}



void CFightWinLayer::OnRemoveWupin()
{
    m_bJiesuan = true;
}



void CFightWinLayer::addParticleEffect(float ft)
{
    ParticleSystemQuad* effect2 = ParticleSystemQuad::create("particleSystem/hechengBomb.plist") ;
    effect2->setScale(1.3f);
    effect2->setAutoRemoveOnFinish(true);
    Vec2 pos = Vec2(m_winSize.width * CCRANDOM_0_1(), m_winSize.height * CCRANDOM_0_1());
    effect2->setPosition(pos);
    addChild(effect2,111);
    
    
    effect2 = ParticleSystemQuad::create("particleSystem/hechengBomb.plist");
    effect2->setScale(1.3f);
    effect2->setAutoRemoveOnFinish(true);
    effect2->setPosition(pos);
    addChild(effect2,111);
    
    effect2 = ParticleSystemQuad::create("particleSystem/hechengBomb.plist") ;
    
    
    effect2->setScale(1.3f);
    effect2->setAutoRemoveOnFinish(true);
    effect2->setPosition(pos);
    addChild(effect2,111);
    
}


void CFightWinLayer::AddBaozhaStar()
{
    Sprite *pflag1 = (Sprite *)getChildByTag(101);
    pflag1->runAction(Sequence::create(EaseExponentialIn::create(RotateTo::create(0.5f,10)),EaseExponentialIn::create(RotateTo::create(0.5f,0)),NULL));
    
    Sprite *pflag2 = (Sprite *)getChildByTag(102);
    pflag2->runAction(Sequence::create(EaseExponentialIn::create(RotateTo::create(0.5f,-10)),EaseExponentialIn::create(RotateTo::create(0.5f,0)),NULL));
    
    ParticleSystemQuad* effect1 = ParticleSystemQuad::create("particleSystem/star2.plist") ;
    
    effect1->setPosition(m_winSize.width*0.5,m_winSize.height*0.7);
    
    addChild(effect1,2,55);
}

void CFightWinLayer::OnRemoveNotice()
{
    CallFunc* getOff = CallFunc::create(CC_CALLBACK_0(CFightWinLayer::OnRemoveSucessLayer, this));
    CallFunc* getOff1 = CallFunc::create(CC_CALLBACK_0(CFightWinLayer::AddBaozhaStar, this));
    
    Spawn *ret = Spawn::create(FadeIn::create(0.2),ScaleTo::create(0.2,1.25),NULL);
    
    m_pSucess->runAction(Sequence::create(ret,getOff1 ,DelayTime::create(1.0f),getOff, NULL));
    
    schedule(schedule_selector(CFightWinLayer::addParticleEffect), 0.5);
    
}

void CFightWinLayer::AddSucessDonghua()
{
    float  iScale = 0;
    
    m_pSucess = Sprite::createWithSpriteFrameName("win.png");
    
    
    m_pSucess->setOpacity(0);
    m_pSucess->setPosition(m_winSize.width*0.5,m_winSize.height*0.7);
    m_pSucess->setScale(12.5);
    addChild(m_pSucess,3);

    Sprite *pFlag2 = Sprite::createWithSpriteFrameName("flag.png");
    pFlag2->setFlippedX(true);
    pFlag2->setAnchorPoint(Vec2(1,0));
    pFlag2->setPosition(m_winSize.width*0.5,m_winSize.height*0.5);
    pFlag2->setScale(iScale);
    pFlag2->runAction(Sequence::create(ScaleTo::create(0.1f,1.4),NULL));
    addChild(pFlag2,1,101);
    
    
    Sprite *pFlag = Sprite::createWithSpriteFrameName("flag.png");
    pFlag->setAnchorPoint(Vec2(0,0));
    pFlag->setPosition(m_winSize.width*0.5,m_winSize.height*0.5);
    pFlag->setScale(iScale);
    pFlag->runAction(Sequence::create(ScaleTo::create(0.1f,1.4),NULL));
    addChild(pFlag,1,102);
    
    
    Sprite *pHeng = Sprite::createWithSpriteFrameName("hengtiao.png");
    pHeng->setPosition(m_winSize.width*0.5,m_winSize.height*0.5);
    pHeng->setScale(iScale);
    pHeng->runAction(ScaleTo::create(0.1f,1.6));
    addChild(pHeng,2,103);
    
    
    Size tHengSize = pHeng->getContentSize();
    Sprite *pDun = Sprite::createWithSpriteFrameName("jiandun.png");
    pDun->setScale(1);
    pDun->setPosition(tHengSize.width*0.5,tHengSize.height*0.5);
    pHeng->addChild(pDun,22);
    
    
    //长光
    Sprite *pLongLight = Sprite::createWithSpriteFrameName("dilight.png");
    pLongLight->setPosition(m_winSize.width*0.5,m_winSize.height*0.5);
    addChild(pLongLight,1,105);
    pLongLight->runAction(ScaleTo::create(0.1f,5.2));
    
    
    //光
    Sprite *pLight = Sprite::createWithSpriteFrameName("backlight3.png");
    pLight->setPosition(m_winSize.width*0.5,m_winSize.height*0.7);
    addChild(pLight,1,106);
    pLight->setScale(iScale);
    pLight->runAction(ScaleTo::create(0.1f,2.6));
    
    
    Sprite *pStar = Sprite::createWithSpriteFrameName("xx.png");
    pStar->setScale(iScale);
    pStar->setPosition(m_winSize.width*0.5,m_winSize.height*0.8);
    addChild(pStar,2,104);
    pStar->runAction(ScaleTo::create(0.1f,2.6));
    
    CallFunc* getOff = CallFunc::create(CC_CALLBACK_0(CFightWinLayer::OnRemoveNotice,this));
    runAction(Sequence::create(DelayTime::create(0.5f),getOff, NULL));
}


void CFightWinLayer::addBetaStar(float ft)
{
    if(m_iTime == 0)
    {
        AddFightWinNumberAction();
    }
    
    SimpleAudioEngine::getInstance()->playEffect("music/award.mp3");

    m_iTime++;
    ParticleSystemQuad* effect2 = ParticleSystemQuad::create("particleSystem/beatStar.plist") ;
    
    effect2->setAutoRemoveOnFinish(true);
    effect2->setPosition(m_winSize.width*0.65,m_winSize.height*0.58);
    addChild(effect2,111);
    
}


void CFightWinLayer::addBetaStar2(float ft)
{
    m_iTime2++;
      SimpleAudioEngine::getInstance()->playEffect("music/award.mp3");
    unschedule(schedule_selector(CFightWinLayer::addBetaStar));
    ParticleSystemQuad* effect2 = ParticleSystemQuad::create("particleSystem/beatStar.plist") ;
    effect2->setAutoRemoveOnFinish(true);
    effect2->setPosition(m_winSize.width*0.65,m_winSize.height*0.425);
    addChild(effect2,111);
    
    if(m_iTime2<=5)
    {
        unschedule(schedule_selector(CFightWinLayer::addBetaStar2));
        //技能3
        if(CFightData::getInstance()->getAddCoin()>0)
        {
            CallFunc* pCoinAct = CallFunc::create(CC_CALLBACK_0(CFightWinLayer::GoCoinRun, this));
            m_pKillBetaCoin->runAction(Sequence::create(DelayTime::create(0.25),pCoinAct, NULL));
        }

        m_iTime2 = 0;
    }
}


void CFightWinLayer::addBetastar3(float ft)
{
    unschedule(schedule_selector(CFightWinLayer::addBetaStar2));
    m_iTime3++;
      SimpleAudioEngine::getInstance()->playEffect("music/award.mp3");
    ParticleSystemQuad* effect2 = ParticleSystemQuad::create("particleSystem/beatStar.plist") ;
    effect2->setAutoRemoveOnFinish(true);
    effect2->setPosition(m_winSize.width*0.65,m_winSize.height*0.35);
    addChild(effect2,111);
    
    if(m_iTime3<=5)
    {
        unschedule(schedule_selector(CFightWinLayer::addBetastar3));
        m_iTime3 = 0;
    }
    
}


void  CFightWinLayer::AddFightWinNumberAction()
{

    __String tempValue;
    Sprite *pBg =  (Sprite *)getChildByTag(1000);
    
    Sprite *pdk1  = (Sprite *)pBg->getChildByTag(201);
    
    
    Size tDk = pdk1->getContentSize();
    
    Sprite *pdkMid = (Sprite *)pBg->getChildByTag(202);
    
    Size  tPdk2 = pdkMid->getContentSize();
    
    //技能1
    tempValue.initWithFormat("%d",m_iGotGold);
    
    LabelAtlas*  pCoinLable= CCLabelAtlas::create(tempValue.getCString(),"fonts/GetCoinNum.png", 34, 41, '0');
    
    CallFunc* pExpAct = CallFunc::create(CC_CALLBACK_0(CFightWinLayer::GoSecond, this));
    

    m_pExpBeatNum->runAction(Sequence::create(DelayTime::create(0.25),pExpAct, NULL));
    
    m_pExpBeatNum->setPosition(tPdk2.width*0.7,tPdk2.height*0.3);
    
    m_pExpBeatNum->BeatDownTo(0);

    
    if(m_pKillBetaCoin)
    {
        m_pKillBetaCoin->setVisible(true);
    }
    
}


void CFightWinLayer::initUI()
{
    //背光
    Sprite *pBgLight = Sprite::createWithSpriteFrameName("backlight2.png");
    pBgLight->cocos2d::Node::setScale(4);
    
    pBgLight->setPosition(m_winSize.width*0.5,m_winSize.height*0.75);
    addChild(pBgLight);
    
    
    //塔防模式胜利直接三星
    if(CFightData::getInstance()->getTheLevelCondition() == eLevelCondition_TowerDefense)
    {
        m_iGotRare = 3;
        
        int iLvId =  CFightData::getInstance()->getNowLevelId();
        
        levelInfo* pSmallLevelInfo = CDataManager::getTheLevelInfoWithId(iLvId);
        int iGoldNum = pSmallLevelInfo->goldNum;

        m_iGotGold = iGoldNum;
        
    }
    
    
    //战斗胜利
    Scale9Sprite *pBg = Scale9Sprite::createWithSpriteFrameName("common_bg.png");
    pBg->setPosition(m_winSize.width*0.5,m_winSize.height*0.48);
    addChild(pBg,0,1000);
    pBg->setContentSize(Size(pBg->getContentSize().width * 1.1f, pBg->getContentSize().height * 1.1f));
    m_szBg = pBg->getBoundingBox().size;
    
    
    Size tBg = pBg->getContentSize();
    
    
    Sprite* starBg = Sprite::createWithSpriteFrameName("result_starBg.png");
    starBg->cocos2d::Node::setScale(1.2);
    pBg->addChild(starBg);
    starBg->setPosition(Vec2(m_szBg.width / 2, m_szBg.height * 0.91 ));
    

    float height = 0;
    
    Sprite *pLeveAward;
    
    
    if(CFightData::getInstance()->getFightType()== FIGHT_PVP_TYPE)
    {
        height =  m_szBg.height * 0.31f;
        pLeveAward = Sprite::createWithSpriteFrameName("getRobStone.png");
        Sprite  *pWinText = Sprite::createWithSpriteFrameName("fightwin.png");
        pWinText->setPosition(tBg.width*0.5,tBg.height*0.9);
        
        pBg->addChild(pWinText);
        
        
    }
    else
    {
        height = m_szBg.height * 0.51f;
        pLeveAward = Sprite::createWithSpriteFrameName("LevelAward.png");
        pLeveAward->setScale(2);
    }
    
   
    
    //获得经验和金币
    Sprite *pdk1 = Sprite::createWithSpriteFrameName("zhdt.png");
    pdk1->setPosition(Vec2(m_szBg.width * 0.5f, height+100));
    pBg->addChild(pdk1,0,201);

    
    Size tDk = pdk1->getContentSize();

 
    pLeveAward->setPosition(Vec2(tDk.width*0.2,tDk.height*0.5));
    pdk1->addChild(pLeveAward);
    
    std::string str = "coin.png";
    
     Sprite *pCoinSprite1 = Sprite::createWithSpriteFrameName("coin.png");
    
    if(CFightData::getInstance()->getFightType() == FIGHT_PVP_TYPE)
    {
        pCoinSprite1 = Sprite::createWithSpriteFrameName("pvp_ore.png");
        
        pCoinSprite1->setPosition(Vec2(tDk.width*0.4,tDk.height*0.5));
    }
    else
    {
        pCoinSprite1->setPosition(Vec2(tDk.width*0.5,tDk.height*0.5));
    }
  
   
    pCoinSprite1->setAnchorPoint(Vec2(0, 0.5));
  
    pdk1->addChild(pCoinSprite1);
    
    __String tempValue ;
    
    
    tempValue.initWithFormat("%d",0);
    
    LabelAtlas*  pCoinLable= CCLabelAtlas::create(tempValue.getCString(),"fonts/GetCoinNum.png", 34, 41, '0');
    
    m_pNumber = CBeatNumbers::createWithNumberAndSize(0,30,1,pCoinLable);
    
    m_pNumber->setPosition(Vec2(tDk.width*0.7,tDk.height*0.3));
    pdk1->addChild(m_pNumber);
    
    
    if(CFightData::getInstance()->getFightType() == FIGHT_PVP_TYPE)
    {
        int iStone = CFightData::getInstance()->getTheRobStone();
    
        
        char buf[50] = {0};
        sprintf(buf,"%d",iStone);
        
        if(iStone!=0)
        {
            ParticleSystemQuad* effect2 = ParticleSystemQuad::create("particleSystem/beatStar.plist") ;
            effect2->setAutoRemoveOnFinish(true);
            effect2->setPosition(m_winSize.width*0.65,m_winSize.height*0.425);
            addChild(effect2,111);
        }
        

       //一秒100个矿石
        
        m_pNumber->setBeatTime(0.5);
        
        m_pNumber->BeatUpTo(iStone,0);
      
        if(iStone>=48)
        {
            //float fStone = iStone/48;
    
            m_iTime = 0;
            
            schedule(schedule_selector(CFightWinLayer::UpdateRobStone),0.1);
        }
        
        
    Sprite *pNormal = Sprite::createWithSpriteFrameName("bluean.png");
    Sprite *pSelect = Sprite::createWithSpriteFrameName("bluean.png");
    pSelect->setColor(Color3B::GRAY);
    
    MenuItemSprite *pOkItem = MenuItemSprite::create(pNormal,pSelect,CC_CALLBACK_1(CFightWinLayer::OnPvpOK, this));
    pOkItem->setScale(1.65);
    
    Menu *  pMenu  = Menu::create(pOkItem,NULL);
    pMenu->setPosition(tBg.width*0.5,tBg.height*0.1);
    pBg->addChild(pMenu);
    m_guidePos = pOkItem->convertToWorldSpaceAR(Vec2::ZERO);
    
    Label  *pBackLable = Label::createWithTTF("确定","fonts/cuti.ttf",32);
    pBackLable->setScale(0.6);
    pBackLable->setPosition(pOkItem->getContentSize().width*0.5,pOkItem->getContentSize().height*0.5);
    pOkItem->addChild(pBackLable);
    
    }
    else
    {
        //星星
        for(int i=0;i<3;i++)
         {
             Sprite *pStar =  Sprite::createWithSpriteFrameName("darkstar.png");
             pStar->setPosition(m_szBg.width*0.325+i*0.17*m_szBg.width,m_szBg.height*0.95);
             pStar->setVisible(false);
             pBg->addChild(pStar,1 ,RESULT_STAR_ONE + i);
         }
     
    
     
    
    Sprite *pdkMid = Sprite::createWithSpriteFrameName("zhdt.png");
    pdkMid->setPosition(m_szBg.width * 0.5f, height);
    pBg->addChild(pdkMid,0,202);
    
    Size tPdk2 = pdkMid->getContentSize();
    
    Sprite *pLvExp = Sprite ::createWithSpriteFrameName("LevelExp.png");
    pLvExp->setScale(2);
    pLvExp->cocos2d::Node::setPosition(tPdk2.width*0.2,tPdk2.height*0.5);
    pdkMid->addChild(pLvExp);
    
    Sprite *pExp = Sprite::createWithSpriteFrameName("EXP.png");
    pExp->setAnchorPoint(Vec2(0, 0.5));
    pExp->setPosition(tPdk2.width*0.5,tPdk2.height*0.5);
    pdkMid->addChild(pExp);
    
    tempValue.initWithFormat("%d",0);

    LabelAtlas* pExpLable= CCLabelAtlas::create(tempValue.getCString(),"fonts/KillCoinNum.png", 36, 41,'0');
    
    m_pExpBeatNum = CBeatNumbers::createWithNumberAndSize(0,30,1,pExpLable);
    
    m_pExpBeatNum->setPosition(tPdk2.width*0.7,tPdk2.height*0.3);
    pdkMid->addChild(m_pExpBeatNum);

    
    if(CFightData::getInstance()->getAddCoin() > 0)
    {

        Sprite *pdk3 = Sprite::createWithSpriteFrameName("zhdt.png");
        pdk3->setPosition(m_szBg.width * 0.5f, height-100);
        pBg->addChild(pdk3);

        Size tPdk3 = pdk3->getContentSize();
        
        Sprite *pKillGet = Sprite::createWithSpriteFrameName("KillGet.png");
        pKillGet->setScale(2);
        pKillGet->setPosition(Vec2(tPdk3.width*0.2,tPdk3.height*0.5));
        pdk3->addChild(pKillGet);
        
        Sprite *pCoinSprite2 = Sprite::createWithSpriteFrameName("coin.png");
        pCoinSprite2->setAnchorPoint(Vec2(0, 0.5));
        pCoinSprite2->setPosition(tPdk3.width*0.5,tPdk3.height*0.5);
        pdk3->addChild(pCoinSprite2);
        
        tempValue.initWithFormat("%d",CFightData::getInstance()->getAddCoin());
        
        LabelAtlas* pkillCoin= CCLabelAtlas::create(tempValue.getCString(),"fonts/GetCoinNum.png",34, 41, '0');
        m_pKillBetaCoin = CBeatNumbers::createWithNumberAndSize(0,34,1,pkillCoin);
        m_pKillBetaCoin->setPosition(tPdk3.width*0.7,tPdk3.height*0.3);
        m_pKillBetaCoin->BeatDownTo(0);
        
        pdk3->addChild(m_pKillBetaCoin);
        
    }
    
    Sprite *pBule =  Sprite::createWithSpriteFrameName("redan.png");
    Sprite *pBuleSlect =  Sprite::createWithSpriteFrameName("redan.png");
    pBuleSlect->setColor(Color3B::GRAY);
    m_pBackItem = LYMenuItemSprite::create(pBule,pBuleSlect,CC_CALLBACK_1(CFightWinLayer::OnCallBack, this));
    m_pBackItem->setScale(1.65);

        
        
    Label  *pBackLable = Label::createWithTTF("返回","fonts/cuti.ttf",32);
    pBackLable->setScale(0.6);
    pBackLable->setPosition(m_pBackItem->getContentSize().width*0.5,m_pBackItem->getContentSize().height*0.5);
    m_pBackItem->addChild(pBackLable);
    m_pBackItem->setEnabled(false);
    
        
        
    Sprite *pAgain=  Sprite::createWithSpriteFrameName("bluean.png");
    Sprite *pAgainSlect =  Sprite::createWithSpriteFrameName("bluean.png");
    pAgainSlect ->setColor(Color3B::GRAY);
    m_pAgainItem = LYMenuItemSprite::create(pAgain,pAgainSlect,CC_CALLBACK_1(CFightWinLayer::OnFightAgain, this));
    m_pAgainItem->setScale(1.65);
    
        
   

    Label  *pAgainLable = Label::createWithTTF("再来一次","fonts/cuti.ttf",32);
    pAgainLable->setScale(0.6);
    pAgainLable->setPosition(m_pAgainItem->getContentSize().width*0.5,m_pAgainItem->getContentSize().height*0.5);
    m_pAgainItem->addChild(pAgainLable);
    m_pAgainItem->setEnabled(false);
        
    
    
    int midLevel = CFightData::getInstance()->getNowMidLevelId();
    int m_smallLevel = CFightData::getInstance()->getNowLevelId();
    if(CDataManager::getMidLevelInfoById(midLevel)->lastLevel != m_smallLevel)
    {
        Sprite* normal = Sprite::createWithSpriteFrameName("greenan.png");
        Sprite* select = Sprite::createWithSpriteFrameName("greenan.png");
    
        Sprite *pDisbale = Sprite::createWithSpriteFrameName("greenan.png");
        pDisbale->setColor(Color3B::GRAY);
        
        select->setColor(Color3B::GRAY);
        m_pNextItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(CFightWinLayer::OnFightAgain, this));
        m_pNextItem->setTag(10);
        m_pNextItem->setScale(1.65);
        Label* next = Label::createWithTTF("下一关", "fonts/cuti.ttf", 32);
        next->setScale(0.6);
        m_pNextItem->addChild(next);
        m_pNextItem->setEnabled(false);
        
        next->setPosition(Vec2(normal->getContentSize().width / 2, normal->getContentSize().height / 2));
        
        if((!(CDataManager::getEndedBy(GUIDE_GET_TASK)) && (CFightData::getInstance()->getNowLevelId() == 2)) || (!(CDataManager::getEndedBy(GUIDE_UPGRADE_SOLIDER)) && (CFightData::getInstance()->getNowLevelId() == 3)) || (!(CDataManager::getEndedBy(GUIDE_SKILL_UPGRADE)) && (CFightData::getInstance()->getNowLevelId() == 4)) || (!(CDataManager::getEndedBy(GUIDE_SOLIDER_NUM)) &&(CFightData::getInstance()->getNowLevelId() == 6)) || (!(CDataManager::getEndedBy(GUIDE_COMMON_CARD)) && (CFightData::getInstance()->getNowLevelId() == 15)))
        {
            m_pMenu = Menu::create(m_pBackItem, NULL);
            m_pAgainItem = NULL;
            m_pNextItem = NULL;
            
        }
        else{
            m_pMenu = Menu::create(m_pBackItem,m_pAgainItem, m_pNextItem, NULL);
        }
        m_pMenu->alignItemsHorizontallyWithPadding(70);
        m_pMenu->setPosition(m_szBg.width*0.5,m_szBg.height*0.13);
        pBg->addChild(m_pMenu);
    }
    else{
        m_pMenu = Menu::create(m_pBackItem,m_pAgainItem, NULL);
        m_pMenu->alignItemsHorizontallyWithPadding(100);
        m_pMenu->setPosition(m_szBg.width*0.5,m_szBg.height*0.13);
        pBg->addChild(m_pMenu);
    }
    m_pMenu->setEnabled(false);
    if(m_monsterId== 0)
    {
        m_pMenu->setEnabled(true);
    }
    
    Menu* starMenu = Menu::create();
    pBg->addChild(starMenu, 5);
    starMenu->setPosition(Vec2::ZERO);



    for(int i = 0; i < m_iGotRare; i++)
    {
        Sprite* sp = (Sprite*)(pBg->getChildByTag(RESULT_STAR_ONE + i));
        Sprite* starNormal = Sprite::createWithSpriteFrameName("battle_star.png");
        Sprite* starSelect = Sprite::createWithSpriteFrameName("battle_star.png");
        starSelect->setColor(Color3B::GRAY);
        LYMenuItemSprite* starItem = LYMenuItemSprite::create(starNormal, starSelect, CC_CALLBACK_1(CFightWinLayer::btnCallBack, this));
        starMenu->addChild(starItem, 5);
        starItem->setPosition(sp->getPosition());
        starItem->setScale(2.5);
        
        m_StarEffectVec2 = sp->getPosition();
        
        //log("x=%f,y=%f\n",m_StarEffectVec2.x,m_StarEffectVec2.y);
        
        starItem->runAction(Hide::create());
        CallFunc* act = CallFunc::create(CC_CALLBACK_0(CFightWinLayer::addStarEffect, this));

        
        Sequence* starAct = Sequence::create(ScaleTo::create(0.35, 1.0),act, NULL);
        
        starItem->runAction(Sequence::create(DelayTime::create(i/2.1f), Show::create(), starAct, NULL));
        sp->runAction(Sequence::create(DelayTime::create(i/2.1f), RemoveSelf::create(), NULL));

        //加入特效
        
    }
    for(int i = m_iGotRare; i < 3; i++)
    {
        Sprite* sp = (Sprite*)(pBg->getChildByTag(RESULT_STAR_ONE + i));
        Sprite* starNormal = Sprite::createWithSpriteFrameName("star_fail.png");
        Sprite* starSelect = Sprite::createWithSpriteFrameName("star_fail.png");
        LYMenuItemSprite* starItem = LYMenuItemSprite::create(starNormal, starSelect, CC_CALLBACK_1(CFightWinLayer::btnCallBack, this));
        starMenu->addChild(starItem, 5);
        starItem->setScale(0.9f);
        starItem->setPosition(sp->getPosition());
        sp->removeFromParent();
    }
    
    
    CallFunc* expAct = CallFunc::create(CC_CALLBACK_0(CFightWinLayer::addExp, this));
    float delayTime = (m_iGotRare - 1) / 2.1f + 0.1f;
    if(m_iGotRare <= 0)
    {
        delayTime = 0.1f;
    }
    
    runAction(Sequence::create(DelayTime::create(3), expAct, NULL));
    
    if(m_iGotRare ==0)
    {
        m_pNumber->BeatDownTo(0);
        m_pNumber->BeatUpTo(m_iGotGold);
    
        ParticleSystemQuad* effect2 = ParticleSystemQuad::create("particleSystem/beatStar.plist") ;
        
        effect2->setAutoRemoveOnFinish(true);
        effect2->setPosition(m_winSize.width*0.65,m_winSize.height*0.58);
        addChild(effect2,111);
        
        schedule(schedule_selector(CFightWinLayer::addBetaStar),0);
    }
    }
    
}


void CFightWinLayer::GoSecond()
{
    m_pExpBeatNum->BeatDownTo(0);
    m_pExpBeatNum->BeatUpTo(m_iGotExp);
    ParticleSystemQuad* effect2 = ParticleSystemQuad::create("particleSystem/beatStar.plist") ;
    effect2->setAutoRemoveOnFinish(true);
    effect2->setPosition(m_winSize.width*0.65,m_winSize.height*0.425);
    addChild(effect2,111);
    
    schedule(schedule_selector(CFightWinLayer::addBetaStar2), 0.0f);
}


void CFightWinLayer::UpdateRobStone(float ft)
{
    m_iTime++;
    
    ParticleSystemQuad* effect2 = ParticleSystemQuad::create("particleSystem/beatStar.plist") ;
    effect2->setAutoRemoveOnFinish(true);
    effect2->setPosition(m_winSize.width*0.65,m_winSize.height*0.425);
    addChild(effect2,111);
 
    if(m_iTime>5)
    {
        unschedule(schedule_selector(CFightWinLayer::UpdateRobStone));
    }
}


void CFightWinLayer::GoCoinRun()
{
    m_pKillBetaCoin->BeatDownTo(0);
    m_pKillBetaCoin->BeatUpTo(CFightData::getInstance()->getAddCoin());
    
    ParticleSystemQuad* effect2 = ParticleSystemQuad::create("particleSystem/beatStar.plist") ;
    effect2->setAutoRemoveOnFinish(true);
    effect2->setPosition(m_winSize.width*0.65,m_winSize.height*0.35);
    addChild(effect2,111);
    
    schedule(schedule_selector(CFightWinLayer::addBetastar3), 0.0f);
}


void CFightWinLayer::addExp()
{
    //加经验。及提示升级对话框
    //更新玩家体力
    bValid = true;
    if(m_pBackItem)
    {
        m_pBackItem->setEnabled(true);
    }
    
    if(m_pAgainItem)
    {
        m_pAgainItem->setEnabled(true);
    }


    if(m_pNextItem)
    {
        m_pNextItem->setEnabled(true);
    }
    if(CPersonalData::getInstance()->getJudgeUpgrade())
    {
        CommonPropLayer* layer = CommonPropLayer::create(LEVEL_UPGRADE_TYPE, before, after);
        addChild(layer, 100);
        layer->runAction(Sequence::create(Hide::create(), DelayTime::create(0.35f), Show::create(), NULL));
    }
    else{
        if(m_monsterId > 0)
        {
            float delayTime = (m_iGotRare - 1) / 2.1f + 0.1f;
            if(m_iGotRare <= 0)
            {
                delayTime = 0.1f;
            }
            CallFunc* expAct = CallFunc::create(CC_CALLBACK_0(CFightWinLayer::showNewHero, this));
            runAction(Sequence::create(DelayTime::create(delayTime), expAct, NULL));
        }
        else{
            bValid = true;
            if(m_pBackItem)
            {
                m_pBackItem->setEnabled(true);
            }
            
            if(m_pAgainItem)
            {
                m_pAgainItem->setEnabled(true);
            }
            
            if(m_pNextItem)
            {
                m_pNextItem->setEnabled(true);
            }
        }
    }
}

int CFightWinLayer::judgeIfGetNewHero()
{
    bool bExisted = false;
    int monsterId = 0;
    if((CFightData::getInstance()->getNowLevelId() == 1)&& (!CDataManager::getEndedBy(GUIDE_ADD_CIRCLE)))
    {
        monsterId = 2;
    }
    else if((CFightData::getInstance()->getNowLevelId() == 4) && (!CDataManager::getEndedBy(GUIDE_ADD_RECTANGLE)))
    {
        monsterId = 13;
    }
    else if((CFightData::getInstance()->getNowLevelId() == 7) && (!CDataManager::getEndedBy(GUIDE_ADD_V)))
    {
        monsterId = 8;
    }
    
    if(monsterId > 0)
    {
        //防止判断出现漏洞，加一个当前解锁士兵是哦否已经存在判断
        std::vector<FighterInfo*> tempVec= CPersonalData::getInstance()->m_vecMyFighterInfo;
        for(int i = 0; i < tempVec.size(); i++)
        {
            if(tempVec.at(i)->monsterId == monsterId)
            {
                bExisted = true;
                break;
            }
        }
        if(!bExisted)
        {
            CPersonalData::getInstance()->setInsertHero(true);
            CDataManager::insertIntoMonsterInfo(monsterId, 0, 1, 1);//自动添加英雄
        }
        else{
            monsterId = 0;
        }
    }
    return monsterId;
}

void CFightWinLayer::btnCallBack(Ref *pSender)
{
    if(!bValid)
    {
        return;
    }
    CommonPropLayer* layer = CommonPropLayer::create(LEVEL_STAR_TYPE, true);
    addChild(layer, 999);
    
}

void CFightWinLayer::onEnterTransitionDidFinish(){
    
    Layer::onEnterTransitionDidFinish();
}

void CFightWinLayer::OnCallBack(Ref *pSender)
{
    if(!bValid)
    {
        return;
    }
//    CLayerManager::GetSingletonPtr()->createLayer(eLayer_Login, NULL,false);
    if((CFightData::getInstance()->getNowMidLevelId() < TOWER_LEVEL_INDEX) && (!CPersonalData::getInstance()->getJumpFromSkill()))
    {
        int midLevel = CFightData::getInstance()->getNowMidLevelId();
        midLevelInfo* pInfo = CDataManager::getMidLevelInfoById(midLevel);
        if(CFightData::getInstance()->getNowLevelId() <= m_reSmallLevel)
        {
            CPersonalData::getInstance()->setFinishedLevel(true);
        }
        if((pInfo->lastLevel == CFightData::getInstance()->getNowLevelId()) && (CPersonalData::getInstance()->getlevelMode()==0)
           && ((!CPersonalData::getInstance()->getFinishedLevel()) || (CPersonalData::getInstance()->getShowMap())))
        {
            CPersonalData::getInstance()->setNewMidLevel(true);
        }
        CPersonalData::getInstance()->setisWinManual(false);
    }
    
    CLayerManager::GetSingletonPtr()->createLayerWithLoading(eLayer_MainMenu, NULL,false);
    
//    CLayerManager::GetSingletonPtr()->createLayer(eLayer_MainMenu, NULL,false);
    log("返回");
}

void CFightWinLayer::OnFightAgain(Ref *pSender)
{
    if(!bValid)
    {
        return;
    }
    int tag = ((Node*)pSender)->getTag();
    levelInfo* pSmallLevelInfo = CDataManager::getTheLevelInfoWithId(CFightData::getInstance()->getNowLevelId());
    //判断体力是否足够
    int needStaina = pSmallLevelInfo->stamina;
    if (MY_PERSION->getlevelMode() == 1) {
        needStaina = pSmallLevelInfo->stamina * 2;
    }
    if(needStaina > CPersonalData::getInstance()->GetPhysicalNum())//测试用false，看到就删
    {
        SimpleAudioEngine::getInstance()->playEffect("music/noBtn.mp3");
        Sprite * thetarget = CFightManager::getTheFlyWord("体力不足","fonts/cuti.ttf");
        thetarget->setPosition(Vec2(m_winSize.width / 2, m_winSize.height / 2));
        addChild(thetarget, 10);
        return;
    }
    if(tag == 10)
    {
        CPersonalData::getInstance()->setisWinManual(true);
        CPersonalData::getInstance()->setisFailManual(true);
        CPersonalData::getInstance()->setisOpenNewLevel(false);
        
        //CFightData::getInstance()->setAddCoin(<#int var#>)
        CFightData::getInstance()->setNowLevelId(CFightData::getInstance()->getNowLevelId() + 1);
        CFightData::getInstance()->newLevelStart(CFightData::getInstance()->getNowLevelId());
        CPersonalData::getInstance()->setisOpenNewSmallLevel(false);
        if((CDataManager::getGuideStepNum() == GUIDE_ADD_LINE) && (CFightData::getInstance()->getNowLevelId() == 2) && (MY_PERSION->getlevelMode() == 0))
        {
            guiderManagerLayer::getInstance()->removeGuiderLayer();
        }
    }
    else{
        CPersonalData::getInstance()->setisOpenNewSmallLevel(false);
        CFightData::getInstance()->newLevelStart(CFightData::getInstance()->getNowLevelId());
    }
    CLayerManager::GetSingletonPtr()->createLayerWithLoading(eLayer_Fight, NULL,false);
    
}