//
//  pvpLayer.h
//  WarOfGod
//
//  Created by user on 15/6/3.
//
//

#ifndef __WarOfGod__pvpLayer__
#define __WarOfGod__pvpLayer__

#include <stdio.h>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "BaseLayer.h"
#include "AllDefine.h"
#include "LYBaseFighter.h"
#include "pvpCommonInfoLayer.h"
#include "LYBaseBuilding.h"
#include "DataManager.h"
#include "LYBeatNumbers.h"
#include "pvpCommonData.h"
#include "thePartolFight.h"


USING_NS_CC;
USING_NS_CC_EXT;

enum{
    PVP_STAMINA_CHARGE = 1,
    PVP_COIN_CHARGE,
    PVP_STONE_CHARGE,
    PVP_BACK_TAG,
    PVP_RESEARCH_TAG,
    PVP_BUILD_TAG,
    PVP_MESSAGE_TAG,
    PVP_FIGHT_TAG,
    PVP_BULIDING_CANCEL,
    PVP_BULIDING_OK,
    PVP_BULIDING_INFO,
    PVP_BUILDING_PRODUCT,
    PVP_BUILDING_EVOLUTION,//进化
    PVP_BULIDING_UPGRADE,
    PVP_BULIDING_DEL,
    PVP_BULIDING_FINISH,
    PVP_BUILDING_KUANSHI,
    PVP_BUILDING_DINGSHI
};

enum
{
    PVP_NPC1TAG = 101,
    PVP_NPC2TAG,
    PVP_NPC3TAG,
    
};

struct buildingMessage
{
    int buildingId;
    int xPos;//x坐标
    int zpos;//z坐标
    int state;//状态
    int startUpTime;//开始升级时间
    int needTime;//升级完成所需时间
};



struct btnObbInfo
{
    Sprite3D* sp;
    OBB obb;
};


#define MAP_START_XPOS 1950
#define LENGTH_BLOCK_PER 48  //每个块所占的长度
#define LENGTH_WALL_DIS 50   //城墙的半宽
struct newBuildInfo
{
    Sprite* newTower;
    bool valid;//是否可以创建
};

enum BUILDUSETYPE
{
    PVP_MAINBASE = 0,//暂时不用
    PVP_INCOMME,
    PVP_DEFENSE,
};

class thePartolManLayer;

class pvpMainLayer:public CBaseLayer, public ScrollViewDelegate
{
public:
    pvpMainLayer();
    ~pvpMainLayer();
    CREATE_FUNC(pvpMainLayer);
    bool init();
    void initUI();
    void onEnter();
    //    bool onTouchBegan(Touch* pTouch, Event* pEvent);
    //    void onTouchMoved(Touch* pTouch, Event* pEvent);
    //    void onTouchEnded(Touch* pTouch, Event* pEvent);
    virtual void onTouchesBegan(const std::vector<Touch*>& touches, Event *unused_event);
    virtual void onTouchesMoved(const std::vector<Touch*>& touches, Event *unused_event);
    virtual void onTouchesEnded(const std::vector<Touch*>& touches, Event *unused_event);
    
    void update(float dt);
    
    void adduserInfo();
    void addBtn();
    void btnCallBack(Ref* pSender);
    void upgradeBtnCallBack(Ref* pSender);//升级回调
    
    void ZhaiBaoshi();
    void Enemy(Ref *pSender);
    void UpdateKuanshi(Ref *pSender);
    
    void rmvSelectedBlock();//取消所占格子状态
    
    void getValidLocation(Vec2 touchPos);//寻找放置的位置
    void addTower(int buildingID);
    void initTowerInfo(int buildId);//添加原有的塔（新建账户时初始化一次）
    void intFence(int buildId, float xPos, float zPos, float opcity, float rotate = 0);//初始化围墙
    void addBuildObb(buildInfo* pBuild);
    void calculateRayByLocationInView(Ray* ray,Camera* theCamera, const Vec2& location);
    
    int adjustIsTake(float xValue, float yValue);//判断当前位置是否被占用 0:未被占用 1:占用 2:越界不可用
    void updateAllBulidObb();//刷新所有塔的包围盒
    
