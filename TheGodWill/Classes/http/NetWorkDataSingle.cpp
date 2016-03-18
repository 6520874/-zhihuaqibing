//
//  NetWorkDataSingle.cpp
//  TheGodWill
//
//  Created by prefer_shan on 14/12/18.
//
//

#include "NetWorkDataSingle.h"
#include "zzbNetControlModule.h"
#include "zzbNewNetStruct.h"
#include "LayerManager.h"




CNetWorkDataSingle *CNetWorkDataSingle::t = NULL;
CNetWorkDataSingle::CNetWorkDataSingle()
{
    
    m_bIfHadCallBack = false;
 
    
    m_pTarget = NULL;
    m_pHandlerInfo = NULL;
    
}

CNetWorkDataSingle::~CNetWorkDataSingle()
{

}


CNetWorkDataSingle * CNetWorkDataSingle::GetSingletonPtr()
{
    if (!t)
    {
        t = new CNetWorkDataSingle();
        
    }
    return t;
}


bool CNetWorkDataSingle::GetDrawCardInfo(cocos2d::Ref * pTarget,cocos2d::SEL_CallFuncO phandler)
{
    
    if (m_pTarget==NULL&&m_pHandlerInfo==NULL) {
        m_pTarget = pTarget;
        m_pHandlerInfo = phandler;
        //`CNetControlModule::sharedNetControlModule()->GetDrawCardInfo(this,menu_selector(CNetWorkDataSingle::OnCommonCallBack));
        return true;
    }else{
        CCLOG("what happens");
    }
 
    
    return false;
    
   
}

void CNetWorkDataSingle::SaveDrawCardInfo(cocos2d::Ref * pTarget,cocos2d::SEL_CallFuncO phandler,int iType,int iDrawNum)
{
    m_pTarget = pTarget;
    m_pHandlerInfo = phandler;
   // CNetControlModule::sharedNetControlModule()->SaveDrawCardInfo(this,menu_selector(CNetWorkDataSingle::OnCommonCallBack),iType,iDrawNum);
}


void CNetWorkDataSingle::SaveDinshiTime(cocos2d::Ref *target, cocos2d::SEL_CallFuncO handle,int iUserBuildId,int iTime)
{

    m_pTarget = target;
    m_pHandlerInfo = handle;
    
    
   // CNetControlModule::sharedNetControlModule()->SaveDinShiTime(this,menu_selector(CNetWorkDataSingle::OnCommonCallBack),iUserBuildId,iTime);
    
    
}

void CNetWorkDataSingle::SaveUserMCF(cocos2d::Ref *target, cocos2d::SEL_CallFuncO handler,int iCoinAdd,int iStoneAdd,int iStaminAdd)
{
    if(iStoneAdd < 0)
    {
        CDataManager::saveTaskInfoToDB(17, -iStoneAdd);
    }
    
    m_pTarget = target;
   m_pHandlerInfo = handler;
    
    if (m_pTarget&&m_pHandlerInfo) {
        m_bIfHadCallBack=false;
    }
    
    //CNetControlModule::sharedNetControlModule()->SaveUserMCF(this,menu_selector(CNetWorkDataSingle::OnCommonCallBack),iCoinAdd,iStoneAdd,iStaminAdd);
}


void CNetWorkDataSingle::updateUserName(cocos2d::Ref *target, cocos2d::SEL_CallFuncO handler,std::string strContent)
{
    m_pTarget = target;
    m_pHandlerInfo = handler;
    
    if (m_pTarget&&m_pHandlerInfo) {
        m_bIfHadCallBack=false;
    }
   
   // CNetControlModule::sharedNetControlModule()->updateUserName(this,menu_selector(CNetWorkDataSingle::OnCommonCallBack),strContent);
    
}



void CNetWorkDataSingle::DeleteTarget()
{
    
//    if(CNetWorkDataSingle::GetSingletonPtr()->getTarget() == m_pTarget)
//    {
        //    }
    
    if (!m_bIfHadCallBack) {   //变量未置，代表还没回调过就删除当前类了。
        CCLOG("netcallBack error ----%s",typeid(*m_pTarget).name());
    }

    CNetWorkDataSingle::GetSingletonPtr()->setTarget(NULL);
    m_bIfHadCallBack = true;

    
}


void CNetWorkDataSingle::OnCommonCallBack(Ref *pSender)
{
    if(m_pTarget&&m_pHandlerInfo)
    {
        m_bIfHadCallBack = true;
        (m_pTarget->*m_pHandlerInfo)(this);
        m_pTarget = NULL;
        m_pHandlerInfo= NULL;
        
    }
    
    
}





void CNetWorkDataSingle::Login(cocos2d::Ref *target, cocos2d::SEL_CallFuncO handler)
{
    m_pTarget = target;
    m_pHandlerInfo = handler;
    
    if (m_pTarget&&m_pHandlerInfo) {
        m_bIfHadCallBack=false;
    }
    
    //CNetControlModule::sharedNetControlModule()->Login(target,menu_selector(CNetWorkDataSingle::OnCommonCallBack));
}