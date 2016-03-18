//
//  Chest.h
//  WarOfGod
// 宝箱类
//  Created by prefer_shan on 15/5/5.
//
//

#ifndef __WarOfGod__Chest__
#define __WarOfGod__Chest__

#include <stdio.h>
#include "BaseLayer.h"
#include "CSharedPlist.h"

class ChestLayer:public CBaseLayer
{
public:
    
    CREATE_FUNC(ChestLayer);
    bool init();
    
private:
    ChestLayer();
    ~ChestLayer();
    void UpdateMoney(Ref *pSender);
    
    
    std::string getShopIcoBg(int iPropLevel);
     void OnCloseMenu(Ref *pSender);
    void OnWupin(Ref *pSender);
    void OnChest(Ref *pSender);
    void  OnGreenTouch(Ref *pSender);
    void OnLeftItem(Ref *pSender);
    void OnRightItem(Ref *pSender);
    
    bool onTouchBegan(Touch *pTouch, Event *pEvent);
    void onTouchMoved(Touch *pTouch, Event *pEvent);
    void onTouchEnded(Touch *pTouch, Event *pEvent);
    void onTouchCancelled(Touch *touch, Event *pEvent);
    EventListenerTouchOneByOne * m_pListener;
    std::vector<Sprite*> m_pSpriteVec;
    Label * m_pCoinLabel;
    Label * m_pStoneLabel;
    int m_index;
    
};
#endif /* defined(__WarOfGod__Chest__) */
