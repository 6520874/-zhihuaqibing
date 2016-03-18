//
//  LYLoadingLayer.h
//  TheGodWill
//
//  Created by user on 15/1/6.
//
//

#ifndef __TheGodWill__LYLoadingLayer__
#define __TheGodWill__LYLoadingLayer__

#include <stdio.h>

#include "BaseLayer.h"


class CLoadingLayer:public CBaseLayer {
    
public:
    
    CLoadingLayer();
    ~CLoadingLayer();
    
    virtual bool init();
    
    virtual void onExit();
    
    virtual void getPngVec(); //获取需要加载的png
    
    void setTheChanging(layerType theType,cocos2d::Node* parentNode,bool ifChangeScene,std::vector<std::string> parameters);
    
    
    
    CREATE_FUNC(CLoadingLayer);
    
    virtual void onEnterTransitionDidFinish();
    
    void update(float time);
    
    void loadPlistCallBack(cocos2d::Texture2D *texture);   //异步加载plist
    void loadPngCallBack(cocos2d::Texture2D *texture);    //异步加载png
    void asyncLoad_Callback(cocos2d::Sprite3D* sprite, void* param);//异步加载模型，防止卡顿
    
    bool ifOverAndChange();
    
    
    virtual void changeLayer(float time);
    virtual void changeLayer2(cocos2d::Ref* pSender);
    
    
protected:
    
    layerType m_itheType;
    cocos2d::Node* m_pParentNode;
    bool m_bIfChangeScene;
    std::vector<std::string> m_vParameters;
    
    LayerColor* m_pAnimateLayer;//过渡动画层
    
    cocos2d::Sprite* m_pRider;//过渡的骑兵
    cocos2d::Sprite* m_pGround;//过渡的草地
    
    float m_fOriginPosx;//原始骑兵的x轴坐标
    
    float m_fMaxOffsetX;//骑兵需要移动的最大x轴位移
    float m_fNowOffsetX;//骑兵目前移动的当前x轴位移
    
    Label* m_pPercentLabel ;
    
    bool m_bIfLoadOver;
    
    
    /*
     所需过渡场景的数据
     */
    std::vector<std::string>* targetVec;
    long m_iMaxPlistNum;
    long m_iNowPlistNum;
    
    std::vector<std::string> m_vecPngs;
    long m_iMaxPngNum;
    long m_iNowPngNum;
    
    std::vector<std::string> m_vecModels;
    long m_iMaxModelNum;
    long m_iNowModelNum;
    
};




#endif /* defined(__TheGodWill__LYLoadingLayer__) */
