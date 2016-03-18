//
//  BeName.h
//  WarOfGod
//
//  Created by prefer_shan on 15/4/14.
//
//

#ifndef __WarOfGod__BeName__
#define __WarOfGod__BeName__

#include <stdio.h>
#include "BaseLayer.h"

//命名layer
class CBeNameLayer:public CBaseLayer,public TextFieldDelegate
{
    CBeNameLayer(int index); //0为登陆
    ~CBeNameLayer();
    void OnYaoShaizi(Ref *pSender);
    void OnOpenInput(Ref *pSender);
    
    void onExit();
    void OnSure(Ref *pSender);
    void OnCancle(Ref *pSender);
    
    EventListenerTouchOneByOne * m_pListener;
    Sprite * m_pInput;
    Sprite * m_pBg;
    std::string   m_sTalkContent;
    TextFieldTTF  * m_pTalkText;
    cocos2d::Size  m_szInputBox;
    std::vector<__String>  m_AdjVec; //名字形容词
    std::vector<__String>  m_NameVec;//名字本身

    int m_index;
    
public:
    bool onTouchBegan(Touch *touch, Event *event);
    void onTouchMoved(Touch *touch, Event *event) ;
    void onTouchEnded(Touch *touch, Event *event);
    void onTouchCancelled(Touch *touch, Event *event);

    void OnUpdateSure(Ref *pSender);
    
    virtual bool onTextFieldAttachWithIME(TextFieldTTF * sender);
    virtual bool onTextFieldDetachWithIME(TextFieldTTF * sender);
    virtual bool onTextFieldInsertText(TextFieldTTF * sender, const char * text, int nLen);
    virtual bool onTextFieldDeleteBackward(TextFieldTTF * sender, const char * delText, int nLen);
    
    //CREATE_FUNC(CBeNameLayer);
    
    static CBeNameLayer* create(int index) \
    { \
        CBeNameLayer *pRet = new(std::nothrow) CBeNameLayer(index); \
        if (pRet && pRet->init()) \
        { \
            pRet->autorelease(); \
            return pRet; \
        } \
        else \
        { \
            delete pRet; \
            pRet = NULL; \
            return NULL; \
        } \
    }
    
    bool init();
    
};
#endif /* defined(__WarOfGod__BeName__) */
