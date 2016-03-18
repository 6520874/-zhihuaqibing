//
//  zzbNewNetStruct.h
//  GAME2
//
//   Created by spf on 14-7-21
//
//

#ifndef __GAME2__zzbNewNetStruct__
#define __GAME2__zzbNewNetStruct__

#include <iostream>
#include "cocos2d.h"

#include "LYPersonalData.h"
#include "LayerManager.h"
#include "DataManager.h"

#include "zzbNetControlModule.h"
#include "PublicDefine.h"

#include "LYFightManager.h"


#include "LoginLayer.h"

#include "CHitDialog.h"
#include "NetWorkDataSingle.h"
#include "LYFightData.h"

//pvp
#include "DBMgr.h"
#include "LYBaseTower.h"
#include "LYFightData.h"


//((const char*){CWebsiteInfoPro::sharedWebsiteInfoProcess()->GetWebFromId(1).c_str()})//服务器地址

USING_NS_CC;

typedef struct netNewEventNameInf
{
    int m_eventType;
    char m_eventName[64];
    char m_eventHead[24];
    char m_eventCName[64];//hsx 请求名字
}NETNEWEVENTNAMEINF;
/*
 net define
 */
enum
{
    NET_RETURN_SUCC = 0,
    
    NET_RETURN_FAILER = 1
};
enum
{
    //    NET_EVENT_DRAWCARD,//hsx 抽卡 20140331
    
    NET_EVENT_VERSION,
    
    NET_EVENT_NEWUSER,
    
    NET_EVENT_LOGIN,//登入
    
    NET_EVENT_UPDATEUSERNAME, //更新用户名
    
    NET_EVENT_SUGGES,//意见箱
    
    NET_EVENT_SAVEMCF,//保存金币魔法石
    
    NET_EVENT_GET_CHARGE,//获取充值信息
    
    NET_EVENT_GET_ENEMY_TEAM,//获取目标玩家阵营信息
    
    NET_EVENT_GET_RANDOM_ENEMY_TEAM,//随机获取目标玩家阵营信息
    
    NET_EVENT_GET_USER_TEAM,//获取玩家阵营信息
    
    NET_EVENT_SET_USER_TEAM,//保存玩家阵营信息
    
    NET_EVENT_UPLOAD_VIDEO,//上传玩家对战录像
    
    NET_EVENT_GET_DEFENSE_VIDEOLIST,//获取玩家防守录像目录
    
    NET_EVENT_GET_DEFENSE_VIDEO,//获取玩家防守录像
    
    NET_EVENT_UPDATE_AWARD_STATE,//更新防守成功奖励
    
    NET_EVENT_UPDATE_STONE_RATE,//更新矿石生产速率
    
    NET_EVENT_COLLECT_STONE,//收集矿石
    
    NET_EVENT_UPDATE_KUANSHI_NUM,//更新矿石量
    
    NET_EVENT_START_PRODUCT,//开始熔炼
    
    NET_EVENT_IS_END_PRODUCT,//是否熔炼结束
    
    NET_EVENT_UPDATE_KUANGSHI,//更新矿石上限
    
    NET_EVENT_UPDATE_ONLINE_TIME,//更新在线时间
    
    NET_EVENT_UPDATE_EVOLUTION,//更新进化标识
    
    NET_EVENT_UPDATE_PROTECT_TIME,//更新玩家保护时间
    
    NET_EVENT_GET_TIME,//获取服务器时间
    
    NET_EVENT_SEND_BUY,//发送购买回调给服务器
    
    NET_EVENT_BUYSHOP_PROPONLY, //标记商店道具已被购买
    NET_EVENT_ISSHOP_PROPONLY,  //判断商店道具是否被购买
    
    NET_EVENT_GET_DRAWCARDINFO,//获取抽卡信息
    
    NET_EVENT_SAVE_DRAWCARDINFO,//保存抽卡信息
    
    NET_EVENT_SAVE_BOOT_MAPINFO,//保存机器人地图信息

    
    NET_EVENT_BORN_TIME, //borntime
     NET_NEW_EVENT_NR,
    
};

struct _netNewEventStructBase:public Ref
{
    int m_eventType;
    virtual bool execute(){return false;}
};

struct _netNewSendEventBase : public _netNewEventStructBase{
    //生成需要发送的字段,头和尾巴都不需要生成
    virtual std::string generateContentStr()=0;
    std::string getContentStr();//重新拼接
    //发送的完整消息
    std::string generateSendStr();
    
    std::string postData;
};


//接收消息的基类
struct _netNewRecvEventBase : public _netNewEventStructBase
{
    _netNewRecvEventBase():m_retCode(NET_RETURN_SUCC){}
    static _netNewRecvEventBase* create(const int& nEventType,const std::string&receiveCommand);
    
    
    static std::string truncateCommandHeaderStr(std::string commanstr);//去掉问号之前的东西
    
    virtual bool init(){
        m_retCode = NET_RETURN_SUCC;
        if(m_paramsStr.size() <= 0)
            return true;
        if((m_paramsStr[0].find("info=") == -1))
            return true;
        m_retCode = parseNumberAfterEqualFromInstruction(m_paramsStr[0]);
        return isRight();
    }
    
