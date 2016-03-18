    //
//  LYLoadingLayer.cpp
//  TheGodWill
//
//  Created by user on 15/1/6.
//
//

#include "LYLoadingLayer.h"

#include "LYFightManager.h"
#include "CSharedPlist.h"

#include "DataManager.h"

#include "LYFightData.h"

#include <Base/CCAsyncTaskPool.h>

//#include <cocostudio/CCArmatureDataManager.h>

//using namespace cocostudio;

CLoadingLayer::CLoadingLayer()
{
    
    m_itheType = eLayer_error;
    CSharedPlist::getInstance()->pushPlist("plist/loading.plist");
    
    m_bIfLoadOver = false;
    
    targetVec =NULL;
}

CLoadingLayer::~CLoadingLayer()
{
    CSharedPlist::getInstance()->erasePlist("plist/loading.plist");
}

bool CLoadingLayer::init(){
    
    if (!CBaseLayer::init()) {
        return false;
    }
    
//    setColor(Color3B::BLACK);
    
    Size winsize = Director::getInstance()->getWinSize();
    
    m_pAnimateLayer = LayerColor::create(Color4B::BLACK);
//    addChild(tempLayer);
    
    Director::getInstance()->setNotificationNode(m_pAnimateLayer);
    
    m_pAnimateLayer->onEnter();
    
    CCLOG("enter fight Time");
    
    
    
    m_pGround = Sprite::createWithSpriteFrameName("loading_caodi.png");
    m_pGround->setPosition(Vec2(winsize.width*0.5f, winsize.height*0.5f));
    m_pAnimateLayer->addChild(m_pGround);
    
    
    m_pRider = Sprite::createWithSpriteFrameName("loading_1.png");
    
    Animate* theAnimate = CFightManager::getAnimateWithPath("loading_", 1, 11 , ".png"  ,0.07f);
    m_pRider->runAction(RepeatForever::create(theAnimate));
    m_pAnimateLayer->addChild(m_pRider);
    
    
    float targetX = m_pGround->getPositionX()-m_pGround->getBoundingBox().size.width*0.5f;
    float targetY = m_pGround->getPositionY()+m_pGround->getBoundingBox().size.height*0.5f+m_pRider->getBoundingBox().size.height*0.5f;
    
    m_pRider->setPosition(Vec2(targetX, targetY));
    
    
    m_pPercentLabel =  Label::createWithTTF("0%", "fonts/cuti.ttf", 40);
    m_pAnimateLayer->addChild(m_pPercentLabel);
    m_pPercentLabel->setPosition(Vec2(m_pGround->getPositionX(), m_pGround->getPositionY()-m_pGround->getBoundingBox().size.height-m_pPercentLabel->getBoundingBox().size.height));
    
    m_pPercentLabel->setColor(Color3B(36, 157, 192));
    
    
    
    //tips信息
    std::string tips = "Tips:" + CDataManager::getRandomTips();
    
    
    
    Label* tipsLabel = Label::createWithTTF(tips.c_str(), "fonts/cuti.ttf", 30);
    m_pAnimateLayer->addChild(tipsLabel);
    tipsLabel->setPosition(Vec2(m_pGround->getPositionX(), winsize.height*0.1f+tipsLabel->getBoundingBox().size.height*0.5f));
    tipsLabel->setColor(Color3B::WHITE);
    
//    Label* temp = Label::createWithSystemFont("loading..", "Arial", 50);
//    
//    
//    temp->setPosition(Vec2(winsize.width*0.5f, winsize.height*0.5f));
//    
//    m_pAnimateLayer->addChild(temp,100);
//    
//    temp->runAction(RepeatForever::create(RotateBy::create(0.5f, 30)));
    
    
    
    
    
    //如果没有需要加载的，则默认已经加载完毕，加载进度会跟随骑兵位置变化
//    if ((m_iMaxPlistNum+m_iMaxModelNum+m_iMaxPngNum)==0) {
//        
//        float runTime =1; //默认0.5秒动画时间
//        
//        m_bIfLoadOver = true;
//        
//        m_pRider->runAction(Sequence::create( MoveTo::create(runTime, Vec2(m_fOriginPosx+m_fMaxOffsetX, m_pRider->getPositionY())),CallFuncN::create(CC_CALLBACK_1(CLoadingLayer::changeLayer2, this)),NULL));
//        
//    }
    
    
    scheduleUpdate();
    
    
    
    return true;
}

void CLoadingLayer::setTheChanging(layerType theType, cocos2d::Node *parentNode, bool ifChangeScene,std::vector<std::string> tempparameters){
    
    
    m_itheType=theType;
    m_pParentNode=parentNode;
    m_bIfChangeScene=ifChangeScene;
    m_vParameters =  tempparameters;
    
    getPngVec();
    
    
}



