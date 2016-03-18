//
//  DBMg.cpp
//  PinBallHeroes
//
//  Created by hsx on 14-3-21.
//
//

#include "DBMgr.h"
//#include "DataBase64.h"
#include "commonPropLayer.h"
#include "PhysicalSystem.h"

#include "TDCCTalkingDataGA.h"

#include "PublicDefine.h"
#include "LYPersonalData.h"
#include "btDBOper.h"
#include "NetWorkDataSingle.h"

static CDBMgr *s_SharedDBMgr = nullptr;
CDBMgr::CDBMgr()
{
    theUserInfo=NULL;
    
}
CDBMgr::~CDBMgr()
{
    
}
CDBMgr* CDBMgr::getInstance()
{
    if (!s_SharedDBMgr)
    {
        s_SharedDBMgr = new CDBMgr();
        s_SharedDBMgr->init();
    }
    
    return s_SharedDBMgr;
}




void CDBMgr::init()
{
    
    
    CCLOG("开始数据库初始化");
    
    
    //判断是更新数据库还是建立数据库
    
    
    
    
    std::string fullDBPath = FileUtils::getInstance()->getWritablePath() + "gameDB/gameDB.db";
    
    
    CCLOG("数据路径为:%s",fullDBPath.c_str());
    
    FILE* file = fopen(fullDBPath.c_str(), "r");
    
    
    
    if (file == nullptr) {  //如果没有数据库，则拷贝数据库
        
        CCLOG("目标数据不存在，开始拷贝");
        
        copyDB(eCopDBType_create);
    }else{ //如果有数据库，则判断数据库的版本号，需不需要更新
        
        
        CCLOG("目标数据存在，开始检验数据库版本");
        
        CDBMgr::getInstance()->OpenDB();
        loadUserInfo();
        CDataBaseOper::closeDB();
        
        CCLOG("玩家数据库版本号:%d",theUserInfo->DBVersion);
        CCLOG("目标数据库版本号:%d",CLIENT_DB_VERSION);
        
        if (CLIENT_DB_VERSION>theUserInfo->DBVersion) {   //测试用，看到就把true去掉
            
            CCLOG("玩家数据库版本过低，开始升级");
            
            copyDB(eCopDBType_update);
            
        }
        
    }
    
    
    CCLOG("开始数据库初始化完毕");
    
    
    
    
    loadDB();
    
    
    CCLOG("读取数据库");
    
//    m_bRead = 0;
//    m_totalCardInfVec.clear();
//    m_totalCardEvolutionInfVec.clear();
//    m_totalPointsInfoInfVec.clear();
//    m_totalPropsInfoInfVec.clear();
//    m_totalSkillInfoInfVec.clear();
//    m_totalSkillEvoInfoInfVec.clear();
//    m_totalHeroFragmentInfoInfVec.clear();
//    m_totalMyHeroFragmentInfoInfVec.clear();
//    m_totalExchangeHeroInfoInfVec.clear();
//    m_PropShop.clear();
//    for (int i=0; i<eMaxMonsterMap; i++) {
//        m_vecTotalMonsterMap[i].clear();
//    }
//    m_totalDrawHeroFragmentInfoInfVec.clear();
//    m_vecTotalDownResource.clear();
//    m_totalMapInfoVec.clear();
}

void CDBMgr::copyDB(copyDBType type){
    
    
    //写入数据库
    std::string fullDBPath = FileUtils::getInstance()->getWritablePath() + "gameDB/gameDB.db";
    
    const std::string directoryPath = FileUtils::getInstance()->getWritablePath()+"gameDB";
    
    std::string originDBPath = FileUtils::getInstance()->fullPathForFilename("gameDB/gameDB.db");
    
    
    
    CCLOG("数据库沙盒路径为:%s",originDBPath.c_str());
    
    CCLOG("数据库目标路径为:%s",fullDBPath.c_str());
    
    CCLOG("数据库目标文件夹路径为:%s",directoryPath.c_str());
    
    
    if (type ==eCopDBType_create) {  //如果是创建，则存在就不创建
        
        FILE* file = fopen(fullDBPath.c_str(), "r");
        if (file == nullptr) {
            
            CCLOG("目标没有数据库，开始创建");
            
            //新建一个文件夹
            bool ifSuccess = FileUtils::getInstance()->createDirectory(directoryPath);
            
            if (ifSuccess) {
                CCLOG("创建文件夹成功");
            }else{
                CCLOG("创建文件夹失败");
            }

            
//            ssize_t size;
//            char *data = NULL;
//            
//            data = (char*)FileUtils::getInstance()->getFileData("gameDB/gameDB.db","rb",&size);
//            
//            
//            
//            FILE* openfile = fopen(fullDBPath.c_str(), "wb");
//            if(openfile){
//                
//                CCLOG("打开指针");
//                
//                size_t ret = fwrite(data, 1, strlen(data), openfile);
//                fclose(openfile);
//                if (ret != 0)
//                    log("Writing file to writable path succeed.");
//                
//            }
            
            
            
            
            FILE* openfile = fopen(fullDBPath.c_str(), "wb");
            long size;
            auto data = (char*)FileUtils::getInstance()->getFileData("gameDB/gameDB.db","rb",&size);
            
            CCLOG("数据库拷贝的数据为:%s",data);
            
            size_t ret=fwrite(data, sizeof(char), size , openfile);
            if (ret != 0){
                CCLOG("Writing file to writable path succeed %zu",ret);
            }
            
            CC_SAFE_DELETE_ARRAY(data);
            
            CCLOG("释放数据库数据成功");
            fclose(openfile);
            CCLOG("关闭数据库指针成功");
            
        }else{
            fclose(file);         //LY注：此处为一大坑，如果不判断就删除该文件指针，在ios下不报错，在android下会报错。
            CCLOG("关闭数据库验证用指针成功");
        }
        

        IsRelease();

    }
    
    if (type ==eCopDBType_cover) {  //如果是覆盖，则直接删除，然后写数据
        
        
        //此处一坑，如果不把数据库先删除再写入。那么虽然模拟器是好使的，但是打包却不行
        bool ifSuc = FileUtils::getInstance()->removeFile(fullDBPath);

        if (ifSuc) {
            CCLOG("数据库删除成功");
        }
        
        
        //新建一个文件夹
        FileUtils::getInstance()->createDirectory(directoryPath);
        
        FILE* openfile = fopen(fullDBPath.c_str(), "wb");
        ssize_t size;
        auto data = (char*)FileUtils::getInstance()->getFileData("gameDB/gameDB.db","rb",&size);
        fwrite(data, size, 1, openfile);
        CC_SAFE_DELETE_ARRAY(data);
        fclose(openfile);

        IsRelease();
    }
    
    
    
    
    
    if (type == eCopDBType_update) {   //如果是update,则先把数据库内容读出来,然后拷贝新的数据库，然后再将保存本地的相关数据写入
        
        /*
         拷贝数据到内存
         */
        
        CDBMgr::getInstance()->OpenDB();
        
        //读取用户数据
        loadGuideProgress();
        loadTaskProInfo();
        loadUserMonsterDB();
        loadUserPropDB();
        loadUserInfo();
        loadTaskDB();
        
        //读取辅助信息
        loadUserUpgradeInfo();
        loadSkillDB();
        loadMonsterDB();
        loadHeroSkillInfoDB();
        
        

        CDataBaseOper::closeDB();
        
        
        //删除原数据库
        bool ifSuc = FileUtils::getInstance()->removeFile(fullDBPath);
        if (ifSuc) {
            CCLOG("数据库删除成功");
        }
        
        
        //数据替换
        FileUtils::getInstance()->createDirectory(directoryPath);
        
        FILE* openfile = fopen(fullDBPath.c_str(), "wb");
        ssize_t size;
        auto data = (char*)FileUtils::getInstance()->getFileData("gameDB/gameDB.db","rb",&size);
        fwrite(data, size, 1, openfile);
        CC_SAFE_DELETE_ARRAY(data);
        fclose(openfile);
        
        
        IsRelease();//拷贝数据库后加密  by ly
        
        
        //将内存数据存进数据库
        saveUserAllInfo();
        saveUserGuideInfo();
        saveUserTaskInfo();
        saveUserMonsterInfo();
        saveUserItemsInfo();
        saveUserTaskListInfo();
        
        
        Show::create();

    
        
    }
    

}


void  CDBMgr::IsRelease()
{

 //加密时收动打开
//#ifdef Pro_Release
//    
//    std::string pass = "shanghaicerongwangluo";
//    std::string fullDBPath = FileUtils::getInstance()->getWritablePath() + "/gameDB/gameDB.db";
//    
//    sqlite3 * pDB = NULL;
//    
//    sqlite3_open(fullDBPath.c_str(), &pDB);
//    
//    sqlite3_rekey(pDB,pass.c_str(), pass.length());
//#endif

    
}




//TODO: 打开数据库
void CDBMgr::OpenDB()
{
    std::string fullDBPath = FileUtils::getInstance()->getWritablePath() + "/gameDB/gameDB.db";
    CDataBaseOper::initDB(fullDBPath.c_str());
}


void CDBMgr::loadDB(){
    
    OpenDB();
    
    loadTaskDB();
    loadTaskProInfo();
    loadModelDB();
    loadMonsterDB();
    loadUserMonsterDB();
    
    loadPropDB();
    loadUserPropDB();
    loadUserNameInfo();
    
    loadSkillDB();
    loadItemOpenInfoDB();
    
    loadLevelInfoDB();
    loadSkillEffectDB();
    loadMapInfoDB();
    
    loadGameStoryInnfo();
    loadGuiderInfo();
    loadGuideProgress();
    
    loadUserInfo();
    loadUserUpgradeInfo();
    loadMidLevelInfo();
    loadLevelStarInfo();
    loadChargeInfo();
    loadBuildInfo();
    loadPvpVIdoeInfo();
    loadHeroSkillInfoDB();
    loadBulletDB();
    loadSkillUpgradeInfo();
    loadHeroSkillInfoDB();
    loadEnergyUpgradeInfo();
    loadBuildingUpgradeInfo();
    loadMonsterUpgradeInfo();
    loadMonsterEvolutionInfo();
    
    loadFightTalkingInfo();
    
    loadFightReinforcementInfo();

    
    loadFightBuildingInfo();
    loadHeroSkillUpgradeInfo();
    
    loadTipsInfo();
    loadShop();
    loadUserInfoum();
    loadMessageInfo();
    loadConfigure();
    loadTmpSaveInfo();
    
    loadDrawCardResult();
    
    CDataBaseOper::closeDB();
    
    
    //刷新一下用户数据
    CPersonalData::refreshUserInfo();
    
}



Sprite * CDBMgr::GetShareSpriteByFigtherType(FighterType monsterType)
{
    
    /*
     eFighterType_line=0,//横行兵种 ，均衡行
     eFighterType_triangle,//三角形兵种 ，敏捷型
     eFighterType_circle,//圆形兵种， 远程
     eFighterType_square,//方形兵种，肉盾型
     eFighterType_v,//V形兵种
     */
    
    std::string strImg;
    
    if(monsterType == eFighterType_line)
    {
        strImg = "Sl_heng.png";
    }
    else if(monsterType == eFighterType_triangle)
    {
        strImg = "Sl_Triangle.png";
        
    }
    else if(monsterType == eFighterType_circle)
    {
        strImg = "Sl_Cricle.png";
    }
    else if(monsterType == eFighterType_square)
    {
        strImg = "Sl_Rect.png";
    }
    else if(monsterType == eFighterType_v)
    {
        strImg = "Sl_V.png";
    }
    
    Sprite *pBlackDi = Sprite::createWithSpriteFrameName("Sl_di.png");
    
    //pBlackDi->cocos2d::Node::setScale(0.25);
    
    Size tBlackBg = pBlackDi->getBoundingBox().size;
    
    Sprite *pImg = Sprite::createWithSpriteFrameName(strImg);
    pImg->setPosition(tBlackBg.width*0.5,tBlackBg.height*0.5);
    pBlackDi->addChild(pImg);
    
    return  pBlackDi;
    
}

bool CDBMgr::IsSkillCanLvUp(int iMonsterId,int iSkillId,bool bHero,int iSkillLv)
{
    bool bEnough = true;
    
    SkillUpgradeInfo* pSkillUpgradeInfo = NULL;
    
    heroSkillUpgradeInfo *pHeroSkillUpradeInfo = NULL;
    
    //int iNeedTime = 0;
    
    monsterInfo* pMonsterInfo = CDataManager::getMonsterInfoByMonsterId(iMonsterId);
    
    if(bHero)
    {
        int iLv = CDBMgr::getInstance()->GetHeroSkillLvById(iSkillId);
        
        pHeroSkillUpradeInfo = CDBMgr::getInstance()->GetHeroSkillWithId(iSkillId,iLv);
        CDBMgr::getInstance()->AnalyseSkillUpgradeNeedItem(pHeroSkillUpradeInfo->m_strItems);
    }
    else
    {
        pSkillUpgradeInfo =  CDataManager::getSkillUpgradeInfowithId(pMonsterInfo->monsterType,iSkillLv);
        
        
        //iSkillLv = 0 的bug
        
        CDBMgr::getInstance()->AnalyseSkillUpgradeNeedItem(pSkillUpgradeInfo->m_strItems);
    }
    
    map<int,int> m_pNeedPropmap = CDBMgr::getInstance()->GetSkillUpgradeItemMap();
    
    int iHaveNum1 = 0;
    int iNeedNum1 = 0;
    int iHaveNum2 = 0;
    int iNeedNum2 = 0;
    int iHaveNum3 = 0;
    int iNeedNum3 = 0;
    
    map<int,int>::iterator it = m_pNeedPropmap.begin();
    
    for(int i =0;it!=m_pNeedPropmap.end();it++,i++)
    {
        if(i==0)
        {
           iNeedNum1 =  it->second;
            iHaveNum1 = HavePropNumById(it->first);
        }
        else if(i==1)
        {
            iNeedNum2 =  it->second;
            iHaveNum2 = HavePropNumById(it->first);
        }
        else if(i==2)
        {
            iNeedNum3 =  it->second;
            iHaveNum3 = HavePropNumById(it->first);
        }
    }

    if(iHaveNum1<iNeedNum1)
    {
        bEnough = false;
    }
    
    if(iHaveNum2<iNeedNum2)
    {
        bEnough = false;
    }
    
    if(iHaveNum3<iNeedNum3)
    {
        bEnough = false;
    }
    
    if(bEnough)
    {
        return true;
    }
    else
    {
        return false;
    }
}


void CDBMgr::loadSkillUpgradeInfo()
{
    m_pSkillUpgradeInfo.clear();
    
    sqlite3 *pDB = CDataBaseOper::getDB();
    int iResult;
    char *pErrMsg=NULL;
    char **dbResult;
    int iRow,iColum;
    int index = 0;
    
    iResult = sqlite3_get_table(pDB, "select * from skillUpgradeInfo", &dbResult, &iRow, &iColum, &pErrMsg);
    
    if(SQLITE_OK==iResult)
    {
        index = iColum;
        
        for( int i = 0; i < iRow ;i++ )
        {
            SkillUpgradeInfo* info = new SkillUpgradeInfo;
            
            __String value = dbResult[index++];
            info->m_id = value.intValue();
            
            value = dbResult[index++];
            info->iSkillLevel = value.intValue();
            
            value = dbResult[index++];
            info->m_strItems = value.getCString();
            
            value = dbResult[index++];
            info->m_iNeedTime = value.intValue();
            
            value = dbResult[index++];
            info->m_eMonsterType = (FighterType)value.intValue();
            
            m_pSkillUpgradeInfo.push_back(info);
        }
    }
    
    sqlite3_free_table(dbResult);
}

void CDBMgr::loadHeroSkillUpgradeInfo()
{
    m_pHeroSkillUpgradeInfo.clear();
    
    sqlite3 *pDB = CDataBaseOper::getDB();
    int iResult;
    char *pErrMsg=NULL;
    char **dbResult;
    int iRow,iColum;
    int index = 0;
    
    iResult = sqlite3_get_table(pDB, "select * from heroSkillUpgradeInfo", &dbResult, &iRow, &iColum, &pErrMsg);
    
    if(SQLITE_OK==iResult)
    {
        index = iColum;
        
        for( int i = 0; i < iRow ;i++ )
        {
            heroSkillUpgradeInfo* info = new heroSkillUpgradeInfo;
            
            __String value = dbResult[index++];
            info->m_id = value.intValue();
            
            
            value = dbResult[index++];
            info->m_heroSkillId = value.intValue();

            
            value = dbResult[index++];
            info->m_SkillLevel = value.intValue();
            
            value = dbResult[index++];
            info->m_strItems = value.getCString();
            
            value = dbResult[index++];
            info->m_iNeedTime = value.intValue();
            
            m_pHeroSkillUpgradeInfo.push_back(info);
            
        }
    }
    
    sqlite3_free_table(dbResult);

    
}

void CDBMgr::loadEnergyUpgradeInfo()
{
    m_pEnergyUpgradeInfo.clear();
    
    sqlite3 *pDB = CDataBaseOper::getDB();
    int iResult;
    char *pErrMsg=NULL;
    char **dbResult;
    int iRow,iColum;
    int index = 0;
    
    iResult = sqlite3_get_table(pDB, "select * from energyUpgradeInfo", &dbResult, &iRow, &iColum, &pErrMsg);
    
    if(SQLITE_OK==iResult)
    {
        index = iColum;
        
        for( int i = 0; i < iRow ;i++ )
        {
            energyUpgradeInfo* info = new energyUpgradeInfo;
            
            __String value = dbResult[index++];
            info->m_id = value.intValue();
            
            value = dbResult[index++];
            info->m_iEnergyUpgradeId = value.intValue();
            
            value = dbResult[index++];
            info->m_iUpgradeLevel = value.intValue();
            
            value = dbResult[index++];
            info->m_sName = value.getCString();
            
            value = dbResult[index++];
            info->m_sDesc = value.getCString();
            
            value = dbResult[index++];
            info->m_iCostGold = value.intValue();
            
            value = dbResult[index++];
            info->m_iAffectNum = value.intValue();
            
            value = dbResult[index++];
            info->m_sIco =  value.getCString();
            
            m_pEnergyUpgradeInfo.push_back(info);
        }
    }
    
    sqlite3_free_table(dbResult);
}

void CDBMgr::loadBuildingUpgradeInfo()
{
    m_pBuildingUpgradeInfo.clear();
    
    sqlite3 *pDB = CDataBaseOper::getDB();
    int iResult;
    char *pErrMsg=NULL;
    char **dbResult;
    int iRow,iColum;
    int index = 0;
    
    iResult = sqlite3_get_table(pDB, "select * from buildingUpgradeInfo", &dbResult, &iRow, &iColum, &pErrMsg);
    
    if(SQLITE_OK==iResult)
    {
        index = iColum;
        
        for( int i = 0; i < iRow ;i++ )
        {
            buildingUpgradeInfo* info = new buildingUpgradeInfo;
            
            __String value = dbResult[index++];
            info->m_id = value.intValue();
            
            value = dbResult[index++];
            info->m_iBuildingUpgradeId = value.intValue();
            
            value = dbResult[index++];
            info->m_iUpgradeLevel = value.intValue();
            
            value = dbResult[index++];
            info->m_sName = value.getCString();
            
            value = dbResult[index++];
            info->m_sDesc = value.getCString();
            
            value = dbResult[index++];
            info->m_iCostGold = value.intValue();
            
            value = dbResult[index++];
            info->m_iAffectNum = value.intValue();
            
            value = dbResult[index++];
            info->m_sIco =  value.getCString();
            
            m_pBuildingUpgradeInfo.push_back(info);
        }
    }
    
    sqlite3_free_table(dbResult);

    
}

void CDBMgr::loadMonsterEvolutionInfo()
{
    m_pMonsterEvoltionInfo.clear();
    sqlite3 *pDB = CDataBaseOper::getDB();
    int iResult;
    char *pErrMsg=NULL;
    char **dbResult;
    int iRow,iColum;
    int index = 0;
    
    iResult = sqlite3_get_table(pDB, "select * from monsterEvoInfo", &dbResult, &iRow, &iColum, &pErrMsg);
    
    if(SQLITE_OK==iResult)
    {
        index = iColum;
        for( int i = 0; i < iRow ;i++ )
        {
            MonsterEvolutionInfo info;
            __String value = dbResult[index++];
            info.m_star = value.intValue();
            
            value = dbResult[index++];
            info.m_number = value.intValue();
            
            value = dbResult[index++];
            info.m_addHp = value.intValue();
            
            value = dbResult[index++];
            info.m_addAtk = value.intValue();
            
            value = dbResult[index++];
            info.m_skillNum = value.intValue();

            value = dbResult[index++];
            info.m_coinNum = value.intValue();
            
            m_pMonsterEvoltionInfo.push_back(info);
            
        }
    }
    sqlite3_free_table(dbResult);
}




void CDBMgr::SaveBuildingInfo()
{
    
}


void CDBMgr::SaveEnergyUpgradeInfo()
{

    
}


void CDBMgr::SaveUserMonsterSkillToDB(int iMonsterId,int iSKillIndex,int iSKillLv)
{
    bool bExisted = false;
    
    int i=0;
    
    for(;i<UserMonsterInfo.size();i++)
    {
        if(iMonsterId == UserMonsterInfo[i]->monsterId)
        {
            bExisted = true;
            break;
        }
    }
    
    CDBMgr::OpenDB();
    sqlite3 *pDB = CDataBaseOper::getDB();
    int iResult;
    char* pErrMsg = NULL;
    
    char strSkill[100] = {0};
    sprintf(strSkill,"skillLevel%d",iSKillIndex);
    
    
    __String * temp = __String::createWithFormat("update userMonsterInfo set %s=%d where monsterId=%d",strSkill,iSKillLv ,iMonsterId);
    
    iResult = sqlite3_exec(pDB, temp->getCString(), NULL, NULL, &pErrMsg);
    if(iResult != SQLITE_OK )
        CCLOG( "保存怪物技能信息失败，错误码:%d ，错误原因:%s\n" , iResult, pErrMsg );
    CDataBaseOper::closeDB();
    
}

void CDBMgr::SaveUserMonsterInfo(int iMonsterId,int iMonsterLv,int iSkillLv1,int iSkillLv2,int iSkillLv3,int iSkillLv4,bool bFight,
                                     int iType,int iMonsterRare)
{
    bool bExisted = false;
    
    int i=0;
    
    for(;i<UserMonsterInfo.size();i++)
    {
        if(iMonsterId == UserMonsterInfo[i]->monsterId)
        {
            bExisted = true;
            break;
        }
    }
    
    CDBMgr::OpenDB();
    sqlite3 *pDB = CDataBaseOper::getDB();
    int iResult;
    char* pErrMsg = NULL;
    
   __String * temp = __String::createWithFormat("update userMonsterInfo set monsterLevel = %d where monsterId=%d", iMonsterLv,iMonsterId);
    iResult = sqlite3_exec(pDB, temp->getCString(), NULL, NULL, &pErrMsg);
    if(iResult != SQLITE_OK )
        CCLOG( "保存怪物信息失败，错误码:%d ，错误原因:%s\n" , iResult, pErrMsg );
    CDataBaseOper::closeDB();
    
}

void CDBMgr::loadMessageInfo()
{
    m_requestInfo.clear();
    sqlite3 *pDB = CDataBaseOper::getDB();
    int iResult;
    char *pErrMsg=NULL;
    char **dbResult;
    int iRow,iColum;
    int index = 0;
    
    iResult = sqlite3_get_table(pDB, "select * from messageInfo", &dbResult, &iRow, &iColum, &pErrMsg);
    
    if(SQLITE_OK==iResult)
    {
        index = iColum;
        
        for( int i = 0; i < iRow ;i++ )
        {
            __String value = dbResult[index++];
            
            std::string temp = value.getCString();
            m_requestInfo.push_back(temp);
        }
    }
    
    sqlite3_free_table(dbResult);
}


void CDBMgr::loadUserInfoum()
{
    
    sqlite3 *pDB = CDataBaseOper::getDB();
    int iResult;
    char *pErrMsg=NULL;
    char **dbResult;
    int iRow,iColum;
    int index = 0;
    
    iResult = sqlite3_get_table(pDB, "select * from userInfoNum", &dbResult, &iRow, &iColum, &pErrMsg);
    
    if(SQLITE_OK==iResult)
    {
        index = iColum;
        
        for( int i = 0; i < iRow ;i++ )
        {
            __String value = dbResult[index++];
            m_userInfoNum.m_id = value.intValue();
            
            value = dbResult[index++];
            m_userInfoNum.exchangeCoinNum = value.intValue();
            
            value = dbResult[index++];
            m_userInfoNum.exchangeStaminaNum = value.intValue();
            
            value = dbResult[index++];
            m_userInfoNum.refreshTime = value.getCString();
        }
    }
    
    sqlite3_free_table(dbResult);
}

