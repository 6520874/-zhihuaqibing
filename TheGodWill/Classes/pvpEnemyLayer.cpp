//
//  pvpEnemyLayer.cpp
//  WarOfGod
//
//  Created by user on 15/6/11.
//
//

#include "pvpEnemyLayer.h"
#include "zzbNetControlModule.h"
#include "zzbNewNetStruct.h"
#include "DBMgr.h"
#include "LYBaseTower.h"
#include "LYFightData.h"
#include "CSharedPlist.h"
#include "PhysicalSystem.h"
#include "Common.h"
#include "DataManager.h"
#include "LYPersonalData.h"

pvpEnemyLayer::pvpEnemyLayer()
{
    m_listen = NULL;
    _camera = NULL;
    m_btnMenu = NULL;
    m_progress = NULL;
    m_allBuildObb.clear();
    m_stone = 0;
    m_pLayer = NULL;
    m_searchBg = NULL;
    layer3D = NULL;
    m_pBackItem = NULL;
    m_pFightItem = NULL;
    m_searchItem = NULL;
    m_pAllStone = NULL;
    
    CSharedPlist::getInstance()->pushPlist("plist/pvpMain.plist");
    CSharedPlist::getInstance()->pushPlist("plist/fight.plist");
}

pvpEnemyLayer::~pvpEnemyLayer()
{
    _eventDispatcher->removeEventListener(m_listen);
    CSharedPlist::getInstance()->erasePlist("plist/pvpMain.plist");
    CSharedPlist::getInstance()->erasePlist("plist/fight.plist");
}



