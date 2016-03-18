
//
//  pvpLayer.cpp
//  WarOfGod
//
//  Created by user on 15/6/3.
//
//

#include "pvpMainLayer.h"
#include "PublicDefine.h"
#include "LYFightManager.h"
#include "CSharedPlist.h"
#include "LYPersonalData.h"
#include "PhysicalSystem.h"
#include "DataManager.h"
#include "LYBaseTower.h"
#include "LYFightData.h"
#include "Common.h"
#include "zzbNetControlModule.h"
#include "zzbNewNetStruct.h"
#include "DBMgr.h"
#include "LYBaseCity.h"
#include "commonPropLayer.h"

#include "pvpWarReportLayer.h"
#include "guiderManagerLayer.h"
#include "CNpcFighter.h"
#include "DBMgr.h"
#include "evolutionLayer.h"
#include "sysTime.h"


#define BUILD_HEIGHT_PER 440
#define BUILD_WIDTH_PER 295
#define MAIN_BASE_MAX_LEVEL 7

//#define BTN_DISTANCE_UP  100
#define BTN_DISTANCE_DOWN 115 //下面按钮距离模型的位置


pvpMainLayer::pvpMainLayer()
{
    m_listen = NULL;
    //    m_spriteBg = NULL;
    layer3D = NULL;
    m_staminaBg = NULL;
    m_pStaminaLabel = NULL;
    m_pStaminaMaxLabel = NULL;
    m_pTime = NULL;
    m_pCoin = NULL;
    m_pStone = NULL;
    m_progress = NULL;
    m_bufferIime = 0;
    m_blockxLineVec.clear();
    m_blockyLineVec.clear();
    m_blockXPointVec.clear();
    m_blockyPointVec.clear();
    m_allBuildType.clear();
    m_bActive = false;
    //m_buildInfo = NULL;
    m_curBuild = NULL;
    m_newTowerObbInfo = new towerOBBInfo;
    m_bValid = false;
    m_oldPos = Vec3::ZERO;
    m_bMoveMap = false;
    m_bMove = false;
    m_btnMenu = NULL;
    m_arrowVec.clear();
    m_btnVec.clear();
    m_btnObbVec.clear();
    m_pKuanchanBuildVec.clear();
    m_startPos = Vec2::ZERO;
    m_towerPos = Vec3::ZERO;
    m_bFirst = false;
    m_pLayer = NULL;
    m_pStoneAllNum = NULL;
    m_stone = 0;
    m_neddStone = 0;
    m_serverTime = 0;
    m_addStone = 0;
    m_curBuildingId = 0;
    m_distance = 0;
    m_bNewTower = false;
    m_friendTowerLevel = 0;
    m_layer = NULL;
    m_msgItem = NULL;
    m_preBuild = NULL;
    m_allVideoInfo.clear();
    m_stoneVec.clear();
    m_iBuildUseType = 1;
    m_buildItem = NULL;
    m_pDefenseItem = NULL;
    m_pIncomeItem = NULL;
    m_pFightItem = NULL;
    m_buildLayer = NULL;
    m_backItem = NULL;
    m_scrollView = NULL;
    m_selectedItem = NULL;
    m_timeProgressBg = NULL;
    m_timeProgress = NULL;
    m_upgradeTower = NULL;
    m_pTimeLabel = NULL;
    m_searchBg = NULL;
    m_cost = 0;
    m_remainTime = 0;
    m_upgradeBuildId = 0;
    m_time = 0;
    m_upgradeEnd = false;
    m_towerName = NULL;
    m_towerLevel = NULL;
    m_hammer = NULL;
    m_bBaoshi = true;
    m_pvPStone = NULL;
    m_addLabel = NULL;
    m_pCurStoneNum = NULL;
    m_userBuildingId = 0;
    m_fKuanShiAllTime = 0;
    //m_fAddStone = CDBMgr::getInstance()->GetTempPvpUnpickStone();
    
    m_fAddStone = 0;
    m_iBuildUseType = 2;
    m_bValidMove = false;
    m_productRemainTime = 0;
    m_fTime = 0;
    m_productNum = 0;
    m_productNeedTime = 0;
    m_productTower = NULL;
    m_productProgressBg = NULL;
    m_productProgress = NULL;
    m_productTimeLabel = NULL;
    m_productBuildId = 0;
    m_bProductEnd = false;
    m_protectedTime = 0;
    m_limitTime = 0;
    m_iTipNum = 0;
    m_patrolman = NULL;
    m_pCircle = NULL;
    m_touchFlag = false;
    //m_bTouchOk = false;
    m_pCoinItem = NULL;
    m_pStoneItem = NULL;
    m_pStamainItem = NULL;
    m_fAllPerTime = 0.0;
    m_bAddPer = false;
    
    m_fSecPass = 0.0;
    m_stoneRate = 0;
    CFightData::getInstance()->setNowLevelId(0);
    
    
}



pvpMainLayer::~pvpMainLayer()
{
    delete m_newTowerObbInfo;
    m_newTowerObbInfo = NULL;
    unscheduleUpdate();
    
    CPersonalData::getInstance()->setUpdateUserInfo(true);
    //CDBMgr::getInstance()->SetTempPvpUnpikcStone(m_fAddStone);
    
    
    _eventDispatcher->removeEventListener(m_listen);
    CSharedPlist::getInstance()->erasePlist("plist/pvpMain.plist");
    CSharedPlist::getInstance()->erasePlist("plist/pvpBuild.plist");
    CSharedPlist::getInstance()->erasePlist("plist/pvpEffect.plist");
    CSharedPlist::getInstance()->erasePlist("plist/pvpIcon.plist");
    if(CLayerManager::GetSingletonPtr()->getCurLayerType() != eLayer_FightPVP)
    {
        CSharedPlist::getInstance()->erasePlist("plist/pvpCommon.plist");
    }
}


vector<Vec3> pvpMainLayer::getNpcValidPlace(vector<Vec3> tAdd)
{
    tAdd.clear();
    
    for(int i = 1; i < MAP_X_INDEX-1; i++)
    {
        for(int j = 1 ; j < MAP_Y_INDEX-1; j++)
        {
            if(m_allBlock[i][j].sprite)
            {
                if(m_allBlock[i][j].type ==0)
                {
                    Vec3  t = m_allBlock[i][j].sprite->getPosition3D();
                    
                    
                    {
                        tAdd.push_back(t);
                    }
                    
                }
                
            }
        }
    }
    return tAdd;
    
}


Vec3 pvpMainLayer::getNearEightPos(int x,int z)
{
    bool bTure= false;
    Vec3 pt;
    
    do
    {
        
        bTure= false;
        int iRandomX = random(-3,3);
        int iRandomZ = random(-3,3);
        
        
        if(iRandomX==0 &&iRandomZ ==0)
        {
            iRandomZ  =1;
        }
        int ix = x+iRandomX;
        int iz = z+iRandomZ;
        
        
        if(ix<0)
        {
            ix =0;
        }
        else if(ix>23)
        {
            ix = 23;
        }
        
        
        if(iz<-9)
        {
            iz = -9;
        }
        else if(iz>0)
        {
            iz = 0;
        }
        pt = Vec3(MAP_START_XPOS+ (ix)*48,0,iz*LENGTH_BLOCK_PER+STARTZPOINT);
        
        
        for(int i = 0; i < MAP_X_INDEX; i++)
        {
            for(int j = 0 ; j < MAP_Y_INDEX; j++)
            {
                
                if(m_allBlock[i][j].sprite&&m_allBlock[i][j].type!=0)
                {
                    Vec3  t = m_allBlock[i][j].sprite->getPosition3D();
                    
                    if(t==pt)
                    {
                        bTure = true;
                        break;
                        
                    }
                }
            }
            if( bTure == true)
            {
                break;
            }
        }
        
    }while(bTure);
    return pt;
}


locationInfo * pvpMainLayer::GetAllBlock()
{
    return  &m_allBlock[MAP_X_INDEX][MAP_Y_INDEX];
}
void pvpMainLayer::getUserDefenseVideoList(Ref* pSender)
{
    _netRecvEventGetDefenseVideoListInfo* pInfo = (_netRecvEventGetDefenseVideoListInfo*)pSender;
    int state = 1;
    std::set<videoListInfo*> pVideoList;
    videoListInfo pVideoInfo;
    for(std::vector<std::string>::iterator it = pInfo->m_paramsStr.begin(); it!= pInfo->m_paramsStr.end(); it++)
    {
        string str = *it;
        string name = CDataManager::getStrFromBackStr(str);
        if(name == "state")
        {
            state = atoi(CDataManager::getDigitFromBackStr(*it).c_str());
        }
        else if(name == "videoId")
        {
            pVideoInfo.videoId = atoi(CDataManager::getDigitFromBackStr(*it).c_str());
        }
        else if(name == "EnemyInfo")
        {
            std::string tempStr = CDataManager::getDigitFromBackStr(*it);
            unsigned long index =  tempStr.find(",");
            pVideoInfo.enemyUserId = atoi(tempStr.substr(0, index).c_str());
            tempStr = tempStr.substr(index + 1);
            index =  tempStr.find(",");
            pVideoInfo.enemyName = tempStr.substr(0, index);
            tempStr = tempStr.substr(index + 1);
            pVideoInfo.enemyLevel = atoi(tempStr.c_str());
            
        }
        else if(name == "EnemySoldier")
        {
            pVideoInfo.enemyInfo.clear();
            std::string tempStr = CDataManager::getDigitFromBackStr(*it);
            std::vector<__String> parameters = CDBMgr::separateInfoWithMark(tempStr, ";");
            for (int i=0; i < (int)parameters.size(); i++) {
                
                std::vector<__String> parameters2 = CDBMgr::separateInfoWithMark(parameters[i], ",");
                
                IdAndTwoNum  info ;
                info.theId = parameters2[0].intValue();
                info.level = parameters2[1].intValue();
                info.number = parameters2[2].intValue();
                pVideoInfo.enemyInfo.push_back(info);
            }
        }
        else if(name == "robStone")
        {
            pVideoInfo.robStone = atoi(CDataManager::getDigitFromBackStr(*it).c_str());
        }
        else if(name == "flag")
        {
            pVideoInfo.state = atoi(CDataManager::getDigitFromBackStr(*it).c_str());
        }
        else if(name == "uploadTime")
        {
            pVideoInfo.uploadTime = atoi(CDataManager::getDigitFromBackStr(*it).c_str());
        }
        else if(name == "isWin")
        {
            pVideoInfo.iWin = atoi(CDataManager::getDigitFromBackStr(*it).c_str());
        }
        else if(name == "isGet")
        {
            pVideoInfo.iGetWard = atoi(CDataManager::getDigitFromBackStr(*it).c_str());
        }
        else if(name == "destroyDegree")
        {
            pVideoInfo.destroyDegree = atoi(CDataManager::getDigitFromBackStr(*it).c_str());
            m_allVideoInfo.push_back(pVideoInfo);
        }
    }
    log("解析结束");
    int newVideoNum = 0;
    //重新排序
    getVideoListInfo();
    //找出最新的录像数量
    for(int i = 0; i < (int)m_allVideoInfo.size(); i++)
    {
        if(m_allVideoInfo[i].state == 0)
        {
            newVideoNum++;
        }
    }
    if(newVideoNum > 0)
    {
        Sprite* pVideoTip = Sprite::createWithSpriteFrameName("pvpTip.png");
        m_msgItem->addChild(pVideoTip, 1, 100);
        pVideoTip->setPosition(Vec2(m_msgItem->getNormalImage()->getBoundingBox().size.width * 0.7f,
                                    m_msgItem->getNormalImage()->getBoundingBox().size.height * 0.9f));
        
        __String* pBuf = __String::createWithFormat("%d", newVideoNum);
        Label* pLabelNum = Label::createWithTTF(pBuf->getCString(), "fonts/cuti.ttf", 20);
        pVideoTip->addChild(pLabelNum);
        pLabelNum->enableOutline(Color4B(0, 0, 0, 255), 1);
        pLabelNum->setPosition(Vec2(pVideoTip->getContentSize().width * 0.5f, pVideoTip->getContentSize().height * 0.5f));
    }
}

void pvpMainLayer::getVideoListInfo()
{
    std::vector<videoListInfo> videoListInfo = CDataManager::getVideoListInfo();
    for(int i = 0; i < (int)m_allVideoInfo.size(); i++)
    {
        videoListInfo.push_back(m_allVideoInfo.at(i));
    }
    m_allVideoInfo.clear();
    if(videoListInfo.size() <= 15)
    {
        m_allVideoInfo = videoListInfo;
    }
    else
    {
        int num = (int)videoListInfo.size() - 15;
        for(int k = 0; k < (int)videoListInfo.size(); k++)
        {
            bool bExisted = false;
            for(int j = 0; j < (int)m_allVideoInfo.size(); j++)
            {
                if(m_allVideoInfo[j].videoId == videoListInfo[k].videoId)
                {
                    bExisted = true;
                    break;
                }
            }
            if((!bExisted) && (k >= num))
            {
                m_allVideoInfo.push_back(videoListInfo[k]);
            }
        }
    }
    //根据videoId排序
    for(int i = 0; i < (int)m_allVideoInfo.size() - 1; i++)
    {
        struct::videoListInfo temp;
        for(int j = 0; j < (int)m_allVideoInfo.size() - 1 - i; j++)
        {
            if(m_allVideoInfo[j].videoId < m_allVideoInfo[j + 1].videoId)
            {
                temp = m_allVideoInfo[j];
                m_allVideoInfo[j] = m_allVideoInfo[j + 1];
                m_allVideoInfo[j + 1] = temp;
            }
        }
    }
    if(m_allVideoInfo.size() > 0)
    {
        CDataManager::saveVideoInfo(m_allVideoInfo);
    }
}


bool pvpMainLayer::init()
{
    if(!CBaseLayer::init())
    {
        return false;
    }
    m_listen = EventListenerTouchAllAtOnce::create();
    m_listen->onTouchesBegan = CC_CALLBACK_2(pvpMainLayer::onTouchesBegan, this);
    m_listen->onTouchesMoved = CC_CALLBACK_2(pvpMainLayer::onTouchesMoved, this);
    m_listen->onTouchesEnded = CC_CALLBACK_2(pvpMainLayer::onTouchesEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(m_listen, this);
    
    
    // m_allBuildObb.clear();
    
    //    EventListenerTouchAllAtOnce* listen = EventListenerTouchAllAtOnce::create();
    //    listen->onTouchesBegan =
    m_WinSize = Director::getInstance()->getWinSize();
    std::string fileName = "models/gameScene/theScene2.c3b";
    
    layer3D = Layer::create();
    addChild(layer3D,0,101);
    
    //    m_spriteBg = Sprite3D::create(fileName);
    //
    //    m_spriteBg->setPosition3D(Vec3(m_WinSize.width/2, -9,-350));
    //    m_spriteBg->setScale(1);
    //    layer3D->addChild(m_spriteBg);
    
    _camera = Camera::createPerspective(60, (GLfloat)m_WinSize.width/m_WinSize.height, 1, 2000);
    
    _camera->setCameraFlag(CameraFlag::USER1);
    //Vec3  VCamera(END3DPOINT,CAMERA_POSITION_Y,CAMERA_POSITION_Z); //350
    Vec3  VCamera(2700,514,114);
    _camera->setPosition3D(VCamera);
    _camera->lookAt(Vec3(2700,CAMERA_LOOK_POSITION_Y,CAMERA_LOOK_POSITION_Z), Vec3(0,1,0) );
    
    layer3D->addChild(_camera);
    layer3D->setCameraMask((unsigned short )CameraFlag::USER1);
    
    
    //FighterInfo  theInfo;
    
    //CDataManager::CalcFighterInfoWith(&theInfo,3, 1, 1,1);
    
    
    
    m_btnMenu = Menu::create();
    addChild(m_btnMenu, 10);
    m_btnMenu->setPosition(Vec2::ZERO);
    
    
    //添加随机背景音乐
    char buf[256];
    int randNum = CCRANDOM_0_1()*2+1;
    sprintf(buf, "music/backGround%d.mp3", randNum);
    if(UserDefault::getInstance()->getBoolForKey("bgMusic", true) && CPersonalData::getInstance()->getrestartPlayMusic())
    {
        SimpleAudioEngine::getInstance()->playBackgroundMusic(buf, true);
        UserDefault::getInstance()->setBoolForKey("bgMusic", true);
        SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(1.5f);
    }
    
    //加载升级动画
    Animation* pHammer = Animation::create();
    for(int i = 1; i < 9; i++)
    {
        sprintf(buf, "pvp_hammer_%d.png", i);
        pHammer->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(buf));
    }
    pHammer->setDelayPerUnit(0.1f);
    AnimationCache::getInstance()->addAnimation(pHammer, "hammer");
    
    //熔炼动画
    Animation* pRong = Animation::create();
    for(int i = 1; i < 11; i++)
    {
        sprintf(buf, "ronglian_%d.png", i);
        pRong->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(buf));
    }
    pRong->setDelayPerUnit(0.15f);
    AnimationCache::getInstance()->addAnimation(pRong, "RongLian");
    m_allBuildObb = pvpCommonData::getInstance()->m_allBuildObb;
    
    
    
    for (int i = 0; i < (int)m_allBuildObb.size(); i++) {
        pvpBuildInfo* pBuildInfo = m_allBuildObb[i].towerInfo->pInfo;
        CBaseBuilding* tower = CBaseTower::create(&m_allBuildObb[i].theInfo,eFightGroup_Freind);

        tower->ChangeModelWithPVPBuildingId(pBuildInfo->buildingID);
        tower->m_bIfActive = true;
        
        Vec3 p = Vec3(m_allBuildObb[i].xPos, 0, m_allBuildObb[i].zPos) + tower->gettheSprite()->getPosition3D();
        float height = tower->gettheSprite()->getBoundingBox().size.height;
        p.y = p.y+height*0.5f;
        Vec3 extents = Vec3(height * 0.35f, height * 0.35f, height * 0.35f + 10);
        if((m_allBuildObb[i].towerInfo->pInfo->buildingType == 7) || (m_allBuildObb[i].towerInfo->pInfo->buildingType == 8)
           || (m_allBuildObb[i].towerInfo->pInfo->buildingType == 12))
        {
            extents = Vec3(height * 0.35f + 15, height * 0.35f + 15, height * 0.35f + 10);
        }
        AABB aabb(-extents, extents);
        auto obb = OBB(aabb);
        obb._center = p;
        m_allBuildObb[i].towerInfo->theTower = tower;
        m_allBuildObb[i].obb = obb;
    }
    m_serverTime = pvpCommonData::getInstance()->getsetverTime();
    
    //保护时间
    m_protectedTime = pvpCommonData::getInstance()->getProtectedTime();
    std::string str = calculateTime(m_protectedTime);
    __String* temp = __String::createWithFormat("保护时间剩余:%s", str.c_str());
    m_protectLabel = Label::createWithTTF(temp->getCString(), "fonts/cuti.ttf", 30);
    addChild(m_protectLabel, 100);
    m_protectLabel->enableOutline(Color4B(0, 0, 0, 255), 3);
    m_protectLabel->setVisible(false);
    m_protectLabel->setPosition(Vec2(m_WinSize.width * 0.5f, m_WinSize.height * 0.08f));
    if(m_protectedTime > 0)
    {
        m_protectLabel->setVisible(true);
    }
    
    
    
    initUI();
    _camera->runAction(Sequence::create(EaseSineOut::create(MoveTo::create(0.48f, Vec3(2700, CAMERA_POSITION_Y, CAMERA_POSITION_Z))) ,
                                        CallFunc::create([=]{
                                scheduleUpdate();
                                m_pCurStoneNum->BeatUpTo(m_stone);
        
        
                        }) , NULL));
    
    char strMode[] = {16,18,19,20,22,23,25};
    
    for(int i=0;i<5;i++)
    {
        CDataManager::CalcFighterInfoWith(&m_theInfo,14, 1, 1, 0, false, 1, 0, 0, 0);
        
  
        int iRadom  = random(0,6);
        
        CNpcFighter *p = CNpcFighter::create(&m_theInfo,strMode[iRadom]);
        
        p->setPvpMainLayer(this);
        
        
        int iRandomX =   random(1, 22);
        int iRandomy =  random(-8,-1);
        
        
        int iDescX  =  MAP_START_XPOS+ (iRandomX)*48;
        int iDescY =   iRandomy*LENGTH_BLOCK_PER-200;
        
        p->SetDest(Vec3(iDescX,0,iDescY));
        
        addChild(p,999);
        
        iRandomX =   random(1, 22);
        iRandomy =  random(-8,-1);
        
        
        int iPosX  =  MAP_START_XPOS+ (iRandomX)*48;
        int iPosY =   iRandomy*LENGTH_BLOCK_PER-200;
        
        p->setPosition3D(Vec3(iPosX,0,iPosY));
        p->setCameraMask((unsigned short )CameraFlag::USER1);
        p->scheduleUpdate();
        
    }
    Sprite3D * m_pXian = Sprite3D::create();
    m_pXian->setPosition3D(Vec3(1680, 0,-203));
    m_pXian->setRotation3D(Vec3(-90,0,0));
    addChild(m_pXian,1000);
    
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
    
    
    
    //保存机器人地图按钮
    Sprite* maptest1 = Sprite::create("call_method.png");
    Sprite* maptest2 = Sprite::create("call_method.png");
    maptest2->setColor(Color3B::GRAY);
    
    
    MenuItemSprite* mapButton = MenuItemSprite::create(maptest1, maptest2,CC_CALLBACK_1(pvpMainLayer::saveTheBootMap, this));
    
    Menu* tempMapMenu = Menu::create(mapButton, NULL);
    

    tempMapMenu->setVisible(false);
    
    addChild(tempMapMenu,100);
    
    tempMapMenu->setPosition(Vec2(m_WinSize.width*0.5f, m_WinSize.height*0.1f));
    
    
    
    return true;
}

void pvpMainLayer::onEnter()
{
    CBaseLayer::onEnter();
    CNetControlModule::sharedNetControlModule()->getDefenseVideoList(this, menu_selector(pvpMainLayer::getUserDefenseVideoList));
    CFightData::getInstance()->restartLevel();//再次清理关卡数据
    CPersonalData::getInstance()->setTouchRmvFlag(false);
}

bool pvpMainLayer::dectectRongluGuide()
{
    //如果玩家当前阵营中有熔炉并且熔炉的引导还未完成
    bool exIsted = false;
    Vec3 targetPos = Vec3::ZERO;
    int index = 0;
    for(int i = 0; i < (int)m_allBuildObb.size(); i++)
    {
        pvpBuildInfo* pInfo = CDataManager::getBuildInfo(m_allBuildObb[i].buildingId);
        if((pInfo->buildingType == 2) && (m_allBuildObb[i].state == 0))
        {
            exIsted = true;
            index = i;
            break;
        }
    }
    if(exIsted && !CDataManager::getEndedBy(GUIDE_PVP_RONGLU))
    {
        //滑动界面使熔炉正好处于界面中心
        if(!pvpCommonData::getInstance()->getFollowBuild())
        {
            float towerPosX = m_allBuildObb[index].towerInfo->theTower->getPositionX();
            float cameraPosX = _camera->getPositionX();
            float xPos = cameraPosX - towerPosX;
            _camera->runAction(MoveBy::create(0.2f, Vec2(-xPos, 0)));
            
            Vec3 pos = m_allBuildObb[index].towerInfo->theTower->getPosition3D();
            targetPos = Vec3(pos.x + xPos, pos.y, pos.z);
            m_guidePos = CommonData::GetSingletonPtr()->getPosOnScreen(_camera, targetPos);
        }
        else{
            Vec3 pos = m_allBuildObb[index].towerInfo->theTower->getPosition3D();
            m_guidePos = CommonData::GetSingletonPtr()->getPosOnScreen(_camera, pos);
        }
        return true;
    }
    return false;
}

void pvpMainLayer::intFence(int buildId, float xPos, float zPos, float opcity, float rotate)
{
    Vec2 tempPos = CommonData::GetSingletonPtr()->getPosOnLayer3D(_camera, Vec2(xPos, zPos));//获取的屏幕中心点的3D坐标
    Vec3 Mid3DPos = Vec3(xPos, 0, zPos);

    FighterInfo theInfo;
    //pvpBuildInfo* pInfo = CDataManager::getBuildInfo(buildId);
    monsterInfo* pMonster = CDataManager::getMonsterInfoByMonsterId(buildId);
    modelInfo* pMondelInfo = CDataManager::getModelInfoByModelId(pMonster->modelId);
    Sprite3D* tower = Sprite3D::create(pMondelInfo->modelName.getCString());
    layer3D->addChild(tower);
    tower->setOpacity(opcity);
    tower->setScale(58);
    tower->setRotation3D(Vec3(0, rotate, 0));
    if(opcity > 200)
    {
        tower->setScaleY(50);
    }
    else {
        tower->setScaleY(30);
    }
    tower->setCameraMask((unsigned short )CameraFlag::USER1);
    tower->setPosition3D(Mid3DPos);
}

void pvpMainLayer::initUI()
{
    //初始化背景
    Sprite3D* tempTarget = Sprite3D::create();
    Sprite* temp = Sprite::create("models/gameScene/fightBG_Snow1_1.png");
    temp->setAnchorPoint(Vec2(0.5f, 0));
    temp->setScale(2);
    
    tempTarget->addChild(temp);
    
    
    CFightManager::addCamera(tempTarget);
    
    tempTarget->setRotation3D(Vec3(-60, 0, 0));
    tempTarget->setPosition3D(Vec3(temp->getBoundingBox().size.width*0.70,-1,-800));
    
    layer3D->addChild(tempTarget);
    
    tempTarget->setForceDepthWrite(true);
    
    Sprite3D* tempTarget2 = Sprite3D::create();
    Sprite* temp2 = Sprite::create("models/gameScene/fightBG_Snow1_2.png");
    temp2->setAnchorPoint(Vec2(0.5f, 0));
    temp2->setScale(2);
    
    tempTarget2->addChild(temp2);
    
    CFightManager::addCamera(tempTarget2);
    
    tempTarget2->setRotation3D(Vec3(90, 0, 0));  //依然是那个坑，如果旋转角度为-90度，就会出现渲染错误
    tempTarget2->setPosition3D(Vec3(tempTarget->getPositionX(), tempTarget->getPositionY(), tempTarget->getPosition3D().z));
    
    
    layer3D->addChild(tempTarget2);
    
    
    //初始化一个10X8的坐标系
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
            locationInfo info;
            info.sprite = sprite;
            info.type = 0;
            m_allBlock[x][z + 9] = info;
            
            m_blockXPointVec.insert(x * LENGTH_BLOCK_PER + MAP_START_XPOS);
            m_blockyPointVec.insert(z * LENGTH_BLOCK_PER + STARTZPOINT);
            float xPos = sprite->getPositionX() + sprite->getChildByTag(100)->getBoundingBox().size.width * 0.3f;
            m_blockxLineVec.insert(xPos);
            
            float zPos = sprite->getPositionZ() + sprite->getChildByTag(100)->getBoundingBox().size.height * 0.3f;
            m_blockyLineVec.insert(zPos);
        }
    }

    //添加16个城墙 在主基地四周
    float xPos = 0;
    float zPos = 0;
