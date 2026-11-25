//=============================================================================================================
//=============================================================================================================
//=============================================================================================================
//=============================================================================================================
//====================Тут находится серверная часть============================================================
void __fastcall TFr45::CBLogClick(TObject *Sender)//check LOG
{
if(CBLog->Checked)
{
 PanelSrvLog->Visible=true;//Панелька LOG
 Button8Click(Sender);//кнопка Перезапустить
}
else
{
 PanelSrvLog->Visible=false;
 int a=SrvLOG->Socket->ActiveConnections;
 if(a>0) {for(int b=0;b<a;b++) SrvLOG->Socket->Connections[b]->Close();}//закрываем все коннекты
 SrvLOG->Close();
 SrvLOG->Active=false;
 TimerLog->Enabled=false;//Остановим таймер (отладка)
}
}
//========================================================================
void __fastcall TFr45::Button8Click(TObject *Sender)
{
 int portLog=27910;
 //if((nport<134)||(nport>65535)) {ShowMessage("Номер порта должен быть в диапазоне 134-65535"); return;}
              //host 192.168.0.1 and tcp port 27910
if(SrvLOG->Active==false)
{
  SrvLOG->Port= portLog;
  SrvLOG->Open();
  Label18->Caption="Ждем подключения клиентов к порту REC";//+IntToStr(portLog);//внизу панельки LOG
  Label24->Caption="REC-сервер";// (порт "+IntToStr(portLog)+")";//заголовок панельки LOG
  Label8->Caption="";//на главном окне
}
if(SrvLOG->Active==true)
{
int a,b;
  a=SrvLOG->Socket->ActiveConnections;
  if(a>0) {for(b=0;b<a;b++) SrvLOG->Socket->Connections[b]->Close();}
  SrvLOG->Close();
  Label18->Caption="Сервер остановлен";
  Label8->Caption="";
  SrvLOG->Port= portLog;
  SrvLOG->Open();
  Label18->Caption="Ждем подключения клиентов к порту REC";//+IntToStr(portLog);
  Label24->Caption="REC-сервер";// (порт "+IntToStr(portLog)+")";
  Label8->Caption="";
}
SaveLog("Старт REC-сервера======================================================");
}
//========================================================================
void __fastcall TFr45::CloseLogServerClick(TObject *Sender) //Кнопка Закрыть
{
PanelSrvLog->Visible=false;
}
//========================================================================
void __fastcall TFr45::SrvLOGClientConnect(TObject *Sender,TCustomWinSocket *Socket)
{
AnsiString s;
 if(Fr45->SrvLOG->Socket->ActiveConnections < 4)//подключаем не больше 4х клиентов
 {
  //LogNumber=Socket->SocketHandle;//хэндл клиента
  s="Хост "+Socket->RemoteHost+"  подключен к порту REC";//+IntToStr(SrvLOG->Port);
  Label18->Caption=s;
  Label8->Caption=s;
  SaveLog(s);
  //ShowMessage(Socket->RemoteAddress);
  //TimerLog->Enabled=true;//Запустим таймер (отладка)
 }
 else
 {Socket->Close(); //Остальные соединения закрываем
  s=Socket->RemoteHost;
  if(s=="") s="(?)";
  s="чужой сокет "+s+" пытался подключиться";
  SaveLog(s);
 }
 Application->ProcessMessages();
}
//========================================================================
void __fastcall TFr45::SrvLOGClientDisconnect(TObject *Sender,TCustomWinSocket *Socket)
{
AnsiString s;
  s="Хост  "+Socket->RemoteHost+"  отключился ";
  if(Socket->RemoteHost=="") s=s+"(обрыв связи?)";
  Label18->Caption=s;
  Label8->Caption=s;
  if(Fr45->SrvLOG->Socket->ActiveConnections<1) TimerLog->Enabled=false;//Остановим таймер
  SaveLog(s);
 Application->ProcessMessages();
}
//========================================================================
void __fastcall TFr45::SrvLOGClientError(TObject *Sender,TCustomWinSocket *Socket, TErrorEvent ErrorEvent, int &ErrorCode)
{
AnsiString s;
 //PanelSrvLog->Visible=true;//не будем активировать окошко при ошибке
  switch(ErrorEvent)
  {
   case eeSend: s="Ошибка записи сообщения клиенту "; break;
   case eeReceive: s="Ошибка чтения сообщения клиента "; break;
   case eeConnect: s="Ошибка открытия соединения с клиентом "; break;
   case eeDisconnect: s="Ошибка закрытия соединения с клиентом "; break;
   case eeAccept: s="Ошибка доступа к клиенту "; break;
   case eeGeneral: s="Ошибка соединения с  клиентом "; break;
   default: s="Неизвестная ошибка "; break;
  }
  Label18->Caption=s;
  Label8->Caption=s;
  Socket->Close();
  s=s+"= "+IntToStr(ErrorCode);
  SaveLog(s);
  //10060-нет в сети, 10061- нет программы
 ErrorCode=0;
 Application->ProcessMessages();
}
//========================================================================
void __fastcall TFr45::SrvLOGClientRead(TObject *Sender,TCustomWinSocket *Socket)
{
AnsiString s;
    s="Неожиданный пакет принят сервером REC:\r\n";
    s+=Socket->ReceiveText();
    SaveLog(s);
}
//========================================================================
const uint8_t zag2[]=
{
0xff,0x01,0xff,0xff,0xff,0xff,0xff,0xff,0x20,0x4e,0xd8,0x13,0x00,0x00,0xff,0xff,
0xa0,0x0f,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x02,0x21,0xff,0xff,0x01,0x00,
0x11,0x00,0xff,0xff,0x8c,0xa7,0x03,0x00,0xa1,0xa0,0x03,0x00,0x34,0xa5,0x03,0x00,
0x82,0xa1,0x03,0x00,0x66,0xa7,0x03,0x00,0x82,0xa1,0x03,0x00,0x26,0xa4,0x03,0x00,
0xff,0xff,0xff,0xff,0xc8,0x9d,0x03,0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0x0c,0xae,0x9c,0x3f,0x00,0x00,0x00,0x00,0xff,0xff,0xff,0xff,
0x00,0x00,0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x7b,0x14,0x1d,0x42,
0x00,0x00,0x00,0x00,0x0c,0xae,0x1c,0x40,0x61,0x17,0x64,0x1e,0xdd,0x0f,0x6a,0x27,
0x00,0x00,0x80,0xbf,0x38,0xa8,0xc7,0x40,0xff,0xff,0xff,0xff,0x1f,0x85,0xb3,0x40,
0x00,0x00,0x00,0x00,0x10,0x3e,0x78,0x42,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00,0x48,0x43,0xff,0xff,0xff,0xff,
0x1d,0x2b,0x32,0x57,0xb2,0x08,0xe6,0x40,0xa8,0x11,0xff,0xff,0x00,0x00,0x00,0x00,//время 1d2b3257b208e640=18.07.2023 13:45:19
0x1a,0x00,0x31,0x01,0x17,0x00,0x17,0x00,0xff,0xff,0xff,0xff,0xff,0xff,0x06,0x00,
0x0c,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0x9c,0x0a,0x84,0x08,0x7d,0x01,0x8f,0x02,0x01,0x00,0x17,0x12,0x07,0x0d,
0x2d,0x2c,0x33,0x01,0x5d,0x09,0xff,0xff,0xff,0xff,0xff,0x00,0x43,0x02,0x06,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x5c,0xb5,0x19,0x42,0x70,0x4b,0x8c,0x65,
0x47,0xb7,0x88,0x5c,0x00,0x00,0x80,0xbf,0x58,0x46,0x20,0x43,0x00,0x00,0x80,0x41,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff
};
//========================================================================
//пока пробуем послать отладочный пакет по таймеру 1 сек
void __fastcall TFr45::TimerLogTimer(TObject *Sender)
{
BUF_LOG_Type buf;
float fl, mk;
static int count_Log_packets=0;

 //засылаем в порт
 if(Fr45->SrvLOG->Active==true && Fr45->SrvLOG->Socket->ActiveConnections)//если есть клиенты
 {
  for(int a=0;a<DLINA_BUF_LOG;a++) buf.mb[a]=0xff;//чистим буфер
  //for(int a=0;a<sizeof(zag2);a++) buf.mb[a]=zag2[a];//кладем заготовку
  buf.param.res13 = 1;//так надо
  buf.param.res14 = 17;
  buf.mb[253]=0;//x02;
  count_Log_packets+=1;//%128;

  //раскладываем параметры
  buf.param.Datetime = Now();//время сразу кладем
  GetDateTime(&buf.mb[236]);//кладем еще одно время, байтовое
  buf.param.BlockSize = DLINA_BUF_LOG - 1;//длина буфера в начале
  fl = MyStrToFloat(Eglubina->Text);//глубина долота
  if(fl<0) buf.param.GlubDol = 0; else buf.param.GlubDol = inta(fl*100);
  //сначала кладем ГК от ЗТС
  mk = fl - MyStrToFloat(Fr45->Sn->Cells[1][14]);//мертвый конец ГК ЗТС
  //ГК будем передавать по второму каналу ГК2
  if(mk<0) buf.param.GlubGK2 = 0; else buf.param.GlubGK2 = inta(mk*100);//глубина ГК2
  buf.param.GK2 = (float)count_Log_packets/10;//gkzts;//ГК2
  if(mk<0) buf.param.GlubZU = 0; else buf.param.GlubZU = inta(mk*100);//глубина ЗУ
  buf.param.ZUzts = 9000;
  buf.param.AZzts = 27000;

  //если есть НДМ, то добавляем
  mk = fl - MyStrToFloat(Fr45->Sn->Cells[1][15]);//мертвый конец НДМ
  if(mk<0) buf.param.GlubNDM = 0; else buf.param.GlubNDM = inta(mk*100);//глубина НДМ
  buf.param.ZUndm = 9001;//inta(ZUndm*100);
  buf.param.GKndm = 10.1;//roundf(GKndm_int,1);
  buf.param.GKndm_up = 11.1;//roundf(GKup,1);
  buf.param.GKndm_dn = 12.1;//roundf(GKdn,1);
  buf.param.NAGRndm = 13.1;//roundf(NAGRndm,1);
  buf.param.OBORndm = 900;//(int)OBORndm;
  buf.param.DAVLout = 14.11;//roundf(DAVLndm,2);

  for(int a=0;a<Fr45->SrvLOG->Socket->ActiveConnections;a++)
  { Fr45->SrvLOG->Socket->Connections[a]->SendBuf(buf.mb, DLINA_BUF_LOG);
    Label8->Caption="Послал пакет "+IntToStr(count_Log_packets);
  }
 }
 else
 {TimerLog->Enabled=false;//Остановим таймер
  count_Log_packets=0;
 }
}
//========================================================================
void sendLogParam(const char* ptr)
{
String str = String(ptr);
BUF_LOG_Type buf;
float fl, mk;
 //засылаем в порт
 if(Fr45->SrvLOG->Active==true && Fr45->SrvLOG->Socket->ActiveConnections)//если есть клиенты
 {
  for(int a=0;a<DLINA_BUF_LOG;a++) buf.mb[a]=0xff;//чистим буфер
  buf.param.res13 = 1;//так надо
  buf.param.res14 = 17;
  buf.mb[253]=0;//x02;

  //раскладываем независимые параметры
  buf.param.Datetime = Now();//время сразу кладем
  GetDateTime(&buf.mb[236]);//кладем еще одно время, байтовое
  buf.param.BlockSize = DLINA_BUF_LOG - 1;//длина буфера в начале
  fl = MyStrToFloat(Fr45->Eglubina->Text);//глубина долота
  if(fl<0) buf.param.GlubDol = 0; else buf.param.GlubDol = inta(fl*100);//глубина долота

  //теперь кладем параметр из аргумента
  if(str=="GKzts")//по приходу параметра
  {	mk = fl - MyStrToFloat(Fr45->Sn->Cells[1][14]);//мертвый конец ГК ЗТС
  	//ГК будем передавать по второму каналу ГК2
  	if(mk<0) buf.param.GlubGK2 = 0; else buf.param.GlubGK2 = inta(mk*100);//глубина ГК2
  	if(gkzts!=-9999) buf.param.GK2 = gkzts;//ГК2
    //Зенит и Азимут
    if(mk<0) buf.param.GlubZU = 0; else buf.param.GlubZU = inta(mk*100);//глубина ЗУ
    if(zenit!=-9999) buf.param.ZUzts = inta(zenit*100);//ЗУзтс
    if(MODE_AZ==0) {if(Azk!=-9999) buf.param.AZzts = inta(Azk*100);}//АЗзтс магнитный
    else {if(Azk_ist!=-9999) buf.param.AZzts = inta(Azk_ist*100);}//АЗзтс истинный
  }
  else if(str=="GKndm")//по приходу обоих параметров
  {	mk = fl - MyStrToFloat(Fr45->Sn->Cells[1][15]);//мертвый конец НДМ
  	if(mk<0) buf.param.GlubNDM = 0; else buf.param.GlubNDM = inta(mk*100);//глубина НДМ
    if(GKndm_int!=-9999) buf.param.GKndm = roundf(GKndm_int,1);
    if(GKup!=-9999) buf.param.GKndm_up = roundf(GKup,1);
    if(GKdn!=-9999) buf.param.GKndm_dn = roundf(GKdn,1);
  }
  else if(str=="NDMall")//в конце кадра
  {	mk = fl - MyStrToFloat(Fr45->Sn->Cells[1][15]);//мертвый конец НДМ
  	if(mk<0) buf.param.GlubNDM = 0; else buf.param.GlubNDM = inta(mk*100);//глубина НДМ
    if(ZUndm!=-9999) buf.param.ZUndm = inta(ZUndm*100);
    if(NAGRndm!=-9999) buf.param.NAGRndm = roundf(NAGRndm,1);
    if(OBORndm!=-9999) buf.param.OBORndm = (int)OBORndm;
    if(DAVLndm!=-9999) buf.param.DAVLout = roundf(DAVLndm,2);
    if(KCndm!=-9999) buf.param.KCndm = roundf(KCndm,2);
    //если надо, то допишем сопротивление нагрузки
    //if(mk<0) buf.param.GlubRnagr = 0; else buf.param.GlubRnagr = inta(mk*100);//глубина Rнагрузки
  }
  else return;

  for(int a=0;a<Fr45->SrvLOG->Socket->ActiveConnections;a++)
  { Fr45->SrvLOG->Socket->Connections[a]->SendBuf(buf.mb, DLINA_BUF_LOG);
    //Label8->Caption="Послал пакет "+IntToStr(count_Log_packets);
  }
 }
}
//========================================================================
void GetDateTime(uint8_t *mas)
{
uint16_t Year,Month,Day,Hour,Min,Sec,msec;
TDateTime Today,TTime;
    Today = Date();
    TTime = Time();
    Today.DecodeDate(&Year, &Month, &Day);
    TTime.DecodeTime(&Hour, &Min, &Sec, &msec);
    mas[0] = Year-2000;
    mas[1] = Day;
    mas[2] = Month;
    mas[3] = Hour;
    mas[4] = Min;
    mas[5] = Sec;
    //ShowMessage(IntToStr(Year)+"/"+IntToStr(Month)+"/"+IntToStr(Day)+"  "+IntToStr(Hour)+":"+IntToStr(Min)+":"+IntToStr(Sec));
}
//========================================================================
uint16_t swap16(uint16_t x)//реверс порядка байтов
{
un16 tmp;
	tmp.word=x;
	x=tmp.H;
	tmp.H=tmp.L;
	tmp.L=x;
	return tmp.word;
}
//==================================================================================
uint32_t swap32(uint32_t x)//реверс порядка байтов
{
un32 a,b;
	b.word=a.word=x;
	a.b[3]=b.b[0];//мл. x
	a.b[2]=b.b[1];
	a.b[1]=b.b[2];
	a.b[0]=b.b[3];//ст. x
	return a.word;
}
//==================================================================================
float swapFloat(float x)//реверс порядка байтов
{
fl32 b,a;
	b.f=a.f=x;
	a.b[3]=b.b[0];//мл. x
	a.b[2]=b.b[1];
	a.b[1]=b.b[2];
	a.b[0]=b.b[3];//ст. x
	return a.f;
}
//==================================================================================
uint64_t swapDouble(uint64_t x)//реверс порядка байтов
{
sun64 b,a;
	b.word=a.word=x;
    a.b[7]=b.b[0];//мл. x
	a.b[6]=b.b[1];
	a.b[5]=b.b[2];
	a.b[4]=b.b[3];
	a.b[3]=b.b[4];
	a.b[2]=b.b[5];
	a.b[1]=b.b[6];
	a.b[0]=b.b[7];//ст. x
	return a.word;
}
//==================================================================================
//перемещение панели
void __fastcall TFr45::PanelSrvLogMouseMove(TObject *Sender, TShiftState Shift, int X, int Y)
{
 const int SC_DRAGMOVE=61458;//0xF012;
 ReleaseCapture();
 ((TControl *)Sender)->Perform(WM_SYSCOMMAND,SC_DRAGMOVE,0);
}
//========================================================================
 