void CLoadingLayer::getPngVec(){
    
    
    if (m_itheType==eLayer_Fight) {  //如果是战斗场景，则需要加载所有用到的模型图片
        
        m_vecModels = CFightData::getInstance()->getNeededPngPath(true);
        m_vecPngs = CFightData::getInstance()->getNeededPngPath(false);
    }
    
    
    
}




void CLoadingLayer::onEnterTransitionDidFinish(){
    
    CBaseLayer::onEnterTransitionDidFinish();
    
    CFightManager::removeUnusedMemory();
    
    
    
    m_fOriginPosx = m_pRider->getPositionX();
    
    m_fMaxOffsetX = m_pGround->getBoundingBox().size.width-m_pRider->getBoundingBox().size.width*0.5f;
    m_fNowOffsetX=0;
    
    //加载所需要更换的界面的plist
    
    targetVec = CLayerManager::GetSingletonPtr()->getPlistsByLayerType(m_itheType);
    
    if (!targetVec&&m_vecPngs.size()==0&&m_vecModels.size()==0) {//如果取不到plist，则直接加载,跑个骑兵动画后直接跳转
//        m_pRider->setPositionX(m_fOriginPosx+m_fMaxOffsetX);
//        scheduleOnce(schedule_selector(CLoadingLayer::changeLayer), 0);
        
        float runTime =0.2; //默认0.5秒动画时间
        
        m_bIfLoadOver = true;
        
        m_pRider->runAction(Sequence::create( MoveTo::create(runTime, Vec2(m_fOriginPosx+m_fMaxOffsetX, m_pRider->getPositionY())),CallFuncN::create(CC_CALLBACK_1(CLoadingLayer::changeLayer2, this)),NULL));
        return;
    }
    
    m_iMaxPlistNum=0;
    m_iNowPlistNum=0;
    
    if (targetVec) {
        m_iMaxPlistNum = targetVec->size();
        m_iNowPlistNum = 0;
    }
    
    
    
    m_iMaxPngNum = m_vecPngs.size();
    m_iNowPngNum = 0;
    
    m_iMaxModelNum = m_vecModels.size();
    m_iNowModelNum = 0;
    
    
    if(targetVec){
        
        //异步加载plist纹理
        for (long i = m_iMaxPlistNum-1; i>=0; i--) {
            
            std::string thePath = targetVec->at(i);
            
            long offset = thePath.find(".plist");
            
            thePath = thePath.substr(0,offset);
            
            Director::getInstance()->getTextureCache()->addImageAsync(thePath+".pvr.ccz", CC_CALLBACK_1(CLoadingLayer::loadPlistCallBack, this));
            
        }

    }
    
    if (m_vecPngs.size()>0) {
        
        for (long i = m_iMaxPngNum-1; i>=0; i--) {
            
            std::string thePath = m_vecPngs.at(i);

            Director::getInstance()->getTextureCache()->addImageAsync(thePath, CC_CALLBACK_1(CLoadingLayer::loadPngCallBack, this));

        }

    }
    
    
    AsyncTaskPool::getInstance()->stopTasks(AsyncTaskPool::TaskType::TASK_IO);
    
    if (m_vecModels.size()>0) {
        
        for (long i = m_iMaxModelNum-1; i>=0; i--) {
            
            std::string thePath = m_vecModels.at(i);
            
            Sprite3D::createAsync(thePath, CC_CALLBACK_2(CLoadingLayer::asyncLoad_Callback, this), NULL);
            
        }
        
    }
    
    
    
    
    
    
    
    
    
    
    
    
    
    
//    float changeTime = 2;
//    
//    scheduleOnce(schedule_selector(CLoadingLayer::changeLayer), changeTime);
//    
//    float distance = m_pGround->getBoundingBox().size.width-m_pRider->getBoundingBox().size.width*0.5f;
//    
//    m_pRider->runAction(MoveBy::create(changeTime, Vec2(distance, 0)));
    

}

