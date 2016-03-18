//
//  AllDefine.h
//  TheGodWill
//
//  Created by user on 14/12/1.
//
//

#ifndef __TheGodWill__AllDefine__
#define __TheGodWill__AllDefine__

#include <stdio.h>

#include "cocos2d.h"


#define INVALIDE_VALUE -1

#define AtrEffect 0.25



typedef enum  {
    eFighteState_Idle=0,
    eFighteState_Walk,
    eFighteState_Attack,
    eFighteState_SpecialAttack,
    eFighteState_Hurt,
    eFighteState_Die,
    eFighteState_Dizzy,
    
    
    eFighteState_None,
} FighteState;

typedef enum{
    
    eFightGroup_Freind,
    eFightGroup_Enemy,
    
    eFightGroup_midFight,//中立攻击者
    eFightGroup_midNotFight,//中立不攻击者
    
    efightGroup_err,
    
    
}FightGroup;


typedef enum{
    
    eAttackType_hero=0,//英雄
    eAttackType_monster,//怪物
    
    eAttackType_min,    //产能量资源
    eAttackType_minMonster,//产怪物资源
    eAttackType_tower,  //中立防御塔
    
    eAttackType_lifeWater,//生命之泉
    
    eAttackType_wall,//城墙
    
    eAttackType_city, //主基地
    
    eAttackType_protector,//英雄的守护者，暂时只有女主角
    
    eAttackType_walker,//护送者
    
    eAttackType_Trap,//陷阱
    
    eAttackType_Partol,//巡逻的人
    
    eAttackType_NPC,//npc
    
}AttackType;

enum DamageType
{
    eDamageType_GeneralAttack = 0,//普攻
    eDamageType_SoliderSkill,//士兵技能
    eDamageType_HeroSkill,//英雄技能
    
    eDamageType_StoneSkill,//投石机技能
    
    eDamageType_System,//系统让其死亡
    
    eDamageType_Upgrade,//塔防模式升级
};

typedef enum{
    
    eFighterType_line=0,//横行兵种 ，均衡行
    
    eFighterType_triangle,//三角形兵种 ，敏捷型
    eFighterType_circle,//圆形兵种， 远程
    eFighterType_square,//方形兵种，肉盾型
    
    eFighterType_v,//V形兵种
    eFighterType_enemy,//敌人
    eFighterType_toushicheluodian,//投石车落点
    eFighterType_Tower,//塔
    
    eFighterType_Trap,//陷阱
    
}FighterType;


typedef enum{
    
    eSkillType_static,//静止不动的技能
    
    eSkillType_Move,//会移动的技能
    
    
    
}SkillType;

typedef enum{
    
    eEnemyType_normal,
    eEnemyType_boss,
    
}EnemyType;

typedef enum{
    eLevelCondition_destroyCity,//推掉基地
    
    eLevelCondition_protect,//护送
    
    eLevelCondition_kill,//杀怪
    
    eLevelCondition_defense,//防守
    
    eLevelCondition_fastKill,//速攻
    
    eLevelCondition_killBoss,//杀死boss
    
    eLevelCondition_TowerDefense,//塔防模式
    eLevelConditon_error,
    
}LevelCondition;


typedef enum{
    
    eChooseCondition_random=0,//随机
    eChooseCondition_minHp=1,//血量最低
    eChooseCondition_maxHp=2,//血量最高
    eChooseCondition_minDist=3,//离得最近
    
    eChooseCondition_cure=4,//治疗规则
    
    
}ChooseCondition;