    virtual bool isRight(){
        return m_retCode == NET_RETURN_SUCC;
    }
    
    
    static std::string parseStrAfterEqualFromInstruction(const std::string&str);
    static int parseNumberAfterEqualFromInstruction(const std::string&useridStr);
    static long parseLongNumberAfterEqualFromInstruction(const std::string&useridStr);
    static unsigned long long parseLongLongNumberAfterEqualFromInstruction(const std::string&useridStr);
    static float parseFloatAfterEqualFromInstruction(const std::string&useridStr);
    std::string parseHasEqualInstructionKeyname(const std::string& key);
    
    void parseAllParams(const std::string&str);
    // std::map<std::string, std::string> m_paramsStr;
    std::vector<std::string> m_paramsStr;
    int m_retCode;
};

struct _netSendEventBuyInfo: public _netNewSendEventBase
{
    virtual std::string generateContentStr()
    {
        std::string buf = "";
        
        return buf;
    }
    std::string tempStr;
};

struct _netRecvEventGetBuyInfo: public _netNewRecvEventBase
{
    bool init()
    {
        if (!_netNewRecvEventBase::init())
            return false;
        return true;
    }
};

struct _netSendEventSetUserTeamInfo: public _netNewSendEventBase
{
    virtual std::string generateContentStr()
    {
        
        __String* buf = __String::createWithFormat("userId=%d&authCode=%s&Map=%s&level=%d", CPersonalData::getInstance()->getUserInfo()->m_iUserId, CPersonalData::getInstance()->getUserInfo()->m_sAuthCode.c_str(), teamInfo.c_str(), level);
        
        return buf->getCString();
    }
    std::string teamInfo;
    int level;
};

struct _netRecvEventSetUserTeamInfo:public _netNewRecvEventBase
{
    bool init()
    {
        if (!_netNewRecvEventBase::init())
            return false;
        return true;
    }
};

struct _netSendEventGetUserTeamInfo: public _netNewSendEventBase
{
    virtual std::string generateContentStr()
    {
        
        __String* buf = __String::createWithFormat("userId=%d&authCode=%s", CPersonalData::getInstance()->getUserInfo()->m_iUserId, CPersonalData::getInstance()->getUserInfo()->m_sAuthCode.c_str());
        
        return buf->getCString();
    }
};

struct _netRecvEventGetUserTeamInfo:public _netNewRecvEventBase
{
    bool init()
    {
        if (!_netNewRecvEventBase::init())
            return false;
        pvpCommonData::getInstance()->parseUserTeamInfo(m_paramsStr);
        return true;
    }
};

struct _netSendEventGetEnemyTeamInfo: public _netNewSendEventBase
{
    virtual std::string generateContentStr()
    {
        
        __String* buf = __String::createWithFormat("userId=%d", userId);
        
        return buf->getCString();
    }
    int userId;
};

struct _netRecvEventGetEnemyTeamInfo:public _netNewRecvEventBase
{
    bool init()
    {
        if (!_netNewRecvEventBase::init())
            return false;
        return true;
    }
};

struct _netSendEventGetRandomEnemyTeamInfo: public _netNewSendEventBase
{
    virtual std::string generateContentStr()
    {
        
        __String* buf = __String::createWithFormat("userId=%d&authCode=%s&level=%d&iHaveUser=%s", CPersonalData::getInstance()->getUserInfo()->m_iUserId,
                                                   CPersonalData::getInstance()->getUserInfo()->m_sAuthCode.c_str(), level, content.c_str());
        
        return buf->getCString();
    }
    int level;
    std::string content;
};

struct _netRecvEventGetRandomEnemyTeamInfo:public _netNewRecvEventBase
{
    bool init()
    {
        if (!_netNewRecvEventBase::init())
            return false;
        return true;
    }
};

struct _netSendEventUploadVideoInfo: public _netNewSendEventBase
{
    virtual std::string generateContentStr()
    {
        //
        
        __String* buf = __String::createWithFormat("userId=%d&defenseMap=%s&attackTeam=%s&enemyInfo=%s&enemySoldier=%s&robStone=%d&videoInfo=%s&isWin=%d&destroyDegree=%d", userId, teamInfo.c_str(), attackTeam.c_str(), enemyInfo.c_str(), enemySolider.c_str(), stone, video.c_str(),iSuccess, percent);
        
        return buf->getCString();
    }
    int userId;
    std::string teamInfo;
    std::string attackTeam;
    std::string enemyInfo;
    std::string enemySolider;
    int stone;
    std::string video;  //废弃，不能使用
    int iSuccess;
    int percent;
};

struct _netRecvEventUploadVideoInfo:public _netNewRecvEventBase
{
    bool init()
    {
        if (!_netNewRecvEventBase::init())
            return false;
        return true;
    }
};

struct _netSendEventGetDefenseVideoListInfo: public _netNewSendEventBase
{
    virtual std::string generateContentStr()
    {
        
        __String* buf = __String::createWithFormat("userId=%d", CPersonalData::getInstance()->getUserInfo()->m_iUserId);
        
        return buf->getCString();
    }
};

struct _netRecvEventGetDefenseVideoListInfo:public _netNewRecvEventBase
{
    bool init()
    {
        if (!_netNewRecvEventBase::init())
            return false;
        return true;
    }
};

struct _netSendEventGetDefenseVideoInfo: public _netNewSendEventBase
{
    virtual std::string generateContentStr()
    {
        
        __String* buf = __String::createWithFormat("userId=%d&videoId=%d", CPersonalData::getInstance()->getUserInfo()->m_iUserId, videoId);
        
        return buf->getCString();
    }
    int videoId;
};