void CDBMgr::loadShop()
{
    m_ShopInfo.clear();
    sqlite3 *pDB = CDataBaseOper::getDB();
    int iResult;
    char *pErrMsg=NULL;
    char **dbResult;
    int iRow,iColum;
    int index = 0;
    
    iResult = sqlite3_get_table(pDB, "select * from shop", &dbResult, &iRow, &iColum, &pErrMsg);
    
    if(SQLITE_OK==iResult)
    {
        index = iColum;
        
        for( int i = 0; i < iRow ;i++ )
        {
            ShopItemInfo* info = new ShopItemInfo;
            
            __String value = dbResult[index++];
            info->m_id = value.intValue();
            
            value = dbResult[index++];
            info->m_iItemId = value.intValue();
            
            value = dbResult[index++];
            info->m_itemType = value.intValue();
            
            value = dbResult[index++];
            info->m_iCostType = value.intValue();
            
            value = dbResult[index++];
            info->m_iPrize = value.intValue();
            
            value = dbResult[index++];
            info->m_iNum = value.intValue();
            
            value = dbResult[index++];
            info->m_saleType  =  value.getCString();
            
            value = dbResult[index++];
            info->m_iRebate  =  value.intValue();
            
            m_ShopInfo.push_back(info);
        }
    }
    
    sqlite3_free_table(dbResult);
    

}



void CDBMgr::loadMonsterUpgradeInfo()
{

    m_pMonsterUpgradeInfo.clear();
    
    sqlite3 *pDB = CDataBaseOper::getDB();
    int iResult;
    char *pErrMsg=NULL;
    char **dbResult;
    int iRow,iColum;
    int index = 0;
    
    iResult = sqlite3_get_table(pDB, "select * from monsterUpgradeInfo", &dbResult, &iRow, &iColum, &pErrMsg);
    
    if(SQLITE_OK==iResult)
    {
        index = iColum;
        
        for( int i = 0; i < iRow ;i++ )
        {
            MonsterUpgradeInfo* info = new MonsterUpgradeInfo;
            
            __String value = dbResult[index++];
            info->m_id = value.intValue();
            info->m_iGoldNum = value.intValue();
            
            m_pMonsterUpgradeInfo.push_back(info);
        }
    }
    
    sqlite3_free_table(dbResult);
}

void CDBMgr::loadTaskProInfo()
{
    m_pTaskProVec.clear();
    
    sqlite3 *pDB = CDataBaseOper::getDB();
    int iResult;
    char *pErrMsg=NULL;
    char **dbResult;
    int iRow,iColum;
    int index = 0;
    
    iResult = sqlite3_get_table(pDB, "select * from taskProgressInfo", &dbResult, &iRow, &iColum, &pErrMsg);
    
    if (SQLITE_OK==iResult)
    {
        index = iColum;
        
        for( int i = 0; i < iRow ; i++ )
        {
            taskProInfo *info = new taskProInfo;
            //taskType
            __String value = dbResult[index++];
            info->m_taskType = value.intValue();
            //num
            value = dbResult[index++];
            info->m_num = value.intValue();
            m_pTaskProVec.push_back(info);
        }
        
    }
    sqlite3_free_table(dbResult);
}

void CDBMgr::loadTaskDB()
{
    m_pTaskInfoVec.clear();
    
    sqlite3 *pDB = CDataBaseOper::getDB();
    int iResult;
    char *pErrMsg=NULL;
    char **dbResult;
    int iRow,iColum;
    int index = 0;
    
    iResult = sqlite3_get_table(pDB, "select * from TaskList", &dbResult, &iRow, &iColum, &pErrMsg);
    
    if (SQLITE_OK==iResult)
    {
        index = iColum;
        
        for( int i = 0; i < iRow ; i++ )
        {
            TaskBase *info = new TaskBase;
            //taskId
            __String value = dbResult[index++];
            info->m_iTaskId = value.intValue();
            //taskType
            value = dbResult[index++];
            info->m_iTaskType = value.intValue();
    
            //desc
            std::string tpStr;
            value = dbResult[index++];
            tpStr = value.getCString();
            info->m_sDesc = tpStr;
            
            //icon
            value = dbResult[index++];
            info->m_taskIcon = value.getCString();
            
            //num
            value = dbResult[index++];
            info->m_iNum = value.intValue();
            
            //money
            value = dbResult[index++];
            info->m_iMoney = value.intValue();
            //exp
            value = dbResult[index++];
            info->m_iExp = value.intValue();
            
            //stone
            value = dbResult[index++];
            info->m_stone = value.intValue();
            
            value = dbResult[index++];
            std::vector<__String> parameters = separateInfoWithMark(value, ";");
            for (int i=0; i<parameters.size(); i++) {
                
                std::vector<__String> parameters2 = separateInfoWithMark(parameters[i], ",");
                
                if(parameters2.size() > 1){
                    IdAndNum  tempBomb ;
                    
                    tempBomb.theId = parameters2[0].intValue();
                    tempBomb.theNumber = parameters2[1].intValue();
                    
                    info->itemVec.push_back(tempBomb);
                }
            }

            
            value = dbResult[index++];
            parameters = separateInfoWithMark(value, ";");
            for (int i=0; i<parameters.size(); i++) {
                
                std::vector<__String> parameters2 = separateInfoWithMark(parameters[i], ",");
                
                if(parameters2.size() > 1)
                {
                    IdAndNum  tempBomb ;
                    
                    tempBomb.theId = parameters2[0].intValue();
                    tempBomb.theNumber = parameters2[1].intValue();
                    info->heroItemVec.push_back(tempBomb);
                }
                
            }
            
            //前置任务开启限制
            value = dbResult[index++];
            info->m_iTaskIdLimmit = value.intValue();
            
            //等级开启限制
            value = dbResult[index++];
            info->m_iLvLimmit = value.intValue();
            
            //已完成
            value = dbResult[index++];
            info->m_eTaskState = (TaskState)value.intValue();
            
            //是否是每日任务
            value = dbResult[index++];
            info->taskProp = value.intValue();
            
            m_pTaskInfoVec.push_back(info);
        }

    }
    sqlite3_free_table(dbResult);
    
}


void CDBMgr::loadFightReinforcementInfo(){
    
    
    
    m_vecFightReinforcementInfo.clear();
    
    sqlite3 *pDB = CDataBaseOper::getDB();
    int iResult;
    char *pErrMsg=NULL;
    char **dbResult;
    int iRow,iColum;
    int index = 0;
    
    iResult = sqlite3_get_table(pDB, "select * from levelReinforcements", &dbResult, &iRow, &iColum, &pErrMsg);
    
    if (SQLITE_OK==iResult)
    {
        index = iColum;
        
        for( int i = 0; i < iRow ; i++ )
        {
            reinforcementInfo *info = new reinforcementInfo;
            //taskType
            
            
            index++; //id 废弃不用
            
            __String value = dbResult[index++];
            info->levelId = value.intValue();

            value = dbResult[index++];
            info->monsterId = value.intValue();
            
            value = dbResult[index++];
            info->monsterLevel = value.intValue();
            
            value = dbResult[index++];
            info->monsterGroup = value.intValue();
            
            value = dbResult[index++];
            info->happenTime = value.intValue();
            
            
            value = dbResult[index++];
            std::vector<__String> parameters = separateInfoWithMark(value, ";");
            for (int i=0; i<parameters.size(); i++) {
                
                std::vector<__String> parameters2 = separateInfoWithMark(parameters[i], ",");
                
                if(parameters2.size() > 1)
                {
                    Vec2  pos ;
                    pos.x = parameters2[0].intValue();
                    pos.y = parameters2[1].intValue();
                    info->allPos.push_back(pos);
                }
                
            }

            
            m_vecFightReinforcementInfo.push_back(info);
        }
        
    }
    sqlite3_free_table(dbResult);
    
}



std::vector<SkillUpgradeInfo*> CDBMgr::GetSkillUpgradeInfo()
{
    return m_pSkillUpgradeInfo;
}


heroSkillUpgradeInfo* CDBMgr::GetHeroSkillWithId(int iHeroSkillId, int iSkillLv)
{
   
    for(int i=0;i<m_pHeroSkillUpgradeInfo.size();i++)
    {
        if(m_pHeroSkillUpgradeInfo[i]->m_heroSkillId == iHeroSkillId && m_pHeroSkillUpgradeInfo[i]->m_SkillLevel == iSkillLv )
        {
            return m_pHeroSkillUpgradeInfo[i];
        }
    
    }
    return NULL;
}


std::vector<heroSkillUpgradeInfo*> CDBMgr::GetHeroSkillUpgradeInfo()
{
    return m_pHeroSkillUpgradeInfo;
}

std::vector<energyUpgradeInfo*> CDBMgr::GetEnergyUpGradeInfo()
{
    return m_pEnergyUpgradeInfo;
}

std::vector<buildingUpgradeInfo*> CDBMgr::GetBuildingUpgradeInfo()
{
    return m_pBuildingUpgradeInfo;
}

std::vector<MonsterUpgradeInfo*> CDBMgr::GetMonsterUpgradeInfo()
{
    return m_pMonsterUpgradeInfo;
}

void CDBMgr::setUserIdAndAuthCode()
{
    UserInfo* pUserInfo = CPersonalData::getInstance()->getUserInfo();
    CDBMgr::OpenDB();
    sqlite3 *pDB = CDataBaseOper::getDB();
    int iResult;
    char* pErrMsg = NULL;
    __String * temp = __String::createWithFormat("update user set userid=%d, Authcode = '%s'",pUserInfo->m_iUserId,pUserInfo->m_sAuthCode.c_str());
    iResult = sqlite3_exec(pDB, temp->getCString(), NULL, NULL, &pErrMsg);
    if(iResult != SQLITE_OK )
        CCLOG( "保存怪物技能信息失败，错误码:%d ，错误原因:%s\n" , iResult, pErrMsg );
    CDataBaseOper::closeDB();
}

void CDBMgr::setUserIfSuggessed()
{
    UserInfo* pUserInfo = CPersonalData::getInstance()->getUserInfo();
    CDBMgr::OpenDB();
    sqlite3 *pDB = CDataBaseOper::getDB();
    int iResult;
    char* pErrMsg = NULL;
    __String * temp = __String::createWithFormat("update user set ifSuggesed=%d",pUserInfo->ifSuggesed);
    iResult = sqlite3_exec(pDB, temp->getCString(), NULL, NULL, &pErrMsg);
    if(iResult != SQLITE_OK )
        CCLOG( "保存怪物技能信息失败，错误码:%d ，错误原因:%s\n" , iResult, pErrMsg );
    CDataBaseOper::closeDB();
}

std::vector<TaskBase*>  CDBMgr::GetAllTaskInfo()
{
    return  m_pTaskInfoVec;
}


std::vector<propInfo*>  CDBMgr::GetAllPropInfo()
{
    return  m_propInfo;
}

std::vector<MyPropInfo*> CDBMgr::GetUserProp()
{
    return m_pMyPropInfo;
}

std::vector<PropOpenInfo*> CDBMgr::GetPropOpenInfo()
{
    return  m_pPropOpenInfo;
    
}

std::vector<ConfigureInfo*> CDBMgr::GetConfigureInfo()
{
    
    return m_vecConfigureInfo;
}

std::vector<UserName>  CDBMgr::GetUserNameInfo()
{
    return m_theUserNameInfo_vec;
}

std::vector<PropOpenInfo*> CDBMgr::getPropOpenInfoById(int iPropId)
{
    std::vector<PropOpenInfo*> pp;
    
    for(int i=0;i<m_pPropOpenInfo.size();i++)
    {
        if(iPropId == m_pPropOpenInfo[i]->m_iUserItem)
        {
            pp.push_back(m_pPropOpenInfo[i]);
        }
    }
    return pp;
}

std::vector<skillInfo*> CDBMgr::GetAllSkillInfo()
{
    return  theSkillInfo;
}



skillInfo *  CDBMgr::GetSoldierSkillInfoBySkillId(int iSkillId)
{
    for(int i=0;i<theSkillInfo.size();i++)
    {
    
        if(theSkillInfo[i]->skillId == iSkillId)
        {
            return theSkillInfo[i];
        }
    
    }
    return NULL;
    
}




void CDBMgr::loadModelDB(){
    
    theModleInfo.clear();
    
    sqlite3 *pDB = CDataBaseOper::getDB();
    int iResult;
    char *pErrMsg=NULL;
    char **dbResult;
    int iRow,iColum;
    int iIndex;
    
    iResult = sqlite3_get_table(pDB, "select * from modelInfo", &dbResult, &iRow, &iColum, &pErrMsg);
    
    if (SQLITE_OK==iResult) {
        
        iIndex = iColum;
        
        for (int i =0; i<iRow; i++) {
            
            modelInfo *info = new modelInfo;
            __String value = "";
            
            //id
            value = dbResult[iIndex++];
            info->modelId = value.intValue();
            
            //levelId
            value = dbResult[iIndex++];
            info->modelName = value.getCString();
            
            
            value = dbResult[iIndex++];
            std::vector<__String> parameters = separateInfoWithMark(value, ",");
            info->idleFrame.start = parameters[0].floatValue();
            info->idleFrame.duration = parameters[1].floatValue();
            
            value = dbResult[iIndex++];
            parameters = separateInfoWithMark(value, ",");
            info->walkFrame.start = parameters[0].floatValue();
            info->walkFrame.duration = parameters[1].floatValue();
            
            value = dbResult[iIndex++];
            parameters = separateInfoWithMark(value, ",");
            info->attackFrame.start = parameters[0].floatValue();
            info->attackFrame.duration = parameters[1].floatValue();
            
            value = dbResult[iIndex++];
            parameters = separateInfoWithMark(value, ",");
            info->specialAttackFrame.start = parameters[0].floatValue();
            info->specialAttackFrame.duration = parameters[1].floatValue();
            
            value = dbResult[iIndex++];
            parameters = separateInfoWithMark(value, ",");
            info->hurtFrame.start = parameters[0].floatValue();
            info->hurtFrame.duration = parameters[1].floatValue();
            
            value = dbResult[iIndex++];
            parameters = separateInfoWithMark(value, ",");
            info->DieFrame.start = parameters[0].floatValue();
            info->DieFrame.duration = parameters[1].floatValue();
            
            value = dbResult[iIndex++];
            parameters = separateInfoWithMark(value, ",");
            info->DizzyFrame.start = parameters[0].floatValue();
            info->DizzyFrame.duration = parameters[1].floatValue();
            
            
            value = dbResult[iIndex++];
            info->radius = value.intValue();
            
            value = dbResult[iIndex++];
            info->attackDamageTime = value.floatValue();
            
            value = dbResult[iIndex++];
            info->SpecialDamageTime = value.floatValue();
            
            value = dbResult[iIndex++];
            info->modelScale = value.floatValue();
            
            
            
            /*
             获得所有的模型所需png路径
             */
            value = dbResult[iIndex++];
            
            if (value.intValue()!=0) {
                std::string originPath = info->modelName.getCString();
                int offset = originPath.find_last_of("/"); //找到最后一个/ 则之前的都是路径名 ，比如  models/saman/saman.c3b  则最后一个/之前都是路径名，图片就在同路径下
                originPath = originPath.substr(0,offset);
                
                
                parameters = separateInfoWithMark(value, ";");
                for (int i = 0;i < parameters.size(); i++) {
                    
                    std::string temp =  originPath  + "/" + parameters[i].getCString();  //路径名和图片名组合起来
                    
                    info->vecAllPng.push_back(temp);
                    
                }
            }
            
            
            
            
            
            theModleInfo.push_back(info);
        }
        
    }
    
    sqlite3_free_table(dbResult);
    
    
    
}



void CDBMgr::loadMonsterDB(){
    
    theMonsterInfo.clear();
    
    sqlite3 *pDB = CDataBaseOper::getDB();
    int iResult;
    char *pErrMsg=NULL;
    char **dbResult;
    int iRow,iColum;
    int iIndex;
    
    iResult = sqlite3_get_table(pDB, "select * from monsterInfo", &dbResult, &iRow, &iColum, &pErrMsg);
    
    if (SQLITE_OK==iResult) {
        
        iIndex = iColum;
        
        for (int i =0; i<iRow; i++) {
            
            monsterInfo *info = new monsterInfo;
            __String value = "";
            
            //id
            value = dbResult[iIndex++];
            info->monsterId = value.intValue();
            
            value = dbResult[iIndex++];
            info->monsterName = value.getCString();
            
            value = dbResult[iIndex++];
            info->monsterType = (FighterType)value.intValue();
            
            value = dbResult[iIndex++];
            info->walkSpeed = value.floatValue();
            
            value = dbResult[iIndex++];
            info->attackRange = value.intValue();
            
            value = dbResult[iIndex++];
            info->attackCD = value.floatValue();

            value = dbResult[iIndex++];
            info->energyCost = value.intValue();
            
            value = dbResult[iIndex++];
            std::vector<__String> parameters = separateInfoWithMark(value, ",");
            
            
            if(info->monsterId == 1004)
            {
                log("123");
                
            }
            
            for (int i=0; i<parameters.size(); i++) {
                if(parameters[i].intValue() > 0)
                    info->allSkill.push_back(parameters[i].intValue());
            }
            
            value = dbResult[iIndex++];
            info->modelId = value.intValue();
            
            value = dbResult[iIndex++];
            info->minAtk = value.intValue();

            value = dbResult[iIndex++];
            info->maxAtk = value.intValue();

            value = dbResult[iIndex++];
            info->minHp = value.intValue();

            value = dbResult[iIndex++];
            info->maxHp = value.intValue();

            value = dbResult[iIndex++];
            info->weight = value.intValue();

            value = dbResult[iIndex++];
            info->maxLevel = value.intValue();

            value = dbResult[iIndex++];
            info->bulletId = value.intValue();
            
            value = dbResult[iIndex++];
            info->monsterIcon = value.getCString();
            
            value = dbResult[iIndex++];
            info->monsterIntroduce = value.getCString();
            
            value = dbResult[iIndex++];
            info->isHaveDebris = value.intValue();
            
            value = dbResult[iIndex++];
            if(value.compare("0"))
            {
                info->attackEffect = value.getCString();
            }
            
            value = dbResult[iIndex++];
            if(value.compare("0"))
            {
                info->dieEffect = value.getCString();
            }
            
            value = dbResult[iIndex++];
            info->ifFly = value.boolValue();
            
            theMonsterInfo.push_back(info);
        }
        
    }
    
    sqlite3_free_table(dbResult);
}



int CDBMgr::HavePropNumById(int iPropId)
{
    int iPropNum = 0;
    
    for(int i=0;i<m_pMyPropInfo.size();i++)
    {
        if(iPropId == m_pMyPropInfo[i]->m_iPropId)
        {
            iPropNum = m_pMyPropInfo[i]->m_iPropNum;
            break;
        }
    }
    
    return iPropNum;
 }


void CDBMgr::DelPropIdByNum(int iPropId,int iNum)
{
    std::vector<MyPropInfo*>::iterator it = m_pMyPropInfo.begin();
    
    for(;it!=m_pMyPropInfo.end();it++)
    {
        if(iPropId == (*it)->m_iPropId)
        {
            if((*it)->m_iPropNum==1)
            {
             m_pMyPropInfo.erase(it);
            }
            else
            {
                (*it)->m_iPropNum -= iNum;
            }
            break;
        }
    }
    
}

void CDBMgr::refreshUserInfoNum()
{
    CDBMgr::OpenDB();
    sqlite3 *pDB = CDataBaseOper::getDB();
    int iResult;
    char* pErrMsg = NULL;
    __String* temp = __String::createWithFormat("update userInfoNum set chargeCoinNum=%d, chargeStaminaNum=%d where id =%d", 0, 0,m_userInfoNum.m_id);
    iResult = sqlite3_exec(pDB, temp->getCString(), NULL, NULL, &pErrMsg);
    if(iResult != SQLITE_OK )
        CCLOG( "保存用户任务兑换次数失败，错误码:%d ，错误原因:%s\n" , iResult, pErrMsg );
    CDataBaseOper::closeDB();
    m_userInfoNum.exchangeCoinNum = 0;
    m_userInfoNum.exchangeStaminaNum = 0;
}

void CDBMgr::refreshDailyTask()
{
    //对于已经完成但是未领取的每日任务则不刷新
    CDBMgr::OpenDB();
    sqlite3 *pDB = CDataBaseOper::getDB();
    int iResult;
    char* pErrMsg = NULL;
    
    std::vector<TaskBase*> allTaskInfo = CDataManager::GetAllTaskInfo();
    for(int i = 0 ; i < allTaskInfo.size(); i++)
    {
        if (allTaskInfo[i]->taskProp == 1)
        {
            if(allTaskInfo[i]->m_eTaskState != Task_Get)
            {
                __String* temp = __String::createWithFormat("update taskProgressInfo set num=%d where taskType between 1 and 8", 0);
                iResult = sqlite3_exec(pDB, temp->getCString(), NULL, NULL, &pErrMsg);
                if(iResult != SQLITE_OK )
                    CCLOG( "保存用户任务刷新时间失败，错误码:%d ，错误原因:%s\n" , iResult, pErrMsg );
                
                temp = __String::createWithFormat("update TaskList set havefinish=%d where taksType between 1 and 8", 0);
                iResult = sqlite3_exec(pDB, temp->getCString(), NULL, NULL, &pErrMsg);
                if(iResult != SQLITE_OK )
                    CCLOG( "保存用户任务刷新时间失败，错误码:%d ，错误原因:%s\n" , iResult, pErrMsg );
            }
        }
    }
    
    CDataBaseOper::closeDB();
    
    for(int i = 0; i < m_pTaskProVec.size(); i++)
    {
        if((m_pTaskProVec.at(i)->m_taskType >= 1) && (m_pTaskProVec.at(i)->m_taskType <= 8))
        {
            for(int j = 0; j < m_pTaskInfoVec.size(); j++)
            {
                if(m_pTaskProVec.at(i)->m_taskType == m_pTaskInfoVec.at(j)->m_iTaskType)
                {
                    if(m_pTaskInfoVec.at(j)->m_eTaskState != Task_Get)
                    {
                        m_pTaskInfoVec.at(j)->m_eTaskState = Task_Go;
                        m_pTaskProVec.at(i)->m_num = 0;
                    }
                }
            }
        }
    }
}

void CDBMgr::saveUpdateExchangeNum(int type)
{
    CDBMgr::OpenDB();
    sqlite3 *pDB = CDataBaseOper::getDB();
    int iResult;
    char* pErrMsg = NULL;
    __String* temp = __String::createWithFormat("update userInfoNum set chargeCoinNum=%d where id =%d", m_userInfoNum.exchangeCoinNum + 1, m_userInfoNum.m_id);
    if(type == 1)
    {
        temp = __String::createWithFormat("update userInfoNum set chargeStaminaNum=%d where id =%d", m_userInfoNum.exchangeStaminaNum + 1, m_userInfoNum.m_id);
        m_userInfoNum.exchangeStaminaNum += 1;
    }
    else{
        m_userInfoNum.exchangeCoinNum += 1;
    }
    iResult = sqlite3_exec(pDB, temp->getCString(), NULL, NULL, &pErrMsg);
    if(iResult != SQLITE_OK )
        CCLOG( "保存用户任务兑换次数失败，错误码:%d ，错误原因:%s\n" , iResult, pErrMsg );
    CDataBaseOper::closeDB();
}

void CDBMgr::saveUpdateExchangeNumTime(std::string time)
{
    CDBMgr::OpenDB();
    sqlite3 *pDB = CDataBaseOper::getDB();
    int iResult;
    char* pErrMsg = NULL;
    
    __String* temp = __String::createWithFormat("update userInfoNum set refreshTime=%s where id =%d", time.c_str(), m_userInfoNum.m_id);
    iResult = sqlite3_exec(pDB, temp->getCString(), NULL, NULL, &pErrMsg);
    if(iResult != SQLITE_OK )
        CCLOG( "保存用户兑换次数刷新时间失败，错误码:%d ，错误原因:%s\n" , iResult, pErrMsg );
    CDataBaseOper::closeDB();
    m_userInfoNum.refreshTime = time;
}

void CDBMgr::saveUpdateDailyaTaskTime(std::string time)
{
    CDBMgr::OpenDB();
    sqlite3 *pDB = CDataBaseOper::getDB();
    int iResult;
    char* pErrMsg = NULL;
    __String* temp = __String::createWithFormat("update user set taskTimeStamp=%s where userid =%d", time.c_str(),
                                                CPersonalData::getInstance()->getUserInfo()->m_iUserId);
    iResult = sqlite3_exec(pDB, temp->getCString(), NULL, NULL, &pErrMsg);
    if(iResult != SQLITE_OK )
        CCLOG( "保存用户任务刷新时间失败，错误码:%d ，错误原因:%s\n" , iResult, pErrMsg );
    CDataBaseOper::closeDB();
    
    CPersonalData::getInstance()->getUserInfo()->m_iTimeStamp = time;
}

void CDBMgr::saveUserLoginTime(std::string time)
{
    CDBMgr::OpenDB();
    sqlite3 *pDB = CDataBaseOper::getDB();
    int iResult;
    char* pErrMsg = NULL;
    __String* temp = __String::createWithFormat("update user set LoginDay='%s' where userid =%d", time.c_str(),
                                                CPersonalData::getInstance()->getUserInfo()->m_iUserId);
    iResult = sqlite3_exec(pDB, temp->getCString(), NULL, NULL, &pErrMsg);
    if(iResult != SQLITE_OK )
        CCLOG( "保存用户任务刷新时间失败，错误码:%d ，错误原因:%s\n" , iResult, pErrMsg );
    CDataBaseOper::closeDB();
    CPersonalData::getInstance()->getUserInfo()->m_sloginDay = time;
}

