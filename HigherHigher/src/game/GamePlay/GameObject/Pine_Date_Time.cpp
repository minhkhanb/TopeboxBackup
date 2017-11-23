#include "pch.h"
#include "../../GameCore/CGame.h"

void Pine_Date_Time::setDateByMinute(INT_TIME _total_minute)
{
    _milisecond = 0;
    _second = 0;
	_minute = 0;
	_hour = 0;
	_day = 0;

    _minute = (BYTE)(_total_minute % 60);
    _total_minute -= _minute;
    INT_TIME temp = _total_minute / 60;
    _hour = (BYTE)(temp % 24);
    _total_minute -= (_hour * 60);
    _day = temp / 24;
}

void Pine_Date_Time::setDateBYSecond(INT_TIME _total_second)
{
	_milisecond = 0;
    _second = 0;
	_minute = 0;
	_hour = 0;
	_day = 0;

	_second = (_total_second % 60);
	_total_second -= _second;

	if (_total_second <= 0) return;

	INT_TIME temp = _total_second / 60;	//total minute
	_minute = temp % 60;
	_total_second = temp - _minute; // current total minute

	if (_total_second <= 0) return;

	temp = _total_second / 60; //total hour
	_hour = temp % 24;
	_total_second = temp - _hour; // current total hour

	if (_total_second <= 0) return;

	_day = _total_second / 24; //total day
}

void Pine_Date_Time::setDateBYMilisecond(INT_TIME _total_milisecond)
{
    _milisecond = 0;
    _second = 0;
	_minute = 0;
	_hour = 0;
	_day = 0;
    
    _milisecond = (_total_milisecond % 1000);
    _total_milisecond -= _milisecond;
    
    if (_total_milisecond <= 0) return;
    
    INT_TIME temp = _total_milisecond / 1000; //total second
    _second = (temp % 60);
    _total_milisecond = temp - _second;
    
    if (_total_milisecond <= 0) return;
    
    temp = _total_milisecond / 60; //total minute
    _minute = (temp % 60);
    _total_milisecond = temp - _minute;
    
    if (_total_milisecond <= 0) return;
    
    temp = _total_milisecond / 60; //total hour
    _hour = (temp % 60);
    _total_milisecond = temp - _hour;
    
    if (_total_milisecond <= 0) return;
    
    temp = _total_milisecond / 24; //total day
    _day = temp;
}

void Pine_Date_Time::addDate(Pine_Date_Time _time)
{
    _milisecond += _time._milisecond;
    
    if (_milisecond > 1000)
    {
        _second +=1;
        _milisecond -= 1000;
    }
    
    _second += _time._second;
    if (_second > 60)
    {
        _minute +=1;
        _second -= 60;
    }
    
    _minute += _time._minute;
    if (_minute > 60)
    {
        _hour +=1;
        _minute -= 60;
    }
    
    _hour += _time._hour;
    if (_hour > 24)
    {
        _day +=1;
        _hour -= 24;
    }
    
    _day += _time._day;
}

