#include "BaseLayer.h"


#include "LayerManager.h"

#include "AllDefine.h"

#include "LYFightManager.h"

#include "NetWorkDataSingle.h"

CBaseLayer::CBaseLayer()
: m_iLayerType(eLayer_error)
, m_bParentTouch(false)
, m_pBaseBg(NULL)
, m_bEnable(false)
{
    m_pMoveStar=NULL;
    m_bMoveStarVisible=false;
    m_pTouchStar = NULL;
    m_bShowStar = true;
    m_winSize = Director::getInstance()->getWinSize();
//    m_pSceneManager = CSceneManager::GetSingletonPtr();
    m_pLayerManager = CLayerManager::GetSingletonPtr();



}

CBaseLayer::~CBaseLayer()
{
    //删掉网络回调
    

    //CLayerManager::delLayerType();
}

bool CBaseLayer::init()
{
    
    if (!LayerColor::init()) {
        return false;
    }
    LayerColor::init();
//
//    setlayerType(type);
    

    return true;
}



//void CBaseLayer::changeScene(Object* pSender)
//{
//    MenuItem* pItem = (MenuItem*)pSender;
//    long sceneTag = (long)pItem->getUserData();
//    m_pSceneManager->changeScene(sceneTag);
//    m_bEnable = false;
//}


void CBaseLayer::onExit()
{
    if(CNetWorkDataSingle::GetSingletonPtr()->getTarget() == this)
    {
        
        CNetWorkDataSingle::GetSingletonPtr()->DeleteTarget();
        
        //CNetWorkDataSingle::GetSingletonPtr()->setTarget(NULL);
        //CCLOG("netcallBack error ----%s",typeid(this).name());
        

    }
    
    
    LayerColor::onExit();

}

void CBaseLayer::onEnter()
{
    LayerColor::onEnter();
}


//void CBaseLayer::setEnableTrue()
//{
//    m_bEnable = true;
//}

//void CBaseLayer::update(float dt)
//{
//
//}



bool CBaseLayer::onTouchBegan(Touch* pTouch, Event* pEvent)
{
//    m_bParentTouch = m_pParentMenu->onTouchBegan(pTouch, pEvent);
    if (m_bShowStar)
    {
        Point touchPos = CCDirector::getInstance()->convertToGL(pTouch->getLocationInView());
        
        m_pTouchStar = CCParticleSystemQuad::create("particleSystem/touchStar.plist");
        m_pTouchStar->setPosition(touchPos);
        m_pTouchStar->setAutoRemoveOnFinish(true);
        addChild(m_pTouchStar,9999999);
        
        
        if (m_pMoveStar&&m_pMoveStar->getReferenceCount()!=0) {
            unschedule(schedule_selector(CBaseLayer::moveStarVisibleTrue));
            scheduleOnce(schedule_selector(CBaseLayer::moveStarVisibleTrue), 0.15f);
            //        moveStar = CCParticleSystemQuad::create("particleSystem/moveStar.plist");
            m_pMoveStar->setPosition(touchPos);
            //        moveStar->setVisible(false);
            //        addChild(moveStar,9999999);
        }
        
    }


    return true;
}

void CBaseLayer::onTouchMoved(Touch* pTouch, Event* pEvent)
{
    if (m_bParentTouch)
//        m_pParentMenu->onTouchMoved(pTouch, pEvent);
    
    if (m_bShowStar) {
        
        Point touchPos = CCDirector::getInstance()->convertToGL(pTouch->getLocationInView());
        
        unschedule(schedule_selector(CBaseLayer::moveStarVisibleFalse));
        
        if(m_pMoveStar&& m_pMoveStar->getReferenceCount()!=0){//有卡牌测设置位置，无卡牌则重新创建卡牌
            m_pMoveStar->setPosition(touchPos);
            //        moveStar->setVisible(true);
            
            if (m_bMoveStarVisible==true) {
                m_pMoveStar->setScale(1);
                m_pMoveStar->setVisible(true);
            }
            
        }else{
            m_pMoveStar = CCParticleSystemQuad::create("particleSystem/moveStar.plist");
            m_pMoveStar->setPosition(touchPos);
            m_pMoveStar->setVisible(true);
            addChild(m_pMoveStar,9999);
        }
        
        scheduleOnce(schedule_selector(CBaseLayer::moveStarVisibleFalse), 0.15f);
        
    }

}

void CBaseLayer::onTouchEnded(Touch* pTouch, Event* pEvent)
{
    if (m_bParentTouch)
//        m_pParentMenu->onTouchEnded(pTouch, pEvent);
    
    m_bParentTouch = false;
    
    
}

void CBaseLayer::onTouchCancelled(Touch* pTouch, Event* pEvent)
{
    if (m_bParentTouch)
//        m_pParentMenu->onTouchCancelled(pTouch, pEvent);
     m_bMoveStarVisible=false;
     m_bParentTouch = false;
}

//void CBaseLayer::removeSelf(Node* pNode)
//{
//    CLayerManager::GetSingletonPtr()->popLayer();
//    pNode->removeFromParent();
//}

void CBaseLayer::onBtnExit(Object* pSender)
{
//    removeSelf(this);
    removeFromParentAndCleanup(true);
}

void CBaseLayer::onEnterTransitionDidFinish(){
    
    LayerColor::onEnterTransitionDidFinish();
    
    CFightManager::getMemoryList();
}