struct _netRecvEventGetDefenseVideoInfo:public _netNewRecvEventBase
{
    bool init()
    {
        if (!_netNewRecvEventBase::init())
            return false;
        return true;
    }
};

struct _netSendEventUpdateAwardState: public _netNewSendEventBase
{
    virtual std::string generateContentStr()
    {
        __String* buf = __String::createWithFormat("userId=%d&videoId=%d&stamin=%d", CPersonalData::getInstance()->getUserInfo()->m_iUserId,
                                                   videoId, stamina);
        return buf->getCString();
    }
    int videoId;
    int stamina;
};

struct _netRecvEventUpdateAwardState:public _netNewRecvEventBase
{
    bool init()
    {
        if (!_netNewRecvEventBase::init())
            return false;
        
        
        int index = 0;
        int state = _netNewRecvEventBase::parseNumberAfterEqualFromInstruction(m_paramsStr[index++]);
        CPersonalData::getInstance()->setState(state);
        long iStamina = _netNewRecvEventBase::parseLongNumberAfterEqualFromInstruction(m_paramsStr[index++]);
        CPersonalData::getInstance()->SetPhysicalNum(iStamina);
        CPersonalData::getInstance()->setUpdateUserInfo(true);
        return true;
    }
};

struct _netSendEventUpdateKuangChang: public _netNewSendEventBase
{
    virtual std::string generateContentStr()
    {
        __String* buf = __String::createWithFormat("userId=%d&authCode=%s&userBuildId=%s&stoneRate=%d&maxAdd=%d", CPersonalData::getInstance()->getUserInfo()->m_iUserId,CPersonalData::getInstance()->getUserInfo()->m_sAuthCode.c_str(),identify.c_str(), iAddStone,maxAddStone);
        return buf->getCString();
    }
    std::string identify;
    int iAddStone;
    int maxAddStone;
};


struct _netRecvEventUpdateKuangChang:public _netNewRecvEventBase
{
    bool init()
    {
        if (!_netNewRecvEventBase::init())
            return false;
        int index = 0;
        int state = _netNewRecvEventBase::parseNumberAfterEqualFromInstruction(m_paramsStr[index++]);
        if(state != 0)
        {
            log("更新矿场矿石生产速率失败");
        }
        return true;
    }
};

struct _netSendEventCollectStone: public _netNewSendEventBase
{
    virtual std::string generateContentStr()
    {
        __String* buf = __String::createWithFormat("authCode=%s&userId=%d&userBuildId=%s", CPersonalData::getInstance()->getUserInfo()->m_sAuthCode.c_str(),CPersonalData::getInstance()->getUserInfo()->m_iUserId, identify.c_str());
        return buf->getCString();
    }
    std::string identify;
};

struct _netRecvEventCollectStone:public _netNewRecvEventBase
{
    bool init()
    {
        if (!_netNewRecvEventBase::init())
            return false;
        int index = 0;
        int state = _netNewRecvEventBase::parseNumberAfterEqualFromInstruction(m_paramsStr[index++]);
        CPersonalData::getInstance()->setState(state);
        return true;
    }
};

struct _netSendEventStartProduct: public _netNewSendEventBase
{
    virtual std::string generateContentStr()
    {
        __String* buf = __String::createWithFormat("userId=%d&authCode=%s&userBuildId=%s&productNum=%d&productTime=%d",
                                                   CPersonalData::getInstance()->getUserInfo()->m_iUserId,CPersonalData::getInstance()->getUserInfo()->m_sAuthCode.c_str(), identify.c_str(),productNum, productTime);
        return buf->getCString();
    }
    std::string identify;
    int productNum;
    int productTime;
};

struct _netRecvEventStartProduct:public _netNewRecvEventBase
{
    bool init()
    {
        if (!_netNewRecvEventBase::init())
            return false;
        int index = 0;
        int state = _netNewRecvEventBase::parseNumberAfterEqualFromInstruction(m_paramsStr[index++]);
        CPersonalData::getInstance()->setState(state);
        return true;
    }
};

struct _netSendEventIsEndProduct: public _netNewSendEventBase
{
    virtual std::string generateContentStr()
    {
        __String* buf = __String::createWithFormat("userId=%d&authCode=%s&userBuildId=%s", CPersonalData::getInstance()->getUserInfo()->m_iUserId,CPersonalData::getInstance()->getUserInfo()->m_sAuthCode.c_str(), identify.c_str());
        return buf->getCString();
    }
    std::string identify;
};

struct _netRecvEventIsEndProduct:public _netNewRecvEventBase
{
    bool init()
    {
        if (!_netNewRecvEventBase::init())
            return false;
        return true;
    }
};

struct _netSendEventUpdateKuangshiLimit: public _netNewSendEventBase
{
    virtual std::string generateContentStr()
    {
        __String* buf = __String::createWithFormat("userId=%d&authCode=%s&maxLimit=%d", CPersonalData::getInstance()->getUserInfo()->m_iUserId,CPersonalData::getInstance()->getUserInfo()->m_sAuthCode.c_str(), maxLimit);
        return buf->getCString();
    }
    int maxLimit;
};

