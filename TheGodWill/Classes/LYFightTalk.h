//
//  LYFightTalk.h
//  TheGodWill
//
//  Created by user on 15/3/6.
//
//

#ifndef __TheGodWill__LYFightTalk__
#define __TheGodWill__LYFightTalk__

#include <stdio.h>

#include "cocos2d.h"

#include "DBMgr.h"

#include "LYBaseFighter.h"

class CFightTalk : public cocos2d::Node{
    
public:
    
    CREATE_FUNC(CFightTalk);
    
    CFightTalk();
    ~CFightTalk();
    
    
    void setTheInfo(fightTalkingInfo* theInfo,CBaseFighter* target);
    
    void update(float time);
    
protected:
    
    
    CBaseFighter* m_pTheFighter;
    
//    cocos2d::Sprite* m_pTheBG;
    
    CC_SYNTHESIZE(cocos2d::Sprite*, m_pTheBG, TheBG);
    
    cocos2d::Sprite* m_pExpression;
    
    
    CC_SYNTHESIZE(cocos2d::Label*, m_pTheContent, TheContent);
    
    float m_fNowTime;
    
    static int talkNumber;
    
//    cocos2d::Label* m_pTheContent;
    
    
};




#endif /* defined(__TheGodWill__LYFightTalk__) */
