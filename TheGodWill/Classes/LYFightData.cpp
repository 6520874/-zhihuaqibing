//
//  LYFightData.cpp
//  TheGodWill
//
//  Created by user on 14/12/1.
//
//

#include "LYFightData.h"
#include "DataManager.h"

#include "LYBaseCity.h"
#include "LYBaseHero.h"

#include "DBMgr.h"
#include "PublicDefine.h"

USING_NS_CC;



CFightData* CFightData::m_singleton = NULL;//m_singleton指向空指针


CFightData* CFightData::getInstance(){
    
    //判断m_singleton是否已被实例化， 如果没有，将之实例
    
    
    if(nullptr == m_singleton)
    {
        m_singleton = new CFightData();//
    }
    
    return m_singleton;
    
}

CFightData::CFightData(){
    
    
    m_bActiveToushi = false;
    
    m_iNowEnergy = m_iMaxEnergy =0;
    
    m_iNowEnergy_Enemy = m_iMaxEnergy_Enemy = 0;
    
    m_pSmallMapLayer = NULL;
    
    m_pTheHero = NULL;
    
    m_pTheFightScene = NULL;
    

    m_ptheEnemyCity = NULL;
    m_iMaxMonsterNum = 8;//暂时测试用
    m_iNowMonsterNum = 0;
    
    m_iAddCoin = 0;
    m_pBigBoss =NULL;
    
    m_starInfoVec.clear();
    
    
    m_iBronPathX=0;
    
    //初始化塔防模式节点
    m_iMaxPathX = (END3DPOINT +ONESIDE -(Start3DPOINT-ONESIDE*0.5)) /BUILDING_POS_OFFSET_X;
//    int minX = 0;
    
    m_iMaxPathY = -(ENDZPOINT -STARTZPOINT)/BUILDING_POS_OFFSET_Y;
//    int minY = 0;
    
    m_iNowTalkTime = 0;
    
    m_iNowWave=0;
    
    
    
    m_iBornTime = 60;
    m_iNowMidLevelId = 1;
    
    //m_PvpEnemyInfo->m_iUserId = 0;
    //m_PvpEnemyInfo->m_iLevel = 0;
    
    //怪物跑的终点
    float posY = (ENDZPOINT-STARTZPOINT)*1.0/BUILDING_POS_OFFSET_Y/2;
    int realY = (int(-posY+0.5f));

//    int targetPosY = -realY*BUILDING_POS_OFFSET_Y+STARTZPOINT;
    
    
    pathPos* temp[100][100] = {NULL};
    
    
    //初始化所有路径点
    for (int i=0; i<=abs(m_iMaxPathX) ; i++) {
        
        for (int j=0; j<=abs(m_iMaxPathY) ; j++) {
            
            pathPos* target=new pathPos();
            
            target->thePoint.x = i;
            target->thePoint.y = j;
            
            target->ifActive = true;
            
            target->weight = MAX_PATH_WEIGHT;
            
            
            target->temp = NULL;
            
            target->availible = true;
            
            target->ifHaveOne = false;
            
            //链表连上
            temp[i][j]=target;
            
            if (i>0) {
                target->links.push_back(temp[i-1][j]);
                temp[i-1][j]->links.push_back(target);
            }
            if (j>0) {
                target->links.push_back(temp[i][j-1]);
                temp[i][j-1]->links.push_back(target);
            }
            
            m_vecPos.push_back(target);
            
            
            //确定终点
            if (i==0&&j==realY) {
                theHead=target;
                theHead->weight=0;
            }
            
        }

    }
    
//    CFightManager::preloadShader();   //暂时弃用，切屏幕会引起bug  by ly
    
//设置监听，从后台返回前台时，重新初始化shader。     暂时弃用，切屏幕会引起bug  by ly
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//    Director::getInstance()->getEventDispatcher()->addCustomEventListener(EVENT_RENDERER_RECREATED, [=](EventCustom* event){
//        this->reloadShader();
//    });
#endif
    
}

CFightData::~CFightData(){
    
    
    for (int i=0; i<m_vecPos.size(); i++) {
        
        delete  m_vecPos.at(i);
    }
    
}

void CFightData::addMonster(CBaseFighter *aa,AttackType theType){
    
    FightGroup targetType = aa->getFightGroup();
    
//    if (theType ==eAttackType_min) {
//        m_vecMines.pushBack(aa);
//    }else
    if (theType ==eAttackType_tower
        ||theType==eAttackType_min
        ||theType==eAttackType_minMonster
        ||theType==eAttackType_wall){   //塔和能量泉都将会被攻击
        m_vecTowers.pushBack(aa);
    }else if (theType == eAttackType_city) {
        m_ptheEnemyCity = (CBaseCity*)aa;
    }else if (theType == eAttackType_Trap) {//如果是陷阱，则不做处理，因为陷阱不会被攻击

        m_vecTrapes.pushBack(aa);
        
    }else if (targetType==eFightGroup_Freind) {
        m_vecSoldier.pushBack(aa);
        
//        if(aa->getAttackType()==eAttackType_monster&&!aa->getIfCampMonster()){
//            m_iNowMonsterNum++;
//        }
        
        
    }else if (targetType==eFightGroup_Enemy) {
        m_vecEnemy.pushBack(aa);
    }
}




void CFightData::addSoldier(CBaseFighter* target){
    
    
    
    
    m_vecSoldier.pushBack(target);
}


void CFightData::addEnemy(CBaseFighter* target){
    
    
    m_vecEnemy.pushBack(target);
}

void CFightData::removeAttacker(CBaseFighter *cc){
    
    
    
    m_vecSoldier.eraseObject(cc);
    m_vecEnemy.eraseObject(cc);
    m_vecMines.eraseObject(cc);
    m_vecTowers.eraseObject(cc);
    m_vecTrapes.eraseObject(cc);
    
//    if (cc->getFightGroup()==eFightGroup_Freind) {
//        
////        if (!cc->getIfCampMonster()) {
////            m_iNowMonsterNum--;
////        }
//        
//        m_vecSoldier.eraseObject(cc);
//    }else if(cc->getFightGroup()==eFightGroup_Enemy){
//        m_vecEnemy.eraseObject(cc);
//    }
    
}

void CFightData::addEnergyWith(float number,FightGroup target){
    
    
    if (target==eFightGroup_Freind) {
        m_iNowEnergy+=number;
        m_iNowEnergy = MIN(m_iNowEnergy, m_iMaxEnergy);
        m_iNowEnergy = MAX(m_iNowEnergy, 0);
    }else if (target==eFightGroup_Enemy) {
        m_iNowEnergy_Enemy+=number;
        m_iNowEnergy_Enemy = MIN(m_iNowEnergy_Enemy, m_iMaxEnergy_Enemy);
        m_iNowEnergy_Enemy = MAX(m_iNowEnergy_Enemy, 0);
    }
    
    
    
}