struct _netRecvEventUpdateKuangshiLimit:public _netNewRecvEventBase
{
    bool init()
    {
        if (!_netNewRecvEventBase::init())
            return false;
        int index = 0;
        CPersonalData::getInstance()->getUserInfo()->serverLogintime = _netNewRecvEventBase::parseLongLongNumberAfterEqualFromInstruction(m_paramsStr[index++]);
        int state = _netNewRecvEventBase::parseNumberAfterEqualFromInstruction(m_paramsStr[index++]);
        if(state == 0)
        {
            CPersonalData::getInstance()->setUpdateUserInfo(true);
        }
        else
        {
            log("更新矿石上限失败");
        }
        return true;
    }
};

struct _netSendEventUpdateOnLineTime: public _netNewSendEventBase
{
    virtual std::string generateContentStr()
    {
        __String* buf = __String::createWithFormat("userId=%d&authCode=%s", CPersonalData::getInstance()->getUserInfo()->m_iUserId,
                                                   CPersonalData::getInstance()->getUserInfo()->m_sAuthCode.c_str());
        return buf->getCString();
    }
};

struct _netRecvEventUpdateOnLineTime:public _netNewRecvEventBase
{
    bool init()
    {
        if (!_netNewRecvEventBase::init())
            return false;
        return true;
    }
};

struct _netSendEventUpdateEvolution: public _netNewSendEventBase
{
    virtual std::string generateContentStr()
    {
        __String* buf = __String::createWithFormat("userId=%d&authCode=%s", CPersonalData::getInstance()->getUserInfo()->m_iUserId,
                                                   CPersonalData::getInstance()->getUserInfo()->m_sAuthCode.c_str());
        return buf->getCString();
    }
};

struct _netRecvEventUpdateEvolution:public _netNewRecvEventBase
{
    bool init()
    {
        if (!_netNewRecvEventBase::init())
            return false;
        
        int index = 0;
        int state = _netNewRecvEventBase::parseNumberAfterEqualFromInstruction(m_paramsStr[index++]);
        if(state == 0)
        {
            pvpCommonData::getInstance()->setEvolution(1);
        }
        return true;
    }
};

struct _netSendEventUpdateProtectTime: public _netNewSendEventBase
{
    virtual std::string generateContentStr()
    {
        __String* buf = __String::createWithFormat("userId=%d&authCode=%s", CPersonalData::getInstance()->getUserInfo()->m_iUserId,
                                                   CPersonalData::getInstance()->getUserInfo()->m_sAuthCode.c_str());
        return buf->getCString();
    }
};

struct _netRecvEventUpdateProtectTime:public _netNewRecvEventBase
{
    bool init()
    {
        if (!_netNewRecvEventBase::init())
            return false;
        return true;
    }
};

struct _netSendEventGetChargeInfo: public _netNewSendEventBase
{
    virtual std::string generateContentStr()
    {
        
        __String* buf = __String::createWithFormat("userId=%d&authCode=%s", CPersonalData::getInstance()->getUserInfo()->m_iUserId, CPersonalData::getInstance()->getUserInfo()->m_sAuthCode.c_str());
        
        return buf->getCString();
    }
};

struct _netRecvEventGetChargeInfo:public _netNewRecvEventBase
{
    bool init()
    {
        if (!_netNewRecvEventBase::init())
            return false;
        return true;
    }
};



struct _netRecvEventBuyShopInfo:public _netNewRecvEventBase
{
    bool init()
    {
        if (!_netNewRecvEventBase::init())
            return false;
        return true;
    }

};


struct _netRecvEventIsShopPropBuy:public _netNewRecvEventBase
{

    bool init()
    {
        if (!_netNewRecvEventBase::init())
            return false;
        return true;
    }
};



//获取服务器时间
struct _netSendEventGetSevTime: public _netNewSendEventBase
{
    virtual std::string generateContentStr()
    {
        std::string buf = "";
        
        return buf;
    }
};

struct _netRecvEventGetSevTime:public _netNewRecvEventBase
{
    bool init()
    {
        if (!_netNewRecvEventBase::init())
            return false;
        
        //解析时间
        int index = 0;
        CPersonalData::getInstance()->getUserInfo()->serverLogintime = _netNewRecvEventBase::parseLongLongNumberAfterEqualFromInstruction(m_paramsStr[index++]);
        int state = _netNewRecvEventBase::parseNumberAfterEqualFromInstruction(m_paramsStr[index++]);
        CPersonalData::getInstance()->setState(state);
        return true;
    }
};

struct _netSendEventNewUser: public _netNewSendEventBase
{
    virtual std::string generateContentStr()
    {
        
        std::string buf = "";
        
        return buf;
        
        // sprintf(buffer, "uin=91&sessionId=%s&type=app&platform=AppStore&version=1.0","FCCDC380D169CA4501BC32944BC294FC");
        
    }
};

struct _netRecvEventNewUser :public _netNewRecvEventBase
{
    
