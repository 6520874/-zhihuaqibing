//
//  LYFightScenePVP.cpp
//  WarOfGod
//
//  Created by user on 15/5/29.
//
//

#include "LYFightScenePVP.h"
#include "LYFightData.h"
#include "DBMgr.h"
#include "zzbNetControlModule.h"
#include "zzbNewNetStruct.h"

#include "LYBaseBuilding.h"
#include "FightFailLayer.h"
#include "FightWinLayer.h"
#include "guiderManagerLayer.h"

//static  std::string  m_sPVPRecord="";
//static std::string m_sEnemyInfo ="";
//static std::string m_sCampInfo="";


CFightScenePVP::CFightScenePVP(){
    
    m_eFightSceneType = eFightSceneType_PVP;
    m_pLableTime =NULL;
    m_iAllTime =  100;
    m_bTimerStart = true;
    
    m_bIfVideoReady = true;
    m_listen = NULL;
    m_curTower = NULL;
    m_preTower = NULL;
    m_towerName = NULL;
    m_towerLevel = NULL;
    m_curBuildingId = 0;

    m_iNowTrueId=1;

    m_patrolman = NULL;

}

CFightScenePVP::~CFightScenePVP(){
    
    
    //暂时把数据上传放在析构函数中
    
//    if(CPersonalData::getInstance()->getEnemyType() != SPECIFIED_ENEMY&&CFightData::getInstance()->getIsPvpStart())
//    {
//        uploadInfo();
//    }
//    //测试用，看到就删。--如果是播放状态，则清掉所有的敌人信息
//    if (m_ePVPMode==EPVPMode_REC) {
//        m_sPVPRecord="";
//    }
    
    //_eventDispatcher->removeEventListener(m_listen);
    for(int i = 0; i < m_vecBuildingInfo.size(); i++)
    {
        delete m_vecBuildingInfo[i].pBbuildInfo;
        m_vecBuildingInfo[i].pBbuildInfo = NULL;
    }
    m_vecBuildingInfo.clear();
}



CFightScenePVP* CFightScenePVP::create(PVPMode temp,int robStone,std::string MapInfo,std::string EnemyInfo,std::string recInfo){
    
    CFightScenePVP* target = new CFightScenePVP();
    

   target->setPVPMode(temp,robStone, MapInfo,EnemyInfo,recInfo);
    
    if (target&&target->init()) {
        
        target->autorelease();
    }else{
        
        CC_SAFE_DELETE(target);
        target=NULL;
    }
    

    
    return target;
}

bool CFightScenePVP::init()
{
    if(!CFightScene::init())
    {
        return false;
    }
    /*m_listen = EventListenerTouchOneByOne::create();
    m_listen->onTouchBegan = CC_CALLBACK_2(CFightScenePVP::onTouchBegan, this);
    m_listen->onTouchMoved = CC_CALLBACK_2(CFightScenePVP::onTouchMoved, this);
    m_listen->onTouchEnded = CC_CALLBACK_2(CFightScenePVP::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(m_listen, this);*/
    return true;
}

CBaseBuilding* CFightScenePVP::getTouchTower(Vec2 touchPos)
{
    Ray ray;
    calculateRayByLocationInView(&ray,_camera, touchPos);
    CBaseBuilding* tempTarget = NULL;
    for(int i = 0; i < (int)m_vecBuildingInfo.size(); i++)
    {
        //避免重复选择z轴值最大的一个
        if(ray.intersects(m_vecBuildingInfo[i].obb))
        {
            if(!tempTarget)
            {
                tempTarget = m_vecBuildingInfo[i].pBbuildInfo->theTower;
                m_curBuildingId = m_vecBuildingInfo[i].buildingId;
            }
            else
            {
                if(m_vecBuildingInfo[i].pBbuildInfo->theTower->getPositionZ() > tempTarget->getPositionZ())
                {
                    tempTarget = m_vecBuildingInfo[i].pBbuildInfo->theTower;
                    m_curBuildingId = m_vecBuildingInfo[i].buildingId;
                }
            }
        }
    }
    return tempTarget;
}

bool CFightScenePVP::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    if(CFightData::getInstance()->getIsPvpStart() == 0)
    {
        m_startPos = touch->getLocationInView();
        if(m_curTower)
        {
            m_preTower = getTouchTower(m_startPos);
        }
    }
    
     return  CFightScene::onTouchBegan(touch, event);

}

void CFightScenePVP::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event)
{
    CFightScene::onTouchMoved(touch, event);
    if((CFightData::getInstance()->getIsPvpStart() == 0) && (CPersonalData::getInstance()->getEnemyType() != FIGHT_BACK_VIDEO))
    {
        updateTowerObbInfo();
    }
}

void CFightScenePVP::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event)
{
    if(CFightData::getInstance()->getIsPvpStart() == 0)
    {
        if(touch->getLocationInView().distance(m_startPos) < 20)
        {
            if(m_preTower && m_curTower && (m_preTower == m_curTower))
            {
                return;
            }
            rmvNameAndLevel();
            Vec2 pos = touch->getLocationInView();
            m_curTower = getTouchTower(pos);
            if(m_curTower)
            {
                addTowerNameAndLevel();
            }
        }
    }
    CFightScene::onTouchEnded(touch, event);
}

void CFightScenePVP::rmvNameAndLevel()
{
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
    if(m_curTower && m_curTower->m_pAtkRangeCircle->isVisible())
    {
        m_curTower->m_pAtkRangeCircle->setVisible(false);
    }
    
}