cocos2d::Vector<CBaseFighter*> CFightData::getSoldierById(int theId){
    
    Vector<CBaseFighter*>target ;
    
    for (int i =0; i<m_vecSoldier.size(); i++) {
        
        CBaseFighter* targetFight = m_vecSoldier.at(i);
        
        if (targetFight->getTheInfo()->monsterId==theId) {
            target.pushBack(targetFight);
        }

    }

    return target;
}


cocos2d::Vector<CBaseFighter*> CFightData::getFightMonstersByGroup(FightGroup theGroup,int aliveState){
    
    Vector<CBaseFighter*>target;
    
    
    if (theGroup==eFightGroup_Freind) {
        target = m_vecEnemy;
        
        
        //获取建筑中的敌对
        
        
    }else if (theGroup==eFightGroup_Enemy){
        target = m_vecSoldier;
    }else if (theGroup==eFightGroup_midFight){
        target.pushBack(m_vecSoldier);
        target.pushBack(m_vecEnemy);
    }
    
    
    
    if (aliveState!=0) {
        
        for (int i=0; i<target.size(); ) {
            
            CBaseFighter* theTemp = target.at(i);
            
            
            
            
            
            
            if (theTemp->getIfDied()&&aliveState==1) {
                target.eraseObject(theTemp);
            }else if (!theTemp->getIfDied()&&aliveState==-1){
                target.eraseObject(theTemp);
                
            }else{
                i++;
            }
            
//            CCLOG("%zd",target.size());

        }

    }
    

    return target;
}

cocos2d::Vector<CBaseFighter*> CFightData::getFriendMonstersByGroup(FightGroup theGroup,int aliveState){
    
    Vector<CBaseFighter*>target;
    
    if (theGroup==eFightGroup_Freind) {
        target = m_vecSoldier;
    }else if(theGroup==eFightGroup_Enemy){
        target = m_vecEnemy;
    }
    
    if (aliveState!=0) {
        
        for (int i=0; i<target.size(); ) {
            
            CBaseFighter* theTemp = target.at(i);
            
            
            if (theTemp->getIfDied()&&aliveState==1) {
                target.eraseObject(theTemp);
            }else if (!theTemp->getIfDied()&&aliveState==-1){
                target.eraseObject(theTemp);
                
            }else if (theTemp->getAttackType()==eAttackType_hero){//现在hero时建筑了，所以不参与怪物的获取方式。  因为怪物是要获取建筑来攻击的，所以不再获取敌对怪物时去掉hero
                target.eraseObject(theTemp);
                
            }else{
                i++;
            }
            
        }
        
    }

    
    
    return target;
}


cocos2d::Vector<CBaseFighter*>CFightData::getFightTowerByGroup(FightGroup theGroup){
    
    
    Vector<CBaseFighter*>target;
    
    
    for (int i=0; i<m_vecTowers.size(); i++) {
        
        CBaseFighter* theTower = m_vecTowers.at(i);
        
        
        if (theTower->getAttackType()==eAttackType_lifeWater) {//如果是生命之泉则跳过
            continue;
        }
        
        
        if (theGroup==eFightGroup_Freind&& theTower->getFightGroup()==eFightGroup_Enemy) {
            target.pushBack(theTower);
        }else if (theGroup==eFightGroup_Enemy&& theTower->getFightGroup()==eFightGroup_Freind) {
            target.pushBack(theTower);
        }else if ((theGroup!=eFightGroup_midFight&&theGroup!=eFightGroup_midNotFight)&& (theTower->getFightGroup()==eFightGroup_midFight||theTower->getFightGroup()==eFightGroup_midNotFight)) {
            target.pushBack(theTower);
        }

    }
    
    return target;
    
}

cocos2d::Vector<CBaseFighter*>CFightData::getFriendTowerByGroup(FightGroup theGroup){
    
    
    Vector<CBaseFighter*>target;
    
    
    for (int i=0; i<m_vecTowers.size(); i++) {
        
        CBaseFighter* theTower = m_vecTowers.at(i);
        
        
        if (theTower->getAttackType()==eAttackType_lifeWater) {//如果是生命之泉则跳过
            continue;
        }
        
        if (theGroup==theTower->getFightGroup()) {
            target.pushBack(theTower);
        }
        
    }
    
    return target;

}



cocos2d::Vector<CBaseFighter*>CFightData::getFightTargetByGroup(FightGroup theGroup,bool ifFightBuilding,bool ifFightFly){
    
    
    Vector<CBaseFighter*>target = getFightMonstersByGroup(theGroup);
    
    if (ifFightBuilding) {
        Vector<CBaseFighter*>target2 = getFightTowerByGroup(theGroup);
        target.pushBack(target2);
    }
    
    
    if (theGroup==eFightGroup_Freind) {
        
        if (m_ptheEnemyCity) {
            target.pushBack(m_ptheEnemyCity);
        }
    }
    
    
    
    for (Vector<CBaseFighter*>::iterator ite=target.begin(); ite!=target.end(); ) {
        
        if (!ifFightFly&&(*ite)->getTheInfo()->theMonsterInfo->ifFly) {
            target.erase(ite);
        }else{
            ite++;
        }
        
    }
    return target;
    
}


void CFightData::newLevelStart(int levelId){
    
    
    
    
    
    //开始新关卡所做的初始化。
    

    //相关数据清空
    restartLevel();
    
    
    //测试用
//    levelId = 21;
    
    
    
    //相关数据初始化
    if (levelId>=0) {
        
        m_iNowLevelId = levelId;
        
       // memset((void*)&m_stTheEnemyKulou,0,sizeof(m_stTheEnemyKulou));
        //memset((void*)&m_stTheFriendKulou,0,sizeof(m_stTheFriendKulou));
        
        
        initLevelInfo();

        initUserInfo();

        
    }
    
}


void CFightData::newPVPStart(){
    
    
    restartLevel();
    
    initUserInfo();
    
    
}


void CFightData::restartLevel(){
    
    m_iTheLevelCondition = eLevelConditon_error;
    m_pSmallMapLayer = NULL;
    m_pTheHero = NULL;
    m_ptheEnemyCity = NULL;
    m_pTheLevelInfo = NULL;
    m_pTheFightScene = NULL;
    m_pLevelConditionLab = NULL;
    
    m_vecSoldier.clear();
    m_vecEnemy.clear();
    m_vecTowers.clear();
    m_vecMines.clear();
    m_pTheMapInfo.clear();
    theReinforcementInfo.clear();
    
    m_vecTrapes.clear();
    
    m_vecPVPFighterInfo.clear();
    
    m_vecTheEnemyInfo.clear();
    m_fAllFightTime =0;
    m_iNowWave=0;
    m_iWinState =0;
    
    m_iNowMonsterNum = 0;
    
    m_ptheEnemyCity = NULL;
    m_pTheHero = NULL;
    
    m_vecTowerOBB.clear();
    
    _endTarget = NULL;
    m_pBigBoss = NULL;
    
    m_iNowLevelId=0;
    
    m_bFirstSolider = false;
    m_bUseSkill = false;
    m_bStopAddEnemy = false;
    m_bClearSolider = false;
}

