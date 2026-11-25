//---------------------------------------------------------------------------
#include <math.h>
#include <time.h>
#include <vcl.h>
#include <SysUtils.hpp>
#include <System.hpp>                                         
#include <fstream.h>
#include <iomanip.h>
#include <math.h>
#include <io.h>
#include <fcntl.h>
#include <winbase.h>
#include <process.h>
#include <istream>
#include <registry.hpp>
#include "SysUtils.hpp"
#include "setupapi.h"
#include <mmsystem.h>
#include "my_typedef.h"
#include "TRAEKT2k.h"
#include "yco8_1.h"
#include "LThread.h"
#include "format1_2k.h"
#include "format2_2k.h"
#include "format11_2k.h"
#include "format12_2k.h"
#include "format13_2k.h"
#include "format30_2k.h"
#include "format31_2k.h"
#include "LThread.h"
#include "PBThread.h"
#include "format_kadra.h"
#include <windows.h>
#include <stdlib.h>

#pragma hdrstop
#pragma package(smart_init)
#pragma link "CSPIN"
#pragma link "trayicon"
#pragma resource "*.dfm"
#pragma resource "prov.res"

extern bool QUAL_STAT, QUAL_NDM_STAT;

TFr45 *Fr45;
sic4 *Thread;
LThread *Thr=NULL;
PBThread *PBThr=NULL;
void __fastcall PutLeuza(AnsiString STR);
Graphics::TBitmap *BM = new Graphics::TBitmap;
Graphics::TBitmap *BM1 = new Graphics::TBitmap;
TRect T,Tm,Tn,Tc,Tckw;
#include "WitsCodesMWD.h"
AnsiString ver="2k_062", ver2="", ZagolForInk, ZagolForStatic;
#define NASTR_FILE_NAME	"nastr2k_0.dat"
AnsiString HELP_FILE="help.txt";
AnsiString TITLE="ПРОВОДКА ";
#define DlinaParam  10 //символьная длина места под параметр в тексте
//Начало заголовка LAS
AnsiString LasZagolBegin[]=
{
"~VERSION INFORMATION BLOCK",
"VERS.                             2.0: CWLS log ASCII Standard -VERSION 2.0",
"WRAP.                              NO: ONE LINE PER DEPTH STEP",
"~WELL INFORMATION BLOCK",
"STRT.M                               : START DEPTH",
"STOP.M                               : STOP DEPTH",
"STEP.M                               : STEP",
"NULL.                       -9999.000: NULL VALUE",
"COMP.                         COMPANY: ",//Заказчик
"WELL.                            WELL: ",//Скважина
"FLD .                           FIELD: ",//Месторождение
"CNTY.                          COUNTY: ",
"STAT.                           STATE: RUSSIA",
"SRVC.                 SERVICE COMPANY: НПФ 'Горизонт'",//Подрядчик
"~CURVE INFORMATION BLOCK"
};
//Массив для заголовков - MNEM.UNIT(для LAS), DESCRIPTION(для LAS), Название(для ink)
AnsiString Zag_global[][3]=
{
{"DEPTH.M",			": DEPTH",									"Глубина"},
{"ZENIT_ZTS.grad",	": Zenit, inclinometr",						"Зенит"},
{"AZrw_ZTS.grad",	": Azimut raw, inclinometr",				"АЗсырой"},
{"AZmag_ZTS.grad",	": Azimut korr-mag, inclinometr",			"АЗ.магн"},
{"AZist_ZTS.grad",	": Azimut korr-geograf, inclinometr",		"АЗ.ист"},
{"Hmod_ZTS.nTesla",	": Module H, inclinometr",					"МодульH"},
{"OTK_ZTS.grad",	": Otklonitel gravitation, inclinometr",	"ОТКграв"},
{"OTK2_ZTS.grad",	": Otklonitel magnitny, inclinometr",		"ОТКмагн"},
{"OBOR_ZTS.ob/min",	": Oboroty generatora, inclinometr",		"Обороты"},
{"TEMP_ZTS.grad",	": Temperatura, inclinometr",				"Темпер."},
{"NAPR_ZTS.V",		": Volt, inclinometr",						"Напр."},
{"VIBR_ZTS.G",		": Vibration, inclinometr",					"Виб."},
{"GK_ZTS.mcR/h",	": Gamma, inclinometr",						"ГК"},
{"ZENIT_NDM.grad",	": Zenit, NDM",								"ЗУндм"},
{"GK_UP_NDM.mcR/h",	": Gamma UP, NDM",							"ГКверх"},
{"GK_DN_NDM.mcR/h",	": Gamma DOWN, NDM",						"ГКниз"},
{"TEMP_NDM.grad",	": Temperatura, NDM",						"Темп"},
{"OBOR_NDM.ob/min",	": Oboroty doloto, NDM",					"Обор"},
{"F_NDM.Tonn",		": Nagruzka, NDM",							"Нагр,кН"},
{"TOK_NDM.mA",		": Servise, NDM",							"Ток"},
{"PEAK_NDM.mV",		": Servise, NDM",							"mV"},
{"KS_NDM.om*m",		": Servise, NDM",							"КС"},
{"IND_NDM.pc",		": Servise, NDM",							"Индик"},
{"DU_NDM.V",		": Servise, NDM",							"dU"},
{"MOM_NDM.kN",		": Moment, NDM",							"Мом,кН"},
{"DAVL_NDM.MPa",	": Davlenie, NDM",							"Давл,МПа"},
{"MAH_NDM.A*h",		": Servise, NDM",							"A*ч"},
{"KKS_ZTS.mV",		": Servise, ZTS",							"ККС,mV"},
{"ROP.m/h",			": Rate Of Penitration",					"Дата/Время"},
{"Gt_ZTS.mg",		": Gtotal ZTS",								"Gt_zts,mg"},
};
//Конец заголовка LAS
AnsiString LasZagolEnd[]=
{
"~Parameter information block",
"# Created by",
"# MNEM.UNIT                    VALUE   DESCRIPTION",
"# ====.==============================:====================",
"~ASCII Log Data"
};

//запрещенные символы в имени файла
char ForbiddenSymbols[]={'\\', '/', ':', '"', '<', '>', '|', '?', '*', '.', ','};

//Адреса параметров Пульта Бурильщика win*8+pole
#define     wMOM    0   //win=0; pole=0
#define     wOBOR   1   //win=0; pole=1
#define     wDAVL   2   //win=0; pole=2
#define     wNAGR   3   //win=0; pole=3
#define     wAZ     40  //win=5; pole=0
#define     wZU     41  //win=5; pole=1
#define     wOTK    42  //win=5; pole=2

std::queue <qStruc> Qpb;//очередь для потока ПБ
CRITICAL_SECTION cs;//имя критической секции
AnsiString IDskv;//идектификатор скважины из wits
int jjjj=0, SockNumber=0, SourcItem, CountConnects, FlagInterval=0, PBrotation=0, PBposition;
uint8_t masQuality[rowCount], masQualNDM[rowCountNDM];
float gkzts, zu_inc;
float level_NUBT=70.0;
int ur=31;
bool uco32, Corr_check=0, PnastrChange=0, SvCheck, PbCheck, BusyPB, PotokON, PotokOFF, PotokPBON;
bool START=0;
float Ctan[4097];
int ioo=1,ioss=1,ioo2=1;
int ctat=0;
bool PODChecked=true;
bool server=false;
bool USIL=0;
bool StatusPlayer=false;
bool StatusRecorder=false;
bool StatusPause=false;
float NAPR,zz0;
int indic, TOKndm;
float ZUndm, nap1,Kgeo, GKup, GKdn, NAGRndm, OBORndm, PEAKndm, TEMPndm, KCndm, dUndm, MAHndm, NAGRndm_null;
float MOMndm, DAVLndm, Kgk_ndm, GKndm_int, Kgk_zts, MAHbatt;
float zu1;
AnsiString RoleStr="";
int num_pause_vectors=0, MarkerFreq=0;
//инитим массивы с номерами начала векторных пауз, потом они изменяются при старте
int mas_pause_format2opor[NUM_PAUSE_R2]={PAUZA1_R2,PAUZA2_R2,PAUZA3_R2,PAUZA4_R2,PAUZA5_R2,PAUZA6_R2,PAUZA7_R2};//начало пауз для режима 2
int mas_pause_format11opor[NUM_PAUSE_R11]={PAUZA1_R11,PAUZA2_R11,PAUZA3_R11,PAUZA4_R11};//начало пауз для режима 11
int mas_pause_format13opor[NUM_PAUSE_R13]={PAUZA1_R13,PAUZA2_R13,PAUZA3_R13,PAUZA4_R13};//начало пауз для режима 13
int mas_pause_format31opor[NUM_PAUSE_R31]={PAUZA1_R31};//начало пауз для режима 6
int mas_pause_format2[NUM_PAUSE_R2];
int mas_pause_format11[NUM_PAUSE_R11];
int mas_pause_format13[NUM_PAUSE_R13];
int mas_pause_format31[NUM_PAUSE_R31];
int FORMAT_KADRA=0;
int Ncod=0;//номер кодировки
int Count_Geomash=0;
//векторные паузы от частоты прибора
#define PAUSE_FREQ16		(3)
#define PAUSE_FREQ10		(2)
#define PAUSE_FREQ7			(2)
//на остальных частотах PAUSE=1

//FILE *SKV;
   //int newr=0,newr0=0;
 int BASEACC=4096;
 int NUSO=0,cek,cek0;
 // TDateTime cek=0;
float TUSO=0;
TColor C[]={clRed,clYellow,clRed,clYellow,clRed,clYellow,clRed,clYellow,clRed,clYellow,clRed,clYellow,clRed,clYellow,clRed,clYellow,clRed,clYellow};
int iccc=0,ippp=0;  int dd[5]={0,2,4,5,6};
TColor cform=clWhite,C2start=clWhite,C2end=clWhite,C3start=clWhite,C3end=clWhite,Ctstart=clWhite,Ctend=clWhite;
int C2_gr=0,C2_dr=0,C3_gr=0,C3_dr=0,Ct_gr=0,Ct_dr=0;
SDAT Sdat;
float *kih;
int M1_1=25629,M1_2=3205,M1_5=6409,M1_10=1605,M1_1_8=10681,M1_3=6409,M1_7=3205;//длины фильтров по-умолчанию
int DL=4096,ks=5,DS=DL/2+1,ipik=0,II[3];
//float MasC[20480],B[2049];
float MasC[20480*2],B[2049*2];
float df,kss,xmax,sr;
FILE *FileSignal=NULL, *FileSignal2=NULL;
AnsiString SPIK=" ";
int it=0,itm=0,W,IND=0,ind0=-1,t0=2,ctr=0,X0=0,Y0=0,Xt=0,Yt=0,iotk=-1,iotkm=-1;
int IT,IL,IH,IW,ID=0; int xc0,yc0,xc1,yc1,yc00;
float yt,hxt,hxt0=0.125;
float nap=6.8;
int nport=0, nport_geo=1000, nport_wits=10000, Port_Leuza=65004, port_gti=4151;
int MCP=-1,DLKADR,NGT=-1,MGT=-1;
int com=1, prib=3, DCP=60, iG=2, sdvig=0, DCPt=0,
NGO=-1,MCP0=-1,DLKADRn,indt0,PIK,inpik,npik=5,PIK5[5];
float ampl, PCP=-1;
 double H;
double MIN=0,MAX=100;
float newot[5],newotm[5];
   double pi=M_PI;
float  xkm=0, KmB=2.5,dp,dpp,dcpm,dcpl;
float   TDISCUSOr,AZVOROTA=10,ZUVOROTA=3,OSNFREQ=1.25,TDISCUSO;
int DLMAS,DLDIG,Nfiltr=2,DLKADR0,DELTAr,DELTA,DELTA0=256,NK=-1;
double  DLKADRf, DLMASf,DLDIGf;
float TDISCUSO_200=0.0040;
double TKADR5=201.326592; //Опорное время кадра при 1.25 Гц
//static float FRE[7]={10.,5.,2.5,1.25,7.,3.6,1.8};
//int DELTAi[7]={25,50,100,200,35,71,142 };
//static float FFRE[7]={1.,0.5,0.25,0.125,0.7071068,0.353553,0.176776};
//static float pw[7]={8,4,2,1,5.6568424949,2.828427124745,1.41421356237};
static float pw[7]={8*1.6,8,5.6568424949,4,2.828427124745,2,1.41421356237};
static float FRE[7]={16.,10.,7.,5.,3.6,2.5,1.8};
static float FFRE[7]={1.6,1.,0.7071068,0.5,0.353553,0.25,0.176776};
int DELTAi[7]={16,25,35,50,71,100,142 };

int NUMBER_KOD;
#define PAUSE_SEC   (0) //Пауза между кадрами в секундах

#define FAZA4 1      //объявим 4-х фазную модуляцию
#include "VEC_NUMS.h"//кол-во векторов кода (без инверсии)
float xkd[VEC_NUMS];
int mas_num[]={0,DLINA_CODA_R1,DLINA_CODA_R2,0,0,0,0,0,0,0,0,DLINA_CODA_R11,DLINA_CODA_R12,DLINA_CODA_R13,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,DLINA_CODA_R30,DLINA_CODA_R31};//Длины кодов от форматов
int DLSP=128;
volatile int DLPOLr=13;


double TKADR;
int L,Dmas;

unsigned char num_mode;
unsigned char dig[300], data_perc[300];// проценты достоверности данных, с запасом
int dproc[300], dg[300]; // проценты достоверности данных, с запасом
//unsigned char *dig, *data_perc;
//int *dproc, *dg;
int DLD=32;
int DLK;
volatile int DLMASr,DLDIGr;
volatile int DLKADRr,DLKADRrp;

int r_az;
float zu,mn,az_otk;
int dat_o1, dat_o2,dat_o3,dat_o4,dat_oa1,dat_oa2,dat_oa3,dat_oa4,HX,HY,HZ,Htotal;
int dat_o5, dat_o6,dat_o7,dat_o8,dat_oa5,dat_oa6,dat_oa7,dat_oa8;
float modg,Azk,sigKKS,Azk_ist=-9999;
int tok;
//unsigned int CountKadrWits07=0, CountKadrWits08=0, CountKadrWits09=0;
unsigned int CountKadrWits=0;

float *korr,*mas1,*masc,*mas;
int DLBUF;
float *mast;// основной буфер сигнала
float *masf;
TColor c1=clLime,c2=clYellow;
TColor c1m=clLime,c2m=clYellow;
int ity=-1,napr=1,naprm=1,ius=1;
int volatile iprov=0;
AnsiString CoreDir,tpathckw,dtime,skvtxt="по умолчанию";
volatile bool fl_prov=true;
volatile bool fl_Th=false;
volatile bool kor_freq=false;
volatile bool fl_buf=false;
volatile bool fl_fil=false;
volatile bool Socket1_ON=false;
volatile bool WitsServer_ON=false;
volatile bool WitsClient_ON=false;
volatile bool GtiClient_ON=false;
 int dst=0;
   int m1;
HANDLE port,portPB; // {дескриптор COM-порта}
AnsiString  Fdat=" ",NCOM,NCOMh,NCOMPB,NCOMPBh;
AnsiString tar,date0,time0,ckwName,Fink,fckwt,Flas,Fstat,Fin,Fmagcorr,AZist_str="",Fhelp,Fstatus,Fnastr,Fsur,Fgeotrack,Fpasport,Fraw4,Fraw5;
TDateTime T0,D0;
unsigned long kolb;
int isavedan=0;//код наличия файла данных
int numbyte=0,i2=0,i3=0,ipos=100;
float glubina=0, glubina_last=0, glub_Dol=0, glub_Zab;
float DZ=0.1;
int Hop=54300;
int xosc=0,ytek=0,xtek=0,hx=1,ox,oy,rad,om,radm;
float mashvs,mashvst;//масштаб вывода сигнала
//int  zarez;
AnsiString ST;
float xt;
int freq=0;
int mrmt=0;//"угол между прибором и турбоб.";
int azorien=90;//"азимут (ориентировочно)";
float zuorien=60;//"зенит (ориентировочно)";
int otkmin=0,otkminm=0;//"минимальный угол отклонения";
int otkmax=0,otkmaxm=0;//"максимальный угол отклонения";
int porogsp=30;//"порог достоверности СП ,%";
int nulotk=0; // нуль отклонителя
int RedC=0;
int wr;
float otklonf,gk,mgk=1;
int maxgk=26;
float azimut, Azk_stat; // азимутный угол
int   iot, d1,d2,
      //otklon, otklon1,otklon2,otklon3,otklon4,otklon5,otklon6,otklon7,// угол отклонителя
      otktek,// ОТК текущий гравитационный
      obor, // обороты генератора
      tem, // темпер
      vib,
      levelsig, // уровень исходного сигнала
      faza, // фаза сигнала  =1 - нормальный сигнал,  =-1 - инверсный сигнал
      oldfaza, //      ampl=0,      kodz=0,
      ind=0,indl=0;
float zenit, zenit_stat;// зенитный угол
int MODE_AZ = 0;//режим вывода Азимута: 0=магнитный, 1=истинный

 int Nz,Na,Nat[10];
 float grzu[10]={10.,35.,65.,85.,0,0,0,0,0,0};
 float grzu0[4]={10.,35.,65.,85.},zuf[50];
static float filpar[4]={0.375,0.03125,0.875,1.75};
static float filpar1[4];
float usi[10][100],tx[10][100],kspline[10][3][100];
static int minind,maxind,aznum,kolaz,azsr,zunum,kolzu,zusr;
int podstava,dspec=256;
float spec[256], // спектр сигнала
      frint; // переменная для фильтрации
int kls=1,lsp, // переменные для фильтрации
    rpcht=0; // смещение фазы относительно основной частоты

//щас коды передаются и принимаются слева направо
//4 фазные коды из 2фазных (5 на 3, а на с), влево
#if FAZA4
unsigned long digkod[VEC_NUMS];
unsigned long Gcodes[2][VEC_NUMS]=
{ 
{ 0x33339999L, 0x33cc9966L, 0x363c6369L, 0x36c36396L, 0x39336c66L, 0x39cc6c99L, 0x3c3c9696L, 0x3cc39669L,
  0x6369363cL, 0x639636c3L, 0x6666ccccL, 0x6699cc33L, 0x6969c3c3L, 0x6996c33cL, 0x6c663933L, 0x6c9939ccL
},
{ 0xa95a9a69L, 0x565a6569L, 0x59aa6a99L, 0xa6aa9599L, 0x659656a5L, 0x9a96a9a5L, 0x9566a655L, 0x6a665955L,
  0x5556999aL, 0x55a99965L, 0x5a599695L, 0x5aa6966aL, 0x6665aaa9L, 0x669aaa56L, 0x696aa5a6L, 0x6995a559L
}
};
unsigned long spkod[4];
unsigned long Gsp[2][4] =
{
{0xa5995a55L, 0x955aaa66L, 0x6969a9a6L, 0x596a5669L},
{0x9955a956L, 0x595a6966L, 0x99a9aa5aL, 0x56665aa9L}
};

#else
unsigned long digkod[VEC_NUMS]=
{0xa95a9a69L,0x565a6569L,0x59aa6a99L,0xa6aa9599L,0x659656a5L,0x9a96a9a5L,0x9566a655L,0x6a665955L
};
unsigned long spkod[4] = {0xaaa55655L, 0xa5665aa6L, 0x56a59669L, 0xa9a5a669L};
#endif

//-----ini
 bool skv_file=false;
 int pskv[VEC_NUMS];
//-----ini
   float sp[32];
   void kordat4_8(int,int,float *,float *,float *,int *,int *,int *,int *,int *,int* ,float* ,int*,
   float *,float *,float *,float *,int *, int *);
   void BPF(bool,float *,int DL);
   void filtr10();
   void filtr5();
   void filtr2();
   void filtr1();
    void filtr7();
   void filtr3();
   void filtr1_8();
   double  MyStrToFloat(AnsiString s);
   AnsiString FloatToStrF0(double x,int n,int m);
   void FillNetLevel(LPNETRESOURCE, TStrings *);
    FILE *dat;

   HINSTANCE dllInstance ;
   typedef void (__import *FType (int , float *, float *, float *));
   FType * FloatMulAdd32no16=NULL;
   typedef void (__import *FType1 (int *));
   FType1 * CheckSSE=NULL;
   void SaveLog(AnsiString s);
   void SaveWitsLog(AnsiString s);
   void write_to_file_inc(void);
   //void write_to_file_dat(void);
   //void write_json_dat(AnsiString key, AnsiString val);
   void write_tabl_dat(AnsiString key, AnsiString val);
   void write_to_file_las(void);
   void write_to_file_in(void);
   void write_to_file_in_rev4(void);
   void write_to_file_in_rev5(void);
   void add_string_to_memo(TMemo *memo, AnsiString glub);
   AnsiString get_string_for_stat(AnsiString glub);
   void add_string_to_MemoUgol(void);
   void ShowPanelStat(int top, int left);
   void ShowPanelInterval(int top, int left);
   AnsiString PadLeft(AnsiString input, unsigned int len);
   AnsiString PadRight(AnsiString input, unsigned int len);
   float roundf(float x, unsigned int n);
   int round(float x);
   bool FileExists(const char *fname);
   AnsiString CreateTopicWits(AnsiString zap, float glub, float glub_bit);
   void SendStaticToWits(void);
   void SendOtkToWits(AnsiString otk_g, AnsiString otk_h);
   void SendGkZtsToWits(void);
   void SendGkNdmToWits(void);
   double interpol(double begin, double end, double ratio);
   double averaging(double one, double two, int *ptr);
   bool IsGlubGeo(void);
   bool IsGlubLeusa(void);
   void N_USO2(AnsiString w,int *ssn);
   HANDLE initPortPB(AnsiString NUM);
   void initDisplayPB(void);
   //void SendToPB(unsigned char adr, float val);
   void SendOneQpb(unsigned char adr, float val);
   void SendOneQpb_rfd(unsigned short adr, float val);
   void SendByteQpb_rfd(uint16_t adr, uint8_t val);
   void SendCommQpb_rfd(uint16_t adr);
   void clearPB(void);
   void setDisplayPB(void);
   int port_list_cmd(void);
   int port_list_reg(void);
   void Send09record(AnsiString kod, AnsiString param);
   void GetDateTime(uint8_t *mas);
   void DrawOTK1(float otk);
   void DrawOTK2(float otk);
   int solv_format_kadra(void);
   float get_ums(void);
   float get_umn(void);
   float get_Hop(void);
   double get_Gt_opor(void);
   float get_mkonec_zts(void);
   float get_mkonec_gkzts(void);
   float get_mkonec_ndm(void);
   void LoadToMemoStatFromFile(void);
   void LoadToMemoInkFromFile(AnsiString file);
   uint16_t swap16(uint16_t x);
   uint32_t swap32(uint32_t x);
   float swapFloat(float x);
   uint64_t swapDouble(uint64_t x);
   void sendLogParam(const char* ptr);
   void SaveStatusLog(AnsiString str, TColor color);
   void SaveNastrLog(AnsiString File);
   AnsiString getNastrString(void);
   AnsiString getZagolInk(void);
   AnsiString getZagolDat(void);
   int GetPlayerSpeed(void);
   AnsiString get_StringFromUTime(time_t time);
   AnsiString get_DateFromUTime(time_t time);
   AnsiString get_TimeFromUTime(time_t time);
   time_t get_UTimeFromString(AnsiString str);
   time_t getUnixTimeSec(void);
   long long getUnixTimeMs(void);
   uint32_t getTimeShiftGeomash(uint32_t FORMAT, uint32_t freqnum);
   void SavePasportFile(AnsiString File);
   void LoadPasportFile(AnsiString File);
   uint16_t crc16_simple (uint8_t *ptr, uint16_t dlina);
   uint32_t crc32_simple (uint8_t *ptr, uint16_t dlina);
   uint32_t join_NBbit_arr(const uint8_t* in, uint32_t len, uint8_t* out);
   uint32_t crc32_poly (uint8_t* in, uint32_t len);
   void SendToWitsSocket(uint8_t* buf, uint32_t len);
   //void send_test(void);
//--------------------
int inta(double a)
{int i=(int)a;
if(a-i>0.5)i=i+1;
return i;
}
//-----------------------------
#include "myUTime.h"
//-----------------------------
float rotor(float up,float down)
 {if(fabs(up)>fabs(down)){
float aa=fabs(down*BASEACC/up );
 int a=inta(aa);  if(a>4096)a=4096;
 if(up<0&&down<0) return 270-Ctan[a];
  if(up>=0&&down<0)   return 90+Ctan[a];
  if(up<0&&down>=0)  return 270+Ctan[a];
  if(up>=0&&down>=0)  return 90-Ctan[a];
                     }
 if(fabs(up)<=fabs(down)){float aa=fabs(up*BASEACC/down );
  int a=inta(aa);  if(a>4096)a=4096;
 if(up<0&&down<0)return 180+Ctan[a];
  if(up>=0&&down<0)return 180-Ctan[a];
  if(up<0&&down>=0)return 360-Ctan[a];
  if(up>=0&&down>=0)return Ctan[a];
                     }

  }
//-------------------------------------------------
void Mul(int m,int iv1,float *v1,float *v2, int I1,int N,float * res)
{	int m1,m2,j,k,iv;
  	float r,r1;
	if(I1+m<=N)
    {	if(iv1==0)FloatMulAdd32no16((__int16)m,v1,&v2[I1],&r);
    	else FloatMulAdd32no16((__int16)m,&v2[I1],&v2[I1],&r);
    }
  	else
    {	m1=((N-I1)/4)*4;
		if(m1>0)
        {	if(iv1==0) FloatMulAdd32no16((__int16)m1,v1,&v2[I1],&r);
            else FloatMulAdd32no16((__int16)m,&v2[I1],&v2[I1],&r);
        }
		else r=0;
        j=I1+m1;
        m2=m-m1;
        {
		for(k=0;k<4;k++)
        {	if(j==N)j=0;
			if(iv1==0)r=r+v1[m1+k]*v2[j];
			else   r=r+v2[j]*v2[j];
            j++;}
            m2=m2-4;
        }
        if(m2>0)
        {	if(j==N)j=0;
        	if(iv1==0)FloatMulAdd32no16((__int16)m2,&v1[m-m2],&v2[j],&r1);
            else FloatMulAdd32no16((__int16)m2,&v2[j],&v2[j],&r1);
            r=r+r1;
        }
    }
       *res=r;
}
 //--------------------------------------
