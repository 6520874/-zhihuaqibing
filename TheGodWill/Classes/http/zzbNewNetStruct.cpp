//
//  zzbNewNetStruct.cpp
//  GAME2
//
//   Created by spf on 14-7-21
//
//

#include "zzbNewNetStruct.h"



//"http://192.168.0.78:8001/"
//"http://180.166.59.147:8086/"


//HoodleServer/ass
NETNEWEVENTNAMEINF g_newEventNameInf[NET_NEW_EVENT_NR] =
{
    //    {NET_EVENT_DRAWCARD,"c=Card&m=takeOutCards"," "},
    {NET_EVENT_LOGIN,"login","WarOfGod","登入"},
    {NET_EVENT_NEWUSER,"createUserId","WarOfGod","新玩家"},
    {NET_EVENT_VERSION,"versionJudge","WarOfGod","版本判断"},
    {NET_EVENT_SUGGES,"suggestion","WarOfGod","提交意见"},
    {NET_EVENT_SAVEMCF,"paramChange","WarOfGod","保存金币魔法石"},
    {NET_EVENT_GET_CHARGE,"getGoods","WarOfGod","获取充值信息"},
    {NET_EVENT_GET_TIME, "getServerTime","WarOfGod","获取服务器时间"},
    {NET_EVENT_BUYSHOP_PROPONLY,"prophavebuy","WarOfGod","购买商店道具唯一"},
    {NET_EVENT_ISSHOP_PROPONLY,"propIsbuy","WarOfGod","判断商店道具唯一"},
    {NET_EVENT_SEND_BUY,"winfo","WarOfGod","发送充值成功请求"},
    {NET_EVENT_GET_USER_TEAM,"GetCampInfo","WarOfGod","获取玩家阵营信息"},
    {NET_EVENT_SET_USER_TEAM, "SetCampInfo", "WarOfGod", "保存玩家阵营信息"},
    {NET_EVENT_GET_ENEMY_TEAM, "getEnemy", "WarOfGod", "获取目标玩家阵营信息"},
    {NET_EVENT_GET_RANDOM_ENEMY_TEAM,"getRandom","WarOfGod","随机获取目标阵营信息"},
    {NET_EVENT_UPLOAD_VIDEO,"uploadVideo","WarOfGod","上传玩家对战录像"},
    {NET_EVENT_GET_DEFENSE_VIDEOLIST,"defenseVideo","WarOfGod","获取玩家防守录像目录"},
    {NET_EVENT_GET_DEFENSE_VIDEO, "dowloadVideo", "WarOfGod", "获取玩家玩家防守录像"},
    {NET_EVENT_UPDATE_AWARD_STATE, "gvUpdate", "WarOfGod", "更新玩家防守成功获取奖励"},
    {NET_EVENT_UPDATE_STONE_RATE, "productRate", "WarOfGod", "更新矿场的生产速率"},
    {NET_EVENT_COLLECT_STONE, "collectStone", "WarOfGod", "收集矿石"},
    {NET_EVENT_START_PRODUCT, "startProduct", "WarOfGod", "开始熔炼"},
    {NET_EVENT_IS_END_PRODUCT, "isEndProduct", "WarOfGod", "是否熔炼结束"},
    {NET_EVENT_UPDATE_KUANGSHI ,"update", "WarOfGod", "更新矿石上限"},
    {NET_EVENT_UPDATE_ONLINE_TIME, "ifOnLine", "WarOfGod", "更新玩家在线时间"},
    {NET_EVENT_UPDATEUSERNAME,"setName","WarOfGod","设置姓名"},
    {NET_EVENT_UPDATE_EVOLUTION, "updatejitanstatus", "WarOfGod", "更新进化标识"},
    {NET_EVENT_UPDATE_PROTECT_TIME, "clearProtectTime", "WarOfGod", "更新玩家保护时间"},
    {NET_EVENT_UPDATE_KUANSHI_NUM,"UpdateKuanshi","WarOfGod","增加矿石"},
    {NET_EVENT_BORN_TIME,"updateBombtime","WarOfGod","炸弹爆炸时间"},
    {NET_EVENT_GET_DRAWCARDINFO,"getDrawCardInfo","WarOfGod", "获取抽卡个数"},
    {NET_EVENT_SAVE_DRAWCARDINFO,"SaveDrawCardInfo","WarOfGod","保存抽卡个数"},
    {NET_EVENT_SAVE_BOOT_MAPINFO,"SaveMapInfo","WarOfGod","保存机器人地图"}
};

const char* getNewEventNameByEventType(int eventType)
{
    for(int i = 0; i < NET_NEW_EVENT_NR; i++){
        if(g_newEventNameInf[i].m_eventType == eventType){
            return g_newEventNameInf[i].m_eventName;
        }
    }
    return NULL;
}

const char* getNewEventHeadStr(int eventType)
{
    for(int i = 0; i < NET_NEW_EVENT_NR; i++){
        if(g_newEventNameInf[i].m_eventType == eventType){
            return g_newEventNameInf[i].m_eventHead;
        }
    }
    return "";
}


