//
//  NetWorkDataSingle.h
//  TheGodWill
//
//  Created by prefer_shan on 14/12/18.
//
//

#ifndef __TheGodWill__NetWorkDataSingle__
#define __TheGodWill__NetWorkDataSingle__

#include <stdio.h>
#include "cocos2d.h"

USING_NS_CC;


class CNetWorkDataSingle:public Node
{
public:
    static CNetWorkDataSingle* GetSingletonPtr();
   
    void Login(cocos2d::Ref *target, cocos2d::SEL_CallFuncO handler);
    
    //保存魔法石体力金币接口
    void SaveUserMCF(cocos2d::Ref *target, cocos2d::SEL_CallFuncO handler,int iCoinAdd,int iStoneAdd,int iStaminAdd);
    //更新玩家名字
    void updateUserName(cocos2d::Ref *target, cocos2d::SEL_CallFuncO handler,std::string strContent);
    void DeleteTarget();
    

    bool GetDrawCardInfo(cocos2d::Ref * pTarget,cocos2d::SEL_CallFuncO phandler);
    
    
    void SaveDinshiTime(cocos2d::Ref *target, cocos2d::SEL_CallFuncO handle,int iUserBuildId,int iTime);
    
     void SaveDrawCardInfo(cocos2d::Ref * pTarget,cocos2d::SEL_CallFuncO phandler,int iType,int iDrawNum);
    

private:
    CNetWorkDataSingle();
    ~CNetWorkDataSingle();
   
    
    cocos2d::SEL_CallFuncO m_pHandlerInfo;//获得的回调
     CC_SYNTHESIZE(cocos2d::Ref*,m_pTarget,Target);//回调函数的目标
    
    void OnCommonCallBack(Ref *pSender);
    

    bool m_bIfHadCallBack;//是否已经调用过回调。用于判断是否未经过网络回调就删除了目标类
    
    
    
    static CNetWorkDataSingle *t;
    
    
};
#endif /* defined(__TheGodWill__NetWorkDataSingle__) */