AnsiString FloatToStrF0(double x,int n,int m)
{
 AnsiString s= FloatToStrF(x,ffFixed,n,m);
 s=StringReplace(s,",",".",TReplaceFlags()<<rfIgnoreCase);
 return s;
}
 //--------------------------------------
AnsiString FloatToStr0(float x)
{AnsiString s= FloatToStr(x);
s=StringReplace(s,",",".",TReplaceFlags()<<rfIgnoreCase);
return s;}

//--------------------------------------
double MyStrToFloat(AnsiString s)
{
s=Trim(s);
if(CompareText(s,"")==NULL)return 0;
s=StringReplace(s,",",DecimalSeparator,TReplaceFlags()<<rfIgnoreCase);
s=StringReplace(s,".",DecimalSeparator,TReplaceFlags()<<rfIgnoreCase);
return StrToFloat(s);
}
//---------------------------------------------------------------------------
int StrToIntdeff(AnsiString s,int I)
{AnsiString ss="0123456789";
for(int i=1;i<=StrLen(s.c_str());i++)if(!IsDelimiter(ss,s,i))return I;

if(CompareText(Trim(s),"")!=NULL)return StrToInt(s);
else return I;}
//-------------------------------
int deli(AnsiString m, int n,int * r)
{ int j=0;int e1=1,e0=0;
AnsiString ss="0123456789,";
for(int i=1;i<=StrLen(m.c_str());i++)if(!IsDelimiter(ss,m,i))return e1;
 j= AnsiPos(",",m);
if(j==0)return e1;
AnsiString m1=m.SubString(1,j-1);
int rr=StrToIntdeff(m1,100);
 if(rr!=100&&rr<38)r[0]=rr; else return e1 ;
AnsiString m2=m.SubString(j+1,m.Length());
if(n==2){rr=StrToIntdeff(m2,100);
 if(rr!=100&&rr<38)r[1]=rr;  else  return e1;  }
 if(n==3){  j= AnsiPos(",",m2);
if(j==0)return e1;
AnsiString m3=m2.SubString(1,j-1);
int rr=StrToIntdeff(m3,100);
 if(rr!=100&&rr<38)r[1]=rr;  else  return e1;
AnsiString m4=m2.SubString(j+1,m2.Length());
rr=StrToIntdeff(m4,100);
 if(rr!=100&&rr<38)r[2]=rr;  else  return e1;
  }
 return e0;

}

//-----------------

