//
//  LYFightScenePVP.h
//  WarOfGod
//
//  Created by user on 15/5/29.
//
//

#ifndef __WarOfGod__LYFightScenePVP__
#define __WarOfGod__LYFightScenePVP__

#include <stdio.h>
#include <iostream>

#include "fightScene.h"

#include "cocos2d.h"
#include "pvpCommonData.h"

USING_NS_CC;




typedef enum {
    
    ERecordTpe_AddMonster,
    ERecordTpe_Catapult,
    ERecordTpe_ChangeHP,
    ERecordTpe_ChangeTarget,
    
}RecordType;


typedef struct{
    
    float time;
    
    std::string theInfo;
    
}PlayInfo;


typedef struct{
    
    int buildingId;
    
    float posx;
    float posy;
    
    int state;//0表示普通，1表示升级中
    
    FighterInfo theBuildInfo;
    
    buildInfo*  pBbuildInfo;
    OBB obb;
    
}PVPBuildingInfo;



class CFightScenePVP : public CFightScene {
    
public:
    
    CFightScenePVP();
    ~CFightScenePVP();
    
    static CFightScenePVP* create(PVPMode temp,int robStone,std::string MapInfo,std::string EnemyInfo="",std::string recInfo="");
    
//    CREATE_FUNC(CFightScenePVP);
    bool init();
    bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
    void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event);
    void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event);
    
    void setPVPMode(PVPMode temp,int robStone,std::string mapInfo,std::string PVPEnemyInfo="",std::string recInfo="");
    
    
    void initPVPPlayMode(std::string MapInfo);
    
    void initPVPRecMode(std::string MapInfo,std::string EnemyInfo,std::string recInfo);  //回放模式下，不光要有敌人士兵信息，录像信息，还需要自己的map信息（因为可能看录像的时候，自己的map信息已经变了，所以需要保存一份此信息）
    
    void initCampInfo(std::string campInfo);
    
    
    virtual CBaseFighter* addMonsterWithIdAndGroupAndPosition(FighterInfo *theInfo, FightGroup theGroup,Vec2 position,AttackType ,bool ifMidCamp,bool ifBoss,bool ifElite,bool bZhaohuan=false,bool bUpdating=false); //ifmidcamp 是否中立兵营出怪
    
    virtual void update(float time);
    
    virtual void levelInit();//初始化关卡信息
    
    
    void videoUpdate(float time);
    
    
    
    void addRecordStringWithAddMonster(int monsterId, Vec2 Pos,bool ifElite);  //增加招怪记录
    void addRecordStringWithCatapult(Vec3 ballPos, Vec2 ballSpeed,int damage);
    void addRecordStringWithChangeHP(int TrueId,int damage,int damageType,bool ifDied);//增加怪物伤血记录
    void addRecordStringWithchangeTarget(int attackTrueId,int beattackedTrueId);//增加怪物目标记录
    
    void addSoldierNum(int monsterId, int level, int num=1);
    
    void analysePlayInfo(std::string theInfo);
    
    void uploadInfo();  //上传本次战斗相关信息
    
    
    void uploadInfoBack(Ref* pSender);
    void updateTowerObbInfo();//更新建筑obb信息
    CBaseBuilding* getTouchTower(Vec2 touchPos);
    void addTowerNameAndLevel();//添加建筑信息
    void rmvNameAndLevel();
    void resetOriginPos();//将怪物重新放置到最初的位置
protected:
    
    CC_SYNTHESIZE(int, m_iRobStones, RobStones);
    
    CC_SYNTHESIZE(int, m_iNowTrueId, NowTrueId);//怪物唯一标识序号，用于pvp下的怪物出生逻辑
    
    
private:
    

    
    void OnClose(Ref *pSender);
    
    std::vector<PlayInfo> m_vecPlayInfo;
    EventListenerTouchOneByOne* m_listen;
    CBaseBuilding* m_curTower;
    CBaseBuilding* m_preTower;
    int m_curBuildingId;
    Label* m_towerName;//建筑名字
    Label* m_towerLevel;//建筑等级
    Label *m_pLableTime;
    CPartolFight* m_patrolman;//巡逻者
    FighterInfo m_thePatrolInfo;
    Vec3 m_originPos;

    Sprite3D* m_TowerBg;
    int m_iAllTime;
    
    int m_iHour;
    int m_iMin;
    int m_iSec;
    
    std::vector<IdAndTwoNum> m_vecSolderNumber;
    
    bool m_bTimerStart;
    
    std::vector<PVPBuildingInfo> m_vecBuildingInfo;
    std::string m_sPVPRecord;
    std::string m_sEnemyInfo;
    std::string m_sCampInfo;
    
    bool m_bIfVideoReady;//是否计时器正在等待下一次记录调用
    
    
};












#endif /* defined(__WarOfGod__LYFightScenePVP__) */
