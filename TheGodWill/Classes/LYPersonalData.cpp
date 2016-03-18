//
//  LYPersonalData.cpp
//  TheGodWill
//
//  Created by user on 14/12/5.
//
//

#include "LYPersonalData.h"
#include "HttpEncrypt.h"
#include "CSharedPlist.h"
#include "DBMgr.h"
#include "sysTime.h"
#include "NetWorkDataSingle.h"
USING_NS_CC;



CPersonalData* CPersonalData::m_singleton = nullptr;//m_singleton指向空指针


CPersonalData::CPersonalData()
{
 
    m_stUserInfo = new UserInfo();
    m_iStar = 0;
    m_bOpenNewLevel = false;
    m_bWinManual = true;
    m_bFailManual = true;
    m_mode = 0;
    m_bOpenNewSmallLevel = false;
    m_bNewMidLevel = false;
    m_bJudgeUpgrade = false;
    m_bCatapult = false;
    m_brestartPlayMusic = true;
    m_bCutGuide = false;

    m_mode = 0;
    m_pTDAccount = NULL;

    m_bInsertHero = false;
    m_bFinishedLevel = false;
    m_bifUpdateUserInfo = false;
    m_bChargeSucess = false;
    m_pChargeInfo = NULL;
    m_bJumpFromSkill = false;
    m_bPvpMainLayer = false;
    m_iState = 1;
    m_enemyType = RANDOM_ENEMY;
    m_bFromVideoList = false;
    m_bTouchRmvFlag = false;
    m_iVideoId = 0;
    m_iEffectId = 0;
    m_bUpdateKuanshiInfo = false;
    m_bShowMap = false;
}


CPersonalData::~CPersonalData()
{
}

CPersonalData* CPersonalData::getInstance(){
    
    //判断m_singleton是否已被实例化， 如果没有，将之实例
    
    if(nullptr == m_singleton)
    {
        m_singleton = new CPersonalData();//
        
        m_singleton->initUserInfo();
    }
    
    return m_singleton;
}

void CPersonalData::refreshUserInfo(){
    
    
    if (nullptr == m_singleton) {
        m_singleton = new CPersonalData();//
        
        m_singleton->initUserInfo();
    }else{
        
        m_singleton->initUserInfo();
    }

}



std::vector<BuildingUpgradeInfo*> CPersonalData::getBuildingUpgradeInfo()
{
    return m_pVecMyBuildinUpgradeInfo;
}

std::vector<EnergyUpgradeInfo*>  CPersonalData::getEnergyUpgradeInfo()
{
    return m_pVecMyEnergyUpgradeInfo;
}

std::vector<FighterInfo*>  CPersonalData::getAllSoldierInfo()
{
    return m_vecMyFighterInfo;
}

int CPersonalData::DataStrToInt(std::string temp){
    
    
    int target = atoi(ParseData(temp.c_str()).c_str());
    return target;
}

std::string CPersonalData::DataIntToStr(int temp){
    
    __String *strImpirical = __String::createWithFormat("%d",temp);
    std::string str = SaveData(reinterpret_cast<const unsigned char*>(strImpirical->getCString()), strImpirical->length());
    return  str;
}


void CPersonalData::setUserCoin(int num)
{
    __String *strImpirical = __String::createWithFormat("%d",num);
    std::string str = SaveData(reinterpret_cast<const unsigned char*>(strImpirical->getCString()), strImpirical->length());
    m_stUserInfo->m_sCoin =  str;
}

int CPersonalData::getUserCoin()
{
    int coin = atoi(ParseData(m_stUserInfo->m_sCoin.c_str()).c_str());
    return coin;
}

void CPersonalData::setUserStone(int num)
{
    __String *strImpirical = __String::createWithFormat("%d",num);
    std::string str = SaveData(reinterpret_cast<const unsigned char*>(strImpirical->getCString()), strImpirical->length());
    
    m_stUserInfo->m_sMagicStone =  str;
    CPersonalData::getInstance()->setUpdateUserInfo(true);
}

int CPersonalData::getUserStone()
{
    int stone = atoi(ParseData(m_stUserInfo->m_sMagicStone.c_str()).c_str());
    
    return stone;
}

