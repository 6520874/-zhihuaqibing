//
//  SkillUpLayer.h
//  TheGodWill
//
//  Created by prefer_shan on 14/12/22.
//
//

#ifndef __TheGodWill__SkillUpLayer__
#define __TheGodWill__SkillUpLayer__

#include <stdio.h>
#include "cocos2d.h"
#include "AllDefine.h"
#include "BaseLayer.h"
USING_NS_CC;

//技能升级主操作界面
class CSkillUpLayer:public CBaseLayer
{
public:
    static CSkillUpLayer* create(int index,int iTag,int SkillIndex,bool bHero=false);
    void RefreshUI();
    
private:
    CSkillUpLayer(int index,int iTag,int SkillIndex,bool bHero);
    ~CSkillUpLayer();
    bool init();
    void initUI();//技能详情
    void heroSkillInfo();//英雄技能升级
    void soliderSkillInfo();//士兵技能升级
    void skillPropInfo();//技能属性详情
    void showBuyItem();//显示需要购买的道具数量及价格
    void buyAfterCallBack(Ref *pSender);//购买之后的回调
    void btnCallBack(Ref* pSender);
    void buyMaterialCallBack(Ref* pSender);
    void confirmBuyBack(Ref* pSender);
    
    bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
    void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event) ;
    void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event);
    void onTouchCancelled(Touch *touch, Event *event);
    void OnSkillUpMenu(Ref *pSender);
    void OnRemoveNotice();
    void onExit();
    void OnCloseMenu(Ref *pSender);
    
    
    void gotoShop(Ref* pSender);//去充值界面
    
    Menu *m_pMenuLvUp;
    bool m_bMenuLvUp;
    Menu *m_pMenuClose;
    bool m_bMenuClose;
    bool m_bHero;//标识是否是英雄技能升级
    int m_index;
    int m_iSkillIdIndex;//当前页面的第几个技能
    int m_iThreeOfInex;//升级的三个技能中的第几个
    int m_iNeedNum1;
    int m_iHaveNum1;
    int m_iNeedNum2;
    int m_iHaveNum2;
    int m_iNeedNum3;
    int m_iHaveNum3;
    Label *m_pLabel1Have;
    //Label *m_pLabel1Need;
    Label *m_pLabel2Have;
    //Label *m_pLabel2Need;
    Label *m_pLabel3Have;
    //Label *m_pLabel3Need;
    int m_iProp1;
    int m_iProp2;
    int m_iProp3;
    int m_iSkillId;
    int m_iNeedTime;
    skillInfo * m_pSkillInfo;
    Sprite *m_pBg;
    heroSkill* m_pHeroSkillInfo;
    char m_strNeedTime[100];
    Label *m_pSkillName;//技能名字
    //std::map<int,int>  m_PropMap;
    std::vector<FighterInfo*> m_pSoldierVec;
    EventListenerTouchOneByOne * m_pListener;
    cocos2d::Size m_winSize;
    int m_iLv;
    int m_needStoneNum;//需要的魔法石数量
    Node* m_node;
};


#endif /* defined(__TheGodWill__SkillUpLayer__) */
