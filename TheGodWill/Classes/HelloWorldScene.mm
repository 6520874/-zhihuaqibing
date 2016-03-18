#include "HelloWorldScene.h"
#include "fightScene.h"

#include "SmallMap.h"
//@class MicBlowViewController;

//#include "MicBlowViewController.h"


#include "graphicsIdentify.h"
#include "FightPrepareLayer.h"
#include "PhysicalSystem.h"

#include "LayerManager.h"

#include "LYFightData.h"

#include "DBMgr.h"
#include "TaskMgr.h"
#include "LvUpLayer.h"
#include "TaskLayer.h"
#include "FightWinLayer.h"
#include "FightFailLayer.h"
#include "PropLayer.h"
#include "zzbNetControlModule.h"
#include "NetWorkDataSingle.h"
#include "SkillUpMenuLayer.h"
#include "cocostudio/CocoStudio.h"
#include "DBMgr.h"
#include "CSharedPlist.h"

#include "BeName.h"
#include "LoginLayer.h"

USING_NS_CC;
using namespace cocostudio;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

HelloWorld::HelloWorld()
{
    CSharedPlist::getInstance()->pushPlist("plist/common.plist");
    CSharedPlist::getInstance()->pushPlist("plist/common2.plist");
    CSharedPlist::getInstance()->pushPlist("plist/common3.plist");
    CSharedPlist::getInstance()->pushPlist("plist/login.plist");
}