bool pvpEnemyLayer::init()
{
    if(!CBaseLayer::init())
    {
        return false;
    }
    
    m_listen = EventListenerTouchAllAtOnce::create();
    m_listen->setEnabled(true);
    m_listen->onTouchesBegan = CC_CALLBACK_2(pvpEnemyLayer::onTouchesBegan, this);
    m_listen->onTouchesMoved = CC_CALLBACK_2(pvpEnemyLayer::onTouchesMoved, this);
    m_listen->onTouchesEnded = CC_CALLBACK_2(pvpEnemyLayer::onTouchesEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(m_listen, this);
    m_winSize = Director::getInstance()->getWinSize();
    
    //测试接口
    std::string fileName = "models/gameScene/theScene2.c3b";
    
    layer3D = Layer::create();
    addChild(layer3D);
    
    _camera = Camera::createPerspective(60, (GLfloat)m_winSize.width/m_winSize.height, 1, 2000);
    
    _camera->setCameraFlag(CameraFlag::USER1);
    Vec3  VCamera(END3DPOINT,CAMERA_POSITION_Y,CAMERA_POSITION_Z); //350
    
    _camera->setPosition3D(VCamera);
    _camera->lookAt(Vec3(END3DPOINT,CAMERA_LOOK_POSITION_Y,CAMERA_LOOK_POSITION_Z), Vec3(0,1,0) );
    
    layer3D->addChild(_camera);
    layer3D->setCameraMask((unsigned short )CameraFlag::USER1);
    
    m_btnMenu = Menu::create();
    addChild(m_btnMenu, 10);
    m_btnMenu->setPosition(Vec2::ZERO);
    
    //初始化背景
    Sprite3D* tempTarget = Sprite3D::create();
    Sprite* temp = Sprite::create("models/gameScene/fightBG_Forest1_1.png");
    temp->setAnchorPoint(Vec2(0.5f, 0));
    
    tempTarget->addChild(temp);
    
    CFightManager::addCamera(tempTarget);
    
    tempTarget->setRotation3D(Vec3(-60, 0, 0));
    tempTarget->setPosition3D(Vec3(temp->getBoundingBox().size.width*0.70,-1,-800));
    
    layer3D->addChild(tempTarget);
    
    tempTarget->setForceDepthWrite(true);
    
    Sprite3D* tempTarget2 = Sprite3D::create();
    Sprite* temp2 = Sprite::create("models/gameScene/fightBG_Forest1_2.png");
    temp2->setAnchorPoint(Vec2(0.5f, 0));
    
    tempTarget2->addChild(temp2);
    
    CFightManager::addCamera(tempTarget2);
    
    tempTarget2->setRotation3D(Vec3(90, 0, 0));  //依然是那个坑，如果旋转角度为-90度，就会出现渲染错误
    tempTarget2->setPosition3D(Vec3(tempTarget->getPositionX(), tempTarget->getPositionY(), tempTarget->getPosition3D().z));
    layer3D->addChild(tempTarget2);
    
    addCloud();
    
    if(CPersonalData::getInstance()->getEnemyType() == SPECIFIED_ENEMY)
    {
        //获取指定玩家的阵营信息
        int enemyUserId = CPersonalData::getInstance()->getEnemyUserId();
        CNetControlModule::sharedNetControlModule()->getEnemyTeamInfo(this, menu_selector(pvpEnemyLayer::getEnemyTeamInfoBack), enemyUserId);
    }
    else{
        //显示搜索图标
        CallFunc* searchAct = CallFunc::create(CC_CALLBACK_0(pvpEnemyLayer::startSearchEnemy, this));
        runAction(Sequence::create(DelayTime::create(0.6f), searchAct, NULL));
    }
    return true;
}


void pvpEnemyLayer::addCloud()
{
    CSharedPlist::getInstance()->pushPlist("plist/mainMenu.plist");
    m_pLayer = LayerColor::create(Color4B(0, 0, 0, 220));
    addChild(m_pLayer, 100);
    m_pLayer->setPosition(Vec2::ZERO);
    
    for(int i = 0; i < 5; i++)
    {
        for(int j = 0; j < 2; j++)
        {
            Sprite* sp = Sprite::createWithSpriteFrameName("cloud_1.png");
            m_pLayer->addChild(sp, 100);
            sp->setScaleX(5.0f);
            sp->setScaleY(4.5f);
            sp->setOpacity(50);
            float xPos = m_winSize.width * -0.2f;
            if(j == 1)
            {
                xPos = m_winSize.width * 0.05f;
            }
            Vec2 pos = Vec2(xPos, m_winSize.height * (0.9f - i * 0.2f));
            log("坐标x=%f,y=%f", pos.x, pos.y);
            sp->setPosition(pos);
            Spawn* pAction = Spawn::create(MoveBy::create(0.5f, Vec2(m_winSize.width * 0.3f, 0)), FadeTo::create(0.5, 255), NULL);
            sp->runAction(EaseSineOut::create(pAction));
        }
    }
    for(int i = 0; i < 5; i++)
    {
        for(int j = 0; j < 2; j++)
        {
            Sprite* sp = Sprite::createWithSpriteFrameName("cloud_1.png");
            m_pLayer->addChild(sp, 100);
            sp->setOpacity(50);
            sp->setFlippedX(true);
            sp->setScaleX(5.0f);
            sp->setScaleY(4.5f);
            float xPos = m_winSize.width * 1.2f;
            if(j == 1)
            {
                xPos = m_winSize.width * 0.95f;
            }
            Vec2 pos = Vec2(xPos, m_winSize.height * (0.9f - i * 0.2f));
            log("坐标x=%f,y=%f", pos.x, pos.y);
            sp->setPosition(pos);
            Spawn* pAction = Spawn::create(MoveBy::create(0.5f, Vec2(-m_winSize.width * 0.3f, 0)), FadeTo::create(0.5, 255), NULL);
            sp->runAction(EaseSineOut::create(pAction));
        }
    }
}

void pvpEnemyLayer::startSearchEnemy()
{
    m_searchBg = Sprite::createWithSpriteFrameName("pvpSearch.png");
    addChild(m_searchBg, 100);
    m_searchBg->setPosition(Vec2(m_winSize.width * 0.5f, m_winSize.height * 0.5f));
    Size bgSize = m_searchBg->getBoundingBox().size;
    
    Label* pLabel = Label::createWithTTF("搜索", "fonts/cuti.ttf", 30);
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
    CallFunc* searchAct = CallFunc::create(CC_CALLBACK_0(pvpEnemyLayer::getEnemyTeamInfo, this));
    runAction(Sequence::create(DelayTime::create(1.0f), searchAct, NULL));
    
}

void pvpEnemyLayer::getEnemyTeamInfo()
{
    //先找出玩家主基地的等级，然后搜索实力相当的玩家
    //为了测试，这里先传玩家id
    int level = CPersonalData::getInstance()->getUserInfo()->m_iUserId;
    CNetControlModule::sharedNetControlModule()->getRandomEnemyTeamInfo(this, menu_selector(pvpEnemyLayer::getEnemyTeamInfoBack), level);
}

void pvpEnemyLayer::getEnemyTeamInfoBack(Ref* pSender)
{
    if(m_pLayer)
    {
        m_pLayer->removeFromParent();
        m_pLayer = NULL;
    }
    if(m_searchBg)
    {
        m_searchBg->removeFromParent();
        m_searchBg = NULL;
    }
    
    _netRecvEventGetUserTeamInfo* pInfo = (_netRecvEventGetUserTeamInfo*)pSender;
    int state = 1;
    for(std::vector<std::string>::iterator it = pInfo->m_paramsStr.begin(); it != pInfo->m_paramsStr.end(); it++)
    {
        string str = *it;
        string name = CDataManager::getStrFromBackStr(str.c_str());
        
        if(name == "info")
        {
            state = atoi(CDataManager::getDigitFromBackStr(it->c_str()).c_str());
            if(state != 0)
            {
                log("获取玩家阵营信息失败");
                CLayerManager::GetSingletonPtr()->createLayer(eLayer_PVPLayer, NULL,false);
                return;
            }
        }
        else if(name == "userId")
        {
            m_userId = atoi(CDataManager::getDigitFromBackStr(it->c_str()).c_str());
        }
        else if(name == "name")
        {
            m_name = CDataManager::getDigitFromBackStr(it->c_str());
        }
        else if(name == "level")
        {
            m_level = atoi(CDataManager::getDigitFromBackStr(it->c_str()).c_str());
        }
        else if (name == "defenseMap")
        {
            m_enemyMaps = CDataManager::getDigitFromBackStr(it->c_str());
            std::vector<__String> parameters = CDBMgr::getInstance()->separateInfoWithMark(m_enemyMaps, ";");
            for (int i = 0;i < parameters.size(); i++)
            {
                std::vector<__String> tempParam = CDBMgr::getInstance()->separateInfoWithMark(parameters[i], ",");
                towerOBBInfo spInfo;
                spInfo.buildingId = tempParam[0].intValue();
                spInfo.xPos = tempParam[1].intValue();
                spInfo.zPos = tempParam[2].intValue();
                spInfo.state = tempParam[3].intValue();
                spInfo.startUpTime = 0;//tempParam[4].intValue();
                spInfo.needTime = 0;//tempParam[5].intValue();
                pvpBuildInfo* pBuildInfo = CDataManager::getBuildInfo(spInfo.buildingId);
                FighterInfo theInfo;
                CDataManager::CalcFighterInfoWith(&theInfo, pBuildInfo->monsterId, pBuildInfo->level, 1, 0, false, 1, 0, 0, 0);
                CBaseBuilding* tower = CBaseTower::create(&theInfo,eFightGroup_Freind);
                tower->ChangeModelWithPVPBuildingId(pBuildInfo->buildingID);
                tower->m_bIfActive = true;
                CFightData::getInstance()->addMonster(tower,eAttackType_tower);
                
                //如果建筑类型是陷阱，则不显示
                if (tower->getTheInfo()->monsterType==eFighterType_Trap) {
                    tower->setVisible(false);
                }
                
                
                
                Vec3 p = Vec3(spInfo.xPos, 0, spInfo.zPos) + tower->gettheSprite()->getPosition3D();
                float height = tower->gettheSprite()->getBoundingBox().size.height;
                p.y = p.y+height*0.5f;
                Vec3 extents = Vec3(height*0.35f, height*0.35f, height*0.35f);
                AABB aabb(-extents, extents);
                auto obb = OBB(aabb);
                obb._center = p;
                spInfo.theTower = tower;
                spInfo.obb = obb;
                m_allBuildObb.push_back(spInfo);
            }
        }
        else if(name == "originStone")
        {
            m_stone = atoi(CDataManager::getDigitFromBackStr(it->c_str()).c_str());
        }
    }
    initUI();
}

void pvpEnemyLayer::initUI()
{
    for(int i = 0; i < m_allBuildObb.size(); i++)
    {
        m_curBuild = m_allBuildObb[i].theTower;
        Vec3 pos = Vec3(m_allBuildObb[i].xPos, 0, m_allBuildObb[i].zPos);
        layer3D->addChild(m_curBuild);
        m_curBuild->setPosition3D(pos);
        m_curBuild->setCameraMask((unsigned short )CameraFlag::USER1);
    }
    m_curBuild = NULL;
    
    adduserInfo();
}

void pvpEnemyLayer::adduserInfo()
{
    char buf[256];
    Sprite* normal = Sprite::createWithSpriteFrameName("pvpBack.png");
    Sprite* select = Sprite::createWithSpriteFrameName("pvpBack.png");
    select->setColor(Color3B::GRAY);
    m_pBackItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(pvpEnemyLayer::btnCallBack, this));
    Menu* backMenu = Menu::create(m_pBackItem,NULL);
    m_pBackItem->setTag(PVP_BACK_TAG);
    addChild(backMenu, 100);
    backMenu->setPosition(Vec2(m_winSize.width * 0.04f, m_winSize.height * 0.94f));
    
    //能量框
    Sprite* enemyBg = Sprite::createWithSpriteFrameName("pvpStoneProgressBg.png");
    addChild(enemyBg, 100);
    enemyBg->setPosition(Vec2(m_winSize.width * 0.9f, m_winSize.height * 0.96));
    Sprite* sp = Sprite::createWithSpriteFrameName("pvpStoneProgress.png");
    m_progress = ProgressTimer::create(sp);
    addChild(m_progress, 100);
    m_progress->setType(ProgressTimer::Type::BAR);
    m_progress->setMidpoint(Point(0,0.5));
    m_progress->setBarChangeRate(Vec2(1, 0));
    m_progress->setPosition(enemyBg->getPosition());
    
    Label* pcurStoneNum = Label::createWithSystemFont("", "Arial", 26);
    enemyBg->addChild(pcurStoneNum, 100);
    pcurStoneNum->setPosition(Vec2(enemyBg->getBoundingBox().size.width * 0.5f, enemyBg->getBoundingBox().size.height * 0.5f));
    
    int allStoneNum = 0;
    //遍历所有的建筑根据所有类型为魔法石库的容量来计算当前魔法石上限
    for(int i = 0; i < m_allBuildObb.size(); i++)
    {
        //在这里计算魔法石每秒的恢复速度
        pvpBuildInfo* buildInfo = CDataManager::getBuildInfo(m_allBuildObb[i].buildingId);
        if(buildInfo->buildingType == 1)//魔法石库
        {
            allStoneNum += buildInfo->effectTime;
        }
    }    if(allStoneNum <= m_stone)
    {
        m_progress->setPercentage(100);
        sprintf(buf, "%d", allStoneNum);
        pcurStoneNum->setString(buf);
    }
    else{
        float percent = m_stone/(float)allStoneNum * 100;
        m_progress->setPercentage(percent);
        sprintf(buf, "%d", m_stone);
        pcurStoneNum->setString(buf);
    }
    
    m_pAllStone = Sprite::createWithSpriteFrameName("pvpStoneBox.png");
    addChild(m_pAllStone, 100);
    m_pAllStone->setPosition(Vec2(m_winSize.width * 0.82f, m_winSize.height * 0.95));
    
    normal = Sprite::createWithSpriteFrameName("pvp_fight.png");
    select = Sprite::createWithSpriteFrameName("pvp_fight.png");
    select->setColor(Color3B::GRAY);
    m_pFightItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(pvpEnemyLayer::btnCallBack, this));
    Menu* fightMenu = Menu::create(m_pFightItem , NULL);
    //fightItem->setScale(3.0f);
    m_pFightItem ->setTag(PVP_FIGHT_TAG);
    addChild(fightMenu, 100);
    fightMenu->setPosition(Vec2(m_winSize.width * 0.1f, m_winSize.height * 0.08f));
    
    Label* pLabel = Label::createWithTTF("开始战斗", "fonts/cuti.ttf", 32);
    m_pFightItem ->addChild(pLabel,0,100);
    pLabel->setPosition(Vec2(normal->getContentSize().width * 0.5f, normal->getContentSize().height * 0.52f));
    
    
    normal = Sprite::createWithSpriteFrameName("reSearch.png");
    select = Sprite::createWithSpriteFrameName("reSearch.png");

    select->setColor(Color3B::GRAY);
    m_searchItem = LYMenuItemSprite::create(normal, select, CC_CALLBACK_1(pvpEnemyLayer::btnCallBack, this));
    Menu* searchMenu = Menu::create(m_searchItem, NULL);
    m_searchItem->setTag(PVP_RESEARCH_TAG);
    addChild(searchMenu, 100);
    searchMenu->setPosition(Vec2(m_winSize.width * 0.9f, m_winSize.height * 0.08f));
    
    //搜索下一个目标需要的金币
    Sprite* coin = Sprite::createWithSpriteFrameName("coin.png");
    coin->setAnchorPoint(Vec2(1, 0.5f));
    m_searchItem->addChild(coin);
    coin->setScale(0.65f);
    coin->setPosition(Vec2(normal->getContentSize().width * 0.95f, normal->getContentSize().height * 0.25f));

    sprintf(buf, "%d", 6);
    Label* pCostCoinNum = Label::createWithSystemFont(buf, "Arial", 30);
    m_searchItem->addChild(pCostCoinNum,0,100);
    pCostCoinNum->setPosition(Vec2(normal->getContentSize().width * 0.51f, normal->getContentSize().height * 0.25f));
    
    //可能获得的魔法石
    Label* pGetStone = Label::createWithTTF("可获取魔法石", "fonts/cuti.ttf", 24);
    addChild(pGetStone, 100);
    pGetStone->enableOutline(Color4B(0,0,0,255),3);
    pGetStone->setPosition(Vec2(m_winSize.width * 0.5f, m_winSize.height * 0.1f));
    
    Sprite* stoneBg = Sprite::createWithSpriteFrameName("pvpStoneBg.png");
    addChild(stoneBg, 100);
    stoneBg->setPosition(Vec2(m_winSize.width * 0.5f, m_winSize.height * 0.04f));
    
    Sprite* stone = Sprite::createWithSpriteFrameName("stone.png");
    stoneBg->addChild(stone);
    stone->setScale(0.6f);
    stone->setPosition(Vec2(stone->getBoundingBox().size.width * 0.35f, stoneBg->getBoundingBox().size.height * 0.45f));
    sprintf(buf, "%d", int(m_stone * 0.1f));
    
    Label* pStoneNum = Label::createWithSystemFont(buf, "Arial", 32);
    stoneBg->addChild(pStoneNum);
    pStoneNum->setPosition(Vec2(stoneBg->getBoundingBox().size.width * 0.55f, stoneBg->getBoundingBox().size.height * 0.5f));
    
    sprintf(buf, "%s的窝 Lv:%d", m_name.c_str(), m_level);
    Label* pEnemy = Label::createWithTTF(buf, "fonts/cuti.ttf", 30);
    addChild(pEnemy, 100);
    pEnemy->enableOutline(Color4B(0, 0, 0, 255), 3);
    pEnemy->setPosition(Vec2(m_winSize.width * 0.5f, m_winSize.height * 0.15f));

}