void CFightScenePVP::addTowerNameAndLevel()
{
    rmvNameAndLevel();
    Size bgSize = m_curTower->gettheSprite()->getBoundingBox().size;
    pvpBuildInfo* pInfo = CDataManager::getBuildInfo(m_curBuildingId);
    monsterInfo* pMonsterInfo = CDataManager::getMonsterInfoByMonsterId(pInfo->monsterId);
    m_towerName = Label::createWithTTF(pMonsterInfo->monsterName.c_str(),"fonts/cuti.ttf",20);   // Label::createWithSystemFont(pInfo-
    m_curTower->addChild(m_towerName, 101);
    m_towerName->enableOutline(Color4B(0, 0, 0, 255), 2);
    m_towerName->setPosition3D(Vec3(0, bgSize.height + 35, 0));
    m_towerName->setCameraMask((unsigned short )CameraFlag::USER1);
    
    int level = pInfo->level;
    for(int i = 0; i < (int)m_vecBuildingInfo.size(); i++)
    {
        if(m_curTower == m_vecBuildingInfo[i].pBbuildInfo->theTower)
        {
            if(m_vecBuildingInfo[i].state == 2 && pInfo->level > 1)
            {
                level--;
            }
            break;
        }
    }
    __String* temp = __String::createWithFormat("%d级", level);
    m_towerLevel = Label::createWithTTF(temp->getCString(), "fonts/cuti.ttf", 18);
    m_curTower->addChild(m_towerLevel, 102);
    m_towerLevel->enableOutline(Color4B(0, 0, 0, 255), 2);
    m_towerLevel->setPosition3D(Vec3(0, bgSize.height + 10, 0));
    m_towerLevel->setCameraMask((unsigned short )CameraFlag::USER1);
    if(m_curTower && (m_curTower->m_pAtkRangeCircle->getScale() > 0) && (!m_curTower->m_pAtkRangeCircle->isVisible()))
    {
        float scale = m_curTower->m_fScale;
        m_curTower->m_pAtkRangeCircle->setScale(0);
        m_curTower->m_pAtkRangeCircle->setVisible(true);
        m_curTower->m_pAtkRangeCircle->runAction(ScaleTo::create(0.16f, scale));
    }
}

void CFightScenePVP::updateTowerObbInfo()
{
    for(int i = 0; i < (int)m_vecBuildingInfo.size(); i++)
    {
        CBaseBuilding* tower = m_vecBuildingInfo[i].pBbuildInfo->theTower;
        Vec3 p = tower->getPosition3D() + tower->gettheSprite()->getPosition3D();
        float height = tower->gettheSprite()->getBoundingBox().size.height;
        
        p.y = p.y+height*0.5f;
        
        Vec3 extents = Vec3(height * 0.35f + 15, height * 0.35f + 15, height * 0.35f + 15);
        AABB aabb(-extents, extents);
        auto obb = OBB(aabb);
        obb._center = p;
        
        m_vecBuildingInfo[i].obb = obb;
        m_vecBuildingInfo[i].posx = tower->getPositionX();
        m_vecBuildingInfo[i].posy = tower->getPositionZ();
    }
}


void CFightScenePVP::setPVPMode(PVPMode temp,int robStone,std::string mapInfo,std::string PVPEnemyInfo,std::string theInfo){
    
    
    m_ePVPMode = temp;
//
    m_sPVPRecord = theInfo;
    
    m_sEnemyInfo = PVPEnemyInfo;
    
    
    
    //测试用
//    if (m_sPVPRecord=="") {
//        m_ePVPMode = EPVPMode_PLAY;
//    }else{
//        m_ePVPMode = EPVPMode_REC;
//    }
    
    m_iRobStones = robStone;

    
    CFightScene::setRobStones(robStone);
    
    //初始化关卡
    CFightData::getInstance()->newPVPStart();
    
    
    //如果是录像，则进行相关数据初始化
    if (m_ePVPMode == EPVPMode_REC) {
        
        initPVPRecMode(mapInfo, m_sEnemyInfo, m_sPVPRecord);
    }else{
        
        initPVPPlayMode(mapInfo);
    }


    
}

void CFightScenePVP::initPVPPlayMode(std::string MapInfo){
    
    //addChild(CFightWinLayer::create(),1000);

    //进攻模式初始化地图等信息
    
    //关闭按钮
    
    
    //倒计时
    

    initCampInfo(MapInfo);

}



void CFightScenePVP::initCampInfo(std::string campInfo)
{
    
    
    m_sCampInfo = campInfo;
    
    
    //测试用
//    m_sCampInfo = "6,2870,-399,1,0,230";
    
    std::vector<__String> parametaers =CDBMgr::separateInfoWithMark(m_sCampInfo, ";");
    
    for (int i=0; i<parametaers.size(); i++) {
        
        PVPBuildingInfo temp;
        
        std::vector<__String> infos =CDBMgr::separateInfoWithMark(parametaers.at(i), ",");
        
        //参数0代表userbuildingid，不用解析
        
        temp.buildingId = infos.at(1).intValue();
        temp.posx = infos.at(2).floatValue();
        temp.posy = infos.at(3).floatValue();
        temp.state = infos.at(4).intValue();
        
        pvpBuildInfo* theBuildInfo = CDataManager::getBuildInfo(temp.buildingId);
        
        int monsterId = theBuildInfo->monsterId;
        
        CDataManager::CalcFighterInfoWith(&temp.theBuildInfo, monsterId,  1,1,0,false,1);
        
        temp.theBuildInfo.attk =theBuildInfo->attack;  //加入完美度修改
        temp.theBuildInfo.maxHP = theBuildInfo->life;  //加入完美度修改
        
        
        m_vecBuildingInfo.push_back(temp);
        
    }
}


