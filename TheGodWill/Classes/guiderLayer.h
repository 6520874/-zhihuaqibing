//
//  guiderLayer.h
//  TheGodWill
//
//  Created by user on 15/1/28.
//
//

#ifndef __TheGodWill__guiderLayer__
#define __TheGodWill__guiderLayer__

#include <stdio.h>
#include "AllDefine.h"
#include "BaseLayer.h"
#include "DataManager.h"

enum GuideType
{
    GUIDE_SHOW_DIALOG = 0,//只显示对话
    GUIDE_SHOW_FINGER,//只显示指引
    GUIDE_SHOW_BOTH,//两者都显示
    GUIDE_SHOW_WND,//完美度介绍
};

class guiderLayer: public CBaseLayer
{
public:
    guiderLayer();
    ~guiderLayer();
    
    //CREATE_FUNC(guiderLayer);
    static guiderLayer* create(GuideType type = GUIDE_SHOW_DIALOG, Vec2 pos = Vec2::ZERO , bool ifShadow = true);
    bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
    void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event);
    bool init();
    void showGuideUI();
    void showDialog();
    void showFinger();
    
    void onExit();
    void showFingerGuide(Vec2 pos, bool isBtn = true);
    void solideFinger(Ref *pSender);
    void setTouchValid();
    
    void initPerfectIntroduce();
    void btnCallBack(Ref* pSender);
    
    
    void touchableInit();// 用于每个引导的 触摸传递初始化 by ly
    
private:
    int m_type;
    cocos2d::Size m_winSize;
    EventListenerTouchOneByOne* listener;
    Sprite* m_stencil;//模板
    Label* m_dialogue;
    std::vector<guideInfo> m_guideInfo;
    bool m_bStencial;
    LayerColor* m_back;
    Sprite* m_finger;
    ClippingNode* m_clip;
    Sprite* m_girl;
    Sprite* m_diagBg;
    GuideType m_showType;
    cocos2d::Vec2 m_pos;
    Node* m_node;
    bool m_bTouchValid;//标识触摸是否有效
    bool m_bRomoveFlag;//删除引导标识，防止出现触摸向下传递
    
    bool m_bIfShadow;//是否背景暗掉
    
    int m_iTouchState;// 0表示无规则， -1表示无法响应触摸，就算是遮罩框以内也不响应。   1表示任何触摸都会透过去，无论有没有遮罩框
    
    
    
};

#endif /* defined(__TheGodWill__guiderLayer__) */