//    for(int i = 0; i < 25; i++)
//    {
//        for(int j = -10; j <= 1; j++)
//        {
//            if((i == 24) || (( j == -10) || ( j == 1)))
//            {
//                xPos = MAP_START_XPOS + i * LENGTH_BLOCK_PER;
//                zPos = STARTZPOINT + j * LENGTH_BLOCK_PER;
//                intFence(121, xPos, zPos);
//            }
//        }
//    }
    
    for(int i = 0;i < 12; i++)
    {
        xPos = MAP_START_XPOS + (0.5f + i * 2) * 49;
        zPos = STARTZPOINT + (-10) * LENGTH_BLOCK_PER;
        intFence(1006, xPos, zPos, 0.6 * 255);
    }

    for(int i = 0;i < 5; i++)
    {
        xPos = MAP_START_XPOS - 5 + 12 * 2 * LENGTH_BLOCK_PER;
        zPos = STARTZPOINT + (-9) * LENGTH_BLOCK_PER + (0.48f + i * 2) * LENGTH_BLOCK_PER;
        intFence(1006, xPos, zPos, 0.7 * 255, 90);
    }

    for(int i = 0;i < 12; i++)
    {
        xPos = MAP_START_XPOS + (0.5f + i * 2) * 49;
        zPos = STARTZPOINT + LENGTH_BLOCK_PER;
        intFence(1006, xPos, zPos, 0.8 * 255);
    }
    if(m_allBuildObb.size() == 0)
    {
        //初始化主基地
        initTowerInfo(1);
    }
    else//根据服务器返回的阵营信息自动排布
    {
        for(int i = 0; i < (int)m_allBuildObb.size(); i++)
        {
            m_curBuild = m_allBuildObb[i].towerInfo;
            Vec3 pos = Vec3(m_allBuildObb[i].xPos, 0, m_allBuildObb[i].zPos);
            //初始化建筑底
            std::string bgImg;
            float scale = 1.0f;
            if(m_curBuild->pInfo->blockPos.size() == 1)
            {
                bgImg = "pvp_one.png";
                scale = 1.1f;
            }
            else if(m_curBuild->pInfo->blockPos.size() == 4)
            {
                bgImg = "pvp_four.png";
                scale = 2.0f;
            }
            else if(m_curBuild->pInfo->blockPos.size() == 9)
            {
                bgImg = "pvp_nine.png";
                scale = 3.2f;
            }
            
            
            //影子shadow
            Sprite3D* pShadow = CFightManager::getTheModelWithPath("touying.png");
            layer3D->addChild(pShadow, 100);
            pShadow->getChildByTag(100)->setScale(scale);
            pShadow->getChildByTag(100)->setOpacity(170);
            pShadow->setRotation3D(Vec3(-90,0,0));
            pShadow->setPosition3D(Vec3(pos.x, pos.y, pos.z));
            pShadow->setCameraMask((unsigned short )CameraFlag::USER1);
            
            Sprite3D* TowerBg = CFightManager::getTheModelWithPath(bgImg.c_str());
            layer3D->addChild(TowerBg);
            TowerBg->setPosition3D(pos);
            TowerBg->setRotation3D(Vec3(270, 0, 0));
            TowerBg->getChildByTag(100)->setScale(0.6f);
            TowerBg->setCameraMask((unsigned short )CameraFlag::USER1);
            
            if(m_curBuild->pInfo->buildingType == 0)
            {
                m_pCircle = CFightManager::getTheModelWithPath("fight_CityCircle2.png");
                layer3D->addChild(m_pCircle, 100);
                m_pCircle->getChildByTag(100)->setScale(1.25f);
                m_pCircle->setRotation3D(Vec3(-90, 0, 0));
                m_pCircle->setPosition3D(Vec3(pos.x, pos.y + 1, pos.z));
                m_pCircle->setCameraMask((unsigned short )CameraFlag::USER1);
                //m_pCircle->getChildByTag(100)->runAction(RepeatForever::create(RotateBy::create(1.0f, 20)));
            }
            
            m_allBuildObb[i].towerInfo->towerBase = TowerBg;
            m_allBuildObb[i].towerInfo->shadow = pShadow;
            m_curBuild->towerBase = TowerBg;
            m_curBuild->shadow = pShadow;
            
            layer3D->addChild(m_curBuild->theTower);
            m_curBuild->theTower->setPosition3D(Vec3(pos.x, pos.y + 1, pos.z));
            m_curBuild->theTower->setCameraMask((unsigned short )CameraFlag::USER1);
            
            if(m_curBuild->pInfo->buildingType == 0)
            {
                m_friendTowerLevel = m_curBuild->pInfo->level;
                CPersonalData::getInstance()->getUserInfo()->m_mainBaseLevel = m_curBuild->pInfo->level;
            }
            
            else if(m_curBuild->pInfo->buildingType == 11)//如果是祭坛则添加怪物
            {
                if(!((m_allBuildObb[i].state != 0) && (m_curBuild->pInfo->level == 1)))
                {
                    addPattolInfo(m_curBuild);
                }
            }
            
            for(int i = 0; i < (int)(m_curBuild->pInfo->blockPos.size()); i++)
            {
                float xPos = m_curBuild->theTower->getPositionX() + m_curBuild->pInfo->blockPos[i].x * LENGTH_BLOCK_PER;
                float zPos = m_curBuild->theTower->getPositionZ() - m_curBuild->pInfo->blockPos[i].y * LENGTH_BLOCK_PER;
                int index = (xPos - MAP_START_XPOS)/LENGTH_BLOCK_PER;
                int indey = (zPos - STARTZPOINT)/LENGTH_BLOCK_PER + 9;
                m_allBlock[index][indey].type = 1;
                m_allBlock[index][indey].sprite->setVisible(false);
            }
            if((m_allBuildObb[i].state == 3) && (m_curBuild->pInfo->buildingType == 2))//如果是正在熔炼的熔炉
            {
                //发送请求获取熔炉是否已经熔连接束
                m_productTower = m_curBuild;
                m_productBuildId = m_allBuildObb[i].userBuildingId;
                __String* pBuf = __String::createWithFormat("%d", m_allBuildObb[i].userBuildingId);
                CNetControlModule::sharedNetControlModule()->isEndProduct(this, menu_selector(pvpMainLayer::productInfoBack), pBuf->getCString());
            }
            else  if(m_allBuildObb[i].state == 2)//表示正在升级
            {
                m_upgradeBuildId = m_allBuildObb[i].buildingId;
                m_upgradeTower = m_curBuild;
                int continueTime = m_serverTime - m_allBuildObb[i].startUpTime;//计算自从赏析升级之后持续的时间
                
                //获取上一级的建筑信息
                pvpBuildInfo* preLevelInfo = CDataManager::getBuildInfo(m_upgradeBuildId);
                if(continueTime >= preLevelInfo->costTime)
                {
                    m_upgradeEnd = true;
                    m_allBuildObb[i].state = 0;
                    log("上次开始升级的时间%d", m_allBuildObb[i].startUpTime);
                    sendRequestSaveTeamInfo();
                }
                else
                {
                    m_remainTime = preLevelInfo->costTime - continueTime;
                    addUpgradeInfo(true);
                }
            }
        }
        m_curBuild = NULL;
    }
    if(CPersonalData::getInstance()->getFromVideoList())
    {
        CPersonalData::getInstance()->setFromVideoList(false);
        
        Label* msgLabel  = NULL;
        
        if(m_msgItem)
        {
            msgLabel = (Label*)m_msgItem->getChildByTag(100);
        }
        
        
        if(msgLabel)
        {
            msgLabel->removeFromParent();
            msgLabel = NULL;
        }
        pvpWarReportLayer* layer = pvpWarReportLayer::create();
        addChild(layer, 1000);
    }
    
    //添加引导
    if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_PVP_BUILDING) && (guiderManagerLayer::getInstance()->getStepNum() == 1))
    {
        guiderManagerLayer::getInstance()->setStepNum(guiderManagerLayer::getInstance()->getStepNum() + 1);
        guiderManagerLayer::getInstance()->createLayer(this, GUIDE_SHOW_DIALOG, Vec2::ZERO, false);
        m_iBuildUseType = 1;
    }
    if(!CDataManager::getEndedBy(GUIDE_PVP_RONGLU))
    {
        pvpCommonData::getInstance()->setFollowBuild(false);
    }
    adduserInfo();
    detectIfCanCreate();
    detectPvpGuideInfo();
}

void pvpMainLayer::detectPvpGuideInfo()
{
    //pvp引导，测试用
    if(dectectRongluGuide())
    {
        guiderManagerLayer::getInstance()->setGuideType(GUIDE_PVP_RONGLU);
        guiderManagerLayer::getInstance()->setStepNum(1);
        Vec2 pos = Vec2(m_winSize.width * 0.9f, m_WinSize.height * 0.95);
        guiderManagerLayer::getInstance()->createLayer(this, GUIDE_SHOW_BOTH, pos);
    }
    if(CDataManager::getEndedBy(GUIDE_PVP_RONGLU) && !(CDataManager::getEndedBy(GUIDE_PVP_ROB)))
    {
        guiderManagerLayer::getInstance()->setGuideType(GUIDE_PVP_ROB);
        guiderManagerLayer::getInstance()->setStepNum(1);
        guiderManagerLayer::getInstance()->createLayer(this);
    }
    if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_PVP_ROB) && (guiderManagerLayer::getInstance()->getStepNum() == 7))
    {
        guiderManagerLayer::getInstance()->setStepNum(guiderManagerLayer::getInstance()->getStepNum() + 1);
        guiderManagerLayer::getInstance()->createLayer(this);
    }
}

void pvpMainLayer::productInfoBack(Ref* pSender)
{
    _netRecvEventIsEndProduct* pInfo = (_netRecvEventIsEndProduct*)pSender;
    int state = 1;
    for(std::vector<std::string>::iterator it = pInfo->m_paramsStr.begin(); it!= pInfo->m_paramsStr.end(); it++)
    {
        string str = *it;
        string name = CDataManager::getStrFromBackStr(str);
        
        if(name == "state")
        {
            state = atoi(CDataManager::getDigitFromBackStr(*it).c_str());
            if(state != 0)
            {
                log("判断熔炼是否结束");
                return;
            }
        }
        else if (name == "retTime")
        {
            m_productRemainTime = atoi(CDataManager::getDigitFromBackStr(*it).c_str());
        }
        else if(name == "retStone")
        {
            m_productNum = atoi(CDataManager::getDigitFromBackStr(*it).c_str());
        }
        else if(name == "productTime")
        {
            m_productNeedTime = atoi(CDataManager::getDigitFromBackStr(*it).c_str());
        }
    }
    if(m_productRemainTime > 0)
    {
        addProductInfo();
    }
    else{
        
        saveRongluState(m_productBuildId, true);
        SimpleAudioEngine::getInstance()->playEffect("music/task.mp3");
        __String* pBuf = __String::createWithFormat("熔炼成功，获得%d魔法石", m_productNum);
        
        Sprite * thetarget = CFightManager::getTheFlyWord(pBuf->getCString(), "fonts/cuti.ttf");
        thetarget->setPosition(m_winSize.width*0.5,m_winSize.height*0.5);
        addChild(thetarget,100);
        
        for(int i = 0; i < 5; i++)
        {
            //获取随机值(半径为50内的圆内)
            float addDistacneX = rand()%201 - 100;
            float addDistacneY = rand()%201 - 100;
            Sprite* pStone = Sprite::createWithSpriteFrameName("stone.png");
            pStone->setScale(0);
            addChild(pStone,105);
            Vec3 targetPos = m_productTower->theTower->getPosition3D();
            Vec2 pos = CommonData::GetSingletonPtr()->getPosOnScreen(_camera, targetPos);
            pStone->setPosition(pos);
            ActionInterval* pAction1 = Spawn::create(MoveBy::create(0.35f, Vec2(addDistacneX, addDistacneY)), ScaleTo::create(0.2f, 0.8f), NULL);
            
            MoveTo* pMoveAction = MoveTo::create(1.2f, Vec2(m_WinSize.width * 0.64, m_winSize.height * 0.945f));
            ScaleTo* pScaleAction = ScaleTo::create(1.2, 0.3);
            ActionInterval* pAction2 = EaseExponentialIn::create(pMoveAction);
            pStone->runAction(Sequence::create(pAction1, DelayTime::create(0.2f), Spawn::create(pAction2, pScaleAction, NULL), RemoveSelf::create(), CallFunc::create([=]{
                CNetWorkDataSingle::GetSingletonPtr()->SaveUserMCF(this,menu_selector(pvpWarReportLayer::updateUserData),0, m_productNum,0);
            }), NULL));
        }
        //CNetWorkDataSingle::GetSingletonPtr()->SaveUserMCF(this,menu_selector(pvpWarReportLayer::updateUserData),0,m_productNum,0);
    }
}

void pvpMainLayer::update(float dt)
{
    char buf[128] = {0};
    
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
        int tempTime = CPhysicalSystem::GetSingletonPtr()->GetNextUpTime();
        
        if (tempTime!=m_bufferIime) {
            m_bufferIime = tempTime;
            sprintf(buf, "%02d:%02d", m_bufferIime/60, m_bufferIime%60);
            m_pTime->setString(buf);
        }
    }
    
    int iStone = CPersonalData::getInstance()->getUserKuangStone();
    
    if(m_bAddPer)
    {
        
        m_fAllPerTime +=  dt;
        
        m_fAllPerTime = MIN(m_fAllPerTime, m_fSecPass);
    
        float fNowPer = m_fAllPerTime/m_fSecPass*(m_fEndPercent-m_fStartPercent)+m_fStartPercent;
    
        if(abs(fNowPer-m_fEndPercent)>0.01)
        {
            
        }
        else
        {
            fNowPer =m_fEndPercent;
            m_bAddPer = false;
           
        }
        
        m_progress->setPercentage(fNowPer);
    
    }
    
    
    if (CPersonalData::getInstance()->getUpdateUserInfo())
    {
        
        char buf[256];
        sprintf(buf, "%d", CPersonalData::getInstance()->getUserCoin());
        m_pCoin->setString(buf);
        sprintf(buf, "%d", CPersonalData::getInstance()->getUserStone());
        m_pStone->setString(buf);
        int maxStamina = CPersonalData::getInstance()->GetPhysicalMaxNum();
        sprintf(buf, "%d", CPersonalData::getInstance()->GetPhysicalNum());
        m_pStaminaLabel->setString(buf);
        sprintf(buf, "/%d", maxStamina);
        m_pStaminaMaxLabel->setString(buf);
        CPersonalData::getInstance()->setUpdateUserInfo(false);
    }
    
    /*if(CPersonalData::getInstance()->getUpdateKuanshiInfo())
    {
         m_pCurStoneNum->setBeatTime(m_fSecPass);
        
        if(m_bMinusPer )
        {
            m_pCurStoneNum->BeatDownTo(iStone);
        }
        else
        {
            m_pCurStoneNum->BeatUpTo(iStone,iStone);
        }
        
        CPersonalData::getInstance()->setUpdateKuanshiInfo(false);
        m_bAddPer = true;
        
        m_fEndPercent = iStone*1.0/CPersonalData::getInstance()->getUserMaxKuangStone()*100;
        m_fStartPercent = m_progress->getPercentage();
        m_fSecPass = abs(m_fEndPercent-m_fStartPercent)/40;
        m_fAllPerTime = 0;
        
    }*/
    
    //如果当前矿石储量已经达到上限则停止生产
    if(CPersonalData::getInstance()->getUserKuangStone() < CPersonalData::getInstance()->getUserMaxKuangStone())
    {
        for(int i = 0; i < (int)m_pKuanchanBuildVec.size();i++)
        {
            if(m_pKuanchanBuildVec[i].state == 0)
            {
                pvpBuildInfo* pBuildingInfo = CDataManager::getBuildInfo(m_pKuanchanBuildVec[i].buildingId);
                int iTime = pBuildingInfo->effectTime;
                float fAddSec = (float)iTime / 1200;//计算每5分钟的产量
                
                
                if(m_pKuanchanBuildVec[i].validTime == -1)//可领取
                {
                    LYMenuItemSprite *p =  (LYMenuItemSprite *)m_btnMenu->getChildByTag(PVP_BUILDING_KUANSHI);
                    if(!p)
                    {
                        Vec3  tPos= m_pKuanchanBuildVec[i].towerInfo->theTower->getPosition3D();
                        Vec2 targetPos = CommonData::GetSingletonPtr()->getPosOnScreen(_camera, tPos);
                        float height =  m_pKuanchanBuildVec[i].towerInfo->theTower->getBoundingBox().size.height * 1.5f;
                        Sprite *pKuan = Sprite::createWithSpriteFrameName("pvp_product.png");
                        Sprite *pKuanSelect = Sprite::createWithSpriteFrameName("pvp_product.png");
                        //pKuanSelect->setColor(Color3B::GRAY);
                        LYMenuItemSprite *pGetKuang  = LYMenuItemSprite::create(pKuan,pKuanSelect,CC_CALLBACK_1(pvpMainLayer::upgradeBtnCallBack,this));
                        pGetKuang->setScale(0.5);
                        
                        pGetKuang->setPosition(Vec2(targetPos.x,targetPos.y+height+100));
                        m_btnMenu->addChild(pGetKuang,999,PVP_BUILDING_KUANSHI);
                        pGetKuang->setUserData((void*)(m_pKuanchanBuildVec[i].userBuildingId));
                        m_stoneVec.push_back(pGetKuang);
                    }
                }
                else
                {
                    //如果当前矿石数量已经达到上限则停止生产
                    
                    m_fKuanShiAllTime+= dt;
                    
                    if(m_fKuanShiAllTime >= 300)
                    {
                        m_fAddStone += fAddSec;
//                        if(m_fAddStone>= m_stoneRate * 0.08)
//                        {
//                            m_pKuanchanBuildVec[i].validTime = -1;
//                            m_fAddStone = 0;
//                        }
                        m_pKuanchanBuildVec[i].validTime = -1;
                        m_fAddStone = 0;
                        m_fKuanShiAllTime = 0;
                    }
                }
            }
        }
    }
    if(m_remainTime > 0)
    {
        m_time+=dt;
        if(m_time >= 1)
        {
            m_remainTime--;
            m_time -= 1;
            if(m_timeProgressBg && m_pTimeLabel && (m_remainTime > 0))//不显示0
            {
                std::string str = calculateTime(m_remainTime);
                m_pTimeLabel->setString(str);
                
                pvpBuildInfo* pBuildingInfo = CDataManager::getBuildInfo(m_upgradeBuildId);
                float percent = (1-(float)m_remainTime/pBuildingInfo->costTime)*100;
                m_timeProgress->setPercentage(percent);
                
            }
        }
    }
    else
    {
        if(m_timeProgressBg && !m_upgradeEnd)
        {
            m_upgradeEnd = true;
            //获取服务器时间，判断当前是否真的已经升级结束
            CNetControlModule::sharedNetControlModule()->getServerTime(this, menu_selector(pvpMainLayer::adjustUpgradeEnd));
        }
        else if(m_upgradeEnd)//防止升级完成时移动建筑数据为赋值结束bug
        {
            rmvUpgradeInfo();
            for(int i = 0; i < m_allBuildObb.size(); i++)
            {
                if(m_upgradeTower->theTower == m_allBuildObb[i].towerInfo->theTower)
                {
                    m_allBuildObb[i].state = 0;
                    if((m_upgradeTower->pInfo->buildingType == 11) && (m_upgradeTower->pInfo->level == 1))//如果是祭坛则添加怪物
                    {
                        addPattolInfo(m_upgradeTower);
                    }
                    if(m_upgradeTower->pInfo->buildingType == 10)
                    {
                        updateKuangchangState(m_upgradeTower->theTower);
                        updateStoneRate();
                    }
                    if(m_upgradeTower->pInfo->buildingType == 1)//如果升级的是矿石库泽发送消息更新矿石上限
                    {
                        updateKuangshiLimit();
                    }
                    if(m_upgradeTower->pInfo->buildingType == 0)
                    {
                        detectIfCanCreate();
                        CDataManager::saveTaskInfoToDB(20, m_upgradeTower->pInfo->level, true);
                    }
                    if(m_upgradeTower)
                    {
                        m_upgradeTower->theTower->ChangeModelWithPVPBuildingId(m_upgradeBuildId);
                    }
                }
            }
        }
    }
    if(m_productRemainTime > 0)
    {
        m_fTime+=dt;
        if(m_fTime >= 1)
        {
            m_productRemainTime--;
            m_fTime -= 1;
            if(m_productProgressBg && m_productTimeLabel && (m_productRemainTime > 0))//不显示0
            {
                std::string str = calculateTime(m_productRemainTime);
                m_productTimeLabel->setString(str);
                
                pvpBuildInfo* pBuildingInfo = CDataManager::getBuildInfo(m_upgradeBuildId);
                float percent = (float)(m_productNeedTime - m_productRemainTime)/m_productNeedTime*100;
                m_productProgress->setPercentage(percent);
                
            }
        }
    }
    else{
        if(m_productProgressBg && !m_bProductEnd && m_productBuildId > 0)
        {
            m_bProductEnd = true;
            //判断当前是否真的已经熔炼结束
            __String* pBuf = __String::createWithFormat("%d", m_productBuildId);
            CNetControlModule::sharedNetControlModule()->isEndProduct(this, menu_selector(pvpMainLayer::isEndProductBack), pBuf->getCString());
        }
    }
    if(m_protectedTime > 0)
    {
        m_limitTime+=dt;
        if(m_limitTime > 1)
        {
            m_protectedTime--;
            m_limitTime -= 1;
        }
        if(m_protectLabel && (m_protectedTime > 0))
        {
            std::string str = calculateTime(m_protectedTime);
            str = "保护时间剩余:" + str;
            m_protectLabel->setString(str);
        }
    }
    else
    {
        if(m_protectLabel)
        {
            m_protectLabel->removeFromParent();
            m_protectLabel = NULL;
        }
    }
    if(m_patrolman && (m_patrolman->getMonsterId() != 14))//虚化,如果是龙则不虚化
    {
        Vec3 pos = m_patrolman->getPosition3D();
        int index = (pos.x - MAP_START_XPOS)/LENGTH_BLOCK_PER;
        int indey = (pos.z - STARTZPOINT)/LENGTH_BLOCK_PER + 9;
        
        for(int i = 0; i < (int)m_allBuildObb.size(); i++)
        {
            pvpBuildInfo* pBuildInfo = m_allBuildObb[i].towerInfo->pInfo;
            for(int j = 0 ; j < (int)pBuildInfo->blockPos.size(); j++)
            {
                int tempX = (m_allBuildObb[i].xPos - MAP_START_XPOS)/LENGTH_BLOCK_PER;
                int tempy = (m_allBuildObb[i].zPos - STARTZPOINT)/LENGTH_BLOCK_PER + 9;
                if((fabs(tempX - index) <= 1) && (tempy - indey <= 1) && ( tempy >= indey))
                {
                    m_allBuildObb[i].towerInfo->theTower->gettheSprite()->setOpacity(100);
                }
                else {
                    m_allBuildObb[i].towerInfo->theTower->gettheSprite()->setOpacity(255);
                }
            }
        }
    }
}

void pvpMainLayer::startProduct(int time)
{
    
    m_fEndPercent =  CPersonalData::getInstance()->getUserKuangStone()*1.0/CPersonalData::getInstance()->getUserMaxKuangStone()*100;
    m_fStartPercent = m_progress->getPercentage();
    m_fSecPass = abs(m_fEndPercent-m_fStartPercent)/40;
    m_fAllPerTime = 0;
    

    m_bAddPer = true;

    
    int iStone = CPersonalData::getInstance()->getUserKuangStone();
    
    m_pCurStoneNum->setBeatTime(m_fSecPass);
    m_pCurStoneNum->BeatDownTo(iStone);
    
    
    m_productTower = m_curBuild;
    m_productRemainTime = time;
    m_productNeedTime = time;
    //    m_curBuild = NULL;
    addProductInfo();
}

void pvpMainLayer::isEndProductBack(Ref *pSender)
{
    int remainTime = 0;
    int needTime = 0;
    int stone = 0;
    if(CPersonalData::getInstance()->getState() == 0)
    {
        remainTime = pvpCommonData::getInstance()->getRemainTime();
        needTime = pvpCommonData::getInstance()->getProductTime();
        stone = pvpCommonData::getInstance()->getProductGetStone();
    }
    
    if(remainTime > 0)
    {
        m_productRemainTime = remainTime;
        m_bProductEnd = false;
    }
    else{
        saveRongluState(m_productBuildId, true);
        if(m_productProgressBg)
        {
            m_productProgressBg->removeFromParent();
            m_productProgressBg = NULL;
        }
        
        SimpleAudioEngine::getInstance()->playEffect("music/task.mp3");
        __String* pBuf = __String::createWithFormat("熔炼成功，获得%d魔法石", m_productNum);
        
        Sprite * thetarget = CFightManager::getTheFlyWord(pBuf->getCString(), "fonts/cuti.ttf");
        thetarget->setPosition(m_winSize.width*0.5,m_winSize.height*0.5);
        addChild(thetarget,100);
        CNetWorkDataSingle::GetSingletonPtr()->SaveUserMCF(this,NULL,0,m_productNum,0);
        for(int i = 0; i < 5; i++)
        {
            //获取随机值(半径为50内的圆内)
            float addDistacneX = rand()%201 - 100;
            float addDistacneY = rand()%201 - 100;
            Sprite* pStone = Sprite::createWithSpriteFrameName("stone.png");
            pStone->setScale(0);
            addChild(pStone,105);
            Vec3 targetPos = m_productTower->theTower->getPosition3D();
            Vec2 pos = CommonData::GetSingletonPtr()->getPosOnScreen(_camera, targetPos);
            pStone->setPosition(pos);
            ActionInterval* pAction1 = Spawn::create(MoveBy::create(0.35f, Vec2(addDistacneX, addDistacneY)), ScaleTo::create(0.2f, 0.8f), NULL);
            
            MoveTo* pMoveAction = MoveTo::create(1.2f, Vec2(m_WinSize.width * 0.64, m_winSize.height * 0.945f));
            ScaleTo* pScaleAction = ScaleTo::create(1.2, 0.3);
            ActionInterval* pAction2 = EaseExponentialIn::create(pMoveAction);
            pStone->runAction(Sequence::create(pAction1, DelayTime::create(0.2f), Spawn::create(pAction2, pScaleAction, NULL),
                                               RemoveSelf::create(),NULL));
        }
        
        //CNetWorkDataSingle::GetSingletonPtr()->SaveUserMCF(this,menu_selector(pvpWarReportLayer::updateUserData),0,m_productNum,0);
        runAction(Sequence::create(DelayTime::create(1.75f), CallFunc::create([=]{
            updateUserData(NULL);
            m_productRemainTime = 0;
            m_productTower = NULL;
            m_bProductEnd = false;
        }), NULL));
    }
}

void pvpMainLayer::smeltSuccessCallBack(Ref* pSender)
{
    
}

void pvpMainLayer::getValidLocation(Vec2 touchPos)
{
    //位置类型
    float xMinValue = 100000;
    float yMinValue = 100000;
    Vec2 pos = CommonData::GetSingletonPtr()->getPosOnLayer3D(_camera, touchPos);
    Vec3 touch = Vec3(pos.x, 0, pos.y);
    
    float xValue = 0;
    float yValue = 0;//记录离触摸点最近的z轴坐标
    if(m_curBuild->pInfo->xPosType == 0)
    {
        for(std::set<float>::iterator it = m_blockXPointVec.begin(); it != m_blockXPointVec.end(); it++)
        {
            float value = fabsf(touch.x - *it);
            if(value < xMinValue)
            {
                xMinValue = value;
                xValue = *it;
            }
        }
    }
    else
    {
        for(std::set<float>::iterator it = m_blockxLineVec.begin(); it != m_blockxLineVec.end(); it++)
        {
            float value = fabsf(touch.x - *it);
            if(value < xMinValue)
            {
                xMinValue = value;
                xValue = *it;
            }
        }
    }
    if(m_curBuild->pInfo->yPosType == 0)
    {
        for(std::set<float>::iterator it = m_blockyPointVec.begin(); it != m_blockyPointVec.end(); it++)
        {
            float value = fabsf(touch.z - *it);
            if(value < yMinValue)
            {
                yMinValue = value;
                yValue = *it;
            }
        }
    }
    else
    {
        for(std::set<float>::iterator it = m_blockyLineVec.begin(); it != m_blockyLineVec.end(); it++)
        {
            float value = fabsf(touch.z - *it);
            if(value < yMinValue)
            {
                yMinValue = value;
                yValue = *it;
            }
        }
    }
    //向下计算出放置塔的位置 如果是正方向的时候
    if((yMinValue < 100000) && (xMinValue < 100000))
    {
        
        int type = adjustIsTake(xValue, yValue);//先遍历一遍发现有已被占用则直接全部显示红色
        if(type == 2)
        {
            //m_bValid = false;
            return;
        }
        rmvSelectedBlock();
        
        log("建筑的最佳位置 x = %f, y = %f", xValue, yValue);
        
        for(int i = 0; i < (int)(m_curBuild->pInfo->blockPos.size()); i++)
        {
            float xPos = xValue + m_curBuild->pInfo->blockPos[i].x * LENGTH_BLOCK_PER;
            float zPos = yValue + m_curBuild->pInfo->blockPos[i].y * LENGTH_BLOCK_PER;
            Sprite3D* sp = CFightManager::getTheModelWithPath("pvpGreenBlock.png");
            //判断当前放置的位置是否被占用
            if(type == 1)
            {
                //m_bValid = false;
                sp = CFightManager::getTheModelWithPath("pvpRedBlock.png");
            }
            sp->setScale(0.6f);
            sp->setPosition3D(Vec3(xPos, 1, zPos));
            sp->setRotation3D(Vec3(90, 0, 0));
            layer3D->addChild(sp);
            m_selectedBlock.push_back(sp);
            sp->setCameraMask((unsigned short )CameraFlag::USER1);
        }
        m_curBuild->theTower->setPosition3D(Vec3(xValue, 0, yValue));
        if(m_bActive && m_curBuild && m_bMove)
        {
            m_curBuild->theTower->gettheSprite()->setOpacity(100);
            addArrow();
        }
    }
}

int pvpMainLayer::adjustIsTake(float xValue, float yValue)
{
    int flag = 0;
    m_bValid = true;
    
    for(int i = 0; i < (int)(m_curBuild->pInfo->blockPos.size()); i++)
    {
        float xPos = xValue + m_curBuild->pInfo->blockPos[i].x * LENGTH_BLOCK_PER;
        float zPos = yValue - m_curBuild->pInfo->blockPos[i].y * LENGTH_BLOCK_PER;
        //判断当前放置的位置是否被占用
        int index = (xPos - MAP_START_XPOS)/LENGTH_BLOCK_PER;
        int indey = (zPos - STARTZPOINT)/LENGTH_BLOCK_PER + 9;
        
        log("当前索引x = %d", index);
        if(index < 0 || indey < 0 || index >= MAP_X_INDEX || indey >= MAP_Y_INDEX)
        {
            flag = 2;
            m_bValid = false;
            break;
        }
        else if(m_allBlock[index][indey].type == 1)
        {
            flag = 1;
            m_bValid = false;
        }
    }
    return flag;
}