void CFightScenePVP::initPVPRecMode(std::string MapInfo, std::string EnemyInfo, std::string recInfo)
{
    
    int iPos=  recInfo.find("EndTime=");
    
    if(iPos>=0)
    {
        
        log("endTime= ");
        
        std::string strEndTime = recInfo.substr(iPos+8,strEndTime.length()-1);
        
        int  iTime = atoi(strEndTime.c_str());
        
        CFightScene::addRecEndTime(iTime+1);
    
    }

    
    iPos  = recInfo.find("isWin=");
                         
    if(iPos>=0)
    {
        std::string str  =  recInfo.substr(iPos+6,recInfo.length()-1);
        
        int iWin = atoi(str.c_str());
        
        addRecIsWin(iWin);
        
        
    }
                         
                        
    
    std::vector<FighterInfo>* AllFighters = CFightData::getInstance()->getALLPVPFightInfo();
//    AllFighters->clear();
    
    
    //解析地图信息
    initCampInfo(MapInfo);
    
    /*
     解析敌人士兵信息
     */
    std::vector<__String> parametaers =CDBMgr::separateInfoWithMark(EnemyInfo, ";");
    
    for (int i=0; i<parametaers.size(); i++) {
        
        FighterInfo theMonsterinf;
        
        std::vector<__String> infos = CDBMgr::separateInfoWithMark(parametaers.at(i), ",");
        
        if (infos.size()<7) {
            
            CCLOG("LYASSERT::analysePVPEnemyInfo Error::%s",parametaers.at(i).getCString());
            continue;
        }
        
        
        int monsterId = infos.at(0).intValue();
        int monsterLevel = infos.at(1).intValue();
        int monsterRare = infos.at(2).intValue();
        
        int skillLevel1 = infos.at(3).intValue();
        int skillLevel2 = infos.at(4).intValue();
        int skillLevel3 = infos.at(5).intValue();
        int skillLevel4 = infos.at(6).intValue();
        
        
        CDataManager::CalcFighterInfoWith(&theMonsterinf, monsterId, monsterLevel,monsterRare,0,false,skillLevel1,skillLevel2,skillLevel3,skillLevel4);
        
//        CDataManager::CalcFighterInfoWith(&theMonsterinf, theGroup[i], m_pTheLevelInfo->monsterLevel,1,0,false,m_pTheLevelInfo->monsterSkillLevel,m_pTheLevelInfo->monsterSkillLevel,m_pTheLevelInfo->monsterSkillLevel,m_pTheLevelInfo->monsterSkillLevel);
        
        AllFighters->push_back(theMonsterinf);
    }
    

    
    /*
     解析回放信息
     */
    std::string head = "videoInfo=";
    unsigned long offset = m_sPVPRecord.find(head.c_str()); //去掉信息头
    m_sPVPRecord = m_sPVPRecord.substr(offset+head.length(),-1);
    
    
    parametaers =CDBMgr::getInstance()->separateInfoWithMark(m_sPVPRecord, ";");
    for (int i=0; i<parametaers.size(); i++) {
        
        PlayInfo resultInfo;
        
        std::string theString = parametaers.at(i).getCString();
        
        __String value = "";
        
        //初步解析数据，把时间头解析出来
        long offset = theString.find(",");
        value = theString.substr(0,offset);
        resultInfo.time = value.floatValue();
        
        resultInfo.theInfo = theString.substr(offset+1,-1);
        
        
        m_vecPlayInfo.push_back(resultInfo);
    }

}