    virtual bool init()
    {
        if (!_netNewRecvEventBase::init())
        {
            return false;
        }
        
        int index = 1;
        
        if (m_paramsStr.size()<7) {
            CCASSERT(0, "LYAssert wrong response ----_netRecvEventNewUser");
        }
        
        CPersonalData::getInstance()->getUserInfo()->m_iUserId = _netNewRecvEventBase::parseNumberAfterEqualFromInstruction(m_paramsStr[index++]);
        CPersonalData::getInstance()->getUserInfo()->m_sAuthCode = _netNewRecvEventBase::parseStrAfterEqualFromInstruction(m_paramsStr[index++]);
        
        CPersonalData::getInstance()->getUserInfo()->serverLogintime = _netNewRecvEventBase::parseLongLongNumberAfterEqualFromInstruction(m_paramsStr[index++]);
        
        long iCoin = _netNewRecvEventBase::parseLongNumberAfterEqualFromInstruction(m_paramsStr[index++]);
        
        long iMagic = _netNewRecvEventBase::parseLongNumberAfterEqualFromInstruction(m_paramsStr[index++]);
        
        long iStamina = _netNewRecvEventBase::parseLongNumberAfterEqualFromInstruction(m_paramsStr[index++]);
        
        
        CPersonalData::getInstance()->setUserCoin(iCoin);
        
        CPersonalData::getInstance()->setUserStone(iMagic);
        
        CPersonalData::getInstance()->SetPhysicalNum(iStamina);
        
        
        __String* loginStr = __String::createWithFormat("%ld",CPersonalData::getInstance()->getUserInfo()->serverLogintime/1000);
        
        CDataManager::saveUserLoginTime(loginStr->getCString());
        CDataManager::saveUpdateDailyaTaskTime(loginStr->getCString());//纪录刷新时间
        //写入数据库
        CDataManager::setUserIdAndAuthCode();
        
        
        //talkingData计数
        __String * tempStr = __String::createWithFormat("%d",CPersonalData::getInstance()->getUserInfo()->m_iUserId);
        TDCCAccount* account = TDCCAccount::setAccount(tempStr->getCString());
        account->setAccountType(TDCCAccount::kAccountAnonymous);
        account->setLevel(CPersonalData::getInstance()->getUserInfo()->m_iUserLevel);
        
        
        
        if(CPersonalData::getInstance()->getUserInfo()->m_iSmallLevel <= 0)
        {
            auto scene = CLoginLayer::createScene();
            //_director->replaceScene(scene);
            
            auto transition= TransitionFade::create(1.0f,scene);//给场景包装动画特效
            Director::getInstance()->replaceScene(transition);//运用导演类来进行切换场景
            //pageTransitionForward
            
        }else{
            //跳转主界面
            CLayerManager::GetSingletonPtr()->createLayer(eLayer_MainMenu, NULL,true);
        }
        
        
        return true;
    }
    
};



struct _netSendEventPropHaveBuy:public _netNewSendEventBase
{
    virtual std::string generateContentStr()
    {
        
        __String *tempStr = __String::createWithFormat("userid=%d&authCode=%s&propId=%d&buy=true",CPersonalData::getInstance()->getUserInfo()->m_iUserId,CPersonalData::getInstance()->getUserInfo()->m_sAuthCode.c_str(),m_iPropId);
        
        return tempStr->getCString();
    }

    int m_iPropId;

};


struct _netSendEventISPropHaveBuy:public  _netNewSendEventBase
{
    virtual std::string generateContentStr()
    {
        
        __String *tempStr = __String::createWithFormat("userid=%d&authCode=%s&propId=%d",CPersonalData::getInstance()->getUserInfo()->m_iUserId,CPersonalData::getInstance()->getUserInfo()->m_sAuthCode.c_str(),m_iPropId);
        
        return tempStr->getCString();
    }
    
    int m_iPropId;
};


struct _netSendEventMCF:public _netNewSendEventBase
{
    virtual std::string generateContentStr()
    {
        
        __String *tempStr = __String::createWithFormat("userid=%d&authCode=%s&coin=%d&magic=%d&stamin=%d",CPersonalData::getInstance()->getUserInfo()->m_iUserId,CPersonalData::getInstance()->getUserInfo()->m_sAuthCode.c_str(),m_iCoinAdd,m_iMagicAdd,m_iStaminaAdd);
        
        return tempStr->getCString();
    }
    
    int m_iCoinAdd;
    int m_iMagicAdd;
    int m_iStaminaAdd;
    
};


struct _netRecvEventMCF :public _netNewRecvEventBase
{
    bool init()
    {
        if(!_netNewRecvEventBase::init())
            return false;
        int index = 1;
        
        
        if (m_paramsStr.size()<4) {
            CCASSERT(0, "LYAssert wrong response ----_netRecvEventLogin");
        }
        
        int state = _netNewRecvEventBase::parseNumberAfterEqualFromInstruction(m_paramsStr[index++]);
        
        long iCoin = _netNewRecvEventBase::parseLongNumberAfterEqualFromInstruction(m_paramsStr[index++]);
        
        long iMagic = _netNewRecvEventBase::parseLongNumberAfterEqualFromInstruction(m_paramsStr[index++]);
        
        long iStamina = _netNewRecvEventBase::parseLongNumberAfterEqualFromInstruction(m_paramsStr[index++]);
        
        CPersonalData::getInstance()->setUserCoin(iCoin);
        
        CPersonalData::getInstance()->setUserStone(iMagic);
        
        CPersonalData::getInstance()->SetPhysicalNum(iStamina);
        
        CPersonalData::getInstance()->setUpdateUserInfo(true);
        
        return true;
    }
};


struct _netSendEventUpdateUserName:public _netNewSendEventBase
{

    virtual std::string generateContentStr()
    {
        
        std::string gb2312Code = CNetControlModule::encode(m_strUserName);   //编码
        
        
        __String *buf = __String::createWithFormat("userId=%d&authCode=%s&name=%s",CPersonalData::getInstance()->getUserInfo()->m_iUserId, CPersonalData::getInstance()->getUserInfo()->m_sAuthCode.c_str(),gb2312Code.c_str());
         return buf->getCString();
    

    }
    
