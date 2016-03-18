//
//  mainMenuLayer.h
//  TheGodWill
//
//  Created by user on 14/12/22.
//
//

#ifndef __TheGodWill__mainMenuLayer__
#define __TheGodWill__mainMenuLayer__


#include <stdio.h>
#include "LayerManager.h"
#include "BaseLayer.h"
#include "cocos-ext.h"
#include "AllDefine.h"
#include "LYFightData.h"

USING_NS_CC;
USING_NS_CC_EXT;

enum{
    MAP_BTN_DAILY = 100,//每日任务
    MAP_LEVEL_ONE,
    MAP_LEVEL_TWO,
    MAP_LEVEL_THREE,
    MAP_LEVEL_FOUR,
    MAP_LEVEL_FIVE,
    MAP_LEVEL_SIX,
    MAP_LEVEL_SEVEN,
    
    MAP_BTN_TASK,
    MAP_BTN_DRAWCARD,
    MAP_BTN_TEAM,
    MAP_BTN_UPGRADE,
    MAP_BTN_STORAGE,
    MAP_BTN_COLLEGE,
    MAP_BTN_HERO,
    MAP_BTN_SET,
    COIN_CHARGE_TAG,
    STONE_CHARGE_TAG,
    STAMINA_CHARGE_TAG,//体力充值
    MAP_BTN_PVP,
    MAP_BTN_ACTIVITY,
    MAP_BTN_CHARGE,
    MAP_BTN_SHOP,
};
enum{
    LEVEL_LINE_ONE = 1001,
    LEVEL_LINE_TWO,
    LEVEL_LINE_THREE,
    LEVEL_LINE_FOUR,
    LEVEL_LINE_FIVE,
    LEVEL_LINE_SIX,
    USER_INTRDUCE = 2000  //用户介绍
};

class CMainMenuLayer:public CBaseLayer,public TextFieldDelegate
{
public:
    CMainMenuLayer();
    ~CMainMenuLayer();
    static Scene* createScene()
    {
        // 'scene' is an autorelease object
        auto scene = Scene::create();
        
        // 'layer' is an autorelease object
        //auto layer = CMainMenuLayer::create();
        
        auto layer  = CLayerManager::GetSingletonPtr()->createLayer(eLayer_MainMenu, NULL,true);
        
        
        // add layer as a child to scene
        scene->addChild(layer);
        
        // return the scene
        return scene;
    }
    
    
    CREATE_FUNC(CMainMenuLayer);
    bool init();
    void initMap();
    void onEnterTransitionDidFinish();
    void btnCallBack(Ref* pSender);
    void addUserIcon();//添加金币，魔法师等图标
    void addBottomMenus();//添加地图下的菜单
    void scrollViewDidScroll( ScrollView *view ){}
    void addLevelBtn(const char* normal, const char* selected, Node* parent, cocos2d::Point pos, int tag, bool bTower = false);
    void addMapLine(const char* img, cocos2d::Point pos, int tag, int opcity = 0);
    void enterLevelInfo(int level);//关卡详情
    void update(float dt);
    void updateTime(float dt);
    void updateUsetInfo();//更新金币，魔法石，等数据
    void addMapEffect();
    void OnLvDetail(Ref *pSender);
    bool onTouchBegan(Touch *pTouch, Event *pEvent);
    void onTouchMoved(Touch *pTouch, Event *pEvent);
    void onTouchEnded(Touch *pTouch, Event *pEvent);
    void onTouchCancelled(Touch *touch, Event *pEvent);
    void guideEnterGame(float dt);
    void onEnter();
    void onExit();
    void detectTask();//任务检测
    void detectHeroUpgrade();//检测主角的升级
    void detectCompound();//检测是否可以合成怪物了
    
    void showDrawCardResult();//显示高级抽卡结束后的引导
    void OnMenuNameTouch(Ref *pSender);
    
    
    
