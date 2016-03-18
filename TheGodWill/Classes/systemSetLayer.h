//
//  systemSetLayer.h
//  TheGodWill
//
//  Created by user on 15/2/16.
//
//

#ifndef __TheGodWill__systemSetLayer__
#define __TheGodWill__systemSetLayer__

#include <stdio.h>
#include "BaseLayer.h"

class CSystemSetLayer:public CBaseLayer
{
public:
    CSystemSetLayer();
    ~CSystemSetLayer();
    virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
    bool init();
    void initUI();
    
    CREATE_FUNC(CSystemSetLayer);
    void btnCallBack(Ref* Psender);
private:
    Size m_winSize;
};


#endif /* defined(__TheGodWill__systemSetLayer__) */
