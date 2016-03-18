//
//  getNewHeroLayer.h
//  TheGodWill
//
//  Created by user on 15/2/11.
//
//

#ifndef __TheGodWill__getNewHeroLayer__
#define __TheGodWill__getNewHeroLayer__

#include "cocos2d.h"
#include "BaseLayer.h"
USING_NS_CC;

class CGetNewHeroLayer:public CBaseLayer
{
public:
    CGetNewHeroLayer();
    ~CGetNewHeroLayer();
    static CGetNewHeroLayer* create(int monsterId);
    bool init();
    
    void initUI();
    void onExit();
    void removeSelf();
    virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
    virtual void onTouchEnded(Touch *touch, Event *unused_event);
    
private:
    cocos2d::Size m_winSize;
    int m_monsterId;
    EventListenerTouchOneByOne* listen;
};

#endif /* defined(__TheGodWill__getNewHeroLayer__) */
