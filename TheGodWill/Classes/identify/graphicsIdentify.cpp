//
//  graphicsIdentify .cpp
//  TheGodWill
//
//  Created by zhaixh on 14-11-25.
//
//

#include "graphicsIdentify.h"
#include "renderer/CCRenderer.h"
#include "renderer/CCCustomCommand.h"

std::string templateNameV[]={"Arrow","Caret","Pigtail","QuestionMark","Star","Spiral"};
graphicsIdentify::graphicsIdentify()
{
    m_isDraw = true;
    m_opcity = 255;
    m_isValid = true;
    m_touchStar = NULL;
}

graphicsIdentify::~graphicsIdentify()
{
    
}


bool graphicsIdentify::init()
{
    m_winSize = Director::getInstance()->getWinSize();
    if (!LayerColor::initWithColor(Color4B(255,255,255,100), m_winSize.width, m_winSize.height)) {
        return false;
    }
    
    auto listener1 = EventListenerTouchOneByOne::create();//创建一个触摸监听
    listener1->setSwallowTouches(true); //设置是否想下传递触摸
    listener1->onTouchBegan = CC_CALLBACK_2(graphicsIdentify::onTouchBegan, this);
    listener1->onTouchMoved = CC_CALLBACK_2(graphicsIdentify::onTouchMoved, this);
    listener1->onTouchEnded = CC_CALLBACK_2(graphicsIdentify::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener1,this);
    
    //加载模板
    g_rGemertricRecognizer = new GeometricRecognizer();
    g_rGemertricRecognizer->loadTemplates();
    
    m_touchStar = ParticleSystemQuad::create("plist/star.plist") ;
    this->addChild(m_touchStar, 10);
    m_touchStar->setVisible(false);
    
    return true;
}

void graphicsIdentify::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
    LayerColor::draw(renderer, transform, flags);
    _customCommand.init(_globalZOrder);
    _customCommand.func = CC_CALLBACK_0(graphicsIdentify::onDraw, this, transform, flags);
    renderer->addCommand(&_customCommand);
}

void graphicsIdentify::onDraw(const Mat4 &transform, bool transformUpdated)
{
    Director* director = Director::getInstance();
    director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    director->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, transform);
    
    CHECK_GL_ERROR_DEBUG();
    glLineWidth( 5.0f );
    DrawPrimitives::setDrawColor4B(255,0,0,m_opcity);

    if (m_pointArr.size() > 0) {
        for (int i = 0; i < m_pointArr.size(); i++){
            if (i == 0) {
                DrawPrimitives::drawPoint(Point(m_pointArr.at(0).x, m_pointArr.at(0).y));
            }
            else
            {
                Point pstart = m_pointArr.at(i - 1);
                DrawPrimitives::drawLine(Point(m_pointArr.at(i).x, m_pointArr.at(i).y ),Point(pstart.x, pstart.y));
            }
        }
    }
    CHECK_GL_ERROR_DEBUG();
    director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
}

bool graphicsIdentify::onTouchBegan(Touch *touch, Event *unused_event)
{
    if (!m_isDraw) {
        return false;
    }
    m_startPos = Director::getInstance()->convertToGL(touch->getLocationInView());
    m_touchStar->resetSystem() ;//清除所有的粒子
    m_touchStar->setPosition(m_startPos) ;

    return true;
}
void graphicsIdentify::onTouchMoved(Touch *touch, Event *unused_event)
{
    Point location = touch->getLocationInView();
    m_touchStar->setVisible(true) ;
    m_touchStar->setPosition(Director::getInstance()->convertToGL(location)) ;

    m_pointArr.push_back(touch->getLocation());
    Point2D p_Point2DTemp;
    p_Point2DTemp.x=location.x;
    p_Point2DTemp.y=location.y;
    p_2dPath.push_back(p_Point2DTemp);
}


