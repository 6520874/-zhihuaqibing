    //
//  guiderLayer.cpp
//  TheGodWill
//
//  Created by user on 15/1/28.
//
//

#include "guiderLayer.h"
#include "guiderManagerLayer.h"
#include "LYFightData.h"
#include "CSharedPlist.h"
#include "fightScene.h"
#include "heroCompoundLayer.h"
#include "FightPrepareLayer.h"
#include "pvpMainLayer.h"
#include "pvpCommonInfoLayer.h"
#include "fightScene.h"
#include "DrawCard.h"

guiderLayer::guiderLayer()
{
    m_stencil = NULL;
    m_dialogue = NULL;
    m_guideInfo.clear();
    m_bStencial = false;
    m_back = NULL;
    m_clip = NULL;
    m_finger = NULL;
    m_girl = NULL;
    m_diagBg = NULL;
    m_bTouchValid = false;
    m_bRomoveFlag = false;    
    m_iTouchState = 0;
    CSharedPlist::getInstance()->pushPlist("plist/guide_1.plist");
    CSharedPlist::getInstance()->pushPlist("plist/guide_2.plist");
    
    char buf[256];
    
    //手指动画
    Animation* pFingerAnimation = Animation::create();
    for (int i = 1; i < 3; i++)
    {
        sprintf(buf, "finger_%d.png", i);
        pFingerAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(buf));
    }
    pFingerAnimation->setDelayPerUnit(0.25f);
    AnimationCache::getInstance()->addAnimation(pFingerAnimation, "fingerAnimation");
}

guiderLayer::~guiderLayer()
{
    CSharedPlist::getInstance()->erasePlist("plist/guide_1.plist");
    CSharedPlist::getInstance()->erasePlist("plist/guide _2.plist");
    guiderManagerLayer::getInstance()->setGuiderLayer(NULL);
}

