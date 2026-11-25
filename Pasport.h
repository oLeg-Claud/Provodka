//====================================================================================
//====================================================================================
//====================================================================================
//====================================================================================
//обработка правильных цифр в таблице
void __fastcall TFr45::TablePasportKeyPress(TObject *Sender, char &Key)
{
 //дробные положительные числа
 if(TablePasport->Row==3||TablePasport->Row==4||TablePasport->Row==5||TablePasport->Row==6||TablePasport->Row==7||TablePasport->Row==8)
 {
  char s[]={'0','1','2','3','4','5','6','7','8','9','.'};
  int ikey=0;
  for (int i=0;i<sizeof(s);i++) if(Key==s[i]){ikey=1; break;}
  if(ikey==1 && Key=='.')//считаем, сколько точек уже было в строке
  {String str=TablePasport->Cells[1][TablePasport->Row];
   int i=0, b=0;
   while(i++ < str.Length()) {if(str.IsDelimiter('.', i)) b++;}
   if(b>0) ikey=0;//уже были точки
  }
  if(ikey==0){Key=NULL; PlaySound("MYS2",0,SND_RESOURCE);}
 }

 //целые числа
 /*else if(TablePasport->Row==2||TablePasport->Row==3||TablePasport->Row==8)
 {
  char s[]={'0','1','2','3','4','5','6','7','8','9' };
  int ikey=0;
  for (int i=0;i<sizeof(s);i++) if(Key==s[i])ikey=1;
  if(ikey==0){Key=NULL; PlaySound("MYS2",0,SND_RESOURCE); }
 }*/
 /*//для знаковых float
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
 }*/
 //else {Key=NULL; PlaySound("MYS2",0,SND_RESOURCE); }
}
//====================================================================================
// Выбрать папку скважины
void __fastcall TFr45::DirBtnClick(TObject *Sender)
{
//SetCurrentDir(tpathckw);
OpenDialog2->InitialDir=tpathckw+"\\";
OpenDialog2->Filter="имя скважины|*.bkp";
OpenDialog2->FileName="";
//открываем диалог
if(OpenDialog2->Execute())
{
	tpathckw=ExtractFileDir(OpenDialog2->FileName);//новый путь к файлам скважины
    SetCurrentDir(tpathckw);//
	ckwName=ExtractFileName(OpenDialog2->FileName);//новое имя скважины
	if(CompareStr(ExtractFileExt(OpenDialog2->FileName),"")!=0)//убираем расширение для имени
    {ckwName=ckwName.SetLength(ckwName.Length()-ExtractFileExt(OpenDialog2->FileName).Length());
    }
 	Ckw->Text=ckwName;//обновляем имя скважины в окошке
 	CkwChange(Sender);//выполняем скрипт изменения имени в файлах скважины
    //собираем список скважин  для выпадающего меню
	Ckw->Items->Clear();
	TSearchRec sr;
    AnsiString ex;
	if(FindFirst("*.*",faAnyFile,sr)==0)
    {
		ex= ExtractFileExt(sr.Name);
		if(sr.Attr!=faDirectory && sr.Name!="." && sr.Name!="..")
		if(CompareText(ex,".ink")==0) Ckw->Items->Add(Trim(sr.Name));
		while(FindNext(sr)==0)
        {
    		ex= ExtractFileExt(sr.Name);
    		if(sr.Attr!=faDirectory && sr.Name!="." && sr.Name!="..") if(CompareText(ex,".ink")==0){Ckw->Items->Add(ChangeFileExt(sr.Name,""));}
        }
    }
 	FindClose(sr);
}
}
//====================================================================================
//Кнопка ID скважины
void __fastcall TFr45::PasportBtnClick(TObject *Sender)
{
    //настроим таблицу
    Fr45->TablePasport->ColCount = 2;//кол-во колонок
    Fr45->TablePasport->RowCount = 9;//кол-во параметров
    Fr45->TablePasport->DefaultRowHeight = 20;//высота строки
    Fr45->TablePasport->ColWidths[0] = 130;//ширина 1-й колонки
    Fr45->TablePasport->ColWidths[1] = 250;//ширина 2-й колонки
    int tmp=0; for(int i=0;i<Fr45->TablePasport->ColCount;i++) tmp += Fr45->TablePasport->ColWidths[i];
    Fr45->TablePasport->Width = tmp+7;//ширина таблицы
    Fr45->TablePasport->Height = (Fr45->TablePasport->DefaultRowHeight + Fr45->TablePasport->GridLineWidth) * Fr45->TablePasport->RowCount +5;//высота таблицы
    //расположим кнопку выбора папки
    Fr45->DirBtn->Top = Fr45->TablePasport->Top + Fr45->TablePasport->Height + 5;
    Fr45->DirBtn->Left = 10;
    Fr45->DirBtn->Width = Fr45->TablePasport->ColWidths[0];
    //настроим размеры панели
    Fr45->PanelPasport->Width = 5 + Fr45->TablePasport->Width +5;
    Fr45->PanelPasport->Height = Fr45->DirBtn->Top + Fr45->DirBtn->Height + 10;
    //расположим номер скважины
    Ckw->Top = Fr45->DirBtn->Top;
    Ckw->Width = 100;
    Ckw->Left = Fr45->PanelPasport->Width - Ckw->Width - 7;
    Label2->Top = Ckw->Top + 5;
    Label2->Left = Ckw->Left - Label2->Width - 7;
    //кнопка Закрыть - правый верхний угол
    Fr45->ClosePasport->Top=7;//по вертикали
    Fr45->ClosePasport->Left=Fr45->PanelPasport->Width-Fr45->ClosePasport->Width-7;
    //заполним названия
    Fr45->TablePasport->Cells[0][0] = "Заказчик";
    Fr45->TablePasport->Cells[0][1] = "Месторождение";
    Fr45->TablePasport->Cells[0][2] = "Куст";
    Fr45->TablePasport->Cells[0][3] = "Секция";
    Fr45->TablePasport->Cells[0][4] = "С.Ш.";
    Fr45->TablePasport->Cells[0][5] = "В.Д.";
    Fr45->TablePasport->Cells[0][6] = "Альтитуда ротора";
    Fr45->TablePasport->Cells[0][7] = "Gt на скважине, м/c^2";
    Fr45->TablePasport->Cells[0][8] = "Номер рейса";
    //заполним значения из файла
    LoadPasportFile(Fpasport);

    Fr45->PanelPasport->Visible = true;//показать панель паспорта скважины
}
//====================================================================================
//Кнопка Закрыть панель паспорта скважины
void __fastcall TFr45::ClosePasportClick(TObject *Sender)
{
    Fr45->PanelPasport->Visible = false;
    SavePasportFile(Fpasport);//записываем параметры в файл
    CkwChange(Sender);//обновляем файлы скважины
}
//====================================================================================
void __fastcall TFr45::PanelPasportMouseMove(TObject *Sender, TShiftState Shift, int X, int Y)
{
 const int SC_DRAGMOVE=61458;//0xF012;
 ReleaseCapture();
 ((TControl *)Sender)->Perform(WM_SYSCOMMAND,SC_DRAGMOVE,0);
}
//====================================================================================
//запись паспорта скважины в файл
void SavePasportFile(AnsiString File)
{
    TStringList *list = new TStringList();//создаем список строк
    for(int i=0;i<Fr45->TablePasport->RowCount;i++)
    {list->Add(Fr45->TablePasport->Cells[1][i]);
    }
    list->Add(Fr45->Cname->Text+" № "+Fr45->Cdat->Text);//Название ЗТС и номер прибора
    list->Add(Fr45->Ckw->Text);//Номер скважины
    list->SaveToFile(File);
    delete list;//удаляем временный список
}
//========================================================================
//чтение паспорта скважины из файла
void LoadPasportFile(AnsiString File)
{
    FILE *F;
    if((F=fopen(File.c_str(),"a"))==NULL){ShowMessage("не удается открыть"+File);return;}
    fclose(F);
    F=NULL;

    TStringList *list = new TStringList();//создаем список строк
    list->LoadFromFile(File);//читаем файл
    if(list->Count >= Fr45->TablePasport->RowCount)
    {   //значения из файла
        for(int i=0;i<Fr45->TablePasport->RowCount;i++)
        {   Fr45->TablePasport->Cells[1][i] = list->Strings[i];
        }
    }
    else
    {   //заполним значения по-умолчанию
        Fr45->TablePasport->Cells[1][0] = "Лучший в мире";
        Fr45->TablePasport->Cells[1][1] = "Любимое";
        Fr45->TablePasport->Cells[1][2] = "1";
        Fr45->TablePasport->Cells[1][3] = "1";
        Fr45->TablePasport->Cells[1][4] = "53.0";
        Fr45->TablePasport->Cells[1][5] = "54.0";
        Fr45->TablePasport->Cells[1][6] = "0";
        Fr45->TablePasport->Cells[1][7] = "1.0";
        Fr45->TablePasport->Cells[1][8] = "1";
        SavePasportFile(File);
    }
    delete list;//удаляем временный список
}
//========================================================================
 