int StrToIntdef(AnsiString s,int I)
{AnsiString ss="-+0123456789";
for(int i=1;i<=StrLen(s.c_str());i++)if(!IsDelimiter(ss,s,i))return I;

if(CompareText(Trim(s),"")!=NULL)return StrToInt(s);
else return I;}
//-----------------
//---------------------------------------------------------------------------
//до прорисовки формы
__fastcall TFr45::TFr45(TComponent* Owner) : TForm(Owner)
{
int i=1, hran, hran2, hran3, hran4, hran5, hran6;
float mkonec_zts,mkonec_gkzts,mkonec_gkndm,ums;
double Gt_opor;

DecimalSeparator = '.';//принудительно меняем на точку
Application->UpdateFormatSettings = false;//запретим обновление форматов из системы
CoreDir=GetCurrentDir();//корневая директория
tpathckw=CoreDir+"\\tek";//папка скважины по-умолчанию
ckwName="test";
if(IsPathDelimiter(tpathckw,tpathckw.Length()))
T0=Now();  time0=TimeToStr(T0);       D0=Date();
SaveWitsLog("Начало проводки *************************************\r\n");
ZagolForInk=getZagolInk();//Получим заголовок для ink файла

ZagolForStatic=PadRight("Глуб.(м)",DlinaParam) + PadRight("ЗУзтс",DlinaParam) + PadRight("ЗУндм",DlinaParam)
  + PadRight("АЗсырой",DlinaParam) + PadRight("АЗ.магн",DlinaParam) + PadRight("АЗ.ист",DlinaParam)
  + PadRight("ОТКстат",DlinaParam)+ PadRight("Gx",DlinaParam) + PadRight("Gy",DlinaParam) + PadRight("Gz",DlinaParam)
  + PadRight("Hx(nT)",DlinaParam) + PadRight("Hy(nT)",DlinaParam) + PadRight("Hz(nT)",DlinaParam)
  + PadRight("Href(nT)",DlinaParam) + PadRight("Gref",DlinaParam) + PadRight("UTime",DlinaParam*2)
  + PadRight("Gt,mg",DlinaParam) + PadRight("RefGt",DlinaParam) + PadRight("RefDip",DlinaParam) + PadRight("RefNC",DlinaParam);

ZagolForStatic=ZagolForStatic.Trim();//Убираем пробелы с обоих концов

 //счетчик строк в колонках угловых параметров один для всех
 L1->RowCount=rowCount;
 L1a->RowCount=rowCount;
 Lakopp->RowCount=rowCount;
 Lh->RowCount=rowCount;
 L0->RowCount=rowCount;

 L1->Cells[0][0]="Зенит";
 ZU2->Cells[0][0]="ZU-Inc";
 Lo2->Cells[0][0]="Отк2";

 ifstream ifile(NASTR_FILE_NAME);//ищем в корне файл nastr.dat
 if(ifile!=NULL)//если файл найден, читаем настройки
 {    	char ctar[500];  AnsiString mm="";
        //читаем первуя строку с файлом скважины
      	ifile.get(ctar,500); ifile.get();
      	mm=Trim(AnsiString(ctar));
      	AnsiString dir=ExtractFileDir(mm);//выдергиваем папку скважины
      	if(CompareText("",dir)!=0) tpathckw=dir;//сохраняем папку скважины
		ckwName=ExtractFileName(mm);//выдергиваем имя файла скважины

        //читаем вторую строку с именем
      	mm="";
      	ifile.get(ctar,500); ifile.get();
      	mm=Trim(AnsiString(ctar));
      	tar=ExtractFileName(mm);//выдергиваем имя датчика

        //читаем третью строку неизвестно для чего
      	ifile.get(ctar,500); ifile.get();
        mm=Trim(AnsiString(ctar));
      	skvtxt=ExtractFileName(mm);//по умолчанию

        //УСО
      	ifile.get(ctar,500);ifile.get();
      	NCOM=Trim(AnsiString(ctar));
      	if(CompareStr(NCOM,"-")==0) NCOM="";
      	Sport->Text=NCOM;
      	if(NCOM.Length()>=4)
      	{
       		AnsiString SNUSO="";
       		if(NCOM.Length()>=5) for(int i=4;i<=5;i++)  if(IsDelimiter("0123456789",NCOM[i],1)) SNUSO=SNUSO+NCOM[i];
       		if(SNUSO != "") NUSO=StrToInt(SNUSO); else NUSO=0;
      	}
		ifile>>i; //просто 0
		ifile>>i; Epausa->Text=IntToStr(i);
		ifile>>prib>>com>>glubina>>DZ>>zuorien>>azorien>>mn>>Hop>>nap>>mgk>>otkmin>>otkmax>>otkminm>>otkmaxm;
		ifile>>mrmt;
		ifile>>freq;
		int zar;
		ifile>>zar;
		ifile>>DCP>>ius>>ipos>>i; u32->Checked=i;   uco32=u32->Checked;

		if(!ifile.eof()) {int ccf; ifile>>ccf; if(ccf!=ipos)(int)cform=ccf;}
		if(!ifile.eof()) ifile>>(int)C2start>>(int)C2end>>C2_gr>>C2_dr;
		if(!ifile.eof())  ifile>>(int)C3start>>(int)C3end>>C3_gr>>C3_dr;
		if(!ifile.eof())    ifile>>(int)Ctstart>>(int)Ctend>>Ct_gr>>Ct_dr;
		if(!ifile.eof())    ifile>>(int)c1>>(int)c2>>(int)c1m>>(int)c2m;
   		ifile>>i; Caz_otk->Checked=i;
   		ifile>>i;  SvCheck=i;//Sv->Checked=i;
   		ifile>>i;  hran=i;//Sv2->Checked=i;
   		ifile>>ioo;
   		ifile>>ioss;
   		ifile>>ioo2;
   		ifile>>i; Caz_otk->Checked=i;
   		ifile>>nport;
   		ifile>>i; hran2=i;
   		ifile>>nulotk; //НульОТК из тарировки
   		ifile>>Kgeo;//Геометрический фактор
   		ifile>>NAGRndm_null;//Смещение нуля нагрузки НДМ
   		ifile>>Kgk_ndm;//Коэфф интегрального ГКндм
   		ifile>>nport_geo;//Номер порта Геосенсора
   		ifile>>mkonec_zts;//Мертвый конец до ЗТС
   		ifile>>mkonec_gkzts;//Мертвый конец до датчика ГКзтс
   		ifile>>mkonec_gkndm;//Мертвый конец до датчика ГКндм
   		ifile>>i;  SourcItem=i;//Выбор источника глубины
   		ifile>>PbCheck;//Пульт бурильщика

   		ifile.get();
   		ifile.get(ctar,500);
   		if(CompareText("-",Trim(AnsiString(ctar)))!=0) Czis->Text=Trim(AnsiString(ctar));
   		else Czis->Text="";

   		if(!ifile.eof()) {ifile.get(); ifile.get(ctar,500);ifile.get(); TUSO=MyStrToFloat(Trim(AnsiString(ctar)));}

   		ifile.get(ctar,500);
   		if(CompareText("-",Trim(AnsiString(ctar)))!=0 ) Cleusa->Text=Trim(AnsiString(ctar));
   		else Cleusa->Text="";

   		ifile.get();
   		ifile.get(ctar,500);
   		if(CompareText("-",Trim(AnsiString(ctar)))!=0 ) NCOMPB=AnsiString(ctar);
   		else NCOMPB="";
   		i=LastDelimiter("(",NCOMPB);//найдем индекс скобки
   		if(i>0) NCOMPBh=Trim(NCOMPB.SubString(1,i-1));//обрежем до скобки и уберем пробелы
   		else  NCOMPBh=NCOMPB;

   		if(!ifile.eof()) {ifile>>i; hran3=i;}//Cl2->Checked=i;
   		if(!ifile.eof()) {ifile>>Port_Leuza;}//порт Леузовской глубины
   		if(!ifile.eof()) {ifile>>hran4;}//выбор ОТК на пульт Бурильщика
   		if(!ifile.eof()) {ifile>>i; if(i) CBndm->Checked=true; else CBndm->Checked=false;}//НДМ
   		if(!ifile.eof()) {ifile>>i; if(i) CBnnk->Checked=true; else CBnnk->Checked=false;}//ННК
   		if(!ifile.eof()) {ifile>>i; if(i) CBretr->Checked=true; else CBretr->Checked=false;}//Ретр
   		if(!ifile.eof()) {ifile>>Kgk_zts;}//коэффициент ГКзтс
   		if(!ifile.eof()) {ifile>>i; if(i) CBgkzts->Checked=true; else CBgkzts->Checked=false;}//ГКзтс
   		if(!ifile.eof()) {ifile>>i; RGcoder->ItemIndex=i;}//номер кодировки
   		if(!ifile.eof()) {ifile>>port_gti;}//порт глубины от ГТИ wits
   		//IP ГТИ
   		if(!ifile.eof())
   		{	ifile.get();
    		ifile.get(ctar,500);
    		if(CompareText("-",Trim(AnsiString(ctar)))!=0 ) Cgti->Text=Trim(AnsiString(ctar));
    		else Cgti->Text="";
   		}
   		//IP WITS клиент
   		if(!ifile.eof())
   		{	ifile.get();
    		ifile.get(ctar,500);
    		if(CompareText("-",Trim(AnsiString(ctar)))!=0 ) Cwits->Text=Trim(AnsiString(ctar));
    		else Cwits->Text="";
   		}

   		if(!ifile.eof()) {ifile>>i; Nubt->Checked=0;}//НУБТ всегда 0 при открытии
   		if(!ifile.eof()) {ifile>>ums;}//Склонение
   		if(!ifile.eof()) {ifile>>hran5;}//LOG-сервер
        if(!ifile.eof()) {ifile>>Gt_opor;}//Gt из паспорта прибора
        if(!ifile.eof()) {ifile.get();ifile.get(ctar,500);Cname->Text = Trim(AnsiString(ctar));}//Имя телесистемы
        if(!ifile.eof()) {ifile>>hran6;}//выбор типа ПБ - наш/не наш
        if(!ifile.eof()) {ifile>>PBrotation;}//счетчик вращений ПБ
        if(!ifile.eof()) {ifile>>i; if(i) CBgeomash->Checked=true; else CBgeomash->Checked=false;}//Геомаш


   		ifile.close();

        //если выбран НДМ, то принудительно ставим ГК и запрещаем изменение ГК и Ретр
    	if(CBndm->Checked==true)
    	{CBgkzts->Checked = true;
     	 CBgkzts->Enabled = false;
     	 CBretr->Enabled = false;
         CBgeomash->Enabled = false;
    	}
    	else
    	{CBgkzts->Enabled = true;
     	 CBretr->Enabled = true;
         CBgeomash->Enabled = true;
    	}
        //CBndmClick(Sender);//эмулируем выбор галочки
 }
 else
 {	//tpathckw=CoreDir;//по умолчанию остается
 	ckwName="default";//первая строка
    tar="датчик";//вторая строка
    skvtxt="по умолчанию";//3 строка
    NCOM="";//4 строка
    Epausa->Text="6";
    prib=3; com=1; glubina=0; DZ=0.2; zuorien=90; azorien=100; mn=71; Hop=54000; nap=5.9; mgk=1; otkmin=0; otkmax=360; otkminm=0; otkmaxm=360;
	mrmt=0;freq=1;DCP=60;ius=0;ipos=18056;u32->Checked=0;(int)cform=16777215;
    (int)C2start=16777215;(int)C2end=16777215;C2_gr=C2_dr=0;
    (int)C3start=16777215;(int)C3end=16777215;C3_gr=C3_dr=0;
    (int)Ctstart=16777215;(int)Ctend=16777215;Ct_gr=Ct_dr=0;
    (int)c1=8453888;(int)c2=16744703;(int)c1m=8453888;(int)c2m=16744703;
	Caz_otk->Checked=0;SvCheck=0;
    nport=27911;Kgeo=1;Kgk_ndm=0.5;nport_geo=134;mkonec_gkndm=0;SourcItem=2;
    TUSO=0.004;
    RGcoder->ItemIndex=0;
    Kgk_zts=1;Gt_opor=1;
    hran=hran2=hran3=hran4=hran5=0;
 }
 //проверяем папку скважины
 if(!DirectoryExists(tpathckw))
 {	if(tpathckw==CoreDir+"\\tek") CreateDirectory(tpathckw.c_str(),NULL);//папка по-умолчанию
 	else
    {ShowMessage("Папка для скважины "+tpathckw+" отсутствует на диске, ее необходимо создать и указать в Настройках");
     tpathckw = CoreDir+"\\tek";
     CreateDirectory(tpathckw.c_str(),NULL);//папка по-умолчанию
    }
 }

 skv_file=false;
 if(CompareText(skvtxt,"по умолчанию"))  skv_file=false;

 //номер прибора
 Cdat->Text=tar;

TSearchRec sr;
AnsiString ex;

SetCurrentDir(tpathckw);
tpathckw=GetCurrentDir();
Ckw->Items->Clear();
if(FindFirst("*.*",faAnyFile,sr)==0)
{
	ex= ExtractFileExt(sr.Name);
	if(sr.Attr!=faDirectory&&sr.Name!="."&&sr.Name!="..")
	if(CompareText(ex,".ink")==0) Ckw->Items->Add(Trim(sr.Name));
	while(FindNext(sr)==0)
	{
    	ex= ExtractFileExt(sr.Name);
    	if(sr.Attr != faDirectory && sr.Name != "." && sr.Name!="..")
    	if(CompareText(ex,".ink")==0) Ckw->Items->Add(ChangeFileExt(sr.Name,""));
	}
}
 FindClose(sr);
 if( Ckw->Items->Count==0) Ckw->Items->Add(ckwName);
 Ckw->ItemIndex=0;
 Ckw->Text=ckwName;

Edz->Text=FloatToStrF0(DZ,5,1);
for(int i=0;i<Ckw->Items->Count;i++) if(CompareStr(ckwName,Ckw->Items->Strings[i])==0)
Ckw->ItemIndex=i;
if(Ckw->ItemIndex==-1) Ckw->Items->Add(ckwName);
Ckw->Text=ckwName;
Cuss->ItemIndex=ius;
Fink=tpathckw+"\\"+ckwName+".ink";
Flas=tpathckw+"\\"+ckwName+".las";
Fstat=tpathckw+"\\"+ckwName+".stat";
Fin=tpathckw+"\\"+ckwName+".in";
Fraw5=tpathckw+"\\"+"raw5.in";
Fraw4=tpathckw+"\\"+"raw4.in";
Fsur=tpathckw+"\\"+ckwName+".sur";
Fgeotrack=tpathckw+"\\"+ckwName+"_geotrack.txt";
Fstatus=tpathckw+"\\status.log";//в папке скважины
Fnastr=tpathckw+"\\nastr.log";//в папке скважины
Fpasport=tpathckw+"\\pasport.txt";//в папке скважины
Fmagcorr=tpathckw+"\\"+"magcorr3.in";
Fhelp=CoreDir+"\\"+HELP_FILE;//корень программы
Eglubina->Text=FloatToStrF0(glubina,7,2);
Gll->Caption="забой "+Eglubina->Text;

AnsiString Sf[7]={"16","10","7","5","3","2.5","1.8"};
if(prib==3){
 R4->Items->Clear();
 for(int i=0;i<7;i++)  R4->Items->Add(Sf[i]);
 R4->Columns=2;
 R4->ItemIndex=freq;
}
L1a->Cells[0][0]="Аз.сыр";
L0->Cells[0][0]="Отк1";
Label7->Visible=false;//Верхняя полоса Углы-Отклонители
Lh->Cells[0][0]="       H";
Lh->Cells[0][1]=IntToStr(Hop);
if(MODE_AZ==0) Lakopp->Cells[0][0]="Аз.маг";
else Lakopp->Cells[0][0]="Аз.ист";
Cmin->Value=otkmin;Cmax->Value=otkmax;

L1->Row=L1->RowCount-1;   L1->LeftCol=0;     L1->TopRow=2;
ZU2->Row=ZU2->RowCount-1;   ZU2->LeftCol=0;     ZU2->TopRow=2;
L1a->Row=L1->RowCount-1;   L1a->LeftCol=0;     L1a->TopRow=2;
Lakopp->Row=Lakopp->RowCount-1;   Lakopp->LeftCol=0;     Lakopp->TopRow=2;
Lh->Row=Lh->RowCount-1;   Lh->LeftCol=0;     Lh->TopRow=2;
L0->Row=L0->RowCount-1;   L0->LeftCol=0;     L0->TopRow=2;
Lo2->Row=Lo2->RowCount-1;   Lo2->LeftCol=0;     Lo2->TopRow=2;
Spar->Row=Spar->RowCount-1;  Spar->LeftCol=0;     Spar->TopRow=2;
Lo2->Row=Lo2->RowCount-1;  Lo2->LeftCol=0;     Lo2->TopRow=2;

//   описание  Pnastr меню настройка
Sn->Cells[0][0]="угол между прибором и турбобуром";
Sn->Cells[0][1]="азимут ориентировочно (погр=10)";
Sn->Cells[0][2]="зенит ориентировочно (погр=3)";
Sn->Cells[0][3]="порог достоверности СП, %";
Sn->Cells[0][4]="магнитное наклонение";
Sn->Cells[0][5]="модуль Ноп, nT";
Sn->Cells[0][6]="порог напряжения, В";
Sn->Cells[0][7]="номер порта для WITS";
Sn->Cells[0][8]="нульОТК из паспорта прибора";
Sn->Cells[0][9]="геометрический фактор, Kзонд";
Sn->Cells[0][10]="смещение нуля нагрузки НДМ, т";
Sn->Cells[0][11]="коэффициент интегрального ГКндм";
Sn->Cells[0][12]="номер порта Геосенсора";
Sn->Cells[0][13]="мертвый конец ЗТС, м";
Sn->Cells[0][14]="мертвый конец ГКзтс, м";
Sn->Cells[0][15]="мертвый конец НДМ, м";
Sn->Cells[0][16]="номер порта Леузы";
Sn->Cells[0][17]="коэффициент ГКзтс";
Sn->Cells[0][18]="номер порта ГТИ(wits)";
Sn->Cells[0][19]="магнитное склонение";
Sn->Cells[0][20]="Gt из паспорта прибора, м/с^2";
Sn->Cells[1][0]=IntToStr(mrmt);
Sn->Cells[1][1]=IntToStr(azorien);
Sn->Cells[1][2]=FloatToStrF0(zuorien,7,1);
Sn->Cells[1][3]=IntToStr(DCP);
Sn->Cells[1][4]=FloatToStrF0(mn,7,2);
Sn->Cells[1][5]=IntToStr(Hop);
Sn->Cells[1][6]=FloatToStrF0(nap,7,1);
Sn->Cells[1][7]=IntToStr((int)nport);
Sn->Cells[1][8]=IntToStr((int)nulotk);
Sn->Cells[1][9]=FloatToStrF0(Kgeo,7,3);
Sn->Cells[1][10]=FloatToStrF0(NAGRndm_null,6,1);
Sn->Cells[1][11]=FloatToStrF0(Kgk_ndm,7,3);
Sn->Cells[1][12]=IntToStr((int)nport_geo);
Sn->Cells[1][13]=FloatToStrF0(mkonec_zts,7,2);
Sn->Cells[1][14]=FloatToStrF0(mkonec_gkzts,7,2);
Sn->Cells[1][15]=FloatToStrF0(mkonec_gkndm,7,2);
Sn->Cells[1][16]=IntToStr((int)Port_Leuza);
Sn->Cells[1][17]=FloatToStrF0(Kgk_zts,7,3);
Sn->Cells[1][18]=IntToStr((int)port_gti);
Sn->Cells[1][19]=FloatToStrF0(ums,7,2);
Sn->Cells[1][20]=FloatToStrF0(Gt_opor,7,6);//double
double pi=M_PI;
for (int i=0;i<4097;i++)
{
Ctan[i]=atan(i/4096.)*180/pi;
Ctan[i]=inta(Ctan[i]*100.);
Ctan[i]=Ctan[i]/100.;
}
TObject *Sender;
//При изменении значения Checked вызывается событие OnCheck
//Для инициализации значения Checked нужно на это время отключить событие
Sv->OnClick=NULL;//отключаем событие
PB->OnClick=NULL;
RadioGroup1->OnClick=NULL;
Sv->Checked=SvCheck;//изменяем значение
PB->Checked=PbCheck;
RadioGroup1->ItemIndex=hran2;
Sv->OnClick=SvClick;//включаем событие
PB->OnClick=PBClick;
RadioGroup1->OnClick=RadioGroup1Click;
SourceGlub->ItemIndex=SourcItem;//устанавливаем источник глубины
PBRadioGroup->ItemIndex=hran4;//какой ОТК передавать на ПБ
PBType->ItemIndex=hran6;//тип ПБ
PBClick(Sender);//если пульт бурильщика
if(SvCheck) SvClick(Sender);//если Внешняя глубина

PanelStat->Visible=false;
WriteStat->Enabled=true;

LoadPasportFile(Fpasport);//прочитаем паспорт скважины

//------Запустим сервер или Клиент------------------------------------
Sv2->OnClick=NULL;//отключаем пока реакцию
Sv2->Checked=hran;//ранее запомненное состояние
Sv2->OnClick=Sv2Click;//подключаем реакцию
if(Sv2->Checked) {Sv2Click(Sender); Button22Click(Sender);}//зажигаем панельку и включаем сразу

Cl2->Checked=hran3;//ранее запомненное состояние WITS клиент
if(Cl2->Checked) {Cl2Click(Sender);}//зажигаем панельку

CBLog->OnClick=NULL;
CBLog->Checked=hran5;//ранее запомненное состояние LOG-сервер
CBLog->OnClick=CBLogClick;
if(CBLog->Checked) {CBLogClick(Sender); Button8Click(Sender);}

//----------Проверим наличие правильного файла help
FILE *out1;
if((out1=fopen(Fhelp.c_str(),"r"))==NULL) {ShowMessage("Отсутствует файл "+Fhelp);}

//начальная скорость проигрывания плеера
PlayerSpeedButton->Tag=1;
PlayerSpeedButton->Caption = "x"+IntToStr(GetPlayerSpeed());//покажем скорость на кнопке

//инициализация механизма критической секции
InitializeCriticalSection( &cs );
}
//---------------------------------------------------------------------------------------------------------------
void __fastcall TFr45::N3Click(TObject *Sender)
{ 	ifstream F(Fhelp.c_str());
    if(!F) ShowMessage("Не удается открыть файл "+Fhelp);
  	else
  	{	AnsiString str;
        char line[1024];
        while(F.getline(line, sizeof(line))) str += String(line)+"\r\n";
        ShowMessage(str);
  	}
}
//---------------------------------------------------------------------------------------------------------------
//--------------СТАРТ проводки-------------------------------------------------------------
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
void __fastcall TFr45::startClick(TObject *Sender)
{
 START=1;
 DCPt=0;
 if(RadioGroup1->ItemIndex==0) Edz->Enabled=false;//если глубина ВНЕШНЯЯ, то гасим добавок
 //чистим ПБ, если выбран наш пульт
 if(PB->Checked && PBType->ItemIndex == 0) {clearPB();}
 //if(PB->Checked) SendOneQpb(wZU, 201);
 FORMAT_KADRA=solv_format_kadra();//вычисляем формат кадра
 Ncod=RGcoder->ItemIndex;//номер кодировки
 if(Ncod>1 || Ncod<0) {ShowMessage("Критическая ошибка в кодировке!"); stopClick(Sender); return;}
 //загрузим массивы кодировок
 for(int i=0;i<VEC_NUMS;i++) digkod[i]=Gcodes[Ncod][i];//вектора данных
 for(int i=0;i<4;i++) spkod[i]=Gsp[Ncod][i];//вектора СП

 //выключаем органы настроек
 R4->Enabled=false;
 Sport-> Enabled=false;
 //R5->Enabled=false;
 CBretr->Enabled=false; CBndm->Enabled=false; CBnnk->Enabled=false; CBgkzts->Enabled=false; CBgeomash->Enabled=false;
 u32->Enabled=false;
 TAR-> Enabled=false;
 Ckw->Enabled=false;
 Cdat->Enabled=false;
 RGcoder->Enabled=false;
 Label2->Enabled=false;
 Sport2->Enabled=false;

 if(iprov==0)
 {
  mn=90.0-get_umn();
  if(mn<1||mn>89) ShowMessage("Не задано магнитное наклонение");
  Hop=StrToInt(Sn->Cells[1][5]);
  Lh->Cells[0][1]=IntToStr(Hop);
  ckwName=Ckw->Text;
  if(CompareText(ckwName,"")==0) {ShowMessage("Задайте скважину"); stop->Down=true;return;}
  if(CompareText(ckwName,fckwt)!=0)
  {
   fckwt=ckwName;
   if(CompareText(ckwName,"")==0) {ShowMessage("Задайте скважину"); stop->Down=true;return;}
   CkwChange(Sender);//загружаем все файлы с новым именем
  }
  if(CompareText(NCOM,"")==0)
  {
   ShowMessage("Задайте порт связи");
   stop->Down=true;
   return;
  }
  if(iprov==0)
  {
   if(CompareText(Sn->Cells[1][0],"")!=0)mrmt=StrToInt(Sn->Cells[1][0]);
   else mrmt=0;
   StatusBar->SimpleText=date0;
   T2->Position=T2->Min;
   ctr=T2->Position-T2->Min;
   Series3->Clear();   Series2->Clear();
   Label7->Color=Panel7->Color;//цвет метки статики
   Label7->Caption="СТАТИКА!";

//---------------
//на этом этапе уже известны Частота freq и FORMAT_KADRA
TDISCUSO=TDISCUSO_200;//0.004сек
//Номер СОМ-порта
if(NCOM.Length()>=4)
      {
       AnsiString SNUSOt="";
       int NUSOt=0;
       if(NCOM.Length()>=5) for(int i=4;i<=5;i++)  if(IsDelimiter("0123456789",NCOM[i],1)) SNUSOt=SNUSOt+NCOM[i];
       if(SNUSOt != "") NUSOt=StrToInt(SNUSOt); else NUSOt=0;
       if(NUSOt==NUSO&&fabs(TUSO-TDISCUSO)<0.000013) TDISCUSO=TUSO;
      }
NUMBER_KOD=mas_num[FORMAT_KADRA];//По режиму работы выбираем кол-во векторов по умолчанию
//в зависимости от режима и частоты добавляем паузные вектора
if(freq==0) num_pause_vectors=PAUSE_FREQ16;//векторов на 1 паузу, 16Гц
else if(freq==1) num_pause_vectors=PAUSE_FREQ10;//векторов на 1 паузу, 10Гц
else if(freq==2) num_pause_vectors=PAUSE_FREQ7;//векторов на 1 паузу, 7Гц
else num_pause_vectors=1;//все остальные частоты
if(FORMAT_KADRA==FORMAT_GK_RETR) num_pause_vectors=1;//на любой частоте одиночная пауза
if(FORMAT_KADRA==FORMAT_STD_RETR) num_pause_vectors=1;//на любой частоте одиночная пауза
if(FORMAT_KADRA==FORMAT_STD_GK) num_pause_vectors=0;//в 1-м режиме пауз нет (ГК)
if(FORMAT_KADRA==FORMAT_STD) num_pause_vectors=0;//в 12-м режиме пауз нет (без ГК)
if(FORMAT_KADRA==FORMAT_GEOMASH) num_pause_vectors=0;//в 5-м режиме пауз нет
if(FORMAT_KADRA==FORMAT_GEOMASH_RETR) num_pause_vectors=1;//в 6-м режиме одиночная пауза
//переводим массивы начала пауз в абсолютную индексацию
if(FORMAT_KADRA==FORMAT_NDM_GK_RETR) {for(int i=0;i<NUM_PAUSE_R2;i++) {mas_pause_format2[i] = mas_pause_format2opor[i]+(num_pause_vectors-1)*i;}}
if(FORMAT_KADRA==FORMAT_GK_RETR) {for(int i=0;i<NUM_PAUSE_R11;i++) {mas_pause_format11[i] = mas_pause_format11opor[i]+(num_pause_vectors-1)*i;}}
if(FORMAT_KADRA==FORMAT_STD_RETR) {for(int i=0;i<NUM_PAUSE_R13;i++) {mas_pause_format13[i] = mas_pause_format13opor[i]+(num_pause_vectors-1)*i;}}
if(FORMAT_KADRA==FORMAT_GEOMASH_RETR) {for(int i=0;i<NUM_PAUSE_R31;i++) {mas_pause_format31[i] = mas_pause_format31opor[i]+(num_pause_vectors-1)*i;}}

//увеличим длину кадра на дополнительные вектора пауз
if(FORMAT_KADRA==FORMAT_NDM_GK_RETR) NUMBER_KOD += (NUM_PAUSE_R2*num_pause_vectors)-NUM_PAUSE_R2;
if(FORMAT_KADRA==FORMAT_GK_RETR) NUMBER_KOD += (NUM_PAUSE_R11*num_pause_vectors)-NUM_PAUSE_R11;
if(FORMAT_KADRA==FORMAT_STD_RETR) NUMBER_KOD += (NUM_PAUSE_R13*num_pause_vectors)-NUM_PAUSE_R13;
if(FORMAT_KADRA==FORMAT_GEOMASH_RETR) NUMBER_KOD += (NUM_PAUSE_R31*num_pause_vectors)-NUM_PAUSE_R31;
L=NUMBER_KOD;

TKADR=(128+32*NUMBER_KOD)*TKADR5 / (128+32*12);//Новое опорное время кадра через отношение (1.25Гц)
hxt=hxt0*pw[freq];//pw[7]={8,4,2,1,5.6568424949,2.828427124745,1.41421356237}  hxt0=0.125
DLKADRf=(TKADR/pw[freq])/TDISCUSO;//Приведенная длина кадра в реальных тиках УСО (434.110464/4/0.004=27131.904)
double DLKADRfp=DLKADRf;//в тиках УСО прибавляем все паузы
DLKADR=inta(DLKADRfp);//Целых тиков УСО на кадр
DLKADR0=DLKADR;
DLK=DLSP+L*DLD;//длина кадра в битах (256+61*32=2208)
DLMASr=DLSP*DLPOLr;//Длина СП в тиках УСО (DLPOLr=13)(256*13=728)
DLDIGr=32*DLPOLr;  //Длина кодов в тиках УСО (32*13=416)
DLKADRr=DLMASr+L*DLDIGr;//Длина всего кадра в абстрактных тиках УСО (728+61*416=26104)
DELTA=DELTAi[freq];//дельта от частоты для чего-то
DLMASf=(double)DLKADRf*DLSP/DLK;
DLMAS=DLMASf;
if(DLMASf-DLMAS>=0.5)DLMAS=DLMAS+1;
DLKADRrp=DLKADRr;//Длина всего кадра в тиках УСО
DLDIGf=(double)(DLKADRf-DLMAS)/L;
DLDIG=DLDIGf;
if(DLDIGf-DLDIG>=0.5)DLDIG=DLDIG+1;
DLBUF=2*(DLKADR+DELTA);
/*ShowMessage("Длина korr="+IntToStr(DLKADRrp+DLMASr));
ShowMessage("Длина mas1="+IntToStr(DLKADRrp+2*DLMASr));
ShowMessage("Длина masc="+IntToStr(2*DLKADRrp));
ShowMessage("Длина mas="+IntToStr(DLKADRrp));
ShowMessage("Длина mast="+IntToStr(DLBUF));
ShowMessage("Длина masf="+IntToStr(DLBUF));*/

 delete korr; korr=NULL;
 korr = new float[DLKADRrp+DLMASr];//60736
 if(korr==NULL){ShowMessage("Не хватает памяти");stop->Down=true;return;}

 delete mas1; mas1=NULL;
 mas1 = new float[DLKADRrp+2*DLMASr];//62400
 if(mas1==NULL){ShowMessage("Не хватает памяти");stop->Down=true;return;}

 delete masc; masc=NULL;
 masc = new float[2*DLKADRrp];//118144
 if(masc==NULL){ShowMessage("Не хватает памяти");stop->Down=true;return;}

 delete mas; mas=NULL;
 mas = new float[DLKADRrp];//59072
 if(mas==NULL){ShowMessage("Не хватает памяти");stop->Down=true;return;}

 delete mast; mast=NULL;
 mast = new float[DLBUF];//69828
 if(mast==NULL){ShowMessage("Не хватает памяти");stop->Down=true;return;}

 delete masf; masf=NULL;
 masf = new float[DLBUF];//69828
 if(masf==NULL){ShowMessage("Не хватает памяти");stop->Down=true;return;}

 /*delete dig; dig=NULL;
 dig = new unsigned char[NUMBER_KOD];
 if(dig==NULL){ShowMessage("Не хватает памяти");stop->Down=true;return;}

 delete data_perc; data_perc=NULL;
 data_perc = new unsigned char[NUMBER_KOD];
 if(data_perc==NULL){ShowMessage("Не хватает памяти");stop->Down=true;return;}

 delete data_perc; data_perc=NULL;
 data_perc = new unsigned char[NUMBER_KOD];
 if(data_perc==NULL){ShowMessage("Не хватает памяти");stop->Down=true;return;}

 delete dproc; dproc=NULL;
 dproc = new int[NUMBER_KOD];
 if(dproc==NULL){ShowMessage("Не хватает памяти");stop->Down=true;return;}

 delete dg; dg=NULL;
 dg = new int[NUMBER_KOD];
 if(dg==NULL){ShowMessage("Не хватает памяти");stop->Down=true;return;}*/

//----------маска--------
StatusBar->Color=clRed;
StatusBar->SimpleText="ЖДИТЕ!  Идет подготовка СП и Фильтров";
int i,j,k;
float spkod1[128]; j=0;
#if FAZA4
    for(k=0;k<4;k++) for(i=0;i<32;i++) {spkod1[j]=(int)-pow(-1,((spkod[k]>>(31-i))&1)); j++;}
#else
    for(k=0;k<4;k++) for(i=0;i<32;i++) {spkod1[j]=(int)-pow(-1,((spkod[3-k]>>i)&1)); j++;}
#endif
j=0;  //  ShowMessage ((float)DLMASr/(float)DLSP);
for(i=0;i<DLMASr;i++)
{
 int ii= (float)DLMASr/(float)DLSP*(j+1);
 if((float)DLMASr/(float)DLSP*(j+1)-ii>0.5) ii=ii+1;
 if(i==ii) j=j+1;
 if(j>=DLSP)j=DLSP-1;
 mas[i]=spkod1[j];
}
//---------------------коды-------------
 //for(k=0;k<8;k++)
for(k=0;k<VEC_NUMS;k++)
{
//переворачиваем коды с заду наперед, преобразуя по-ходу в +1, -1
//СП уже сидит правильно
#if FAZA4
  //тут код переворачивается сзаду наперед
  //for(int j=0;j<32;j++) sp[j]=(int)-pow(-1,((digkod[k]>>(j))&1));
  //тут загружается напрямую
  for(int j=0;j<32;j++) sp[j]=(int)-pow(-1,((digkod[k]>>(31-j))&1));
#else
  for(int j=0;j<32;j++) sp[j]=(int)-pow(-1,((digkod[k]>>(31-j))&1));
#endif
  j=0;
  for(int i=0;i<DLDIGr;i++)
   {int ii=(float)DLDIGr/(float)DLD*(j+1);
    if((float)DLDIGr/(float)DLD*(j+1)-ii>0.5) ii=ii+1;
    if(i==ii) j=j+1;
    mas[DLMASr+k*DLDIGr+i]=sp[j];//длина СП + k кодов
   }
}
 //--------------фильтрация маски и кодов---------------
m1=0;
//---------------KIH фильтры---------------------
{
SetCurrentDir(CoreDir);  AnsiString name; float rk;
#if FAZA4
//AnsiString name0[]= {"KIH10_4F.dat","KIH5_4F.dat","KIH2_4F.dat","KIH2_4F.dat","KIH7_4F.dat","KIH3_4F.dat","KIH2_4F.dat" };
AnsiString name0[]= {"KIH16_4F.dat","KIH10_4F.dat","KIH7_4F.dat","KIH5_4F.dat","KIH3_4F.dat","KIH2_4F.dat","KIH2_4F.dat" };
#else
AnsiString name0[]= {"KIH10.dat","KIH5.dat","KIH2.dat","KIH1.dat","KIH7.dat","KIH3.dat","KIH1_8.dat" };
#endif
name=name0[freq];
ifstream fa(name.c_str());
if(!fa)//если файл не найден, то фильтры по-умолчанию
{
 if(freq==0)//16
 {
  m1=M1_10;
  free(kih);
  kih=(float*)calloc(m1+4,sizeof(float));
  if(kih==NULL) {ShowMessage("Не хватает памяти");stop->Down=true;return;}
  filtr10();
 }
 if(freq==1)//10
 {
  m1=M1_10;
  free(kih);
  kih=(float*)calloc(m1+4,sizeof(float));
  if(kih==NULL) {ShowMessage("Не хватает памяти");stop->Down=true;return;}
  filtr10();
 }
 if(freq==2)//7
 {
  m1=M1_7;
  free(kih);
  kih=(float*)calloc(m1+4,sizeof(float));
  if(kih==NULL){ShowMessage("Не хватает памяти");stop->Down=true;return;}
  filtr7();
 }
 if(freq==3)//5
 {
  m1=M1_5;
  free(kih);
  kih=(float*)calloc(m1+4,sizeof(float));
  if(kih==NULL) {ShowMessage("Не хватает памяти");stop->Down=true;return;}
  filtr5();
 }
 if(freq==4)//3
 {
  m1=M1_3;
  free(kih);
  kih=(float*)calloc(m1+4,sizeof(float));
  if(kih==NULL) {ShowMessage("Не хватает памяти");stop->Down=true;return;}
  filtr3();
 }
 if(freq==5)
 {
  m1=M1_3;
  free(kih);
  kih=(float*)calloc(m1+4,sizeof(float));
  if(kih==NULL) {ShowMessage("Не хватает памяти");stop->Down=true;return;}
  filtr3();
 }
 if(freq==6)
 {
  m1=M1_3;
  free(kih);
  kih=(float*)calloc(m1+4,sizeof(float));
  if(kih==NULL) {ShowMessage("Не хватает памяти");stop->Down=true;return;}
  filtr1_8();
 }
}
else //если файл KIH найден
{
 m1=0;
 while(!fa.eof()) {fa>>rk; m1=m1+1;}
 m1=m1-1;
 fa.close();
 free(kih);
 kih=(float*)calloc(m1+4,sizeof(float));
 if(kih==NULL) {ShowMessage("Не хватает памяти");stop->Down=true;return;}
 ifstream fa(name.c_str());
 for (int i=0;i<m1;i++)  fa>>kih[i];
 fa.close();
}
int m1_4=(m1/4+1)*4;
for(int i=m1;i<m1_4;i++) kih[i]=0;
m1=m1_4;
}   //Nflltr==2

//----------------mas1[DLKADRr]-----------
{
for(int i=0;i<DLMASr+VEC_NUMS*DLDIGr;i++) mas1[i]=mas[i];
float *vrem,*vremr;
int dl=3*DLMASf+VEC_NUMS*DLDIGf;
if(3*DLMASf+VEC_NUMS*DLDIGf-dl>0.5)  dl=dl+1;
int dlr=3*DLMASr+VEC_NUMS*DLDIGr;
vrem=(float*)calloc(dl+10,sizeof(float));
if(vrem==NULL) {ShowMessage("Не хватает памяти");stop->Down=true;return;}
vremr=(float*)calloc(dlr+10,sizeof(float));
if(vremr==NULL) {ShowMessage("Не хватает памяти");stop->Down=true;return;}
for (int i=0;i<DLMASr;i++)  vremr[i]=mas[i];
for (int i=0;i<DLMASr+VEC_NUMS*DLDIGr;i++)  vremr[i+DLMASr]=mas[i];
for (int i=0;i<DLMASr;i++)  vremr[i+VEC_NUMS*DLDIGr+2*DLMASr]=mas[i];
float x,h=(float)DLKADRrp/DLKADRfp;
int k;
for (int i=0;i<dl;i++)
{
 x=h*i;
 k=(int)x;
 vrem[i]=1000*(vremr[k]+(vremr[k+1]-vremr[k])*(x-k));
}

int j;
if(dst!=0)
{
 for(int num=0;num<dl;num++)
 {
  j=num-m1; if(j<0)j=j+dl;
  Mul(m1,0,kih,vrem,j,dl, &masf[num]);
 }
}  //  dst!=0
else
{
 for(int num=0;num<dl;num++)
 {
  masf[num]=0;
  for (int ip=0;ip<m1;ip++)
  {
   j=num-ip;
   if(num-ip<0) j=0;
   masf[num]=masf[num]+kih[ip]*vrem[j];
  }
 }
}// else
for (int i=0;i<dl/2;i++)
{
 float yy=masf[i];
 masf[i]=masf[dl-1-i];
 masf[dl-1-i]=yy;
}
if(dst!=0)
{
 for(int num=0;num<dl;num++)
 {
  j=num-m1;
  if(j<0)j=j+dl;
  Mul(m1,0,kih,masf,j,dl, &vrem[num]);
 }
} // dst!=0
else
{
 for(int num=0;num<dl;num++)
 {
  vrem[num]=0;
  for (int ip=0;ip<m1;ip++)
  {j=num-ip;
   if(num-ip<0)j=0;
   vrem[num]=vrem[num]+kih[ip]*masf[j];
  }
 }
} //  else
for (int i=0;i<dl/2;i++)
{float yy=vrem[i];
 vrem[i]=vrem[dl-1-i];
 vrem[dl-1-i]=yy;
}
//--------------------------
h=(float)DLKADRfp/DLKADRrp;
for (int i=0;i<dlr;i++)
{
 x=h*i;
 k=(int)x;
 vremr[i]=vrem[k]+(vrem[k+1]-vrem[k])*(x-k);
}
 for (int i=0;i<DLMASr+VEC_NUMS*DLDIGr;i++) mas[i]=vremr[i+DLMASr]/1000.;
 free(vrem); free(vremr);
}//пустая скобка

if(dst==0)//расчет через здесь
{xkm=0;
 for (int i=0;i<DLMASr;i++) xkm=xkm+mas[i]*mas[i];//сп
 for(int k=0;k<VEC_NUMS;k++)//коды
 {xkd[k]=0;
  for(int i=0;i<DLDIGr;i++) xkd[k]=xkd[k]+mas[DLMASr+i+DLDIGr*k]*mas[DLMASr+i+DLDIGr*k];
 }
}
else//расчет черех mulladd
{
 FloatMulAdd32no16((__int16) DLMASr, mas, mas, &xkm);
 for(int k=0;k<VEC_NUMS;k++)
  FloatMulAdd32no16((__int16) DLDIGr,&mas[DLMASr+DLDIGr*k], &mas[DLMASr+DLDIGr*k], &xkd[k]);
}
for(int i=0;i<2*DLKADR;i++) mast[i]=0;

nulotk=StrToInt(Sn->Cells[1][8]);//обновляем значения
mrmt=StrToInt(Sn->Cells[1][0]);
//-----------------------
MCP=-1; NGT=-1; MGT=-1;
ind0=-1;    PCP=-1;   PIK=0;     inpik=0; for(int i=0;i<npik;i++)PIK5[i]=0;
df=0; xmax=0;    fl_buf=false;
C2->Title->Font->Color=clBlack;
C2->Title->Text->Strings[0]="Корреляция ";
C2->Title->Text->Strings[1]="Сп ";
C2->BottomAxis->Maximum=DLKADRrp;
Panel2->Color=clRed;
Sport->Text=NCOM;
Fr45->Caption=TITLE+ver+ver2+", скважина: "+tpathckw+"\\"+ckwName+",   датчик: " ;
Fr45->Caption=Fr45->Caption+tar+", "+Sport->Text;
    iot=0; ctat=0;    //newr=0;    newr0=0;
H=(double)DLKADRfp/DLKADRrp;   DELTAr=inta(1.+DELTA/H);
xtek=m1*TDISCUSO*1000;
Canvas->Pen->Color=clBlack;
Canvas->Brush->Color=cform;
 Canvas->Rectangle(xc0,yc0,xc1,yc1);
  Label14->Caption="";

Canvas->Pen->Color=clSilver;
Canvas->MoveTo(xc0+1,yc00);  Fr45->Canvas->LineTo(xc1-1,yc00);
Canvas->Pen->Color=clBlack;
xt=xc0;  yt=yc00;
 dp=(float)(xc1-xc0)/(m1+DLKADR+2*DLMAS);  dpp=0;
 dcpm=DLMAS*(float)(xc1-xc0)/DLKADR; dcpl=DLDIG*(float)(xc1-xc0)/DLKADR;
Canvas->MoveTo(xt,yt);
Fr45->Panel2->Width=0;
StatusBar->Color=clBlack;
StatusBar->SimpleText=date0+"      Время с начала проводки: "+dtime;
ZeroOTK->Caption="";//чистим Нуль ОТК
Lgkzts->Caption="";//чистим ГКзтс
Lgkndm->Caption="";//чистим ГКндм


 //----------поток--------
 //ShowMessage("ius="+IntToStr(ius)+"   Index="+IntToStr(Fr45->R4->ItemIndex));
 StatusBar->Color=clWhite;
 zz0=MyStrToFloat(Eglubina->Text);
 iprov=1;
 Thread=new sic4(false);
 Thread->FreeOnTerminate=true;
 T0=Now();
 }
}
}
//----------------СТОП проводки --------------------------------------------
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
void __fastcall TFr45::stopClick(TObject *Sender)
{
  //Memo1->Lines->SaveToFile(tpathckw+"\\"+"otladka.txt");//записали в отладочный файл
  //включаем органы настроек
  R4->Enabled=true;
  Sport-> Enabled=true;
  //R5->Enabled=true;
  CBretr->Enabled=true;
  CBndm->Enabled=true;
  CBnnk->Enabled=true;
  CBgeomash->Enabled=true;
  if(CBndm->Checked==false) CBgkzts->Enabled=true;//только если не выбран НДМ
  u32->Enabled=true;
  TAR-> Enabled=true;
  Ckw->Enabled=true;
  Cdat->Enabled=true;
  RGcoder->Enabled=true;
  Label2->Enabled=true;
  Sport2->Enabled=true;

START=0;
Edz->Enabled=True;
if(fl_Th)
{
 iprov=0;  Sleep((unsigned long)500);
 Thread->Terminate();
 StatusBar->Color=clWhite;
 StatusBar->SimpleText=date0+"      Время с начала проводки: "+TimeToStr(Now()-T0)+"   проводка окончена.";
}
if(StatusRecorder==true) {RecButton->Down=false; RecButtonClick(Sender);}//отожмем кнопку записи
if(FileSignal!=NULL) {fclose(FileSignal); FileSignal=NULL;}
if(FileSignal2!=NULL) {fclose(FileSignal2); FileSignal2=NULL;}
if(StatusPlayer==true) {PlayButton->Down=false; PlayButtonClick(Sender);}//отожмем кнопку воспоизведения
//ждем поток ПБ
if(PB->Checked)
{
 if(PBThr) PBThr->Terminate();
 while(PotokPBON) Application->ProcessMessages();//ждем освобождения потока
 while(BusyPB) Application->ProcessMessages();//ждем освобождения порта ПБ
}
}
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//-----------------------------------------------------------------------------------------------------
//Тут по-моему рисование осциллограммы
void DIGIT(float Y)
{
#if FAZA4
xt=xt+hxt/2;
#else
xt=xt+hxt;
#endif
if(xt>xc1-1)
{
 xt=xc0;
 Fr45->Canvas->Pen->Color=clBlack;
 Fr45->Canvas->Brush->Color=cform;
 Fr45->Canvas->Rectangle(xc0,yc0,xc1,yc1);
 Fr45->Canvas->Pen->Color=clSilver;
 Fr45->Canvas->MoveTo(xc0+1,yc00);  Fr45->Canvas->LineTo(xc1-1,yc00);
 Fr45->Canvas->Pen->Color=clSilver;
 Fr45->Canvas->MoveTo(xt,yt);
}
yt=yc00-Y*mashvst;
if(yt<yc1) yt=yc1;
if(yt>yc0-1) yt=yc0-1;
if(Fr45->Co->Checked) Fr45->Canvas->Pen->Color=clBlue;
else  Fr45->Canvas->Pen->Color=clBlack;
Fr45->Canvas->LineTo(xt,yt);
if(NK<1)
{
 if((int)dpp > Fr45->Panel2->Width+5) Fr45->Panel2->Width=(int)dpp;
 dpp=dpp+dp;
}
}
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#include "OnMyPost1.h"
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#include "OnMyPost2.h"
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#include "OnMyPost3.h"
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#include "OnMyPost4.h"
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
void __fastcall TFr45::OnMyPost5(TMessage& a)
{if(iprov!=0)
{DateTimeToString(dtime,"h:nn:ss",Now()-T0);
Panel2->Width=dcpm;
 Fr45->StatusBar->Color=clWhite;
StatusBar->SimpleText=date0+"      Время с начала проводки: "+dtime;
 if(RadioGroup1->ItemIndex==1)
 { if(fabs(glubina-MyStrToFloat(Eglubina->Text))>0.05)
   { SaveStatusLog("Для губины "+Eglubina->Text+" запись в файл не произведена.", clRed);
     PlaySound("MYS2",0,SND_RESOURCE);
   }
 }
MCP=-1;
 int j;
int nkr=(int) a.LParam;  int nnr=nkr-DLKADRrp;if(nnr<0)nnr=nnr+2*DLKADRrp;
         j=nnr;
  for (int i=0;i<DLKADRrp;i++){if(j==2*DLKADRrp)j=0;
  mas1[i]=masc[j]; j=j+1;}

//    спектр и фильтрация сигнала
 //------------- корреляция СП
levelsig=0;
 for (int i=0;i<DLKADRrp;i++)if(levelsig< mas1[i])levelsig=mas1[i];
levelsig=levelsig*(KmB*1000/8388607.);
if(uco32)levelsig=levelsig*((KmB*1000./256.)/8388607./19.43) ;   //ShowMessage(levelsig);
if(CompareText(Sn->Cells[1][3],"")!=0)DCP=StrToInt(Sn->Cells[1][3]);
else DCP=0;
    int jj;//,j0,j1;
         float xkv;
         ind=0;    maxind=0; minind=0;
if(dst!=0){
 for(int i=0;i<DLKADRrp;i++)
{
Mul((__int16) DLMASr,1,mas1,  mas1,i,DLKADRrp, &xkv);
   if(xkv==0)korr[i]=0;
else{Mul((__int16) DLMASr,0,mas, mas1,i,DLKADRrp, &korr[i]);
korr[i]=korr[i]/(sqrt(xkm)*sqrt(xkv));   }

if(korr[i]>korr[maxind])maxind=i;
if(korr[i]<korr[minind])minind=i;
 }         }
 else {  for(int i=0;i<DLKADRrp;i++){
  korr[i]=0;    jj=i; xkv=0;
         for(int j=0;j<DLMASr;j++)
         {if(jj==DLKADRrp)jj=0;
          korr[i]=korr[i]+mas[j]*(float)mas1[jj];
          xkv=xkv+(float)mas1[jj]*(float)mas1[jj];
          jj=jj+1;}
        if(xkv==0)korr[i]=0;
        else korr[i]=korr[i]/(sqrt(xkm)*sqrt(xkv));

       if(korr[i]>korr[maxind])maxind=i;
	if(korr[i]<korr[minind])minind=i;
                                          }
      }
      if(fabs(korr[maxind])>fabs(korr[minind]))ind=maxind;
      else  ind=minind;
      if(korr[ind]>=0)faza=1;
      else faza=-1;
      DCPt=abs((int)(korr[ind]*100));  DCPt=DCPt+1;
if( DCPt>=DCP)Panel2->Color=clGreen;
else  Panel2->Color=clRed;
//-------------------ind
  int indt=ind+nnr;   if(indt>2*DLKADRrp)indt=indt-2*DLKADRrp;
  double h=(double)DLKADRf/DLKADRr;
double icp=indt*h;  if(icp>DLKADR)icp-=DLKADR;
 int ii=icp-PCP;
  if(ii>0&&icp-PCP-ii>0.5)ii=ii+1;
  if(ii<0&&(icp-PCP)-(ii-1)<0.5)ii=ii-1;
  if(PIK==1)  C2->Title->Text->Strings[0]="Корреляция, подстройка частоты "+IntToStr(ID);
if(PIK==0){
  inpik=inpik+1;  if(inpik<npik+1) PIK5[inpik-1]=ii;
 else{for(int i=1;i<npik;i++)
 PIK5[i-1]=PIK5[i];
PIK5[npik-1]=ii;inpik=inpik-1; }

 AnsiString s="";
 for (int i=0;i<inpik;i++)s=s+IntToStr(PIK5[i])+",  ";
  SPIK= IntToStr(PIK5[inpik-1]);
   C2->Title->Text->Strings[0]="Корреляция, сдвиг пика на "+s;
        } //if(PIK==0)
        PIK=0;
   PCP=icp;
   if(inpik==npik &&PIK==0&& abs(PIK5[npik-1])>0&& abs(PIK5[npik-2])>0)
   {  int j=0;
   for (int i=0;i<npik;i++){if(abs(PIK5[i])<DELTA)j=j+1; }
   if(j==5){if(PODChecked){ID=0;

    for (int i=0;i<npik;i++)ID=ID+PIK5[i];
     float fid=(float)ID/npik;
  ID=fid;
  if(fid-ID>0.5)ID=ID+1;

     DLKADRn=DLKADR+ID;
  if(abs(ID)>=2&&abs(DLKADRn-DLKADR0)<DELTA){
  PIK=1;



DLKADR=DLKADR+ID;


H=(double)DLKADR/DLKADRrp;

double qqq=DLKADR/TKADR;
TUSO=1/qqq/pw[freq];
 for (int i=0;i<npik;i++)PIK5[i]=0; inpik=0;
    }       }                         }
               }

NGO=0;
NGT=NGO;
MGT=indt+DLMASr+DLDIGr+iG;
fl_buf=false;
if(MGT>=2*DLKADRrp){MGT-=2*DLKADRrp; fl_buf=true;}
NGO=-1;

    //рисование корреляции  красным
	Series3->Clear();
	for(int j=0;j<DLKADRrp;j++) Series3->AddXY(j,korr[j],"",clRed);

	if( DCPt<DCP)
    {	C2->Title->Font->Color=clRed;
		C2->Title->Text->Strings[1]="Сп "+IntToStr(DCPt)+"%(";
		return;
    }
	else
    {	C2->Title->Font->Color=clBlack;
  		C2->Title->Text->Strings[1]="Сп "+IntToStr(DCPt)+"%("; }
   	}
}
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
void __fastcall TFr45::OnMyPost6(TMessage& a)
{if(iprov!=0)  if(spektr->Checked)
{float r1,r2;
int ii=a.WParam;     int it=a.LParam;
BPF(false,&MasC[ii*DL],DL);
for (int i=0;i<DL;i++)MasC[i+ii*DL]=MasC[i+ii*DL]/sqrt(DL);
B[0]=0;for(int j=0;j<it;j++)B[0]+=fabs(MasC[j*DL]); B[0]=B[0]/it;
B[DS-1]=0;for(int j=0;j<it;j++)B[DS-1]+=fabs(MasC[1+j*DL]);B[DS-1]=B[DS-1]/it;
for (int i=1;i<DS-1;i++){ B[i]=0;  r1=0; r2=0;
for(int j=0;j<it;j++)r1+= fabs(MasC[2*i+j*DL]);r1=r1/it;
for(int j=0;j<it;j++)r2+=fabs(MasC[2*i+1+j*DL]);r2=r2/it;
B[i]=sqrt(r1*r1+r2*r2);
}
xmax=0; sr=0;
for (int i=0;i<DS;i++)
{sr=sr+B[i]*B[i]/DS;if(xmax<B[i])xmax=B[i];}
sr=sqrt(sr);
Series2->Clear();
df=TDISCUSO*DL;
if(xmax>0){
 kss=sr/(xmax*(pow(2,ur)-1));
for (int i=0;i<DS;i++)
{if(B[i]*kss>0)Series2->AddXY(i/df,20*log10(B[i]*kss),"",clBlue);
  else Series2->AddXY(i/df,-150,"",clBlue);}
             }

   }
}
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
void __fastcall TFr45::TrackBar1Change(TObject *Sender)
{
int i=TrackBar1->Position;
{ mashvs=i;
 mashvst=100.*(yc00-yc1)/mashvs;   Canvas->Brush->Color=Fr45->Color;
 Canvas->TextOut(IL+4,IT+2,"                ");
 Canvas->TextOut(IL+4,IH+IT-16,"                           ");
 Canvas->TextOut(IL+4,IT+2,FloatToStrF0(mashvs/100.,8,2));
 Canvas->TextOut(IL+4,IH+IT-16,FloatToStrF0(-mashvs/100.,8,2));
 Canvas->MoveTo(xt,yt);           }
}
//-----------------------------------------------------------------------------------------------------
void __fastcall TFr45::N2Click(TObject *Sender)
{  Close();
}
//-----------------------------------------------------------------------------------------------------
void __fastcall TFr45::FormClose(TObject *Sender, TCloseAction &Action)
{
BM->Free();
BM1->Free();
free(kih);
FreeLibrary( dllInstance);
FloatMulAdd32no16=NULL;
CheckSSE=NULL;
delete korr;
korr=NULL;
delete mas;
mas=NULL;
delete mas1;
mas1=NULL;
delete masc;
masc=NULL;
delete mast;
mast=NULL;
delete masf;
masf=NULL;
delete dig;
/*dig=NULL;
delete data_perc;
data_perc=NULL;
delete dproc;
dproc=NULL;
delete dg;
dg=NULL;*/
Timer8->Enabled=false;
Timer4->Enabled=false;
TimerGti->Enabled=false;
if(PotokON) {PotokON=0; while(!PotokOFF)Application->ProcessMessages();}//останавливаем поток
Fr45->TcpClient1->Active=false;
if(portPB!=NULL || portPB!=INVALID_HANDLE_VALUE)
  {PurgeComm(portPB,PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR); CloseHandle(portPB);}
Timer10->Enabled=false;//Выключаем таймер против сна системы
if(FileSignal!=NULL) {fclose(FileSignal); FileSignal=NULL;}
if(FileSignal2!=NULL) {fclose(FileSignal2); FileSignal2=NULL;}
}
//-----------------------------------------------------------------------------------------------------
void __fastcall TFr45::T2Change(TObject *Sender)
{ hxt=T2->Position*FFRE[freq];
}
//-----------------------------------------------------------------------------------------------------
void __fastcall TFr45::I2MouseUp(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{ if(c2==I1->Canvas->Pixels[X][Y]||c1==I1->Canvas->Pixels[X][Y])
  {
  if (ColorDialog1->Execute())
   {I1->Canvas->Brush->Color=ColorDialog1->Color;
   if(c1==I1->Canvas->Pixels[X][Y])c1=ColorDialog1->Color;
   if(c2==I1->Canvas->Pixels[X][Y])c2=ColorDialog1->Color;
   I1->Canvas->FloodFill(X,Y,I1->Canvas->Pixels[X][Y],fsSurface);
    }
   }
   else
   {
   if(Rnapr->Visible==false)
{TRect Tt;
TPoint P;
P=Rnapr->ClientToScreen(Point(0,0));
Tt.Left=P.x;
Tt.Top=P.y;
P=Rnapr->ClientToScreen(Point(Rnapr->UndockWidth,Rnapr->UndockHeight));
Tt.Right=P.x;
Tt.Bottom=P.y;
Rnapr->ManualFloat(Tt);RnaprClick(Sender);}
  }
}
//-----------------------------------------------------------------------------------------------------
void __fastcall TFr45::PotkResize(TObject *Sender)
{Panel4->Width=Potk->Width-Cmin->Width-Cmax->Width;
BM->Width=min(Potk->Height-OBar->Height,Potk->Width); BM->Height= BM->Width;
ox=BM->Width/2;  oy=BM->Width/2-17;
rad=4*BM->Width/10;
I2->Canvas->FillRect(Rect(0,0,I2->Width,I2->Height));
I1->Canvas->FillRect(Rect(0,0,I1->Width,I1->Height));
if(Potk->Visible){Button3Click(Sender);}
}
//-----------------------------------------------------------------------------------------------------
void __fastcall TFr45::CmaxChange(TObject *Sender)
{
otkmax=Cmax->Value;
if(Potk->Visible){Button3Click(Sender); }
}
//---------------------------------------------------------------------------
void __fastcall TFr45::Button3Click(TObject *Sender)
{
int ox,oy;
BM->Canvas->FillRect(Rect(0,0,BM->Width,BM->Height));
BM->Width=min(Potk->Height-OBar->Height,Potk->Width); BM->Height= BM->Width;
ox=BM->Width/2;
oy=BM->Width/2-17;//поднимем вверх
rad=4*BM->Width/10;
I2->Canvas->FillRect(Rect(0,0,I2->Width,I2->Height));
I1->Canvas->FillRect(Rect(0,0,I1->Width,I1->Height));
BM->Canvas->Pen->Width=1;
BM->Canvas->Brush->Color=c1;
BM->Canvas->Pen->Color=c1;
BM->Canvas->Ellipse( ox-rad,oy-rad,ox+rad,oy+rad);
BM->Canvas->Brush->Color=c2;
int x3,x4,y3,y4;
if(napr==1){
 x3=ox-(rad)*sin(M_PI*(360-otkmin)/180) ;
 y3=oy-(rad)*cos(M_PI*(360-otkmin)/180);
 x4=ox+(rad)*sin(M_PI*otkmax/180) ;
 y4=oy-(rad)*cos(M_PI*otkmax/180);}
if(napr==-1){
 x4=ox-(rad)*sin(M_PI*otkmin/180) ;
 y4=oy-(rad)*cos(M_PI*otkmin/180);
 x3=ox+(rad)*sin(M_PI*(360-otkmax)/180) ;
 y3=oy-(rad)*cos(M_PI*(360-otkmax)/180);}
BM->Canvas->Pen->Color=c1;
 BM->Canvas->Pie( ox-rad,oy-rad,ox+rad,oy+rad,x3,y3,x4,y4);
 BM->Canvas->Pen->Color=clBlack;
BM->Canvas->Brush->Color=Potk->Color;
 BM->Canvas->TextOut(ox-4,oy-rad-14,"0");
 BM->Canvas->TextOut(ox-10,oy+rad+4,"180");
if(napr==1)
{BM->Canvas->TextOut(ox+rad+4,oy-7,"90");
 BM->Canvas->TextOut(ox-rad-20,oy-5,"270");
}
if(napr==-1)
{BM->Canvas->TextOut(ox+rad+4,oy-7,"270");
 BM->Canvas->TextOut(ox-rad-20,oy-5,"90");
}
  for(int i=0;i<360;i+=30){
  int x=ox+(rad)*cos(M_PI+M_PI/180*i) ;
  int y=oy+(rad)*sin(M_PI+M_PI/180*i);
  int x1=ox+(rad-5)*cos(M_PI+M_PI/180*i) ;
  int y1=oy+(rad-5)*sin(M_PI+M_PI/180*i);
  BM->Canvas->MoveTo(x1,y1);
  BM->Canvas->LineTo(x,y);
  }
I1->Picture->Assign(BM);
if(iotk>=0){
BM->Canvas->FillRect(Rect(0,0,BM->Width,BM->Height));
BM->Canvas->Pen->Width=2;
int j=0,xo,yo;
for(int i=4-iotk;i<5;i++){
if(napr==1){
 xo=ox-((5-j)*rad/5)*sin(M_PI*(360-newot[i])/180) ;
 yo=oy-((5-j)*rad/5)*cos(M_PI*(360-newot[i])/180);
}
if(napr==-1){
 xo=ox-((5-j)*rad/5)*sin(M_PI*(newot[i])/180) ;
 yo=oy-((5-j)*rad/5)*cos(M_PI*(newot[i])/180);
}
if(i>4-iotk)BM->Canvas->Pen->Width=1;
BM->Canvas->MoveTo(ox,oy);
BM->Canvas->LineTo(xo,yo);
j++;}
BM->Canvas->Pen->Width=1;
I2->Picture->Assign(BM);
}
   }
//---------------------------------------------------------------------------
void __fastcall TFr45::CminChange(TObject *Sender)
{
otkmin=Cmin->Value;
if(Potk->Visible){Button3Click(Sender);}
}
//---------------------------------------------------------------------------
void __fastcall TFr45::EglubinaKeyPress(TObject *Sender, char &Key)
{
char s[]={'0','1','2','3','4','5','6','7','8','9','0','.' };
WORD kk=Key;
if(kk==8)return;
int ikey=0;
for (int i=0;i<12;i++)if(Key==s[i])ikey=1;
if(ikey==0){Key=NULL;  PlaySound("MYS2",0,SND_RESOURCE);  }
}
//---------------------------------------------------------------------------
void __fastcall TFr45::l(TObject *Sender, int ACol, int ARow,
      TRect &Rect, TGridDrawState State)
{
if(ACol==0)L1->Canvas->Font->Color=clRed;
if(ACol==1)L1->Canvas->Font->Color=clBlue;
L1->Canvas->TextOut(Rect.Left+2,Rect.Top,L1->Cells[ACol][ARow]);
}
//---------------------------------------------------------------------------
void __fastcall TFr45::spektrClick(TObject *Sender)
{
if(spektr->Checked==true){C2-> Width=IW/2;C3->Visible=true;}
else {C2-> Width=IW;C3->Visible=false;}
}
//---------------------------------------------------------------------------
void __fastcall TFr45::CotklClick(TObject *Sender)
{
if(Cotkl->Checked==true)
{
 Potk->Visible=true;
 Panel4Click(Sender);
 ZeroOTK->Left=Potk->Left+4;
 ZeroOTK->Top=Potk->Height-ZeroOTK->Height-1;
}
else  Potk->Visible=false;
}
//---------------------------------------------------------------------------
void __fastcall TFr45::Panel3Click(TObject *Sender)//Кнопка '+' глубины
{
 if(RadioGroup1->ItemIndex != 1) return;//если не ручной реэим, то возврат
 if(CompareText(Fr45->Eglubina->Text,"")==0) return;//если глубина пустая - выходим
 glubina=MyStrToFloat(Eglubina->Text);
 if(CompareText(Edz->Text,"")!=0) glubina=glubina+MyStrToFloat(Edz->Text);
 else {ShowMessage("Введите шаг по глубине");return;}
 Eglubina->Text=FloatToStrF0(glubina,7,2);
 Panel6Click(Sender);//запись в файл inc
}
//---------------------------------------------------------------------------
void __fastcall TFr45::Panel4Click(TObject *Sender)
{if(it==1)Potk->ManualFloat(T);
}
//---------------------------------------------------------------------------
void __fastcall TFr45::C3Zoom(TObject *Sender)
{
C3->LeftAxis->SetMinMax(-150,0) ;

C3->LeftAxis->Increment=50;
C3->BottomAxis->Minimum=0;
}
//---------------------------------------------------------------------------
#include "write_to_file.h"
#include "write_LAS.h"
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TFr45::CoClick(TObject *Sender)
{  fl_fil=Co->Checked ;
if(Co->Checked==true){
Label9->Font->Color=clBlue;
Label9->Caption="ОСЦИЛЛОГРАММА (фильтрованная)";
Canvas->Pen->Color=clBlue;}
if(Co->Checked==false){
Label9->Font->Color=clBlack;
Label9->Caption="ОСЦИЛЛОГРАММА (не фильтрованная)";
Canvas->Pen->Color=clBlack;}
 Co->Visible=true;
}
//---------------------------------------------------------------------------
void __fastcall TFr45::R4Click(TObject *Sender)
{   
if(u32->Checked)
 {//R4->Items->Strings[0]="10";
  KmB=2.5*256;
  ur=31;
  TrackBar1->Max=100*8388607*((KmB/256)*1000/8388607.);
 }
 else {   //R4->Items->Strings[0]="10" ;
          KmB=2.5;
          ur=23;
          TrackBar1->Max=100*8388607*((KmB)*1000/8388607.);
      }
freq=R4->ItemIndex;
}
//---------------------------------------------------------------------------
void __fastcall TFr45::CdatChange(TObject *Sender)
{
	tar=Cdat->Text;
    AnsiString str = TITLE+ver+ver2+", скважина: "+tpathckw+"\\"+ckwName+",   датчик: " ;
    str += tar+", "+Sport->Text;
    Fr45->Caption = str;
}
//---------------------------------------------------------------------------
void __fastcall TFr45::CkwChange(TObject *Sender)
{
//изменяем имена файлов при смене скважины
//проверяем, есть ли уже такие файлы, и если нет, то создаем новые
//перезаполняем используемые массивы
FILE *F;
ckwName=Ckw->Text;
if(ckwName=="") {ShowMessage("Задайте скважину"); return;}

//файл .bkp
Fdat=tpathckw+"\\"+ckwName+".bkp";
    //проверяем наличие файла
    F=fopen(Fdat.c_str(),"r");
    if(F==NULL)
    {F=fopen(Fdat.c_str(),"a+b");//если файла нет, то создаем новый
     AnsiString str = getZagolDat();
     fprintf(F,"%s",str+"\r\n");//пишем заголовок
    }
    if(F) fclose(F);//закрываем файл

//файл .ink
Fink=tpathckw+"\\"+ckwName+".ink";
    LoadToMemoInkFromFile(Fink);//грузим в мемо из файла
    //проверяем заголовок таблицы в файле
    if(ZagolForInk != MemoInc->Lines->Strings[0]) ShowMessage("Заголовок в файле "+Fink+" не соответствует текущему набору данных.");
	//загрузим глубину из мемо, если она там есть
	if(MemoInc->Lines->Count>1) glubina_last=MyStrToFloat(AnsiString(MemoInc->Lines->Strings[MemoInc->Lines->Count-1]).SubString(0,DlinaParam-1));
    else glubina_last=0;
	glubina_last=roundf(glubina_last,2);
	Gll->Caption="забой "+FloatToStrF0(glubina_last,7,2);
	Eglubina->Text=FloatToStrF0(glubina_last,7,2);
    Pckw->Caption="Скважина "+ckwName;

//файл .stat
Fstat=tpathckw+"\\"+ckwName+".stat";
	F=fopen(Fstat.c_str(),"a");//если файла нет, то создаем новый
	fclose(F);//закрываем файл
    LoadToMemoStatFromFile(Fstat);//загружаем статику из файла в MemoStat

Fin=tpathckw+"\\"+ckwName+".in";
Fraw5=tpathckw+"\\"+"raw5.in";
Fraw4=tpathckw+"\\"+"raw4.in";
Fmagcorr=tpathckw+"\\"+"magcorr3.in";
Fsur=tpathckw+"\\"+ckwName+".sur";
Fgeotrack=tpathckw+"\\"+ckwName+"_geotrack.txt";
Flas=tpathckw+"\\"+ckwName+".las";
Fpasport=tpathckw+"\\pasport.txt";//в папке скважины
LoadPasportFile(Fpasport);
Fstatus=tpathckw+"\\status.log";//в папке скважины
Fnastr=tpathckw+"\\nastr.log";//в папке скважины
Fr45->Caption=TITLE+ver+ver2+", скважина: "+tpathckw+"\\"+ckwName+"," ;
Fr45->Caption=Fr45->Caption+"   датчик: ";
Fr45->Caption = Fr45->Caption+tar+", "+Sport->Text;
}
//---------------------------------------------------------------------------
//Меню Настройка
void __fastcall TFr45::N5Click(TObject *Sender)
{
Pnastr->Visible=true;
TimerNastr->Enabled = true;//запустим таймер авто-закрытия дочерних панелей
//   список файлов тарировки для разных датчиков
Cdat->Text=tar;
Sport->Text=NCOM;
if(PB->Checked) Sport2->Text=NCOMPB;
//Покажем активные окошки
if(Sv2->Checked) Pwits->Visible=true;//WITS сервер
if(Cl2->Checked) PwitsClient->Visible=true;//WITS клиент
if(Sv->Checked)//внешняя глубина
{
 if(SourceGlub->ItemIndex==0) Pgeo->Visible=true;//Геосенсор
 if(SourceGlub->ItemIndex==1) Pleusa->Visible=true;//Леуза
 if(SourceGlub->ItemIndex==2) Pgti->Visible=true;//ГТИ
}
if(CBLog->Checked) PanelSrvLog->Visible=true;//WITS сервер
}
//---------------------------------------------------------------------------
void __fastcall TFr45::N6Click(TObject *Sender)//Кнопка меню Скважина
{ckwName=Ckw->Text;
 Pckw->Caption = "Скважина "+ckwName;
 if( CompareText(ckwName,fckwt)!=0)//если не совпадают
 {	fckwt=ckwName;
    if(CompareText(ckwName,"")==0) {ShowMessage("Задайте скважину"); return;}
    CkwChange(Sender);//загружаем файлы и массивы
 }
 Pckw->Visible=true;
}
//---------------------------------------------------------------------------
void __fastcall PutLeuza(AnsiString STR)
{HANDLE Leuza;
unsigned long kolb;
 Leuza=CreateFile("\\\\*\\mailslot\\Registration\\FixedData",GENERIC_WRITE,
  FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);
  if (Leuza==INVALID_HANDLE_VALUE)return;
{WriteFile(Leuza,&STR[1],StrLen(STR.c_str()),&kolb,NULL);
   CloseHandle(Leuza);  }
  Leuza=CreateFile("\\\\*\\mailslot\\Registra",GENERIC_WRITE,
  FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);
  if (Leuza==INVALID_HANDLE_VALUE)return;
{WriteFile(Leuza,&STR[1],StrLen(STR.c_str()),&kolb,NULL);
  CloseHandle(Leuza);  }
}
//-----------------------------------------------------------------------------------------------------
 void BPF( bool ModeBPF,float Mas[],int DL)
/*Быстрое преобразование Фурье вещественного массива
 Входные параметры:
  Mas - массив значений (длина должна быть степенью двойки)
  ModeFFT - направление преобразования
             =true - обратное,
             =false - прямое
 Выходные параметры:
  Mas - результат преобразования*/
{
double twr,twi,twpr,twpi,twtemp,ttheta;
 int i,i1,i2,i3,i4;
double c1,c2,h1r,h1i,h2r,h2i,wrs,wis;
 int  nn,ii,jj,n,mmax,m,j,istep,isign;
double  wtemp,wr,wpr,wpi,wi,theta,tempr,tempi;
double Pi=M_PI;
 if( DL<=1)return;
 if (!ModeBPF)
 {ttheta=2*Pi/DL;
  c1=0.5;
  c2=-0.5;
 }
 else {
  ttheta=2.*Pi/DL;
  c1=0.5;
  c2=0.5;
  ttheta=-ttheta;
  twpr=-2.0*sin(0.5*ttheta)*sin(0.5*ttheta);
  twpi=sin(ttheta);
  twr=1.0+twpr;
  twi=twpi;
  i=2;
  while (i<=DL/4+1)
  {
   i1=i+i-1;
   i2=i1+1;
   i3=DL+3-i2;
   i4=i3+1;
   wrs=twr;
   wis=twi;
   h1r=c1*(Mas[i1-1]+Mas[i3-1]);
   h1i=c1*(Mas[i2-1]-Mas[i4-1]);
   h2r=-c2*(Mas[i2-1]+Mas[i4-1]);
   h2i=c2*(Mas[i1-1]-Mas[i3-1]);
   Mas[i1-1]=h1r+wrs*h2r-wis*h2i;
   Mas[i2-1]=h1i+wrs*h2i+wis*h2r;
   Mas[i3-1]=h1r-wrs*h2r+wis*h2i;
   Mas[i4-1]=-h1i+wrs*h2i+wis*h2r;
   twtemp=twr;
   twr=twr*twpr-twi*twpi+twr;
   twi=twi*twpr+twtemp*twpi+twi;
   i++;
  }
  h1r=Mas[0];
  Mas[0]=c1*(h1r+Mas[1]);
  Mas[1]=c1*(h1r-Mas[1]);
 }

 if (ModeBPF)
  isign=-1;
 else
  isign=1;
 n=DL;
 nn=DL/2;
 j=1;
 ii=1;
 while ( ii<=nn)
 {
  i=2*ii-1;
  if( j>i)
  {
   tempr=Mas[j-1];
   tempi=Mas[j];
   Mas[j-1]=Mas[i-1];
   Mas[j]=Mas[i];
   Mas[i-1]=tempr;
   Mas[i]=tempi;
  }
  m=n / 2;
  while (m>=2&&j>m)
  {
   j=j-m;
   m=m/2;
  }
  j=j+m;
  ii++;
 }
 mmax=2;
 while( n>mmax)
 {istep=2*mmax;
  theta=2.*Pi/(isign*mmax);
  wpr=-2.0*sin(0.5*theta)*sin(0.5*theta);
  wpi=sin(theta);
  wr=1.0;
  wi=0.0;
  ii=1;
  while (ii<=mmax/2)
  {
   m=2*ii-1;
   jj=0;
   while( jj<=((n-m)/istep) )
   {i=m+jj*istep;
    j=i+mmax;
    tempr=wr*Mas[j-1]-wi*Mas[j];
    tempi=wr*Mas[j]+wi*Mas[j-1];
    Mas[j-1]=Mas[i-1]-tempr;
    Mas[j]=Mas[i]-tempi;
    Mas[i-1]=Mas[i-1]+tempr;
    Mas[i]=Mas[i]+tempi;
    jj++;
   }

   wtemp=wr;
   wr=wr*wpr-wi*wpi+wr;
   wi=wi*wpr+wtemp*wpi+wi;
   ii++;
  }
  mmax=istep;
 }
 if( ModeBPF)
 {
  i=1;
  while( i<=2*nn)
  {
   Mas[i-1]=Mas[i-1]/nn;
   i++;
  }
 }
 if(!ModeBPF)
 {
  twpr=-2.0*sin(0.5*ttheta)*sin(0.5*ttheta);
  twpi=sin(ttheta);
  twr=1.0+twpr;
  twi=twpi;
  i=2;
  while( i<=DL/4+1)
  {
   i1=i+i-1;
   i2=i1+1;
   i3=DL+3-i2;
   i4=i3+1;
   wrs=twr;
   wis=twi;
   h1r=c1*(Mas[i1-1]+Mas[i3-1]);
   h1i=c1*(Mas[i2-1]-Mas[i4-1]);
   h2r=-c2*(Mas[i2-1]+Mas[i4-1]);
   h2i=c2*(Mas[i1-1]-Mas[i3-1]);
   Mas[i1-1]=h1r+wrs*h2r-wis*h2i;
   Mas[i2-1]=h1i+wrs*h2i+wis*h2r;
   Mas[i3-1]=h1r-wrs*h2r+wis*h2i;
   Mas[i4-1]=-h1i+wrs*h2i+wis*h2r;
   twtemp=twr;
   twr=twr*twpr-twi*twpi+twr;
   twi=twi*twpr+twtemp*twpi+twi;
   i++;
  }
  h1r=Mas[0];
  Mas[0]=h1r+Mas[1];
  Mas[1]=h1r-Mas[1];
 }
}
//------------------------------------------------------------
void __fastcall TFr45::SportChange(TObject *Sender)
{
NCOM=Sport->Text;
NCOMh=NCOM;
int j=LastDelimiter("(",NCOM);
if(j>0) NCOMh=Trim( AnsiString(NCOMh.SubString(1,j-1)));
else  NCOMh=NCOM;
Fr45->Caption=TITLE+ver+ver2+", скважина: "+tpathckw+"\\"+ckwName+",   датчик: " ;
Fr45->Caption=Fr45->Caption+tar+", "+Sport->Text;
}
//---------------------------------------------------------------------------
void __fastcall TFr45::SportDropDown(TObject *Sender)
{
 SportDblClick(Sender);
}
//---------------------------------------------------------------------------
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
void __fastcall TFr45::FormShow(TObject *Sender)
{
N5->Visible=true;
N6->Visible=true;
TB2->Visible=true;
Timer10->Interval=59000;//59c
Timer10->Enabled=true;//Включаем таймер против сна системы
if( fl_prov)
{
W=Fr45->ClientWidth;
int H=Fr45->ClientHeight-StatusBar->Height-TB2->Height;
Panel7->Width=W-Panel7->Left;
Potk->Top=TB2->Height;
Potk ->Left=0;
int hh=1.6*H/3;
Potk->Width=hh-OBar->Height;
Potk->Height=hh;
Lgkzts->Left = Potk->Left+2;
Lgkzts->Top = Potk->Height - Lgkzts->Height-20;
ZeroOTK->Left = Lgkzts->Left;
ZeroOTK->Top = Lgkzts->Top + ZeroOTK->Height+2;
//Lgkzts->Caption = "ГКзтс=";
//ZeroOTK->Caption = "ZeroOTK=";
Lgkndm->Left = Potk->Left+2+210;
Lgkndm->Top = Lgkzts->Top;
FORMAT_KADRA=solv_format_kadra();//вычисляем формат кадра
Lgkndm->Caption="";

Panel4->Width=Potk->Width-Cmin->Width-Cmax->Width;
IW=W-Potk->Width;
IH=H/3;
IT=TB2->Height;
IL=Potk->Width;
TrackBar1->Top=IT;
TrackBar1->Left=IW+IL-TrackBar1->Width-1;
TrackBar1->Height=IH;
T2->Left=IL+50;
T2->Width=IW-100;
T2->Top=IT+IH-T2->Height-2;
Co->Top=IT+2;
Co->Left=IL+100;
Label9->Top=IT+2;
Corrector->Top=IT+2;
Label9->Left=Co->Left+Co->Width+50;
Corrector->Left=Label9->Left+Label9->Width+250;

//Колонка ЗУ-Inc, опорная, по ней настраиваются все остальные
ZU2->Left=0;
ZU2->Top=Potk->Height+Potk->Top;
ZU2->Width=64;//69;//Опорная ширина всех колонок
ZU2->Height=H-Potk->Height;
ZU2->DefaultColWidth=(ZU2->Width)/(ZU2->ColCount);
ZU2->Visible=true;
//Колонка Зенита
L1->Top=ZU2->Top;
L1->Width=ZU2->Width;
L1->Height=ZU2->Height;
L1->DefaultColWidth=ZU2->DefaultColWidth;
L1->Left=0;
ZU2->Visible=false;
//Колонка Азимута
L1a->Left=L1->Width+L1->Left; 
L1a->Top=L1->Top;
L1a->Width=L1->Width;
L1a->Height=L1->Height;
L1a->DefaultColWidth=L1->DefaultColWidth;
//Колонка Азимута корр.
Lakopp->Left=L1a->Width+L1a->Left; 
Lakopp->Top=L1->Top;
Lakopp->Width=L1->Width;
Lakopp->Height=L1->Height;
Lakopp->DefaultColWidth=L1->DefaultColWidth;
//Колонка модуля H
Lh->Left=Lakopp->Width+Lakopp->Left; 
Lh->Top=L1->Top;
Lh->Width=L1->Width+35;//1.3*L1->Width;
Lh->Height=L1->Height;
Lh->DefaultColWidth=1.5*L1->DefaultColWidth;
//Колонка ОТК
L0->Left=Lh->Width+Lh->Left;  
L0->Top=L1->Top;
L0->Width=L1->Width;
L0->Height=L1->Height;
L0->DefaultColWidth=L1->DefaultColWidth;

MemoUgol->Top=80;
MemoUgol->Left=300;
MemoUgol->Height=305;
MemoUgol->Width=966+200;
//MemoUgol->Lines->Add("Начало");

Lz->Top= L1->Top+L1->DefaultRowHeight;
Lz->Left=L1->Left;
Lz->Width=L1->Width;

La->Top= L1a->Top+L1a->DefaultRowHeight;
La->Width=L1a->Width;
La->Left=L1a->Left;

Lkopp->Top= Lakopp->Top+Lakopp->DefaultRowHeight;
Lkopp->Width=Lakopp->Width;
Lkopp->Left=Lakopp->Left;

Lhh->Top= Lh->Top+Lh->DefaultRowHeight;
Lhh->Width=Lh->Width;
Lhh->Left=Lh->Left;

Lo2->Width=(Potk->Width)/4;
Lo2->Height=L1->Height;
Lo2->Top=L1->Top;

Panel5->Left=L0->Width+L0->Left; //Панель служебных параметров
Panel5->Top=Potk->Height+Potk->Top;
Panel5->Width=L1->Width-5;//(W/2-W/3)/2;
Panel5->Height=L1->Height;
Cobor->Top=Label4->Height;
Cobor->Left=0;
Cobor->Width=Panel5->Width-3;
Cobor->Height=20;
Label5->Top=Cobor->Top+Cobor->Height;
Ctem->Top=Label5->Top+Label5->Height;
Ctem->Left=0;
Ctem->Width=Panel5->Width-3;
Ctem->Height=20;
Label6->Top=Ctem->Top+Ctem->Height;
Cnapr->Top=Label6->Top+Label6->Height;
Cnapr->Left=0;
Cnapr->Width=Panel5->Width-3;
Cnapr->Height=20;
Label11->Top=Cnapr->Top+Cnapr->Height;
Cvib->Top=Label11->Top+Label11->Height;
Cvib->Left=0;
Cvib->Width=Panel5->Width-3;
Cvib->Height=20;
Label12->Top=Cvib->Top+Cvib->Height;
Cgk->Top=Label12->Top+Label12->Height;
Cgk->Left=0;
Cgk->Width=Panel5->Width-3;
Cgk->Height=20;
Label15->Top=Cgk->Top+Cgk->Height;
Cwr->Top=Label15->Top+Label15->Height;
Cwr->Left=0;Cwr->Width=Panel5->Width-3; Cwr->Height=20;
Lwr->Width=65;
Lo->Width=65;
Lte->Width=65;
Lnapr->Width=65;
Lvib->Width=65;
Lgk->Width=65;
Lwr->Height=305;
Lo->Height=305;
Lte->Height=305;
Lnapr->Height=305;
Lvib->Height=305;
Lgk->Height=305;
Label19->Top=Cwr->Top+Cwr->Height;
CG->Top=Label19->Top+Label19->Height;
CG->Left=0;
CG->Width=Panel5->Width-3;
CG->Height=20;
Diod->Top=CG->Top+CG->Height+4;
Diod->Width=10;
Diod->Height=10;
Diod->Left=CG->Width/2-Diod->Width/2;
LG->Width=65;
LG->Height=305;
H=Fr45->ClientHeight-StatusBar->Height;
if( Panel5->Height-Cgk->Top>LG->Height )LG->Top=Panel5->Top+Cgk->Top;
else   LG->Top=H-LG->Height;

if( Panel5->Height-Cwr->Top>Lwr->Height )Lwr->Top=Panel5->Top+Cwr->Top;
else   Lwr->Top=H-Lwr->Height;
if( Panel5->Height-Cobor->Top>Lwr->Height )Lo->Top=Panel5->Top+Cobor->Top;
else   Lo->Top=H-Lwr->Height;
if( Panel5->Height-Ctem->Top>Lwr->Height )Lte->Top=Panel5->Top+Ctem->Top;
else   Lte->Top=H-Lte->Height;
if( Panel5->Height-Cnapr->Top>Lwr->Height )Lnapr->Top=Panel5->Top+Cnapr->Top;
else   Lnapr->Top=H-Lwr->Height;
if( Panel5->Height-Cvib->Top>Lwr->Height )Lvib->Top=Panel5->Top+Cvib->Top;
else   Lvib->Top=H-Lvib->Height;
if( Panel5->Height-Cgk->Top>Lwr->Height )Lgk->Top=Panel5->Top+Cgk->Top;
else   Lgk->Top=H-Lwr->Height;
LG->Left=Panel5->Left+Panel5->Width;
Lwr->Left=Panel5->Left+Panel5->Width;
Lo->Left=Panel5->Left+Panel5->Width;
Lte->Left=Panel5->Left+Panel5->Width;
Lnapr->Left=Panel5->Left+Panel5->Width;
Lvib->Left=Panel5->Left+Panel5->Width;
Lgk->Left=Panel5->Left+Panel5->Width;
Label5->Left=0;
Label5->Width=Panel5->Width;//-10;
Label6->Left=0;
Label6->Width=Panel5->Width;//-10;
Label11->Left=0;
Label11->Width=Panel5->Width;//-10;
Label12->Left=0;
Label12->Width=Panel5->Width;//-10;
Label15->Left=0;
Label15->Width=Panel5->Width;//-10;
Label19->Left=0;
Label19->Width=Panel5->Width;//-10;
if(spektr->Checked==true){C2-> Width=IW/2;C3->Width=C2->Width;}
if(spektr->Checked==false){C2-> Width=IW;C3->Visible=false;}
C2-> Width=IW/2;
C2->Height=Potk->Height-IH;
C2->Top=IT+IH;
C2->Left=IL;
C3->Width=C2->Width;C3->Height=C2->Height;
C3->Top=C2->Top;
C3->Left=C2->Left+C2->Width;

Potkm->Top      =L1->Top;
Potkm->Height   =L1->Height-32;
Potkm->Width    =Potkm->Height - OBarm->Height;
Potkm->Left     =W - Potkm->Width ;

Caz_otk->Top=Potkm->Height-Caz_otk->Height-10;
Caz_otk->Left=Potkm->Width-10-Caz_otk->Width;

STotk2->Top=Caz_otk->Top+2;
STotk2->Left=5;

Pi->Width=Potkm->Width-8;
Pi->Height=Pi->Width ;
Panel9->Width=Potkm->Width-Cminm->Width-Cmaxm->Width;

par->Height =L1->Height;//Таблица НДМ
par->Top    =L1->Top;
par->Left   =Panel5->Width+Panel5->Left;

Lo2->Width=L0->Width-5;//Колонка ОТК2
Lo2->Height=L1->Height;
Lo2->Top=L1->Top;
Lo2->DefaultColWidth=(Lo2->Width)/(Lo2->ColCount);

//окошки сокетов
int mytop=135, myleft=559;
Pwits->Left=myleft;//все одинаково слева
Pgeo->Left=myleft;
Pleusa->Left=myleft;
Pwits->Top=mytop;
Pgeo->Top=Pwits->Top+Pwits->Height+3;
Pleusa->Top=Pgeo->Top+Pgeo->Height+3;
Pgti->Top=Pleusa->Top;
Pgti->Left=Pleusa->Left;

//PCt->Width  =W - PCt->Left-Potkm->Width-Lo2->Width;
par->Width=W - (Panel5->Width+Panel5->Left) - Potkm->Width - Lo2->Width;
Lo2->Left=(Panel5->Width+Panel5->Left)+(W-(Panel5->Width+Panel5->Left)-Potkm->Width-Lo2->Width);

Potk->Visible=true;
Button3Click(Sender);
Potkm->Visible=true;
Button1Click(Sender);
D0=Date();
DLKADRr=DLMASr+L*DLDIGr;
DLK=DLSP+L*DLD;
DateTimeToString(date0,"Дата: d mmmm yyy г., день - dddd,",D0);
StatusBar->SimpleText=date0;
Label10->Visible=true;
Nfiltr=2;
Co->Visible=true;
Cotkl->Checked=ioo;  spektr->Checked=ioss;  Cotklm->Checked=ioo2;
DLKADRr=DLMASr+L*DLDIGr;
DLK=DLSP+L*DLD;
C2->BottomAxis->Maximum=DLKADRrp;

//Проверяем имя скважины
if(CompareText(ckwName,"")!=0) {CkwChange(Sender);} else  {ShowMessage("Задайте скважину"); return;}
}// fl_prov

TPoint P;
if(PnastrChange==0)//Тут задается ширина окна inc, это первый запуск
{
    Pnastr->Top=1+TB2->Height;
    Pnastr->Left=0;
    P=Pnastr->ClientToScreen(Point(0,0));//???
    Tn.Left=P.x; Tn.Top=P.y;
    Tckw.Left=P.x; Tckw.Top=P.y;
    P=Pnastr->ClientToScreen(Point(Pnastr->UndockWidth,Pnastr->UndockHeight));
    Tn.Right=P.x; Tn.Bottom=P.y;
    Tckw.Right=1180; Tckw.Bottom=P.y;
    //Pnastr->ManualFloat(Tn);
    Pnastr->Visible=false;
    Pnastr->BringToFront();//расположим поверх всех
    SaveNastrLog(Fnastr);//при первом запуске запишем текущие настройки
}
else {PnastrChange=0; Pnastr->Visible=true;}

//тут задается положение панели ID скважины
Fr45->PanelPasport->Top = 1+TB2->Height;
Fr45->PanelPasport->Left = 3+Pnastr->Width;

Pckw->ManualFloat(Tckw);
Pckw->Visible=false;

P=Pcolor->ClientToScreen(Point(0,0));
Tn.Left=P.x;
Tn.Top=P.y;
P=Pcolor->ClientToScreen(Point(Pcolor->UndockWidth,Pcolor->UndockHeight));
Tn.Right=P.x;
Tn.Bottom=P.y;
Pcolor->ManualFloat(Tn);    Pcolor->Visible=false;
//------------------
if(CompareText(Eglubina->Text,"")!=0)glubina=MyStrToFloat(Eglubina->Text);
else ShowMessage("Введите глубину");
azimut=-9999; otktek=-9999;  zenit=-9999; obor=-9999;
tem=-9999;az_otk=-9999; vib=-9999; gk=-9999;
tok=-9999;  NAPR=-9999;  wr=-9999;   modg=-9999; Azk=-9999;
 dat_o1=-9999; dat_o2=-9999; dat_o3=-9999; dat_o4=-9999;
 dat_oa1=-9999;dat_oa2=-9999;dat_oa3=-9999; dat_oa4=0;
 GKup=GKdn=ZUndm=NAGRndm=TOKndm=OBORndm=TEMPndm=PEAKndm=KCndm=MOMndm=MAHndm=MAHbatt=indic=DAVLndm=sigKKS=-9999;
 nap1=gkzts=Htotal=-9999;
//--------------------------------
xc0=IL+20; xc1=IL+IW-20; yc1=IT+20; yc0=IT+IH-20;  yc00=(yc1+yc0)/2;
Panel2->Left=xc0;Panel2->Top=yc0;
Label10->Top=yc00;  Label10-> Left=IL+2;
NCOMh=NCOM;
int j=LastDelimiter("(",NCOM);
if(j>0) NCOMh=Trim( AnsiString(NCOMh.SubString(1,j-1)));
else  NCOMh=NCOM;
Fr45->Caption=TITLE+ver+ver2+", скважина: "+tpathckw+"\\"+ckwName+",   датчик: " ;
Fr45->Caption=Fr45->Caption+tar+", "+Sport->Text;

j=LastDelimiter("(",NCOMPB);//найдем индекс скобки
if(j>0) NCOMPBh=Trim(NCOMPB.SubString(1,j-1));//обрежем до скобки и уберем пробелы
else  NCOMPBh=NCOMPB;

dllInstance =LoadLibrary("muladd.dll");
if(dllInstance)
{
 CheckSSE=(FType1 *)GetProcAddress(dllInstance ,"CheckSSE");
 if(CheckSSE)
 {
  CheckSSE(&dst);
  if(dst!=0) FloatMulAdd32no16=(FType *)GetProcAddress(dllInstance ,"FloatMulAdd32no16");
  if(!FloatMulAdd32no16) dst=0;
 }
}

Fr45->Caption=TITLE+ver+ver2+", скважина: "+tpathckw+"\\"+ckwName+",   датчик: " ;
Fr45->Caption=Fr45->Caption+tar+", "+Sport->Text;
C2->Gradient->EndColor=C2end;
 C3->Gradient->EndColor=C3end;
 C2->Gradient->StartColor=C2start;   C2->Color=C2start;
 C3->Gradient->StartColor=C3start;   C3->Color=C3start;
 C2->Gradient->Visible=C2_gr;
  C3->Gradient->Visible=C3_gr;
   C2->Gradient->Direction=dd[C2_dr];
   C3->Gradient->Direction=dd[C3_dr];
 Panel7->Width=W-Panel7->Left;
if(u32->Checked){/*R4->Items->Strings[0]="10";*/ KmB=2.5*256;ur=31;}
else {/*R4->Items->Strings[0]="10" ;*/  KmB=2.5; ur=23;}
  TrackBar1->Max=100*8388607*((KmB/256)*1000/8388607.);
if(uco32){//R4->Items->Strings[0]="10";
  KmB=2.5*256;ur=31; TrackBar1->Max=100*8388607*((KmB/256)*1000/8388607.);}
else {//R4->Items->Strings[0]="10" ;
  KmB=2.5; ur=23;
  TrackBar1->Max=100*8388607*((KmB)*1000/8388607.);}
TrackBar1->Position=ipos; 
mashvs=ipos;

//заполняем заголовки таблицы НДМ
if(FORMAT_KADRA==FORMAT_NDM_GK_RETR)//в режиме 2 (НДМ)
{
	Spar->Cells[0][0]=" Индик";
	Spar->Cells[1][0]=" ЗУндм";
	Spar->Cells[2][0]="ГКверх";
	Spar->Cells[3][0]=" ГКниз";
	Spar->Cells[4][0]=" Темп";
	Spar->Cells[5][0]="Об/мин";
	Spar->Cells[6][0]="   F, т";
	Spar->Cells[7][0]="Ток, мА";
	Spar->Cells[8][0]="    mV";
	Spar->Cells[9][0]=" dU,мВ";
	Spar->Cells[10][0]="    кН";
	Spar->Cells[11][0]="  МПа";
	Spar->Cells[12][0]="Режим";
}
else
{
	Spar->Cells[0][0]="";
	Spar->Cells[1][0]="";
	Spar->Cells[2][0]="";
	Spar->Cells[3][0]="";
	Spar->Cells[4][0]="";
	Spar->Cells[5][0]="";
	Spar->Cells[6][0]="";
	Spar->Cells[7][0]="";
	Spar->Cells[8][0]="";
	Spar->Cells[9][0]="";
	Spar->Cells[10][0]="";
	Spar->Cells[11][0]="";
	Spar->Cells[12][0]="Режим";
}
Spar->RowCount = rowCountNDM;
if(Cplayer->Checked) PanelPlayer->Visible = true; else PanelPlayer->Visible = false;
}
//---------------------------------------------------------------------------
void __fastcall TFr45::CussClick(TObject *Sender)
{
ius=Cuss->ItemIndex;
if(fl_Th) if(port!=INVALID_HANDLE_VALUE)
{
 iprov=0;
 while(fl_Th){};
 Thread->Terminate();
 startClick(Sender);
}
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TFr45::EdzKeyPress(TObject *Sender, char &Key)
{if(Key==VK_RETURN) {Panel6Click(Sender);Key=NULL; return;}
char s[]={'0','1','2','3','4','5','6','7','8','9','0' };
WORD kk=Key;
if(kk==8)return;
int ikey=0;
for (int i=0;i<11;i++)if(Key==s[i])ikey=1;
if(ikey==0){Key=NULL;  PlaySound("MYS2",0,SND_RESOURCE);  }
}
//---------------------------------------------------------------------------
void N_USO(AnsiString w,int *ssn)
{ HANDLE iport=NULL;
 int iierr;
 COMMTIMEOUTS  cto;// {структура тайм-аутов COM-порта}
DCB    cdcb;// {блок параметров COM-порта}
unsigned char buf;
unsigned long kolb;
iierr=0;
*ssn=0;
w= "\\\\.\\"+w;
iport=CreateFile(w.c_str(),GENERIC_READ | GENERIC_WRITE ,0,NULL,OPEN_EXISTING,0,NULL);
if(iport!=INVALID_HANDLE_VALUE)
  {     if(! GetCommTimeouts(iport,&cto))iierr=1;
     else { cto.ReadTotalTimeoutConstant=20;
     cto.WriteTotalTimeoutConstant=20;
     if(! SetCommTimeouts(iport,&cto))iierr=2;
     else {
     if (!GetCommState(iport,&cdcb)) iierr=3;
     else {
     cdcb.BaudRate=CBR_57600;//скорость передачи данных
     cdcb.ByteSize=8; //число информационных бит
     cdcb.Parity=NOPARITY;//бит четности отсутствует
     cdcb.StopBits=ONESTOPBIT;//один стоnовый бит
     if (!SetCommState(iport,&cdcb)) iierr=4;
     else {
     PurgeComm(iport,PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);
     }  }  } }

     if(iierr==0)
 {  TransmitCommChar(iport,0xAA);
      unsigned char b=0xAA;
      //WriteFile(iport,&b,1,&kolb,NULL);
      ReadFile(iport,&buf,1,&kolb,NULL);
  buf=0x55; kolb=1;
  if( buf==0x55 &&kolb==1)
  {
   b=0x66;
   WriteFile(iport,&b,1,&kolb,NULL);
   if(kolb==1)
    {
    unsigned char buf20[22];
    ReadFile(iport,&buf20[0],5,&kolb,NULL);
    if(kolb==5&& buf20[0]==0x01 && buf20[4]==0xFF)
    *ssn=(int)buf20[1];
    }
  }
 }
 CloseHandle(iport);
 }
}
//---------------------------------------------------------------------------
void __fastcall TFr45::SportDblClick(TObject *Sender)
{
int ssn=0;
int i,ncom=0;

 Sport->Items->Clear();
 int majorVersion = Win32MajorVersion;//определяем версию windows, 5=XP
 //если старше XP, то через cmd
 if(majorVersion>5) ncom=port_list_cmd();//получим имена портов через cmd, лежат в MemoLas
 else ncom=port_list_reg();//иначе через реестр

 for (int i = 0; i< ncom; i++)
 {
   N_USO(MemoLas->Lines->Strings[i],&ssn);
   if(ssn!=0) Sport->Items->Add(MemoLas->Lines->Strings[i]+"(USB УСО №"+ssn+")");
   else  Sport->Items->Add(MemoLas->Lines->Strings[i]+"(?)");
 }
 Fr45->MemoLas->Lines->Clear();

 for(int i=0;i<Sport->Items->Count;i++)
 if(CompareStr(AnsiString(NCOM.SubString(1,6)),AnsiString(Sport->Items->Strings[i].SubString(1,6)))==0)
  {
   Sport->ItemIndex=i;
   Sport->Text=Sport->Items->Strings[i];
   NCOM=Sport->Items->Strings[i];
   int j=LastDelimiter("(",NCOM);//найдем индекс скобки
   if(j>0) NCOMh=Trim(NCOM.SubString(1,j-1));//обрежем до скобки и уберем пробелы
   else  NCOMh=NCOM;
  }
}
//---------------------------------------------------------------------------
void __fastcall TFr45::C3MouseUp(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{ if(Button==mbRight){
C3->BottomAxis->Minimum=0;
C3->BottomAxis->Maximum=130;  }
if( iccc==1) {  iccc=0;
Pcolor->Visible=true; Pcolor->Caption="Панель спектра";
 Pstart->Color=C3start; Pend->Color=C3end;
Cgr->Checked=C3_gr; ComboBox1->ItemIndex=C3_dr;}

}
//---------------------------------------------------------------------------
void __fastcall TFr45::C2MouseUp(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{if(Button==mbRight){
C2->BottomAxis->Minimum=0;
C2->BottomAxis->Maximum=DLKADRrp;
C2->LeftAxis->Minimum=-1;
C2->LeftAxis->Maximum=1;  }
if( iccc==1) {  iccc=0;
Pcolor->Visible=true;  Pcolor->Caption="Панель корреляции";
 Pstart->Color=C2start; Pend->Color=C2end;
Cgr->Checked=C2_gr; ComboBox1->ItemIndex=C2_dr;
}

}
//---------------------------------------------------------------------------
void __fastcall TFr45::Label3Click(TObject *Sender)
{
 SportDblClick(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFr45::L0Click(TObject *Sender)
{
L0->Row=L0->RowCount-1;  L0->LeftCol=0;     L0->TopRow=2;
}
//---------------------------------------------------------------------------
void __fastcall TFr45::L1MouseMove(TObject *Sender, TShiftState Shift,
      int X, int Y)
{Lz->Visible=true;}
//---------------------------------------------------------------------------
void __fastcall TFr45::n(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
if(Y<L1->DefaultRowHeight)Lz->Visible=!Lz->Visible;//ыкл/выкл выпадающего списка
}
//---------------------------------------------------------------------------
void __fastcall TFr45::Label4MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
Lo->Visible=true;
}
//---------------------------------------------------------------------------

void __fastcall TFr45::Label4MouseUp(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
Lo->Visible=false;
}
//---------------------------------------------------------------------------
void __fastcall TFr45::Label5MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
 Lte->Visible=true;
}
//---------------------------------------------------------------------------

void __fastcall TFr45::Label6MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
Lnapr->Visible=true;
}
//---------------------------------------------------------------------------

void __fastcall TFr45::Label5MouseUp(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
Lte->Visible=false;
}
//---------------------------------------------------------------------------

void __fastcall TFr45::Label6MouseUp(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
Lnapr->Visible=false;
}
//---------------------------------------------------------------------------
void __fastcall TFr45::FormPaint(TObject *Sender)
{ TB2-> Align=alTop;
Canvas->Pen->Color=clBlack;
Canvas->Brush->Color=cform;
 Canvas->Rectangle(xc0,yc0,xc1,yc1);
 Canvas->MoveTo(xc0,yc0);
   Canvas->Pen->Color=clSilver;     Canvas->Brush->Color=Fr45->Color;
  Canvas->MoveTo(xc0+1,yc00);  Canvas->LineTo(xc1-1,yc00);
Canvas->Pen->Color=clBlack;
 mashvs=TrackBar1->Position;

mashvst=100.*(yc00-yc1)/mashvs;
  Canvas->TextOut(IL+4,IT+2,"                ");
Canvas->TextOut(IL+4,IH+IT-16,"                           ");
 Canvas->TextOut(IL+4,IT+2,FloatToStrF0(mashvs/100.,8,2));
Canvas->TextOut(IL+4,IH+IT-16,FloatToStrF0(-mashvs/100.,8,2));
 Canvas->MoveTo(xt,yt);
}
//---------------------------------------------------------------------------
void __fastcall TFr45::Timer1Timer(TObject *Sender)
{
    Timer1->Enabled=false;
    Label8->Caption="";//чистка верхней строки состояния
}
//---------------------------------------------------------------------------
//запускается из YSO8 при переполнении или в конце
void __fastcall TFr45::Timer2Timer(TObject *Sender)
{
    Timer2->Enabled=false;

    if(StatusPlayer)//если включен плеер
    {	stopClick(Sender);//останавливаем все
    }
    else //если переполнение или др. ошибка
    {
    	if(fl_Th)//если поток запущен
    	{stopClick(Sender);//останавливаем потоки
     	 //понижаем усиление и стартуем снова
     	 if(ius>0)
     	 {ius=ius-1;
      	  Cuss->ItemIndex=ius;
      	  startClick(Sender);//стартуем опять
      	  start->Down=true;
     	 }
    	}
    	else stopClick(Sender);//останавливаем потоки в другом случае
    	Timer1->Enabled=true;//запускаем очистку верхней строки состояния
    }
}
//---------------------------------------------------------------------------
void __fastcall TFr45::L1aMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
if(Y < L1a->DefaultRowHeight) La->Visible = !La->Visible;//ыкл/выкл выпадающего списка
}
//---------------------------------------------------------------------------
void __fastcall TFr45::L1aClick(TObject *Sender)
{
L1a->Row=L1a->RowCount-1;  L1a->LeftCol=0;     L1a->TopRow=2;
}
//---------------------------------------------------------------------------

void __fastcall TFr45::L1Click(TObject *Sender)
{
L1->Row=L1->RowCount-1;  L1->LeftCol=0;     L1->TopRow=2;
}
//---------------------------------------------------------------------------

void __fastcall TFr45::SnKeyPress(TObject *Sender, char &Key)
{
 if(Sn->Row==2||Sn->Row==4||Sn->Row==6||Sn->Row==9||Sn->Row==11||Sn->Row==13||Sn->Row==14||Sn->Row==15||Sn->Row==17||Sn->Row==20)
 {
  //дробные положительные числа
  char s[]={'0','1','2','3','4','5','6','7','8','9','.'};
  int ikey=0;
  for (int i=0;i<sizeof(s);i++) if(Key==s[i])ikey=1;
  if(ikey==1 && Key=='.')
  {String str=Sn->Cells[1][Sn->Row];
   int i=0, b=0;
   while(i++ < str.Length()) {if(str.IsDelimiter('.', i)) b++;}
   if(b>0) ikey=0;
  }
  if(ikey==0){Key=NULL; PlaySound("MYS2",0,SND_RESOURCE);}
 }
 //целые числа
 else if(Sn->Row==0||Sn->Row==1||Sn->Row==3||Sn->Row==5||Sn->Row==7||Sn->Row==8||Sn->Row==12||Sn->Row==16||Sn->Row==18)
 {
  char s[]={'0','1','2','3','4','5','6','7','8','9' };
  int ikey=0;
  for (int i=0;i<sizeof(s);i++) if(Key==s[i])ikey=1;
  if(ikey==0){Key=NULL; PlaySound("MYS2",0,SND_RESOURCE); }
 }
 //для знаковых float
 else if(Sn->Row==10||Sn->Row==19)
 {
  char s[]={'0','1','2','3','4','5','6','7','8','9','.','-',0x08};
  int ikey=0;
  for (int i=0;i<sizeof(s);i++) if(Key==s[i])ikey=1;
  if(ikey==1 && Key=='.')
  {String str=Sn->Cells[1][Sn->Row];
   int i=0, b=0;
   while(i++ < str.Length()) {if(str.IsDelimiter('.', i)) b++;}
   if(b>0) ikey=0;
  }
  if(ikey==1 && Key=='-')
  {AnsiString str=Sn->Cells[1][Sn->Row]+Key;
   if(str.Length()>1)
   {
    float f;
    if(!TryStrToFloat(str,f)) ikey=0;
   }
  }
  if(ikey==0){Key=NULL; PlaySound("MYS2",0,SND_RESOURCE);}
 }
 else {Key=NULL; PlaySound("MYS2",0,SND_RESOURCE); }
}
//---------------------------------------------------------------------------
/*void __fastcall TFr45::Label8Click(TObject *Sender)
{
 Label8->Caption="";
}*/
//---------------------------------------------------------------------------

void __fastcall TFr45::FormCloseQuery(TObject *Sender, bool &CanClose)
{
float mkonec_zts,mkonec_gkzts,mkonec_gkndm,ums;
double Gt_opor;
stopClick(Sender);
Timer4->Enabled=false;
Timer8->Enabled=false;
TimerGti->Enabled=false;
if(PotokON) {PotokON=0; while(!PotokOFF)Application->ProcessMessages();}//останавливаем поток
Fr45->TcpClient1->Active=false;
ClientSocket1->Close();
ServerSocket1->Close();
ClientSocket1->Active=false;
ClientSocket2->Active=false;
ClientSocketGti->Active=false;
ServerSocket1->Active=false;
SrvLOG->Close();
SrvLOG->Active=false;
SetCurrentDir(CoreDir);
SaveLog("Останов программы========================================================");

 ofstream ifile1(NASTR_FILE_NAME);
 if(ifile1==NULL){ShowMessage("Ошибка открытия файла "+(AnsiString)NASTR_FILE_NAME); return;}

 if(CompareText(Sn->Cells[1][0],"")!=0)mrmt=StrToInt(Sn->Cells[1][0]);
 else mrmt=0;
 if(CompareText(Sn->Cells[1][3],"")!=0)DCP=StrToInt(Sn->Cells[1][3]);
 else DCP=0;
 if(CompareText(Sn->Cells[1][1],"")!=0)azorien=StrToInt(Sn->Cells[1][1]);
 else azorien=0;
 if(CompareText(Sn->Cells[1][5],"")!=0)Hop=StrToInt(Sn->Cells[1][5]);
 else Hop=54300;
 if(CompareText(Sn->Cells[1][9],"")!=0)Kgeo=MyStrToFloat(Sn->Cells[1][9]);
 else Kgeo=1.0;
 if(CompareText(Sn->Cells[1][10],"")!=0)NAGRndm_null=MyStrToFloat(Sn->Cells[1][10]);
 else NAGRndm_null=0.0;
 if(CompareText(Sn->Cells[1][11],"")!=0)Kgk_ndm=MyStrToFloat(Sn->Cells[1][11]);
 else {Kgk_ndm=1.0; Sn->Cells[1][11]=FloatToStrF0(Kgk_ndm,7,3);}
 if(CompareText(Sn->Cells[1][17],"")!=0)Kgk_zts=MyStrToFloat(Sn->Cells[1][17]);
 else {Kgk_zts=1.0; Sn->Cells[1][17]=FloatToStrF0(Kgk_zts,7,3);}
 nport=StrToInt(Sn->Cells[1][7]);
 nap=MyStrToFloat(Sn->Cells[1][6]);
 zuorien=MyStrToFloat(Sn->Cells[1][2]);
 nport_geo=StrToInt(Sn->Cells[1][12]);
 mkonec_zts=get_mkonec_zts();
 mkonec_gkzts=get_mkonec_gkzts();
 mkonec_gkndm=get_mkonec_ndm();
 Port_Leuza=StrToInt(Sn->Cells[1][16]);
 port_gti=StrToInt(Sn->Cells[1][18]);
 ums = get_ums();
 mn = get_umn();
 Gt_opor = get_Gt_opor();
    if(CompareText(ckwName,"")==0)ckwName="default";
    ifile1<<(tpathckw+"\\"+ckwName).c_str()<<endl;
     ifile1<<tar.c_str()<<endl;
     ifile1<<skvtxt.c_str()<<endl;
     if(CompareText(NCOM,"")==0)NCOM="-"; ifile1<<NCOM.c_str()<<endl;
    DZ=MyStrToFloat(Edz->Text);
 ifile1<</*R5->ItemIndex*/0<<" "<<StrToInt(Epausa->Text)<<" " <<prib<<" "<<com<<" "<<glubina<<" "<<DZ<<" "<<zuorien
 <<" "<<azorien<<" "<<mn<<" "<<Hop<<" "<<nap<<" "<<mgk<<" "
 <<" "<<otkmin<<" "<<otkmax<<" "<<otkminm<<" "<<otkmaxm<<" "<<mrmt
 <<"   "<<freq<<"  "<<0<<" "<<" "<<DCP<<"  "<<ius<<
 " "<<TrackBar1->Position<<" "<<u32->Checked<<"  "<<cform<<endl;
 ifile1<<C2start<<" "<<C2end<<" "<<C2_gr<<" "<<C2_dr<<endl;
  ifile1<<C3start<<" "<<C3end<<" "<<C3_gr<<" "<<C3_dr<<endl;
 ifile1<<Ctstart<<" "<<Ctend<<" "<<Ct_gr<<" "<<Ct_dr<<endl;
 ifile1<<c1<<" "<<c2<<endl;
 ifile1<<c1m<<" "<<c2m<<endl;
 ifile1<<Caz_otk->Checked <<endl;
 ifile1<<Sv->Checked<<"  ";
 ifile1<<Sv2->Checked<<"  ";
 ifile1<<Cotkl->Checked<<"  ";
 ifile1<<spektr->Checked<<"  ";
 ifile1<<Cotklm->Checked<<"  ";
 ifile1<<Caz_otk->Checked<<endl;
 ifile1<<nport<<"  "<<RadioGroup1->ItemIndex<<"  "<<nulotk<<"  "<<Kgeo<<"  "<<NAGRndm_null<<"  "<<Kgk_ndm<<"  ";
 ifile1<<nport_geo<<"  "<<mkonec_zts<<"  "<<mkonec_gkzts<<"  "<<mkonec_gkndm<<"  ";
 ifile1<<SourceGlub->ItemIndex<<"  "<<PB->Checked<<endl;
 if(CompareText(Czis->Text,"")==0)ifile1<<"-"<<endl;
 else ifile1<<Czis->Text.c_str()<<endl;
 if( fabs(TUSO-TDISCUSO)>=0.000013)TUSO=TDISCUSO;
 ifile1<<FloatToStr0(TUSO).c_str()<<endl;
 if(CompareText(Cleusa->Text,"")==0) ifile1<<"-"<<endl;
 else ifile1<<Cleusa->Text.c_str()<<endl;
 if(CompareText(NCOMPB,"")==0) NCOMPB="-";
 ifile1<<NCOMPB.c_str()<<endl;
 ifile1<<Cl2->Checked<<endl;//WITS клиент
 ifile1<<Port_Leuza<<endl;//порт Леузы
 ifile1<<PBRadioGroup->ItemIndex<<endl;//какой ОТК посылать в ПБ
 ifile1<<CBndm->Checked<<endl;
 ifile1<<CBnnk->Checked<<endl;
 ifile1<<CBretr->Checked<<endl;
 ifile1<<Kgk_zts<<endl;
 ifile1<<CBgkzts->Checked<<endl;
 ifile1<<RGcoder->ItemIndex<<endl;
 ifile1<<port_gti<<endl;//порт ГТИ
 if(CompareText(Cgti->Text,"")==0) ifile1<<"-"<<endl;//IP ГТИ
 else ifile1<<Cgti->Text.c_str()<<endl;
 if(CompareText(Cwits->Text,"")==0) ifile1<<"-"<<endl;//IP WITS клиент
 else ifile1<<Cwits->Text.c_str()<<endl;
 ifile1<<Nubt->Checked<<endl;//НУБТ
 ifile1<<ums<<endl;//Склонение
 ifile1<<CBLog->Checked<<endl;//LOG-сервер клиент
 ifile1<<FloatToStrF0(Gt_opor,7,6).c_str()<<endl;//Gt из паспорта прибора
 ifile1<<Cname->Text.c_str()<<endl;//Имя телесистемы
 ifile1<<PBType->ItemIndex<<endl;//тип ПБ
 ifile1<<PBrotation<<endl;//счетчик вращений ПБ
 ifile1<<CBgeomash->Checked<<endl;//Геомаш
 ifile1.close();

 CanClose=true;
}
//---------------------------------------------------------------------------
void __fastcall TFr45::Label11MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
Lvib->Visible=true;
}
//---------------------------------------------------------------------------

void __fastcall TFr45::Label11MouseUp(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
Lvib->Visible=false;
}
//---------------------------------------------------------------------------

void __fastcall TFr45::Label12MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
Lgk->Visible=true;
}
//---------------------------------------------------------------------------

