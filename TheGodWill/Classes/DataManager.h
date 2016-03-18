//
//  DataManager.h
//  TheGodWill
//
//  Created by user on 14/12/16.
//
//

#ifndef __TheGodWill__DataManager__
#define __TheGodWill__DataManager__

#include <stdio.h>
#include "AllDefine.h"

enum GuideState
{
    GUIDE_ADD_LINE = 1,  //第一关横线教程
    GUIDE_ADD_CIRCLE,   //第二关圆形教程
    GUIDE_LEVEL_TARGET,//第二关过关条件和士兵数量
    
    GUIDE_MAP_INTRODUCE = 100, //大地图引导
    
    GUIDE_GET_TASK = 4,//第二关完成后任务引导
    
    
    GUIDE_PERFECT_INTRO,//第三关 完美度介绍 5
    GUIDE_LEVEL_STAR,//第三关 星级引导            废弃
    GUIDE_UPGRADE_SOLIDER,//第三关完成后士兵升级
    GUIDE_ESCORT_TASK,//护送任务
    GUIDE_SKILL_UPGRADE,//技能升级
    GUIDE_ADD_RECTANGLE,   //10
    GUIDE_FAIL_TIP,//第六关失败提示
    GUIDE_SOLIDER_NUM,//第六关完成后 士兵数量
    GUIDE_ADD_V,
    GUIDE_SKILL_CUT,//切割技能
    GUIDE_GET_AWARD,//领取奖励  15
    GUIDE_ENTER_STORAGE,//仓库
    GUIDE_GET_FLAG,//获取旗帜
    GUIDE_USE_MANGONEL,//第十五关投石机的使用
    GUIDE_COMMON_CARD,//普通抽卡
    GUIDE_SENIOR_CARD,//高级抽卡   20
    GUIDE_SOLIDER_COMPOUND,//士兵合成
    GUIDE_SOLIDER_TEAM,//士兵编队
    GUIDE_SHOW_DRAGON,//第十八关飞龙
    GUIDE_SKILL_BLOW,//吹风技能
    GUIDE_PVP_BUILDING,//pvp建造  25
    GUIDE_PVP_RONGLU,//pvp熔炉
    GUIDE_PVP_ROB,//抢夺矿石
    GUIDE_GET_TOWER,//占领攻击塔
    GUIDE_SHOW_SWORDSMEN,//介绍女剑士
    GUIDE_KILL_BOSS,//杀掉boss
    GUIDE_SPRING_WATER,//泉水
    GUIDE_LACK_STAMINA,//体力不足提示
    GUIDE_BOY_HERO_SKILL,//升级男主角技能等级
    GUIDE_GIRL_HERO,//升级女主角等级
    GUIDE_GIRL_HERO_SKILL,//升级女主角技能等级
    GUIDE_TOWER_LEVEL,//塔防模式
    GUIDE_ERROR_STATE,
} ;

enum storyState
{
    GAME_STORY_RETREAT = 1,// 撤退
    GAME_STORY_SKELETON,//骷髅
    GAME_STORY_BACK,//复仇
    GAME_STORY_STRIKER,//添加射手
    GAME_STORY_USER,//帮手      5
    GAME_STORY_SCROLL,//获取卷轴
    GAME_STORY_AMBUSH,//埋伏
    GAME_STORY_WIN,//胜利突围
    GAME_STORY_GIRL,//帮助小萝莉
    GAME_STORY_GEM,//获取宝石    10
    GAME_STORY_STUDY,//练习技能
    GAME_STORY_ORCS,//遭遇兽人
};

enum
{
    TASK_COMMON_NUM = 1,//普通关卡通关次数(5关)
    TASK_EQLITE_NUM,    //精英关卡通关次数(5关)
    UPGRADE_SOLIDER_ONCE,//升级任意士兵1次
    UPGRADE_SKILL_ONCE,//升级任意技能
    USE_SKILL_NUM,      //使用英雄技能次数
    THREE_STAR_NUM,     //三星通关次数(1次)
    USE_TOUSHIJI_NUM,   //使用投石机次数(1次)
    TASK_COMMON_LEVEL,  //完成第XXX关
    TASK_EQLITE_LEVEL,  //完成第XXX关
    UPGRADE_USER_LEVEL, //升到XXX级
    OPEN_GOLDBOX_NUM,   //打开黄金宝箱次数
    GET_THIRD_STAR_NUM, //获得三星英雄的数量
    GET_FOUR_STAR_NUM,  //获得四星英雄的数量
    FINISH_ANYLEVEL_NUM,//完成任意关卡次数
    FINISH_DAILYTASK_NUM, //完成每日任务的数量
    COST_STONE_NUM,     //花费XXX水晶
    ANY_SKILL_LEVEL,    //任意技能达到XXX级
    ANY_SOLIDER_LEVEL,  //任意兵种达到XXX级
    
};