bool Pine_Date_Time::minusDate(Pine_Date_Time date)
{	
    int tmpSecond = _second, tmpMinute = _minute, tmpHour = _hour;
    
    _milisecond -= date._milisecond;
    if (_milisecond < 0)
    {
        _milisecond = 1000 + _milisecond;
        tmpSecond -= 1;
    }
    
    if (tmpSecond < 0)
    {
        tmpSecond = 60 + tmpSecond;
        tmpMinute--;
    }
    
    //printf("\n @@@@@@ minus function ");
    //printf("\n milisecond = %d, second = %d ", _milisecond, _second);
    
    tmpSecond -= date._second;
    if (tmpSecond < 0)
    {
        tmpSecond = 60 + tmpSecond;
        tmpMinute -= 1;
    }
    
    if (tmpMinute < 0)
    {
        tmpMinute = 60 + tmpMinute;
        tmpHour--;
    }
    
    //printf("\n @@@@@@ minus function ");
    //printf("\n second = %d, minute = %d ", _second, _minute);
    
    tmpMinute -= date._minute;
    if (tmpMinute < 0)
    {
        tmpMinute = 60 + tmpMinute;
        tmpHour -= 1;
    }
    
    if (tmpHour < 0)
    {
        tmpHour = 24 + tmpHour;
        _day--;
    }
    
    //printf("\n @@@@@@ minus function ");
    //printf("\n minute = %d, hour = %d ", _minute, _hour);
    
    tmpHour = tmpHour - date._hour;
    if (tmpHour < 0)
    {
        tmpHour = 24 + tmpHour;
        _day -= 1;
    }
    
    if (_day < 0)
    {
        _day--;
    }
    
    //printf("\n @@@@@@ minus function ");
    //printf("\n hour = %d, day = %d ", _hour, _day);
    
    _day -= date._day;
    
    if (_day < 0)
    {
        return false;
    }
    else 
    {
        _second = tmpSecond;
        _minute = tmpMinute;
        _hour = tmpHour;
        return true;
    }
}

void Pine_Date_Time::UpdateTime()
{
    if (DT() < 1000) //optimize
    {
        _milisecond =_milisecond + DT();
        if (_milisecond >= 1000)
        {
            _second += 1;
            _milisecond = _milisecond - 1000;
        }
        
        if (_second == 60)
        {
            _minute += 1;
            _second = 0;
        }
        
        if (_minute == 60)
        {
            _hour += 1;
            _minute = 0;
        }
        
        if (_hour == 24)
        {
            _day += 1;
            _hour = 0;
        }
    }
    else
    {
        Pine_Date_Time temp;
        temp.setDateBYMilisecond(DT());        
        addDate(temp);        
    }
}

void Pine_Date_Time::setDateByCountDownDayFromDay(Pine_Date_Time _day)
{
    _milisecond = 999 - _day._milisecond;
    _second = 59 - _day._second;
    _minute = 59 - _day._minute;
    _hour = 23 - _day._hour;		
}

INT32 Pine_Date_Time::GetCurrentTotalMiliSec()
{
	INT32 milisec = _hour * 3600 * 1000 + _minute * 60 * 1000 + _second * 1000 + _milisecond;
	if (_hour == 255 && _minute == 255 && _second == 255)
	{
		milisec = 0;
	}
	return milisec;

}
INT32 Pine_Date_Time::GetTotalMiliSec()
{
	INT32 milisec = GetTotalSec() * 1000 + _milisecond;
	if (_hour == 255 && _minute == 255 && _second == 255)
	{
		milisec = 0;
	}
	return milisec;
}

INT32 Pine_Date_Time::GetTotalSec()
{ 
	INT32 sec = _day * 24 * 3600 + _hour * 3600 + _minute * 60 + _second;
	if (_hour == 255 && _minute == 255 && _second == 255)
	{
		sec = 0;
	}	
	return sec;
}

INT32 Pine_Date_Time::GetTotalMin()
{
	INT32 min = _day * 24 * 60 + _hour * 60 + _minute;
	if (_hour == 255 && _minute == 255 && _second == 255)
	{
		min = 0;
	}
	return min;
}

INT32 Pine_Date_Time::GetTotalHour()
{
	INT32 hour = _day * 24 + _hour;
	if (_hour == 255 && _minute == 255 && _second == 255)
	{
		hour = 0;
	}
	return hour;
}