HelloWorld::~HelloWorld()
{
    CSharedPlist::getInstance()->erasePlist("plist/login.plist");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    
    CCLOG("hello world init");
    
    //////////////////////////////
    // 1. super init first
    if ( !CBaseLayer::init() )
    {
        return false;
    }
    
    
    m_bIfLogin = false;
    
    cocos2d::Size winsize = Director::getInstance()->getWinSize();
    
    Sprite *pBg = Sprite::create("login/lg_bg.png");
    pBg->setPosition(m_winSize.width*0.5,m_winSize.height*0.5);
    addChild(pBg);
    
    cocos2d::Size tBg = pBg->getBoundingBox().size;
    
    
    Sprite *pLoginIco = Sprite::create("login/logo.png");
    pLoginIco->setPosition(Vec2(tBg.width*0.265,tBg.height*0.85));
    pBg->addChild(pLoginIco,99);

    
    Sprite *pSoldier = Sprite::createWithSpriteFrameName("lg_zhanshi.png");
    pSoldier->setPosition(Vec2(m_winSize.width/1.929,m_winSize.height/2.259));
    addChild(pSoldier,10);
    
    int iUpDown = 5;
    
    pSoldier->runAction(RepeatForever::create(Sequence::create(MoveBy::create(0.5f,Vec2(0,iUpDown)),MoveBy::create(0.5f,Vec2(0,-iUpDown)),NULL)));
    
    Sprite *pDun = Sprite::createWithSpriteFrameName("lg_dun.png");
    pDun->setPosition(m_winSize.width/1.764,m_winSize.height/2.2);
    pDun->setAnchorPoint(Vec2(0.29,0.38));
    cocos2d::Node::addChild(pDun,9);
    pDun->runAction(RepeatForever::create(Sequence::create(MoveBy::create(0.5f,Vec2(0,iUpDown)),MoveBy::create(0.5f,Vec2(0,-iUpDown)),MoveBy::create(0.5f,Vec2(0,-iUpDown)),MoveBy::create(0.5f,Vec2(0,iUpDown)),NULL)));

    
    Sprite *pDao = Sprite::createWithSpriteFrameName("lg_dao.png");
    pDao->setAnchorPoint(Vec2(0.75,0.72));
    
    Sprite *pGuang = Sprite::createWithSpriteFrameName("lg_dg.png");
    pGuang->setOpacity(255*0.75);
    pGuang->setScale(1.5);
    ClippingNode* clippingNode = ClippingNode::create();
    clippingNode->setPosition(m_winSize.width/2.0,m_winSize.height/2.6);
    addChild(clippingNode,11);
    
    cocos2d::Size  clipSize = pDao->getContentSize();
    clippingNode->setAlphaThreshold(0.01f); //设置alpha闸值
    clippingNode->setContentSize(clipSize); //设置尺寸大小
    
    clippingNode->setStencil(pDao);   //设置模板stencil
    clippingNode->addChild(pDao,1);  //先添加标题,会完全显示出来,因为跟模板一样大小
    clippingNode->addChild(pGuang,2);       //会被裁减
    
    iUpDown  = 3;
    
    clippingNode->runAction(RepeatForever::create(Sequence::create(RotateTo::create(0.5f,-iUpDown),RotateTo::create(0.5f,iUpDown),NULL)));
    
    //RotateTo::create(0.5f,0)
    iUpDown = 10;
    
    float fStartguangX  = -clipSize.width*0.2;
    float fEndguangX = -clipSize.width*0.9;
    
    pGuang->setPosition(fStartguangX ,-clipSize.height*0.2);
    
    MoveTo* moveAction =  MoveTo::create(0.5f, Vec2(fEndguangX,pGuang->getPositionY()));
    MoveTo* moveBackAction = MoveTo::create(3.0f, Vec2(fStartguangX ,pGuang->getPositionY()-50));
    pGuang->runAction(RepeatForever::create(Sequence::create(moveAction,
                                                             ToggleVisibility::create(),moveBackAction, ToggleVisibility::create(), NULL)));

    
    cocos2d::Size tSoldierBg  = pSoldier->getContentSize();
    
    Sprite *pYin = Sprite::createWithSpriteFrameName("lg_lling.png");
    pYin->setAnchorPoint(Vec2(1,0.6));

    pYin->setPosition(tSoldierBg.width*0.12,tSoldierBg.height*0.9125);
    
    //1334 750
    
    pSoldier->addChild(pYin,9);
    RotateTo::create(2.0f,0),
    
    pYin->runAction(RepeatForever::create(Sequence::create(RotateTo::create(0.5f,-6),RotateTo::create(0.5f,6),NULL)));
    
    

    //龙
    Sprite *pDragon = Sprite::createWithSpriteFrameName("lg_dragon.png");
    pDragon->cocos2d::Node::setPosition(m_winSize.width/1.39,m_winSize.height/1.32);
    addChild(pDragon,2);

    iUpDown = 10;
    pDragon->runAction(RepeatForever::create(Sequence::create(MoveBy::create(0.5f,Vec2(0,-iUpDown)),MoveBy::create(0.5f,Vec2(0,0)),MoveBy::create(0.5f,Vec2(0,iUpDown)),MoveBy::create(0.5f,Vec2(0,0)),NULL)));
    
    iUpDown = 2;
    cocos2d::Size tDragon = pDragon->getContentSize();
    
    
    //龙火粒子效果
    ParticleSystemQuad *particle = ParticleSystemQuad::create("particleSystem/dragonFire.plist");
    particle->setScale(0.7f);
    particle->setPosition(-tDragon.width*0.1f,tDragon.height*0.27f);
    particle->setAutoRemoveOnFinish(true);
    pDragon->addChild(particle,4);
    
    
    Sprite *pLeftwing = Sprite::createWithSpriteFrameName("lg_zuochi.png");
    pLeftwing->setAnchorPoint(Vec2(0.5,0));
    pLeftwing->runAction(RepeatForever::create(Sequence::create(RotateTo::create(0.5f,10),RotateTo::create(0.5f,0),RotateTo::create(0.5f,-10),RotateTo::create(0.5f,0),NULL)));
    pLeftwing->setPosition(tDragon.width*0.38,tDragon.height*0.78);
    pDragon->addChild(pLeftwing,-1);
    

    
    Sprite *pRightwing = Sprite::createWithSpriteFrameName("lg_youchi.png");
    pRightwing->setAnchorPoint(Vec2(0,0));
    pRightwing->runAction(RepeatForever::create(Sequence::create(RotateTo::create(0.5f,-8),RotateTo::create(0.5f,0),RotateTo::create(0.5f,8),RotateTo::create(0.5f,0),NULL)));
    pRightwing->setPosition(tDragon.width*0.435,tDragon.height*0.695);
    pDragon->addChild(pRightwing,1);
    
    
    //地狱火
    Sprite *pDiyuhuo = Sprite::createWithSpriteFrameName("lg_di.png");
    pDiyuhuo->setAnchorPoint(Vec2(0.51,0));
    pDiyuhuo->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(2.0f,1,0.95),ScaleTo::create(2.0f,1,1),NULL)));
    
    float fx = tBg.width/1.12;
    float fy = tBg.height/1.97;
    

    pDiyuhuo->setPosition(fx,fy);
    pBg->addChild(pDiyuhuo,1);

    //1334 750
    
    //大怪
    Sprite *pGuaiSmall = Sprite::createWithSpriteFrameName("lg_xiao1.png");
    pGuaiSmall->setAnchorPoint(Vec2(0.46,0.44));
    //pGuaiSmall->setPosition(1166.26,254.57);
    pGuaiSmall->setPosition(m_winSize.width-pGuaiSmall->getContentSize().width*0.5,m_winSize.height*0.27);
    
    pGuaiSmall->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(2.0f,1,0.85),ScaleTo::create(2.0f,1,1),NULL)));
    addChild(pGuaiSmall,2);
    
    //右边小怪
    Sprite *pGuaiBig = Sprite::createWithSpriteFrameName("lg_xiao2.png");
    pGuaiBig->setAnchorPoint(Vec2(0.44,0.19));
    pGuaiBig->setPosition(m_winSize.width*0.87,m_winSize.height*0.33);
    pGuaiBig->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(2.0f,1,0.9),ScaleTo::create(2.0f,1,1),NULL)));
    addChild(pGuaiBig,1);
    
    int iOffectX = 8;
    int iOffectY = 8;
    //196.47   3.6
    
    //奇兵
    Sprite *pQibin = Sprite::createWithSpriteFrameName("lg_qibi.png");
    pQibin->setPosition(m_winSize.width/7.08-iOffectX,m_winSize.height/208.3-iOffectY*2);
    
    //-x->0->x->0->-x
    //0-y->0->-y->0
    
    pQibin->runAction(RepeatForever::create(Sequence::create(EaseSineIn::create(MoveBy::create(1.0f,Vec2(iOffectX,0))),EaseSineOut::create(MoveBy::create(1.0f,Vec2(iOffectX,0))),EaseSineIn::create(MoveBy::create(1.0f,Vec2(-iOffectX,0))),EaseSineOut::create(MoveBy::create(1.0f,Vec2(-iOffectX,0))),NULL)));
    
    pQibin->runAction(RepeatForever::create(Sequence::create(
    EaseSineOut::create(MoveBy::create(1.0f,Vec2(0,iOffectY))),
    EaseSineIn::create(MoveBy::create(1.0f,Vec2(0,-iOffectY))),EaseSineOut::create(MoveBy::create(1.0f,Vec2(0,-iOffectY))),EaseSineIn::create(MoveBy::create(1.0f,Vec2(0,iOffectY))),NULL)));
    pQibin->cocos2d::Node::setAnchorPoint(Vec2(0.5,0));
    addChild(pQibin,12);
    
    
    Sprite *pXiaoXiao = Sprite::createWithSpriteFrameName("lg_sl.png");
    pXiaoXiao->setPosition(m_winSize.width/8.31,m_winSize.height/2.48);
    addChild(pXiaoXiao,1);
    pXiaoXiao->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(2.0f,1,0.9),ScaleTo::create(2.0f,1,1),NULL)));
    
    //地面火星
    ParticleSystemQuad *particlefire = ParticleSystemQuad::create("particleSystem/fireStar.plist");
    particlefire->setScale(1.2f);
    particlefire->setPosition(winsize.width*0.5f,tDragon.height*0.01f);
    particlefire->setAutoRemoveOnFinish(true);
    addChild(particlefire,100);
    
    
    //地面火焰
    ParticleSystemQuad *groundfire = ParticleSystemQuad::create("particleSystem/groundFire.plist");
    groundfire->setScale(1.2f);
    groundfire->setPosition(winsize.width*0.5f,tDragon.height*0.01f);
    groundfire->setAutoRemoveOnFinish(true);
    addChild(groundfire,88);
    
    //按钮
    
    Sprite *pStartGame = Sprite::create("login/lg_kai.png");
    Sprite *pStartGameSel = Sprite::create("login/lg_kaian.png");
    
    fightNow =  MenuItemSprite::create(pStartGame,pStartGameSel,CC_CALLBACK_1(HelloWorld::onStart,this) );
    fightNow->setPosition(winsize.width*0.5f, winsize.height*0.1f);
    

    char strBuf[50] = {0};
    sprintf(strBuf,"V%s",CLINENT_VERSION);
    
    //版本号    
    Label *pVersion = Label::createWithTTF(strBuf,"fonts/cuti.ttf",30);
    pVersion->enableOutline(Color4B(0,0,0,255),3);
    
    pVersion->setPosition(m_winSize.width-pVersion->getContentSize().width*0.6,m_winSize.height*0.03);
    addChild(pVersion,88);
    
    MenuItemFont* newBegin = MenuItemFont::create("从零开始", CC_CALLBACK_1(HelloWorld::reStart,this));
    newBegin->setScale(0.3f);
    newBegin->setPosition(Vec2(winsize.width*0.95f, winsize.height*0.025f));
    
    Menu *pMenu =  Menu::create(fightNow,newBegin,NULL);
    addChild(pMenu,100);
    pMenu->setPosition(Vec2::ZERO);
    
    fightNow->setEnabled(false);
    
    //SimpleAudioEngine::getInstance()->playEffect("music/loginWar.mp3");
    
    SimpleAudioEngine::getInstance()->playBackgroundMusic("music/loginWar.mp3", true);
    
    //newBegin->setVisible(false);
    
    //注释掉从零开始
    