void pvpEnemyLayer::addTower(buildingMessage& messInfo)
{
    m_buildInfo = CDataManager::getBuildInfo(messInfo.buildingId);
    FighterInfo theInfo;
    CDataManager::CalcFighterInfoWith(&theInfo, m_buildInfo->monsterId, m_buildInfo->level, 1, 0, false, 1, 0, 0, 0);
    CBaseBuilding* m_curBuild = CBaseTower::create(&theInfo,eFightGroup_Freind);
    m_curBuild->ChangeModelWithPVPBuildingId(m_buildInfo->buildingID);
    m_curBuild->m_bIfActive = true;
    CFightData::getInstance()->addMonster(m_curBuild,eAttackType_tower);
    
    layer3D->addChild(m_curBuild);
    Vec3 Mid3DPos = Vec3(messInfo.xPos, 0, messInfo.zpos);
    m_curBuild->setPosition3D(Mid3DPos);
    m_curBuild->setCameraMask((unsigned short )CameraFlag::USER1);
    
    addBuildObb(m_curBuild);
}

void pvpEnemyLayer::addBuildObb(CBaseBuilding* tower)
{
    Vec3 p = tower->getPosition3D() + tower->gettheSprite()->getPosition3D();
    float height = tower->gettheSprite()->getBoundingBox().size.height;
    
    p.y = p.y+height*0.5f;
    
    Vec3 extents = Vec3(height*0.35f, height*0.35f, height*0.35f);
    AABB aabb(-extents, extents);
    auto obb = OBB(aabb);
    obb._center = p;
    towerOBBInfo pInfo;
    pInfo.obb = obb;
    pInfo.theTower = tower;
    m_allBuildObb.push_back(pInfo);
    
    buildingMessage messageInfo;
    //messageInfo.buildingId = tower->m_iBuildingID;
    messageInfo.xPos = tower->getPositionX();
    messageInfo.zpos = tower->getPositionZ();
    messageInfo.state = 1;//需要建造时间
    messageInfo.startUpTime = 0;
    messageInfo.needTime = m_buildInfo->costTime;
}