void CFightData::setWinState(int temp,CBaseFighter* target){
    
    m_iWinState = temp;
    
    
    _endTarget = target;
}




cocos2d::Vector<CBaseFighter*> CFightData::getAllAttacker(){
    
    
    Vector<CBaseFighter*>target ;
    
    for (int i =0; i<m_vecSoldier.size(); i++) {
        
        CBaseFighter* targetFight = m_vecSoldier.at(i);
        
        target.pushBack(targetFight);

    }
    
    
    for (int i =0; i<m_vecEnemy.size(); i++) {
        
        CBaseFighter* targetFight = m_vecEnemy.at(i);
        
        target.pushBack(targetFight);
        
    }
    
    
    
    
    return target;
    
}


cocos2d::Vector<CBaseFighter*> CFightData::getAllBaseFight(){
    
    
    Vector<CBaseFighter*>target ;
    
    
    target.pushBack(m_vecSoldier);
    
    target.pushBack(m_vecEnemy);
    
    target.pushBack(m_vecTowers);
    
    target.pushBack(m_vecMines);
    
    target.pushBack(m_vecTrapes);
    
    if(m_pTheHero&&m_pTheHero->getReferenceCount()>0){
        target.pushBack(m_pTheHero);
    }
    
    
    if (m_ptheEnemyCity) {
        target.pushBack(m_ptheEnemyCity);
    }
    
    
    
    
    return target;
}



float CFightData::addTheFightTime(float time){
    
    m_fAllFightTime+=time;
    
    addTheFightConditon(eLevelCondition_defense, -time);
    addTheFightConditon(eLevelCondition_fastKill, -time);
    
    return m_fAllFightTime;
}



void CFightData::addTimeEnergy(float time){
    
    if(m_bUseSkill || m_bStopAddEnemy)//施放技能的时候能量条不增加
    {   
        return;
    }
    //换算增长的能量
    float myEnergy = m_iAddEnergy*time;
    
    float enemyEnergy = m_iAddEnergy_Enemy*time;
    
    addEnergyWith(myEnergy,eFightGroup_Freind);
    
    addEnergyWith(enemyEnergy,eFightGroup_Enemy);

}

void CFightData::addEnergySpeedWith(float number, FightGroup target){

    
    if (target==eFightGroup_Freind) {
        m_iAddEnergy+=number;
    }else if (target==eFightGroup_Enemy) {
        m_iAddEnergy_Enemy+=number;
    }
    
    
}


void CFightData::initPvpZhaohuanKulou()
{

    //初始化我方召唤骷髅信息
    
    
    memset((void*)&m_stTheEnemyKulou,0,sizeof(m_stTheEnemyKulou));
    memset((void*)&m_stTheFriendKulou,0,sizeof(m_stTheFriendKulou));
    
    
    reinforcementInfo* temp = NULL;
     
     theReinforcementInfo = CDataManager::getReinforceMentInfoByLevel(m_iNowLevelId);
     for (int i=0; i<theReinforcementInfo.size(); i++)
     {
     
     if(theReinforcementInfo[i]->monsterId == 11)
     {
         temp = theReinforcementInfo.at(i);
         CDataManager::CalcFighterInfoWith(&temp->theMonsterInfo, temp->monsterId, temp->monsterLevel,4,0,false,20,20,20,20);
         break;
     }
     }
    
    
    
    std::vector<FighterInfo*> pAllTeamSoldier = CPersonalData::getInstance()->m_vecMyTeamInfo;
    
    
    bool bHave = false;
    
    for(int i=0;i<pAllTeamSoldier.size();i++)
    {
        
        if(pAllTeamSoldier[i]->monsterId==11)
        {
            bHave = true;
            break;
        }
    }
    
    if(!bHave&&temp)
    {
        
        pAllTeamSoldier.push_back(&temp->theMonsterInfo);
        
    }
    
    
    for(int i=0;i<pAllTeamSoldier.size();i++)
    {
        if(m_stTheFriendKulou.monsterId!=0)
        {
            break;
        }
        else
        {
            //遍历技能
            for(int j=0;j<pAllTeamSoldier[i]->theFighterSkillInfo.size();j++)
            {
                if(pAllTeamSoldier[i]->theFighterSkillInfo[j].theSkillInfo->skillId== eAllSkills_silinqiyue)
                {
                    
                    
                    int a[4] = {0};
                    
                    for(int k = 0;k<pAllTeamSoldier[i]->theFighterSkillInfo.size();k++)
                    {
                        a[k] = pAllTeamSoldier[i]->theFighterSkillInfo[k].skillLevel;
                    }
                    
                    CDataManager::CalcFighterInfoWith(&m_stTheFriendKulou,3, pAllTeamSoldier[i]->level,pAllTeamSoldier[i]->theType,
                                                      pAllTeamSoldier[i]->ifFight,a[0],a[1],a[2],a[3]);
                    break;
                    
                }
                
            }
        }
    }
    
    //初始化敌方召唤骷髅信息
    
    std::vector<FighterInfo> pAllEnemySoldier ;
    
    if(CFightData::getInstance()->getFightType() != FIGHT_PVP_TYPE)
    {
      pAllEnemySoldier = *CFightData::getInstance()->getTheMonsterInfo();
    }
    else
    {
          pAllEnemySoldier = m_vecPVPFighterInfo;
        
    }
    
    
    if(m_stTheBossInfo.monsterId>0)
    {
        pAllEnemySoldier.push_back(m_stTheBossInfo);
    }
    bHave = false;
    
    for(int i=0;i<pAllEnemySoldier.size();i++)
    {
        
        if(pAllEnemySoldier[i].monsterId==11)
        {
            bHave = true;
            break;
        }
    }
    
    if(!bHave&&temp)
    {
        
        pAllEnemySoldier.push_back(temp->theMonsterInfo);
        
    }
    
    
    for(int i=0;i<pAllEnemySoldier.size();i++)
    {
        if(m_stTheEnemyKulou.monsterId!=0)
        {
            break;
        }
        else
        {
            //遍历技能
            for(int j=0;j<pAllEnemySoldier[i].theFighterSkillInfo.size();j++)
            {
                if(pAllEnemySoldier[i].theFighterSkillInfo[j].theSkillInfo->skillId== eAllSkills_silinqiyue)
                {
                    
                    int a[4] = {0};
                    
                    for(int k = 0;k<pAllEnemySoldier[i].theFighterSkillInfo.size();k++)
                    {
                        a[k] = pAllEnemySoldier[i].theFighterSkillInfo[k].skillLevel;
                    }
                    
                    CDataManager::CalcFighterInfoWith(&m_stTheEnemyKulou,3,  pAllEnemySoldier[i].level, pAllEnemySoldier[i].theType,
                                                      pAllEnemySoldier[i].ifFight,a[0],a[1],a[2],a[3]);
                    
                    break;
                }
            }
        }
    }

    
}