//return true is out time
bool Pine_Date_Time::UpdateCountDown()
{
    if (DT() < 1000) //optimize
    {
        bool flag = false;
		//printf("\n _milisecond = %d, DT() = %d", _milisecond, DT());
        _milisecond =_milisecond - DT();
        if (_milisecond < 0)
        {
            flag = true;
            _milisecond = 1000 + _milisecond;
        }
        
        if (flag)
        {
            if (_second == 0)
            {
                flag = true;
                _second = 59;
            }
            else
            {
                flag = false;
                _second -= 1;
            }
        }
        
        if (flag)
        {
            if (_minute == 0)
            {
                flag = true;
                _minute = 59;
            }
            else
            {
                flag = false;
                _minute -= 1;
            }
        }

        if (flag)
        {
            if (_hour == 0)
            {
                flag = true;
                _hour = 23;
            }
            else
            {
                flag = false;
                _hour -= 1;
            }
        }
        
        if (flag)
        {
            _day -= 1;
        }
        
    }
    else
    {
        Pine_Date_Time temp;
        temp.setDateBYMilisecond(DT());
		if (temp > *this)
		{
            setNullDay();
			return true;
		}
        minusDate(temp);
    }

	if (_day < 0)
	{
        setNullDay();
		return true;
	}
	else
	{
		return false;
	}
}

void Pine_Date_Time::SetPineDateTimeAt(char* buffer,int offset)
{
	int data_int = _day&0xFFFFFFFF;
	memcpy(buffer + offset,&data_int,4);

	BYTE data = _hour&0xff;	
	memcpy(buffer + offset + 4,&data,1);

	data = _minute&0xff;	
	memcpy(buffer + offset + 4 + 1,&data,1);

	data = _second&0xff;	
	memcpy(buffer + offset + 4 + 1 + 1,&data,1);

	short data_short = _milisecond&0xFFFF;	
	memcpy(buffer + offset + 4 + 1 + 1 + 1,&data_short,2);
	
}

void Pine_Date_Time::GetPineDateTimeAt(char* buffer,int offset)
{
	_day = 0;
	memcpy(&_day,buffer + offset,4);

	_hour = 0;
	memcpy(&_hour,buffer + offset + 4, 1);	

	_minute = 0;
	memcpy(&_minute,buffer + offset + 4 + 1, 1);

	_second = 0;
	memcpy(&_second,buffer + offset + 4 + 1 + 1, 1);

	_milisecond = 0;
	memcpy(&_milisecond,buffer + offset + 4 + 1 + 1 + 1, 2);
}

void Pine_Date_Time::setDateByLocalDate()
{
	time_t rawtime;
	struct tm * timeinfo;

	time ( &rawtime );
	timeinfo = localtime ( &rawtime );

	_wday = timeinfo->tm_wday;
	_mday = timeinfo->tm_mday;
	_yday = timeinfo->tm_yday;
	_mon = timeinfo->tm_mon+1;
	_year = timeinfo->tm_year + 1900;

	timeinfo->tm_year = DATE_START_COUNT_YEAR - 1900;
	timeinfo->tm_mon = DATE_START_COUNT_MONTH - 1;
	timeinfo->tm_mday = DATE_START_COUNT_DAY;
	timeinfo->tm_hour = 0;
	timeinfo->tm_min = 0;
	timeinfo->tm_sec = 0;

	/* call mktime: timeinfo->tm_wday will be set */
	//rawtime = mktime ( timeinfo ); // date start count down (tm format)
	
	double _diff2 = difftime(time(NULL), mktime ( timeinfo ));
	INT_TIME _diff = _diff2;	
    
	setDateBYSecond( _diff );
}

void Pine_Date_Time::setDateByRealDate(int year, int month, int day, int hour, int minute, int second)
{
    
    time_t rawtime, rawtime2;
	struct tm  timeinfoDefine, timeInput;
    
	time ( &rawtime );
	timeinfoDefine = *localtime ( &rawtime );
	timeinfoDefine.tm_year = DATE_START_COUNT_YEAR - 1900;
	timeinfoDefine.tm_mon = DATE_START_COUNT_MONTH - 1;
	timeinfoDefine.tm_mday = DATE_START_COUNT_DAY;
	timeinfoDefine.tm_hour = 0;
	timeinfoDefine.tm_min = 0;
	timeinfoDefine.tm_sec = 0;
    
    rawtime = mktime(&timeinfoDefine);
    
    time ( &rawtime2 );
    timeInput = *localtime ( &rawtime2 );
	timeInput.tm_year = year - 1900;
	timeInput.tm_mon = month - 1;
	timeInput.tm_mday = day;
	timeInput.tm_hour = hour;
	timeInput.tm_min = minute;
	timeInput.tm_sec = second;
	
    rawtime2 = mktime(&timeInput);
    
	double _diff2 = difftime(rawtime2, rawtime);
	INT_TIME _diff = _diff2;
    
	setDateBYSecond( _diff );
}

