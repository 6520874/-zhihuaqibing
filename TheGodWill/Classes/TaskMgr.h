//
//  TaskMgr.h
//  TheGodWill
//
//  Created by prefer_shan on 14/12/15.
//
//

#ifndef __TheGodWill__TaskMgr__
#define __TheGodWill__TaskMgr__

#include <stdio.h>
#include "cocos2d.h"
#include "AllDefine.h"
USING_NS_CC;

//任务逻辑
class CTaskMgr:public Node
{
public:
    static CTaskMgr* sharedTaskMgr();
    
    // 初始化基本任务列表
    void initAllTaskBases();
    std::vector<TaskBase>  m_totalTaskInfVec;
    
private:
    TaskBase *GetTaskById(int id);
    int  GetTaskNumById(int id);
    bool IsTaskHaveFinish(int id);
    bool IsTaskHaveGet(int id);

    CTaskMgr();
    ~CTaskMgr();
};
#endif /* defined(__TheGodWill__TaskMgr__) */