void CFightData::initZhaohuanKulou(reinforcementInfo* temp)
{

    //初始化我方召唤骷髅信息

    
    memset((void*)&m_stTheEnemyKulou,0,sizeof(m_stTheEnemyKulou));
    memset((void*)&m_stTheFriendKulou,0,sizeof(m_stTheFriendKulou));
    
    
    
    std::vector<FighterInfo*> pAllTeamSoldier = CPersonalData::getInstance()->m_vecMyTeamInfo;
    

    bool bHave = false;
    
    for(int i=0;i<pAllTeamSoldier.size();i++)
    {

        if(pAllTeamSoldier[i]->monsterId==11)
        {
            bHave = true;
            break;
        }
    }
    
    if(!bHave&&temp)
    {
        
        pAllTeamSoldier.push_back(&temp->theMonsterInfo);
        
    }
    
    
    for(int i=0;i<pAllTeamSoldier.size();i++)
    {
        if(m_stTheFriendKulou.monsterId!=0)
        {
            break;
        }
        else
        {
            //遍历技能
            for(int j=0;j<pAllTeamSoldier[i]->theFighterSkillInfo.size();j++)
            {
                if(pAllTeamSoldier[i]->theFighterSkillInfo[j].theSkillInfo->skillId== eAllSkills_silinqiyue)
                {
                    
                    
                    int a[4] = {0};
                    
                    for(int k = 0;k<pAllTeamSoldier[i]->theFighterSkillInfo.size();k++)
                    {
                        a[k] = pAllTeamSoldier[i]->theFighterSkillInfo[k].skillLevel;
                    }
                    
                    CDataManager::CalcFighterInfoWith(&m_stTheFriendKulou,3, pAllTeamSoldier[i]->level,pAllTeamSoldier[i]->theType,
                                                      pAllTeamSoldier[i]->ifFight,a[0],a[1],a[2],a[3]);
                    break;
                    
                }
                
            }
        }
    }
    
    //初始化敌方召唤骷髅信息
    

    
    std::vector<FighterInfo> pAllEnemySoldier = *CFightData::getInstance()->getTheMonsterInfo();
    
    pAllEnemySoldier.push_back(m_stTheBossInfo);
    
    bHave = false;
    
    for(int i=0;i<pAllEnemySoldier.size();i++)
    {
        
        if(pAllEnemySoldier[i].monsterId==11)
        {
            bHave = true;
            break;
        }
    }
    
    if(!bHave&&temp)
    {
        
        pAllEnemySoldier.push_back(temp->theMonsterInfo);
        
    }
    
    
    for(int i=0;i<pAllEnemySoldier.size();i++)
    {
        if(m_stTheEnemyKulou.monsterId!=0)
        {
            break;
        }
        else
        {
            //遍历技能
            for(int j=0;j<pAllEnemySoldier[i].theFighterSkillInfo.size();j++)
            {
                if(pAllEnemySoldier[i].theFighterSkillInfo[j].theSkillInfo->skillId== eAllSkills_silinqiyue)
                {
                    
                    int a[4] = {0};
                    
                    for(int k = 0;k<pAllEnemySoldier[i].theFighterSkillInfo.size();k++)
                    {
                        a[k] = pAllEnemySoldier[i].theFighterSkillInfo[k].skillLevel;
                    }
                    
                    CDataManager::CalcFighterInfoWith(&m_stTheEnemyKulou,3,  pAllEnemySoldier[i].level, pAllEnemySoldier[i].theType,
                                                      pAllEnemySoldier[i].ifFight,a[0],a[1],a[2],a[3]);
                    
                    break;
                }
            }
        }
    }
    

}



