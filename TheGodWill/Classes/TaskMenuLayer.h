//
//  TaskMenuLayer.h
//  TheGodWill
//
//  Created by prefer_shan on 14/12/22.
//
//

#ifndef __TheGodWill__TaskMenuLayer__
#define __TheGodWill__TaskMenuLayer__

#include <stdio.h>
#include "cocos2d.h"
#include "BaseLayer.h"

USING_NS_CC;


class CTaskMenuLayer:public CBaseLayer
{
public:
    CREATE_FUNC(CTaskMenuLayer);
private:
    CTaskMenuLayer();
    ~CTaskMenuLayer();
    bool init();
    
};
#endif /* defined(__TheGodWill__TaskMenuLayer__) */