typedef enum{
    
    eAllSkills_shengpan=1, //审判
    eAllSkills_morishengpan, //末日审判
    eAllSkills_luanshe,     //乱射
    eAllSkills_zhimingsheji,  //致命射击
    eAllSkills_dundang,       //盾挡
    eAllSkills_dunfan,        //盾反
    eAllSkills_huoqiu,        //火球
    eAllSkills_bingdong,      //冰冻术
    eAllSkills_jushipifu,     //巨石皮肤
    eAllSkills_jurenhuiji,    //巨人挥击
    eAllSkills_chongfeng,     //冲锋
    eAllSkills_jianta,        //践踏
    eAllSkills_huichunshu, //回春术
    eAllSkills_yexinggrow, //野性成长
    eAllSkills_huifushu,//恢复术
    eAllSkills_Bintuanyoushi = 16,//兵团优势=16
    eAllSkills_shechenlong = 17,// 强弓
    eAllSkills_xixue=18,//吸血
    eAllSkills_reborn=19, //重生
    eAllSkills_shuanrenjian=20,//双刃剑
    eAllSkills_wushuyanxi=21,//巫术研习
    eAllSkills_GuduYanxi=22,//蛊毒研习
    eAllSkills_Shixue=23,//嗜血
    eAllSkills_Xuanfengzhan=24,//旋风斩
    eAllSkills_juDun=25, //举盾
    eAllSkills_renShitou=26,//巨石投掷
    eAllSkills_huoyanpifu=27,//火焰皮肤
    eAllSkills_yuansuqiyue=28,//元素契约
    eAllSkills_ziranyuhe=29,//自然愈合
    eAllSkills_yushou=30,//御兽
    eAllSkills_shihua=31,//石化
    eAllSkills_silinqiyue=32, //死灵契约
    eAlSkills_wanzheyiyan=33,//亡者遗言
    eAllSkills_linTI =34, //灵体
    eAllSkills_Feilongdu=35,//腐蚀吐息
    eAllSkills_Huanjiakaijia=36,//皇家铠甲
    eAllskills_Zibao = 37, //自爆
    
    eAllSkills_Qiannenjifa=99,//潜能激发
    eAllSkills_toushi=102,//投石车
    
    eAllSkills_Trap_Single=201,//单体伤害陷阱
    eAllSkills_Trap_Range=202,//范围伤害陷阱
    eAllSkills_Trap_Dinshi=203,//定时伤害陷阱
 
    
}AllSkills;

typedef enum{
    
    eAllBuffes_bingdong=1,
    eAllBuffes_xuanyun,
    eAllBuffes_jitui,
    eAllBuffes_chongfeng,
    eAllBuffes_jiansu,
    eAllBuffes_du,//毒
    eAllBuffes_huoyanskin,//火焰
    
    
}AllBuffes;


struct pvpEnemyInfo //敌人信息
{
    int m_iUserId;
    std::string m_sName;
    int m_iLevel;
    int m_iRobStone;
};


typedef enum{
    
    eShadeType_TurnWhite,
    eShadeType_TurnGold,
    eShadeType_3D_Elite, //精英
    eShadeType_3D_Boss, //boss
    eShadeType_3D_beAttacked, //被攻击
    eShadeType_3D_BossBeAttacked,//boss被攻击
    
}ShadeType;





/*
 下面是结构体
 */


struct animateFrameInfo{
    
    float start;
    float duration;
    
};

struct IdAndTwoNum
{
    int theId;
    int level;
    int number;
};

struct IdAndNum{  //公用的类，存储对应id下的对应数量
    
    int theId;
    
    int theNumber;
};

struct animatePathAndNum{
    
    std::string animatePath;
    
    int animateNumber;
    
};


//struct SkillInfo{
//
//    int skillId;//技能id
//
//    float damageRatio;//伤害系数
//
//    float triggerRatio;//触发几率
//
//
//
//};


struct starInfo{
    starInfo()
    {
        monsterId = -1;//默认值
    }
    int starId;
    std::string starDesc;
    int starType;
    int monsterId;//怪物id
    int num;
    int positive;//是否是> 0:< 1:>
};

struct modelInfo{
    
    int modelId;
    
    cocos2d::__String modelName;//模型路径
    
    animateFrameInfo idleFrame;  //相关动画帧数信息
    animateFrameInfo walkFrame;
    animateFrameInfo attackFrame;
    animateFrameInfo specialAttackFrame;
    animateFrameInfo hurtFrame;
    animateFrameInfo DieFrame;
    animateFrameInfo DizzyFrame;
    
    int radius;//碰撞半径
    
    float attackDamageTime;//普通攻击的伤害时间点
    float SpecialDamageTime;//特殊攻击的伤害时间点
    
    float modelScale;
    
    std::vector<std::string> vecAllPng;//模型所用到的贴图文件路径
    
};

//itemId	itemName	itemDes	itemIcon	itemLevel