guiderLayer* guiderLayer::create(GuideType type, Vec2 pos,bool ifshadow)
{
    guiderLayer* pRet = new guiderLayer();
    pRet->m_showType = type;
    pRet->m_pos = pos;
    pRet->m_bIfShadow= ifshadow;
    if(pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    else{
        CC_SAFE_DELETE(pRet);
        return NULL;
    }
}

void guiderLayer::setTouchValid()
{
    m_bTouchValid = true;
}

void guiderLayer::touchableInit(){
    
    
    //地图介绍的特殊处理，此时屏蔽所有触摸，以防介绍地图时点到后面的按钮    by ly
    if (m_type==GUIDE_MAP_INTRODUCE) {
        m_iTouchState = -1;
    }
    
    if(m_type == GUIDE_GET_TASK ) //任务引导后不屏蔽触摸 by ly
    {
        if((guiderManagerLayer::getInstance()->getStepNum() >= 4))
        {
            m_iTouchState=1;
        }
    }
    else if((m_type == GUIDE_SENIOR_CARD) && (guiderManagerLayer::getInstance()->getStepNum() == 8))
    {
        m_iTouchState=1;
    }
    else if((m_type == GUIDE_SOLIDER_COMPOUND) && (guiderManagerLayer::getInstance()->getStepNum() == 6))
    {
        m_iTouchState=1;
    }
    
    if(m_type == GUIDE_UPGRADE_SOLIDER ) //升级引导后不屏蔽触摸 by ly
    {
        if((guiderManagerLayer::getInstance()->getStepNum() >= 7))
        {
            m_iTouchState=1;
        }
    }
    
    if(m_type == GUIDE_SKILL_UPGRADE ) //技能升级引导后不屏蔽触摸 by ly
    {
        if((guiderManagerLayer::getInstance()->getStepNum() >= 9))
        {
            m_iTouchState=1;
        }
    }
    
    if(m_type == GUIDE_SOLIDER_NUM ) //能量升级引导后不屏蔽触摸 by ly
    {
        if((guiderManagerLayer::getInstance()->getStepNum() >= 5))
        {
            m_iTouchState=1;
        }
    }


    
    
}


bool guiderLayer::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    if(!m_bTouchValid)
    {
        return true;
    }
    
    touchableInit();
    
    
    if (m_stencil && (m_stencil->getBoundingBox().containsPoint(touch->getLocation()))) {
        
        if (m_iTouchState==-1) {  //不往下传递触摸，手指点到遮罩区域时也不会往下传递 by ly
            listener->setSwallowTouches(true);
        }else{
            listener->setSwallowTouches(false);
        }
        
        if (((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_UPGRADE_SOLIDER) && (guiderManagerLayer::getInstance()->getStepNum() == 10)) || ((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SKILL_UPGRADE) && (guiderManagerLayer::getInstance()->getStepNum() == 13)) )
        {
            guiderManagerLayer::getInstance()->setStepNum(0);
            CDataManager::saveGuideInfoToDB(guiderManagerLayer::getInstance()->getGuideType());
            guiderManagerLayer::getInstance()->setNextGuide(true);
            m_bRomoveFlag = true;
        }
    }
    else if(!m_stencil)
    {
        
        if (m_iTouchState==1) {   //往下传递触摸，手指点不点到到遮罩区域时都往下传递 by ly
            listener->setSwallowTouches(false);
        }else{
            listener->setSwallowTouches(true);
        }
        
        
        if(guiderManagerLayer::getInstance()->getStepNum() < m_guideInfo.size())
        {
            guiderManagerLayer::getInstance()->setStepNum(guiderManagerLayer::getInstance()->getStepNum() + 1);
            if (m_type == GUIDE_ADD_LINE) {
                if(guiderManagerLayer::getInstance()->getStepNum() == 2)
                {
                    m_dialogue->setString(m_guideInfo.at(guiderManagerLayer::getInstance()->getStepNum() - 1).desc.c_str());
                    ((CFightScene*)getParent())->m_bifNorAddSolider = true;
                }
                else if(guiderManagerLayer::getInstance()->getStepNum() == 7)
                {
                    m_bRomoveFlag = true;
                }
                else if(guiderManagerLayer::getInstance()->getStepNum() == 3)
                {
                    guiderManagerLayer::getInstance()->setstart(true);
                    m_bRomoveFlag = true;
                }
                else if(guiderManagerLayer::getInstance()->getStepNum() == 4)
                {
                    
                    Vec2 pos = Vec2(m_winSize.width * 0.22f, m_winSize.height * 0.94f);
                    showFingerGuide(pos);//显示能量条
                    m_dialogue->setString(m_guideInfo.at(guiderManagerLayer::getInstance()->getStepNum() - 1).desc.c_str());
                }
                else if(guiderManagerLayer::getInstance()->getStepNum() == 6)
                {
                    m_bRomoveFlag = true;
                }
                
            }
            else if((m_type == GUIDE_ADD_CIRCLE) && (guiderManagerLayer::getInstance()->getStepNum() == 3))
            {
                guiderManagerLayer::getInstance()->setstart(true);//标识播放圆圈动画
                ((CFightScene*)getParent())->m_bifNorAddSolider = true;
                m_bRomoveFlag = true;
            }
            else if(m_type == GUIDE_GET_TASK )
            {

                if((guiderManagerLayer::getInstance()->getStepNum() == 4))
                {
                    m_bRomoveFlag = true;
                }
                
            }
            else if(m_type == GUIDE_UPGRADE_SOLIDER)
            {
                if(guiderManagerLayer::getInstance()->getStepNum() == 2)
                {
                    m_diagBg->removeFromParent();
                    m_diagBg = NULL;
                    m_girl->removeFromParent();
                    m_girl = NULL;
                    Vec2 pos = Vec2(m_winSize.width * 0.098f, m_winSize.height * 0.09);
                    showFingerGuide(pos);//显示升级按钮
                }
                else if(guiderManagerLayer::getInstance()->getStepNum() == 7)
                {
                    m_bRomoveFlag = true;
                    guiderManagerLayer::getInstance()->setStepNum(guiderManagerLayer::getInstance()->getStepNum() + 1);
                }
                else{
                    m_dialogue->setString(m_guideInfo.at(guiderManagerLayer::getInstance()->getStepNum() - 1).desc.c_str());
                }
            }

            else if(m_type == GUIDE_SKILL_UPGRADE)
            {
                if(guiderManagerLayer::getInstance()->getStepNum() == 3)
                {
                    m_diagBg->removeFromParent();
                    m_diagBg = NULL;
                    m_girl->removeFromParent();
                    m_girl = NULL;
                    Vec2 pos = Vec2(m_winSize.width * 0.295f, m_winSize.height * 0.09);
                    showFingerGuide(pos);//显示升级按钮
                }
                else if(guiderManagerLayer::getInstance()->getStepNum() == 5)
                {
                    m_diagBg->removeFromParent();
                    m_diagBg = NULL;
                    m_girl->removeFromParent();
                    m_girl = NULL;
                    Vec2 pos = guiderManagerLayer::getInstance()->m_pos;
                    showFingerGuide(pos);//显示升级按钮
                }
                else if(guiderManagerLayer::getInstance()->getStepNum() == 10)
                {
                    m_bRomoveFlag = true;
                }
                else{
                    m_dialogue->setString(m_guideInfo.at(guiderManagerLayer::getInstance()->getStepNum() - 1).desc.c_str());
                }
            }
            else if(m_type == GUIDE_ADD_RECTANGLE)
            {
                guiderManagerLayer::getInstance()->setstart(true);//标识播放圆圈动画
                m_bRomoveFlag = true;
                ((CFightScene*)getParent())->m_bifNorAddSolider = true;
            }
            else if(m_type == GUIDE_FAIL_TIP)
            {
                m_bRomoveFlag = true;
            }
            else if(m_type == GUIDE_LACK_STAMINA)
            {
                if(guiderManagerLayer::getInstance()->getStepNum() == 2)
                {
                    CDataManager::saveGuideInfoToDB(guiderManagerLayer::getInstance()->getGuideType());
                    m_bRomoveFlag = true;
                }
            }
           else if((m_type == GUIDE_SOLIDER_NUM) && (guiderManagerLayer::getInstance()->getStepNum() == 2))
           {
               m_diagBg->removeFromParent();
               m_diagBg = NULL;
               m_girl->removeFromParent();
               m_girl = NULL;
               Vec2 pos = Vec2(m_winSize.width * 0.098f, m_winSize.height * 0.09);
               showFingerGuide(pos);//显示升级按钮
           }
           else if((m_type == GUIDE_ADD_V) && (guiderManagerLayer::getInstance()->getStepNum() == 2))
            {
                guiderManagerLayer::getInstance()->setstart(true);
                ((CFightScene*)getParent())->m_bifNorAddSolider = true;
                m_bRomoveFlag = true;;
            }
           else if((m_type == GUIDE_GET_AWARD) && (guiderManagerLayer::getInstance()->getStepNum() == 2))
           {
               m_bRomoveFlag = true;
           }
            else if((m_type == GUIDE_SKILL_CUT) && (guiderManagerLayer::getInstance()->getStepNum() == 5))
            {
                m_bRomoveFlag = true;
                guiderManagerLayer::getInstance()->setStepNum(4);
            }
//            else if((m_type == GUIDE_ENTER_STORAGE) && (guiderManagerLayer::getInstance()->getStepNum() == 2))
//            {
//                Vec2 pos = Vec2(m_winSize.width * 0.5f, m_winSize.height * 0.09);
//                showFingerGuide(pos);//显示升级按钮
//                m_dialogue->setString(m_guideInfo.at(guiderManagerLayer::getInstance()->getStepNum() - 1).desc.c_str());
//            }
            else if(m_type == GUIDE_USE_MANGONEL)
            {
                if(guiderManagerLayer::getInstance()->getStepNum() == 2)
                {
                    Vec2 pos = Vec2(m_winSize.width * 0.05f, m_winSize.height * 0.42);
                    m_dialogue->setString(m_guideInfo.at(guiderManagerLayer::getInstance()->getStepNum() - 1).desc.c_str());
                    m_diagBg->setPosition(m_winSize.width * 0.53, m_winSize.height * 0.54);
                    m_girl->setPosition(m_winSize.width * 0.78, m_winSize.height * 0.55);
                    showFingerGuide(pos);
                }
                else if(guiderManagerLayer::getInstance()->getStepNum() >= 4)
                {
                    m_bRomoveFlag = true;
                }
            }
            else if(m_type == GUIDE_COMMON_CARD)
            {
                if(guiderManagerLayer::getInstance()->getStepNum() == 2)
                {
                    Vec2 pos = Vec2(m_winSize.width*0.06f,m_winSize.height*0.46f);
                    m_dialogue->setString(m_guideInfo.at(guiderManagerLayer::getInstance()->getStepNum() - 1).desc.c_str());
                    m_diagBg->setPosition(m_winSize.width * 0.53, m_winSize.height * 0.54);
                    m_girl->setPosition(m_winSize.width * 0.78, m_winSize.height * 0.55);
                    showFingerGuide(pos);
                }
                else if(guiderManagerLayer::getInstance()->getStepNum() == 4)
                {
                    Vec2 pos = Vec2(m_winSize.width*0.1,m_winSize.height*0.9);
                    m_diagBg->removeFromParent();
                    m_diagBg = NULL;
                    m_girl->removeFromParent();
                    m_girl = NULL;
                    showFingerGuide(pos);
                }
            }
            else if(m_type == GUIDE_SENIOR_CARD)
            {
                if(guiderManagerLayer::getInstance()->getStepNum() == 2)
                {
                    ((CDrawCard*)getParent())->addGuideInfo();
                }
//                else if(guiderManagerLayer::getInstance()->getStepNum() == 4)
//                {
//                    Vec2 pos = Vec2(m_winSize.width*0.1,m_winSize.height*0.9);
//                    m_diagBg->removeFromParent();
//                    m_diagBg = NULL;
//                    m_girl->removeFromParent();
//                    m_girl = NULL;
//                    showFingerGuide(pos);
//                }
            }
            else if(m_type == GUIDE_SOLIDER_COMPOUND)
            {
                if(guiderManagerLayer::getInstance()->getStepNum() == 3)
                {
                    m_bRomoveFlag = true;
                }
                else if(guiderManagerLayer::getInstance()->getStepNum() == 6)
                {
                    ((CHeroCompoundLayer*)getParent())->addCloseGuide();
                }
            }
            else if(m_type == GUIDE_SOLIDER_TEAM)
            {
                if(guiderManagerLayer::getInstance()->getStepNum() == 3)
                {
                    m_bRomoveFlag = true;
                }
                else
                {
                    m_dialogue->setString(m_guideInfo.at(guiderManagerLayer::getInstance()->getStepNum() - 1).desc.c_str());
                }
            }
            else if(m_type == GUIDE_PVP_BUILDING)
            {
                if(guiderManagerLayer::getInstance()->getStepNum() == 3)
                {
                    Vec2 pos = Vec2(m_winSize.width * 0.06f, m_winSize.height * 0.1f);
                    m_dialogue->setString(m_guideInfo.at(guiderManagerLayer::getInstance()->getStepNum() - 1).desc.c_str());
                    showFingerGuide(pos);
                }
                else if(guiderManagerLayer::getInstance()->getStepNum() == 6)
                {
                    m_bRomoveFlag = true;
                    CPersonalData::getInstance()->setTouchRmvFlag(true);
                }
                else if(guiderManagerLayer::getInstance()->getStepNum() == 10)
                {
                    m_bRomoveFlag = true;
                    CPersonalData::getInstance()->setTouchRmvFlag(true);
                }
            }
            else if(m_type == GUIDE_PVP_RONGLU)
            {
                if(guiderManagerLayer::getInstance()->getStepNum() == 4)
                {
                    m_bRomoveFlag = true;
                }
                else if(guiderManagerLayer::getInstance()->getStepNum() == 8)
                {
                    m_diagBg->removeFromParent();
                    m_diagBg = NULL;
                    m_girl->removeFromParent();
                    m_girl = NULL;
                    Vec2 pos = Vec2(m_winSize.width * 0.56f, m_winSize.height * 0.645f);
                    showFingerGuide(pos);
                }
                else if(guiderManagerLayer::getInstance()->getStepNum() == 10)
                {
                    m_bRomoveFlag = true;
                }
                else{
                    m_dialogue->setString(m_guideInfo.at(guiderManagerLayer::getInstance()->getStepNum() - 1).desc.c_str());
                }
            }
            else if(m_type == GUIDE_PVP_ROB)
            {
                if((guiderManagerLayer::getInstance()->getStepNum() == 3) || (guiderManagerLayer::getInstance()->getStepNum() == 6))
                {
                    m_bRomoveFlag = true;
                }
                else if(guiderManagerLayer::getInstance()->getStepNum() == 5)
                {
                    guiderManagerLayer::getInstance()->removeGuiderLayer();
                }
                else{
                    m_dialogue->setString(m_guideInfo.at(guiderManagerLayer::getInstance()->getStepNum() - 1).desc.c_str());
                }
            }
            else if(m_type == GUIDE_SKILL_BLOW)
            {
                if(guiderManagerLayer::getInstance()->getStepNum() == 4)
                {
                    Vec2 pos = Vec2(m_winSize.width * 0.915f, m_winSize.height * 0.2);
                    showFingerGuide(pos);
                    m_dialogue->setString(m_guideInfo.at(guiderManagerLayer::getInstance()->getStepNum() - 1).desc.c_str());
                }
                else if(guiderManagerLayer::getInstance()->getStepNum() == 7)
                {
                    m_bRomoveFlag = true;
                    guiderManagerLayer::getInstance()->setStepNum(6);
                }
                else{
                    m_dialogue->setString(m_guideInfo.at(guiderManagerLayer::getInstance()->getStepNum() - 1).desc.c_str());
                }
            }
            else if((m_type == GUIDE_SKILL_CUT) && (guiderManagerLayer::getInstance()->getStepNum() == 2))
            {
                Vec2 pos = Vec2(m_winSize.width * 0.915f, m_winSize.height * 0.2);
                showFingerGuide(pos);
                m_dialogue->setString(m_guideInfo.at(guiderManagerLayer::getInstance()->getStepNum() - 1).desc.c_str());
            }
            else if (m_type == GUIDE_TOWER_LEVEL)
            {
                if((guiderManagerLayer::getInstance()->getStepNum() == 3) || (guiderManagerLayer::getInstance()->getStepNum() == 5))
                {
                    m_bRomoveFlag = true;
                    ((CFightScene*)getParent())->m_bifNorAddSolider = true;
                    guiderManagerLayer::getInstance()->setstart(true);
                }
                else if((guiderManagerLayer::getInstance()->getStepNum() == 7) || (guiderManagerLayer::getInstance()->getStepNum() == 9))
                {
                    m_diagBg->removeFromParent();
                    m_diagBg = NULL;
                    m_girl->removeFromParent();
                    m_girl = NULL;
                    Vec2 pos = guiderManagerLayer::getInstance()->m_pos;
                    showFingerGuide(pos);
                }
                else{
                    
                    if(guiderManagerLayer::getInstance()->getStepNum() == 2)
                    {
                        std::string content = m_guideInfo.at(guiderManagerLayer::getInstance()->getStepNum() - 1).desc.c_str();
                        //先将对勾变成绿色
                        std::string::size_type npos = content.find("\“");
                        std::string tempStr = content.substr(0, npos);
                        auto tempStrLabel = Label::createWithTTF(tempStr.c_str(), "fonts/cuti.ttf", 30);
                        npos = tempStrLabel->getStringLength();
                        std::string lastStr = content.substr(tempStr.length(), -1);
                        auto temp = Label::createWithTTF(lastStr.c_str(), "fonts/cuti.ttf", 30);
                        auto BChar = (Sprite*) m_dialogue->getLetter(npos + 1);
                        if(BChar)
                        {
                            BChar->setColor(Color3B(4,153,29));
                        }
                        
                        npos = content.find("\\n");
                        content.replace(npos, 2, "\n");
                        m_dialogue->setString(content.c_str());
                        tempStr = content.substr(0, npos);
                        tempStrLabel = Label::createWithTTF(tempStr.c_str(), "fonts/cuti.ttf", 30);
                        npos = tempStrLabel->getStringLength();
                        
                        lastStr = content.substr(tempStr.length(), -1);
                        temp = Label::createWithTTF(lastStr.c_str(), "fonts/cuti.ttf", 30);
                        int length = npos + temp->getStringLength() + 1;//需要变红的字符串占两行，这里强制+1显示
                        for(int i = npos;i < length; i++)
                        {
                            auto BChar = (Sprite*) m_dialogue->getLetter(i);
                            if(BChar)
                            {
                                BChar->setColor(Color3B(211,82,26));
                            }
                        }
                    }
                    else{
                        m_dialogue->setString(m_guideInfo.at(guiderManagerLayer::getInstance()->getStepNum() - 1).desc.c_str());
                    }
                }
            }
            else if (m_type == GUIDE_MAP_INTRODUCE)   //新增地图介绍 by ly
            {
                
                m_dialogue->setString(m_guideInfo.at(guiderManagerLayer::getInstance()->getStepNum() - 1).desc.c_str());
                
                if(guiderManagerLayer::getInstance()->getStepNum() >= 3&&guiderManagerLayer::getInstance()->getStepNum()<=5){
                    
                    Vec2 pos = Vec2(m_winSize.width * 0.1f, m_winSize.height * 0.95f);
                    showFingerGuide(pos,false);
                    m_back->setOpacity(180);
                    
                }else if (guiderManagerLayer::getInstance()->getStepNum() == 6){
                    
                    CDataManager::saveGuideInfoToDB(guiderManagerLayer::getInstance()->getGuideType());
                    m_bRomoveFlag = true;
                }
                
            }
            else{
                m_dialogue->setString(m_guideInfo.at(guiderManagerLayer::getInstance()->getStepNum() - 1).desc.c_str());
            }
        }
        else
        {
            guiderManagerLayer::getInstance()->setStepNum(0);
            CDataManager::saveGuideInfoToDB(guiderManagerLayer::getInstance()->getGuideType());
            guiderManagerLayer::getInstance()->setNextGuide(true);
            m_bRomoveFlag = true;
            if(m_type == GUIDE_TOWER_LEVEL)
            {
                ((CFightScene*)getParent())->finishTowerLevelGuide();
            }
            else if(m_type == GUIDE_SENIOR_CARD)
            {
                NotificationCenter::getInstance()->postNotification("drawCardAfter");
            }
            else if((m_type == GUIDE_PVP_RONGLU) && (guiderManagerLayer::getInstance()->getStepNum() == 0))
            {
                guiderManagerLayer::getInstance()->removeGuiderLayer();
                CPersonalData::getInstance()->setTouchRmvFlag(true);
            }
        }
    }
    else {
        
        if (m_iTouchState==1) {   //往下传递触摸，手指点不点到到遮罩区域时都往下传递 by ly
            listener->setSwallowTouches(false);
        }else{
            listener->setSwallowTouches(true);
        }
        
//        listener->setSwallowTouches(true);
        if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_ADD_LINE) && (guiderManagerLayer::getInstance()->getStepNum() == 4))
        {
            m_back =LayerColor::create(Color4B(0,0,0,180));
            addChild(m_back);
            m_stencil->removeFromParent();
            m_stencil = NULL;
            m_clip->removeFromParent();
            m_clip = NULL;
            m_finger->removeFromParent();
            m_finger = NULL;
            guiderManagerLayer::getInstance()->setStepNum(guiderManagerLayer::getInstance()->getStepNum() + 1);
            m_dialogue->setString(m_guideInfo.at(guiderManagerLayer::getInstance()->getStepNum() - 1).desc.c_str());
        }
        else if(m_type == GUIDE_LEVEL_TARGET)
        {
            if(guiderManagerLayer::getInstance()->getStepNum() == 1)
            {
                guiderManagerLayer::getInstance()->setStepNum(guiderManagerLayer::getInstance()->getStepNum() + 1);
                m_diagBg->setPosition(Vec2(m_winSize.width * 0.62F, m_winSize.height * 0.4));
                m_girl->setPosition(Vec2(m_winSize.width * 0.87F, m_winSize.height * 0.5));
                Vec2 pos = Vec2(m_winSize.width * 0.5f, m_winSize.height * 0.94);
                m_dialogue->setString(m_guideInfo.at(guiderManagerLayer::getInstance()->getStepNum() - 1).desc.c_str());
                showFingerGuide(pos);
            }
            else if(guiderManagerLayer::getInstance()->getStepNum() == 2)
            {
                CDataManager::saveGuideInfoToDB(guiderManagerLayer::getInstance()->getGuideType());
                m_bRomoveFlag = true;
            }
        }
        else if(m_type == GUIDE_LEVEL_STAR)
        {
            CDataManager::saveGuideInfoToDB(guiderManagerLayer::getInstance()->getGuideType());
        }
        else if((m_type == GUIDE_TOWER_LEVEL) && (guiderManagerLayer::getInstance()->getStepNum() == 3))
        {
            m_back =LayerColor::create(Color4B(0,0,0,180));
            addChild(m_back);
            m_stencil->removeFromParent();
            m_stencil = NULL;
            m_clip->removeFromParent();
            m_clip = NULL;
            m_finger->removeFromParent();
            m_finger = NULL;
            guiderManagerLayer::getInstance()->setStepNum(guiderManagerLayer::getInstance()->getStepNum() + 1);
            m_dialogue->setString(m_guideInfo.at(guiderManagerLayer::getInstance()->getStepNum() - 1).desc.c_str());
        }
        else if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_MAP_INTRODUCE) )    //地图引导 by ly
        {
        
                
            m_back =LayerColor::create(Color4B(0,0,0,180));
            addChild(m_back);
            m_stencil->removeFromParent();
            m_stencil = NULL;
            m_clip->removeFromParent();
            m_clip = NULL;
            m_finger->removeFromParent();
            m_finger = NULL;
            guiderManagerLayer::getInstance()->setStepNum(guiderManagerLayer::getInstance()->getStepNum() + 1);
            
            if (guiderManagerLayer::getInstance()->getStepNum() == 6){
                
                CDataManager::saveGuideInfoToDB(guiderManagerLayer::getInstance()->getGuideType());
                m_bRomoveFlag = true;
            }else{
            
                m_dialogue->setString(m_guideInfo.at(guiderManagerLayer::getInstance()->getStepNum() - 1).desc.c_str());
                
                if (guiderManagerLayer::getInstance()->getStepNum() == 4) {
                    
                    Vec2 pos = Vec2(m_winSize.width * 0.34f, m_winSize.height * 0.95f);
                    showFingerGuide(pos);
                    m_back->setOpacity(180);
                }
                if (guiderManagerLayer::getInstance()->getStepNum() == 5) {
                    
                    Vec2 pos = Vec2(m_winSize.width * 0.55f, m_winSize.height * 0.95f);
                    showFingerGuide(pos);
                    m_back->setOpacity(180);
                }
            }

        }
        else if(guiderManagerLayer::getInstance()->getGuideType() == GUIDE_PVP_RONGLU)
        {
            if(guiderManagerLayer::getInstance()->getStepNum() == 1)
            {
                m_stencil->removeFromParent();
                m_stencil = NULL;
                m_clip->removeFromParent();
                m_clip = NULL;
                m_finger->removeFromParent();
                m_finger = NULL;
                guiderManagerLayer::getInstance()->setStepNum(guiderManagerLayer::getInstance()->getStepNum() + 1);
                m_dialogue->setString(m_guideInfo.at(guiderManagerLayer::getInstance()->getStepNum() - 1).desc.c_str());
            }
            if(guiderManagerLayer::getInstance()->getStepNum() == 8)
            {
                m_bRomoveFlag = true;
            }
        }
        
    }
    return true;
}

