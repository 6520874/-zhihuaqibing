//
//  LYFightData.h
//  TheGodWill
//
//  Created by user on 14/12/1.
//
//

#ifndef __TheGodWill__LYFightData__
#define __TheGodWill__LYFightData__

#include <stdio.h>
#include "AllDefine.h"

#include "cocos2d.h"
//#include "AllHead.h"

#include"LYBaseFighter.h"

#include "fightScene.h"

#define CITY_MONSTER_ID 999  //怪物主基地的id

#define CITY_FREIND_ID  1007 //我方主基地的id

#define PROTECTOR_ID 1005 //守护者的id

#define MAX_PATH_WEIGHT 99999

#define TD_BASE_TOWER_ID  10000

struct pathPos{
    
    Vec2 thePoint;

    int weight;//权重
    
    bool ifActive; //是否激活
    
    bool availible; //是否可用
    
    bool ifHaveOne;//是否有敌人在上面，此处用于建筑的虚化，防止怪物被遮挡
    
    bool ifNeedVirtual;//如果有建筑在这个路点，则表示判断是否需要虚化
    
    std::vector<pathPos*> links; //相连节点
    
    
    cocos2d::Label* temp ;//测试用
    
};


struct DrawCardInfo
{
    int m_iCoindrawtime;
    int m_iStonedrawtime;
    int m_iCoindrawNum;
    int m_istoneDrawNum;
    bool  m_bfirstDrawCoin;
    bool m_bfirstDrawStone;
    int  m_iServerTime;
    
};


enum fightMode
{
    FIGHT_COMMON_TYPE = 0,
    FIGHT_TOWERDENFESE_TYPE,
    FIGHT_PVP_TYPE,
};


//class CBaseFighter;
class CBaseHero;
class CBaseCity;

class CSmallMap;
class CFightData {
    
public:
    
    static CFightData* getInstance();
    
    CFightData();
    ~CFightData();
    
    
    void newLevelStart(int levelId=-1); //-1表示清空数据但是不初始化数据
    
    void newPVPStart();
    
    void restartLevel();
    
    
    void addMonster(CBaseFighter* aa,AttackType theType=eAttackType_monster);
    void addSoldier(CBaseFighter* aa);
    void addEnemy(CBaseFighter* bb);
    
    void removeAttacker(CBaseFighter* cc);
    
    
    void addEnergyWith(float number,FightGroup target=eFightGroup_Freind);
    
    void addEnergySpeedWith(float number,FightGroup target);//

    
    /*
     获取战斗中相应怪物
     */
    cocos2d::Vector<CBaseFighter*> getAllAttacker();//获得攻击双方的所有士兵
   
    
    cocos2d::Vector<CBaseFighter*>* getAllSoldier(){return &m_vecSoldier;};
    cocos2d::Vector<CBaseFighter*>* getAllEnemy(){return &m_vecEnemy;};
    
    cocos2d::Vector<CBaseFighter*> getAllTowers(){return m_vecTowers;};
    
    cocos2d::Vector<CBaseFighter*> getAllMines(){return m_vecMines;}
    cocos2d::Vector<CBaseFighter*> getSoldierById(int monsterId);
    
    
    
    cocos2d::Vector<CBaseFighter*> getFightMonstersByGroup(FightGroup theGroup,int aliveState=1);//获取所有敌对士兵 ,  param2    1：活着的，0：所有，-1死掉得
    cocos2d::Vector<CBaseFighter*> getFriendMonstersByGroup(FightGroup theGroup,int aliveState=1);//获取所有友好士兵， param2   1：活着的，0：所有，-1死掉得
    
    
    cocos2d::Vector<CBaseFighter*> getFightTowerByGroup(FightGroup theGroup);//获取所有敌对建筑
    cocos2d::Vector<CBaseFighter*> getFriendTowerByGroup(FightGroup theGroup);//获取所有友军建筑
    
    cocos2d::Vector<CBaseFighter*> getFightTargetByGroup(FightGroup theGroup,bool ifFightBuilding=true,bool ifFightFly=true);//获取所有敌对的可攻击物,包含上面二者
    
    
    cocos2d::Vector<CBaseFighter*> getAllBaseFight();//获取战场里所有的东西
    
    
    std::vector<obbInfo>* getAllTowerObb(){return &m_vecTowerOBB;};
    void changeTowerObb(bool ifAdd,CBaseFighter* theTower);
    
    
    CBaseFighter* getAttackerByGroupAndCondition(FightGroup theGroup,ChooseCondition theCondition,int theChooseGroup,CBaseFighter* attacker=NULL);//参数1是攻击者阵营，参数2是挑选条件，参数3是希望的阵营，0表示友军，1表示敌军。
    
