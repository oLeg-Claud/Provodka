//---------------------------------------------------------------------------

#ifndef traekH
#define traekH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
#include <ComCtrls.hpp>
#include <ToolWin.hpp>
#include <Grids.hpp>
#include <Chart.hpp>
#include <Series.hpp>
#include <TeEngine.hpp>
#include <TeeProcs.hpp>
#include <Dialogs.hpp>
#include <Buttons.hpp>
#include "CSPIN.h"
#include <ArrowCha.hpp>
#include <ScktComp.hpp>
#include <Sockets.hpp>
#include <FileCtrl.hpp>
#include <IdAntiFreeze.hpp>
#include <IdAntiFreezeBase.hpp>
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdHTTP.hpp>
#include <IdTCPClient.hpp>
#include <IdTCPConnection.hpp>
#include <Graphics.hpp>
#include <CheckLst.hpp>
#include <MPlayer.hpp>
#include <ImgList.hpp>
#include <queue>
#include "trayicon.h"
#define WM_MyPost1  WM_USER
#define WM_MyPost2  WM_USER+1
#define WM_MyPost3  WM_USER+2
#define WM_MyPost4  WM_USER+3
#define WM_MyPost5  WM_USER+4
#define WM_MyPost6  WM_USER+5
#define rowCount 47
#define rowCountNDM 17
//Адреса параметров Пульта Бурильщика не нашего
#define     SET_ZU          0x0065
#define     SET_AZ          0x0066
#define     SET_OTK         0x0067
#define     SET_OTK_MODE    0x0068
#define     SET_DISP_ROT    0x0069
#define     GET_DISP_POS    0x006A
#define     OTK_MODE_GRAV   1
#define     OTK_MODE_MAG    0
//---------------------------------------------------------------------------
class TFr45 : public TForm
{
__published:	// IDE-managed Components
        TMainMenu *MainMenu1;
        TMenuItem *N3;
        TStatusBar *StatusBar;
        TMenuItem *N2;
        TColorDialog *ColorDialog1;
        TToolBar *TB2;
        TButton *Button3;
        TGroupBox *GroupBox4;
        TCheckBox *spektr;
        TCheckBox *Cotkl;
        TGroupBox *GroupBox2;
        TPanel *Pnastr;
        TStringGrid *Sn;
        TRadioGroup *R4;
        TMenuItem *N5;
        TMenuItem *N6;
        TComboBox *Sport;
        TLabel *Label3;
        TRadioGroup *Cuss;
        TPanel *Panel7;
        TLabel *Label8;
        TChart *C2;
        TFastLineSeries *Series3;
        TChart *C3;
        TFastLineSeries *Series2;
        TStringGrid *L0;
        TStringGrid *L1;
        TListBox *Lz;
        TPanel *Panel5;
        TLabel *Label4;
        TLabel *Label5;
        TLabel *Label6;
        TEdit *Cobor;
        TEdit *Ctem;
        TEdit *Cnapr;
        TPanel *Pckw;
    TMemo *MemoInc;
        TPanel *Potk;
        TImage *I1;
        TImage *I2;
        TToolBar *OBar;
        TCSpinEdit *Cmin;
        TPanel *Panel4;
        TCSpinEdit *Cmax;
        TTrackBar *TrackBar1;
        TCheckBox *Co;
        TTrackBar *T2;
        TLabel *Label10;
        TLabel *Label7;
        TPanel *Panel1;
        TSpeedButton *stop;
        TTimer *Timer2;
        TStringGrid *L1a;
        TListBox *La;
        TTimer *Timer1;
        TPanel *Panel2;
        TLabel *Label11;
        TLabel *Label12;
        TEdit *Cvib;
        TMenuItem *TAR;
        TLabel *Label9;
        TPanel *Pcolor;
        TCheckBox *Cgr;
        TButton *Bend;
        TButton *Bstart;
        TComboBox *ComboBox1;
        TLabel *Label13;
        TPanel *Pstart;
        TPanel *Pend;
        TSpeedButton *start;
        TGroupBox *Gll;
        TEdit *Eglubina;
        TEdit *Edz;
        TPanel *Panel3;
        TPanel *Panel6;
        TRadioGroup *Rnapr;
        TCheckBox *Cotklm;
        TPanel *Potkm;
        TToolBar *OBarm;
        TCSpinEdit *Cminm;
        TPanel *Panel9;
        TCSpinEdit *Cmaxm;
        TRadioGroup *Rnaprm;
        TPanel *Pi;
        TImage *I1m;
        TButton *Button1;
        TImage *I2m;
        TLabel *Label1;
        TOpenDialog *OpenDialog1;
        TOpenDialog *OpenDialog2;
        TEdit *Cgk;
        TLabel *Label14;
        TCheckBox *Caz_otk;
        TPanel *Panel8;
        TSpeedButton *Timer;
        TEdit *Edit7;
        TTimer *Timer3;
        TLabel *Label15;
        TEdit *Cwr;
        TListBox *Lo;
        TListBox *Lte;
        TListBox *Lnapr;
        TListBox *Lvib;
        TListBox *Lgk;
        TListBox *Lwr;
        TStringGrid *Lo2;
        TStringGrid *Lakopp;
        TStringGrid *Lh;
        TListBox *Lkopp;
        TListBox *Lhh;
        TClientSocket *ClientSocket1;
        TRadioGroup *RadioGroup1;
        TPanel *Pgeo;
        TComboBox *Czis;
        TLabel *cerver;
        TButton *Button2;
        TListBox *LG;
        TTimer *Timer4;
        TCheckBox *u32;
        TCheckBox *Sv;
        TPanel *par;
        TStringGrid *Spar;
        TLabel *Label17;
        TEdit *Epausa;
        TLabel *Label19;
        TEdit *CG;
        TCheckBox *Sv2;
        TPanel *Pwits;
        TLabel *cerver2;
        TButton *Button22;
        TLabel *Label20;
        TLabel *Label21;
        TTimer *Timer5;
     TLabel *ZeroOTK;
    TCheckBox *Corrector;
    TStringGrid *ZU2;
    TLabel *Lgkzts;
    TLabel *Lgkndm;
    TMemo *MemoLas;
    TServerSocket *ServerSocket1;
    TPanel *Panel10;
    TLabel *Label16;
    TPanel *Pleusa;
    TLabel *cerver3;
    TLabel *Label22;
    TButton *Button2leusa;
    TTcpClient *TcpClient1;
    TRadioGroup *SourceGlub;
    TCheckBox *PB;
    TComboBox *Sport2;
    TLabel *Lport2;
    TTimer *Timer8;
    TTimer *Timer9;
    TPanel *Diod;
    TClientSocket *Soc;
    TTimer *Timer10;
    TGroupBox *GroupBox1;
    TGroupBox *Baker;
    TButton *WriteStat;
    TMemo *MemoTemp;
    TPanel *PanelStat;
    TBitBtn *ButtonStatSave;
    TMemo *MemoStat;
    TCheckBox *Cl2;
    TPanel *PwitsClient;
    TLabel *cerver4;
    TLabel *Label23;
    TComboBox *Cwits;
    TButton *Button5;
    TClientSocket *ClientSocket2;
	TIdHTTP *HTTP1;
	TIdAntiFreeze *IdAntiFreeze1;
	TCheckBox *CBLog;
	TPanel *PanelSrvLog;
	TLabel *Label18;
	TLabel *Label24;
	TButton *Button8;
	TComboBox *Cleusa;
	TServerSocket *SrvLOG;
	TTimer *TimerLog;
	TRadioGroup *PBRadioGroup;
    TGroupBox *GBmoduls;
    TCheckBox *CBndm;
    TCheckBox *CBnnk;
    TCheckBox *CBretr;
	TCheckBox *CBgkzts;
	TRadioGroup *RGcoder;
	TPanel *Pgti;
	TLabel *cerverGti;
	TLabel *Label26;
	TButton *Button10gti;
	TComboBox *Cgti;
	TClientSocket *ClientSocketGti;
	TMemo *Memo1;
	TTimer *TimerGti;
	TTimer *Timer11;
    TPanel *PanelInterval;
    TLabel *LBegin;
    TLabel *LEnd;
    TBitBtn *BitBtnIntervalSave;
    TEdit *EBegin;
    TEdit *EEnd;
    TStaticText *StaticText2;
    TCheckBox *Nubt;
	TPanel *PanelStrings;
	TStaticText *STstrings;
	TMemo *MemoUgol;
	TStaticText *STparam;
	TStaticText *STotk2;
	TTimer *TimerNastr;
	TLabel *Label25;
	TPanel *PanelINK;
	TPanel *PanelEditInk;
	TLabel *Label27;
	TLabel *Label28;
	TBitBtn *BitBtnRun;
	TEdit *EditBegin;
	TEdit *EditEnd;
	TStaticText *StaticText3;
	TRadioGroup *RadioGroup2;
	TLabel *Label29;
	TEdit *EditLast;
    TEdit *EditEnd2;
    TEdit *EditBegin2;
	TImage *Image1;
	TImage *CloseWitsServer;
	TImage *CloseWitsClient;
	TImage *ClosePgeo;
	TImage *CloseLeusa;
	TImage *CloseGti;
	TImage *CloseLogServer;
	TImage *CloseInterval;
	TImage *ClosePanelStrings;
	TImage *CloseStatPanel;
	TImage *Image2;
	TPanel *PanelLasParam;
	TImage *CloseLas;
	TBitBtn *BitBtnLasGo;
	TStaticText *StaticText4;
	TCheckListBox *LBLas;
	TImageList *ImageList1;
	TTimer *TimerPlayer;
    TPanel *PanelPlayer;
	TSpeedButton *PlayButton;
	TSpeedButton *RecButton;
	TSpeedButton *PlayerSpeedButton;
    TSpeedButton *PauseButton;
    TCheckBox *Cplayer;
	TRadioGroup *RGAbacus;
    TPanel *PanelPasport;
    TImage *ClosePasport;
    TStaticText *StaticText5;
	TComboBox *Ckw;
    TLabel *Label2;
    TBitBtn *PasportBtn;
    TStringGrid *TablePasport;
    TBitBtn *DirBtn;
    TStaticText *StaticText6;
	TEdit *Cdat;
	TEdit *Cname;
	TLabel *Label30;
    TRadioGroup *PBType;
	TSpeedButton *RotateButton;
	TLabel *Label31;
	TCheckBox *CBgeomash;
        void __fastcall N3Click(TObject *Sender);
        void __fastcall startClick(TObject *Sender);
        void __fastcall stopClick(TObject *Sender);
        void __fastcall TrackBar1Change(TObject *Sender);
        void __fastcall N2Click(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall T2Change(TObject *Sender);
        void __fastcall I2MouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
        void __fastcall PotkResize(TObject *Sender);
        void __fastcall CmaxChange(TObject *Sender);
        void __fastcall Button3Click(TObject *Sender);
        void __fastcall CminChange(TObject *Sender);
        void __fastcall EglubinaKeyPress(TObject *Sender, char &Key);
        void __fastcall l(TObject *Sender, int ACol, int ARow,
          TRect &Rect, TGridDrawState State);
        void __fastcall spektrClick(TObject *Sender);
        void __fastcall CotklClick(TObject *Sender);
        void __fastcall Panel3Click(TObject *Sender);
        void __fastcall Panel4Click(TObject *Sender);
        void __fastcall C3Zoom(TObject *Sender);
        void __fastcall Panel6Click(TObject *Sender);
        void __fastcall CoClick(TObject *Sender);
        void __fastcall R4Click(TObject *Sender);
        void __fastcall CkwChange(TObject *Sender);
        void __fastcall N5Click(TObject *Sender);
        void __fastcall N6Click(TObject *Sender);
        void __fastcall SportChange(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall CussClick(TObject *Sender);
        void __fastcall EdzKeyPress(TObject *Sender, char &Key);
        void __fastcall SportDblClick(TObject *Sender);
        void __fastcall C3MouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
        void __fastcall Label3Click(TObject *Sender);
        void __fastcall L0Click(TObject *Sender);
        void __fastcall L1MouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
        void __fastcall n(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
        void __fastcall Label4MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall Label4MouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
        void __fastcall Label5MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall Label6MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall Label5MouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
        void __fastcall Label6MouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
        void __fastcall FormPaint(TObject *Sender);
        void __fastcall Timer1Timer(TObject *Sender);
        void __fastcall Timer2Timer(TObject *Sender);
        void __fastcall L1aMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
        void __fastcall L1aClick(TObject *Sender);
        void __fastcall L1Click(TObject *Sender);
        void __fastcall SnKeyPress(TObject *Sender, char &Key);
        //void __fastcall Label8Click(TObject *Sender);
        void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
        void __fastcall Label11MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall C2MouseUp(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y);
        void __fastcall Label11MouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall Label12MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall Label12MouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall SnDrawCell(TObject *Sender, int ACol, int ARow,
          TRect &Rect, TGridDrawState State);
        void __fastcall CkwKeyPress(TObject *Sender, char &Key);
        void __fastcall C2DblClick(TObject *Sender);
        void __fastcall C3DblClick(TObject *Sender);
        void __fastcall FormDblClick(TObject *Sender);
        void __fastcall BstartClick(TObject *Sender);
        void __fastcall BendClick(TObject *Sender);
        void __fastcall CgrClick(TObject *Sender);
        void __fastcall ComboBox1Change(TObject *Sender);
        void __fastcall PcolorClick(TObject *Sender);
        void __fastcall RnaprClick(TObject *Sender);
        void __fastcall CotklmClick(TObject *Sender);
        void __fastcall Panel9Click(TObject *Sender);
        void __fastcall PotkmResize(TObject *Sender);
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall CminmChange(TObject *Sender);
        void __fastcall CmaxmChange(TObject *Sender);
        void __fastcall RnaprmClick(TObject *Sender);
        void __fastcall I2mMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
        void __fastcall TimerClick(TObject *Sender);
        void __fastcall Timer3Timer(TObject *Sender);
        void __fastcall Label15MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall Label15MouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall LakoppClick(TObject *Sender);
        void __fastcall LhClick(TObject *Sender);
        void __fastcall LakoppMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall LhMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
        void __fastcall SnKeyUp(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall ClientSocket1Read(TObject *Sender,
          TCustomWinSocket *Socket);
        void __fastcall ClientSocket1Error(TObject *Sender,
          TCustomWinSocket *Socket, TErrorEvent ErrorEvent,
          int &ErrorCode);
        void __fastcall RadioGroup1Click(TObject *Sender);
        void __fastcall Button2Click(TObject *Sender);
        void __fastcall ClientSocket1Disconnect(TObject *Sender,
          TCustomWinSocket *Socket);
        void __fastcall Label17MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall Label17MouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall Timer4Timer(TObject *Sender);
        void __fastcall u32Click(TObject *Sender);
        //void __fastcall Label18Click(TObject *Sender);
        void __fastcall L1DrawCell(TObject *Sender, int ACol, int ARow,
          TRect &Rect, TGridDrawState State);
        void __fastcall FormMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
        void __fastcall Button4Click(TObject *Sender);
        void __fastcall SvClick(TObject *Sender);
        void __fastcall Sv2Click(TObject *Sender);
        void __fastcall Label19MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall Label19MouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
     void __fastcall SparClick(TObject *Sender);
     void __fastcall Lo2Click(TObject *Sender);
     void __fastcall Button22Click(TObject *Sender);
        void __fastcall Timer5Timer(TObject *Sender);
    void __fastcall CorrectorClick(TObject *Sender);
    void __fastcall ZU2Click(TObject *Sender);
    void __fastcall ZU2DrawCell(TObject *Sender, int ACol, int ARow,
          TRect &Rect, TGridDrawState State);
    void __fastcall SrvLOGClientConnect(TObject *Sender,
          TCustomWinSocket *Socket);
    void __fastcall SrvLOGClientDisconnect(TObject *Sender,
          TCustomWinSocket *Socket);
    void __fastcall SrvLOGClientError(TObject *Sender,
          TCustomWinSocket *Socket, TErrorEvent ErrorEvent,
          int &ErrorCode);
    void __fastcall SrvLOGClientRead(TObject *Sender,
          TCustomWinSocket *Socket);
    void __fastcall Panel10Click(TObject *Sender);
    void __fastcall TcpClient1Error(TObject *Sender, int SocketError);
    void __fastcall TcpClient1Receive(TObject *Sender, PChar Buf,
          int &DataLen);
    void __fastcall TcpClient1Connect(TObject *Sender);
    void __fastcall TcpClient1Disconnect(TObject *Sender);
    void __fastcall SourceGlubClick(TObject *Sender);
    void __fastcall Button8Click(TObject *Sender);
    void __fastcall PBClick(TObject *Sender);
    void __fastcall Sport2Change(TObject *Sender);
    void __fastcall Sport2DropDown(TObject *Sender);
    void __fastcall SportDropDown(TObject *Sender);
    void __fastcall Timer8Timer(TObject *Sender);
    void __fastcall CleusaDropDown(TObject *Sender);
    void __fastcall Timer9Timer(TObject *Sender);
    void __fastcall SocError(TObject *Sender, TCustomWinSocket *Socket,
          TErrorEvent ErrorEvent, int &ErrorCode);
    void __fastcall SocConnect(TObject *Sender, TCustomWinSocket *Socket);
    void __fastcall CzisDropDown(TObject *Sender);
    void __fastcall Timer10Timer(TObject *Sender);
    void __fastcall Sport2KeyPress(TObject *Sender, char &Key);
    void __fastcall WriteStatClick(TObject *Sender);
    void __fastcall ButtonStatSaveClick(TObject *Sender);
	void __fastcall L1aDrawCell(TObject *Sender, int ACol, int ARow,
          TRect &Rect, TGridDrawState State);
	void __fastcall LakoppDrawCell(TObject *Sender, int ACol, int ARow,
          TRect &Rect, TGridDrawState State);
	void __fastcall LhDrawCell(TObject *Sender, int ACol, int ARow,
          TRect &Rect, TGridDrawState State);
	void __fastcall SparDrawCell(TObject *Sender, int ACol, int ARow,
          TRect &Rect, TGridDrawState State);
    void __fastcall Cl2Click(TObject *Sender);
    void __fastcall Button5Click(TObject *Sender);
    void __fastcall CwitsDropDown(TObject *Sender);
    void __fastcall ClientSocket2Connect(TObject *Sender,
          TCustomWinSocket *Socket);
    void __fastcall ClientSocket2Disconnect(TObject *Sender,
          TCustomWinSocket *Socket);
    void __fastcall ClientSocket1Connect(TObject *Sender,
          TCustomWinSocket *Socket);
    void __fastcall ClientSocket2Error(TObject *Sender,
          TCustomWinSocket *Socket, TErrorEvent ErrorEvent,
          int &ErrorCode);
    void __fastcall ClientSocket2Read(TObject *Sender,
          TCustomWinSocket *Socket);
	void __fastcall CBLogClick(TObject *Sender);
	void __fastcall Button2leusaClick(TObject *Sender);
	void __fastcall ServerSocket1ClientConnect(TObject *Sender,
          TCustomWinSocket *Socket);
	void __fastcall ServerSocket1ClientDisconnect(TObject *Sender,
          TCustomWinSocket *Socket);
	void __fastcall ServerSocket1ClientError(TObject *Sender,
          TCustomWinSocket *Socket, TErrorEvent ErrorEvent,
          int &ErrorCode);
	void __fastcall ServerSocket1ClientRead(TObject *Sender,
          TCustomWinSocket *Socket);
	void __fastcall TimerLogTimer(TObject *Sender);
    void __fastcall CBretrClick(TObject *Sender);
    void __fastcall CBnnkClick(TObject *Sender);
    void __fastcall CBndmClick(TObject *Sender);
    void __fastcall CBgkztsClick(TObject *Sender);
	void __fastcall ClientSocketGtiConnect(TObject *Sender,
          TCustomWinSocket *Socket);
	void __fastcall ClientSocketGtiDisconnect(TObject *Sender,
          TCustomWinSocket *Socket);
	void __fastcall ClientSocketGtiError(TObject *Sender,
          TCustomWinSocket *Socket, TErrorEvent ErrorEvent,
          int &ErrorCode);
	void __fastcall ClientSocketGtiRead(TObject *Sender,
          TCustomWinSocket *Socket);
	void __fastcall CgtiDropDown(TObject *Sender);
	void __fastcall Button10gtiClick(TObject *Sender);
	void __fastcall TimerGtiTimer(TObject *Sender);
	void __fastcall Timer11Timer(TObject *Sender);
    void __fastcall BitBtnIntervalSaveClick(TObject *Sender);
    void __fastcall EBeginKeyPress(TObject *Sender, char &Key);
    void __fastcall EEndKeyPress(TObject *Sender, char &Key);
	void __fastcall MemoUgolClick(TObject *Sender);
	void __fastcall Caz_otkClick(TObject *Sender);
	void __fastcall TimerNastrTimer(TObject *Sender);
	void __fastcall PanelINKClick(TObject *Sender);
	void __fastcall RadioGroup2Click(TObject *Sender);
	void __fastcall BitBtnRunClick(TObject *Sender);
	void __fastcall Label8DblClick(TObject *Sender);
	void __fastcall StatusBarDblClick(TObject *Sender);
	void __fastcall PnastrMouseMove(TObject *Sender, TShiftState Shift, int X,
          int Y);
	void __fastcall Image1Click(TObject *Sender);
	void __fastcall PwitsMouseMove(TObject *Sender, TShiftState Shift, int X,
          int Y);
	void __fastcall CloseWitsServerClick(TObject *Sender);
	void __fastcall PwitsClientMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
	void __fastcall CloseWitsClientClick(TObject *Sender);
	void __fastcall ClosePgeoClick(TObject *Sender);
	void __fastcall PgeoMouseMove(TObject *Sender, TShiftState Shift, int X,
          int Y);
	void __fastcall CloseLeusaClick(TObject *Sender);
	void __fastcall PleusaMouseMove(TObject *Sender, TShiftState Shift, int X,
          int Y);
	void __fastcall PgtiMouseMove(TObject *Sender, TShiftState Shift, int X,
          int Y);
	void __fastcall CloseGtiClick(TObject *Sender);
	void __fastcall PanelSrvLogMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
	void __fastcall CloseLogServerClick(TObject *Sender);
	void __fastcall CloseIntervalClick(TObject *Sender);
	void __fastcall ClosePanelStringsClick(TObject *Sender);
	void __fastcall CloseStatPanelClick(TObject *Sender);
	void __fastcall Image2Click(TObject *Sender);
	void __fastcall BitBtnLasGoClick(TObject *Sender);
	void __fastcall CloseLasClick(TObject *Sender);
    void __fastcall LBLasClick(TObject *Sender);
	void __fastcall CkwKeyUp(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall PlayButtonClick(TObject *Sender);
	void __fastcall RecButtonClick(TObject *Sender);
	void __fastcall TimerPlayerTimer(TObject *Sender);
	void __fastcall PlayerSpeedButtonClick(TObject *Sender);
    void __fastcall PauseButtonClick(TObject *Sender);
    void __fastcall CplayerClick(TObject *Sender);
	void __fastcall PanelLasParamMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
    void __fastcall ClosePasportClick(TObject *Sender);
    void __fastcall PanelPasportMouseMove(TObject *Sender,
          TShiftState Shift, int X, int Y);
    void __fastcall PasportBtnClick(TObject *Sender);
    void __fastcall DirBtnClick(TObject *Sender);
    void __fastcall TablePasportKeyPress(TObject *Sender, char &Key);
	void __fastcall CdatChange(TObject *Sender);
	void __fastcall PBTypeClick(TObject *Sender);
	void __fastcall RotateButtonClick(TObject *Sender);

        private:	// User declarations

void __fastcall OnMyPost1(TMessage& Message);
void __fastcall OnMyPost2(TMessage& Message);
void __fastcall OnMyPost3(TMessage& Message);
void __fastcall OnMyPost4(TMessage& Message);
void __fastcall OnMyPost5(TMessage& Message);
void __fastcall OnMyPost6(TMessage& Message);
 
public:		// User declarations
__fastcall TFr45(TComponent* Owner);
  BEGIN_MESSAGE_MAP
       MESSAGE_HANDLER(WM_MyPost1,TMessage,OnMyPost1)
       MESSAGE_HANDLER(WM_MyPost2,TMessage,OnMyPost2)
       MESSAGE_HANDLER(WM_MyPost3,TMessage,OnMyPost3)
       MESSAGE_HANDLER(WM_MyPost4,TMessage,OnMyPost4)
       MESSAGE_HANDLER(WM_MyPost5,TMessage,OnMyPost5)
       MESSAGE_HANDLER(WM_MyPost6,TMessage,OnMyPost6)
       MESSAGE_HANDLER(WM_SYSCOMMAND, TMessage, WMSysCommand)
   END_MESSAGE_MAP(TComponent)
void __fastcall WMSysCommand(TMessage &Msg);

};
//---------------------------------------------------------------------------
extern PACKAGE TFr45 *Fr45;
//---------------------------------------------------------------------------
#endif
