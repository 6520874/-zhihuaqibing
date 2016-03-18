//
//  zzbNetControlModule.h
//  GAME2
//
//  Created by spf on 14-7-21
//
#ifndef __GAME2__zzbNetControlModule__
#define __GAME2__zzbNetControlModule__

#include <iostream>
#include "cocos2d.h"
#include <string>

#include "../LYPersonalData.h"


struct _netNewSendEventBase;





#define NetDelayTime 15


class CNetDialog;

using namespace std;

//又是一个单例类，但是这个类貌似不保存数据的说
struct HTTP_PACKAGE
{
    int m_nEventType;//消息类型
    
    std::string m_strHttpUrlDate;
    /*
     每一个包都带回调函数
     */
    cocos2d::Ref * m_pTarget;
    cocos2d::SEL_CallFuncO m_pHandler;
    
    bool m_bIfNeedDialog;
    
    std::string postData;
    
    //构造析构函数
    HTTP_PACKAGE():
    m_pTarget(NULL)
    ,m_pHandler(NULL)
    {}
    ~HTTP_PACKAGE(){}
};



class CNetControlModule:public cocos2d::Node
{
    
public:
    static CNetControlModule* sharedNetControlModule();
    
    
    static std::string encode(const std::string str);  //防止乱码的编码格式，编码后传给服务器
    static std::string decode(const std::string str);  //防止乱码的解码
    

private:
    CNetControlModule();
    ~CNetControlModule();
    bool addHttpPackage(HTTP_PACKAGE& strPackage);
    void update(float dt);
    void sendHttpPackage(HTTP_PACKAGE& package);
    
   
    
private:
    bool init();
    void recvHttpPackage(cocos2d::Node* node,void* data);
    void onBtnErrorHint(std::string hintBuffer);
    void onEventCallBackYes(cocos2d::Ref* pSender);
    void onEventCallBackNo(cocos2d::Ref* pSender);
    bool m_bShowHttpDialog; //是否显示网络对话框

protected:
    std::list<HTTP_PACKAGE> m_lstNetPackage;//保存http发送的消息包
    
    bool m_bIfSending;//是否正在发送中
    int m_nResendCount;
    
//    CNetDialog* m_pNetDialog;
    CC_SYNTHESIZE(CNetDialog*, m_pNetDialog, NetDialog);
    void removeNetDialog();
    float m_fNetTime;
    
    
    
    
public:
     void SaveDinShiTime(cocos2d::Ref* pTarget,cocos2d::SEL_CallFuncO phandler,int iUserBuildId,int iTime);
    void AddHttpPackage(cocos2d::Ref* pTarget,cocos2d::SEL_CallFuncO phandler,_netNewSendEventBase & event,bool ifNeedDialog=false);
    void AddImportantHttpPackage(cocos2d::Ref* pTarget,cocos2d::SEL_CallFuncO phandler,_netNewSendEventBase & event, std::string httpStr, bool ifNeedDialog=false);
    
//    void getEventDrawCard(cocos2d::Ref*pTarget,cocos2d::SEL_CallFuncO phandler);//得到抽卡信息 hsx
//    bool GetShowHttpDialog();
//    
//    void SetHttpDialog(bool bShowHttpDialog);
    
    //版本号判断
    void versionJudge(cocos2d::Ref* pTarget,cocos2d::SEL_CallFuncO phandler);
    
    //新用户登陆
    void newLogin(cocos2d::Ref* pTarget,cocos2d::SEL_CallFuncO phandler);
    
    //登陆
    void Login(cocos2d::Ref* pTarget,cocos2d::SEL_CallFuncO phandler);
    
    //修改姓名
    void updateUserName(cocos2d::Ref* pTarget,cocos2d::SEL_CallFuncO phandler,std::string strUserName);
    
    //提交意见
    void uploadSuggestion(cocos2d::Ref* pTarget,cocos2d::SEL_CallFuncO phandler,std::string content);
    
    
    //游戏结束
    void gameOver(cocos2d::Ref *pTarget);
    
    
    void SaveUserMCF(cocos2d::Ref* pTarget,cocos2d::SEL_CallFuncO phandler,int iCoinAdd=0,int iStoneAdd=0,int iStamina=0);
    
    //获取充值信息
    void getChargeInfo(cocos2d::Ref* pTarget,cocos2d::SEL_CallFuncO phandler);
    
