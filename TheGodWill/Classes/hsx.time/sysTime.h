#ifndef _GAME_TIME_H_SYST
#define _GAME_TIME_H_SYST


#include "cocos2d.h"
#include <time.h>

USING_NS_CC;

#define MIN_SECS 60
#define HOUR_SECS (MIN_SECS * 60)
#define DAY_SECS (HOUR_SECS * 24)

class CGameTime
{
public:
	typedef struct
	{
		int year;
		int month;
		int day;
		int hour;
		int minute;
		int sec;
	}my_time_t;
    
    
	static int isLeapYear(int year);
	static std::string convertTimeFormatToString(time_t t);
	static my_time_t convertTimeToMyTimeformat(time_t t);
	static my_time_t convertDateStringToFormat(const char *string_date);
	static int CalcDiffDaysBetweenTwoTime(const my_time_t& a,const my_time_t& b);
	static int CalcDiffDaysBetweenTwoFromatString(const char *string_begin, const char *string_end);
	static int calcDiffDaysBetweenTwoYear(int year_begin, int year_end) ;
	static int CalcWeekDay(char *string_date);
	static int isFormatValid(const my_time_t& d1);
	static my_time_t CalcNowtoLaterFormat(const my_time_t& now, int gap);
	static my_time_t CalcNowtoFronterFormat(const my_time_t& now, int gap);
	static time_t CalcTotalSecsByStringFormat(const char* timestr);
    static time_t getSecs(my_time_t t);
    static time_t getDirectConversionSecs(my_time_t t);//直接转换为秒
	static int CalcDaysOfOneYearToNow(const CGameTime::my_time_t& now);
	static int CalcYearSecs(int year);
	static int CalcMonthSecs(int year,int month);
	static std::string GenerateTimeDiffStr(const std::string& lastTime);
	static time_t getNowTimeInSecond();
	static time_t getNowTimeInMillionSec();
    //判断今天是否超过了12点
    static bool getBoolThe12point();
    
};

#endif 
