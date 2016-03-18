//
//  LYBaseFighter.h
//  TheGodWill
//
//  Created by user on 14/12/1.
//
//

#ifndef __TheGodWill__LYBaseFighter__
#define __TheGodWill__LYBaseFighter__

#include <stdio.h>
#include "AllDefine.h"
#include "AllHead.h"

#include "cocos2d.h"

#include "LYHPBar.h"

#include "LYBaseBullet.h"




#define SOLDIER_ANIMATE_TAG  100
#define SOLDIER_FLY_ROTATE_TAG 101
#define SOLDIER_SHAKE_TAG 99

#define SOLDIER_DOCTOR 8

#define MAX_COLLID_TIMES  5    //最大的碰撞次数，超过这个次数将开启寻路


#define LINE_CHARGE_DIST  150   //横行士兵的冲刺距离

#define FIGHT_ELITE_ACTION_TAG 88

#include "LYSprite3dAnimateShaderOnce.h"

#include "LYSprite3dShaderWhite.h"

#include "LYSprite3dAnimateShaderWithFile.h"



class CBaseAttack;
class CBaseBuff;

struct pathPos;

class CFightTalk;


class CBaseFighter: public cocos2d::Node {
    
public:
    
    CBaseFighter();
    ~CBaseFighter();
    
    
    static CBaseFighter *create(FighterInfo *theInfo,FightGroup theGroup=eFightGroup_Freind,bool ifBoss=false,bool ifElite=false);
    
    
    virtual bool initWithInfo(FighterInfo *theInfo,FightGroup theGroup);
    
    virtual void onEnterTransitionDidFinish();
    
    void initAnimate();
    void initData();
    
    void initModelWithPath(std::string path);  //跟模型相关的所有初始化。
    
    void asyncLoad_Callback(cocos2d::Sprite3D* sprite, void* param);//异步加载模型，防止卡顿
    
    
    void RefreshFighter();
    
    
    /*
     3D相关处理
     */
    //    void setPosition3D();
    //    void setRotation3D();
    
    virtual void bornAnimate();
    
    virtual void update(float time);
    virtual void stateUpdate(float time);
    
    virtual void onExit();
    
    virtual void positionUpdate(float time);//位置更新
    void roatationUpdate(float time);//面向更新
    
    virtual void collidUpdate(float time);//碰撞更新，防止重叠
    virtual bool collidWithOther(CBaseFighter* target);//位置更新，防止重叠
    
    void stateChange(FighteState targetState);//改变状态时所做的初始化
    
    void KulouRebornAnimate();
    
    void XixueAnimate();

    void kuloureborn();
    

        
    void animateUpdate();
    virtual void animateChange(FighteState
                               targetAniamte,bool ifDoit=false);//改变动画 ,参数1：动画类型 参数2：是否强制播放（忽略动画优先级）
    
    void playAniamteAction(cocos2d::ActionInterval* theAction,bool ifloop=false,bool bReverse=false);
    
    
    void animationIdleCallBack();
    
    
    cocos2d::Point getFightPosition(){return cocos2d::Point(getPositionX(),getPositionZ());};//战斗中的坐标，z为y轴
    
    float getFightDistance(CBaseFighter* target);//获得真正的战斗距离
    
    void SearchingStart();
    
    bool ifSkillStart(int startCondition=0);//判断是否触发技能。并将要触发的技能id保存.  param：触发条件，0为攻击时触发，1为被攻击时触发，2为移动时触发 3未初始化时触发  4.时间隔触发 5死亡时触发  6.进入攻击范围就触发
    
    
    virtual void beAttacked(CBaseFighter* target,int attack=0,float damagePercent=1,FightGroup attackerGroup=eFightGroup_Freind,
                    DamageType type = eDamageType_GeneralAttack);
    
    
    void becured(int number,bool bxixue=false);
    
    
    
    virtual void onDied(DamageType type = eDamageType_GeneralAttack);
    
    void onAddEnergy();
    void OnUpdateEnergy();
    void onEnergySetNoraml();
    void onEnergySetWhite();
    