    void addArrow();
    void addOkAndCancel();//添加放置和取消按钮
    void addUpgradeAndDetail();//添加详情和升级按钮
    void addBtnObbInfo();//初始化删除和放置的包围盒
    void addUpgradeInfo(bool bNewTower = false);//添加升级图标和进度
    void adjustTouchBtn(Vec2 touchPos);//判断是否点击到了删除或者建造按钮
    void rmvBtnAndArrow();//删除按钮和箭头
    void showFreeBlock();//移动的时候只显示未被占用的块
    void showSelectedBlock();//限制选中塔的占用区域
    
    void showBuildList();//显示可建造的建筑列表
    void addBuildUseTypeNum();//添加各类型下的可创建建筑的数量
    void showListInfo();
    void detectIfCanCreate();//检测在主基地当前等级下是否有可以添加的建筑
    int getNumCreate(BUILDUSETYPE buildUseType = PVP_DEFENSE);//返回当前类型的建筑可创建的数量 1:防御类 2:经济类
    std::string getTowerLimitInfo(int buildId);//判断当前建筑是否可以被创建/升级
    int getUpgradeLimit(int buildId);//判断当前是否可以
    bool adjustIfUpgrade(int buildId);//判断是否能够升级
    std::string calculateTime(int allTime);//时间换算
    bool judgeBuildNumLimit(int buildType);//判断当前添加的塔是否满足限制条件
    void addNewTower(int buildType);//创建新的塔
    void sureCreateTower();//确定新建建筑
    void addTowerOBBInfo();//添加新塔兵保存新塔包围盒
    void finsishUpgradeImmediate();//立即完成升级（升级过程中）
    void directlyFinish();//在升级未开始前直接完成升级
    void rmvUpgradeInfo();//删除升级的进度条和动画
    void addProductInfo();//添加熔炉进度信息
    void saveRongluState(int userBuildingId, bool bProductEnd = false);//保存熔炉的熔炼状态
    void isEndProductBack(Ref *pSender);//判断是否熔炼结束回调
    void startProduct(int time);//开始熔炼
    void addStartUpgradeInfo(int state = 2);//点击升级后修改建筑属性
    void productInfoBack(Ref* pSender);//获取熔炼消息回调
    void switchBuildType(Ref* pSender);//切换建筑类型
    void setBtnEnable(bool valid = false);
    
    void sendRequestSaveTeamInfo(bool bIfUpgrade = false);//保存玩家阵营信息
    void saveUserTeamInfo(Ref* pSender);
    void updateTowerInfo();//移动位置后更新塔的信息并保存到数据库
    void rmvTowerInfoLayer();
    void getUserDefenseVideoList(Ref* pSender);//获取玩家防守对象列表
    void getServerTime(Ref* pSender);//获取服务器时间
    
    buildInfo* adjustTouchTower();//判断是是否点击到了塔
    
    void resetBlockState();//将块的状态置为原来的状态
    
    void addBuildInfo(Node* parent, Vec2 pos, int tag, int index, bool bValid);//最后一个参数表示是否可以创建
    void updateUserData(Ref *pSender);//更新用户金币等
    void startUpgradeTower(int buildingId);//开始升级
    
    void cancelUpgrade(Ref* pSender);//确定取消回调
    void sureUpgrade(Ref* pSender);//确定升级回调
    void rmvNewTower();//删除新添加的塔
    void updateKuangshiLimit();//计算矿石上限
    void updateStoneRate();//更新生产矿石速率
    
    void getEnemyTeamInfoBack(Ref *pSender);
    
    void startSearchEnemy();
    
    void updateVideoState(int videoId);//状态
    void updateKuangchangState(CBaseBuilding* tower);//更新矿场的生产状态
    void adjustUpgradeEnd(Ref* pSender);//获取服务器时间判断当前是否已经升级结束
    void collectStoneBack(Ref* pSender);//采集矿石的回调
    void addGuideInfo(GuideState type, int num);//添加引导信息
    
