//
//  DBMgr.h
//  PinBallHeroes
//
//  Created by hsx on 14-3-21.
//  数据库解析管理类
//

#ifndef __PinBallHeroes__DBMgr__
#define __PinBallHeroes__DBMgr__



#include "btDBOper.h"
#include "cocos2d.h"

#include "AllDefine.h"

#include "DataManager.h"
#include "LYPersonalData.h"

USING_NS_CC;



typedef enum {
    
    eCopDBType_cover,    //完全覆盖
    eCopDBType_create,   //没有则新增
    eCopDBType_update,   //更新操作
    
}copyDBType;



class CDBMgr
{
    
    friend CDataManager;
    friend CPersonalData;
    
public:
    CDBMgr();
    ~CDBMgr();
    static CDBMgr* getInstance();
    static void destroyInstance();

    typedef  enum eAffectType
    {
        eBuildingType,
        eEnergyType
    }eAffectType;
    
    
    
    

    typedef struct MonsterSkillTime
    {
        int m_iMonsterId;
        int m_iSkillId;
        int m_iTime;
        int m_iNeedTime;
        MonsterSkillTime()
        {
            m_iMonsterId = 0;
            m_iSkillId = 0;
            m_iTime = 0;
            m_iNeedTime = 0;
        }
    }MonsterSkillTime_s;
    
    typedef struct skillLvInfo
    {
        int m_iHeroid;
        int m_iLv;
        skillLvInfo()
        {
            m_iHeroid = 0;
            m_iLv = 0;
        }
    }skillLvInfo_s;
    
    
    void copyDB(copyDBType type = eCopDBType_create);
    
    
    
    void loadDB();
    void loadModelDB();//读取所有模型信息
    void loadMonsterDB();//读取所有怪物信息
    void loadUserMonsterDB();//读取玩家怪物信息

    void loadPropDB();//读取道具信息
    void loadUserPropDB();//读取玩家道具信息
    
    void loadSkillDB();
    void loadSkillEffectDB();
    
    void loadLevelInfoDB();//读取关卡信息
    void loadMapInfoDB();//读取地图信息
    void loadTaskDB(); //导入任务信息
    void loadTaskProInfo();//读取任务进度信息
    void loadSkillUpgradeInfo(); //导入技能升级表
    void loadHeroSkillUpgradeInfo();//导入英雄技能升级表
    void loadEnergyUpgradeInfo();//导入能量升级表
    void loadBuildingUpgradeInfo(); //导入建筑升级表
    void loadMonsterUpgradeInfo();//导入怪物升级表
    void loadMonsterEvolutionInfo();//导入怪物进阶信息
    
    void loadFightTalkingInfo();//导入战斗对话信息
    
    void loadTipsInfo();//导入tips信息
    
    void loadShop();
    void loadUserInfoum();
    void loadMessageInfo();

    void loadFightReinforcementInfo();//导入战斗援军信息
    
    bool IsSkillCanLvUp(int iMonsterId,int iSkillId,bool bHero = false,int iSkillLv=1); //判断技能是否可以升级
    
    
    Sprite * GetShareSpriteByFigtherType(FighterType monsterType);
    
    void SaveUserMonsterSkillToDB(int iMonsterId,int iSKillIndex,int iSKillLv);
    
    void SaveUserMonsterInfo(int iMonsterId,int iMonsterLv,int iSkillLv1 =-1,int iSkillLv2=-1,int iSkillLv3=-1,int iSkillLv4=-1,bool bFight=false,
    int iType=-1,int iMonsterRare=-1); //保存怪物信息表
    
    void saveUserTeamToDB(int monsterId, int iFight);
    
    void saveUserAwardInfo(int midLevelId, int num, bool isEqlite);
    
    void  SaveBuildingInfo();
    
    void  SaveEnergyUpgradeInfo();

    int HavePropNumById(int iPropId);
    
    void DelPropIdByNum(int iPropId,int iNum);
    
    void SaveUserPropDb(int PropId,int num);
    
    void saveGameStoryInfoToDB(int type);
    
    void saveGuideInfoToDB(int type);
    
    void saveTaskInfoToDB(int taskType, int num, bool ibMax);
    
    void SaveMonsterSKillLv(int iMonsterId,int iSkillLv1,int iSkillLv2,
                                     int iSkillLv3,int iSkillLv4);
    
    void updateTaskSate(int taskId, TaskState state);//更新指定任务的状态
    void saveTaskState(int taskType, TaskState state, bool bTaskId = false);
    void saveDailyTaskInfo(int taskType);
    
    void eraseTaskById(int taskId);//删除任务
    