    CBaseBullet* bulletStart(int bulletId=0);  //不给参数则默认为怪物信息里的子弹信息。
    
    void onRemoved(bool ifRemoveSelf=false);
    
    virtual void reborn();
    
    void skillPrepare(bool ifShowName=true);
    void skillStart();
    void blowShake(bool ifShake);//士兵抖动
    
    
    void skillNameShow();//展示技能名字
    void skillNameShow(std::string strSkillName);
    
    
    void beatBackFrom(cocos2d::Vec3 originPos, float dist, float time,float highDist=0);//击退效果
    
    void beatBackUpdate(float time);
    
    void setElite(bool ifElite);//设置是否精英
    
    
    
    cocos2d::Vector<CBaseFighter*> chooseTarget();
    CBaseFighter* chooseTheCloseOneInVec(cocos2d::Vector<CBaseFighter*> target);//在容器里挑选离自己最近的那个
    
    
    
    MonsterDebuff* getMonsterDebuff();
    
    //    void  LaganMove(int x,int y,float fAngle);
    //    void  StopLaganMove();
    cocos2d::Sprite *m_pMapSprite;
    
    cocos2d::Vector<CBaseBullet*>* getAllBullets(){return &m_vecAllBullets;};
    cocos2d::Vector<CBaseBuff*>* getAllBuffes(){return &m_vecAllBuffes;};
    cocos2d::Vector<CBaseAttack*>* getAllAttackes(){return &m_vecAllAttackes;};
    
    
    
    MonsterDebuff* getTheDebuff(){ return &m_stMonsterDebuff;};
    
    float getNowSpeed(){return m_fAddMoveSpeed+m_strTheInfo->walkSpeed;};
    
    float getNowSpeedWithOutFrame(float durTime){return getNowSpeed()*durTime*60;};//无视帧数影响的速度位移。
    void set3DPosition(cocos2d::Vec3 pos, float dt);
    

    void setAddMoveSpeed(float time,int addOrmulti=0); //第二个参数表示是直接加值还是加比例,0表示直接加值
    
    bool ifClickFly(int damage);//是否击飞
    
    cocos2d::Vec3 getPhysicalPosition();//获得物理位置
    
    
    void setTheFightTalk(CFightTalk* theTalk){m_pTheTalk = theTalk;};
    
    bool ifCanTalk();
    
    void judgeActive();
    
    
    void clearAllSkillAndBulletAndBuff(bool ifClean=true); //清除所有子弹技能及buff , 为true 是直接删除不做任何操作
    
    
    void onDieing();
    virtual void afterDied();
    
    void attackAction();
    void setAddHpBlood();
    
    void changeSpriteScale(float targetScale);
    virtual void resetHPPosition();


    void setAddHpBlood(CBaseFighter *pFighter,int iAddHP);
    
    CC_SYNTHESIZE(cocos2d::Sprite3D*, m_pTheSrite, theSprite);
    
    void initShader();
    
    void resetShader(float time);  //将shader变回普通的
    
    void changeShader(ShadeType theType);//改变相应shader
    
    void reloadShader();//重新加载shader，用于android适配
    
    void changeHP(int damage,bool ifText,DamageType damageType,CBaseFighter* attackTarget,int ifForceDied);//第一个参数：伤害数值。第二个参数：是否显示.第三个参数：攻击类型。第四个参数：攻击者, 第五个参数：0表示无作用，1表示强制死亡
    
    double  m_fWushuXishu;
    double  m_fYexinXishu;
    double  m_fZiranXishu;
    
protected:
    
    /**
     动画相关
     **/
    cocos2d::ActionInterval* m_pAction_IDle;
    cocos2d::ActionInterval* m_pAction_Walk;
    cocos2d::ActionInterval* m_pAction_Attack;
    cocos2d::ActionInterval* m_pAction_specialAttack;
    cocos2d::ActionInterval* m_pAction_Hurt;
    cocos2d::ActionInterval* m_pAction_Die;
    cocos2d::ActionInterval* m_pAction_Dizzy;
    
