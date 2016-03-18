//
//  fightScene.h
//  TheGodWill
//
//  Created by 刘勇 on 14-11-25.
//
//

#ifndef __TheGodWill__fightScene__
#define __TheGodWill__fightScene__

#include <iostream>
#include "cocos2d.h"
//#include "AllHead.h"
#include "AllDefine.h"
#include "LYBaseFighter.h"
#include "PaoWuxian.h"

#include "LYPersonalData.h"

#include "LYFightManager.h"
#include "GeometricRecognizer.h"

#include "LYBuildingControllLayer.h"
#include "CNpcFighter.h"

#include "cocos-ext.h"

//#include "cocos2dx"

#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#endif

#include "PvpMainLayer.h"

using namespace DollarRecognizer;
USING_NS_CC;

#define CR_SHAKE_DISTANCE 5     //士兵每次抖动时移动的距离

#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    #define CR_BASE_PROBILITY 1.0   //士兵被吹走时概率计算的最小值
#else
    #define CR_BASE_PROBILITY 0.3   //士兵被吹走时概率计算的最小值
#endif

#define CR_SCREENMID_RANGE 600        //吹风技能的有效范围。以当前屏幕为中心向两边扩展
#define CR_MAP_RANCGE 150   //切兵技能引导小地图滑动距离的计算值
#define CR_MAP_BLOW_RANCGE 250   //吹风技能引导小地图滑动距离的计算值
#define CR_SHAKEING_FLY  -1200     //抖动未完时z轴的最小值
#define CR_MOVE_MAXDISTANCE  -999     //被吹走的最远距离
#define CR_FLY_EACHFRAME 45     //飞走时每帧移动的距离
#define CR_SHAKE_EACHFRAME 5     //抖动移动时时每帧移动的距离


#define BUILDING_POS_OFFSET_X   100  //塔防模式下，建筑的x间隔
#define BUILDING_POS_OFFSET_Y   100 //塔防模式下，建筑的y间隔

#define POINT_NUM 16
#define MIDDLE_POINT_R 255
#define MIDDLE_POINT_G 255
#define MIDDLE_POINT_B 255
#define MIDDLE_POINT_A 255
#define TOP_POINT_R	100
#define TOP_POINT_G	100
#define TOP_POINT_B	255
#define TOP_POINT_A	255
#define BOTTOM_POINT_R 100
#define BOTTOM_POINT_G 100
#define BOTTOM_POINT_B 255
#define BOTTOM_POINT_A 255


#define TD_WAVE_TIME 20
#define TD_BOSSWAVE_TIME 90

#define LEVEL_START_PROTECT 23  //获得美女守护者的关卡
#define LEVEL_START_MAN  9    //获得男主角的关卡
#define MAX_CAMERA_Y    514
#define MIN_CAMERA_Y    162

#define MAX_CAMERA_Z    114
#define MIN_CAMERA_Z    -238

#define WALKER_TARGET_X -400


enum{
    SKILL_BLOW_TAG = 10,
    SKILL_CUT_FRUIT_TAG,
    SKILL_START_TAG,
    GAME_PAUSE_TAG,
    LEVEL_STAR_TAG,
    FIGHT_TOUSHICHE_TAG,
    FIGHT_LAGAN_TAG,
    FIGHT_STAR_ONE,
    FIGHT_STAR_TWO,
    FIGHT_STAR_THREE,
    FIGHT_DEL_SOLIDER,
    FIGHT_RMV_BTN,
    FIGHT_ENERGY_LINETAG,//能量线
    FIGHT_ENERGY_QIUTAG, //能量球
    FIGHT_ENERGY_CAOTAG,//能量槽
    FIGHT_ENERGY_BGTAG,//能量背景
    FIGHT_SOLDIER_NUMBGTAG,
    FIGHT_SOLDIER_NUMTAG,
    FIGHT_DAOJISHTAG,
    FIGHT_GUANBITAG,       //关闭
    FIGHT_SHENGSUOTIAOTAG, //伸缩条
    FIGHT_SHENGSUOMENUTAG,//伸缩menu
    FIGHT_SMALLMAP, //pvp小地图
    FIGHT_LAYER3DTAG,
    FIGHT_BIGMAPTAG,//大地图
};

enum{
    GUIDER_DRAW_LINE = 400,
    GUIDER_DRAW_TRIANGLE,
    GUIDER_DRAW_RECTANGLE,
    GUIDER_DRAW_CIRCLE,
    GUIDER_DRAW_V,
    GUIDER_CHECK,
    GUIDER_WORD_TIP,
    GUIDER_LIGHT,
};

