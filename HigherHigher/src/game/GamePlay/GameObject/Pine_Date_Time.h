#ifndef _PINE_DATE_TIME_H_
#define _PINE_DATE_TIME_H_

// Class store time pass -> day, hour, minute, second, milisecond
#define DATE_START_COUNT_YEAR		2015
#define DATE_START_COUNT_MONTH		9
#define DATE_START_COUNT_DAY		28
class Pine_Date_Time
{
	int _milisecond;
	BYTE _second;				/* seconds after the minute - [0,59] */
	BYTE _minute;				/* minutes after the hour - [0,59] */
	BYTE _hour;					/* hours since midnight - [0,23] */
	INT32 _day;					/* total day from 28/9/2015 to current*/

	int _wday;					 /* days since Sunday - [0,6] */
	int _mday;					/* day of the month - [1,31] */
	int _yday;					/* days since January 1 - [0,365] */
	int _mon;					/* months since January - [0,11] */
	int _year;					/* years since 1900 */

public:
	Pine_Date_Time(){
		setNullDay();		
	};	

	int GetDayInMonth(){ return _mday; };
	int GetMonth(){ return _mon; };
	int GetYear(){ return _year; };

	Pine_Date_Time(INT32 day, BYTE hour, BYTE minute, BYTE second, int milisecond)
	{
		_day = day;
		_hour = hour;
		_minute = minute;
		_second = second;
		_milisecond = milisecond;
	};
	
	void setDateByDate(Pine_Date_Time value)
	{
		_day = value.getDay();
		_hour = value.getHour();
		_minute = value.getMinute();
		_second = value.getSecond();
		_milisecond = value.getMilisecond();
	};

	void setDate(INT32 day, BYTE hour, BYTE minute, BYTE second, int milisecond = 0)
	{
		_day = day;
		_hour = hour;
		_minute = minute;
		_second = second;
		_milisecond = milisecond;
	};

	void setDateByMinute(INT_TIME _total_minute);
    void setDateBYMilisecond(INT_TIME _milisecond);
	void setDateBYSecond(INT_TIME second);
	void setDateByLocalDate();
    void setDateByRealDate(int year, int month, int day, int hour, int minute, int second);

    void addDate(Pine_Date_Time date);
	//if time < 0 -> return false
    bool minusDate(Pine_Date_Time date);
    void setDateByCountDownDayFromDay(Pine_Date_Time _day);
    void printDate();

	INT32 GetCurrentTotalMiliSec();
	INT32 GetTotalMiliSec();
	INT32 GetTotalSec();
	INT32 GetTotalMin();
	INT32 GetTotalHour();

	int getMilisecond()
	{
		return _milisecond;
	};

	BYTE getSecond()
	{
		return _second;
	};

	BYTE getMinute()
	{
		return _minute;
	};

	BYTE getHour()
	{
		return _hour;
	};

	INT32 getDay()
	{
		return _day;
	};

    void setDay(INT32 day)
    {
        _day = day;
    };
    
	void UpdateTime();
    bool UpdateCountDown();
    
	bool isMinuteShow()
	{
		return (_day == 0 && _hour == 0);
	};

	bool isHourShow()
	{
		return (_day == 0 && _hour > 0);
	};

	bool isDayShow()
	{
		return (_day > 0);
	};
    
    bool isNullDay()
    {
        if (_hour == 255 && _minute == 255 && _second == 255)
            return true;
        return false;               
    };
    
    void setNullDay()
    {
        _day = 0;
        _milisecond = 0;
        _hour = 255;
        _minute = 255;
        _second = 255;

		_wday = 255;
		_mday = 255;
		_yday = 255;
		_mon = 255;
		_year = 255;
    };

	//store use 9 byte -> 4 + 1 + 1 + 1 + 2  
	void SetPineDateTimeAt(char* buffer,int offset);
	void GetPineDateTimeAt(char* buffer,int offset);

	//util
	void GetCurrentTimeLocal();
	
	static const char* GetStringTime(INT_TIME milisecond, bool useDot = false, bool useMili = false);
	static const char* GetStringTimeFormat(INT_TIME milisecond, bool useDot = false);
	static const char* GetStringTimeSecond(INT_TIME milisecond, int mode);
	Pine_Date_Time operator = (Pine_Date_Time value);
	bool operator > (Pine_Date_Time value);
	bool operator < (Pine_Date_Time value);
	bool operator == (Pine_Date_Time value);
	bool operator >= (Pine_Date_Time value);
	bool operator <= (Pine_Date_Time value);
	Pine_Date_Time operator + (Pine_Date_Time value);
	Pine_Date_Time operator += (Pine_Date_Time value);
	Pine_Date_Time operator - (Pine_Date_Time value);
	Pine_Date_Time operator -= (Pine_Date_Time value);
};

#endif