//
//  PhysicalSystem.cpp
//  TheGodWill
//
//  Created by prefer_shan on 14/12/10.
//
//

#include "PhysicalSystem.h"
#include "LYPersonalData.h"
#include "sysTime.h"
#include "DataManager.h"
#include "DBMgr.h"
#include "NetWorkDataSingle.h"

#include "HttpEncrypt.h"

CPhysicalSystem *CPhysicalSystem::t = NULL;

#define TIMEUP   300

CPhysicalSystem::CPhysicalSystem()
{
    m_winSize =  Director::getInstance()->getWinSize();
    m_iCurSecond = 0;
    m_pTime = NULL;
    m_pTarget = NULL;
    m_pHander1 = NULL;
    m_pHander2 = NULL;
    m_pLabelRemain = NULL;
    
}

CPhysicalSystem::~CPhysicalSystem()
{
    
}


CPhysicalSystem* CPhysicalSystem::GetSingletonPtr()
{
    if (!t)
    {
        t = new CPhysicalSystem();
        t->init();
        
    }
    return t;
}

void CPhysicalSystem::SavePhysicalTime(long sec)
{
    
    long   iCurSec = CGameTime::getNowTimeInSecond();
    
    if (sec!=0) {
        iCurSec = sec;
    }
    
    
    m_iCurSecond = iCurSec;
    
    CDBMgr::getInstance()->saveUserStaminaTime(iCurSec);
    
    CPersonalData::getInstance()->getUserInfo()->m_iStaminTime = m_iCurSecond;
    
    CCLOG("保存体力时间 :%ld",m_iCurSecond);
    
} 


void CPhysicalSystem::OnSetPhysicalNum(Ref *pSender)
{
    log("onSetPhysicalNum\n");
    
    if (m_pTarget&&m_pHander1) {
        (m_pTarget->*m_pHander1)(NULL);
    }
    
}


int CPhysicalSystem::GetNextUpTime()
{
    long  iCurSec = CGameTime::getNowTimeInSecond();
    
//    char strKey[10] = {"timet"};

//    int iTimeBegin =  UserDefault::getInstance()->getIntegerForKey(strKey,0);

    long iTimeBegin= CPersonalData::getInstance()->getUserInfo()->m_iStaminTime;
    
    long iSpendTime =  iCurSec - iTimeBegin;
    
    
    long iBei = iSpendTime/TIMEUP;  //几次涨体力
    
    int lasttime = iSpendTime%TIMEUP;  //剩余升级时间
    
    
    //更新体力值
    int iPhysicalNum = CPersonalData::getInstance()->GetPhysicalNum();
    

    
    UserInfo* pInfo = CPersonalData::getInstance()->getUserInfo();
    int iUpperStamin = CDataManager::getUpgradeInfoByLevel(pInfo->m_iUserLevel)->upperStamina;

    int iNum = iPhysicalNum+iBei;

    if(iNum > iUpperStamin)
    {
        iNum = iUpperStamin;
    }

    
    //更新保存体力时间
    int realNum = iNum-iPhysicalNum; //真正恢复体力数(容错，防止超出最大体力值)
    if(realNum>0){
        long saveTime = iTimeBegin + realNum*TIMEUP;
        
        CNetWorkDataSingle::GetSingletonPtr()->SaveUserMCF(this,menu_selector(CPhysicalSystem::OnSetPhysicalNum),0,0,realNum);
        SavePhysicalTime(saveTime);
        
        //CPersonalData::getInstance()->SetPhysicalNum(iNum);//保存体力值
        
        CPersonalData::getInstance()->setUpdateUserInfo(true);
    }
    
    int level = CPersonalData::getInstance()->getUserInfo()->m_iUserLevel;
    if (iNum >= CDataManager::getUpgradeInfoByLevel(level)->upperStamina) {
        CPersonalData::getInstance()->setIfStaminaFull(true);
    }
    
    
    
    
    
    
    return TIMEUP-lasttime;
    
    
//    if(iTimeBegin<=0)
//    {
//        return TIMEUP;
//    }
//    else
//    {
//        long iNowTime = iTimeBegin-iSpendTime;
//    
//        if(iNowTime>=0)
//        {
//            return  iNowTime;
//        }
//        else
//        {
//         int iBei = -iNowTime/TIMEUP;
//        
//        int iPhysicalNum = CPersonalData::getInstance()->GetPhysicalNum();
//        
//        UserInfo* pInfo = CPersonalData::getInstance()->getUserInfo();
//        int iUpperStamin = CDataManager::getUpgradeInfoByLevel(pInfo->m_iUserLevel)->upperStamina;
//        
//        int iNum = iPhysicalNum+iBei;
//        
//        if(iNum > iUpperStamin)
//        {
//            iNum = iUpperStamin;
//        }
//        
//        CPersonalData::getInstance()->SetPhysicalNum(iNum);
//        
//        m_iCurSecond = iCurSec;
//        
//        return -(iNowTime)%TIMEUP;
//        }
//
//    }
}