void graphicsIdentify::onTouchEnded(Touch *touch, Event *unused_event)
{
    Point endPos = touch->getLocationInView();
    m_touchStar->stopSystem();
    if (p_2dPath.size() < 1){
        return ;
    }
    //m_touchStar->setVisible(false);
    //对保存的触摸点顺序逆序都做一次识别，去识别度较高的结果作为判断结果
    RecognitionResult resultOrder = g_rGemertricRecognizer->recognize(p_2dPath);
    
    Path2D reversePath;
    for (int i = p_2dPath.size() - 1; i >= 0; i--)
    {
        reversePath.push_back(p_2dPath.at(i));
    }
    if((fabs(endPos.x - m_startPos.x) < 60) && (fabs(endPos.y - m_startPos.y) < 50 && (reversePath.size() < 20)))
    {
        m_isValid = false;
    }
    RecognitionResult resultReverse = g_rGemertricRecognizer->recognize(reversePath);
    if (resultReverse.score > resultOrder.score)
    {
        m_GraphicsName = resultReverse.name;
        m_similarity = resultReverse.score;
    }
    else
    {
        m_GraphicsName = resultOrder.name;
        m_similarity = resultOrder.score;
    }
    
    for (int i = 0; i < 6; i++)
    {
        if(m_GraphicsName == templateNameV[i])
        {
            CCLOG("没有对应的兵种！");
            p_2dPath.clear();
            m_pointArr.clear();
            return;
        }
    }
    
    if(m_GraphicsName == "LeftSquareBracket" || m_GraphicsName == "RightSquareBracket"
       || m_GraphicsName == "LeftCurlyBrace" || m_GraphicsName == "RightCurlyBrace"
            || (m_GraphicsName == "Unknown" && m_similarity == 1.000000))
    {
        m_GraphicsName = "LineDownDiagonal";
        CCLOG("直线");
        
        
    }
    
    else if(m_GraphicsName == "X" || m_GraphicsName == "Delete")
    {
        m_GraphicsName = "Triangle";
    }
    else if(m_GraphicsName == "Diamond")
    {
        m_GraphicsName = "Circle";
    }
    if (m_isValid) {
        m_isDraw = false;
        scheduleUpdate();
    }
}

void graphicsIdentify::update(float dt)
{
    if (m_opcity < 0) {
        unscheduleUpdate();
        p_2dPath.clear();
        m_pointArr.clear();
        m_opcity = 255;
    }
    else if (m_opcity == 60) {
        //显示结果
        
        char buf[32];
        //调用对应函数
        CCLOG("图形为:%s, 相似度为:%f", m_GraphicsName.c_str(), m_similarity);
        //显示识别的图形和相似度
        sprintf(buf, "%s.png", m_GraphicsName.c_str());
        Sprite* grap = Sprite::createWithSpriteFrameName(buf);
        grap->setPosition(Vec2(m_winSize.width / 2, m_winSize.height / 2));
        addChild(grap,1,60);
        
        sprintf(buf, "准确度 %d%%", int(m_similarity*100));
        Label* pSimilar = Label::createWithSystemFont(buf, "Arial", 32);
        addChild(pSimilar,1,65);
        pSimilar->setPosition(Vec2(m_winSize.width / 2, m_winSize.height / 2 + grap->getContentSize().height / 2 + 20));
        
        ActionInterval*  pActionIn = FadeIn::create(0.5f);
        ActionInterval*  pActionOut = FadeOut::create(0.5f);
        CallFunc* rmv = CallFunc::create(CC_CALLBACK_0(graphicsIdentify::removeResult, this));
        Spawn* spnw = Spawn::create(FadeOut::create(0), pActionOut, rmv, NULL);
        grap->runAction(Sequence::create(pActionIn, DelayTime::create(0.15f), spnw, NULL));
        pSimilar->runAction(Sequence::create(ScaleTo::create(0.15, 1.5), ScaleTo::create(0.3, 1.0),NULL));
        m_opcity -= 5;
    }
    else
    {
        m_opcity -= 5;
    }
}


void graphicsIdentify::clearPoint()
{
    p_2dPath.clear();
    m_pointArr.clear();
}

void graphicsIdentify::removeResult()
{
    m_isDraw = true;
    Sprite* sp1 = (Sprite*)getChildByTag(60);
    if (sp1) {
        sp1->removeFromParent();
    }
    Sprite* sp2 = (Sprite*)getChildByTag(65);
    if (sp2) {
        sp2->removeFromParent();
    }
}



