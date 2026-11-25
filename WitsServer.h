//=============================================================================================================
//=============================================================================================================
//=============================================================================================================
//=============================================================================================================
//====================Тут находится серверная часть============================================================
void __fastcall TFr45::Sv2Click(TObject *Sender)//check WITS
{
if(Sv2->Checked)
{
 Pwits->Left=559;
 Pwits->Top=135;
 Pwits->Visible=true;//Панелька WITS
 Button22Click(Sender);//кнопка Перезапустить
 if(Cl2->Checked) Cl2->Checked=false;//отключить клиент WITS
}
else
{
 Pwits->Visible=false;
 int a=ServerSocket1->Socket->ActiveConnections;
 if(a>0) {for(int b=0;b<a;b++) ServerSocket1->Socket->Connections[b]->Close();}
 ServerSocket1->Close();//закрываем сокет сервера
 ServerSocket1->Active=false;//выключаем сервер
}
}
//========================================================================
void __fastcall TFr45::Button22Click(TObject *Sender)
{
 nport=StrToInt(Sn->Cells[1][7]);
 if((nport<134)||(nport>65535)) {ShowMessage("Номер порта должен быть в диапазоне 134-65535"); return;}

if(ServerSocket1->Active==false)
{
  ServerSocket1->Port= nport;
  ServerSocket1->Open();
  cerver2->Caption="Ждем подключения WITS к порту "+IntToStr(nport);
  Label21->Caption="WITS-сервер (порт "+IntToStr(nport)+")";
  Label8->Caption="";
}
if(ServerSocket1->Active==true)
{
int a,b;
  a=ServerSocket1->Socket->ActiveConnections;
  if(a>0) {for(b=0;b<a;b++) ServerSocket1->Socket->Connections[b]->Close();}
  ServerSocket1->Close();
  cerver2->Caption="Сервер остановлен";
  Label8->Caption="";
  ServerSocket1->Port= nport;
  ServerSocket1->Open();
  cerver2->Caption="Ждем подключения WITS к порту "+IntToStr(nport);
  Label21->Caption="WITS-сервер (порт "+IntToStr(nport)+")";
  Label8->Caption="";
}
SaveLog("Старт WITS-сервера======================================================");
}
//========================================================================
void __fastcall TFr45::CloseWitsServerClick(TObject *Sender)//Кнопка Закрыть
{
Pwits->Visible=false;
}
//========================================================================
void __fastcall TFr45::ServerSocket1ClientConnect(TObject *Sender,TCustomWinSocket *Socket)
{
AnsiString s;
 if(SockNumber==0) //Соединяемся с первым попавшимся клиентом
 {
  SockNumber=Socket->SocketHandle;
  //Pwits->Visible=true;
  s="Хост "+Socket->RemoteHost+"  подключен к порту "+IntToStr(nport);
  cerver2->Caption=s;
  Label8->Caption=s;
  SaveLog(s);
  WitsServer_ON=true;
  //Timer5->Enabled=true;//Запустим таймер
 }
 else
 {Socket->Close(); //Остальные соединения закрываем
  s=Socket->RemoteHost;
  if(s=="") s="(?)";
  s="чужой сокет "+s+" пытался подключиться";
  SaveLog(s);
  //Timer5->Enabled=false;//Остановим таймер
 }
 Application->ProcessMessages();
}
//========================================================================
void __fastcall TFr45::ServerSocket1ClientDisconnect(TObject *Sender,TCustomWinSocket *Socket)
{
AnsiString s;
 if(SockNumber==Socket->SocketHandle)
 {
  SockNumber=0; //Освобождаем номер сокета для повторного соединения
  s="Хост  "+Socket->RemoteHost+"  отключился ";
  if(Socket->RemoteHost=="") s=s+"(обрыв связи?)";
  cerver2->Caption=s;
  Label8->Caption=s;
  //Pwits->Visible=true;
  //Timer5->Enabled=false;//Остановим таймер
  SaveLog(s);
  WitsServer_ON=false;
 }
 Application->ProcessMessages();
}
//========================================================================
void __fastcall TFr45::ServerSocket1ClientError(TObject *Sender,TCustomWinSocket *Socket, TErrorEvent ErrorEvent, int &ErrorCode)
{
AnsiString s;
 Pwits->Visible=true;
 if(SockNumber==Socket->SocketHandle)
 {
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
  cerver2->Caption=s;
  Label8->Caption=s;
  Socket->Close();
  //Timer5->Enabled=false;//Остановим таймер
  SockNumber=0;
  s=s+"= "+IntToStr(ErrorCode);
  SaveLog(s);
  //10060-нет в сети, 10061- нет программы
 }
 else Socket->Close();
 ErrorCode=0;
 Application->ProcessMessages();
}
//========================================================================
void __fastcall TFr45::ServerSocket1ClientRead(TObject *Sender,TCustomWinSocket *Socket)
{
AnsiString s;
    s="Неожиданный пакет принят сервером WITS:\r\n";
    s+=Socket->ReceiveText();
    SaveWitsLog(s);
}
//========================================================================
void __fastcall TFr45::Timer5Timer(TObject *Sender)
{
/*static int n;
 if(SockNumber>0)
 {
  //ServerSocket1->Socket->Connections[0]->SendText("bbb");
  if(n==0) n=700;
  AnsiString s="0";
  SendToLeuza(s+IntToStr(n), FloatToStrF0((float)n/10,3,1));
  StatusBar->SimpleText="n="+IntToStr(n);
  n++;
  if(n==1000) n=700;
 }*/
}
//========================================================================
void SaveLog(AnsiString s)
{
FILE *log;
AnsiString dt, file=CoreDir+"\\server.log";//в корневой папке программы
 log=NULL;
 while((log=fopen(file.c_str(),"a+t"))==NULL) Application->ProcessMessages();
 DateTimeToString(dt,"dd.mm.yy=hh:nn:ss",Now());
 fprintf(log,"%s\n",dt+"= "+s);
 fflush(log);
 fclose(log);
}
//========================================================================
//перемещение панели
void __fastcall TFr45::PwitsMouseMove(TObject *Sender, TShiftState Shift,
      int X, int Y)
{
 const int SC_DRAGMOVE=61458;//0xF012;
 ReleaseCapture();
 ((TControl *)Sender)->Perform(WM_SYSCOMMAND,SC_DRAGMOVE,0);
}
//========================================================================
 