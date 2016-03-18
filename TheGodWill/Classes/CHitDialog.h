//
//  CHitDialog.h
//  TheGodWill
//
//  Created by prefer_shan on 15/1/4.
//
//

#ifndef __TheGodWill__CHitDialog__
#define __TheGodWill__CHitDialog__

#include <stdio.h>
#include "cocos2d.h"
#include "BaseLayer.h"
USING_NS_CC;


class CHitDialog:public CBaseLayer
{
     typedef enum
    {
        e_Common = 0,
        e_OpenUrl= 1,
        
    }e_DialogType;

    
public:
    CREATE_FUNC(CHitDialog);
    void CreateDialog(std::string strText,Ref * pTarget,cocos2d::SEL_MenuHandler pHandler);
    void createYesNO(Ref * pTarget,cocos2d::SEL_MenuHandler pHandler,std::string sContent,cocos2d::SEL_MenuHandler pHandlerNo=NULL);
    
    void CreateUrlDialog(std::string sContent,std::string strUrl,cocos2d::Ref * pTarget=NULL,cocos2d::SEL_MenuHandler pHandlerYes=NULL,cocos2d::SEL_MenuHandler pHandlerNo=NULL,e_DialogType  t=e_OpenUrl);
    
    
    void CreateHuifanDialog(cocos2d::Ref * pTarget=NULL,cocos2d::SEL_MenuHandler pHandlerYes=NULL,cocos2d::SEL_MenuHandler pHandlerNo=NULL);
    
    void CreateFightExitDialog(cocos2d::Ref * pTarget,cocos2d::SEL_MenuHandler pHandlerYes,cocos2d::SEL_MenuHandler pHandlerNo);
    
    
private:
    CHitDialog();
    ~CHitDialog();
    bool onTouchBegan(Touch *pTouch, Event *pEvent);
    void onTouchMoved(Touch *pTouch, Event *pEvent);
    void onTouchEnded(Touch *pTouch, Event *pEvent);
    void onTouchCancelled(Touch *touch, Event *pEvent);

    bool init();
    
    void onExit();
  
    void OnExitLayer(Ref *pSender);

    
    void OnYes(Ref *pSender);
    void OnNo(Ref *pSender);
    Menu * m_pMenuHitDialog;
    Menu * m_pMenuYes;
    Menu * m_pMenuNo;
    
    bool m_bMenuYes;
    bool m_bMenuNo;
    bool m_bMenuHitDialog;
    EventListenerTouchOneByOne* m_pListner;
    cocos2d::Ref * m_pTarget;
    e_DialogType  m_eDType;
    std::string m_strUrl;
    cocos2d::SEL_MenuHandler m_pHandler;
    cocos2d::SEL_MenuHandler m_pHandler2;
    cocos2d::Size m_winSize;
    
};


#endif /* defined(__TheGodWill__CHitDialog__) */