void __fastcall TFr45::Label12MouseUp(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
Lgk->Visible=false;
}
//---------------------------------------------------------------------------
void __fastcall TFr45::SnDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect, TGridDrawState State)
{if(RedC==0)return;
 if((RedC==1||RedC==3)&&ARow==1&&ACol==1)//азимут ориентировочно
 { 	Sn->Canvas->Font->Color=clRed;
 	Sn->Canvas->TextOut(Rect.Left+2,Rect.Top+1,Sn->Cells[1][1]);
 	Sn->Canvas->Font->Color=clBlack;
 }
 if((RedC==2||RedC==3)&&ARow==2&&ACol==1)//зенит ориентировочно
 {	Sn->Canvas->Font->Color=clRed;
 	Sn->Canvas->TextOut(Rect.Left+2,Rect.Top+1,Sn->Cells[1][2]);
 	Sn->Canvas->Font->Color=clBlack;
 }
}
//---------------------------------------------------------------------------
//проверка на запрещенные символы в имени скважины
void __fastcall TFr45::CkwKeyPress(TObject *Sender, char &Key)
{
for (int c=0;c<sizeof(ForbiddenSymbols);c++)
{if (Key==ForbiddenSymbols[c]) {Key=NULL;   PlaySound("MYS2",0,SND_RESOURCE);}
}
}
//---------------------------------------------------------------------------
//нажатие на Enter после ввода ID скважины
void __fastcall TFr45::CkwKeyUp(TObject *Sender, WORD &Key, TShiftState Shift)
{if (Key == VK_RETURN) CkwChange(Sender);}//обновляем файлы скважины
//---------------------------------------------------------------------------
void __fastcall TFr45::C2DblClick(TObject *Sender)
{ iccc=1; ippp=2;}
//---------------------------------------------------------------------------
void __fastcall TFr45::C3DblClick(TObject *Sender)
{
iccc=1; ippp=3;
}
//---------------------------------------------------------------------------
void __fastcall TFr45::FormDblClick(TObject *Sender)
{
if (ColorDialog1->Execute())
{
	cform=ColorDialog1->Color;
 	FormPaint(Sender) ;
}
}
//---------------------------------------------------------------------------
void __fastcall TFr45::BstartClick(TObject *Sender)
{
if(ippp==2){ if (ColorDialog1->Execute())
   {C2start=ColorDialog1->Color;C2->Gradient->StartColor=C2start;
   Pstart->Color=C2start;
   C2->Color=C2start;}   }
   if(ippp==3){ if (ColorDialog1->Execute())
   {C3start=ColorDialog1->Color;C3->Gradient->StartColor=C3start;
   Pstart->Color=C3start;
   C3->Color=C3start;}   }
   if(ippp==4){ if (ColorDialog1->Execute())
   {Ctstart=ColorDialog1->Color;
   Pstart->Color=Ctstart;

   }   }
}
//---------------------------------------------------------------------------

