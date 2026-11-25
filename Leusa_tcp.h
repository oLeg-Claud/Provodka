//=============================================================================
void __fastcall TFr45::Button2leusaClick(TObject *Sender)//кнопка Подключить
{
  Fr45->TcpClient1->Active=false;
  Port_Leuza=StrToInt(Sn->Cells[1][16]);//номер порта
  Fr45->TcpClient1->RemotePort= Port_Leuza;
  Fr45->TcpClient1->RemoteHost= Cleusa->Text;
  //cerver3->Caption="";
  Fr45->TcpClient1->Active=true;
  if(Fr45->TcpClient1->Active==false) ShowMessage("не удалось подключить");
  else
  {
  Fr45->Timer8->Enabled=false;//выключаем вачдог, включает его поток
  Thr = new LThread(false);//запускаем поток
  Thr->FreeOnTerminate=true;
  }
}
//=============================================================================
void __fastcall TFr45::TcpClient1Error(TObject *Sender, int SocketError)
{
    //Fr45->TcpClient1->Active=false;
    //Timer8->Enabled=true;//включаем вачдог
    cerver3->Caption="ошибка Леузы="+IntToStr(SocketError);
    SocketError=0;
}
//=============================================================================
void __fastcall TFr45::TcpClient1Receive(TObject *Sender, PChar Buf,int &DataLen)
{
//StatusBar->SimpleText="Сработал TcpRecieve, DataLen="+IntToStr(DataLen);
}
//=============================================================================
void __fastcall TFr45::TcpClient1Connect(TObject *Sender)
{
    cerver3->Caption="подключен к порту "+Fr45->TcpClient1->RemotePort;
}
//=============================================================================
void __fastcall TFr45::TcpClient1Disconnect(TObject *Sender)
{

}
//=============================================================================
void __fastcall TFr45::CloseLeusaClick(TObject *Sender)//закрыть
{
Pleusa->Visible=false;
}
//=============================================================================
//перемещение панели
void __fastcall TFr45::PleusaMouseMove(TObject *Sender, TShiftState Shift, int X, int Y)
{
 const int SC_DRAGMOVE=61458;//0xF012;
 ReleaseCapture();
 ((TControl *)Sender)->Perform(WM_SYSCOMMAND,SC_DRAGMOVE,0);
}
//=============================================================================
void __fastcall TFr45::Timer8Timer(TObject *Sender)
{
    if(PotokON) {PotokON=0; while(!PotokOFF)Application->ProcessMessages();}//останавливаем поток
    Fr45->Timer8->Enabled==false;
    Fr45->Timer8->Interval=5000;
    Fr45->Timer8->Enabled==true;
    Fr45->TcpClient1->Active=false;
    Fr45->cerver3->Caption="потеря связи"+(IntToStr(CountConnects++));
    //ShowMessage("Timer8->Interval="+IntToStr(Timer8->Interval));
    Fr45->Soc->Host=Cleusa->Text;
    Fr45->Soc->Port=StrToInt(Fr45->TcpClient1->RemotePort);
    Fr45->Soc->ClientType=ctNonBlocking;
    Fr45->Soc->Active=true;
}
//=============================================================================
void __fastcall TFr45::CleusaDropDown(TObject *Sender)
{
    Fr45->Timer8->Enabled=false;
    Fr45->TcpClient1->Active=false;
    if(PotokON) {PotokON=0; while(!PotokOFF)Application->ProcessMessages();}//останавливаем поток
    cerver3->Caption="Отключен!";
    Application->ProcessMessages();
    Cleusa->Items->Clear();
    FillNetLevel(0,Cleusa->Items);
}
//=============================================================================
void __fastcall TFr45::SocError(TObject *Sender, TCustomWinSocket *Socket,
      TErrorEvent ErrorEvent, int &ErrorCode)
{
 Soc->Active=false;
 ErrorCode=0;
}
//=============================================================================
void __fastcall TFr45::SocConnect(TObject *Sender, TCustomWinSocket *Socket)
{
 Soc->Active=false;
 Fr45->Timer8->Enabled=false;//выключаем вачдог
 Button2leusaClick(Sender);
}
//=============================================================================
//=============================================================================