void guiderLayer::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event)
{
    if(m_bRomoveFlag)
    {
        m_bRomoveFlag = false;
        if((m_type == GUIDE_SOLIDER_COMPOUND) && (guiderManagerLayer::getInstance()->getStepNum() == 3))
        {
            ((CHeroCompoundLayer*)getParent())->addGuideInfo();
        }
        else if((m_type == GUIDE_USE_MANGONEL) && (guiderManagerLayer::getInstance()->getStepNum() >= 4))
        {
            ((CFightScene*)getParent())->addGuideInfo();
        }
        else if((m_type == GUIDE_SOLIDER_TEAM) && (guiderManagerLayer::getInstance()->getStepNum() == 3))
        {
            ((FightPrepareLayer*)getParent())->addGuideInfo();
        }
        else if((m_type == GUIDE_PVP_BUILDING) && (guiderManagerLayer::getInstance()->getStepNum() == 10))
        {
            ((pvpMainLayer*)getParent())->addGuideInfo(GUIDE_PVP_BUILDING, 10);
        }
        else if(m_type == GUIDE_PVP_RONGLU)
        {
            if(guiderManagerLayer::getInstance()->getStepNum() == 4)
            {
                ((pvpMainLayer*)getParent())->addGuideInfo(GUIDE_PVP_RONGLU, 4);
            }
            else if((guiderManagerLayer::getInstance()->getStepNum() == 8) || (guiderManagerLayer::getInstance()->getStepNum() == 10))
            {
                ((pvpCommonInfoLayer*)getParent())->showGuideInfo();
            }
        }
        else if((m_type == GUIDE_PVP_BUILDING) && ((guiderManagerLayer::getInstance()->getStepNum() == 0)))
        {
            pvpCommonData::getInstance()->setFollowBuild(true);
            CPersonalData::getInstance()->setTouchRmvFlag(true);
            ((pvpMainLayer*)getParent())->detectPvpGuideInfo();
        }
        else if(m_type == GUIDE_PVP_ROB)
        {
            if(guiderManagerLayer::getInstance()->getStepNum() == 3)
            {
                ((pvpMainLayer*)getParent())->addGuideInfo(GUIDE_PVP_ROB, 3);
            }
            else if(guiderManagerLayer::getInstance()->getStepNum() == 6)
            {
                ((CFightScene*)getParent())->addGuideInfo();
            }
            else {
                guiderManagerLayer::getInstance()->removeGuiderLayer();
            }
        }
        else
        {
            guiderManagerLayer::getInstance()->removeGuiderLayer();
        }
        if((m_type == GUIDE_SKILL_UPGRADE) && (guiderManagerLayer::getInstance()->getStepNum() == 10))
        {
            NotificationCenter::getInstance()->postNotification("upgradeCloseGuide");
        }
        else if((m_type == GUIDE_SKILL_CUT) && (guiderManagerLayer::getInstance()->getStepNum() == 4))
        {
            NotificationCenter::getInstance()->postNotification("StartCut");
        }
        else if((m_type == GUIDE_SKILL_BLOW) && (guiderManagerLayer::getInstance()->getStepNum() == 6))
        {
            NotificationCenter::getInstance()->postNotification("StartBlow");
        }
        else if((m_type == GUIDE_GET_AWARD) && (guiderManagerLayer::getInstance()->getStepNum() == 2))
        {
            NotificationCenter::getInstance()->postNotification("getAward");
        }
        else if((m_type == GUIDE_GET_TASK) && (guiderManagerLayer::getInstance()->getStepNum() == 4))
        {
            NotificationCenter::getInstance()->postNotification("guideClose");
        }
        else if((m_type == GUIDE_UPGRADE_SOLIDER) && (guiderManagerLayer::getInstance()->getStepNum() == 8))
        {
            NotificationCenter::getInstance()->postNotification("SoliderCloseGuide");
        }
        else if((m_type == GUIDE_FAIL_TIP) && (guiderManagerLayer::getInstance()->getStepNum() == 2))
        {
            NotificationCenter::getInstance()->postNotification("failGuide");
        }
        else if((m_type == GUIDE_LACK_STAMINA) && (guiderManagerLayer::getInstance()->getStepNum() == 2))
        {
            NotificationCenter::getInstance()->postNotification("exitGuide");
        }
    }
}