void CFightData::initLevelInfo(){
    
    m_pTheLevelInfo = CDataManager::getTheLevelInfoWithId(m_iNowLevelId);
    
    m_pTheMapInfo = CDataManager::getTheMapInfoWithId(m_pTheLevelInfo->mapId);
    
    //初始化怪物信息
    std::vector<int> theGroup = m_pTheLevelInfo->monsterGroup;
    
    m_vecTheEnemyInfo.clear();
    
    
    //怪物技能前两个普通模式开放，后两个会在精英模式开放
    int monsterSkillLevel1 = m_pTheLevelInfo->monsterSkillLevel;
    int monsterSkillLevel2 = m_pTheLevelInfo->monsterSkillLevel;
    
    if (CPersonalData::getInstance()->getlevelMode()==0) {//普通模式，后两个技能等级为0
        monsterSkillLevel2=0;
    }
    
    
    
    for (int i=0; i<theGroup.size(); i++) {//普通怪物信息初始化
        
        FighterInfo theMonsterinf;
        CDataManager::CalcFighterInfoWith(&theMonsterinf, theGroup[i], m_pTheLevelInfo->monsterLevel,1,0,false,monsterSkillLevel1,monsterSkillLevel1,monsterSkillLevel2,monsterSkillLevel2);
        
        //如果是精英模式，血量和攻击力提升
        if (CPersonalData::getInstance()->getlevelMode()==1) {
            theMonsterinf.maxHP = theMonsterinf.maxHP*1.5f;
            theMonsterinf.attk = theMonsterinf.attk*1.5f;
        }
        
        m_vecTheEnemyInfo.push_back(theMonsterinf);
    }
    
    if (m_pTheLevelInfo->bossId!=0) {//boss信息初始化
        
        CDataManager::CalcFighterInfoWith(&m_stTheBossInfo, m_pTheLevelInfo->bossId, m_pTheLevelInfo->monsterLevel,1,0,false,monsterSkillLevel1,monsterSkillLevel1,monsterSkillLevel2,monsterSkillLevel2);
        

    }
    
    
   
    
    
    //初始化守护者信息
    int HeroLevel = CDBMgr::getInstance()->GetHeroLvWithId(PROTECTOR_ID);
    vector<heroSkill*> temp = CDBMgr::getInstance()->getHeroSkillByMonsterId(PROTECTOR_ID);
    
    int skillLevel[4]={0};
    
    for (int i=0; i<temp.size(); i++) {
        skillLevel[i]=CDBMgr::getInstance()->GetHeroSkillLvById(temp.at(i)->heroSkillId);
    }
    
    CDataManager::CalcFighterInfoWith(&m_stTheProtector, PROTECTOR_ID, HeroLevel,5,0,false,skillLevel[0],skillLevel[1],skillLevel[2],skillLevel[3]);//保护者为5星技能全满
    
    
    //初始化怪物主城信息
    CDataManager::CalcFighterInfoWith(&m_stTheEnemyCityInfo, CITY_MONSTER_ID, m_pTheLevelInfo->cityLevel);
    
    //初始化我方主城信息
    CDataManager::CalcFighterInfoWith(&m_stTheFriendCity, CITY_FREIND_ID, m_pTheLevelInfo->cityLevel);
    
    
    
    
    
    
    
    
    
    
    //初始化关卡条件
    m_sTheLevelConditionDes = m_pTheLevelInfo->levelConditionDes;
    
    m_iTheLevelCondition = (LevelCondition)m_pTheLevelInfo->levelCondition;

    m_iLevelProgress = m_pTheLevelInfo->conditionNum;
    
    
    initTheFightConditon();
    
    
    //如果是护送关卡，则初始化护送的怪物,怪物等级与关卡中怪物等级同级
    if (m_iTheLevelCondition ==eLevelCondition_protect) {
        CDataManager::CalcFighterInfoWith(&m_stTheBeProtected, m_pTheLevelInfo->conditionNum, m_pTheLevelInfo->monsterLevel,5,0,false,20,20,20,20);//护送怪物为5星技能全满
        m_stTheBeProtected.walkSpeed = m_stTheBeProtected.walkSpeed*0.5f;//护送怪移动速度都低
    }
    
    
    //如果是boss关卡，则初始化boss
    if(m_iTheLevelCondition==eLevelCondition_killBoss){
        CDataManager::CalcFighterInfoWith(&m_stTheLevelBoss, m_pTheLevelInfo->conditionNum, m_pTheLevelInfo->monsterLevel,5,0,false,20,20,20,20);//boss怪物为5星技能全满
        m_stTheLevelBoss.walkSpeed = m_stTheLevelBoss.walkSpeed*0.5f;//boss移动速度都低
        m_stTheLevelBoss.maxHP= m_stTheLevelBoss.maxHP*40;//boss血量翻40倍
//        m_stTheLevelBoss.attackRang += 20;//因为体型变大，所以攻击距离也需要变长
        
    }

    
    //初始化建筑物
    std::vector<mapInfo*>* TheMapInfo = &m_pTheMapInfo;//根据地图信息初始化
    
    m_vecTheBuilding.clear();
    for (std::vector<mapInfo*>::iterator ite = TheMapInfo->begin(); ite!=TheMapInfo->end(); ite++) {
        
        mapFightInfo temp;
        
        temp.TheMapInfo=*ite;
        
        CDataManager::CalcFighterInfoWith(&temp.theFightInfo, (*ite)->monsterId, m_pTheLevelInfo->monsterLevel, 1, 0,false,1,1,1,1);
//        addMonsterWithIdAndGroupAndPosition(&temp, (*ite)->buildingGroup, (*ite)->pos,(*ite)->buildingType);
        m_vecTheBuilding.push_back(temp);
        
    }
    
    
    
    /*
     塔防模式相关初始化
     */
    m_iNowLife = 10;
    m_iAllStone = 280;
    m_iNowRound = 0;
    
    
    //如果是塔防模式，则根据当前怪物等级来给力
    
    if (CFightData::getInstance()->getTheLevelCondition()==eLevelCondition_TowerDefense) {
        
        //怪物暂时写死。
        int monsterid[10] = {11001,11002,11003,11004,11005,11006,11007,11008,11009,11010};
        
        for (int i=0; i<10; i++) {
            
            FighterInfo theMonsterinf;
            CDataManager::CalcFighterInfoWith(&theMonsterinf, monsterid[i], 1,1,0,false);
            
            m_vecTheTDMonsterInfo.push_back(theMonsterinf);

        }
        
    }
    
    
    
    //初始化援军信息
    
    reinforcementInfo* pWanlinFashi = NULL;
    
    theReinforcementInfo = CDataManager::getReinforceMentInfoByLevel(m_iNowLevelId);
    for (int i=0; i<theReinforcementInfo.size(); i++) {
        
        reinforcementInfo* temp = theReinforcementInfo.at(i);
        
        if(temp->monsterId == 11)
        {
            pWanlinFashi = temp;
            
        }
        CDataManager::CalcFighterInfoWith(&temp->theMonsterInfo, temp->monsterId, temp->monsterLevel,2,0,false,20,20,20,20);
    }
    
    
    //初始化我方召唤骷髅信息

    initZhaohuanKulou(pWanlinFashi);

}

void CFightData::initUserInfo(){
    
    if (m_pTheLevelInfo) {
        m_iAddEnergy_Enemy = m_pTheLevelInfo->energySpeed;
    }
    
    
    m_iMaxEnergy_Enemy = 999;//因为怪有能量就造兵，因此此处可以无视这个值。
    
    m_iNowEnergy_Enemy = 0;
    
    //初始化玩家相关数据
    m_iMaxEnergy = CDBMgr::getInstance()->getAffectNum(CDBMgr::eAffectType::eEnergyType, 2);
    m_iAddEnergy = CDBMgr::getInstance()->getAffectNum(CDBMgr::eAffectType::eEnergyType, 3);
    m_iNowEnergy = m_iMaxEnergy;
    
    m_iMaxMonsterNum = CDBMgr::getInstance()->getAffectNum(CDBMgr::eAffectType::eEnergyType, 1);
    
    
}




FighterInfo* CFightData::getTheCallKulouFriend()
{

    return &m_stTheFriendKulou;
}



FighterInfo* CFightData::getTheCallKulouEnemy()
{
    
    return &m_stTheEnemyKulou;
}

void CFightData::initTheFightConditon(){
    
    
    
    switch (m_iTheLevelCondition) {
        case eLevelCondition_destroyCity:
        case eLevelCondition_protect:
        {
            m_iLevelProgress=0;
            break;
        }
        case eLevelCondition_kill:
        {
            m_iLevelProgress=0;
            break;
        }
        case eLevelCondition_defense:
        case eLevelCondition_fastKill:
        {
            m_iLevelProgress=m_pTheLevelInfo->conditionNum;
            break;
        }
            
        default:
            break;
    }

}

void CFightData::addTheFightConditon(LevelCondition theType, float number,CBaseFighter* target){
    
    if (theType!=m_iTheLevelCondition) {//如果增加的不是当前的条件，则返回
        return;
    }
    
    //如果已经胜利或者失败，则返回
    if (m_iWinState!=0) {
        return;
    }
    
    if((m_bClearSolider || m_bUseSkill) && (m_iTheLevelCondition == eLevelCondition_defense))
    {
        return;
    }
    m_iLevelProgress+=number;
    
    m_iLevelProgress = MAX(m_iLevelProgress, 0);
    
    
    switch(m_iTheLevelCondition){
        
        case eLevelCondition_destroyCity:
            m_iWinState =1;
            _endTarget = target;
            return;
            
        case eLevelCondition_protect:
        {
            refreshTheFightCondition();
            
            if (m_iLevelProgress<0) {
                m_iWinState=-1;
                _endTarget = target;
            }else if (m_iLevelProgress>0){
                m_iWinState=1;
                _endTarget = target;
            }
        }
            
        case eLevelCondition_kill:
        {
            if (m_iLevelProgress>m_pTheLevelInfo->conditionNum) {
                m_iWinState =1;
                _endTarget = target;
            }else{
                refreshTheFightCondition();
            }
            
            
            break;
        }
        case eLevelCondition_defense:
        {
            refreshTheFightCondition();
            if (m_iLevelProgress<=0&&m_iWinState!=-1) {
                m_iWinState =1;
            }
            break;
        }
            
        case eLevelCondition_fastKill:
        {
            refreshTheFightCondition();
            if (m_iLevelProgress<=0&&m_iWinState!=1) {
                m_iWinState =-1;
            }
            break;
        }
            
        
        default:
            break;
        
        
    }
    
    if (m_iWinState==1) {
        CCLOG("gameWin");
    }else if (m_iWinState==-1){
        CCLOG("gameLose");
    }
    
   
    
    
}