class CDataManager {
    
public:
    
    /*
     模型相关
     */
    static modelInfo* getModelInfoByModelId(int modelId);
    static monsterInfo* getMonsterInfoByMonsterId(int monsterId);
    static std::vector<monsterInfo*> getAllMonsterInfo();
    static MonsterEvolutionInfo* getMonsterEvolutionById(int rare);
    /*
     子弹相关
     */
    static bulletInfoDB* getBulletInfoByBulletId(int bulletId);
    
    /*
     根据怪物id返回怪物头像和背景框
     */
    
    static cocos2d::Sprite * getMonsterInfodebris(int iMonsterId);
    
    static cocos2d::Sprite* getMonsterIconAndBg(int monsterId, bool isShowStar = false,float fMonsterScale =0.25);
    
    /*
     士兵相关
     */
    static void CalcFighterInfoWith(FighterInfo* targetInfo,int monsterId,int monsterLevel, int rare = 1,int theType=0,int ifFight=false,int skilllevel1=0,int skilllevel2=0,int skilllevel3=0,int skilllevel4=0);
    static void getTheAddAtrWith(monsterInfo* targetInfo,int *addHP, int *addAtk);
    
    static MonsterAllEvolutionInfo getAllEvolutionInfoByRare(int rare);//获取当前星级累计的所有属性奖励
    
    
    /*
     技能相关
     */
    static skillInfo* getTheSkillInfoWithId(int skillId);
    static skillEffect* getTheSkillEffectWithId(int skillEffectId);
    static FighterSkillInfo CalcFightSkillInfoWithIdAndLevel(int skillId,int skillLevel);
    
    static float getTheDamageRatioWithIdAndLevel(int skillId,int skillLevel);
    static float getTheStartRatioWithIdAndLevel(int skillId,int skillLevel);
    
    static float getTheLastTimeWithIdAndLevel(int skillId,int skillLevel);//用于返回英雄技能的持续时间
    static float getTheDamageWithHeroKillIdAndLevel(int skillId,int skillLevel);//用于返回英雄技能的伤害值
    
    
    
    static cocos2d::Sprite * getSkillIcoWithId(std::string strImg,float fScale=1.2, const cocos2d::Color3B& color = cocos2d::Color3B::WHITE);
    
    static cocos2d::Sprite * getSkillIcoWithId(skillInfo *p,float fScale=1,bool bDisble= false);
    static void  InitWithSkillId(cocos2d::Sprite*pBg,skillInfo *pInfo);
    
    /*
     塔防信息相关
     */
    
    static fightBuildingInfo* getTheFightBuildingInfoWithId(int monsterId);
    static std::vector<fightBuildingInfo*>* getAllFightBuildingInfo();
    
    
    /*
     战斗气泡相关
     */
    
    static fightTalkingInfo* getTheFightTalkingInfoWithId(int talkingId);
    static std::vector<fightTalkingInfo*> getFightTalkingInfoByType(int type);
    static fightTalkingInfo* getTheRandomFightTalkingInfoByType(int type);
    
    
    /*
     战斗援军相关
     */
    static std::vector<reinforcementInfo*> getReinforceMentInfoByLevel(int level);
    
    
    
    //技能升级
    static SkillUpgradeInfo* getSkillUpgradeInfowithId(FighterType  eMonsterType);
    
    static SkillUpgradeInfo * getSkillUpgradeInfowithId(FighterType eMonsterType,int iSkillLevel);
    
    /*
     英雄技能相关
     */
    static heroSkill* getTheHeroSkillWithId(int heroSkillId);
    
    /*
     根据英雄id和星级获取已开启技能
     */
    static std::vector<skillInfo*> getAvailableSkillInfo(int monsterId, int rare);
    /*
     根据英雄id和星级获取未开启技能
     */
    static std::vector<skillInfo*> getUnavailableSkillInfo(int monsterId, int rare);
    /*
     关卡相关
     */
    static levelInfo* getTheLevelInfoWithId(int levelId);
    static std::vector<mapInfo*>  getTheMapInfoWithId(int levelId);
    
    /*
     用户数据相关
     */
    static userInfoInDB* getTheUSerInfDB();
    
    /*
     根据碎片id乎获取数量
     */
    static int getDebrisNumById(int monsterId);
    /*
     保存用户关卡信息
     */
    static void saveUserLevelInfoToDB(int level, int rare, bool isEqlite = false);
    
    /*
     保存用户塔防模式关卡信息
     */
    static void saveUserTowerLevelInfoToDB(int level);
    
    /*
      保存用户等级、经验、体力信息
      */
    static void saveUserInfo(int stamina, int level, int exp);
    