int CDBMgr::getTaskProByType(int taskType)
{
    for(int i = 0; i < m_pTaskProVec.size(); i++)
    {
        if(m_pTaskProVec.at(i)->m_taskType == taskType)
        {
            return m_pTaskProVec.at(i)->m_num;
        }
    }
    return 0;
}

void CDBMgr::saveTaskInfoToDB(int taskType, int num, bool ibMax)
{
    //先获取当前任务的进度
    int curNum = 0;
    for(int i = 0; i < m_pTaskProVec.size(); i++)
    {
        if(m_pTaskProVec.at(i)->m_taskType == taskType)
        {
            if (ibMax)
            {
                if(m_pTaskProVec.at(i)->m_num < num)
                {
                    m_pTaskProVec.at(i)->m_num = num;
                }
                else
                    return;
            }
            else
            {
                m_pTaskProVec.at(i)->m_num += num;
            }
            curNum = m_pTaskProVec.at(i)->m_num;
            break;
        }
    }
    CDBMgr::OpenDB();
    sqlite3 *pDB = CDataBaseOper::getDB();
    int iResult;
    char* pErrMsg = NULL;
    __String* temp = __String::createWithFormat("update taskProgressInfo set num=%d where taskType =%d",curNum, taskType);
    iResult = sqlite3_exec(pDB, temp->getCString(), NULL, NULL, &pErrMsg);
    if(iResult != SQLITE_OK )
        CCLOG( "保存用户技能升级信息失败，错误码:%d ，错误原因:%s\n" , iResult, pErrMsg );
    CDataBaseOper::closeDB();
    saveTaskState(taskType, Task_Get);
    saveDailyTaskInfo(8);//更新每日任务信息
    saveDailyTaskInfo(16);
}

TaskBase* CDBMgr::getTaskInfoById(int taskId)
{
    for(int i = 0; i < m_pTaskInfoVec.size(); i++)
    {
        if(m_pTaskInfoVec[i]->m_iTaskId == taskId)
        {
            return m_pTaskInfoVec[i];
        }
    }
    return NULL;
}

void CDBMgr::eraseTaskById(int taskId)
{
    for(std::vector<TaskBase *>::iterator it= m_pTaskInfoVec.begin(); it != m_pTaskInfoVec.end(); it++)
    {
        if((*it)->m_iTaskId == taskId)
        {
            m_pTaskInfoVec.erase(it);
            break;
        }
    }
}

void CDBMgr::updateTaskSate(int taskId, TaskState state)
{
    __String* temp = __String::createWithFormat("update TaskList set havefinish=%d where taksId =%d",state, taskId);
    CDBMgr::OpenDB();
    sqlite3 *pDB = CDataBaseOper::getDB();
    int iResult;
    char* pErrMsg = NULL;
    iResult = sqlite3_exec(pDB, temp->getCString(), NULL, NULL, &pErrMsg);
    if(iResult != SQLITE_OK )
        CCLOG( "保存用户技能升级信息失败，错误码:%d ，错误原因:%s\n" , iResult, pErrMsg );
    CDataBaseOper::closeDB();
}

void CDBMgr::saveTaskState(int taskType, TaskState state, bool bTaskId)
{
    bool bValid;//是否满足插入数据库的条件
    __String* temp = NULL;
    if(bTaskId)
    {
        updateTaskSate(taskType, state);
        for(int i = 0; i < m_pTaskInfoVec.size(); i++)
        {
            if(m_pTaskInfoVec.at(i)->m_iTaskId == taskType)
            {
                m_pTaskInfoVec.at(i)->m_eTaskState = state;
                break;
            }
        }
    }
    else
    {
        for(int i = 0; i < m_pTaskInfoVec.size(); i++)
        {
            int nowNum = getTaskProByType(m_pTaskInfoVec.at(i)->m_iTaskType);
            if(m_pTaskInfoVec.at(i)->m_iTaskType == taskType)
            {
                if((nowNum >= m_pTaskInfoVec.at(i)->m_iNum) && (m_pTaskInfoVec.at(i)->m_eTaskState < Task_Fininsh))
                {
                   updateTaskSate(m_pTaskInfoVec.at(i)->m_iTaskId, state);
                    m_pTaskInfoVec.at(i)->m_eTaskState = state;
                }
            }
        }
    }
}

void CDBMgr::saveDailyTaskInfo(int taskType)
{
    int TaskNum = 0;
    for (int i = 0; i < 7; i++)
    {
        if(m_pTaskProVec[i]->m_num >= m_pTaskInfoVec[i]->m_iNum)//直接按顺序来判断
        {
            TaskNum++;
        }
    }
    int nowNum = CDataManager::getTaskProByType(taskType);
    if(TaskNum > nowNum)
    {
        CDBMgr::OpenDB();
        sqlite3 *pDB = CDataBaseOper::getDB();
        int iResult;
        char* pErrMsg = NULL;
        __String* temp = __String::createWithFormat("update taskProgressInfo set num=%d where taskType =%d",TaskNum, 8);
        iResult = sqlite3_exec(pDB, temp->getCString(), NULL, NULL, &pErrMsg);
        if(iResult != SQLITE_OK )
            CCLOG( "保存用户技能升级信息失败，错误码:%d ，错误原因:%s\n" , iResult, pErrMsg );
        CDataBaseOper::closeDB();
        saveTaskInfoToDB(taskType, TaskNum, true);
    }
    saveTaskState(8, Task_Get);
}

void CDBMgr::saveGameStoryInfoToDB(int type)
{
    //先判断该剧情是否已经保存
    bool bExisted = false;
    char buf[128];
    std::vector<int> tempVec = theUserInfo->storyInfo;
    for(int i = 0; i < tempVec.size(); i++)
    {
        if(type == tempVec[i])
        {
            bExisted = true;
            break;
        }
    }
    
    if(!bExisted)
    {
        std::string storyStr;
        for(int i = 0; i < tempVec.size(); i++)
        {
            sprintf(buf, "%d", tempVec[i]);
            storyStr += buf;
            if(i != tempVec.size() - 1)
            {
                storyStr += ",";
            }
        }
        if(tempVec.size() > 0)
        {
            storyStr += ",";
        }
        sprintf(buf, "%d", type);
        storyStr += buf;
        tempVec.push_back(type);
        
        theUserInfo->storyInfo = tempVec;
        
        CDBMgr::OpenDB();
        sqlite3 *pDB = CDataBaseOper::getDB();
        int iResult;
        char* pErrMsg = NULL;
        __String* temp = __String::createWithFormat("update user set storyInfo = '%s' where userid =%d",storyStr.c_str(), CPersonalData::getInstance()->getUserInfo()->m_iUserId);
        iResult = sqlite3_exec(pDB, temp->getCString(), NULL, NULL, &pErrMsg);
        if(iResult != SQLITE_OK )
            CCLOG( "保存用户技能升级信息失败，错误码:%d ，错误原因:%s\n" , iResult, pErrMsg );
        CDataBaseOper::closeDB();
    }
}

void CDBMgr::saveGuideInfoToDB(int type)
{
    //先判断该引导是否已经保存
    bool bExisted = false;
    for(int i = 0; i < thsGuideProInfo.size(); i++)
    {
        if(type == thsGuideProInfo[i].typeId)
        {
            bExisted = true;
            break;
        }
    }
    if(!bExisted)
    {
        CDBMgr::OpenDB();
        sqlite3 *pDB = CDataBaseOper::getDB();
        int iResult;
        char* pErrMsg = NULL;
        __String* temp = __String::createWithFormat("insert into guideProgress values(%d,%d)",type, 1);
        iResult = sqlite3_exec(pDB, temp->getCString(), NULL, NULL, &pErrMsg);
        if(iResult != SQLITE_OK )
            CCLOG( "保存用户技能升级信息失败，错误码:%d ，错误原因:%s\n" , iResult, pErrMsg );
        CDataBaseOper::closeDB();
        
        //更新本地数据
        guidePro info;
        info.typeId = type;
        info.isEnd = 1;
        thsGuideProInfo.push_back(info);
    }
}

void CDBMgr::SaveUserPropDb(int iPropId,int iPropNum)
{
    bool bExisted = false;
 
    int i=0;
    
    CDBMgr::OpenDB();
    sqlite3 *pDB = CDataBaseOper::getDB();
   
    std::vector<MyPropInfo*>::iterator it = m_pMyPropInfo.begin();
    
    for(;i<m_pMyPropInfo.size();i++,it++)
    {
        if(iPropId == m_pMyPropInfo[i]->m_iPropId)
        {
            bExisted = true;
            log("i= %d\n",i);
            break;
        }
    }

      log("i= %d\n",i);
    
    int iResult;
    char* pErrMsg = NULL;
    __String * temp = NULL;
    

    if(bExisted)
    {
        if(iPropNum==0)
        {
            temp = __String::createWithFormat("delete from UserItems  where itemId=%d",iPropId);
            m_pMyPropInfo.erase(it);
        }
        else
        {
            temp = __String::createWithFormat("update UserItems set itemNum = %d where itemId=%d",iPropNum,iPropId);
            m_pMyPropInfo[i]->m_iPropNum = iPropNum;
        }
    }
    
    
    if(!bExisted )
    {
        temp = __String::createWithFormat("insert into UserItems values(%d,%d)",iPropId,iPropNum);
        MyPropInfo *info = new  MyPropInfo;
        info->m_iPropId = iPropId;
        info->m_iPropNum = iPropNum;
        
        m_pMyPropInfo.push_back(info);
        
    }
    iResult = sqlite3_exec(pDB, temp->getCString(), NULL, NULL, &pErrMsg);
    
    
    if(iResult != SQLITE_OK )
        CCLOG( "保存用户道具信息失败，错误码:%d ，错误原因:%s\n" , iResult, pErrMsg );
    CDataBaseOper::closeDB();
}

void CDBMgr::loadUserPropDB()
{
    
    m_pMyPropInfo.clear();
    
    sqlite3 *pDB = CDataBaseOper::getDB();
    
    int iResult = 0 ;
    char *pErrMsg = NULL;
    char **dbResult;
    int iRow,iColum;
    int iIndex = 0;
    
    iResult = sqlite3_get_table(pDB, "select * from UserItems", &dbResult, &iRow, &iColum, &pErrMsg);
    
    if(SQLITE_OK == iResult)
    {
        iIndex = iColum;
        for (int i =0; i<iRow; i++)
        {
            
            MyPropInfo* info = new MyPropInfo;
            __String value = "";
            
            //id
            value = dbResult[iIndex++];
            info->m_iPropId = value.intValue();
            
            //num
            value = dbResult[iIndex++];
            info->m_iPropNum =  value.intValue();

            if(info->m_iPropId>0 &&info->m_iPropId<1000)
            m_pMyPropInfo.push_back(info);
        }
    }
    
    sqlite3_free_table(dbResult);
}


void CDBMgr::loadPropDB()
{
    m_propInfo.clear();
    
    sqlite3 *pDB = CDataBaseOper::getDB();
    int iResult;
    char *pErrMsg=NULL;
    char **dbResult;
    int iRow,iColum;
    int iIndex;
    
    iResult = sqlite3_get_table(pDB, "select * from itemInfo", &dbResult, &iRow, &iColum, &pErrMsg);
    
    if(SQLITE_OK == iResult)
    {
        iIndex = iColum;
        for (int i =0; i<iRow; i++)
        {
            
           propInfo *info = new propInfo;
            __String value = "";
            
            //id
            value = dbResult[iIndex++];
            info->m_itemId = value.intValue();
            
            value = dbResult[iIndex++];
            info->m_sItemName =  value.getCString();
            
            value = dbResult[iIndex++];
            info->m_sItemDes = value.getCString();
            
            value = dbResult[iIndex++];
            info->m_sItemIcon = value.getCString();
            
            value = dbResult[iIndex++];
            info->m_iItemLevel = value.intValue();
            
            value = dbResult[iIndex++];
            info->m_bUse = value.intValue();
            
            value = dbResult[iIndex++];
            info->m_iNeedPropId = value.intValue();
            
            value = dbResult[iIndex++];
            info->m_iNeedPropNum = value.intValue();
            
            value = dbResult[iIndex++];
            info->m_iSalePrize= value.intValue();
            
            value = dbResult[iIndex++];
            info->m_fDropRatio= value.floatValue();


            //log("salePrize = %d",info->m_iSalePrize);
            m_propInfo.push_back(info);
        }
        
    }
    
    sqlite3_free_table(dbResult);
}




void CDBMgr::loadUserMonsterDB()
{
    UserMonsterInfo.clear();
    
    sqlite3 *pDB = CDataBaseOper::getDB();
    int iResult;
    char *pErrMsg=NULL;
    char **dbResult;
    int iRow,iColum;
    int iIndex;
    
    iResult = sqlite3_get_table(pDB, "select * from userMonsterInfo", &dbResult, &iRow, &iColum, &pErrMsg);
    
    if (SQLITE_OK==iResult) {
        
        iIndex = iColum;
        
        for (int i =0; i<iRow; i++) {
            
            myMonster *info = new myMonster;
            __String value = "";
            
            //id
            value = dbResult[iIndex++];
            info->monsterId = value.intValue();

            value = dbResult[iIndex++];
            info->monsterLevel = value.intValue();
            
            value = dbResult[iIndex++];
            info->skillLevel1 = value.intValue();
            
            value = dbResult[iIndex++];
            info->skillLevel2 = value.intValue();
            
            value = dbResult[iIndex++];
            info->skillLevel3 = value.intValue();
            
            value = dbResult[iIndex++];
            info->skillLevel4 = value.intValue();
            
            value = dbResult[iIndex++];
            info->ifFight = value.intValue();
            
            value = dbResult[iIndex++];
            info->theType = value.intValue();
            
            value = dbResult[iIndex++];
            info->rare = value.intValue();
            
            value = dbResult[iIndex++];
            info->num = value.intValue();
            CCLOG("debrisinfo 读取成功");
            
            UserMonsterInfo.push_back(info);
        }
        
    }
    
    sqlite3_free_table(dbResult);

}


void CDBMgr::loadLevelInfoDB(){
    
    theLevelInfo.clear();
    
    
    sqlite3 *pDB = CDataBaseOper::getDB();
    int iResult;
    char *pErrMsg=NULL;
    char **dbResult;
    int iRow,iColum;
    int iIndex;
    
    iResult = sqlite3_get_table(pDB, "select * from levelInfo", &dbResult, &iRow, &iColum, &pErrMsg);
    
    if (SQLITE_OK==iResult) {
        
        iIndex = iColum;
        
        for (int i =0; i<iRow; i++) {
            
            levelInfo *info = new levelInfo;
            __String value = "";
            
            //levelid
            value = dbResult[iIndex++];
            info->levelId = value.intValue();
            
            value = dbResult[iIndex++];
            info->levelName = value.getCString();
            
            value = dbResult[iIndex++];
            info->levelDes = value.getCString();
            
            value = dbResult[iIndex++];
            info->cityLevel = value.intValue();
            
            value = dbResult[iIndex++];
            info->goldNum = value.intValue();
            
            value = dbResult[iIndex++];
            info->playerExp = value.intValue();

            value = dbResult[iIndex++];
            std::vector<__String> parameters = separateInfoWithMark(value, ";");
            for (int i = 0;i < parameters.size(); i++) {
                std::vector<__String> parameters2 = separateInfoWithMark(parameters[i], ",");
                if (parameters2.size() > 1) {
                    smallAwardInfo pAwardInfo;
                    pAwardInfo.type = parameters2[0].intValue();
                    pAwardInfo.Id = parameters2[1].intValue();
                    info->itemsVec.push_back(pAwardInfo);
                }
            }
            
            value = dbResult[iIndex++];
            parameters = separateInfoWithMark(value, ",");
            for (int i=0; i<parameters.size(); i++) {
                info->starIds.push_back(parameters[i].intValue());
            }
            
            value = dbResult[iIndex++];
            parameters = separateInfoWithMark(value, ",");
            for (int i=0; i<parameters.size(); i++) {
                info->monsterGroup.push_back(parameters[i].intValue());
            }
            
            value = dbResult[iIndex++];
            info->bossId = value.intValue();
            
            value = dbResult[iIndex++];
            info->monsterLevel = value.intValue();
            
            value = dbResult[iIndex++];
            info->monsterSkillLevel = value.intValue();
            
            value = dbResult[iIndex++];
            info->energySpeed = value.intValue();
            
            value = dbResult[iIndex++];
            parameters = separateInfoWithMark(value, ";");
            for (int i=0; i<parameters.size(); i++) {
                
                std::vector<__String> parameters2 = separateInfoWithMark(parameters[i], ",");
                
                bombTime  tempBomb ;
                
                tempBomb.theBombTime = parameters2[0].intValue();
                tempBomb.theBombNum = parameters2[1].intValue();
                tempBomb.theBossNum = parameters2[2].intValue();
                
                info->theBombInfo.push_back(tempBomb);

            }

            value = dbResult[iIndex++];
            info->levelCondition = value.intValue();
            
            value = dbResult[iIndex++];
            info->levelConditionDes = value.getCString();
            
            value = dbResult[iIndex++];
            info->conditionNum = value.intValue();

            value = dbResult[iIndex++];
            info->mapId = value.intValue();
            
            value = dbResult[iIndex++];
            parameters = separateInfoWithMark(value, ";");
            for (int i = 0;i < parameters.size(); i++) {
                std::vector<__String> parameters2 = separateInfoWithMark(parameters[i], ",");
                if (parameters2.size() > 1) {
                    smallAwardInfo pAwardInfo;
                    pAwardInfo.type = parameters2[0].intValue();
                    pAwardInfo.Id = parameters2[1].intValue();
                    info->heroItemsVec.push_back(pAwardInfo);
                }
            }

            value = dbResult[iIndex++];
            info->stamina = value.intValue();
            
            theLevelInfo.push_back(info);
        }
        
    }
    
    sqlite3_free_table(dbResult);
    

}


void CDBMgr::loadMapInfoDB(){
    
    
    theMapInfo.clear();
    
    sqlite3 *pDB = CDataBaseOper::getDB();
    int iResult;
    char *pErrMsg=NULL;
    char **dbResult;
    int iRow,iColum;
    int iIndex;
    
    iResult = sqlite3_get_table(pDB, "select * from mapInfo", &dbResult, &iRow, &iColum, &pErrMsg);
    
    if (SQLITE_OK==iResult) {
        
        iIndex = iColum;
        
        for (int i =0; i<iRow; i++) {
            
            mapInfo *info = new mapInfo;
            __String value = "";
            
            //id不用
            value = dbResult[iIndex++];
            
            value = dbResult[iIndex++];
            info->mapId = value.intValue();
            
            value = dbResult[iIndex++];
            info->monsterId = value.intValue();
            
            value = dbResult[iIndex++];
            info->monsterLevel = value.intValue();
            
            value = dbResult[iIndex++];
            info->buildingType = (AttackType)value.intValue();
            
            value = dbResult[iIndex++];
            info->buildingGroup = (FightGroup)value.intValue();
            
            value = dbResult[iIndex++];
            std::vector<__String> parameters = separateInfoWithMark(value, ",");
            
            info->pos.x =parameters[0].intValue();
            info->pos.y =parameters[1].intValue();
            
            theMapInfo.push_back(info);
        }
        
    }
    
    sqlite3_free_table(dbResult);
    
}

void CDBMgr::loadSkillDB(){
    
    
    theSkillInfo.clear();
    
    sqlite3 *pDB = CDataBaseOper::getDB();
    int iResult;
    char *pErrMsg=NULL;
    char **dbResult;
    int iRow,iColum;
    int iIndex;
    
    iResult = sqlite3_get_table(pDB, "select * from skillInfo", &dbResult, &iRow, &iColum, &pErrMsg);
    
    if (SQLITE_OK==iResult) {
        
        iIndex = iColum;
        
        for (int i =0; i<iRow; i++) {
            
            skillInfo *info = new skillInfo;
            __String value = "";
            
            value = dbResult[iIndex++];
            info->skillId = value.intValue();
            
            value = dbResult[iIndex++];
            info->skillName = value.getCString();
            
            value = dbResult[iIndex++];
            info->skillDes = value.getCString();

            value = dbResult[iIndex++];
            info->skillIcon = value.getCString();
            
            value = dbResult[iIndex++];
            info->skillType = value.intValue();
            
            value = dbResult[iIndex++];
        
            info->damageRatioMin = value.doubleValue();
        
            value.getCString();
            
            value = dbResult[iIndex++];
            info->startRatioMin = value.floatValue();
            
            value = dbResult[iIndex++];
            info->damageRatioMax = value.doubleValue();
            
            value = dbResult[iIndex++];
            info->startRatioMax = value.floatValue();
            
            value = dbResult[iIndex++];
            info->skillEffectId = value.intValue();
            
            value = dbResult[iIndex++];
            info->maxLevel = value.intValue();
            
            value = dbResult[iIndex++];
//            info->moveSpeed = value.floatValue();
            std::vector<__String> parameters = separateInfoWithMark(value, ",");
            info->moveSpeed = parameters[0].floatValue();
            info->moveSpeedY = parameters[1].floatValue();   //高度上的速度，用于抛物线处理
            
            value = dbResult[iIndex++];
            info->damageRange = value.intValue();
            
            value = dbResult[iIndex++];
            info->hurtNumbers = value.intValue();
            
            value = dbResult[iIndex++];
            info->lifeTime = value.floatValue();

            value = dbResult[iIndex++];
            parameters = separateInfoWithMark(value, ",");
            for (int i=0; i<parameters.size(); i++) {
                
                info->hurtTime.push_back(parameters[i].floatValue());
                
            }
            
            value = dbResult[iIndex++];
            info->hurtType = value.intValue();
            
            value = dbResult[iIndex++];
            info->buffId = value.intValue();
            
            value = dbResult[iIndex++];
            info->positionGroup = value.intValue();
            
            value = dbResult[iIndex++];
            info->positionCondition = (ChooseCondition)value.intValue();
            
            value = dbResult[iIndex++];
            info->effectGroup = value.intValue();
            
            value = dbResult[iIndex++];
            info->effectCondition = (ChooseCondition)value.intValue();

            
            value = dbResult[iIndex++];
            info->ifLocked = value.intValue();
            
            value = dbResult[iIndex++];
            parameters = separateInfoWithMark(value, ",");
            if (parameters.size()==3) {
                info->offset.x=parameters[0].intValue();
                info->offset.y=parameters[1].intValue();
                info->offset.z=parameters[2].intValue();
            }else{
                
                CCLOG("LYAssert: skillDB wrong at skillId = %d",info->skillEffectId);
            }
            
            
            value = dbResult[iIndex++];
            info->startCondition = value.intValue();

            value = dbResult[iIndex++];
            info->skillNamePic = value.getCString();
            
            value = dbResult[iIndex++];
            if(value.compare("0"))
            {
                info->skillEffect = value.getCString();
            }
            theSkillInfo.push_back(info);
        }
        
    }
    
    sqlite3_free_table(dbResult);

}

void CDBMgr::loadSkillEffectDB(){
    
    theSkillEffectInfo.clear();
    
    sqlite3 *pDB = CDataBaseOper::getDB();
    int iResult;
    char *pErrMsg=NULL;
    char **dbResult;
    int iRow,iColum;
    int iIndex;
    
    iResult = sqlite3_get_table(pDB, "select * from skillEffect", &dbResult, &iRow, &iColum, &pErrMsg);
    
    if (SQLITE_OK==iResult) {
        
        iIndex = iColum;
        
        for (int i =0; i<iRow; i++) {
            
            skillEffect *info = new skillEffect;
            __String value = "";
            
            
            value = dbResult[iIndex++];
            info->skillEffectId = value.intValue();
            
            value = dbResult[iIndex++];
            info->effectType = value.intValue();
            
            
            value = dbResult[iIndex++];
            info->skillPath = value.getCString();
            
            value = dbResult[iIndex++];
            info->skillFrameNumber = value.intValue();
            
            
            value = dbResult[iIndex++];
            std::vector<__String> parameters = separateInfoWithMark(value, ",");
            if (parameters.size()==3) {
                info->offset.x=parameters[0].intValue();
                info->offset.y=parameters[1].intValue();
                info->offset.z=parameters[2].intValue();
            }else{
                
                CCLOG("LYAssert: skillEffectDB wrong at skillEffectId = %d",info->skillEffectId);
            }
            
            value = dbResult[iIndex++];
            info->effectScale = value.floatValue();
            

            value = dbResult[iIndex++];
            parameters = separateInfoWithMark(value, ",");
            if (parameters.size()==2) {
                info->hurtAnimate.animatePath=parameters[0].getCString();
                info->hurtAnimate.animateNumber=parameters[1].intValue();
            }else{
                
                CCLOG("LYAssert: skillEffectDB wrong at skillEffectId = %d",info->skillEffectId);
            }
            
            value = dbResult[iIndex++];
            parameters = separateInfoWithMark(value, ",");
            if (parameters.size()==2) {
                info->bombAnimate.animatePath=parameters[0].getCString();
                info->bombAnimate.animateNumber=parameters[1].intValue();
            }else{
                
                CCLOG("LYAssert: skillEffectDB wrong at skillEffectId = %d",info->skillEffectId);
            }
            

            theSkillEffectInfo.push_back(info);
        }
        
    }
    
    sqlite3_free_table(dbResult);

}