void CPersonalData::setUserKuangStone(int num)
{
    __String *strImpirical = __String::createWithFormat("%d",num);
    std::string str = SaveData(reinterpret_cast<const unsigned char*>(strImpirical->getCString()), strImpirical->length());
    
    m_stUserInfo->m_pvpStoneNum = str;
}


void CPersonalData::setUserMaxKuangStone(int num)
{
    __String *strImpirical = __String::createWithFormat("%d",num);
    std::string str = SaveData(reinterpret_cast<const unsigned char*>(strImpirical->getCString()), strImpirical->length());
    
    m_stUserInfo->m_pvpStoneMaxNum = str;
}


int CPersonalData::getUserKuangStone()
{
    int stone = atoi(ParseData(m_stUserInfo->m_pvpStoneNum.c_str()).c_str());
    
    return stone;
}

int CPersonalData::getUserMaxKuangStone()
{
    int stone = atoi(ParseData(m_stUserInfo->m_pvpStoneMaxNum.c_str()).c_str());
    
    return stone;
    
}

void CPersonalData::SetPhysicalNum(int iPhysicalNum)
{
    CDBMgr::getInstance()->saveUserStamina(iPhysicalNum);
    __String *strStamina = __String::createWithFormat("%d",iPhysicalNum);
    std::string str = SaveData(reinterpret_cast<const unsigned char*>(strStamina->getCString()),strStamina->length());
    m_stUserInfo->m_sStaminaNum = str;
}

int CPersonalData::GetPhysicalNum()
{
    int iStamina = atoi(ParseData(m_stUserInfo->m_sStaminaNum.c_str()).c_str());
    return iStamina;
}

void CPersonalData::setPhysicalMaxNum(int maxPhysicalNum)
{
    __String *strStamina = __String::createWithFormat("%d",maxPhysicalNum);
    std::string str = SaveData(reinterpret_cast<const unsigned char*>(strStamina->getCString()), strStamina->length());
    
    m_stUserInfo->m_sMaxStaminaNum = str.c_str();
}

int CPersonalData::GetPhysicalMaxNum()
{
    int MaxPhysicalNum = atoi(ParseData(m_stUserInfo->m_sMaxStaminaNum.c_str()).c_str());
    
    return MaxPhysicalNum;
}

int CPersonalData::getStarByLevel(int smallLevel, bool isEqlite)
{
    std::vector<levelAndStar> userLevelVec = CPersonalData::getInstance()->getUserInfo()->m_userLevelVec;
    if(isEqlite){
        userLevelVec = CPersonalData::getInstance()->getUserInfo()->m_userEqliteLevelVec;
    }
    for (int i = 0; i < userLevelVec.size(); i++) {
        if (userLevelVec.at(i).level == smallLevel) {
            return userLevelVec.at(i).star;
        }
    }
    return -1;
}


