//
//  zzbNetControlModule.cpp
//  GAME2
//
//  Created by zzb on 14-1-11.
//
//

#include "zzbNetControlModule.h"
#include "cocos-ext.h"
#include "network/HttpClient.h"
#include "zzbNewNetStruct.h"

#include "LYNetDialog.h"

#include "CHitDialog.h"
//#include "zzbLayerNetCommunication.h"


static const char safe[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,1,1,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

USING_NS_CC;
USING_NS_CC_EXT;

using namespace cocos2d::network;
static CNetControlModule* g_NetControlModule = NULL;
CNetControlModule* CNetControlModule::sharedNetControlModule()
{
    if (!g_NetControlModule)
    {
        g_NetControlModule = new CNetControlModule();
        g_NetControlModule->init();
    }
    return g_NetControlModule;
}


std::string CNetControlModule::encode(const std::string uri){
    
    
    string ret;
    const unsigned char *ptr = (const unsigned char *)uri.c_str();
    ret.reserve(uri.length());
    
    for (; *ptr ; ++ptr)
    {
        if (!safe[*ptr])
        {
            char buf[5];
            memset(buf, 0, 5);
#ifdef WIN32
            _snprintf_s(buf, 5, "%%%X", (*ptr));
#else
            snprintf(buf, 5, "%%%X", (*ptr));
#endif
            ret.append(buf);
        }
        else if (*ptr==' ')
        {
            ret+='+';
        }
        else{
            ret += *ptr;
        }
    }
    return ret;
    
    
    
}

std::string CNetControlModule::decode(const std::string uri){
    
    
    
    const unsigned char *ptr = (const unsigned char *)uri.c_str();
    string ret;
    ret.reserve(uri.length());
    for (; *ptr; ++ptr)
    {
        if (*ptr == '%')
        {
            if (*(ptr + 1))
            {
                char a = *(ptr + 1);
                char b = *(ptr + 2);
                if (!((a >= 0x30 && a < 0x40) || (a >= 0x41 && a < 0x47))) continue;
                if (!((b >= 0x30 && b < 0x40) || (b >= 0x41 && b < 0x47))) continue;
                char buf[3];
                buf[0] = a;
                buf[1] = b;
                buf[2] = 0;
                ret += (char)strtoul(buf, NULL, 16);
                ptr += 2;
                continue;
            }
        }
        if (*ptr=='+')
        {
            ret+=' ';
            continue;
        }
        ret += *ptr;
    }
    return ret;

    
    
}




bool CNetControlModule::addHttpPackage(HTTP_PACKAGE& strPackage)
{
    m_lstNetPackage.push_back(strPackage);//添加在队尾
    return true;
}

bool CNetControlModule::init()
{
    if (!CCNode::init())
        return false;
    
    m_pNetDialog = NULL;
    
    onEnter();
    onEnterTransitionDidFinish();
    scheduleUpdate();
    return true;
}
void CNetControlModule::update(float dt)
{
    CCNode::update(dt);
    if (m_lstNetPackage.size()>0 && !m_bIfSending)
    {
        m_bIfSending = true;
        sendHttpPackage(m_lstNetPackage.front());
    }
    
    m_fNetTime-=dt;
    
    if (m_fNetTime<0&&m_pNetDialog) {
        m_pNetDialog->removeFromParentAndCleanup(true);
    }
    
}
/*
 发送接收函数
 */
void CNetControlModule::sendHttpPackage(HTTP_PACKAGE& package)
{
    //开始发送前先将标志位设成true表示已经有消息在发送了
    //    CLayerNetCommunication* pLayer = new CLayerNetCommunication();
    //    pLayer->init();
    //    Director::getInstance()->getRunningScene()->addChild(pLayer,999999);
    if(m_bShowHttpDialog)
    {
        // CDialogMgr::GetSingletonPtr()->createHttpDialog();
    }
    m_bShowHttpDialog = true;
    log("%s",package.m_strHttpUrlDate.c_str());
    

    
    HttpRequest* request = new HttpRequest();
    request->setUrl(package.m_strHttpUrlDate.c_str());
    request->setRequestType(network::HttpRequest::Type::POST);
    request->setResponseCallback(this, httpresponse_selector(CNetControlModule::recvHttpPackage));
    
    request->setRequestData( package.postData.c_str(),package.postData.length());  //ly新增。post方式下的数据流
    
    request->setUserData((void*)package.m_nEventType);
    char buffer[10] = {0};//将event type作为标示
    sprintf(buffer, "%d",package.m_nEventType);
    request->setTag(buffer);
    
    HttpClient::getInstance()->setTimeoutForConnect(NetDelayTime);//等待时间设置为10秒
    HttpClient::getInstance()->setTimeoutForRead(NetDelayTime);
    HttpClient::getInstance()->send(request);
    request->release();
    
    
    //增加屏蔽层
    if (package.m_bIfNeedDialog) {  //为什么在之前也重写了一次，是因为此方法是在update中调用，之前写一次是防止在update之前又响应了别的逻辑，因此提前加上屏蔽层
        
        if (m_pNetDialog) {
            
            m_fNetTime = NetDelayTime;
            
        }else{
            m_pNetDialog = CNetDialog::create();
            Director::getInstance()->getRunningScene()->addChild(m_pNetDialog,9999999);
            m_fNetTime = NetDelayTime;
            
        }
        
    }
    
}

void CNetControlModule::recvHttpPackage(Node* node, void* data)
{
    //延时1秒
    //runAction(Sequence::create(<#cocos2d::FiniteTimeAction *action1, ...#>, NULL))
    
     //CallFunc* guide = CallFunc::create(CC_CALLBACK_0(CNetControlModule::removeNetDialog, this));
    
    //unAction(Sequence::create(DelayTime::create(1.5),guide,NULL));
    
     removeNetDialog();
    
    
    HttpResponse *response = (HttpResponse*)data;
    
    char hintBuffer[127] = {0};
    
    if (!response)
    {
        /*
         消息发送错误
         */
        onBtnErrorHint("消息发送失败！\r\n请重试！");
        return;
    }
    int s= (int)response->getHttpRequest()->getRequestType();
    log("request type %d",s);
    
    
    if (0 != strlen(response->getHttpRequest()->getTag()))
    {
        log("%s ------>oked", response->getHttpRequest()->getTag());
    }
    
    int statusCode = response->getResponseCode();
    log("response code: %d", statusCode);
    
    
    char statusString[64] = {};
    sprintf(statusString, "HTTP Status Code: %d, tag = %s", statusCode, response->getHttpRequest()->getTag());
    log("%s",statusString);//可以根据不同的tag进行解析
    
    if (!response->isSucceed())
    {
        log("response failed");
        log("error buffer: %s", response->getErrorBuffer());
        onBtnErrorHint("网络请求错误！\r\n请重试!");
        return;
    }
    
    std::vector<char> *buffer = response->getResponseData();
    printf("Http Test, dump data: ");
    std::string sBuffer = "";
    for (unsigned int i = 0; i < buffer->size(); i++)
    {
        sBuffer += (*buffer)[i];
    }
    log("%s",sBuffer.c_str());
    
    /*
     判断返回的消息是否为空
     */
    if (sBuffer.size()<=0 && sBuffer.find("info=") == std::string::npos) {
        onBtnErrorHint("服务器返回的数据为空！\r\n请重试！");
        return;
    }
    /*
     根据htag判断是哪条消息，调用那个struct解析
     */
    int nEventType = atoi(response->getHttpRequest()->getTag());
    log("\r\nEvent type=%d",nEventType);
    
    _netNewRecvEventBase* pNetEvent = _netNewRecvEventBase::create(nEventType, sBuffer);
    
    
    pNetEvent->m_retCode;
    
    
    
    
    if (!pNetEvent) {
        onBtnErrorHint("错误的命令！\r\n请重试！");
        return;
    }

    else if (pNetEvent->m_retCode != 0)
    {/*
      还要判断返回的info是否等于0 表示服务器返回的信息成功与否
      */
        memset(hintBuffer, 0, sizeof(hintBuffer));
        sprintf(hintBuffer, "服务器返回错误！info=%d\r\n请重试！",pNetEvent->m_retCode);
        onBtnErrorHint(hintBuffer);
        
        CC_SAFE_DELETE(pNetEvent);
        return;
    }
    if (m_lstNetPackage.size()<=0)
    {
        CC_SAFE_DELETE(pNetEvent);
        m_nResendCount = 0;
        m_bIfSending = false;
        return;
    }
    
    if (m_lstNetPackage.front().m_pTarget!=NULL && m_lstNetPackage.front().m_pHandler!=NULL)
    {
        (m_lstNetPackage.front().m_pTarget->*m_lstNetPackage.front().m_pHandler)(pNetEvent);
    }
    
    CC_SAFE_DELETE(pNetEvent);
    m_lstNetPackage.pop_front();//发送成功后从第一条从新开始发送
    //发送玩把标志位设成false，允许下一条消息继续发送
    m_bIfSending = false;
    m_nResendCount = 0;
}
/*
 弹出框点击确认后再点
 */
void CNetControlModule::onBtnErrorHint(std::string hintBuffer)
{
    m_nResendCount ++;
    if (m_nResendCount<=1) {
        onEventCallBackYes(NULL);
        return;
    }
    if (m_nResendCount>=5)
    {
        CHitDialog* temp = CHitDialog::create();
        temp->createYesNO(this, menu_selector(CNetControlModule::onEventCallBackYes), hintBuffer.c_str(),menu_selector(CNetControlModule::onEventCallBackNo));
        Director::getInstance()->getRunningScene()->addChild(temp,9999999);
        
        
        //CDialogMgr::GetSingletonPtr()->createDialog(hintBuffer.c_str(), this, menu_selector(CNetControlModule::onEventCallBackYes), this, menu_selector(CNetControlModule::onEventCallBackNo));
    }else if(m_nResendCount>=2)
    {
        CHitDialog* temp = CHitDialog::create();
        temp->CreateDialog(hintBuffer.c_str(),this, menu_selector(CNetControlModule::onEventCallBackYes));
        Director::getInstance()->getRunningScene()->addChild(temp,9999999);
        
        //  CDialogMgr::GetSingletonPtr()->createDialog(hintBuffer.c_str(), this, menu_selector(CNetControlModule::onEventCallBackYes),NULL,NULL);
    }else
    {
        
    }
    
}

void CNetControlModule::onEventCallBackYes(cocos2d::Ref* pSender)
{
    m_bIfSending = false;
}

void CNetControlModule::onEventCallBackNo(cocos2d::Ref* pSender)
{
    //    m_lstNetPackage.pop_front();
    //    m_bIfSending = false;
    //如果可以的话点取消就跳到登陆界面
    m_lstNetPackage.clear();
    m_bIfSending = false;
    m_nResendCount = 0;
    //LAYER_MANAGER->createScene(LAYER_TYPE_HELLO_WORLD,true,true);
}

CNetControlModule::CNetControlModule()
: m_bIfSending(false)
, m_nResendCount(0)
{
}

CNetControlModule::~CNetControlModule()
{
}

void CNetControlModule::removeNetDialog(){
    
    if (m_pNetDialog) {
        m_pNetDialog->removeFromParentAndCleanup(true);
    }
    m_pNetDialog = NULL;
    
    
    
}

void CNetControlModule::AddImportantHttpPackage(cocos2d::Ref* pTarget,cocos2d::SEL_CallFuncO phandler,_netNewSendEventBase & event, std::string httpStr,bool ifNeedDialog)
{
    HTTP_PACKAGE strPackage;
    strPackage.m_pTarget = pTarget;
    strPackage.m_pHandler = phandler;
    strPackage.m_nEventType = event.m_eventType;
    std::string str = event.getContentStr() += httpStr;
    strPackage.m_strHttpUrlDate = str;
    strPackage.m_bIfNeedDialog = ifNeedDialog;
    addHttpPackage(strPackage);
    
    
    //增加屏蔽层
    if (ifNeedDialog) {
        
        
        if (m_pNetDialog) {
            
            m_fNetTime = NetDelayTime;
            
        }else{
            m_pNetDialog = CNetDialog::create();
            Director::getInstance()->getRunningScene()->addChild(m_pNetDialog,9999999);
            m_fNetTime = NetDelayTime;
            
        }
        
    }
}

void CNetControlModule::AddHttpPackage(cocos2d::Ref* pTarget,cocos2d::SEL_CallFuncO phandler,_netNewSendEventBase & event,bool ifNeedDialog)
{
    HTTP_PACKAGE strPackage;
    strPackage.m_pTarget = pTarget;
    strPackage.m_pHandler = phandler;
    strPackage.m_nEventType = event.m_eventType;
    strPackage.m_strHttpUrlDate = event.generateSendStr();
    strPackage.m_bIfNeedDialog = ifNeedDialog;
    strPackage.postData = event.postData;
    
    addHttpPackage(strPackage);
    
    
    //增加屏蔽层
    if (ifNeedDialog) {
        
        
        if (m_pNetDialog) {
            
            m_fNetTime = NetDelayTime;
            
        }else{
            m_pNetDialog = CNetDialog::create();
            Director::getInstance()->getRunningScene()->addChild(m_pNetDialog,9999999);
            m_fNetTime = NetDelayTime;
            
        }
        
    }

}



void CNetControlModule::updateUserName(cocos2d::Ref* pTarget,cocos2d::SEL_CallFuncO phandler,std::string strUserName)
{
   _netSendEventUpdateUserName event;
    event.m_eventType =  NET_EVENT_UPDATEUSERNAME;
    event.m_strUserName =   strUserName;
    
    AddHttpPackage(pTarget,phandler,event,true);
    
}

void CNetControlModule::Login(cocos2d::Ref* pTarget,cocos2d::SEL_CallFuncO phandler)
{
    _netSendEventLogin event;
    event.m_eventType = NET_EVENT_LOGIN;
    AddHttpPackage(pTarget,phandler,event,true);
}


void CNetControlModule::SaveDinShiTime(cocos2d::Ref* pTarget,cocos2d::SEL_CallFuncO phandler,int iUserBuildId,int iTime)
{
    _netSendEventDingshiTime event;
    event.m_eventType = NET_EVENT_BORN_TIME;
    event.m_iBuildingId = iUserBuildId;
    event.m_iTime = iTime;
    
    AddHttpPackage(pTarget,phandler,event,true);
    
}

void CNetControlModule::SaveUserMCF(cocos2d::Ref* pTarget,cocos2d::SEL_CallFuncO phandler,int iCoinAdd,int iStoneAdd,int iStamina)
{
    _netSendEventMCF event;
    event.m_eventType = NET_EVENT_SAVEMCF;
    event.m_iMagicAdd = iStoneAdd;
    event.m_iCoinAdd =  iCoinAdd;
    event.m_iStaminaAdd = iStamina;
    AddHttpPackage(pTarget,phandler,event,true);
}



void CNetControlModule::setUserTeamInfo(cocos2d::Ref* pTarget,cocos2d::SEL_CallFuncO phandler, std::string teamInfo, int level)
{
    _netSendEventSetUserTeamInfo event;
    event.m_eventType = NET_EVENT_SET_USER_TEAM;
    event.teamInfo = teamInfo;
    event.level = level;
    AddHttpPackage(pTarget, phandler, event,true);
}

void CNetControlModule::getUserTeamInfo(cocos2d::Ref* pTarget,cocos2d::SEL_CallFuncO phandler)
{
    _netSendEventGetUserTeamInfo event;
    event.m_eventType = NET_EVENT_GET_USER_TEAM;
    AddHttpPackage(pTarget, phandler, event,true);
}

void CNetControlModule::getEnemyTeamInfo(cocos2d::Ref* pTarget,cocos2d::SEL_CallFuncO phandler, int userId)
{
    _netSendEventGetEnemyTeamInfo event;
    event.m_eventType = NET_EVENT_GET_ENEMY_TEAM;
    event.userId = userId;
    AddHttpPackage(pTarget, phandler, event,true);
}

void CNetControlModule::getRandomEnemyTeamInfo(cocos2d::Ref* pTarget,cocos2d::SEL_CallFuncO phandler, int level, std::string content)
{
    _netSendEventGetRandomEnemyTeamInfo event;
    event.m_eventType = NET_EVENT_GET_RANDOM_ENEMY_TEAM;
    event.level = level;
    event.content = content;
    AddHttpPackage(pTarget, phandler, event,true);
}

void CNetControlModule::uploadVideo(cocos2d::Ref* pTarget,cocos2d::SEL_CallFuncO phandler, int userId, std::string teamInfo, std::string attackTeam,
                 std::string enemyInfo, std::string enemySolider, int stone, std::string video, int iSuccess, int percent)
{
    _netSendEventUploadVideoInfo event;
    event.m_eventType = NET_EVENT_UPLOAD_VIDEO;
    event.userId = userId;
    event.teamInfo = teamInfo;
    event.attackTeam = attackTeam;
    event.enemyInfo = enemyInfo;
    event.enemySolider = enemySolider;
    event.stone = stone;
//    event.video = video;
    event.iSuccess = iSuccess;
    event.percent = percent;
    event.postData = video;
    AddHttpPackage(pTarget, phandler, event,true);
}

void CNetControlModule::getDefenseVideoList(cocos2d::Ref* pTarget,cocos2d::SEL_CallFuncO phandler)
{
    _netSendEventGetDefenseVideoListInfo event;
    event.m_eventType = NET_EVENT_GET_DEFENSE_VIDEOLIST;
    AddHttpPackage(pTarget, phandler, event,true);
}

void CNetControlModule::getDefenseVideoByVideoId(cocos2d::Ref* pTarget,cocos2d::SEL_CallFuncO phandler, int videoId)
{
    _netSendEventGetDefenseVideoInfo event;
    event.m_eventType = NET_EVENT_GET_DEFENSE_VIDEO;
    event.videoId = videoId;
    AddHttpPackage(pTarget, phandler, event,true);
}

void CNetControlModule::updateDefenseAward(cocos2d::Ref* pTarget,cocos2d::SEL_CallFuncO phandler, int videoId, int stamina)
{
    _netSendEventUpdateAwardState event;
    event.m_eventType = NET_EVENT_UPDATE_AWARD_STATE;
    event.videoId = videoId;
    event.stamina = stamina;
    AddHttpPackage(pTarget, phandler, event,true);
}

void CNetControlModule::updateKuangChangRate(cocos2d::Ref *pTarget,cocos2d::SEL_CallFuncO phandler, std::string identify,int iAddStone,int maxAddStone)
{
    _netSendEventUpdateKuangChang event;
    event.m_eventType = NET_EVENT_UPDATE_STONE_RATE;
    event.identify = identify;
    event.iAddStone = iAddStone;
    event.maxAddStone = maxAddStone;
    AddHttpPackage(pTarget, phandler, event,true);
}



void CNetControlModule::collectStone(cocos2d::Ref* pTarget, cocos2d::SEL_CallFuncO phander, std::string identify)
{
    _netSendEventCollectStone event;
    event.m_eventType = NET_EVENT_COLLECT_STONE;
    event.identify = identify;
    AddHttpPackage(pTarget, phander, event,true);
}

void CNetControlModule::startProduct(cocos2d::Ref* pTarget, cocos2d::SEL_CallFuncO phander, std::string identify, int productNum, int productTime)
{
    _netSendEventStartProduct event;
    event.m_eventType = NET_EVENT_START_PRODUCT;
    event.identify = identify;
    event.productNum = productNum;
    event.productTime = productTime;
    AddHttpPackage(pTarget, phander, event,true);
}

void CNetControlModule::isEndProduct(cocos2d::Ref* pTarget, cocos2d::SEL_CallFuncO phander, std::string identify)
{
    _netSendEventIsEndProduct event;
    event.m_eventType = NET_EVENT_IS_END_PRODUCT;
    event.identify = identify;
    AddHttpPackage(pTarget, phander, event,true);
}

void CNetControlModule::updateKuangshiLimit(cocos2d::Ref* pTarget, cocos2d::SEL_CallFuncO phander, int maxLimit)
{
    _netSendEventUpdateKuangshiLimit event;
    event.m_eventType = NET_EVENT_UPDATE_KUANGSHI;
    event.maxLimit = maxLimit;
    AddHttpPackage(pTarget, phander, event,true);
}

void CNetControlModule::updateUserOnLineTime(cocos2d::Ref* pTarget, cocos2d::SEL_CallFuncO phander)
{
    _netSendEventUpdateOnLineTime event;
    event.m_eventType = NET_EVENT_UPDATE_ONLINE_TIME;
    AddHttpPackage(pTarget, phander, event);
}

void CNetControlModule::getChargeInfo(cocos2d::Ref* pTarget,cocos2d::SEL_CallFuncO phandler)
{
    _netSendEventGetChargeInfo event;
    event.m_eventType = NET_EVENT_GET_CHARGE;
    AddHttpPackage(pTarget, phandler, event,true);
}

void CNetControlModule::updateUserEvolution(cocos2d::Ref* pTarget, cocos2d::SEL_CallFuncO phander)
{
    _netSendEventUpdateEvolution event;
    event.m_eventType = NET_EVENT_UPDATE_EVOLUTION;
    AddHttpPackage(pTarget, phander, event,true);
}

void CNetControlModule::updateUserProtectTime(cocos2d::Ref* pTarget, cocos2d::SEL_CallFuncO phander)
{
    _netSendEventUpdateProtectTime event;
    event.m_eventType = NET_EVENT_UPDATE_PROTECT_TIME;
    AddHttpPackage(pTarget, phander, event,true);
}

void CNetControlModule::getServerTime(cocos2d::Ref* pTarget,cocos2d::SEL_CallFuncO phandler)
{
    _netSendEventGetSevTime event;
    event.m_eventType = NET_EVENT_GET_TIME;
    AddHttpPackage(pTarget, phandler, event,true);
}

void CNetControlModule::sendChargeInfo(cocos2d::Ref* pTarget,cocos2d::SEL_CallFuncO phandler, std::string tempStr)
{
    _netSendEventBuyInfo event;
    event.m_eventType = NET_EVENT_SEND_BUY;
    AddImportantHttpPackage(pTarget, phandler, event,tempStr,true);
}

void CNetControlModule::newLogin(cocos2d::Ref *pTarget, cocos2d::SEL_CallFuncO phandler){
    
    _netSendEventNewUser event;
    event.m_eventType = NET_EVENT_NEWUSER;
    AddHttpPackage(pTarget, phandler, event,true);
}

void CNetControlModule::versionJudge(cocos2d::Ref *pTarget, cocos2d::SEL_CallFuncO phandler){
    
    _netSendEventVersionJudge event;
    event.m_eventType = NET_EVENT_VERSION;
    AddHttpPackage(pTarget, phandler, event,true);

}

void CNetControlModule::uploadSuggestion(cocos2d::Ref *pTarget, cocos2d::SEL_CallFuncO phandler, std::string content){
    
    _netSendEventSuggestion event;
    event.m_eventType = NET_EVENT_SUGGES;
    event.theSugeesitonContent = content;
    
    AddHttpPackage(pTarget, phandler, event,false);
}


void CNetControlModule::UpdateKuanshiNum(cocos2d::Ref *pTarget,cocos2d::SEL_CallFuncO phandler,int iKuanshiAdd)
{
    _netSendEventKuanshi event;
    event.m_eventType = NET_EVENT_UPDATE_KUANSHI_NUM;
    event.m_iAddNum = iKuanshiAdd;
     AddHttpPackage(pTarget, phandler, event,true);
}



void CNetControlModule::GetDrawCardInfo(cocos2d::Ref * pTarget,cocos2d::SEL_CallFuncO phandler)
{
    _netSendEventGetDrawCard event;
    event.m_eventType =  NET_EVENT_GET_DRAWCARDINFO;
    
    AddHttpPackage(pTarget, phandler, event,true);
}

void CNetControlModule::SaveDrawCardInfo(cocos2d::Ref * pTarget,cocos2d::SEL_CallFuncO phandler,int iType,int iDrawNum)
{
    _netSendEventSaveDrawCard event;
    event.m_eventType = NET_EVENT_SAVE_DRAWCARDINFO;
    
    event.m_iType = iType;
    event.m_iDrawNum  = iDrawNum;
    
   AddHttpPackage(pTarget, phandler, event,true);
}

void CNetControlModule::saveTheMap(cocos2d::Ref *pTarget, cocos2d::SEL_CallFuncO phandler){
    
    _netSendEventSaveBootMap event;
    
    event.m_eventType = NET_EVENT_SAVE_BOOT_MAPINFO;
    
    AddHttpPackage(pTarget, phandler, event,false);
}



void CNetControlModule::gameOver(cocos2d::Ref *pTarget){
    
    Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
    
}

