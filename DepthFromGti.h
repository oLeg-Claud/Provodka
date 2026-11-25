//=============================================================================================================
//=============================================================================================================
//=============================================================================================================
//=============================================================================================================
//==================== Тут находится клиентская часть слушателя WITS от ГТИ ============================================================

void __fastcall TFr45::Button10gtiClick(TObject *Sender)//кнопка Подключиться
{
  ClientSocketGti->Active=false;
  port_gti=StrToInt(Sn->Cells[1][18]);//номер порта WITS-ГТИ
  Label26->Caption="Глубина от ГТИ-wits (порт "+IntToStr(port_gti)+")";
  if((port_gti<134)||(port_gti>65535)) {ShowMessage("Номер порта должен быть в диапазоне 134-65535"); return;}
  ClientSocketGti->Port= port_gti;//порт
  ClientSocketGti->Host= Cgti->Text;// IP
  ClientSocketGti->Active=true;
  cerverGti->Caption="Ждите! Идет подключение к серверу "+ClientSocketGti->Host;
  Label8->Caption="";
  SaveLog("Старт ГТИ-wits-клиента======================================================");
  TimerGti->Enabled=false;//выключаем вачдог
  Button10gti->Enabled=false;
}
//---------------------------------------------------------------------------
void __fastcall TFr45::CgtiDropDown(TObject *Sender)
{
  ClientSocketGti->Active=false;
  TimerGti->Enabled=false;//выключаем вачдог
  cerverGti->Caption="Отключен!";
  Application->ProcessMessages();
  Cgti->Items->Clear();
  FillNetLevel(0,Cgti->Items);
  Button10gti->Enabled=true;
  GtiClient_ON=false;
}
//---------------------------------------------------------------------------
void __fastcall TFr45::CloseGtiClick(TObject *Sender)
{
  Pgti->Visible=false;
}
//---------------------------------------------------------------------------
void __fastcall TFr45::ClientSocketGtiConnect(TObject *Sender, TCustomWinSocket *Socket)
{
 AnsiString s="ГТИ-wits подключен к серверу "+ClientSocketGti->Host;
 cerverGti->Caption=s;
 Label8->Font->Color=clBlack;//черный
 Label8->Caption=s;
 SaveLog(s);
 GtiClient_ON=true;
 Pgti->Visible=true;
 Button10gti->Enabled=false;
 TimerGti->Enabled=false;//выключаем вачдог
 Timer11->Enabled=true;//включаем время жизни сообщения на панели Label8
}
//---------------------------------------------------------------------------

void __fastcall TFr45::ClientSocketGtiDisconnect(TObject *Sender, TCustomWinSocket *Socket)
{
AnsiString s="ГТИ-wits отключился от сервера";
cerverGti->Caption=s;
Label8->Font->Color=clFuchsia;
Label8->Caption=s;
SaveLog(s);
Button10gti->Enabled=true;
GtiClient_ON=false;
if(SourceGlub->ItemIndex==2 && Sv->Checked==true) TimerGti->Enabled=true;//включаем вачдог
if(SourceGlub->ItemIndex==2 && Sv->Checked==true) Pgti->Visible=true;
}
//---------------------------------------------------------------------------
void __fastcall TFr45::ClientSocketGtiError(TObject *Sender,
      TCustomWinSocket *Socket, TErrorEvent ErrorEvent, int &ErrorCode)
{
AnsiString s;
switch(ErrorEvent)
{
case eeSend: s="Ошибка записи сообщения";
break;
case eeReceive: s="Ошибка чтения из "+Cgti->Text;
break;
case eeConnect: s="Ошибка подключения к "+Cgti->Text;
break;
case eeDisconnect: s="Ошибка закрытия "+Cgti->Text;
break;
case eeAccept: s="Ошибка доступа "+Cgti->Text;
break;
case eeGeneral: s="Ошибка соединения с "+Cgti->Text;
break;
default: s="Неизвестная ошибка "; break;
}
cerverGti->Caption=s;
Label8->Font->Color=clFuchsia;
Label8->Caption=s;
SaveLog(s);
ErrorCode=0;
Button10gti->Enabled=true;
if(SourceGlub->ItemIndex==2 && Sv->Checked==true && GtiClient_ON) TimerGti->Enabled=true;//включаем вачдог
if(SourceGlub->ItemIndex==2 && Sv->Checked==true) Pgti->Visible=true;
}
//---------------------------------------------------------------------------
void __fastcall TFr45::ClientSocketGtiRead(TObject *Sender, TCustomWinSocket *Socket)
{
AnsiString s;
    Memo1->Lines->Clear();
    Memo1->Lines->Add(Socket->ReceiveText());//читаем пакет
    //найдем строку с номером скважины
    for(int i=0;i<Memo1->Lines->Count;i++)
    {s=Memo1->Lines->Strings[i];
     if(s.Pos("0101")==1)
     {IDskv=s.Delete(1,4);
      //Memo1->Lines->Delete(i);//удаляем строку
      break;
     }//строка с идентификатором скважины
    }
    //найдем строку с глубиной забоя
    for(int i=0;i<Memo1->Lines->Count;i++)
    {s=Memo1->Lines->Strings[i];
     if(s.Pos("0110")==1) {glub_Zab=MyStrToFloat(s.Delete(1,4)); break;}//глубина забоя
    }
    //найдем строку с глубиной долота
    for(int i=0;i<Memo1->Lines->Count;i++)
    {s=Memo1->Lines->Strings[i];
     if(s.Pos("0108")==1) {glub_Dol=MyStrToFloat(s.Delete(1,4)); break;}//глубина долота
    }
    //если глубина Внешняя - покажем глубину долота
    if(RadioGroup1->ItemIndex==0) Fr45->Eglubina->Text=FloatToStrF0(glub_Dol,7,2);//

    //Memo1->Lines->Clear();
    //Memo1->Lines->Add("ID скважины = "+IDskv);
    //Memo1->Lines->Add("Глубина долота = "+FloatToStrF0(glub_Dol,6,2));
    //Memo1->Lines->SaveToFile(tpathckw+"\\"+"gti_wits.txt");//записали в отладочный файл
}
//========================================================================
void __fastcall TFr45::TimerGtiTimer(TObject *Sender)
{
    Button10gtiClick(Sender);
}
//---------------------------------------------------------------------------
//перемещение панели
void __fastcall TFr45::PgtiMouseMove(TObject *Sender, TShiftState Shift, int X, int Y)
{
 const int SC_DRAGMOVE=61458;//0xF012;
 ReleaseCapture();
 ((TControl *)Sender)->Perform(WM_SYSCOMMAND,SC_DRAGMOVE,0);
}
//========================================================================