    /*
     保存用户魔法石信息
     */
    static void saveUserStoneAndCoinToDB(int num, int type, bool ifAdd = false);//0表示金币，1表示魔法石,ifAdd false:消耗 true:增加
    
    /*
     保存用户星级信息
     */
    static void saveMonsterStarToDB(int monsterId, int rare);
    
    /*
     保存用户购买的碎片
     */
    static void saveUserDebrisToDB(int debrisId, int num, int type = 0);//碎片id, 碎片数量
    
//    /*
//     保存关卡星级到本地
//     */
//    static void saveLevelStar(int smallLevelId, int rare);
    
    /*
     保存用户奖励领取信息
     */
    static void saveUserAwardInfo(int midLevelId, int num, bool isEqlite = false);//中关卡id, 奖励领取次数
    
    /*
     保存玩家出战信息到本地
     */
    static void saveUserTeamToDB(int monsterId, int iFight);//0:未出战 1:出战
    
    /*
     获取当前等级升级所需经验
     */
    static float getExpInfoByLevel(int level);
    
    /*
     根据小关卡id获取星级条件id
     */
    static std::vector<int> getStarConditionById(int levelId);
    
    /*
     获取中关卡信息
     */
    static std::vector<midLevelInfo> getMidLevelInfoFromDB();
    
    /*
     获取所有星级条件信息
     */
    static std::vector<starInfo> getAllStarInfo();
    
    /*
     根据星级id获取星级条件信息
     */
    static starInfo* getStarInfoById(int starId);
    
    /*
     获取中关卡奖励信息
     */
    static int getMidLevelAwardById(int midLevel, bool ifEqlite = false);
    
    /*
     通过关卡id获取中关卡信息
     */
    static midLevelInfo* getMidLevelInfoById(int midLevelId);
    
    /*
     通过当前中关卡下所有已开启小关卡的星级总数
     */
    static int getAllLevelStarById(int midLevelId, bool ifEqlite = false);
    
    /*
     当前中关卡下小关卡总数
     */
    static int getAllLevelById(int midLevelId);

    /*
     根据小关卡id获取星级
     */
    static int getStarBySmallLevelId(int smallLevelId, bool ifEqlite = false);
    
    /*
     根据小关卡id获取关卡目标图片
     */
    static std::string getImgByLevelId(int levelId);
    
    /*
     根据道具id获取道具信息
     */
    static propInfo* getPropInfoById(int propId);
    
    /*
     根据道具id获取关卡途径
     */
    static std::vector<int> getItemPathById(int propId, bool eQlite = false);
   
    static cocos2d::Sprite * getPropSpriteById(int propId, bool isNeedBg = false);
    
    static PropOpenInfo *getPropOpenInfoById(int iPropId);

    
    /*
     根据怪物id获取怪物星级
     */
    static int getMonsterStarById(int monsterId);
    
    /*
     根据怪物id保存怪物星级
     */
    static void setMonsterStarById(int monsterId);
    
    /*
     //玩家的怪物信息
     */
    static std::vector<myMonster*> getuserMonsterInfo();
    
    /*
     根据怪物id获取玩家怪物信息
     */
    static myMonster * getuserMonsterById(int iMonsterId);
    
    
    static  void SaveMonsterInfoById(myMonster* pMonster);
    /*
     插入一条怪物信息
     */
    static void insertIntoMonsterInfo(int monsterId, int num = 1, int rare = 0, int ifFight = 0);
    
    /*
     //根据怪物id获取怪物信息
     */
    std::vector<FighterSkillInfo>* getFighterSkillInfoById(int monsterId);
    
    /*
     根据等级获取升级信息
     */
    static upgradeInfo* getUpgradeInfoByLevel(int level);

    /*
     根据传入的经验值判断是否升级  param3: 返回升级所提升的体力值
     */
    static void judgeUserIfUpgrade(cocos2d::Node * pTarget, int exp,int *resultAddStamina);
    
    /*
     根据碎片id返回碎片框和头像
     */
    static cocos2d::Sprite* getSpriteIconAndKuangById(int monsterId);
    
    /*
     辅助方法
     */
    static float getTheNumWithMinMax(int minLevel,int maxLevel, float minNum, float maxNum, int nowLevel );//根据最大最小值和最大最小等级来得到当前等级所对应的数值。
    
    
    static void SaveUserPropToDB(int iPropId,int iPropNum);
    
    static std::vector<gameStory> getStoryInfo();
    
    /*
     获取引导总数
     */
    static int getGuideInfoSum();
    
    static std::vector<guideInfo> getGuideInfo();
    
    static std::vector<guidePro> getTheGuideProgress();
    
    /*
     获取即将显示的引导
     */
    static int getGuideStepNum();
    
