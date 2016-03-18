//
//  LYBeatNumbers.h
//  mofaxueyuan
//
//  Created by user on 13-10-15.
//
//

#ifndef __mofaxueyuan__LYBeatNumbers__
#define __mofaxueyuan__LYBeatNumbers__

#include <iostream>
#include "cocos2d.h"

using namespace cocos2d;

class CBeatNumbers :public Node{
    
public:
    
    static CBeatNumbers *createWithNumberAndSize(int number,int size,int defaultNumber=0,LabelAtlas *numberLabel=NULL);
    bool initWithNumberAndSize(int number,int size,int defaultNumber=0,LabelAtlas *numberLabel=NULL);
    void BeatDownTo(int min=0);
    void BeatUpTo(int max,int start=0);
    void setColr(Color3B color){m_pNumberLabel->setColor(color);};
    void setBeatTime(float temp){beatTime = temp;};
    
    void setMaxNumber(int temp){MaxNumber = temp;};
    
    void setNumberCorlor(Color3B corlor);
    
    
    LabelAtlas *m_pNumberLabel;
protected:
    int MaxNumber;
    int MinNumber;
    float tempNumber;
    
    bool upOrDown;
    float speed;//每秒变化数字
    
    float beatTime;//变化所需时间
    
    
    void update(float time);
    
};

#endif /* defined(__mofaxueyuan__LYBeatNumbers__) */