void CDBMgr::loadMidLevelInfo()
{
    
    midLevelInfoVec.clear();
    
    sqlite3 *pDB = CDataBaseOper::getDB();
    int iResult;
    char *pErrMsg=NULL;
    char **dbResult;
    int iRow,iColum;
    int iIndex;
    
    iResult = sqlite3_get_table(pDB, "select * from midPassInfo", &dbResult, &iRow, &iColum, &pErrMsg);
    
    if (SQLITE_OK==iResult) {
        
        iIndex = iColum;
        
        for (int i =0; i<iRow; i++) {
            
            midLevelInfo info;
            __String value = "";
            
            value = dbResult[iIndex++];//id
            info.midLevelId = value.intValue();
            
            value = dbResult[iIndex++];
            info.name = value.getCString();
            
            value = dbResult[iIndex++];
            info.icon = value.getCString();
            
            value = dbResult[iIndex++];
            __String tempStr = value.getCString();
            std::vector<__String> parameters = separateInfoWithMark(tempStr, ",");
            info.xpos = parameters[0].floatValue();
            info.ypos = parameters[1].floatValue();
            
            value = dbResult[iIndex++];
            tempStr = value.getCString();
            parameters = separateInfoWithMark(tempStr, ",");
            info.firstLevel = parameters[0].intValue();
            info.lastLevel = parameters[1].intValue();
            
            //奖励1
            value = dbResult[iIndex++];
            tempStr = value.getCString();
            parameters = separateInfoWithMark(tempStr, ",");
            if(parameters.size() > 0)
            {
                awardInfo  award ;
                award.type = parameters[0].intValue();
                if(award.type == 0)
                {
                    award.PropId = parameters[1].intValue();
                    award.num = parameters[2].intValue();
                    
                }
                else{
                    award.num = parameters[1].intValue();
                    award.PropId = parameters[2].intValue();
                }
                info.awardVec.push_back(award);
            }
            
            //奖励2
            value = dbResult[iIndex++];
            tempStr = value.getCString();
            parameters = separateInfoWithMark(tempStr, ",");
            if(parameters.size() > 0)
            {
                awardInfo  award ;
                award.type = parameters[0].intValue();
                if(award.type == 0)
                {
                    award.PropId = parameters[1].intValue();
                    award.num = parameters[2].intValue();
                    
                }
                else{
                    award.num = parameters[1].intValue();
                    award.PropId = parameters[2].intValue();
                }
                
                info.awardVec.push_back(award);
            }
            
            //奖励3
            value = dbResult[iIndex++];
            tempStr = value.getCString();
            parameters = separateInfoWithMark(tempStr, ",");
            if(parameters.size() > 0)
            {
                awardInfo  award ;
                award.type = parameters[0].intValue();
                if(award.type == 0)
                {
                    award.PropId = parameters[1].intValue();
                    award.num = parameters[2].intValue();
                    
                }
                else{
                    award.num = parameters[1].intValue();
                    award.PropId = parameters[2].intValue();
                }
                
                info.awardVec.push_back(award);
            }
            
            //精英关卡奖励
            //奖励1
            value = dbResult[iIndex++];
            tempStr = value.getCString();
            parameters = separateInfoWithMark(tempStr, ",");
            if(parameters.size() > 0)
            {
                awardInfo  heroAward ;
                heroAward.type = parameters[0].intValue();
                if(heroAward.type == 0)
                {
                    heroAward.PropId = parameters[1].intValue();
                    heroAward.num = parameters[2].intValue();
                    
                }
                else{
                    heroAward.num = parameters[1].intValue();
                    heroAward.PropId = parameters[2].intValue();
                }
                
                info.heroAwardVec.push_back(heroAward);
            }
            //奖励2
            value = dbResult[iIndex++];
            tempStr = value.getCString();
            parameters = separateInfoWithMark(tempStr, ",");
            if(parameters.size() > 0)
            {
                awardInfo  heroAward ;
                heroAward.type = parameters[0].intValue();
                if(heroAward.type == 0)
                {
                    heroAward.PropId = parameters[1].intValue();
                    heroAward.num = parameters[2].intValue();
                    
                }
                else{
                    heroAward.num = parameters[1].intValue();
                    heroAward.PropId = parameters[2].intValue();
                }
                info.heroAwardVec.push_back(heroAward);
            }
            
            //奖励3
            value = dbResult[iIndex++];
            tempStr = value.getCString();
            parameters = separateInfoWithMark(tempStr, ",");
            if(parameters.size() > 0)
            {
                awardInfo  heroAward ;
                heroAward.type = parameters[0].intValue();
                if(heroAward.type == 0)
                {
                    heroAward.PropId = parameters[1].intValue();
                    heroAward.num = parameters[2].intValue();
                    
                }
                else{
                    heroAward.num = parameters[1].intValue();
                    heroAward.PropId = parameters[2].intValue();
                }
                info.heroAwardVec.push_back(heroAward);
            }
            
            value = dbResult[iIndex++];
            parameters = separateInfoWithMark(value, ";");
            for (int i=0; i<parameters.size(); i++) {
                
                std::vector<__String> parameters2 = separateInfoWithMark(parameters[i], ",");
                
                cocos2d::Vec2  pos ;
                
                pos.x = parameters2[0].floatValue();
                pos.y = parameters2[1].floatValue();
                
                info.pointPocVec.push_back(pos);
            }
            midLevelInfoVec.push_back(info);
        }
    }
    sqlite3_free_table(dbResult);
}

void CDBMgr::loadLevelStarInfo()
{
    m_levelStarInfo.clear();
    
    
    sqlite3 *pDB = CDataBaseOper::getDB();
    int iResult;
    char *pErrMsg=NULL;
    char **dbResult;
    int iRow,iColum;
    int iIndex;
    
    iResult = sqlite3_get_table(pDB, "select * from starInfo", &dbResult, &iRow, &iColum, &pErrMsg);
    if (SQLITE_OK==iResult) {
        
        iIndex = iColum;
        
        for (int i =0; i<iRow; i++) {
            
            starInfo info;
            __String value = "";
            
            value = dbResult[iIndex++];
            info.starId = value.intValue();
            
            value = dbResult[iIndex++];
            info.starDesc = value.getCString();
            
            
            value = dbResult[iIndex++];
            info.starType = value.intValue();
            
            value = dbResult[iIndex++];
            std::vector<__String> parameters = separateInfoWithMark(value, ";");
            for (int i=0; i<parameters.size(); i++) {
                
                std::vector<__String> parameters2 = separateInfoWithMark(parameters[i], ",");
                if(parameters2.size() > 1)
                {
                    info.monsterId = parameters2[0].intValue();
                    info.num = parameters2[1].intValue();
                }
                else{
                    info.num = parameters2[0].intValue();
                }
            }
            value = dbResult[iIndex++];
            info.positive = value.intValue();
            m_levelStarInfo.push_back(info);
        }
        
    }
    
    sqlite3_free_table(dbResult);
}


void CDBMgr::loadUserUpgradeInfo()
{
    
    thsUserUpgradeInfo.clear();
    
    sqlite3 *pDB = CDataBaseOper::getDB();
    int iResult;
    char *pErrMsg=NULL;
    char **dbResult;
    int iRow,iColum;
    int iIndex;
    iResult = sqlite3_get_table(pDB, "select * from userInfo", &dbResult, &iRow, &iColum, &pErrMsg);
    if (SQLITE_OK==iResult) {
        
        iIndex = iColum;
        
        for (int i =0; i<iRow; i++) {
            
            upgradeInfo info;
            __String value = "";
            
            
            value = dbResult[iIndex++];
            info.level = value.intValue();
            
            value = dbResult[iIndex++];
            info.upperLevel = value.intValue();
            
            
            value = dbResult[iIndex++];
            info.upgradeExp = value.intValue();
            
            value = dbResult[iIndex++];
            info.upperStamina = value.intValue();
            
            value = dbResult[iIndex++];
            info.upgradeStamina = value.intValue();
            
            value = dbResult[iIndex++];
            info.userExp = value.intValue();
            
            thsUserUpgradeInfo.push_back(info);
        }
        
    }
    
    sqlite3_free_table(dbResult);
}


int CDBMgr::GetHeroLvWithId(int iHeroId)
{
    std::string strHeroLv = theUserInfo->m_strHeroLv;
    
    AnalyseHeroLvInfo(strHeroLv);
    
    for(int i=0;i<m_HeroLvInfo_vec.size();i++)
    {
        if(iHeroId == m_HeroLvInfo_vec[i].m_iHeroid)
        {
            return m_HeroLvInfo_vec[i].m_iLv;
        }
    }
    return 0;
}


void CDBMgr::loadUserInfo(){
    
    if (theUserInfo) {
        delete theUserInfo;
    }
    
    
    sqlite3 *pDB = CDataBaseOper::getDB();
    int iResult;
    char *pErrMsg=NULL;
    char **dbResult;
    int iRow,iColum;
    int iIndex;
    
    iResult = sqlite3_get_table(pDB, "select * from user", &dbResult, &iRow, &iColum, &pErrMsg);
    
    if (SQLITE_OK==iResult) {
        
        iIndex = iColum;
        
        CCLOG("从数据库读取用户信息");
        
        for (int i =0; i<iRow; i++) {
            
            
            CCLOG("有数据，开始读取");
            
            theUserInfo = new userInfoInDB;
            __String value = "";
            
            value = dbResult[iIndex++];//id不用了
            
            value = dbResult[iIndex++];
            theUserInfo->userId = value.intValue();
            
            value = dbResult[iIndex++];
            theUserInfo->userName = value.getCString();
            
            value = dbResult[iIndex++];
            theUserInfo->userLevel = value.intValue();
            
            value = dbResult[iIndex++];
            theUserInfo->userExp = value.intValue();
            
            value = dbResult[iIndex++];
            theUserInfo->coinStr = value.getCString();
            
            value = dbResult[iIndex++];
            theUserInfo->magicStoneStr = value.getCString();
            
            value = dbResult[iIndex++];
            theUserInfo->loginDay = value.getCString();
            
    	
            value = dbResult[iIndex++];
            std::vector<__String> parameters = separateInfoWithMark(value, ";");
            for (int i=0; i<parameters.size(); i++) {
                
                std::vector<__String> parameters2 = separateInfoWithMark(parameters[i], ",");
                
                IdAndNum  tempBomb ;
                
                tempBomb.theId = parameters2[0].intValue();
                tempBomb.theNumber = parameters2[1].intValue();

                
                theUserInfo->heroSkills.push_back(tempBomb);
                
            }
            
            value = dbResult[iIndex++];
            theUserInfo->authCode = value.getCString();

            //用户通关星级信息
            value = dbResult[iIndex++];
            parameters = separateInfoWithMark(value, ";");
            for (int i = 0;i < parameters.size(); i++) {
                std::vector<__String> parameters2 = separateInfoWithMark(parameters[i], ",");
                
                levelAndStar  levelInfo ;
                
                levelInfo.level = parameters2[0].intValue();
                levelInfo.star = parameters2[1].intValue();
                theUserInfo->userLevelVec.push_back(levelInfo);
            }
            theUserInfo->smallLevel = theUserInfo->userLevelVec.size();//通关数
            
            value = dbResult[iIndex++];
            parameters = separateInfoWithMark(value, ";");
            for (int i = 0;i < parameters.size(); i++) {
                std::vector<__String> parameters2 = separateInfoWithMark(parameters[i], ",");
                
                levelAwardInfo  awardInfo ;
                
                awardInfo.midLevel = parameters2[0].intValue();
                awardInfo.rewards = parameters2[1].intValue();
                theUserInfo->userMidLevelRewardVec.push_back(awardInfo);
            }
            
            value = dbResult[iIndex++];
            parameters = separateInfoWithMark(value, ";");
            for (int i = 0;i < parameters.size(); i++) {
                std::vector<__String> parameters2 = separateInfoWithMark(parameters[i], ",");
                
                debrisNumInfo  pInfo ;
                pInfo.monsterId = parameters2[0].intValue();
                pInfo.num = parameters2[1].intValue();
                theUserInfo->m_debrisVec.push_back(pInfo);
            }
            CCLOG("debrisinfo 读取成功");
            
            //用户通关星级信息
            value = dbResult[iIndex++];
            parameters = separateInfoWithMark(value, ";");
            for (int i = 0;i < parameters.size(); i++) {
                std::vector<__String> parameters2 = separateInfoWithMark(parameters[i], ",");
                if(parameters2.size() >= 2){
                    levelAndStar  levelInfo ;
                    
                    levelInfo.level = parameters2[0].intValue();
                    levelInfo.star = parameters2[1].intValue();
                    theUserInfo->userEqliteLevelVec.push_back(levelInfo);
                }
            }
            theUserInfo->eqliteSmallLevel = theUserInfo->userEqliteLevelVec.size();//精英通关数
            
            CCLOG("英雄关卡星级 读取成功");
            
            value = dbResult[iIndex++];
            theUserInfo->staminaStr = value.getCString();
            
            value = dbResult[iIndex++];
            theUserInfo->staminaTime = value.intValue();

            value = dbResult[iIndex++];
            theUserInfo->strBuildingLv = value.getCString();
            
            AnalyseStringFromBuilding(theUserInfo->strBuildingLv);
            
            
            value = dbResult[iIndex++];
            theUserInfo->strEnergyLv = value.getCString();
            AnalyseStringFromEnergy(theUserInfo->strEnergyLv);
            
            value = dbResult[iIndex++];
            parameters = separateInfoWithMark(value, ";");
            for (int i = 0;i < parameters.size(); i++)
            {
                std::vector<__String> parameters2 = separateInfoWithMark(parameters[i], ",");
                
                levelAwardInfo  awardInfo ;
                
                awardInfo.midLevel = parameters2[0].intValue();
                awardInfo.rewards = parameters2[1].intValue();
                theUserInfo->userHeroMidLevelRewardVec.push_back(awardInfo);
            }
            value = dbResult[iIndex++];
            theUserInfo->m_skillUpgradeInfo = value.getCString();
            AnalyseSkillUpgradeInfo(theUserInfo->m_skillUpgradeInfo);
            
            CCLOG("技能升级信息 读取成功");
            
            value = dbResult[iIndex++];
            theUserInfo->taskTimeStamp = value.getCString();
            
            value = dbResult[iIndex++];
            theUserInfo->m_strHeroLv = value.getCString();
            
            //用户塔防模式通关星级信息
            value = dbResult[iIndex++];
            parameters = separateInfoWithMark(value, ",");
            for (int i = 0;i < parameters.size(); i++)
            {
                theUserInfo->userTowerLevelVec.push_back(parameters[i].intValue());
            }
            theUserInfo->towerLevel = theUserInfo->userTowerLevelVec.size();//通关数
            
            CCLOG("塔防星级 读取成功");
            
            value = dbResult[iIndex++];
            theUserInfo->DBVersion = value.intValue();
            
            //剧情进度
            value = dbResult[iIndex++];
            parameters = separateInfoWithMark(value, ",");
            for (int i = 0;i < parameters.size(); i++)
            {
                theUserInfo->storyInfo.push_back(parameters[i].intValue());
            }
            
            value = dbResult[iIndex++];
            theUserInfo->ifSuggesed = value.intValue();
            
            CCLOG("剧情 读取成功");
            
        }
    }
    sqlite3_free_table(dbResult);
}

void CDBMgr::loadPvpVIdoeInfo()
{
    theVideoListInfo.clear();
    sqlite3 *pDB = CDataBaseOper::getDB();
    int iResult;
    char *pErrMsg=NULL;
    char **dbResult;
    int iRow,iColum;
    int iIndex;
    iResult = sqlite3_get_table(pDB, "select * from videoInfo", &dbResult, &iRow, &iColum, &pErrMsg);
    if (SQLITE_OK==iResult) {
        
        iIndex = iColum;
        
        for (int i =0; i<iRow; i++) {
            
            videoListInfo info;
            __String value = "";
            
            value = dbResult[iIndex++];
            info.videoId = value.intValue();
            
            value = dbResult[iIndex++];
            info.enemyUserId = value.intValue();
            
            value = dbResult[iIndex++];
            info.enemyName = value.getCString();
            
            value = dbResult[iIndex++];
            info.enemyLevel = value.intValue();
            
            value = dbResult[iIndex++];
            std::vector<__String> parameters = separateInfoWithMark(value, ";");
            for (int i=0; i<parameters.size(); i++)
            {
                std::vector<__String> parameters2 = separateInfoWithMark(parameters[i], ",");
                IdAndTwoNum  tempInfo ;
                tempInfo.theId = parameters2[0].intValue();
                tempInfo.level = parameters2[1].intValue();
                tempInfo.number = parameters2[2].intValue();
                info.enemyInfo.push_back(tempInfo);
            }
            value = dbResult[iIndex++];
            info.robStone = value.intValue();
            
            value = dbResult[iIndex++];
            info.state = value.intValue();
            
            value = dbResult[iIndex++];
            info.uploadTime = value.intValue();
            
            value = dbResult[iIndex++];
            info.iWin = value.intValue();
            
            value = dbResult[iIndex++];
            info.iGetWard = value.intValue();
            
            value = dbResult[iIndex++];
            info.bValid = value.intValue();
            
            value = dbResult[iIndex++];
            info.iAttack = value.intValue();
            
            value = dbResult[iIndex++];
            info.destroyDegree = value.intValue();
            
            theVideoListInfo.push_back(info);
        }
    }
    sqlite3_free_table(dbResult);
}

void CDBMgr::loadBuildInfo()
{
    thePvpBuildInfo.clear();
    sqlite3 *pDB = CDataBaseOper::getDB();
    int iResult;
    char *pErrMsg=NULL;
    char **dbResult;
    int iRow,iColum;
    int iIndex;
    iResult = sqlite3_get_table(pDB, "select * from pvpBuildInfo", &dbResult, &iRow, &iColum, &pErrMsg);
    if (SQLITE_OK==iResult) {
        
        iIndex = iColum;
        
        for (int i =0; i<iRow; i++) {
            
            pvpBuildInfo info;
            __String value = "";
            
            value = dbResult[iIndex++];
            info.buildingID = value.intValue();
            
            value = dbResult[iIndex++];
            info.buildingType = value.intValue();
            
            value = dbResult[iIndex++];
            info.monsterId = value.intValue();
            
            value = dbResult[iIndex++];
            info.staticModelId = value.intValue();
            
            value = dbResult[iIndex++];
            info.texturePath = value.getCString();
            
            value = dbResult[iIndex++];
            info.activeModelId = value.intValue();
            
            value = dbResult[iIndex++];
            info.level = value.intValue();
            
            value = dbResult[iIndex++];
            info.cost = value.intValue();
            
            value = dbResult[iIndex++];
            info.costTime = value.intValue();
            
            value = dbResult[iIndex++];
            if(info.buildingType == 0)
            {
                std::vector<__String> parameters = separateInfoWithMark(value, ";");
                for (int i=0; i<parameters.size(); i++)
                {
                    std::vector<__String> parameters2 = separateInfoWithMark(parameters[i], ",");
                    levelBuildInfo  tempInfo ;
                    tempInfo.buitldType = parameters2[0].intValue();
                    tempInfo.num = parameters2[1].intValue();
                    tempInfo.level = parameters2[2].intValue();
                    info.allBuildLimitVec.push_back(tempInfo);
                }
            }
            else if(info.buildingType == 11)//祭坛
            {
                std::vector<__String> parameters = separateInfoWithMark(value, ";");
                for (int i=0; i<parameters.size(); i++)
                {
                    std::vector<__String> parameters2 = separateInfoWithMark(parameters[i], ",");
                    partolInfo tempPinfo;
                    tempPinfo.monsterId = parameters2[0].intValue();
                    tempPinfo.attack = parameters2[1].intValue();
                    tempPinfo.life = parameters2[2].intValue();
                    tempPinfo.monsterIcon = parameters2[3].getCString();
                    info.allEvolutionVec.push_back(tempPinfo);
                }
                
            }
            else
            {
                info.effectTime = value.intValue();
            }
            
            
            value = dbResult[iIndex++];
            std::vector<__String> parameters = separateInfoWithMark(value, ";");
            for (int i=0; i<parameters.size(); i++)
            {
                std::vector<__String> parameters2 = separateInfoWithMark(parameters[i], ",");
                Vec2  tempBomb ;
                tempBomb.x = parameters2[0].floatValue();
                tempBomb.y = parameters2[1].floatValue();
                info.blockPos.push_back(tempBomb);
            }
            
            value = dbResult[iIndex++];
            info.xPosType = value.intValue();
            
            value = dbResult[iIndex++];
            info.yPosType = value.intValue();
            
            value = dbResult[iIndex++];
            info.nextBuildingID = value.intValue();
            
            value = dbResult[iIndex++];
            info.life = value.intValue();
            
            value = dbResult[iIndex++];
            info.attack = value.intValue();
            
            value = dbResult[iIndex++];
            info.desc = value.getCString();
            
            value = dbResult[iIndex++];
            info.icon = value.getCString();
            
            value = dbResult[iIndex++];
            parameters = separateInfoWithMark(value, ",");
            for(int i = 0; i < parameters.size(); i++)
            {
                info.modelImgVec.push_back(parameters[i].getCString());
            }
            
            value = dbResult[iIndex++];
            info.useType = value.intValue();
            
            thePvpBuildInfo.push_back(info);
        }
    }
    sqlite3_free_table(dbResult);
}

void CDBMgr::loadChargeInfo()
{
    theChargeInfo.clear();
    
    sqlite3 *pDB = CDataBaseOper::getDB();
    int iResult;
    char *pErrMsg=NULL;
    char **dbResult;
    int iRow,iColum;
    int iIndex;
    iResult = sqlite3_get_table(pDB, "select * from chargeInfo", &dbResult, &iRow, &iColum, &pErrMsg);
    if (SQLITE_OK==iResult) {
        
        iIndex = iColum;
        
        for (int i =0; i<iRow; i++) {
            
            chargeInfo info;
            __String value = "";
            
            value = dbResult[iIndex++];
            info.chargeId = value.intValue();
            
            value = dbResult[iIndex++];
            info.icon = value.getCString();
            
            value = dbResult[iIndex++];
            info.desc = value.getCString();
            
            value = dbResult[iIndex++];
            info.num = value.intValue();
            
            value = dbResult[iIndex++];
            info.price = value.intValue();
            
            value = dbResult[iIndex++];
            info.type = value.intValue();
            
            value = dbResult[iIndex++];
            info.disCount = value.intValue();
            
            theChargeInfo.push_back(info);
        }
    }
    sqlite3_free_table(dbResult);
}

void CDBMgr::loadGuideProgress()
{
    
    thsGuideProInfo.clear();
    
    sqlite3 *pDB = CDataBaseOper::getDB();
    int iResult;
    char *pErrMsg=NULL;
    char **dbResult;
    int iRow,iColum;
    int iIndex;
    iResult = sqlite3_get_table(pDB, "select * from guideProgress", &dbResult, &iRow, &iColum, &pErrMsg);
    if (SQLITE_OK==iResult) {
        
        iIndex = iColum;
        
        for (int i =0; i<iRow; i++) {
            
            guidePro info;
            __String value = "";
            
            value = dbResult[iIndex++];
            info.typeId = value.intValue();
            
            value = dbResult[iIndex++];
            info.isEnd = value.intValue();
            
            thsGuideProInfo.push_back(info);
        }
    }
    sqlite3_free_table(dbResult);
}

void CDBMgr::loadGuiderInfo()
{
    
    thsGuiderInfo.clear();
    
    sqlite3 *pDB = CDataBaseOper::getDB();
    int iResult;
    char *pErrMsg=NULL;
    char **dbResult;
    int iRow,iColum;
    int iIndex;
    iResult = sqlite3_get_table(pDB, "select * from guideInfo", &dbResult, &iRow, &iColum, &pErrMsg);
    if (SQLITE_OK==iResult) {
        
        iIndex = iColum;
        
        for (int i =0; i<iRow; i++) {
            
            guideInfo info;
            __String value = "";
            
            
            value = dbResult[iIndex++];
            info.guideId = value.intValue();
            
            value = dbResult[iIndex++];
            info.type = value.intValue();
            
            
            value = dbResult[iIndex++];
            info.stepId = value.intValue();
            
            value = dbResult[iIndex++];
            info.icon = value.getCString();
            
            value = dbResult[iIndex++];
            if(value.compare("0"))
            {
                info.desc = value.getCString();
            }
            
            value = dbResult[iIndex++];
            info.limitType = value.intValue();
            
            thsGuiderInfo.push_back(info);
        }
        
    }
    
    sqlite3_free_table(dbResult);
}