enum{
    ePVP_STAMINA_CHARGE = 1,
    ePVP_COIN_CHARGE,
    ePVP_STONE_CHARGE,
    ePVP_BACK_TAG,
    ePVP_RESEARCH_TAG,
    ePVP_BUILD_TAG,
    ePVP_MESSAGE_TAG,
    ePVP_FIGHT_TAG,
    ePVP_BULIDING_CANCEL,
    ePVP_BULIDING_OK,
    ePVP_BULIDING_INFO,
    ePVP_BULIDING_UPGRADE,
    ePVP_BULIDING_DEL,
    ePVP_BULIDING_FINISH,
};

enum
{
    ePvp_Cloud1Tag = 100,
    ePvp_Cloud2Tag,
    ePvp_Cloud3Tag,
    ePvp_Cloud4Tag,
    ePvp_Cloud5Tag,
    ePvp_Cloud6Tag
};

#define TOWER_LEVEL_INDEX 1001 //塔防模式关卡的起始索引

struct enemyInfo{
    OBB obb;
    CBaseFighter* enemy;
};

struct obbInfo{
    OBB obb;
    CBaseFighter*  theFighter;
};


enum eFightTouchPri
{
    eLaganTouch = -200,
    eChubingTouch,
    
};

struct starShow
{
    int tag;
    cocos2d::Sprite* star;
};






typedef enum{
    
    eFightSceneType_Normal,
    eFightSceneType_PVP,
    
}FightSceneType;

typedef enum{
    
    EPVPMode_NOPVP,
    EPVPMode_REC,   //录像模式
    EPVPMode_PLAY,  //正常战斗
    
}PVPMode;


typedef enum{
    
    eFightBG_Forest2,
    
    eFightBG_Forest1,
    
    eFightBG_Snow1_night,
    
    eFightBG_Wall1,
    
    eFightBG_Snow1,
    

    eFightBG_error,
    
    eFightBG_Desert,
    
}fightBG;



class CFightScene : public CBaseLayer

{
    
public:
    CFightScene();
    ~CFightScene();
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    virtual void update(float time);
    
    virtual void updateBGpng(float time);//更新背景相关的特效图片
    
    void updateCamera(float time);
    
    void EndCameraUpdate(float time);
  
    virtual void onEnterTransitionDidFinish();
    
    virtual void onExit();
    
    virtual void initData();
    
    void initUI();
    
    void initBG();
    
    
    void RunEnergyAction(Node *p,int iTox=-1000,int iToY=1000);
    
    void  OnToushicheShow();
    
    void CameraRunWithSmallMap(float x);
    
    void initTDUI();
    void initNormalUI();    
    void initLight();
    
    void initAddSolider();//初始化画图形造兵模板
    void fighterInit();
    
    void animateInit();//部分常用动画初始化
    
    virtual void levelInit();//初始化关卡信息
    
    void fightStartAnimateInit();//初始化战斗开始动画
    bool InitReadyFightPvpUI();
    
    
    void addTaskBgAnimation();//添加任务框背景动画
    
    CREATE_FUNC(CFightScene);
    virtual void uploadInfo();//上传玩家录像
    
    bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
    void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event);
    void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event);
    void onTouchCancelled(Touch *touch, Event *event);
    
    void MoveCameraWithSmallMapPos(cocos2d::Point targetPos);
    
    void addSoldierByType(FighterType theFightType, float yPos , bool ifPerfect=false);//根据类型增加士兵
    void addLevelMonster(int thenumbers=1,cocos2d::Point thePos=cocos2d::Point::ZERO,EnemyType thetype= eEnemyType_normal );//关卡出怪   参数1：位置， 参数2 数量
    
    virtual CBaseFighter* addMonsterWithIdAndGroupAndPosition(FighterInfo *theInfo, FightGroup theGroup,Vec2 position,AttackType theType=eAttackType_monster,bool ifMidCamp = false,bool ifBoss=false,bool ifElite=false,bool bZhaohuan=false,bool bUpdating=false); //ifmidcamp 是否中立兵营出怪
    
    void onSkillActive(Ref *pSender);
    
    cocos2d::Sprite * m_pCurView;
    void updateBlowingSkill(float dt);//计时吹风技能的持续时间
    void startBlowCallBack(Ref* pSender);//开启吹风技能
    void traverseSolider();//遍历所有士兵根据概率判断是否被吹走
    void updateEnemyPos(float dt);
    void updateCameraAngle(float dt);//施放吹风技能前调整视角
    
    void moveEnemy(float dt);
    void resetEnemy(float dt);//初始化敌人信息
    void setBtnEnable(bool ifValid = false);
    void switchBtnValid(float dt);//开关按钮
    void enemyMonsterUpdate(float time);//敌人怪物管理
    
    void updateCutSkill(float dt);
    void setColorLight(Color3B color);
    virtual void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags) ;
    void onDraw(const cocos2d::Mat4 &transform, bool transformUpdated);
    void drawKnifeTrace();//画刀痕
    void erasureNail() ;//删除线
    void triangle(cocos2d::Point* vertex, cocos2d::Point p1, cocos2d::Point p2, GLfloat w);
    
    void showAllRmvSolider();
    void rmvAllRmvSolider(Ref* pSender);
    void resetRmvSolider();
    
    void ResetGlobalZ();
    
    void addNewOBBWithCoords(Vec3 p,CBaseFighter* sprite);//添加敌方士兵包围盒
    void removeOBBWithFighter(CBaseFighter* target);//删除士兵所对应的包围盒
    
    void calculateRayByLocationInView(Ray* ray,cocos2d::Camera* theCamera, const Vec2& location);
    void unproject(const Mat4& viewProjection, const cocos2d::Size* viewport, Vec3* src, Vec3* dst);
    void addCutEffect(cocos2d::Vec3 pos, CBaseFighter* sprite);
    
    void updateCombo();//更新连击数
    void resetCombo();//初始化连击数
    
    void stopAllBattle();//停止所有的战场元素
    
    void addStarEffect();//添加星星音效
    void addSoliderEffect();
    void updateLevelStar(bool isEnd = false);//更新关卡星级
    void adjustMoveMapPos(cocos2d::Touch *touch, cocos2d::Event *event);//引导检测小地图移动的位置
    
    void onBtnCreateBuilding(Ref *pSender);
    void showCutGuide();//显示切割技能的最后一个引导
    void startCutSolider(Ref* pSender);//直到玩家使用切兵技能杀死一个怪物才调用
    void startBlowSolider(Ref* pSender);//直到玩家吹飞一个怪物才调用
    void setDrawLine();//设置可以正常画线
    void showFightResult(Ref* pSender);//显示胜利结算界面
    void stopAllAttack();//停止所有攻击
    
    void addCloud();
    
    
    void DefendSucessRecv(std::string str);
    
    void DefendFailRecv();    
    std::vector<Menu*> m_allMarkSolider;//保存所有可以删除的士兵
    int m_addEnergy;//删除士兵所加的能量
    LYMenuItemSprite* m_rmvMenuItem;
    bool m_bRmvFlag;//标识是否准备删除士兵
    
    std::vector<heroSkillInfo>m_HeroSkill;//英雄技能
    int blowCD;//吹风技能cd
    float m_blowTime;
    float m_cutTime;
    Label* m_blowLabel;
    int cutCD;//切兵技能CD
    Label* m_cutLabel;
    Menu* m_pBlowMenu;
    Menu* m_pCutMenu;
    bool m_bCutGuide;//标识是否切兵技能引导
    
    int monsterNum;
    Sprite *m_ptaskBg;
    Label *m_pTaskLable;
    Menu* m_pauseMenu;
    /**************画图造兵************/
    void removeResult(bool ifPerfect=false);
    void removeResultPerfect();
    void removeResultNormal();
    
    void clearPoint();//清除所有触摸点
    void drawLine();//画线
    void saveTouchPoint(cocos2d::Touch *touch);
    void identifyTouch(cocos2d::Touch *touch);
    void addParticleEffect(Ref* pObject);//添加造兵时的粒子特效
    
    void updateIdentify();
    
    void detectSdtory();//判断是否有剧情
    void initGameDate(Ref *pSender);//游戏的动态初始化
    void backPvpMainLayer(Ref* pSender);//回到pvp主界面
    std::vector<starShow> m_starVec;//存储星级
    
    
    /*
     塔防模式相关
     */
    bool addbuildingVirtual(cocos2d::Vec2 buildPos);
    CBaseFighter* addbuildingReal(cocos2d::Vec2 buildPos);
    
    CBaseFighter* buildingSelect(cocos2d::Vec2 touchPos);
 
    //士兵选择
    CBaseFighter * SoliderSelect(cocos2d::Vec2 touchPos);
    
    int buildingPosSelect(Vec2* targetPos); //建造建筑的位置选择函数，  返回0表示成功，返回1表示已经有建筑物，返回2表示会把路堵死
    
    void  OnCallBackTDHelp(Ref *pSender);
    void  OnCallBackClockOver(Ref *pSender);
    
    void updateTD(float time);
    void gotTheSton(int stoneNum,cocos2d::Vec3 thePos);
    
    void TDMonsterStart();//塔防模式下开始出怪
    
    void addTDmonster(float time);
    
    void resetTDTime();//重置塔防模式下的时间
    
    
    void StartToushiCD();
    void finishTowerLevelGuide();//塔防模式完成后回复游戏状态
    void EndToushijiCD();
    
    void addBornAnimate(CBaseFighter* target);
    
    void soliderCallBack(Ref * sender, cocos2d::extension::Control::EventType controlEvent);//缩放滑动条回调
    
    void callBackZoomIN(Ref* Sender);
    void callBackZoomOut(Ref* Sender);
    
    //移动镜头的方法，包含场景特效移动的内容
    void moveCameraPos(float x=-999,float y=-999,float z=-999);
    void setOriginCameraPos(Vec3 originPos);
    
    Vec3 getCameraPos(){if(_camera){return _camera->getPosition3D();} return Vec3::ZERO;};
    
    
    void addRecEndTime(int fTime);
    
    void addRecIsWin(int iWin);
    
    void ifGameOver(float time);//输赢判断
    
    
    void addResult();
    void addGuideInfo();//添加pvp引导
    virtual void rmvNameAndLevel();
    virtual void resetOriginPos();
    
    CC_SYNTHESIZE(Sprite*, m_pToushiActive, TheToushiActive);
    