//道具信息
struct propInfo
{
    int m_itemId;
    std::string m_sItemName;
    std::string m_sItemDes;
    std::string m_sItemIcon;
    int m_iItemLevel;
    bool m_bUse;
    int m_iNeedPropId;
    int m_iNeedPropNum;
    int m_iSalePrize;
    
    float m_fDropRatio;
};

//道具打开信息
struct PropOpenInfo
{
    int m_id;
    int m_iUserItem; //物品id
    int m_iGotItem; //打开宝箱获得物品id
    int m_itemType;  //物品类型 道具，碎片，还是金币
    int m_iGotItemNum;
    int m_iGotMoney;
    int m_iGotStone;
    std::string  m_sRatio;
};



//我的道具
struct MyPropInfo
{
    int m_iPropId;//道具id
    int m_iPropNum;
};



//抽卡结果表
struct  DrawCardResultInfo
{
    int m_id;
    int m_iType; //0 1道具 2碎片 3魔法石
    int m_iGotItem;//抽到的id
    int m_iGotNum;//抽到的个数
    std::string  m_sRadio;//随机值
    int m_iGotMoney; //抽到的数据
    bool m_bCoin; //1为金币抽卡 0为魔法石抽卡
};


struct ConfigureInfo
{
    int m_id;
    int m_iDrawOneCoin;
    int m_iDrawOneStone;
    int m_iDrawTenCoin;
    int m_iDrawTenStone;
    int m_iDrawOneTime;
    int m_iDrawTenTime;
    int m_iDrawOneNum;  //金币总共次数
    int m_iDrawMagicNum; //魔法石总共次数
    
};

struct  TempSaveInfo
{
    int m_id;
    int m_iCoinTime; //金币刷新时间
    int m_iStoneTime; //魔法石刷新时间
    int m_iCoinNum;  //金币次数
    int m_iStoneNum; //魔法石次数
    bool m_bFirstDrawCoin;//首次抽金币
    bool m_bFirstDrawStone;//首抽魔法石
    std::string iHaveUsers;//当天已经搜索到的玩家id
    
};


struct skillInfo{
    
    int skillId;
    
    std::string skillName;
    
    std::string skillDes;
    
    std::string skillIcon;
    
    int skillType;//0表示造成伤害，1表示造成治疗
    
    double damageRatioMin;//最小伤害系数
    
    float startRatioMin;//最小出现几率
    
    double damageRatioMax;
    
    float startRatioMax;
    
    int skillEffectId;
    
    int maxLevel;
    
    float moveSpeed;   //移动速度大于0,则特效初始点在施法者，然后从施法者飞到目标点，移动速度为0表示，直接出现在目标点位置上
    float moveSpeedY;  //y轴，也就是高度轴上的速度
    
    int damageRange;
    
    int hurtNumbers; //1对单个伤害,>1范围伤害
    
    float lifeTime;         //生命周期  0表示动画结束时攻击指令结束,-1表示到达目标点后消失,-2表示造成伤害后就自毁
    
    std::vector<float> hurtTime; //造成伤害的时间跟动画进行调(一般0.5)，如果hurttime为-1，则每帧都判断造成伤害，造成伤害后就自毁
    
    int hurtType;//0表示从一开始计算伤害，1表示从到达目标点后计算伤害
    
    int buffId;//0表示没有buff
    
    int positionGroup;//表示释放的目标类型。  0表示友军，1表示敌人，-1表示自己
    
    ChooseCondition positionCondition;//表示挑选目标位置规则 0,随意。 1，血量最少。2，血量最多。3，离得最近 4.治疗规则
    
    int effectGroup;//表示受影响的阵营  0表示友军，1表示敌人，-1表示自己
    
    ChooseCondition effectCondition;//表示挑选目标位置规则 0,随意。 1，血量最少。2，血量最多。3，离得最近,4.治疗规则
    
    int ifLocked;//表示是否锁定。  0表示不锁定，1表示锁定。  锁定就是以敌人为目标，敌人到哪也躲不过。非锁定就是以敌人所在坐标为目标。敌人走了还是打在原地
    
    cocos2d::Vec3 offset;//偏移
    
    int startCondition;//0表示攻击时触发，1表示被攻击时触发，2表示移动时触发，3表示初始化时触发  4.时间隔触发 5死亡时触发 6.进入攻击范围就触发
    