void guiderLayer::onExit()
{
    _eventDispatcher->removeEventListener(listener);
    NotificationCenter::getInstance()->removeObserver(this, "SolideVisible");
    CBaseLayer::onExit();
    
}


bool guiderLayer::init()
{
    if(m_showType == GUIDE_SHOW_WND)
    {
        CBaseLayer::initWithColor(Color4B(0, 0, 0, 150));
    }
    else{
        CBaseLayer::init();
    }
    m_winSize = Director::getInstance()->getWinSize();
    m_type = guiderManagerLayer::getInstance()->getGuideType();
    listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(guiderLayer::onTouchBegan, this);
    listener->onTouchEnded = CC_CALLBACK_2(guiderLayer::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    listener->setSwallowTouches(true);
    if(m_showType == GUIDE_SHOW_WND)
    {
        initPerfectIntroduce();
    }
    else
    {
        m_guideInfo = CDataManager::getGuideInfoByType(m_type);
        NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(guiderLayer::solideFinger), "SolideVisible", NULL);
        m_node = Node::create();
        addChild(m_node, 1);
        m_node->setCascadeOpacityEnabled(true);
        m_node->setPosition(Vec2::ZERO);
        m_node->setOpacity(0);
        showGuideUI();
        if(m_diagBg)
        {
            CallFunc* set = CallFunc::create(CC_CALLBACK_0(guiderLayer::setTouchValid, this));
            runAction(Sequence::create(DelayTime::create(0.65f), set, NULL));
        }
        else{
            m_bTouchValid = true;
        }
    }
    
    
    
    
    
    return true;
}

