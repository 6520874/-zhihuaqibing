//
//  BaseScene.h
//  TheGodWill
//
//  Created by user on 14/12/11.
//
//

#ifndef __TheGodWill__BaseScene__
#define __TheGodWill__BaseScene__

#include <stdio.h>

#include "cocos2d.h"

#include "BaseLayer.h"


class CBaseScene: public cocos2d::Scene {
    
public:
    
    CBaseScene();
    ~CBaseScene();
    
    
    CREATE_FUNC(CBaseScene);
    virtual bool init();
    void update(float dt);
    void removeAllLayer();
    
    void addLayer(CBaseLayer* newOne);
    void removeLayer(CBaseLayer* deleteOne);
    
    
    
    
protected:
    
    cocos2d::Vector<CBaseLayer*> m_VecLayers;
    float m_time;
    
    
};

#endif /* defined(__TheGodWill__BaseScene__) */