void CFightScenePVP::levelInit(){
    

    pvpCommonData::getInstance()->setDestroyedBuildNum(0);
    CFightData::getInstance()->initPvpZhaohuanKulou();
    
    bool bExisted = false;//是否有祭坛
    int index = 0;
    if(CFightData::getInstance()->getFightType() == FIGHT_PVP_TYPE)
    {
        pvpCommonData::getInstance()->setAllBuildNum((int)m_vecBuildingInfo.size());
    }
    for(int i=0;i<m_vecBuildingInfo.size();i++){
        
        CBaseBuilding* target = NULL;
        
        pvpBuildInfo* pBuildInfo = CDataManager::getBuildInfo(m_vecBuildingInfo.at(i).buildingId);
        
        if(pBuildInfo->buildingType == 11)
        {
            bExisted = true;
            index = i;
        }
        int iType = m_vecBuildingInfo.at(i).theBuildInfo.monsterType;
        
        
        
        if (iType==eFighterType_Trap) {
            
            bool bUpdate = false;
            if(m_vecBuildingInfo[i].state == 2)
            {
                bUpdate = true;
                
            }
            
            target =  (CBaseBuilding*)addMonsterWithIdAndGroupAndPosition(&m_vecBuildingInfo.at(i).theBuildInfo,eFightGroup_Enemy,Vec2(m_vecBuildingInfo.at(i).posx, m_vecBuildingInfo.at(i).posy),eAttackType_Trap,false,false,false,false,bUpdate);//
            
            
//            target->m_bIfActive = true;
            
//        }else if (buildingType==0){ //主基地
//            
//            target =  (CBaseBuilding*)addMonsterWithIdAndGroupAndPosition(&m_vecBuildingInfo.at(i).theBuildInfo,eFightGroup_Enemy,Vec2(m_vecBuildingInfo.at(i).posx, m_vecBuildingInfo.at(i).posy),eAttackType_city,false,false,false);//
//            
        }else{
            
            
            bool bUpdate = false;
            if(m_vecBuildingInfo[i].state == 2)
            {
                bUpdate = true;
                
            }
    
            target =  (CBaseBuilding*)addMonsterWithIdAndGroupAndPosition(&m_vecBuildingInfo.at(i).theBuildInfo,eFightGroup_Enemy,Vec2(m_vecBuildingInfo.at(i).posx, m_vecBuildingInfo.at(i).posy),eAttackType_tower,false,false,false,false, bUpdate);//
            
        }
        
        
        //如果是主基地，则要记录该对象
        if (pBuildInfo->buildingType==0) {
            CFightData::getInstance()->setPVPTheCity(target);
        }
         if(m_vecBuildingInfo[i].state == 2)
         {
             target->ChangeModelWithPVPBuildingId(m_vecBuildingInfo.at(i).buildingId - 1);
         }
         else{
             target->ChangeModelWithPVPBuildingId(m_vecBuildingInfo.at(i).buildingId);
         }
        
        target->setVisible(true);
        
        target->resetHPPosition();
        
        target->m_bIfActive = true;
        
        Vec3 p = Vec3(m_vecBuildingInfo.at(i).posx, 0, m_vecBuildingInfo.at(i).posy) + target->gettheSprite()->getPosition3D();
        float height = target->gettheSprite()->getBoundingBox().size.height;
        p.y = p.y+height*0.5f;
        Vec3 extents = Vec3(height * 0.35f + 15, height * 0.35f + 15, height * 0.35f + 15);
        AABB aabb(-extents, extents);
        auto obb = OBB(aabb);
        obb._center = p;
        m_vecBuildingInfo.at(i).obb = obb;
        
        //根据坐标初始化方块
        locationInfo allBlock[MAP_X_INDEX][MAP_Y_INDEX];
        for(int m = 0; m < MAP_X_INDEX; m++)
        {
            for(int n = 0; n < MAP_Y_INDEX; n++)
            {
                allBlock[m][n].type = 0;
            }
        }
        for(int k = 0; k < (int)pBuildInfo->blockPos.size(); k++)
        {
            float xPos = m_vecBuildingInfo.at(i).posx + pBuildInfo->blockPos[k].x * LENGTH_BLOCK_PER;
            float zPos = m_vecBuildingInfo.at(i).posy + pBuildInfo->blockPos[k].y * LENGTH_BLOCK_PER;
            int index = (xPos - MAP_START_XPOS)/LENGTH_BLOCK_PER;
            int indey = (zPos - STARTZPOINT)/LENGTH_BLOCK_PER + 9;
            allBlock[index][indey].type = 1;
        }
        std::string bgImg;
        float scale = 1.0f;
        if(pBuildInfo->blockPos.size() == 1)
        {
            bgImg = "pvp_one.png";
            scale = 1.1f;
        }
        else if(pBuildInfo->blockPos.size() == 4)
        {
            bgImg = "pvp_four.png";
            scale = 2.0f;
        }
        else if(pBuildInfo->blockPos.size() == 9)
        {
            bgImg = "pvp_nine.png";
            scale = 3.3f;
        }
        if(pBuildInfo->buildingType == 0)
        {
            Sprite3D* pCircle = CFightManager::getTheModelWithPath("fight_CityCircle2.png");
            layer3D->addChild(pCircle, 100);
            pCircle->getChildByTag(100)->setScale(1.25f);
            pCircle->setRotation3D(Vec3(-90, 0, 0));
            pCircle->setPosition3D(Vec3(m_vecBuildingInfo.at(i).posx, 1, m_vecBuildingInfo.at(i).posy));
            pCircle->setCameraMask((unsigned short )CameraFlag::USER1);
            //pCircle->runAction(RepeatForever::create(RotateBy::create(0.5f, Vec3(0,10,0))));
        }

        //影子shadow
        
        Sprite3D* pShadow = CFightManager::getTheModelWithPath("touying.png");
        layer3D->addChild(pShadow, 100);
        pShadow->getChildByTag(100)->setScale(scale);
        pShadow->getChildByTag(100)->setOpacity(170);
        pShadow->setRotation3D(Vec3(-90,0,0));
        pShadow->setPosition3D(Vec3(m_vecBuildingInfo.at(i).posx, 0, m_vecBuildingInfo.at(i).posy));
        pShadow->setCameraMask((unsigned short )CameraFlag::USER1);
        
        Sprite3D* TowerBg = CFightManager::getTheModelWithPath(bgImg.c_str());
        layer3D->addChild(TowerBg);
        TowerBg->setPosition3D(Vec3(m_vecBuildingInfo.at(i).posx, 0, m_vecBuildingInfo.at(i).posy));
        TowerBg->setRotation3D(Vec3(270, 0, 0));
        TowerBg->getChildByTag(100)->setScale(0.6f);
        TowerBg->setCameraMask((unsigned short )CameraFlag::USER1);
        
        buildInfo* pBuild = new buildInfo();
        pBuild->pInfo = pBuildInfo;
        pBuild->shadow = pShadow;
        pBuild->towerBase = TowerBg;
        pBuild->theTower = target;
        m_vecBuildingInfo.at(i).pBbuildInfo = pBuild;
        pvpCommonData::getInstance()->setAllBack(allBlock);
        
        target->setPvpShadow(pShadow);
        target->setPvpDizuo(TowerBg);
        if (iType==eFighterType_Trap)
        {
            if(target->getPvpShadow())
            {
                target->getPvpShadow()->setVisible(false);
            }
            if(target->getPvpDizuo())
            {
                target->getPvpDizuo()->setVisible(false);
            }
        }
    }
    
    if(bExisted)//如果是祭坛则添加怪物
    {
        int monsterId = m_vecBuildingInfo.at(index).pBbuildInfo->pInfo->allEvolutionVec[0].monsterId;
        int i = 0;
        if(pvpCommonData::getInstance()->getEvolution() == 1)
        {
            monsterId = m_vecBuildingInfo.at(index).pBbuildInfo->pInfo->allEvolutionVec[1].monsterId;
            i = (int)(m_vecBuildingInfo.at(index).pBbuildInfo)->pInfo->allEvolutionVec.size() - 1;
        }
        
        CDataManager::CalcFighterInfoWith(&m_thePatrolInfo, monsterId, 1, 1, 0, false, 0, 0, 0, 0);
        pvpCommonData::getInstance()->setTowerInfo(m_vecBuildingInfo.at(index).pBbuildInfo);
        Vec3 pos = Vec3(m_vecBuildingInfo.at(index).posx - 0.5*LENGTH_BLOCK_PER, 1, m_vecBuildingInfo.at(index).posy - 0.5*LENGTH_BLOCK_PER);
        m_patrolman =  (CPartolFight*)addMonsterWithIdAndGroupAndPosition(&m_thePatrolInfo, eFightGroup_Enemy, Vec2(pos.x, pos.z),eAttackType_Partol,false,false,false);
        m_patrolman->setMonsterId(monsterId);
        m_patrolman->setTargetPos(Vec2(m_vecBuildingInfo.at(index).posx, m_vecBuildingInfo.at(index).posy));
        
        Vec3 tergetPos  = m_patrolman->getValidPosition();
        m_originPos = m_patrolman->m_persionPos[0];
        m_patrolman->setDestationPos(tergetPos);
        m_patrolman->setMaxHP(m_vecBuildingInfo.at(index).pBbuildInfo->pInfo->allEvolutionVec[i].life);
        m_patrolman->setNowAttack(m_vecBuildingInfo.at(index).pBbuildInfo->pInfo->allEvolutionVec[i].attack);
    }
    
}

void CFightScenePVP::resetOriginPos()
{
    if(m_patrolman)
    {
        m_patrolman->setPosition3D(m_originPos);
        pvpCommonData::getInstance()->setUpBack(true);
        m_patrolman->setIndex(0);
    }
}


CBaseFighter* CFightScenePVP::addMonsterWithIdAndGroupAndPosition(FighterInfo* theInfo, FightGroup theGroup, cocos2d::Vec2 position,AttackType theType,bool ifMidCamp,bool ifboss,bool ifElite,bool bZhaohuan,bool bUpdating)
{
    
    
    
    if (m_ePVPMode == EPVPMode_PLAY && theGroup== eFightGroup_Freind) {  //如果是玩家操作，则记录
        
        addRecordStringWithAddMonster(theInfo->monsterId, position, ifElite);
        
        
        addSoldierNum(theInfo->monsterId, theInfo->level);
        
    }

    CBaseFighter* target =CFightScene::addMonsterWithIdAndGroupAndPosition(theInfo, theGroup, position, theType, ifMidCamp, ifboss, ifElite,bZhaohuan,bUpdating);
    
    target->setTrueId(m_iNowTrueId);
    
    m_iNowTrueId++;//怪物唯一标识++
    
    
    return target;
}


