#include <math.h>
#include <stdlib.h>
#include <fstream.h>
#include "TRAEKT2k.h"
#include "format1_2k.h"
#include "format2_2k.h"
#include "format11_2k.h"
#include "format12_2k.h"
#include "format13_2k.h"
#include "format30_2k.h"
#include "format31_2k.h"
#include "format_kadra.h"
#include "my_typedef.h"
typedef unsigned int    uint32_t;
typedef unsigned short  uint16_t;
typedef unsigned char  	uint8_t;
extern void Mul(int,int,float *,float *, int ,int,float * );
extern float rotor(float up,float down);
extern double  MyStrToFloat(AnsiString s);
extern uint8_t masQuality[rowCount];
extern AnsiString FloatToStrF0(double x,int n,int m);
extern int MarkerFreq;
extern int Count_Geomash;
extern float get_ums(void);
extern float get_umn(void);
extern float get_Hop(void);
extern void SaveStatusLog(AnsiString str, TColor color);
//extern void write_json_dat(AnsiString key, AnsiString val);
extern void write_tabl_dat(AnsiString key, AnsiString val);

int HXout=0, HYout=0, HZout=0, servis;
float GXout=0, GYout=0, GZout=0;
bool QUAL_STAT=0, QUAL_NDM_STAT=0;

float resolve_az(float hx, float hy, float hz, float rzu, float rotk);
float scale_gk_log(uint8_t code);

GEOMASH_type GEOMASH;

#define SCALE_OTK		360/254 //масштабирование ОТК в 8 бит
#define SCALE_ZU		10240/4096 //масштабирование ЗУ в 12 бит

