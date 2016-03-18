//
//  LYNetDialog.cpp
//  WarOfGod
//
//  Created by user on 15/3/27.
//
//

#include "LYNetDialog.h"

#include "zzbNetControlModule.h"

CNetDialog::CNetDialog(){
    
    
    
}

CNetDialog::~CNetDialog(){
    
    
     CNetControlModule::sharedNetControlModule()->setNetDialog(NULL);
    
}


bool CNetDialog::init(){
    
    if (!CBaseLayer::init()) {
        return false;
    }
    
    Size winsize = Director::getInstance()->getWinSize();
    
    
    

    
    LayerColor* temp = LayerColor::create(Color4B(0, 0, 0, 170));
    addChild(temp);
    temp->setVisible(false);
    

    temp->runAction(Sequence::create(DelayTime::create(0.7f),Show::create(),NULL));
    
    Label* loadingLabel = Label::createWithSystemFont("服务器通信中...", "Arial", 30);
    loadingLabel->setPosition(Vec2(winsize.width*0.5f,winsize.height*0.7f));
    temp->addChild(loadingLabel);
    
    
    Sprite *pCricleBg = Sprite::createWithSpriteFrameName("net_loading2.png");
    pCricleBg->cocos2d::Node::setScale(1.5);
    
    pCricleBg->setPosition(Vec2(winsize.width*0.5,winsize.height*0.5));
    temp->addChild(pCricleBg);
    
    
    Sprite *pCricleRotate = Sprite::createWithSpriteFrameName("net_loading1.png");
    pCricleRotate->cocos2d::Node::setScale(1.5);
    pCricleRotate->runAction(RepeatForever::create(RotateBy::create(0.1,30)));
     pCricleRotate->setPosition(Vec2(winsize.width*0.5,winsize.height*0.5));
     temp->addChild(pCricleRotate,1);
    

    //触摸初始化
    m_pListener = EventListenerTouchOneByOne::create();
    m_pListener->setSwallowTouches(true);
    
    m_pListener->onTouchBegan = CC_CALLBACK_2(CNetDialog::onTouchBegan, this);

    _eventDispatcher->addEventListenerWithFixedPriority(m_pListener, -500);
    

    return true;
}

bool CNetDialog::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event){
    
    return true;
    
}

void CNetDialog::onExit(){
    
    
    _eventDispatcher->removeEventListener(m_pListener);
    
    CBaseLayer::onExit();
    
}