void CPersonalData::initUserInfo()
{
    userInfoInDB* userInfoDB = CDataManager::getTheUSerInfDB();
    m_stUserInfo->m_id = CDataManager::getTheUSerInfDB()->m_id;
    m_stUserInfo->m_iUserId = CDataManager::getTheUSerInfDB()->userId;
    m_stUserInfo->userName = CDataManager::getTheUSerInfDB()->userName;
    m_stUserInfo->m_iUserLevel = CDataManager::getTheUSerInfDB()->userLevel;
    m_stUserInfo->m_iUserExp = CDataManager::getTheUSerInfDB()->userExp;
    

    
    
//    setUserCoin(CDataManager::getTheUSerInfDB()->coin);
//    
//    setUserStone(CDataManager::getTheUSerInfDB()->magicStone);
//    
//    SetPhysicalNum(CDataManager::getTheUSerInfDB()->stamina);
    
    
    
    m_stUserInfo->m_sCoin = userInfoDB->coinStr;
    m_stUserInfo->m_sMagicStone = userInfoDB->magicStoneStr;
    m_stUserInfo->m_sStaminaNum = userInfoDB->staminaStr;
    m_stUserInfo->m_iStaminTime = userInfoDB->staminaTime;
    
    upgradeInfo* pUpgradeInfo = CDataManager::getUpgradeInfoByLevel(m_stUserInfo->m_iUserLevel);
    
    setPhysicalMaxNum(pUpgradeInfo->upperStamina);
    m_stUserInfo->m_iSmallLevel =  CDataManager::getTheUSerInfDB()->smallLevel;
    m_stUserInfo->m_sloginDay = CDataManager::getTheUSerInfDB()->loginDay;
    m_stUserInfo->m_iEqliteSmallLevel = CDataManager::getTheUSerInfDB()->eqliteSmallLevel;
   // m_stUserInfo->m_heroSkillsVec = CDataManager::getTheUSerInfDB()->heroSkills;
    m_stUserInfo->m_useDebrisVec = CDataManager::getTheUSerInfDB()->m_debrisVec;
    m_stUserInfo->m_userEqliteLevelVec = CDataManager::getTheUSerInfDB()->userEqliteLevelVec;
    m_stUserInfo->m_userLevelVec = CDataManager::getTheUSerInfDB()->userLevelVec;
    m_stUserInfo->m_userMidLevelRewardVec = CDataManager::getTheUSerInfDB()->userMidLevelRewardVec;
    m_stUserInfo->m_userHeroMidLevelRewardVec = CDataManager::getTheUSerInfDB()->userHeroMidLevelRewardVec;
    m_stUserInfo->m_sAuthCode = CDataManager::getTheUSerInfDB()->authCode;
    m_stUserInfo->m_strBudilingLv = CDataManager::getTheUSerInfDB()->strBuildingLv;
    m_stUserInfo->m_strEnergyLv = CDataManager::getTheUSerInfDB()->strEnergyLv;
    m_stUserInfo->m_iTimeStamp = CDataManager::getTheUSerInfDB()->taskTimeStamp;
    m_stUserInfo->m_userTowerLevelVec = CDataManager::getTheUSerInfDB()->userTowerLevelVec;
    m_stUserInfo->m_skillUpgradeInfo = CDataManager::getTheUSerInfDB()->m_skillUpgradeInfo;
    m_stUserInfo->m_strHeroLv = CDataManager::getTheUSerInfDB()->m_strHeroLv;
    log("stamina =%s,maxstamina = %s\n", m_stUserInfo->m_sStaminaNum.c_str(), m_stUserInfo->m_sMaxStaminaNum.c_str());
    
    m_stUserInfo->DBVersion = CDataManager::getTheUSerInfDB()->DBVersion;
    m_stUserInfo->storyInfo = CDataManager::getTheUSerInfDB()->storyInfo;
//    setUserCoin(CDataManager::getTheUSerInfDB()->coin);
//    setUserStone(CDataManager::getTheUSerInfDB()->magicStone);
    m_stUserInfo->ifSuggesed = CDataManager::getTheUSerInfDB()->ifSuggesed;
    
    
    if(m_stUserInfo->m_iUserLevel == 1)
    {
        if(CDataManager::getTheUSerInfDB()->maxStamina == 0)
        {
            SetPhysicalNum(CDataManager::getUpgradeInfoByLevel(1)->upperStamina);
            setPhysicalMaxNum(CDataManager::getUpgradeInfoByLevel(1)->upperStamina);
        }
        
    }
    
    //初始化所有士兵信息
    std::vector<myMonster*>* theVec = &CDBMgr::getInstance()->UserMonsterInfo;
    
    
    m_vecMyFighterInfo.clear();
    for (std::vector<myMonster*>::iterator ite = theVec->begin(); ite!=theVec->end(); ite++)
    {
        myMonster* temp = *ite;
        
        FighterInfo  *targetInfo = refreshSoldierInfoWithInfo(temp);
        
//        FighterInfo  *targetInfo = new FighterInfo;
//        
//        
//        targetInfo->theMonsterInfo = CDataManager::getMonsterInfoByMonsterId(temp->monsterId);
//        targetInfo->skillFrameNum=0;
//        targetInfo->level = temp->monsterLevel;
//        
//        CDataManager::CalcFighterInfoWith(targetInfo, temp->monsterId, temp->monsterLevel, temp->rare, temp->theType,temp->ifFight, temp->skillLevel1,temp->skillLevel2,temp->skillLevel3,temp->skillLevel4);
        
        m_vecMyFighterInfo.push_back(targetInfo);
    }

    
    //初始化出战士兵信息
    m_vecMyTeamInfo.clear();
    for (int i=0; i<m_vecMyFighterInfo.size(); i++)
    {
        
        if (m_vecMyFighterInfo[i]->ifFight==1)
        {
            //根据类型初始化，0为士兵，1为英雄
            if(m_vecMyFighterInfo[i]->theType==0)
            {
                m_vecMyTeamInfo.push_back(m_vecMyFighterInfo[i]);
            }
//            else if (m_vecMyFighterInfo[i]->theType==1&&m_vecMyFighterInfo[i]->monsterId==1004)
//            {
//                m_pTheHero = m_vecMyFighterInfo[i];
//            }
        }

    }
    
    
    
    
    
    
    //初始化英雄技能信息
    m_vecTheHeroSkill.clear();
    for (int i=0; i<userInfoDB->heroSkills.size(); i++)
    {
        
       
        heroSkillInfo target;
        
        IdAndNum temp = userInfoDB->heroSkills[i];
        
        target.level = temp.theNumber;
        
        target.theSkill = CDataManager::getTheHeroSkillWithId(temp.theId);
        
        
        if(target.theSkill)
        {
        target.lastTime = CDataManager::getTheNumWithMinMax(1, target.theSkill->levelMax, target.theSkill->lastTimeMin, target.theSkill->lastTimeMax, temp.theNumber);
        
        target.damage = CDataManager::getTheNumWithMinMax(1, target.theSkill->levelMax, target.theSkill->damageMin, target.theSkill->damageMax, temp.theNumber);
        
            
        m_vecTheHeroSkill.push_back(target);
        }
        
    }
    
    
    
    
    
    
//    if (!targetInfo) {
//        targetInfo = new FighterInfo;
//    }
    
    
    //初始化塔防相关信息
    std::vector<fightBuildingInfo*>* allFightBuilding= CDataManager::getAllFightBuildingInfo();
    
    m_vecTheBuildingInfo.clear();
    for (int i=0; i<allFightBuilding->size(); i++) {
        
        
        FighterInfo targetInfo ;
        targetInfo.theMonsterInfo = CDataManager::getMonsterInfoByMonsterId(allFightBuilding->at(i)->monsterId);
        targetInfo.skillFrameNum=0;
        targetInfo.level = 1;
        CDataManager::CalcFighterInfoWith(&targetInfo, targetInfo.theMonsterInfo->monsterId, targetInfo.level,1,0,false,1,1,1,1);
        m_vecTheBuildingInfo.push_back(targetInfo);
    }
    
    
    //初始化英雄等级
    myMonster * pMyMonster = CDataManager::getuserMonsterById(1005);
    if(pMyMonster)
    {
       pMyMonster->monsterLevel = CDBMgr::getInstance()->GetHeroLvWithId(1005);
       CPersonalData::getInstance()->refreshSoldierInfoWithInfo(pMyMonster);
     }

    
}