void __fastcall TFr45::BendClick(TObject *Sender)
{
if(ippp==2){ if (ColorDialog1->Execute())
   {C2end=ColorDialog1->Color;C2->Gradient->EndColor=C2end;
    Pend->Color=C2end;      }  }
    if(ippp==3){ if (ColorDialog1->Execute())
   {C3end=ColorDialog1->Color;C3->Gradient->EndColor=C3end;
    Pend->Color=C3end;      }  }
    if(ippp==4){ if (ColorDialog1->Execute())
   {Ctend=ColorDialog1->Color;
    Pend->Color=Ctend;      }  }
}
//---------------------------------------------------------------------------

void __fastcall TFr45::CgrClick(TObject *Sender)
{ if(ippp==2){ C2_gr=Cgr->Checked;
  C2->Gradient->Visible=Cgr->Checked;
  Bend->Enabled=Cgr->Checked; ComboBox1->Enabled=Cgr->Checked; }
   if(ippp==3){ C3_gr=Cgr->Checked;
  C3->Gradient->Visible=Cgr->Checked;
  Bend->Enabled=Cgr->Checked; ComboBox1->Enabled=Cgr->Checked; }
   if(ippp==4){ Ct_gr=Cgr->Checked;
  Bend->Enabled=Cgr->Checked; ComboBox1->Enabled=Cgr->Checked; }


  }