    /*
     判断引导是否已完成
     */
    static bool getEndedBy(GuideState type);
    
    /*
     判断该引导类型是否已完成
     */
    static int getStoryTypeByLevelId(int Id, int startTime = -1);
    
    /*
     获取某一类型的剧情
     */
    static std::vector<gameStory> getStoryInfoByLevelId(storyState type);
    
    /*
     保存剧情信息到数据库
     */
    static void saveGameStoryInfoToDB(int type);
    
    /*
     判断该引导类型是否存在该引导
     */
    static bool getIsExistedByType(GuideState type);
    
    static std::vector<guideInfo> getGuideInfoByType(int type);
    
    /*
     保存引导信息到数据库
     */
    static void saveGuideInfoToDB(int type);
    
    /*
     保存主线任务进度信息到数据库
     */
    static void saveTaskInfoToDB(int taskType, int num = 1, bool ibMax = false);
    
    /*
     根据任务类型获取任务进度
     */
    static int getTaskProByType(int taskType);
    
    /*
     保存玩家登录时间
     */
    static void saveUserLoginTime(std::string time);
    
    /*
     保存刷新每日任务时间
     */
    static void saveUpdateDailyaTaskTime(std::string time);
    
    /*
     保存刷新用户兑换金币, 体力次数
     */
    static void saveUpdateExchangeNum(int type = 0);//0:表示金币兑换,1:表示体力兑换
    
    /*
     保存刷新用户兑换次数的时间
     */
    static void saveUpdateExchangeNumTime(std::string time);
    
    /*
     保存刷新用户兑换次数
     */
    static void refreshUserInfoNum();
    
    /*
     保存刷新用户兑换次数
     */
    static userNum* getUserInfoNum();
    
    /*
     保存刷新每日任务
     */
    static void refreshDailyTask();
    
    /*
     更新任务的状态
     */
    static void saveTaskState(int taskType, TaskState state, bool bTaskId = false);//bTaskId为ture则表示taskType指的是taskId
    
    /*
     删除一个任务
     */
    static void eraseTaskById(int taskId);
    
    /*
     根据任务id获取任务信息
     */
    static TaskBase* getTaskInfoById(int taskId);
    
    /*
     获取所有的任务信息
     */
    static std::vector<TaskBase*> GetAllTaskInfo();
    
    /*
     修改玩家id和验证码
     */
    static void setUserIdAndAuthCode();
    
    
    /*
     随机获得一条tips信息
     */
    static std::string getRandomTips();
    
    //添加更新数据库的借口
    /*
     保存玩家信息
     */
    static void saveUserAllInfo();
    
    /*
     保存玩家引导信息
     */
    static void saveUserGuideInfo();
    
    /*
     保存玩家任务信息
     */
    static void saveUserTaskListInfo();
    
    /*
     保存玩家任务进度信息
     */
    static void saveUserTaskInfo();
    
    /*
     保存玩家怪物信息
     */
    static void saveUserMonsterInfo();
    
    /*
     保存玩家物品信息
     */
    static void saveUserItemsInfo();
    
    /*
     获取充值信息
     */
    static std::vector<chargeInfo> getChageInfo();
    
    /*
     根据商品id获取充值信息
     */
    static chargeInfo* getChargeInfoById(int chargeId);

    /*
     获取玩家委发出的消息
     */
    static std::vector<std::string> getMessageInfo();
    
    /*
     保存未发出的网络消息
     */
    static void insertMessageInfo(std::string meesgaeInfo);
    
    /*
     保存未发出的网络消息
     */
    static void delMeeageInfo(std::string meesgaeInfo);
    
    /*
     根据建筑id获取建筑的布阵信息
     */
    static pvpBuildInfo* getBuildInfo(int buildId);
    
    /*
     根据建筑类型获取初级建筑的id
     */
    static int getBuildIdByType(int buildType);
    
    /*
     获取所有建筑类型的数量
     */
    static int getBuildTypeNum();
    
    /*
     获取所有录像列表
     */
    static std::vector<videoListInfo> getVideoListInfo();
    
    /*
     保存玩家录像
     */
    static void saveVideoInfo(std::vector<videoListInfo> tempVec);
    
    /*
     保存玩家指定录像是否可被查看
     */
    static void saveVideoById(int videoId);
    
    /*
     保存玩家不可复仇标识
     */
    static void saveRevenueFlag(int videoId);
    
    /*
     保存玩家领取奖励状态
     */
    static void saveRewardInfo(int videoId);
    
    /*
     保存玩家录像为非新的状态
     */
    static void saveNewVideoInfo();
    
    //解析服务器返回字符串
    static std::string getDigitFromBackStr(std::string str);
    static std::string getStrFromBackStr(std::string str);
};

#endif /* defined(__TheGodWill__DataManager__) */
