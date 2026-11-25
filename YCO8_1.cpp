//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <fstream.h>
#include <math.h>
#include <winbase.h>
#include <SysUtils.hpp>
#include "YCO8_1.h"
#include "traekt2k.h"
#include "my_typedef.h"
#pragma package(smart_init)
extern volatile int iprov,DLKADR,DLBUF,DLMAS,DLKADRr,DLKADRrp;
extern volatile bool fl_Th, Corr_check;
extern FILE *FileSignal;
extern bool uco32, StatusPlayer;
extern float *masf,*mast,*kih;
extern double H;
extern  int m1,dst, SockNumber;
extern int DL,ks,NK;
extern float *masc;//[2*(DLKADRrp)];
extern float MasC[];
extern void Mul(int,int,float *,float *, int ,int,float * );
extern void SaveStatusLog(AnsiString str, TColor color);
extern int GetPlayerSpeed(void);

AnsiString NCOMht,ss;
int iprovt;
int ispek,ispekt,err=0;
#define DLINA_SOCK 250
int num=0,nums,numc,numt=0;
extern int ius,com,Nfiltr;
extern  HANDLE port; // {дескриптор COM-порта}
COMMTIMEOUTS  cto;// {структура тайм-аутов COM-порта}
DCB    cdcb;// {блок параметров COM-порта}
bool num_mas=0;
int massiv0[DLINA_SOCK],massiv1[DLINA_SOCK],CountSock=0;


 //-----------------------------------------------

