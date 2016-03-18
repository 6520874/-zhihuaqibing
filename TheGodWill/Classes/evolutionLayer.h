//
//  CEvolutionLayer.h
//  WarOfGod
//
//  Created by user on 15/8/20.
//
//

#ifndef __WarOfGod__CEvolutionLayer__
#define __WarOfGod__CEvolutionLayer__

#include <stdio.h>

#include "cocos2d.h"
#include "BaseLayer.h"
USING_NS_CC;

class CEvolutionLayer:public CBaseLayer
{
public:
    CEvolutionLayer();
    ~CEvolutionLayer();
    CREATE_FUNC(CEvolutionLayer);
    bool init();
    bool onTouchBegan(Touch *touch, Event *unused_event);
private:
    Size m_winWize;
    EventListenerTouchOneByOne* m_pListen;
    bool m_bEvolutionEnd;//是否进化动作结束
};


#endif /* defined(__WarOfGod__CEvolutionLayer__) */