void CFightData::refreshTheFightCondition(){
    
    
    __String* tempStr;
    
    m_sTheLevelProgress = "";
    
    switch (m_iTheLevelCondition) {
        case eLevelCondition_destroyCity:
        {
            m_sTheLevelProgress = "";
            break;
        }
        case eLevelCondition_protect:
        case eLevelCondition_killBoss:
        {
            m_sTheLevelProgress = "";
            break;
        }
        case eLevelCondition_kill:
        {
            tempStr = __String::createWithFormat("%.0f/%d波",m_iLevelProgress,m_pTheLevelInfo->conditionNum);
            m_sTheLevelProgress = tempStr->getCString();
            break;  
        }
        case eLevelCondition_defense:
        case eLevelCondition_fastKill:
        {
            tempStr = __String::createWithFormat("%.01f秒",m_iLevelProgress);
            m_sTheLevelProgress = tempStr->getCString();
            break;
        }
            
//        case eLevelCondition_TowerDefense:
//        {
//            tempStr = __String::createWithFormat("剩余%d关",(int)m_iLevelProgress);
//            m_sTheLevelProgress = tempStr->getCString();
//            break;
//        }
            
           
        default:
            break;
    }
    
    
    
    if(m_pLevelConditionLab)
    {
        m_pLevelConditionLab->setString(m_sTheLevelConditionDes+" "+m_sTheLevelProgress);
    }
    

}


CBaseFighter* CFightData::getAttackerByGroupAndCondition(FightGroup theGroup, ChooseCondition theCondition, int theChooseGroup,CBaseFighter* attacker){
    
    cocos2d::Vector<CBaseFighter*> targetVec;
    
    CBaseFighter* target = NULL;
    
    
    
    
    //先根据阵营选
    targetVec = getAttackerByGroupCondition(theGroup, theChooseGroup);
    
//    if (theChooseGroup==0) {//如果是获得友军
//        
//        targetVec = getFriendMonstersByGroup(theGroup);
//        
//    }else if(theChooseGroup==1){//如果是获得敌军
//        
//        targetVec = getFightMonstersByGroup(theGroup);
//        
//    }
    
    
    //再根据条件选
    
    if (theCondition==eChooseCondition_random) {
        if (CFightData::getInstance()->getTheFightScene()&&CFightData::getInstance()->getTheFightScene()->gettheFightSceneType()==eFightSceneType_PVP) { //pvp时，不随机
            theCondition=eChooseCondition_minDist;
        }
    }

    
    switch (theCondition) {
        case eChooseCondition_random:
        {
            
            int randomId = rand()%targetVec.size();
            
            
            target = targetVec.at(randomId);
            
            break;
        }
        case eChooseCondition_minHp:
        {
            
            int minHp =999999;
            
            for ( int i=0 ; i<targetVec.size(); i++) {
                
                CBaseFighter* temp = targetVec.at(i);
                
                
                if (temp->getNowHP()<minHp) {
                    
                    minHp =temp->getNowHP();
                    target = temp;
                    
                }

            }

            break;
        }
        case eChooseCondition_maxHp:
        {
            
            int maxHp =0;
            
            for ( int i=0 ; i<targetVec.size(); i++) {
                
                CBaseFighter* temp = targetVec.at(i);
                
                if (temp->getNowHP()>maxHp) {
                    
                    maxHp =temp->getNowHP();
                    target = temp;
                    
                }
                
            }
            
            break;
        }
        case eChooseCondition_minDist:
        {
            
            float minDist =999999;
            
            for ( int i=0 ; i<targetVec.size(); i++) {
                
                CBaseFighter* temp = targetVec.at(i);
                
                if (attacker&&temp ==attacker) {//如果是自己则返回
                    continue;
                }
                
                
                float tempDist = temp->getFightPosition().distance(attacker->getFightPosition());
                
                if (tempDist<minDist) {
                    
                    minDist =tempDist;
                    target = temp;
                    
                }
                
            }
            
            break;
        }
            
        case eChooseCondition_cure:
        {
            
            //治疗规则就是，挑选1.5倍攻击范围内的，随机一个残血目标，没有则随便治疗自己
            
            float minDist =attacker->getTheInfo()->attackRang*1.5;
            
            for ( int i=0 ; i<targetVec.size(); i++) {
                
                CBaseFighter* temp = targetVec.at(i);
                
                if (attacker&&temp ==attacker) {//如果是自己则返回
                    continue;
                }

                float tempDist = temp->getFightPosition().distance(attacker->getFightPosition())-attacker->getNowRadius();
                
                if (tempDist<minDist&&temp->getNowHP()<temp->getMaxHP()) {

                    target = temp;
                    break;
                    
                }
                
            }
            
            if (target==NULL) {
                target = attacker;
            }
            
            break;
        }

            
            
            
            
            
            

        default:
            break;
    }
    
    
    
    return target;
}

Vector<CBaseFighter*> CFightData::getAttackerByGroupCondition(FightGroup theGroup, int theChooseGroup){
    
    Vector<CBaseFighter*> targetVec;
    
    //先根据阵营选
    if (theChooseGroup==0) {//如果是获得友军
        
        targetVec = getFriendMonstersByGroup(theGroup);
        
    }else if(theChooseGroup==1){//如果是获得敌军
        
        targetVec = getFightMonstersByGroup(theGroup);
        
        targetVec.pushBack(getFightTowerByGroup(theGroup));
        
        if(theGroup==0&&m_ptheEnemyCity){  //如果是友军的话，获得敌军的基地
            targetVec.pushBack(m_ptheEnemyCity);
        }
        
        
    }

    
    
    return targetVec;
}

void CFightData::setFightData(int starType, int monsterId)
{
    bool isExisted = false;
    for(int i = 0;i < CFightData::getInstance()->m_starInfoVec.size(); i++)
    {
        if((CFightData::getInstance()->m_starInfoVec.at(i).starType == starType)
           && (CFightData::getInstance()->m_starInfoVec.at(i).monsterId == monsterId))
        {
            isExisted = true;
            CFightData::getInstance()->m_starInfoVec.at(i).num+=1;
            break;
        }
    }
    if(!isExisted)
    {
        levelStarInfo info;
        info.starType = starType;
        info.monsterId = monsterId;
        info.num = 1;
        CFightData::getInstance()->m_starInfoVec.push_back(info);
    }
}