    cocos2d::Vector<CBaseFighter*> getAttackerByGroupCondition(FightGroup theGroup,int theChooseGroup);//获取相应对应阵营条件的攻击对象。
    
    
    
    
    
    std::vector<FighterInfo>* getTheMonsterInfo(){return &m_vecTheEnemyInfo;};//获得所有当前关卡的怪物信息
    
    CBaseHero* getTheFriendHero(){return m_pTheHero;};
    void setTheFriendHero(CBaseFighter* temp){m_pTheHero = (CBaseHero*)temp;};
    
    CBaseCity* getTheEnemyCity(){return m_ptheEnemyCity;};
    void setTheEnemyCity(CBaseFighter* temp){m_ptheEnemyCity = (CBaseCity*)temp;};
    
    
    levelInfo* getTheLevelInfo(){return m_pTheLevelInfo;};//关卡相关信息
    std::vector<mapInfo*>* getTheMapInfo(){return &m_pTheMapInfo;};//地图相关信息
    std::vector<mapFightInfo>* getTheMapFightInfo(){return &m_vecTheBuilding;};//地图相关建筑信息
    
    
    void addTimeEnergy(float time);//随时间增长的能量
    CSmallMap * m_pSmallMapLayer;
    
    void initLevelInfo();//关卡所需信息初始化
    
    void  initPvpZhaohuanKulou();
    
    void  initZhaohuanKulou(reinforcementInfo* temp);
    
    void initUserInfo();//玩家相关信息初始化
    
    
    
    FighterInfo* getTheBossInfo(){return &m_stTheBossInfo;}; //每波怪里的小boss
    FighterInfo* getTheEnemyCityInfo(){return &m_stTheEnemyCityInfo;};
    FighterInfo* getTheBeProtectedInfo(){return &m_stTheBeProtected;};
    FighterInfo* getTheLevelBossInfo(){return &m_stTheLevelBoss;}; //boss关卡的大boss
    FighterInfo* getTheProtectorInfo(){return &m_stTheProtector;};
    FighterInfo* getTheFriendCityInfo(){return &m_stTheFriendCity;};
    FighterInfo* getTheCallKulouFriend();   //{return &m_stTheFriendKulou;};
    FighterInfo* getTheCallKulouEnemy(); //{return &m_stTheEnemyKulou;};

    
    float addTheFightTime(float time);
    
    /*
     关卡完成条件相关
     */
    void initTheFightConditon();
    void addTheFightConditon(LevelCondition theType, float number,CBaseFighter* target=NULL);
    void refreshTheFightCondition();
    
    
    
    
    void setFightData(int starType, int monsterId = -1);
    
    
    /*
     塔防模式相关
     */
    void calcAllPathWeight(bool ifInit=false);//计算所有路径的权重 , 参数表示是否初始化，如果是的话，则所有数据清空。
    void calcThePos(pathPos* target);
    pathPos* findPos(Vec2 thePoint);//根据路径点坐标，找路径点

    bool addPos(Vec2 thePoint);
    void removePos(Vec2 thePoint);
    Vec2 activeClosedPosByNowPos(Vec2 nowPoint);//暂时不用
    pathPos* findPathPosByPos(Vec2 nowPos); //根据3d坐标，找路径点
    
    
    pathPos* findNextPath(pathPos* target,bool ifIgnoreBuilding=false); //根据当前路径点，找下个可用的路径点 param2 是否无视建筑物（飞行单位）
    Vec3 convertPathPosToPos(pathPos* target); //转换路径点坐标为3d坐标
    
    void updateAllPathPosIdle();//刷新所有路点的路径信息，看是否有怪物踩在上面。
    
    bool ifBlockRoad();//是否把路给堵了
    
    void costStone(int temp){m_iAllStone-=temp;};
    void addStone(int temp){m_iAllStone+=temp;};
    int getStone(){return m_iAllStone;};
    
    void addLife(int temp,CBaseFighter* target);
    void addRound(int temp);
    
    pathPos* getTheHead(){return theHead;};
    
    FighterInfo* getTDFighterInfoByTurn(int turn){return &(m_vecTheTDMonsterInfo[turn]);};
    
    
    std::vector<std::string> getNeededPngPath(bool ifModel=false); //返回本次战斗中所有用刀的怪物png .参数为ture则返回3d模型路径
    
