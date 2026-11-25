
#include <vcl.h>
#pragma hdrstop

#include "PBThread.h"
#include "TRAEKT2k.h"
#include "my_typedef.h"
#pragma package(smart_init)
extern HANDLE initPortPB(AnsiString NUM);
extern CRITICAL_SECTION csQpb;
extern void SaveStatusLog(AnsiString str, TColor color);
extern std::queue <qStruc> Qpb;
extern bool BusyPB, PotokPBON;
extern CRITICAL_SECTION cs;
//extern bool FPBresponse;//флаг готовности пакета
AnsiString gString="";
TColor gColor=clFuchsia;
int timeout, ms_big=100, ms_little=2, dlina_resp, error_portPB = 0;
uint8_t buf[512];
#define SHOWLOG	0
//=============================================================================

__fastcall PBThread::PBThread(bool CreateSuspended)
    : TThread(CreateSuspended)
{
}
//=============================================================================
/*void __fastcall PBThread::Execute()
{
extern std::queue <unsigned char> Qpb;
extern bool BusyPB, PotokPBON;
extern AnsiString NCOMPBh;
HANDLE portPB;
unsigned long kolb, mask;
unsigned char buf[260];
int a,i;

PotokPBON=1;
FreeOnTerminate=true;
while(BusyPB) {Application->ProcessMessages();};//ждем паузу между передачами
portPB=initPortPB(NCOMPBh);//включаем порт
if(portPB==NULL)//выходим из потока, если порт неудачный
 {Synchronize(txt); while(!Qpb.empty()) Qpb.pop(); goto exit;}
while(!Qpb.empty())
{
    a=Qpb.front(); //Кол-во параметров, включая команду и конец
    //выходим из потока, если в очереди меньше, чем надо
    if(Qpb.size()<(a*4)) {Synchronize(qtxt); while(!Qpb.empty()) Qpb.pop(); goto exit;}
    //if(Qpb.size()<(a*4)) {continue;}//набираем в очереди полную строку байтов
    Qpb.pop();//удаляем вытащенный элемент из очереди
    for(i=0;i<a;i++)
    {
     buf[0+i*4]=Qpb.front(); Qpb.pop();//адрес
     buf[1+i*4]=Qpb.front(); Qpb.pop();//старшая часть числа
     buf[2+i*4]=Qpb.front(); Qpb.pop();//младшая часть числа
     buf[3+i*4]=Qpb.front(); Qpb.pop();//КС
    }
    while(BusyPB) {Application->ProcessMessages();};//ждем паузу между передачами
    Synchronize(led);//зажжем индикатор
    WriteFile(portPB,buf,a*4,&kolb,NULL);
    WaitCommEvent(portPB,&mask,NULL);//Ждем, если передача не закончена
    if(mask==EV_TXEMPTY)//передан последний символ
     PurgeComm(portPB,PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);//очистить все
    Synchronize(pause);//пауза между передачами
}
CloseHandle(portPB);//выключаем порт
exit:
PotokPBON=0;
}*/
//=============================================================================
void __fastcall PBThread::Execute()
{
extern AnsiString NCOMPBh;
HANDLE portPB=NULL;
unsigned long kolb, mask;
int i, len=0;
qStruc a;
bool myPult;

PotokPBON=1; Application->ProcessMessages();//сообщаем всем, что поток включен
FreeOnTerminate=true;

while(BusyPB) {Application->ProcessMessages();}//ждем паузу между передачами, или если занят внешним опросом
portPB = initPortPB(NCOMPBh);//включаем порт
if(portPB==NULL)//выходим из потока, если порт неудачный
{	gString="не смог открыть порт ПБ = "+NCOMPBh; gColor=clFuchsia; Synchronize(txt);
 	dlina_resp = 0;
 	Synchronize(send_response);
 	error_portPB = 1;//ошибка порта ПБ
 	//------------------------------------
 	EnterCriticalSection( &cs );
 	while(!Qpb.empty()) Qpb.pop();
 	LeaveCriticalSection( &cs );
 	//------------------------------------
 	timeout = ms_little;
 	Synchronize(pause);
 	while(BusyPB) {Application->ProcessMessages();}//ждем паузу между передачами
 	//Fr45->Diod->Visible=false;//потушим индикатор, каким бы он ни был
 	goto exit;
}

error_portPB = 0;//чистим ошибку порта
//выбираем тип пульта
if(Fr45->PBType->ItemIndex == 0) myPult = true;
else myPult = false;
if(SHOWLOG) {if(portPB) gString="Открыли порт "+NCOMPBh; gColor=clWhite; Synchronize(txt);}
if(portPB) PurgeComm(portPB,PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);//очистить все

//основной цикл
while(!Qpb.empty() && portPB)
{
    //ищем элемент с длиной массива, включая команду и конец
    a=Qpb.front(); Qpb.pop();
    if(a.mas[0]==255 && a.mas[2]==255 && a.mas[1]==a.mas[3]) {len=a.mas[1];}//поймали длину, это первый эл-т
    else {continue;}//пропускаем цикл
    while(Qpb.size()<(len)) {Application->ProcessMessages();}//набираем в очереди полную длину элементов
    //теперь в очереди есть кол-во эл-тов необходимой длины
    if(myPult)//если наш ПБ
    {   for(i=0;i<len;i++)
        {   a=Qpb.front();
            buf[0+i*4]=a.mas[0]; //адрес
            buf[1+i*4]=a.mas[1]; //старшая часть числа
            buf[2+i*4]=a.mas[2]; //младшая часть числа
            buf[3+i*4]=a.mas[3]; //КС
            EnterCriticalSection( &cs );
            Qpb.pop();
            LeaveCriticalSection( &cs );
        }
        len = len*4;//приводим к байтам
    }
    else if(!myPult)//если не наш ПБ
    {   for(i=0;i<len;i++)
        {   a=Qpb.front();
            buf[i]=a.mas[3];//нужный байт сидит последним
            EnterCriticalSection( &cs );
            Qpb.pop();
            LeaveCriticalSection( &cs );
        }
    }
    else len = 0;

    Synchronize(ledON);//зажжем индикатор
    PurgeComm(portPB,PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);//очистить все
    if(SHOWLOG) {gString="посылаю пакет из очереди:"; for(int n=0;n<len;n++) gString+=" "+IntToHex(buf[n], 2); gColor=clWhite; Synchronize(txt);}
    WriteFile(portPB,buf,len,&kolb,NULL);//передаем пакетом
    //передан последний символ
    //чистить нельзя, если адаптер TCP
    //if(mask==EV_TXEMPTY) PurgeComm(portPB,PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);
    //gString="закончена передача пакета из потока"; gColor=clWhite; Synchronize(txt);
    //---------------------------------------------------------------------------------

    //gString="ждем ответа от ПБ"; gColor=clWhite; Synchronize(txt);
    if(myPult)//если наш ПБ
    {   kolb=4;
        while(kolb==4)
        {
        	ReadFile(portPB,buf,4,&kolb,NULL);//читаем 4ками, таймаут дает 0
            //AnsiString str="Принята пачка:"; for(int n=0;n<kolb;n++) str+=" "+IntToHex(buf[n], 2); gString=str; gColor=clWhite; Synchronize(txt);
            if(buf[3]==0xFD) break;
        }
    }
    else if(!myPult)//если не наш ПБ
    {   //с этого ПБ по-любому придет 6 байт
        i = 6;
        dlina_resp = 0;
        ReadFile(portPB,buf,i,&kolb,NULL);//читаем, таймаут дает 0
        //ответ принят
        if(i == kolb) dlina_resp = i;
        Synchronize(send_response);//передать результат
        if(SHOWLOG) {gString="ответ от ПБ принят:"; if(kolb>0) {for(int n=0;n<kolb;n++) gString+=" "+IntToHex(buf[n], 2);}else{gString+=" пусто";} gColor=clWhite; Synchronize(txt);}
    }

    Synchronize(ledOFF);//потушим индикатор

    //пауза между передачами
    if(myPult) timeout = ms_big;
    else timeout = ms_little;
    Synchronize(pause);
    //gString="пауза между передачами = "+IntToStr(timeout); gColor=clWhite; Synchronize(txt);
    while(BusyPB) {Application->ProcessMessages();}//ждем паузу между передачами

    if(Terminated)//если обнаружен принудительный выход из потока
    {//------------------------------------
 	 EnterCriticalSection( &cs );
 	 while(!Qpb.empty()) Qpb.pop();
 	 LeaveCriticalSection( &cs );
 	 //------------------------------------
 	 timeout = ms_little;
 	 Synchronize(pause);
 	 while(BusyPB) {Application->ProcessMessages();}//ждем паузу
 	 //Fr45->Diod->Visible=false;//потушим индикатор, каким бы он ни был
 	 goto exit;
    }
}
exit:
if(portPB) CloseHandle(portPB);//выключаем порт
PotokPBON=0; //сообщаем всем о выключении потока
if(SHOWLOG) {gString="выходим из потока и закрываем порт "+NCOMPBh; gColor=clWhite; Synchronize(txt);}
}
//=============================================================================
void __fastcall PBThread::txt()
{
    //gString += "\r\nPotokPBON="+IntToStr(PotokPBON)+"; portPB="+IntToStr(BusyPB)+"; Qpb.size()="+IntToStr(Qpb.size());
    //gString += "\r\nQpb.size()="+IntToStr(Qpb.size());
    SaveStatusLog(gString, gColor);
    Application->ProcessMessages();
}
//=============================================================================
void __fastcall PBThread::ledON()
{
    Fr45->Diod->Color=clRed;
    Fr45->Diod->Visible=true;//зажжем индикатор
    Application->ProcessMessages();
}
//=============================================================================
void __fastcall PBThread::ledOFF()
{
	if(Fr45->Diod->Color == clRed) Fr45->Diod->Visible=false;//потушим индикатор, если красный
    Application->ProcessMessages();
}
//=============================================================================
void __fastcall PBThread::pause()
{
extern bool BusyPB;
    Fr45->Timer9->Interval=timeout;
    Fr45->Timer9->Enabled=true;
    BusyPB=1;
    Application->ProcessMessages();
}
//=============================================================================
void __fastcall PBThread::qtxt()
{
    SaveStatusLog("ошибка очереди сообщений", clFuchsia);
    Application->ProcessMessages();
}
//=============================================================================
void __fastcall PBThread::send_response()
{
extern int PBposition;//текущая ориентация ПБ
	//передаем параметры принятого пакета в основной поток
    //dlina_PBresponse = dlina_resp;//длина пакета
    //if(dlina_PBresponse > 0) for(int i=0;i<dlina_PBresponse;i++) buf_PBresponse[i] = buf[i];//копируем буфер
    //FPBresponse = true;//флаг принятия пакета
    if(dlina_resp==6 && buf[0]==(char)GET_DISP_POS)//это ответ позиции дисплея
    {	PBposition = buf[1]&0x0F;
    }
    else if(dlina_resp!=6) {Fr45->Diod->Color = clYellow;}//при неответе делаем желтым

    Application->ProcessMessages();
}
//=============================================================================