//---------------------------------------------------------------------------
int initcom20()
{ port=CreateFile(NCOMht.c_str(),GENERIC_READ | GENERIC_WRITE,0,NULL,OPEN_EXISTING,0,NULL);
 if(port==INVALID_HANDLE_VALUE) {
ss="Ошибка открытия "+NCOMht;
 return 1;
      }
if(! GetCommTimeouts(port,&cto)){
ss= "Ошибка чтения структуры тайм-аутов  "+NCOMht;
                              return 1;}
cto.ReadTotalTimeoutConstant=1000;
cto.WriteTotalTimeoutConstant=20;
if(! SetCommTimeouts(port,&cto)){
ss="Ошибка записи структуры тайм-аутов  "+NCOMht;
                              return 1;}
if (!GetCommState(port,&cdcb)){
ss="Ошибка чтения сост.  "+NCOMht;
                 return 1;}
cdcb.BaudRate=CBR_57600;//скорость передачи данных
cdcb.ByteSize=8; //число информационных бит
cdcb.Parity=NOPARITY;//бит четности отсутствует
cdcb.StopBits=ONESTOPBIT;//один стоnовый бит
if (!SetCommState(port,&cdcb)){
ss="Ошибка инициализации  "+NCOMht;
                 return 1;}

PurgeComm(port,PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);
unsigned char b,c;
 unsigned long kolb;
switch(ius){ //команда усиления и УСО32
case 0: if(uco32)b=0x90; else b=0x20; break;
case 1: if(uco32)b=0x91; else b=0x21;break;
case 2: if(uco32)b=0x92; else b=0x22; break;
case 3: if(uco32)b=0x93; else b=0x23; break;
case 4: if(uco32)b=0x94; else b=0x24; break;
case 5: if(uco32)b=0x95; else b=0x25;  break;
            }
 //команда частоты
 if (Fr45->R4->ItemIndex==0) c=0;//16Гц
 if (Fr45->R4->ItemIndex==1) c=0;//10Гц
 if (Fr45->R4->ItemIndex==2) c=1;//7Гц
 if (Fr45->R4->ItemIndex==3) c=2;//5Гц
 if (Fr45->R4->ItemIndex==4) c=3;//3Гц
 if (Fr45->R4->ItemIndex==5) c=4;//2.5Гц
 if (Fr45->R4->ItemIndex==6) c=5;//1.8Гц
 //if (Fr45->R4->ItemIndex==3) c=6;//1.25Гц

 WriteFile(port,&b,1,&kolb,NULL);
 if(kolb!=1){ss="Ошибка посылки команды"+NCOMht; return 1;}
 Sleep((unsigned long)10);
 WriteFile(port,&c,1,&kolb,NULL);
 if(kolb!=1){ss="Ошибка посылки кода усиления"+NCOMht; return 1;}

 Sleep((unsigned long)500);
 if(Fr45->Corrector->Checked) c=0x71;
 else c=0x72;
 WriteFile(port,&c,1,&kolb,NULL);
 if(kolb!=1){ss="Ошибка посылки кода коррекции"+NCOMht; return 1;}
 return 0;
}
//------------------------------------------
int  ReadCom( unsigned char *buf,int kol)
{
unsigned long kolb;
 //если включена читалка из файла, то подмешиваем значения вместо УСО
 if(StatusPlayer==true)
 {
    //DWORD time=::GetTickCount();
    //while((::GetTickCount()-time)<1) {Application->ProcessMessages();}
    __int64 Start, Freq, k, temp;
    QueryPerformanceCounter((LARGE_INTEGER*)&Start);
    QueryPerformanceFrequency((LARGE_INTEGER*)(&Freq));
    k=Freq/1000000;//тиков в 1 мксек
    float level = (float)4000/GetPlayerSpeed();//заданная скорость
    int fl=0;
    while(!fl)
    {Application->ProcessMessages();
     QueryPerformanceCounter((LARGE_INTEGER*)&temp);
     if((float)(temp-Start)/k >= level) fl=1;
    }
 	sun32 n;
 	if(fread(&n, sizeof(n),1,FileSignal)==1)//читаем из файла
    {
    	if(uco32)
    	{	buf[0]=0;
            buf[1]=n.b[3];
            buf[2]=n.b[2];
            buf[3]=n.b[1];
            buf[4]=n.b[0];
        	buf[5]=0xff;
            return 0;
    	}
    	else
    	{	n.word = n.word>>8;//сдвинем до 24 разрядов
        	buf[0]=0;
            buf[1]=n.b[2];
            buf[2]=n.b[1];
            buf[3]=n.b[0];
        	buf[4]=0xff;
            return 0;
    	}
    }
    else {iprov=0; err=6; return 1;}//конец файла
 }
 else //иначе читаем из порта
 {
 	ReadFile(port,&buf[0],kol,&kolb,NULL);
 	if(kolb==kol) {iprovt=1; return 0;}//верный возврат
 	//все остальное - ошибка
 	if(kolb==0){iprovt=-1; err=3; return 3;}
 	iprovt=0; err=1; return 1;
 }
}
   //------------------------------------------------------

//-------------------------------
__fastcall sic4::sic4(bool CreateSuspended)
        : TThread(CreateSuspended)
{
 }