    //获取服务器时间
    void getServerTime(cocos2d::Ref* pTarget,cocos2d::SEL_CallFuncO phandler);
    
    //发送充值成功请求
    void sendChargeInfo(cocos2d::Ref* pTarget,cocos2d::SEL_CallFuncO phandler, std::string tempStr);
    
    void BuyShopPropOnly(cocos2d::Ref* pTarget,cocos2d::SEL_CallFuncO phandler,int iPropId);
    void IsShopOnlyBuy(cocos2d::Ref* pTarget,cocos2d::SEL_CallFuncO phandler,int iPropId);
    
    
    
///pvp
    //获取玩家pvp阵营信息
    void getUserTeamInfo(cocos2d::Ref* pTarget,cocos2d::SEL_CallFuncO phandler);
    
    //保存玩家pvp阵营信息
    void setUserTeamInfo(cocos2d::Ref* pTarget,cocos2d::SEL_CallFuncO phandler, std::string teamInfo, int level);//主基地等级
    
    //获取敌方pvp阵营信息
    void getEnemyTeamInfo(cocos2d::Ref* pTarget,cocos2d::SEL_CallFuncO phandler, int userId);
    
    //随机获取敌方pvp阵营信息
    void getRandomEnemyTeamInfo(cocos2d::Ref* pTarget,cocos2d::SEL_CallFuncO phandler, int level, std::string content);
    
    //上传万家对战录像
    void uploadVideo(cocos2d::Ref* pTarget,cocos2d::SEL_CallFuncO phandler, int userId, std::string teamInfo, std::string attackTeam,
                     std::string enemyInfo, std::string enemySolider, int stone, std::string video, int iSuccess, int percent);//0标识防守成功1标识失败
    
    //获取玩家防守录像目录
    void getDefenseVideoList(cocos2d::Ref* pTarget,cocos2d::SEL_CallFuncO phandler);
    
    //获取玩家指定防守录像
    void getDefenseVideoByVideoId(cocos2d::Ref* pTarget,cocos2d::SEL_CallFuncO phandler, int videoId);
    
    //更新奖励领取状态
    void updateDefenseAward(cocos2d::Ref* pTarget,cocos2d::SEL_CallFuncO phandler, int videoId, int stamina);
    
    //更新矿场的生产速率     //最后一个参数表示矿厂的生产上限
    void updateKuangChangRate(cocos2d::Ref *pTarget,cocos2d::SEL_CallFuncO phandler, std::string identify, int iAddStone, int maxAddStone);
    
    //采集矿石
    void collectStone(cocos2d::Ref* pTarget, cocos2d::SEL_CallFuncO phander, std::string identify);
    
    //开始熔炼
    void startProduct(cocos2d::Ref* pTarget, cocos2d::SEL_CallFuncO phander, std::string identify, int productNum, int productTime);
    
    //判断是否熔炼结束
    void isEndProduct(cocos2d::Ref* pTarget, cocos2d::SEL_CallFuncO phander, std::string identify);
    
    //更新玩家矿石上限
    void updateKuangshiLimit(cocos2d::Ref* pTarget, cocos2d::SEL_CallFuncO phander, int maxLimit);
    
    //更新玩家在线时间
    void updateUserOnLineTime(cocos2d::Ref* pTarget, cocos2d::SEL_CallFuncO phander);
    
    //更新进化标识
    void updateUserEvolution(cocos2d::Ref* pTarget, cocos2d::SEL_CallFuncO phander);
    
    //更新玩家保护时间
    void updateUserProtectTime(cocos2d::Ref* pTarget, cocos2d::SEL_CallFuncO phander);
    
    void UpdateKuanshiNum(cocos2d::Ref *pTarget,cocos2d::SEL_CallFuncO phandler,int KuanshiAdd);
    
    
    //获取抽卡信息
    void GetDrawCardInfo(cocos2d::Ref * pTarget,cocos2d::SEL_CallFuncO phandler);
    
    void SaveDrawCardInfo(cocos2d::Ref * pTarget,cocos2d::SEL_CallFuncO phandler,int iType,int iDrawNum);
    
    
    
    
    /*
     debug用
     */
    void saveTheMap(cocos2d::Ref * pTarget,cocos2d::SEL_CallFuncO phandler);//
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
///pvp
};

#endif /* defined(__GAME2__zzbNetControlModule__) */