    void addMidLevelLine();//添加大关卡之间的连线
    void addSKillFinishEffect(std::string  strMonsterSKill1,std::string  strMonsterSKill2,std::string strMonsterSKill3);

    void addCloud();//添加云层
    void updateMusicState();//更新背景音乐状态
    void updateArrowPos();//更新箭头位置

    void OnRemoveNotice();
    /*
     意见箱相关
     */
    void callBackSugges(cocos2d::Ref* pSender);

    void onBtnSuggesCancel(cocos2d::Ref *pSender);
    void onBtnSuggesConfirm(cocos2d::Ref *pSender);
    bool onSuggesTouch(Touch *pTouch, Event *pEvent);
    void OnSuggestion(Ref *pSender);
    
    void getMessgageBack(Ref* pSender);
    /*
     触摸方法
     */
    virtual bool onTextFieldAttachWithIME(TextFieldTTF * sender);
    virtual bool onTextFieldDetachWithIME(TextFieldTTF * sender);
    virtual bool onTextFieldInsertText(TextFieldTTF * sender, const char * text, size_t nLen);
    virtual bool onTextFieldDeleteBackward(TextFieldTTF * sender, const char * delText, size_t nLen);

    void upgradeAfter(Ref* pSender);
    void drawCardAfter(Ref* pSender);//抽卡之后的引导
    std::string getStrFromBackStr(const char* str);
    std::string getDigitFromBackStr(const char* str);
    void getSerTime(Ref* pSender);//获取服务器时间的回调
    void getUserTeamInfo(Ref* pSender);//获取玩家阵营
    
    void addGuideInfo();//添加英雄合成的引导
    bool m_bSkillStart;
 
private:
    void UpdateCheckSkill(float ft);
    
    bool IsSkillFinish();
    bool IsDrawCardRed();
    void OnGetCardInfo(Ref *pSender);
    cocos2d::Size m_winSize;
    ScrollView* m_pScrollView;
    LayerColor* m_pColor;
    Menu* p_Menu;
    Menu* m_pMenu;
    Menu* m_towerMenu;
    Menu* m_pTaskMenu;
    std::vector<LYMenuItemSprite*> m_bottomBtn;
    ProgressTimer* m_progress;
    Label* m_userLevel;
    EventListenerTouchOneByOne *m_pListener;
    
    EventListenerTouchOneByOne *m_pSuggesListener;
    
    Sprite* m_staminaBg;
    Label* m_pTime;//体力回复的时间
    long m_bufferIime;
    float  m_fpercentage;
    Label* m_pStaminaLabel;
    Label* m_pStaminaMaxLabel;
    Label* m_pCoin;
    Label* m_pStone;
    float m_buffer;
    Sprite* m_map;
    Sprite * m_pRedPoint; //红点
    Sprite * m_pDrawCardRedPt; //抽卡红点
    LYMenuItemSprite * m_pCollegeItem;
    LYMenuItemSprite * m_pDrawCardItem;
    int m_midLevel;//显示的通关数
    int m_reLevel;//实际通关数
    int m_lineNum;
    bool m_isEqlite;
    bool m_bShowTeam;//是否显示编队
    std::vector<midLevelInfo> m_midLevelInfoVec;
    int m_levelSum;//统计塔防模式关卡数量
    Sprite* m_arrow;
    Sprite* m_circle;
    float m_cloudTime;//云层出现的间隔时间
    std::string m_messageInfo;
    
    Sprite* m_compoundTip;//合成提示
    
    /*
     意见箱相关
     */
    std::string m_sSuggestionText;
    Sprite* m_pSuggestionBG;
    LayerColor* m_pSuggestionShadow;
    Label*  m_pUserName;
    TextFieldTTF* SuggestionInputText;
    
    //抽卡
    bool  m_bGetCard;
    float m_fTimeAll;
    int   m_iDrawCnt;
    DrawCardInfo  m_CardInfos;
    int  m_iRemainTime;
    int  m_iRemainStoneTime;
    
};
#endif /* defined(__TheGodWill__mainMenuLayer__) */