void Pine_Date_Time::printDate()
{
	//printf("\n year = %d, mon = %d, yday = %d, mday = %d, wday = %d", _year, _mon, _yday, _mday, _wday);
	printf("\n day = %d, hour = %d, minute = %d, second = %d, milisecond = %d", _day, _hour, _minute, _second, _milisecond);

}

void Pine_Date_Time::GetCurrentTimeLocal()
{
	time_t rawtime;
}
//util
static char _timeString[128];
const char* Pine_Date_Time::GetStringTimeSecond(INT_TIME milisecond, int mode)
{
	
	Pine_Date_Time time;
	time.setDateBYMilisecond(milisecond);	
	sprintf(_timeString, "%d.%02ds", time.getSecond(), time.getMilisecond() / 10);
	return _timeString;
}
const char* Pine_Date_Time::GetStringTime(INT_TIME milisecond, bool useDot, bool useMili)
{		
	Pine_Date_Time time;
	time.setDateBYMilisecond(milisecond);
	if (time.getDay() > 0 )
	{
		if (useDot)
		{
			if (useMili)
			{
				sprintf(_timeString, "%dd %02d:%02d:%02d:%03d", time.getDay(), time.getHour(), time.getMinute(), time.getSecond(), time.getMilisecond());
			}
			else
			{
				sprintf(_timeString, "%dd %02d:%02d", time.getDay(), time.getHour(), time.getMinute());
			}
		}
		else
		{
			sprintf(_timeString, "%dd  %dh", time.getDay(), time.getHour());
		}
	}
	else if (time.getHour()>0)
	{
		if (useDot)
		{
			if (useMili)
			{
				sprintf(_timeString, "%02d:%02d:%02d:%03d", time.getHour(), time.getMinute(), time.getSecond(), time.getMilisecond());
			}
			else
			{
				sprintf(_timeString, "%02d:%02d:%02d", time.getHour(), time.getMinute(), time.getSecond());
			}
		}
		else
		{
			sprintf(_timeString, "%dh  %dm", time.getHour(), time.getMinute());
		}
	}
	else if (time.getMinute()>0)
	{
		if (useDot)
		{
			if (useMili)
			{
				sprintf(_timeString, "%02d:%02d:%03d", time.getMinute(), time.getSecond(), time.getMilisecond());
			}
			else
			{
				sprintf(_timeString, "%02d:%02d", time.getMinute(), time.getSecond());
			}
		}
		else
		{
			sprintf(_timeString, "%dm  %ds", time.getMinute(), time.getSecond());
		}
	}
	else 
	{
		if (useDot)
		{
			if (useMili)
			{
				sprintf(_timeString, "%02d:%02d:%03d", time.getMinute(), time.getSecond(), time.getMilisecond());
			}
			else
			{
				sprintf(_timeString, "%02d:%02d", time.getMinute(), time.getSecond());
			}
		}
		else
		{
			sprintf(_timeString, "%ds", time.getSecond());
		}
	}
	return _timeString;
}

const char* Pine_Date_Time::GetStringTimeFormat(INT_TIME milisecond, bool useDot)
{

	Pine_Date_Time time;
	time.setDateBYMilisecond(milisecond);

	if (useDot)
	{
		sprintf(_timeString, "%02d:%02d:%03d", time.GetTotalHour(), time.getMinute(), time.getSecond());
	}
	else
	{
		sprintf(_timeString, "%dh  %dm  %ds", time.getHour(), time.getMinute(), time.getSecond());
	}
	return _timeString;
}