void CFightData::changeTowerObb(bool ifAdd, CBaseFighter *theTower){
    
    
    if (ifAdd) {
        Vec3 p = theTower->getPosition3D()+theTower->gettheSprite()->getPosition3D();
        
        
        float height = theTower->gettheSprite()->getBoundingBox().size.height;
        
        p.y = p.y+height*0.5f;
        
        Vec3 extents = Vec3(height*0.35f, height*0.35f, height*0.35f);
        AABB aabb(-extents, extents);
        auto obb = OBB(aabb);
        obb._center = p;
        obbInfo pInfo;
        pInfo.obb = obb;
        pInfo.theFighter = theTower;
        m_vecTowerOBB.push_back(pInfo);
    }else{
        
        for (std::vector<obbInfo>::iterator ite = m_vecTowerOBB.begin(); ite!=m_vecTowerOBB.end(); ) {
            
            if ((*ite).theFighter==theTower) {
                m_vecTowerOBB.erase(ite);
            }else{
                ite++;
            }

        }
        
    }
    
}

bool CFightData::addPos(cocos2d::Vec2 thePoint){
    
    
    pathPos* target = findPos(thePoint);
    
    target->weight=MAX_PATH_WEIGHT;
    target->availible = false;
    target->ifActive = false;
    
    calcAllPathWeight();
    
    return true;
}

void CFightData::removePos(cocos2d::Vec2 thePoint){
    
    pathPos* target = findPos(thePoint);
    
    target->weight=MAX_PATH_WEIGHT;
    target->availible = true;
    target->ifActive = true;
    
    calcAllPathWeight();
    
}





pathPos* CFightData::findPos(cocos2d::Vec2 thePoint){
    
    
    for (int i=0; i<m_vecPos.size(); i++) {
        
        if (m_vecPos.at(i)->thePoint == thePoint) {
            return  m_vecPos.at(i);
        }
    }
    
    return NULL;
}



void CFightData::calcAllPathWeight(bool ifInit){
    
    
    //刷新所有权重前，先把所有权重重置
    
    for (int i=0; i<m_vecPos.size(); i++) {
    
        pathPos* target = m_vecPos.at(i);
        target->weight= MAX_PATH_WEIGHT;
        
        
        if (ifInit) {//如果是初始化，则所有数据清空
            target->availible = true;
            target->temp= NULL;
        }
        
        
        
        
        if (target==theHead) {
            target->weight = 0;
        }
        
        
        if (target->availible) {
            target->ifActive=true;
        }
        
    }

    
    
    calcThePos(theHead);
    
    
    //刷新所有节点的显示,测试用
    
    
//    for (int i=0; i<m_vecPos.size(); i++) {
//        
//        pathPos* target = m_vecPos.at(i);
//        
//        //测试用，显示当前节点权重
//        if (!target->temp) {
//            
//            target->temp= Label::createWithCharMap("fonts/britanic bold.png", 24, 36, '0');
//            target->temp->setColor(Color3B::RED);
//            target->temp->setAnchorPoint(Vec2(0, 0.5));
//            target->temp->setPosition3D(Vec3(target->thePoint.x*BUILDING_POS_OFFSET_X+Start3DPOINT-ONESIDE*0.5,10,-target->thePoint.y*BUILDING_POS_OFFSET_Y+STARTZPOINT));
//            target->temp->setCameraMask((unsigned short )CameraFlag::USER1);
//            getTheFightScene()->addChild(target->temp,11);
//            
//            target->temp->setGlobalZOrder(1000);
//            
//            target->temp->setBlendFunc(cocos2d::BlendFunc::ALPHA_NON_PREMULTIPLIED);
//            //theHurt->enableOutline(Color4B(0,0,0,255));
//            
//            target->temp->setScale(0.5f);
//            
//            
//        }
//        
//        if (target->weight==MAX_PATH_WEIGHT) {
//            target->temp->setVisible(false);
//        }
//        
//        __String* hurtStr = __String::createWithFormat("%d",target->weight);
//        target->temp->setString(hurtStr->getCString());
//        
//    }
    
}



void CFightData::calcThePos(pathPos *target){
    
    
    int childWeight = target->weight+1;
    
    target->ifActive = false;
    

    
    
    
    
    
    //找寻所有节点，给他们的权重加1，看看是否比当前权重要低，要低的话，则激活节点。并且把权重替换过去
    for (int i=0; i<target->links.size(); i++) {
        
        pathPos* temp = target->links.at(i);
        
        if (temp->weight>childWeight&&temp->availible) {
            temp->ifActive = true;
            temp->weight = childWeight;
            
        }else{
            temp->ifActive = false;
        }
        
        if (temp->ifActive) {
            calcThePos(temp);
        }

        
    }
    
//    //所有节点都处理好后，再遍历子节点
//    
//    for (int i=0; i<target->links.size(); i++) {
//        
//        pathPos* temp = target->links.at(i);
//        
//        if (temp->ifActive) {
//            calcThePos(temp);
//        }
//        
//    }
    
}

pathPos* CFightData::findPathPosByPos(Vec2 nowPos){
    
    
    float posX = (nowPos.x-(Start3DPOINT-0.5*ONESIDE))*1.0/BUILDING_POS_OFFSET_X;
    
    float posY = (nowPos.y-STARTZPOINT)*1.0/BUILDING_POS_OFFSET_Y;
    
    
    int realX = int(posX+0.5f);
    
    int realY = int(-posY+0.5f);
    
    return findPos(Vec2(realX,realY));
}



pathPos* CFightData::findNextPath(pathPos* target,bool ifIgnoreBuilding){
    
    std::vector<pathPos*> nextVec;
    
    int minWeight = 999999;
    int minX = 99999;
    for (int i=0; i<target->links.size(); i++) {
        
        pathPos* temp= target->links.at(i);
        
        
        if (ifIgnoreBuilding) {//如果无视建筑物的话
            
            int tempWeight = abs(temp->thePoint.x-theHead->thePoint.x)+abs(temp->thePoint.y-theHead->thePoint.y);
            
            
            if (tempWeight<minWeight) {
                minWeight = tempWeight;
                minX = temp->thePoint.x;
                nextVec.clear();
                nextVec.push_back(temp);
            }else if (tempWeight==minWeight){
                
                //这里希望挑选x轴最小的那一群，这样可以避免卡死，并且表现效果更好
                
                if (temp->thePoint.x< minX) {
                    minX = temp->thePoint.x;
                    nextVec.clear();
                    nextVec.push_back(temp);
                }else if (temp->thePoint.x== minX){
                    nextVec.push_back(temp);
                }
                
            }
  
            
        }else{
            if (temp->weight<minWeight) {
                minWeight = temp->weight;
                minX = temp->thePoint.x;
                
                nextVec.clear();
                
                nextVec.push_back(temp);
                
            }else if (temp->weight==minWeight){
                
                //这里希望挑选x轴最小的那一群，这样可以避免卡死，并且表现效果更好
                
                if (temp->thePoint.x< minX) {
                    minX = temp->thePoint.x;
                    nextVec.clear();
                    nextVec.push_back(temp);
                }else if (temp->thePoint.x== minX){
                    nextVec.push_back(temp);
                }
                
            }
        }

        
    }
    
    //然后在结果数组中遍历
    
    if (nextVec.size()>0) {
        int theRand = rand()%nextVec.size();
        
        return nextVec.at(theRand);
    }
    
    return NULL;
}