//---------------------------------------------------------------------------
void __fastcall sic4::Execute()
{
DWORD mask;
//unsigned char buf[2],kol;
unsigned char buf20[10];
unsigned long kolb;
short bb;
int bbs,errf=0,n2,n5,n4;
char c;

  	iprovt=iprov;
    err=0;
    if(uco32){n2=5;n5=6;n4=2;}else{n2=2;n5=5;n4=4;}
   	//Synchronize(st0);
  	AnsiString NCOM0=Fr45->Sport->Text;//Номер порта из текстовой области
 	int j=LastDelimiter("(",NCOM0);
	if(j>0) NCOMht=Trim(AnsiString(NCOM0.SubString(1,j-1)));
	else  NCOMht=NCOM0;
    NCOMht= "\\\\.\\"+NCOMht;
    double  x=0;
    int k=0;
    NK=-1;
    if(Nfiltr==2) num=DLBUF-m1; else num=0;
	numc=0;  nums=0;   numt=0;
  	ispek=0;
    ispekt=1;
	while(iprov!=0)
    {
     	Synchronize(sterr);//устройство подсоединилось или нет
        if(StatusPlayer==false)
        {
        	//настраиваемся на длину пакета
     		if (initcom20()==0)
     		{
        		fl_Th=true;
       			err=0;
       			iprovt=1;
       			int f5=0;
       			while(f5==0 && iprov!=0 && iprovt!=0)
       			{
        			if(ReadCom(&buf20[0],n2)==0)
        			{
         				if(buf20[0]==0xFE && buf20[1]==0x00)
         				{
          					if(ius==0) { Synchronize(sterr1); buf20[0]=0xFF ; }
          					if(ius>0) {err=2;iprov=0;}
         				}
         				if(buf20[0]==0xFF && buf20[1]==0x00) {if(ReadCom(&buf20[0],n4)==0) f5=1;}
        			}
       			}//while
       			Synchronize(sterr);
     		}//initcom20
     		else if(iprovt==1) {iprov=0;err=4;}
        }
        else fl_Th=true;//при плеере

        //рабочий цикл
     	while(iprov==1 && iprovt>0)
 	 	{
            	if(ReadCom( &buf20[0],n5)==0)
      			{
    				if( buf20[n5-1]==0xFE && buf20[0]==0x00)
        			{
      					if(ius==0)
            			{
            				Synchronize(sterr1);
      						buf20[n5-1]=0xFF ;
            			}
            			if(ius>0){err=2;iprov=0;}
        			}
   					if( buf20[n5-1]==0xFF && buf20[0]==0x00)
      				{
						if(uco32)
 						{
            				bbs=((buf20[4])+(buf20[3]<<8)+(buf20[2]<<16))+(signed int)(buf20[1]<<24);
  							mast[num]=(float)bbs;
 						}
						else
 						{
  							bbs=(signed int)((buf20[3]<<8)+(buf20[2]<<16)+( signed int)(buf20[1]<<24));
  							bbs=bbs>>8;
  							mast[num]=(float)bbs;
 						}
						if(Corr_check) //Если было установлено-снято Корректор
        				{
            				if(Fr45->Corrector->Checked) c=0x71;
            				else c=0x72;
            				WriteFile(port,&c,1,&kolb,NULL);
            				if(kolb!=1){ss="Ошибка посылки кода коррекции"+NCOMht;}
            				Corr_check=0;
        				}
					}
    				else
        			{
     					mast[num]=0;
            			int f5=0;
     					while(f5==0&&iprov!=0)
       					{
            				if(ReadCom(&buf20[0],n2)==0)
                			{
                				if( buf20[0]==0xFE && buf20[1]== 0x00)
                    			{
   									if(ius==0)
                        			{
                        				Synchronize(sterr1);
     									buf20[0]=0xFF ;
                        			}
  									if(ius>0) {err=2;iprov=0;}
     							}
       							if(buf20[0]==0xFF && buf20[1]==0x00)
             					{
     								if(ReadCom(&buf20[0],n4)==0) f5=1;
                   				}
            				}
       					}
    				}
      			}
	 			if(iprovt>0 && iprov==1)
     			{
                    //---------------spektr--------------
					MasC[nums+ispek*DL]=mast[num];///sqrt(DL);
 					if(nums==DL)
        			{
        				PostMessage(Fr45->Handle,WM_MyPost6,ispek,ispekt);
   						ispek=ispek+1;
            			if(ispek==ks)ispek=0;
    					ispekt=ispekt+1;
            			if(ispekt>ks) ispekt=ks;
    					nums=0;
        			}
        			nums=nums+1;
					//---------------filtr-----------
 					masf[num]=mast[num];
    				int j;
 					j=num-m1;
            		if(j<0) j=j+DLBUF;
 					if(dst==0)
            		{
            			masf[num]=0;
						for (int ip=0;ip<m1;ip++)
                		{
                			j=num-ip;
                    		if(j<0) j=j+DLBUF;
  							masf[num]=masf[num]+kih[ip]*mast[j];
                		}
       				}
					else  Mul(m1,0,kih,mast,j,DLBUF, &masf[num]);
					//---------------masc------------
                    //Fr45->StatusBar1->SimpleText="NK="+IntToStr(NK);
                    //-----------Сообщение потоку 1--------------------
					if(NK!=-1)  PostMessage(Fr45->Handle,WM_MyPost1,num,numc-1);
                    //-----------Сообщение потоку 1--------------------
					if(NK==-1)  PostMessage(Fr45->Handle,WM_MyPost1,num,DLBUF-1);//DLBUF удвоенная длина кадра с запасом
					else
        			while(k<num)
					{
        				masc[numc]=masf[k]+(masf[k]-masf[k+1])*(k-x);
						numc=numc+1;
						x=x+H;
						k=(int)x;
  						if(numc==2*DLKADRrp) numc=0;
        			}
                    num=num+1;
     				numt=numt+1;

					//------передача данных
					if(num==DLBUF && NK==-1) {NK=0;num=0;numt=0;x=0;k=0;}
     				if(num==DLKADR+2*DLMAS && NK==0)
        			{
                        //-----------Сообщение потоку 2--------------------
      					PostMessage(Fr45->Handle,WM_MyPost2,0,0); NK=1;
                        //Fr45->StatusBar1->SimpleText="num="+IntToStr(num);
        			}
     				if(num==DLBUF) {x=0;k=0;numt=0; num=0;}

 				}  //  iprov==1
        } //  while
 		if(iprovt==-1) if(port!=INVALID_HANDLE_VALUE) {CloseHandle(port); port=NULL;}
    }// while iprov!=0
    if(port!=INVALID_HANDLE_VALUE)
 	{
    	buf20[0]=0x10;
   		WriteFile(port,&buf20[0],1,&kolb,NULL);
 		CloseHandle(port);port=NULL;
    }
    fl_Th=false;
  	Synchronize(st);
}
//---------------------------------------------------------------------------
void __fastcall sic4:: st()
{ if(err!=0) Fr45->stop->Down=true;
  if(err==1)
  {SaveStatusLog("Ошибка чтения  "+NCOMht, clRed);
  }
  if(err==2)
  { Fr45->Label8->Caption="ПЕРЕПОЛНЕНИЕ на усилении "+IntToStr((int)pow(2,ius));
    Fr45->Timer2->Enabled=true;
  }
  if(err==4)
  {SaveStatusLog(ss, clRed);
   Fr45->Timer2->Enabled=true;
  }
  if(err==6)//конец файла в проигрывателе
  {Fr45->Timer2->Enabled=true;
  }
}
//---------------------------------------------------------------------------
void __fastcall sic4:: st0()
 {
 }
//---------------------------------------------------------------------------
void __fastcall sic4:: sterr()
{   if(iprovt==-1) SaveStatusLog("Нет отклика от УСО "+NCOMht, clRed);
    else
    {Fr45->StatusBar->Color=clWhite;
     Fr45->StatusBar->SimpleText="УСО "+NCOMht+"  подсоединилось ";
    }
}
 //---------------------------------------------------------------------------
 void __fastcall sic4:: sterr1()
 { Fr45->Label8->Caption="ПЕРЕПОЛНЕНИЕ на усилении 1";
 Fr45-> Timer1->Enabled=true;
 }
  //---------------------------------------------------------------------------
void __fastcall sic4:: kih0()
{  	int j;
 	j=num-m1;
    if(j<0)j=j+DLBUF;
 	if(dst==0)
    {	masf[num]=0;
		for (int ip=0;ip<m1;ip++)
		{j=num-ip; if(j<0)j=j+DLBUF;
         masf[num]=masf[num]+kih[ip]*mast[j];
        }
    }
	else     Mul(m1,0,kih,mast,j,DLBUF, &masf[num]);
}