    TaskBase* getTaskInfoById(int taskId);//获取任务信息
    
    int getTaskProByType(int taskType);//获取任务进度
    
    void saveUserLoginTime(std::string time);
    
    void saveUpdateExchangeNum(int type);
    
    void saveUpdateDailyaTaskTime(std::string time);//保存每日任务刷新的时间
    
    void saveUpdateExchangeNumTime(std::string time);//保存每日兑换次数刷新的时间
    
    void refreshDailyTask();//刷新每日任务
    
    void refreshUserInfoNum();//刷新用户兑换次数
    
    float  getExpInfoFromDB(int level);//获取升级所需的经验
    
    std::vector<int> getStarConditionById(int levelId);
    
    starInfo* getStarInfoById(int starId);
    
    midLevelInfo* getMidLevelInfoById(int midLevelId);//获取中关卡信息
    
    int getMidLevelAwardById(int midLevel, bool ifEqlite);//根据关卡id获取奖励情况
    
    int getAllLevelStarById(int midLevelId, bool ifEqlite = false);//获取所有小关卡星级总数
    
    int getStarBySmallLevelId(int smaLevelId, bool ifEqlite = false);//根据小关卡id获取星级
    
    int getAllLevelById(int midLevelId);//获取所有小关卡总数
    
    propInfo* getPropInfoById(int propId);//根据道具id获取道具信息
    
    std::vector<int> getItemPathById(int propId, int eQlite);//根据道具id获取关卡途径
    
    std::string getPropImgByid(int propId); //根据道具id获取道具图片
    
    void loadBulletDB();//读取子弹信息
    
    void insertIntoMonsterInfo(int monsterId, int num = 1, int rare = 0, int ifFight = 0);//新增一条怪物信息
    
   vector<heroSkill*>  getHeroSkillByMonsterId(int iMonsterId);
    
    void loadHeroSkillInfoDB();
    
    void loadItemOpenInfoDB();
    
    
    void loadDrawCardResult();
    
    void loadUserInfo();
    
    void loadUserUpgradeInfo();
    
    void loadMidLevelInfo();
    
    void loadLevelStarInfo();
    
    void loadGuiderInfo();
    
    void loadGuideProgress();
    
    void loadFightBuildingInfo();//读取塔防信息
    
    void loadChargeInfo();//读取充值信息
    
    void loadBuildInfo();//读取pvp模式建塔的信息
    
    void loadPvpVIdoeInfo();//读取pvp模式下的录像
    
    void loadGameStoryInnfo();
    
    void loadUserNameInfo();
    
    void loadConfigure(); //导入配置表

    void loadTmpSaveInfo();//保存数据表
    
    void SaveDrawCardCommonTime(int iNowTime);
    
    void SaveDrawCardCommonNum(int iNum);

    void SaveDrawCardAdvanceNum(int iNum);
    
    
    void SaveDrawCardAdvanceTime(int iNowTime);
    
    void saveUserAllInfo();
    void saveUserGuideInfo();
    void saveUserTaskListInfo();
    void saveUserTaskInfo();
    void saveUserMonsterInfo();
    void saveUserItemsInfo();
    void insertMessageInfo(std::string meesgaeInfo);
    void delMeeageInfo(std::string meesgaeInfo);
    chargeInfo* getChargeInfoById(int chargeId);
    pvpBuildInfo* getBuildInfo(int buildId);
    int getBuildIdByType(int buildType);
    int getBuildTypeNum();
    std::vector<videoListInfo> getVideoListInfo();
    void saveVideoInfo(std::vector<videoListInfo> tempVec);
    void saveVideoById(int videoId);
    void saveRevenueFlag(int videoId);
    void saveRewardInfo(int videoId);
    void saveNewVideoInfo();
    void AnalyseStringFromBuilding(std::string strBuilding);
    
    void AnalyseStringFromEnergy(std::string strEnergy);
    
    void AnalyseSkillUpgradeNeedItem (std::string str);
    
    void AnalyseSkillUpgradeInfo(std::string str);
    
    void AnalyseHeroLvInfo(std::string str);
    
    void  SaveBuidlingLvToDB(std::string str);
    void  SaveEnergyLvTosDB(std::string str);
    void  SaveSkillUpgradeInfoToDB(std::string str);
    
    void AddSkillUpgradeToStr(int id,int SKillId,int iTime,int iNeedTime);
    void FengeStr(std::string str,int &id,int &SkillId);
    void DelSkillUpgradeToStr(std::string str);
    void AddSkillIdTostr(std::string str);
    
