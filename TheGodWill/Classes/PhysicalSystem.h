//
//  PhysicalSystem.h
//  TheGodWill
//
//  Created by prefer_shan on 14/12/10.
//
//

#ifndef __TheGodWill__PhysicalSystem__
#define __TheGodWill__PhysicalSystem__

#include <stdio.h>
#include "cocos2d.h"
#include "AllDefine.h"
#include "LYFightManager.h"

USING_NS_CC;


//体力系统
class CPhysicalSystem:public cocos2d::Node
{
    
public:
    static CPhysicalSystem* GetSingletonPtr();
    
    CPhysicalSystem();
    ~CPhysicalSystem();
   
    bool init();
    void SavePhysicalTime(long sec=0);  //无参数时保存默认时间，否则参数时间
    
    CREATE_FUNC(CPhysicalSystem);
    
    //消耗体力  cost负值-  正值+
    void costManual(int cost,cocos2d::Ref* pTarget,cocos2d::SEL_MenuHandler pHandlerSucess,cocos2d::SEL_MenuHandler pHandlerFail);
    int GetNextUpTime();
    
private:
    void OnSetPhysicalNum(Ref *pSender);
    
    //获取当前系统时间的秒数
    int  GetNowTimeBySec();
    
//    int m_iPhysicalNum;

    long m_iCurSecond; //当前时间转换为秒数

    int   m_iCurTmpSec;
    int  m_iTmpCurSec;
    
    cocos2d::Ref* m_pTarget;
    cocos2d::SEL_MenuHandler  m_pHander1;
    cocos2d::SEL_MenuHandler  m_pHander2;
    Label * m_pTime;
    Label * m_pLablePhpysicalNum;
    
    Label * m_pLabelRemain;
    cocos2d::Size  m_winSize;
    static CPhysicalSystem *t;
};
#endif /* defined(__TheGodWill__PhysicalSystem__) */
