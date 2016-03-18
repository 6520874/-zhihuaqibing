#include "AppDelegate.h"
#include "HelloWorldScene.h"
#include "PaoWuxian.h"
#include "mainMenuLayer.h"

#include "TalkingData.h"
#include "TDCCTalkingDataGA.h"
#include "TDCCAccount.h"

#include "LYPersonalData.h"

#include "DBMgr.h"

//#include "  .h"





USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::create("My Game");
        director->setOpenGLView(glview);
    }

    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);
    
    
    auto designSize = Size(1334, 750);
    
    auto designSize2 = Size(1136,640);
    glview->setDesignResolutionSize(designSize.width, designSize.height, ResolutionPolicy::FIXED_HEIGHT);
    
   
    //EGLView::getInstance()->setDesignResolutionSize(640,1136, ResolutionPolicy::EXACT_FIT);
//    Director::getInstance()->getOpenGLView()->setDesignResolutionSize(1334,750, ResolutionPolicy::EXACT_FIT);
    
    // create a scene. it's an autorelease object
    
//    auto scene = CMainMenuLayer::createScene();
//
//    // run
//    director->runWithScene(scene);
    
    
    //talkingdata
    
//    std::string temp = GAME_VERSION;
    
    
    
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    TDCCTalkingDataGA::onStart(TDAppKey, GAME_VERSION);
#endif
    
    
    
    
    
    
    
        
    
    
    
    
    
    auto scene = CMainMenuLayer::createScene();
    
    director->runWithScene(scene);
    
    
    
    
    
    
    
    
    

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
     SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    
    // if you use SimpleAudioEngine, it must resume here
     SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    
    
}

// MARK:android相关的talkingdata方法

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
extern "C" {
 jobjectArray Java_cn_xinpin_view_activity_GameActivity_getChannelId(JNIEnv *env, jobject o) {

	  auto stringClass = env->FindClass("java/lang/String");
	  auto pathArray = env->NewObjectArray(2, stringClass, 0);
	  env->SetObjectArrayElement(pathArray, 0, env->NewStringUTF(TDAppKey));
	  env->SetObjectArrayElement(pathArray, 1, env->NewStringUTF(GAME_VERSION));
	return pathArray;

   }
}
#endif