void CFightScenePVP::addRecordStringWithAddMonster(int monsterId, cocos2d::Vec2 Pos, bool ifElite){
    
    //记录的时间点，类型，和信息
    
    float time = CFightData::getInstance()->getAllFightTime();
    
    __String* temp = __String::createWithFormat("%0.2f,%d,%d,%0.1f,%0.1f,%d;",time,ERecordTpe_AddMonster,monsterId,Pos.x,Pos.y,ifElite);
    
    m_sPVPRecord +=temp->getCString();
    
}

void CFightScenePVP::addRecordStringWithCatapult(cocos2d::Vec3 ballPos, cocos2d::Vec2 ballSpeed,int damage){
    
    
    float time = CFightData::getInstance()->getAllFightTime();
    
    __String* temp = __String::createWithFormat("%0.2f,%d,%0.1f,%0.1f,%0.1f,%0.1f,%0.2f,%d;",time,ERecordTpe_Catapult,ballPos.x,ballPos.y,ballPos.z,ballSpeed.x,ballSpeed.y,damage);
    
    m_sPVPRecord +=temp->getCString();
    

}

void CFightScenePVP::addRecordStringWithChangeHP(int TrueId, int damage,int damageType, bool ifDied){
    
    float time = CFightData::getInstance()->getAllFightTime();
    
    __String* temp = __String::createWithFormat("%0.2f,%d,%d,%d,%d,%d;",time,ERecordTpe_ChangeHP,TrueId,damage,damageType,ifDied);
    
    m_sPVPRecord +=temp->getCString();
    
}

void CFightScenePVP::addRecordStringWithchangeTarget(int attackTrueId, int beattackedTrueId){
    
    
    float time = CFightData::getInstance()->getAllFightTime();
    
    __String* temp = __String::createWithFormat("%0.2f,%d,%d,%d;",time,ERecordTpe_ChangeTarget,attackTrueId,beattackedTrueId);
    
    m_sPVPRecord +=temp->getCString();
    
}





void CFightScenePVP::addSoldierNum(int monsterId, int level,int soldierNum){
    
    
    bool haveOne=false;
    for (int i=0; i<m_vecSolderNumber.size(); i++) {
        
        if (m_vecSolderNumber.at(i).theId==monsterId) {
            haveOne=true;
            
            m_vecSolderNumber.at(i).number+=soldierNum;
        }
        
    }
    
    if (!haveOne) {
        IdAndTwoNum tempOne;
        
        tempOne.theId = monsterId;
        tempOne.level = level;
        tempOne.number = soldierNum;
        
        m_vecSolderNumber.push_back(tempOne);
        
    }
    
}





void CFightScenePVP::update(float time){
 
    CFightScene::update(time);
    
    float fightTime = CFightData::getInstance()->getAllFightTime();
    
    while (m_vecPlayInfo.size()>0) {
        
//        if (m_bIfVideoReady) {
//            
//            m_bIfVideoReady=false;
//            
//            float nextTime = m_vecPlayInfo.at(0).time-fightTime;
//            
//            nextTime = MAX(0, nextTime);
//            
//            scheduleOnce(schedule_selector(CFightScenePVP::videoUpdate), nextTime);
//            
//        }
//        
//        
//        break;
        
        
        if (fightTime>=m_vecPlayInfo.at(0).time)
        {
            analysePlayInfo(m_vecPlayInfo.at(0).theInfo);
            m_vecPlayInfo.erase(m_vecPlayInfo.begin());
        }
        else
        {
            break;
        }
        
        
    }
    
}

//void CFightScenePVP::videoUpdate(float time){
//    
//    float offsetTime = CFightData::getInstance()->getAllFightTime()-m_vecPlayInfo.at(0).time;
//    CCLOG("录像时间误差%f",offsetTime);
//    
//    
//    
//    m_bIfVideoReady=true;
//    
//    analysePlayInfo(m_vecPlayInfo.at(0).theInfo);
//    m_vecPlayInfo.erase(m_vecPlayInfo.begin());
//    
//    
//    
//}



void CFightScenePVP::analysePlayInfo(std::string theInfo){
    
    
    std::vector<__String> parametaers =  CDBMgr::getInstance()->separateInfoWithMark(theInfo, ",");
    
    
    int type = parametaers.at(0).intValue();
    
    
    int iPos=  theInfo.find("EndTime=");
    
    if(iPos>=0)
    {
        return ;
    }

    iPos  = theInfo.find("isWin=");

    if(iPos>=0)
    {
        std::string str  =  theInfo.substr(7,theInfo.length()-1);
        
        return;
    }

     switch (type) {
        case ERecordTpe_AddMonster:
        {
            
            if (parametaers.size()<5) {
                CCLOG("LYASSERT::analysePlayInfo Error::%s",theInfo.c_str());
                return;
            }
            
            int monsterId = parametaers.at(1).intValue();
            float posX = parametaers.at(2).floatValue();
            float posY = parametaers.at(3).floatValue();
            bool ifElite = parametaers.at(4).boolValue();
            
            FighterInfo* temp = CFightData::getInstance()->getPVPFightInfoById(monsterId);
            
            if (temp) {
                addMonsterWithIdAndGroupAndPosition(temp, eFightGroup_Freind, Vec2(posX, posY),eAttackType_monster,false,false,ifElite);
            }
            
        }
            break;
            
        case ERecordTpe_Catapult:
        {
            
            if (parametaers.size()<7) {
                CCLOG("LYASSERT::analysePlayInfo Error::%s",theInfo.c_str());
                return;
            }
            
            Vec3 ballPos = Vec3(parametaers.at(1).floatValue(), parametaers.at(2).floatValue(), parametaers.at(3).floatValue());
            
            Vec2 ballSpead = Vec2(parametaers.at(4).floatValue(),parametaers.at(5).floatValue());
            
            if (m_paoWuLayer) {
                m_paoWuLayer->StartAttackWith(ballPos, ballSpead,parametaers.at(6).intValue());
            }
 
        }
            break;
             
         case ERecordTpe_ChangeHP:
         {
             
             if (parametaers.size()<5) {
                 CCLOG("LYASSERT::analysePlayInfo Error::%s",theInfo.c_str());
                 return;
             }
             
             int theTrueId = parametaers.at(1).intValue();
             int damage = parametaers.at(2).intValue();
             int damageType = parametaers.at(3).intValue();
             int ifForceDied = parametaers.at(4).intValue();
             
             //根据theTrueId找出对应怪物，然后调用它的伤血
             CBaseFighter* target = CFightData::getInstance()->getMonsterWithTrueId(theTrueId);
             
             if (target) {
                 target->changeHP(damage, true, (DamageType)damageType, NULL, ifForceDied);
             }

         }
             break;
             
         case ERecordTpe_ChangeTarget:
         {
             
             if (parametaers.size()<3) {
                 CCLOG("LYASSERT::analysePlayInfo Error::%s",theInfo.c_str());
                 return;
             }
             
             int attackTrueId = parametaers.at(1).intValue();
             int beAttackTrueId = parametaers.at(2).intValue();
             
             //根据theTrueId找出对应怪物，然后调用它的设置目标
             CBaseFighter* attacker = CFightData::getInstance()->getMonsterWithTrueId(attackTrueId);
             
             CBaseFighter* target = CFightData::getInstance()->getMonsterWithTrueId(beAttackTrueId);
             
             if (attacker&&target) {
                 attacker->setAttackTarget(target,true);
             }
             
         }
             break;
            
            
            
            
        default:
            break;
    }
    
    
    
}

