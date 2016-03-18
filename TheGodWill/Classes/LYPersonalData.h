//
//  LYPersonalData.h
//  TheGodWill
//
//  Created by user on 14/12/5.
//
//

#ifndef __TheGodWill__LYPersonalData__
#define __TheGodWill__LYPersonalData__

#include <stdio.h>
#include "cocos2d.h"
#include "AllDefine.h"

#include "TDCCAccount.h"
#include "TDCCTalkingDataGA.h"
#include "TDCCMission.h"
#include "TDCCItem.h"
#include "chargeInfoLayer.h"

enum pvpEnemyType
{
    RANDOM_ENEMY = 0,
    SPECIFIED_ENEMY,
    FIGHT_BACK_VIDEO,//播放录像
};

class CPersonalData {
    
public:
    
    static CPersonalData* getInstance();
    void initUserInfo();
    
    static void refreshUserInfo();
    
    CPersonalData();
    ~CPersonalData();
    void SetPhysicalNum(int PhysicalNum);
    int  GetPhysicalNum();
    
    void setPhysicalMaxNum(int maxPhysicalNum);
    int GetPhysicalMaxNum();
    
    void setUserCoin(int num);
    int getUserCoin();
    
    void setUserStone(int num);
    int getUserStone();
    
    void setUserKuangStone(int num);
    
    void setUserMaxKuangStone(int num);
    
    int getUserKuangStone();
    
    int getUserMaxKuangStone();
    

    
    void detectTaskRefresh(long time);
    void detectExchangeNum(long time);
       
    std::vector<FighterInfo*>* getAllTeamSoldier();//获得所有的上阵士兵信息
    std::vector<FighterInfo*>   getAllSoldierInfo();//获得所有士兵信息
    
    FighterInfo*   getBuildingInfoById(int buildingId);//获得相应的建筑信息
    
    void modifyMyMonster(myMonster theMonster);//根据相应数据库信息修改内存中士兵信息
    
    std::vector<FighterInfo> getSoliderInfoByType(int soliderType);//根据士兵类型获取所有怪物
    
    FighterInfo* getTeamSolderByType(int soldierType);//根据类型获得出战士兵
    
//    FighterInfo* getHero(){return m_pTheHero;};//根据类型获得出战士兵
    
    std::vector<BuildingUpgradeInfo*>  getBuildingUpgradeInfo();
    
    std::vector<EnergyUpgradeInfo*>   getEnergyUpgradeInfo();
    
    int getStarByLevel(int smallLevel, bool isEqlite = false);
    
    
    FighterInfo* refreshSoldierInfoWithInfo( myMonster* theInfo);
    
    
    
    static int DataStrToInt(std::string temp);
    
    static std::string DataIntToStr(int temp);
    
    std::vector<FighterInfo*> getTeamInfo(){return m_vecMyTeamInfo;};
    
public:
    void setJumpInfoNull();
    
protected:
    
    CC_SYNTHESIZE(UserInfo*, m_stUserInfo, UserInfo);

    std::vector<FighterInfo*> m_vecMyFighterInfo;//玩家所有士兵信息
    
    std::vector<FighterInfo*> m_vecMyTeamInfo;//玩家所有上阵士兵信息
    
    std::vector<BuildingUpgradeInfo*> m_pVecMyBuildinUpgradeInfo;
    
    std::vector<EnergyUpgradeInfo*>  m_pVecMyEnergyUpgradeInfo;
    
    FighterInfo* m_pTheHero;//玩家上阵英雄
    
    //std::vector<heroSkillInfo>m_vecTheHeroSkill; //玩家英雄技能
    
    std::vector<FighterInfo> m_vecTheBuildingInfo; //塔防建筑信息
    
    
    
    CC_SYNTHESIZE(TDCCAccount*, m_pTDAccount, TDAccount);
    
    chargeInfoLayer* m_pChargeInfo;
    UserJumpInfo m_jumpInfo;
    
    
private:
    
    static CPersonalData* m_singleton;
    