    std::string m_strUserName;
    
};


struct _netRecvEventUpdateUserName:public _netNewRecvEventBase
{
        bool init()
        {
            if(!_netNewRecvEventBase::init())
                return false;
    

            
            return true;
        }
        

};



struct _netSendEventDingshiTime:public _netNewSendEventBase
{

    virtual std::string generateContentStr()
    {

        __String *tempStr = __String::createWithFormat("userid=%d&authCode=%s&userBuildingId=%d&Time=%d",CPersonalData::getInstance()->getUserInfo()->m_iUserId,CPersonalData::getInstance()->getUserInfo()->m_sAuthCode.c_str(),m_iBuildingId,m_iTime);
        
        return tempStr->getCString();
        
    }
    
    int m_iBuildingId;
    int m_iTime;
};



struct _netRecvEventDingshiTime:public _netNewRecvEventBase
{

    bool init()
    {
        if(!_netNewRecvEventBase::init())
            return false;
       // int index = 1;
    
        
        return true;
    }

};






struct _netSendEventLogin: public _netNewSendEventBase
{
    virtual std::string generateContentStr()
    {
        //app版 uin要小于零sessionId 传的是验证码
        //其他平台需要另外添加
        
        __String* buf = __String::createWithFormat("userId=%d&authCode=%s", CPersonalData::getInstance()->getUserInfo()->m_iUserId, CPersonalData::getInstance()->getUserInfo()->m_sAuthCode.c_str());
        
        return buf->getCString();
        
    }
};

struct _netRecvEventLogin :public _netNewRecvEventBase
{
    bool init()
    {
        if(!_netNewRecvEventBase::init())
            return false;
        int index = 1;
        
        
        if (m_paramsStr.size()<5) {
            
            Sprite * thetarget = CFightManager::getTheFlyWord("该用户不存在,请联系客服QQ群152768414","fonts/cuti.ttf");
            
    
           Scene* p =  Director::getInstance()->getRunningScene();
            
            
            cocos2d::Size winSize = Director::getInstance()->getWinSize();
            
            
            thetarget->setPosition(winSize.width*0.5,winSize.height*0.5);
            
         
            p->addChild(thetarget,10);
           
            return false;
            
           // CCASSERT(0, "LYAssert wrong response ----_netRecvEventLogin");
        }
        
        
        int state = _netNewRecvEventBase::parseNumberAfterEqualFromInstruction(m_paramsStr[index++]);
        
        CPersonalData::getInstance()->getUserInfo()->serverLogintime = _netNewRecvEventBase::parseLongLongNumberAfterEqualFromInstruction(m_paramsStr[index++]);
        
        long iCoin = _netNewRecvEventBase::parseLongNumberAfterEqualFromInstruction(m_paramsStr[index++]);
        
        long iMagic = _netNewRecvEventBase::parseLongNumberAfterEqualFromInstruction(m_paramsStr[index++]);
        
        long iStamina = _netNewRecvEventBase::parseLongNumberAfterEqualFromInstruction(m_paramsStr[index++]);
        
        
        //刷新本地数据库金币，魔法石，体力
        CPersonalData::getInstance()->setUserCoin(iCoin);
        
        CPersonalData::getInstance()->setUserStone(iMagic);
        
        CPersonalData::getInstance()->SetPhysicalNum(iStamina);
        
        //登陆后判断充值金币兑换次数
        CPersonalData::getInstance()->detectExchangeNum(CPersonalData::getInstance()->getUserInfo()->serverLogintime/1000);
       // CPersonalData::getInstance()->DrawCardRefresh(CPersonalData::getInstance()->getUserInfo()->serverLogintime/1000);
        CPersonalData::getInstance()->detectTaskRefresh(CPersonalData::getInstance()->getUserInfo()->serverLogintime/1000);
        
        if (!state) {
            
            //登陆成功
            //talkingData计数
            __String* tempStr = __String::createWithFormat("%d",CPersonalData::getInstance()->getUserInfo()->m_iUserId);
            TDCCAccount* account = TDCCAccount::setAccount(tempStr->getCString());
            account->setAccountType(TDCCAccount::kAccountAnonymous);
            account->setLevel(CPersonalData::getInstance()->getUserInfo()->m_iUserLevel);
            
            //跳转主界面
            //CPersonalData::getInstance()->getUserInfo()->m_iSmallLevel<=
            if(CPersonalData::getInstance()->getUserInfo()->m_iSmallLevel <= 0)
            {
                auto scene = CLoginLayer::createScene();
                //_director->replaceScene(scene);
                
                auto transition= TransitionFade::create(1.0f,scene);//给场景包装动画特效
                Director::getInstance()->replaceScene(transition);//运用导演类来进行切换场景
                //pageTransitionForward
                
            }else{
                CLayerManager::GetSingletonPtr()->createLayer(eLayer_MainMenu, NULL,true);
            }
            
        }else{
            
            //登陆失败
            CNetControlModule::sharedNetControlModule()->removeNetDialog();
            
        }
        
        
        return true;
    }
};

struct _netSendEventVersionJudge: public _netNewSendEventBase
{
    virtual std::string generateContentStr()
    {
        
        __String *tempStr = __String::createWithFormat("versionId=%s",CLINENT_VERSION);
        
        return tempStr->getCString();
        
        
        // sprintf(buffer, "uin=91&sessionId=%s&type=app&platform=AppStore&version=1.0","FCCDC380D169CA4501BC32944BC294FC");
        
    }
};