void pvpMainLayer::initTowerInfo(int buildId)
{
    //先初始化主基地
    cocos2d::Vec2 midPoint = Vec2(m_WinSize.width * 0.5f, m_WinSize.height * 0.5f);
    FighterInfo theInfo;
    pvpBuildInfo* pInfo = CDataManager::getBuildInfo(buildId);
    CDataManager::CalcFighterInfoWith(&theInfo, pInfo->monsterId, pInfo->level, 1, 0, false, 1, 0, 0, 0);
    
    m_curBuild = new buildInfo();
    m_curBuild->theTower = CBaseTower::create(&theInfo,eFightGroup_Freind);
    m_curBuild->theTower->ChangeModelWithPVPBuildingId(buildId);
    m_curBuild->theTower->m_bIfActive = true;
    CFightData::getInstance()->addMonster(m_curBuild->theTower,eAttackType_tower);
    layer3D->addChild(m_curBuild->theTower);
    m_curBuild->theTower->setVisible(true);
    
    
    Vec2 tempPos = CommonData::GetSingletonPtr()->getPosOnLayer3D(_camera, midPoint);//获取的屏幕中心点的3D坐标
    Vec3 Mid3DPos = Vec3(tempPos.x, 0, tempPos.y);
    m_curBuild->theTower->setCameraMask((unsigned short )CameraFlag::USER1);
    m_curBuild->pInfo = CDataManager::getBuildInfo(buildId);
    m_curBuildingId = buildId;
    m_preBuild = m_curBuild;
    
    getValidLocation(midPoint);
    
    m_curBuild->theTower->setPositionX(m_curBuild->theTower->getPositionX());
    m_curBuild->theTower->setPositionZ(m_curBuild->theTower->getPositionZ());
    
    std::string bgImg;
    float scale = 1.0f;
    if(m_curBuild->pInfo->blockPos.size() == 1)
    {
        bgImg = "pvp_one.png";
        scale = 1.1f;
    }
    else if(m_curBuild->pInfo->blockPos.size() == 4)
    {
        bgImg = "pvp_four.png";
        scale = 2.0f;
    }
    else if(m_curBuild->pInfo->blockPos.size() == 9)
    {
        bgImg = "pvp_nine.png";
        scale = 3.2f;
    }
    
    Sprite3D* pShadow = CFightManager::getTheModelWithPath("touying.png");
    layer3D->addChild(pShadow, 100);
    pShadow->getChildByTag(100)->setScale(scale);
    pShadow->getChildByTag(100)->setOpacity(170);
    pShadow->setRotation3D(Vec3(-90,0,0));
    pShadow->setPosition3D(m_curBuild->theTower->getPosition3D());
    pShadow->setCameraMask((unsigned short )CameraFlag::USER1);
    
    Sprite3D* TowerBg = CFightManager::getTheModelWithPath(bgImg.c_str());
    layer3D->addChild(TowerBg);
    TowerBg->setPosition3D(m_curBuild->theTower->getPosition3D());
    TowerBg->setRotation3D(Vec3(270, 0, 0));
    TowerBg->getChildByTag(100)->setScale(0.6f);
    TowerBg->setCameraMask((unsigned short )CameraFlag::USER1);
    m_curBuild->towerBase = TowerBg;
    m_curBuild->shadow = pShadow;
    if(pInfo->buildingType == 0)
    {
        m_pCircle = CFightManager::getTheModelWithPath("fight_CityCircle2.png");
        layer3D->addChild(m_pCircle, 100);
        m_pCircle->getChildByTag(100)->setScale(1.25f);
        m_pCircle->setRotation3D(Vec3(-90, 0, 0));
        m_pCircle->setPosition3D(Vec3(m_curBuild->theTower->getPositionX(), m_curBuild->theTower->getPositionY() + 1,
                                      m_curBuild->theTower->getPositionZ()));
        m_pCircle->setCameraMask((unsigned short )CameraFlag::USER1);
    }
    addBuildObb(m_curBuild);
    for(int i = 0; i < (int)(m_curBuild->pInfo->blockPos.size()); i++)
    {
        float xPos = m_curBuild->theTower->getPositionX() + m_curBuild->pInfo->blockPos[i].x * LENGTH_BLOCK_PER;
        float zPos = m_curBuild->theTower->getPositionZ() - m_curBuild->pInfo->blockPos[i].y * LENGTH_BLOCK_PER;
        int index = (xPos - MAP_START_XPOS)/LENGTH_BLOCK_PER;
        int indey = (zPos - STARTZPOINT)/LENGTH_BLOCK_PER + 9;
        m_allBlock[index][indey].type = 1;
        m_allBlock[index][indey].sprite->setVisible(false);
    }
    for(int i = 0; i < (int)m_selectedBlock.size(); i++)
    {
        m_selectedBlock[i]->removeFromParent();
        m_selectedBlock[i] = NULL;
    }
    sendRequestSaveTeamInfo();
    m_selectedBlock.clear();
    m_curBuild = NULL;//防止首次创建主基地时触发详情和升级按钮
    m_bActive = false;
}

void pvpMainLayer::addTower(int buildingID)
{
    // FighterInfo* theInfo = CPersonalData::getInstance()->getBuildingInfoById(m_buildInfo->monsterId);//获取建筑信息，暂时这么写，后面需要另外添加接口
    
    FighterInfo theInfo;
    pvpBuildInfo* pInfo = CDataManager::getBuildInfo(buildingID);
    CDataManager::CalcFighterInfoWith(&theInfo, pInfo->monsterId, pInfo->level, 1, 0, false, 1, 0, 0, 0);
    
    m_curBuild = new buildInfo();
    m_curBuild->theTower = CBaseTower::create(&theInfo,eFightGroup_Freind);
    m_curBuild->theTower->ChangeModelWithPVPBuildingId(buildingID);
    m_curBuild->theTower->m_bIfActive = true;
    CFightData::getInstance()->addMonster(m_curBuild->theTower,eAttackType_tower);
    layer3D->addChild(m_curBuild->theTower);
    m_curBuild->theTower->setVisible(true);
    
    cocos2d::Point midPoint = Vec2(m_WinSize.width * 0.5f, m_WinSize.height * 0.5f);
    Vec2 tempPos = CommonData::GetSingletonPtr()->getPosOnLayer3D(_camera, midPoint);//获取的屏幕中心点的3D坐标
    Vec3 Mid3DPos = Vec3(tempPos.x, 0, tempPos.y);
    m_curBuild->theTower->setCameraMask((unsigned short )CameraFlag::USER1);
    m_curBuild->pInfo = CDataManager::getBuildInfo(buildingID);
    m_curBuildingId = buildingID;
    m_preBuild = m_curBuild;
    
    if(m_curBuild->pInfo->buildingType != 0)
    {
        m_bFirst = true;
        m_bNewTower = true;
        m_curBuild->theTower->gettheSprite()->setGlobalZOrder(100);
        
        //以当前屏幕中点向两边扩展六个方格宽度的区域内寻找合适的建筑位置，如果没有找到，则直接显示在屏幕中间
        if(!m_bValid)//先找左边
        {
            std::vector<Vec2> tempVec;
            for(int i = 1; i < 7; i++)
            {
                tempVec.clear();
                tempVec.push_back(Vec2(m_WinSize.width * 0.5f, m_WinSize.height * 0.5f + i*81));
                tempVec.push_back(Vec2(m_WinSize.width * 0.5f + i*81, m_WinSize.height * 0.5f + i*81));
                tempVec.push_back(Vec2(m_WinSize.width * 0.5f + i*81, m_WinSize.height * 0.5f));
                tempVec.push_back(Vec2(m_WinSize.width * 0.5f + i*81, m_WinSize.height * 0.5f - i*81));
                
                tempVec.push_back(Vec2(m_WinSize.width * 0.5f, m_WinSize.height * 0.5f - i*81));
                tempVec.push_back(Vec2(m_WinSize.width * 0.5f - i*81, m_WinSize.height * 0.5f - i*81));
                tempVec.push_back(Vec2(m_WinSize.width * 0.5f - i*81, m_WinSize.height * 0.5f));
                tempVec.push_back(Vec2(m_WinSize.width * 0.5f - i*81, m_WinSize.height * 0.5f + i*81));
                int index = 0;
                while (!m_bValid && (index < (int)tempVec.size()))
                {
                    getValidLocation(tempVec[index]);
                    index++;
                }
                if(m_bValid)
                {
                    midPoint = tempVec[index - 1];
                    break;
                }
            }
        }
        tempPos = CommonData::GetSingletonPtr()->getPosOnLayer3D(_camera, midPoint);//获取的屏幕中心点的3D坐标
        Mid3DPos = Vec3(tempPos.x, 0, tempPos.y);
        m_curBuild->theTower->setPosition3D(Mid3DPos);
        getValidLocation(midPoint);
        addArrow();
        addOkAndCancel();
        
        Vec3 p = m_curBuild->theTower->getPosition3D() + m_curBuild->theTower->gettheSprite()->getPosition3D();
        float height = m_curBuild->theTower->gettheSprite()->getBoundingBox().size.height;
        
        p.y = p.y+height*0.5f;
        Vec3 extents = Vec3(height * 0.35f, height * 0.35f, height * 0.35f + 10);
        if((m_curBuild->pInfo->buildingType == 7) || (m_curBuild->pInfo->buildingType == 8) || (m_curBuild->pInfo->buildingType == 12))
        {
            extents = Vec3(height * 0.35f + 15, height * 0.35f + 15, height * 0.35f + 10);
        }
        AABB aabb(-extents, extents);
        auto obb = OBB(aabb);
        
        obb._center = p;
        m_newTowerObbInfo->obb = obb;
        m_newTowerObbInfo->towerInfo = m_curBuild;
        m_newTowerObbInfo->buildingId = buildingID;
        m_newTowerObbInfo->state = 0;
        m_newTowerObbInfo->startUpTime = 0;
        m_newTowerObbInfo->needTime = 0;
        m_newTowerObbInfo->validTime = 0;
    }
    m_bActive = false;
}

void pvpMainLayer::adduserInfo()
{
    float zOrder = 150;
    if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_PVP_RONGLU) && !(CDataManager::getEndedBy(GUIDE_PVP_RONGLU)))
    {
        zOrder = 10;
    }
    
    Sprite* normal = Sprite::createWithSpriteFrameName("pvpBack.png");
    Sprite* select = Sprite::createWithSpriteFrameName("pvpBack.png");
    select->setColor(Color3B::GRAY);
    m_backItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(pvpMainLayer::btnCallBack, this));
    Menu* backMenu = Menu::create(m_backItem, NULL);
    m_backItem->setTag(PVP_BACK_TAG);
    addChild(backMenu, 100);
    backMenu->setPosition(Vec2(m_WinSize.width * 0.04f, m_WinSize.height * 0.94f));
    //体力
    char buf[256];
    m_staminaBg = Sprite::createWithSpriteFrameName("information_bg.png");
    addChild(m_staminaBg, zOrder);
    m_staminaBg->setScaleX(0.9f);
    m_staminaBg->setPosition(Vec2(m_winSize.width * 0.25f, m_winSize.height * 0.95f));
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
    sprintf(buf, "%02d:%02d", m_bufferIime/60, m_bufferIime%60);
    m_pTime = Label::createWithSystemFont(buf, "Arial", 20);
    
    
    m_pTime->setAnchorPoint(Vec2(1, 0.5));
    m_staminaBg->addChild(m_pTime);
    
    UserInfo* pInfo = CPersonalData::getInstance()->getUserInfo();
    int iUpperStamin = CDataManager::getUpgradeInfoByLevel(pInfo->m_iUserLevel)->upperStamina;
    
    if(CPersonalData::getInstance()->GetPhysicalNum()>=iUpperStamin)
    {
        m_pStaminaLabel->setSystemFontSize(26);
        m_pStaminaMaxLabel->setSystemFontSize(26);
        m_pStaminaMaxLabel->setPosition(Vec2(m_staminaBg->getBoundingBox().size.width * 0.98, m_staminaBg->getBoundingBox().size.height * 0.5f));
        m_pTime->setVisible(false);
        m_pStaminaLabel->setPosition(Vec2(m_pStaminaMaxLabel->getPositionX() - m_pStaminaMaxLabel->getContentSize().width, m_staminaBg->getBoundingBox().size.height * 0.5f));
        m_pStaminaLabel->setColor(Color3B(239, 146, 66));
        
    }
    m_pTime->setPosition(Vec2(m_pStaminaMaxLabel->getPositionX(), m_pStaminaLabel->getPositionY() - m_pStaminaLabel->getContentSize().height + 5));
    
    //体力补给
    normal = Sprite::createWithSpriteFrameName("plus.png");
    select = Sprite::createWithSpriteFrameName("plus.png");
    select->setColor(Color3B::GRAY);
    m_pStamainItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(pvpMainLayer::btnCallBack, this));
    m_pStamainItem->setTag(PVP_STAMINA_CHARGE);
    CommonData::enlargeItemSprite(m_pStamainItem, 3);
    Menu* pStaminaMenu = Menu::create(m_pStamainItem, NULL);
    m_staminaBg->addChild(pStaminaMenu);
    m_pStamainItem->setScale(0.7f);
    pStaminaMenu->setPosition(Vec2(m_staminaBg->getContentSize().width * 0.2f, m_staminaBg->getContentSize().height * 0.36f));
    
    //金币
    Sprite* coinBg = Sprite::createWithSpriteFrameName("information_bg.png");
    addChild(coinBg, zOrder);
    coinBg->setScaleX(0.9f);
    coinBg->setPosition(Vec2(m_winSize.width * 0.465f, m_staminaBg->getPositionY()));
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
    m_pCoinItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(pvpMainLayer::btnCallBack, this));
    CommonData::enlargeItemSprite(m_pCoinItem, 3);
    m_pCoinItem->setTag(PVP_COIN_CHARGE);
    m_pCoinItem->setScale(0.7f);
    Menu* pCoinMenu = Menu::create(m_pCoinItem, NULL);
    coinBg->addChild(pCoinMenu);
    pCoinMenu->setPosition(Vec2(coinBg->getBoundingBox().size.width * 0.2f, coinBg->getBoundingBox().size.height * 0.36f));
    
    
    //魔法石
    Sprite* stoneBg = Sprite::createWithSpriteFrameName("information_bg.png");
    addChild(stoneBg, zOrder);
    stoneBg->setScaleX(0.9f);
    stoneBg->setPosition(Vec2(m_winSize.width * 0.68f, m_staminaBg->getPositionY()));
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
    m_pStoneItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(pvpMainLayer::btnCallBack, this));
    m_pStoneItem->setTag(PVP_STONE_CHARGE);
    CommonData::enlargeItemSprite(m_pStoneItem, 3);
    Menu* pStoneMenu = Menu::create(m_pStoneItem, NULL);
    stoneBg->addChild(pStoneMenu);
    m_pStoneItem->setScale(0.7f);
    pStoneMenu->setPosition(Vec2(stoneBg->getBoundingBox().size.width * 0.24f, stoneBg->getBoundingBox().size.height * 0.36f));
    
    //能量框
    Sprite* enemyBg = Sprite::createWithSpriteFrameName("pvpStoneProgressBg.png");
    addChild(enemyBg, zOrder);
    enemyBg->setPosition(Vec2(m_WinSize.width * 0.9f, m_WinSize.height * 0.96));
    Sprite* sp = Sprite::createWithSpriteFrameName("pvpStoneProgress.png");
    m_progress = ProgressTimer::create(sp);
    enemyBg->addChild(m_progress);
    m_progress->setType(ProgressTimer::Type::BAR);
    m_progress->setMidpoint(Point(0,0.5));
    m_progress->setBarChangeRate(Vec2(1, 0));
    m_progress->setPosition(Vec2(enemyBg->getBoundingBox().size.width * 0.5f, enemyBg->getBoundingBox().size.height * 0.5f));

    LabelAtlas *pNumAltas =  LabelAtlas::create("0","fonts/numAtlas.png",24,37,'0');
    
    m_pCurStoneNum = CBeatNumbers::createWithNumberAndSize(0,30,0,pNumAltas);
    m_pCurStoneNum->m_pNumberLabel->setScale(0.6);
    m_pCurStoneNum->m_pNumberLabel->setAnchorPoint(Vec2(1,0));
    m_pCurStoneNum->setPosition(Vec2(enemyBg->getBoundingBox().size.width*0.5,enemyBg->getBoundingBox().size.height*0.105f));
    enemyBg->addChild(m_pCurStoneNum,100);
    
    
    m_pStoneAllNum =  LabelAtlas::create("0","fonts/numAtlas.png",24, 37,'0');
    m_pStoneAllNum->setScale(0.6);
    enemyBg->addChild(m_pStoneAllNum, 100);
    m_pStoneAllNum->setPosition(enemyBg->getBoundingBox().size.width*0.5,enemyBg->getBoundingBox().size.height*0.105f);
    
    m_iAllStoneNum = 100;
    //遍历所有的建筑根据所有类型为魔法石库的容量来计算当前魔法石上限
    for(int i = 0; i < (int)m_allBuildObb.size(); i++)
    {
        //在这里计算魔法石每秒的恢复速度
        pvpBuildInfo* buildInfo = CDataManager::getBuildInfo(m_allBuildObb[i].buildingId);
        if((buildInfo->buildingType == 1) && (m_allBuildObb[i].state == 0))//魔法石库
        {
            m_iAllStoneNum += buildInfo->effectTime;
            
        }
        else  if(buildInfo->buildingType == 10) //矿场
        {
            m_pKuanchanBuildVec.push_back(m_allBuildObb[i]);
            if(m_allBuildObb[i].state == 0)
            {
                m_stoneRate += buildInfo->effectTime;
            }
        }
    }
    
    CPersonalData::getInstance()->setUserMaxKuangStone(m_iAllStoneNum);
    
    
    int iStone =  CPersonalData::getInstance()->getUserKuangStone();
    
    
    
    m_stone = iStone;
    {
        
        CPersonalData::getInstance()->setUserMaxKuangStone(m_iAllStoneNum);
 
        
       
        
  
        sprintf(buf, ":%d",m_iAllStoneNum);
        
        m_pStoneAllNum->setString(buf);
        
  
        
        m_fEndPercent = iStone*1.0/CPersonalData::getInstance()->getUserMaxKuangStone()*100;
        m_fStartPercent = m_progress->getPercentage();
         m_fSecPass = abs(m_fEndPercent-m_fStartPercent)/40;
        m_fAllPerTime = 0;
        
        m_bAddPer = true;
         m_pCurStoneNum->setBeatTime(m_fSecPass);
        
       
    
  
    }
    
    //魔法石产量
    Sprite* enemyProductionBg = Sprite::createWithSpriteFrameName("pvpStoneProgressBg.png");
    addChild(enemyProductionBg, zOrder);
    enemyProductionBg->setScale(0.9f, 0.74f);
    enemyProductionBg->setPosition(Vec2(m_WinSize.width * 0.91f, m_WinSize.height * 0.915));
    
    m_pvPStone= Sprite::createWithSpriteFrameName("pvp_ore.png");
    addChild(m_pvPStone, zOrder + 1);
    m_pvPStone->setPosition(Vec2(m_winSize.width * 0.82f, m_WinSize.height * 0.95));
    
    
    sprintf(buf, "%d/h",m_stoneRate);
    m_addLabel = Label::createWithSystemFont(buf, "Arial", 24);
    enemyProductionBg->addChild(m_addLabel, zOrder);
    m_addLabel->setPosition(Vec2(enemyProductionBg->getBoundingBox().size.width * 0.53f, enemyProductionBg->getBoundingBox().size.height * 0.6f));
    
    addBtn();
}



void pvpMainLayer::addBtn()
{
    Sprite* normal = Sprite::createWithSpriteFrameName("pvpMessage.png");
    Sprite* select = Sprite::createWithSpriteFrameName("pvpMessage.png");
    select->setColor(Color3B::GRAY);
    m_msgItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(pvpMainLayer::btnCallBack, this));
    Menu* backMenu = Menu::create(m_msgItem, NULL);
    m_msgItem->setTag(PVP_MESSAGE_TAG);
    addChild(backMenu, 100);
    backMenu->setPosition(Vec2(m_WinSize.width * 0.05f, m_WinSize.height * 0.78f));
    //建造
    normal = Sprite::createWithSpriteFrameName("pvpBuild.png");
    select = Sprite::createWithSpriteFrameName("pvpBuild.png");
    select->setColor(Color3B::GRAY);
    m_buildItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(pvpMainLayer::btnCallBack, this));
    Menu* buildMenu = Menu::create(m_buildItem, NULL);
    m_buildItem->setTag(PVP_BUILD_TAG);
    addChild(buildMenu, 100);
    buildMenu->setPosition(Vec2(m_WinSize.width * 0.06f, m_WinSize.height * 0.09f));
    
    //战斗
    normal = Sprite::createWithSpriteFrameName("jiandun.png");
    select = Sprite::createWithSpriteFrameName("jiandun.png");
    select->setColor(Color3B::GRAY);
    m_pFightItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(pvpMainLayer::btnCallBack, this));
    Menu* fightMenu = Menu::create(m_pFightItem, NULL);
    m_pFightItem->setTag(PVP_FIGHT_TAG);
    addChild(fightMenu, 100);
    fightMenu->setPosition(Vec2(m_WinSize.width * 0.91f, m_WinSize.height * 0.14f));
    
    //消耗
    Sprite* pEnemyBg = Sprite::createWithSpriteFrameName("pvpStoneBg.png");
    addChild(pEnemyBg, 100);
    pEnemyBg->setScale(1.2f);
    pEnemyBg->setPosition(Vec2(m_winSize.width * 0.91f, m_winSize.height * 0.04));
    
    int iCost = CPersonalData::getInstance()->getUserInfo()->m_iUserLevel * 10;
    m_pFightItem->setUserData((void*)iCost);
    __String* pTemp = __String::createWithFormat("%d", iCost);
    Label*pCostNum = Label::createWithTTF(pTemp->getCString(), "fonts/cuti.ttf", 28);
    addChild(pCostNum, 100);
    pCostNum->setPosition(Vec2(m_winSize.width * 0.878f, m_winSize.height * 0.04));
    
    Sprite* coin = Sprite::createWithSpriteFrameName("coin.png");
    addChild(coin, 100);
    coin->setScale(0.65f);
    coin->setPosition(Vec2(m_winSize.width * 0.942f, m_winSize.height * 0.04));
}

void pvpMainLayer::addGuideInfo(GuideState type,int num)
{
    if(type == GUIDE_PVP_BUILDING)
    {
        if(num == 6)
        {
            LYMenuItemSprite* temp = (LYMenuItemSprite*)m_btnMenu->getChildByTag(PVP_BULIDING_OK);
            if(temp && m_bValid)
            {
                Vec2 pos = temp->convertToWorldSpaceAR(Vec2::ZERO);
                guiderManagerLayer::getInstance()->setStepNum(guiderManagerLayer::getInstance()->getStepNum() + 1);
                guiderManagerLayer::getInstance()->createLayer(this, GUIDE_SHOW_FINGER, pos);
            }
        }
        else if(num == 8)
        {
            guiderManagerLayer::getInstance()->setStepNum(guiderManagerLayer::getInstance()->getStepNum() + 1);
            guiderManagerLayer::getInstance()->createLayer(this);
        }
        else if(num == 10)
        {
            LYMenuItemSprite* temp = (LYMenuItemSprite*)m_btnMenu->getChildByTag(PVP_BULIDING_FINISH);
            if(temp)
            {
                Vec2 pos = temp->convertToWorldSpaceAR(Vec2::ZERO);
                guiderManagerLayer::getInstance()->createLayer(this, GUIDE_SHOW_FINGER, pos);
            }
        }    }
    else  if(type == GUIDE_PVP_RONGLU)
    {
        if(num == 4)
        {
            if(m_curBuild)
            {
                rmvBtnAndArrow();
                rmvSelectedBlock();
                m_curBuild = NULL;
            }
            guiderManagerLayer::getInstance()->createLayer(this, GUIDE_SHOW_FINGER, m_guidePos);
        }
        else if(num == 5)
        {
            LYMenuItemSprite* temp = (LYMenuItemSprite*)m_btnMenu->getChildByTag(PVP_BUILDING_PRODUCT);
            if(temp)
            {
                Vec2 pos = temp->convertToWorldSpaceAR(Vec2::ZERO);
                guiderManagerLayer::getInstance()->createLayer(this, GUIDE_SHOW_FINGER, pos);
            }
        }
    }
    else if(type == GUIDE_PVP_ROB)
    {
        if(num == 3)
        {
            Vec2 pos = m_pFightItem->convertToWorldSpaceAR(Vec2::ZERO);
            guiderManagerLayer::getInstance()->createLayer(this, GUIDE_SHOW_BOTH, pos);
        }
    }
}

