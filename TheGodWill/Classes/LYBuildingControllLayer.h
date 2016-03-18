//
//  LYBuildingControllLayer.h
//  TheGodWill
//
//  Created by user on 15/2/5.
//
//

#ifndef __TheGodWill__LYBuildingControllLayer__
#define __TheGodWill__LYBuildingControllLayer__

#include <stdio.h>

#include "cocos2d.h"

#define CHILD_TAG_BUTTON_ICON 99
#define CHILD_TAG_COST_MONEY 100
#define CHILD_TAG_SELECT 101



class CBaseFighter;
class LYMenuItemSprite;

class CBuildingControllLayer : public cocos2d::Sprite{
    
    
public:
    
    CREATE_FUNC(CBuildingControllLayer);
    
    virtual bool init();
    
    
    void setTheBuildingInfo(CBaseFighter* target);
    
    
    void onBtnUpgrade(cocos2d::Ref* pSender);
    void onBtnSale(cocos2d::Ref* pSender);
    
    
    void TheBtnDetail(cocos2d::Ref *pSender);//按钮第一次按下时提示的信息，参数大于0则传递的时怪物id，等于0则代表着卖出
    
    void showDetail(int monsterId,LYMenuItemSprite* theButton);
    
    void buttonRefresh();//刷新所有按钮的显示
    
protected:
    
    cocos2d::Menu* m_pMenu;
    
    std::vector<LYMenuItemSprite*>   m_vecButtonUpgrade;
    
    LYMenuItemSprite* m_pButtonSale;
    
    Sprite* m_pCircleLayer;
    
    CBaseFighter* m_pTarget;
    
    int m_iAllChoices;//所有的升级选项数量
    
    
    
    //详细描述相关
    cocos2d::Sprite* m_pDetailBG;
    int m_iDetailMonsterId;//显示详细的怪物id
    
};


#endif /* defined(__TheGodWill__LYBuildingControllLayer__) */
