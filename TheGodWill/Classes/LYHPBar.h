//
//  LYHPBar.h
//  TheGodWill
//
//  Created by user on 14/12/3.
//
//

#ifndef __TheGodWill__LYHPBar__
#define __TheGodWill__LYHPBar__

#include <stdio.h>


#include "cocos2d.h"

#define VISIBLE_ACTION_TAG 10


class CHPBar : public cocos2d::Sprite3D {
    
public:
    
    CREATE_FUNC(CHPBar);
    
    void setHPSpriteWithPath(const char *HPName,const char *HPBGName);
    
    void toHPScale(float scale,float time =0);
    
    
protected:
    
    CC_SYNTHESIZE(cocos2d::Sprite*, m_pNowHP, SpriteNowHP);
    CC_SYNTHESIZE(cocos2d::Sprite*, m_pBGHP, BGHP);
    
    cocos2d::Sprite* m_pHPSprite;
    

};



#endif /* defined(__TheGodWill__LYHPBar__) */