void guiderLayer::initPerfectIntroduce()
{
    Sprite* pBg = Sprite::createWithSpriteFrameName("common_bg.png");
    addChild(pBg);
    pBg->setPosition(Vec2(m_winSize.width * 0.5f, m_winSize.height * 0.52f));
    
    Size bgSize = pBg->getBoundingBox().size;
    Label* pTitle = Label::createWithTTF("完美度介绍","fonts/cuti.ttf",30);;
    pBg->addChild(pTitle);
    pTitle->setColor(Color3B::YELLOW);
    pTitle->setPosition(Vec2(bgSize.width / 2, bgSize.height * 0.895));
    
    Sprite* similarty = Sprite::createWithSpriteFrameName("h1.png");
    pBg->addChild(similarty);
    similarty->setPosition(Vec2(bgSize.width * 0.19f, bgSize.height * 0.68f));
    Label* pSimilartyLabel = Label::createWithTTF("您召唤的士兵将根据您的操作给予评价。","fonts/cuti.ttf",28,
                                                  Size(bgSize.width * 0.6f, 100));
    pBg->addChild(pSimilartyLabel);
    pSimilartyLabel->setAnchorPoint(Vec2(0, 0.5f));
    pSimilartyLabel->setPosition(similarty->getPositionX() + similarty->getContentSize().width * 0.5f + 20, bgSize.height * 0.68f);
    
    Sprite* perfect = Sprite::createWithSpriteFrameName("h3.png");
    pBg->addChild(perfect);
    perfect->setPosition(Vec2(bgSize.width * 0.19f, bgSize.height * 0.43f));
    Label* perfectLabel = Label::createWithTTF("当评价达到perfect时，表示您的操作无懈可击。","fonts/cuti.ttf",28,
                                               Size(bgSize.width * 0.6f, 100));
    pBg->addChild(perfectLabel);
    perfectLabel->setAnchorPoint(Vec2(0, 0.5f));
    perfectLabel->setPosition(perfect->getPositionX() + perfect->getContentSize().width * 0.5f + 20, bgSize.height * 0.43f);
    
    Sprite* solider = Sprite::createWithSpriteFrameName("h2.png");
    pBg->addChild(solider);
    solider->setPosition(Vec2(bgSize.width * 0.19f, bgSize.height * 0.18f));
    Label* pSoliderLabel = Label::createWithTTF("此时召唤的士兵将变为“精英”，各方面属性都有很大幅度提升。","fonts/cuti.ttf",28,
                                                Size(bgSize.width * 0.6f, 100));
    pBg->addChild(pSoliderLabel);
    pSoliderLabel->setAnchorPoint(Vec2(0, 0.5f));
    pSoliderLabel->setPosition(solider->getPositionX() + solider->getContentSize().width * 0.5f + 20, bgSize.height * 0.18f);
    
    Sprite *normal = Sprite::createWithSpriteFrameName("closeBtn.png");
    Sprite *select  = Sprite::createWithSpriteFrameName("closeBtn.png");
    select->setColor(Color3B::GRAY);
    LYMenuItemSprite *pCloseItem = LYMenuItemSprite::create(normal,select,CC_CALLBACK_1(guiderLayer::btnCallBack, this));
    Menu* pMenu =  Menu::create(pCloseItem,NULL);
    pMenu->setPosition(bgSize.width*0.962,bgSize.height*0.89);
    pBg->addChild(pMenu,1);
    
    guiderManagerLayer::getInstance()->setStepNum(0);
    CDataManager::saveGuideInfoToDB(guiderManagerLayer::getInstance()->getGuideType());
}

void guiderLayer::btnCallBack(Ref* pSender)
{
    Director::getInstance()->resume();
    guiderManagerLayer::getInstance()->removeGuiderLayer();
}

void guiderLayer::showFingerGuide(Vec2 pos, bool isBtn)
{
    if (m_back) {
        m_back->removeFromParent();
        m_back = NULL;
    }
    if(!m_clip)
    {
        m_clip = ClippingNode::create();
        m_clip->setInverted(true);//设置底板可见
        m_clip->setAlphaThreshold(0.0f);//设置alpha为0
        addChild(m_clip);
        m_clip->setOpacity(0);
        m_clip->setCascadeOpacityEnabled(true);
    }
    if(!m_back)
    {
        if(!m_diagBg)
        {
            if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_PVP_RONGLU) && (guiderManagerLayer::getInstance()->getStepNum() == 8))
            {
                m_back=LayerColor::create(Color4B(0,0,0,180));
            }
            else{
                m_back=LayerColor::create(Color4B(0,0,0,0));
            }
        }
        else{
            m_back=LayerColor::create(Color4B(0,0,0,180));
        }
        
        m_clip->addChild(m_back);//为裁剪节点添加一个黑色带透明（看起了是灰色）的底板
    }
    if(!m_stencil)
    {
        m_stencil = Sprite::createWithSpriteFrameName("guide_circile.png");
        m_stencil->setPosition(pos);
        m_clip->setStencil(m_stencil);
        m_finger = Sprite::createWithSpriteFrameName("finger_1.png");
        m_node->addChild(m_finger, 10);
        m_finger->setPosition(Vec2(pos.x + m_stencil->getContentSize().width* 1.5f , m_stencil->getPositionY() - m_finger->getContentSize().height - 5));
    }
    else{
        m_stencil->setPosition(pos);
        m_finger->setPosition(Vec2(m_stencil->getPositionX() + m_stencil->getContentSize().width * 0.85 , m_stencil->getPositionY() - m_finger->getContentSize().height + 15));
    }
    m_stencil->setScaleX(1.2f);
    m_stencil->setScaleY(0.7f);
    if(isBtn)
    {
        m_finger->setPosition(m_stencil->getPositionX() + m_stencil->getContentSize().width* 0.7f, m_stencil->getPositionY() * 0.4f);
    }
    if((m_type == GUIDE_ADD_LINE) && (guiderManagerLayer::getInstance()->getStepNum() == 4))
    {
        m_stencil->setScaleX(3.2);
        m_stencil->setScaleY(0.45f);
        m_finger->setPosition(m_stencil->getPositionX() + m_stencil->getBoundingBox().size.width* 0.6f, m_stencil->getPositionY()  - m_stencil->getBoundingBox().size.height * 0.45f);
    }
    if((m_type == GUIDE_SKILL_UPGRADE) && (guiderManagerLayer::getInstance()->getStepNum() == 5))
    {
        m_stencil->setScaleX(1.05f);
        m_stencil->setScaleY(0.6f);
        m_finger->setPosition(m_stencil->getPositionX() + m_stencil->getBoundingBox().size.width* 0.6f, m_stencil->getPositionY()  - m_stencil->getBoundingBox().size.height * 0.45f);
    }
    if((m_type == GUIDE_USE_MANGONEL) && (guiderManagerLayer::getInstance()->getStepNum() == 2))
    {
        m_stencil->setScale(0.68);
        m_finger->setPosition(m_stencil->getPositionX() + m_stencil->getBoundingBox().size.width, m_stencil->getPositionY()  - m_stencil->getBoundingBox().size.height * 0.45f);
    }
    if(m_type == GUIDE_COMMON_CARD)
    {
        if(guiderManagerLayer::getInstance()->getStepNum() == 2)
        {
            m_stencil->setScale(0.8);
            m_finger->setPosition(m_stencil->getPositionX() + m_stencil->getBoundingBox().size.width * 0.8f, m_stencil->getPositionY()  - m_stencil->getBoundingBox().size.height * 0.45f);
        }
        else if(guiderManagerLayer::getInstance()->getStepNum() == 4)
        {
            m_stencil->setScale(0.65);
            m_finger->setPosition(m_stencil->getPositionX() + m_stencil->getBoundingBox().size.width * 0.72f , m_stencil->getPositionY()  - m_stencil->getBoundingBox().size.height * 0.45f);
        }
    }
    if(m_type == GUIDE_SENIOR_CARD)
    {
        if(guiderManagerLayer::getInstance()->getStepNum() == 4)
        {
            m_stencil->setScale(0.65);
            m_finger->setPosition(m_stencil->getPositionX() + m_stencil->getBoundingBox().size.width * 0.72f , m_stencil->getPositionY()  - m_stencil->getBoundingBox().size.height * 0.45f);
        }
    }
    if(m_type == GUIDE_PVP_BUILDING)
    {
        if(guiderManagerLayer::getInstance()->getStepNum() == 3)
        {
            m_stencil->setScale(0.8);
            m_finger->setPosition(m_stencil->getPositionX() + m_stencil->getBoundingBox().size.width * 0.8f, m_stencil->getPositionY()  - m_stencil->getBoundingBox().size.height * 0.45f);
        }
    }
    if((m_type == GUIDE_LEVEL_TARGET) && (guiderManagerLayer::getInstance()->getStepNum() == 2))
    {
        m_stencil->setScaleX(0.9);
        m_stencil->setScaleY(0.4);
        m_finger->setPosition(m_stencil->getPositionX() + m_stencil->getBoundingBox().size.width* 0.72f, m_stencil->getPositionY()  - m_stencil->getBoundingBox().size.height * 0.38f);
    }
    if(m_type == GUIDE_TOWER_LEVEL)
    {
        if(guiderManagerLayer::getInstance()->getStepNum() == 7)
        {
            m_stencil->setScaleX(0.7);
            m_stencil->setScaleY(1.2);
            m_stencil->setPositionY(m_stencil->getPositionY() + 20);
            m_finger->setPosition(m_stencil->getPositionX() + m_stencil->getBoundingBox().size.width * 0.3f, m_stencil->getPositionY() -  m_stencil->getBoundingBox().size.height * 0.3f);
        }
        else if(guiderManagerLayer::getInstance()->getStepNum() == 9)
        {
            m_stencil->setScale(0.6);
            m_finger->setPosition(m_stencil->getPositionX() + m_stencil->getBoundingBox().size.width * 0.75f, m_stencil->getPositionY() -  m_stencil->getBoundingBox().size.height * 0.6f);
        }
        

    }
    if(m_type == GUIDE_PVP_RONGLU)
    {
        if(guiderManagerLayer::getInstance()->getStepNum() == 8)
        {
            m_stencil->setScaleX(1.1);
            m_stencil->setScaleY(0.5);
            m_finger->setPosition(m_stencil->getPositionX() + m_stencil->getBoundingBox().size.width * 0.55f, m_stencil->getPositionY() -  m_stencil->getBoundingBox().size.height * 0.65f);
        }
    }
    if(((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SKILL_CUT) && (guiderManagerLayer::getInstance()->getStepNum() == 2))
       || ((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SKILL_BLOW) && (guiderManagerLayer::getInstance()->getStepNum() == 4)))
    {
        m_stencil->setScale(0.8f);
        m_finger->setPosition(m_stencil->getPositionX() + m_stencil->getBoundingBox().size.width * 0.22f, m_stencil->getPositionY() -  m_stencil->getBoundingBox().size.height * 0.54f);
    }
    
    if(m_type == GUIDE_MAP_INTRODUCE)  //地图介绍的手指位置  by ly
    {
        m_finger->setPosition(m_stencil->getPositionX() + m_stencil->getBoundingBox().size.width * 0.5f+m_finger->getBoundingBox().size.width*0.5f, m_stencil->getPositionY() -  m_stencil->getBoundingBox().size.height * 0.3f);
        
    }

    
    
    
    Animation* pAnimation = AnimationCache::getInstance()->getAnimation("fingerAnimation");
    m_finger->runAction(RepeatForever::create(Animate::create(pAnimation)));
    m_clip->runAction(FadeIn::create(0.5f));
}