_netNewRecvEventBase* _netNewRecvEventBase::create(const int& nEventType,const std::string&receiveCommand)
{
    _netNewRecvEventBase* pRecvEvent = NULL; // = new _netNewRecvEventBase();
    std::string leftcommandstr = truncateCommandHeaderStr(receiveCommand);
    switch(nEventType)
    {
        case NET_EVENT_LOGIN:
            pRecvEvent = new _netRecvEventLogin();
            break;
            
        case NET_EVENT_NEWUSER:
            pRecvEvent = new _netRecvEventNewUser();
            break;
            
        case NET_EVENT_VERSION:
            pRecvEvent = new _netRecvEventVersionJudge();
            break;
            
        case NET_EVENT_SUGGES:
            pRecvEvent = new _netRecvEventSuggestion();
            
            break;
        case NET_EVENT_UPDATE_KUANSHI_NUM:
            
            pRecvEvent = new _netRecEventKuanshi();
            break;
        case NET_EVENT_GET_DRAWCARDINFO:
            pRecvEvent = new _netRecvEventSetDrawCard();
            break;
            
        case NET_EVENT_SAVE_DRAWCARDINFO:
            pRecvEvent = new _netRecvEventSaveDrawCard();
            break;
            
        case NET_EVENT_BORN_TIME:
            pRecvEvent = new _netRecvEventDingshiTime();
            break;
            
        case NET_EVENT_GET_CHARGE:
            pRecvEvent = new _netRecvEventGetChargeInfo();
            break;
        case NET_EVENT_GET_USER_TEAM:
            pRecvEvent = new _netRecvEventGetUserTeamInfo();
            break;
        case NET_EVENT_UPDATE_EVOLUTION:
            pRecvEvent = new _netRecvEventUpdateEvolution();
            break;
        case NET_EVENT_UPDATE_PROTECT_TIME:
            pRecvEvent = new _netRecvEventUpdateProtectTime();
            break;
        case NET_EVENT_GET_ENEMY_TEAM:
            pRecvEvent = new _netRecvEventGetEnemyTeamInfo();
            break;
        case NET_EVENT_GET_RANDOM_ENEMY_TEAM:
            pRecvEvent = new _netRecvEventGetRandomEnemyTeamInfo();
            break;
        case NET_EVENT_UPLOAD_VIDEO:
            pRecvEvent = new _netRecvEventUploadVideoInfo();
            break;
        case NET_EVENT_GET_DEFENSE_VIDEO:
            pRecvEvent = new _netRecvEventGetDefenseVideoInfo();
            break;
        case NET_EVENT_UPDATE_AWARD_STATE:
            pRecvEvent = new _netRecvEventUpdateAwardState();
            break;
        case NET_EVENT_UPDATE_STONE_RATE:
            pRecvEvent = new _netRecvEventUpdateKuangChang();
            break;
        case NET_EVENT_COLLECT_STONE:
            pRecvEvent = new _netRecvEventCollectStone();
            break;
        case NET_EVENT_START_PRODUCT:
            pRecvEvent = new _netRecvEventStartProduct();
            break;
        case NET_EVENT_IS_END_PRODUCT:
            pRecvEvent = new _netRecvEventIsEndProduct();
            break;
        case NET_EVENT_UPDATE_KUANGSHI:
            pRecvEvent = new _netRecvEventUpdateKuangshiLimit();
            break;
        case NET_EVENT_UPDATE_ONLINE_TIME:
            pRecvEvent = new _netRecvEventUpdateOnLineTime();
            break;
        case NET_EVENT_GET_DEFENSE_VIDEOLIST:
            pRecvEvent = new _netRecvEventGetDefenseVideoListInfo();
            break;
        case NET_EVENT_SET_USER_TEAM:
            pRecvEvent = new _netRecvEventSetUserTeamInfo();
            break;
        case NET_EVENT_GET_TIME:
            pRecvEvent = new _netRecvEventGetSevTime();
            break;
        case NET_EVENT_SEND_BUY:
            pRecvEvent = new _netRecvEventGetBuyInfo();
            break;
        case NET_EVENT_SAVEMCF:
            pRecvEvent = new _netRecvEventMCF();
            break;
        case NET_EVENT_UPDATEUSERNAME:
            pRecvEvent = new _netRecvEventUpdateUserName();
            break;
        case NET_EVENT_SAVE_BOOT_MAPINFO:
            pRecvEvent = new _netRecvEventSaveBootMap();
            break;
            
    }
    
    if (pRecvEvent) {
        pRecvEvent->m_eventType = nEventType;
        pRecvEvent->parseAllParams(leftcommandstr);
        pRecvEvent->init();
    }
    
    
    return pRecvEvent;
}

std::string _netNewSendEventBase::getContentStr()
{
    std::string sendstr = URL_ADDRESS;
    
    sendstr += getNewEventHeadStr(m_eventType);
    sendstr += "/";
    sendstr += getNewEventNameByEventType(m_eventType);
    sendstr += "?";
    return sendstr;
}