    void createDirectly();//直接创建建筑，没有时间
    bool dectectRongluGuide();//判断是否有熔炉引导
    void adjustTouchEnded(int type);//触摸结束位置判断
    void evolutionBack();//进化回调
    void gotoShop(Ref* pSender);
    void addPattolInfo(buildInfo* pTower, bool bEvolution = false);
    void startPvpFight(Ref* pSender);//开始进入pvp战场
    void smeltSuccessCallBack(Ref* pSender);//熔炼完成回调
    
    void detectPvpGuideInfo();//开始pvp引导
    void searchCallBack(Ref* pSender);
    void getVideoListInfo();
    
    
    void BuildingFininsh(buildInfo* p=NULL);
    Vec3 getNearEightPos(int x,int z);
    locationInfo * GetAllBlock();
    locationInfo m_allBlock[MAP_X_INDEX][MAP_Y_INDEX];//时间确定好方块的分布
    
    
    void saveTheBootMap(Ref* pSender);//debug用，存储当前地图信息
    
    std::vector<Vec3> getNpcValidPlace(std::vector<Vec3> tAdd);
    
private:
    
    FighterInfo m_theInfo;
    
    cocos2d::Size m_WinSize;
    EventListenerTouchAllAtOnce* m_listen;
    cocos2d::Camera *_camera;
    //    CC_SYNTHESIZE(cocos2d::Sprite3D *, m_spriteBg, SpriteBG);
    CC_SYNTHESIZE_READONLY(cocos2d::Layer*, layer3D, theLayer3D);
    
    std::vector<towerOBBInfo> m_allBuildObb;//地图上所有的建筑
    std::vector<locationInfo> m_blockPosVec;//块中心坐标
    std::set<float>m_blockxLineVec;//x线的坐标
    std::set<float>m_blockyLineVec;//y线的坐标
    std::set<float>m_blockXPointVec;//x点的坐标
    std::set<float>m_blockyPointVec;//y点的坐标
    bool m_bValid;//判断移动结束时找到的位置是否可用
    
    Sprite* m_staminaBg;
    Label* m_pStaminaLabel;
    Label* m_pStaminaMaxLabel;
    long m_bufferIime;;
    Label* m_pTime;//体力回复的时间
    Label* m_pCoin;
    Label* m_pStone;
    ProgressTimer* m_progress;
    
    BillBoard* m_timeProgressBg;//进度背景
    ProgressTimer* m_timeProgress;//升级进度，研究进度，建造进度
    Label* m_pTimeLabel;//剩余时间
    
    //pvpBuildInfo* m_buildInfo;
    
    
    std::vector<Sprite3D*> m_selectedBlock;//保存即将占用的块的图片
    buildInfo* m_curBuild;//当前选中的塔
    buildInfo* m_preBuild;//之前前选中的塔
    
    int m_curBuildingId;//当前选中塔的id
    
    bool m_bNewTower;//标识是否是新的塔
    bool m_bActive;//标识是否处于激活状态正在寻找位置建塔
    towerOBBInfo* m_newTowerObbInfo;//记录刚添加的塔的空间信息
    Vec3 m_oldPos;//记录被选中的塔的起始位置
    bool m_bMoveMap;//标识是否移动地图
    bool m_bMove;//标识是否激活寻址
    
    bool m_bValidMove;//用于撤销选中状态时判断在一定范围内移动都不算做移动
    std::vector<Sprite3D*> m_arrowVec;
    std::vector<LYMenuItemSprite*> m_btnVec;//删除和放置
    std::vector<LYMenuItemSprite*> m_stoneVec;//存放矿石按钮
    std::vector<btnObbInfo> m_btnObbVec;
    Vec2 m_startPos;//起始触摸点
    Menu* m_btnMenu;//选择和删除按钮
    bool m_bFirst;//首次创建并找到了合适的位置
    int m_friendTowerLevel;//保存主基地等级
    LayerColor* m_pLayer;
    int m_stone;//用户当前矿石储量
    
    int m_iAllStoneNum; //用户魔法石总量
    
    std::string m_enemyMaps;
    
    //std::vector<buildingMessage> m_allBuildInfoVec;
    
    int m_serverTime;//客户端开始计时的起始时间
    
