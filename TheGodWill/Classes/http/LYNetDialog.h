//
//  LYNetDialog.h
//  WarOfGod
//
//  Created by user on 15/3/27.
//
//

#ifndef __WarOfGod__LYNetDialog__
#define __WarOfGod__LYNetDialog__

#include <stdio.h>


#include "BaseLayer.h"


class CNetDialog: public CBaseLayer {
    
public:
    
    CNetDialog();
    ~CNetDialog();
    
    CREATE_FUNC(CNetDialog);
    
    
    virtual bool init();

    virtual bool onTouchBegan(Touch *touch, Event *unused_event);
    
    virtual void onExit();
    
private:
    
    EventListenerTouchOneByOne* m_pListener;
    
};



#endif /* defined(__WarOfGod__LYNetDialog__) */