//============================================================================
int revers(unsigned char d1)
{
   int j=1,d2=0;
   for (int i=1;i<5;i++)
    {
     if((d1&j)>>(i-1)==0) d2=d2+j;
     j=j*2;
    }
  return d2;
}
//============================================================================
//в этой функции вычисляем параметры в зависимости от номера принятого 5-битного вектора
//вывод полученных значений будем делать или не делать ниже, уже в зависимости от уровня процентов
void kordat4_8(int indl,int kod,float *zu,float *az,float *otk,int *obor,
               int *dotk1,int *temp,int *ind0,int *dotk2,int *vib,float *gk,int *tok,
               float *zur,float *mn,float *az_otk,float *modg,int *sk,int *dpsk)
{
 extern AnsiString AZist_str;
 extern int DLKADRr,DLKADRrp,DLMASr,DLDIGr,zarez,ctat, iot,prib,iG,wr;
 extern int dat_o1, dat_o2,dat_o3,dat_o4,dat_oa1,dat_oa2,dat_oa3,dat_oa4,HX,HY,HZ,Htotal;
 extern int dat_o5, dat_o6,dat_o7,dat_o8,dat_oa5,dat_oa6,dat_oa7,dat_oa8;
 extern float NAPR,mgk, KCndm, Kgeo, Azk, Azk_ist;
 extern unsigned char num_mode;
 extern int FORMAT_KADRA;
 extern int indic, TOKndm;
 extern float ZUndm, gkzts, zu_inc, GKup, GKdn, NAGRndm, OBORndm, PEAKndm, TEMPndm, dUndm, MAHndm, NAGRndm_null, MAHbatt;
 extern float MOMndm, DAVLndm, Kgk_zts, sigKKS;
 extern bool USIL;
 extern float nap1;//,zu1,zugi,zufi,kc;
 extern  float *mas;//[DLKADRr]
 extern  float *mas1;//[DLKADRr+2*DLMASr];
 extern int faza,dproc[],dst,Hop,DCP,r_az,DCPt;
 extern unsigned char dig[];
 extern  int dg[];
 extern int inta(double a);
 #include "VEC_NUMS.h"
 extern float xkd[VEC_NUMS];
 extern int  pskv[VEC_NUMS];
 extern char data_perc[];
 extern AnsiString RoleStr;
 unsigned char digr;
 double pi=M_PI;
 int i,j,k,max0,max1,indt;
 float pdig[VEC_NUMS],pdig1[100],xkv, otkl;
 static float otk_stat;
// {
 for(i=0;i<VEC_NUMS;i++)
     {indt=indl+iG;
      if(dst==0)
       {xkv=0;pdig[i]=0;
        for(j=0;j<DLDIGr;j++)
        {if(indt==DLKADRrp) indt=0;
         pdig[i]+=mas1[indt]*mas[DLMASr+i*DLDIGr+j];
         xkv+=mas1[indt]*mas1[indt];
         indt++;
        }
       }
       else
       {if(indt==DLKADRr)indt=0;
        Mul((__int16) DLDIGr,1,mas1, mas1,indt,DLKADRr, &xkv);
        Mul((__int16) DLDIGr,0, &mas[DLMASr+i*DLDIGr], mas1,indt,DLKADRr, &pdig[i]);
       }
     pdig[i]*=faza;
     if(xkv==0) pdig[i]=0; else pdig[i]/=sqrt(xkd[i]*xkv);
    }//end for
//-------------------------
 //тут похоже выбор ниалучшей корреляции данных
 for(i=0;i<VEC_NUMS;i++)  {pskv[i]=inta(pdig[i]*100);}
 max0=0;
 for(i=0;i<VEC_NUMS;i++) {if(fabs(pdig[i])>fabs(pdig[max0])) max0=i;}
 dig[kod]=max0;//результат корреляции - цифра 5бит
 dg[kod]=max0;
//------------------------------------
 for(i=0;i<2*iG;i++)
     {indt=indl+i;
      if(dst==0)
       {xkv=pdig1[i]=0;
        for(j=0;j<DLDIGr;j++)
         {if(indt==DLKADRrp) indt=0;
          pdig1[i]+=mas1[indt]*mas[DLMASr+max0*DLDIGr+j];
          xkv+=mas1[indt]*mas1[indt];
          indt++;
         }
       }
        else
         {if(indt==DLKADRrp)indt=0; Mul((__int16) DLDIGr,1,mas1,mas1,indt,DLKADRr, &xkv);
          Mul((__int16) DLDIGr,0, &mas[DLMASr+max0*DLDIGr], mas1,indt,DLKADRr, &pdig1[i]);
         }
      pdig1[i]*=faza;
      if(xkv==0)pdig1[i]=0;else pdig1[i]/=sqrt(xkd[max0]*xkv);
     }
 for(max1=i=0;i<2*iG;i++) if(fabs(pdig1[i])>fabs(pdig1[max1])) max1=i;
 if(pdig1[max1]<0) dig[kod]=dig[kod]|VEC_NUMS;//если процент отрицательный, то код=+16
 dproc[kod+1]=inta(fabs(pdig1[max1]*100.));
 *ind0=max1-iG;
 //if(newr==0 && kod==4) ShowMessage(IntToStr(dig[kod]));

 *dpsk=dproc[kod+1];
 data_perc[kod]=dproc[kod+1];//Сохраняем проценты достоверности по данным.
 *sk=dig[kod];

 int16_t ttt=0;
//========================================================================================
  //Mode и признак статики
  if(	(FORMAT_KADRA==FORMAT_STD_GK && kod==FIRST_KOD_R1 && data_perc[FIRST_KOD_R1]>=DCP)
  		||(FORMAT_KADRA==FORMAT_NDM_GK_RETR && kod==FIRST_KOD_R2 && data_perc[FIRST_KOD_R2]>=DCP)
        ||(FORMAT_KADRA==FORMAT_GK_RETR && kod==FIRST_KOD_R11 && data_perc[FIRST_KOD_R11]>=DCP)
        ||(FORMAT_KADRA==FORMAT_STD && kod==FIRST_KOD_R12 && data_perc[FIRST_KOD_R12]>=DCP)
        ||(FORMAT_KADRA==FORMAT_STD_RETR && kod==FIRST_KOD_R13 && data_perc[FIRST_KOD_R13]>=DCP)
        ||(FORMAT_KADRA==FORMAT_GEOMASH && kod==FIRST_KOD_R30 && data_perc[FIRST_KOD_R30]>=DCP)
        ||(FORMAT_KADRA==FORMAT_GEOMASH_RETR && kod==FIRST_KOD_R31 && data_perc[FIRST_KOD_R31]>=DCP)
    )
  {int z=(dig[kod]&0x10)>>4;
   //if(z) newr=0; else newr=1;//режим УГЛЫ или ОТК
   ctat=z;//признак статики - ст.бит
   iot=(dig[kod]&0x0F);//mode
   write_tabl_dat("Статика", IntToStr(ctat));//в черный ящик
  }
//======================================================================================
// распарсим текущий вектор в зависимости от режима работы
//======================================================================================
 //Декодируем все параметры в режиме работы № 1
 if(FORMAT_KADRA==FORMAT_STD_GK)
 {
  switch(kod)
  {
    //ГКзтс (4шт) с логарифмическим масштабированием (8 бит)
    case(GK1_L_R1): gkzts=((dig[GK1_H_R1]&0x07)*32)+dig[GK1_L_R1];//ГK1
    				gkzts=scale_gk_log(gkzts)*Kgk_zts;
                    break;
    case(GK2_L_R1): gkzts=((dig[GK2_H_R1]&0x07)*32)+dig[GK2_L_R1];//ГK2
    				gkzts=scale_gk_log(gkzts)*Kgk_zts;
                    break;
    case(GK3_L_R1): gkzts=((dig[GK3_H_R1]&0x07)*32)+dig[GK3_L_R1];//ГK3
    				gkzts=scale_gk_log(gkzts)*Kgk_zts;
                    break;
    case(GK4_L_R1): gkzts=((dig[GK4_H_R1]&0x07)*32)+dig[GK4_L_R1];//ГK4
    				gkzts=scale_gk_log(gkzts)*Kgk_zts;
                    break;
    //АЗ-ОТК (4шт)
    case(AZ_OTK1_L_R1):dat_oa1=inta((float)((dig[AZ_OTK1_H_R1]&0x07)*32+dig[AZ_OTK1_L_R1])*SCALE_OTK); break;
    case(AZ_OTK2_L_R1):dat_oa2=inta((float)((dig[AZ_OTK2_H_R1]&0x07)*32+dig[AZ_OTK2_L_R1])*SCALE_OTK); break;
    case(AZ_OTK3_L_R1):dat_oa3=inta((float)((dig[AZ_OTK3_H_R1]&0x07)*32+dig[AZ_OTK3_L_R1])*SCALE_OTK); break;
    case(AZ_OTK4_L_R1):dat_oa4=inta((float)((dig[AZ_OTK4_H_R1]&0x07)*32+dig[AZ_OTK4_L_R1])*SCALE_OTK); break;
    //ОТК динамический (4шт)
    case(OTK1_L_R1): dat_o1=inta((float)((dig[OTK1_H_R1]&0x07)*32+dig[OTK1_L_R1])*SCALE_OTK); break;
    case(OTK2_L_R1): dat_o2=inta((float)((dig[OTK2_H_R1]&0x07)*32+dig[OTK2_L_R1])*SCALE_OTK); break;
    case(OTK3_L_R1): dat_o3=inta((float)((dig[OTK3_H_R1]&0x07)*32+dig[OTK3_L_R1])*SCALE_OTK); break;
    case(OTK4_L_R1): dat_o4=inta((float)((dig[OTK4_H_R1]&0x07)*32+dig[OTK4_L_R1])*SCALE_OTK); break;
    //ОТК угловой
    case(OTKu_L_R1): otkl=dig[OTKu_H_R1]*32+dig[OTKu_L_R1]; *otk=otkl*0.4; otk_stat=*otk; break;
    //ЗУ
    case(AZ_OTK4_H_R1): *zu=(float)dig[ZU_H_R1]*128+(float)dig[ZU_L_R1]*4+(dig[AZ_OTK4_H_R1]>>3);*zu=*zu*SCALE_ZU/100.; break;
    //модуль G
    case(GMOD_L_R1): ttt=0;
      ttt=dig[GMOD_H_R1]*32+dig[GMOD_L_R1];
      ttt=ttt<<6; ttt=ttt>>6;//делаем знак, если отрицательное
      *modg=1.+(float)ttt/4096.;//модуль G
      break;
    //Hx
    case(HY_H_R1): ttt=0;
      pars_bits(dig[HX_H_R1],0,0,(uint16_t*)&ttt,11);
      pars_bits(dig[HX_M_R1],4,0,(uint16_t*)&ttt,10);
      pars_bits(dig[HX_L_R1],4,0,(uint16_t*)&ttt,5);
      pars_bits(dig[HY_H_R1],4,4,(uint16_t*)&ttt,0);
      if(ttt&0x0800) ttt=ttt|0xF000;//если отрицательное
      HX=ttt;
      break;
    //Hy
    case(HY_L_R1): ttt=0;
      pars_bits(dig[HY_H_R1],3,0,(uint16_t*)&ttt,11);
      pars_bits(dig[HY_M_R1],4,0,(uint16_t*)&ttt,7);
      pars_bits(dig[HY_L_R1],4,2,(uint16_t*)&ttt,2);
      if(ttt&0x0800) ttt=ttt|0xF000;//если отрицательное
      HY=ttt;
      break;
    //Hz
    case(HZ_L_R1): ttt=0;
      pars_bits(dig[HY_L_R1],1,0,(uint16_t*)&ttt,11);
      pars_bits(dig[HZ_H_R1],4,0,(uint16_t*)&ttt,9);
      pars_bits(dig[HZ_L_R1],4,0,(uint16_t*)&ttt,4);
      if(ttt&0x0800) ttt=ttt|0xF000;//если отрицательное
      HZ=ttt;
      break;
    //Код режима (6бит) и Роль в ст.бите
    case(GK3_H_R1):		ttt=0;
    					pars_bits(dig[AZ_OTK2_H_R1],4,3,(uint16_t*)&ttt,5);
                        pars_bits(dig[OTK3_H_R1],4,3,(uint16_t*)&ttt,3);
                        pars_bits(dig[GK3_H_R1],4,3,(uint16_t*)&ttt,1);
                        num_mode=ttt&31;//только 5 бит
                        if(ttt&32) RoleStr=" Ретр";//роль Ретранслятор
                   	   	else RoleStr=" База";//роль База
        				break;
    //Вибрация (6бит)
    case(GK4_H_R1): ttt=0;
                    pars_bits(dig[AZ_OTK3_H_R1],4,3,(uint16_t*)&ttt,5);
                    pars_bits(dig[OTK4_H_R1],4,3,(uint16_t*)&ttt,3);
                    pars_bits(dig[GK4_H_R1],4,3,(uint16_t*)&ttt,1);
    				*vib=ttt;
                    break;
    //Обороты ЗТС
    case(OTK3_H_R1): ttt=0;
      pars_bits(dig[OTK1_H_R1],4,3,(uint16_t*)&ttt,9);
      pars_bits(dig[GK1_H_R1],4,3,(uint16_t*)&ttt,7);
      pars_bits(dig[AZ_OTK1_H_R1],4,3,(uint16_t*)&ttt,5);
      pars_bits(dig[OTK2_H_R1],4,3,(uint16_t*)&ttt,3);
      pars_bits(dig[GK2_H_R1],4,3,(uint16_t*)&ttt,1);
      servis=ttt;
      break;
  }
 }
//======================================================================================
 //Режим работы № 11
 if(FORMAT_KADRA==FORMAT_GK_RETR)
 {
  switch(kod)
  {
    //ГКзтс (4шт) с логарифмическим масштабированием (8 бит)
    case(GK1_L_R11): gkzts=((dig[GK1_H_R11]&0x07)*32)+dig[GK1_L_R11];//ГK1
    				gkzts=scale_gk_log(gkzts)*Kgk_zts;
                    break;
    case(GK2_L_R11): gkzts=((dig[GK2_H_R11]&0x07)*32)+dig[GK2_L_R11];//ГK2
    				gkzts=scale_gk_log(gkzts)*Kgk_zts;
                    break;
    case(GK3_L_R11): gkzts=((dig[GK3_H_R11]&0x07)*32)+dig[GK3_L_R11];//ГK3
    				gkzts=scale_gk_log(gkzts)*Kgk_zts;
                    break;
    case(GK4_L_R11): gkzts=((dig[GK4_H_R11]&0x07)*32)+dig[GK4_L_R11];//ГK4
    				gkzts=scale_gk_log(gkzts)*Kgk_zts;
                    break;
    //АЗ-ОТК (4шт)
    case(AZ_OTK1_L_R11):dat_oa1=inta((float)((dig[AZ_OTK1_H_R11]&0x07)*32+dig[AZ_OTK1_L_R11])*SCALE_OTK); break;
    case(AZ_OTK2_L_R11):dat_oa2=inta((float)((dig[AZ_OTK2_H_R11]&0x07)*32+dig[AZ_OTK2_L_R11])*SCALE_OTK); break;
    case(AZ_OTK3_L_R11):dat_oa3=inta((float)((dig[AZ_OTK3_H_R11]&0x07)*32+dig[AZ_OTK3_L_R11])*SCALE_OTK); break;
    case(AZ_OTK4_L_R11):dat_oa4=inta((float)((dig[AZ_OTK4_H_R11]&0x07)*32+dig[AZ_OTK4_L_R11])*SCALE_OTK); break;
    //ОТК динамический (4шт)
    case(OTK1_L_R11): dat_o1=inta((float)((dig[OTK1_H_R11]&0x07)*32+dig[OTK1_L_R11])*SCALE_OTK); break;
    case(OTK2_L_R11): dat_o2=inta((float)((dig[OTK2_H_R11]&0x07)*32+dig[OTK2_L_R11])*SCALE_OTK); break;
    case(OTK3_L_R11): dat_o3=inta((float)((dig[OTK3_H_R11]&0x07)*32+dig[OTK3_L_R11])*SCALE_OTK); break;
    case(OTK4_L_R11): dat_o4=inta((float)((dig[OTK4_H_R11]&0x07)*32+dig[OTK4_L_R11])*SCALE_OTK); break;
    //ОТК угловой
    case(OTKu_L_R11): otkl=dig[OTKu_H_R11]*32+dig[OTKu_L_R11]; *otk=otkl*0.4; otk_stat=*otk; break;
    //ЗУ
    case(AZ_OTK4_H_R11): *zu=(float)dig[ZU_H_R11]*128+(float)dig[ZU_L_R11]*4+(dig[AZ_OTK4_H_R11]>>3);*zu=*zu*SCALE_ZU/100.; break;
    //модуль G
    case(GMOD_L_R11): ttt=0;
      ttt=dig[GMOD_H_R11]*32+dig[GMOD_L_R11];
      ttt=ttt<<6; ttt=ttt>>6;//делаем знак, если отрицательное
      *modg=1.+(float)ttt/4096.;//модуль G
      break;
    //Hx
    case(HY_H_R11): ttt=0;
      pars_bits(dig[HX_H_R11],0,0,(uint16_t*)&ttt,11);
      pars_bits(dig[HX_M_R11],4,0,(uint16_t*)&ttt,10);
      pars_bits(dig[HX_L_R11],4,0,(uint16_t*)&ttt,5);
      pars_bits(dig[HY_H_R11],4,4,(uint16_t*)&ttt,0);
      if(ttt&0x0800) ttt=ttt|0xF000;//если отрицательное
      HX=ttt;
      break;
    //Hy
    case(HY_L_R11): ttt=0;
      pars_bits(dig[HY_H_R11],3,0,(uint16_t*)&ttt,11);
      pars_bits(dig[HY_M_R11],4,0,(uint16_t*)&ttt,7);
      pars_bits(dig[HY_L_R11],4,2,(uint16_t*)&ttt,2);
      if(ttt&0x0800) ttt=ttt|0xF000;//если отрицательное
      HY=ttt;
      break;
    //Hz
    case(HZ_L_R11): ttt=0;
      pars_bits(dig[HY_L_R11],1,0,(uint16_t*)&ttt,11);
      pars_bits(dig[HZ_H_R11],4,0,(uint16_t*)&ttt,9);
      pars_bits(dig[HZ_L_R11],4,0,(uint16_t*)&ttt,4);
      if(ttt&0x0800) ttt=ttt|0xF000;//если отрицательное
      HZ=ttt;
      break;
    //Код режима (6бит) и Роль в ст.бите
    case(GK3_H_R11):	ttt=0;
    					pars_bits(dig[AZ_OTK2_H_R11],4,3,(uint16_t*)&ttt,5);
                        pars_bits(dig[OTK3_H_R11],4,3,(uint16_t*)&ttt,3);
                        pars_bits(dig[GK3_H_R11],4,3,(uint16_t*)&ttt,1);
                        num_mode=ttt&31;//только 5 бит
                        if(ttt&32) RoleStr=" Ретр";//роль Ретранслятор
                   	   	else RoleStr=" База";//роль База
        				break;
    //Вибрация (6бит)
    case(GK4_H_R11): ttt=0;
                    pars_bits(dig[AZ_OTK3_H_R11],4,3,(uint16_t*)&ttt,5);
                    pars_bits(dig[OTK4_H_R11],4,3,(uint16_t*)&ttt,3);
                    pars_bits(dig[GK4_H_R11],4,3,(uint16_t*)&ttt,1);
    				*vib=ttt;
                    break;
    //Обороты ЗТС
    case(OTK3_H_R11): ttt=0;
      pars_bits(dig[OTK1_H_R11],4,3,(uint16_t*)&ttt,9);
      pars_bits(dig[GK1_H_R11],4,3,(uint16_t*)&ttt,7);
      pars_bits(dig[AZ_OTK1_H_R11],4,3,(uint16_t*)&ttt,5);
      pars_bits(dig[OTK2_H_R11],4,3,(uint16_t*)&ttt,3);
      pars_bits(dig[GK2_H_R11],4,3,(uint16_t*)&ttt,1);
      servis=ttt;
      break;
  }
 }
//======================================================================================
 //Режим работы № 2
 short s1,s2;
 if(FORMAT_KADRA==FORMAT_NDM_GK_RETR)
 {
  switch(kod)
  {
    //ГК зтс
    case(GK1_L_R2): gkzts=((dig[GK1_H_R2]&0x07)*32)+dig[GK1_L_R2];//ГK1
    				gkzts=scale_gk_log(gkzts)*Kgk_zts;
                    break;
    case(GK2_L_R2): gkzts=((dig[GK2_H_R2]&0x07)*32)+dig[GK2_L_R2];//ГK2
    				gkzts=scale_gk_log(gkzts)*Kgk_zts;
                    break;
    case(GK3_L_R2): gkzts=((dig[GK3_H_R2]&0x07)*32)+dig[GK3_L_R2];//ГK3
    				gkzts=scale_gk_log(gkzts)*Kgk_zts;
                    break;
    case(GK4_L_R2): gkzts=((dig[GK4_H_R2]&0x07)*32)+dig[GK4_L_R2];//ГK4
    				gkzts=scale_gk_log(gkzts)*Kgk_zts;
                    break;
    case(GK5_L_R2): gkzts=((dig[GK5_H_R2]&0x07)*32)+dig[GK5_L_R2];//ГK5
    				gkzts=scale_gk_log(gkzts)*Kgk_zts;
                    break;
    case(GK6_L_R2): gkzts=((dig[GK6_H_R2]&0x07)*32)+dig[GK6_L_R2];//ГK6
    				gkzts=scale_gk_log(gkzts)*Kgk_zts;
                    break;
    case(GK7_L_R2): gkzts=((dig[GK7_H_R2]&0x07)*32)+dig[GK7_L_R2];//ГK7
    				gkzts=scale_gk_log(gkzts)*Kgk_zts;
                    break;
    //ГК ндм
    case(GK1up_L_R2): 	GKup=((dig[GK1up_H_R2]&0x07)*32)+dig[GK1up_L_R2];//ГK1верх
    					GKup=scale_gk_log(GKup)*Kgk_zts;
                    	break;
    case(GK1dn_L_R2): 	GKdn=((dig[GK1dn_H_R2]&0x07)*32)+dig[GK1dn_L_R2];//ГK1низ
    					GKdn=scale_gk_log(GKdn)*Kgk_zts;
                    	break;
    case(GK2up_L_R2): 	GKup=((dig[GK2up_H_R2]&0x07)*32)+dig[GK2up_L_R2];//ГK2верх
    					GKup=scale_gk_log(GKup)*Kgk_zts;
                    	break;
    case(GK2dn_L_R2): 	GKdn=((dig[GK2dn_H_R2]&0x07)*32)+dig[GK2dn_L_R2];//ГK2низ
    					GKdn=scale_gk_log(GKdn)*Kgk_zts;
                    	break;
    case(GK3up_L_R2): 	GKup=((dig[GK3up_H_R2]&0x07)*32)+dig[GK3up_L_R2];//ГK3верх
    					GKup=scale_gk_log(GKup)*Kgk_zts;
                    	break;
    case(GK3dn_L_R2): 	GKdn=((dig[GK3dn_H_R2]&0x07)*32)+dig[GK3dn_L_R2];//ГK3низ
    					GKdn=scale_gk_log(GKdn)*Kgk_zts;
                    	break;
    case(GK4up_L_R2): 	GKup=((dig[GK4up_H_R2]&0x07)*32)+dig[GK4up_L_R2];//ГK4верх
    					GKup=scale_gk_log(GKup)*Kgk_zts;
                    	break;
    case(GK4dn_L_R2): 	GKdn=((dig[GK4dn_H_R2]&0x07)*32)+dig[GK4dn_L_R2];//ГK4низ
    					GKdn=scale_gk_log(GKdn)*Kgk_zts;
                    	break;
    case(GK5up_L_R2): 	GKup=((dig[GK5up_H_R2]&0x07)*32)+dig[GK5up_L_R2];//ГK5верх
    					GKup=scale_gk_log(GKup)*Kgk_zts;
                    	break;
    case(GK5dn_L_R2): 	GKdn=((dig[GK5dn_H_R2]&0x07)*32)+dig[GK5dn_L_R2];//ГK5низ
    					GKdn=scale_gk_log(GKdn)*Kgk_zts;
                    	break;
    case(GK6up_L_R2): 	GKup=((dig[GK6up_H_R2]&0x07)*32)+dig[GK6up_L_R2];//ГK6верх
    					GKup=scale_gk_log(GKup)*Kgk_zts;
                    	break;
    case(GK6dn_L_R2): 	GKdn=((dig[GK6dn_H_R2]&0x07)*32)+dig[GK6dn_L_R2];//ГK6низ
    					GKdn=scale_gk_log(GKdn)*Kgk_zts;
                    	break;
    case(GK7up_L_R2): 	GKup=((dig[GK7up_H_R2]&0x07)*32)+dig[GK7up_L_R2];//ГK7верх
    					GKup=scale_gk_log(GKup)*Kgk_zts;
                    	break;
    case(GK7dn_L_R2): 	GKdn=((dig[GK7dn_H_R2]&0x07)*32)+dig[GK7dn_L_R2];//ГK7низ
    					GKdn=scale_gk_log(GKdn)*Kgk_zts;
                    	break;
    //АЗ-ОТК
    case(AZ_OTK1_L_R2):dat_oa1=inta((float)((dig[AZ_OTK1_H_R2]&0x07)*32+dig[AZ_OTK1_L_R2])*SCALE_OTK); break;
    case(AZ_OTK2_L_R2):dat_oa2=inta((float)((dig[AZ_OTK2_H_R2]&0x07)*32+dig[AZ_OTK2_L_R2])*SCALE_OTK); break;
    case(AZ_OTK3_L_R2):dat_oa3=inta((float)((dig[AZ_OTK3_H_R2]&0x07)*32+dig[AZ_OTK3_L_R2])*SCALE_OTK); break;
    case(AZ_OTK4_L_R2):	dat_oa4=inta((float)((dig[AZ_OTK4_H_R2]&0x07)*32+dig[AZ_OTK4_L_R2])*SCALE_OTK);break;
    case(AZ_OTK5_L_R2):dat_oa5=inta((float)((dig[AZ_OTK5_H_R2]&0x07)*32+dig[AZ_OTK5_L_R2])*SCALE_OTK); break;
    case(AZ_OTK6_L_R2):dat_oa6=inta((float)((dig[AZ_OTK6_H_R2]&0x07)*32+dig[AZ_OTK6_L_R2])*SCALE_OTK); break;
    case(AZ_OTK7_L_R2):dat_oa7=inta((float)((dig[AZ_OTK7_H_R2]&0x07)*32+dig[AZ_OTK7_L_R2])*SCALE_OTK); break;
    //ОТК динамический
    case(OTK1_L_R2): dat_o1=inta((float)((dig[OTK1_H_R2]&0x07)*32+dig[OTK1_L_R2])*SCALE_OTK); break;
    case(OTK2_L_R2): dat_o2=inta((float)((dig[OTK2_H_R2]&0x07)*32+dig[OTK2_L_R2])*SCALE_OTK); break;
    case(OTK3_L_R2): dat_o3=inta((float)((dig[OTK3_H_R2]&0x07)*32+dig[OTK3_L_R2])*SCALE_OTK); break;
    case(OTK4_L_R2): dat_o4=inta((float)((dig[OTK4_H_R2]&0x07)*32+dig[OTK4_L_R2])*SCALE_OTK); break;
    case(OTK5_L_R2): dat_o5=inta((float)((dig[OTK5_H_R2]&0x07)*32+dig[OTK5_L_R2])*SCALE_OTK); break;
    case(OTK6_L_R2): dat_o6=inta((float)((dig[OTK6_H_R2]&0x07)*32+dig[OTK6_L_R2])*SCALE_OTK); break;
    case(OTK7_L_R2): dat_o7=inta((float)((dig[OTK7_H_R2]&0x07)*32+dig[OTK7_L_R2])*SCALE_OTK); break;
    //ОТК угловой
    case(OTKu_L_R2): otkl=dig[OTKu_H_R2]*32+dig[OTKu_L_R2]; *otk=otkl*0.4; otk_stat=*otk; break;
    //ЗУ
    case(AZ_OTK7_H_R2): *zu=(float)dig[ZU_H_R2]*128+(float)dig[ZU_L_R2]*4+(dig[AZ_OTK7_H_R2]>>3);*zu=*zu*SCALE_ZU/100.; break;
    //модуль G
    case(GMOD_L_R2): ttt=0;
      ttt=dig[GMOD_H_R2]*32+dig[GMOD_L_R2];
      ttt=ttt<<6; ttt=ttt>>6;//делаем знак, если отрицательное
      *modg=1.+(float)ttt/4096.;//модуль G
      break;
    //Hx
    case(HX_L_R2): ttt=0;
      pars_bits(dig[HX_H_R2],4,0,(uint16_t*)&ttt,11);
      pars_bits(dig[HX_M_R2],4,0,(uint16_t*)&ttt,6);
      pars_bits(dig[HX_L_R2],4,3,(uint16_t*)&ttt,1);
      if(ttt&0x0800) ttt=ttt|0xF000;//если отрицательное
      HX=ttt;
      break;
    //Hy
    case(GK3up_H_R2): ttt=0;
      pars_bits(dig[HX_L_R2],2,0,(uint16_t*)&ttt,11);
      pars_bits(dig[HY_H_R2],4,0,(uint16_t*)&ttt,8);
      pars_bits(dig[GK3_H_R2],4,3,(uint16_t*)&ttt,3);
      pars_bits(dig[GK3up_H_R2],4,3,(uint16_t*)&ttt,1);
      if(ttt&0x0800) ttt=ttt|0xF000;//если отрицательное
      HY=ttt;
      break;
    //Hz
    case(HZ_L_R2): ttt=0;
      pars_bits(dig[AZ_OTK3_H_R2],4,3,(uint16_t*)&ttt,11);
      pars_bits(dig[GK3dn_H_R2],4,3,(uint16_t*)&ttt,9);
      pars_bits(dig[HZ_H_R2],4,0,(uint16_t*)&ttt,7);
      pars_bits(dig[HZ_L_R2],4,2,(uint16_t*)&ttt,2);
      if(ttt&0x0800) ttt=ttt|0xF000;//если отрицательное
      HZ=ttt;
      break;
    //Код режима (6бит) и Роль в старшем
    case(GK2up_H_R2):	ttt=0;
    					pars_bits(dig[OTK2_H_R2],4,3,(uint16_t*)&ttt,5);
                        pars_bits(dig[GK2_H_R2],4,3,(uint16_t*)&ttt,3);
                        pars_bits(dig[GK2up_H_R2],4,3,(uint16_t*)&ttt,1);
                        num_mode=ttt&31;//только 5 бит
                        if(ttt&32) RoleStr=" Ретр";//роль Ретранслятор
                   	   	else RoleStr=" База";//роль База
       				   	break;
    //Вибрация (6бит)
    case(OTK3_H_R2):	ttt=0;
                    	pars_bits(dig[AZ_OTK2_H_R2],4,3,(uint16_t*)&ttt,5);
                    	pars_bits(dig[GK2dn_H_R2],4,3,(uint16_t*)&ttt,3);
                    	pars_bits(dig[OTK3_H_R2],4,3,(uint16_t*)&ttt,1);
    					*vib=ttt;
                    	break;

    //Обороты ЗТС
    case(GK1dn_H_R2): ttt=0;
      pars_bits(dig[OTK1_H_R2],4,3,(uint16_t*)&ttt,9);
      pars_bits(dig[GK1_H_R2],4,3,(uint16_t*)&ttt,7);
      pars_bits(dig[GK1up_H_R2],4,3,(uint16_t*)&ttt,5);
      pars_bits(dig[AZ_OTK1_H_R2],4,3,(uint16_t*)&ttt,3);
      pars_bits(dig[GK1dn_H_R2],4,3,(uint16_t*)&ttt,1);
      servis=ttt;
      break;

        //ZUndm
        case(GK4_H_R2): ttt=0;
            pars_bits(dig[ZUndm_H_R2],1,0,(uint16_t*)&ttt,10);
            pars_bits(dig[ZUndm_L_R2],4,0,(uint16_t*)&ttt,8);
            pars_bits(dig[OTK4_H_R2],4,3,(uint16_t*)&ttt,3);
            pars_bits(dig[GK4_H_R2],4,3,(uint16_t*)&ttt,1);
            ZUndm=ttt; ZUndm=ZUndm*0.1;
            break;
        //TEMPndm и качество НДМ
        case(TEMPndm_L_R2): ttt=0;
                pars_bits(dig[DAVL_H_R2],0,0,(uint16_t*)&ttt,7);
                pars_bits(dig[TEMPndm_H_R2],4,0,(uint16_t*)&ttt,6);
                pars_bits(dig[TEMPndm_L_R2],4,3,(uint16_t*)&ttt,1);
                if(ttt&0x0080) ttt=ttt|0xFF00;//если отрицательное
                TEMPndm=ttt; //TEMPndm *= 0.1;
                //качество НДМ
                if(dig[TEMPndm_L_R2]&0x04) QUAL_NDM_STAT=1;
                else QUAL_NDM_STAT=0;
                break;
        //IND
        case(TOK_H_R2): ttt=0;
        		pars_bits(dig[OBORndm_L_R2],1,0,(uint16_t*)&ttt,7);
                pars_bits(dig[INDndm_H_R2],4,0,(uint16_t*)&ttt,5);
                pars_bits(dig[TOK_H_R2],4,4,(uint16_t*)&ttt,0);
                indic=ttt;
                break;
        //dU
        case(GK7up_H_R2): ttt=0;
                      pars_bits(dig[TEMPndm_L_R2],1,0,(uint16_t*)&ttt,12);
                      pars_bits(dig[dU_H_R2],4,0,(uint16_t*)&ttt,10);
                      pars_bits(dig[OTK7_H_R2],4,3,(uint16_t*)&ttt,5);
                      pars_bits(dig[GK7_H_R2],4,3,(uint16_t*)&ttt,3);
                      pars_bits(dig[GK7up_H_R2],4,3,(uint16_t*)&ttt,1);
                      dUndm=ttt;
                      break;
        //OBORndm
        case(OBORndm_L_R2): ttt=0;
            pars_bits(dig[GK4up_H_R2],4,3,(uint16_t*)&ttt,8);
            pars_bits(dig[AZ_OTK4_H_R2],4,3,(uint16_t*)&ttt,6);
            pars_bits(dig[GK4dn_H_R2],4,3,(uint16_t*)&ttt,4);
            pars_bits(dig[OBORndm_L_R2],4,2,(uint16_t*)&ttt,2);
            OBORndm=ttt;
            break;
        //PEAK
        case(PEAK_L_R2): USIL=dig[PEAK_H_R2]&0x10;//15 бит беззнаковый
              PEAKndm=(dig[PEAK_H_R2]&0x0F)*1024+dig[PEAK_M_R2]*32+dig[PEAK_L_R2];
              if(USIL) PEAKndm=PEAKndm*0.001; else PEAKndm=PEAKndm*0.01;
              break;
        //TOK
        case(OTK5_H_R2): ttt=0;
                pars_bits(dig[TOK_H_R2],3,0,(uint16_t*)&ttt,10);
                pars_bits(dig[TOK_L_R2],4,0,(uint16_t*)&ttt,6);
                pars_bits(dig[OTK5_H_R2],4,3,(uint16_t*)&ttt,1);
        		TOKndm=ttt;
                break;
        //NAGR
        case(MOMENT_H_R2):
                        if(((dig[GK5_H_R2]>>4)&1)==1) s1=0xFF00;//Делаем в нагрузке знаковое число
                        else s1=0;
                        s2=0;
                        pars_bits(dig[GK5_H_R2],3,3,(uint16_t*)&s2,7);
                        pars_bits(dig[GK5up_H_R2],4,3,(uint16_t*)&s2,6);
                        pars_bits(dig[AZ_OTK5_H_R2],4,3,(uint16_t*)&s2,4);
                        pars_bits(dig[GK5dn_H_R2],4,3,(uint16_t*)&s2,2);
                        pars_bits(dig[MOMENT_H_R2],4,4,(uint16_t*)&s2,0);
                        s1=s1|s2;
                        NAGRndm=(float)s1*0.1;
                        NAGRndm_null=MyStrToFloat(Fr45->Sn->Cells[1][10]);//Смещение из настройки
                        NAGRndm=NAGRndm-NAGRndm_null;
                        break;
        //MOMENT
        case(GK6up_H_R2):if(((dig[MOMENT_H_R2]>>3)&1)==1) s1=0xFF00;//Делаем в Моменте знаковое число
                        else s1=0;
                        s2=0;
                        pars_bits(dig[MOMENT_H_R2],2,0,(uint16_t*)&s2,7);
                        pars_bits(dig[OTK6_H_R2],4,3,(uint16_t*)&s2,4);
                        pars_bits(dig[GK6_H_R2],4,3,(uint16_t*)&s2,2);
                        pars_bits(dig[GK6up_H_R2],4,4,(uint16_t*)&s2,0);
                        s1=s1|s2;
                        MOMndm=(float)s1*0.1;
                        break;
        //DAVL
        case(DAVL_H_R2):ttt=0;
                        pars_bits(dig[GK6up_H_R2],3,3,(uint16_t*)&ttt,8);
                        pars_bits(dig[AZ_OTK6_H_R2],4,3,(uint16_t*)&ttt,7);
                        pars_bits(dig[GK6dn_H_R2],4,3,(uint16_t*)&ttt,5);
                        pars_bits(dig[DAVL_H_R2],4,1,(uint16_t*)&ttt,3);
                        DAVLndm=ttt;
                        DAVLndm=DAVLndm/9.8692;//Переводим в МПа
                        break;
  }
 }
 //======================================================================================
 //Декодируем все параметры в режиме работы № 12
 if(FORMAT_KADRA==FORMAT_STD)
 {
  switch(kod)
  {
    //АЗ-ОТК (4шт)
    case(AZ_OTK1_L_R12):dat_oa1=inta((float)((dig[AZ_OTK1_H_R12]&0x07)*32+dig[AZ_OTK1_L_R12])*SCALE_OTK); break;
    case(AZ_OTK2_L_R12):dat_oa2=inta((float)((dig[AZ_OTK2_H_R12]&0x07)*32+dig[AZ_OTK2_L_R12])*SCALE_OTK); break;
    case(AZ_OTK3_L_R12):dat_oa3=inta((float)((dig[AZ_OTK3_H_R12]&0x07)*32+dig[AZ_OTK3_L_R12])*SCALE_OTK); break;
    case(AZ_OTK4_L_R12):dat_oa4=inta((float)((dig[AZ_OTK4_H_R12]&0x07)*32+dig[AZ_OTK4_L_R12])*SCALE_OTK); break;
    //ОТК динамический (4шт)
    case(OTK1_L_R12): dat_o1=inta((float)((dig[OTK1_H_R12]&0x07)*32+dig[OTK1_L_R12])*SCALE_OTK); break;
    case(OTK2_L_R12): dat_o2=inta((float)((dig[OTK2_H_R12]&0x07)*32+dig[OTK2_L_R12])*SCALE_OTK); break;
    case(OTK3_L_R12): dat_o3=inta((float)((dig[OTK3_H_R12]&0x07)*32+dig[OTK3_L_R12])*SCALE_OTK); break;
    case(OTK4_L_R12): dat_o4=inta((float)((dig[OTK4_H_R12]&0x07)*32+dig[OTK4_L_R12])*SCALE_OTK); break;
    //ОТК угловой
    case(OTKu_L_R12): otkl=dig[OTKu_H_R12]*32+dig[OTKu_L_R12]; *otk=otkl*0.4; otk_stat=*otk; break;
    //ЗУ
    case(VIBR_H_R12): *zu=(float)dig[ZU_H_R12]*128+(float)dig[ZU_L_R12]*4+(dig[VIBR_H_R12]>>3);*zu=*zu*SCALE_ZU/100.; break;
    //модуль G
    case(GMOD_L_R12): ttt=0;
      ttt=dig[GMOD_H_R12]*32+dig[GMOD_L_R12];
      ttt=ttt<<6; ttt=ttt>>6;//делаем знак, если отрицательное
      *modg=1.+(float)ttt/4096.;//модуль G
      break;
    //Hx
    case(HY_H_R12): ttt=0;
      pars_bits(dig[HX_H_R12],0,0,(uint16_t*)&ttt,11);
      pars_bits(dig[HX_M_R12],4,0,(uint16_t*)&ttt,10);
      pars_bits(dig[HX_L_R12],4,0,(uint16_t*)&ttt,5);
      pars_bits(dig[HY_H_R12],4,4,(uint16_t*)&ttt,0);
      if(ttt&0x0800) ttt=ttt|0xF000;//если отрицательное
      HX=ttt;
      break;
    //Hy
    case(HY_L_R12): ttt=0;
      pars_bits(dig[HY_H_R12],3,0,(uint16_t*)&ttt,11);
      pars_bits(dig[HY_M_R12],4,0,(uint16_t*)&ttt,7);
      pars_bits(dig[HY_L_R12],4,2,(uint16_t*)&ttt,2);
      if(ttt&0x0800) ttt=ttt|0xF000;//если отрицательное
      HY=ttt;
      break;
    //Hz
    case(HZ_L_R12): ttt=0;
      pars_bits(dig[HY_L_R12],1,0,(uint16_t*)&ttt,11);
      pars_bits(dig[HZ_H_R12],4,0,(uint16_t*)&ttt,9);
      pars_bits(dig[HZ_L_R12],4,0,(uint16_t*)&ttt,4);
      if(ttt&0x0800) ttt=ttt|0xF000;//если отрицательное
      HZ=ttt;
      break;
    //Роль и Код режима
    case(AZ_OTK4_H_R12):if(dig[AZ_OTK4_H_R12]&0x10) RoleStr=" Ретр";//роль Ретранслятор
    					else RoleStr=" База";//роль База
                        //Код режима
                        ttt=0;
    					pars_bits(dig[AZ_OTK3_H_R12],4,3,(uint16_t*)&ttt,4);
                        pars_bits(dig[OTK4_H_R12],4,3,(uint16_t*)&ttt,2);
                        pars_bits(dig[AZ_OTK4_H_R12],3,3,(uint16_t*)&ttt,0);
                        num_mode=ttt;
        				break;
    //Вибрация (6бит)
    case(VIBR_L_R12): *vib=(dig[VIBR_H_R12]&0x01)*32+dig[VIBR_L_R12]; break;
    //Обороты ЗТС
    case(OTK3_H_R12): ttt=0;
      pars_bits(dig[OTK1_H_R12],4,3,(uint16_t*)&ttt,9);
      pars_bits(dig[AZ_OTK1_H_R12],4,3,(uint16_t*)&ttt,7);
      pars_bits(dig[OTK2_H_R12],4,3,(uint16_t*)&ttt,5);
      pars_bits(dig[AZ_OTK2_H_R12],4,3,(uint16_t*)&ttt,3);
      pars_bits(dig[OTK3_H_R12],4,3,(uint16_t*)&ttt,1);
      servis=ttt;
      break;
  }
 }
 //======================================================================================
 //Декодируем все параметры в режиме работы № 13
 if(FORMAT_KADRA==FORMAT_STD_RETR)
 {
  switch(kod)
  {
    //АЗ-ОТК (4шт)
    case(AZ_OTK1_L_R13):dat_oa1=inta((float)((dig[AZ_OTK1_H_R13]&0x07)*32+dig[AZ_OTK1_L_R13])*SCALE_OTK); break;
    case(AZ_OTK2_L_R13):dat_oa2=inta((float)((dig[AZ_OTK2_H_R13]&0x07)*32+dig[AZ_OTK2_L_R13])*SCALE_OTK); break;
    case(AZ_OTK3_L_R13):dat_oa3=inta((float)((dig[AZ_OTK3_H_R13]&0x07)*32+dig[AZ_OTK3_L_R13])*SCALE_OTK); break;
    case(AZ_OTK4_L_R13):dat_oa4=inta((float)((dig[AZ_OTK4_H_R13]&0x07)*32+dig[AZ_OTK4_L_R13])*SCALE_OTK); break;
    //ОТК динамический (4шт)
    case(OTK1_L_R13): dat_o1=inta((float)((dig[OTK1_H_R13]&0x07)*32+dig[OTK1_L_R13])*SCALE_OTK); break;
    case(OTK2_L_R13): dat_o2=inta((float)((dig[OTK2_H_R13]&0x07)*32+dig[OTK2_L_R13])*SCALE_OTK); break;
    case(OTK3_L_R13): dat_o3=inta((float)((dig[OTK3_H_R13]&0x07)*32+dig[OTK3_L_R13])*SCALE_OTK); break;
    case(OTK4_L_R13): dat_o4=inta((float)((dig[OTK4_H_R13]&0x07)*32+dig[OTK4_L_R13])*SCALE_OTK); break;
    //ОТК угловой
    case(OTKu_L_R13): otkl=dig[OTKu_H_R13]*32+dig[OTKu_L_R13]; *otk=otkl*0.4; otk_stat=*otk; break;
    //ЗУ
    case(ZU_L_R13): *zu=dig[ZU_H_R13]*32+dig[ZU_L_R13];*zu=*zu/10.; break;
    case(VIBR_H_R13): *zu=(float)dig[ZU_H_R13]*128+(float)dig[ZU_L_R13]*4+(dig[VIBR_H_R13]>>3);*zu=*zu*SCALE_ZU/100.; break;
    //модуль G
    case(GMOD_L_R13): ttt=0;
      ttt=dig[GMOD_H_R13]*32+dig[GMOD_L_R13];
      ttt=ttt<<6; ttt=ttt>>6;//делаем знак, если отрицательное
      *modg=1.+(float)ttt/4096.;//модуль G
      break;
    //Hx
    case(HY_H_R13): ttt=0;
      pars_bits(dig[HX_H_R13],0,0,(uint16_t*)&ttt,11);
      pars_bits(dig[HX_M_R13],4,0,(uint16_t*)&ttt,10);
      pars_bits(dig[HX_L_R13],4,0,(uint16_t*)&ttt,5);
      pars_bits(dig[HY_H_R13],4,4,(uint16_t*)&ttt,0);
      if(ttt&0x0800) ttt=ttt|0xF000;//если отрицательное
      HX=ttt;
      break;
    //Hy
    case(HY_L_R13): ttt=0;
      pars_bits(dig[HY_H_R13],3,0,(uint16_t*)&ttt,11);
      pars_bits(dig[HY_M_R13],4,0,(uint16_t*)&ttt,7);
      pars_bits(dig[HY_L_R13],4,2,(uint16_t*)&ttt,2);
      if(ttt&0x0800) ttt=ttt|0xF000;//если отрицательное
      HY=ttt;
      break;
    //Hz
    case(HZ_L_R13): ttt=0;
      pars_bits(dig[HY_L_R13],1,0,(uint16_t*)&ttt,11);
      pars_bits(dig[HZ_H_R13],4,0,(uint16_t*)&ttt,9);
      pars_bits(dig[HZ_L_R13],4,0,(uint16_t*)&ttt,4);
      if(ttt&0x0800) ttt=ttt|0xF000;//если отрицательное
      HZ=ttt;
      break;
    //Роль и Код режима
    case(AZ_OTK4_H_R13):if(dig[AZ_OTK4_H_R13]&0x10) RoleStr=" Ретр";//роль Ретранслятор
    					else RoleStr=" База";//роль База
                        //Код режима
                        ttt=0;
    					pars_bits(dig[AZ_OTK3_H_R13],4,3,(uint16_t*)&ttt,4);
                        pars_bits(dig[OTK4_H_R13],4,3,(uint16_t*)&ttt,2);
                        pars_bits(dig[AZ_OTK4_H_R13],3,3,(uint16_t*)&ttt,0);
                        num_mode=ttt;
        				break;
    //Вибрация (6бит)
    case(VIBR_L_R13): *vib=(dig[VIBR_H_R13]&0x01)*32+dig[VIBR_L_R13]; break;
    //Обороты ЗТС
    case(OTK3_H_R13): ttt=0;
      pars_bits(dig[OTK1_H_R13],4,3,(uint16_t*)&ttt,9);
      pars_bits(dig[AZ_OTK1_H_R13],4,3,(uint16_t*)&ttt,7);
      pars_bits(dig[OTK2_H_R13],4,3,(uint16_t*)&ttt,5);
      pars_bits(dig[AZ_OTK2_H_R13],4,3,(uint16_t*)&ttt,3);
      pars_bits(dig[OTK3_H_R13],4,3,(uint16_t*)&ttt,1);
      servis=ttt;
      break;
  }
 }
 //======================================================================================
  //Декодируем все параметры в режиме работы № 30
 if(FORMAT_KADRA==FORMAT_GEOMASH)
 {
  switch(kod)
  {
    //Роль и Качество
    case(VIBR_H_R30):
    	if(dig[VIBR_H_R30]&0x10) RoleStr=" Ретр";//роль Ретранслятор
    	else RoleStr=" База";//роль База
        if((dig[VIBR_H_R30]>>3)&0x01) QUAL_STAT=1; else QUAL_STAT=0;//Качество
        break;
    //Код режима
    case(FORMAT_R30):
    	ttt=0;
        pars_bits(dig[FORMAT_R30],4,0,(uint16_t*)&ttt,4);
        num_mode=ttt;
        break;
    //Вибрация (6бит)
    case(VIBR_L_R30): *vib=(dig[VIBR_H_R30]&0x01)*32+dig[VIBR_L_R30]; break;
    //Обороты ЗТС
    case(OBOR_L_R30):
      ttt=0;
      pars_bits(dig[OBOR_H_R30],4,0,(uint16_t*)&ttt,9);
      pars_bits(dig[OBOR_L_R30],4,0,(uint16_t*)&ttt,4);
      servis=ttt;
      break;
	//Счетчик Геомаш
    case(COUNT_L_R30):
    	ttt=0;
        pars_bits(dig[COUNT_H_R30],4,0,(uint16_t*)&ttt,9);
        pars_bits(dig[COUNT_L_R30],4,0,(uint16_t*)&ttt,4);
        Count_Geomash=ttt;
        break;
  }
 }
 //======================================================================================
  //Декодируем все параметры в режиме работы № 31
 if(FORMAT_KADRA==FORMAT_GEOMASH_RETR)
 {
  switch(kod)
  {
    //Роль и Качество
    case(VIBR_H_R31):
    	if(dig[VIBR_H_R31]&0x10) RoleStr=" Ретр";//роль Ретранслятор
    	else RoleStr=" База";//роль База
        if((dig[VIBR_H_R31]>>3)&0x01) QUAL_STAT=1; else QUAL_STAT=0;//Качество
        break;
    //Код режима
    case(FORMAT_R31):
    	ttt=0;
        pars_bits(dig[FORMAT_R31],4,0,(uint16_t*)&ttt,4);
        num_mode=ttt;
        break;
    //Вибрация (6бит)
    case(VIBR_L_R31): *vib=(dig[VIBR_H_R31]&0x01)*32+dig[VIBR_L_R31]; break;
    //Обороты ЗТС
    case(OBOR_L_R31):
      ttt=0;
      pars_bits(dig[OBOR_H_R31],4,0,(uint16_t*)&ttt,9);
      pars_bits(dig[OBOR_L_R31],4,0,(uint16_t*)&ttt,4);
      servis=ttt;
      break;
	//Счетчик Геомаш
    case(COUNT_L_R31):
    	ttt=0;
        pars_bits(dig[COUNT_H_R31],4,0,(uint16_t*)&ttt,9);
        pars_bits(dig[COUNT_L_R31],4,0,(uint16_t*)&ttt,4);
        Count_Geomash=ttt;
        break;
  }
 }
 //======================================================================================
 //новое мастабирование отклонителей
 if(dat_oa1>360) dat_oa1=400;
 if(dat_oa2>360) dat_oa2=400;
 if(dat_oa3>360) dat_oa3=400;
 if(dat_oa4>360) dat_oa4=400;
 if(dat_oa5>360) dat_oa5=400;
 if(dat_oa6>360) dat_oa6=400;
 if(dat_oa7>360) dat_oa7=400;
 if(dat_o1>360) dat_o1=400;
 if(dat_o2>360) dat_o2=400;
 if(dat_o3>360) dat_o3=400;
 if(dat_o4>360) dat_o4=400;
 if(dat_o5>360) dat_o5=400;
 if(dat_o6>360) dat_o6=400;
 if(dat_o7>360) dat_o7=400;

 //Расчет Зенита и Азимутов на нужной цифре.
 //Тут же вытаскиваем признак качества QUAL_STAT
 if(	(FORMAT_KADRA==FORMAT_STD_GK && kod==AZ_OTK4_H_R1)
 		||(FORMAT_KADRA==FORMAT_NDM_GK_RETR &&  kod==AZ_OTK7_H_R2)
        ||(FORMAT_KADRA==FORMAT_GK_RETR && kod==AZ_OTK4_H_R11)
        ||(FORMAT_KADRA==FORMAT_STD && kod==VIBR_H_R12)
        ||(FORMAT_KADRA==FORMAT_STD_RETR && kod==VIBR_H_R13)
   )
 {
    //Умножаем проекции на 2, если пришел соотв. бит в признаках деления
    if(FORMAT_KADRA==FORMAT_STD_GK)
    {
    	if ((dig[HX_H_R1]>>4)&0x01) HX=HX*2;
    	if ((dig[HX_H_R1]>>3)&0x01) HY=HY*2;
    	if ((dig[HX_H_R1]>>2)&0x01) HZ=HZ*2;
    	if ((dig[HX_H_R1]>>1)&0x01) QUAL_STAT=1; else QUAL_STAT=0;//Качество
    }
    if(FORMAT_KADRA==FORMAT_GK_RETR)
    {
    	if ((dig[HX_H_R11]>>4)&0x01) HX=HX*2;
    	if ((dig[HX_H_R11]>>3)&0x01) HY=HY*2;
    	if ((dig[HX_H_R11]>>2)&0x01) HZ=HZ*2;
    	if ((dig[HX_H_R11]>>1)&0x01) QUAL_STAT=1; else QUAL_STAT=0;//Качество
    }
    if(FORMAT_KADRA==FORMAT_NDM_GK_RETR)
    {
    	if ((dig[ZUndm_H_R2]>>4)&0x01) HX=HX*2;
    	if ((dig[ZUndm_H_R2]>>3)&0x01) HY=HY*2;
    	if ((dig[ZUndm_H_R2]>>2)&0x01) HZ=HZ*2;
    	if ((dig[HZ_L_R2]>>1)&0x01) QUAL_STAT=1; else QUAL_STAT=0;//Качество
    }
    if(FORMAT_KADRA==FORMAT_STD)
    {
    	if ((dig[HX_H_R12]>>4)&0x01) HX=HX*2;
    	if ((dig[HX_H_R12]>>3)&0x01) HY=HY*2;
    	if ((dig[HX_H_R12]>>2)&0x01) HZ=HZ*2;
    	if ((dig[HX_H_R12]>>1)&0x01) QUAL_STAT=1; else QUAL_STAT=0;//Качество
    }
    if(FORMAT_KADRA==FORMAT_STD_RETR)
    {
    	if ((dig[HX_H_R13]>>4)&0x01) HX=HX*2;
    	if ((dig[HX_H_R13]>>3)&0x01) HY=HY*2;
    	if ((dig[HX_H_R13]>>2)&0x01) HZ=HZ*2;
    	if ((dig[HX_H_R13]>>1)&0x01) QUAL_STAT=1; else QUAL_STAT=0;//Качество
    }
    //Приводим магнитное поле к наноТесла
    HX=HX*40;
    HY=HY*40;
    HZ=HZ*40;
    Htotal=sqrt((double)HX*HX+(double)HY*HY+(double)HZ*HZ);//модуль H
    HXout=HX; HYout=HY; HZout=HZ;//сохраним для вывода в файл

    //*zu=7; otk_stat=350; *modg=1.;
    float rotk=otk_stat*pi/180., rzu=(*zu)*pi/180., mnr=(*mn)*pi/180., gmod=*modg;
    if(*mn<=0) SaveStatusLog("Задайте магнитное наклонение", clRed);
    //расчитаем составляющие G
    float GX=-sin(rzu)*cos(rotk);
    float GY=sin(rzu)*sin(rotk);
    float GZ=cos(rzu);
    GXout=GX*gmod; GYout=GY*gmod; GZout=GZ*gmod;//сохраним для вывода в файл

    //для АЗ=300, ЗУ=7, ОТК=350, H=54300, УМН=19
    //GX=-0.12f; GY=-0.0211f; GZ=0.9925f;
    //HX=-180; HY=15514; HZ=52036;

    //расчитаем теперь Азимут
    float up=-HX*GY+HY*GX;
    float down=HZ-GZ*(HX*GX+HY*GY+HZ*GZ);
    *az=rotor(up,down);//Азимут сырой

    //коррекция азимута
    float H=Hop;//опорный модуль
    Azk=-9999;//выход расчетов
    try
    {   //Старая коррекция азимута
    	if(Fr45->Nubt->Checked==false && sin(mnr)!=0)
    	{	float n1=((float)HX+sin(rzu)*cos(rotk)*cos(mnr)*H)/sin(mnr);
    		float n2=((float)HY-sin(rzu)*sin(rotk)*cos(mnr)*H)/sin(mnr);
    		float k1=cos(rzu)*cos(rotk);
    		float k2=sin(rotk);
    		float k3=cos(rzu)*sin(rotk);
    		float k4=cos(rotk);
    		float up1=(n1*k3+n2*k1)/(-k1*k4-k2*k3);
    		float down1=(n1*k4-n2*k2)/(k1*k4+k2*k3);
    		Azk=rotor(up1,down1);//Азимут корректированный
    	}
        else if(sin(mnr)==0)
        {SaveStatusLog("Недопустимое значение магнитного наклонения!", clRed);
         PlaySound("MYS1",0,SND_RESOURCE);
         Azk = -9999;
        }
        //Новая упрощенная коррекция азимута
        if(Fr45->Nubt->Checked==true)
        {   double mod=(double)H*H - ((double)HX*HX+(double)HY*HY);//квадрат HZ из опорного модуля
        	if(mod >= 0)
            {	float hz = sqrt(mod);
                float azk1 = resolve_az(HX,HY,hz,rzu,rotk);
                float azk2 = resolve_az(HX,HY,-hz,rzu,rotk);
                float azop = MyStrToFloat(Fr45->Sn->Cells[1][1]);//ожидаемый азимут
                //выберем ближайший к ожидаемому по миним квадрату
                if((azop-azk1)*(azop-azk1) <= (azop-azk2)*(azop-azk2)) Azk = azk1;
                else Azk = azk2;
            }
            else Azk = -9999;//при ошибке кладем отрицательный азимут
        }
        //Истинный азимут сразу посчитаем
        if(Azk!=-9999)
        {
        	Azk_ist = Azk+get_ums();//число
            if(Azk_ist>=360.0) Azk_ist -=360.0;
        	AZist_str = FloatToStrF0(Azk_ist,6,1);//строка
        }
        else {Azk_ist=-9999; AZist_str = "-9999";}
    }
    catch(...){}
 } //Конец расчета азимутов
 //======================================================================================
 //Разложим служебный параметр в зависимости от mode(iot)
 //байт параметра уже распарсен и лежит в *obor
 //Освободившиеся слоты 3 и 6 можно использовать для других целей
 ttt=servis;
 switch(iot)
 {
    case(0): *obor=ttt*20; break;//обороты
    case(1): *temp=(signed char)ttt; break;
    case(2): NAPR=ttt; NAPR=NAPR/10.; break;
    //case(3): *vib=ttt; break;
    case(4): *gk=ttt; *gk=inta(*gk)/10.; break;//ток
    case(5): wr=ttt; break;
    /*case(6): ttt=ttt<<6; ttt=ttt>>6;//делаем знак, если отрицательное
    		 *modg=ttt; *modg=1000.+(float)ttt/4096.*1000; break;//модуль G, mg*/
    case(7): MAHndm=ttt; MAHndm=MAHndm/10; break;
    case(8): sigKKS=ttt; sigKKS=sigKKS/10; break;//сигнал ККС
    case(9): MAHbatt=ttt; /*MAHbatt=MAHbatt/10;*/ break;//потраченная емкость батерей в %
    case(10): MarkerFreq=ttt; break;//маркер переключения частот
 }
 //======================================================================================
}
//=====================================================================================
// Парсинг битов из uint8_t в uint16_t
//=====================================================================================
// парсинг битов из src_8 в ptrnazn_16,
// beg* - номер бита начала битового куска,
// end* - номер конечного бита, слева-направо нумеруются биты = 15-0
void pars_bits(uint8_t src,int8_t begist,int8_t endist,uint16_t *ptrnazn,int8_t begnazn)
{
uint16_t res, mas[6]={0, 0x0001, 0x0003, 0x0007, 0x000f, 0x001f};
uint8_t ist;
int8_t dlina,dvig;
	dlina=1+begist-endist;
	if(dlina>SDVIG || dlina<=0 || begnazn>15) return;
	ist=src;
	dvig=begist-begnazn;//минус=влево, плюс=вправо
	//выделим нужные биты в источнике и задвинем на нужную позицию в назначении
	if(dvig>=0) res=(ist&(mas[dlina]<<endist))>>dvig;//вправо
	else res=(ist&(mas[dlina]<<endist))<<(-dvig);//влево
	//приложим биты в назначение после зачистки
	*ptrnazn=(*ptrnazn & ~(mas[dlina]<<(1+begnazn-dlina))) | res;
}
//=====================================================================================
float resolve_az(float hx, float hy, float hz, float rzu, float rotk)
{	float up = -hx*sin(rotk)-hy*cos(rotk);
	float down = hz*sin(rzu)+cos(rzu)*(hx*cos(rotk)-hy*sin(rotk));
    return rotor(up,down);
}
//=====================================================================================
float scale_gk_log(uint8_t code)
{
float K1=1.5437, K3=2.1957, SF=256, res;
	if(code==0) return 0;
    //res=(exp(K3*code*log(10)/(SF-1))-1)/K1;//хоть так
    res=(pow(10, K3*code/(SF-1))-1)/K1;//хоть эдак
    return res;
}
//=====================================================================================
// Обратная конвертация (NUMBIT-битные группы -> байты)  
uint32_t join_NBbit_arr(const uint8_t* in, uint32_t len, uint8_t* out)
{
uint32_t buf = 0;
uint8_t bits = 0;
uint32_t cnt = 0;
uint32_t i;
    
    if(in==0 || out==0 || len==0) return 0;
	for(i=0; i<len; i++)
	{	if(in[i]==255) return 0;//критическая ошибка, нераспознанный байт
		buf = (buf << 5) | (in[i] & 0x1F);
        bits += 5;
        while(bits >= 8)
		{	uint8_t part = (uint8_t)(buf >> (bits - 8));
			out[cnt++] = part;
            bits -= 8;
            buf &= (1U << bits) - 1;
        }
    }
	// Добавляем остаточные биты как последний байт с ЛЕВЫМ выравниванием
    /*if(bits>0)
	{	// Сдвигаем оставшиеся биты влево, чтобы они заняли старшие позиции байта
        out[cnt++] = (uint8_t)(buf << (8 - bits));
    }*/
    
    return cnt;
}
//==================================================================================







