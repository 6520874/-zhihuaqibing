//
//  LYBeatNumbers.cpp
//  mofaxueyuan
//
//  Created by user on 13-10-15.
//
//

#include "LYBeatNumbers.h"
#include "SimpleAudioEngine.h"

CBeatNumbers *CBeatNumbers::createWithNumberAndSize(int number,int size,int defaultNumber,LabelAtlas *numberLabel){
    
    CBeatNumbers *num = new CBeatNumbers();
    if (num&&num->initWithNumberAndSize(number,size,defaultNumber,numberLabel)) {
        
        num->autorelease();
        return num;
    }else{
        delete num;
        num = NULL;
    }
    
    return num;
}

bool CBeatNumbers::initWithNumberAndSize(int number,int size,int defaultNumber,LabelAtlas *numberLabel){
    
    if (!CCNode::init()) {
        return false;
    }
    
    MaxNumber = number;
    speed = 60;
    beatTime =0.5f;
    
    if (numberLabel ==NULL) {
        __String *numStr = __String::createWithFormat("%d",defaultNumber);
        m_pNumberLabel = CCLabelAtlas::create(numStr->getCString(), "fonts/numAtlas_2.png", 50, 101, '0');
        addChild(m_pNumberLabel);
    }else{
        m_pNumberLabel = numberLabel;
        addChild(m_pNumberLabel);
    }
    
    
    
    return true;
}

void CBeatNumbers::BeatUpTo(int max,int start){
    
    __String *numStr = __String::createWithFormat("%d",start);
    m_pNumberLabel->setString(numStr->getCString());
    tempNumber = start;
    MaxNumber = max;
    upOrDown = true;
    
    speed = (MaxNumber-tempNumber)/beatTime;
    
    unscheduleUpdate();
    scheduleUpdate();
    
    
}

void CBeatNumbers::BeatDownTo(int min){
    
    __String *numStr = __String::createWithFormat("%d",MaxNumber);
    m_pNumberLabel->setString(numStr->getCString());
    tempNumber = MaxNumber;
    MinNumber = min;
    upOrDown = false;

    
    speed = (MaxNumber-MinNumber)/beatTime;
    
    unscheduleUpdate();
    scheduleUpdate();
}


void CBeatNumbers::update(float time){
    

    
    
    if (upOrDown) {
        if (tempNumber>=MaxNumber) {
            tempNumber=MaxNumber;
            __String *numberStr = __String::createWithFormat("%d",(int)tempNumber);
            m_pNumberLabel->setString(numberStr->getCString());
            unscheduleUpdate();
            return;
        }
        tempNumber += time*speed;
    }else{
        if (tempNumber<=MinNumber) {
            tempNumber=MinNumber;
            __String *numberStr = __String::createWithFormat("%d",(int)tempNumber);
            m_pNumberLabel->setString(numberStr->getCString());
            unscheduleUpdate();
            return;
        }
        tempNumber -= time*speed;
    }
    
    __String *numberStr = __String::createWithFormat("%d",(int)tempNumber);
    m_pNumberLabel->setString(numberStr->getCString());

}

void CBeatNumbers::setNumberCorlor(Color3B corlor){
    m_pNumberLabel->setColor(corlor);
}