void pvpMainLayer::calculateRayByLocationInView(Ray* ray,Camera* theCamera, const Vec2& location)
{
    
    auto size = Director::getInstance()->getWinSize();
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

void pvpMainLayer::adjustTouchBtn(Vec2 touchPos)
{
    Ray ray;
    calculateRayByLocationInView(&ray,_camera, touchPos);
    Sprite3D* tempTarget = NULL;
    for(int i = 0; i < (int)m_btnObbVec.size(); i++)
    {
        if(ray.intersects(m_btnObbVec[i].obb))
        {
            tempTarget = m_btnObbVec[i].sp;
            break;
        }
    }
}

void pvpMainLayer::onTouchesBegan(const std::vector<Touch*>& touches, Event *unused_event)
{
    log("触摸开始");
    
    
//    if(m_bTouchOk)
//    {
//        return;
//    }
    if(touches.size() > 1)
    {
        Vec2 mPoint1 = touches.at(0)->getLocationInView();
        Vec2 mPoint2 = touches.at(1)->getLocationInView();
        //计算两个触摸点之间的距离
        m_distance = sqrt((mPoint2.x - mPoint1.x) * (mPoint2.x - mPoint1.x) + (mPoint2.y - mPoint1.y) * (mPoint2.y - mPoint1.y));
    }
    else
    {
        Touch* pTouch = touches.at(0);
        m_startPos = pTouch->getLocationInView();
        if(m_buildLayer)
        {
            if(m_pLayer && m_buildLayer && m_scrollView && ((m_buildLayer->getBoundingBox().containsPoint(pTouch->getLocation()))
                            || (m_scrollView->getBoundingBox().containsPoint(pTouch->getLocation()))))
            {
                //根据scrollView偏移量重新设置精灵坐标
                for (int i = 0 ; i < (int)m_allBuildType.size(); i++)
                {
                    Rect spSize = m_allBuildType[i]->newTower->getBoundingBox();
                    float yPos = m_scrollView->getPositionY();
                    spSize.origin = Vec2(spSize.origin.x + m_scrollView->getContentOffset().x, spSize.origin.y + yPos);
                    if(spSize.containsPoint(pTouch->getLocation()))
                    {
                        m_selectedItem = m_allBuildType[i];
                        m_selectedItem->newTower->setScale(0.9f);
                        m_touchFlag = true;
                        return;
                    }
                }
            }
        }
        if(m_bNewTower)
        {
            if(m_newTowerObbInfo)
            {
                Ray ray;
                calculateRayByLocationInView(&ray,_camera, m_startPos);
                if(ray.intersects(m_newTowerObbInfo->obb))
                {
                    log("摸到了！！！");
                    m_bActive = true;
                    m_preBuild = m_newTowerObbInfo->towerInfo;
                    m_towerPos = m_preBuild->theTower->getPosition3D();
                    m_curBuildingId = m_newTowerObbInfo->buildingId;
                }
                else
                {
                    m_preBuild = NULL;
                }
            }
        }
        else
        {
            if(m_curBuild)
            {
                m_oldPos = m_curBuild->theTower->getPosition3D();
                m_preBuild = adjustTouchTower();
                if(m_curBuild)//(m_preBuild && (m_preBuild->theTower == m_curBuild->theTower))
                {
                    for(int i = 0; i < (int)(m_curBuild->pInfo->blockPos.size()); i++)
                    {
                        float xPos = m_curBuild->theTower->getPositionX() + m_curBuild->pInfo->blockPos[i].x * LENGTH_BLOCK_PER;
                        float zPos = m_curBuild->theTower->getPositionZ() - m_curBuild->pInfo->blockPos[i].y * LENGTH_BLOCK_PER;
                        int index = (xPos - MAP_START_XPOS)/LENGTH_BLOCK_PER;
                        int indey = (zPos - STARTZPOINT)/LENGTH_BLOCK_PER + 9;
                        m_allBlock[index][indey].type = 0;
                    }
                    log("清空");
                }
            }
        }
    }
    m_touchFlag = true;
}

void pvpMainLayer::showFreeBlock()
{
    for(int i = 0; i < MAP_X_INDEX; i++)
    {
        for(int j = 0 ; j < MAP_Y_INDEX; j++)
        {
            m_allBlock[i][j].sprite->setVisible(true);
        }
    }
    for(int k = 0; k < (int)m_allBuildObb.size(); k++)
    {
        if(m_allBuildObb[k].towerInfo->theTower != m_curBuild->theTower)
        {
            for(int i = 0; i < m_allBuildObb[k].towerInfo->pInfo->blockPos.size(); i++)
            {
                float xPos = m_allBuildObb[k].towerInfo->theTower->getPositionX() +
                m_allBuildObb[k].towerInfo->pInfo->blockPos[i].x * LENGTH_BLOCK_PER;
                float zPos = m_allBuildObb[k].towerInfo->theTower->getPositionZ() -
                m_allBuildObb[k].towerInfo->pInfo->blockPos[i].y * LENGTH_BLOCK_PER;
                int index = (xPos - MAP_START_XPOS)/LENGTH_BLOCK_PER;
                int indey = (zPos - STARTZPOINT)/LENGTH_BLOCK_PER + 9;
                if((index >=0) && (indey >= 0) && (m_allBlock[index][indey].sprite))
                {
                    m_allBlock[index][indey].sprite->setVisible(false);
                    if(m_curBuild->towerBase)
                    {
                        m_curBuild->towerBase->setVisible(false);
                    }
                    if(m_curBuild->shadow)
                    {
                        m_curBuild->shadow->setVisible(false);
                    }
                }
            }
        }
    }
}

void pvpMainLayer::onTouchesMoved(const std::vector<Touch*>& touches, Event *unused_event)
{
    if(((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_PVP_RONGLU) && (guiderManagerLayer::getInstance()->getStepNum() >= 4)) || ((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_PVP_BUILDING) && (guiderManagerLayer::getInstance()->getStepNum() >= 8)))
    {
        return;
    }
    if((touches.size() > 1) && (!m_pLayer))
    {
        Vec2 mPoint1 = touches.at(0)->getPreviousLocationInView();
        Vec2 mPoint2 = touches.at(1)->getPreviousLocationInView();
        m_distance = sqrt((mPoint2.x - mPoint1.x) * (mPoint2.x - mPoint1.x) + (mPoint2.y - mPoint1.y) * (mPoint2.y - mPoint1.y));
        mPoint1 = touches.at(0)->getLocationInView();
        mPoint2 = touches.at(1)->getLocationInView();
        //计算两个触摸点之间的距离
        float distance = sqrt((mPoint2.x - mPoint1.x) * (mPoint2.x - mPoint1.x) + (mPoint2.y - mPoint1.y) * (mPoint2.y - mPoint1.y));
        float scale = (distance - m_distance)/m_distance;
        float nowY = _camera->getPositionY() - 88 * scale;//缩放这里以85作为每次缩放基值
        float nowZ = _camera->getPositionZ() - 88 * scale;
        log("当前触摸点距离 %f, 比例为%f", distance, scale);
        if((nowY >= 162 ) && (nowY <= 514) && (nowZ >= -238) && (nowZ <= 114))
        {
            _camera->setPosition3D(Vec3(_camera->getPositionX(), nowY, nowZ));
            //log("当前相机坐标y=%f,z=%f", nowY, nowZ);
        }
    }
    else
    {
        m_bMove = true;
        if(m_touchFlag)
        {
            Touch* pTouch = touches.at(0);
            if(m_pLayer)
            {
                if(m_selectedItem && (pTouch->getLocationInView().distance(m_startPos) > 20))
                {
                    m_selectedItem->newTower->setScale(1.0f);
                    m_selectedItem = NULL;
                }
                return;
            }
            auto preLocation = pTouch->getPreviousLocation();
            auto location = pTouch->getLocationInView();
            float xPos = (preLocation.x - location.x);
            if(!m_bValidMove && (pTouch->getLocationInView().distance(m_startPos) > 30))
            {
                m_bValidMove = true;
            }
            if(m_bActive && m_curBuild && m_bValidMove)// && m_preBuild && (m_curBuild->theTower == m_preBuild->theTower)))
            {
                if(m_pCircle && (m_curBuild->pInfo->buildingType == 0))
                {
                    m_pCircle->setVisible(false);
                }
                showFreeBlock();
                //循环遍历符合条件的方块
                getValidLocation(location);
            }
            else
            {
                m_bMoveMap = true;
                float CameraPosX = _camera->getPositionX() + xPos;
                //float CameraPosY = _camera->getPositionZ() + yPos;
                bool moveValid = true;
                if(((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_PVP_BUILDING) && !(CDataManager::getEndedBy(GUIDE_PVP_BUILDING)))
                   || ((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_PVP_RONGLU) && !(CDataManager::getEndedBy(GUIDE_PVP_RONGLU))))
                {
                    moveValid = false;
                }
                if(moveValid)
                {
                    _camera->setPosition3D(Vec3(CameraPosX , _camera->getPositionY(),_camera->getPositionZ()));
                    
                    if(_camera->getPosition3D().x<Start3DPOINT)
                    {
                        _camera->setPosition3D(Vec3(Start3DPOINT,_camera->getPositionY(),_camera->getPositionZ()));
                    }
                    
                    else if(_camera->getPosition3D().x>END3DPOINT)
                    {
                        _camera->setPosition3D(Vec3(END3DPOINT,_camera->getPositionY(),_camera->getPositionZ()));
                    }
                    else{
                        for (int i = 0; i < (int)m_btnVec.size(); i++)
                        {
                            Vec3 pos1 = _camera->getPosition3D();
                            Vec2 targetPos1 = CommonData::GetSingletonPtr()->getPosOnScreen(_camera, pos1);
                            Vec3 pos2 = Vec3(CameraPosX, _camera->getPositionY(), _camera->getPositionZ());
                            Vec2 targetPos2 = CommonData::GetSingletonPtr()->getPosOnScreen(_camera, pos2);
                            float pos = m_btnVec[i]->getPositionX() - xPos;
                            m_btnVec[i]->setPositionX(pos);
                        }
                    }
                }
            }
        }
    }
    
    for (int i = 0; i < (int)m_btnVec.size(); i++)
    {
        m_btnVec[i]->setVisible(false);
    }
    for(int j = 0; j < (int)m_stoneVec.size(); j++)
    {
        if(m_stoneVec[j])
        {
            m_stoneVec[j]->setVisible(false);
        }
    }
    if(m_towerName)
    {
        m_towerName->setVisible(false);
    }
    if(m_towerLevel)
    {
        m_towerLevel->setVisible(false);
    }
}



void  pvpMainLayer::BuildingFininsh(buildInfo* p )
{
    log("建筑升级");
    Sprite3D *pUp = Sprite3D::create();
    
    pUp->setRotation3D(Vec3(-90,0,0));
    
    
    Sprite* soul = Sprite::createWithSpriteFrameName("buildfinish1.png");
    
    soul->setScale(4);
    
    pUp->addChild(soul);
    
    
    pUp->setPosition3D(Vec3(0,4, -50));
    
    if(p)
    {
        p->theTower->addChild(pUp,9999,100);
    }
    
    
    
    pUp->setCameraMask((unsigned short )CameraFlag::USER1);
    

    
    Animate* pAction = CFightManager::getAnimateWithPath("buildfinish",1,14);
    soul->runAction(Sequence::create(pAction,RemoveSelf::create(),NULL));
    SimpleAudioEngine::getInstance()->playEffect("music/task.mp3");
}


void pvpMainLayer::onTouchesEnded(const std::vector<Touch*>& touches, Event *unused_event)
{
    //log("触摸结束");
    if(CPersonalData::getInstance()->getTouchRmvFlag())
    {
        CPersonalData::getInstance()->setTouchRmvFlag(false);
        return;
    }
    if(((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_PVP_RONGLU) && (guiderManagerLayer::getInstance()->getStepNum() >= 5)) || ((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_PVP_BUILDING) && (guiderManagerLayer::getInstance()->getStepNum() >= 10)))
    {
        return;
    }
    if((touches.size() <= 1))
    {
        Touch* pTouch = touches.at(0);
        if(m_pLayer)
        {
            if(m_buildLayer)
            {
                if(!(m_buildLayer->getBoundingBox().containsPoint(pTouch->getLocation()))
                   && !(m_scrollView->getBoundingBox().containsPoint(pTouch->getLocation())))
                {//如果是pvp创建建筑引导则强制让玩家点击
                    if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_PVP_BUILDING) && (guiderManagerLayer::getInstance()->getStepNum() == 4))
                    {
                        return;
                    }
                    setBtnEnable(true);
                    m_buildLayer->removeFromParent();
                    m_buildLayer = NULL;
                    m_scrollView->removeFromParent();
                    m_scrollView = NULL;
                    m_pLayer->removeFromParent();
                    m_pLayer = NULL;
                    m_allBuildType.clear();
                }
                else
                {
                    if(m_selectedItem)
                    {
                        if(pTouch->getLocationInView().distance(m_startPos) < 20)
                        {
                            m_bValid = false;
                            long type = (long)m_selectedItem->newTower->getUserData();
                            addNewTower(int(type));
                        }
                    }
                }
            }
            m_bMove = false;
            return;
        }
        if(!m_touchFlag)//触摸bug
        {
            return;
        }
        
        for(int i = 0; i < MAP_X_INDEX; i++)
        {
            for(int j = 0 ; j < MAP_Y_INDEX; j++)
            {
                if(m_allBlock[i][j].sprite)
                {
                    m_allBlock[i][j].sprite->setVisible(false);
                }
            }
        }
        cocos2d::Point touchPoint = pTouch->getLocationInView() ;
        if((pTouch->getLocationInView().distance(m_startPos) < 30) && !m_bNewTower)
        {
            if(!m_bMove && m_curBuild)// && m_preBuild)
            {
                for(int i = 0; i < (int)(m_curBuild->pInfo->blockPos.size()); i++)//不管两次点击的是不是同一建筑物，都将所第一次所选中的建筑所占方格状态设置回来
                {
                    float xPos = m_curBuild->theTower->getPositionX() + m_curBuild->pInfo->blockPos[i].x * LENGTH_BLOCK_PER;
                    float zPos = m_curBuild->theTower->getPositionZ() - m_curBuild->pInfo->blockPos[i].y * LENGTH_BLOCK_PER;
                    int index = (xPos - MAP_START_XPOS)/LENGTH_BLOCK_PER;
                    int indey = (zPos - STARTZPOINT)/LENGTH_BLOCK_PER + 9;
                    m_allBlock[index][indey].type = 1;
                }
                if(m_preBuild &&(m_curBuild->theTower == m_preBuild->theTower))//如果当前塔已被选中再次点击选中不创建
                {
                    return;
                }
            }
            rmvBtnAndArrow();
            if(!m_bMove)
            {
                //如果首次选中的建筑和第二次选中的建筑不一致，或者第二次未选中任何建筑，则将之前选中的建筑的阴影和底座显示出来
                if(m_curBuild && ((!m_preBuild) || (m_curBuild->theTower != m_preBuild->theTower)))
                {
                    m_curBuild->theTower->gettheSprite()->stopAllActions();
                    m_curBuild->theTower->gettheSprite()->setColor(Color3B(255, 255, 255));
                    if(m_curBuild->towerBase)
                    {
                        m_curBuild->towerBase->setVisible(true);
                    }
                    if(m_curBuild->shadow)
                    {
                        m_curBuild->shadow->setVisible(true);
                    }
                }
                m_curBuild = adjustTouchTower();
            }
            //只要没移动状态不变
            if(m_curBuild)
            {
                if(((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_PVP_RONGLU) && (guiderManagerLayer::getInstance()->getStepNum() == 4))
                   ||((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_PVP_BUILDING) && (guiderManagerLayer::getInstance()->getStepNum() == 8)))//限制只能点击熔炉，弥补遮罩限制区域的不足
                {
                    if(m_curBuild->pInfo->buildingType != 2)
                    {
                        return;
                    }
                }
                if((m_curBuild->pInfo->buildingType== 10) && !m_bMove)
                {
                    LYMenuItemSprite *p = (LYMenuItemSprite*)m_btnMenu->getChildByTag(PVP_BUILDING_KUANSHI);
                    
                    if(p)
                    {
                        int userBuildId;
                        for(int i = 0; i < (int)m_pKuanchanBuildVec.size(); i++)
                        {
                            if(m_pKuanchanBuildVec[i].towerInfo->theTower == m_curBuild->theTower)
                            {
                                m_pKuanchanBuildVec[i].validTime = 0;
                                userBuildId = m_pKuanchanBuildVec[i].userBuildingId;
                            }
                        }
                        __String* tpf = __String::createWithFormat("%d", userBuildId);
                        CNetControlModule::sharedNetControlModule()->collectStone(this, menu_selector(pvpMainLayer::collectStoneBack), tpf->getCString());
                        m_curBuild = NULL;
                        return;
                    }
                }
                
                m_bActive = true;
                addArrow();
                addOkAndCancel();
                showSelectedBlock();
                m_curBuild->theTower->gettheSprite()->setOpacity(255);
                m_curBuild->theTower->gettheSprite()->runAction(RepeatForever::create(Sequence::create(TintTo::create(1.1, 150, 150, 150), TintTo::create(1.1, 255, 255, 255), NULL)));
                m_curBuild->theTower->runAction(Sequence::create(MoveBy::create(0.09f, Vec2(0, 8)), MoveBy::create(0.09f, Vec2(0, -8)), NULL));
                addUpgradeAndDetail();
                if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_PVP_RONGLU) && (guiderManagerLayer::getInstance()->getStepNum() == 4))
                {
                    guiderManagerLayer::getInstance()->setStepNum(guiderManagerLayer::getInstance()->getStepNum() + 1);
                    addGuideInfo(GUIDE_PVP_RONGLU, 5);
                }
                if(m_bMove && m_curBuild)//m_preBuild && ((m_curBuild->theTower == m_preBuild->theTower)))//如果建筑被移动的距离小于20像素并且更新了位置，则作如下下判断
                {
                    int type = adjustIsTake(m_curBuild->theTower->getPositionX(), m_curBuild->theTower->getPositionZ());
                    log("移动设置1");
                    adjustTouchEnded(type);
                }
            }
            
            if(!m_curBuild)
            {
                if(m_selectedBlock.size() > 0 && !m_bFirst)
                {
                    for(int i = 0; i < (int)m_selectedBlock.size(); i++)
                    {
                        m_selectedBlock[i]->removeFromParent();
                        m_selectedBlock[i] = NULL;
                    }
                    m_selectedBlock.clear();
                }
                m_bActive = false;
            }
        }
        else
        {
            //if(m_curBuild && m_preBuild && ((m_curBuild->theTower == m_preBuild->theTower) || (m_bNewTower)) && m_bActive)
            if((m_curBuild || m_bNewTower) && m_bActive && m_bMove)
            {
                int type = adjustIsTake(m_curBuild->theTower->getPositionX(), m_curBuild->theTower->getPositionZ());
                if((type == 0) && (m_curBuild->pInfo->buildingType == 11) && m_patrolman)
                {
                    pvpCommonData::getInstance()->setTowerInfo(m_curBuild);
                }
                if(!m_bNewTower)
                {
                    adjustTouchEnded(type);
                }
                else
                {
                    addOkAndCancel();
                    if((type == 1) && (!m_bNewTower))
                    {
                        m_curBuild->theTower->gettheSprite()->setOpacity(100);
                    }
                    else{
                        updateAllBulidObb();
                        m_curBuild->theTower->gettheSprite()->setOpacity(255);
                    }
                    //添加引导
                    if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_PVP_BUILDING) && (guiderManagerLayer::getInstance()->getStepNum() == 6) && (type == 0))
                    {
                        addGuideInfo(GUIDE_PVP_BUILDING, 6);
                    }
                }
            }
            else
            {
                if(m_curBuild && !m_bNewTower)
                {
                    addUpgradeAndDetail();
                }
                //                else if(m_bNewTower && !m_curBuild)
                //                {
                //                    m_curBuild = m_preBuild;
                //                    addOkAndCancel();
                //                }
            }
            if(m_bMoveMap)
            {
                m_bMoveMap = false;
                updateAllBulidObb();
                if(!m_curBuild)
                {
                    for (int i = 0; i < (int)m_btnVec.size(); i++)
                    {
                        m_btnVec[i]->removeFromParent();
                        m_btnVec[i]=NULL;
                    }
                    m_btnVec.clear();
                }
                for (int j = 0; j < (int)m_stoneVec.size(); j++)
                {
                    if(m_stoneVec[j])
                    {
                        m_stoneVec[j]->removeFromParent();
                        m_stoneVec[j]=NULL;
                    }
                }
                m_stoneVec.clear();
            }
        }
    }
    if(m_bNewTower && m_curBuild)
    {
        addOkAndCancel();
    }
    
    m_bMove = false;
    m_preBuild = NULL;
    // m_bBaoshi  = true;]
    m_touchFlag = false;
    m_bValidMove = false;
}

void pvpMainLayer::adjustTouchEnded(int type)
{
    if(type == 0)
    {
        updateAllBulidObb();
        updateTowerInfo();
        if(m_curBuild)
        {
            if(m_curBuild->towerBase)
            {
                m_curBuild->towerBase->removeFromParent();
                m_curBuild->towerBase = NULL;
            }
            if(m_curBuild->shadow)
            {
                m_curBuild->shadow->removeFromParent();
                m_curBuild->shadow = NULL;
            }
            std::string bgImg;
            float scale = 1.0f;
            if(m_curBuild->pInfo->blockPos.size() == 1)
            {
                bgImg = "pvp_one.png";
                scale = 1.1f;
            }
            else if(m_curBuild->pInfo->blockPos.size() == 4)
            {
                bgImg = "pvp_four.png";
                scale = 2.0f;
            }
            else if(m_curBuild->pInfo->blockPos.size() == 9)
            {
                bgImg = "pvp_nine.png";
                scale = 3.2f;
            }
            
            Sprite3D* pShadow = CFightManager::getTheModelWithPath("touying.png");
            layer3D->addChild(pShadow, 100);
            pShadow->getChildByTag(100)->setScale(scale);
            pShadow->getChildByTag(100)->setOpacity(170);
            pShadow->setRotation3D(Vec3(-90,0,0));
            pShadow->setPosition3D(m_curBuild->theTower->getPosition3D());
            pShadow->setCameraMask((unsigned short )CameraFlag::USER1);
            m_curBuild->shadow = pShadow;
            
            Sprite3D* TowerBg = CFightManager::getTheModelWithPath(bgImg.c_str());
            layer3D->addChild(TowerBg);
            TowerBg->setPosition3D(m_curBuild->theTower->getPosition3D());
            TowerBg->setRotation3D(Vec3(270, 0, 0));
            TowerBg->setScale(0.6f);
            TowerBg->setCameraMask((unsigned short )CameraFlag::USER1);
            m_curBuild->towerBase = TowerBg;
            
            showSelectedBlock();
        }
    }
    else if(type == 1)
    {
        m_curBuild->theTower->setPosition3D(m_oldPos);
        if(m_curBuild->towerBase)
        {
            m_curBuild->towerBase->setVisible(true);
        }
        if(m_curBuild->shadow)
        {
            m_curBuild->shadow->setVisible(true);
        }
        addArrow();
        showSelectedBlock();
    }
    if(m_pCircle && (m_curBuild->pInfo->buildingType == 0))
    {
        m_pCircle->setPosition3D(Vec3(m_curBuild->theTower->getPositionX(), m_curBuild->theTower->getPositionY() + 1,
                                      m_curBuild->theTower->getPositionZ()));
        m_pCircle->setVisible(true);
    }
    resetBlockState();
    addUpgradeAndDetail();
    m_curBuild->theTower->gettheSprite()->setOpacity(255);
    for (int j = 0; j < (int)m_stoneVec.size(); j++)
    {
        if(m_stoneVec[j])
        {
            m_stoneVec[j]->removeFromParent();
            m_stoneVec[j]= NULL;
        }
    }
    m_stoneVec.clear();
    //log("当前位置 x= %f, z=  %f",m_curBuild->theTower->getPositionX(), m_curBuild->theTower->getPositionZ());
}



void pvpMainLayer::showSelectedBlock()
{
//    if(!m_bMove)
//    {
//        rmvSelectedBlock();
//    }
    rmvSelectedBlock();
    for(int i = 0; i < (int)(m_curBuild->pInfo->blockPos.size()); i++)
    {
        if(m_curBuild->towerBase)
        {
            m_curBuild->towerBase->setVisible(false);
        }
        if(m_curBuild->shadow)
        {
            m_curBuild->shadow->setVisible(false);
        }
        float xPos = m_curBuild->theTower->getPositionX() + m_curBuild->pInfo->blockPos[i].x * LENGTH_BLOCK_PER;
        float zPos = m_curBuild->theTower->getPositionZ() + m_curBuild->pInfo->blockPos[i].y * LENGTH_BLOCK_PER;
        Sprite3D* sp = CFightManager::getTheModelWithPath("pvpGreenBlock.png");
        sp->setPosition3D(Vec3(xPos, 1, zPos));
        sp->setScale(0.6f);
        sp->setRotation3D(Vec3(90, 0, 0));
        layer3D->addChild(sp);
        m_selectedBlock.push_back(sp);
        if(m_curBuild->pInfo->attack > 0)
        {
            sp->setVisible(false);
            sp->runAction(Sequence::create(DelayTime::create(0.16f), Show::create(), NULL));
        }
        sp->setCameraMask((unsigned short )CameraFlag::USER1);
    }
}

void pvpMainLayer::addNewTower(int buildType)
{
    char buf[256];
    log("选中了");
    m_selectedItem->newTower->setScale(1.0f);
    int buildId = CDataManager::getBuildIdByType(buildType);
    if(m_selectedItem->valid)
    {
        
        //判断金币是否足够
        pvpBuildInfo* pBuildInfo = CDataManager::getBuildInfo(buildId);
        m_cost = pBuildInfo->cost;
        if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_PVP_BUILDING) && (guiderManagerLayer::getInstance()->getStepNum() == 4))
        {
            guiderManagerLayer::getInstance()->setStepNum(guiderManagerLayer::getInstance()->getStepNum() + 1);
            guiderManagerLayer::getInstance()->createLayer(this);
        }
        else if(CPersonalData::getInstance()->getUserCoin() < pBuildInfo->cost)
        {
            Sprite * thetarget = CFightManager::getTheFlyWord("金币不足","fonts/cuti.ttf");
            thetarget->setPosition(m_winSize.width*0.5,m_winSize.height*0.5);
            addChild(thetarget,100);
            return;
        }
        if(m_pLayer)
        {
            m_pLayer->removeFromParent();
            m_pLayer = NULL;
            m_scrollView= NULL;
            m_buildLayer = NULL;
            m_allBuildType.clear();
        }
        addTower(buildId);
        if(!m_upgradeTower)//如果创建之前没有正在创建或者升级的建筑则直接赋值
        {
            m_upgradeTower = m_curBuild;
            m_upgradeBuildId = buildId;
        }
        setBtnEnable(true);
        m_bActive = true;
    }
    else
    {
        std::string temp = getTowerLimitInfo(buildId);
        Sprite * thetarget = CFightManager::getTheFlyWord(temp.c_str(), "fonts/cuti.ttf");
        thetarget->setPosition(m_winSize.width*0.5,m_winSize.height*0.5);
        addChild(thetarget,999);
    }
    m_selectedItem = NULL;
}

bool pvpMainLayer::adjustIfUpgrade(int buildId)
{
    bool ret = false;
    pvpBuildInfo* pBuildInfo = CDataManager::getBuildInfo(buildId);
    if(pBuildInfo->buildingType == 0)//如果是主基地只可能与玩家经验相关 具体限制待定
    {
        return true;
    }
    //查找当前等级下的主基地中建筑id为buildId的建筑的最高等级
    for(int i = 0 ; i < (int)m_allBuildObb.size(); i++)
    {
        pvpBuildInfo* pTowerInfo = CDataManager::getBuildInfo(m_allBuildObb[i].buildingId);
        if(pTowerInfo && (pTowerInfo->buildingType == 0))
        {
            for(int j = 0; j < (int)pTowerInfo->allBuildLimitVec.size(); j++)
            {
                if(pTowerInfo->allBuildLimitVec[j].buitldType == pBuildInfo->buildingType)
                {
                    //主基地等级满足条件并且主基地不能处于升级状态
                    if((pTowerInfo->allBuildLimitVec[j].level > pBuildInfo->level) && (m_allBuildObb[i].state == 0))
                    {
                        ret = true;
                    }
                    break;
                }
            }
            break;
        }
    }
    return ret;
}


int pvpMainLayer::getUpgradeLimit(int buildId)
{
    pvpBuildInfo* pBuildInfo = CDataManager::getBuildInfo(buildId);
    int mainBaseBuildId = 1;
    int mainBaseLevel = 1;
    bool bUpgradeIng = false;//标识是否正在升级
    for(int i = 0 ; i < (int)m_allBuildObb.size(); i++)
    {
        pvpBuildInfo* pMainBaseInfo = CDataManager::getBuildInfo(m_allBuildObb[i].buildingId);
        if(pMainBaseInfo && (pMainBaseInfo->buildingType == 0))
        {
            mainBaseBuildId = pMainBaseInfo->buildingID;
            if(m_allBuildObb[i].state != 0)
            {
                bUpgradeIng = true;
                mainBaseBuildId -= 1;
            }
            else if(pMainBaseInfo->level == 7)
            {
                mainBaseLevel = 7;
                return mainBaseLevel;
            }
        }
    }
    //查找可以升级当前建筑的下一主基地等级
    bool bReachLimit = true;//判断当前建筑是否已经达到数量上限
    for(int i = mainBaseBuildId + 1; i < MAIN_BASE_MAX_LEVEL; i++)
    {
        pvpBuildInfo* pMainBaseInfo = CDataManager::getBuildInfo(i);
        for(int j = 0; j < (int)pMainBaseInfo->allBuildLimitVec.size(); j++)
        {
            if((pBuildInfo->buildingType == pMainBaseInfo->allBuildLimitVec[j].buitldType)
               && (pBuildInfo->level < pMainBaseInfo->allBuildLimitVec[j].level))
            {
                mainBaseLevel = pMainBaseInfo->level;
                bReachLimit = false;
                break;
            }
        }
        if(!bReachLimit)
        {
            break;
        }
    }
    return mainBaseLevel;
}

std::string pvpMainLayer::getTowerLimitInfo(int buildId)
{
    char buf[256];
    pvpBuildInfo* pBuildInfo = CDataManager::getBuildInfo(buildId);
    int mainBaseBuildId = 1;
    int mainBaseLevel = 1;
    int iHaveNum = 0;
    bool bUpgradeIng = false;//标识是否正在升级
    for(int i = 0 ; i < (int)m_allBuildObb.size(); i++)
    {
        pvpBuildInfo* pMainBaseInfo = CDataManager::getBuildInfo(m_allBuildObb[i].buildingId);
        if(pMainBaseInfo->buildingType == pBuildInfo->buildingType)
        {
            iHaveNum++;
        }
        if(pMainBaseInfo && (pMainBaseInfo->buildingType == 0))
        {
            mainBaseBuildId = pMainBaseInfo->buildingID;
            if(m_allBuildObb[i].state != 0)
            {
                bUpgradeIng = true;
                mainBaseBuildId -= 1;
            }
        }
    }
    //查找可以添加当前建筑的下一主基地等级
    bool bReachLimit = true;//判断当前建筑是否已经达到数量上限
    for(int i = mainBaseBuildId + 1; i < MAIN_BASE_MAX_LEVEL; i++)
    {
        pvpBuildInfo* pMainBaseInfo = CDataManager::getBuildInfo(i);
        for(int j = 0; j < (int)pMainBaseInfo->allBuildLimitVec.size(); j++)
        {
            if((pBuildInfo->buildingType == pMainBaseInfo->allBuildLimitVec[j].buitldType)
               && (iHaveNum < pMainBaseInfo->allBuildLimitVec[j].num))
            {
                mainBaseLevel = pMainBaseInfo->level;
                bReachLimit = false;
                break;
            }
        }
        if(!bReachLimit)
        {
            break;
        }
    }
    if(bReachLimit)
    {
        sprintf(buf, "该建筑的数量已达到上限！");
    }
    else{
        sprintf(buf, "将圣像的等级升级到%d级以建造更多！", mainBaseLevel);
    }
    return buf;
}

void pvpMainLayer::updateUserData(Ref *pSender)
{
    CPersonalData::getInstance()->setUpdateUserInfo(true);
}

bool pvpMainLayer::judgeBuildNumLimit(int buildType)
{
    int num = 0;//类型为buildType的塔的当前数量
    int sum = 0;//类型为buildType的塔的限制数量
    for(int i = 0 ; i < (int)m_allBuildObb.size(); i++)
    {
        pvpBuildInfo* pBuildInfo = CDataManager::getBuildInfo(m_allBuildObb[i].buildingId);
        if(pBuildInfo && (pBuildInfo->buildingType == buildType))
        {
            num++;
        }
    }
    
    for(int i = 0 ; i < (int)m_allBuildObb.size(); i++)
    {
        pvpBuildInfo* pBuildInfo = CDataManager::getBuildInfo(m_allBuildObb[i].buildingId);
        if(pBuildInfo && (pBuildInfo->buildingType == 0))
        {
            if(m_allBuildObb[i].state != 0)//如果主基地正在升级则以升级之前的等级来判断
            {
                pBuildInfo = CDataManager::getBuildInfo(m_allBuildObb[i].buildingId - 1);
            }
            for(int k = 0; k < (int)pBuildInfo->allBuildLimitVec.size(); k++)
            {
                if(pBuildInfo->allBuildLimitVec[k].buitldType == buildType)
                {
                    sum = pBuildInfo->allBuildLimitVec[k].num;
                    break;
                }
            }
            break;
        }
    }
    if(num < sum)
    {
        return true;
    }
    else
        return false;
}

int pvpMainLayer::getNumCreate(BUILDUSETYPE buildUseType)
{
    pvpBuildInfo* pMainBuildInfo= NULL;
    int TipNum = 0;
    for(int i = 0 ; i < (int)m_allBuildObb.size(); i++)
    {
        pvpBuildInfo* pBuildInfo = CDataManager::getBuildInfo(m_allBuildObb[i].buildingId);
        if(pBuildInfo && (pBuildInfo->buildingType == 0))
        {
            pMainBuildInfo = pBuildInfo;
            break;
        }
    }
    //先找出当前类型下的所有建筑
    std::vector<towerOBBInfo> tempVec;
    for(int i = 0; i < (int)m_allBuildObb.size(); i++)
    {
        pvpBuildInfo* pBuildInfo = CDataManager::getBuildInfo(m_allBuildObb[i].buildingId);
        if(pBuildInfo->useType == buildUseType)
        {
            tempVec.push_back(m_allBuildObb[i]);
        }
    }
    
    //在主基地当前等级下判断每一种类型的塔的数量是否以达到上限
    for (int k = 0; k < (int)pMainBuildInfo->allBuildLimitVec.size(); k++)
    {
        int buildType = pMainBuildInfo->allBuildLimitVec[k].buitldType;
        int num = 0;//类型为buildType的塔的当前数量
        int buildId = CDataManager::getBuildIdByType(buildType);
        pvpBuildInfo* pInfo = CDataManager::getBuildInfo(buildId);
        if(pInfo && (pInfo->useType == buildUseType))
        {
            for(int n = 0; n < (int)tempVec.size(); n++)
            {
                pvpBuildInfo* pBuildInfo = CDataManager::getBuildInfo(tempVec[n].buildingId);
                if (pBuildInfo && (buildType == pBuildInfo->buildingType))
                {
                    num++;
                }
            }
            //判断是否超过上限
            if(num < pMainBuildInfo->allBuildLimitVec[k].num)
            {
                TipNum++;
            }
        }
    }
    return TipNum;
}

void pvpMainLayer::detectIfCanCreate()
{
    char buf[256];
    int TipNum = getNumCreate(PVP_DEFENSE) + getNumCreate(PVP_INCOMME);
    Sprite* tip = (Sprite*)m_buildItem->getChildByTag(100);
    if(tip)
    {
        tip->removeFromParent();
    }
    if (TipNum > 0) {
        Sprite* tip = Sprite::createWithSpriteFrameName("pvpTip.png");
        m_buildItem->addChild(tip, 1, 100);
        tip->setPosition(Vec2(m_buildItem->getNormalImage()->getContentSize().width * 0.2f,
                              m_buildItem->getNormalImage()->getContentSize().height * 0.75f));
        
        sprintf(buf, "%d", TipNum);
        Label* pLabelNum = Label::createWithTTF(buf, "fonts/cuti.ttf", 20);
        tip->addChild(pLabelNum);
        pLabelNum->enableOutline(Color4B(0, 0, 0, 255), 1);
        pLabelNum->setPosition(Vec2(tip->getContentSize().width * 0.5f, tip->getContentSize().height * 0.5f));
    }
}