//---------------------------------------------------------------------------


void __fastcall TFr45::ComboBox1Change(TObject *Sender)
{
if(ippp==2){C2->Gradient->Direction=dd[ComboBox1->ItemIndex]; C2_dr=ComboBox1->ItemIndex; }
if(ippp==3){C3->Gradient->Direction=dd[ComboBox1->ItemIndex]; C3_dr=ComboBox1->ItemIndex; }
}
//---------------------------------------------------------------------------

void __fastcall TFr45::PcolorClick(TObject *Sender)
{
//
}
//---------------------------------------------------------------------------
void __fastcall TFr45::RnaprClick(TObject *Sender)
{
if(Rnapr->ItemIndex==0)napr=1;
if(Rnapr->ItemIndex==1)napr=-1;
if(Potk->Visible){
Button3Click(Sender);}
}
//---------------------------------------------------------------------------

void __fastcall TFr45::CotklmClick(TObject *Sender)
{if(Cotklm->Checked){Potkm->Visible=true; Lo2->Visible=true;}
else
{ Potkm->Visible=false;
 Lo2->Visible=false;
}

}
//---------------------------------------------------------------------------

void __fastcall TFr45::Panel9Click(TObject *Sender)
{
if(itm==1)Potkm->ManualFloat(Tm);
}
//---------------------------------------------------------------------------

