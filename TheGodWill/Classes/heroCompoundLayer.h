//
//  CHeroCompoundLayer.h
//  TheGodWill
//
//  Created by user on 14/12/24.
//
//

#ifndef __TheGodWill__CHeroCompoundLayer__
#define __TheGodWill__CHeroCompoundLayer__

#include <stdio.h>
#include "cocos2d.h"

#include "BaseLayer.h"
#include "AllDefine.h"

USING_NS_CC;


enum{
    LEFT_ARROW_TAG = 100,
    RIGHT_ARROW_TAG,
    EXIT_TAG,
    ADVANCE_START_TAG,
    BUY_DEBRIS_TAG,
    HREO_INFO_TAG,
    BACK_HERO_INFO_TAG,
    HERO_ELECTED_KUANG,
};

struct skill_show{
    std::string skillName;
    bool isHave;//是否显示该技能
};

class CHeroCompoundLayer:public CBaseLayer
{
public:
    CHeroCompoundLayer();
    ~CHeroCompoundLayer();
    CREATE_FUNC(CHeroCompoundLayer);
    bool init();
    void initUI();
    void addHeroInfo(const char* normalImg, const char* selectImg, Point pos, Node* parent, int tag);
    void adjustHeroInfo();
    void heroDetails();//英雄详情
    void updateHeroShow();//英雄介绍
    void btnCallBack(Ref* pSender);
    
    void sortHero();//英雄排序，优先级次序：星级>材料数
    void addSpriteByImg(Sprite* parent, const char* img);
    virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
    virtual void onTouchMoved(Touch *touch, Event *unused_event);
    virtual void onTouchEnded(Touch *touch, Event *unused_event);
    
    void saveBuyResult(int stoneNum,int dibrisNum);//购买碎片
    void showHero();
    void OnSkillTouch(Ref *pSender);
    void OnUpdateCoinAndStone(Ref *pSender);
    void OnSaveBuyResult(Ref *pSender);
    void addGuideInfo();
    void addCloseGuide();//添加关闭引导
private:
    Size m_winSize;
    Sprite* m_bg ;
    Node* m_node;
    Menu* m_pMenu;
    int m_page;//当前页
    int m_maxPage;
    Label* m_pageLabel;
    LYMenuItemSprite* m_leftItem;
    LYMenuItemSprite* m_rightItem;
    Sprite3D* m_hero;
    Layer* m_layer;
    int m_curShow;//1表示为英雄详情,0表示英雄属性
    int m_selectedHeroId;//记录选中的英雄id
    std::vector<skillInfo*> m_skill;//当前英雄可使用技能
    bool m_bisLackMaterial;//是否当前进阶材料不足
    Menu * m_pSKillMenu;
    bool m_bSkillMenu;
    Point m_starPos;
    Menu* m_otherMenu;
    int  m_dibrisNum;
    std::vector<int> m_sortVec;
    std::vector<int> m_allMonster;//怪物总数
    Sprite* m_InfoBg;
    LYMenuItemSprite* m_exitItem;
    Vec2 m_guidePos;
    int m_validMonsterNum;//当前游戏中可以用的怪物数量
};


#endif /* defined(__TheGodWill__CHeroCompoundLayer__) */