void pvpMainLayer::resetBlockState()
{
    
    for(int i = 0; i < (int)(m_curBuild->pInfo->blockPos.size()); i++)
    {
        float xPos = m_curBuild->theTower->getPositionX() + m_curBuild->pInfo->blockPos[i].x * LENGTH_BLOCK_PER;
        float zPos = m_curBuild->theTower->getPositionZ() - m_curBuild->pInfo->blockPos[i].y * LENGTH_BLOCK_PER;
        int index = (xPos - MAP_START_XPOS)/LENGTH_BLOCK_PER;
        int indey = (zPos - STARTZPOINT)/LENGTH_BLOCK_PER + 9;
        m_allBlock[index][indey].type = 1;
        //m_allBlock[index][indey].sprite->setColor(Color3B::RED);
    }
}

void pvpMainLayer::updateAllBulidObb()
{
    if(m_curBuild)
    {
        Vec3 p = m_curBuild->theTower->getPosition3D() + m_curBuild->theTower->gettheSprite()->getPosition3D();
        float height = m_curBuild->theTower->gettheSprite()->getBoundingBox().size.height;
        
        p.y = p.y+height*0.5f;
        Vec3 extents = Vec3(height * 0.35f, height * 0.35f, height * 0.35f + 10);
        if((m_curBuild->pInfo->buildingType == 7) || (m_curBuild->pInfo->buildingType == 8) || (m_curBuild->pInfo->buildingType == 12))
        {
            extents = Vec3(height * 0.35f + 15, height * 0.35f + 15, height * 0.35f + 10);
        }
        AABB aabb(-extents, extents);
        auto obb = OBB(aabb);
        obb._center = p;
        m_newTowerObbInfo->obb = obb;
        m_newTowerObbInfo->towerInfo = m_curBuild;
    }
    
    //m_allBuildObb.clear();
    for(int i = 0; i < (int)m_allBuildObb.size(); i++)
    {
        CBaseBuilding* tower = m_allBuildObb[i].towerInfo->theTower;
        Vec3 p = tower->getPosition3D() + tower->gettheSprite()->getPosition3D();
        float height = tower->gettheSprite()->getBoundingBox().size.height;
        
        p.y = p.y+height*0.5f;
        
        Vec3 extents = Vec3(height * 0.35f, height * 0.35f, height * 0.35f + 10);
        if((m_allBuildObb[i].towerInfo->pInfo->buildingType == 7) || (m_allBuildObb[i].towerInfo->pInfo->buildingType == 8)
           || (m_allBuildObb[i].towerInfo->pInfo->buildingType == 12))
        {
            extents = Vec3(height * 0.35f + 15, height * 0.35f + 15, height * 0.35f + 10);
        }
        AABB aabb(-extents, extents);
        auto obb = OBB(aabb);
        obb._center = p;
        
        m_allBuildObb[i].obb = obb;
        m_allBuildObb[i].xPos = tower->getPositionX();
        m_allBuildObb[i].zPos = tower->getPositionZ();
    }
    pvpCommonData::getInstance()->setAllBack(m_allBlock);
}

void pvpMainLayer::rmvSelectedBlock()
{
    if(m_selectedBlock.size() > 0)
    {
        for(int i = 0; i < (int)m_selectedBlock.size(); i++)
        {
            m_selectedBlock[i]->removeFromParent();
            m_selectedBlock[i] = NULL;
        }
        m_selectedBlock.clear();
    }
}

buildInfo* pvpMainLayer::adjustTouchTower()
{
    //m_curBuildingId = 0;
    Ray ray;
    calculateRayByLocationInView(&ray,_camera, m_startPos);
    CBaseBuilding* tempTarget = NULL;
    
    bool bGuide = false;
    std::vector<towerOBBInfo> tepClickVec;//保存已经点击到的建筑
    if(((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_PVP_RONGLU) && (guiderManagerLayer::getInstance()->getStepNum() == 4))
       ||((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_PVP_BUILDING) && (guiderManagerLayer::getInstance()->getStepNum() == 8)))//限制只能点击熔炉，弥补遮罩限制区域的不足
    {
        bGuide = true;
    }
    
    int index = 0;
    for(int i = 0; i < (int)m_allBuildObb.size(); i++)
    {
        //避免重复选择z轴值最大的一个
        if(bGuide){
            tepClickVec.push_back(m_allBuildObb[i]);
        }
        else{
            if(ray.intersects(m_allBuildObb[i].obb))
            {
                if(!tempTarget)
                {
                    tempTarget = m_allBuildObb[i].towerInfo->theTower;
                    m_curBuildingId = m_allBuildObb[i].buildingId;
                    index = i;
                }
                else
                {
                    if(m_allBuildObb[i].towerInfo->theTower->getPositionZ() > tempTarget->getPositionZ())
                    {
                        tempTarget = m_allBuildObb[i].towerInfo->theTower;
                        m_curBuildingId = m_allBuildObb[i].buildingId;
                        index = i;
                    }
                }
                //m_curBuild = tempTarget;
                m_towerPos = tempTarget->getPosition3D();
            }

        }
    }
    for(int k = 0; k < tepClickVec.size(); k++)
    {
        //如果是pvp创建建筑引导和熔炉引导，只要点击到了熔炉直接响应他，不管是不是在最前面
        if(tepClickVec[k].towerInfo->pInfo->buildingType == 2)
        {
            tempTarget = tepClickVec[k].towerInfo->theTower;
            m_curBuildingId = tepClickVec[k].buildingId;
            index = k;
            break;
        }
    }
    if(tempTarget)
    {
        buildInfo *pInfo = new buildInfo();
        pInfo->theTower = tempTarget;
        if(bGuide)
        {
            pInfo->towerBase = tepClickVec[index].towerInfo->towerBase;
            pInfo->shadow = tepClickVec[index].towerInfo->shadow;
            pInfo->pInfo = CDataManager::getBuildInfo(m_curBuildingId);
            return tepClickVec[index].towerInfo;
        }
        pInfo->towerBase = m_allBuildObb[index].towerInfo->towerBase;
        pInfo->shadow = m_allBuildObb[index].towerInfo->shadow;
        pInfo->pInfo = CDataManager::getBuildInfo(m_curBuildingId);
        return m_allBuildObb[index].towerInfo;
        
    }
    else
        return NULL;
}

void pvpMainLayer::updateTowerInfo()
{
    bool m_bSameLast = false;//判断是否和上一次是在同一位置
    if((m_curBuild->theTower->getPositionX() == m_towerPos.x) && (m_curBuild->theTower->getPositionY() == m_towerPos.y)
       && ((m_curBuild->theTower->getPositionZ() == m_towerPos.z)))
    {
        m_bSameLast = true;
    }
    //保存玩家阵营信息
    if(!m_bSameLast)
    {
        sendRequestSaveTeamInfo();
    }
}

void pvpMainLayer::sendRequestSaveTeamInfo(bool bIfUpgrade)
{
    std::string content;
    char buf[256];
    for(int i = 0; i < (int)m_allBuildObb.size(); i++)
    {
        pvpBuildInfo* pInfo = CDataManager::getBuildInfo(m_allBuildObb[i].buildingId);
        if(pInfo->buildingType == 0)
        {
            m_friendTowerLevel = pInfo->level;
        }
        sprintf(buf, "%d", m_allBuildObb[i].userBuildingId);
        content += buf;
        sprintf(buf, ",%d", m_allBuildObb[i].buildingId);
        content += buf;
        sprintf(buf, ",%d", m_allBuildObb[i].xPos);
        content += buf;
        sprintf(buf, ",%d", m_allBuildObb[i].zPos);
        content += buf;
        if(bIfUpgrade && m_upgradeTower && (m_upgradeTower->theTower == m_allBuildObb[i].towerInfo->theTower))//升级的时候传递1
        {
            content += ",1";
        }
        else
        {
            sprintf(buf, ",%d", m_allBuildObb[i].state);
            content += buf;
        }
        
        sprintf(buf, ",%d", m_allBuildObb[i].startUpTime);
        content += buf;
        sprintf(buf, ",%d", m_allBuildObb[i].needTime);
        content += buf;
        if(i < m_allBuildObb.size() - 1)
        {
            content += ";";
        }
    }
    CPersonalData::getInstance()->getUserInfo()->m_mainBaseLevel = m_friendTowerLevel;
    
    CNetControlModule::sharedNetControlModule()->setUserTeamInfo(this, menu_selector(pvpMainLayer::saveUserTeamInfo),
                                                                 content.c_str(), m_friendTowerLevel);
}

void pvpMainLayer::saveUserTeamInfo(Ref* pSender)
{
    _netRecvEventSetUserTeamInfo* pInfo = (_netRecvEventSetUserTeamInfo*)pSender;
    int state = 1;
    for(std::vector<std::string>::iterator it = pInfo->m_paramsStr.begin(); it!= pInfo->m_paramsStr.end(); it++)
    {
        string str = *it;
        string name = CDataManager::getStrFromBackStr(str);
        if(name == "state")
        {
            state = atoi(CDataManager::getDigitFromBackStr(*it).c_str());
            if(state != 0)
            {
                log("保存玩家阵营信息失败");
                return;
            }
        }
    }
    log("保存玩家阵营信息成功");
    if(m_upgradeEnd)
    {
        pvpBuildInfo* preLevelInfo = CDataManager::getBuildInfo(m_upgradeBuildId);
        monsterInfo* pInfo = CDataManager::getMonsterInfoByMonsterId(preLevelInfo->monsterId);
        __String* tempStr = __String::createWithFormat("%s已升级到%d级", pInfo->monsterName.c_str(), preLevelInfo->level);
        //        if(preLevelInfo->level == 1)
        //        {
        //            tempStr = __String::createWithFormat("%s创建成功", pInfo->monsterName.c_str());
        //        }
        Sprite * thetarget = CFightManager::getTheFlyWord(tempStr->getCString(),"fonts/cuti.ttf");
        thetarget->setPosition(m_winSize.width*0.5,m_winSize.height*0.5);
        addChild(thetarget,100);
        //刷新该建筑模型
        m_upgradeEnd = false;
        m_upgradeBuildId = 0;//先取消掉
        
        if(m_curBuild && (m_upgradeTower->theTower == m_curBuild->theTower))
        {
            rmvBtnAndArrow();
            rmvSelectedBlock();
            if(m_curBuild->towerBase)
            {
                m_curBuild->towerBase->setVisible(true);
            }
            if(m_curBuild->shadow)
            {
                m_curBuild->shadow->setVisible(true);
            }
            m_curBuild = NULL;
        }
        m_upgradeTower = NULL;
        //m_curBuild = NULL;
        //如果
        if(m_layer)
        {
            m_layer->removeFromParent();
            m_layer = NULL;
            if(m_bNewTower && m_curBuild)
            {
                rmvNewTower();
            }
        }
    }
}

void pvpMainLayer::rmvBtnAndArrow()
{
    if(m_bFirst)
    {
        return;
    }
    for(int i = 0; i < (int)m_arrowVec.size(); i++)
    {
        m_arrowVec[i]->removeFromParent();
    }
    m_arrowVec.clear();
    for(int i = 0; i < (int)m_btnVec.size(); i++)
    {
        m_btnVec[i]->removeFromParent();
        m_btnVec[i] = NULL;
    }
    if(m_towerName)
    {
        m_towerName->removeFromParent();
        m_towerName = NULL;
    }
    if(m_towerLevel)
    {
        m_towerLevel->removeFromParent();
        m_towerLevel = NULL;
    }
    if(m_curBuild)
    {
        m_curBuild->theTower->m_pAtkRangeCircle->setVisible(false);
    }
    m_btnVec.clear();
    m_btnObbVec.clear();
}

void pvpMainLayer::addOkAndCancel()
{
    for (int i = 0; i < (int)m_btnVec.size(); i++)
    {
        m_btnVec[i]->removeFromParent();
        m_btnVec[i]=NULL;
    }
    
    m_btnVec.clear();
    Vec3 pos = m_curBuild->theTower->getPosition3D();
    if(m_bFirst)
    {
        Sprite* normal = Sprite::createWithSpriteFrameName("pvp_create_cancel.png");
        Sprite* select = Sprite::createWithSpriteFrameName("pvp_create_cancel.png");
        select->setColor(Color3B::GRAY);
        LYMenuItemSprite* cancelItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(pvpMainLayer::btnCallBack, this));
        m_btnMenu->addChild(cancelItem, 1, PVP_BULIDING_CANCEL);
        
        float height = m_curBuild->theTower->gettheSprite()->getBoundingBox().size.height * 1.5f;;
        Vec2 targetPos = CommonData::GetSingletonPtr()->getPosOnScreen(_camera, pos);
        
        
        cancelItem->setPosition(Vec2(targetPos.x - 50, targetPos.y + height));
        m_btnVec.push_back(cancelItem);
        
        normal = Sprite::createWithSpriteFrameName("pvp_create_ok.png");
        select = Sprite::createWithSpriteFrameName("pvp_create_ok.png");
        select->setColor(Color3B::GRAY);
        LYMenuItemSprite* sureItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(pvpMainLayer::upgradeBtnCallBack, this));
        m_btnMenu->addChild(sureItem, 1, PVP_BULIDING_OK);
        sureItem->setPosition(Vec2(targetPos.x + 50, targetPos.y + height));
        m_btnVec.push_back(sureItem);
        
    }
}

void pvpMainLayer::addArrow()
{
    
    for(int i = 0; i < (int)m_arrowVec.size(); i++)
    {
        m_arrowVec[i]->removeFromParent();
    }
    m_arrowVec.clear();
    if(!m_bFirst && !m_bActive)
    {
        return;
    }
    Vec3 pos = m_curBuild->theTower->getPosition3D();
    Size bgSize = m_curBuild->theTower->gettheSprite()->getBoundingBox().size;
    float addHeight = 0;
    float addwidth = 0;
    Sprite* sp = Sprite::createWithSpriteFrameName("pvpArrow.png");
    Vec3 arrowRotation[4] = {Vec3(90, 0, 0),Vec3(90, 0, 180),Vec3(90, 90, 0),Vec3(90, -90, 0)};
    if((m_curBuild->pInfo->blockPos.size() == 1) || (m_curBuild->pInfo->blockPos.size() == 9))
    {
        if(m_curBuild->pInfo->blockPos.size() == 1)
        {
            addwidth = LENGTH_BLOCK_PER * 0.5f + sp->getBoundingBox().size.width * 0.55f;
            addHeight = LENGTH_BLOCK_PER * 0.5f + sp->getBoundingBox().size.height * 0.57f;
        }
        if(m_curBuild->pInfo->blockPos.size() == 9)//如果是九个格子
        {
            addwidth = LENGTH_BLOCK_PER * 1.5f + sp->getBoundingBox().size.width * 0.55f;
            addHeight = LENGTH_BLOCK_PER * 1.5f + sp->getBoundingBox().size.height * 0.57f;
        }
    }
    else if(m_curBuild->pInfo->blockPos.size() == 2)
    {
        addwidth = LENGTH_BLOCK_PER * 0.5f + sp->getBoundingBox().size.width * 0.55f;
        addHeight = LENGTH_BLOCK_PER * 1.0f + sp->getBoundingBox().size.height * 0.57f;
    }
    else if(m_curBuild->pInfo->blockPos.size() == 6)
    {
        addwidth = LENGTH_BLOCK_PER + sp->getBoundingBox().size.width * 0.55f;
        addHeight = LENGTH_BLOCK_PER * 1.5f + sp->getBoundingBox().size.height * 0.57f;
    }
    else if(m_curBuild->pInfo->blockPos.size() == 4)
    {
        addwidth = LENGTH_BLOCK_PER + sp->getBoundingBox().size.width * 0.55f;
        addHeight = LENGTH_BLOCK_PER + sp->getBoundingBox().size.height * 0.57f;
    }
    Vec3 arrowPos[4] = {Vec3(pos.x + addwidth, 0, pos.z),Vec3(pos.x - addwidth, 0, pos.z), Vec3(pos.x, 0, pos.z - addHeight),
        Vec3(pos.x, 0, pos.z + addHeight)};
    
    
    
    for(int i = 0; i < 4; i++)
    {
        Sprite3D* sprite = CFightManager::getTheModelWithPath("pvpArrow.png");
        sprite->setPosition3D(arrowPos[i]);
        sprite->setVisible(false);
        sprite->setRotation3D(arrowRotation[i]);
        layer3D->addChild(sprite);
        sprite->setCameraMask((unsigned short )CameraFlag::USER1);
        if(!m_bMove && (m_curBuild->pInfo->attack > 0))
        {
            sprite->setVisible(false);
            sprite->runAction(Sequence::create(DelayTime::create(0.25f), Show::create(), NULL));
        }
        else
        {
            sprite->setVisible(true);
        }
        
        m_arrowVec.push_back(sprite);
    }
}


void pvpMainLayer::addUpgradeAndDetail()
{
    int clickBtnState = 0;//默认为0 1:熔炉, 2:祭坛进化, 3:定时炸弹
    
    Vec3 pos = m_curBuild->theTower->getPosition3D();
    float xAddPos = 65;
    //判断当前塔是否正在升级
    int type = 0;//0表示普通状态，2表示升级
    for(int i = 0; i < (int)m_allBuildObb.size(); i++)
    {
        if(m_allBuildObb[i].towerInfo->theTower == m_curBuild->theTower)
        {
            type = m_allBuildObb[i].state;
            pvpBuildInfo* pInfo = CDataManager::getBuildInfo(m_allBuildObb[i].buildingId);
            if((pInfo->buildingType == 2) && (type != 2))//熔炉
            {
                clickBtnState = 1;
                xAddPos = 120;
            }
            
            else if((pInfo->buildingType == 12) && (type != 2))
            {
                clickBtnState = 3;
                xAddPos = 120;
            }
            else if((pInfo->buildingType == 11) && (type != 2) && (pvpCommonData::getInstance()->getEvolution() == 0))
            {
                clickBtnState = 2;
                xAddPos = 120;
            }
            break;
        }
    }
    if((m_curBuild->theTower->m_pAtkRangeCircle->getScale() > 0) && (!m_curBuild->theTower->m_pAtkRangeCircle->isVisible()))//避免移动结束后二次创建
    {
        m_curBuild->theTower->m_pAtkRangeCircle->setOpacity(60);
        m_curBuild->theTower->m_pAtkRangeCircle->setVisible(true);
        float scale = m_curBuild->theTower->m_fScale;
        
        m_curBuild->theTower->m_pAtkRangeCircle->setScale(0);
        m_curBuild->theTower->m_pAtkRangeCircle->runAction(ScaleTo::create(0.16f, scale));
        log("缩放大小为%f", scale);
        
    }
    for (int i = 0; i < (int)m_btnVec.size(); i++)
    {
        m_btnVec[i]->removeFromParent();
        m_btnVec[i] = NULL;
        if(m_towerName)
        {
            m_towerName->removeFromParent();
            m_towerName = NULL;
        }
        if(m_towerLevel)
        {
            m_towerLevel->removeFromParent();
            m_towerLevel = NULL;
        }
    }
    m_btnVec.clear();
    
    if(type != 2)
    {
        Sprite* normal = Sprite::createWithSpriteFrameName("pvp_details.png");
        Sprite* select = Sprite::createWithSpriteFrameName("pvp_details.png");
        select->setColor(Color3B::GRAY);
        LYMenuItemSprite* detailItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(pvpMainLayer::btnCallBack, this));
        m_btnMenu->addChild(detailItem, 1, PVP_BULIDING_INFO);
        
        Vec2 targetPos = CommonData::GetSingletonPtr()->getPosOnScreen(_camera, pos);
        detailItem->setPosition(Vec2(targetPos.x - xAddPos, targetPos.y - BTN_DISTANCE_DOWN));
        m_btnVec.push_back(detailItem);
        
        normal = Sprite::createWithSpriteFrameName("pvp_upgrade.png");
        select = Sprite::createWithSpriteFrameName("pvp_upgrade.png");
        select->setColor(Color3B::GRAY);
        LYMenuItemSprite* upgradeItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(pvpMainLayer::upgradeBtnCallBack, this));
        m_btnMenu->addChild(upgradeItem, 1, PVP_BULIDING_UPGRADE);
        upgradeItem->setPosition(Vec2(targetPos.x + xAddPos, targetPos.y - BTN_DISTANCE_DOWN));
        m_btnVec.push_back(upgradeItem);
        if(m_curBuild->pInfo->nextBuildingID == 0)
        {
            upgradeItem->setVisible(false);
        }
        else if(m_curBuild->pInfo->attack > 0)
        {
            m_btnMenu->setVisible(false);
            m_btnMenu->runAction(Sequence::create(DelayTime::create(0.16f), Show::create(), NULL));
        }
        
        if(clickBtnState == 1)
        {
            upgradeItem->setPositionX(upgradeItem->getPositionX() - xAddPos);
            Sprite* normal = Sprite::createWithSpriteFrameName("pvp_product.png");
            Sprite* select = Sprite::createWithSpriteFrameName("pvp_product.png");
            select->setColor(Color3B::GRAY);
            LYMenuItemSprite* detailItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(pvpMainLayer::upgradeBtnCallBack, this));
            m_btnMenu->addChild(detailItem, 1, PVP_BUILDING_PRODUCT);
            Vec2 targetPos = CommonData::GetSingletonPtr()->getPosOnScreen(_camera, pos);
            detailItem->setPosition(Vec2(targetPos.x + xAddPos, targetPos.y - BTN_DISTANCE_DOWN));
            m_btnVec.push_back(detailItem);
        }
        else if(clickBtnState == 2)
        {
            upgradeItem->setPositionX(upgradeItem->getPositionX() - xAddPos);
            Sprite* normal = Sprite::createWithSpriteFrameName("pvp_evolution.png");
            Sprite* select = Sprite::createWithSpriteFrameName("pvp_evolution.png");
            select->setColor(Color3B::GRAY);
            LYMenuItemSprite* evolutionItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(pvpMainLayer::upgradeBtnCallBack, this));
            m_btnMenu->addChild(evolutionItem, 1, PVP_BUILDING_EVOLUTION);
            Vec2 targetPos = CommonData::GetSingletonPtr()->getPosOnScreen(_camera, pos);
            evolutionItem->setPosition(Vec2(targetPos.x + xAddPos, targetPos.y - BTN_DISTANCE_DOWN));
            m_btnVec.push_back(evolutionItem);
        }
        else if(clickBtnState == 3)
        {
            upgradeItem->setPositionX(upgradeItem->getPositionX() - xAddPos);
            Sprite* normal = Sprite::createWithSpriteFrameName("settime.png");
            Sprite* select = Sprite::createWithSpriteFrameName("settime.png");
            select->setColor(Color3B::GRAY);
            LYMenuItemSprite* detailItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(pvpMainLayer::upgradeBtnCallBack, this));
            m_btnMenu->addChild(detailItem, 1, PVP_BUILDING_DINGSHI);
            Vec2 targetPos = CommonData::GetSingletonPtr()->getPosOnScreen(_camera, pos);
            detailItem->setPosition(Vec2(targetPos.x + xAddPos, targetPos.y - BTN_DISTANCE_DOWN));
            m_btnVec.push_back(detailItem);
        }
    }
    else{
        CNetControlModule::sharedNetControlModule()->getServerTime(this, menu_selector(pvpMainLayer::getServerTime));
    }
    
    Size bgSize = m_curBuild->theTower->gettheSprite()->getBoundingBox().size;
    monsterInfo* pInfo = CDataManager::getMonsterInfoByMonsterId(m_curBuild->pInfo->monsterId);
    m_towerName = Label::createWithTTF(pInfo->monsterName.c_str(),"fonts/cuti.ttf",20);   // Label::createWithSystemFont(pInfo-
    m_curBuild->theTower->addChild(m_towerName, 101);
    m_towerName->enableOutline(Color4B(0, 0, 0, 255), 2);
    m_towerName->setPosition3D(Vec3(0, bgSize.height + 35, 0));
    m_towerName->setCameraMask((unsigned short )CameraFlag::USER1);
    
    int level = m_curBuild->pInfo->level;
    for(int i = 0; i < (int)m_allBuildObb.size(); i++)
    {
        if(m_curBuild->theTower == m_allBuildObb[i].towerInfo->theTower)
        {
            if(m_allBuildObb[i].state == 2 && m_curBuild->pInfo->level > 1)
            {
                level--;
            }
            break;
        }
    }
    __String* temp = __String::createWithFormat("%d级", level);
    m_towerLevel = Label::createWithTTF(temp->getCString(), "fonts/cuti.ttf", 18);
    m_curBuild->theTower->addChild(m_towerLevel, 102);
    m_towerLevel->enableOutline(Color4B(0, 0, 0, 255), 2);
    m_towerLevel->setPosition3D(Vec3(0, bgSize.height + 10, 0));
    m_towerLevel->setCameraMask((unsigned short )CameraFlag::USER1);
}

void pvpMainLayer::adjustUpgradeEnd(Ref* pSender)
{
    if(CPersonalData::getInstance()->getState() == 1)
    {
        return;
    }
    long time = CPersonalData::getInstance()->getUserInfo()->serverLogintime;
    //获取升级所需时间
    if(m_upgradeTower)
    {
        BuildingFininsh(m_upgradeTower);
        //升级结束保存玩家建筑信息
        int index = 0;
        int startTime = 0;
        int needtime = 0;
        for(int i = 0; i < (int)m_allBuildObb.size(); i++)
        {
            if(m_upgradeTower && (m_allBuildObb[i].towerInfo->theTower == m_upgradeTower->theTower))
            {
                index = i;
                needtime = m_allBuildObb[i].needTime;
                startTime = m_allBuildObb[i].startUpTime;
            }
        }
        
        if(time - startTime >= needtime)
        {
            m_upgradeTower->theTower->gettheSprite()->stopAllActions();
            m_upgradeTower->theTower->gettheSprite()->setColor(Color3B(255, 255, 255));
            m_allBuildObb[index].state = 0;
            pvpBuildInfo* pInfo = CDataManager::getBuildInfo(m_upgradeBuildId);
            if((pInfo->buildingType == 11) && (pInfo->level == 1))//如果是祭坛则添加怪物
            {
                addPattolInfo(m_upgradeTower);
            }
            if(pInfo->buildingType == 10)
            {
                updateKuangchangState(m_upgradeTower->theTower);
                updateStoneRate();
            }
            if(pInfo->buildingType == 1)//如果升级的是矿石库泽发送消息更新矿石上限
            {
                updateKuangshiLimit();
            }
            sendRequestSaveTeamInfo();
            rmvUpgradeInfo();
            if(pInfo->buildingType == 0)
            {
                detectIfCanCreate();
                CDataManager::saveTaskInfoToDB(20, pInfo->level, true);
            }
            if(m_upgradeTower)
            {
                m_upgradeTower->theTower->ChangeModelWithPVPBuildingId(m_upgradeBuildId);
            }
        }
        else
        {
            m_remainTime = int(time - startTime);
            m_upgradeEnd = false;
        }
    }
}

void pvpMainLayer::getServerTime(Ref* pSender)
{
    if(CPersonalData::getInstance()->getState() == 1)
    {
        return;
    }
    long time = CPersonalData::getInstance()->getUserInfo()->serverLogintime;
    long startUpgradetime = 0;
    Vec3 pos = m_curBuild->theTower->getPosition3D();
    float xAddPos = 65;
    for(int i = 0; i < (int)m_allBuildObb.size(); i++)
    {
        if(m_allBuildObb[i].towerInfo->theTower == m_curBuild->theTower)
        {
            startUpgradetime = m_allBuildObb[i].startUpTime;
            int type = m_allBuildObb[i].state;
            if((m_curBuild->pInfo->buildingType == 2) && (type == 0))//熔炉
            {
                xAddPos = 120;
            }
            break;
        }
    }
    Sprite* normal = Sprite::createWithSpriteFrameName("pvp_cancel.png");
    Sprite* select = Sprite::createWithSpriteFrameName("pvp_cancel.png");
    select->setColor(Color3B::GRAY);
    LYMenuItemSprite* delItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(pvpMainLayer::upgradeBtnCallBack, this));
    m_btnMenu->addChild(delItem, 1, PVP_BULIDING_DEL);
    
    Vec2 targetPos = CommonData::GetSingletonPtr()->getPosOnScreen(_camera, pos);
    delItem->setPosition(Vec2(targetPos.x - xAddPos, targetPos.y - BTN_DISTANCE_DOWN));
    m_btnVec.push_back(delItem);
    
    normal = Sprite::createWithSpriteFrameName("pvp_finish.png");
    select = Sprite::createWithSpriteFrameName("pvp_finish.png");
    select->setColor(Color3B::GRAY);
    LYMenuItemSprite* finishItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(pvpMainLayer::upgradeBtnCallBack, this));
    m_btnMenu->addChild(finishItem, 1, PVP_BULIDING_FINISH);
    finishItem->setPosition(Vec2(targetPos.x + xAddPos, targetPos.y - BTN_DISTANCE_DOWN));
    m_btnVec.push_back(finishItem);
    
    
    Sprite* stone = Sprite::createWithSpriteFrameName("stone.png");
    finishItem->addChild(stone);
    stone->setScale(0.6f);
    stone->setAnchorPoint(Vec2(1, 0.5f));
    stone->setPosition(Vec2(normal->getBoundingBox().size.width - 5, normal->getBoundingBox().size.height * 0.24f));
    
    
    //计算需要的魔法石数量(应该用剩余的时间来计算)
    char buf[256];
    int stoneNum = m_remainTime/60;
    if(stoneNum == 0)
    {
        stoneNum = 1;
    }
    sprintf(buf, "%d", stoneNum);
    Label* pLabelStone = Label::createWithTTF(buf, "fonts/cuti.ttf", 28);
    finishItem->addChild(pLabelStone);
    pLabelStone->enableOutline(Color4B(0, 0, 0, 255), 3);
    pLabelStone->setAnchorPoint(Vec2(1, 0.5f));
    pLabelStone->setPosition(Vec2(normal->getBoundingBox().size.width * 0.5f, normal->getBoundingBox().size.height * 0.26f));
    if(m_curBuild->pInfo->attack > 0)
    {
        m_btnMenu->setVisible(false);
        m_btnMenu->runAction(Sequence::create(DelayTime::create(0.16f), Show::create(), NULL));
    }
    if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_PVP_BUILDING) && (guiderManagerLayer::getInstance()->getStepNum() == 8))
    {
        addGuideInfo(GUIDE_PVP_BUILDING, 8);
    }
}