void CPersonalData::detectExchangeNum(long time)
{
    long lastTime = atol(CDataManager::getUserInfoNum()->refreshTime.c_str());
    CGameTime::my_time_t timeLast = CGameTime::convertTimeToMyTimeformat(lastTime);
    
    CGameTime::my_time_t timeNow = CGameTime::convertTimeToMyTimeformat(time);
    
    
    if((lastTime == 0)|| (timeNow.year > timeLast.year) || ((timeNow.year == timeLast.year) && (timeNow.month > timeLast.month))
       ||((timeNow.year == timeLast.year) && (timeNow.month == timeLast.month) && (timeNow.day > timeLast.day)
          && (timeNow.hour >= 3)))
    {
        CDataManager::refreshUserInfoNum();
        __String* timeStr = __String::createWithFormat("%ld",time);
        CDataManager::saveUpdateExchangeNumTime(timeStr->getCString());//纪录刷新时间
    }
}


void CPersonalData::detectTaskRefresh(long time)
{
    long lastTime = atol(CPersonalData::getInstance()->getUserInfo()->m_iTimeStamp.c_str());
    CGameTime::my_time_t timeLast = CGameTime::convertTimeToMyTimeformat(lastTime);
    
    //CGameTime::my_time_t timeNow = CGameTime::convertTimeToMyTimeformat(CPersonalData::getInstance()->getUserInfo()->serverLogintime/1000);
    CGameTime::my_time_t timeNow = CGameTime::convertTimeToMyTimeformat(time);
    
    if((lastTime == 0)|| (timeNow.year > timeLast.year) || ((timeNow.year == timeLast.year) && (timeNow.month > timeLast.month))
       ||((timeNow.year == timeLast.year) && (timeNow.month == timeLast.month) && (timeNow.day > timeLast.day)
          && (timeNow.hour >= 3)))
    {
        
        CDataManager::refreshDailyTask();
        __String* timeStr = __String::createWithFormat("%ld",time);
        CPersonalData::getInstance()->getUserInfo()->m_iTimeStamp = timeStr->getCString();
        CDataManager::saveUpdateDailyaTaskTime(timeStr->getCString());//纪录刷新时间
    }
}