    /**
     士兵数据
     **/
    CC_SYNTHESIZE(FighterInfo*, m_strTheInfo, TheInfo);//士兵信息

    CC_SYNTHESIZE(FightGroup, m_iFightGroup, FightGroup);
    
    CC_SYNTHESIZE(AttackType, m_iAttackType, AttackType);
    
    CC_SYNTHESIZE(bool, m_bIfReborn, IFReborn);
    
    CC_SYNTHESIZE(int, m_iNowHP, NowHP);
    CC_SYNTHESIZE(int, m_iMaxHP, MaxHP);
    
    CC_SYNTHESIZE(int ,m_iAddHpDaoBin,AddHpDaoBin); //刀兵兵团优势专有攻击值
    
    
    CC_SYNTHESIZE(int, m_iNowAttack, NowAttack);
    
    CC_SYNTHESIZE(int, m_iNowRadius, NowRadius);
    
    CC_SYNTHESIZE(bool, m_bIfElite, IfElite);
    
    CC_SYNTHESIZE(bool, m_bIfSmallBoss, IfSmallBoss);
    
    CC_SYNTHESIZE(bool, m_bIfReinforce, IfReinforce);  //是否援军
    
    
    /*
     临时数据
     */
    
    float m_fAttackColdNow;//攻击当前间隔
    
    float m_distance;
    
    cocos2d::Vec2 m_vec2IdlePos;
    
    
    CC_SYNTHESIZE(bool, m_bIfDied, IfDied);//士兵死亡标识
    
    
    CC_SYNTHESIZE(FighteState, m_iFightState, FightState);//战斗状态
    CC_SYNTHESIZE(FighteState, m_iAnimateState, AnimateState);//动画状态。  例：战斗中攻击间隔时的空闲动画。因此战斗状态和动画状态不一定一致
    
//    CC_SYNTHESIZE_RETAIN(CBaseFighter*, m_pAttackTarget, AttackTarget);//攻击士兵信息
protected:
    CBaseFighter* m_pAttackTarget;
public:
    CBaseFighter* getAttackTarget(){return m_pAttackTarget;};
    virtual void setAttackTarget(CBaseFighter* target,bool ifRec=false);
    
    
    CC_SYNTHESIZE_RETAIN(CBaseFighter*, m_pDeadlyTarget, DeadlyTarget);//致死者
    CC_SYNTHESIZE(FightGroup, m_iDeadlyGroup, DeadlyGroup);//致死者阵营
    
    CC_SYNTHESIZE_RETAIN(CBaseFighter*, m_pSkillTarget, SkillAttackTarget);//技能瞄准的士兵
    
    CC_SYNTHESIZE(cocos2d::Vec2, m_v2MoveControll, MoveControll);//移动控制
    
    
    CC_SYNTHESIZE_READONLY(float, m_fAddMoveSpeed, AddMoveSpeed);//额外移动速度
    
    
    CC_SYNTHESIZE(bool, m_bIfCampMonster, IfCampMonster);//是否兵营怪物
    
    CC_SYNTHESIZE(bool, m_bIfLevelBoss, IfLevelBoss);//是否关卡boss
    
    
//    CC_SYNTHESIZE(cocos2d::Vec3, m_v3BeatBackPos, BeatBackPos);
    
    
    cocos2d::Vector<CBaseBullet*> m_vecAllBullets;//存储发射的所有子弹。如果自身死亡。则所有这些子弹都要消失。免得出bug
    cocos2d::Vector<CBaseBuff*> m_vecAllBuffes;
    cocos2d::Vector<CBaseAttack*> m_vecAllAttackes;
    
    
    //击退相关
    float m_v3BeatMaxtime;//击退的最终位置
    float m_v3BeatNowtime;//击退的最终位置
    cocos2d::Vec3 m_v3BeatSpeed;//击退的速度
    
    float m_fBeatFlyAccelerated;//击飞的加速度
    
    
    
