//层管理类
#pragma once

#include "cocos2d.h"


class CBaseLayer;
class CBaseScene;
class CLoadingLayer;

enum layerType
{
    eLayer_Login = 1,
    eLayer_Fight,
    eLayer_FightPVP,
    eLayer_MainMenu,
    eLayer_LevelInfo,
    eLayer_Compound,
    eLayer_LvUp,
    eLayer_LvSkill,
    eLayer_PropLayer,
    eLayer_TeamLayer,
    eLayer_SoliderSelectLayer,
    eLayer_TaskLayer,
    eLayer_DailyTaskLayer,
    eLayer_SetLayer,
    eLayer_error,
    eLayer_LoginFirst,
    eLayer_LoginSec,
    eLayer_Charge,
    eLayer_ShopLayer,
    eLayer_PVPLayer,
    eLayer_Loading,
    eLayer_DrawCardLayer
    
};



class CLayerManager
{
    
public:
    
    
    struct layerPlist{
        
        layerType type;
        
        std::vector<std::string> thePlist;
    };
    
    
    static CLayerManager* GetSingletonPtr();
    
    CLayerManager();
    ~CLayerManager();
    
    cocos2d::Node* createLayer(layerType type, cocos2d::Node* parentNode,bool ifChangeScene=false, bool ifAddAutomatic=true,std::vector<std::string>* partameters=NULL);//
    
    cocos2d::Node* createLayerWithLoading(layerType type, cocos2d::Node* parentNode,bool ifChangeScene=false,std::vector<std::string> parameters = {});
    
    
    void addLoadingLayer();
    void removeLoadingLayer();
    
    
    
    CBaseLayer*  getCurrentLayer();
    
    
    int getCurLayerType();
    
    bool findPlist(const char* plistFile);
    void pushPlist(const char* plistFile);
    bool erasePlist(const char* plistFile);
    
    
    std::vector<std::string>* getPlistsByLayerType(layerType type);  //主要用于loading界面的显示加载。如果有需要用laoding界面的，则在plist列表中写入
    
    
    
    //
private:
    
    CBaseLayer* m_pCurrentLayer;
    CBaseScene* m_pCurrentScene;
    
    CLoadingLayer* m_pLoadingLayer;
    
    
    std::map<std::string,int> m_mapPlist;
    
    int m_iLayerType;
    static CLayerManager *t;
    
    std::vector<layerPlist> m_VecAllPlist;
    
};