    std::string skillNamePic;//技能名字图片，用于释放技能
    
    
    std::string skillEffect;//技能音效
    
};

struct upgradeInfo{
    int level;          //等级
    int upperLevel;     //等级上限
    int upgradeExp;     //到本级所需经验
    int upperStamina;   //体力上限
    int upgradeStamina; //升级回复的体力
    int userExp;        //玩家总的经验
};

struct fightBuildingInfo{
    int monsterId;
    std::vector<int> upgradeMonstersId; //升级的怪物id信息
    int towerType;                      //塔类型：0基础塔，1箭塔，2魔法塔，3炮塔，4能量泉，5兵营，6城墙
    std::string towerIcon;              //防御塔图标
    int buildCost;                      //建筑消耗
    int saleGold;                       //售出所得金币
};



struct guideInfo{
    int guideId;
    int type;//类型
    int stepId;//该类型下的步骤
    std::string icon;//美女图标
    std::string desc;//引导语言
    int limitType;//受限的引导类型id
};

struct fightTalkingInfo{
    
    int talkId;
    int type;
    std::string iconPath;
    int iconNum;
    std::string content;
    int nextTalkId;
    
};



struct guidePro
{
    int typeId;
    int isEnd;//当前类型的引导是否完成
};

struct skillEffect{
    
    int skillEffectId;
    
    int effectType;//0为模型，1为帧动画
    
    std::string skillPath;
    
    int skillFrameNumber;
    
    cocos2d::Vec3 offset;
    
    float effectScale;
    
    animatePathAndNum hurtAnimate;
    
    animatePathAndNum bombAnimate;
    
    skillEffect(){
        
        hurtAnimate.animatePath="";
        hurtAnimate.animateNumber=0;
    }
    
    
};

struct locationInfo
{
    locationInfo()
    {
        sprite = NULL;
        type = 0;
    }
    cocos2d::Sprite3D* sprite;
    int type;//0:表示未占用, 1:表示暂时占用, 2:表示已占用
};

struct partolInfo
{
    int monsterId;
    int attack;
    int life;
    std::string monsterIcon;//进化怪物头像
};

struct levelBuildInfo
{
    int buitldType;
    int num;
    int level;
};

struct pvpBuildInfo
{
    int buildingID;//建筑id
    int buildingType;//建筑类型 0:主基地,1:魔法石库,2:熔炉,3:弓箭塔,4:加农炮,5:投石车,6:城墙,7:群体陷阱,8:单体陷阱,9:防空炮;10:定时炸弹,11:祭坛,12:定时炸弹
    int monsterId;
    int staticModelId;//塔底座部分id
    std::string texturePath;//塔贴图路径
    int activeModelId;//塔上部可活动部分id
    int level;//主基地当前等级
    int cost;//升到此级所需金币
    int costTime;//升到此级所需时间
    std::vector<levelBuildInfo> allBuildLimitVec;//如果是主基地，则表示该等级下的建筑属性限制，否则表示容量
    std::vector<partolInfo> allEvolutionVec;//如果是祭坛，表示该等级下的巡逻者的属性
    int effectTime;
    std::vector<cocos2d::Vec2> blockPos;//建筑所占格子布局
    int xPosType;//0:表示坐标在方块内，1表示坐标在线上
    int yPosType;
    int nextBuildingID;//下一级升级信息
    int life;//生命值
    int attack;//攻击力
    std::string desc;//用途描述
    std::string icon;//塔的2d图形
    std::vector<std::string> modelImgVec;//各等级所对应的建筑图标
    int useType;//建筑用途类型
};

struct videoListInfo
{
    int videoId;//录像id
    int enemyUserId;//敌方玩家id
    std::string enemyName;//昵称
    int enemyLevel;//等级
    std::vector<IdAndTwoNum> enemyInfo;//敌方士兵信息 id,数量
    int robStone;//抢到的魔法石
    int state;//是否被查看 0:未查看，1:已被查看
    int uploadTime;//攻击时间
    int iWin;//标识是否防守成功 0:成功 1:失败
    int iGetWard;//是否领取了奖励 0:未领取 1:已领取
    int bValid;//是否可以查看录像 0:可查看 1:不可查看
    int iAttack;//是否可以复仇 0:可复仇 1:不可复仇
    int destroyDegree;//建筑的摧毁比例
    videoListInfo()
    {
        bValid = 0;
        iAttack = 0;
    }
};

