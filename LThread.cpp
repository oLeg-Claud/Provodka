//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "LThread.h"
#include "TRAEKT2k.h"
#pragma package(smart_init)
extern LThread *Thr;
extern bool IsGlubLeusa(void);
extern AnsiString FloatToStrF0(double x,int n,int m);
extern bool PotokON, PotokOFF;
float glub_zaboy, glub_dol;
int a;

//это поток для парсинга пакета от Леузы для монитора
//=============================================================================
__fastcall LThread::LThread(bool CreateSuspended): TThread(CreateSuspended)
{
}
//=============================================================================
void __fastcall LThread::Execute()
{
int len_buf, index, n;
char buf[15000];
char maska[]={'U','D','a','t','a','S','t','o','r','a','g','e'};
typedef union fl64 {double d; char c[8];} fl64;
fl64 val;

FreeOnTerminate=true;
PotokON=1;
PotokOFF=0;
while(PotokON)
{
    //Длина пакета от Леузы=4814 (4712), может быть различной
    len_buf=0;
    //Synchronize(txt);
    while(len_buf<4000)
    {
     //ждем порции данных
     n=Fr45->TcpClient1->ReceiveBuf(&buf[len_buf],sizeof(buf)-len_buf,0);
     //for(DWORD tt=GetTickCount();GetTickCount()-tt<500;Sleep((unsigned long)10))Application->ProcessMessages();
     if(!PotokON) break;
     if(n<=0)//потеря связи
     {
      Synchronize(tmr);//запустим вачдог
      while(PotokON) Application->ProcessMessages();
      len_buf=0;
      break;
     }
     else len_buf+=n;
    }

    //Поиск подстроки в буфере (должен быть ~=2101)
    if(len_buf>0)
    {
     int len=sizeof(maska);
     for(index=0;index<len_buf-len;index++)
     {
      for(n=0;n<len;n++) {if(maska[n] != buf[index+n]) break;}
      if( n == len ) break;
     }
     if(index<len_buf-(19+8+8))
     {
      index=index+19;//начало первого double
      for(n=0;n<8;n++) val.c[n]=buf[index+n];
      glub_zaboy=val.d;//глубина забоя
      for(n=0;n<8;n++) val.c[n]=buf[index+8+n];
      glub_dol=val.d;//глубина долота
      if(PotokON) Synchronize(glu);//сообщим новую глубину
     }
     //a=index; Synchronize(txt);
    }
}
PotokOFF=1;
return;
}
//=============================================================================
void __fastcall LThread::glu()
{
    //если глубина внешняя
    if(IsGlubLeusa) Fr45->Eglubina->Text=FloatToStrF0(glub_zaboy,7,2);
    Application->ProcessMessages();
}
//=============================================================================
void __fastcall LThread::tmr()
{
extern int CountConnects;
    CountConnects=0;//счетчик реконектов
    Fr45->Timer8->Interval=50;
    Fr45->Timer8->Enabled=true;//запустим таймер вачдога
    Application->ProcessMessages();
}
//=============================================================================
void __fastcall LThread::txt()
{
    //ShowMessage("поток завершен");
    Fr45->cerver3->Caption="принято "+IntToStr(a)+"; глубина="+FloatToStrF0(glub_zaboy,6,2);
    Application->ProcessMessages();
}
//=============================================================================
