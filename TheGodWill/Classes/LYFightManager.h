//
//  LYFightManager.h
//  TheGodWill
//
//  Created by user on 14/12/9.
//
//

#ifndef __TheGodWill__LYFightManager__
#define __TheGodWill__LYFightManager__

#include <stdio.h>

#include "cocos2d.h"

#include "BaseLayer.h"

#include "AllDefine.h"

class CBaseFighter;

class CFightManager {
    
public:
    
    static void addCamera(cocos2d::Node* target);
    
    
    /*
     parm1:开始的坐标
     parm2:结束的坐标
     parm3:默认的面向向量， 例： 面朝X轴正方向，就是（1，0） 面朝X轴负方向，就是（-1，0）.
            默认为y轴正方向（士兵的面向）
     
     return: 应该转的角度值， 可以用在setrotation
     */
    static float getTheDegreeWith(cocos2d::Vec2 StartPos, cocos2d::Vec2 endPos, bool b3D,cocos2d::Vec2 defaultVector=cocos2d::Vec2(0, 1));
    
    /*
     获取一个会往上漂浮。然后消失的系统字
     */
    static cocos2d::Sprite* getTheFlyWord(std::string text,std::string font="Arial",float fontSize=35,float upSpeed=0.5f, float waitTime=0.5f, std::string BGPath="zhdt.png", Color3B textColor=Color3B(253,161,63));
    
    static Sprite* getRewardWord(std::string text, float delayTime = 0.0f, std::string font="fonts/cuti.ttf", float fontSize=35);
    
    /*
     获取一个会网上漂浮的动作
     */
    static void getTheFlyAction(cocos2d::Node* target);
    
    /*
     给目标节点加上一个抖动动作
     */
    static void RunShakeAction(cocos2d::Node *target,float shakeDist,float shakeTime,float shakeTimes,float delay=0);
    
    /*
     给目标节点加上一个不倒翁抖动动作
     */
    static void RunTumblerAction(cocos2d::Node *target,float angle,float shakeTime,float shakeTimes,bool ifWeak,float delay=0,Vec3 defaultRoatation=Vec3::ZERO);
    
    
    /*
     根据路径来获取一个sprite3d对象（如果是png格式，则将此图附着在一个sprite3d上返回）
     */
    static Sprite3D* getTheModelWithPath(std::string theBulletPath);
    
    
    /*
     根据路径来获取一个animate动画
     */
    static Animate* getAnimateWithPath(std::string path1,int startNumber, int allNumber, std::string = ".png" , float interval = 0.1f);
    
    
    /*
     获得最靠前的友军士兵
     parm1:士兵阵营
     */
    static CBaseFighter* getTheFrontSoldier(FightGroup theGroup);
    
    /*
     获得最靠前的友军士兵或者建筑
     parm1:士兵阵营
     */
    static CBaseFighter* getTheFrontSoldierAndBuilding(FightGroup theGroup);
    
    
    /*
     获得护送者
     */
    static CBaseFighter* getTheWalker();
    
    
    
    /*
     获取相克系数
     */
    static float getTheAtrEffect(FighterType attacker,FighterType beAttacker);
    
    
    /*
     是否掉落物品
     */
    static bool ifDropItem(int type,int id);
    
    /*
     给角色加上指定buff
     */
    static void addTheBuffToFighter(CBaseFighter* target,AllBuffes buffId,float duration=3,float baseDamage=0);
    
    /*
     给角色加上表情对话
     */
    static void addTheFightTalkToFighter(CBaseFighter* target,int type,int talkId=0); //给指定人加给定类型对话
    static void addTheFightTalkToGroupAndId(FightGroup targetGroup,int talkId);//随机给阵营中一个人加对话
    
    
    
    /*
     边界判断，参数是一个位置值，返回值是符合地图边界的位置值
     */
    static Vec3 getThePosInMap(Vec3 tempPos);
    static Vec2 getThePosInMap(Vec2 tempPos);//这个x轴和z轴位移
    
    static void getMemoryList();//获得纹理列表
    
    static void removeUnusedMemory();//清空内存，删除不用的资源
    
    
    static void addSprite3dAnimationBeforeFight();//预加载3d模型动画
    
    
    /*
     根据所给buildingid，返回一个sprite3d
     */
    static Sprite3D* getModelWithPVPBuildingId(int PVPbuildingId,int type=0,int globZorder=0); //type 为0，直接返回整个模型， 1则直接返回上面模型，2则直接返回下面模型 . globZorder表示是否设置全局的zorder
    
    
    /*
     给一个sprite3d 附加上一个 变白的shader效果
     */
    static cocos2d::Node* addShaderToSprite3D(Sprite3D* temp,ShadeType targetShader,std::string GLProgramKey="");
    
    static void  addCloud(cocos2d::Ref *target, cocos2d::SEL_CallFuncO handler,LayerColor *pClor);
    
    
    static void closeCloud(cocos2d::Ref *target, cocos2d::SEL_CallFuncO handler,LayerColor *m_pLayer);
    
    
    /*
     预加载所有的shader效果
     */
    static void preloadShader(bool ifRefresh=false); //参数表示强制重新预加载一遍，用于android适配，android切后台会让所有shader出错，必须重新初始化
    
//    static void reloadAllFighterShader();//重新初始化所有士兵的shader
    
    
    
    
    cocos2d::SEL_CallFuncO m_pHandlerWhenGetUserFriendInfo;//获得的回调
    CC_SYNTHESIZE(cocos2d::Ref*,m_pTarget,Target);//回调函数的目标
    
};




#endif /* defined(__TheGodWill__LYFightManager__) */