struct chargeInfo
{
    int chargeId;
    std::string icon;
    std::string desc;
    int num;
    int price;
    int type;//0:什么都没 1:限购 2:热销 3:both
    int disCount;//折扣
};

struct gameStory//情节对话
{
    int storyType;
    int levelId;
    int startTime;
    int step;
    std::string name;
    std::string icon;
    std::string content;
    int posType;
};


//用户名字
struct UserName
{
    int m_id;
    std::string m_sUserName1;
    std::string m_sUserName2;
};

struct heroSkill{ //存在数据库的英雄技能信息
    
    int heroSkillId;
    
    int m_iMonsterId;
    std::string heroSkillName;
    
    std::string heroSkillDes;
    
    float damageMin;
    
    float damageMax;
    
    float lastTimeMin;
    
    float lastTimeMax;
    
    int skillCD;
    
    int costEnergy;
    
    std::string skillIcon;
    
    int levelLimit;
    
    int levelMax;
    
};

struct heroSkillInfo{ //转换后的英雄技能信息
    
    heroSkill* theSkill;
    
    int level ;
    
    int damage;
    
    int lastTime;

    heroSkillInfo(){
        theSkill=NULL;
    }
    
};



struct  FighterSkillInfo{//转换后的技能信息
    
    skillInfo* theSkillInfo;
    
    skillEffect* theSkillEffectInfo;
    
    int skillLevel;
    
    float damageRatio;
    
    float startRatio;
    
    FighterSkillInfo(){
        theSkillInfo=NULL;
        theSkillEffectInfo=NULL;
    }
    
};

struct bulletInfoDB{
    
    int bulletId;
    
    std::string filePath;
    
    int bulletType;//0为抛射，1为平射
    
    int bulletMinRange;//最小发射距离，如果和敌人之间距离比这个小，则直接造成伤害，没有子弹效果
    
    int bulletMaxRange;//子弹所能走得最远距离，超出则消失。防止平射时子弹永远不停。
    
    int bulletDamageRange;//受伤范围，当到达目标点后，
    
    float lifeTime;//停止移动后的存在时间
    
    float bulletSpeed;
    
    std::string hitEffect;//子弹命中音效
    
    std::string particlePath;
    
    float bulletScale;
    
    animatePathAndNum hurtAnimate;
    

    
    bulletInfoDB(){
        
        particlePath = "0";
        
        hurtAnimate.animateNumber=0;
        hurtAnimate.animatePath="0";
    }
    
};

struct monsterInfo{
    monsterInfo()
    {
        attackEffect = "";
        dieEffect = "";
    }
    
    int monsterId;
    
    FighterType monsterType;
    
    std::string monsterName;
    
    std::string monsterIcon;//怪物头像
    
    std::string monsterIntroduce;
    
    float walkSpeed;
    
    int attackRange;
    
    float attackCD;
    
    int energyCost;
    
    std::vector<int> allSkill;//技能
    
    int modelId;
    
    int minAtk;
    
    int maxAtk;
    
    int minHp;
    
    int maxHp;
    
    int weight;//重量
    
    int maxLevel;
    
    int bulletId;
    
    int isHaveDebris;//是否可以显示怪物在列表中，0:表示可以显示。1：不能显示
    
    std::string attackEffect;//攻击音效
    std::string dieEffect;//死亡音效
    
    bool ifFly;//是否空军
};

struct FighterInfo{ //转换后的士兵信息
    
    int monsterId;
    
    FighterType monsterType;//士兵类型
    
    modelInfo* theModelInfo;
    
    monsterInfo* theMonsterInfo;
    
    bulletInfoDB* theBulletInfo;
    
    std::vector<FighterSkillInfo> theFighterSkillInfo;
    
    int level;
    
    float walkSpeed;//走路速度
    
    float attackRang;//攻击距离
    float attackCD;//攻击间隔
    
    int energyCost;//消耗能量
    
    int attk;//攻击力
    int maxHP;//最大血量
    
    int theType;//0怪物 1英雄
    
    int skillId;//技能id
    cocos2d::__String skillPath; //技能动画名
    int skillFrameNum;//技能动画帧数量
    int ifFight;//是否出战
    int rare;//怪物星级
    
