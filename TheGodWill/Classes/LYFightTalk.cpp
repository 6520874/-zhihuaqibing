//
//  LYFightTalk.cpp
//  TheGodWill
//
//  Created by user on 15/3/6.
//
//

#include "LYFightTalk.h"

#include "Common.h"

#include "fightScene.h"

#include "LYFightData.h"

int CFightTalk::talkNumber = 0;

CFightTalk::CFightTalk(){
    
    m_pTheFighter = NULL;
    m_pTheBG=NULL;
    m_pExpression=NULL;
    m_pTheContent=NULL;
    
    m_fNowTime=0;
    
    talkNumber++;
//    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("plist/fightTalkIcon.plist");
}

CFightTalk::~CFightTalk(){
    
    talkNumber--;
//    SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("plist/fightTalkIcon.plist");
}




void CFightTalk::setTheInfo(fightTalkingInfo *theInfo,CBaseFighter* target){
    
    
    m_pTheFighter = target;
    
    std::string tempValue = theInfo->content;

    m_pTheBG = Sprite::createWithSpriteFrameName("FT_BG.png");
    addChild(m_pTheBG,-1);

    float textSize=30;
    
    float BG_Width = m_pTheBG->getBoundingBox().size.width;
    float BG_Height = m_pTheBG->getBoundingBox().size.height;
    
    
    //测试用
//    tempValue = "  你说卧室谁啊，，猜到有奖";
    m_pTheContent = Label::createWithSystemFont(tempValue.c_str(), "Arial", textSize);
    
    float textWidht = m_pTheContent->getContentSize().width;
    
    m_pTheContent->setDimensions(BG_Width*0.95f, 0);
    addChild(m_pTheContent,3);
    
    

//    long textNum = tempValue.length()/2;//总字数`
//
//    int lineText = BG_Width/(textSize+4); //每行最多字数

//    float lineNum = int(textNum/lineText) +  ((textNum*1.0f/lineText-int(textNum/lineText))>0? 1:0);  //所有字行数
    
    int lineNum =  int(textWidht/(BG_Width*0.95f)) +  ((textWidht*1.0f/(BG_Width*0.95f)-int(textWidht/(BG_Width*0.95f)))>0? 1:0) ;

    float textHight = lineNum* (textSize+5);

    float scaleY = textHight/(m_pTheBG->getBoundingBox().size.height*0.6f);//因为对话框还有个小尖头，这个也得算进去

    m_pTheBG->setScale(1, scaleY);
    
    float BG_Height_new =m_pTheBG->getBoundingBox().size.height;
    
    m_pTheContent->setPosition(Vec2(BG_Width*0.065f, BG_Height_new*0.1f));
    
    CCLOG("talkBG--high:%f",scaleY);
    
    
    float offsetY = 0;

    
    //获取相应士兵的2d坐标
//    m_pTheBG->setPosition3D(Vec3(0,theFighter->gettheSprite()->getBoundingBox().size.height+m_pTheBG->getBoundingBox().size.height*0.5f,0));
    
    //表情动画
    if(theInfo->iconPath!="0"){
        tempValue = theInfo->iconPath+"1.png";
        m_pExpression = Sprite::createWithSpriteFrameName(tempValue.c_str());
        
        __String* tempStr = __String::create("");
        
        Size spriteSize = m_pExpression->getBoundingBox().size;
        
        Vector<SpriteFrame*> animFrames;
        for (int i=1; i<=theInfo->iconNum; i++) {
            
            tempStr->initWithFormat("%s%d.png",theInfo->iconPath.c_str(),i);
            
            animFrames.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName(tempStr->getCString()));
        }
        Animation* theAnimation = Animation::createWithSpriteFrames(animFrames);
        theAnimation->setDelayPerUnit(0.1f);
        
        m_pExpression->runAction(RepeatForever::create(Animate::create(theAnimation)));
        
        m_pExpression->setScale(0.6f);
        
        addChild(m_pExpression);
        
        m_pExpression->setPosition(Vec2(-BG_Width*0.5f+m_pExpression->getBoundingBox().size.width, BG_Height_new*0.5f-m_pExpression->getBoundingBox().size.height*0.7f));
        
        
        
    }
    
    
    
    
    
    
    
    

    
    
//    m_pTheContent->setPosition3D(m_pTheBG->getPosition3D());
    
    
    
    
    CFightScene* targetScene = CFightData::getInstance()->getTheFightScene();
    
    if (targetScene) {
        setGlobalZOrder(100000);
        targetScene->addChild(this);
    }
    
    
    scheduleUpdate();
    
}


void CFightTalk::update(float time){
    
    
    //最多存在1秒钟
    float maxTime = 1.2f;
    
    m_fNowTime+=time;
    
    if (m_fNowTime>=maxTime) {

        unscheduleUpdate();
        m_pTheFighter->m_pTheTalk=NULL;
        removeFromParentAndCleanup(true);

    }

    
}