struct _netRecvEventVersionJudge :public _netNewRecvEventBase
{
    bool init()
    {
        if(!_netNewRecvEventBase::init())
            return false;
        int index = 1;
        
        
        if (m_paramsStr.size()<3) {
            CCASSERT(0, "LYAssert wrong response ----_netRecvEventVersionJudge");
        }
        
        
        int state = _netNewRecvEventBase::parseNumberAfterEqualFromInstruction(m_paramsStr[index++]);
        
        
        std::string URLStr = _netNewRecvEventBase::parseStrAfterEqualFromInstruction(m_paramsStr[index++]);
        
        
        cocos2d::Size winsize = Director::getInstance()->getWinSize();
        
        
        
        switch (state) {
            case 1:
            {
                //版本号高
                CCLOG("版本号太低");
                
                
                //调用跳转界面
                
                CHitDialog* temp = CHitDialog::create();
                
                auto target = CNetControlModule::sharedNetControlModule();
                
                //                menu_selector(CNetControlModule::gameOver);
                
                temp->CreateUrlDialog("您的版本号太低，是否更新", URLStr,target,NULL,menu_selector(CNetControlModule::gameOver));
                Director::getInstance()->getRunningScene()->addChild(temp,9999999);
                
                //                Sprite* temp =  CFightManager::getTheFlyWord("你的版本号太低");
                //
                //                temp->setPosition(Vec2(winsize.width*0.5f, winsize.height*0.5f));
                //
                //                Director::getInstance()->getRunningScene()->addChild(temp,9999999);
                
                break;
            }
            case 0:
            {
                
                //版本号匹配
                CCLOG("版本号相同");
                
                //                Sprite* temp =  CFightManager::getTheFlyWord("版本匹配成功");
                //
                //
                //                temp->setPosition(Vec2(winsize.width*0.5f, winsize.height*0.5f));
                //                Director::getInstance()->getRunningScene()->addChild(temp,9999999);
                
                
                break;
            }
            case -1:
            {
                //版本号低
                CCLOG("版本号太高");
                
                
                
                //调用跳转界面
                CHitDialog* temp = CHitDialog::create();
                
                auto target = CNetControlModule::sharedNetControlModule();
                temp->CreateUrlDialog("您的版本号太高，是否更新", URLStr,target,NULL,menu_selector(CNetControlModule::gameOver));
                Director::getInstance()->getRunningScene()->addChild(temp,9999999);
                
                
                //                Sprite* temp =  CFightManager::getTheFlyWord("你的版本号太高");
                //
                //                temp->setPosition(Vec2(winsize.width*0.5f, winsize.height*0.5f));
                //                Director::getInstance()->getRunningScene()->addChild(temp,9999999);
                
                break;
            }
                
            default:
                break;
        }
        
        
        //测试用，查看当前客户端数据库版本号
        //        __String *tempStr = __String::createWithFormat("客户端数据库版本号%f",CPersonalData::getInstance()->getUserInfo()->DBVersion);
        //        Sprite* temp = CFightManager::getTheFlyWord(tempStr->getCString());
        //        temp->setPosition(winsize.width*0.5f, winsize.height*0.2f);
        //        Director::getInstance()->getRunningScene()->addChild(temp,999);
        
        
        return true;
    }
};


/*
 pvp增加矿石总量
 */
struct _netSendEventKuanshi:public _netNewSendEventBase
{
    virtual std::string generateContentStr()
    {
        //app版 uin要小于零sessionId 传的是验证码
        //其他平台需要另外添加
        
        
        //&authCode=%s
        
        __String* buf = __String::createWithFormat("userId=%d&authCode=%s&kuoshiAddNum=%d", CPersonalData::getInstance()->getUserInfo()->m_iUserId,CPersonalData::getInstance()->getUserInfo()->m_sAuthCode.c_str()
,m_iAddNum);
        
        return buf->getCString();
    }
    
    int m_iAddNum;
                                                   
};


struct _netRecEventKuanshi:public _netNewRecvEventBase
{
    
    bool init()
    {
        if(!_netNewRecvEventBase::init())
            return false;
        //int index = 1;
        
        
        if (m_paramsStr.size()<1) {
            CCASSERT(0, "LYAssert wrong response ----_netRecvEventSuggestion");
        }
        
        
        
        std::string str = m_paramsStr[0];
        
        int index = str.find("ksNum=");
        
        str = str.substr(index+6,str.length());
    
        
        int iStone = atoi(str.c_str());
        
        CFightData::getInstance()->setTheKuanshiNum(iStone);

        return true;
    }

};

/*
 提交意见
 */
struct _netSendEventSuggestion: public _netNewSendEventBase
{
    
    std::string theSugeesitonContent;
    
    virtual std::string generateContentStr()
    {
        //app版 uin要小于零sessionId 传的是验证码
        //其他平台需要另外添加
        
        
        std::string gb2312Code = CNetControlModule::encode(theSugeesitonContent);   //编码
        
        __String* buf = __String::createWithFormat("userId=%d&authCode=%s&content=%s", CPersonalData::getInstance()->getUserInfo()->m_iUserId, CPersonalData::getInstance()->getUserInfo()->m_sAuthCode.c_str(),gb2312Code.c_str());
        
        
        return buf->getCString();
        
    }
};


