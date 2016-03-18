//
//  graphicsIdentify .h
//  TheGodWill
//
//  Created by zhaixh on 14-11-25.
//
//

#ifndef TheGodWill_graphicsIdentify__h
#define TheGodWill_graphicsIdentify__h

#include "cocos2d.h"
#include "GeometricRecognizer.h"
USING_NS_CC;
using namespace DollarRecognizer;

class graphicsIdentify:public LayerColor
{
public:
    graphicsIdentify();
    ~graphicsIdentify();
    virtual bool init();
    CREATE_FUNC(graphicsIdentify);
    virtual bool onTouchBegan(Touch *touch, Event *unused_event);
    virtual void onTouchMoved(Touch *touch, Event *unused_event);
    virtual void onTouchEnded(Touch *touch, Event *unused_event);
    virtual void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags) ;
    void onDraw(const cocos2d::Mat4 &transform, bool transformUpdated);
    void removeResult();
    void update(float dt);
    //void setOpcity();
    void clearPoint();

private:
    cocos2d::Size m_winSize;
    cocos2d::CustomCommand _customCommand;
    GeometricRecognizer* g_rGemertricRecognizer;
    std::vector<cocos2d::Point> m_pointArr;
    Path2D p_2dPath;//坐标集合，记录触摸操作，Layer的ccTouchesMoved方法中记录坐标
    std::string m_GraphicsName;
    float m_similarity;
    cocos2d::Point m_startPos;
    
    cocos2d::Ref * m_pTarget;
    
    cocos2d::SEL_MenuHandler m_pHandler;
    bool m_isDraw;
    int m_opcity;//线的透明度
    bool m_isValid;//标识当前画线是否有效
    ParticleSystemQuad* m_touchStar;
};


#endif