void pvpEnemyLayer::btnCallBack(Ref* pSender)
{
    int tag = ((Node*)pSender)->getTag();
    if(tag == PVP_BACK_TAG)
    {
        CLayerManager::GetSingletonPtr()->createLayer(eLayer_PVPLayer, NULL,false);
    }
    else if(tag == PVP_RESEARCH_TAG)
    {
        //判断金币是否足够 //花费金币数量待定
        if(CPersonalData::getInstance()->getUserCoin() < 6)
        {
            Sprite * thetarget = CFightManager::getTheFlyWord("金币不足","fonts/cuti.ttf");
            thetarget->setPosition(m_winSize.width*0.5,m_winSize.height*0.5);
            addChild(thetarget,100);
            return;
        }
        else
        {
            CNetWorkDataSingle::GetSingletonPtr()->SaveUserMCF(this, NULL, -6,0,0);
            m_allBuildObb.clear();
            addCloud();
            //显示搜索图标
            CallFunc* searchAct = CallFunc::create(CC_CALLBACK_0(pvpEnemyLayer::startSearchEnemy, this));
            runAction(Sequence::create(DelayTime::create(0.6f), searchAct, NULL));
        }
    }
    else if(tag == PVP_FIGHT_TAG)
    {
  
        if(CPersonalData::getInstance()->getEnemyType() == SPECIFIED_ENEMY)
        {
            CPersonalData::getInstance()->setFromVideoList(true);
        }
        CFightData::getInstance()->setFightType(FIGHT_PVP_TYPE);
        
        
        m_searchItem->runAction(MoveTo::create(1,Vec2(m_searchItem->getPositionX(),-1000)));
        
        Label *pLabel = (Label*)m_searchItem->getChildByTag(100);
        
        if(pLabel)
        {
            pLabel->setVisible(false);
        }
        
        m_pFightItem->runAction(Sequence::create(MoveTo::create(3,Vec2(-1000,-1000)),CallFunc::create([=]{
    
        char buf[256];
        std::vector<std::string> tempValue;
        sprintf(buf, "%d", EPVPMode_PLAY);
        tempValue.push_back(buf);
        sprintf(buf, "%d", int(m_stone * 0.1f));
        tempValue.push_back(buf);
        tempValue.push_back(m_enemyMaps);
            
        CPersonalData::getInstance()->setPvpMainLayer(false);
        CLayerManager::GetSingletonPtr()->createLayer(eLayer_FightPVP, NULL,false, true, &tempValue);

        }),NULL));

         pLabel =  (Label*)m_pFightItem->getChildByTag(100);
        
        if(pLabel)
        {
            pLabel->setVisible(false);
        }
                                        //所有按钮开始移动
        //战斗场景飞入
        
        m_pBackItem->runAction(MoveTo::create(3,Vec2(-1000,1000)));
        
        m_progress->runAction(MoveTo::create(3,Vec2(1000,1000)));
        
        m_pAllStone->runAction(MoveTo::create(3,Vec2(1000,1000)));
        

        
        
    }

}