    int GetMonsterSkillIndex(int iMonsterId,int iSkillId);
    void AddMonsterSkillLvOne(int iMonsterId,int iSkillId);
    int  GetMonsterSKillLv(int iMonsterId,int iSkillId);
    
    void  SaveBuidlingLvTostr(int id,int lv);
    
    void  SaveEnergyLvTostr(int id,int lv);
    
    
    void  SaveHeroLv(std::string str);
    
    void  setUserIdAndAuthCode();//
    
    void setUserIfSuggessed();
    
    std::vector<skillInfo*> GetAllSkillInfo();
    skillInfo * GetSoldierSkillInfoBySkillId(int iSkillId);
    
    std::vector<TaskBase*>  GetAllTaskInfo();
    std::vector<SkillUpgradeInfo*> GetSkillUpgradeInfo();

    
    //heroSkillUpgradeInfo* GetSkillUpgradeInfoWithId(int iHeroId,int iSkillLv);
    
    
    heroSkillUpgradeInfo*  GetHeroSkillWithId(int iHeroSkillId, int iSkillLv);
    
    std::vector<heroSkillUpgradeInfo*>GetHeroSkillUpgradeInfo();
    
    std::vector<energyUpgradeInfo*>  GetEnergyUpGradeInfo();
    
    std::vector<buildingUpgradeInfo*> GetBuildingUpgradeInfo();
    
    std::vector<MonsterUpgradeInfo*> GetMonsterUpgradeInfo();
    
    std::vector<propInfo*>  GetAllPropInfo();
    
    std::vector<MyPropInfo*>  GetUserProp();//获取用户道具内容
    
    std::vector<PropOpenInfo*> GetPropOpenInfo();
    
    std::vector<PropOpenInfo*> getPropOpenInfoById(int iPropId);
    
    static std::vector<__String> separateInfoWithMark(__String originStr,__String Mark);
    
    std::vector<UserName>  GetUserNameInfo();
    
    std::vector<ConfigureInfo*> GetConfigureInfo();
    
    
    int GetHeroLvWithId(int iHeroId);
    
    userInfoInDB * GetUserInfo()
    {
        return theUserInfo;
    }
    
    void  SetUserInfo(userInfoInDB * pUserInfo);
    
    void saveUserInfo(int stamina,int level, int exp,int staminaTime=-1);
    int  GetHeroSkillLvById(int iMonsterId);
    int GetHeroSkillMaxLvById(int iMonsterId);
    
    void SaveUserInfo(UserInfo *pInfo);
    void saveUserStaminaTime(long staminaTime);
    void saveUserStamina(int iStamina);
    
    void saveUserTowerLevelInfoToDB(int level);
    void saveUserInfoToDB(int level, int rare, bool isEqlite);
    void saveUserStoneAndCoinToDB(int num, int type, bool ifAdd = false);//减少的魔法石数量
    void saveUserDebrisToDB(int debrisId, int num, int type = 0);//0表示增加，1表示减少
    void saveMonsterStarToDB(int monsterId,int rare);//保存用户星级
    upgradeInfo* getUpgradeInfoByLevel(int level);
    void judgeUserIfUpgrade(Node * pTarget, int exp,int *resultAddStamina);//判断是否升级
    
    
    void SaveHeroSkillInfoToDb(string strHero);
    
    void OpenDB();
    void IsRelease();
    
    int getAffectNum(eAffectType eType,int iId);
    
    EnergyUpgradeInfo* getBuildingInfo(eAffectType eType,int iId);
    
    EnergyUpgradeInfo* getEnergyUpgradeInfo(int iEnergyId,int iLevl);
    
    BuildingUpgradeInfo* GetBuidlingUpgradeInfo(int iBuidlingId,int iLevel);
    
    int GetBuidlingLvById(int id);
    
    int GetEnergyLvById(int id);
    
    std::map<int,int> GetBuidlingMap();
    
    std::map<int,int>  GetEnergyMap();
    
    std::map<int,int> GetSkillUpgradeItemMap();

    vector<MonsterSkillTime_s>  GetSkillUpgradeInfoVec();

    bool HaveSkillUpgradeInfo(int id,int iSkillId);
    
    std::vector<heroSkill*>  GetHeroSkillInfo();
    
    int GetHeroSkillIndex(int iHeroId);
    
    std::map<int,string> GetSkillUpgradeStrInfoMap();
    
    std::vector<ShopItemInfo*>  GetUserShopInfo();
    
    
    
    int  GeShopPropPrize(int id);
    
    
    std::vector<TempSaveInfo*>  GetTempSaveInfo();
    