    CC_SYNTHESIZE(int, m_iStar, star);//当前关卡的星级是否增加了
    CC_SYNTHESIZE(bool, m_bOpenNewLevel, isOpenNewLevel);//是否开启新的大关卡
    CC_SYNTHESIZE(bool, m_bOpenNewSmallLevel, isOpenNewSmallLevel);//是否是新的小关卡
    CC_SYNTHESIZE(bool, m_bWinManual, isWinManual);//是否是胜利手动进入关卡详情
    CC_SYNTHESIZE(bool, m_bFailManual, isFailManual);//是否是失败或者暂停手动进入关卡详情
    CC_SYNTHESIZE(bool, m_bFinishedLevel, FinishedLevel);//标识是否是已通关的关卡
    CC_SYNTHESIZE(bool, m_bNewMidLevel, NewMidLevel);//记录中关卡
    CC_SYNTHESIZE(int, m_mode, levelMode);//标识当前模式，0:普通模式 1:精英模式
    CC_SYNTHESIZE(bool, m_bifUpdateUserInfo, UpdateUserInfo);//是否需要更新用户信息
     CC_SYNTHESIZE(bool, m_bUpdateDrawCardInfo, UpdateDrawCardInfo);//是否需要抽卡信息
    CC_SYNTHESIZE(bool,m_bUpdateKuanshiInfo,UpdateKuanshiInfo);// 更新用户矿石信息
    
    CC_SYNTHESIZE(bool, m_bStaminaFull, IfStaminaFull);//编队时选中的士兵类型
    
    CC_SYNTHESIZE(int, m_iSoliderType, SoliderType);//编队时选中的士兵类型
    
    CC_SYNTHESIZE(std::vector<heroSkillInfo>,m_vecTheHeroSkill,HeroSkillVec);
    
//    CC_SYNTHESIZE(bool, m_bBgMusic, BgMusic);//是否开启背景音乐
//    CC_SYNTHESIZE(bool, m_bEffect, Effect);//是否开启音效
    CC_SYNTHESIZE(bool, m_bJudgeUpgrade, JudgeUpgrade);//结算时是否判断了有英雄解锁
    CC_SYNTHESIZE(bool, m_bInsertHero, InsertHero); //是否插入新英雄
    CC_SYNTHESIZE(bool, m_bCatapult, Catapult);//投石车引导删除的标志
    CC_SYNTHESIZE(bool, m_bShotted, Shotted);//投石车是否击中目标的标志
    CC_SYNTHESIZE(bool, m_brestartPlayMusic, restartPlayMusic);//是否重新开始播放背景音乐
    
    CC_SYNTHESIZE(bool, m_bCutGuide, CutGuide);//是否完成了切死第一个士兵的引导
    CC_SYNTHESIZE(bool, m_bShowMap, ShowMap);//表示当前关卡是否是通过再来一次开始的
    CC_SYNTHESIZE(bool, m_bChargeSucess, ChargeSucess);//标识是否充值成功
    
    CC_SYNTHESIZE(bool,m_bJumpFromSkill,JumpFromSkill);//标记是否从技能界面跳转的
    CC_SYNTHESIZE(bool, m_bPvpMainLayer, PvpMainLayer);//是否pvpMianLayer界面
    CC_SYNTHESIZE(int, m_iVideoId, VideoId);//当前的录像id
    //消息数据相关
    CC_SYNTHESIZE(int, m_iState, State);//网络强求返回的状态值
    CC_SYNTHESIZE(pvpEnemyType, m_enemyType, EnemyType);//网络强求返回的状态值
    CC_SYNTHESIZE(bool, m_bFromVideoList, FromVideoList);//标识是否是从录像列表进入的游戏（回放和复仇）
    
    CC_SYNTHESIZE(bool, m_bTouchRmvFlag, TouchRmvFlag);//用于标识当前的触摸是否是关闭子界面以后传递到主界面的，规避掉多点触摸的bug
    CC_SYNTHESIZE(int, m_iEffectId, EffectId);//当前音效id
};




#endif /* defined(__TheGodWill__LYPersonalData__) */