void pvpEnemyLayer::calculateRayByLocationInView(Ray* ray,Camera* theCamera, const Vec2& location)
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

void pvpEnemyLayer::onTouchesBegan(const std::vector<Touch*>& touches, Event *unused_event)
{
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
        Ray ray;
        calculateRayByLocationInView(&ray,_camera, m_startPos);
        CBaseBuilding* tempTarget = NULL;
        
        for(int i = 0; i < m_allBuildObb.size(); i++)
        {
            //避免重复选择z轴值最大的一个
            if(ray.intersects(m_allBuildObb[i].obb))
            {
                if(!tempTarget)
                {
                    tempTarget = m_allBuildObb[i].theTower;
                }
                else
                {
                    if(m_allBuildObb[i].theTower->getPositionZ() > tempTarget->getPositionZ())
                    {
                        tempTarget = m_allBuildObb[i].theTower;
                    }
                }
                m_curBuild = tempTarget;
            }
        }
    }
}

void pvpEnemyLayer::onTouchesMoved(const std::vector<Touch*>& touches, Event *unused_event)
{
    if(touches.size() > 1)
    {
        Vec2 mPoint1 = touches.at(0)->getLocationInView();
        Vec2 mPoint2 = touches.at(1)->getLocationInView();
        //计算两个触摸点之间的距离
        float distance = sqrt((mPoint2.x - mPoint1.x) * (mPoint2.x - mPoint1.x) + (mPoint2.y - mPoint1.y) * (mPoint2.y - mPoint1.y));
        float addDistance = distance - m_distance;
        float nowY = _camera->getPositionY() - addDistance;
        float nowZ = _camera->getPositionZ() - addDistance;
        if((nowY >= 162 ) && (nowY <= 514) && (nowZ >= -238) && (nowZ <= 114))
        {
            _camera->setPosition3D(Vec3(_camera->getPositionX(), nowY, nowZ));
            log("当前相机坐标y=%f,z=%f", nowY, nowZ);
        }
    }
    else
    {
        m_curBuild = NULL;
        Touch* pTouch = touches.at(0);
        auto preLocation = pTouch->getPreviousLocation();
        auto location = pTouch->getLocationInView();
        float xPos = (preLocation.x - location.x);
        float CameraPosX = _camera->getPositionX() + xPos;
        //float CameraPosY = _camera->getPositionZ() + yPos;
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
}

void pvpEnemyLayer::onTouchesEnded(const std::vector<Touch*>& touches, Event *unused_event)
{
    if(touches.size() <= 1)
    {
        Touch* pTouch = touches.at(0);
        Vec2 touchPos = pTouch->getLocationInView();
        if(m_curBuild && (touchPos.distance(m_startPos) < 20))
        {
            Ray ray;
            calculateRayByLocationInView(&ray,_camera, touchPos);
            CBaseFighter* tempTarget = NULL;
            for(int i = 0; i < m_allBuildObb.size(); i++)
            {
                //避免重复选择z轴值最大的一个
                if(ray.intersects(m_allBuildObb[i].obb))
                {
                    log("摸到了");
                    break;
                }
            }
        }
    }
}