void CFightScenePVP::uploadInfo(){
    

    
    //玩家士兵相关信息
    m_sEnemyInfo = "";
    std::vector<FighterInfo*> teamInfo = CPersonalData::getInstance()->getTeamInfo();
    for (int i=0; i<teamInfo.size(); i++) {
        
        //判断是否有技能
        std::vector<int> skilInfo;//默认四个技能
        for(int k = 0; k < teamInfo.at(i)->theFighterSkillInfo.size(); k++)
        {
            skilInfo.push_back(teamInfo.at(i)->theFighterSkillInfo[k].skillLevel);
        }
        if((int)skilInfo.size() < 4)
        {
            for(int j = (int)skilInfo.size();  j < 4; j++)
            {
                skilInfo.push_back(0);
            }
        }
        __String *temp = __String::createWithFormat("%d,%d,%d,%d,%d,%d,%d;",teamInfo.at(i)->monsterId,teamInfo.at(i)->level,teamInfo.at(i)->rare,
                                                    skilInfo[0],skilInfo[1],skilInfo[2],skilInfo[3]);
        
        m_sEnemyInfo+=temp->getCString();
    }
    
    m_sPVPRecord;//录像信息
    
    m_sCampInfo;//阵营信息
    
    
    //玩家使用的士兵数量信息
    std::string allNum ="";
    
    for (int i=0; i<m_vecSolderNumber.size(); i++) {
        
        __String *tempValue = __String::createWithFormat("%d,%d,%d;",m_vecSolderNumber.at(i).theId, m_vecSolderNumber.at(i).level, m_vecSolderNumber.at(i).number);
        
        allNum = allNum+ tempValue->getCString();
    }
    
    
    //玩家信息
    UserInfo* pInfo = CPersonalData::getInstance()->getUserInfo();
    std::string name = CNetControlModule::encode(pInfo->userName);   //编码
    __String* playedrInfo = __String::createWithFormat("%d,%s,%d",pInfo->m_iUserId,name.c_str(),pInfo->m_iUserLevel);
    
    
    //输赢判断
    int ifWin = 0; //0代表输，1代表赢
    
    if (CFightData::getInstance()->getWinState()==1) {
        ifWin = 1;
    }

    
    float fNowTime = CFightData::getInstance()->getAllFightTime();

    char strbuf[50] = {0};
    sprintf(strbuf,"EndTime=%f;",fNowTime);
    
    m_sPVPRecord += strbuf;
    pvpEnemyInfo EnemyInfo_s = CFightData::getInstance()->getThePvpEnemyInfo();
    
    
    //测试用，修改录像内容
//    m_sPVPRecord="";
//    m_sPVPRecord="1.83,0,1,1680.0,-518.0,0;1.83,3,24,25;2.72,3,25,5;3.12,0,1,1680.0,-435.0,0;3.80,0,1,1680.0,-543.0,1;4.02,3,26,2;4.51,0,1,1680.0,-463.0,0;4.72,3,27,5;5.40,3,28,2;5.66,3,18,25;6.32,2,25,294,1,0;6.49,3,23,25;6.57,3,21,25;7.07,2,5,181,0,0;7.74,2,25,294,1,0;7.74,3,24,26;7.82,3,21,26;8.07,3,28,24;8.32,2,2,181,0,0;8.49,2,25,441,1,1;8.49,2,26,441,1,0;8.49,2,27,441,1,0;8.59,2,26,1,0,0;8.76,3,18,26;9.09,2,5,271,0,0;9.25,2,26,294,1,0;9.84,2,24,181,0,0;9.92,2,2,181,0,0;10.09,0,1,1680.0,-273.0,1;10.18,2,26,1,0,0;10.70,2,26,294,1,1;10.70,2,5,271,0,0;10.70,3,21,27;11.04,3,29,8;11.46,2,24,181,0,1;11.79,3,18,28;12.29,2,27,294,1,0;12.29,2,5,271,0,0;12.90,3,28,3;13.00,3,21,28;13.51,2,3,181,0,0;13.80,2,27,294,1,0;13.97,2,5,271,0,0;15.10,0,1,1680.0,-342.0,0;15.19,2,3,181,0,0;15.29,2,28,294,1,0;15.29,2,8,271,0,0;16.04,3,30,7;16.37,2,2,406,1,0;16.80,2,27,294,1,0;16.89,2,8,271,0,0;17.41,2,5,271,0,0;17.59,2,2,271,1,0;18.44,2,27,294,1,1;18.52,2,8,271,0,0;18.61,2,3,181,0,0;19.94,2,28,294,1,0;20.11,2,8,271,0,0;20.19,2,3,181,0,0;20.35,2,7,181,0,0;21.44,2,28,294,1,0;21.77,2,3,181,0,0;22.02,2,7,181,0,0;22.52,2,3,406,1,0;23.02,2,28,294,1,1;23.02,3,21,30;23.52,2,8,271,0,0;23.69,2,7,181,0,0;24.10,3,18,30;24.35,0,1,1680.0,-270.0,1;24.62,2,29,294,1,0;25.12,2,8,271,0,0;25.29,3,31,8;25.29,2,7,181,0,0;26.12,2,29,294,1,0;26.70,2,8,271,0,0;27.62,2,2,271,1,0;27.70,2,29,294,1,0;28.39,2,8,271,0,0;28.64,2,7,181,0,0;29.22,2,29,294,1,0;29.55,2,8,271,0,0;30.22,2,7,181,0,0;30.72,2,3,406,1,0;30.72,2,29,294,1,1;31.22,2,8,271,0,1;31.85,2,7,181,0,0;32.32,2,30,294,1,0;32.57,3,31,7;33.02,0,1,1680.0,-608.0,0;33.20,2,7,271,0,0;33.47,2,7,181,0,0;33.80,2,30,294,1,0;33.97,3,32,4;34.80,2,7,271,0,0;35.30,2,30,294,1,0;35.80,2,2,271,1,0;36.89,2,30,294,1,1;36.89,3,21,31;37.14,2,2,406,1,0;37.89,3,18,31;38.14,2,7,271,0,0;38.30,2,4,181,0,0;38.39,2,31,294,1,0;39.80,2,7,271,0,1;39.89,2,31,294,1,0;39.89,2,4,181,0,0;40.89,0,1,1680.0,-404.0,0;41.16,3,31,3;41.40,2,31,294,1,0;41.49,2,4,181,0,0;41.57,2,3,271,0,0;41.82,3,33,3;42.91,2,31,294,1,0;43.15,2,4,181,0,0;43.24,2,3,271,0,0;43.50,0,1,1680.0,-329.0,1;44.45,3,34,3;44.45,2,31,294,1,1;45.54,2,4,271,1,0;45.54,3,18,33;45.54,3,21,33;46.04,2,33,294,1,0;46.12,2,3,181,0,0;46.54,2,4,181,0,0;47.55,2,33,294,1,0;47.72,2,3,181,0,1;47.80,3,34,2;48.14,2,4,181,0,0;48.80,2,2,271,0,0;48.99,2,33,294,1,0;49.07,3,33,2;49.49,2,2,181,0,0;49.74,2,4,181,0,0;49.99,0,1,1680.0,-466.0,1;50.42,2,2,271,0,1;50.52,2,33,294,1,1;50.52,3,21,34;50.87,3,35,6;51.37,2,4,181,0,0;51.62,3,18,34;51.79,3,34,6;52.12,2,34,294,1,0;52.20,2,6,271,0,0;53.20,0,1,1680.0,-522.0,1;53.72,2,34,294,1,0;53.80,2,4,271,1,0;53.80,2,6,271,0,0;54.14,3,36,5;54.80,2,4,181,0,0;55.14,2,6,271,0,0;55.22,2,34,294,1,0;56.22,2,4,406,1,1;56.80,2,34,294,1,0;56.80,2,6,271,0,0;57.30,2,6,271,0,0;57.80,3,32,5;57.89,3,18,35;57.89,3,21,35;58.22,2,5,181,0,0;58.41,2,34,294,1,1;58.41,2,5,271,0,0;58.41,2,6,271,0,0;58.49,0,1,1680.0,-515.0,1;59.42,3,37,5;59.84,2,5,181,0,0;59.92,2,35,294,1,0;60.00,2,5,271,0,0;60.00,2,6,271,0,0;61.50,2,35,294,1,0;61.67,2,5,271,0,1;61.75,3,37,6;62.27,2,6,271,1,0;62.44,2,6,406,1,1;62.52,3,37,10;63.02,3,32,10;63.02,3,36,10;63.02,2,35,294,1,0;63.19,3,35,12;63.62,2,10,271,0,0;63.79,2,12,271,0,0;64.29,0,1,1680.0,-436.0,1;64.47,2,35,294,1,0;64.47,2,10,271,0,0;65.09,3,32,10;65.17,3,38,12;65.44,2,12,271,0,0;65.95,2,10,406,1,0;66.04,2,35,294,1,1;66.04,3,21,37;66.14,2,10,271,0,0;67.07,2,10,271,0,0;67.17,3,18,37;67.47,2,10,181,0,0;67.67,2,36,294,1,0;67.67,2,10,271,0,0;67.87,0,1,1680.0,-341.0,1;68.74,2,10,271,0,0;68.82,3,39,9;69.17,2,10,181,0,0;69.25,2,36,294,1,0;69.35,2,10,271,0,1;69.84,2,12,271,0,0;70.14,3,36,16;70.24,3,18,38;70.24,3,21,36;70.54,3,32,16;70.72,3,37,12;70.72,2,37,294,1,0;70.82,2,16,271,0,0;71.22,2,16,181,0,0;71.52,2,12,271,0,0;71.92,2,12,406,1,0;72.32,2,37,294,1,0;72.52,2,16,271,0,0;73.02,2,12,271,0,0;73.02,3,21,39;73.22,2,12,271,0,0;73.42,3,18,39;73.52,2,12,271,1,0;73.52,2,9,271,0,0;73.62,0,1,1680.0,-373.0,1;73.92,2,38,294,1,0;74.52,3,40,9;74.52,2,16,181,0,0;74.72,2,12,271,0,1;74.82,2,13,406,1,0;75.12,2,9,271,0,0;75.52,2,38,294,1,0;75.92,2,16,271,0,0;76.12,3,37,16;76.22,2,16,181,0,0;76.22,3,21,37;76.32,3,38,18;76.42,3,21,38;76.52,3,18,38;76.82,2,16,271,0,0;76.82,2,9,271,0,0;77.02,2,37,294,1,0;77.72,2,13,406,1,0;77.72,2,16,271,0,0;77.72,2,16,181,0,0;78.45,2,16,271,0,0;78.45,2,9,271,0,0;78.54,2,37,294,1,0;78.79,2,18,271,0,0;79.29,2,16,271,0,1;79.64,2,9,271,0,0;79.80,3,37,14;80.05,2,37,294,1,1;80.34,2,18,271,0,0;80.69,3,36,14;80.77,3,32,14;80.77,2,9,406,1,0;81.29,2,14,271,0,0;81.29,2,9,271,0,0;81.29,0,1,1680.0,-452.0,0;81.65,2,36,294,1,0;81.82,2,9,271,0,0;81.99,2,18,271,0,0;82.15,2,13,271,1,0;82.26,3,41,9;82.94,2,9,271,0,1;83.02,3,41,11;83.10,2,14,181,0,0;83.19,3,39,11;83.19,2,36,294,1,0;83.64,2,18,271,0,0;83.64,2,11,271,0,0;83.72,2,13,406,1,0;84.32,3,40,11;84.74,2,36,294,1,1;84.74,2,14,181,0,0;84.92,2,11,271,0,0;85.25,2,11,271,0,0;85.95,2,11,406,1,0;86.29,2,38,294,1,0;86.39,2,14,181,0,0;86.74,3,21,40;86.84,2,11,271,0,0;87.10,2,18,271,0,1;87.27,2,11,406,1,0;87.27,2,11,181,0,0;87.95,2,14,181,0,0;88.29,2,11,271,0,0;88.47,3,38,14;88.55,3,21,38;88.89,2,11,181,0,1;89.05,2,14,271,0,0;89.32,2,15,406,1,0;89.69,3,40,17;90.10,3,39,15;90.10,2,17,271,0,0;90.27,3,41,15;90.37,2,13,271,1,0;90.70,2,14,271,0,0;90.70,2,15,271,0,0;90.87,2,15,181,0,0;91.44,2,14,181,0,0;91.44,0,1,1680.0,-577.0,1;91.82,2,17,271,0,0;92.39,3,42,14;92.39,2,14,271,0,0;92.39,2,15,271,0,0;92.49,2,15,181,0,0;93.04,2,14,181,0,0;93.40,2,17,271,0,0;94.09,2,15,271,0,0;94.69,2,14,181,0,0;94.89,2,13,406,1,0;94.89,2,15,271,1,0;95.79,2,15,271,0,0;95.87,2,14,271,0,1;95.97,3,42,17;95.97,2,15,406,1,1;95.97,2,15,181,0,1;96.07,3,32,13;96.17,3,21,32;96.77,2,13,181,0,0;96.97,2,17,271,0,0;97.17,3,39,17;97.27,3,38,13;97.37,3,41,17;97.37,3,21,38;97.87,2,13,271,0,1;98.07,2,17,271,0,0;98.17,3,32,21;98.27,3,21,32;98.57,2,17,406,1,0;98.57,2,17,271,0,0;99.27,3,38,17;99.27,3,41,17;99.67,2,21,271,1,0;99.77,2,17,271,0,0;99.87,2,17,271,0,1;99.87,2,17,181,0,1;100.77,2,21,181,0,0;100.97,3,39,21;101.17,3,42,21;101.37,3,38,21;101.37,3,41,21;101.57,3,38,21;101.57,3,40,21;101.67,3,21,38;101.87,2,21,271,0,1;101.87,2,21,271,0,1;101.97,3,40,1;101.97,3,41,1;102.17,3,32,1;103.09,2,1,181,0,0;103.19,3,39,1;103.29,3,42,1;103.59,3,38,1;103.69,2,1,271,0,0;104.39,2,1,181,0,0;104.79,2,1,181,0,0;105.09,2,1,271,0,0;105.39,2,1,271,0,0;105.99,2,1,271,0,0;106.09,2,1,406,1,0;106.39,3,40,1;106.39,2,1,181,0,0;106.79,2,1,271,1,0;106.99,3,38,1;107.60,2,1,406,1,0;107.70,2,1,406,1,0;107.90,2,1,181,0,0;108.42,2,1,406,1,0;108.72,2,1,271,1,0;108.82,2,1,271,0,0;108.82,2,1,271,0,0;109.52,2,1,181,0,0;109.52,2,1,271,0,0;109.92,2,1,181,0,0;110.42,2,1,271,0,0;110.42,2,1,271,0,0;110.42,2,1,271,0,0;111.12,2,1,181,0,0;111.12,2,1,271,0,0;111.52,2,1,181,0,0;112.12,2,1,271,0,0;112.12,2,1,271,0,0;112.12,2,1,271,0,0;112.72,2,1,181,0,0;112.72,2,1,271,0,0;113.12,2,1,181,0,0;113.72,2,1,271,0,0;113.72,2,1,271,0,0;113.72,2,1,271,0,0;114.42,2,1,271,0,0;114.82,2,1,181,0,0;115.12,2,1,271,1,0;115.52,2,1,271,0,0;116.22,2,1,406,1,0;116.22,2,1,406,1,0;116.22,2,1,181,0,0;116.92,2,1,406,1,0;117.22,2,1,271,1,0;117.32,2,1,271,0,0;117.32,2,1,271,0,0;117.82,2,1,181,0,0;117.82,2,1,271,0,0;117.92,2,1,406,1,0;118.12,2,1,181,0,0;118.92,2,1,271,0,0;118.92,2,1,271,0,0;118.92,2,1,271,0,0;119.62,2,1,181,0,0;119.62,2,1,271,0,0;119.92,2,1,181,0,0;EndTime=120.020805;";
    
//    int length = m_sPVPRecord.length();
    
    m_sPVPRecord = "videoInfo="+ m_sPVPRecord;  //因为录像信息可能过长，因此使用postdata方式上传
    
    if(CPersonalData::getInstance()->getEnemyType() != SPECIFIED_ENEMY)//回放不再保存录像
    {
        //建筑的摧毁程度
        float percent = 100*(float)pvpCommonData::getInstance()->getDestroyedBuildNum() / pvpCommonData::getInstance()->getAllBuildNum();
        CNetControlModule::sharedNetControlModule()->uploadVideo(this, menu_selector(CFightScenePVP::uploadInfoBack), EnemyInfo_s.m_iUserId, m_sCampInfo.c_str(),m_sEnemyInfo,playedrInfo->getCString(), allNum.c_str(), m_iRobStones, m_sPVPRecord.c_str(), ifWin, (int)percent);
    }
    
    
}

void CFightScenePVP::uploadInfoBack(Ref* pSender)
{
    _netRecvEventUploadVideoInfo* pInfo = (_netRecvEventUploadVideoInfo*)pSender;
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
                log("上传玩家录像信息失败");
                return;
            }
        }
    }
    if(CFightData::getInstance()->getIsPvpStart() == 2)
    {
        CNetControlModule::sharedNetControlModule()->getUserTeamInfo(this, menu_selector(CFightScene::backPvpMainLayer));
    }
}