    FighterInfo()
    {
        skillId=0;
        skillPath="";
        skillFrameNum =0;
        monsterId = 0;
        theModelInfo = NULL;
        theMonsterInfo = NULL;
        theBulletInfo = NULL;
    }
};

//怪物数量
struct debrisNumInfo
{
    int monsterId;//id
    int num;//数量
};

struct myMonster{ //用于存储士兵信息
    
    
    int monsterId;
    
    int monsterLevel;
    
    int skillLevel1;
    int skillLevel2;
    int skillLevel3;
    int skillLevel4;
    
    int ifFight;//是否出战 1出战 0不出站
    
    int theType;//0怪物 1英雄
    
    int rare;//怪物星级
    int num;//碎片
};


//战斗过程中星级判断
struct levelStarInfo
{
    int starType;//星级类型
    int monsterId;//兵种id
    int num;//时间或者次数
    //int positive;//大于或小于num
};



struct AttackCommandInfo{
    

    float MaxlifeTime;//生命周期  0表示动画结束时攻击指令结束,-1表示不会结束。
    
    float damageRange;//伤害半径
    float damageRatio;//伤害系数
    
    int hurtNum;//攻击数量，打到攻击数量后不攻击了
    
    
    
    cocos2d::__String bulletName;//子弹模型路径
    
    
    SkillType theSkillType;//技能攻击类型
    
    float attackMoveSpeed;//技能子弹移动速度
    
};

struct mapInfo{
    
    int mapId;
    
    int monsterId;
    
    int monsterLevel;
    
    
    AttackType buildingType;
    
    FightGroup buildingGroup;
    
    cocos2d::Point pos;
    
};

struct mapFightInfo{//转换后的地图信息
    
    mapInfo* TheMapInfo;
    
    FighterInfo theFightInfo;
    
    mapFightInfo(){
        TheMapInfo = NULL;
    }
    
};


struct bombTime{//大波僵尸来得信息
    
    int theBombTime;//大波僵尸来的时间点
    
    int theBombNum;//僵尸数量
    
    int theBossNum;//boss数量
    
};



struct smallAwardInfo{//小关卡奖励，
    int type;//id代表0:物品 1:英雄碎片
    int Id;//物品id
};

struct levelInfo{
    
    int levelId;
    
    std::string levelName;
    
    std::string levelDes;
    
    int cityLevel;//主城等级
    
    int goldNum;
    
    int playerExp;
    
    std::vector<smallAwardInfo> itemsVec;
    
    std::vector<smallAwardInfo> heroItemsVec;
    
    std::vector<int> starIds;
    
    std::vector<int> monsterGroup;
    
    int bossId;
    
    int monsterLevel;
    
    int monsterSkillLevel;
    
    int energySpeed;
    
    std::vector<bombTime> theBombInfo;
    
    int levelCondition;
    
    std::string levelConditionDes;
    
    int conditionNum;
    
    int mapId;
    
    int stamina;//关卡消耗的体力
};

struct levelAndStar{
    int level;
    int star;
};

struct levelAwardInfo{
    int midLevel;
    int rewards;//0:未领取，1:领取一级奖励
};

struct UserInfo
{
    int m_id;
    int  m_iUserId;
    std::string userName;
    int m_iUserLevel;
    int m_iUserExp;
    std::string m_sCoin;
    std::string m_sMagicStone;
    std::string m_sStaminaNum;//体力值
    long m_iStaminTime;
    std::string m_iTimeStamp;//任务刷新时间
    std::string m_sMaxStaminaNum;//最大体力值
    int m_iSmallLevel;//小关卡通关数
    int m_iEqliteSmallLevel;//精英模式下小关卡通关数
    int m_towerLevel;//塔防模式下通关数
    std::string m_sloginDay;
   // std::vector<IdAndNum> m_heroSkillsVec;
    std::vector<levelAwardInfo> m_userMidLevelRewardVec;
    std::vector<levelAwardInfo> m_userHeroMidLevelRewardVec;
    std::string m_sAuthCode;//验证码
    std::string m_strBudilingLv; //建筑等级
    std::string m_strEnergyLv;  //能量等级
    std::vector<debrisNumInfo> m_useDebrisVec;
    std::vector<levelAndStar> m_userLevelVec;
    std::vector<levelAndStar> m_userEqliteLevelVec;//精英模式下关卡星级信息
    std::vector<int> m_userTowerLevelVec;//塔防模式下关卡信息
    std::string m_skillUpgradeInfo;
    std::string m_strHeroLv;//英雄等级 id,等级；id,等级
    int DBVersion;
    std::vector<int> storyInfo;
    unsigned long long serverLogintime;
    int m_mainBaseLevel;//玩家主基地等级
    int ifSuggesed;
    std::string m_kuangshiMaxNum;//记录玩家当前矿石上限
    