void __fastcall TFr45::PotkmResize(TObject *Sender)
{
Panel9->Width=Potkm->Width-Cminm->Width-Cmaxm->Width;
int i=Potkm->Width-8;
int j= Potkm->Height-OBarm->Height;
Pi->Width=min(i,j);
Pi->Height=Pi->Width ;
BM->Width=Pi->Width; BM->Height= Pi->Width;
om=BM->Width/2;  radm=4*BM->Width/10;
I2m->Canvas->FillRect(Rect(0,0,I2->Width,I2->Height));
I1m->Canvas->FillRect(Rect(0,0,I1->Width,I1->Height));
 if(Potkm->Visible) Button1Click(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFr45::Button1Click(TObject *Sender)
{
  BM->Canvas->FillRect(Rect(0,0,BM->Width,BM->Height));
  BM->Canvas->Pen->Width=1;
  BM->Canvas->Brush->Color=c1m;
  BM->Canvas->Pen->Color=c1m;
  BM->Canvas->Ellipse( om-radm,om-radm,om+radm,om+radm);
  BM->Canvas->Brush->Color=c2m;
  int x3,x4,y3,y4;
  if(naprm==1)
  {
    x3=om-(radm)*sin(M_PI*(360-otkminm)/180) ;
    y3=om-(radm)*cos(M_PI*(360-otkminm)/180);
    x4=om+(radm)*sin(M_PI*otkmaxm/180) ;
    y4=om-(radm)*cos(M_PI*otkmaxm/180);
  }
  if(naprm==-1)
  {
    x4=om-(radm)*sin(M_PI*otkminm/180) ;
    y4=om-(radm)*cos(M_PI*otkminm/180);
    x3=om+(radm)*sin(M_PI*(360-otkmaxm)/180) ;
    y3=om-(radm)*cos(M_PI*(360-otkmaxm)/180);
  }
  BM->Canvas->Pen->Color=c1;
  BM->Canvas->Pie( om-radm,om-radm,om+radm,om+radm,x3,y3,x4,y4);
  BM->Canvas->Pen->Color=clBlack;
  BM->Canvas->Brush->Color=Potk->Color;
  BM->Canvas->TextOut(om-4,om-radm-14,"0");
  BM->Canvas->TextOut(om-10,om+radm+4,"180");
  if(naprm==1)
  {BM->Canvas->TextOut(om+radm+4,om-7,"90");
   BM->Canvas->TextOut(om-radm-20,om-5,"270");
  }
  if(naprm==-1)
  {BM->Canvas->TextOut(om+radm+4,om-7,"270");
   BM->Canvas->TextOut(om-radm-20,om-5,"90");
  }
  for(int i=0;i<360;i+=30)
  {
   int x=om+(radm)*cos(M_PI+M_PI/180*i) ;
   int y=om+(radm)*sin(M_PI+M_PI/180*i);
   int x1=om+(radm-5)*cos(M_PI+M_PI/180*i) ;
   int y1=om+(radm-5)*sin(M_PI+M_PI/180*i);
   BM->Canvas->MoveTo(x1,y1);
   BM->Canvas->LineTo(x,y);
  }
  I1m->Picture->Assign(BM);
  if(iotkm>=0)
  {
   BM->Canvas->FillRect(Rect(0,0,BM->Width,BM->Height));
   BM->Canvas->Pen->Width=2;
   int j=0,xo,yo;
   for(int i=4-iotkm;i<5;i++)
   {
    if(naprm==1)
    {
     xo=om-((5-j)*radm/5)*sin(M_PI*(360-newotm[i])/180) ;
     yo=om-((5-j)*radm/5)*cos(M_PI*(360-newotm[i])/180);
    }
    if(naprm==-1)
    {
     xo=om-((5-j)*radm/5)*sin(M_PI*(newotm[i])/180) ;
     yo=om-((5-j)*radm/5)*cos(M_PI*(newotm[i])/180);
    }
    if(i>4-iotkm) BM->Canvas->Pen->Width=1;
    BM->Canvas->MoveTo(om,om);
    BM->Canvas->LineTo(xo,yo);
    j++;
   }
   BM->Canvas->Pen->Width=1;
   I2m->Picture->Assign(BM);
  }
}
//---------------------------------------------------------------------------

void __fastcall TFr45::CminmChange(TObject *Sender)
{
otkminm=Cminm->Value;
 if(Potkm->Visible)Button1Click(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFr45::CmaxmChange(TObject *Sender)
{
otkmaxm=Cmaxm->Value;
 if(Potkm->Visible)Button1Click(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFr45::RnaprmClick(TObject *Sender)
{
if(Rnaprm->ItemIndex==0) naprm=1;
if(Rnaprm->ItemIndex==1) naprm=-1;
if(Potkm->Visible) Button1Click(Sender);
}
//---------------------------------------------------------------------------
//таскание форм мышкой
void __fastcall TFr45::I2mMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
 if(c2m==I1m->Canvas->Pixels[X][Y]||c1m==I1m->Canvas->Pixels[X][Y])
 {
  if (ColorDialog1->Execute())
  {
   I1m->Canvas->Brush->Color=ColorDialog1->Color;
   if(c1m==I1m->Canvas->Pixels[X][Y])c1m=ColorDialog1->Color;
   if(c2m==I1m->Canvas->Pixels[X][Y])c2m=ColorDialog1->Color;
   I1m->Canvas->FloodFill(X,Y,I1m->Canvas->Pixels[X][Y],fsSurface);
  }
 }
 else
 {
  if(Rnaprm->Visible==false)
  {
   TRect Tt;
   TPoint P;
   P=Rnaprm->ClientToScreen(Point(0,0));
   Tt.Left=P.x;
   Tt.Top=P.y;
   P=Rnaprm->ClientToScreen(Point(Rnaprm->UndockWidth,Rnaprm->UndockHeight));
   Tt.Right=P.x;
   Tt.Bottom=P.y;
   Rnaprm->ManualFloat(Tt);RnaprmClick(Sender);
  }
 }
}
//---------------------------------------------------------------------------
void __fastcall TFr45::TimerClick(TObject *Sender)
{
if( Timer->Down==false) { Edit7->Text=IntToStr(cek0); Timer3->Enabled=false;}
 if( Timer->Down==true){
Timer3->Enabled=Timer->Down; cek=0;
if(CompareText(Edit7->Text,"")!=NULL)cek=StrToInt(Edit7->Text);
 cek0=cek;    }
}
//---------------------------------------------------------------------------
void __fastcall TFr45::Timer3Timer(TObject *Sender)
{     //   TDateTime tcek=EncodeTime(0,0,1,0);
 // cek=cek-tcek;
 cek=cek-1;
//  Edit6->Text=TimeToStr(cek);
 Edit7->Text=IntToStr(cek);
  if(cek<=0){Timer3->Enabled=false;Timer->Down=false;
    Edit7->Text=IntToStr(cek0);
    PlaySound("MYS1",0,SND_RESOURCE);}
}
//---------------------------------------------------------------------------
void __fastcall TFr45::Label15MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
Lwr->Visible=true;
}
//---------------------------------------------------------------------------
void __fastcall TFr45::Label15MouseUp(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
Lwr->Visible=false;        
}
//---------------------------------------------------------------------------
void __fastcall TFr45::LakoppClick(TObject *Sender)
{
Lakopp->Row=Lakopp->RowCount-1;  Lakopp->LeftCol=0;     Lakopp->TopRow=2;
}
//---------------------------------------------------------------------------

void __fastcall TFr45::LhClick(TObject *Sender)
{
Lh->Row=Lh->RowCount-1;  Lh->LeftCol=0;     Lh->TopRow=2;
}
//---------------------------------------------------------------------------
//смена режима Азимута магнитный-истинный
void __fastcall TFr45::LakoppMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
AnsiString s1, str;
int pos;
	if(Y < Lakopp->DefaultRowHeight)
	{	//Lkopp->Visible = !Lkopp->Visible;//вкл/выкл выпадающего списка
    	MODE_AZ = (MODE_AZ+1)%2;
        if(MODE_AZ==0) {Lakopp->Cells[0][0]="Аз.маг"; STotk2->Caption="Аз.маг";}
        else {Lakopp->Cells[0][0]="Аз.ист"; STotk2->Caption="Аз.ист";}
        //вытащим нужную колонку азимутов из MemoUgol
        if(MemoUgol->Lines->Count>0)
        {	for(int i=0;i<MemoUgol->Lines->Count;i++)//пройдемся по всем строкам MemoUgol
        	{	s1=MemoUgol->Lines->Strings[i];
                str="";
                if(s1!="")//если строка не пуста
                {	int num;
                	if(MODE_AZ==0) num=4;//колонка магнитного азимута
                	else num=5;			//колонка истинного азимута
                	//удалим колонки слева
                	for(int j=0;j<num;j++)
                	{	while(AnsiString(s1).Pos(" ")==1) s1.Delete(1,1);//удалим пробелы слева
						pos=AnsiString(s1).Pos(" ")-1;//узнаем, где заканчивается число
                	    s1.Delete(1,pos);//удалим число из строки
                	}
                    //дошли до нужного числа
                	while(AnsiString(s1).Pos(" ")==1) s1.Delete(1,1);//удалим пробелы слева
                	pos=AnsiString(s1).Pos(" ")-1;//узнаем, где заканчивается число
                	str=AnsiString(s1).SubString(1,pos);//сохраним первое число из строки
                }
                //загрузим значение в Lakopp
                Lakopp->Cells[0][2+i]=str;
                if(i>=12) break;//глубже загружать смысла нет
            }
        }
    }
}
//---------------------------------------------------------------------------

void __fastcall TFr45::LhMouseDown(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
if(Y<Lh->DefaultRowHeight)Lhh->Visible=!Lhh->Visible;
}
//---------------------------------------------------------------------------
void __fastcall TFr45::SnKeyUp(TObject *Sender, WORD &Key, TShiftState Shift)
{
if(Sn->Row==5)Lh->Cells[0][1]="Hop="+Sn->Cells[1][5];

}
//---------------------------------------------------------------------------
void __fastcall TFr45::RadioGroup1Click(TObject *Sender)
{
Eglubina->Font->Color=clBlack;
if(Sv->Checked)//если выбрана глубина внешняя в настройке
{RadioGroup1->Items->Strings[0]="внешняя";}
else
{RadioGroup1->ItemIndex=1;}//принудительно ручная

if(RadioGroup1->ItemIndex==1)//ручная
{Panel3->Enabled=true;//плюсик в добавках
 Panel6->Enabled=true;//ОК в глубине
}
if(RadioGroup1->ItemIndex==0)//внешняя
{Panel3->Enabled=false;//плюсик в добавках
 Panel6->Enabled=false;//ОК в глубине
}
if(RadioGroup1->ItemIndex==0 && START==1) Edz->Enabled=false;//если глубина ВНЕШНЯЯ, притушим добавок
if(RadioGroup1->ItemIndex==1 && START==1) Edz->Enabled=true;//если глубина ВНУТРЕННЯЯ, засветим добавок
}
//---------------------------------------------------------------------------
void FillNetLevel( LPNETRESOURCE xxx,TStrings *L)
{ bool Result; int I;
// NETRESOURCE  xxx;
 HANDLE twx;
 NETRESOURCE  tnr;
   AnsiString WSName;
   NETRESOURCE  x[60];
   unsigned long    EntrReq ;
unsigned long    SizeReq;

 Result= WNetOpenEnum(RESOURCE_GLOBALNET, RESOURCETYPE_ANY,
    RESOURCEUSAGE_CONTAINER,xxx,&twx);
  if (Result== ERROR_NO_NETWORK)
  return;
  if (Result== NO_ERROR)
  {
 EntrReq = 1;
SizeReq= sizeof(TNetResource) * 59;
     while ((twx!=0) &&
      (WNetEnumResource(twx,&EntrReq,x,&SizeReq) != ERROR_NO_MORE_ITEMS) )
      { for (int i= 0;i<(int)EntrReq;i++)
                 {
             Move(&x[i], &tnr, sizeof(tnr));
       if (tnr.dwDisplayType==2)
                                 {


              if (tnr.lpRemoteName!="")
                WSName = tnr.lpRemoteName;
              else
                WSName = tnr.lpComment;
                L->Add(WSName.SubString(3,WSName.Length() ));


                                         }
        else   FillNetLevel(&tnr, L);
                 } //for i



      } // while
    WNetCloseEnum(twx);

  }

 }
//---------------------------------------------------------------------------
void __fastcall TFr45::CzisDropDown(TObject *Sender)
{
Czis->Items->Clear();
FillNetLevel(0,Czis->Items);
}
//---------------------------------------------------------------------------
void __fastcall TFr45::Button2Click(TObject *Sender)
{
  nport_geo=StrToInt(Sn->Cells[1][12]);
  if((nport_geo<134)||(nport_geo>65535)) {ShowMessage("Номер порта должен быть в диапазоне 134-65535"); return;}
  ClientSocket1->Port= nport_geo;
  ClientSocket1->Host= Czis->Text;
  ClientSocket1->Active=true;
  cerver->Caption="Ждите! Идет подключение к хосту "+ClientSocket1->Host;
  Label8->Caption="";
}
//---------------------------------------------------------------------------
void __fastcall TFr45::ClientSocket1Connect(TObject *Sender, TCustomWinSocket *Socket)
{
 cerver->Caption="Геосенсор подключен к порту "+IntToStr(ClientSocket1->Port);
 Label8->Font->Color=clBlack;
 Label8->Caption="Геосенсор подключен к порту "+IntToStr(ClientSocket1->Port);
 Timer4->Enabled=false;
 Socket1_ON=true;
}
//---------------------------------------------------------------------------
void __fastcall TFr45::ClientSocket1Disconnect(TObject *Sender,
      TCustomWinSocket *Socket)
{
cerver->Caption="Геосенсор отключился";
Label8->Font->Color=clFuchsia;
Label8->Caption="Геосенсор отключился";
Pgeo->Visible=true;
if(IsGlubGeo || PbCheck) Timer4->Enabled=true;
Socket1_ON=false;
}
//---------------------------------------------------------------------------
void __fastcall TFr45::ClientSocket1Read(TObject *Sender,TCustomWinSocket *Socket)
{
AnsiString s1, s;
s= Socket->ReceiveText();
 if(IsGlubGeo)
 {
    int i=AnsiPos("1k",s);
    if(i==0)return;
    s1=s.SubString(i+2,s.Length());
    int j=AnsiPos("po",s1);
    s1=s1.SubString(1,j-1);
    float zzt=MyStrToFloat(s1);
    //если глубина Внешняя
    if(RadioGroup1->ItemIndex==0) Eglubina->Text=FloatToStrF0(zzt,8,2);//покажем глубину долота
 }
}
//---------------------------------------------------------------------------
void __fastcall TFr45::ClientSocket1Error(TObject *Sender,
      TCustomWinSocket *Socket, TErrorEvent ErrorEvent, int &ErrorCode)
{
switch(ErrorEvent)
{
case eeSend:cerver->Caption="Ошибка записи сообщения";
break;
case eeReceive: cerver->Caption="Ошибка чтения "+Czis->Text;
break;
case eeConnect: cerver->Caption="Ошибка открытия "+Czis->Text;
break;
case eeDisconnect: cerver->Caption="Ошибка закрытия "+Czis->Text;
break;
case eeAccept: cerver->Caption="Ошибка доступа "+Czis->Text;
break;
case eeGeneral: cerver->Caption="Ошибка соединения "+Czis->Text;
break;
}
Pgeo->Visible=true;
//if(IsGlubGeo) Eglubina->Font->Color=clBlack;
ErrorCode=0;
//10060-нет в сети, 10061- нет программы
}
//---------------------------------------------------------------------------
void __fastcall TFr45::Timer4Timer(TObject *Sender)
{
 if(IsGlubGeo || PbCheck) Button2Click(Sender);//если глубина от Гео или ПБ, то повторяем коннект
 else Timer4->Enabled=false;
}
//---------------------------------------------------------------------------
void __fastcall TFr45::Label17MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
LG->Visible=true;        
}
//---------------------------------------------------------------------------

void __fastcall TFr45::Label17MouseUp(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
LG->Visible=false;
}
//---------------------------------------------------------------------------
void __fastcall TFr45::u32Click(TObject *Sender)
{
uco32=u32->Checked;
if(uco32){//R4->Items->Strings[0]="10";
  KmB=2.5*256;ur=31; TrackBar1->Max=100*8388607*((KmB/256)*1000/8388607.);}
else {//R4->Items->Strings[0]="10" ;
  KmB=2.5; ur=23;
  TrackBar1->Max=100*8388607*((KmB)*1000/8388607.);}

}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

void __fastcall TFr45::FormMouseDown(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
if(Y<L1->DefaultRowHeight)Lz->Visible=!Lz->Visible;
}
//---------------------------------------------------------------------------
void __fastcall TFr45::Button4Click(TObject *Sender)
{
Pgeo->Visible=false;
}
//---------------------------------------------------------------------------
void __fastcall TFr45::ClosePgeoClick(TObject *Sender)
{
Pgeo->Visible=false;
}
//---------------------------------------------------------------------------
//перемещение панели
void __fastcall TFr45::PgeoMouseMove(TObject *Sender, TShiftState Shift, int X, int Y)
{
 const int SC_DRAGMOVE=61458;//0xF012;
 ReleaseCapture();
 ((TControl *)Sender)->Perform(WM_SYSCOMMAND,SC_DRAGMOVE,0);
}
//---------------------------------------------------------------------------
void __fastcall TFr45::SvClick(TObject *Sender)
{
//if (Sv->Focused()==0) return;//если изменение произведено не мышкой
SvCheck=Sv->Checked;
if(Sv->Checked)//если глубина внешняя
{
 SourceGlub->Visible=true;
 RadioGroup1->Items->Strings[0]="внешняя";
 RadioGroup1->ItemIndex=0;//внешняя глубина
 SourceGlubClick(Sender);//эмуляция клика по выбору
}
else
{
 Timer4->Enabled=false;
 Timer8->Enabled=false;
 TimerGti->Enabled=false;

 ClientSocket1->Active=false;
 ClientSocketGti->Active=false;
 TcpClient1->Active=false;

 Pgeo->Visible=false;//выключаем окошко Геосенсора
 Pleusa->Visible=false;
 Pgti->Visible=false;
 SourceGlub->Visible=false;
 
 RadioGroup1->ItemIndex=1;//ручная глубина
 RadioGroup1->Items->Strings[0]="";
 if(PotokON) {PotokON=0; while(!PotokOFF)Application->ProcessMessages();}//останавливаем поток
 Label8->Caption="";
}
}
//---------------------------------------------------------------------------
void __fastcall TFr45::SourceGlubClick(TObject *Sender)
{
    SourcItem=SourceGlub->ItemIndex;//запомним
    if(SourcItem==0 && SvCheck)//глубина от Геосенсора
    {
     Pgeo->Visible=true;
     Pleusa->Visible=false;
     Pgti->Visible=false;
    }

    if(SourcItem==1 && SvCheck)//глубина от Леузы
    {
     Pleusa->Visible=true;
     Pgeo->Visible=false;
     Pgti->Visible=false;
    }

    if(SourcItem==2 && SvCheck)//глубина от ГТИ
    {
     Pgti->Visible=true;
     Pleusa->Visible=false;
     Pgeo->Visible=false;
    }

    if(SvCheck==0)//если все выключено
    {

     Pgeo->Visible=false;
     Pleusa->Visible=false;
     Pgti->Visible=false;

    }

    Timer4->Enabled=false;
    Timer8->Enabled=false;
    TimerGti->Enabled=false;
    ClientSocket1->Active=false;
    ClientSocketGti->Active=false;
    TcpClient1->Active=false;
    if(PotokON) {PotokON=0; while(!PotokOFF)Application->ProcessMessages();}//останавливаем поток
}
//---------------------------------------------------------------------------
void __fastcall TFr45::Label19MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
LG->Visible=true;
}
//---------------------------------------------------------------------------

void __fastcall TFr45::Label19MouseUp(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
LG->Visible=false;
}
//---------------------------------------------------------------------------
void __fastcall TFr45::SparClick(TObject *Sender)
{
Spar->Row=Spar->RowCount-1;  Spar->LeftCol=0;     Spar->TopRow=2;     
}
//---------------------------------------------------------------------------

void __fastcall TFr45::Lo2Click(TObject *Sender)
{
Lo2->Row=Lo2->RowCount-1;  Lo2->LeftCol=0;     Lo2->TopRow=2;
}
//---------------------------------------------------------------------------
void __fastcall TFr45::CorrectorClick(TObject *Sender)
{
 Corr_check=1;    
}
//---------------------------------------------------------------------------

void __fastcall TFr45::ZU2Click(TObject *Sender)
{
ZU2->Row=ZU2->RowCount-1;  ZU2->LeftCol=0;     ZU2->TopRow=2;
}
//---------------------------------------------------------------------------

void __fastcall TFr45::ZU2DrawCell(TObject *Sender, int ACol, int ARow,
      TRect &Rect, TGridDrawState State)
{
if(ACol==0)ZU2->Canvas->Font->Color=clRed;
if(ACol==1)ZU2->Canvas->Font->Color=clBlue;
ZU2->Canvas->TextOut(Rect.Left+2,Rect.Top,ZU2->Cells[ACol][ARow]);
}
//---------------------------------------------------------------------------
//
//====================================================================================
float roundf(float x, unsigned int n)
{
AnsiString s1;
    s1=FloatToStrF0(x,8,n);
    return MyStrToFloat(s1);
}
//====================================================================================
int round(float x)
{
 return floor(x+0.5);
}
//====================================================================================
bool IsGlubGeo(void)
{
 if(Fr45->Sv->Checked && Fr45->SourceGlub->ItemIndex==0 && Fr45->RadioGroup1->ItemIndex==0) return 1;
 else return 0;
}
//====================================================================================
bool IsGlubLeusa(void)
{
 if(Fr45->Sv->Checked && Fr45->SourceGlub->ItemIndex==1 && Fr45->RadioGroup1->ItemIndex==0) return 1;
 else return 0;
}
//====================================================================================
#include "Leusa_tcp.h"
#include "PB.h"
//====================================================================================
//Периодически сообщаем системе, чтоб не засыпала
void __fastcall TFr45::Timer10Timer(TObject *Sender)
{
SetThreadExecutionState(ES_SYSTEM_REQUIRED | ES_DISPLAY_REQUIRED);    
}
//====================================================================================
bool FileExists(const char *fname)
{
return access(fname, 0) != -1;
}
//====================================================================================
//Кнопка Статика Запись - запись значений статики в файд *.stat
void __fastcall TFr45::WriteStatClick(TObject *Sender)
{ //Заголовок-шапка
  STstrings->Left=Fr45->PanelStrings->Width/2 - Fr45->STstrings->Width/2;
  STstrings->Top=7;
  STparam->Left=7;
  STparam->Top=STstrings->Top+STstrings->Height+2;
  STparam->Caption=ZagolForStatic.SubString(0,176)+"=>";//заголовок
  //Настроим и покажем панель строк
  PanelStrings->Top=90;
  PanelStrings->Left=0;
  PanelStrings->Height=305;
  //PanelStrings->Width=19*(DlinaParam*7)+3;
  PanelStrings->Width=TB2->Width-2;
  ClosePanelStrings->Top=PanelStrings->BevelWidth*2+1;
  ClosePanelStrings->Left = PanelStrings->Width - ClosePanelStrings->Width - (PanelStrings->BevelWidth*2+1);
  PanelStrings->Visible=true;
  //Настроим MemoUgol со всеми углами
  MemoUgol->Left=STparam->Left;
  MemoUgol->Width=Fr45->PanelStrings->Width - MemoUgol->Left-7;
  MemoUgol->Top=Fr45->STparam->Top+STparam->Height+2;
  MemoUgol->Height=PanelStrings->Height-7-MemoUgol->Top;
}
//====================================================================================
void __fastcall TFr45::ClosePanelStringsClick(TObject *Sender)
{
	PanelStrings->Visible=false;//скроем панель строк
}
//====================================================================================
//кнопка Закрыть на панели Stat
void __fastcall TFr45::CloseStatPanelClick(TObject *Sender)
{
  PanelStat->Visible=false;
  //активируем кнопку Запись статики
  WriteStat->Enabled=true;
}
//====================================================================================
//кнопка Запись на панели Stat
void __fastcall TFr45::ButtonStatSaveClick(TObject *Sender)
{
String ss;
  TStringList *list = new TStringList();//создаем список строк
  list->Add("Мертвый конец ЗТС_Инк = "+Sn->Cells[1][13]+"м");//Загружаем строки мертвых концов
  list->Add("Мертвый конец ЗТС_ГК = "+Sn->Cells[1][14]+"м");
  list->Add("Мертвый конец НДМ = "+Sn->Cells[1][15]+"м");
  //list->Add(ZagolForStatic);//загружаем строку заголовка
  list->AddStrings(MemoStat->Lines);//загружаем весь MemoStat
  list->SaveToFile(Fstat);//сохраним строки в файл Fstat
  delete list;//удаляем временный список строк
  Fr45->StatusBar->SimpleText="файл '"+ExtractFileName(Fstat)+"' успешно записан!";
  write_to_file_in();//запишем замер в формате baker в файл *.in
  write_to_file_sur();//запишем замер в формате SUCOP в файл *.sur
  write_to_file_geotrack();//запишем замер для GeoTrack в файл *.txt
  SendStaticToWits();//Пошлем замер в WITS, если требуется

  //распарсим последнюю строку в MemoStat и найдем там ЗУ и АЗкорр
  if(MemoStat->Lines->Count > 1)
  { AnsiString s1;
  	s1=Fr45->MemoStat->Lines->Strings[Fr45->MemoStat->Lines->Count-1];//берем последнюю строку в МемоСтат
    //ЗУзтс[1], АЗмагн[4]
    for(int i=0;i<5;i++)
    {	//удалим пробелы слева
      	while(AnsiString(s1).Pos(" ")==1) s1.Delete(1,1);
      	//узнаем, где заканчивается число
      	int pos=AnsiString(s1).Pos(" ")-1;
        if(i==1)//зенит
        {	//запишем первое число из строки
       		zenit_stat = MyStrToFloat(AnsiString(s1).SubString(1,pos));
        }
        if(i==4)//азимут магнитный корректированный
        {	//запишем первое число из строки
       		Azk_stat = MyStrToFloat(AnsiString(s1).SubString(1,pos));
        }
        //удалим число из строки
      	s1.Delete(1,pos);
    }
  }
  else {zenit_stat=-9999.0; Azk_stat=-9999.0;}

  //перенесем текущие Зенит и АзимутКор в настройки-ориентировочно
  if(zenit_stat!=-9999.0) Sn->Cells[1][2]=FloatToStrF0(zenit_stat,6,1);//Зенит ориентировочно
  if(Azk_stat!=-9999.0) Sn->Cells[1][1]=round(Azk_stat);//Азимут ориентировочно
  //проверяем НУБТ по Зениту
  if(zenit_stat > level_NUBT && Nubt->Checked==false)
  {	SaveStatusLog("Не включена коррекция НУБТ при ЗУ > "+FloatToStrF0(level_NUBT,4,1), clRed);
    PlaySound("MYS2",0,SND_RESOURCE);
  }
}
//====================================================================================
//Показать панель Статики
void ShowPanelStat(int top, int left)
{
  //Отформатируем Панель и расставим на ней все компоненты
  Fr45->PanelStat->Top=Fr45->PanelStrings->Top;
  Fr45->PanelStat->Left=Fr45->PanelStrings->Left;
  Fr45->PanelStat->Width=Fr45->PanelStrings->Width;
  Fr45->PanelStat->Height=Fr45->PanelStrings->Height;
  //кнопка Закрыть - правый верхний угол
  Fr45->CloseStatPanel->Top=Fr45->PanelStat->BevelWidth*2+1;
  Fr45->CloseStatPanel->Left = Fr45->PanelStat->Width - Fr45->CloseStatPanel->Width - (Fr45->PanelStat->BevelWidth*2+1);
  //кнопка Запись - внизу по центру
  Fr45->ButtonStatSave->Top=Fr45->PanelStat->Height-Fr45->ButtonStatSave->Height-7;//по вертикали внизу слева
  Fr45->ButtonStatSave->Left=Fr45->PanelStat->Width/2-Fr45->ButtonStatSave->Width/2;
  //ABACUS версия
  Fr45->RGAbacus->Top=9;
  Fr45->RGAbacus->Height=40;
  Fr45->RGAbacus->Width=130;
  Fr45->RGAbacus->Left = 7;
  //надпись Статика
  Fr45->StaticText6->Left = Fr45->PanelStat->Width/2 - Fr45->StaticText6->Width/2;
  Fr45->StaticText6->Top = Fr45->RGAbacus->Top;
  //MemoStat
  Fr45->MemoStat->Left = Fr45->RGAbacus->Left;
  Fr45->MemoStat->Width = Fr45->PanelStat->Width-Fr45->MemoStat->Left-7;
  Fr45->MemoStat->Top = Fr45->RGAbacus->Top+Fr45->RGAbacus->Height+5;
  Fr45->MemoStat->Height = Fr45->ButtonStatSave->Top-7-Fr45->MemoStat->Top;
}
//====================================================================================
void __fastcall TFr45::EBeginKeyPress(TObject *Sender, char &Key)
{
  //дробные положительные числа
  char s[]={'0','1','2','3','4','5','6','7','8','9','.'};
  int ikey=0;
  for (int i=0;i<sizeof(s);i++) if(Key==s[i])ikey=1;
  if(ikey==1 && Key=='.')
  {String str=EBegin->Text;
   int i=0, b=0;
   while(i++ < str.Length()) {if(str.IsDelimiter('.', i)) b++;}
   if(b>0) ikey=0;
  }
  if(ikey==0){Key=NULL; PlaySound("MYS2",0,SND_RESOURCE);}
}
//====================================================================================
void __fastcall TFr45::EEndKeyPress(TObject *Sender, char &Key)
{
  //дробные положительные числа
  char s[]={'0','1','2','3','4','5','6','7','8','9','.'};
  int ikey=0;
  for (int i=0;i<sizeof(s);i++) if(Key==s[i])ikey=1;
  if(ikey==1 && Key=='.')
  {String str=EEnd->Text;
   int i=0, b=0;
   while(i++ < str.Length()) {if(str.IsDelimiter('.', i)) b++;}
   if(b>0) ikey=0;
  }
  if(ikey==0){Key=NULL; PlaySound("MYS2",0,SND_RESOURCE);}
}
//====================================================================================
//ячейка Азимут
void __fastcall TFr45::L1aDrawCell(TObject *Sender, int ACol, int ARow,
      TRect &Rect, TGridDrawState State)
{
 if(ARow<2) L1a->Canvas->Font->Color=clBlue;//верхние ячейки
 else if(masQuality[ARow]==0) L1a->Canvas->Font->Color=clBlue;//хорошо в Динамике
 else if(masQuality[ARow]==1) L1a->Canvas->Font->Color=clRed;//плохо в Динамике
 else if(masQuality[ARow]==2) L1a->Canvas->Font->Color=clGreen;//хорошо в Статике
 else if(masQuality[ARow]==3) L1a->Canvas->Font->Color=clFuchsia;//clRed;//плохо в Статике
 L1a->Canvas->TextOut(Rect.Left+2,Rect.Top,L1a->Cells[ACol][ARow]);
}
//====================================================================================
//ячейка АЗкорр
void __fastcall TFr45::LakoppDrawCell(TObject *Sender, int ACol, int ARow,
      TRect &Rect, TGridDrawState State)
{
 if(ARow<2) Lakopp->Canvas->Font->Color=clNavy;//верхние ячейки
 else if(masQuality[ARow]==0) Lakopp->Canvas->Font->Color=clNavy;//хорошо в Динамике
 else if(masQuality[ARow]==1) Lakopp->Canvas->Font->Color=clRed;//плохо в Динамике
 else if(masQuality[ARow]==2) Lakopp->Canvas->Font->Color=clGreen;//хорошо в Статике
 else if(masQuality[ARow]==3) Lakopp->Canvas->Font->Color=clFuchsia;//clRed;//плохо в Статике
 Lakopp->Canvas->TextOut(Rect.Left+2,Rect.Top,Lakopp->Cells[ACol][ARow]);
}
//====================================================================================
//ячейка Htotal
void __fastcall TFr45::LhDrawCell(TObject *Sender, int ACol, int ARow,
      TRect &Rect, TGridDrawState State)
{
 if(ARow<2) Lh->Canvas->Font->Color=clBlack;//верхние ячейки
 else if(masQuality[ARow]==0) Lh->Canvas->Font->Color=clBlack;//хорошо в Динамике
 else if(masQuality[ARow]==1) Lh->Canvas->Font->Color=clRed;//плохо в Динамике
 else if(masQuality[ARow]==2) Lh->Canvas->Font->Color=clGreen;//хорошо в Статике
 else if(masQuality[ARow]==3) Lh->Canvas->Font->Color=clFuchsia;//clRed;//плохо в Статике
 Lh->Canvas->TextOut(Rect.Left+2,Rect.Top,Lh->Cells[ACol][ARow]);
}
//====================================================================================
//ячейки ЗЕНИТА
void __fastcall TFr45::L1DrawCell(TObject *Sender, int ACol, int ARow,
      TRect &Rect, TGridDrawState State)
{
 if(ARow<2) L1->Canvas->Font->Color=clHotLight;//это верхние ячейки
 else if(masQuality[ARow]==0) L1->Canvas->Font->Color=clHotLight;//хорошо в Динамике
 else if(masQuality[ARow]==1) L1->Canvas->Font->Color=clRed;//плохо в Динамике
 else if(masQuality[ARow]==2) L1->Canvas->Font->Color=clGreen;//хорошо в Статике
 else if(masQuality[ARow]==3) L1->Canvas->Font->Color=clFuchsia;//clRed;//плохо в Статике
 L1->Canvas->TextOut(Rect.Left+2,Rect.Top,L1->Cells[ACol][ARow]);//перерисовываем
}
//====================================================================================
//ячейки НДМ
void __fastcall TFr45::SparDrawCell(TObject *Sender, int ACol, int ARow,
      TRect &Rect, TGridDrawState State)
{
 Spar->Canvas->Font->Color=clBlack;
 if(ARow>1)
 {
  if(masQualNDM[ARow]==1) Spar->Canvas->Font->Color=clRed;//из массива качества
  else Spar->Canvas->Font->Color=clBlack;
 }
 Spar->Canvas->TextOut(Rect.Left+2,Rect.Top,Spar->Cells[ACol][ARow]);
}
//---------------------------------------------------------------------------
#include "WitsClient.h"
#include "WitsServer.h"
#include "DepthFromGti.h"
#include "LogServer.h"
#include "DrawOTK.h"
//---------------------------------------------------------------------------
//Выбор Ретранслятора в Настройках
void __fastcall TFr45::CBretrClick(TObject *Sender)
{
if(Pnastr->Visible)
{
    PnastrChange=1;
    FormShow(Sender);
}    
}
//---------------------------------------------------------------------------
//Выбор ННК в Настройках
void __fastcall TFr45::CBnnkClick(TObject *Sender)
{
if(Pnastr->Visible)
{
    PnastrChange=1;
    FormShow(Sender);
}    
}
//---------------------------------------------------------------------------
//Выбор НДМ в Настройках
void __fastcall TFr45::CBndmClick(TObject *Sender)
{
if(Pnastr->Visible)
{   //если выбран НДМ, то принудительно ставим ГК и запрещаем изменение ГК и Ретр
    if(CBndm->Checked==true)
    {CBgkzts->Checked = true;
     CBgkzts->Enabled = false;
     CBretr->Enabled = false;
     CBgeomash->Enabled = false;
    }
    else
    {CBgkzts->Enabled = true;
     CBretr->Enabled = true;
     CBgeomash->Enabled = true;
    }

    PnastrChange=1;
    FormShow(Sender);
}
}
//---------------------------------------------------------------------------
//Выбор ГКзтс в Настройках
void __fastcall TFr45::CBgkztsClick(TObject *Sender)
{
if(Pnastr->Visible)
{
    PnastrChange=1;
    FormShow(Sender);
}
}
//---------------------------------------------------------------------------
//вычисление FORMAT_KADRA
int solv_format_kadra(void)
{
int a=0;
    if(Fr45->CBndm->Checked==true && Fr45->CBnnk->Checked==true) a=FORMAT_NDM_NNK_GK_RETR;//НДМ+ННК
    else if(Fr45->CBndm->Checked==true && Fr45->CBnnk->Checked==false) a=FORMAT_NDM_GK_RETR;//НДМ
    else if(Fr45->CBndm->Checked==false && Fr45->CBnnk->Checked==true) a=FORMAT_NNK_GK_RETR;//ННК
    else if(Fr45->CBndm->Checked==false && Fr45->CBnnk->Checked==false)//если без модулей НДМ и ННК
    {
    	if(Fr45->CBretr->Checked==true)//с Ретром, с паузами
        {	if(Fr45->CBgkzts->Checked==true) a=FORMAT_GK_RETR;//с ГКзтс
           	else a=FORMAT_STD_RETR;//без ГКзтс
        }
        else //без Ретра, без пауз
        {	if(Fr45->CBgkzts->Checked==true) a=FORMAT_STD_GK;//с ГКзтс
           	else a=FORMAT_STD;//без ГКзтс
        }
    }
    //приоритетные, не взирая на вышеустановленные
	if(Fr45->CBgeomash->Checked && !Fr45->CBretr->Checked) a=FORMAT_GEOMASH;//ГеоМаш, без пауз
	if(Fr45->CBgeomash->Checked && Fr45->CBretr->Checked) a=FORMAT_GEOMASH_RETR;//ГеоМаш+Ретр, короткая пауза

    return a&31;
}
//---------------------------------------------------------------------------
void __fastcall TFr45::Timer11Timer(TObject *Sender)
{
Timer11->Enabled=false;
Label8->Caption="";//чистка панели
}
//---------------------------------------------------------------------------
void __fastcall TFr45::MemoUgolClick(TObject *Sender)
{
AnsiString /*ss,s1,*/Ftmp;
int num = MemoUgol->CaretPos.y;
//StatusBar->SimpleText="Номер строки = "+IntToStr(MemoUgol->CaretPos.y);

  PanelStrings->Visible=false;//скроем панель со строками
  //Настроим и покажем отформатированную панель Статики в координатах top,left
  ShowPanelStat(10,250);
  //Деактивируем эту кнопку на время, пока открыта панель Cтатики
  WriteStat->Enabled=false;

  //Теперь прочитаем в мемо файл замеров статики
  FILE *F;
  if((F=fopen(Fstat.c_str(),"a"))==NULL){ShowMessage("не удается открыть"+Fstat);return;}
  fclose(F);
  F=NULL;
  LoadToMemoStatFromFile(Fstat);//загружаем статику из файла в MemoStat

  //Теперь добавим в мемо выбранную строку из текущих замеров
  //уберем из мемо все пустые строки внизу
  while(MemoStat->Lines->Count > 1 && MemoStat->Lines->Strings[MemoStat->Lines->Count-1]=="")
    MemoStat->Lines->Delete(MemoStat->Lines->Count-1);
  if(num < MemoUgol->Lines->Count)//позиция курсора
  {AnsiString ss=MemoUgol->Lines->Strings[num];
   int index=ss.Pos("*"); if(index>0) ss=ss.Delete(index,1);//удаляем звездочку Статики
   MemoStat->Lines->Add(ss);
  }
  //прокрутим скролл в самый низ
  SendMessage(MemoStat->Handle, WM_VSCROLL, SB_BOTTOM, 0);

  PanelStat->Visible=true;//показываем панель статики
}
//---------------------------------------------------------------------------
//прочитать магнитное склонение
float get_ums(void)
{
try
{
    float t=MyStrToFloat(Fr45->Sn->Cells[1][19]);
	return t;
}
catch(...)
{   SaveStatusLog("Ошибка в магнитном склонении!", clRed);
    PlaySound("MYS2",0,SND_RESOURCE);
	return 0.;
}
}
//---------------------------------------------------------------------------
//прочитать магнитное наклонение
float get_umn(void)
{
try
{   float t = MyStrToFloat(Fr45->Sn->Cells[1][4]);//проверяем на число заодно
    if(t < 50) SaveStatusLog("Некорректное значение УМН!\r\nВведите УМН в естественном виде, без вычитания из 90", clRed);
	return t;
}
catch(...)
{   SaveStatusLog("Ошибка в магнитном наклонении!", clRed);
    PlaySound("MYS2",0,SND_RESOURCE);
	return -9999.0;
}
}
//---------------------------------------------------------------------------
//прочитать опорное магнитное поле Hop
float get_Hop(void)
{
try
{
    float t=MyStrToFloat(Fr45->Sn->Cells[1][5]);
	return t;
}
catch(...)
{   SaveStatusLog("Ошибка в модуле Ноп!", clRed);
    PlaySound("MYS2",0,SND_RESOURCE);
	return 0.;
}
}
//---------------------------------------------------------------------------
//прочитать Gt из паспорта прибора
double get_Gt_opor(void)
{
try
{
    double t=MyStrToFloat(Fr45->Sn->Cells[1][20]);
	return t;
}
catch(...)
{   SaveStatusLog("Ошибка в Gt из паспорта прибора!", clRed);
    PlaySound("MYS2",0,SND_RESOURCE);
	return 0.;
}
}
//---------------------------------------------------------------------------
//зарезка в ОТК2
void __fastcall TFr45::Caz_otkClick(TObject *Sender)
{
	if(Caz_otk->Checked) STotk2->Visible=true;
    else STotk2->Visible=false;
}
//---------------------------------------------------------------------------
void __fastcall TFr45::WMSysCommand(TMessage &Msg)
{
//перехват системных событий формы
    //unsigned int sys_code = Msg.WParam & 0xFFF0;
    //if (sys_code == SC_CLOSE) ShowMessage(L"Нажали крестик");
    //StatusBar->SimpleText="Msg.WParam = "+IntToStr(Msg.WParam)+"   Msg.LParam = "+IntToStr(Msg.LParam);
    TForm::Dispatch(&Msg);  //для штатного выполнения команды (обработки сообщения)
}
//---------------------------------------------------------------------------
//авто-закрытие дочерних панелей при закрытии панели настроек
void __fastcall TFr45::TimerNastrTimer(TObject *Sender)
{//сработал таймер окна Настройки для закрытия дочерних панелей
	if(Pnastr->Visible==true) return;//ничего не делаем, пока панель настроек видима
    //если панель настроек закрыта, то закрываем все дочерние панели, которые видимы
    if(Pwits->Visible==true) Pwits->Visible = false;
    if(PwitsClient->Visible==true) PwitsClient->Visible = false;
    if(Pgeo->Visible==true) Pgeo->Visible = false;
    if(Pleusa->Visible==true) Pleusa->Visible = false;
    if(Pgti->Visible==true) Pgti->Visible = false;
    if(PanelSrvLog->Visible==true) PanelSrvLog->Visible = false;
    //остановим таймер до следующего открытия панели настроек
    TimerNastr->Enabled = false;
}
//---------------------------------------------------------------------------
//Мертвый конец до ЗТС
float get_mkonec_zts(void)
{   return MyStrToFloat(Fr45->Sn->Cells[1][13]);
}
//---------------------------------------------------------------------------
//Мертвый конец до датчика ГКзтс
float get_mkonec_gkzts(void)
{   return MyStrToFloat(Fr45->Sn->Cells[1][14]);
}
//---------------------------------------------------------------------------
//Мертвый конец до датчика НДМ
float get_mkonec_ndm(void)
{   return MyStrToFloat(Fr45->Sn->Cells[1][15]);
}
//---------------------------------------------------------------------------
#include "Edit_Ink.h"
//---------------------------------------------------------------------------
//Вывод файла статус бар по двойному клику
void __fastcall TFr45::Label8DblClick(TObject *Sender)
{
FILE *F;
    F=fopen(Fstatus.c_str(),"a");//если файла нет, то создаем новый
	fclose(F);//закрываем файл
    ShellExecute(Handle,NULL,"notepad",Fstatus.c_str(),NULL,SW_RESTORE);
    //чистим все, если есть
    if(Label8->Caption == "( ! )" || Label8->Color != TB2->Color)
    {Label8->Color=TB2->Color;
     Label8->Caption="";
    }
}
//---------------------------------------------------------------------------
//Вывод файла статус бар по двойному клику
void __fastcall TFr45::StatusBarDblClick(TObject *Sender)
{	Label8DblClick(Sender);
}
//---------------------------------------------------------------------------
//перемещение панели Настроек
void __fastcall TFr45::PnastrMouseMove(TObject *Sender, TShiftState Shift, int X, int Y)
{
 const int SC_DRAGMOVE=61458;//0xF012;
 ReleaseCapture();
 ((TControl *)Sender)->Perform(WM_SYSCOMMAND,SC_DRAGMOVE,0);
}
//---------------------------------------------------------------------------
//закрытие панели Настроек
void __fastcall TFr45::Image1Click(TObject *Sender)
{	Pnastr->Visible=false;
    CkwChange(Sender);//обновляем файлы скважины
	SaveNastrLog(Fnastr);//записываем настройки в лог
    if(StrToFloat(Sn->Cells[1][20])==0) ShowMessage("Ошибка! Gt из паспорта прибора не может быть равным нулю!");
    get_umn();//если там ошибка, то будет выведено сообщение
}
//---------------------------------------------------------------------------
#include "Player.h"
//---------------------------------------------------------------------------
void __fastcall TFr45::CplayerClick(TObject *Sender)
{
    if(Cplayer->Checked)
    {   PanelPlayer->Visible = true;
        //TB2->Update();
    }
    else
    {   if(StatusPlayer==true) {PlayButton->Down = false; PlayButtonClick(Sender);}
        if(StatusRecorder==true) {RecButton->Down = false; RecButtonClick(Sender);}
        PanelPlayer->Visible = false;
        //TB2->Update();
    }
}
//---------------------------------------------------------------------------
#include "Pasport.h"
//---------------------------------------------------------------------------
//возвращает простой CRC 16 бит с инверсией
uint16_t crc16_simple (uint8_t *ptr, uint16_t dlina)
{
int a;
uint16_t res;
	if(dlina==0) return 0;
	res=0;
	//алгоритм CRC16
	for(a=0;a<dlina;a++) res += *(ptr+a);//простая сумма
    return ~res;//инверсия
}
//---------------------------------------------------------------------------
//возвращает простой CRC 32 бит с инверсией
uint32_t crc32_simple (uint8_t *ptr, uint16_t dlina)
{
int a;
uint32_t res;
	if(dlina==0) return 0;
	res=0;
	//алгоритм CRC32
	for(a=0;a<dlina;a++) res += ptr[a];//простая сумма
    return ~res;//инверсия
}
//---------------------------------------------------------------------------
//poly=0x04C11DB7, Init=0xFFFFFFFF, RefIn/RefOut=true, XorOut=0xFFFFFFFF
uint32_t crc32_poly(uint8_t* in, uint32_t len)
{
    static const uint32_t crc_table[256] = {
        0x00000000, 0x77073096, 0xEE0E612C, 0x990951BA, 0x076DC419, 0x706AF48F,
        0xE963A535, 0x9E6495A3, 0x0EDB8832, 0x79DCB8A4, 0xE0D5E91E, 0x97D2D988,
        0x09B64C2B, 0x7EB17CBD, 0xE7B82D07, 0x90BF1D91, 0x1DB71064, 0x6AB020F2,
        0xF3B97148, 0x84BE41DE, 0x1ADAD47D, 0x6DDDE4EB, 0xF4D4B551, 0x83D385C7,
        0x136C9856, 0x646BA8C0, 0xFD62F97A, 0x8A65C9EC, 0x14015C4F, 0x63066CD9,
        0xFA0F3D63, 0x8D080DF5, 0x3B6E20C8, 0x4C69105E, 0xD56041E4, 0xA2677172,
        0x3C03E4D1, 0x4B04D447, 0xD20D85FD, 0xA50AB56B, 0x35B5A8FA, 0x42B2986C,
        0xDBBBC9D6, 0xACBCF940, 0x32D86CE3, 0x45DF5C75, 0xDCD60DCF, 0xABD13D59,
        0x26D930AC, 0x51DE003A, 0xC8D75180, 0xBFD06116, 0x21B4F4B5, 0x56B3C423,
        0xCFBA9599, 0xB8BDA50F, 0x2802B89E, 0x5F058808, 0xC60CD9B2, 0xB10BE924,
        0x2F6F7C87, 0x58684C11, 0xC1611DAB, 0xB6662D3D, 0x76DC4190, 0x01DB7106,
        0x98D220BC, 0xEFD5102A, 0x71B18589, 0x06B6B51F, 0x9FBFE4A5, 0xE8B8D433,
        0x7807C9A2, 0x0F00F934, 0x9609A88E, 0xE10E9818, 0x7F6A0DBB, 0x086D3D2D,
        0x91646C97, 0xE6635C01, 0x6B6B51F4, 0x1C6C6162, 0x856530D8, 0xF262004E,
        0x6C0695ED, 0x1B01A57B, 0x8208F4C1, 0xF50FC457, 0x65B0D9C6, 0x12B7E950,
        0x8BBEB8EA, 0xFCB9887C, 0x62DD1DDF, 0x15DA2D49, 0x8CD37CF3, 0xFBD44C65,
        0x4DB26158, 0x3AB551CE, 0xA3BC0074, 0xD4BB30E2, 0x4ADFA541, 0x3DD895D7,
        0xA4D1C46D, 0xD3D6F4FB, 0x4369E96A, 0x346ED9FC, 0xAD678846, 0xDA60B8D0,
        0x44042D73, 0x33031DE5, 0xAA0A4C5F, 0xDD0D7CC9, 0x5005713C, 0x270241AA,
        0xBE0B1010, 0xC90C2086, 0x5768B525, 0x206F85B3, 0xB966D409, 0xCE61E49F,
        0x5EDEF90E, 0x29D9C998, 0xB0D09822, 0xC7D7A8B4, 0x59B33D17, 0x2EB40D81,
        0xB7BD5C3B, 0xC0BA6CAD, 0xEDB88320, 0x9ABFB3B6, 0x03B6E20C, 0x74B1D29A,
        0xEAD54739, 0x9DD277AF, 0x04DB2615, 0x73DC1683, 0xE3630B12, 0x94643B84,
        0x0D6D6A3E, 0x7A6A5AA8, 0xE40ECF0B, 0x9309FF9D, 0x0A00AE27, 0x7D079EB1,
        0xF00F9344, 0x8708A3D2, 0x1E01F268, 0x6906C2FE, 0xF762575D, 0x806567CB,
        0x196C3671, 0x6E6B06E7, 0xFED41B76, 0x89D32BE0, 0x10DA7A5A, 0x67DD4ACC,
        0xF9B9DF6F, 0x8EBEEFF9, 0x17B7BE43, 0x60B08ED5, 0xD6D6A3E8, 0xA1D1937E,
        0x38D8C2C4, 0x4FDFF252, 0xD1BB67F1, 0xA6BC5767, 0x3FB506DD, 0x48B2364B,
        0xD80D2BDA, 0xAF0A1B4C, 0x36034AF6, 0x41047A60, 0xDF60EFC3, 0xA867DF55,
        0x316E8EEF, 0x4669BE79, 0xCB61B38C, 0xBC66831A, 0x256FD2A0, 0x5268E236,
        0xCC0C7795, 0xBB0B4703, 0x220216B9, 0x5505262F, 0xC5BA3BBE, 0xB2BD0B28,
        0x2BB45A92, 0x5CB36A04, 0xC2D7FFA7, 0xB5D0CF31, 0x2CD99E8B, 0x5BDEAE1D,
        0x9B64C2B0, 0xEC63F226, 0x756AA39C, 0x026D930A, 0x9C0906A9, 0xEB0E363F,
        0x72076785, 0x05005713, 0x95BF4A82, 0xE2B87A14, 0x7BB12BAE, 0x0CB61B38,
        0x92D28E9B, 0xE5D5BE0D, 0x7CDCEFB7, 0x0BDBDF21, 0x86D3D2D4, 0xF1D4E242,
        0x68DDB3F8, 0x1FDA836E, 0x81BE16CD, 0xF6B9265B, 0x6FB077E1, 0x18B74777,
        0x88085AE6, 0xFF0F6A70, 0x66063BCA, 0x11010B5C, 0x8F659EFF, 0xF862AE69,
        0x616BFFD3, 0x166CCF45, 0xA00AE278, 0xD70DD2EE, 0x4E048354, 0x3903B3C2,
        0xA7672661, 0xD06016F7, 0x4969474D, 0x3E6E77DB, 0xAED16A4A, 0xD9D65ADC,
        0x40DF0B66, 0x37D83BF0, 0xA9BCAE53, 0xDEBB9EC5, 0x47B2CF7F, 0x30B5FFE9,
        0xBDBDF21C, 0xCABAC28A, 0x53B39330, 0x24B4A3A6, 0xBAD03605, 0xCDD70693,
        0x54DE5729, 0x23D967BF, 0xB3667A2E, 0xC4614AB8, 0x5D681B02, 0x2A6F2B94,
        0xB40BBE37, 0xC30C8EA1, 0x5A05DF1B, 0x2D02EF8D
    };
    
    uint32_t crc = 0xFFFFFFFF;//начальное значение
    for (uint32_t i = 0; i < len; i++)
    {
        crc = (crc >> 8) ^ crc_table[(crc ^ in[i]) & 0xFF];
    }
    return crc ^ 0xFFFFFFFF;//XorOut
}


