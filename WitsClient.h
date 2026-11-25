//=============================================================================================================
//=============================================================================================================
//=============================================================================================================
//=============================================================================================================
//==================== Тут находится клиентская часть WITS ============================================================
void __fastcall TFr45::Cl2Click(TObject *Sender)//галочка выбора клиента
{
  if(Cl2->Checked)
  {
    nport_wits=StrToInt(Sn->Cells[1][7]);//номер порта WITS
    Label23->Caption="WITS-клиент (порт "+IntToStr(nport_wits)+")";
    PwitsClient->Left=559;
    PwitsClient->Top=135;
    PwitsClient->Visible=true;//Панелька WITS клиент
    Button5->Enabled=true;
    if(Sv2->Checked) {Sv2->Checked=false;}//отключить сервер WITS
  }
  else
  {
    Fr45->ClientSocket2->Active=false;//отключаем клиента
    PwitsClient->Visible=false;//Панелька WITS клиент
    Button5->Enabled=false;
  }
}
//========================================================================

void __fastcall TFr45::Button5Click(TObject *Sender)//кнопка Подключиться
{
  nport_wits=StrToInt(Sn->Cells[1][7]);//номер порта WITS
  Label23->Caption="WITS-клиент (порт "+IntToStr(nport_wits)+")";
  if((nport_wits<134)||(nport_wits>65535)) {ShowMessage("Номер порта должен быть в диапазоне 134-65535"); return;}
  ClientSocket2->Port= nport_wits;//порт
  ClientSocket2->Host= Cwits->Text;// IP
  ClientSocket2->Active=true;
  cerver4->Caption="Ждите! Идет подключение к серверу "+ClientSocket2->Host;
  Label8->Caption="";
  SaveLog("Старт WITS-клиента======================================================");
}
//---------------------------------------------------------------------------

void __fastcall TFr45::CwitsDropDown(TObject *Sender)
{
  Fr45->ClientSocket2->Active=false;
  cerver4->Caption="Отключен!";
  Application->ProcessMessages();
  Cwits->Items->Clear();
  FillNetLevel(0,Cwits->Items);
  Button5->Enabled=true;
}
//---------------------------------------------------------------------------
//перемещение панели
void __fastcall TFr45::PwitsClientMouseMove(TObject *Sender, TShiftState Shift, int X, int Y)
{
 const int SC_DRAGMOVE=61458;//0xF012;
 ReleaseCapture();
 ((TControl *)Sender)->Perform(WM_SYSCOMMAND,SC_DRAGMOVE,0);
}
//---------------------------------------------------------------------------

void __fastcall TFr45::ClientSocket2Connect(TObject *Sender, TCustomWinSocket *Socket)
{
 AnsiString s="WITS-клиент подключен к серверу "+ClientSocket2->Host;
 cerver4->Caption=s;
 Label8->Caption=s;
 SaveLog(s);
 WitsClient_ON=true;
 PwitsClient->Visible=true;
 Button5->Enabled=false;
 //SendOtkToWits("25", "26");
}
//---------------------------------------------------------------------------

void __fastcall TFr45::ClientSocket2Disconnect(TObject *Sender, TCustomWinSocket *Socket)
{
AnsiString s="WITS-клиент отключился от сервера";
cerver4->Caption=s;
Label8->Caption=s;
SaveLog(s);
PwitsClient->Visible=true;
WitsClient_ON=false;
}
//---------------------------------------------------------------------------
void __fastcall TFr45::ClientSocket2Error(TObject *Sender,
      TCustomWinSocket *Socket, TErrorEvent ErrorEvent, int &ErrorCode)
{
AnsiString s;
switch(ErrorEvent)
{
case eeSend: s="Ошибка записи сообщения";
break;
case eeReceive: s="Ошибка чтения из "+Cwits->Text;
break;
case eeConnect: s="Ошибка подключения к "+Cwits->Text;
break;
case eeDisconnect: s="Ошибка закрытия "+Cwits->Text;
break;
case eeAccept: s="Ошибка доступа "+Cwits->Text;
break;
case eeGeneral: s="Ошибка соединения с "+Cwits->Text;
break;
default: s="Неизвестная ошибка "; break;
}
cerver4->Caption=s;
Label8->Caption=s;
SaveLog(s);
PwitsClient->Visible=true;
ErrorCode=0;
}
//---------------------------------------------------------------------------
void __fastcall TFr45::ClientSocket2Read(TObject *Sender, TCustomWinSocket *Socket)
{//
}
//========================================================================
//закрыть панель
void __fastcall TFr45::CloseWitsClientClick(TObject *Sender)
{
PwitsClient->Visible=false;
}
//========================================================================