Pine_Date_Time Pine_Date_Time::operator = (Pine_Date_Time value)
{
	setDateByDate(value);
	return *this;
}

bool Pine_Date_Time::operator > (Pine_Date_Time value)
{
	//check null
	if (isNullDay()) return false;
	else
	{
		if (value.isNullDay()) return true;
		else
		{
			//check day
			if (_day > value.getDay()) return true;
			else if (_day < value.getDay()) return false;
			else
			{
				//check hour
				if (_hour > value.getHour()) return true;
				else if (_hour < value.getHour()) return false;
				else
				{
					//check minute
					if (_minute > value.getMinute()) return true;
					else if (_minute < value.getMinute()) return false;
					else
					{
						//check second
						if (_second > value.getSecond()) return true;
						else if (_second < value.getSecond()) return false;
						else
						{
							//check milisecond
							if (_milisecond > value.getMilisecond()) return true;
							else if (_milisecond < value.getMilisecond()) return false;
							else
							{						
								return false;
							}
						} //end check second
					} //end check minue
				} //end check hour
			} //end check day
		} //end check value null
	}

	
}

bool Pine_Date_Time::operator >= (Pine_Date_Time value)
{
	return (*this > value) || (*this == value);
}

bool Pine_Date_Time::operator < (Pine_Date_Time value)
{
	if (isNullDay()) return true;
	else
	{
		if (value.isNullDay()) return false;
		else
		{
			//check day
			if (_day < value.getDay()) return true;
			else if (_day > value.getDay()) return false;
			else
			{
				//check hour
				if (_hour < value.getHour()) return true;
				else if (_hour > value.getHour()) return false;
				else
				{
					//check minute
					if (_minute < value.getMinute()) return true;
					else if (_minute > value.getMinute()) return false;
					else
					{
						//check second
						if (_second < value.getSecond()) return true;
						else if (_second > value.getSecond()) return false;
						else
						{
							//check milisecond
							if (_milisecond < value.getMilisecond()) return true;
							else if (_milisecond > value.getMilisecond()) return false;
							else
							{						
								return false;
							}
						} //end check second
					} //end check minue
				} //end check hour
			} //end check day
		} //end check value null
	}
}

bool Pine_Date_Time::operator <= (Pine_Date_Time value)
{
	return (*this < value) || (*this == value);
}

bool Pine_Date_Time::operator == (Pine_Date_Time value)
{
	//check null
	if (isNullDay() || value.isNullDay()) return false;	

	//check day
	if (_day != value.getDay()) return false;	
	else
	{
		//check hour
		if (_hour != value.getHour()) return false;
		else
		{
			//check minute
			if (_minute != value.getMinute()) return false;			
			else
			{
				//check second
				if (_second != value.getSecond()) return false;				
				else
				{
					//check milisecond
					if (_milisecond != value.getMilisecond()) return false;					
					else
					{						
						return true;
					}
				} //end check second
			} //end check minue
		} //end check hour 
	} // end check day
}

Pine_Date_Time Pine_Date_Time::operator + (Pine_Date_Time value)
{
	Pine_Date_Time result;	

	if (isNullDay())
	{
		return value;
	}
	else if (value.isNullDay())
	{
		return (*this);
	}
	else
	{
		result.setDate(_day, _hour, _minute, _second, _milisecond);
		result.addDate(value);
				
	}

	return result;
}

Pine_Date_Time Pine_Date_Time::operator += (Pine_Date_Time value)
{
	return (*this) + value;	
}

Pine_Date_Time Pine_Date_Time::operator - (Pine_Date_Time value)
{
	Pine_Date_Time result;

	if (isNullDay())
	{
		return value;
	}
	else if (value.isNullDay())
	{
		return (*this);
	}
	else
	{
		result.setDate(_day, _hour, _minute, _second, _milisecond);
		if (!result.minusDate(value))
			result.setNullDay();		
	}

	return result;
}

Pine_Date_Time Pine_Date_Time::operator -= (Pine_Date_Time value)
{
	return (*this) - value;	
}
