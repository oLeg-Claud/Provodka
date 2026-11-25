//====================================================================================
AnsiString get_StringFromUTime(time_t time)
{
    tm *mas = localtime(&time);//делаем структуру из метки времени
    //собираем строку
    AnsiString str="";
    if(mas->tm_mday < 10) str += "0";
    str += IntToStr(mas->tm_mday)+".";
    if(mas->tm_mon+1 < 10) str += "0";
    str += IntToStr(mas->tm_mon+1)+".";//приводим мес€ц в 1..12
    if(mas->tm_year <10) str += "0";
    str += IntToStr(mas->tm_year+1900-2000)+"_";//приводим год к 2 цифры после 2000
    if(mas->tm_hour < 10) str += "0";
    str += IntToStr(mas->tm_hour)+":";
    if(mas->tm_min < 10) str += "0";
    str += IntToStr(mas->tm_min)+":";
    if(mas->tm_sec < 10) str += "0";
    str += IntToStr(mas->tm_sec);
    return str;
}
//====================================================================================
AnsiString get_DateFromUTime(time_t time)
{
    tm *mas = localtime(&time);//делаем структуру из метки времени
    //собираем строку
    AnsiString str="";
    if(mas->tm_mday < 10) str += "0";
    str += IntToStr(mas->tm_mday)+".";
    if(mas->tm_mon+1 < 10) str += "0";
    str += IntToStr(mas->tm_mon+1)+".";//приводим мес€ц в 1..12
    if(mas->tm_year <10) str += "0";
    str += IntToStr(mas->tm_year+1900-2000);//приводим год к 2 цифры после 2000
    return str;
}
//====================================================================================
AnsiString get_TimeFromUTime(time_t time)
{
    tm *mas = localtime(&time);//делаем структуру из метки времени
    //собираем строку
    AnsiString str="";
    if(mas->tm_hour < 10) str += "0";
    str += IntToStr(mas->tm_hour)+":";
    if(mas->tm_min < 10) str += "0";
    str += IntToStr(mas->tm_min)+":";
    if(mas->tm_sec < 10) str += "0";
    str += IntToStr(mas->tm_sec);
    return str;
}
//====================================================================================
time_t get_UTimeFromString(AnsiString str)
{
int pos;
    time_t tt=time(NULL);
    tm *mas = localtime(&tt);//структура дл€ UTime, дл€ инициализации
    str = Trim(str);//уберем пробелы слева и справа
    //число
    pos=str.Pos(".");
    if(pos!=0) {try{mas->tm_mday = StrToInt(str.SubString(1,pos-1)); str.Delete(1,pos);} catch(...){return -1;}}
    else return -1;
    //мес€ц приводим к 0..11
    pos=AnsiString(str).Pos(".");
    if(pos!=0) {try{mas->tm_mon = StrToInt(str.SubString(1,pos-1))-1; str.Delete(1,pos);} catch(...){return -1;}}
    else return -1;
    //год 2 цифры приводим
    pos=AnsiString(str).Pos("_");
    if(pos!=0) {try{mas->tm_year = StrToInt(str.SubString(1,pos-1))+2000-1900; str.Delete(1,pos);} catch(...){return -1;}}
    else return -1;
    //часы
    pos=AnsiString(str).Pos(":");
    if(pos!=0) {try{mas->tm_hour = StrToInt(str.SubString(1,pos-1)); str.Delete(1,pos);} catch(...){return -1;}}
    else return -1;
    //минуты
    pos=AnsiString(str).Pos(":");
    if(pos!=0) {try{mas->tm_min = StrToInt(str.SubString(1,pos-1)); str.Delete(1,pos);} catch(...){return -1;}}
    else return -1;
    //секунды, последник в строке
    try{mas->tm_sec = StrToInt(str);} catch(...){return -1;}
    //mas->tm_mday=9;
    //mas->tm_mon=11;
    //возвращаем врем€ UTime
    return mktime(mas);
}
//====================================================================================
time_t getUnixTimeSec(void)
{
    return time(NULL);// Unix в секундах
}
//====================================================================================
long long getUnixTimeMs(void)
{
    time_t seconds = time(NULL);// Unix в секундах
    SYSTEMTIME st;
    GetSystemTime(&st);
	return (long long)seconds * 1000LL + st.wMilliseconds;  // Unix в миллисекундах
}
//====================================================================================
uint32_t getTimeShiftGeomash(uint32_t FORMAT, uint32_t freqnum)
{
	if(freqnum == 0)//16√ц
    {
    	if(FORMAT == FORMAT_GEOMASH) 			return 41244;//мс
        else if(FORMAT == FORMAT_GEOMASH_RETR) 	return 42226;//мс
        else return 0;
    }
    else if(freqnum == 1)//10√ц
    {
    	if(FORMAT == FORMAT_GEOMASH) 			return 63624;//мс
        else if(FORMAT == FORMAT_GEOMASH_RETR) 	return 65230;//мс
        else return 0;
    }
    else if(freqnum == 2)//7√ц
    {
    	if(FORMAT == FORMAT_GEOMASH) 			return 89382;//мс
        else if(FORMAT == FORMAT_GEOMASH_RETR) 	return 91606;//мс
        else return 0;
    }
    else if(freqnum == 3)//5√ц
    {
    	if(FORMAT == FORMAT_GEOMASH) 			return 127310;//мс
        else if(FORMAT == FORMAT_GEOMASH_RETR) 	return 130455;//мс
        else return 0;
    }
    else if(freqnum == 4)//3√ц
    {
    	if(FORMAT == FORMAT_GEOMASH) 			return 176107;//мс
        else if(FORMAT == FORMAT_GEOMASH_RETR) 	return 180556;//мс
        else return 0;
    }
    else return 0;
}
//====================================================================================
 