    std::string m_pvpStoneNum;//pvp矿石数量
    std::string m_pvpStoneMaxNum; //pvp矿石总量
    
    UserInfo()
    {
        m_iStaminTime = 0;
        m_iUserLevel = 1;
        m_iUserExp = 1;
        serverLogintime = 0;
        ifSuggesed=0;
    }
};


struct MonsterDebuff{
    
    MonsterDebuff()
    {
        ifFlyOut = false;
        ifShake = false;
        ifFreeze = false;
        ifBeatBack = false;
        ifDizzy = false;
    }
    bool ifFlyOut;//标识士兵是否处于飞出状态
    bool ifShake;//标识士兵是否处于抖动状态
    bool ifFreeze;//标识士兵是否处于冰冻状态
    
    bool ifSlow;//标识士兵是否处于减速状态
    
    bool ifDizzy;//标识士兵是否处于眩晕状态
    bool ifBeatBack;//标识士兵是否处于被击退状态
    
    bool ifCharge;//标志士兵是否处于冲锋状态
    
    bool ifDu; //标志士兵是否处于中毒状态
    bool ifhuoyanSkin;//标志是否处于火焰皮肤

    
};

struct awardInfo{
    int type;//奖励类型 0:道具 1:金币 2:魔法石
    int num;//数量
    int PropId;//道具id，如果是金币和魔法石默认为0
};

struct midLevelInfo
{
    int midLevelId;
    std::string name;
    std::string icon;
    float xpos;
    float ypos;
    int firstLevel;//当前关卡的第一小关
    int lastLevel;//当前关卡的最后一小关
    std::vector<awardInfo> awardVec;
    std::vector<awardInfo> heroAwardVec;
    std::vector<cocos2d::Vec2> pointPocVec;
};


struct reinforcementInfo{
    
    int levelId;
    int monsterId;
    int monsterLevel;
    int monsterGroup;
    int happenTime;
    
    std::vector<cocos2d::Vec2> allPos;
    
    FighterInfo theMonsterInfo;
    
};


struct userInfoInDB{
    
    int m_id;
    int userId;
    
    std::string userName;
    
    int userLevel;
    
    int userExp;
    
    std::string coinStr;
    
    std::string magicStoneStr;
    
    std::string staminaStr;//当前体力
    
    long staminaTime;//当前体力时间
  
    int maxStamina;//体力上限
    int smallLevel;//小关卡通关数
    int eqliteSmallLevel;//精英模式下小关卡通关数
    int towerLevel;//塔防模式下通关数
    std::string loginDay;
    std::string strBuildingLv;
    std::string strEnergyLv;
    std::vector<IdAndNum> heroSkills;
    std::vector<levelAwardInfo> userMidLevelRewardVec;
    std::vector<levelAwardInfo> userHeroMidLevelRewardVec;
    std::string authCode;
    std::vector<debrisNumInfo> m_debrisVec;
    std::vector<levelAndStar> userLevelVec;
    std::vector<levelAndStar> userEqliteLevelVec;//精英模式下关卡星级信息
    std::vector<int> userTowerLevelVec;//塔防模式下关卡信息
    std::string m_skillUpgradeInfo; //技能升级情况比如:1,2,1000 怪物id为1，技能id为2,升级时间为1000
    std::string taskTimeStamp;//任务更新是时间
    std::string m_strHeroLv;//英雄等级 id,等级；id,等级
    int DBVersion;
    std::vector<int> storyInfo;
    
    int ifSuggesed;//是否评论过
    
};


typedef enum TaskState
{
    Task_Go = 0, //前往
    Task_Get,//领取
    Task_Fininsh, //任务已完成
    
}TaskState;