std::string _netNewSendEventBase::generateSendStr()
{
    std::string sendstr = URL_ADDRESS;
    
    sendstr += getNewEventHeadStr(m_eventType);
    sendstr += "/";
    sendstr += getNewEventNameByEventType(m_eventType);
    sendstr += "?";
    std::string contentstr = generateContentStr();
    sendstr += contentstr;
    
    return sendstr;
}

std::string _netNewRecvEventBase::truncateCommandHeaderStr(std::string commandstr)
{
    int pos = commandstr.find('?');
    if (pos != std::string::npos) {
        std::string headname = commandstr.substr(0,pos);
        commandstr = commandstr.substr(pos+1,-1);
        return commandstr;
    }
    return commandstr;
}
/******************************************************************
 //funcName:parseAllParams
 
 //date:2013/3/14 16:11
 
 //desc:对传入的字符串进行解析
 以&作为分隔符,!作为结束
 比如:"userid=1000&name=中国!"
 vector中会存入
 userid=1000
 name=中国
 //param:
 
 //ret value:
 
 //author:butcher
 ******************************************************************/
void _netNewRecvEventBase::parseAllParams(const std::string&str)
{
    m_paramsStr.reserve(10);
    m_paramsStr.clear();
    
    std::string tempstr = str;
    int seperatorpos = tempstr.find('&');
    if(seperatorpos == -1){//如果没有&,就是一条整的命令
        m_paramsStr.push_back(tempstr);
    }else{
        //第一个是不是&,是就直接跳过
        if(seperatorpos == 0){
            tempstr = tempstr.substr(1,-1);
        }
        while(tempstr.length() > 0){
            int seperatorpos = tempstr.find('&');
            if(seperatorpos == -1){//如果没有&,就是一条整的命令
                m_paramsStr.push_back(tempstr);
                break;
            }
            std::string oneInstructionstr = tempstr.substr(0,seperatorpos);
            if(oneInstructionstr.length() > 0){
                m_paramsStr.push_back(oneInstructionstr);
            }
            if(seperatorpos >= (int)(tempstr.length()) - 1){
                break;
            }
            tempstr = tempstr.substr(seperatorpos + 1,-1);
        }
    }
    
}
/******************************************************************
 //funcName:parseNumberAfterEqualFromInstruction
 
 //date:2013/3/14 16:17
 
 //desc:根据一条指令解析出后面对应的数字,因为比较通用,所以提取出来
 比如 userid=1203
 type=4567
 
 //param:
 
 //ret value:=后面的数字
 
 //author:butcher
 ******************************************************************/
int _netNewRecvEventBase::parseNumberAfterEqualFromInstruction(const std::string&str)
{
    std::string tempstr = parseStrAfterEqualFromInstruction(str);
    if(tempstr.length() > 0)
        return atoi(tempstr.c_str());
    return 0;
}

unsigned long long _netNewRecvEventBase::parseLongLongNumberAfterEqualFromInstruction(const std::string&useridStr)
{
    std::string tempstr = parseStrAfterEqualFromInstruction(useridStr);
    if(tempstr.length() > 0)
        return atoll(tempstr.c_str());
    return 0;
}

long _netNewRecvEventBase::parseLongNumberAfterEqualFromInstruction(const std::string&str)
{
    std::string tempstr = parseStrAfterEqualFromInstruction(str);
    if(tempstr.length() > 0)
        return atol(tempstr.c_str());
    return 0;
}

float _netNewRecvEventBase::parseFloatAfterEqualFromInstruction(const std::string&str)
{
    std::string tempstr = parseStrAfterEqualFromInstruction(str);
    if(tempstr.length() > 0)
        return (float)(atof(tempstr.c_str()));
    return 0;
}

/******************************************************************
 //funcName:parseNumberAfterEqualFromInstruction
 
 //date:2013/3/14 16:17
 
 //desc:根据一条指令解析出后面对应的数字,因为比较通用,所以提取出来
 比如 username=中国
 
 //param:
 
 //ret value:中国
 
 //author:butcher
 ******************************************************************/
std::string _netNewRecvEventBase::parseStrAfterEqualFromInstruction(const std::string&str)
{
    std::string result;
    int pos = str.find('=');
    CCAssert(pos != -1,"");
    if(pos == -1)
        return result;
    return str.substr(pos+1,-1);
}
/******************************************************************
 //funcName:parseHasEqualInstructionKeyname
 
 //date:2013/10/21
 
 //desc:获取一个含有=指令的key
 比如 userid=1234
 返回"userid"
 
 //param:
 
 //ret value:
 
 //author:zzb
 ******************************************************************/
std::string _netNewRecvEventBase::parseHasEqualInstructionKeyname(const std::string& key)
{
    /*int pos = str.find('=');
     CCAssert(pos != -1,"");
     if(pos == -1)
     
     return str.substr(0,pos);*/
    return 0;
}