void pvpMainLayer::addBtnObbInfo()
{
    //    for(int i = 0 ; i < m_btnVec.size(); i++)
    //    {
    //        Sprite3D* sp = m_btnVec[i];
    //        Vec3 p = sp->getPosition3D();
    //        float height = sp->getChildByTag(100)->getBoundingBox().size.height;
    //
    //        p.y = p.y+height*0.5f;
    //
    //        Vec3 extents = Vec3(height*0.35f, height*0.35f, height*0.35f);
    //        AABB aabb(-extents, extents);
    //        auto obb = OBB(aabb);
    //        obb._center = p;
    //        btnObbInfo pInfo;
    //        pInfo.obb = obb;
    //        pInfo.sp = sp;
    //        m_btnObbVec.push_back(pInfo);
    //    }
}

void pvpMainLayer::addBuildObb(buildInfo* pBuild)
{
    Vec3 p = pBuild->theTower->getPosition3D() + pBuild->theTower->gettheSprite()->getPosition3D();
    float height = pBuild->theTower->gettheSprite()->getBoundingBox().size.height;
    
    p.y = p.y+height*0.5f;
    
    Vec3 extents = Vec3(height * 0.35f, height * 0.35f, height * 0.35f + 10);
    if((pBuild->pInfo->buildingType == 7) || (pBuild->pInfo->buildingType == 8) || (pBuild->pInfo->buildingType == 12))
    {
        extents = Vec3(height * 0.35f + 15, height * 0.35f + 15, height * 0.35f + 10);
    }

    AABB aabb(-extents, extents);
    auto obb = OBB(aabb);
    obb._center = p;
    
    towerOBBInfo pInfo;
    pInfo.obb = obb;
    pInfo.towerInfo = pBuild;
    pInfo.buildingId = m_curBuild->pInfo->buildingID;
    pInfo.xPos = pBuild->theTower->getPositionX();
    pInfo.zPos = pBuild->theTower->getPositionZ();
    pInfo.state = 0;//需要建造时间
    pInfo.startUpTime = 0;
    pInfo.needTime = m_curBuild->pInfo->costTime;
    pInfo.validTime = 0;
    if(m_allBuildObb.size() == 0)
    {
        pInfo.userBuildingId = 1;
    }
    else{
        pInfo.userBuildingId = int(m_allBuildObb.size() + 1);
    }
    
    m_allBuildObb.push_back(pInfo);
    //    sendRequestSaveTeamInfo();
}

void pvpMainLayer::rmvTowerInfoLayer()
{
    if(m_curBuild)
    {
        if(m_curBuild->shadow)
        {
            m_curBuild->shadow->setVisible(true);
        }
        if(m_curBuild->towerBase)
        {
            m_curBuild->towerBase->setVisible(true);
        }
        m_curBuild->theTower->gettheSprite()->stopAllActions();
        m_curBuild->theTower->gettheSprite()->setColor(Color3B(255, 255, 255));
        m_curBuild = NULL;
    }
    if(m_layer)
    {
        m_layer->removeFromParent();
        m_layer = NULL;
    }
    if(m_pLayer)
    {
        setBtnEnable(true);
        m_pLayer->removeFromParent();
        m_pLayer = NULL;
        m_scrollView= NULL;
        m_buildLayer = NULL;

        m_allBuildType.clear();
    }
    detectPvpGuideInfo();
}

void pvpMainLayer::rmvUpgradeInfo()
{
    if(m_timeProgressBg)
    {
        m_timeProgressBg->removeFromParent();
        m_timeProgressBg = NULL;
    }
    if(m_hammer)
    {
        m_hammer->stopAllActions();
        m_hammer->removeFromParent();
        m_hammer = NULL;
    }
}

void pvpMainLayer::cancelUpgrade(Ref* pSender)
{
    //取消升级
    m_remainTime = 0;
    m_bFirst = false;
    m_cost = 0;
    rmvUpgradeInfo();
    
    if(m_layer)
    {
        m_layer->removeFromParent();
        m_layer = NULL;
    }
    //保存玩家阵营信息
    int iCoin = 0;
    for(std::vector<towerOBBInfo>::iterator it = m_allBuildObb.begin(); it != m_allBuildObb.end(); it++)
    {
        pvpBuildInfo* pBuildInfo = CDataManager::getBuildInfo(it->buildingId);
        if(m_upgradeTower && (it->towerInfo->theTower == m_upgradeTower->theTower))
        {
            m_upgradeTower->towerBase->setVisible(true);
            m_upgradeTower->shadow->setVisible(true);
            if((pBuildInfo->level == 1) && (pBuildInfo->buildingType != 0))//如果是还未创建成功的建筑取消升级则直接删掉，否则只更新状态并且削减建筑id即可
            {
                for(int i = 0; i < (int)(m_upgradeTower->pInfo->blockPos.size()); i++)
                {
                    float xPos = m_upgradeTower->theTower->getPositionX() + m_upgradeTower->pInfo->blockPos[i].x * LENGTH_BLOCK_PER;
                    float zPos = m_upgradeTower->theTower->getPositionZ() - m_upgradeTower->pInfo->blockPos[i].y * LENGTH_BLOCK_PER;
                    int index = (xPos - MAP_START_XPOS)/LENGTH_BLOCK_PER;
                    int indey = (zPos - STARTZPOINT)/LENGTH_BLOCK_PER + 9;
                    m_allBlock[index][indey].type = 0;
                }
                it->towerInfo->theTower->removeFromParent();
                it->towerInfo->towerBase->removeFromParent();
                it->towerInfo->shadow->removeFromParent();
                m_allBuildObb.erase(it);
                detectIfCanCreate();
            }
            else
            {
                m_upgradeTower->theTower->gettheSprite()->stopAllActions();
                m_upgradeTower->theTower->gettheSprite()->setColor(Color3B(255, 255, 255));
                it->towerInfo->pInfo = CDataManager::getBuildInfo(it->buildingId - 1);
                it->buildingId--;
                it->state = 0;
            }
            break;
        }
    }
    pvpBuildInfo* pInfo = CDataManager::getBuildInfo(m_upgradeBuildId);
    iCoin = pInfo->cost * 0.5;
    
    sendRequestSaveTeamInfo();
    m_upgradeTower = NULL;
    m_curBuild = NULL;
    m_curBuildingId = 0;
    m_upgradeBuildId = 0;
    CNetWorkDataSingle::GetSingletonPtr()->SaveUserMCF(this, menu_selector(pvpMainLayer::updateUserData), iCoin,0,0);
    log("取消创建");
    for(int i = 0; i < (int)m_selectedBlock.size(); i++)
    {
        m_selectedBlock[i]->removeFromParent();
        m_selectedBlock[i] = NULL;
    }
    m_selectedBlock.clear();
    m_bNewTower = false;
}


void pvpMainLayer::UpdateKuanshi(Ref *pSender)
{
    
    
    int iStone = CFightData::getInstance()->getTheKuanshiNum();
    
    
    char buf[50] = {0};
    sprintf(buf,":%d",m_iAllStoneNum);
    
    m_pStoneAllNum->setString(buf);
    
    
    
}



void pvpMainLayer::ZhaiBaoshi()
{
    
    LYMenuItemSprite *p = (LYMenuItemSprite*)m_btnMenu->getChildByTag(PVP_BUILDING_KUANSHI);
    Vec2 t = p->getPosition();
    for(int i = 0; i < (int)m_stoneVec.size(); i++)
    {
        if(m_stoneVec[i] == p)
        {
            m_stoneVec[i]->removeFromParent();
            m_stoneVec[i] = NULL;
        }
    }
    
    
    if(p)
    {
        Sprite *pNomal = Sprite::createWithSpriteFrameName("pvp_ore.png");
        
        pNomal->setPosition(t);
        addChild(pNomal,999);
        
        pNomal->runAction(Sequence::create(MoveTo::create(0.5,Vec2(t.x,t.y+100)),EaseSineOut::create(MoveTo::create(1,Vec2(m_WinSize.width*0.8,m_WinSize.height*0.95))),ToggleVisibility::create(),RemoveSelf::create(),NULL));
        
        
        m_pvPStone->runAction(Sequence::create(DelayTime::create(1.5f), ScaleTo::create(0.2,2),ScaleTo::create(0.2,1),CallFunc::create([=]{
            
            char buf[50] = {0};
            
            sprintf(buf,":%d",m_iAllStoneNum);
            
            m_pStoneAllNum->setString(buf);
        

            
            m_fEndPercent = CPersonalData::getInstance()->getUserKuangStone()*1.0/CPersonalData::getInstance()->getUserMaxKuangStone()*100;
            m_fStartPercent = m_progress->getPercentage();
            m_fSecPass = abs(m_fEndPercent-m_fStartPercent)/40;
            m_fAllPerTime = 0;
            
            m_pCurStoneNum->setBeatTime(m_fSecPass);
            
            
            int iPerStone = CPersonalData::getInstance()->getUserKuangStone()-m_addStone;
            
            m_pCurStoneNum->BeatUpTo(CPersonalData::getInstance()->getUserKuangStone(),iPerStone);
        
            m_bAddPer = true;
            
            m_addStone = 0;
            //CPersonalData::getInstance()->setUpdateUserInfo(true);
       
            
        }), NULL));
        pNomal->runAction(RepeatForever::create(RotateBy::create(1,360)));
    }
    
    float fTime = 1.0;
    
    Sprite *psmall =  Sprite::createWithSpriteFrameName("pvp_ore.png");
    psmall->setPosition(t);
    psmall->setScale(0.6);
    addChild(psmall,999);
    psmall->runAction(Sequence::create(MoveTo::create(0.5,Vec2(t.x+100,t.y+100)),MoveTo::create(1.5,Vec2(m_WinSize.width*0.8,m_WinSize.height*0.95)),RemoveSelf::create(),NULL));
    
    psmall->runAction(RepeatForever::create(RotateBy::create(fTime,360)));
    
    
    Sprite* pSmall2 = Sprite::createWithSpriteFrameName("pvp_ore.png");
    pSmall2->setPosition(t);
    pSmall2->cocos2d::Node::setScale(0.5);
    pSmall2->runAction(Sequence::create(MoveTo::create(0.5,Vec2(t.x-100,t.y+100)),MoveTo::create(1.25,Vec2(m_WinSize.width*0.8,m_WinSize.height*0.95)),RemoveSelf::create(),NULL));
    pSmall2->runAction(RepeatForever::create(RotateBy::create(fTime,360)));
    addChild(pSmall2,999);
    
    
    Sprite* pSmall3 = Sprite::createWithSpriteFrameName("pvp_ore.png");
    pSmall3->setPosition(t);
    pSmall3->cocos2d::Node::setScale(0.6);
    pSmall3->runAction(Sequence::create(MoveTo::create(0.5,Vec2(t.x-100,t.y+80)),MoveTo::create(1.1,Vec2(m_WinSize.width*0.8,m_WinSize.height*0.95)),RemoveSelf::create(),NULL));
    pSmall3->runAction(RepeatForever::create(RotateBy::create(fTime,360)));
    addChild(pSmall3,999);
    
    
    
    Sprite* pSmall4 = Sprite::createWithSpriteFrameName("pvp_ore.png");
    pSmall4->setPosition(t);
    pSmall4->cocos2d::Node::setScale(0.4);
    pSmall4->runAction(Sequence::create(MoveTo::create(0.5,Vec2(t.x+100,t.y-20)),MoveTo::create(1,Vec2(m_WinSize.width*0.8,m_WinSize.height*0.95)),RemoveSelf::create(),NULL));
    pSmall3->runAction(RepeatForever::create(RotateBy::create(fTime,360)));
    addChild(pSmall4,999);
    
}

void pvpMainLayer::gotoShop(Ref* pSender)
{
    //CPersonalData::getInstance()->setTouchRmvFlag(true);
    CLayerManager::GetSingletonPtr()->createLayer(eLayer_Charge,getParent(),false);
    String* tempStr = String::createWithFormat("1");
    EventParamMap paramMap;
    
    paramMap.insert(EventParamPair("点击次数",tempStr->getCString()));
    
    TDCCTalkingDataGA::onEvent("英雄宝石充值", &paramMap);
}

void pvpMainLayer::upgradeBtnCallBack(Ref* pSender)
{
    int tag = ((Node*)pSender)->getTag();
    
    //rmvTowerInfoLayer();
    if(PVP_BULIDING_UPGRADE == tag)//PVP_COMMSG_NOT_UPGRADE
    {
        if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_PVP_RONGLU) && (guiderManagerLayer::getInstance()->getStepNum() >= 5))
        {
            return;
        }
        //先判断当前是否是熔炉，如果熔炉正在熔炼则不能升级
        if(m_curBuild->pInfo->buildingType == 2)
        {
            for(int i = 0; i < (int)m_allBuildObb.size(); i++)
            {
                if(m_curBuild->theTower == m_allBuildObb[i].towerInfo->theTower)
                {
                    if(m_allBuildObb[i].state == 3)
                    {
                        Sprite * thetarget = CFightManager::getTheFlyWord("熔炼过程中不可升级","fonts/cuti.ttf");
                        thetarget->setPosition(m_winSize.width*0.5,m_winSize.height*0.5);
                        addChild(thetarget,100);
                        return;
                    }
                }
            }
        }
        bool ret = adjustIfUpgrade(m_curBuild->pInfo->buildingID);
        if(ret)
        {
            m_curBuild->theTower->gettheSprite()->stopAllActions();
            m_curBuild->theTower->gettheSprite()->setColor(Color3B(255, 255, 255));
            m_layer = pvpCommonInfoLayer::create(m_curBuild->pInfo->buildingID, PVP_COMMSG_UPGRADE);
            addChild(m_layer, 100);
        }
        else
        {
            __String* temp = NULL;
            int level = getUpgradeLimit(m_curBuild->pInfo->buildingID);
            if(level >= 7)
            {
                 temp = __String::createWithFormat( "该建筑等级已达上限");
            }
            else{
                 temp = __String::createWithFormat( "请先将圣像升到%d级！", level);
            }
            m_layer = pvpCommonInfoLayer::create(m_curBuild->pInfo->buildingID, PVP_COMMSG_NOT_UPGRADE, temp->getCString());
            addChild(m_layer, 100);
        }
        
    }
    else if(PVP_BULIDING_FINISH == tag)
    {
        int stoneNum = m_remainTime/60;
        if(stoneNum == 0)
        {
            stoneNum = 1;
        }
        
        if((CPersonalData::getInstance()->getUserStone() < stoneNum) && (guiderManagerLayer::getInstance()->getGuideType() != GUIDE_PVP_BUILDING))
        {
//            Sprite * thetarget = CFightManager::getTheFlyWord("魔法石不足","fonts/cuti.ttf");
//            thetarget->setPosition(m_winSize.width*0.5,m_winSize.height*0.5);
//            addChild(thetarget,100);
            CHitDialog *pDlg = CHitDialog::create();
            pDlg->createYesNO(this, menu_selector(pvpCommonInfoLayer::gotoShop),"魔法石不足，是否去充值？");
            addChild(pDlg, 100);
            return;
        }
        __String* tempStr = __String::createWithFormat("%d", stoneNum);
        EventParamMap paramMap;
        
        paramMap.insert(EventParamPair("升级花费魔法石",tempStr->getCString()));
        TDCCTalkingDataGA::onEvent("升级", &paramMap);
        //更新建筑升级信息到服务器
        for(int i = 0; i < (int)m_curBuild->pInfo->blockPos.size(); i++)
        {
            float xPos = m_curBuild->theTower->getPositionX() + m_curBuild->pInfo->blockPos[i].x * LENGTH_BLOCK_PER;
            float zPos = m_curBuild->theTower->getPositionZ() - m_curBuild->pInfo->blockPos[i].y * LENGTH_BLOCK_PER;
            int index = (xPos - MAP_START_XPOS)/LENGTH_BLOCK_PER;
            int indey = (zPos - STARTZPOINT)/LENGTH_BLOCK_PER + 9;
            m_allBlock[index][indey].sprite->setVisible(false);
        }
        finsishUpgradeImmediate();
    }
    
    else if(tag == PVP_BULIDING_OK)
    {
        if(!m_bValid)
        {
            Sprite * thetarget = CFightManager::getTheFlyWord("当前位置已有建筑，拖动建筑换个地方试试吧~","fonts/cuti.ttf");
            thetarget->setPosition(m_winSize.width*0.5,m_winSize.height*0.5);
            addChild(thetarget,100);
            return;
        }
        
        if(guiderManagerLayer::getInstance()->getGuideType() == GUIDE_PVP_BUILDING)
        {
            if(guiderManagerLayer::getInstance()->getStepNum() == 7)
            {
                Vec3 pos = m_upgradeTower->theTower->getPosition3D();
                CDataManager::saveGuideInfoToDB(GUIDE_PVP_BUILDING);
                Vec2 targetPos = CommonData::GetSingletonPtr()->getPosOnScreen(_camera, pos);
                guiderManagerLayer::getInstance()->setStepNum(guiderManagerLayer::getInstance()->getStepNum() + 1);
                guiderManagerLayer::getInstance()->createLayer(this, GUIDE_SHOW_FINGER, targetPos);
            }
            else{
                return;
            }
        }
        sureCreateTower();
        m_bValid = false;
    }
    else if(tag == PVP_BUILDING_KUANSHI)
    {
        LYMenuItemSprite* temp = (LYMenuItemSprite*)pSender;
        m_userBuildingId = (long)temp->getUserData();
        __String* tpf = __String::createWithFormat("%d", (int)m_userBuildingId);
        //计算玩家矿场生产上限
        int allNum = 100;
        CBaseBuilding* pTemp = NULL;
        for(int i = 0; i < (int)m_allBuildObb.size(); i++)
        {
            if(m_allBuildObb[i].userBuildingId == m_userBuildingId)
            {
                pTemp = m_allBuildObb[i].towerInfo->theTower;
            }
            pvpBuildInfo* pInfo = CDataManager::getBuildInfo(m_allBuildObb[i].buildingId);
            if(m_allBuildObb[i].state != 0)
            {
                pInfo = CDataManager::getBuildInfo(m_allBuildObb[i].buildingId - 1);
            }
            //            if(pInfo->buildingType == 10)
            //            {
            //                allNum += pInfo->effectTime;
            //            }
        }
        //updateKuangchangState(pTemp);
        for(int i = 0; i < (int)m_pKuanchanBuildVec.size(); i++)
        {
            if(m_pKuanchanBuildVec[i].towerInfo->theTower == pTemp)
            {
                m_pKuanchanBuildVec[i].validTime = 0;
            }
        }
        CNetControlModule::sharedNetControlModule()->collectStone(this, menu_selector(pvpMainLayer::collectStoneBack), tpf->getCString());
    }
    else if(tag == PVP_BUILDING_PRODUCT)
    {
        for(int i = 0; i < (int)m_allBuildObb.size(); i++)
        {
            if(m_curBuild->theTower == m_allBuildObb[i].towerInfo->theTower)
            {
                m_userBuildingId = m_allBuildObb[i].userBuildingId;
                break;
            }
        }
        m_layer = pvpCommonInfoLayer::create(m_curBuild->pInfo->buildingID, m_userBuildingId, PVP_COMMSG_PRORUCT);
        addChild(m_layer, 100);
    }
    else if(tag == PVP_BULIDING_DEL)
    {
        if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_PVP_BUILDING) && (guiderManagerLayer::getInstance()->getStepNum() >= 10))
        {
            return;
        }
        log("取消升级");
        m_layer = pvpCommonInfoLayer::create(m_curBuildingId, PVP_COMMSG_CANCEL_UPGRADE);
        addChild(m_layer, 100);
        m_layer->addCancelUpgrade(this, menu_selector(pvpMainLayer::cancelUpgrade));
    }
    
    else if(tag == PVP_BUILDING_DINGSHI)
    {
        
        log("立即完成");
        m_layer = pvpCommonInfoLayer::create(m_curBuildingId, PVP_COMMSG_DINGSHI);
        addChild(m_layer, 100);
        //m_layer->addCancelUpgrade(this, menu_selector(pvpMainLayer::cancelUpgrade));
        
    }
    else if(tag == PVP_BUILDING_EVOLUTION)
    {
        m_layer = pvpCommonInfoLayer::create(m_curBuildingId, m_userBuildingId, PVP_COMMSG_EVOLUTION);
        addChild(m_layer, 100);
        //m_layer->addEvolutionInfo(this, menu_selector(pvpMainLayer::evolutionBack));
    }
    
    if(tag!= PVP_BUILDING_KUANSHI)
    {
        rmvBtnAndArrow();
        rmvSelectedBlock();
    }
}

void pvpMainLayer::addPattolInfo(buildInfo* pTower, bool bEvolution)
{
    if(m_patrolman)
    {
        m_patrolman->stopAllActions();
        m_patrolman->unscheduleUpdate();
        pvpCommonData::getInstance()->setUpBack(true);
        m_patrolman->setIndex(0);
        m_patrolman->removeFromParent();
        m_patrolman = NULL;
    }
    int monsterId = pTower->pInfo->allEvolutionVec[0].monsterId;
    if(bEvolution)
    {
        monsterId = pTower->pInfo->allEvolutionVec[1].monsterId;
    }
    int index = (int)pTower->pInfo->allEvolutionVec.size() - 1;
    if(pvpCommonData::getInstance()->getEvolution() == 1)
    {
        monsterId = pTower->pInfo->allEvolutionVec[index].monsterId;
    }
    CDataManager::CalcFighterInfoWith(&m_thePatrolInfo, monsterId, 1, 1, 0, false, 0, 0, 0, 0);
    
    m_patrolman = CPartolFight::create(&m_thePatrolInfo, eFightGroup_Freind);
    layer3D->addChild(m_patrolman);
    pvpCommonData::getInstance()->setTowerInfo(pTower);
    m_patrolman->setMonsterId(monsterId);
    
    Vec3 pos = Vec3(pTower->theTower->getPositionX() - 0.5*LENGTH_BLOCK_PER, pTower->theTower->getPositionY(),
                    pTower->theTower->getPositionZ() - 0.5*LENGTH_BLOCK_PER);
    m_patrolman->setPosition3D(pos);
    
    Vec3 destPos = m_patrolman->getValidPosition();
    m_patrolman->setAddHeight(0);
    m_patrolman->setTargetPos(Vec2(pTower->theTower->getPositionX(), pTower->theTower->getPositionZ()));
    m_patrolman->setDestationPos(destPos);
    m_patrolman->setCameraMask((unsigned short )CameraFlag::USER1);
    m_patrolman->scheduleUpdate();
    
    m_patrolman->setMaxHP(pTower->pInfo->allEvolutionVec[index].life);
    m_patrolman->setNowAttack(pTower->pInfo->allEvolutionVec[index].attack);
}

void pvpMainLayer::evolutionBack()
{
    if(m_layer)
    {
        m_layer->removeFromParent();
        m_layer = NULL;
    }
    CEvolutionLayer* layer = CEvolutionLayer::create();
    addChild(layer,100);
    //更新巡逻怪物
    log("");
    addPattolInfo(m_curBuild, true);
    //发送消息更新进化标识
    CNetControlModule::sharedNetControlModule()->updateUserEvolution(this, NULL);
    m_curBuild->theTower->gettheSprite()->stopAllActions();
    m_curBuild->theTower->gettheSprite()->setColor(Color3B(255, 255, 255));
    m_curBuild->theTower->gettheSprite()->setOpacity(255);
    m_curBuild = NULL;
}

void pvpMainLayer::collectStoneBack(Ref* pSender)
{
    _netRecvEventCollectStone* pInfo = (_netRecvEventCollectStone*)pSender;
    int state = 1;
    int allStone = 0;
    for(std::vector<std::string>::iterator it = pInfo->m_paramsStr.begin(); it!= pInfo->m_paramsStr.end(); it++)
    {
        string str = *it;
        string name = CDataManager::getStrFromBackStr(str);
        
        if(name == "state")
        {
            state = atoi(CDataManager::getDigitFromBackStr(*it).c_str());
            if(state != 0)
            {
                log("采集矿石失败");
                return;
            }
        }
        else if (name == "ksAddNum")
        {
            m_addStone = atoi(CDataManager::getDigitFromBackStr(*it).c_str());
            
            CPersonalData::getInstance()->setUserKuangStone(CPersonalData::getInstance()->getUserKuangStone() + m_addStone);
        }
        else if(name == "ksNum")
        {
            allStone = atoi(CDataManager::getDigitFromBackStr(*it).c_str());
        }
    }
    //找出数字出现的位置
    int index = 0;
    for(int i = 0; i < (int)m_pKuanchanBuildVec.size(); i++)
    {
        if(m_pKuanchanBuildVec[i].userBuildingId == m_userBuildingId)
        {
            index = i;
            break;
        }
    }
    Vec3 pos = m_pKuanchanBuildVec[index].towerInfo->theTower->getPosition3D();
    Vec2 targetPos = CommonData::GetSingletonPtr()->getPosOnScreen(_camera, pos);
    float height =  m_pKuanchanBuildVec[index].towerInfo->theTower->getBoundingBox().size.height * 1.25f;
    __String* tep = __String::createWithFormat("%d", m_addStone);
    Sprite * thetarget = CFightManager::getTheFlyWord(tep->getCString(),"fonts/cuti.ttf");
    thetarget->setPosition(targetPos.x, targetPos.y + height);
    addChild(thetarget,100);
    ZhaiBaoshi();
    //判断当前矿石是否已经达到上限
}

void pvpMainLayer::searchCallBack(Ref* pSender)
{
//    CLayerManager::GetSingletonPtr()->createLayer(eLayer_PVPLayer, NULL,false);
    //调用云散开函数
    CFightManager::closeCloud(NULL,NULL,m_pLayer);
    runAction(Sequence::create(DelayTime::create(2.0f), CallFunc::create([=]{
        CPersonalData::getInstance()->setPvpMainLayer(true);
    }), NULL));
}

void pvpMainLayer::getEnemyTeamInfoBack(Ref *pSender)
{
    _netRecvEventGetUserTeamInfo* pInfo = (_netRecvEventGetUserTeamInfo*)pSender;
    std::vector<std::string>  p =  pInfo->m_paramsStr;
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
                //如果没有搜索到玩家
                m_searchBg->removeFromParent();
                m_layer = pvpCommonInfoLayer::create(PVP_COMMSG_NOFIND);
                addChild(m_layer, 1000);
                m_layer->showSearchResult(this, menu_selector(pvpMainLayer::searchCallBack));
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
    
    pvpEnemyInfo pEnemyInfo;
    
    pEnemyInfo.m_iLevel = m_level;
    pEnemyInfo.m_iUserId = m_userId;
    pEnemyInfo.m_sName = m_name;
    pEnemyInfo.m_iRobStone  = m_stone;
    
    CFightData::getInstance()->setThePvpEnemyInfo(pEnemyInfo);
    
    std::string StrEnemyMaps = m_enemyMaps;
    
    char buf[256];
    std::vector<std::string> tempValue;
    sprintf(buf, "%d", EPVPMode_PLAY);
    tempValue.push_back(buf);
    sprintf(buf, "%d", m_stone);
    tempValue.push_back(buf);
    tempValue.push_back(StrEnemyMaps);
    
    if(CDataManager::getEndedBy(GUIDE_PVP_ROB))//如果是引导搜索则限制玩家只能搜索固定目标
    {
        //保存搜索到的玩家id
        CDBMgr::getInstance()->saveTempUserInfo(m_userId);
    }
    CFightData::getInstance()->setFightType(FIGHT_PVP_TYPE);
    CLayerManager::GetSingletonPtr()->createLayer(eLayer_FightPVP, NULL,false, true,&tempValue);
    return;
}



void  pvpMainLayer::Enemy(Ref *pSender)
{
    m_searchBg = Sprite::createWithSpriteFrameName("pvpSearch.png");
    addChild(m_searchBg, 999);
    m_searchBg->setPosition(Vec2(m_winSize.width * 0.5f, m_winSize.height * 0.5f));
    cocos2d::Size bgSize = m_searchBg->getBoundingBox().size;
    
    m_searchBg->setOpacity(0);
    
    m_searchBg->runAction(FadeIn::create(0.5));
    
    Label * pLabel = Label::createWithTTF("搜索", "fonts/cuti.ttf", 30);
    m_searchBg->addChild(pLabel);
    pLabel->enableOutline(Color4B(0, 0, 0, 255), 3);
    pLabel->setPosition(Vec2(bgSize.width * 0.5f, bgSize.height * 0.9f));
    
    Sprite* pStart = Sprite::createWithSpriteFrameName("pvpGlass.png");
    m_searchBg->addChild(pStart);
    pStart->setPosition(Vec2(bgSize.width * 0.5f, bgSize.height * 0.55f));
    
    ActionInterval* action1 = Sequence::create(EaseSineOut::create(MoveBy::create(0.4f, Vec2(0, 15))),
                                               EaseSineIn::create(MoveBy::create(0.4f, Vec2(0, -15))),
                                               EaseSineOut::create(MoveBy::create(0.4f, Vec2(0, -15))),
                                               EaseSineIn::create(MoveBy::create(0.4f, Vec2(0, 15))), NULL);
    ActionInterval* action2 = Sequence::create(EaseSineIn::create(MoveBy::create(0.4f, Vec2(15, 0))),
                                               EaseSineOut::create(MoveBy::create(0.4f, Vec2(15, 0))),
                                               EaseSineIn::create(MoveBy::create(0.4f, Vec2(-15, 0))),
                                               EaseSineOut::create(MoveBy::create(0.4f, Vec2(-15, 0))), NULL);
    pStart->runAction(RepeatForever::create(action1));
    pStart->runAction(RepeatForever::create(action2));
    Label* pSearch = Label::createWithSystemFont("正在搜索对手...", "Arial", 24);
    m_searchBg->addChild(pSearch);
    pSearch->setColor(Color3B::BLACK);
    pSearch->setPosition(Vec2(bgSize.width * 0.5f, bgSize.height * 0.28f));
    
    
    runAction(Sequence::create(DelayTime::create(1),CallFunc::create([=]{
        
        m_searchBg->setCascadeOpacityEnabled(true);
        m_searchBg->runAction(FadeOut::create(0.25));
        
        std::string content = CDBMgr::getInstance()->getTempAllUser();
        int level = CPersonalData::getInstance()->getUserInfo()->m_mainBaseLevel;
        
        
        //如果是玩家首次搜索泽指定一个userId
        if(content.empty())
        {
            CNetControlModule::sharedNetControlModule()->getEnemyTeamInfo(this, menu_selector(pvpMainLayer::getEnemyTeamInfoBack),3133);
        }
        else{
            CNetControlModule::sharedNetControlModule()->getRandomEnemyTeamInfo(this, menu_selector(pvpMainLayer::getEnemyTeamInfoBack), level, content);
        }
//        CNetControlModule::sharedNetControlModule()->getEnemyTeamInfo(this, menu_selector(pvpMainLayer::getEnemyTeamInfoBack),userId);
        
    }),NULL));
    
}
void pvpMainLayer::startSearchEnemy()
{
    
    int userId = CPersonalData::getInstance()->getUserInfo()->m_iUserId;
    CNetControlModule::sharedNetControlModule()->getEnemyTeamInfo(this, menu_selector(pvpMainLayer::getEnemyTeamInfoBack), userId);
    
//    CNetControlModule::sharedNetControlModule()->getEnemyTeamInfo(this, menu_selector(pvpMainLayer::getEnemyTeamInfoBack), 3974);

    
}



