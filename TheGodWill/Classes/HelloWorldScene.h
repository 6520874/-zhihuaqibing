#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "BaseLayer.h"

USING_NS_CC;

class HelloWorld : public CBaseLayer
{
public:
    HelloWorld();
    ~HelloWorld();
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    virtual void onEnterTransitionDidFinish();

    
    CREATE_FUNC(HelloWorld);

    void reStart(cocos2d::Ref* pSender);
    
    
    
    void onStart(cocos2d::Ref* pSender);
    
    void requestNewId();
    
    
    void tofight(cocos2d::Ref* pSender);
    

private:

    void OnLogin(Ref *pSender);

    cocos2d::Sprite3D * m_pSprite ;
//    cocos2d::Sprite3D * m_spriteBg;
    
    MenuItemSprite *fightNow; //开始按钮
    
    
    bool m_bIfLogin;
};

#endif // __HELLOWORLD_SCENE_H__