void CDBMgr::loadUserNameInfo()
{
    m_theUserNameInfo_vec.clear();
    sqlite3 *pDB = CDataBaseOper::getDB();
    int iResult;
    char *pErrMsg=NULL;
    char **dbResult;
    int iRow,iColum;
    int iIndex;
    iResult = sqlite3_get_table(pDB, "select * from UserName", &dbResult, &iRow, &iColum, &pErrMsg);
    if (SQLITE_OK==iResult) {
        
        iIndex = iColum;
        
        for (int i =0; i<iRow; i++)
        {
            
            UserName info;
            __String value = "";
            value = dbResult[iIndex++];
            info.m_id = value.intValue();
            value = dbResult[iIndex++];
            
            info.m_sUserName1 = value.getCString();
            
            value = dbResult[iIndex++];
            info.m_sUserName2 =  value.getCString();
            m_theUserNameInfo_vec.push_back(info);
        }
    }
    sqlite3_free_table(dbResult);
}

void CDBMgr::loadGameStoryInnfo()
{
    theGameStoryInfo.clear();
    sqlite3 *pDB = CDataBaseOper::getDB();
    int iResult;
    char *pErrMsg=NULL;
    char **dbResult;
    int iRow,iColum;
    int iIndex;
    iResult = sqlite3_get_table(pDB, "select * from gameStory", &dbResult, &iRow, &iColum, &pErrMsg);
    if (SQLITE_OK==iResult) {
        
        iIndex = iColum;
        
        for (int i =0; i<iRow; i++) {
            
            gameStory info;
            __String value = "";
            
            iIndex++;
            
            value = dbResult[iIndex++];
            info.storyType = value.intValue();
            
            value = dbResult[iIndex++];
            info.levelId = value.intValue();
            
            value = dbResult[iIndex++];
            info.startTime = value.intValue();
            
            value = dbResult[iIndex++];
            info.step = value.intValue();
            
            value = dbResult[iIndex++];
            info.name = value.getCString();
            
            value = dbResult[iIndex++];
            info.icon = value.getCString();
            
            value = dbResult[iIndex++];
            info.content = value.getCString();
            
            value = dbResult[iIndex++];
            info.posType = value.intValue();
            theGameStoryInfo.push_back(info);
        }
    }
    sqlite3_free_table(dbResult);
}

void CDBMgr::loadFightBuildingInfo()
{
    
    m_vecFightBuildingInfo.clear();
    
    sqlite3 *pDB = CDataBaseOper::getDB();
    int iResult;
    char *pErrMsg=NULL;
    char **dbResult;
    int iRow,iColum;
    int iIndex;
    iResult = sqlite3_get_table(pDB, "select * from fightBuildingInfo", &dbResult, &iRow, &iColum, &pErrMsg);
    if (SQLITE_OK==iResult) {
        
        iIndex = iColum;
        
        for (int i =0; i<iRow; i++) {
            
            fightBuildingInfo* info = new fightBuildingInfo();
            __String value = "";
            
            
            value = dbResult[iIndex++];
            info->monsterId = value.intValue();
            
            value = dbResult[iIndex++];
            std::vector<__String> parameters = separateInfoWithMark(value, ",");
            for (int i = 0;i < parameters.size(); i++)
            {
                int temp = parameters[i].intValue();
                info->upgradeMonstersId.push_back(temp);
            }
            
            
            value = dbResult[iIndex++];
            info->towerType = value.intValue();
            
            
            value = dbResult[iIndex++];
            info->towerIcon = value.getCString();
            
            value = dbResult[iIndex++];
            info->buildCost = value.intValue();
            
            value = dbResult[iIndex++];
            info->saleGold = value.intValue();
        
            m_vecFightBuildingInfo.push_back(info);
        }
        
    }
    
    sqlite3_free_table(dbResult);
}


void CDBMgr::loadFightTalkingInfo(){
    
    
    m_vecFightTalkingInfo.clear();
    
    sqlite3 *pDB = CDataBaseOper::getDB();
    int iResult;
    char *pErrMsg=NULL;
    char **dbResult;
    int iRow,iColum;
    int iIndex;
    iResult = sqlite3_get_table(pDB, "select * from fightTalking", &dbResult, &iRow, &iColum, &pErrMsg);
    if (SQLITE_OK==iResult) {
        
        iIndex = iColum;
        
        for (int i =0; i<iRow; i++) {
            
            fightTalkingInfo* info = new fightTalkingInfo();
            __String value = "";
            
            
            value = dbResult[iIndex++];
            info->talkId = value.intValue();
            
            
            value = dbResult[iIndex++];
            info->type = value.intValue();
            
            
            value = dbResult[iIndex++];
            info->iconPath = value.getCString();
            
            value = dbResult[iIndex++];
            info->iconNum = value.intValue();
            
            value = dbResult[iIndex++];
            info->content = value.getCString();
            
            value = dbResult[iIndex++];
            info->nextTalkId = value.intValue();
            
            m_vecFightTalkingInfo.push_back(info);
        }
        
    }
    
    sqlite3_free_table(dbResult);
}


void CDBMgr::loadTmpSaveInfo()
{
    CDBMgr::OpenDB();
    
    sqlite3 *pDB = CDataBaseOper::getDB();

    char *pErrMsg = NULL;

    char **dbResult;
    int iRow,iColum;
    int iIndex = 0;
    int iResult = 0;
    
    iResult = sqlite3_get_table(pDB,"select * from TempSaveInfo",&dbResult,&iRow,&iColum,&pErrMsg);
    
    if(SQLITE_OK == iResult)
    {
        iIndex = iColum;
        m_vecTempSaveInfo.clear();
        for(int i=0;i<iRow;i++)
        {
             __String value = "";
            
            TempSaveInfo *info = new TempSaveInfo();
            
            value = dbResult[iIndex++];
            info->m_id =  value.intValue();
            
            value = dbResult[iIndex++];
            
            info->m_iCoinTime = value.intValue();
            
            value = dbResult[iIndex++];
            
            info->m_iStoneTime = value.intValue();
            
            value = dbResult[iIndex++];
            
            info->m_iCoinNum = value.intValue();
            
            value = dbResult[iIndex++];
            
            info->m_iStoneNum = value.intValue();
            
            value = dbResult[iIndex++];
            
            info->m_bFirstDrawCoin = value.intValue();
            
            
            value = dbResult[iIndex++];
            info->m_bFirstDrawStone = value.intValue();
            
            value = dbResult[iIndex++];
            if(value.compare("null"))
            {
                info->iHaveUsers = value.getCString();
            }
            
            m_vecTempSaveInfo.push_back(info);
            
            
        }
    
    }
    sqlite3_free_table(dbResult);
    
}



void CDBMgr::SaveDrawCardCommonNum(int iNum)
{
    CDBMgr::OpenDB();
    
    sqlite3 *pDB = CDataBaseOper::getDB();
    int iResult;
    char* pErrMsg = NULL;
    
    __String * temp = __String::createWithFormat("update TempSaveInfo set coinDrawNum=%d where id =1",iNum);
    
  
    SetTempCoinNum(iNum);
    
    iResult = sqlite3_exec(pDB, temp->getCString(), NULL, NULL, &pErrMsg);
    if(iResult != SQLITE_OK )
        CCLOG( "保存金币抽卡次数失败，错误码:%d ，错误原因:%s\n" , iResult, pErrMsg );
    CDataBaseOper::closeDB();
}



void CDBMgr::SaveDrawCardAdvanceNum(int iNum)
{
    CDBMgr::OpenDB();
    
    sqlite3 *pDB = CDataBaseOper::getDB();
    int iResult;
    char* pErrMsg = NULL;
    
    SetTempStoneNum(iNum);
    
    __String * temp = __String::createWithFormat("update TempSaveInfo set stoneDrawNum=%d where id =1",iNum);
    
    
    iResult = sqlite3_exec(pDB, temp->getCString(), NULL, NULL, &pErrMsg);
    if(iResult != SQLITE_OK )
        CCLOG( "保存金币抽卡次数失败，错误码:%d ，错误原因:%s\n" , iResult, pErrMsg );
    CDataBaseOper::closeDB();
}


void CDBMgr::SaveDrawCardCommonTime(int iNowTime)
{
    
    CDBMgr::OpenDB();
    
    sqlite3 *pDB = CDataBaseOper::getDB();
    int iResult;
    char* pErrMsg = NULL;
    
    __String * temp = __String::createWithFormat("update TempSaveInfo set coindrawtime=%d where id =1",iNowTime);
      //std::vector<TempSaveInfo*>  pSave =   CDBMgr::getInstance()->GetTempSaveInfo();
    
    CDBMgr::getInstance()->SetTempCoinTime(iNowTime);
    
                                            
    iResult = sqlite3_exec(pDB, temp->getCString(), NULL, NULL, &pErrMsg);
    if(iResult != SQLITE_OK )
        CCLOG( "保存魔法石抽卡次数失败，错误码:%d ，错误原因:%s\n" , iResult, pErrMsg );
    CDataBaseOper::closeDB();

}


void CDBMgr::SaveDrawCardAdvanceTime(int iNowTime)
{
    CDBMgr::OpenDB();
    
    sqlite3 *pDB = CDataBaseOper::getDB();
    int iResult;
    char* pErrMsg = NULL;
    
    __String * temp = __String::createWithFormat("update TempSaveInfo set stonedrawtime=%d where id =1",iNowTime);
    
    CDBMgr::getInstance()->SetTempStoneTime(iNowTime);
    
    iResult = sqlite3_exec(pDB, temp->getCString(), NULL, NULL, &pErrMsg);
    if(iResult != SQLITE_OK )
        CCLOG( "保存魔法石抽卡时间失败，错误码:%d ，错误原因:%s\n" , iResult, pErrMsg );
    CDataBaseOper::closeDB();
    
}




void CDBMgr::loadConfigure()
{
    CDBMgr::OpenDB();
    
    sqlite3 *pDB = CDataBaseOper::getDB();
    char *pErrMsg=NULL;
    char **dbResult;
    int iRow,iColum;
    int iIndex = 0;
    int iResult = 0;
    

    iResult = sqlite3_get_table(pDB, "select * from configure", &dbResult, &iRow, &iColum, &pErrMsg);
    if (SQLITE_OK==iResult) {
        
        iIndex = iColum;
        
        m_vecConfigureInfo.clear();
        
        for (int i =0; i<iRow; i++)
        {
            
           ConfigureInfo * info = new ConfigureInfo();
            __String value = "";
            
            value = dbResult[iIndex++];
            
            info->m_id = value.intValue();
            
            value = dbResult[iIndex++];
            
            info->m_iDrawOneCoin = value.intValue();
            
            value = dbResult[iIndex++];
            info->m_iDrawOneStone = value.intValue();
            
            value = dbResult[iIndex++];
            info->m_iDrawTenCoin = value.intValue();
        
            value = dbResult[iIndex++];
            info->m_iDrawTenStone = value.intValue();
            
            value = dbResult[iIndex++];
            info->m_iDrawOneTime = value.intValue();
            
            value = dbResult[iIndex++];
            info->m_iDrawTenTime = value.intValue();
            
            value = dbResult[iIndex++];
            info->m_iDrawOneNum = value.intValue();
            
            value = dbResult[iIndex++];
            info->m_iDrawMagicNum = value.intValue();

            
            m_vecConfigureInfo.push_back(info);
        
        }
        
    }
    
    sqlite3_free_table(dbResult);
    
}


void CDBMgr::SaveHeroLv(std::string str)
{
    CDBMgr::OpenDB();
    
    sqlite3 *pDB = CDataBaseOper::getDB();
    int iResult;
    char* pErrMsg = NULL;
    
    __String * temp = __String::createWithFormat("update user set heroLv='%s' where userid =%d",str.c_str(),CPersonalData::getInstance()->getUserInfo()->m_iUserId);
    
    iResult = sqlite3_exec(pDB, temp->getCString(), NULL, NULL, &pErrMsg);
    if(iResult != SQLITE_OK )
        CCLOG( "保存用户等级失败，错误码:%d ，错误原因:%s\n" , iResult, pErrMsg );
    CDataBaseOper::closeDB();
}


void CDBMgr::AnalyseHeroLvInfo(std::string str)
{
    int ipos = 0;
    
    string sz = str;
    
    m_HeroLvInfo_vec.clear();
    
    if(str.empty())
    {
        return;
    }
    
    int i =0;
    int k1 = 0;
    int k2 = 0;
    
    do
    {
        ipos =  sz.find(";");
        
        string str;
        
        /*if(ipos== -1)
        {
            break;
        }
        else*/
        {
            str = sz.substr(0,ipos);
        }
        
        size_t ipos1 = 0;
        
        for(int j=0;j<2;j++)
        {
            if(j==1)
            {
                k2 = atoi(str.c_str());
                skillLvInfo_s tt;
                tt.m_iHeroid = k1;
                tt.m_iLv = k2;
                m_HeroLvInfo_vec.push_back(tt);
                break;
            }
            
            ipos1=  str.find(",");
            if(j==0)
            {
                if(ipos1!=-1)
                {
                    k1 = atoi(str.substr(0,ipos1).c_str());
                }
            }
            str = str.substr(ipos1+1);
        }
        i++;
        sz = sz.substr(ipos+1);
        
    }while(ipos!=-1);

}

void CDBMgr::AnalyseSkillUpgradeInfo(std::string str)
{
    int ipos = 0;
    
    string sz = str;
    
    m_SkillUpgradeInfo_vec.clear();
    
    if(str.empty())
    {
        return;
    }
    
    int i =0;
    int k1 = 0;
    int k2 = 0;
    int k3= 0;
    int k4 = 0;
    do
    {
        ipos =  sz.find(";");
        
        string str;

        if(ipos == -1)
        {
            break;
        }
        else
        {
            str = sz.substr(0,ipos);
        }
        
        size_t ipos1 = 0;
        
        for(int j=0;j<4;j++)
        {
            if(j==1)
            {
                k2 = atoi(str.c_str());
            }
            
            if(j==2)
            {
                k3 = atoi(str.c_str());
                
               
            }
            
            if(j==3)
            {
                k4 = atoi(str.c_str());
                
                MonsterSkillTime_s tt;
                tt.m_iMonsterId = k1;
                tt.m_iSkillId  = k2;
                tt.m_iTime = k3;
                tt.m_iNeedTime = k4;
                m_SkillUpgradeInfo_vec.push_back(tt);
                break;
            }
            
            ipos1=  str.find(",");
            if(j==0)
            {
                if(ipos1!=-1)
                {
                    k1 = atoi(str.substr(0,ipos1).c_str());
                }
            }
            str = str.substr(ipos1+1);
        }
        i++;
        sz = sz.substr(ipos+1);
        
    }while(ipos!=-1);
}


void CDBMgr::SaveSkillUpgradeInfoToDB(std::string str)
{
    CDBMgr::OpenDB();
    sqlite3 *pDB = CDataBaseOper::getDB();
    char buffer[128] = {0};
    int iResult;
    char* pErrMsg = NULL;
    memset(buffer, 0, sizeof(buffer));
    {
        sprintf(buffer, "update user set skillUpgradeInfo='%s' where userid =%d",str.c_str(),
                CPersonalData::getInstance()->getUserInfo()->m_iUserId);//修改语句
    }
    
    iResult = sqlite3_exec(pDB, buffer, NULL, NULL, &pErrMsg);
    if(iResult != SQLITE_OK )
        CCLOG( "保存用户技能升级信息失败，错误码:%d ，错误原因:%s\n" , iResult, pErrMsg );
    CDataBaseOper::closeDB();
}

void CDBMgr::SaveEnergyLvTosDB(std::string str)
{
    CDBMgr::OpenDB();
    
    sqlite3 *pDB = CDataBaseOper::getDB();
    int iResult;
    char* pErrMsg = NULL;
    __String * temp = __String::createWithFormat("update user set energyLv='%s' where userid =%d",str.c_str(),
                                                 CPersonalData::getInstance()->getUserInfo()->m_iUserId);
    iResult = sqlite3_exec(pDB, temp->getCString(), NULL, NULL, &pErrMsg);
    if(iResult != SQLITE_OK )
        CCLOG( "保存用户能量等级信息失败，错误码:%d ，错误原因:%s\n" , iResult, pErrMsg );
    CDataBaseOper::closeDB();

}

void CDBMgr::SaveBuidlingLvToDB(std::string str)
{
    CDBMgr::OpenDB();
    
    sqlite3 *pDB = CDataBaseOper::getDB();
    int iResult;
    char* pErrMsg = NULL;
    
    __String * temp = __String::createWithFormat("update user set buildingLv='%s' where userid =%d",str.c_str(),
                                                 CPersonalData::getInstance()->getUserInfo()->m_iUserId);
    iResult = sqlite3_exec(pDB, temp->getCString(), NULL, NULL, &pErrMsg);
    if(iResult != SQLITE_OK )
        CCLOG( "保存用户建筑等级信息失败，错误码:%d ，错误原因:%s\n" , iResult, pErrMsg );
    CDataBaseOper::closeDB();
    
}


void  CDBMgr::SaveBuidlingLvTostr(int id,int lv)
{
    map<int,int>::iterator it = m_BuildingLv_map.begin();
    
    for(;it!=m_BuildingLv_map.end();it++)
    {
        if(it->first == id)
        {
            it->second = lv;
            break;
        }
    }
    
    char str[100] = {0};
    it = m_BuildingLv_map.begin();
    
    for(int i =0;it!=m_BuildingLv_map.end();it++,i++)
    {
        size_t iLen = strlen(str);
        
        if(i==m_BuildingLv_map.size()-1)
        {
             sprintf(str+iLen,"%d,%d",it->first,it->second);
        }
        else
        {
            sprintf(str+iLen,"%d,%d;",it->first,it->second);
        }
    }
    
    log("strlen = %s\n",str);
    SaveBuidlingLvToDB(str);
}

void  CDBMgr::SaveEnergyLvTostr(int id,int lv)
{
    map<int,int>::iterator it = m_EneygyLv_map.begin();
    
    bool bExist = false;
    
    for(;it!=m_EneygyLv_map.end();it++)
    {
        if(it->first == id)
        {
            it->second = lv;
            bExist = true;
            break;
        }
    }
    
    if(!bExist)
    {
        m_EneygyLv_map[id] = lv;
    }
    
    char str[100] = {0};
    
    it = m_EneygyLv_map.begin();
    
    for(int i =0;it!=m_EneygyLv_map.end();it++,i++)
    {
        size_t iLen = strlen(str);
        
        if(i==m_EneygyLv_map.size()-1)
        {
            sprintf(str+iLen,"%d,%d",it->first,it->second);
        }
        else
        {
            sprintf(str+iLen,"%d,%d;",it->first,it->second);
        }
        
    }
    
    log("strlen = %s\n",str);
    
    SaveEnergyLvTosDB(str);
}



void CDBMgr::FengeStr(std::string str,int&id,int &SkillId)
{
    int iPos = str.find("_");
    
    std::string str1 = str.substr(0,iPos);
    std::string str2 = str.substr(iPos+1,str.length());
    id = atoi(str1.c_str());
    SkillId = atoi(str2.c_str());
}


void CDBMgr::AddSkillUpgradeToStr(int id,int iSKillId,int iTime,int iNeedTime)
{
    char strIdSkillId[50] = {0};
    sprintf(strIdSkillId,"%d_%d",id,iSKillId);
    
    MonsterSkillTime_s tt;
    tt.m_iMonsterId = id;
    tt.m_iSkillId  = iSKillId;
    tt.m_iTime = iTime;
    tt.m_iNeedTime = iNeedTime;
    
    m_SkillUpgradeInfo_vec.push_back(tt);

    
    std::vector<MonsterSkillTime_s>::iterator itVec =  m_SkillUpgradeInfo_vec.begin();
    
    char str[100] = {0};

    for(;itVec!=m_SkillUpgradeInfo_vec.end();itVec++)
    {
        size_t iLen = strlen(str);
        sprintf(str+ iLen,"%d,%d,%d,%d;",(itVec)->m_iMonsterId,(itVec)->m_iSkillId,itVec->m_iTime,itVec->m_iNeedTime);
    }
    
    SaveSkillUpgradeInfoToDB(str);
}



void CDBMgr::AddMonsterSkillLvOne(int iMonsterId,int iSkillId)
{
    std::vector<FighterInfo*> pVec = CPersonalData::getInstance()->getAllSoldierInfo();
    
    for(int i=0;i<pVec.size();i++)
    {
        if(pVec[i]->monsterId== iMonsterId)
        {
            FighterInfo* p =  pVec[i];
            
            for(int j=0;j<p->theFighterSkillInfo.size();j++)
            {
                
                if(p->theFighterSkillInfo[j].theSkillInfo->skillId == iSkillId)
                {
                     p->theFighterSkillInfo[j].skillLevel++;
                }
            }
        }
    }
    assert(-1);
    
}

int CDBMgr::GetMonsterSKillLv(int iMonsterId,int iSkillId)
{
    std::vector<FighterInfo*> pVec = CPersonalData::getInstance()->getAllSoldierInfo();
    
    for(int i=0;i<pVec.size();i++)
    {
        if(pVec[i]->monsterId== iMonsterId)
        {
            FighterInfo* p =  pVec[i];
            
            for(int j=0;j<p->theFighterSkillInfo.size();j++)
            {
                
                if(p->theFighterSkillInfo[j].theSkillInfo->skillId == iSkillId)
                {
                    return p->theFighterSkillInfo[j].skillLevel;
                }
            }
        }
    }
    assert(-1);
    return -1;
}



int CDBMgr::GetMonsterSkillIndex(int iMonsterId,int iSkillId)
{
    int index = -1;
    
    std::vector<FighterInfo*> pVec = CPersonalData::getInstance()->getAllSoldierInfo();
    
    for(int i=0;i<pVec.size();i++)
    {
        if(pVec[i]->monsterId== iMonsterId)
        {
            FighterInfo* p =  pVec[i];
            
            for(int j=0;j<p->theFighterSkillInfo.size();j++)
            {
                index++;
                if(p->theFighterSkillInfo[j].theSkillInfo->skillId == iSkillId)
                {
                    return index;
                }
            }
        }
    }
    
    assert(-1);
    
    return -1;
}


void CDBMgr::AddSkillIdTostr(std::string strMonsSkillId)
{
    int index = strMonsSkillId.find("_");
    int iMonsterId  = 0;
    
    std::string str = strMonsSkillId.substr(index+1,strMonsSkillId.length());
    
    std::string strMonster = strMonsSkillId.substr(0,index);
    
    iMonsterId = atoi(strMonster.c_str());
    
    bool bHero = false;
    
    if(iMonsterId>1000)
    {
        bHero = true;
    }
    
    int  iSkillId = atoi(str.c_str());
    
    std::string strSKillName;
    std::vector<heroSkill*> pHeroSkillInfo = CDBMgr::getInstance()->GetHeroSkillInfo();
    

    std::string  strImg;
    
   if(bHero)
    {
        int iHeroSkillIndex = CDBMgr::getInstance()->GetHeroSkillIndex(iSkillId);
        strSKillName =  pHeroSkillInfo[iHeroSkillIndex]->heroSkillName;
        /*CPersonalData::getInstance()->getUserInfo()->m_heroSkillsVec[iHeroSkillIndex].theNumber++;
        std::vector<IdAndNum> pIdNumVec = CPersonalData::getInstance()->getUserInfo()->m_heroSkillsVec;
        char strSkillPath[100] = {0};
        
        strImg = pHeroSkillInfo[iHeroSkillIndex]->skillIcon;
        
        for(int i=0;i<pIdNumVec.size();i++)
        {
            sprintf(strSkillPath+strlen(strSkillPath),"%d,%d;",pIdNumVec[i].theId
                    ,pIdNumVec[i].theNumber);
        }
        
        userInfoInDB* userInfoDB = CDataManager::getTheUSerInfDB();
        
        userInfoDB->heroSkills = pIdNumVec;
        
        CDBMgr::getInstance()->SaveHeroSkillInfoToDb(strSkillPath);*/
        
    }
    else
    {
    
        int iIndex = CDBMgr::getInstance()->GetMonsterSkillIndex(iMonsterId,iSkillId);

        monsterInfo* pMonsterInfo = CDataManager::getMonsterInfoByMonsterId(iMonsterId);
        
        myMonster * pMyMonster = CDataManager::getuserMonsterById(iMonsterId);
    
        if(iIndex == 0)
        {
            pMyMonster->skillLevel1++;
        }
        else if(iIndex == 1)
        {
            pMyMonster->skillLevel2++;
        }
        else if(iIndex == 2)
        {
            pMyMonster->skillLevel3++;
        }
        else if(iIndex == 3)
        {
            pMyMonster->skillLevel4++;
        }

        FighterInfo* pFightInfo = CPersonalData::getInstance()->refreshSoldierInfoWithInfo(pMyMonster);
        skillInfo* pInfo = CDataManager::getTheSkillInfoWithId(iSkillId);
        strImg = pInfo->skillIcon;
        strSKillName = pInfo->skillName;
        
        //保存到数据库
        CDBMgr::getInstance()->SaveUserMonsterSkillToDB(iMonsterId,iIndex+1,pFightInfo->theFighterSkillInfo[iIndex].skillLevel);
    }
    
}

