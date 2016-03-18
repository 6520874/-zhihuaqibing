//
//  TaskMgr.cpp
//  TheGodWill
//
//  Created by prefer_shan on 14/12/15.
//
//

#include "TaskMgr.h"

#include "btDBOper.h"

CTaskMgr* CTaskMgr::sharedTaskMgr()
{
    static CTaskMgr taskMgr;
    return &taskMgr;
}


CTaskMgr::CTaskMgr()
{
   
}

CTaskMgr::~CTaskMgr()
{
    
}


TaskBase *CTaskMgr::GetTaskById(int id)
{
    for(int i=0;i<m_totalTaskInfVec.size();i++)
    {
        if(id == m_totalTaskInfVec[i].m_iTaskId)
        {
            return &m_totalTaskInfVec[i];
        }
    }
    return NULL;
}


int CTaskMgr::GetTaskNumById(int id)
{
    for(int i=0;i<m_totalTaskInfVec.size();i++)
    {
        if(id == m_totalTaskInfVec[i].m_iTaskId)
        {
            return m_totalTaskInfVec[i].m_iMoney;
        }
    }
    return -1;
}


bool CTaskMgr::IsTaskHaveFinish(int id)
{
    for(int i=0;i<m_totalTaskInfVec.size();i++)
    {
        if(id == m_totalTaskInfVec[i].m_iTaskId)
        {
            return m_totalTaskInfVec[i].m_eTaskState==Task_Fininsh?true:false;
        }
    }
    return false;
}


bool CTaskMgr::IsTaskHaveGet(int id)
{
    for(int i=0;i<m_totalTaskInfVec.size();i++)
    {
        if(id == m_totalTaskInfVec[i].m_iTaskId)
        {
            return m_totalTaskInfVec[i].m_eTaskState==Task_Get?true:false;

        }
    }
    return false;
}

// 初始化基本装备列表
void CTaskMgr::initAllTaskBases()
{
    
}