//    Sprite * m_pToushiActive;
private:
    void closeClound();
    
    void OnbtnCancle(Ref *pSender);
    void OnRebo(Ref *pSender);
    
    void OnbtnOk(Ref *pSender);
    
    void btnCallBack(Ref *pSender);
    
    void getEnemyTeamInfo();
    
    void startSearchEnemy();
    
    void EnemyTo(Ref *pSender);
    
    void getVideoInfo(Ref* pSender);
    
    void getEnemyTeamInfoBack(Ref* pSender);
    void searchCallBack(Ref* pSender);
    
    CC_SYNTHESIZE(cocos2d::Camera *, _camera, TheCamera);
    
    Vec3 m_vec3OriginCameraPos;
    
//    cocos2d::Camera *_camera;
    cocos2d::CustomCommand _customCommand;
    GeometricRecognizer* g_rGemertricRecognizer;
    std::vector<cocos2d::Point> m_pointArr;
    Path2D p_2dPath;//坐标集合，记录触摸操作，Layer的ccTouchesMoved方法中记录坐标
    std::string m_GraphicsName;//是别的图形类型
    float m_similarity;//相似度
    cocos2d::Point m_startPos;
    bool m_bUpdateTime;//是否更新士兵的出兵时间
    
    cocos2d::SEL_MenuHandler m_pHandler;
    bool m_isDraw;//是否可以开始画线
    int m_opcity;//线的透明度
    bool m_isValid;//标识当前画线是否有效
    bool m_bIsshowResult;//是否正在显示识别结果
    ParticleSystemQuad* m_touchStar;
    float yHeight;//造兵的高度
    bool m_isShowBtn;//是否显示技能按钮
    
    
    int m_soliderNum;//记录已经添加的士兵数量
    
    LYMenuItemSprite* m_pSwitchItem1;//投石车
    LYMenuItemSprite* m_pRmvItem;//删除按钮
    LYMenuItemSprite* m_pRmvBackItem;//撤销删除按钮
    LYMenuItemSprite* m_pSwitchItem2;//拉杆
    
    bool m_bEndGuide;//是否完成了小地图引导
    
    int m_iToushiCDTime;
    //灯光效果
    cocos2d::DirectionLight* m_pAmbientLight;
    int m_storyStartTime;//剧情的开始时间
    bool m_bStartStory;//标识是否在游戏开始有剧情
    bool m_bHaveStory;//标识游戏过程中是否有剧情
    bool m_bBlowSolider;//标志是否已经有士兵被吹走
    
    //划线效果
    cocos2d::Vector<cocos2d::Sprite*> m_vecDrawSprite;
    bool bArrow;//是否显示箭头
    
    
    //场景的图片效果
    std::string m_strBGEffect;
    Sprite* m_searchBg;
    
    Sprite3D *m_pXian;
    
    
    
