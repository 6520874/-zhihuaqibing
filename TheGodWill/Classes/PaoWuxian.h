#ifndef __CPAOWUXIAN_SCENE_H__
#define __CPAOWUXIAN_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;


#include "DataManager.h"

class CFightScene;

class CPaoWuxian: public cocos2d::Layer
{
public:
    
    CPaoWuxian(Vec3 VCurPos);
    ~CPaoWuxian();
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* scene();
    
    // a selector callback
    void menuCloseCallback(Ref* pSender);
    
    bool onTouchBegan(Touch *pTouch, Event *pEvent);
    void onTouchMoved(Touch *pTouch, Event *pEvent);
    void onTouchEnded(Touch *pTouch, Event *pEvent);
    void onTouchCancelled(Touch *touch, Event *pEvent);
    void refreshPath();
    void StartToushiche(cocos2d::Ref * m_pTarget,
                        cocos2d::SEL_MenuHandler m_pHandler,CFightScene *pScene);
    
    
    void EndTouuShiche();
    void RemoveTouch();
    
    void lineUpdate(float time);
    
    void onDamage();
    
    
    /*
     pvp相关
     */
    void StartAttack();
    void StartAttackWith(Vec3 ballPos,Vec2 speed,int attackDamage);
    
    
    
    static CPaoWuxian* create(Vec3 vCurPos,CFightScene* tempScene) \
    { \
        CPaoWuxian *pRet = new CPaoWuxian(vCurPos); \
        pRet->m_pFightScene=tempScene; \
        if (pRet && pRet->init()) \
        { \
            pRet->autorelease(); \
            return pRet; \
        } \
        else \
        { \
            delete pRet; \
            pRet = NULL; \
            return NULL; \
        } \
    }
    
    Sprite3D* m_startBall;
    Sprite3D* m_theBall;
    bool  m_bPaoActive;
    
    CC_SYNTHESIZE(Sprite*, m_pTheDragBall, TheDragBall);//拖动的按钮
    
    
private:
    void OnTouchMoveBall(Ref  *pSender);
    void OnTouchStarBall(Ref *pSender);
    void update(float time);
    void onExit();
    
    void UpdateShake(float ft);
    
    void DrawSmallPath();
    void CameraRunWithSmallMap(float x);
    Vec3 startPoint;
    Vec3 endedPoint;

    cocos2d::Point m_sBeginPt;
    cocos2d::Point m_sEndedPt;
    float  SpeedMax;
    float pullMax;
    float addSpeed;
    float targetSpeedX;
    float targetSpeedY;
    float flyTime;
    
    Vec2 m_vecTouchStartPoint;//起始的触摸点，用于判断是否取消投掷

 
    Vec3  m_TmpCameraVec3;
    
    Sprite3D * m_pMiaozhunJing;  //用于显示投石机落点
    Sprite3D * m_pMiaozhunJing2;
    
    Sprite *  m_pToushicheSmallMap;
    
    CC_SYNTHESIZE(Vec3, m_BallStartVec3, UpdateBallStarVec3);//是否需要更新用户信息
    
    CC_SYNTHESIZE(Vec3, m_CarOffsetVec3, CarOffsetVec3);//投石车的位置偏移

    Vec3 m_startPosVec3;//投石车的起始坐标
    
    //震动逻辑
    bool  m_bShake;
    int   m_iShakeRange;  //震动幅度
    int m_iShakeTime;
    int m_iShakeSec;
    
    int m_iPastY;
    int m_iOffsetY;
    
    int m_iShakeTimes;
    
    std::vector<Sprite3D*> m_pathSpr;
    std::vector<Sprite*> m_pSmallPathVec;
    Vec3 m_VCurPos;
    cocos2d::Size  m_WinSize;
    EventListenerTouchOneByOne* m_pListener;
    cocos2d::Ref * m_pTarget;
    cocos2d::SEL_MenuHandler m_pHandler;
    CFightScene *m_pFightScene;
    
    int m_iLineUpdateIndex;//用于显示抛物线动作的顺序
    FighterSkillInfo m_stSkillInfo;
    
    /*
     pvp相关
     */
    bool m_bIfRecMode ;//是否录像模式
    int m_iTheDamage;
    
};

#endif // __HELLOWORLD_SCENE_H__