void pvpMainLayer::btnCallBack(Ref* pSender)
{
    int tag = ((Node*)pSender)->getTag();
    //rmvTowerInfoLayer();
    if(PVP_BACK_TAG == tag)
    {
        if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_PVP_BUILDING) && (guiderManagerLayer::getInstance()->getStepNum() == 6))
        {
            return;
        }
        CLayerManager::GetSingletonPtr()->createLayer(eLayer_MainMenu, NULL,false);
        return;
    }
    else if(PVP_BULIDING_INFO == tag)
    {
        if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_PVP_RONGLU) && (guiderManagerLayer::getInstance()->getStepNum() >= 5))
        {
            return;
        }
        m_curBuild->theTower->gettheSprite()->stopAllActions();
        m_curBuild->theTower->gettheSprite()->setColor(Color3B(255, 255, 255));
        m_layer = pvpCommonInfoLayer::create(m_curBuild->pInfo->buildingID, PVP_COMMSG_DETAIL);
        addChild(m_layer, 100);
    }
    
    else if(PVP_FIGHT_TAG == tag)
    {
        long iCost = (long)m_pFightItem->getUserData();
        if(CPersonalData::getInstance()->getUserCoin() < iCost)//暂时定为这么多
        {
            Sprite * thetarget = CFightManager::getTheFlyWord("金币不足","fonts/cuti.ttf");
            thetarget->setPosition(m_winSize.width*0.5,m_winSize.height*0.5);
            addChild(thetarget,100);
            return;
        }
        if(guiderManagerLayer::getInstance()->getGuideType() == GUIDE_PVP_BUILDING)
        {
            if(guiderManagerLayer::getInstance()->getStepNum() == 6)
            {
                return;
            }
        }
        if(pvpCommonData::getInstance()->getProtectedTime() > 0)
        {
            m_layer = pvpCommonInfoLayer::create(PVP_COMMSG_PROTECTTIME);
            m_layer->clearProtectTime(this, menu_selector(pvpMainLayer::startPvpFight));
            addChild(m_layer, 100);
        }
        else{
            startPvpFight(NULL);
        }
    }
    else if(PVP_STAMINA_CHARGE == tag)
    {
        CommonPropLayer* pLayer = CommonPropLayer::create(STONE_EXCHANGE_STAMINA);
        addChild(pLayer, 999);
        __String* tempStr = __String::createWithFormat("1");
        EventParamMap paramMap;
        
        paramMap.insert(EventParamPair("点击次数",tempStr->getCString()));
        
        TDCCTalkingDataGA::onEvent("充体力", &paramMap);
        
    }
    else if(PVP_STONE_CHARGE == tag)
    {
        CLayerManager::GetSingletonPtr()->createLayer(eLayer_Charge,this,false);
        __String* tempStr = __String::createWithFormat("1");
        EventParamMap paramMap;
        
        paramMap.insert(EventParamPair("点击次数",tempStr->getCString()));
        
        TDCCTalkingDataGA::onEvent("充宝石", &paramMap);
    }
    else if(PVP_COIN_CHARGE == tag)
    {
        CommonPropLayer* pLayer = CommonPropLayer::create(STONE_EXCHANGE_COIN);
        addChild(pLayer, 999);
        __String* tempStr = __String::createWithFormat("1");
        EventParamMap paramMap;
        
        paramMap.insert(EventParamPair("点击次数",tempStr->getCString()));
        
        TDCCTalkingDataGA::onEvent("充金币", &paramMap);
    }
    else if(tag == PVP_MESSAGE_TAG)
    {
        Label* msgLabel = (Label*)m_msgItem->getChildByTag(100);
        
        if(msgLabel)
        {
            msgLabel->removeFromParent();
            msgLabel = NULL;
        }
        m_iTipNum = 0;
        pvpWarReportLayer* layer = pvpWarReportLayer::create();
        addChild(layer, 1000);
        //通知服务器已经查看
    }
    else if(tag == PVP_BULIDING_CANCEL)
    {
        if(guiderManagerLayer::getInstance()->getGuideType() == GUIDE_PVP_BUILDING)
        {
            return;
        }
        rmvNewTower();
        m_bValid = false;
        //删除
    }
    else
    {
        if(m_bNewTower && m_bFirst && m_curBuild)
        {
            rmvNewTower();
        }
        if(m_curBuild)
        {
            if(m_curBuild->towerBase)
            {
                m_curBuild->towerBase->setVisible(true);
            }
            if(m_curBuild->shadow)
            {
                m_curBuild->shadow->setVisible(true);
            }
            m_curBuild->theTower->gettheSprite()->stopAllActions();
            m_curBuild->theTower->gettheSprite()->setColor(Color3B(255, 255, 255));
        }
        setBtnEnable();
        showBuildList();
    }
    rmvSelectedBlock();
    if(m_curBuild)
    {
        for(int i = 0; i < (int)(m_curBuild->pInfo->blockPos.size()); i++)
        {
            float xPos = m_curBuild->theTower->getPositionX() + m_curBuild->pInfo->blockPos[i].x * LENGTH_BLOCK_PER;
            float zPos = m_curBuild->theTower->getPositionZ() - m_curBuild->pInfo->blockPos[i].y * LENGTH_BLOCK_PER;
            int index = (xPos - MAP_START_XPOS)/LENGTH_BLOCK_PER;
            int indey = (zPos - STARTZPOINT)/LENGTH_BLOCK_PER + 9;
            m_allBlock[index][indey].sprite->setVisible(false);
        }
    }
    rmvBtnAndArrow();
    //m_curBuild = NULL;
}

void pvpMainLayer::startPvpFight(Ref* pSender)
{
    if(pSender)
    {
        CNetControlModule::sharedNetControlModule()->updateUserProtectTime(this, NULL);
    }
    if(m_layer)
    {
        m_layer->removeFromParent();
        m_layer = NULL;
    }
    CNetWorkDataSingle::GetSingletonPtr()->SaveUserMCF(this,menu_selector(pvpMainLayer::updateUserData),-156,0,0);
    m_curBuild = NULL;
    CPersonalData::getInstance()->setEnemyType(RANDOM_ENEMY);
    CPersonalData::getInstance()->setPvpMainLayer(false);
    
    m_pLayer = LayerColor::create(Color4B(0, 0, 0, 0));
    addChild(m_pLayer, 900);
    m_pLayer->setPosition(Vec2::ZERO);
    
    m_pFightItem->setEnabled(false);
    if(m_patrolman)
    {
        m_patrolman->unscheduleUpdate();
        m_patrolman->removeFromParent();
        m_patrolman = NULL;
    }
    
    setBtnEnable(false);
    
    CFightManager::addCloud(this,menu_selector(pvpMainLayer::Enemy),m_pLayer);
}

void pvpMainLayer::directlyFinish()
{
    pvpBuildInfo* pInfo = CDataManager::getBuildInfo(m_curBuildingId);
    pvpBuildInfo* pNextInfo = CDataManager::getBuildInfo(pInfo->nextBuildingID);
    int iCostStone = pNextInfo->costTime/60;
    if(iCostStone == 0)
    {
        iCostStone = 1;
    }
    if((CPersonalData::getInstance()->getUserStone() < iCostStone) && (CDataManager::getEndedBy(GUIDE_PVP_BUILDING)))
    {
        Sprite * thetarget = CFightManager::getTheFlyWord("魔法石不足","fonts/cuti.ttf");
        thetarget->setPosition(m_winSize.width*0.5,m_winSize.height*0.5);
        addChild(thetarget,100);
        return;
    }
    if(m_layer)
    {
        CPersonalData::getInstance()->setTouchRmvFlag(true);
        m_layer->removeFromParent();
        m_layer = NULL;
    }
    
    BuildingFininsh(m_curBuild);
    
    int index = 0;
    for(int i = 0; i < (int)m_allBuildObb.size(); i++)
    {
        if(m_curBuild && (m_allBuildObb[i].towerInfo->theTower == m_curBuild->theTower))
        {
            
            if(m_curBuild->towerBase)
            {
                m_curBuild->towerBase->setVisible(true);
            }
            if(m_curBuild->shadow)
            {
                m_curBuild->shadow->setVisible(true);
            }
            index = i;
            m_allBuildObb[i].buildingId = pInfo->nextBuildingID;
            m_allBuildObb[i].state = 0;
            m_allBuildObb[i].towerInfo->pInfo = CDataManager::getBuildInfo(pInfo->nextBuildingID);
            if(m_curBuild->pInfo->buildingType == 0)
            {
                detectIfCanCreate();
                CDataManager::saveTaskInfoToDB(20, m_curBuild->pInfo->level, true);
            }
            else if(m_curBuild->pInfo->buildingType == 1)//如果升级的是矿石库泽发送消息更新矿石上限
            {
                updateKuangshiLimit();
            }
            else if(m_curBuild->pInfo->buildingType == 10)
            {
                updateStoneRate();
                updateKuangchangState(m_curBuild->theTower);
            }
            else if(m_curBuild->pInfo->buildingType == 11)
            {
                addPattolInfo(m_curBuild);
            }
        }
    }
    
    
    monsterInfo* pMonster = CDataManager::getMonsterInfoByMonsterId(pInfo->monsterId);
    __String *temp = __String::createWithFormat("%s已升级到%d级", pMonster->monsterName.c_str(), pInfo->level + 1);
    Sprite * thetarget = CFightManager::getTheFlyWord(temp->getCString(),"fonts/cuti.ttf");
    thetarget->setPosition(m_winSize.width*0.5,m_winSize.height*0.5);
    addChild(thetarget,100);
    
    int coin = pNextInfo->cost;
    CNetWorkDataSingle::GetSingletonPtr()->SaveUserMCF(this, menu_selector(pvpMainLayer::updateUserData), -coin, -iCostStone, 0);
    TDCCItem::onPurchase("pvp直接完成升级", 1, iCostStone);
    sendRequestSaveTeamInfo();
    if(m_curBuild)
    {
        m_curBuild->theTower->ChangeModelWithPVPBuildingId(pInfo->nextBuildingID);
    }
    m_curBuild = NULL;
}

void pvpMainLayer::finsishUpgradeImmediate()
{
    
    m_curBuild->theTower->ChangeModelWithPVPBuildingId(m_curBuildingId);//刷新建筑模型显示
    if(m_curBuild->towerBase)
    {
        m_curBuild->towerBase->setVisible(true);
    }
    if(m_curBuild->shadow)
    {
        m_curBuild->shadow->setVisible(true);
    }
    if(m_curBuild->theTower->m_pAtkRangeCircle->isVisible())
    {
        m_curBuild->theTower->m_pAtkRangeCircle->setVisible(false);
    }
    rmvUpgradeInfo();
    
    BuildingFininsh(m_curBuild);
    
    //m_upgradeEnd = true;
    //升级结束保存玩家建筑信息
    int i = 0;
    
    for( ;i < (int)m_allBuildObb.size(); i++)
    {
        if(m_upgradeTower && (m_allBuildObb[i].towerInfo->theTower == m_upgradeTower->theTower))
        {
            m_allBuildObb[i].state = 0;
            pvpBuildInfo* pInfo = CDataManager::getBuildInfo(m_allBuildObb[i].buildingId);
            if(pInfo->buildingType == 0)
            {
                detectIfCanCreate();
                CDataManager::saveTaskInfoToDB(20, m_upgradeTower->pInfo->level, true);
            }
            if(pInfo->buildingType == 1)//如果升级的是矿石库泽发送消息更新矿石上限
            {
                updateKuangshiLimit();
            }
            if(pInfo->buildingType == 10)
            {
                updateStoneRate();
                updateKuangchangState(m_upgradeTower->theTower);
            }
            if(pInfo->buildingType == 11)//如果是祭坛则添加怪物
            {
                addPattolInfo(m_upgradeTower);
            }
            m_allBuildObb[i].towerInfo->pInfo = pInfo;
            break;
        }
    }
    int iCostStone = m_remainTime/60;
    if(iCostStone == 0)
    {
        iCostStone = 1;
    }
    
    //spf
    
    pvpBuildInfo* pBuildingInfo = CDataManager::getBuildInfo(m_allBuildObb[i].buildingId);
    
    monsterInfo* pMonster = CDataManager::getMonsterInfoByMonsterId(m_upgradeTower->pInfo->monsterId);
    __String *temp = __String::createWithFormat("%s已升级到%d级", pMonster->monsterName.c_str(), pBuildingInfo->level);
    
    Sprite * thetarget = CFightManager::getTheFlyWord(temp->getCString(),"fonts/cuti.ttf");
    thetarget->setPosition(m_winSize.width*0.5,m_winSize.height*0.5);
    addChild(thetarget,100);
    
    sendRequestSaveTeamInfo();
    m_remainTime = 0;
    
    if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_PVP_BUILDING) && (guiderManagerLayer::getInstance()->getStepNum() == 10))
    {
        guiderManagerLayer::getInstance()->createLayer(this);
    }
    else
    {
        CNetWorkDataSingle::GetSingletonPtr()->SaveUserMCF(this, menu_selector(pvpMainLayer::updateUserData), 0, -iCostStone, 0);
        TDCCItem::onPurchase("pvp立即完成升级", 1, iCostStone);
    }
    m_upgradeBuildId = 0;
    m_curBuild = NULL;
}


void pvpMainLayer::rmvNewTower()
{
    m_bFirst = false;
    m_cost = 0;
    if(m_curBuild->pInfo->level == 1)
    {
        m_curBuild->theTower->removeFromParent();
        if(m_curBuild->shadow)
        {
            m_curBuild->shadow->removeFromParent();
        }
        if(m_curBuild->towerBase)
        {
            m_curBuild->towerBase->removeFromParent();
        }
    }
    else {
        m_curBuild->theTower->gettheSprite()->stopAllActions();
        m_curBuild->theTower->gettheSprite()->setColor(Color3B(255, 255, 255));
    }
    //m_curBuild->pInfo = NULL;
    m_curBuild = NULL;
    log("删除建筑");
    rmvSelectedBlock();
    m_bNewTower = false;
    
    rmvTowerInfoLayer();
    rmvBtnAndArrow();
}

void pvpMainLayer::sureCreateTower()
{
    pvpBuildInfo* pBuildingInfo = CDataManager::getBuildInfo(m_curBuildingId);
    if(m_remainTime > 0)
    {
        char buf[1024];
        pvpBuildInfo* pInfo = CDataManager::getBuildInfo(m_upgradeBuildId);
        monsterInfo* pMonsterInfo = CDataManager::getMonsterInfoByMonsterId(pInfo->monsterId);
        
        monsterInfo* pBuildingMonsterInfo = CDataManager::getMonsterInfoByMonsterId(pBuildingInfo->monsterId);
        m_neddStone = m_remainTime / 60;
        m_neddStone = MAX(1, m_neddStone);
        m_cost = pBuildingInfo->cost;;
        sprintf(buf, "确定要完成%s的升级后开始创建%s吗？", pMonsterInfo->monsterName.c_str() , pBuildingMonsterInfo->monsterName.c_str());
        m_layer = pvpCommonInfoLayer::create(PVP_COMMSG_UPGRADE_TIP);
        addChild(m_layer, 100);
        m_layer->addCostStoneTip(this, menu_selector(pvpMainLayer::sureUpgrade), buf, m_neddStone);
    }
    else{
        addTowerOBBInfo();
        m_remainTime = pBuildingInfo->costTime;
        for(int i = 0; i < (int)(m_curBuild->pInfo->blockPos.size()); i++)
        {
            float xPos = m_curBuild->theTower->getPositionX() + m_curBuild->pInfo->blockPos[i].x * LENGTH_BLOCK_PER;
            float zPos = m_curBuild->theTower->getPositionZ() - m_curBuild->pInfo->blockPos[i].y * LENGTH_BLOCK_PER;
            int index = (xPos - MAP_START_XPOS)/LENGTH_BLOCK_PER;
            int indey = (zPos - STARTZPOINT)/LENGTH_BLOCK_PER + 9;
            m_allBlock[index][indey].type = 1;
            m_allBlock[index][indey].sprite->setVisible(false);
        }
        m_upgradeBuildId = m_curBuildingId;
        m_upgradeTower = m_curBuild;
        
        if(m_remainTime == 0)
        {
            createDirectly();
            addStartUpgradeInfo(0);//添加底座和影子
            sendRequestSaveTeamInfo();
        }
        else{
            addUpgradeInfo(true);
            addStartUpgradeInfo();//添加底座和影子
            sendRequestSaveTeamInfo(true);
        }
    }
    m_bFirst = false;
}

void pvpMainLayer::createDirectly()
{
    rmvUpgradeInfo();
    BuildingFininsh(m_upgradeTower);
    int index = 0;
    for(int i = 0; i < (int)m_allBuildObb.size(); i++)
    {
        if(m_upgradeTower && (m_allBuildObb[i].towerInfo->theTower == m_upgradeTower->theTower))
        {
            index = i;
        }
    }
    m_upgradeTower->theTower->gettheSprite()->stopAllActions();
    m_upgradeTower->theTower->gettheSprite()->setColor(Color3B(255, 255, 255));
    m_allBuildObb[index].state = 0;
    pvpBuildInfo* pInfo = CDataManager::getBuildInfo(m_upgradeBuildId);
    if((pInfo->buildingType == 11) && (pInfo->level == 1))//如果是祭坛则添加怪物
    {
        addPattolInfo(m_upgradeTower);
    }
    if(pInfo->buildingType == 10)
    {
        updateKuangchangState(m_upgradeTower->theTower);
        updateStoneRate();
    }
    if(pInfo->buildingType == 1)//如果升级的是矿石库泽发送消息更新矿石上限
    {
        updateKuangshiLimit();
    }
    if(pInfo->buildingType == 0)
    {
        detectIfCanCreate();
    }
    m_curBuild->theTower->ChangeModelWithPVPBuildingId(m_upgradeBuildId);
    m_upgradeEnd = true;
    m_curBuild = NULL;
}

void pvpMainLayer::addStartUpgradeInfo(int state)
{
    //如果已经有阴影和底座则直接返回
    int index = 0;
    for(int i = 0; i < (int)m_allBuildObb.size(); i++)
    {
        if(m_upgradeTower && (m_allBuildObb[i].towerInfo->theTower == m_upgradeTower->theTower))
        {
            m_allBuildObb[i].buildingId = m_upgradeBuildId;
            m_allBuildObb[i].state = state;
            index = i;
            m_allBuildObb[i].towerInfo->pInfo = CDataManager::getBuildInfo(m_upgradeBuildId);
            break;
        }
    }
    if(m_upgradeTower && (m_upgradeTower->towerBase) && (m_upgradeTower->shadow))
    {
        return;
    }
    std::string bgImg;
    float scale = 1.0f;
    if(m_upgradeTower->pInfo->blockPos.size() == 1)
    {
        bgImg = "pvp_one.png";
        scale = 1.1f;
    }
    else if(m_upgradeTower->pInfo->blockPos.size() == 4)
    {
        bgImg = "pvp_four.png";
        scale = 2.0f;
    }
    else if(m_upgradeTower->pInfo->blockPos.size() == 9)
    {
        bgImg = "pvp_nine.png";
        scale = 3.2f;
    }
    
    //影子shadow
    
    Sprite3D* pShadow = CFightManager::getTheModelWithPath("touying.png");
    layer3D->addChild(pShadow, 100);
    pShadow->getChildByTag(100)->setScale(scale);
    pShadow->getChildByTag(100)->setOpacity(170);
    pShadow->setRotation3D(Vec3(-90,0,0));
    pShadow->setPosition3D(m_upgradeTower->theTower->getPosition3D());
    pShadow->setCameraMask((unsigned short )CameraFlag::USER1);
    
    Sprite3D* TowerBg = CFightManager::getTheModelWithPath(bgImg.c_str());
    layer3D->addChild(TowerBg);
    TowerBg->setPosition3D(m_upgradeTower->theTower->getPosition3D());
    TowerBg->setRotation3D(Vec3(270, 0, 0));
    TowerBg->getChildByTag(100)->setScale(0.6f);
    TowerBg->setCameraMask((unsigned short )CameraFlag::USER1);
    
    
    m_upgradeTower->towerBase = TowerBg;
    m_upgradeTower->shadow = pShadow;
    m_allBuildObb[index].towerInfo->shadow = pShadow;
    m_allBuildObb[index].towerInfo->towerBase = TowerBg;
    
}

void pvpMainLayer::updateVideoState(int videoId)
{
    CDataManager::saveRewardInfo(videoId);
    for(int i = 0; i < (int)m_allVideoInfo.size(); i++)
    {
        if(videoId == m_allVideoInfo[i].videoId)
        {
            m_allVideoInfo[i].iGetWard = 1;
            break;
        }
    }
}

void pvpMainLayer::addTowerOBBInfo()
{
    m_bFirst = false;
    m_curBuild = m_newTowerObbInfo->towerInfo;
    addBuildObb(m_curBuild);
    for(int i = 0; i < (int)(m_curBuild->pInfo->blockPos.size()); i++)
    {
        float xPos = m_curBuild->theTower->getPositionX() + m_curBuild->pInfo->blockPos[i].x * LENGTH_BLOCK_PER;
        float zPos = m_curBuild->theTower->getPositionZ() - m_curBuild->pInfo->blockPos[i].y * LENGTH_BLOCK_PER;
        int index = (xPos - MAP_START_XPOS)/LENGTH_BLOCK_PER;
        int indey = (zPos - STARTZPOINT)/LENGTH_BLOCK_PER + 9;
        m_allBlock[index][indey].type = 1;
        m_allBlock[index][indey].sprite->setVisible(false);
    }
    for(int i = 0; i < m_selectedBlock.size(); i++)
    {
        m_selectedBlock[i]->removeFromParent();
        m_selectedBlock[i] = NULL;
    }
    m_selectedBlock.clear();
    m_bNewTower = false;
    log("就放这里了");//建造
    if(!((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_PVP_BUILDING) && !(CDataManager::getEndedBy(GUIDE_PVP_BUILDING))))
    {
        CNetWorkDataSingle::GetSingletonPtr()->SaveUserMCF(this, menu_selector(pvpMainLayer::updateUserData), -m_cost,0,0);
    }
    
    detectIfCanCreate();
}

void pvpMainLayer::showBuildList()
{
    //white_bg.png
    if(m_pLayer)
    {
        m_pLayer->removeFromParent();
        m_pLayer = NULL;
    }
    
    m_pLayer = LayerColor::create(Color4B(0, 0, 0, 150), m_WinSize.width, m_WinSize.height);
    addChild(m_pLayer, 100);
    m_pLayer->setPosition(Vec2::ZERO);
    
    Sprite* m_buildBg = Sprite::createWithSpriteFrameName("pvp_buildList_bg.png");
    m_pLayer->addChild(m_buildBg);
    m_buildBg->setScaleX(2.6f);
    m_buildBg->setScaleY(2.0f);
    m_buildBg->setPosition(Vec2(m_WinSize.width * 0.5f, -m_buildBg->getBoundingBox().size.height * 0.5f));
    Size bgSize = m_buildBg->getBoundingBox().size;
    
    m_buildLayer = LayerColor::create(Color4B(100, 0, 0, 0), bgSize.width, bgSize.height);
    m_pLayer->addChild(m_buildLayer);
    m_buildLayer->setPosition(Vec2(m_buildBg->getPositionX() - bgSize.width * 0.5f, m_buildBg->getPositionY() - bgSize.height * 0.5f));
    
    
    //建筑类型按钮
    Sprite* normal = Sprite::createWithSpriteFrameName("pvp_normal_btn.png");
    Sprite* selecet = Sprite::createWithSpriteFrameName("pvp_normal_btn.png");
    selecet->setColor(Color3B::GRAY);
    m_pDefenseItem = LYMenuItemSprite::create(normal, selecet, CC_CALLBACK_1(pvpMainLayer::switchBuildType, this));
    m_pDefenseItem->setTag(2);
    Menu* pDenfenseMenu = Menu::create(m_pDefenseItem, NULL);
    m_buildLayer->addChild(pDenfenseMenu);
    pDenfenseMenu->setPosition(Vec2(bgSize.width * 0.18f, normal->getContentSize().height * 0.5f));
    
    
    Label* pDefenseLabel = Label::createWithTTF("防御类建筑", "fonts/cuti.ttf", 32);
    pDefenseLabel->setPosition(Vec2(normal->getContentSize().width * 0.5f, normal->getContentSize().height * 0.5f - 5));
    m_pDefenseItem->addChild(pDefenseLabel, 1000);
    pDefenseLabel->enableOutline(Color4B(0, 0, 0, 255), 2);
    pDefenseLabel->runAction(MoveBy::create(0.1, Vec2(0, 1)));
    m_pDefenseItem->setEnabled(false);
    
    normal = Sprite::createWithSpriteFrameName("pvp_normal_btn.png");
    selecet = Sprite::createWithSpriteFrameName("pvp_normal_btn.png");
    selecet->setColor(Color3B::GRAY);
    m_pIncomeItem = LYMenuItemSprite::create(normal, selecet, CC_CALLBACK_1(pvpMainLayer::switchBuildType, this));
    m_pIncomeItem->setTag(1);
    m_pIncomeItem->setEnabled(false);
    Menu* pIncomeMenu = Menu::create(m_pIncomeItem, NULL);
    m_buildLayer->addChild(pIncomeMenu);
    pIncomeMenu->setPosition(Vec2(bgSize.width * 0.52f, normal->getContentSize().height * 0.5f));
    
    Label* pIncomeLabel = Label::createWithTTF("经济类建筑", "fonts/cuti.ttf", 32);
    m_pIncomeItem->addChild(pIncomeLabel, 1000);
    pIncomeLabel->enableOutline(Color4B(0, 0, 0, 255), 2);
    pIncomeLabel->setPosition(Vec2(normal->getContentSize().width * 0.5f, normal->getContentSize().height * 0.5f - 5));
    pIncomeLabel->runAction(MoveBy::create(0.1, Vec2(0, 1)));
    if(m_iBuildUseType == 1)
    {
        Sprite* sp = Sprite::createWithSpriteFrameName("bpb_selected_btn.png");
        m_pIncomeItem->setNormalImage(sp);
        m_pDefenseItem->setEnabled(true);
    }
    else
    {
        Sprite* sp = Sprite::createWithSpriteFrameName("bpb_selected_btn.png");
        m_pDefenseItem->setNormalImage(sp);
        m_pIncomeItem->setEnabled(true);
    }
    
    m_buildBg->runAction(MoveBy::create(0.1f, Vec2(0, m_buildBg->getBoundingBox().size.height)));
    m_buildLayer->runAction(MoveBy::create(0.1f, Vec2(0, m_buildBg->getBoundingBox().size.height)));
    CallFunc* listInfoAct = CallFunc::create(CC_CALLBACK_0(pvpMainLayer::showListInfo, this));
    m_buildLayer->runAction(listInfoAct);
    addBuildUseTypeNum();
}

void pvpMainLayer::addBuildUseTypeNum()
{
    int tipNum = getNumCreate(PVP_DEFENSE);
    if(tipNum > 0)
    {
        Sprite* denfenseTip = Sprite::createWithSpriteFrameName("pvpTip.png");
        m_pDefenseItem->addChild(denfenseTip, 1, 100);
        denfenseTip->setPosition(Vec2(m_pDefenseItem->getNormalImage()->getContentSize().width * 0.83f,
                                      m_pDefenseItem->getNormalImage()->getContentSize().height * 0.7f));
        
        __String* pBuf = __String::createWithFormat("%d", tipNum);
        Label* pLabelDenfenseNum = Label::createWithTTF(pBuf->getCString(), "fonts/cuti.ttf", 20);
        denfenseTip->addChild(pLabelDenfenseNum);
        pLabelDenfenseNum->enableOutline(Color4B(0, 0, 0, 255), 1);
        pLabelDenfenseNum->setPosition(Vec2(denfenseTip->getContentSize().width * 0.5f, denfenseTip->getContentSize().height * 0.5f));
        pLabelDenfenseNum->runAction(MoveBy::create(0.1, Vec2(0, 1)));
    }
    tipNum = getNumCreate(PVP_INCOMME);
    if(tipNum > 0)
    {
        Sprite* incommeTip = Sprite::createWithSpriteFrameName("pvpTip.png");
        m_pIncomeItem->addChild(incommeTip, 1, 100);
        incommeTip->setPosition(Vec2(m_pIncomeItem->getNormalImage()->getContentSize().width * 0.83f,
                                     m_pIncomeItem->getNormalImage()->getContentSize().height * 0.7f));
        
        __String* pBuf = __String::createWithFormat("%d", tipNum);
        Label* pLabelIncommeNum = Label::createWithTTF(pBuf->getCString(), "fonts/cuti.ttf", 20);
        incommeTip->addChild(pLabelIncommeNum);
        pLabelIncommeNum->enableOutline(Color4B(0, 0, 0, 255), 1);
        pLabelIncommeNum->setPosition(Vec2(incommeTip->getContentSize().width * 0.5f, incommeTip->getContentSize().height * 0.5f));
        pLabelIncommeNum->runAction(MoveBy::create(0.1, Vec2(0, 1)));
    }
}

