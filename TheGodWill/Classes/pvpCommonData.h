//
//  pvpDataManager.h
//  WarOfGod
//
//  Created by user on 15/8/10.
//
//

#ifndef __WarOfGod__pvpDataManager__
#define __WarOfGod__pvpDataManager__

#include <stdio.h>
#include "AllDefine.h"
#include "cocos2d.h"
#include "LYBaseBuilding.h"

#define MAP_X_INDEX 24
#define MAP_Y_INDEX 10

USING_NS_CC;

struct buildInfo
{
    CBaseBuilding*  theTower;
    pvpBuildInfo* pInfo;
    Sprite3D* towerBase;//建筑底
    Sprite3D* shadow;//建筑影子
};

struct towerOBBInfo{
    OBB obb;
    buildInfo* towerInfo;
    int userBuildingId;//建筑唯一标识
    int xPos;//x坐标
    int zPos;//z坐标
    int buildingId;
    int state;//状态 0:普通状态， 1:正在升级(服务器用) 2:正在升级 3:正在熔炼
    int startUpTime;//开始升级时间
    int needTime;//升级完成所需时间
    int validTime;//标识是否显示矿石图标(即是否可领取矿石)-1:可领取 0:不可领取
    FighterInfo theInfo;
};

class pvpCommonData
{
public:
    pvpCommonData();
    ~pvpCommonData();
    static pvpCommonData* getInstance();
    
    void parseUserTeamInfo(std::vector<std::string> paramsStr);
    void parseIsEndProductBack(std::vector<std::string> paramsStr);
    void clearAllBuildInfo();
    //获取玩家阵营信息接口
//    static void getUserTeamInfo();
//    static void getUserTeamInfoBack(Ref* pSender);
    CC_SYNTHESIZE(int, m_serverTime, setverTime);
    std::vector<towerOBBInfo> m_allBuildObb;//地图上所有的建筑
    static pvpCommonData* m_singleton;
    
    void setAllBack(locationInfo allBlock[][MAP_Y_INDEX]);
    locationInfo m_allBlock[MAP_X_INDEX][MAP_Y_INDEX];
private:
    
    CC_SYNTHESIZE(buildInfo*, m_towerInfo, TowerInfo);//祭坛建筑
    CC_SYNTHESIZE(int, m_bEvolution, Evolution);//祭坛进化标识 0:未进化 1:已进化
    CC_SYNTHESIZE(bool, m_bUpBack, UpBack);//pvp中巡逻者坐标取出方式，顺时针行走 true，反之false
    CC_SYNTHESIZE(bool, m_bFollowBuild, FollowBuild);//标识是不是紧随在pvp建造引导之后直接出现的
    //CC_SYNTHESIZE(cocos2d::Sprite3D*, m_pCircleSprite, CircleSprite);
    CC_SYNTHESIZE(int, m_iProtectedTime, ProtectedTime);//玩家受保护的时间
    
    CC_SYNTHESIZE(int, m_iAllBuildNum, AllBuildNum);//当前战斗中敌方所有的建筑数量
    CC_SYNTHESIZE(int, m_iDestroyedBuildNum, DestroyedBuildNum)//当前战斗中敌方已被摧毁的建筑
    
    //关于熔炼
    CC_SYNTHESIZE(int, m_bRemainTime, RemainTime);//熔炼剩余时间
    CC_SYNTHESIZE(int, m_bProductTime, ProductTime);//熔炼所需时间
    CC_SYNTHESIZE(int, m_bProductGetStone, ProductGetStone);//熔炼成功后获得的魔法石
};

#endif /* defined(__WarOfGod__pvpDataManager__) */
