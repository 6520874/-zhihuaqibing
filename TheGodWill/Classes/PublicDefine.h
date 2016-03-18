//
//  PublicDefine.h
//  TheGodWill
//
//  Created by prefer_shan on 14/12/1.
//
//

#ifndef TheGodWill_PublicDefine_h
#define TheGodWill_PublicDefine_h


//此处为正交相机的配置参数
//#define  Start3DPOINT  1500
//#define  END3DPOINT     3000 //2200
//
//#define  BIGMAPLEN    2800     // END3DPOINT - Start3DPOINT + 2*ONESIDE
//#define  ONESIDE      650 //镜头一边的距离
//#define  BIGMAPHIGH   80
//
//#define  STARTZPOINT -150
//#define  ENDZPOINT -650

//#define  CAMERA_POSITION_Y              200
//#define  CAMERA_POSITION_Z             300
//#define  CAMERA_LOOK_POSITION_Y         0
//#define  CAMERA_LOOK_POSITION_Z         100

//#define  CAMERA_TOWER_DEFFENSE_POSITION_Y              200
//#define  CAMERA_TOWER_DEFFENSE_POSITION_Z               300
//#define  CAMERA_LOOK_TOWER_DEFFENSE_POSITION_Z        100


#define  Start3DPOINT  1800
#define  END3DPOINT     2800 //2200

#define  SMALLMAPLEN   298
#define  BIGMAPLEN    2200     // END3DPOINT - Start3DPOINT + 2*ONESIDE
#define  ONESIDE      600 //镜头一边的距离
#define  BIGMAPHIGH   80

#define  STARTZPOINT -250
#define  ENDZPOINT -650

#define  CAMERA_POSITION_Y              338        //透视相机的参数
#define  CAMERA_POSITION_Z             -62
#define  CAMERA_LOOK_POSITION_Y         0
#define  CAMERA_LOOK_POSITION_Z         -400

#define  CAMERA_TOWER_DEFFENSE_POSITION_Y              450
#define  CAMERA_TOWER_DEFFENSE_POSITION_Z               50
#define  CAMERA_LOOK_TOWER_DEFFENSE_POSITION_Z        -400








#define GameDB_Name "/gameDB/gameDB.db"

#define YANFA_TEST

//#define DEBUG_Mode


#define Pro_Release    //发布版本
//#define Pro_DEBUG_COMPANY   //内网测试版本
//#define Pro_DEBUG_CLOUD   //云服测试版本

#define Pro_DEBUG_OutSide //外网测试版本


//#define NO_PAY   //是否无支付功能,暂时ios版需要使用，android版去掉

#ifdef Pro_Release
    #define URL_ADDRESS "http://122.226.100.73:8099/"  //云服地址
    #define TDAppKey "9AA9510081C2FE98057D631A81060EE8"  //外网数据用 talkingdata
#endif


#ifdef Pro_DEBUG_COMPANY 
    #define URL_ADDRESS  "http://192.168.1.49:8080/"    //内网地址
    #define TDAppKey "5792ED38A911A5FBA7829EC430666767"  //测试用 talkingdata
#endif

#ifdef Pro_DEBUG_CLOUD
    #define URL_ADDRESS  "http://122.226.100.73:8099/"    //云服地址
    #define TDAppKey "5792ED38A911A5FBA7829EC430666767"  //测试用 talkingdata
#endif

#ifdef Pro_DEBUG_OutSide
#define URL_ADDRESS  "http://116.228.150.218:8099/"    //外网地址
#define TDAppKey "5792ED38A911A5FBA7829EC430666767"  //测试用 talkingdata
#endif



//#define URL_ADDRESS  "http://192.168.0.99:8080/"  //测试用错地址



#define CLINENT_VERSION "1.1.1"
#define CLIENT_DB_VERSION 111


#define GAME_VERSION   "LYTEST"             //渠道编号  "BAIDU"  "LYTEST" "App Store" "360"



#define MY_BASIC_INFO CPersonalData::getInstance()//个人信息


#endif