    void getNeededPngPathByFightInfo(FighterInfo* target, std::vector<std::string>* targetVec,bool ifModel=false);
    
    FighterInfo* getPVPFightInfoById(int monsterID);
    
    std::vector<FighterInfo>* getALLPVPFightInfo(){return &m_vecPVPFighterInfo;};
    
    CBaseFighter* getMonsterWithTrueId(int TrueId);//根据真实怪物id，返回指定怪物
    
    //监听事件，进入游戏时重新初始化shader
    void reloadShader();
    
//    inline CBaseFighter* getEndTarget(){return _endTarget;};
    
protected:
    
    std::vector<FighterInfo> m_vecTheEnemyInfo;
    
    std::vector<mapFightInfo> m_vecTheBuilding; //关卡存储的建筑信息
    
//    CC_SYNTHESIZE(FighterInfo, m_stTheBossInfo, TheBossInfo);
//    CC_SYNTHESIZE(FighterInfo, m_stTheEnemyCityInfo, TheEnemyCityInfo);
    
    FighterInfo m_stTheBossInfo;
    FighterInfo m_stTheEnemyCityInfo;//敌人基地信息
    
    FighterInfo m_stTheBeProtected;//被护送的怪物信息
    
    FighterInfo m_stTheLevelBoss;//boss信息
    
    FighterInfo m_stTheProtector;//守护者信息
    
    FighterInfo m_stTheFriendCity;//我方基地信息
    
    FighterInfo  m_stTheFriendKulou;// 我方召唤骷髅
    
    FighterInfo  m_stTheEnemyKulou;//  敌方召唤骷髅
    
    cocos2d::Vector<CBaseFighter*> m_vecSoldier;
    cocos2d::Vector<CBaseFighter*> m_vecEnemy;
    
    cocos2d::Vector<CBaseFighter*> m_vecTowers;
    cocos2d::Vector<CBaseFighter*> m_vecMines;
    
    cocos2d::Vector<CBaseFighter*> m_vecTrapes;
    
    CBaseHero* m_pTheHero;
    CBaseCity* m_ptheEnemyCity;
    
    levelInfo* m_pTheLevelInfo;
    std::vector<mapInfo*> m_pTheMapInfo;
    

    
    CC_SYNTHESIZE(CFightScene*, m_pTheFightScene, TheFightScene);
    
    
    CC_SYNTHESIZE(cocos2d::Label*, m_pLevelConditionLab, LevelConditionLab);
    
    CC_SYNTHESIZE(LevelCondition, m_iTheLevelCondition, TheLevelCondition);
    
    CC_SYNTHESIZE(std::string, m_sTheLevelConditionDes, TheLevelConditionDes);//关卡条件描述
    
    CC_SYNTHESIZE(std::string, m_sTheLevelProgress, TheLevelProgress);//关卡进度描述
    
    CC_SYNTHESIZE(float, m_iLevelProgress, LevelProgress);//关卡进度数值
    
    
    CC_SYNTHESIZE(int, m_iNowLevelId, NowLevelId);//当前关卡id
    CC_SYNTHESIZE(int, m_iNowMidLevelId, NowMidLevelId);//当前大关卡id
    
    
    
    CC_SYNTHESIZE_READONLY(int, m_iWinState, WinState);//赢输 1赢 0没结束 -1输
    void setWinState(int temp,CBaseFighter* target = NULL);
    
    CC_SYNTHESIZE(CBaseFighter*, _endTarget, EndTarget);
    
//    CBaseFighter* _endTarget;//战斗结束时所带影响的怪物。用于做战斗结束后的子弹时间
    
    
    CC_SYNTHESIZE(float, m_fAllFightTime, AllFightTime);//战场总时间
    CC_SYNTHESIZE(int, m_iNowWave, NowWave);//当前战场波数 0的话表示还没出第一波怪
    
    CC_SYNTHESIZE(DrawCardInfo, m_StDrawCardInfo,DrawCardInfo);//抽卡信息
    
    CC_SYNTHESIZE(int, m_iNowMonsterNum, NowMonsterNum);//当前制造的怪物数量
    CC_SYNTHESIZE(int, m_iMaxMonsterNum, MaxMonsterNum);//最大怪物数量
    
