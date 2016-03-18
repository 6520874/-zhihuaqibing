//
//  BaseDrawLayer.h
//  TheGodWill
//
//  Created by prefer_shan on 14/11/27.
//
//

#ifndef __TheGodWill__BaseDrawLayer__
#define __TheGodWill__BaseDrawLayer__

#include <stdio.h>
#include "cocos2d.h"

using namespace cocos2d;

class BaseDrawLayer:public Layer
{
public:
    BaseDrawLayer();
    ~BaseDrawLayer(void);
    virtual void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags);
    
    //LAYER_CREATE_FUNC(BaseDrawLayer);
    CREATE_FUNC(BaseDrawLayer);
};
#endif /* defined(__TheGodWill__BaseDrawLayer__) */
