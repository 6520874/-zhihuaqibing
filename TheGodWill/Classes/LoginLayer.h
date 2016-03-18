//
//  LoginLayer.h
//  TheGodWill
//
//  Created by prefer_shan on 14/12/19.
//
//

#ifndef __TheGodWill__LoginLayer__
#define __TheGodWill__LoginLayer__

#include <stdio.h>
#include "BaseLayer.h"

class CLoginLayer:public CBaseLayer
{
public:
    CREATE_FUNC(CLoginLayer);

    static cocos2d::Scene* createScene();
    
    bool onTouchBegan(Touch *touch, Event *event);
    void onTouchMoved(Touch *touch, Event *event) ;
    void onTouchEnded(Touch *touch, Event *event);
    void onTouchCancelled(Touch *touch, Event *event);
    
private:
    void update(float ft);
    Sprite * m_pBig;
    Sprite * m_pBig2;
    Sprite * m_pBig3;
    
    Label * m_pTextInfo;
    Label * m_pTextAuthor;
    void OnFadeOut();
    void OnFadeIn();
    
    void  UpdateTime1(float ft);
    void  UpdateTime2(float ft);
    
    bool init();
    CLoginLayer();
    ~CLoginLayer();
    void onExit();
    
    int m_iIndex;
    int m_iAutorIndex;
    std::string m_strText;
    std::string m_sAuthor;

    float  m_lBeginTime;
    float  m_lAutorTime;
    
    EventListenerTouchOneByOne * m_pListener;
    
    int m_iChangeState;  //当前切换状态
    
    
};
#endif /* defined(__TheGodWill__LoginLayer__) */