FighterInfo* CPersonalData::getBuildingInfoById(int buildingId){
    
    
    for (int i=0; i<m_vecTheBuildingInfo.size(); i++) {
        
        if (m_vecTheBuildingInfo[i].monsterId==buildingId) {
            return &m_vecTheBuildingInfo[i];
        }
        
    }
    
    return NULL;
}




FighterInfo* CPersonalData::refreshSoldierInfoWithInfo(myMonster *theInfo){
    
    
    FighterInfo  *targetInfo = NULL;
    
    for (int i=0; i<m_vecMyFighterInfo.size(); i++) {
        
        if (m_vecMyFighterInfo.at(i)->monsterId==theInfo->monsterId) {
            targetInfo = m_vecMyFighterInfo.at(i);
        }
    }

    if (!targetInfo) {
        targetInfo = new FighterInfo;
    }
    
    targetInfo->theMonsterInfo = CDataManager::getMonsterInfoByMonsterId(theInfo->monsterId);
    targetInfo->skillFrameNum=0;
    targetInfo->level = theInfo->monsterLevel;
    
    CDataManager::CalcFighterInfoWith(targetInfo, theInfo->monsterId, theInfo->monsterLevel, theInfo->rare, theInfo->theType,theInfo->ifFight, theInfo->skillLevel1,theInfo->skillLevel2,theInfo->skillLevel3,theInfo->skillLevel4);

    
    return targetInfo;
}




void CPersonalData::modifyMyMonster(myMonster theMonster)
{
    bool ifExist=false;
    for (int i=0; i<m_vecMyFighterInfo.size(); i++) {
        
        if (m_vecMyFighterInfo[i]->monsterId==theMonster.monsterId) {//如果内存中已经有这个数据，则只修改。不添加
            
            m_vecMyFighterInfo[i]->level = theMonster.monsterLevel;
            
            CDataManager::CalcFighterInfoWith(m_vecMyFighterInfo[i], theMonster.monsterId, theMonster.monsterLevel, theMonster.rare, theMonster.theType,theMonster.ifFight, theMonster.skillLevel1,theMonster.skillLevel2,theMonster.skillLevel3,theMonster.skillLevel4);
            
            
            ifExist = true;
        }

    }
    
    if (!ifExist) {
        
        FighterInfo targetInfo;
        
        //targetInfo.skillId=0;
        targetInfo.skillFrameNum=0;
        targetInfo.level = theMonster.monsterLevel;
        
        CDataManager::CalcFighterInfoWith(&targetInfo, theMonster.monsterId, theMonster.monsterLevel, theMonster.rare, theMonster.theType,theMonster.ifFight, theMonster.skillLevel1,theMonster.skillLevel2,theMonster.skillLevel3,theMonster.skillLevel4);
        
        m_vecMyFighterInfo.push_back(&targetInfo);

    }
    
    
    
}


std::vector<FighterInfo> CPersonalData::getSoliderInfoByType(int soliderType)
{
    std::vector<FighterInfo> tempVec;
    for (int i = 0; i < m_vecMyFighterInfo.size(); i++) {
        if((soliderType == m_vecMyFighterInfo.at(i)->monsterType) && (m_vecMyFighterInfo.at(i)->rare > 0))
        {
            tempVec.push_back(*m_vecMyFighterInfo.at(i));
        }
    }
    return tempVec;
}



FighterInfo* CPersonalData::getTeamSolderByType(int soldierType){
    
    
    for (int i=0; i<m_vecMyTeamInfo.size(); i++) {
        
        if (m_vecMyTeamInfo[i]->monsterType == soldierType) {
            return m_vecMyTeamInfo[i];
        }
    
    }
    
    return NULL;
}

void CPersonalData::setJumpInfoNull()
{
    m_jumpInfo.type = 0;
    m_jumpInfo.monsterId = 0;
    m_jumpInfo.skillId = 0;
}