    MonsterDebuff m_stMonsterDebuff;
    bool m_bIfSearchMode;//是否寻路模式
    int m_iCollidTimes;//碰撞次数
    cocos2d::Vec3 m_v3SearchPos;//寻路模式下的坐标点
    
    FighterSkillInfo* m_iNextSkill;//即将释放的技能信息
    FighterSkillInfo* m_pPassiveSkill;//即将释放的被动技能
    
    
    int m_MoveTime;  //因受玩家风力影响而表现出的位移次数，暂定为3次
    int m_offSet;//记录移动距离
    bool m_ifValid;//标识是否是抖动之后的位移
    
    bool m_isCut;//士兵是否被刀光砍到
    //    FighterInfo theInfo;
    
    /*
     3D表现相关
     */
    CC_SYNTHESIZE(cocos2d::Vec3, m_v2AttackPosOffset, AttackPosOffset);//攻击开始位置偏移初始化
    CC_SYNTHESIZE(float,m_fReDamagexishu,TheXishu);  //被动技能系数
    
    CSprite3dAnimateShaderOnce* _hurtShader;
    
    CSprite3dShaderWhite* _hurtShaderBuilding;//这里不得不吐槽一下。因为上一个shader无法让建筑正常显示，而这个shader无法让有动作的模型正常显示，所以只能写两个shader，分情况处理。
    
    CSprite3dAnimateShaderWithFile* bossShader;
    
    CSprite3dAnimateShaderWithFile* eliteShader;
    
    // float  m_fReDamagexishu;
    
    
    
    //CC_SYNTHESIZE(float,);

    //减伤系数
    
  
    CC_SYNTHESIZE(CHPBar*, m_pTheHpBar, TheHPBar);
    
    cocos2d::ParticleSystemQuad* walkingSmoke;
    
    CC_SYNTHESIZE(cocos2d::Sprite3D*, m_pShadow, TheShadow);
    CC_SYNTHESIZE(cocos2d::Sprite3D*, m_pShadow2, TheShadow2);
    
//    cocos2d::Sprite3D* m_pShadow;
    
    cocos2d::ParticleSystem* eleteStar;
    
    cocos2d::Sprite3D* m_pEliteCircle;
    
    
    float m_fSpriteScale;
    
    
    //塔防模式数据
    pathPos* thePathInfo;
    
    //战斗对话框
    CFightTalk* m_pTheTalk;
    
    float m_fNowTalkCold;
    float m_fMaxTalkCold;

    
    int  m_iHuoYanSkinNum;
    
    bool m_bWanshi;
    bool m_bKulouDie;
    
 
 
    
    

    bool m_bIfActive;  //出生前为非活动状态，出生后激活
    CC_SYNTHESIZE(cocos2d::Sprite3D*, m_pTheActiveCircle, TheActiveCircle);
    
    CC_SYNTHESIZE(bool,m_bzhaohuan,IsZhaohuan);
    
    
    //进入战斗时的时间
    CC_SYNTHESIZE(float,m_FightStateTime,FightStateTime);
    
    //m_bSoul
    CC_SYNTHESIZE(bool ,m_bSoul,Soul);
    
    /*
     pvp相关
     */
    int skilltimes;//统计技能次数，根据此技能次数，决定放什么技能
    float skillRatios[4];//统计技能触发，根据此数值，决定放不放技能
    
    CC_SYNTHESIZE(int, m_iTrueId, TrueId);//怪物唯一标识
    
    
    /*
     测试用数据
     */
    
    float m_fLiveTime;

    /*
     炸弹人是否爆炸过
     */
    bool m_bIfBombed;
    bool m_bStop;//标识当前巡逻的人是否停止巡逻（用于巡逻者）
    
    
    bool m_bShixue;
    CC_SYNTHESIZE(cocos2d::Sprite3D*, m_pvpShadow, PvpShadow);//建筑阴影
    CC_SYNTHESIZE(cocos2d::Sprite3D*, m_pvpDizuo, PvpDizuo);//建筑底座
    
};










#endif /* defined(__TheGodWill__LYBaseFighter__) */