void CLoadingLayer::update(float time){
    
    //每帧向目标点移动1/10,最小移动30
    
    
//    float minDist =  MIN(m_fMaxOffsetX* m_iNowPlistNum*1.0f/ m_iMaxPlistNum - m_fNowOffsetX , 30); //取最小位移，如果和目标点距离大于30，则取30
    
    
    float percent = 0;
    
    if (!m_bIfLoadOver) { //没完成laoding的时候，慢慢走
        float dis = MIN(m_fMaxOffsetX* (m_iNowPlistNum+m_iNowPngNum+m_iNowModelNum) *1.0f/ (m_iMaxPlistNum + m_iMaxPngNum+m_iMaxModelNum) *0.02f, m_fMaxOffsetX* (m_iNowPlistNum+m_iNowPngNum+m_iNowModelNum) *1.0f/ (m_iMaxPlistNum + m_iMaxPngNum+m_iMaxModelNum) - m_fNowOffsetX);
        
        
        
        m_fNowOffsetX+=dis;
        
        m_pRider->setPositionX(m_pRider->getPositionX()+dis);
        
        
        percent = m_fNowOffsetX/m_fMaxOffsetX*100;
        
        
    }else{ //完成loading的时候，按action来走
        
        percent = m_pRider->getPositionX()/(m_fOriginPosx+m_fMaxOffsetX)*100;
        
    }
    
    
    percent= MIN(percent, 99);
    
    
    __String* tempStr = __String::createWithFormat("Loading...%.0f%%",percent);
    
    m_pPercentLabel->setString(tempStr->getCString());
    
    
    
    
}



void CLoadingLayer::loadPlistCallBack(cocos2d::Texture2D *texture){
    
    m_iNowPlistNum++;
    
    
//    std::string thePath = targetVec->at(m_iMaxPlistNum-m_iNowPlistNum);
//    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(thePath,texture);
    
    
    if (m_iNowPlistNum>=m_iMaxPlistNum) {
        for (long i = m_iMaxPlistNum-1; i>=0; i--) {
            
            std::string thePath = targetVec->at(i);
            
            
            long offset = thePath.find(".plist");
            
            std::string PvrPath = thePath.substr(0,offset);
            
            CSharedPlist::getInstance()->pushPlist(thePath);
            
        }
        
        
//        //加载完plist，开始加载png
//
//        
//        for (long i = m_iMaxPngNum-1; i>=0; i--) {
//            
//            std::string thePath = m_vecPngs.at(i);
//            
//            
//            if (m_itheType==eLayer_Fight) {  //如果是战斗场景，则需要加载所有用到的模型图片
//                
//                Sprite3D::createAsync(thePath, CC_CALLBACK_2(CLoadingLayer::asyncLoad_Callback, this), NULL);
//                
//                
//            }else{
//                
//                Director::getInstance()->getTextureCache()->addImageAsync(thePath, CC_CALLBACK_1(CLoadingLayer::loadPngCallBack, this));
//            }
//            
//            
//            
//            
//        }
//        
//        if (m_iMaxPngNum<=0) {
//            loadPngCallBack(NULL);
//        }
        
        ifOverAndChange();
        
    }
    
    
    
    
}


void CLoadingLayer::asyncLoad_Callback(cocos2d::Sprite3D *sprite, void *param){
    
    m_iNowModelNum++;
    
    if (m_iNowModelNum>=m_iMaxModelNum) {
        
        ifOverAndChange();

    }
    
}



void CLoadingLayer::loadPngCallBack(cocos2d::Texture2D *texture){
    
    m_iNowPngNum++;
    
    if(m_iNowPngNum>=m_iMaxPngNum){
        
        ifOverAndChange();

    }
    
    
}

bool CLoadingLayer::ifOverAndChange(){
    
    
    
    
    if ((m_iNowPngNum+m_iNowModelNum+m_iNowPlistNum)>=(m_iMaxPngNum+m_iMaxModelNum+m_iMaxPlistNum)) {
        float overTime = 0.1f;

        //加载完后，就不用update来更新动画了，这样不能保证终点时100%，而使用action来更新动画
        m_bIfLoadOver = true;

        m_pRider->runAction(Sequence::create( MoveTo::create(overTime, Vec2(m_fOriginPosx+m_fMaxOffsetX, m_pRider->getPositionY())),CallFuncN::create(CC_CALLBACK_1(CLoadingLayer::changeLayer2, this)),NULL));

        //        scheduleOnce(schedule_selector(CLoadingLayer::changeLayer), overTime);
        return true;
    }
    
    return false;
}



void CLoadingLayer::changeLayer(float time){
    
    
    if (m_itheType!=eLayer_error) {
        
        CLayerManager::GetSingletonPtr()->createLayer(m_itheType, m_pParentNode,m_bIfChangeScene,&m_vParameters);
    
    }
    
}

void CLoadingLayer::changeLayer2(cocos2d::Ref *pSender){ //为了能用callfuncN，并且在动作结束前置为100%
    
    m_pPercentLabel->setString("Loading...99%");
    m_pRider->setPositionX(m_fOriginPosx+m_fMaxOffsetX);
    
    scheduleOnce(schedule_selector(CLoadingLayer::changeLayer), 0);
    
}


void CLoadingLayer::onExit(){
    
    CBaseLayer::onExit();
    
    m_pAnimateLayer->onExit();
    
    
    Director::getInstance()->setNotificationNode(NULL);
    
}




