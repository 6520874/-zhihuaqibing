//
//  BaseScene.cpp
//  TheGodWill
//
//  Created by user on 14/12/11.
//
//

#include "BaseScene.h"
#include "zzbNetControlModule.h"
#include "zzbNewNetStruct.h"

CBaseScene::CBaseScene(){
    
    m_time = 0;
}

CBaseScene::~CBaseScene(){
    
    
}

bool CBaseScene::init()
{
    if(!Scene::init())
    {
        return false;
    }
    CNetControlModule::sharedNetControlModule()->updateUserOnLineTime(this, NULL);
    scheduleUpdate();
    return true;
}

void CBaseScene::update(float dt)
{
    if(m_time >= 120)//两分钟请求一次
    {
        m_time -= 120;
        CNetControlModule::sharedNetControlModule()->updateUserOnLineTime(this, NULL);
    }
    else
    {
        m_time += dt;
    }
}

void CBaseScene::addLayer(CBaseLayer *newOne){
    
    addChild(newOne);
    
    m_VecLayers.pushBack(newOne);
    
}

void CBaseScene::removeLayer(CBaseLayer *deleteOne){
    
    removeChild(deleteOne);
    
    m_VecLayers.eraseObject(deleteOne);
    
}

void CBaseScene::removeAllLayer(){
    
    
    for (const auto &thelayer : m_VecLayers)
    {
        
        thelayer->removeFromParentAndCleanup(true);
        
        m_VecLayers.eraseObject(thelayer);
    }
    
    
//    for (int i=0; i<m_VecLayers.size(); i++) {
//        
//        CBaseLayer* thelayer = m_VecLayers.at(i);
//        
//        thelayer->removeFromParentAndCleanup(true);
//        
//    }
//    
//    m_VecLayers.clear();
    
    
    
    
}