void CDBMgr::DelSkillUpgradeToStr(std::string str1)
{
    vector<MonsterSkillTime_s>::iterator itVec = m_SkillUpgradeInfo_vec.begin();
    
    int id = 0;
    int iSkillId = 0;
    
    FengeStr(str1, id,iSkillId);
    
    for(int i=0;itVec!=m_SkillUpgradeInfo_vec.end();itVec++)
    {
        if(id == itVec->m_iMonsterId && iSkillId == itVec->m_iSkillId)
        {
            m_SkillUpgradeInfo_vec.erase(itVec);
            
            break;
        }
    }
    
    char str[100] = {0};
    
    itVec = m_SkillUpgradeInfo_vec.begin();
    
    for(int i=0;itVec!=m_SkillUpgradeInfo_vec.end();itVec++,i++)
    {
        size_t iLen = strlen(str);
        int id = (itVec)->m_iMonsterId;
        int iSkillId = (itVec)->m_iSkillId;
        int iTime = itVec->m_iTime;
        if(i==m_SkillUpgradeInfo_vec.size()-1)
        {
            sprintf(str+iLen,"%d,%d,%d",id,iSkillId,iTime);
        }
        else
        {
            sprintf(str+iLen,"%d,%d,%d;",id,iSkillId,iTime);
        }
    }
    
    SaveSkillUpgradeInfoToDB(str);
}



void CDBMgr::AnalyseSkillUpgradeNeedItem(std::string str)
{
    int ipos = 0;
    string sz = str;
  
    m_SkillUpgradeItem_map.clear();
    int i =0;
    int k1 = 0;
    
    int k2 = 0;
    
    do
    {
        ipos =  sz.find(";");
        string str = sz.substr(0,ipos);
        size_t ipos1 = 0;
        
        for(int j=0;j<2;j++)
        {
            if(j==1)
            {
                k2 = atoi(str.c_str());
                m_SkillUpgradeItem_map[k1] = k2;
                
                break;
            }
            
            ipos1=  str.find(",");
            
            if(ipos1!=-1)
            {
                k1 =  atoi(str.substr(0,ipos1).c_str());
                
            }
            str = str.substr(ipos1+1);
        }
        i++;
        sz = sz.substr(ipos+1);
    }while(ipos!=-1);
    
    
}


void CDBMgr::AnalyseStringFromBuilding(std::string strBuilding)
{
    int ipos = 0;
    string sz = strBuilding;
    m_BuildingLv_map.clear();
    
    int i =0;
    int k1 = 0;

    int k2 = 0;
    
    do
    {
        ipos =  sz.find(";");
        string str = sz.substr(0,ipos);
        size_t ipos1 = 0;
        
        for(int j=0;j<2;j++)
        {
            if(j==1)
            {
                k2 = atoi(str.c_str());
                m_BuildingLv_map[k1] = k2;
                
                break;
            }
            
            ipos1=  str.find(",");
            
            if(ipos1!=-1)
            {
                k1 =  atoi(str.substr(0,ipos1).c_str());
            
            }
            str = str.substr(ipos1+1);
        }
        i++;
        sz = sz.substr(ipos+1);
    }while(ipos!=-1);
    
}
void CDBMgr::AnalyseStringFromEnergy(std::string strEnergy)
{
    int ipos = 0;
    
    m_EneygyLv_map.clear();
    string sz = strEnergy;
    
    int i =0;
    int k1 = 0;
    int k2 = 0;
    
    do
    {
        ipos =  sz.find(";");
        string str = sz.substr(0,ipos);
        size_t ipos1 = 0;
        
        for(int j=0;j<2;j++)
        {
            if(j==1)
            {
                k2  =  atoi(str.c_str());
                m_EneygyLv_map[k1] = k2;
                break;
            }

            ipos1=  str.find(",");
            
            if(ipos1!=-1)
            {
               k1 =  atoi(str.substr(0,ipos1).c_str());
                
            }
            str = str.substr(ipos1+1);
        }
        i++;
        sz = sz.substr(ipos+1);
    }while(ipos!=-1);
}

void CDBMgr::SetUserInfo(userInfoInDB * pUserInfo)
{
    theUserInfo = pUserInfo;
}

int CDBMgr::GetHeroSkillMaxLvById(int iMonsterId)
{
    //theHeroSkillInfo
    
    for(int i=0;i<theHeroSkillInfo.size();i++)
    {
        if(iMonsterId == theHeroSkillInfo[i]->m_iMonsterId)
        {

            return theHeroSkillInfo[i]->levelMax;
        }
    }
    return 0;
}

int CDBMgr::GetHeroSkillLvById(int iSkillHeroId)
{
    userInfoInDB* userInfoDB = CDataManager::getTheUSerInfDB();
    std::vector<IdAndNum> heroSkills = userInfoDB->heroSkills;
    
    for(int i=0;i<heroSkills.size();i++)
    {
        if(heroSkills[i].theId == iSkillHeroId)
        {
            return heroSkills[i].theNumber;
        }
    }
    return 0;
}


void CDBMgr::saveUserDebrisToDB(int debrisId, int num, int type)
{
    //先遍历用户是否有该碎片，如果有累加，如果没有追加
    bool isexisted = false;//是否有碎片
    int debrisNum = 0 ;
    for(int i = 0; i < UserMonsterInfo.size(); i++)
    {
        if(UserMonsterInfo[i]->monsterId == debrisId)
        {
            isexisted = true;
            if(type == 0)
            {
                UserMonsterInfo[i]->num += num;
            }
            else{
                UserMonsterInfo[i]->num -= num;
            }
            debrisNum = UserMonsterInfo[i]->num;
        }
    }
    if(!isexisted)
    {
        insertIntoMonsterInfo(debrisId, num);
    }
    else
    {
        CDBMgr::OpenDB();
        sqlite3 *pDB = CDataBaseOper::getDB();
        int iResult;
        char* pErrMsg = NULL;
        __String * temp = __String::createWithFormat("update userMonsterInfo set num=%d where monsterId =%d", debrisNum, debrisId);
        iResult = sqlite3_exec(pDB, temp->getCString(), NULL, NULL, &pErrMsg);
        if(iResult != SQLITE_OK )
            CCLOG( "保存用户关卡碎片失败，错误码:%d ，错误原因:%s\n" , iResult, pErrMsg );
        CDataBaseOper::closeDB();
    }
}



void CDBMgr::SaveHeroSkillInfoToDb(string strHero)
{
    CDBMgr::OpenDB();
    sqlite3  *pDB = CDataBaseOper::getDB();
    int iResult;
    char *pErrMsg = NULL;
    
    __String *temp = __String::createWithFormat("update user set HeroSkillLevel='%s' where userid =%d",strHero.c_str(),CPersonalData::getInstance()->getUserInfo()->m_iUserId);

    iResult = sqlite3_exec(pDB, temp->getCString(), NULL, NULL, &pErrMsg);
    if(iResult != SQLITE_OK )
        CCLOG( "保存英雄技能信息失败，错误码:%d ，错误原因:%s\n" , iResult, pErrMsg );
    CDataBaseOper::closeDB();
}


void  CDBMgr::SaveMonsterSKillLv(int iMonsterId,int iSkillLv1,int iSkillLv2,
int iSkillLv3,int iSkillLv4)
{
    
    CDBMgr::OpenDB();
    sqlite3 *pDB = CDataBaseOper::getDB();
    int iResult;
    char* pErrMsg = NULL;
    
    __String * temp = __String::createWithFormat("update userMonsterInfo set skillLevel1=%d,skillLevel2=%d,skillLevel3=%d,skillLevel4=%d where monsterId =%d",iSkillLv1,iSkillLv2,iSkillLv3,iSkillLv4,iMonsterId);
    
    
    iResult = sqlite3_exec(pDB, temp->getCString(), NULL, NULL, &pErrMsg);
    if(iResult != SQLITE_OK )
        CCLOG( "保存用户技能等级信息失败，错误码:%d ，错误原因:%s\n" , iResult, pErrMsg );
    CDataBaseOper::closeDB();

    
}


void CDBMgr::saveMonsterStarToDB(int monsterId,int rare)
{
    bool bExisted = false;
    //判断当前是否已有该怪物类型
    for(int i = 0; i < UserMonsterInfo.size(); i++)
    {
        if(UserMonsterInfo.at(i)->monsterId == monsterId)
        {
            bExisted = true;
            UserMonsterInfo[i]->rare = rare;
            
            
            //没更新一阶初始化对应技能为1
            if(rare == 1)
            {
                UserMonsterInfo[i]->skillLevel1 = 1;
            }
            
            else if(rare == 2)
            {
                UserMonsterInfo[i]->skillLevel2 = 1;
            }
            else  if(rare == 3)
            {
                UserMonsterInfo[i]->skillLevel3 = 1;
            }
            
            else if(rare == 4)
            {
                UserMonsterInfo[i]->skillLevel4 = 1;
            }
        
            for(int j = 0;j < CPersonalData::getInstance()->m_vecMyFighterInfo.size(); j++)
            {
                if(CPersonalData::getInstance()->m_vecMyFighterInfo[j]->monsterId == monsterId)
                {

                    for(int i=0;i<CPersonalData::getInstance()->m_vecMyFighterInfo[j]->theFighterSkillInfo.size();i++)
                    {
                        if(i<=rare)
                        {
                            CPersonalData::getInstance()->m_vecMyFighterInfo[j]->theFighterSkillInfo[i].skillLevel = 1;
                        }
                        
                    }
                }
            }
            SaveMonsterSKillLv(UserMonsterInfo.at(i)->monsterId,UserMonsterInfo[i]->skillLevel1,UserMonsterInfo[i]->skillLevel2,UserMonsterInfo[i]->skillLevel3,UserMonsterInfo[i]->skillLevel4);
            
            break;
        }
    }
    if(bExisted)
    {
        CDBMgr::OpenDB();
        sqlite3 *pDB = CDataBaseOper::getDB();
        int iResult;
        char* pErrMsg = NULL;

        __String * temp = __String::createWithFormat("update userMonsterInfo set monsterRare=%d where monsterId =%d", rare, monsterId);
        iResult = sqlite3_exec(pDB, temp->getCString(), NULL, NULL, &pErrMsg);
        if(iResult != SQLITE_OK )
            CCLOG( "保存用户关卡信息失败，错误码:%d ，错误原因:%s\n" , iResult, pErrMsg );
        CDataBaseOper::closeDB();
        CDataManager::setMonsterStarById(monsterId);//更新玩家本地怪物星级
        
        if(rare == 3)//保存星级为3的英雄数量
        {
            CDataManager::saveTaskInfoToDB(13);
        }
        else if(rare == 4)
        {
            CDataManager::saveTaskInfoToDB(14);
        }
    }
    else{
        insertIntoMonsterInfo(monsterId, 1, 1);
    }
    
}

void CDBMgr::saveUserStoneAndCoinToDB(int num, int type, bool ifAdd)
{
    CDBMgr::OpenDB();
    num *= (ifAdd == false)?1:-1;
    if(ifAdd)
    {
        SimpleAudioEngine::getInstance()->playEffect("music/getMoney.mp3");
    }
    sqlite3 *pDB = CDataBaseOper::getDB();
    int iResult;
    char* pErrMsg = NULL;
    __String * temp = NULL;
    if(type == 0){
        temp = __String::createWithFormat("update user set Coin='%s' where userid =%d", CPersonalData::DataIntToStr(CPersonalData::getInstance()->getUserCoin() - num).c_str() ,
                                          CPersonalData::getInstance()->getUserInfo()->m_iUserId);
        CPersonalData::getInstance()->setUserCoin(CPersonalData::getInstance()->getUserCoin() - num);
        
        //CNetWorkDataSingle::GetSingletonPtr()->SaveUserMCF(NULL,NULL,-num,0,0);

    }
    else if(type == 1){
        temp = __String::createWithFormat("update user set MagicStone='%s' where userid =%d", CPersonalData::DataIntToStr(CPersonalData::getInstance()->getUserStone() - num).c_str(),
                                          CPersonalData::getInstance()->getUserInfo()->m_iUserId);
        //修改本地数据
        CPersonalData::getInstance()->setUserStone(CPersonalData::getInstance()->getUserStone() - num);
        
        //CNetWorkDataSingle::GetSingletonPtr()->SaveUserMCF(NULL,NULL,0,-num,0);
        
        if(!ifAdd)
        {
            CDataManager::saveTaskInfoToDB(17, num);//记录消耗的魔法石数量
        }
    }
    
    
    iResult = sqlite3_exec(pDB, temp->getCString(), NULL, NULL, &pErrMsg);
    if(iResult != SQLITE_OK )
        CCLOG( "保存用户信息失败，错误码:%d ，错误原因:%s\n" , iResult, pErrMsg );
    CDataBaseOper::closeDB();
    CPersonalData::getInstance()->setUpdateUserInfo(true);
}


std::map<int,int> CDBMgr::GetBuidlingMap()
{
    return m_BuildingLv_map;
}

BuildingUpgradeInfo * CDBMgr::GetBuidlingUpgradeInfo(int iBuidlingId,int iLevel)
{
    for(int i=0;i<m_pBuildingUpgradeInfo.size();i++)
    {
        if(m_pBuildingUpgradeInfo[i]->m_iBuildingUpgradeId== iBuidlingId && m_pBuildingUpgradeInfo[i]->m_iUpgradeLevel == iLevel)
        {
            return m_pBuildingUpgradeInfo[i];
        }
    }
    return NULL;
}


EnergyUpgradeInfo* CDBMgr::getEnergyUpgradeInfo(int iEnergyId,int iLevl)
{
    for(int i=0;i<m_pEnergyUpgradeInfo.size();i++)
    {
        if(m_pEnergyUpgradeInfo[i]->m_iEnergyUpgradeId == iEnergyId &&m_pEnergyUpgradeInfo[i]->m_iUpgradeLevel == iLevl)
        {
            //影响值
            return m_pEnergyUpgradeInfo[i];
        }
    }
    return  NULL;
}


int CDBMgr::getAffectNum(eAffectType eType,int iId)
{
    
    int iLv1 =  CDBMgr::getInstance()->GetEnergyLvById(iId);

    
    int iBLv1 =  CDBMgr::getInstance()->GetBuidlingLvById(iId);
 
    
    
    if(eType == eBuildingType)
    {
        for(int i=0;i<m_pBuildingUpgradeInfo.size();i++)
        {
            if(m_pBuildingUpgradeInfo[i]->m_iBuildingUpgradeId == iId&&m_pBuildingUpgradeInfo[i]->m_iUpgradeLevel == iBLv1)
            {
                return m_pBuildingUpgradeInfo[i]->m_iAffectNum;
            }
        }
    }
    
    else if(eType == eEnergyType)
    {
        for(int i=0;i<m_pEnergyUpgradeInfo.size();i++)
        {
            if(m_pEnergyUpgradeInfo[i]->m_iEnergyUpgradeId == iId &&m_pEnergyUpgradeInfo[i]->m_iUpgradeLevel == iLv1)
            {
                return m_pEnergyUpgradeInfo[i]->m_iAffectNum;
            }
        }
    }
    
    return 0;
}

int  CDBMgr::GetBuidlingLvById(int id)
{
    std::map<int,int>::iterator it = m_BuildingLv_map.begin();
    
    int iNum = 0;
    for(;it!=m_BuildingLv_map.end();it++)
    {
        if(it->first == id)
        {
            iNum  = it->second;
            break;
        }
    }
    return iNum;
}


int CDBMgr::GetEnergyLvById(int id)
{
    std::map<int,int>::iterator it = m_EneygyLv_map.begin();
    
    int iNum = 1;
    for(;it!=m_EneygyLv_map.end();it++)
    {
        if(it->first == id)
        {
            iNum  = it->second;
            break;
        }
    }
    return iNum;
}


std::map<int,int> CDBMgr::GetEnergyMap()
{
    return m_EneygyLv_map;
}

std::vector<ShopItemInfo*> CDBMgr::GetUserShopInfo()
{
    return m_ShopInfo;
}


int  CDBMgr::GeShopPropPrize(int id)
{
    for(int i=0;i<m_ShopInfo.size();i++)
    {
        if(id == m_ShopInfo[i]->m_iItemId )
            return  m_ShopInfo[i]->m_iPrize ;
    }
    return 10; //找不到的暂时10魔法石一个
}
std::vector<TempSaveInfo*> CDBMgr::GetTempSaveInfo()
{
    return m_vecTempSaveInfo;
}


std::vector<DrawCardResultInfo*> CDBMgr::GetDrawCardResultInfo()
{
    return m_pvecDrawCardResultInfo;
}



int  CDBMgr::GetAllMonsterNum()
{
    int iNum = 0;

    std::vector<monsterInfo*> theModleInfo = CDataManager::getAllMonsterInfo();
    for(int i = 0; i < theModleInfo.size(); i++)
    {
        if(theModleInfo.at(i)->isHaveDebris == 0)//代表显示碎片
        {
            iNum++;
        }
    }
    return iNum;
    
}



void CDBMgr::SetTempCoinTime(int iTime)
{
    m_vecTempSaveInfo[0]->m_iCoinTime = iTime;
}

void CDBMgr::SetTempCoinNum(int iNum)
{
    m_vecTempSaveInfo[0]->m_iCoinNum = iNum;
}

void CDBMgr::SetTempStoneNum(int iNum)
{
    m_vecTempSaveInfo[0]->m_iStoneNum = iNum;
}

void CDBMgr::SetTempStoneTime(int iTime)
{
    m_vecTempSaveInfo[0]->m_iStoneTime = iTime;
}



modelInfo* CDBMgr::GetModleInfoById(int id)
{

    for(int i=0;i<theModleInfo.size();i++)
    {
        
        if(theModleInfo[i]->modelId ==id)
        {
            return theModleInfo[i];
        }
    }
    return NULL;
    
}




void CDBMgr::saveTempUserInfo(int userId)
{
    CDBMgr::OpenDB();
    sqlite3 *pDB = CDataBaseOper::getDB();
    int iResult;
    char* pErrMsg = NULL;
    __String* pBuf = __String::createWithFormat("%d", userId);
    if(!m_vecTempSaveInfo[0]->iHaveUsers.empty())
    {
        m_vecTempSaveInfo[0]->iHaveUsers += ",";
    }
    m_vecTempSaveInfo[0]->iHaveUsers += pBuf->getCString();
    
    std::string content;
    std::vector<__String> sortVec;
    
    std::vector<__String> tempVec = separateInfoWithMark(m_vecTempSaveInfo[0]->iHaveUsers, ",");
    tempVec.push_back(pBuf->getCString());
    
    if(tempVec.size() > 30)
    {
        int index = (int)tempVec.size() - 30;
        for(int i = index; i < (int)tempVec.size(); i++)
        {
            sortVec.push_back(tempVec[i]);
        }
    }
    else
    {
        sortVec = tempVec;
    }
    for(int i = 0; i < sortVec.size(); i++)
    {
        content += sortVec[i].getCString();
        if( i < sortVec.size() - 1)
        {
            content += ",";
        }
    }
    __String * temp = __String::createWithFormat("update TempSaveInfo set playerUserId ='%s' where id =1", content.c_str());
    iResult = sqlite3_exec(pDB, temp->getCString(), NULL, NULL, &pErrMsg);
    if(iResult != SQLITE_OK )         CCLOG( "保存用户表全信息失败，错误码:%d ，错误原因:%s\n" , iResult, pErrMsg );
    CDataBaseOper::closeDB();
}

std::string CDBMgr::getTempAllUser()
{
    return m_vecTempSaveInfo[0]->iHaveUsers;
}

std::map<int,int> CDBMgr::GetSkillUpgradeItemMap()
{
    return  m_SkillUpgradeItem_map;
}


bool CDBMgr::HaveSkillUpgradeInfo(int id,int iSkillId)
{
    vector<MonsterSkillTime_s>::iterator it = m_SkillUpgradeInfo_vec.begin();
    
    for(;it!=m_SkillUpgradeInfo_vec.end();it++)
    {
        if((*it).m_iMonsterId== id && (*it).m_iSkillId == iSkillId)
        {
            return true;
        }
    }
    return  false;
}


vector<CDBMgr::MonsterSkillTime_s>  CDBMgr::GetSkillUpgradeInfoVec()
{
    return m_SkillUpgradeInfo_vec;
}

int CDBMgr::GetHeroSkillIndex(int iHeroId)
{
    for(int i=0;i<theHeroSkillInfo.size();i++)
    {
        if(theHeroSkillInfo[i]->heroSkillId == iHeroId)
        {
            return i;
        }
    }
    return -1;
}


std::vector<heroSkill*>  CDBMgr::GetHeroSkillInfo()
{
    return theHeroSkillInfo;
}

void CDBMgr::judgeUserIfUpgrade(Node * pTarget, int exp,int *resultAddStamina)
{
    int userLevel= CPersonalData::getInstance()->getUserInfo()->m_iUserLevel;
    int userExp = CPersonalData::getInstance()->getUserInfo()->m_iUserExp + exp;
    int index = userLevel;
    int addStamina = 0;
    for (int i = userLevel; i < 100; i++) {
        int temp = getUpgradeInfoByLevel(i + 1)->userExp;
        if (userExp < temp)
        {
            index = i;
            break;
        }
        else{
            addStamina += getUpgradeInfoByLevel(i)->upgradeStamina;
        }
    }
    saveUserInfo(CPersonalData::getInstance()->GetPhysicalNum() + addStamina, index, userExp);
    
    *resultAddStamina = addStamina;
    
    if (index > userLevel)
    {
        CPersonalData::getInstance()->setJudgeUpgrade(true);
        CDataManager::saveTaskInfoToDB(11, index, true);
        //更新玩家体力
        CommonPropLayer* layer = CommonPropLayer::create(LEVEL_UPGRADE_TYPE, userLevel, index);
        pTarget->addChild(layer, 100);
        layer->runAction(Sequence::create(Hide::create(), DelayTime::create(0.5f), Show::create(), NULL));
        SimpleAudioEngine::getInstance()->playEffect("music/victory.mp3");
    }
    else{
        CPersonalData::getInstance()->setJudgeUpgrade(false);
    }
    CPersonalData::getInstance()->setUpdateUserInfo(true);
}

upgradeInfo* CDBMgr::getUpgradeInfoByLevel(int level)
{
    for (int i = 0; i < thsUserUpgradeInfo.size(); i++) {
        if(thsUserUpgradeInfo.at(i).level == level)
            return &thsUserUpgradeInfo.at(i);
    }
    return NULL;
}


void CDBMgr::SaveUserInfo(UserInfo *pInfo)
{
    //UserInfo* userInfo = CPersonalData::getInstance()->getUserInfo();
    
    CDBMgr::OpenDB();
    sqlite3 *pDB = CDataBaseOper::getDB();
    int iResult;
    char* pErrMsg = NULL;
    __String * temp = __String::createWithFormat("update user set Name ='%s',Level=%d,Exp=%d,LoginDay='%s',staminaTime=%d,buildingLv='%s',energyLv='%s' where userid =%d",pInfo->userName.c_str(),pInfo->m_iUserLevel,pInfo->m_iUserExp,pInfo->m_sloginDay.c_str(),pInfo->m_iStaminTime,pInfo->m_strBudilingLv.c_str(),pInfo->m_strEnergyLv.c_str(),pInfo->m_iUserId);
    iResult = sqlite3_exec(pDB, temp->getCString(), NULL, NULL, &pErrMsg);
    if(iResult != SQLITE_OK )
        CCLOG( "保存用户表全信息失败，错误码:%d ，错误原因:%s\n" , iResult, pErrMsg );
    CDataBaseOper::closeDB();
}


void CDBMgr::saveUserStamina(int iStamina)
{
    CDBMgr::OpenDB();
    sqlite3 *pDB = CDataBaseOper::getDB();
    char buffer[128] = {0};
    int iResult;
    char* pErrMsg = NULL;
    memset(buffer, 0, sizeof(buffer));
    
    UserInfo* userInfo = CPersonalData::getInstance()->getUserInfo();
    sprintf(buffer, "update user set stamina='%s' where userid =%d",CPersonalData::DataIntToStr(iStamina).c_str(), userInfo->m_iUserId);//修改语句
    
    iResult = sqlite3_exec(pDB, buffer, NULL, NULL, &pErrMsg);
    if(iResult != SQLITE_OK )
        CCLOG( "保存体力表信息失败，错误码:%d ，错误原因:%s\n" , iResult, pErrMsg );
    CDataBaseOper::closeDB();
}


void CDBMgr::saveUserStaminaTime(long iStaminaTime)
{
    CDBMgr::OpenDB();
    sqlite3 *pDB = CDataBaseOper::getDB();
    int iResult;
    char* pErrMsg = NULL;
    
    UserInfo* userInfo = CPersonalData::getInstance()->getUserInfo();
    __String * temp = __String::createWithFormat("update user set staminaTime=%ld where userid =%d",iStaminaTime, userInfo->m_iUserId);
    iResult = sqlite3_exec(pDB, temp->getCString(), NULL, NULL, &pErrMsg);
    if(iResult != SQLITE_OK )
        CCLOG( "保存体力时间表信息失败，错误码:%d ，错误原因:%s\n" , iResult, pErrMsg );
    CDataBaseOper::closeDB();
}