    std::vector<DrawCardResultInfo*> GetDrawCardResultInfo();
    
    int  GetAllMonsterNum();
    
    void SetTempCoinTime(int iTime);
    
    void SetTempCoinNum(int iNum);
    
    void SetTempStoneTime(int iTime);
    
    void SetTempStoneNum(int iNum);
    
    
    modelInfo* GetModleInfoById(int iMonsterId);
    
    void saveTempUserInfo(int userId);//保存玩家已经搜索到的玩家id
    std::string getTempAllUser();//返回当天已经搜索到的玩家id
    
private:
    void init();
    
    std::vector<midLevelInfo> midLevelInfoVec;//中关卡信息
    userInfoInDB* theUserInfo;//玩家信息

    std::vector<MonsterSkillTime_s>  m_SkillUpgradeInfo_vec;
    std::vector<skillLvInfo_s>  m_HeroLvInfo_vec;
    
    //std::vector<int>   m_SkillTimeInfo_vec;
    
    std::map<int,int>  m_BuildingLv_map;
    std::map<int,int>  m_EneygyLv_map;
    std::map<int,int>  m_SkillUpgradeItem_map;
    
    std::vector<modelInfo*> theModleInfo;   //模型信息
    std::vector<monsterInfo*> theMonsterInfo; //所有怪物信息
    std::vector<propInfo*> m_propInfo;  //道具信息
    std::vector<MyPropInfo*> m_pMyPropInfo; //玩家道具信息
    std::vector<PropOpenInfo*> m_pPropOpenInfo;//道具打开信息
    std::vector<myMonster*> UserMonsterInfo; //玩家的怪物信息
    std::vector<TaskBase *> m_pTaskInfoVec;//任务信息
    std::vector<taskProInfo*> m_pTaskProVec;//任务进度信息
    std::vector<skillInfo*> theSkillInfo;   //所有技能信息
    std::vector<skillEffect*> theSkillEffectInfo; //所有技能特效信息
    std::vector<upgradeInfo> thsUserUpgradeInfo;//玩家升级信息
    std::vector<guideInfo> thsGuiderInfo;//引导表
    std::vector<guidePro> thsGuideProInfo;//引导进度
    std::vector<heroSkill*> theHeroSkillInfo;
    std::vector<gameStory> theGameStoryInfo;//剧情
    std::vector<chargeInfo> theChargeInfo;//充值
    std::vector<pvpBuildInfo> thePvpBuildInfo;//建塔信息
    std::vector<videoListInfo> theVideoListInfo;//录像列表
    std::vector<UserName>   m_theUserNameInfo_vec; //玩家命名
    
    std::vector<heroSkill*> theCurHeroSkillInfo;//当前id英雄所有技能
    
    std::vector<bulletInfoDB*> theBulletInfo;//所有子弹信息
    std::vector<levelInfo*> theLevelInfo; //关卡信息
    std::vector<mapInfo*> theMapInfo; //关卡信息
    std::vector<SkillUpgradeInfo*>  m_pSkillUpgradeInfo;//技能升级信息
    std::vector<heroSkillUpgradeInfo*> m_pHeroSkillUpgradeInfo;//英雄升级信息
    std::vector<energyUpgradeInfo*>  m_pEnergyUpgradeInfo; //能量升级信息
    std::vector<buildingUpgradeInfo*> m_pBuildingUpgradeInfo; //建筑升级信息
    std::vector<MonsterUpgradeInfo*> m_pMonsterUpgradeInfo; //怪物升级信息
    std::vector<MonsterEvolutionInfo> m_pMonsterEvoltionInfo;//怪物进化信息
    std::vector<starInfo> m_levelStarInfo;//关卡获取星级的条件
    std::vector<ShopItemInfo*> m_ShopInfo;//商店信息
    userNum m_userInfoNum;
    std::vector<string> m_requestInfo;
    std::vector<reinforcementInfo*> m_vecFightReinforcementInfo;
    
    std::vector<fightBuildingInfo*> m_vecFightBuildingInfo;//塔防建筑信息
    
    std::vector<fightTalkingInfo*> m_vecFightTalkingInfo;//塔防建筑信息
    
    std::vector<ConfigureInfo*>  m_vecConfigureInfo;//配置信息
    std::vector<TempSaveInfo*>  m_vecTempSaveInfo;// 临时保存信息
    std::vector<DrawCardResultInfo*> m_pvecDrawCardResultInfo; //抽卡结果表
    
    std::vector<tipsInfo*> m_vecTipsInfo;//tips信息
    
};
#endif /* defined(__PinBallHeroes__DBMg__) */
