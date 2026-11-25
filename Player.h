//---------------------------------------------------------------------------
void __fastcall TFr45::PlayButtonClick(TObject *Sender)
{
	//кнопка нажата
	if(PlayButton->Down==true)
    {//если Старт уже запущен, то play не должен срабатывать
     if(start->Down==true) {PlayButton->Down = false; return;}
     //выберем файл для чтения
     OpenDialog2->InitialDir=tpathckw+"\\smp";
     OpenDialog2->Filter="файлы сигналов|*.smp";
     OpenDialog2->FileName="";
     if(OpenDialog2->Execute())//открываем диалог
     {AnsiString str = OpenDialog2->FileName;//путь к файлу
      FileSignal = fopen(str.c_str(),"rb");//открываем для чтения
      if(FileSignal==NULL)
      {ShowMessage("Не могу открыть файл "+OpenDialog2->FileName);
       PlayButton->Down = false; return;
      }
      //читаем заголовок
      char n[2048];
      if(fread(&n, 1,2048,FileSignal)!=2048)
      {ShowMessage("Не могу прочитать заголовок сигнала");
       PlayButton->Down = false; return;
      }
     }
     else {PlayButton->Down = false; return;}

     RecButton->Enabled=false;//запрещаем кнопку записи
     if(StatusPause) {PauseButton->Down = false; PauseButtonClick(Sender);}//отэимаем паузу
     StatusPlayer=true;
     start->Down = true; startClick(Sender);//запускаем Старт
    }
    //кнопка отжата
    else
    {RecButton->Enabled=true;//разрешаем кнопку записи
	 if(start->Down==true) {stop->Down = true; stopClick(Sender);}//нажимаем Стоп, если идет чтение
     StatusPlayer=false;
     if(FileSignal!=NULL) {fclose(FileSignal); FileSignal=NULL;}
     if(FileSignal2!=NULL) {fclose(FileSignal2); FileSignal2=NULL;}
     if(StatusPause) {PauseButton->Down = false; PauseButtonClick(Sender);}//отожмем паузу
    }
}
//---------------------------------------------------------------------------
void __fastcall TFr45::RecButtonClick(TObject *Sender)
{
	//кнопка нажата
	if(RecButton->Down==true)
    {TimerPlayer->Enabled=true;
     RecButton->Tag=1;
     if(StatusPause) {PauseButton->Down = false; PauseButtonClick(Sender);}//отэимаем паузу
     PlayButton->Enabled=false;//запрещаем кнопку воспроизведения
     PlayerSpeedButton->Enabled=false;//запрещаем кнопку скорости
     PauseButton->Enabled=false;//запрещаем кнопку паузы
     StatusRecorder=true;
     FileSignal=NULL;//при старте записи очищаем
     FileSignal2=NULL;//при старте записи очищаем
    }
    //кнопка отжата
    else
    {TimerPlayer->Enabled=false;
     RecButton->Glyph->Width=0;
     ImageList1->GetBitmap(0,RecButton->Glyph);
     PlayButton->Enabled=true;//разрешаем кнопку воспроизведения
     PlayerSpeedButton->Enabled=true;//разрешаем кнопку скорости
     PauseButton->Enabled=true;//разрешаем кнопку паузы
     StatusRecorder=false;
     if(FileSignal!=NULL) {fclose(FileSignal); FileSignal=NULL;}
     if(FileSignal2!=NULL) {fclose(FileSignal2); FileSignal2=NULL;}
    }
}
//---------------------------------------------------------------------------
//таймер мерцания записи
void __fastcall TFr45::TimerPlayerTimer(TObject *Sender)
{
    if(RecButton->Tag==0)
    {RecButton->Glyph->Width=0;
     ImageList1->GetBitmap(0,RecButton->Glyph);
     RecButton->Tag=1;
    }
    else
    {RecButton->Glyph->Width=0;
     ImageList1->GetBitmap(1,RecButton->Glyph);
     RecButton->Tag=0;
    }
}
//---------------------------------------------------------------------------
void __fastcall TFr45::PlayerSpeedButtonClick(TObject *Sender)
{
	if(PlayerSpeedButton->Tag==1)
    {PlayerSpeedButton->Tag=2;
    }
    else if(PlayerSpeedButton->Tag==2)
    {PlayerSpeedButton->Tag=4;
    }
    else if(PlayerSpeedButton->Tag==4)
    {PlayerSpeedButton->Tag=8;
    }
    else if(PlayerSpeedButton->Tag==8)
    {PlayerSpeedButton->Tag=16;
    }
    else if(PlayerSpeedButton->Tag==16)
    {PlayerSpeedButton->Tag=32;
    }
    else if(PlayerSpeedButton->Tag==32)
    {PlayerSpeedButton->Tag=1;
    }
    PlayerSpeedButton->Caption = "x"+IntToStr(GetPlayerSpeed());
}
//---------------------------------------------------------------------------
int GetPlayerSpeed(void)  {return Fr45->PlayerSpeedButton->Tag;}
//---------------------------------------------------------------------------
void __fastcall TFr45::PauseButtonClick(TObject *Sender)
{
    if(!StatusPlayer) {PauseButton->Down = false; return;}//игнорируем, если не плей
    //кнопка нажата
	if(PauseButton->Down==true)
    {
        StatusPause=true;
        //тормозим поток
        if(fl_Th && StatusPlayer) Thread->Suspend();
    }
    //кнопка отжата
    else
    {
        StatusPause=false;
        //возобновляем поток
        if(fl_Th && StatusPlayer) Thread->Resume();
    }
}
//---------------------------------------------------------------------------
 