int CPhysicalSystem::GetNowTimeBySec()
{
    struct timeval nowTimeval;
    gettimeofday(&nowTimeval, NULL);
    
    struct tm * tm;
    time_t time_sec ;
    time_sec = nowTimeval.tv_sec;
    tm = localtime(&time_sec);
    int inMinute = tm->tm_min;
    int inSecond = tm->tm_sec;
    int inHour = tm->tm_hour;
    int iDay = tm->tm_mday;
    int iMonth = tm->tm_mon+1;
    int iYear = tm->tm_year+1900;
    
    return inHour*3600+inMinute*60+inSecond;
}


bool CPhysicalSystem::init()
{
//    m_iPhysicalNum = CPersonalData::getInstance()->GetPhysicalNum();
    
    int  iNowSec =  CGameTime::getNowTimeInSecond();
    
    log("iNowSec = %d\n",iNowSec);
    
    userInfoInDB* pUserInfo = CDataManager::getTheUSerInfDB();

//    int iTiliValue  = CPersonalData::DataStrToInt(pUserInfo->staminaStr);
    
    int iCurSec =  pUserInfo->staminaTime;
    
     log("tilistamina = %d\n",iCurSec);
    
    m_iCurSecond = iNowSec;
    
    int iAddPhysicalNum = 0;
    
    if(iCurSec != 0)
    {
        m_iCurSecond = iCurSec;
    }
    
//    if(iTiliValue!=0)
//    {
//        m_iPhysicalNum = iTiliValue+iAddPhysicalNum;
//        
//        UserInfo* pInfo = CPersonalData::getInstance()->getUserInfo();
//        int iUpperStamin = CDataManager::getUpgradeInfoByLevel(pInfo->m_iUserLevel)->upperStamina;
//     
//        log("iUpperStamin = %d\n",iUpperStamin);
//        
//        if(m_iPhysicalNum>= iUpperStamin)
//        {
//           // m_iPhysicalNum = iUpperStamin;
//        }
//    }
//    
//    CPersonalData::getInstance()->SetPhysicalNum(m_iPhysicalNum);
    
    m_iTmpCurSec = m_iCurSecond;
    
//    UserInfo* userInfo = CPersonalData::getInstance()->getUserInfo();
    
//    CDBMgr::getInstance()->saveUserInfo(m_iPhysicalNum,userInfo->m_iUserLevel,userInfo->m_iUserExp,m_iCurSecond);
    
    return true;
}



void CPhysicalSystem::costManual(int icost,cocos2d::Ref* pTarget,cocos2d::SEL_MenuHandler pHandlerSucess,cocos2d::SEL_MenuHandler pHandlerFail)
{
    
    m_pTarget = pTarget;
    m_pHander1= pHandlerSucess;
    m_pHander2 = pHandlerFail;
    
    int iCurPhySicalNum = CPersonalData::getInstance()->GetPhysicalNum();
    
    int  targetNum = iCurPhySicalNum+ icost;
    
    if (targetNum<0&&m_pTarget&&m_pHander2) {//如果扣的小于0，则失败
        (m_pTarget->*m_pHander2)(NULL);
    }else{
        
       // CPersonalData::getInstance()->SetPhysicalNum(targetNum);
        
        CNetWorkDataSingle::GetSingletonPtr()->SaveUserMCF(this,menu_selector(CPhysicalSystem::OnSetPhysicalNum),0,0, icost);
        

        
    }
}