void CDBMgr::saveUserInfo(int stamina,int level, int exp,int staminaTime)
{
    //判断本次消耗是否导致体力值不满，如果是则保存体力的首次使用时间
    if((stamina < CDataManager::getUpgradeInfoByLevel(level)->upperStamina) && (CPersonalData::getInstance()->getIfStaminaFull()))
    {
        CPersonalData::getInstance()->setIfStaminaFull(false);
        //保存当前使用体力的时间
        
        CPhysicalSystem::GetSingletonPtr()->SavePhysicalTime();
    }
    
    if (stamina >= CDataManager::getUpgradeInfoByLevel(level)->upgradeStamina) {
        CPersonalData::getInstance()->setIfStaminaFull(true);
    }
    
//    else{
//        CPersonalData::getInstance()->setIfStaminaFull(true);
//    }
    
    //判断是否升级
    if (level>CPersonalData::getInstance()->getUserInfo()->m_iUserLevel) {
        __String * tempStr = __String::createWithFormat("%d",CPersonalData::getInstance()->getUserInfo()->m_iUserId);
        TDCCAccount* account = TDCCAccount::setAccount(tempStr->getCString());
        account->setLevel(level);
    }
    
    UserInfo* userInfo = CPersonalData::getInstance()->getUserInfo();
    CPersonalData::getInstance()->SetPhysicalNum(stamina);
    
    //CNetWorkDataSingle::GetSingletonPtr()->SaveUserMCF(NULL,NULL,0,0, stamina - CPersonalData::getInstance()->GetPhysicalNum());
    

    CPersonalData::getInstance()->setPhysicalMaxNum(CDataManager::getUpgradeInfoByLevel(level)->upperStamina);
    CPersonalData::getInstance()->getUserInfo()->m_iUserExp = exp;
    CPersonalData::getInstance()->getUserInfo()->m_iUserLevel = level;
    //CPersonalData::getInstance()->getUserInfo()->m_i
    
    CDBMgr::OpenDB();
    sqlite3 *pDB = CDataBaseOper::getDB();
    int iResult;
    char* pErrMsg = NULL;
    __String * temp = NULL;
    if(staminaTime==-1)
    {
        temp = __String::createWithFormat("update user set Level=%d,Exp=%d,stamina='%s' where userid =%d", level, exp, CPersonalData::getInstance()->getUserInfo()->m_sStaminaNum.c_str(), userInfo->m_iUserId);
    }
    else
    {
        temp = __String::createWithFormat("update user set Level=%d,Exp=%d,stamina='%s',staminaTime=%d where userid =%d", level, exp,
                                                     CPersonalData::getInstance()->getUserInfo()->m_sStaminaNum.c_str(),staminaTime, userInfo->m_iUserId);
    }
    
    iResult = sqlite3_exec(pDB, temp->getCString(), NULL, NULL, &pErrMsg);
    if(iResult != SQLITE_OK )
        CCLOG( "保存用户表信息失败，错误码:%d ，错误原因:%s\n" , iResult, pErrMsg );
    CDataBaseOper::closeDB();
    CPersonalData::getInstance()->setUpdateUserInfo(true);

}

void CDBMgr::saveUserTowerLevelInfoToDB(int level)
{
    UserInfo* userInfo = CPersonalData::getInstance()->getUserInfo();
    std::vector<int> towerLevelVec = userInfo->m_userTowerLevelVec;
    bool isNewLevel = true;//标识是否是新关卡
    for(int i = 0; i < towerLevelVec.size(); i++)
    {
        if(towerLevelVec[i] == level)
        {
            isNewLevel = false;
            break;
        }
    }
    //更新通关数
    if(isNewLevel)
    {
        towerLevelVec.push_back(level);
        userInfo->m_towerLevel++;
    }
    userInfo->m_userTowerLevelVec = towerLevelVec;
    CPersonalData::getInstance()->setUserInfo(userInfo);
    UserInfo* Info = CPersonalData::getInstance()->getUserInfo();
    std::string value;
    char buf[32];
    for (int i = 0; i < towerLevelVec.size(); i++) {
        sprintf(buf, "%d", towerLevelVec.at(i));
        value += buf;
        if(i != towerLevelVec.size() - 1){
            value += ",";
        }
    }
    CDBMgr::OpenDB();
    sqlite3 *pDB = CDataBaseOper::getDB();
    int iResult;
    char* pErrMsg = NULL;
    __String * temp = NULL;
    temp = __String::createWithFormat("update user set towerStarInfo=\'%s\' where userid=%d", value.c_str(), userInfo->m_iUserId);
    iResult = sqlite3_exec(pDB, temp->getCString(), NULL, NULL, &pErrMsg);
    if(iResult != SQLITE_OK )
        CCLOG( "保存用户关卡信息失败，错误码:%d ，错误原因:%s\n" , iResult, pErrMsg );
    CDataBaseOper::closeDB();
}

void CDBMgr::saveUserInfoToDB(int level, int rare, bool isEqlite)
{
    UserInfo* userInfo = CPersonalData::getInstance()->getUserInfo();
    std::vector<levelAndStar> userLevelVec = userInfo->m_userLevelVec;
    bool isNewLevel = true;//标识是否是新关卡
    if (isEqlite) {
        userLevelVec = userInfo->m_userEqliteLevelVec;
    }
    for(int i = 0;i < userLevelVec.size(); i++)
    {
        if(userLevelVec.at(i).level == level)
        {
            userLevelVec.at(i).star = rare;
            isNewLevel = false;
            break;
        }
    }
    //更新通关数
    if(isNewLevel)
    {
        if(rare > 0)
        {
            CPersonalData::getInstance()->setisOpenNewSmallLevel(true);
        }
        if(!isEqlite)
        {
            CDataManager::saveTaskInfoToDB(9);
        }
        else{
            CDataManager::saveTaskInfoToDB(10);
        }
        levelAndStar info;
        info.level = level;
        info.star = rare;
        userLevelVec.push_back(info);
        if(isEqlite){
            userInfo->m_iEqliteSmallLevel++;
        }
        else{
            userInfo->m_iSmallLevel++;
        }
    }

    if (isEqlite) {
        userInfo->m_userEqliteLevelVec = userLevelVec;
    }
    else{
        userInfo->m_userLevelVec = userLevelVec;
    }
    CPersonalData::getInstance()->setUserInfo(userInfo);
    UserInfo* Info = CPersonalData::getInstance()->getUserInfo();
    std::string value;
    char buf[32];
    for (int i = 0; i < userLevelVec.size(); i++) {
        sprintf(buf, "%d", userLevelVec.at(i).level);
        value += buf;
        value += ",";
        sprintf(buf, "%d", userLevelVec.at(i).star);
        value += buf;
        if(i != userLevelVec.size() - 1){
            value += ";";
        }
    }
    
    
    CDBMgr::OpenDB();
    sqlite3 *pDB = CDataBaseOper::getDB();
    int iResult;
    char* pErrMsg = NULL;
    __String * temp = NULL;
    if (isEqlite) {
        temp = __String::createWithFormat("update user set heroStarInfo=\'%s\' where userid=%d",value.c_str(), userInfo->m_iUserId);
    }
    else{
        temp = __String::createWithFormat("update user set starInfo=\'%s\' where userid=%d", value.c_str(), userInfo->m_iUserId);
    }
    iResult = sqlite3_exec(pDB, temp->getCString(), NULL, NULL, &pErrMsg);
    if(iResult != SQLITE_OK )
        CCLOG( "保存用户关卡信息失败，错误码:%d ，错误原因:%s\n" , iResult, pErrMsg );
    CDataBaseOper::closeDB();
}

int CDBMgr::getMidLevelAwardById(int midLevel, bool ifEqlite)
{
    int temp = 0;
    std::vector<levelAwardInfo> awardInfo = CPersonalData::getInstance()->getUserInfo()->m_userMidLevelRewardVec;
    if(ifEqlite)
    {
        awardInfo = CPersonalData::getInstance()->getUserInfo()->m_userHeroMidLevelRewardVec;
    }
    for (int i = 0; i < awardInfo.size(); i++) {
        if(awardInfo.at(i).midLevel == midLevel){
             temp = awardInfo.at(i).rewards;
            break;
        }
    }
    return temp;
}

int CDBMgr::getAllLevelStarById(int midLevelId, bool ifEqlite)
{
    int allStar = 0;
    midLevelInfo* pInfo = NULL;
    int smallLevel = CPersonalData::getInstance()->getUserInfo()->m_iSmallLevel;//获取小关卡通关数
    if(ifEqlite)
    {
        //如果获取精英关卡的星级，先判断当前关卡的普通关卡是否已经全部三星，否则直接返回0  
        pInfo = getMidLevelInfoById(midLevelId);
        if(smallLevel < pInfo->lastLevel)
        {
            return allStar;
        }
        smallLevel = CPersonalData::getInstance()->getUserInfo()->m_iEqliteSmallLevel;
        
    }
    int index = 0;
    
    for (int i = 1; i <= midLevelId; i++) {
        pInfo = getMidLevelInfoById(i);
        if ((smallLevel > pInfo->firstLevel) && (smallLevel < pInfo->lastLevel)) {
            index = i;
            break;
        }
    }
    if (index == 0) {
        index = midLevelId;
        
    }
    pInfo = getMidLevelInfoById(index);

    for (int i = pInfo->firstLevel; (i <= smallLevel) &&(i <= pInfo->lastLevel); i++) {
        allStar += getStarBySmallLevelId(i, ifEqlite);
    }
    return allStar;
}

void CDBMgr::insertIntoMonsterInfo(int monsterId, int num, int rare, int ifFight)
{
    myMonster *info = new myMonster;
    info->monsterId = monsterId;
    info->monsterLevel = 1;
    info->skillLevel1 = 1;
    info->skillLevel2 = 0;
    info->skillLevel3 = 0;
    info->skillLevel4 = 0;
    info->ifFight = ifFight;
    info->theType = 0;
    info->rare = rare;
    info->num = num;
    UserMonsterInfo.push_back(info);
    
    FighterInfo *targetInfo = new FighterInfo;
    targetInfo->skillFrameNum=0;
    targetInfo->level = info->monsterLevel;
    targetInfo->ifFight = ifFight;
    CDataManager::CalcFighterInfoWith(targetInfo, info->monsterId, info->monsterLevel, info->rare, info->theType,
                                      info->ifFight, info->skillLevel1,info->skillLevel2,info->skillLevel3,info->skillLevel4);
    CPersonalData::getInstance()->m_vecMyFighterInfo.push_back(targetInfo);
    if((targetInfo->theType == 0)&& (targetInfo->ifFight == 1))
    {
        CPersonalData::getInstance()->m_vecMyTeamInfo.push_back(targetInfo);
    }

    CDBMgr::OpenDB();
    sqlite3 *pDB = CDataBaseOper::getDB();
    int iResult;
    char* pErrMsg = NULL;
    __String * temp = __String::createWithFormat("insert into userMonsterInfo values(%d,1,%d,%d,%d,%d,%d,0,%d,%d)",monsterId, info->skillLevel1,
                                                 info->skillLevel2, info->skillLevel3, info->skillLevel4, ifFight, rare, num);
    iResult = sqlite3_exec(pDB, temp->getCString(), NULL, NULL, &pErrMsg);
    if(iResult != SQLITE_OK )
        CCLOG( "保存用户关卡信息失败，错误码:%d ，错误原因:%s\n" , iResult, pErrMsg );
    CDataBaseOper::closeDB();
    
}

int CDBMgr::getStarBySmallLevelId(int smaLevelId, bool ifEqlite)
{
    std::vector<levelAndStar> userLevelVec = CPersonalData::getInstance()->getUserInfo()->m_userLevelVec;
    if (ifEqlite) {
        userLevelVec = CPersonalData::getInstance()->getUserInfo()->m_userEqliteLevelVec;
    }
    if(userLevelVec.size() > 0){
        for (int i = 0; i < userLevelVec.size(); i++) {
            if (smaLevelId == userLevelVec.at(i).level) {
                return userLevelVec.at(i).star;
            }
        }

    }
    return 0;
}




int CDBMgr::getAllLevelById(int midLevelId)
{
    for (int i = 0; i < midLevelInfoVec.size(); i++) {
        if (midLevelInfoVec.at(i).midLevelId == midLevelId) {
            return (midLevelInfoVec.at(i).lastLevel - midLevelInfoVec.at(i).firstLevel + 1);
        }
    }
    return 0;
}

midLevelInfo* CDBMgr::getMidLevelInfoById(int midLevelId)
{
    for (int i = 0; i < midLevelInfoVec.size(); i++)
    {
        if (midLevelId == midLevelInfoVec.at(i).midLevelId) {
            return &midLevelInfoVec.at(i);
        }
    }
    return NULL;
}

void CDBMgr::saveUserAwardInfo(int midLevelId, int num, bool isEqlite)
{
    
    UserInfo* userInfo = CPersonalData::getInstance()->getUserInfo();
    std::vector<levelAwardInfo> userAwardVec = userInfo->m_userMidLevelRewardVec;
    bool isNewLevel = true;//标识是否是新关卡
    if (isEqlite) {
        userAwardVec = userInfo->m_userHeroMidLevelRewardVec;
    }
    for(int i = 0;i < userAwardVec.size(); i++)
    {
        if(userAwardVec.at(i).midLevel == midLevelId)
        {
            userAwardVec.at(i).rewards = num;
            isNewLevel = false;
            break;
        }
    }
    //更新通关数
    if(isNewLevel)
    {
        levelAwardInfo info;
        info.midLevel = midLevelId;
        info.rewards = num;
        userAwardVec.push_back(info);
    }
    
    if (isEqlite) {
        userInfo->m_userHeroMidLevelRewardVec = userAwardVec;
    }
    else{
        userInfo->m_userMidLevelRewardVec = userAwardVec;
    }
    CPersonalData::getInstance()->setUserInfo(userInfo);
    std::string value;
    char buf[32];
    for (int i = 0; i < userAwardVec.size(); i++) {
        sprintf(buf, "%d", userAwardVec.at(i).midLevel);
        value += buf;
        value += ",";
        sprintf(buf, "%d", userAwardVec.at(i).rewards);
        value += buf;
        if(i != userAwardVec.size() - 1){
            value += ";";
        }
    }
    CDBMgr::OpenDB();
    sqlite3 *pDB = CDataBaseOper::getDB();
    int iResult;
    char* pErrMsg = NULL;
    __String * temp = __String::createWithFormat("update user set awardInfo=\'%s\' where userid =%d",value.c_str(), userInfo->m_iUserId);
    if(isEqlite)
    {
        temp = __String::createWithFormat("update user set heroAwardInfo=\'%s\' where userid =%d",value.c_str(), userInfo->m_iUserId);
    }
    iResult = sqlite3_exec(pDB, temp->getCString(), NULL, NULL, &pErrMsg);
    if(iResult != SQLITE_OK )
        CCLOG( "保存用户关卡信息失败，错误码:%d ，错误原因:%s\n" , iResult, pErrMsg );
    CDataBaseOper::closeDB();
}

void CDBMgr::saveUserTeamToDB(int monsterId, int iFight)
{
    std::vector<FighterInfo*> tempVec = CPersonalData::getInstance()->m_vecMyFighterInfo;
    for(int i = 0; i < tempVec.size(); i++){
        if(CPersonalData::getInstance()->getSoliderType() == tempVec[i]->monsterType)
        {
            if(tempVec[i]->monsterId == monsterId)
            {
                tempVec[i]->ifFight = iFight;
                break;
            }
        }
    }
    
    CPersonalData::getInstance()->m_vecMyFighterInfo = tempVec;
    CDBMgr::OpenDB();
    sqlite3 *pDB = CDataBaseOper::getDB();
    int iResult;
    char* pErrMsg = NULL;
    __String * temp = __String::createWithFormat("update userMonsterInfo set ifFight=%d where monsterId =%d", iFight, monsterId);
    iResult = sqlite3_exec(pDB, temp->getCString(), NULL, NULL, &pErrMsg);
    if(iResult != SQLITE_OK )
        CCLOG( "保存士兵出战失败，错误码:%d ，错误原因:%s\n" , iResult, pErrMsg );
    CDataBaseOper::closeDB();
}

float CDBMgr::getExpInfoFromDB(int level)
{
    for(int i = 0; i < thsUserUpgradeInfo.size(); i++)
    {
        if(thsUserUpgradeInfo[i].level == level)
        {
            return thsUserUpgradeInfo[i].upgradeExp;
        }
    }
    return 0;
}

starInfo* CDBMgr::getStarInfoById(int starId)
{
    for(int i = 0; i < m_levelStarInfo.size(); i++)
    {
        if (m_levelStarInfo.at(i).starId == starId) {
            return &m_levelStarInfo.at(i);
        }
    }
    return NULL;
}

std::vector<int> CDBMgr::getStarConditionById(int levelId)
{
    for(int i = 0; i < theLevelInfo.size(); i++)
    {
        if (theLevelInfo.at(i)->levelId == levelId) {
            return theLevelInfo.at(i)->starIds;
        }
    }
}

std::vector<int> CDBMgr::getItemPathById(int propId, int eQlite)
{
    std::vector<int> levelVec;
    if(eQlite)
    {
        for(int i = 0; i < theLevelInfo.size(); i++)
        {
            for(int j= 0; j < theLevelInfo[i]->heroItemsVec.size(); j++)
            {
                if((theLevelInfo[i]->heroItemsVec[j].Id == propId) && (theLevelInfo[i]->heroItemsVec[j].type == 0))
                {
                    levelVec.push_back(theLevelInfo[i]->levelId);
                    break;
                }
            }
        }
    }
    else
    {
        for(int i = 0; i < theLevelInfo.size(); i++)
        {
            for(int j= 0; j < theLevelInfo[i]->itemsVec.size(); j++)
            {
                if((theLevelInfo[i]->itemsVec[j].Id == propId) && (theLevelInfo[i]->itemsVec[j].type == 0))
                {
                    levelVec.push_back(theLevelInfo[i]->levelId);
                    break;
                }
            }
        }
    }
    return levelVec;
}

propInfo* CDBMgr::getPropInfoById(int propId)
{
    for(int i = 0; i < m_propInfo.size(); i++)
    {
        if (m_propInfo.at(i)->m_itemId == propId) {
            return m_propInfo.at(i);
        }
    }
    return NULL;
}



std::string CDBMgr::getPropImgByid(int propId)
{
    
    for(int i = 0; i < m_propInfo.size(); i++)
    {
        if (m_propInfo.at(i)->m_itemId == propId) {
            return m_propInfo[i]->m_sItemIcon;
        }
    }
    return "";
}


void CDBMgr::loadDrawCardResult()
{
    m_pvecDrawCardResultInfo.clear();
    
    sqlite3 *pDB = CDataBaseOper::getDB();
    int iResult;
    char *pErrMsg=NULL;
    char **dbResult;
    int iRow,iColum;
    int iIndex;
    
    iResult = sqlite3_get_table(pDB, "select * from drawCardResult", &dbResult, &iRow, &iColum, &pErrMsg);
    
    if (SQLITE_OK==iResult)
    {
        
        iIndex = iColum;
        
        for (int i =0; i<iRow; i++) {
            
            DrawCardResultInfo * info = new DrawCardResultInfo;
            __String value = "";
            
            value = dbResult[iIndex++];
            info->m_id = value.intValue();
    
            value = dbResult[iIndex++];
            info->m_iType = value.intValue();
            
            value = dbResult[iIndex++];
            info->m_iGotItem = value.intValue();
            
            value = dbResult[iIndex++];
            info->m_iGotNum = value.intValue();
        
            
            value = dbResult[iIndex++];
            info->m_sRadio = value.getCString();
            
            value = dbResult[iIndex++];
            info->m_iGotMoney = value.intValue();
            
            value = dbResult[iIndex++];
            info->m_bCoin  = value.intValue()==1?true:false;
            
            m_pvecDrawCardResultInfo.push_back(info);
            
        }
        
    }
    

    sqlite3_free_table(dbResult);
}



void CDBMgr::loadItemOpenInfoDB()
{
    m_pPropOpenInfo.clear();
    
    sqlite3 *pDB = CDataBaseOper::getDB();
    int iResult;
    char *pErrMsg=NULL;
    char **dbResult;
    int iRow,iColum;
    int iIndex;
    
    iResult = sqlite3_get_table(pDB, "select * from ItemOpen", &dbResult, &iRow, &iColum, &pErrMsg);
    
    if (SQLITE_OK==iResult)
    {
        
        iIndex = iColum;
        
        for (int i =0; i<iRow; i++) {
            
            PropOpenInfo* info = new PropOpenInfo;
            __String value = "";
            
            value = dbResult[iIndex++];
            info->m_id = value.intValue();
            
            value = dbResult[iIndex++];
            info->m_iUserItem = value.intValue();
            
            value = dbResult[iIndex++];
            info->m_iGotItem = value.intValue();
            
            value = dbResult[iIndex++];
            info->m_itemType = value.intValue();
            
            value = dbResult[iIndex++];
            info->m_iGotItemNum = value.intValue();
            
            value = dbResult[iIndex++];
            info->m_iGotMoney = value.intValue();
            
            value = dbResult[iIndex++];
            info->m_iGotStone = value.intValue();
            
            value = dbResult[iIndex++];
            info->m_sRatio = value.getCString();
            
            m_pPropOpenInfo.push_back(info);
            
        }
        
    }
    
    sqlite3_free_table(dbResult);
    
}




vector<heroSkill*> CDBMgr::getHeroSkillByMonsterId(int iMonsterId)
{

    theCurHeroSkillInfo.clear();
    
    for(int i=0;i<theHeroSkillInfo.size();i++)
    {
        if(iMonsterId == theHeroSkillInfo[i]->m_iMonsterId)
        {

            //if(theHeroSkillInfo[i]->heroSkillId>1)
            {
            theCurHeroSkillInfo.push_back(theHeroSkillInfo[i]);
            }
        }
    }
 
    return theCurHeroSkillInfo;
}


void CDBMgr::loadHeroSkillInfoDB()
{
    
    theHeroSkillInfo.clear();
    
    sqlite3 *pDB = CDataBaseOper::getDB();
    int iResult;
    char *pErrMsg=NULL;
    char **dbResult;
    int iRow,iColum;
    int iIndex;
    
    iResult = sqlite3_get_table(pDB, "select * from heroSkillInfo", &dbResult, &iRow, &iColum, &pErrMsg);
    
    if (SQLITE_OK==iResult) {
        
        iIndex = iColum;
        
        for (int i =0; i<iRow; i++) {
            
            heroSkill *info = new heroSkill;
            __String value = "";
            
            
            value = dbResult[iIndex++];
            info->heroSkillId = value.intValue();
            
          
            value = dbResult[iIndex++];
            info->m_iMonsterId = value.intValue();
            
            value = dbResult[iIndex++];
            info->heroSkillName = value.getCString();
            
            value = dbResult[iIndex++];
            info->heroSkillDes = value.getCString();
            
            value = dbResult[iIndex++];
            info->damageMin = value.floatValue();
            
            value = dbResult[iIndex++];
            info->damageMax = value.floatValue();

            value = dbResult[iIndex++];
            info->lastTimeMin = value.intValue();

            value = dbResult[iIndex++];
            info->lastTimeMax = value.intValue();

            value = dbResult[iIndex++];
            info->skillCD = value.intValue();

            value = dbResult[iIndex++];
            info->costEnergy = value.intValue();

            value = dbResult[iIndex++];
            info->skillIcon = value.getCString();
            
            value = dbResult[iIndex++];
            info->levelLimit = value.intValue();

            value = dbResult[iIndex++];
            info->levelMax = value.intValue();

            
            theHeroSkillInfo.push_back(info);
        }
        
    }
    
    sqlite3_free_table(dbResult);
    
}

void CDBMgr::loadBulletDB(){
    
    
    
    
    theBulletInfo.clear();
    
    sqlite3 *pDB = CDataBaseOper::getDB();
    int iResult;
    char *pErrMsg=NULL;
    char **dbResult;
    int iRow,iColum;
    int iIndex;
    
    iResult = sqlite3_get_table(pDB, "select * from bulletInfo", &dbResult, &iRow, &iColum, &pErrMsg);
    
    if (SQLITE_OK==iResult) {
        
        iIndex = iColum;
        
        for (int i =0; i<iRow; i++) {
            
            bulletInfoDB *info = new bulletInfoDB;
            __String value = "";
            
            
            value = dbResult[iIndex++];
            info->bulletId = value.intValue();
            
            value = dbResult[iIndex++];
            info->filePath = value.getCString();

            value = dbResult[iIndex++];
            info->bulletType = value.intValue();

            value = dbResult[iIndex++];
            info->bulletMinRange = value.intValue();

            value = dbResult[iIndex++];
            info->bulletMaxRange = value.intValue();

            value = dbResult[iIndex++];
            info->bulletDamageRange = value.intValue();

            value = dbResult[iIndex++];
            info->lifeTime = value.floatValue();
            
            value = dbResult[iIndex++];
            info->bulletSpeed = value.floatValue();

            value = dbResult[iIndex++];
            info->hitEffect = value.getCString();
            
            value = dbResult[iIndex++];
            info->particlePath = value.getCString();
            
            value = dbResult[iIndex++];
            info->bulletScale = value.floatValue();
            
            value = dbResult[iIndex++];
            std::vector<__String> parameters = separateInfoWithMark(value, ",");
            if (parameters.size()==2) {
                info->hurtAnimate.animatePath=parameters[0].getCString();
                info->hurtAnimate.animateNumber=parameters[1].intValue();
            }else{
                CCLOG("LYAssert: bulletInfoDB wrong at bulletId = %d",info->bulletId);
            }
            
            
            theBulletInfo.push_back(info);
        }
        
    }
    
    sqlite3_free_table(dbResult);
    
}