struct _netRecvEventSuggestion :public _netNewRecvEventBase
{
    bool init()
    {
        if(!_netNewRecvEventBase::init())
            return false;
        int index = 1;
        
        
        if (m_paramsStr.size()<2) {
            CCASSERT(0, "LYAssert wrong response ----_netRecvEventSuggestion");
        }
        
        
        int state = _netNewRecvEventBase::parseNumberAfterEqualFromInstruction(m_paramsStr[index++]);
        
        if (!state) {
            
            cocos2d::Size winsize = Director::getInstance()->getWinSize();
            
            Sprite* temp =  CFightManager::getTheFlyWord("提交成功");
            
            temp->setPosition(Vec2(winsize.width*0.5f, winsize.height*0.5f));
            Director::getInstance()->getRunningScene()->addChild(temp,9999999);
            
        }else{
            
            //登陆失败
            CNetControlModule::sharedNetControlModule()->removeNetDialog();
            
        }
        
        
        return true;
    }
};



struct _netRecvEventSaveDrawCard:public _netNewRecvEventBase
{
    bool init()
    {
        if(!_netNewRecvEventBase::init())
            return false;
  
        /*if (m_paramsStr.size()<2)
        {
            CCASSERT(0, "LYAssert wrong response ----_netRecvEventSuggestion");
        }*/
        

         return true;
    }

    
    

};


struct _netSendEventSaveDrawCard:public _netNewSendEventBase
{

    
    /*
     
     注释：type=0为金币抽卡 1 为魔法石抽卡
     drawNum =1 为抽卡次数为1直接覆盖
     coindrawtime
     */
    
    
    virtual std::string generateContentStr()
    {
        
        __String* buf = __String::createWithFormat("userId=%d&authCode=%s&type=%d&drawNum=%d", CPersonalData::getInstance()->getUserInfo()->m_iUserId, CPersonalData::getInstance()->getUserInfo()->m_sAuthCode.c_str(),m_iType,m_iDrawNum);

        return buf->getCString();
        
    }
    
    int m_iType;
    int m_iDrawNum;


};


struct _netSendEventGetDrawCard:public _netNewSendEventBase
{

    
    virtual std::string generateContentStr()
    {

        __String* buf = __String::createWithFormat("userId=%d&authCode=%s", CPersonalData::getInstance()->getUserInfo()->m_iUserId, CPersonalData::getInstance()->getUserInfo()->m_sAuthCode.c_str());
        
        return buf->getCString();
        
    }
    
};


struct _netRecvEventSetDrawCard:public _netNewRecvEventBase
{
    bool init()
    {
        if(!_netNewRecvEventBase::init())
            return false;
        int index = 0;
        
        
        if (m_paramsStr.size()<2)
        {
            CCASSERT(0, "LYAssert wrong response ----_netRecvEventSuggestion");
        }
    

        int iCoinDrawTime = _netNewRecvEventBase::parseNumberAfterEqualFromInstruction(m_paramsStr[index++]);
        
        int  iStoneDrawTime = _netNewRecvEventBase::parseLongNumberAfterEqualFromInstruction(m_paramsStr[index++]);
        
        int  iCoinDrawNum  = _netNewRecvEventBase::parseLongNumberAfterEqualFromInstruction(m_paramsStr[index++]);
        
        int  iStoneDrawNum = _netNewRecvEventBase::parseLongNumberAfterEqualFromInstruction(m_paramsStr[index++]);
        
        int iFirstDrawCoin =  _netNewRecvEventBase::parseLongNumberAfterEqualFromInstruction(m_paramsStr[index++]);
        
        int iFirstDrawStone =  _netNewRecvEventBase::parseLongNumberAfterEqualFromInstruction(m_paramsStr[index++]);

    
        
       int iServerTime=   _netNewRecvEventBase::parseLongNumberAfterEqualFromInstruction(m_paramsStr[index++]);
        
        DrawCardInfo p;
        

        p.m_iCoindrawNum = iCoinDrawNum;
        p.m_iCoindrawtime = iCoinDrawTime;
        p.m_istoneDrawNum = iStoneDrawNum;
        p.m_iStonedrawtime = iStoneDrawTime;
        p.m_iServerTime = iStoneDrawTime;
        p.m_bfirstDrawCoin = iFirstDrawCoin;
        p.m_bfirstDrawStone  = iFirstDrawStone;
        p.m_iServerTime = iServerTime;
        
    
       CFightData::getInstance()->setDrawCardInfo(p);
        
        
        return true;
    }

};

//用于存储机器人地图
struct _netSendEventSaveBootMap:public _netNewSendEventBase
{
    
    
    virtual std::string generateContentStr()
    {
        
        __String* buf = __String::createWithFormat("userId=%d", CPersonalData::getInstance()->getUserInfo()->m_iUserId);
        
        return buf->getCString();
        
    }
    
};

struct _netRecvEventSaveBootMap:public _netNewRecvEventBase
{
    bool init()
    {
        if(!_netNewRecvEventBase::init())
            return false;
        
        cocos2d::Size winsize = Director::getInstance()->getWinSize();
        
        Sprite* temp =  CFightManager::getTheFlyWord("保存地图成功");

        temp->setPosition(Vec2(winsize.width*0.5f, winsize.height*0.5f));

        Director::getInstance()->getRunningScene()->addChild(temp,9999999);
        
        
        return true;
    }
    
    
};




#endif /* defined(__GAME2__zzbNewNetStruct__) */