    float m_limitTime;//保护时间计时
    int m_protectedTime;//玩家受保护的时间
    Label* m_protectLabel;//保护时间显示
    
    LabelAtlas* m_pStoneAllNum;
    CBeatNumbers* m_pCurStoneNum;
    
    Label* m_addLabel;//当前矿石的生产速率
    int m_addStone;//矿石增量
    Vec3 m_towerPos;//记录塔的起始位置
    
    float m_distance;//多点触摸时两个点之间的起始距离
    double m_deltax;    //目标x轴的改变值
    double m_deltay;    //目标y轴的改变值
    
    pvpCommonInfoLayer* m_layer;
    
    std::vector<videoListInfo> m_allVideoInfo;//所有的录像信息
    LYMenuItemSprite* m_msgItem;//战报列表
    LYMenuItemSprite* m_buildItem;//建筑列表按钮
    LYMenuItemSprite* m_pFightItem;//
    LayerColor* m_buildLayer;//建筑列表下的层
    LYMenuItemSprite* m_pDefenseItem;//防御建筑按钮
    LYMenuItemSprite* m_pIncomeItem;//经济建筑按钮
    LYMenuItemSprite* m_backItem;//返回按钮
    
    LYMenuItemSprite * m_pCoinItem;
    LYMenuItemSprite * m_pStoneItem;
    LYMenuItemSprite * m_pStamainItem;
    
    int m_iBuildUseType;//当前创建的建筑类型  这个值应该根据提示创建的建筑类型来初始化，1:经济类型 2:为防御类型
    int m_iTipNum;//新的录像数量
    ScrollView* m_scrollView;
    std::vector<newBuildInfo*> m_allBuildType;//所有建筑类型
    newBuildInfo* m_selectedItem;//选中项
    int m_neddStone;//升级所需的魔法石
    int m_cost;//创建、升级建筑的花费
    int m_remainTime;//当前升级的建筑剩余时间
    float m_time;//时间计算
    int m_upgradeBuildId;//正在升级的建筑id
    buildInfo* m_upgradeTower;//记录升级的建筑
    bool m_upgradeEnd;//是否升级结束
    Label* m_towerName;//选中塔名字
    Label* m_towerLevel;//选中塔等级
   
    //pvp
    int m_userId;//玩家id
    std::string m_name;//玩家名字
    int m_level;//玩家等级
    Sprite* m_hammer;//升级的锤子
    int m_userBuildingId;//唯一标识建筑id
    Sprite* m_pvPStone;
    Sprite3D* m_pCircle;//主基地底下的光圈
    int m_stoneRate;//当前矿石的生产速率
    Sprite * m_searchBg;//搜索框
    float   m_fKuanShiAllTime;
    float  m_fAddStone;
    float  m_fAllPerTime;
    
    float m_fStartPercent;
    float m_fEndPercent;

    float m_fSecPass;
    
    int m_iHaveStoneNum;
    

    
    bool m_bAddPer;
 
    
    
    //pvp
    std::vector<towerOBBInfo> m_pKuanchanBuildVec;//保存所有矿产建筑
    
    bool m_bBaoshi;
    
    //引导
    Vec2 m_guidePos;
    //熔炉相关操作
    int m_productRemainTime;//熔炼剩余时间
    float m_fTime;//熔炼时间计算
    int m_productNum;//当前熔炼结束后获得的魔法石
    int m_productNeedTime;//熔炼所需时间
    bool m_bProductEnd;//是否熔炼结束
    BillBoard* m_productProgressBg;//进度背景
    ProgressTimer* m_productProgress;//熔炼进度
    Label* m_productTimeLabel;//剩余时间
    int m_productBuildId;//熔炉的建筑id
    buildInfo* m_productTower;//记录熔炼的建筑
    
    CPartolFight* m_patrolman;//巡逻者
    FighterInfo m_thePatrolInfo;
    
    //bool m_bTouchOk;
    
    bool m_touchFlag;//这个标志位用于连续点击时触摸结束后直接调用move函数初始化建筑占用状态
};

#endif /* defined(__WarOfGod__pvpLayer__) */