void guiderLayer::showDialog()
{
    m_diagBg = Sprite::createWithSpriteFrameName("dialog.png");
    m_node->addChild(m_diagBg, 1);
    m_diagBg->setCascadeOpacityEnabled(true);
    m_diagBg->setPosition(Vec2(m_winSize.width * 0.5f, m_winSize.height * 0.55));
    
    std::string content = m_guideInfo.at(guiderManagerLayer::getInstance()->getStepNum() - 1).desc;
    std::string::size_type npos = content.find("userName");
    if(npos != std::string::npos)
    {
        content.replace(npos, 8, CPersonalData::getInstance()->getUserInfo()->userName.c_str());
    }
    
    m_dialogue = Label::createWithTTF(content.c_str(), "fonts/cuti.ttf", 30, Size(460, 200));
    m_diagBg->addChild(m_dialogue, 1);
    m_dialogue->setColor(Color3B::BLACK);
    m_dialogue->setAnchorPoint(Vec2(0, 0.5));
    m_dialogue->setPosition(50, m_diagBg->getContentSize().height * 0.4f);
    
    m_girl = Sprite::createWithSpriteFrameName("girl.png");
    m_girl->setScale(0.75f);
    m_node->addChild(m_girl, 1);
    m_girl->setPosition(Vec2(m_winSize.width * 0.76f, m_winSize.height * 0.53));
    
    if(m_bIfShadow){       //添加是否背景黑掉的代码
        m_back =LayerColor::create(Color4B(0,0,0,180));
    }else{
        m_back =LayerColor::create(Color4B(0,0,0,0));
    }
    
    addChild(m_back);
    m_back->setOpacity(0);
    if(guiderManagerLayer::getInstance()->getGuideType() == GUIDE_GET_TASK)
    {
        if(guiderManagerLayer::getInstance()->getStepNum() == 1)
        {
            m_diagBg->setPosition(Vec2(m_winSize.width * 0.56, m_winSize.height * 0.5f));
            m_girl->setPosition(Vec2(m_winSize.width * 0.81, m_winSize.height * 0.5f));
        }
        else if(guiderManagerLayer::getInstance()->getStepNum() == 3)
        {
            m_diagBg->setPosition(Vec2(m_winSize.width * 0.52, m_winSize.height * 0.55f));
            m_girl->setPosition(Vec2(m_winSize.width * 0.77, m_winSize.height * 0.55f));
        }
        else
        {
            m_diagBg->setPosition(Vec2(m_winSize.width * 0.5, m_winSize.height * 0.4f));
            m_dialogue->setPosition(Vec2(m_diagBg->getContentSize().width * 0.58, m_diagBg->getContentSize().height * 0.4f));
            m_girl->setPosition(Vec2(m_winSize.width * 0.27, m_winSize.height * 0.4f));
            m_girl->setFlippedX(true);
        }
        
        
        
        
        
    }
    else if((guiderManagerLayer::getInstance()->getStepNum() == 4) && (guiderManagerLayer::getInstance()->getGuideType() == GUIDE_UPGRADE_SOLIDER))
    {
        m_diagBg->setPosition(Vec2(m_winSize.width * 0.54, m_winSize.height * 0.63f));
        m_girl->setPosition(Vec2(m_winSize.width * 0.79 , m_winSize.height * 0.63f));
    }
    else if((guiderManagerLayer::getInstance()->getStepNum() == 4) && (guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SOLIDER_NUM))
    {
        m_diagBg->setPosition(Vec2(m_winSize.width * 0.32, m_winSize.height * 0.6f));
        m_girl->setPosition(Vec2(m_winSize.width * 0.55 , m_winSize.height * 0.7f));
    }
    else if((m_showType == GUIDE_SHOW_BOTH) && (guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SKILL_UPGRADE))
    {
        if(guiderManagerLayer::getInstance()->getStepNum() == 4)
        {
            m_diagBg->setPosition(Vec2(m_winSize.width * 0.5, m_winSize.height * 0.4f));
            m_dialogue->setPosition(Vec2(m_diagBg->getContentSize().width * 0.58, m_diagBg->getContentSize().height * 0.4f));
            m_girl->setPosition(Vec2(m_winSize.width * 0.27, m_winSize.height * 0.4f));
            m_girl->setFlippedX(true);
        }
        else if(guiderManagerLayer::getInstance()->getStepNum() == 8)
        {
            m_diagBg->setPosition(Vec2(m_winSize.width * 0.5, m_winSize.height * 0.42f));
            m_dialogue->setPosition(Vec2(m_diagBg->getContentSize().width * 0.4, m_diagBg->getContentSize().height * 0.4f));
            m_girl->setPosition(Vec2(m_winSize.width * 0.26, m_winSize.height * 0.44f));
            m_girl->setFlippedX(true);
            
        }
    }
    else if(guiderManagerLayer::getInstance()->getGuideType() == GUIDE_ENTER_STORAGE)
    {
        if(guiderManagerLayer::getInstance()->getStepNum() == 3)
        {
            m_diagBg->setPositionY(m_winSize.height * 0.4);
            m_girl->setPositionY(m_winSize.height * 0.5);
        }
        if(guiderManagerLayer::getInstance()->getStepNum() == 4)
        {
            m_girl->setFlippedX(true);
            m_diagBg->setPosition(Vec2(m_winSize.width * 0.49, m_winSize.height * 0.45f));
            m_dialogue->setPosition(Vec2(m_diagBg->getContentSize().width * 0.4, m_diagBg->getContentSize().height * 0.4f));
            m_girl->setPosition(Vec2(m_winSize.width * 0.26, m_winSize.height * 0.5f));
        }
        if(guiderManagerLayer::getInstance()->getStepNum() == 6 )
        {
            m_diagBg->setPosition(Vec2(m_winSize.width * 0.48, m_winSize.height * 0.48f));
            m_girl->setPosition(Vec2(m_winSize.width * 0.73, m_winSize.height * 0.58f));
        }
    }
    else if(((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SKILL_BLOW) && (guiderManagerLayer::getInstance()->getStepNum() == 5))||((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SKILL_CUT) && (guiderManagerLayer::getInstance()->getStepNum() == 3)))
    {
        m_diagBg->setPosition(Vec2(m_winSize.width * 0.46, m_winSize.height * 0.55f));
        m_dialogue->setPosition(Vec2(m_diagBg->getContentSize().width * 0.4, m_diagBg->getContentSize().height * 0.4f));
        m_girl->setPosition(Vec2(m_winSize.width * 0.22, m_winSize.height * 0.565f));
        m_girl->setFlippedX(true);
    }
    else if(guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SOLIDER_COMPOUND)
    {
        if(guiderManagerLayer::getInstance()->getStepNum() == 2)
        {
            m_diagBg->setPosition(Vec2(m_winSize.width * 0.54, m_winSize.height * 0.45f));
            m_girl->setPosition(Vec2(m_winSize.width * 0.73, m_winSize.height * 0.45f));
        }
    }
    else if(guiderManagerLayer::getInstance()->getGuideType() == GUIDE_PVP_BUILDING)
    {
        if(guiderManagerLayer::getInstance()->getStepNum() == 1)
        {
            m_diagBg->setPosition(Vec2(m_winSize.width * 0.5, m_winSize.height * 0.5f));
            m_dialogue->setPosition(Vec2(m_diagBg->getContentSize().width * 0.42, m_diagBg->getContentSize().height * 0.4f));
            m_girl->setPosition(Vec2(m_winSize.width * 0.27, m_winSize.height * 0.5f));
            m_girl->setFlippedX(true);
        }
        else  if(guiderManagerLayer::getInstance()->getStepNum() == 9)
        {
            m_diagBg->setPosition(Vec2(m_winSize.width * 0.53, m_winSize.height * 0.6f));
            m_girl->setPosition(Vec2(m_winSize.width * 0.75, m_winSize.height * 0.58f));
        }

    }
//    else if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SKILL_CUT) && (guiderManagerLayer::getInstance()->getStepNum() == 3))
//    {
//        m_diagBg->setPosition(Vec2(m_winSize.width * 0.53, m_winSize.height * 0.5));
//        m_girl->setPosition(Vec2(m_winSize.width * 0.67, m_winSize.height * 0.66f));
//    }
}