void pvpMainLayer::showListInfo()
{
    if(m_scrollView)
    {
        m_scrollView->removeFromParent();
        m_scrollView = NULL;
    }
    m_allBuildType.clear();
    Size bgSize = m_buildLayer->getBoundingBox().size;
    //
    std::vector<pvpBuildInfo*> m_sortBuildVec;//存储所有塔
    std::vector<pvpBuildInfo*> m_grayBuildVec;//存储灰色显示的塔信息
    std::vector<pvpBuildInfo*> m_normalBuildVec;//存储正常显示的塔信息
    
    int allNum = CDataManager::getBuildTypeNum();
    for (int i = 0; i < allNum; i++)
    {
        if(i == 8)
            continue;
        int buildId = CDataManager::getBuildIdByType(i + 1);
        pvpBuildInfo* pInfo = CDataManager::getBuildInfo(buildId);
        if(pInfo->useType == m_iBuildUseType)
        {
            bool ret = judgeBuildNumLimit(i + 1);
            if(!ret)
            {
                m_grayBuildVec.push_back(pInfo);
            }
            else
            {
                m_normalBuildVec.push_back(pInfo);
            }
        }
    }
    m_sortBuildVec = m_normalBuildVec;
    for(int k = 0 ; k < (int)m_grayBuildVec.size(); k++)
    {
        m_sortBuildVec.push_back(m_grayBuildVec[k]);
    }
    
    int num = (int)m_sortBuildVec.size();
    Size layerSize = Size(num * BUILD_WIDTH_PER, BUILD_HEIGHT_PER);
    auto scroll_layer = LayerColor::create(Color4B(0,255,0,0), layerSize.width, layerSize.height);
    Menu* m_stoneMenu = Menu::create();
    scroll_layer->addChild(m_stoneMenu);
    m_stoneMenu->setPosition(Vec2::ZERO);
    m_scrollView = ScrollView::create(cocos2d::Size(bgSize.width, BUILD_HEIGHT_PER), scroll_layer);
    m_pLayer->addChild(m_scrollView);
    m_scrollView->setBounceable(true);
    m_scrollView->setPosition(Vec2(0, bgSize.height * 0.3f));
    m_scrollView->setDirection(ScrollView::Direction::HORIZONTAL);
    if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_PVP_BUILDING) && !(CDataManager::getEndedBy(GUIDE_PVP_BUILDING)))
    {  //by ly 防止拖动时引导手指位置不正确
        m_scrollView->setTouchEnabled(false);
    }
    
    //先设置为9个
    for(int i = 0; i < (int)m_sortBuildVec.size(); i++)
    {
        Vec2 pos = Vec2(BUILD_WIDTH_PER * 0.5f + BUILD_WIDTH_PER * i, layerSize.height * 0.5f);
        bool valid= true;
        if(i >= m_normalBuildVec.size())
        {
            valid = false;
        }
        addBuildInfo(scroll_layer, pos, m_sortBuildVec[i]->buildingType, i, valid);
    }
}

void pvpMainLayer::addBuildInfo(Node* parent, Vec2 pos, int tag, int index, bool bValid)
{
    char buf[256];
    Sprite* normal = Sprite::createWithSpriteFrameName("pvp_build_grayBg.png");
    if(bValid)
    {
        normal = Sprite::createWithSpriteFrameName("pvp_build_bg.png");
    }
    normal->setUserData((int*)tag);
    normal->setCascadeOpacityEnabled(true);
    normal->setOpacity(0);
    parent->addChild(normal);
    normal->setPosition(pos);
    
    newBuildInfo *newInfo = new newBuildInfo();
    newInfo->newTower = normal;
    newInfo->valid = bValid;
    m_allBuildType.push_back(newInfo);
    Size bgSize = normal->getContentSize();
    //模型
    int buildId = CDataManager::getBuildIdByType(tag);
    pvpBuildInfo* pBuildInfo = CDataManager::getBuildInfo(buildId);
    if((pBuildInfo->buildingType == 2) && (guiderManagerLayer::getInstance()->getGuideType() == GUIDE_PVP_BUILDING)
       && (guiderManagerLayer::getInstance()->getStepNum() == 3))//熔炉
    {
        guiderManagerLayer::getInstance()->setStepNum(guiderManagerLayer::getInstance()->getStepNum() + 1);
        Vec2 pos = normal->convertToWorldSpaceAR(Vec2::ZERO);
        guiderManagerLayer::getInstance()->createLayer(this, GUIDE_SHOW_FINGER, pos);
    }
    if(pBuildInfo)
    {
        monsterInfo* pMonsterInfo = CDataManager::getMonsterInfoByMonsterId(pBuildInfo->monsterId);
        Sprite* tower = Sprite::createWithSpriteFrameName(pBuildInfo->icon.c_str());
        normal->addChild(tower);
        tower->setPosition(Vec2(bgSize.width * 0.5f, bgSize.height * 0.82f));
        
        if(pBuildInfo->buildingType == 12)//箭塔
        {
            tower->setPositionY(bgSize.height * 0.78f);
        }
        
        if(pBuildInfo->buildingType == 5)//投石车
        {
            tower->setPositionY(bgSize.height * 0.84f);
        }
        
        
        //名字
        Label* name = Label::createWithTTF(pMonsterInfo->monsterName.c_str(), "fonts/cuti.ttf", 32);
        normal->addChild(name);
        name->enableOutline(Color4B(0, 0, 0, 255), 2);
        name->setPosition(Vec2(bgSize.width * 0.53f, bgSize.width * 0.8f));
        
        //用途描述
        Label* useDesc = Label::createWithTTF(pBuildInfo->desc.c_str(), "fonts/cuti.ttf", 20);
        normal->addChild(useDesc);
        useDesc->setAnchorPoint(Vec2(0.5, 1.0f));
        useDesc->setColor(Color3B(40,40,40));
        useDesc->setPosition(Vec2(bgSize.width * 0.55f, bgSize.width * 0.68f));
        
        if(!bValid)
        {
            tower->setColor(Color3B::GRAY);
            std::string temp = getTowerLimitInfo(buildId);
            
            Label* pTip = Label::createWithTTF(temp.c_str(), "fonts/cuti.ttf", 22);
            normal->addChild(pTip);
            pTip->setWidth(bgSize.width * 0.6f);
            pTip->setAnchorPoint(Vec2(0.5, 1.0f));
            pTip->setPosition(Vec2(bgSize.width * 0.55f, bgSize.width * 0.5f));
        }
        else {
            //消耗金币
            Sprite* coin = Sprite::createWithSpriteFrameName("coin.png");
            normal->addChild(coin);
            coin->setScale(0.85f);
            coin->setAnchorPoint(Vec2(0, 0.5f));
            coin->setPosition(Vec2(bgSize.width * 0.3f, bgSize.height * 0.3f));
            
            sprintf(buf, "%d", pBuildInfo->cost);
            Label* pLabelCoin = Label::createWithTTF(buf, "fonts/cuti.ttf", 30);
            normal->addChild(pLabelCoin);
            pLabelCoin->enableOutline(Color4B(0, 0, 0, 255), 2);
            pLabelCoin->setAnchorPoint(Vec2(0, 0.5f));
            pLabelCoin->setPosition(Vec2(coin->getPositionX() + coin->getBoundingBox().size.width + 10, bgSize.height * 0.3f));
            
            //建造时间
            Label* pBuildTime = Label::createWithTTF("建造时间:", "fonts/cuti.ttf", 20);
            normal->addChild(pBuildTime);
            pBuildTime->setAnchorPoint(Vec2(0, 0.5f));
            pBuildTime->setColor(Color3B::BLACK);
            pBuildTime->setPosition(Vec2(bgSize.width * 0.19f, bgSize.height * 0.18f));
            
            std::string time = calculateTime(pBuildInfo->costTime);
            Label* pTime = Label::createWithTTF(time.c_str(), "fonts/cuti.ttf", 20);
            normal->addChild(pTime);
            pTime->enableOutline(Color4B(0, 0, 0, 255), 1);
            pTime->setAnchorPoint(Vec2(0, 0.5f));
            pTime->setPosition(Vec2(bgSize.width * 0.2f, bgSize.height * 0.115f));
            
            //已建造数量
            Label* pHaveLabel = Label::createWithTTF("已建造:", "fonts/cuti.ttf", 20);
            normal->addChild(pHaveLabel);
            pHaveLabel->setAnchorPoint(Vec2(0, 0.5f));
            pHaveLabel->setColor(Color3B::BLACK);
            pHaveLabel->setPosition(Vec2(bgSize.width * 0.7f, bgSize.height * 0.18f));
            
            int iHave = 0;//已有数量
            int limitNum = 0;//限制数量
            for(int i = 0 ; i < (int)m_allBuildObb.size(); i++)
            {
                pvpBuildInfo* pInfo = CDataManager::getBuildInfo(m_allBuildObb[i].buildingId);
                if(pBuildInfo && (pInfo->buildingType == tag))
                {
                    iHave++;
                }
            }
            int mainBuildId = CDataManager::getBuildIdByType(0);
            pvpBuildInfo* pMainBase = CDataManager::getBuildInfo(mainBuildId);
            for(int j = 0; j < (int)(pMainBase->allBuildLimitVec.size()); j++)
            {
                if(pMainBase->allBuildLimitVec[j].buitldType == tag)
                {
                    limitNum = pMainBase->allBuildLimitVec[j].num;
                    break;
                }
            }
            
            sprintf(buf, "%d/%d", iHave, limitNum);
            Label* pBuildNum = Label::createWithTTF(buf, "fonts/cuti.ttf", 20);
            normal->addChild(pBuildNum);
            pBuildNum->enableOutline(Color4B(0, 0, 0, 255), 1);
            pBuildNum->setAnchorPoint(Vec2(0, 0.5f));
            pBuildNum->setPosition(Vec2(bgSize.width * 0.72f, bgSize.height * 0.115f));
        }
    }
    normal->runAction(Sequence::create(DelayTime::create(0.1*index), FadeTo::create(0.25f, 255), NULL));
    normal->runAction(Sequence::create(DelayTime::create(0.1*index), ScaleTo::create(0.25, 1.1f),
                                       ScaleTo::create(0.25, 1.0f), NULL));
    
}

std::string pvpMainLayer::calculateTime(int allTime)
{
    char buf[256];
    std::string time;
    if(allTime < 60)
    {
        sprintf(buf, "%d秒", allTime);
        time += buf;
    }
    else
    {
        int second = allTime%60;
        int minute = allTime/60;
        int hour = 0;
        int days = 0;
        if(minute > 60)
        {
            hour = minute/60;
            minute = minute%60;
            if(hour > 24)
            {
                days = hour/24;
                hour = hour%24;
                if(days > 0)
                {
                    sprintf(buf, "%d天", hour);
                    time += buf;
                }
            }
            if(hour > 0)
            {
                sprintf(buf, "%d时", hour);
                time += buf;
            }
        }
        if(minute > 0)
        {
            sprintf(buf, "%d分", minute);
            time += buf;
        }
        if(second > 0)
        {
            sprintf(buf, "%d秒", second);
            time += buf;
        }
    }
    return time;
}


void pvpMainLayer::switchBuildType(Ref* pSender)
{
    int tag = ((Node*)pSender)->getTag();
    Sprite* spEnable = Sprite::createWithSpriteFrameName("pvp_normal_btn.png");
    Sprite* spDisable = Sprite::createWithSpriteFrameName("bpb_selected_btn.png");
    if(tag == 1)
    {
        m_pDefenseItem->setNormalImage(spEnable);
        m_pDefenseItem->setEnabled(true);
        m_pIncomeItem->setNormalImage(spDisable);
        m_pIncomeItem->setEnabled(false);
        
    }
    else
    {
        m_pIncomeItem->setNormalImage(spEnable);
        m_pIncomeItem->setEnabled(true);
        m_pDefenseItem->setNormalImage(spDisable);
        m_pDefenseItem->setEnabled(false);
    }
    m_iBuildUseType = tag;
    showListInfo();
}

void pvpMainLayer::setBtnEnable(bool valid)
{
    m_msgItem->setEnabled(valid);
    m_buildItem->setEnabled(valid);
    m_pFightItem->setEnabled(valid);
    m_backItem->setEnabled(valid);
    m_pCoinItem->setEnabled(valid);
    m_pStamainItem->setEnabled(valid);
    m_pStoneItem->setEnabled(valid);
    
}

void pvpMainLayer::saveRongluState(int userBuildingId, bool bProductEnd)
{
    for(int i = 0; i < (int)m_allBuildObb.size(); i++)
    {
        if(m_allBuildObb[i].userBuildingId == userBuildingId)
        {
            if(!bProductEnd)
            {
                m_allBuildObb[i].state = 3;
                m_productBuildId = userBuildingId;
            }
            else
            {
                m_allBuildObb[i].state = 0;
            }
            break;
        }
    }
    sendRequestSaveTeamInfo();
}

void pvpMainLayer::addProductInfo()
{
    pvpBuildInfo* pBuildingInfo = CDataManager::getBuildInfo(m_productTower->pInfo->buildingID);
    Size bgSize = m_productTower->theTower->gettheSprite()->getBoundingBox().size;
    m_productProgressBg = (BillBoard*)BillBoard::createWithSpriteFrameName("pvpProgressBg.png");
    m_productProgressBg->setMode(BillBoard::Mode::VIEW_PLANE_ORIENTED);
    m_productTower->theTower->addChild(m_productProgressBg, 100);
    
    m_productProgressBg->setPosition3D(Vec3(0, bgSize.height + 50, 0));
    m_productProgressBg->setCameraMask((unsigned short )CameraFlag::USER1);
    
    //升级图标
    Sprite* pIcon = Sprite::createWithSpriteFrameName("pvp_ore.png");
    m_productProgressBg->addChild(pIcon);
    pIcon->setScale(0.6f);
    pIcon->setPosition(Vec2(m_productProgressBg->getBoundingBox().size.width * 0.14f, m_productProgressBg->getBoundingBox().size.height * 0.495f));
    pIcon->setCameraMask((unsigned short )CameraFlag::USER1);
    
    Sprite* sp = Sprite::createWithSpriteFrameName("pvpProgress.png");
    m_productProgress = ProgressTimer::create(sp);
    m_productProgressBg->addChild(m_productProgress);
    m_productProgress->setAnchorPoint(Vec2(0, 0.5f));
    m_productProgress->setPosition(Vec2(m_productProgressBg->getBoundingBox().size.width * 0.25f,
                                        m_productProgressBg->getBoundingBox().size.height * 0.495f));
    
    m_productProgress->setType(ProgressTimer::Type::BAR);
    m_productProgress->setMidpoint(Point(0,0.5));
    m_productProgress->setBarChangeRate(Vec2(1, 0));
    
    m_productRemainTime--;
    float percent = (float)(m_productNeedTime - m_productRemainTime)/m_productNeedTime*100;
    m_productProgress->setPercentage(percent);
    m_productProgress->setCameraMask((unsigned short )CameraFlag::USER1);
    
    std::string str = calculateTime(m_productRemainTime);
    m_productTimeLabel = Label::createWithTTF(str.c_str(), "fonts/cuti.ttf", 18);
    m_productProgressBg->addChild(m_productTimeLabel,10);
    m_productTimeLabel->setScale(0.78f);
    m_productTimeLabel->enableOutline(Color4B(0, 0, 0, 255), 2);
    m_productTimeLabel->setPosition(Vec2(m_productProgressBg->getBoundingBox().size.width * 0.53f,
                                         m_productProgressBg->getBoundingBox().size.height * 0.495f));
    m_productTimeLabel->setCameraMask((unsigned short )CameraFlag::USER1);
}

void pvpMainLayer::addUpgradeInfo(bool bNewTower)
{
    rmvUpgradeInfo();
    if(m_curBuild && m_upgradeTower)
    {
        pvpBuildInfo* pBuildingInfo = CDataManager::getBuildInfo(m_curBuild->pInfo->buildingID);
        
        m_curBuild->theTower->gettheSprite()->setGlobalZOrder(0);
        Size bgSize = m_upgradeTower->theTower->gettheSprite()->getBoundingBox().size;
        m_timeProgressBg = (BillBoard*)BillBoard::createWithSpriteFrameName("pvpProgressBg.png");
        m_timeProgressBg->setMode(BillBoard::Mode::VIEW_PLANE_ORIENTED);
        m_upgradeTower->theTower->addChild(m_timeProgressBg, 100);
        
        m_timeProgressBg->setPosition3D(Vec3(0, bgSize.height, 0));
        m_timeProgressBg->setCameraMask((unsigned short )CameraFlag::USER1);
        
        //升级图标
        Sprite* pIcon = Sprite::createWithSpriteFrameName("pvp_upgradeIcon.png");
        m_timeProgressBg->addChild(pIcon);
        pIcon->setScale(0.9f);
        pIcon->setPosition(Vec2(m_timeProgressBg->getBoundingBox().size.width * 0.14f, m_timeProgressBg->getBoundingBox().size.height * 0.495f));
        pIcon->setCameraMask((unsigned short )CameraFlag::USER1);
        
        Sprite* sp = Sprite::createWithSpriteFrameName("pvpProgress.png");
        m_timeProgress = ProgressTimer::create(sp);
        m_timeProgressBg->addChild(m_timeProgress);
        m_timeProgress->setAnchorPoint(Vec2(0, 0.5f));
        m_timeProgress->setPosition(Vec2(m_timeProgressBg->getBoundingBox().size.width * 0.25f,
                                         m_timeProgressBg->getBoundingBox().size.height * 0.495f));
        
        m_timeProgress->setType(ProgressTimer::Type::BAR);
        m_timeProgress->setMidpoint(Point(0,0.5));
        m_timeProgress->setBarChangeRate(Vec2(1, 0));
        
        if(bNewTower)
        {
            pBuildingInfo = CDataManager::getBuildInfo(m_upgradeBuildId);
        }
        
        m_remainTime--;
        float percent = (1-(float)m_remainTime/pBuildingInfo->costTime)*100;
        m_timeProgress->setPercentage(percent);
        m_timeProgress->setCameraMask((unsigned short )CameraFlag::USER1);
        
        std::string str = calculateTime(m_remainTime);
        m_pTimeLabel = Label::createWithTTF(str.c_str(), "fonts/cuti.ttf", 18);
        m_timeProgressBg->addChild(m_pTimeLabel,10);
        m_pTimeLabel->setScale(0.78f);
        m_pTimeLabel->enableOutline(Color4B(0, 0, 0, 255), 2);
        m_pTimeLabel->setPosition(Vec2(m_timeProgressBg->getBoundingBox().size.width * 0.53f,
                                       m_timeProgressBg->getBoundingBox().size.height * 0.495f));
        m_pTimeLabel->setCameraMask((unsigned short )CameraFlag::USER1);
        
        //显示xxx开始升级
        if(m_remainTime + 1 == pBuildingInfo->costTime)
        {
            monsterInfo* pMonster = CDataManager::getMonsterInfoByMonsterId(m_upgradeTower->pInfo->monsterId);
            __String *temp = __String::createWithFormat("%s开始升级", pMonster->monsterName.c_str());
            Sprite * thetarget = CFightManager::getTheFlyWord(temp->getCString(),"fonts/cuti.ttf");
            thetarget->setPosition(m_winSize.width*0.5,m_winSize.height*0.5);
            addChild(thetarget,100);
        }
        //显示升级的动画
        m_hammer = Sprite::createWithSpriteFrameName("pvp_hammer_1.png");
        m_upgradeTower->theTower->addChild(m_hammer, 100);
        m_hammer->setPosition3D(Vec3(bgSize.width * 0.55f, bgSize.height * 0.7f, 0));
        m_hammer->setCameraMask((unsigned short )CameraFlag::USER1);
        Animation* pAction = AnimationCache::getInstance()->getAnimation("hammer");
        m_hammer->runAction(RepeatForever::create(Animate::create(pAction)));
        
    }
    m_curBuild = NULL;//显示升级属性时先将当前表示当前选中建筑的指针置空
}



void pvpMainLayer::startUpgradeTower(int buildingId)
{
    pvpBuildInfo* pBuildingInfo = CDataManager::getBuildInfo(buildingId);//选择要升级的建筑信息
    m_curBuild->pInfo = pBuildingInfo;
    if(m_curBuild->towerBase)
    {
        m_curBuild->towerBase->setVisible(true);
    }
    if(m_curBuild->shadow)
    {
        m_curBuild->shadow->setVisible(true);
    }
    if(m_layer)
    {
        //CPersonalData::getInstance()->setTouchRmvFlag(true);
        m_layer->removeFromParent();
        m_layer = NULL;
    }
    
    //判断是否已经有正在升级的建筑
    if(m_remainTime > 0)
    {
        char buf[1024];
        pvpBuildInfo* pInfo = CDataManager::getBuildInfo(m_upgradeBuildId);
        monsterInfo* pMonsterInfo = CDataManager::getMonsterInfoByMonsterId(pInfo->monsterId);
        
        monsterInfo* pBuildingMonsterInfo = CDataManager::getMonsterInfoByMonsterId(pBuildingInfo->monsterId);
        m_neddStone = m_remainTime / 60;
        m_neddStone = MAX(1, m_neddStone);
        m_cost = pBuildingInfo->cost;;
        sprintf(buf, "您一次只能升级一个建筑。要完成%s然后开始升级%s吗？", pMonsterInfo->monsterName.c_str() , pBuildingMonsterInfo->monsterName.c_str());
        m_layer = pvpCommonInfoLayer::create(PVP_COMMSG_UPGRADE_TIP);
        addChild(m_layer, 100);
        m_layer->addCostStoneTip(this, menu_selector(pvpMainLayer::sureUpgrade), buf, m_neddStone);
    }
    else//直接显示升级进度
    {
        m_upgradeTower = m_curBuild;
        m_upgradeBuildId = buildingId;
        m_remainTime = pBuildingInfo->costTime;
        if(m_remainTime == 0)
        {
            createDirectly();
            //添加底座和影子
            addStartUpgradeInfo(0);
            sendRequestSaveTeamInfo();
            BuildingFininsh(m_upgradeTower);
            
        }
        else{
            addUpgradeInfo();
            //保存玩家阵营信息
            for(int i = 0; i < (int)m_allBuildObb.size(); i++)
            {
                if(m_upgradeTower && (m_allBuildObb[i].towerInfo->theTower == m_upgradeTower->theTower))
                {
                    m_allBuildObb[i].buildingId = pBuildingInfo->buildingID;
                    m_allBuildObb[i].state = 2;
                }
            }
            sendRequestSaveTeamInfo(true);
        }
    }
}

void pvpMainLayer::updateKuangchangState(CBaseBuilding* tower)
{
    //更新矿场建筑的状态，停止或开始生产矿石
    bool existed = false;
    int index = 0;
    __String* temp = NULL;
    int addStone = 0;
    for(int i = 0 ; i < (int)m_pKuanchanBuildVec.size(); i++)
    {
        if(m_pKuanchanBuildVec[i].towerInfo->theTower == tower)
        {
            m_pKuanchanBuildVec[i].validTime = 0;
            temp = __String::createWithFormat("%d", m_pKuanchanBuildVec[i].userBuildingId);
            existed = true;
            index = i;
            break;
        }
    }
    if(!existed)
    {
        for(int j = 0; j < (int)m_allBuildObb.size(); j++)
        {
            if(m_allBuildObb[j].towerInfo->theTower == tower)
            {
                m_allBuildObb[j].validTime = 0;
                temp = __String::createWithFormat("%d", m_allBuildObb[j].userBuildingId);
                pvpBuildInfo* pInfo = CDataManager::getBuildInfo(m_allBuildObb[j].buildingId);
                addStone = pInfo->effectTime;
                m_pKuanchanBuildVec.push_back(m_allBuildObb[j]);
                break;
            }
        }
    }
    else
    {
        pvpBuildInfo* pInfo = CDataManager::getBuildInfo(m_pKuanchanBuildVec[index].buildingId);
        pvpBuildInfo* pNextInfo = CDataManager::getBuildInfo(pInfo->nextBuildingID);
        addStone = pNextInfo->effectTime;
        if(pInfo && (pInfo->buildingType == 10))
        {
            m_pKuanchanBuildVec[index].validTime = 0;
        }
        m_pKuanchanBuildVec[index].state = 0;
    }
    CNetControlModule::sharedNetControlModule()->updateKuangChangRate(this, NULL, temp->getCString(), addStone, addStone*24);
}

void pvpMainLayer::updateStoneRate()
{
    m_stoneRate = 0;
    for(int i = 0; i < (int)m_allBuildObb.size(); i++)
    {
        pvpBuildInfo* pInfo = CDataManager::getBuildInfo(m_allBuildObb[i].buildingId);
        if(pInfo->buildingType == 10) //矿场
        {
            m_stoneRate += pInfo->effectTime;
        }
    }
    
    
    
    //更新矿石的生产速率
    __String* temp = __String::createWithFormat("%d/h", m_stoneRate);
    m_addLabel->setString(temp->getCString());
    CPersonalData::getInstance()->setUpdateUserInfo(true);
    

    
}

void pvpMainLayer::updateKuangshiLimit()
{
    m_iAllStoneNum = 100;
    for(int i = 0; i < (int)m_allBuildObb.size(); i++)
    {
        pvpBuildInfo* pInfo = CDataManager::getBuildInfo(m_allBuildObb[i].buildingId);
        if(pInfo->buildingType == 1)
        {
            m_iAllStoneNum += pInfo->effectTime;
        }
    }
    
    
    char buf[50] = {0};
    sprintf(buf,":%d",m_iAllStoneNum);
    
    m_pStoneAllNum->setString(buf);
    
    
   
    
    CNetControlModule::sharedNetControlModule()->updateKuangshiLimit(this, NULL, m_iAllStoneNum);
    CPersonalData::getInstance()->setUserMaxKuangStone(m_iAllStoneNum);
    
    m_fEndPercent = CPersonalData::getInstance()->getUserKuangStone()*1.0/CPersonalData::getInstance()->getUserMaxKuangStone()*100;
    m_fStartPercent = m_progress->getPercentage();
    m_fAllPerTime = 0;
    m_bAddPer = true;
    
}

void pvpMainLayer::sureUpgrade(Ref* pSender)
{
    //先立即完成当前正在升级的塔
    if(CPersonalData::getInstance()->getUserStone() < m_neddStone)
    {
        Sprite * thetarget = CFightManager::getTheFlyWord("魔法石不足","fonts/cuti.ttf");
        thetarget->setPosition(m_winSize.width*0.5,m_winSize.height*0.5);
        addChild(thetarget,100);
        return;
    }
    
    if(m_layer)
    {
        m_layer->removeFromParent();
        CPersonalData::getInstance()->setTouchRmvFlag(true);
        m_layer = NULL;
    }
    
    
    BuildingFininsh(m_upgradeTower);
    
    
    m_bFirst = false;
    rmvBtnAndArrow();
    rmvSelectedBlock();
    rmvUpgradeInfo();
    CNetWorkDataSingle::GetSingletonPtr()->SaveUserMCF(this, menu_selector(pvpMainLayer::updateUserData), 0, -m_neddStone, 0);
    TDCCItem::onPurchase("pvp立即完成升级", 1, m_neddStone);
    m_upgradeTower->theTower->ChangeModelWithPVPBuildingId(m_upgradeBuildId);
    m_upgradeBuildId = 0;
    //然后开始升级当前选中的塔
    
    pvpBuildInfo* pBuildingInfo = CDataManager::getBuildInfo(m_upgradeTower->pInfo->buildingID);
    if(m_bNewTower)
    {
        pBuildingInfo = CDataManager::getBuildInfo(m_curBuildingId);
    }
    
    bool bNewTower = false;
    if(m_bNewTower)
    {
        m_bNewTower = false;
        bNewTower = true;
        addTowerOBBInfo();
    }
    //升级结束保存玩家建筑信息
    for(int i = 0; i < (int)m_allBuildObb.size(); i++)
    {
        if(m_upgradeTower && (m_allBuildObb[i].towerInfo->theTower == m_upgradeTower->theTower))
        {
            m_allBuildObb[i].state = 0;
            pvpBuildInfo* pInfo = CDataManager::getBuildInfo(m_allBuildObb[i].buildingId);
            if(pInfo->buildingType == 0)
            {
                detectIfCanCreate();
            }
            if(pInfo->buildingType == 1)//如果升级的是矿石库泽发送消息更新矿石上限
            {
                updateKuangshiLimit();
            }
            if(pInfo->buildingType == 10)
            {
                updateStoneRate();
                updateKuangchangState(m_upgradeTower->theTower);
            }
            if(pInfo->buildingType == 11)
            {
                addPattolInfo(m_upgradeTower);
            }
            break;
        }
    }

    for(int i = 0; i < (int)(m_curBuild->pInfo->blockPos.size()); i++)
    {
        float xPos = m_curBuild->theTower->getPositionX() + m_curBuild->pInfo->blockPos[i].x * LENGTH_BLOCK_PER;
        float zPos = m_curBuild->theTower->getPositionZ() - m_curBuild->pInfo->blockPos[i].y * LENGTH_BLOCK_PER;
        int index = (xPos - MAP_START_XPOS)/LENGTH_BLOCK_PER;
        int indey = (zPos - STARTZPOINT)/LENGTH_BLOCK_PER + 9;
        m_allBlock[index][indey].type = 1;
        m_allBlock[index][indey].sprite->setVisible(false);
    }
    
    m_upgradeTower = m_curBuild;
    m_remainTime = m_upgradeTower->pInfo->costTime;
    m_upgradeBuildId = m_upgradeTower->pInfo->buildingID;
    
    if(m_remainTime == 0)
    {
        createDirectly();
        //添加底座和影子
        addStartUpgradeInfo(0);
        sendRequestSaveTeamInfo();
    }
    else{
        addStartUpgradeInfo();
        addUpgradeInfo(bNewTower);
        sendRequestSaveTeamInfo(true);
    }
}

void pvpMainLayer::saveTheBootMap(cocos2d::Ref *pSender){
    
    CNetControlModule::sharedNetControlModule()->saveTheMap(NULL, NULL);
    
    
    
}


