//layer底层基类
#pragma once
#include "cocos2d.h"



#include "LayerManager.h"
#include "LYMenuItemSprite.h"

USING_NS_CC;


class CBaseLayer : public  LayerColor
{
public:
    CBaseLayer();
    virtual ~CBaseLayer();
    
    CREATE_FUNC(CBaseLayer);
    
    
    //init and touch event
    virtual bool init();
    
    virtual void onEnterTransitionDidFinish();
    
    virtual bool onTouchBegan(Touch* pTouch, Event* pEvent);
    virtual void onTouchMoved(Touch* pTouch, Event* pEvent);
    virtual void onTouchEnded(Touch* pTouch, Event* pEvent);
    virtual void onTouchCancelled(Touch* pTouch, Event* pEvent);
    
    
    
    //update
    
    
//    virtual void update(float dt);
    
//    //remove node
//    void removeSelf(Node* pNode);
//    //create menu
//    void createMenu(Node* node);
//    void createUnderBtn(Node* node);
//    void changeScene(Object* pSender);
//    //enable
//    void setEnableTrue();
    //type
//    void setType(layerType type);
//    layerType  getType() const;
protected:
    
    virtual void onBtnExit(Object* pSender);
    //enter and exit
    virtual void onExit();
    virtual void onEnter();
    
    //创建顶层和底层，可控制是否创建
//    Layer* CreateTopBottom(int iSceenIndex =-1,bool bCreateTop = true, bool bCreateBottom = true,bool bCreateAnnounce = true);
    
    //创建底层
//    Layer*  CreateBottom(int index);
protected:

    bool m_bParentTouch;
    cocos2d::Size m_winSize;
    CLayerManager* m_pLayerManager;
    Sprite* m_pBaseBg;
    bool m_bEnable;
    bool m_bShowStar;
    bool  m_bMoveStarVisible;
    cocos2d::ParticleSystemQuad * m_pTouchStar;
    cocos2d::ParticleSystemQuad * m_pMoveStar;
private:
    
    void moveStarVisibleFalse(float ft)
    {
        if (m_bShowStar) {
            if (m_pMoveStar && m_pMoveStar->getReferenceCount()!=0) {
                //        moveStar->setVisible(false);
                m_pMoveStar->setScale(0);
            }else
                m_pMoveStar = NULL;
        }
        
    }
    
    void moveStarVisibleTrue(float ft)
    {
        m_bMoveStarVisible=true;
    }
    
    
    void moveStarDelete(){
        
        if (m_bShowStar) {
            
            unschedule(schedule_selector(CBaseLayer::moveStarVisibleFalse));
            if (m_pMoveStar && m_pMoveStar->getReferenceCount()!=0) {
                m_pMoveStar->removeFromParentAndCleanup(true);
                m_pMoveStar =NULL;
            }else{
                m_pMoveStar=NULL;
            }
        }
    }

    
    CC_SYNTHESIZE(layerType, m_iLayerType, layerType);
    
//    int m_iLayerType;//层类型

};