void CDBMgr::loadTipsInfo(){
    
    m_vecTipsInfo.clear();
    
    sqlite3 *pDB = CDataBaseOper::getDB();
    int iResult;
    char *pErrMsg=NULL;
    char **dbResult;
    int iRow,iColum;
    int iIndex;
    
    iResult = sqlite3_get_table(pDB, "select * from TipsInfo", &dbResult, &iRow, &iColum, &pErrMsg);
    
    if (SQLITE_OK==iResult) {
        
        iIndex = iColum;
        
        for (int i =0; i<iRow; i++) {
            
            tipsInfo *info = new tipsInfo;
            __String value = "";
            
            
            value = dbResult[iIndex++];
            info->tipsId = value.intValue();
            
            value = dbResult[iIndex++];
            info->content = value.getCString();
            
            
            m_vecTipsInfo.push_back(info);
        }
        
    }
    
    sqlite3_free_table(dbResult);

}


std::vector<__String> CDBMgr::separateInfoWithMark(cocos2d::__String originStr, cocos2d::__String Mark){
    
    
    std::vector<__String> targetVec;
    
    std::string tempStr = originStr.getCString();
    if(tempStr == "null")
    {
        return targetVec;
    }
    __String value = "";
    
    unsigned long middle = tempStr.find(Mark.getCString());
    
    while (middle!=-1) {
        
        value = tempStr.substr(0,middle);
        
        targetVec.push_back(value);
        
        tempStr = tempStr.substr(middle+1,-1);
        
        if (tempStr.length()>0) {
            middle = tempStr.find(Mark.getCString());
        }else{
            break;
        }
    }
    
    if (tempStr.length()>0) {
        targetVec.push_back(tempStr);
    }
    
    return targetVec;
    
}

void CDBMgr::saveUserAllInfo()
{
    UserInfo* pUserInfo = CPersonalData::getInstance()->getUserInfo();
    CDBMgr::OpenDB();
    sqlite3 *pDB = CDataBaseOper::getDB();
    int iResult;
    char* pErrMsg = NULL;
    
    __String * temp = __String::createWithFormat("delete from user");
    iResult = sqlite3_exec(pDB, temp->getCString(), NULL, NULL, &pErrMsg);
    if(iResult != SQLITE_OK )
        CCLOG( "删除玩家信息失败，错误码:%d ，错误原因:%s\n" , iResult, pErrMsg);
    
    std::string heroSkillLevel;
    
    userInfoInDB* userInfoDB = CDataManager::getTheUSerInfDB();
    
    
    char buf[256];
    
    std::vector<IdAndNum> heroSkills =  userInfoDB->heroSkills;
    
    for(int i = 0; i <  heroSkills.size(); i++)
    {
        sprintf(buf, "%d",  heroSkills[i].theId);
        heroSkillLevel += buf;
        heroSkillLevel += ",";
        sprintf(buf, "%d",  heroSkills[i].theNumber);
        heroSkillLevel += buf;
        if(i < ( heroSkills.size() - 1))
        {
            heroSkillLevel += ";";
        }
    }
    std::string starInfo;
    for (int i = 0; i < pUserInfo->m_userLevelVec.size(); i++) {
        sprintf(buf, "%d", pUserInfo->m_userLevelVec[i].level);
        starInfo += buf;
        starInfo += ",";
        sprintf(buf, "%d", pUserInfo->m_userLevelVec[i].star);
        starInfo += buf;
        if(i < (pUserInfo->m_userLevelVec.size() - 1))
        {
            starInfo += ";";
        }
    }
    std::string awardInfo;
    for (int i = 0; i < pUserInfo->m_userMidLevelRewardVec.size(); i++) {
        sprintf(buf, "%d", pUserInfo->m_userMidLevelRewardVec[i].midLevel);
        awardInfo += buf;
        awardInfo += ",";
        sprintf(buf, "%d", pUserInfo->m_userMidLevelRewardVec[i].rewards);
        awardInfo += buf;
        if(i < (pUserInfo->m_userMidLevelRewardVec.size() - 1))
        {
            awardInfo += ";";
        }
    }
    
    std::string debrisInfo;
    for (int i = 0; i < pUserInfo->m_useDebrisVec.size(); i++) {
        sprintf(buf, "%d", pUserInfo->m_useDebrisVec[i].monsterId);
        debrisInfo += buf;
        debrisInfo += ",";
        sprintf(buf, "%d", pUserInfo->m_useDebrisVec[i].num);
        debrisInfo += buf;
        if(i < (pUserInfo->m_useDebrisVec.size() - 1))
        {
            debrisInfo += ";";
        }
    }
    
    std::string heroStarInfo;
    for (int i = 0; i < pUserInfo->m_userEqliteLevelVec.size(); i++) {
        sprintf(buf, "%d", pUserInfo->m_userEqliteLevelVec[i].level);
        heroStarInfo += buf;
        heroStarInfo += ",";
        sprintf(buf, "%d", pUserInfo->m_userEqliteLevelVec[i].star);
        heroStarInfo += buf;
        if(i < (pUserInfo->m_userEqliteLevelVec.size() - 1))
        {
            heroStarInfo += ";";
        }
    }
    std::string heroAwardInfo;
    for (int i = 0; i < pUserInfo->m_userHeroMidLevelRewardVec.size(); i++) {
        sprintf(buf, "%d", pUserInfo->m_userHeroMidLevelRewardVec[i].midLevel);
        heroAwardInfo += buf;
        heroAwardInfo += ",";
        sprintf(buf, "%d", pUserInfo->m_userHeroMidLevelRewardVec[i].rewards);
        heroAwardInfo += buf;
        if(i < (pUserInfo->m_userHeroMidLevelRewardVec.size() - 1))
        {
            heroAwardInfo += ";";
        }
    }
    std::string towerStarInfo;
    for (int i = 0; i < pUserInfo->m_userTowerLevelVec.size(); i++)
    {
        sprintf(buf, "%d", pUserInfo->m_userTowerLevelVec[i]);
        towerStarInfo += buf;
        if(i < (pUserInfo->m_userTowerLevelVec.size() - 1))
        {
            towerStarInfo += ",";
        }
    }
    
    char userExp[128];
    sprintf(userExp, "%d", pUserInfo->m_iUserExp);
    
    char version[128];
    sprintf(version, "%d", CLIENT_DB_VERSION);
    
    std::string storyInfo;
    for (int i = 0; i < pUserInfo->storyInfo.size(); i++)
    {
        sprintf(buf, "%d", pUserInfo->storyInfo[i]);
        storyInfo += buf;
        if(i < (pUserInfo->storyInfo.size() - 1))
        {
            storyInfo += ",";
        }
    }
    
    char ifSugges[128];
    sprintf(ifSugges, "%d", theUserInfo->ifSuggesed);
    
    
    
    temp = __String::createWithFormat("insert into user values(%d,%d,'%s',%d,'%s','%s','%s',%d,'%s','%s','%s','%s','%s','%s','%s',%ld,'%s','%s','%s','%s', '%s','%s','%s','%s','%s','%s')",
                 pUserInfo->m_id, pUserInfo->m_iUserId, pUserInfo->userName.c_str(), pUserInfo->m_iUserLevel, userExp, pUserInfo->m_sCoin.c_str(),
                 pUserInfo->m_sMagicStone.c_str(), atoi(pUserInfo->m_sloginDay.c_str()), heroSkillLevel.c_str(),
                 pUserInfo->m_sAuthCode.c_str(), starInfo.c_str(), awardInfo.c_str(), debrisInfo.c_str(),heroStarInfo.c_str(),
                 pUserInfo->m_sStaminaNum.c_str(), pUserInfo->m_iStaminTime, pUserInfo->m_strBudilingLv.c_str(),
                 pUserInfo->m_strEnergyLv.c_str(), heroAwardInfo.c_str(), pUserInfo->m_skillUpgradeInfo.c_str(), pUserInfo->m_iTimeStamp.c_str(),
                 pUserInfo->m_strHeroLv.c_str(),towerStarInfo.c_str(), version, storyInfo.c_str(),ifSugges);
    iResult = sqlite3_exec(pDB, temp->getCString(), NULL, NULL, &pErrMsg);
    if(iResult != SQLITE_OK )
        CCLOG( "保存玩家信息失败，错误码:%d ，错误原因:%s\n" , iResult, pErrMsg);
    
    CCLOG("%s",temp->getCString());
    
    
    CDataBaseOper::closeDB();
}

void CDBMgr::saveUserGuideInfo()
{
    CDBMgr::OpenDB();
    sqlite3 *pDB = CDataBaseOper::getDB();
    int iResult;
    char* pErrMsg = NULL;
    
    __String * temp = __String::createWithFormat("delete from guideProgress");
    iResult = sqlite3_exec(pDB, temp->getCString(), NULL, NULL, &pErrMsg);
    if(iResult != SQLITE_OK )
        CCLOG( "删除引导信息失败，错误码:%d ，错误原因:%s\n" , iResult, pErrMsg);
    
    for(int i = 0; i < thsGuideProInfo.size(); i++)
    {
        temp = __String::createWithFormat("insert into guideProgress values(%d,%d)",thsGuideProInfo[i].typeId, thsGuideProInfo[i].isEnd);
        iResult = sqlite3_exec(pDB, temp->getCString(), NULL, NULL, &pErrMsg);
        if(iResult != SQLITE_OK )
            CCLOG( "保存引导进度技能信息失败，错误码:%d ，错误原因:%s\n" , iResult, pErrMsg);
        CCLOG("%s",temp->getCString());
    }
    
    CDataBaseOper::closeDB();
}

chargeInfo* CDBMgr::getChargeInfoById(int chargeId)
{
    for(int i = 0 ; i < theChargeInfo.size(); i++)
    {
        if(theChargeInfo.at(i).chargeId == chargeId)
        {
            return &theChargeInfo.at(i);
        }
    }
    return NULL;
}

std::vector<videoListInfo> CDBMgr::getVideoListInfo()
{
    return theVideoListInfo;
}

void CDBMgr::saveRewardInfo(int videoId)
{
    CDBMgr::OpenDB();
    sqlite3 *pDB = CDataBaseOper::getDB();
    int iResult;
    char* pErrMsg = NULL;
    
    for(int i = 0; i < theVideoListInfo.size(); i++)
    {
        if(theVideoListInfo[i].videoId == videoId)
        {
            theVideoListInfo[i].iGetWard = 1;
            break;
        }
    }
    
    __String * temp = __String::createWithFormat("update videoInfo set iGetWard=1 where videoId=%d", videoId);
    iResult = sqlite3_exec(pDB, temp->getCString(), NULL, NULL, &pErrMsg);
    if(iResult != SQLITE_OK )
        CCLOG( "删除引导信息失败，错误码:%d ，错误原因:%s\n" , iResult, pErrMsg);
    
    CDataBaseOper::closeDB();
}

void CDBMgr::saveNewVideoInfo()
{
    CDBMgr::OpenDB();
    sqlite3 *pDB = CDataBaseOper::getDB();
    int iResult;
    char* pErrMsg = NULL;
    
    for(int i = 0; i < theVideoListInfo.size(); i++)
    {
        theVideoListInfo[i].state = 1;
    }
    __String * temp = __String::createWithFormat("update videoInfo set state=1");
    iResult = sqlite3_exec(pDB, temp->getCString(), NULL, NULL, &pErrMsg);
    if(iResult != SQLITE_OK )
        CCLOG( "删除引导信息失败，错误码:%d ，错误原因:%s\n" , iResult, pErrMsg);
    
    CDataBaseOper::closeDB();

}

void CDBMgr::saveRevenueFlag(int videoId)
{
    CDBMgr::OpenDB();
    sqlite3 *pDB = CDataBaseOper::getDB();
    int iResult;
    char* pErrMsg = NULL;
    
    for(int i = 0; i < theVideoListInfo.size(); i++)
    {
        if(theVideoListInfo[i].videoId == videoId)
        {
            theVideoListInfo[i].iAttack = 1;
            break;
        }
    }
    
    __String * temp = __String::createWithFormat("update videoInfo set iAttack=1 where videoId=%d", videoId);
    iResult = sqlite3_exec(pDB, temp->getCString(), NULL, NULL, &pErrMsg);
    if(iResult != SQLITE_OK )
        CCLOG( "删除引导信息失败，错误码:%d ，错误原因:%s\n" , iResult, pErrMsg);
    
    CDataBaseOper::closeDB();
}

void CDBMgr::saveVideoById(int videoId)
{
    CDBMgr::OpenDB();
    sqlite3 *pDB = CDataBaseOper::getDB();
    int iResult;
    char* pErrMsg = NULL;
    
    for(int i = 0; i < theVideoListInfo.size(); i++)
    {
        if(theVideoListInfo[i].videoId == videoId)
        {
            theVideoListInfo[i].bValid = 1;
            break;
        }
    }
    
    __String * temp = __String::createWithFormat("update videoInfo set iValid=1 where videoId=%d", videoId);
    iResult = sqlite3_exec(pDB, temp->getCString(), NULL, NULL, &pErrMsg);
    if(iResult != SQLITE_OK )
        CCLOG( "删除引导信息失败，错误码:%d ，错误原因:%s\n" , iResult, pErrMsg);
    
    CDataBaseOper::closeDB();
}

void CDBMgr::saveVideoInfo(std::vector<videoListInfo> tempVec)
{
    CDBMgr::OpenDB();
    sqlite3 *pDB = CDataBaseOper::getDB();
    int iResult;
    char* pErrMsg = NULL;
    char buf[256];
    
    __String * temp = __String::createWithFormat("delete from videoInfo");
    iResult = sqlite3_exec(pDB, temp->getCString(), NULL, NULL, &pErrMsg);
    if(iResult != SQLITE_OK )
        CCLOG( "删除录像信息失败，错误码:%d ，错误原因:%s\n" , iResult, pErrMsg);
    if(tempVec.size() > 0)
    {
        theVideoListInfo = tempVec;
    }
    for(int i = 0; i < tempVec.size(); i++)
    {
        std::string enemyInfo;
        for(int j = 0; j < tempVec[i].enemyInfo.size(); j++)
        {
            sprintf(buf, "%d,", tempVec[i].enemyInfo[j].theId);
            enemyInfo += buf;
            sprintf(buf, "%d,", tempVec[i].enemyInfo[j].level);
            enemyInfo += buf;
            sprintf(buf, "%d", tempVec[i].enemyInfo[j].number);
            enemyInfo += buf;
            if(j < tempVec[i].enemyInfo.size() - 1)
            {
                enemyInfo += ";";
            }
        }
        temp = __String::createWithFormat("insert into videoInfo values(%d,%d,'%s',%d,'%s',%d,%d,%d,%d,%d,%d,%d,%d)",
                                          tempVec[i].videoId,tempVec[i].enemyUserId, tempVec[i].enemyName.c_str(), tempVec[i].enemyLevel,
                                          enemyInfo.c_str(), tempVec[i].robStone, tempVec[i].state, tempVec[i].uploadTime, tempVec[i].iWin,
                                          tempVec[i].iGetWard, tempVec[i].bValid, tempVec[i].iAttack, tempVec[i].destroyDegree);
        
        iResult = sqlite3_exec(pDB, temp->getCString(), NULL, NULL, &pErrMsg);
        if(iResult != SQLITE_OK )
            CCLOG( "删除引导信息失败，错误码:%d ，错误原因:%s\n" , iResult, pErrMsg);
    }
    
    CDataBaseOper::closeDB();
}

int CDBMgr::getBuildTypeNum()
{
     std::vector<int> tempVec;//暂时存放建筑类型
    for(int i = 0 ; i < (int)thePvpBuildInfo.size(); i++)
    {
        bool bExisted = false;
        for(int j = 0; j < (int)tempVec.size(); j++)
        {
            if(tempVec[j] == thePvpBuildInfo[i].buildingType)
            {
                bExisted = true;
                break;
            }
        }
        if(!bExisted)
        {
            tempVec.push_back(thePvpBuildInfo[i].buildingType);
        }
    }
    return tempVec.size();
}

int CDBMgr::getBuildIdByType(int buildType)
{
    for(int i = 0 ; i < thePvpBuildInfo.size(); i++)
    {
        if((thePvpBuildInfo[i].buildingType == buildType) && (thePvpBuildInfo[i].level == 1))
        {
            return thePvpBuildInfo[i].buildingID;
        }
    }
}

pvpBuildInfo* CDBMgr::getBuildInfo(int buildId)
{
    for(int i = 0 ; i < thePvpBuildInfo.size(); i++)
    {
        if(thePvpBuildInfo[i].buildingID == buildId)
        {
            return &thePvpBuildInfo.at(i);
        }
    }
    return NULL;
}

void CDBMgr::delMeeageInfo(std::string meesgaeInfo)
{
    std::vector<string>::iterator it = m_requestInfo.begin();
    for(; it != m_requestInfo.end(); it++)
    {
        if((*it).compare(meesgaeInfo) == 0)
        {
            m_requestInfo.erase(it);
            break;
        }
    }
    CDBMgr::OpenDB();
    sqlite3 *pDB = CDataBaseOper::getDB();
    int iResult;
    char* pErrMsg = NULL;
    
    __String * temp = __String::createWithFormat("delete from messageInfo where request = '%s')",meesgaeInfo.c_str());
    iResult = sqlite3_exec(pDB, temp->getCString(), NULL, NULL, &pErrMsg);
    if(iResult != SQLITE_OK )
        CCLOG( "删除引导信息失败，错误码:%d ，错误原因:%s\n" , iResult, pErrMsg);
    CDataBaseOper::closeDB();
}

void CDBMgr::insertMessageInfo(std::string meesgaeInfo)
{
    CDBMgr::OpenDB();
    sqlite3 *pDB = CDataBaseOper::getDB();
    int iResult;
    char* pErrMsg = NULL;
    
   __String * temp = __String::createWithFormat("insert into messageInfo values('%s')", meesgaeInfo.c_str());
    iResult = sqlite3_exec(pDB, temp->getCString(), NULL, NULL, &pErrMsg);
    if(iResult != SQLITE_OK )
        CCLOG( "删除引导信息失败，错误码:%d ，错误原因:%s\n" , iResult, pErrMsg);
    CDataBaseOper::closeDB();
    m_requestInfo.push_back(meesgaeInfo);
}

void CDBMgr::saveUserTaskListInfo()
{
    CDBMgr::OpenDB();
    sqlite3 *pDB = CDataBaseOper::getDB();
    int iResult;
    char* pErrMsg = NULL;
    char buf[258];
    
    __String * temp = __String::createWithFormat("delete from TaskList");
    iResult = sqlite3_exec(pDB, temp->getCString(), NULL, NULL, &pErrMsg);
    if(iResult != SQLITE_OK )
        CCLOG( "删除任务信息失败，错误码:%d ，错误原因:%s\n" , iResult, pErrMsg);
    
    for(int i = 0; i < m_pTaskInfoVec.size(); i++)
    {
        std::string items;
        for (int j = 0; j < m_pTaskInfoVec[i]->itemVec.size(); j++) {
            sprintf(buf, "%d", m_pTaskInfoVec[i]->itemVec[j].theId);
            items += buf;
            items += ",";
            sprintf(buf, "%d", m_pTaskInfoVec[i]->itemVec[j].theNumber);
            items += buf;
            if(j < (m_pTaskInfoVec[i]->itemVec.size() - 1))
            {
                items += ";";
            }
        }
        std::string heroItems;
        for (int j = 0; j < m_pTaskInfoVec[i]->heroItemVec.size(); j++) {
            sprintf(buf, "%d", m_pTaskInfoVec[i]->heroItemVec[j].theId);
            heroItems += buf;
            heroItems += ",";
            sprintf(buf, "%d", m_pTaskInfoVec[i]->heroItemVec[j].theNumber);
            heroItems += buf;
            if(j < (m_pTaskInfoVec[i]->heroItemVec.size() - 1))
            {
                heroItems += ";";
            }
        }
        
        temp = __String::createWithFormat("insert into TaskList values(%d,%d,'%s','%s',%d,%d,%d,%d,'%s','%s',%d,%d,%d,%d)",
                                                     m_pTaskInfoVec[i]->m_iTaskId,m_pTaskInfoVec[i]->m_iTaskType, m_pTaskInfoVec[i]->m_sDesc.c_str(),
                                                     m_pTaskInfoVec[i]->m_taskIcon.c_str(), m_pTaskInfoVec[i]->m_iNum, m_pTaskInfoVec[i]->m_iMoney,
                                                     m_pTaskInfoVec[i]->m_iExp, m_pTaskInfoVec[i]->m_stone, items.c_str(), heroItems.c_str(),
                                                     m_pTaskInfoVec[i]->m_iTaskIdLimmit, m_pTaskInfoVec[i]->m_iLvLimmit,
                                                     int(m_pTaskInfoVec[i]->m_eTaskState), m_pTaskInfoVec[i]->taskProp);
        iResult = sqlite3_exec(pDB, temp->getCString(), NULL, NULL, &pErrMsg);
        if(iResult != SQLITE_OK )
            CCLOG("保存任务信息失败，错误码:%d ，错误原因:%s\n", iResult, pErrMsg);
    }
    CDataBaseOper::closeDB();
}

void CDBMgr::saveUserTaskInfo()
{
    CDBMgr::OpenDB();
    sqlite3 *pDB = CDataBaseOper::getDB();
    int iResult;
    char* pErrMsg = NULL;
    
    __String * temp = __String::createWithFormat("delete from taskProgressInfo");
    iResult = sqlite3_exec(pDB, temp->getCString(), NULL, NULL, &pErrMsg);
    if(iResult != SQLITE_OK )
        CCLOG( "删除玩家任务进度信息失败，错误码:%d ，错误原因:%s\n" , iResult, pErrMsg);
    
    for(int i = 0; i < m_pTaskProVec.size(); i++)
    {
        temp = __String::createWithFormat("insert into taskProgressInfo values(%d,%d)",m_pTaskProVec[i]->m_taskType, m_pTaskProVec[i]->m_num);
        iResult = sqlite3_exec(pDB, temp->getCString(), NULL, NULL, &pErrMsg);
        if(iResult != SQLITE_OK )
            CCLOG("保存玩家任务进度信息失败，错误码:%d ，错误原因:%s\n", iResult, pErrMsg);
        CCLOG("%s",temp->getCString());
    }
    
    CDataBaseOper::closeDB();
}

void CDBMgr::saveUserMonsterInfo()
{
    CDBMgr::OpenDB();
    sqlite3 *pDB = CDataBaseOper::getDB();
    int iResult;
    char* pErrMsg = NULL;
    
    __String * temp = __String::createWithFormat("delete from userMonsterInfo");
    iResult = sqlite3_exec(pDB, temp->getCString(), NULL, NULL, &pErrMsg);
    if(iResult != SQLITE_OK )
        CCLOG( "删除玩家怪物信息失败，错误码:%d ，错误原因:%s\n" , iResult, pErrMsg);
    
    for(int i = 0; i < UserMonsterInfo.size(); i++)
    {
        temp = __String::createWithFormat("insert into userMonsterInfo values(%d,%d, %d,%d, %d,%d,%d,%d,%d, %d)", UserMonsterInfo[i]->monsterId,
                        UserMonsterInfo[i]->monsterLevel, UserMonsterInfo[i]->skillLevel1, UserMonsterInfo[i]->skillLevel2,
                        UserMonsterInfo[i]->skillLevel3, UserMonsterInfo[i]->skillLevel4, UserMonsterInfo[i]->ifFight, UserMonsterInfo[i]->theType,
                                                     UserMonsterInfo[i]->rare, UserMonsterInfo[i]->num);

        iResult = sqlite3_exec(pDB, temp->getCString(), NULL, NULL, &pErrMsg);
        if(iResult != SQLITE_OK )
            CCLOG("更新玩家怪物信息失败，错误码:%d ，错误原因:%s\n", iResult, pErrMsg);
        CCLOG("%s",temp->getCString());
    }
    CDataBaseOper::closeDB();
}

void CDBMgr::saveUserItemsInfo()
{
    CDBMgr::OpenDB();
    sqlite3 *pDB = CDataBaseOper::getDB();
    int iResult;
    char* pErrMsg = NULL;
    
    __String * temp = __String::createWithFormat("delete from UserItems");
    iResult = sqlite3_exec(pDB, temp->getCString(), NULL, NULL, &pErrMsg);
    if(iResult != SQLITE_OK )
        CCLOG( "删除物品表信息失败，错误码:%d ，错误原因:%s\n" , iResult, pErrMsg);
    
    for(int i = 0; i < m_pMyPropInfo.size(); i++)
    {
        temp = __String::createWithFormat("insert into UserItems values(%d,%d)", m_pMyPropInfo[i]->m_iPropId,m_pMyPropInfo[i]->m_iPropNum);
        iResult = sqlite3_exec(pDB, temp->getCString(), NULL, NULL, &pErrMsg);
        if(iResult != SQLITE_OK )
            CCLOG("插入物品信息失败，错误码:%d ，错误原因:%s\n", iResult, pErrMsg);
        CCLOG("%s",temp->getCString());
    }
    CDataBaseOper::closeDB();
}