Vec3 CFightData::convertPathPosToPos(pathPos* target){
    
    Vec3 targetVec;
    
    targetVec.x = target->thePoint.x*BUILDING_POS_OFFSET_X+Start3DPOINT-0.5*ONESIDE;
    
    targetVec.z = -target->thePoint.y*BUILDING_POS_OFFSET_Y+STARTZPOINT;
    
    return targetVec;
}

void CFightData::updateAllPathPosIdle(){
    
    
    for (int i=0; i<m_vecPos.size(); i++) {
        
        m_vecPos.at(i)->ifHaveOne=false;
        m_vecPos.at(i)->ifNeedVirtual=false;
    }
    
    
    for (int i=0; i<m_vecEnemy.size(); i++) {
        
        CBaseFighter* temp = m_vecEnemy.at(i);
        
        pathPos* target = findPathPosByPos(temp->getFightPosition());
        
        if (!target) {
            continue;
        }
        
        target->ifHaveOne = true;
        
        
        
        for(int i=0;i<=0;i++){
            
            Vec2 coverPos = target->thePoint+Vec2(i, -1);
            
            if (coverPos.x<0||coverPos.x>m_iMaxPathX||coverPos.y<0||coverPos.y>m_iMaxPathY) {
                continue;
            }
            
            pathPos* coverOne = findPos(target->thePoint+Vec2(i, -1));
            
            coverOne->ifNeedVirtual = true;

        }
        
    }

}

void CFightData::addLife(int temp,CBaseFighter* target){
    
    m_iNowLife+= temp;
    
    if (m_iNowLife<=0) {
        m_iNowLife=0;
        CFightData::getInstance()->setWinState(-1,target);
    }
    
    __String* tempStr = __String::createWithFormat("%d",m_iNowLife);
    
    m_pTheFightScene->m_pLifeLable->setString(tempStr->getCString());

}

void CFightData::addRound(int temp){
    
    m_iNowRound+=temp;
    
}

bool CFightData::ifBlockRoad(){
    
    
    
    for (int i=0; i<m_iMaxPathY; i++) {
        
        pathPos* temp = findPos(Vec2(m_iBronPathX,i) );
        
        if (temp->weight>=MAX_PATH_WEIGHT) {
            return true;
        }
        
        
    }
    
    
    return false;

}


void CFightData::setBornPath(float targetX){
    
    
    float posX = (targetX-(Start3DPOINT-0.5*ONESIDE))*1.0/BUILDING_POS_OFFSET_X;
    
    
    
    int realX = int(posX+0.5f);
    
    
    
    m_iBronPathX = realX;
    
}

void CFightData::getNeededPngPathByFightInfo(FighterInfo *target, std::vector<std::string> *targetVec,bool ifModel){
    
    
    if (ifModel) { //模型路径
        
        targetVec->push_back(target->theModelInfo->modelName.getCString());
        
        
    }else{ //贴图路径
        
        std::vector<std::string> * tempVec = &target->theModelInfo->vecAllPng;
        
        for (int i=0; i<tempVec->size(); i++) {
            
            targetVec->push_back(tempVec->at(i));
            
        }
    }
    
    
    

}

std::vector<std::string> CFightData::getNeededPngPath(bool ifModel){
    
    
    std::vector<std::string> targetVec;
    return targetVec;
    
    for (int i=0; i<m_vecTheEnemyInfo.size(); i++) {
        
        getNeededPngPathByFightInfo(&m_vecTheEnemyInfo.at(i),&targetVec,ifModel);
        
    }
    
    for (int i=0; i<m_vecTheBuilding.size(); i++) {
        
        getNeededPngPathByFightInfo(&m_vecTheBuilding.at(i).theFightInfo,&targetVec,ifModel);
        
    }
    
    if (m_pTheLevelInfo->bossId!=0) {
        
        getNeededPngPathByFightInfo(&m_stTheBossInfo,&targetVec,ifModel);
    }
    
    
    getNeededPngPathByFightInfo(&m_stTheEnemyCityInfo, &targetVec,ifModel);
    
    
    if (m_iTheLevelCondition ==eLevelCondition_protect) {
        
        getNeededPngPathByFightInfo(&m_stTheBeProtected, &targetVec,ifModel);
    }
    
    if(m_iTheLevelCondition==eLevelCondition_killBoss){
        
        getNeededPngPathByFightInfo(&m_stTheLevelBoss, &targetVec,ifModel);
    }
    
    getNeededPngPathByFightInfo(&m_stTheProtector, &targetVec,ifModel);
    
    
    
    
    
    
    if (CFightData::getInstance()->getTheLevelCondition()==eLevelCondition_TowerDefense) {
        
        for (int i=0; i<m_vecTheTDMonsterInfo.size(); i++) {
            
            getNeededPngPathByFightInfo(&m_vecTheTDMonsterInfo.at(i),&targetVec,ifModel);
            
        }
    }
    
    
    
    std::vector<FighterInfo*>* soldierVec = &CPersonalData::getInstance()->m_vecMyTeamInfo;
    
    for (int i=0; i<soldierVec->size(); i++) {
        
        getNeededPngPathByFightInfo(soldierVec->at(i),&targetVec,ifModel);
        
    }
    
    
    
    
    return targetVec;
}

FighterInfo* CFightData::getPVPFightInfoById(int monsterID){
    
    
    for (int i=0; i<m_vecPVPFighterInfo.size(); i++) {
        
        FighterInfo* temp = &m_vecPVPFighterInfo.at(i);
        
        if (temp->monsterId==monsterID) {
            return temp;
        }
        
    }
    
    return NULL;
}

CBaseFighter* CFightData::getMonsterWithTrueId(int TrueId){
    
    cocos2d::Vector<CBaseFighter*> allBaseFight =  CFightData::getAllBaseFight();
    
    for (int i=0; i<allBaseFight.size(); i++) {
        
        if (allBaseFight.at(i)->getTrueId()==TrueId) {
            return allBaseFight.at(i);
        }
    }
    
    return NULL;
    
    
}

void CFightData::reloadShader(){
    
//    CFightManager::preloadShader(true);  //暂时弃用，切屏幕会引起bug  by ly
    
    cocos2d::Vector<CBaseFighter*> temp = getAllAttacker();
    
    for (int i =0; i<temp.size(); i++) {
        
        CBaseFighter* target = temp.at(i);
        target->reloadShader();
    }

}