struct taskProInfo//任务进度
{
    int m_taskType;
    int m_num;
    taskProInfo()
    {
        m_num = 0;
    }
};

//任务数据结构
typedef  struct TaskBase
{
    int m_iTaskId;
    int m_iTaskType;
    std::string m_sDesc;
    std::string m_taskIcon;
    int m_iNum;
    int m_iMoney;
    int m_iExp;
    int m_stone;
    std::vector<IdAndNum> itemVec;//获取的物品
    std::vector<IdAndNum> heroItemVec;//英雄碎片id:数量
    int m_iTaskIdLimmit;
    int m_iLvLimmit;
    TaskState m_eTaskState; //任务状态
    int taskProp;//标识是否是每日任务 1:每日任务, 0:主线任务
//    TaskBase()
//    {
//        m_iTaskId  = 0;
//        m_iTaskType= 0; //0主线任务 1支线任务  3日常任务
//        m_sDesc = "";
//        m_iMoney = 0;
//        m_iNum = 0;
//        m_iExp = 0;
//        m_iTaskIdLimmit = 0;
//        m_iLvLimmit = 0;
//        m_eTaskState = Task_Go;
//    }
}TaskBase;

//与用户相关的次数统计 ，以后可能会累加
struct userNum
{
    int m_id;
    int exchangeCoinNum;//兑换次数
    int exchangeStaminaNum;//兑换体力次数
    std::string refreshTime;//刷新时间
};

//商店表
typedef  struct ShopItemInfo
{
    int m_id;
    int m_iItemId;
    int m_itemType; //0 道具 1宝箱 2为碎片
    int m_iCostType; //0金币 1魔法石
    int m_iPrize;
    int m_iNum;
    std::string m_saleType; //卖出类型 1,2,3
    int m_iRebate; //概率
}ShopItemInfo;

//技能升级表
typedef struct SkillUpgradeInfo
{
    int m_id;
    int iSkillLevel;
    std::string m_strItems;
    int m_iNeedTime;
    FighterType m_eMonsterType;
}SkillUpgradeInfo;

//英雄技能升级表
typedef  struct heroSkillUpgradeInfo
{
    int m_id;
    int m_heroSkillId;

    int m_SkillLevel;
    std::string m_strItems;
    int m_iNeedTime;
}heroSkillUpgradeInfo;

//能量升级表
typedef struct energyUpgradeInfo
{
    int m_id;
    int m_iEnergyUpgradeId;
    int m_iUpgradeLevel;
    std::string m_sName;
    std::string m_sDesc;
    int m_iCostGold;
    int m_iAffectNum;
    std::string m_sIco;
}EnergyUpgradeInfo;


//建筑升级表
typedef struct buildingUpgradeInfo
{
    int m_id;
    int m_iBuildingUpgradeId;
    int m_iUpgradeLevel;
    std::string m_sName;
    std::string m_sDesc;
    int m_iCostGold;
    int m_iAffectNum;
    std::string m_sIco;
}BuildingUpgradeInfo;


//怪物升级表
typedef struct MonsterUpgradeInfo
{
    int m_id;
    int m_iGoldNum;
}MonsterUpgradeInfo;

//怪物进阶信息
struct MonsterEvolutionInfo
{
    int m_star;//星级
    int m_number;//数量
    int m_addHp;//增加的血量
    int m_addAtk;//增加的攻击力
    int m_skillNum;//技能数
    int m_coinNum;//进阶需要的金币
};

//怪物进阶累计加的攻击和血量
struct MonsterAllEvolutionInfo
{
    int m_allAddAtk;//增加的攻击力
    int m_allAddHP;//增加的生命值
    
    MonsterAllEvolutionInfo(){
        m_allAddAtk=0;
        m_allAddHP=0;
    }
    
};

//tips信息
struct tipsInfo{
    
    int tipsId;
    
    std::string content;

};

struct UserJumpInfo
{
    UserJumpInfo()
    {
        monsterId = 0;
        skillId = 0;
        type = -1;
    }
    long monsterId;//怪物id
    long skillId;
    int type;//标识是士兵技能还是英雄技能 0:士兵， 1:英雄
};


#endif /* defined(__TheGodWill__AllDefine__) */
