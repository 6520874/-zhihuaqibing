#include "systime.h"
//#include "common.zzbheadfileinclude.h"//大部分类的头文件都包含在这里面
//#include "TheFirstPunchSpree.h"
//正常的月份,如果是闰年,第二个月会加1
const static int month[12]={31,28,31,30,31,30,31,31,30,31,30,31}; 

/******************************************************************
//funcName:calcDiffDaysBetweenTwoYear	

//date:2013/4/1 12:46

//desc:给定的两个年份之间的天数差值  

//param:

//ret value:天数差值

//author:bluesheet
******************************************************************/
int CGameTime::calcDiffDaysBetweenTwoYear(int year_begin, int year_end) 
{ 
	int sum = 0; 
	int number = year_begin; 

	while(number != year_end) 
	{ 
		sum = sum + 365 + isLeapYear(year_begin); 
		number++; 
	} 

	return sum; 
} 
/******************************************************************
//funcName:CalcDaysOfOneYearToNow	

//date:2013/4/1 12:47

//desc:一年开始到现在的天数  

//param:

//ret value:

//author:bluesheet
******************************************************************/
int CGameTime::CalcDaysOfOneYearToNow(const CGameTime::my_time_t& now) 
{ 
	int i, day = 0; 

	//比如now.month是11月,那么只需要计算-1-10月份的日期即可,之后的日期加上now.day即可
	for (i = 0; i < now.month-1; ++i) 
	{ 
		day = day + month[i]; 
	} 
	return (now.month == 2) ? day + now.day + isLeapYear(now.year) : day + now.day; 
} 
/******************************************************************
//funcName:CalcDiffDaysBetweenTwoTime	

//date:2013/4/1 12:49

//desc:计算给定的两个时间之间相差的天数  
		begin <= end
//param:

//ret value:

//author:bluesheet
******************************************************************/
int CGameTime::CalcDiffDaysBetweenTwoTime(const CGameTime::my_time_t& begin,const CGameTime::my_time_t& end) 
{ 
	int diffyeardays = calcDiffDaysBetweenTwoYear(begin.year, end.year);
	int diffbeginDays = CalcDaysOfOneYearToNow(begin);
	int diffenddays = CalcDaysOfOneYearToNow(end);
	return diffyeardays - diffbeginDays + diffenddays; 
} 
/******************************************************************
//funcName:isLeapYear	

//date:2013/4/1 12:51

//desc:是否是闰年  

//param:

//ret value:

//author:bluesheet
******************************************************************/
int CGameTime::isLeapYear(int year) 
{ 
	return  ((year % 400 == 0) || ((year % 4 == 0)&& (year %100 != 0))) ? 1 : 0; 
} 
/******************************************************************
//funcName:CalcYearSecs	

//date:2013/4/1 13:34

//desc:计算这一年的总秒数  

//param:

//ret value:

//author:bluesheet
******************************************************************/
int CGameTime::CalcYearSecs(int year)
{
	int totalsecs = 0;
	for(int i = 0; i < 12; i++){
		totalsecs += CalcMonthSecs(year,i);
	}
	return totalsecs;
}
/******************************************************************
//funcName:CalcMonthSecs	

//date:2013/4/1 13:34

//desc:计算某年的某月的总秒数  

//param:
		monthindex:0-11
//ret value:

//author:bluesheet
******************************************************************/
int CGameTime::CalcMonthSecs(int year,int monthindex)
{
	int monthdays = month[monthindex];
	if(monthindex == 1){
		if(isLeapYear(year)){
			monthdays += 1; 
		}
	}
	return monthdays * DAY_SECS;
}
/******************************************************************
//funcName:convertDateStringToFormat	

//date:2013/4/1 12:52

//desc:时间字符串格式转换成结构

	格式为 2012-12-23 12:33:53

//param:

//ret value:

//author:bluesheet
******************************************************************/
CGameTime::my_time_t CGameTime::convertDateStringToFormat(const char *string_date) 
{ 
	my_time_t myformat;
	char* pStr = (char*)string_date;
	myformat.year = atoi(pStr);
	while(*pStr++!='-');
	myformat.month = atoi(pStr);
	while(*pStr++!='-');
	myformat.day = atoi(pStr);
	while(*pStr++!=' ');
	myformat.hour = atoi(pStr);
	while(*pStr++!=':');
	myformat.minute = atoi(pStr);
	while(*pStr++!=':');
	myformat.sec = atoi(pStr);

	return myformat; 
} 
/******************************************************************
//funcName:CalcDiffDaysBetweenTwoFromatString	

//date:2013/4/1 12:55

//desc:根据指定的两个字符串格式,计算出两个时间之间相差的天数  

//param:

//ret value:

//author:bluesheet
******************************************************************/
int CGameTime::CalcDiffDaysBetweenTwoFromatString(const char *string_begin, const char *string_end)
{ 
	CGameTime::my_time_t d1,d2; 
	int    sum = 0; 
	//for test
	//std::string nowstr = convertTimeFormatToString(time(NULL));

	d1=convertDateStringToFormat(string_begin); 
	d2=convertDateStringToFormat(string_end); 
	if(isFormatValid(d1)&&isFormatValid(d2)) 
	{ 
		if(d1.year<=d2.year) 
		{ 
			sum=CalcDiffDaysBetweenTwoTime(d1,d2); 
		} 
		else 
		{ 
			sum=CalcDiffDaysBetweenTwoTime(d2,d1); 
		} 
	} 

	return sum; 
} 
/******************************************************************
//funcName:CalcWeekDay	

//date:2013/4/1 12:57

//desc:计算出这个时间,是周几  

//param:

//ret value:1--7----传出的星期数 

//author:bluesheet
******************************************************************/
int CGameTime::CalcWeekDay(char *string_date) 
{ 
	CGameTime::my_time_t d1; 
	int     century = 0; 
	int year = 0; 
	int weekday = 0; 
	int month = 0; 
	char ch; 

	d1=convertDateStringToFormat(string_date); 
	if (isFormatValid(d1)) 
	{ 
		if (d1.month < 3) 
		{ 
			month = d1.month + 12; 
			d1.year--; 
		} 
		else 
			month = d1.month; 

		century = d1.year/100; 
		year = d1.year % 100; 

		weekday = year + (year/4)+(century/4)-(2*century)+(26*(month + 1)/10) + d1.day - 1; 
		weekday = (weekday < 0) ? weekday + 7: weekday; 
	} 

	return (weekday % 7 == 0) ? 7 : weekday % 7; 
} 
/******************************************************************
//funcName:CalcNowtoLaterFormat	

//date:2013/4/1 12:58

//desc:计算出给定日期之后的gap天的结构  

//param:

//ret value:

//author:bluesheet
******************************************************************/
CGameTime::my_time_t CGameTime::CalcNowtoLaterFormat(const CGameTime::my_time_t& now, int gap) 
{ 
	CGameTime::my_time_t d1; 
	int sum, sum_bak; 
	int temp; 

	d1 = now; 
	sum = gap; 

	if(isFormatValid(d1)&&(sum>0) && (sum < 3649270)) 
	{ 
		while(sum>365) 
		{ 
			if(d1.month>=3) 
			{ 
				d1.year++; 
				sum=sum-365-isLeapYear(d1.year); 
			} 
			else 
			{ 
				sum=sum-365-isLeapYear(d1.year); 
				d1.year++; 
			} 
		} 
		while(sum > 0) 
		{ 
			if (d1.month != 2) 
			{ 
				temp = month[d1.month - 1] -d1.day + 1; 
			} 
			else 
			{ 
				temp = month[d1.month - 1] +isLeapYear(d1.year)- d1.day + 1; 
			} 
			sum_bak = sum; 
			sum = sum - temp; 
			if (sum   >= 0) 
			{ 
				d1.month++; 
				d1.day = 1; 
				if (d1.month > 12) 
				{ 
					d1.month = 1; 
					d1.year++; 
				} 
				sum_bak = sum; 
			} 
		} 
		d1.day += sum_bak; 
	} 

	return d1; 
} 
/******************************************************************
//funcName:CalcNowtoFronterFormat	

//date:2013/4/1 12:59

//desc:计算出给定日期之前gap天的日期结构  

//param:

//ret value:

//author:bluesheet
******************************************************************/
CGameTime::my_time_t CGameTime::CalcNowtoFronterFormat(const CGameTime::my_time_t& now, int gap) 
{ 
	CGameTime::my_time_t d1; 
	int sum, sum_bak; 
	int temp; 

	d1 = now; 
	sum = gap; 

	if(isFormatValid(d1)&&(sum < 0) && (sum > -3649270)) 
	{ 
		while(sum < -365) 
		{ 
			if(d1.month>=3) 
			{ 
				sum=sum+365+isLeapYear(d1.year); 
				d1.year--; 
			} 
			else 
			{ 
				d1.year--; 
				sum=sum + 365 + isLeapYear(d1.year); 
			} 
		} 

		sum_bak = sum; 

		while(sum < 0) 
		{ 
			temp =-d1.day; 
			sum = sum - temp; 
			if (sum     <= 0) 
			{ 
				d1.month--; 
				if (d1.month < 1) 
				{ 
					d1.month = 12; 
					d1.year--; 
				} 
				if (d1.month == 2) 
					d1.day     = month[d1.month - 1] + isLeapYear(d1.year); 
				else 
					d1.day = month[d1.month - 1]; 

				sum_bak = -sum; 
			} 
		} 
		if (sum_bak < 0) 
		{ 
			d1.day = d1.day + sum_bak; 
		}else 
		{ 

			if (d1.month == 2) 
				d1.day = month[d1.month - 1] + isLeapYear(d1.year) - sum_bak; 
			else 
				d1.day = month[d1.month - 1] - sum_bak; 
		} 
	} 

	return d1; 
} 
/******************************************************************
//funcName:isFormatValid	

//date:2013/4/1 13:00

//desc:  判断输入日期是否合法

//param:

//ret value:

//author:bluesheet
******************************************************************/
int CGameTime::isFormatValid(const CGameTime::my_time_t& d1) 
{ 
	return     ((d1.year > 0 && d1.year <= 9999) && 
		(d1.month > 0 && d1.month <= 12) && 
		(d1.day > 0 &&     ( 
		((d1.month == 2) && (d1.day <= month[d1.month - 1] + isLeapYear(d1.year)))|| 
		((d1.month != 2) && (d1.day <= month[d1.month - 1])) 
		))) ? 1 : 0; 
} 
/******************************************************************
//funcName:convertTimeToStringFormat	

//date:2013/4/1 13:06

//desc:将给定的时间(秒数)转换成字符串格式  

//param:
		t:比如通过localtime获取(time_t结构)

//ret value:

//author:bluesheet
******************************************************************/
std::string CGameTime::convertTimeFormatToString(time_t t)
{
	my_time_t format = 	convertTimeToMyTimeformat(t);
	char buf[128] = {0};
	sprintf(buf,"%04d-%02d-%02d %02d:%02d:%02d",
		format.year,format.month,format.day,format.hour,format.minute,format.sec);
	return buf;
}
/******************************************************************
//funcName:convertTimeToMyTimeformat	

//date:2013/4/1 13:11

//desc:将给定的时间转换成自己的格式

//param:
	t:比如通过time()获取(time_t结构)
	月份:my_time_t的结构的月份要+1
//ret value:

//author:bluesheet
******************************************************************/
CGameTime::my_time_t CGameTime::convertTimeToMyTimeformat(time_t t)
{
	tm *nowTM = localtime(&t);
	my_time_t format;
	format.year = nowTM->tm_year + 1900;
	format.month = nowTM->tm_mon + 1;
	format.day = nowTM->tm_mday;
	format.hour = nowTM->tm_hour;
	format.minute = nowTM->tm_min;
	format.sec = nowTM->tm_sec;
	return format;
}
/******************************************************************
//funcName:	

//CGameTime::my_time_t:2013/3/24 0:31

//desc: 
	根据传入的时间字符串,计算出一个1900年到现在的秒数

//param:
		lastTime: 2012-12-03 21:33:34
		转化成time_t:月份要-1

//ret value:

//author:bluesheet
******************************************************************/
time_t CGameTime::CalcTotalSecsByStringFormat(const char* lastTime)
{
	char* pStr = (char*)lastTime;
	int year = atoi(pStr);
	while(*pStr++!='-');
	int month = atoi(pStr);
	while(*pStr++!='-');
	int day = atoi(pStr);
	while(*pStr++!=' ');
	int hour = atoi(pStr);
	while(*pStr++!=':');
	int minute = atoi(pStr);
	while(*pStr++!=':');
	int sec = atoi(pStr);
	tm temptm;
	memset(&temptm,0,sizeof(tm));
	temptm.tm_year = year - 1900;
	temptm.tm_mon = month - 1;
	temptm.tm_mday = day;
	temptm.tm_hour = hour;
	temptm.tm_min = minute;
	temptm.tm_sec = sec;
	time_t secs = mktime(&temptm);
	return secs;
}
/////到1900有多少秒
time_t CGameTime::getSecs(my_time_t t)
{
    tm temptm;
	memset(&temptm,0,sizeof(tm));
	temptm.tm_year = t.year - 1900;
	temptm.tm_mon = t.month - 1;
	temptm.tm_mday = t.day;
	temptm.tm_hour = t.hour;
	temptm.tm_min = t.minute;
	temptm.tm_sec = t.sec;
	time_t secs = mktime(&temptm);
	return secs;

}
time_t CGameTime::getDirectConversionSecs(my_time_t t)
{
    return t.hour*HOUR_SECS+t.minute*MIN_SECS+t.sec;
}
/******************************************************************
//funcName:lastTime	

//CGameTime::my_time_t:2013/3/23 23:48

//desc:根据传入的时间格式,得到传入的时间和当前的时间差  
	   主要是根据天数来确定

//param:
		lastTime: 2012-12-03 21:33:34

//ret value:

//author:bluesheet
******************************************************************/
std::string CGameTime::GenerateTimeDiffStr(const std::string& lastTime)
{
    CCString result;
	my_time_t lastformat = convertDateStringToFormat(lastTime.c_str());
	std::string nowstr = convertTimeFormatToString(time(NULL));
	my_time_t nowformat = convertDateStringToFormat(nowstr.c_str());
	int diffdays = CalcDiffDaysBetweenTwoTime(lastformat,nowformat);
	int diff = 0;
	if(diffdays > 0){
		diff = diffdays /365;
		if(diff > 0){
			//result.initWithFormat(CResLuaInfoMgr::sharedResLuaInfoMgr()->m_vecMessageLayer[41].promptInfo.getCString(),diff);
		}else{
			diff = diffdays / 30;
			if(diff > 0){
				//result.initWithFormat(CResLuaInfoMgr::sharedResLuaInfoMgr()->m_vecMessageLayer[42].promptInfo.getCString(),diff);
			}else{
				//result.initWithFormat(CResLuaInfoMgr::sharedResLuaInfoMgr()->m_vecMessageLayer[43].promptInfo.getCString(),diffdays);
			}
		}
	}else{
		int lasttimetotalsecs = CalcTotalSecsByStringFormat(lastTime.c_str());
		std::string nowstr = convertTimeFormatToString(time(NULL));
		int nowtimetotalsecs = CalcTotalSecsByStringFormat(nowstr.c_str());
		int diffsecs = nowtimetotalsecs - lasttimetotalsecs;
		if(diffsecs <= 0){
			//result.initWithFormat(CResLuaInfoMgr::sharedResLuaInfoMgr()->m_vecMessageLayer[44].promptInfo.getCString());
		}else{
			diff = diffsecs / HOUR_SECS;
			if(diff > 0){
				//result.initWithFormat(CResLuaInfoMgr::sharedResLuaInfoMgr()->m_vecMessageLayer[45].promptInfo.getCString(),diff);
			}else{
				diff = diffsecs / MIN_SECS;
				if(diff > 0){
				//	result.initWithFormat(CResLuaInfoMgr::sharedResLuaInfoMgr()->m_vecMessageLayer[46].promptInfo.getCString(),diff);
				}else{
					//result.initWithFormat(CResLuaInfoMgr::sharedResLuaInfoMgr()->m_vecMessageLayer[44].promptInfo.getCString());
				}
			}
		}
	}
	return result.getCString();
    /*
	CCString result;
	my_time_t lastformat = convertDateStringToFormat(lastTime.c_str());
	//std::string nowstr = CSystemTime::sharedSystemTime()->getMySysTime();
    //convertTimeFormatToString(time(NULL));
	//my_time_t nowformat = convertDateStringToFormat(nowstr.c_str());
	//int diffdays = CalcDiffDaysBetweenTwoTime(lastformat,nowformat);
//	int diff = 0;
//	if(diffdays > 0){
//		diff = diffdays /365;
//		if(diff > 0){
//			result.initWithFormat("%d年前",diff);
//		}else{
//			diff = diffdays / 30;
//			if(diff > 0){
//				result.initWithFormat("%d月前",diff);
//			}else{
//				result.initWithFormat("%d天前",diffdays);
//			}
//		}
//	}else{
//		int lasttimetotalsecs = CalcTotalSecsByStringFormat(lastTime.c_str());
//		std::string nowstr = CSystemTime::sharedSystemTime()->getMySysTime();
//		int nowtimetotalsecs = CalcTotalSecsByStringFormat(nowstr.c_str());
//		int diffsecs = nowtimetotalsecs - lasttimetotalsecs;
//		if(diffsecs < 0){
//			result.initWithFormat("刚才");
//		}else{
//			diff = diffsecs / HOUR_SECS;
//			if(diff > 0){
//				result.initWithFormat("%d小时前",diff);
//			}else{
//				diff = diffsecs / MIN_SECS;
//				if(diff > 0){
//					result.initWithFormat("%d分钟前",diff);
//				}else{
//					result.initWithFormat("刚才");
//				} 
//			}
//		}		
//	}
//	return result.getCString();
    return "";
     */
}
time_t CGameTime::getNowTimeInSecond()
{
	time_t t;
	time(&t);
	return t;
}

time_t CGameTime::getNowTimeInMillionSec()
{
	time_t t = getNowTimeInSecond();
	return t * 1000;
}


//判断今天是否超过了12点
bool CGameTime::getBoolThe12point()
{
    struct timeval nowTimeval;
    gettimeofday(&nowTimeval, NULL);
    
    struct tm * tm;
    time_t time_sec ;
    time_sec = nowTimeval.tv_sec;
    tm = localtime(&time_sec);
    int inMinute = tm->tm_min;
    int inSecond = tm->tm_sec;
    int inHour = tm->tm_hour;
    int iDay = tm->tm_mday;
    int iMonth = tm->tm_mon+1;
    int iYear = tm->tm_year+1900;
    
    my_time_t lastformat;
    lastformat.year = iYear;
    lastformat.month = iMonth;
    lastformat.minute = inMinute;
    lastformat.day  =iDay;
    lastformat.sec = inSecond;
    lastformat.hour = inHour;
    
    //CSystemTime::sharedSystemTime()->getMySysTime();
    if (lastformat.hour<12)
    {
        return true;
    }
    return false;
}