    CC_SYNTHESIZE(float, m_iNowEnergy, NowEnergy);
    CC_SYNTHESIZE(float, m_iMaxEnergy, MaxEnergy);
    CC_SYNTHESIZE(float, m_iAddEnergy, addEnergy);

    
    CC_SYNTHESIZE(float, m_iNowEnergy_Enemy, NowEnergy_Enemy);
    CC_SYNTHESIZE(float, m_iMaxEnergy_Enemy, MaxEnergy_Enemy);
    CC_SYNTHESIZE(float, m_iAddEnergy_Enemy, AddEnergy_Enemy);
    CC_SYNTHESIZE(bool, m_bStopAddEnemy, StopAddEnemy);//是否停止能量增加
    CC_SYNTHESIZE(bool, m_bClearSolider, ClearSolider);//是否正在删除士兵
    
    CC_SYNTHESIZE(int, m_iAddCoin, AddCoin);//战斗过程中获得的金币
    
     CC_SYNTHESIZE(bool, m_bActiveToushi, Actvie_Toushiche);//是否激活投石车
    
    CC_SYNTHESIZE(bool, m_bUseSkill, UseSkill);//标识是否在使用技能
    
      
    std::vector<levelStarInfo> m_starInfoVec;
    
    bool m_isIfGetVolume;//判断当前是否正在获取吹气
    
    /*
     塔防模式相关数据
     */
    std::vector<obbInfo> m_vecTowerOBB; //所有防御塔的obb
    std::vector<pathPos*> m_vecPos; //所有行走节点，用于塔防模式寻路
    int m_iAllStone;// 当前剩余魔法石 （建造塔所消耗的能量石）
    pathPos* theHead;//塔防模式怪物的最终点
    int m_iMaxPathX;
    int m_iMaxPathY;
    int m_iBronPathX;//怪物出现的X坐标
    CC_SYNTHESIZE(int, m_iNowLife, NowLife);
    CC_SYNTHESIZE(int, m_iNowRound, NowRound);
    
    std::vector<FighterInfo> m_vecTheTDMonsterInfo;   //塔防模式怪物信息，暂时放在此处，因为暂时不会变化
    
    void setBornPath(float posY);
    
    
    /*
     战斗对话相关
     */
    CC_SYNTHESIZE(float, m_iNowTalkTime, NowTalkTime);
    CC_SYNTHESIZE(fightMode, m_iFightType, FightType);//0:普通战斗 1:塔防模式 2:pvp模式
    
    
//    int m_iNowLife;
//    int m_iNowRound;
    
    CC_SYNTHESIZE(bool, m_bFirstSolider, FirstSolider);//标识是是否第一个死亡的士兵用于第二关的引导判断
    
    
    
    /*
     战斗援军相关
     */
    std::vector<reinforcementInfo*> theReinforcementInfo;//援军信息
    

    /*
     pvp相关
     */
    std::vector<FighterInfo> m_vecPVPFighterInfo;//pvp敌人所有士兵信息
    
    CC_SYNTHESIZE(CBaseFighter*, m_pPVPTheCity, PVPTheCity); //pvp敌军主基地
    
    CC_SYNTHESIZE(Node* ,m_PvpReporterThis,PvpReporterThis);
    
    CC_SYNTHESIZE(pvpEnemyInfo,m_PvpEnemyInfo,ThePvpEnemyInfo);
    
    CC_SYNTHESIZE(int,m_iRobStone,TheRobStone); //偷取的魔法石
    
    CC_SYNTHESIZE(int,m_iKuanShiNum,TheKuanshiNum);// 矿石的数量
    
    CC_SYNTHESIZE(int,m_bPvpStart,IsPvpStart);// pvp战斗退出时的状态 0:未开始 1:已开始 2:开始并退出
    
    
    CC_SYNTHESIZE(int,m_bPvpTime,ThePvpTime); //pvp战斗时间
    
    
    CC_SYNTHESIZE(int ,m_iPvpAllTime,ThePvpAllTime);//pvp总时间
    
    CC_SYNTHESIZE(int,m_iBornTime,TheBornTime); //定时炸弹爆炸时间
    
    
    CC_SYNTHESIZE(CBaseFighter*,m_pBigBoss,TheBigBoss);//每一关的大boss
//    CBaseFighter* m_pPVPTheCity;
    
    /*
     战斗结束时效果相关
     */
    CC_SYNTHESIZE(CBaseFighter*, _lastDiedTarget, lastDiedTarget);
//    CBaseFighter* lastDiedTarget;//上一个死亡的目标
    
    
private:
    static CFightData* m_singleton;
    
    
};














#endif /* defined(__TheGodWill__LYFightData__) */