#ifdef  DEBUG_Mode
    newBegin->setVisible(true);
#endif
    
    
    //测试用
//    newBegin->setVisible(true);
    
    
    
    return true;
}


void HelloWorld::onStart(cocos2d::Ref *pSender)
{
    
    cocos2d::Size winsize = Director::getInstance()->getWinSize();

    
    SimpleAudioEngine::getInstance()->playEffect("music/click_btn.mp3");

    

    userInfoInDB * theUser =  CDBMgr::getInstance()->GetUserInfo();

    if(theUser->userId==0)
    {
    
        requestNewId();
    }
    else
    {
        OnLogin(NULL);
    }

}





void HelloWorld::tofight(cocos2d::Ref *pSender)
{
    
    
    
    
    
    
}

void HelloWorld::requestNewId(){
    
    
    CNetControlModule::sharedNetControlModule()->newLogin(NULL, NULL);
}


void HelloWorld::OnLogin(Ref *pSender)
{
    
    CNetControlModule::sharedNetControlModule()->Login(NULL, NULL);
    
}

void HelloWorld::onEnterTransitionDidFinish(){
    
    CBaseLayer::onEnterTransitionDidFinish();
    
    
//    //*********测试用，看到就注释掉
//    CLayerManager::GetSingletonPtr()->createLayer(eLayer_MainMenu, NULL,true);
//    return;
    
    
    
    //延迟一帧进入游戏,预留初始化数据库的时间
    LayerColor* tempLayer = LayerColor::create(Color4B(0, 0, 0, 220));
    addChild(tempLayer,1000);
    
    Label * loading = Label::createWithTTF("集结部队中。。","fonts/cuti.ttf",35);
    tempLayer->addChild(loading);
    loading->setPosition(Vec2(m_winSize.width*0.5f, m_winSize.height*0.65f));
    
    
    
    tempLayer->runAction(Sequence::create(DelayTime::create(0.1f),CallFunc::create([=](){
        CDBMgr::getInstance();//数据库初始化
        fightNow->setEnabled(true);
        CNetControlModule::sharedNetControlModule()->versionJudge(NULL, NULL);
    }),RemoveSelf::create(), nil));

}

void HelloWorld::reStart(cocos2d::Ref *pSender){
    
//    
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//    NSString *toPath = [NSHomeDirectory() stringByAppendingFormat:@"/Documents/gameDB"];
//    NSString *copyPath = [[[NSBundle mainBundle] bundlePath] stringByAppendingFormat:@"/gameDB"];
//    
//    [[NSFileManager defaultManager] removeItemAtPath:toPath error:nil];
//    
//    [[NSFileManager defaultManager] copyItemAtPath:copyPath toPath:toPath error:nil];
//
//#endif
//
//
    
    CDBMgr::getInstance()->copyDB(eCopDBType_cover);
    CDBMgr::getInstance()->loadDB();
    

    
    
}