protected:
    
    void OnToushicheNormal(Ref *pSender);
    
    //激活拉杆
    void  StartLaganActive();

    
    cocos2d::Action* iDle;
    cocos2d::Action* attack1;
    cocos2d::Action* hurt;
    cocos2d::Action* walk;
    
    CBaseFighter* m_pTarget;//视角跟随的士兵
    
    CC_SYNTHESIZE_READONLY(cocos2d::Layer*, layer3D, theLayer3D);
    //    cocos2d::Layer* layer3D;
    int m_skillTime;//技能持续时间
    float m_time;
    Node* m_skill;
    int m_adjustAngle;//调整视角
    int m_reverseAdjust;
    int m_soundId;//吹风音效ID
    std::vector<int> m_soundVec;
    
    CC_SYNTHESIZE(FightSceneType, m_eFightSceneType, theFightSceneType);
    
    CC_SYNTHESIZE(PVPMode, m_ePVPMode, thePVPMode);
    
    CC_SYNTHESIZE(ProgressTimer*,m_pBossHpProcess,theBossProgress);
    
    
    CC_SYNTHESIZE(ProgressTimer *,m_pWhiteBossBg,theWhiteBoss);
    
    CC_SYNTHESIZE(Layer* ,m_layer,TheLayer);
    
public:
    float m_volume;
    bool m_isFlying;//是否正在被吹走
    
    bool m_bValid;//标识吹风技能获取的麦克风声音是否达到过要求
    
    cocos2d::Point        m_pointPath[POINT_NUM] ;
    ParticleSystem*       m_pParticle ;
    unsigned int		  m_Index ;
    bool                  m_bTouched ;
    cocos2d::Point        m_touchBeganPoint ;
    cocos2d::Point        m_curPoint;
    cocos2d::Point        m_touchEndPoint ;
    long                  m_timer ;
    bool m_isblow;//是否开启了吹风技能
    bool m_bifBlowEnd;//是否吹风技能结束
    bool m_isTouch;//标识是否正在使用切兵技能
    bool m_bifNorAddSolider;//是否是正常造兵
    bool m_bToushicheStart;//投石车开始
    bool m_bTouchStar;
    bool m_bTouchMap;
    
    std::vector<enemyInfo> m_obb;
    OBB                   _obbt;
    std::set<int>         _intersetList;
    std::vector<CBaseFighter*> m_hurtEnemyVec;
    Label* m_combo;
    Sprite* m_chop;//连斩
    Sprite* m_blood;//血
    int m_comboNum;//连击数
    int m_showCombo;//当前显示的连击数
    float m_directX;//x轴滑动方向
    float m_directY;//y轴滑动方向
    bool m_isFirst;//标识是否是第一次滑动
    
    LYMenuItemSprite *m_pTouMiaozhunItem;
    LYMenuItemSprite* m_pTouFasheItem;
    
    Sprite* m_guiderArrow;//引导箭头

  
    Node* m_GuideNode;

    float m_fshowComboInteval;
    bool m_bToushiActive;
    
 
    //场景粒子效果
    std::vector<cocos2d::Layer*> m_VecSceneParticle; //此为场景粒子数组，按所给 z值来受镜头影响，被镜头影响的效果递增20%
    //场景非粒子效果
    std::vector<cocos2d::Layer*> m_vecScenePng;
    bool  m_bMoveToushiche; //投石车z轴移动

//    ParticleSystemQuad* m_pSceneParticle;
    