void guiderLayer::showFinger()
{
    m_clip = ClippingNode::create();
    m_clip->setInverted(true);//设置底板可见
    m_clip->setAlphaThreshold(0.1f);//设置alpha为0
    addChild(m_clip);
    
    if(m_back)
    {
        m_back->removeFromParent();
        m_back = NULL;
    }
    if(m_showType == GUIDE_SHOW_FINGER||!m_bIfShadow)
    {
        m_back=LayerColor::create(Color4B(0,0,0,0));
    }
    else{
        m_back = LayerColor::create(Color4B(0,0,0,180));
    }
    
    m_clip->addChild(m_back);//为裁剪节点添加一个黑色带透明（看起了是灰色）的底板
    m_clip->setOpacity(0);
    m_clip->setCascadeOpacityEnabled(true);
    m_stencil = Sprite::createWithSpriteFrameName("guide_circile.png");
    m_stencil->setPosition(m_pos);//Vec2(15, m_winSize.height - 15));
    m_stencil->setScale(0.95f);
    
    m_clip->setStencil(m_stencil);
    m_finger = Sprite::createWithSpriteFrameName("finger_1.png");
    
    m_node->addChild(m_finger, 10);
    m_finger->setPosition(m_stencil->getPositionX() + m_stencil->getBoundingBox().size.width * 0.6f, m_stencil->getPositionY() -  m_stencil->getBoundingBox().size.height * 0.3f);
    m_stencil->setScaleX(1.2f);
    m_stencil->setScaleY(0.7f);
    if((m_showType == GUIDE_SHOW_BOTH) && (guiderManagerLayer::getInstance()->getGuideType() == GUIDE_LEVEL_TARGET))
    {
        m_stencil->setScaleX(2.0);
        m_stencil->setScaleY(0.45f);
    }
    else if((m_showType == GUIDE_SHOW_BOTH) && (guiderManagerLayer::getInstance()->getGuideType() == GUIDE_GET_TASK))
    {
        if (guiderManagerLayer::getInstance()->getStepNum() == 1) {
            m_stencil->setScale(0.8f);
        }
    }
    else if((m_showType == GUIDE_SHOW_FINGER) && (guiderManagerLayer::getInstance()->getGuideType() == GUIDE_GET_TASK))
    {
        if(guiderManagerLayer::getInstance()->getStepNum() == 4)
        {
            m_stencil->setScale(0.48f);
        }
        if(guiderManagerLayer::getInstance()->getStepNum() == 5)
        {
            m_stencil->setScale(0.8f);
        }
    }
    else if((m_showType == GUIDE_SHOW_BOTH) && (guiderManagerLayer::getInstance()->getGuideType() == GUIDE_LEVEL_STAR))
    {
        m_stencil->setPositionY(m_winSize.height * 0.78f);
        m_stencil->setScaleX(0.9f);
        m_stencil->setScaleY(0.6f);
        m_finger->setPosition(m_stencil->getPositionX() + m_stencil->getBoundingBox().size.width * 0.45f, m_stencil->getPositionY() -  m_stencil->getBoundingBox().size.height * 0.54f);
    }
    else if((m_showType == GUIDE_SHOW_BOTH) && (guiderManagerLayer::getInstance()->getGuideType() == GUIDE_UPGRADE_SOLIDER))
    {
        if (guiderManagerLayer::getInstance()->getStepNum() == 4) {
            m_stencil->setScaleX(3.85);
            m_stencil->setScaleY(0.47f);
        }
    }
    else if((m_showType == GUIDE_SHOW_FINGER) && (guiderManagerLayer::getInstance()->getGuideType() == GUIDE_UPGRADE_SOLIDER))
    {
        if((guiderManagerLayer::getInstance()->getStepNum() == 3) || (guiderManagerLayer::getInstance()->getStepNum() == 5))
        {
            m_stencil->setScaleX(0.95);
            m_stencil->setScaleY(0.6f);
        }
        else if(guiderManagerLayer::getInstance()->getStepNum() == 4)
        {
            m_stencil->setScaleX(1.1f);
            m_stencil->setScaleY(0.7f);
        }
        else if (guiderManagerLayer::getInstance()->getStepNum() == 8) {
            m_stencil->setScale(0.48f);
        }
        else if(guiderManagerLayer::getInstance()->getStepNum() == 9)
        {
            m_stencil->setScale(0.8f);
        }
    }
    else if(guiderManagerLayer::getInstance()->getGuideType() == GUIDE_FAIL_TIP)
    {
        m_stencil->setScaleX(0.9f);
        m_stencil->setScaleY(0.4f);
    }
    else if(guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SOLIDER_NUM)
    {
        if(guiderManagerLayer::getInstance()->getStepNum() == 3)
        {
            m_stencil->setScaleX(1.1f);
            m_stencil->setScaleY(0.48f);
        }
        else if(guiderManagerLayer::getInstance()->getStepNum() == 4)
        {
            m_diagBg->setPosition(m_winSize.width * 0.5f, m_winSize.height * 0.45f);
            m_dialogue->setPosition(Vec2(m_diagBg->getContentSize().width * 0.4, m_diagBg->getContentSize().height * 0.4f));
            m_girl->setPosition(Vec2(m_winSize.width * 0.255, m_winSize.height * 0.45f));
            m_girl->setFlippedX(true);
            m_finger->setPositionX(m_finger->getPositionX() + 15);
        }
        else if(guiderManagerLayer::getInstance()->getStepNum() == 5)
        {
            m_stencil->setScale(0.48f);
        }
        else if(guiderManagerLayer::getInstance()->getStepNum() == 6)
        {
            m_stencil->setScale(0.8f);
        }
    }
    else if((m_showType == GUIDE_SHOW_FINGER) && (guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SKILL_UPGRADE))
    {
        if(guiderManagerLayer::getInstance()->getStepNum() == 6)
        {
            m_stencil->setScale(0.6f);
        }
        else if(guiderManagerLayer::getInstance()->getStepNum() == 7)
        {
            m_stencil->setScaleX(0.95f);
            m_stencil->setScaleY(0.6f);
            m_finger->setPosition(m_stencil->getPositionX() + m_stencil->getBoundingBox().size.width * 0.85f, m_stencil->getPositionY() -  m_stencil->getBoundingBox().size.height * 0.65f);
        }
        else if ((guiderManagerLayer::getInstance()->getStepNum() == 10) || (guiderManagerLayer::getInstance()->getStepNum() == 11)) {
            m_stencil->setScale(0.48f);
        }
        else if(guiderManagerLayer::getInstance()->getStepNum() == 12)
        {
            m_stencil->setScale(0.8f);
        }
    }
    else if((m_showType == GUIDE_SHOW_FINGER) && (guiderManagerLayer::getInstance()->getGuideType() == GUIDE_LACK_STAMINA))
    {
        if(guiderManagerLayer::getInstance()->getStepNum() == 2)
        {
            m_stencil->setScale(0.48f);
        }
        else if(guiderManagerLayer::getInstance()->getStepNum() == 3)
        {
            m_stencil->setScale(0.8f);
        }
    }
    else if((m_showType == GUIDE_SHOW_BOTH) && (guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SKILL_UPGRADE))
    {
        if (guiderManagerLayer::getInstance()->getStepNum() == 8) {
            m_stencil->setScaleX(0.62f);
            m_stencil->setScaleY(0.28f);
            m_finger->setPosition(m_stencil->getPositionX() + m_stencil->getBoundingBox().size.width * 0.85f, m_stencil->getPositionY() -  m_stencil->getBoundingBox().size.height * 0.65f);
        }
    }
    else if((m_showType == GUIDE_SHOW_BOTH) && (guiderManagerLayer::getInstance()->getGuideType() == GUIDE_ENTER_STORAGE))
    {
        if(guiderManagerLayer::getInstance()->getStepNum() == 2)
        {
            m_stencil->setScaleY(0.4f);
            m_finger->setPosition(m_stencil->getPositionX() + m_stencil->getBoundingBox().size.width * 0.66f, m_stencil->getPositionY() -  m_stencil->getBoundingBox().size.height * 0.54f);
        }
        if(guiderManagerLayer::getInstance()->getStepNum() == 3)
        {
            m_stencil->setScale(0.9f);
        }
        if(guiderManagerLayer::getInstance()->getStepNum() == 4)
        {
            m_stencil->setScaleX(0.85f);
            m_stencil->setScaleY(0.48f);
        }
        if(guiderManagerLayer::getInstance()->getStepNum() == 6)
        {
            m_stencil->setScale(0.48f);
        }
    }
    else if(guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SENIOR_CARD)
    {
        if(guiderManagerLayer::getInstance()->getStepNum() == 1)
        {
            m_stencil->setScale(0.8);
            m_finger->setPosition(m_stencil->getPositionX() + m_stencil->getBoundingBox().size.width * 0.8f, m_stencil->getPositionY()  - m_stencil->getBoundingBox().size.height * 0.45f);
        }
        else{
            m_stencil->setScale(0.65);
            m_finger->setPosition(m_stencil->getPositionX() + m_stencil->getBoundingBox().size.width * 0.72f , m_stencil->getPositionY()  - m_stencil->getBoundingBox().size.height * 0.45f);
        }
    }
    else if(guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SOLIDER_COMPOUND)
    {
        if(guiderManagerLayer::getInstance()->getStepNum() == 2)
        {
            m_stencil->setScale(0.85f);
        }
        else if(guiderManagerLayer::getInstance()->getStepNum() == 3)
        {
            m_stencil->setScale(0.8);
            m_finger->setPosition(m_stencil->getPositionX() + m_stencil->getBoundingBox().size.width * 0.8f, m_stencil->getPositionY()  - m_stencil->getBoundingBox().size.height * 0.45f);
        }
        else if(guiderManagerLayer::getInstance()->getStepNum() == 4)
        {
            m_stencil->setScaleX(0.85);
            m_stencil->setScaleY(0.5);
            m_finger->setPosition(m_stencil->getPositionX() + m_stencil->getBoundingBox().size.width * 0.8f, m_stencil->getPositionY()  - m_stencil->getBoundingBox().size.height * 0.45f);
        }
    }
//    else if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SKILL_CUT) && (guiderManagerLayer::getInstance()->getStepNum() == 3))
//    {
//        m_finger->setPosition(Vec2(m_winSize.width * 0.3f, m_winSize.height * 0.02));
//        ActionInterval* action1 = MoveBy::create(1.25f, Vec2(300, 0));
//        ActionInterval* action2 = MoveBy::create(0, Vec2(-300, 0));
//        ActionInterval* seq = RepeatForever::create(Sequence::create(action1, action2, NULL));
//        m_finger->runAction(seq);
//    }
    else if(guiderManagerLayer::getInstance()->getGuideType() == GUIDE_GET_AWARD)
    {
        if(guiderManagerLayer::getInstance()->getStepNum() == 2)
        {
            m_stencil->setScaleX(0.8);
            m_stencil->setScaleY(0.4);
            m_finger->setPosition(m_stencil->getPositionX() + m_stencil->getBoundingBox().size.width* 0.7f, m_stencil->getPositionY()  - m_stencil->getBoundingBox().size.height * 0.35f);

        }
        if(guiderManagerLayer::getInstance()->getStepNum() == 3)
        {
            m_stencil->setScale(0.48f);
        }
    }
    else if((m_showType == GUIDE_SHOW_BOTH) && ((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SKILL_BLOW)
                                                || (guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SKILL_CUT)))
    {
        if((guiderManagerLayer::getInstance()->getStepNum() == 5) || (guiderManagerLayer::getInstance()->getStepNum() == 3))
        {
            m_stencil->setScale(0.64f);
        }
        if(guiderManagerLayer::getInstance()->getGuideType() == GUIDE_SKILL_BLOW)
        {
            m_finger->setPosition(m_stencil->getPositionX() + m_stencil->getBoundingBox().size.width * 0.65f, m_stencil->getPositionY() -  m_stencil->getBoundingBox().size.height * 0.65f);
        }
        else{
            m_finger->setPosition(m_stencil->getPositionX() + m_stencil->getBoundingBox().size.width * 0.4f, m_stencil->getPositionY() -  m_stencil->getBoundingBox().size.height * 0.65f);
        }
    }
    else if(guiderManagerLayer::getInstance()->getGuideType() == GUIDE_PVP_BUILDING)
    {
        if(guiderManagerLayer::getInstance()->getStepNum() == 1)
        {
            m_stencil->setScale(0.75f);
        }
        else if(guiderManagerLayer::getInstance()->getStepNum() == 4)
        {
            m_stencil->setScaleX(1.3f);
            m_stencil->setScaleY(2.0f);
        }
        else if(guiderManagerLayer::getInstance()->getStepNum() == 7)
        {
            m_stencil->setScale(0.4f);
        }
        else if(guiderManagerLayer::getInstance()->getStepNum() == 8)
        {
            m_stencil->setScale(0.7f);
        }
        else if(guiderManagerLayer::getInstance()->getStepNum() == 10)
        {
            m_stencil->setScale(0.7f);
        }
    }
    else if(guiderManagerLayer::getInstance()->getGuideType() == GUIDE_PVP_RONGLU)
    {
        if(guiderManagerLayer::getInstance()->getStepNum() == 1)
        {
            m_stencil->setScaleY(0.6f);
        }
        if((guiderManagerLayer::getInstance()->getStepNum() == 4) || (guiderManagerLayer::getInstance()->getStepNum() == 5))
        {
            m_stencil->setScale(0.7f);
        }
    }
    else if(guiderManagerLayer::getInstance()->getGuideType() == GUIDE_PVP_ROB)
    {
        if(guiderManagerLayer::getInstance()->getStepNum() == 3)
        {
            m_stencil->setScale(0.75f);
        }
    }
    else if(guiderManagerLayer::getInstance()->getGuideType() == GUIDE_TOWER_LEVEL)
    {
        if(guiderManagerLayer::getInstance()->getStepNum() == 3)
        {
            m_stencil->setScaleY(0.6f);
        }
        else if(guiderManagerLayer::getInstance()->getStepNum() == 5)
        {
            m_stencil->setScale(0.5f);
        }
        else if(guiderManagerLayer::getInstance()->getStepNum() == 10)
        {
            m_stencil->setScale(0.6f);
        }
    }
    if((guiderManagerLayer::getInstance()->getGuideType() == GUIDE_UPGRADE_SOLIDER) && (guiderManagerLayer::getInstance()->getStepNum() == 4))
    {
        m_finger->setPositionX(m_finger->getPositionX() * 0.72f);
        ActionInterval* action1 = MoveBy::create(1.2f, Vec2(300, 0));
        ActionInterval* action2 = MoveBy::create(0, Vec2(-300, 0));
        ActionInterval* seq = Sequence::create(action1, action2, NULL);
        m_finger->runAction(RepeatForever::create(seq));
    }
    else{
        Animation* pAnimation = AnimationCache::getInstance()->getAnimation("fingerAnimation");
        m_finger->runAction(RepeatForever::create(Animate::create(pAnimation)));
    }
    
}

void guiderLayer::solideFinger(Ref *pSender)
{
    m_finger->setVisible(!m_finger->isVisible());
}


void guiderLayer::showGuideUI()
{
    if(m_showType == GUIDE_SHOW_DIALOG)
    {
        showDialog();
    }
    else if(m_showType == GUIDE_SHOW_FINGER)
    {
        showFinger();
    }
    
    if(m_showType == GUIDE_SHOW_BOTH)
    {
        showDialog();
        showFinger();
    }
    if(m_clip)
    {
        m_clip->runAction(FadeIn::create(0.5f));
    }
    else
    {
        
        if (!m_bIfShadow) {  //如果背景不暗掉，则啥也不做

        }
        else if(m_showType == GUIDE_SHOW_FINGER)
        {
            m_back->runAction(FadeTo::create(0.5f, 225));
        }
        else{
            m_back->runAction(FadeTo::create(0.5f, 180));
        }
    }
    m_node->runAction(FadeIn::create(0.5f));
}