private:
    
    
    cocos2d::Size m_WinSize;
    //Pvp
    LYMenuItemSprite *m_pFightItem;
    LYMenuItemSprite *m_searchItem;
    LYMenuItemSprite * m_pBackItem;
    
    Menu * m_pBackMenu;
    Menu *m_pvpCloseMenu;
    
    
    Sprite * m_pAllStone;
    Sprite*  m_pEnemyBg;
    
    Menu* m_btnMenu;
    int m_userId;//玩家id
    std::string m_name;//玩家名字
    int m_level;//玩家等级
    int m_stone;//服务器返回的增长后的用户魔法石总量
    std::string m_enemyMaps;
    ProgressTimer* m_progress;
    
    float m_distance;//多点触摸时两个点之间的起始距离
    
    Sprite * m_pLaGan;  //拉杆
    Sprite * m_pLanGanDian;

    float attackerY; //召唤士兵的默认高度（刚好踩在地面上的高度）
    
    Menu *  m_pMenuTouShiMove;

    bool m_bSmallMapMoveStart;//小地图移动
    bool m_bPvpAllMapMove;
    
    
    LYMenuItemSprite *m_pToushicheItem;
    LYMenuItemSprite *m_pCurViewItem;
    BillBoard * m_pMoveRange;
    cocos2d::ProgressTimer* m_pTheEnergyBar;
    bool m_bMovedView;
 
    bool m_bToushiMove;
    bool m_bSmallMapAutoMove;//标识小地图是否自动移动
    Vec3  m_PreCameraVec3;
    Vec3  m_EndCameraVec3;
    
    cocos2d::Label * m_pSoldierNum;
    
    float m_dtTime;
    //int m_costNum;//记录每一关游戏时间
    bool m_bGuideBlow;//标识是否正在引导吹风技能，避免多次进入吹风引导
    
    int  m_iRecEndTime;
    bool  m_bWin;
    
    /*
     缩放相关
     */
//    Sprite * m_pZoomBG;
//    Sprite * m_pZoomBlue;
//    LYMenuItemSprite * m_pZoomButton;
    cocos2d::extension::ControlSlider* m_pZommControl;
    
    LYMenuItemSprite * m_pZoomInButton;
    LYMenuItemSprite * m_pZoomOutButton;
    
protected:
    
    std::vector<bombTime>* theBombTime;
    
    CPaoWuxian * m_paoWuLayer;
    
public:
    void updateAnimateGuide(float dt);
    void startNextGuide();
    void remvGuideDraw();
    
    void startMonitorBlow(Ref *pSender);
    void startMonitorCut(Ref* pSender);
    void beforeBlow();
    void beforeCut();
    void removeGuide(Ref *pSender);
    void damageGuide(Ref *pSender);
    void addTouishicheGuide();//添加投石车引导
    /*
     塔防模式相关
     */
    bool m_bIfBuildingTower;//标志位，是否激活建造功能
    long m_iNextBuildingId;//将要建造的建筑id
    Sprite3D* m_pPreparedBuilding;//将要建造的建筑虚像
    Vec2 m_vec2BuildPos;//将要建造建筑的位置
    CBuildingControllLayer* m_pBuildingControll;
    cocos2d::ProgressTimer* m_pTheClock;
    
    LYMenuItemSprite* m_pClockButton;
    
    float m_fNowWaveTime;//每波时间
    
    int m_iNowMonsterNum;//当前波出了第几个怪了
    
    cocos2d::Label* m_pStoneLable;
    cocos2d::Label* m_pLifeLable;
    cocos2d::Label* m_pTrunLable;
    
    /*
     临时变量
     */
    int theMostEnemyCost;//敌人的最大能量消耗，用于招怪逻辑
    
    int m_iKnifeNum; //死神之舞时，显示在屏幕上的刀光数
    int m_iKnifeMax; //死神之舞时，显示在屏幕上的最大刀光数


    bool m_bFightPvpStart;
    Label * m_pLableTime;
    int  m_iAllTime;
    int  m_iCnt; //倒计时计数
    int  m_iSec;
    int  m_iHour;
    int  m_iMin;
    
    float  m_iMyTime;
    
    bool m_bJishiStart;
    
    LayerColor * m_pLayer;
    
    bool m_bHuitan;
    CC_SYNTHESIZE(int, m_iRobStones, RobStones);
    
    EventListenerTouchOneByOne* m_pListener;
    
    //用于战斗结束时的效果
    cocos2d::Vec3 _vec3DeadSpead;
    
    float _accelerationDiedY;
    float _accelerationDiedZ;
    
    CNpcFighter * m_pNpc1;
    CNpcFighter * m_pNpc2;
    CNpcFighter * m_pNpc3;
    
    
    FighterInfo  m_theInfo ;
    int m_iTouchExitState; //是否点击退出游戏, 0表示一般状态，1表示可以点击退出，2表示已经点过点击退出了
    
    bool m_bPvpMove;//标识是否在pvp敌方主界面移动
    bool m_bMenuValid;//开始战斗之前不能点击任何按钮
};









#endif /* defined(__TheGodWill__fightScene__) */
