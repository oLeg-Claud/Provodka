//====================================================================================
//Панель выбора параметров
void ShowPanelParam(int top, int left)
{
AnsiString s1;
int i, j, count_param;
  //Отформатируем Панель и расставим на ней все компоненты
  Fr45->PanelLasParam->Top=top;
  Fr45->PanelLasParam->Left=left;
  Fr45->PanelLasParam->Width=200;
  Fr45->PanelLasParam->Height=470;
  //кнопка Закрыть - правый верхний угол
  Fr45->CloseLas->Top=7;//по вертикали
  Fr45->CloseLas->Left=Fr45->PanelLasParam->Width-Fr45->CloseLas->Width-7;
  //Заголовок-шапка
  //Fr45->StaticText4->Left=Fr45->PanelLasParam->Width/2 - Fr45->StaticText4->Width/2;
  Fr45->StaticText4->Top=9;
  //кнопка Выбрать - внизу по центру
  Fr45->BitBtnLasGo->Top=Fr45->PanelLasParam->Height-Fr45->BitBtnLasGo->Height-7;
  Fr45->BitBtnLasGo->Left=Fr45->PanelLasParam->Width/2-Fr45->BitBtnLasGo->Width/2;
  //список параметров
  Fr45->LBLas->Left=7;
  Fr45->LBLas->Width=Fr45->PanelLasParam->Width-16;
  Fr45->LBLas->Top=Fr45->StaticText4->Top+Fr45->StaticText4->Height+3;
  Fr45->LBLas->Height=Fr45->BitBtnLasGo->Top-Fr45->LBLas->Top-7;

    //формируем чекбоксы в списке параметров
    Fr45->LBLas->Items->Clear();
    count_param = sizeof(Zag_global)/sizeof(*Zag_global);
    for(i=0;i<(count_param);i++)
    {	s1=Zag_global[i][0];//читаем параметр из массива
    	j=AnsiString(s1).Pos(".");//до точки - это название параметра
        if(j>0)
        {	Fr45->LBLas->Items->Add(s1.SubString(1,j-1));
        	Fr45->LBLas->Checked[Fr45->LBLas->Items->Count-1]=true;//пока всех включаем
        }
    }
    //читаем файл чекбоксов, если есть
    if(FileExists(CoreDir+"\\las.ini"))
    {	TIniFile *ini = new TIniFile(CoreDir+"\\las.ini");
        for(i=0; i<count_param;i++)
        {Fr45->LBLas->Checked[i]=ini->ReadBool("items", Fr45->LBLas->Items->Strings[i], false);
        }
    }

  Fr45->PanelLasParam->Visible = true;//показываем панель выбора параметров ЛАС
}
//====================================================================================
//Кнопка LAS в Настройках
void __fastcall TFr45::Panel10Click(TObject *Sender)
{
 //Настраиваем и показываем панель параметров ЛАС
 ShowPanelParam(200,600);
}
//====================================================================================
//Кнопка Выбрать параметры ЛАС на панели выбора
void __fastcall TFr45::BitBtnLasGoClick(TObject *Sender)
{
 CloseLasClick(Sender);//закрываем панель
 //сохраняем выбранные параметры в файл
 TIniFile *ini = new TIniFile(CoreDir+"\\las.ini");
 for(int i = 0; i < Fr45->LBLas->Items->Count; i++)
 {	ini->WriteBool("items", Fr45->LBLas->Items->Strings[i], Fr45->LBLas->Checked[i]);
 }
 
 write_to_file_las();
}
//====================================================================================
//Кнопка Закрыть панель выбора параметров LAS
void __fastcall TFr45::CloseLasClick(TObject *Sender)
{
 Fr45->PanelLasParam->Visible = false;
}
//====================================================================================
void write_to_file_las(void)
{
AnsiString s1,glub;
int i, j, n, count_string=0, count_param=0, count_string_out=0, begin_param=0, count_out_las=0;
float b, mkonec_zts, mkonec_gk, mkonec_ndm;
    //Найдем в MemoInc начальную глубину
    s1=Fr45->MemoInc->Lines->Strings[1];//Строка сразу после заголовка
    //вытащим первую глубину из строки, с 1-го символа до пробела
    glub=Trim(AnsiString(s1).SubString(1,AnsiString(s1).Pos(" ")-1));

    //загрузим в MemoLas заголовок LAS
    Fr45->MemoLas->Lines->Clear();
    //начало заголовка
    j = sizeof(LasZagolBegin)/sizeof(*LasZagolBegin);
    for(i=0;i<j;i++) {Fr45->MemoLas->Lines->Add(LasZagolBegin[i]);}
    s1 = Fr45->MemoLas->Lines->Strings[8] + Fr45->TablePasport->Cells[1][0];//Заказчик
    Fr45->MemoLas->Lines->Strings[8] = s1;
    s1 = Fr45->MemoLas->Lines->Strings[9] + Fr45->Ckw->Text;//Скважина
    Fr45->MemoLas->Lines->Strings[9] = s1;
    s1 = Fr45->MemoLas->Lines->Strings[10] + Fr45->TablePasport->Cells[1][1];//Месторождение
    Fr45->MemoLas->Lines->Strings[10] = s1;
    //теперь параметры
    j = sizeof(Zag_global)/sizeof(*Zag_global);
    for(i=0;i<j;i++) Fr45->MemoLas->Lines->Add(PadRight(Zag_global[i][0],37)+Zag_global[i][1]);
    //теперь конец заголовка
    j = sizeof(LasZagolEnd)/sizeof(*LasZagolEnd);
    for(i=0;i<j;i++) {Fr45->MemoLas->Lines->Add(LasZagolEnd[i]);}

    //мертвые концы
    mkonec_zts=MyStrToFloat(Fr45->Sn->Cells[1][13]);//мертвый конец ЗТС
    mkonec_gk=MyStrToFloat(Fr45->Sn->Cells[1][14]);//мертвый конец ГК ЗТС
    mkonec_ndm=MyStrToFloat(Fr45->Sn->Cells[1][15]);//мертвый конец ГК НДМ
    count_string = Fr45->MemoLas->Lines->Count;

    //будем искать строку версии программы по маске
    for(int i=0;i<(count_string-1);i++)
    {
     s1=Fr45->MemoLas->Lines->Strings[i];
     //вытащим маску из строки
     j=AnsiString(s1).Pos("Created by");
     if(j!=0)
     {s1="# Created by <Проводка "+ver+"> ("+Fr45->Cdat->Text+"); ";//версия Проводки и датчик
      s1+="MERTVIY KONEC: ZTS="+FloatToStrF0(mkonec_zts,7,2)+"m; ";
      s1+="GK_ZTS="+FloatToStrF0(mkonec_gk,7,2)+"m; ";
      s1+="NDM="+FloatToStrF0(mkonec_ndm,7,2)+"m; ";
      Fr45->MemoLas->Lines->Strings[i]=s1;
      break;
     }
    }
    //----------------------------------------------------------------------------------
    //Теперь выделим и заполним массивы чисел
    //Сначала подсчитаем кол-во параметров и начало параметров в заголовке
    for(int i=0;i<(count_string-1);i++)
    {
     s1=Fr45->MemoLas->Lines->Strings[i];
     //ищем маску в строке
     j=AnsiString(s1).Pos("~CURVE");
     if(j!=0) {begin_param=i+1;break;}//Номер строки в заголовке с первым параметром, конец цикла
    }
    //Кол-во параметров
    count_param=sizeof(Zag_global)/sizeof(*Zag_global);
    //кол-во строк исходных данных в MemoInc
	count_string=Fr45->MemoInc->Lines->Count-1;

    //создаем входной двумерный динамический массив
    //первый индекс-строка, второй-параметр
    //в нем будет лежать исходник
    double **mas=new double*[count_string];
    for(i=0;i<count_string;i++) mas[i]=new double[count_param];
    //также создадим массив мертвых концов для каждого параметра из исходника
    double *mkmas=new double[count_param];
	//----------------------------------------------------------------------------------
    //теперь заполним массив числами из MemoInc
    int pos;
    int count_del_param;//счетчик пропущенных параметров;
    __int64 time;//тут будем хранить время
    int indexROP=0;//индекс ROP скорости бурения в массиве параметров
    for(i=0;i<count_param;i++) {if(Zag_global[i][0].Pos("ROP.m/h")>0) {indexROP=i; break;}}
    if(indexROP==0)
    {   ShowMessage("Ошибка в определении скорости бурения!");
        if(count_string>0) for(i=0;i<count_string;i++) delete [] mas[i];
    	delete mkmas;
        return;
    }
    //по строкам (глубинам)
    for(j=0;j<count_string;j++)
    {
     s1=Fr45->MemoInc->Lines->Strings[j+1];
     count_del_param=0;
     //по параметрам на глубине (в строке), состав из заголовка LAS
     for(i=0;i<count_param;i++)
     {//удалим пробелы слева
      while(AnsiString(s1).Pos(" ")==1) s1.Delete(1,1);
      //узнаем, где заканчивается число
      pos=AnsiString(s1).Pos(" ")-1;
      if(Fr45->LBLas->Checked[i])//если этот параметр выбран
      {
       AnsiString s2 = AnsiString(s1).SubString(1,pos);
       //проверим, что это не дата
       if(s2.Pos("_") > 0) mas[j][i-count_del_param] = get_UTimeFromString(s2);//это время
       //запишем первое число из строки, если оно там есть
       else if(TryStrToFloat(s2, mas[j][i-count_del_param])==false)
            mas[j][i-count_del_param]=-9999;//если не число
       //запишем мертвый конец этого параметра
       if(AnsiString(Fr45->LBLas->Items->Strings[i]).Pos("GK_ZTS")!=0) mkmas[i-count_del_param]=mkonec_gk;//метвый конец ЗТС
       else if(AnsiString(Fr45->LBLas->Items->Strings[i]).Pos("ZTS")!=0) mkmas[i-count_del_param]=mkonec_zts;//метвый конец ГК ЗТС
       else if(AnsiString(Fr45->LBLas->Items->Strings[i]).Pos("NDM")!=0) mkmas[i-count_del_param]=mkonec_ndm;//метвый конец НДМ
       else mkmas[i-count_del_param]=0;//для всех остальных - чистый забой

       //на нужном индексе посчитаем скорость бурения ROP
       if(i==indexROP)
       {float sk=0;
        if(j==0) {time=mas[j][i-count_del_param]; sk=-9999;}//на первой строке скорости нету
        else
        {if(mas[j][i-count_del_param]==-9999) {time=-9999; sk=-9999;}
         else if(time==-9999) {time=mas[j][i-count_del_param]; sk=-9999;}
         else
         //собственно расчет скорости
         {__int64 time2 = mas[j][i-count_del_param];//время текущей строки
          //найдем дельту глубины, она в [0]
          float dglub = mas[j][0] - mas[j-1][0];
          if(dglub < 0) dglub = 0;
          //вычислим скорость = м/ч
          float dtime = (float)(time2 - time)/3600;//сек в часы
          //ShowMessage("count_string="+IntToStr(count_string)+"\r\nj="+IntToStr(j)+"\r\ntime2="+IntToStr(time2)+"\r\ntime="+IntToStr(time)+"\r\ndtime="+FloatToStr(dtime));
          if(dtime != 0) sk = dglub/dtime;
          else sk=0;//-9999;
          time=time2;//для следующей строки
         }
        }
        mas[j][i-count_del_param]=sk;//подменяем время скоростью
       }
      }
      else count_del_param++;
      //удалим число из строки
      s1.Delete(1,pos);
     }
    }
	count_param -= count_del_param;//теперь на count_del_param параметров меньше

    //Притянем глубины исходника к ближайшим кратным 0.2
    double begin,end,delta;
    j=1;
    while(j<count_string)
    {
     begin=mas[j-1][0];//начало участка
     end=mas[j][0];//конец участка
     delta=end-begin;//разница на участке
     if(delta<0)
     {ShowMessage("Обнаружено уменьшение глубины, файл не может быть создан ("+FloatToStr(begin)+"м - "+FloatToStr(end)+"м)");
      	if(count_string>0) for(i=0;i<count_string;i++) delete [] mas[i];
    	delete mkmas;
      	return;
     }
     i=floor(delta/0.2+0.5);//округленно число вхождений
     end=begin+0.2*i;
     mas[j][0]=roundf(end, 2);
     j++;
    }

    //----------------------------------------------------------------------------------
    // теперь надо собрать интерполированный массив для выхода
    //----------------------------------------------------------------------------------
    //подсчитаем необходимое кол-во строк выходного массива с шагом 0.2 по разнице глубин, в большую сторону
    count_string_out=floor((mas[count_string-1][0]-mas[0][0])/0.2+0.5)+1;
    //теперь создаем выходной двумерный динамический массив
    //первый индекс-строка, второй-параметр
    double **out=new double*[count_string_out];
    for(i=0;i<count_string_out;i++) out[i]=new double[count_param];
    int *del=new int[count_param];
    int first;

    //нулевую строку кладем без изменений
    for(i=0;i<count_param;i++) out[0][i]=mas[0][i];
    int count_out=0;//счетчик выходных строк
    j=1;
    while(j<count_string)//по строкам исходника
    {
     begin=mas[j-1][0];//начало участка
     end=mas[j][0];//конец участка
     delta=end-begin;//разница глубин
     if(delta<0)
     {ShowMessage("Обнаружено уменьшение глубины, файл не может быть создан ("+FloatToStr(begin)+"м - "+FloatToStr(end)+"м)");
      	if(count_string>0) for(i=0;i<count_string;i++) delete [] mas[i];
        if(count_string_out>0) for(i=0;i<count_string_out;i++) delete [] out[i];
    	delete mkmas;
        delete del;
      	return;
     }
     int a=floor(delta/0.2+0.5);//округленно число вхождений
     if(a>0)//если глубины не одинаковы
     {
      for(n=0;n<a;n++)//то вставляем строки с интерполяцией
      {
       count_out++;
       out[count_out][0]=roundf(out[0][0]+count_out*0.2,2);//глубина
       for(i=1;i<count_param;i++) {out[count_out][i]=interpol(mas[j-1][i],mas[j][i],0.2*(n+1)/delta); del[i]=0;}
       first=0;
      }
     }
     else//одинаковые глубины
     {
      out[count_out][0]=roundf(out[0][0]+count_out*0.2,2);//глубина не меняется, в ту же строку
      first++;
      //при первом входе усредняем входные величины
      if(first==1) for(i=1;i<count_param;i++) {del[i]++; out[count_out][i]=averaging(mas[j-1][i],mas[j][i],&del[i]);}
      //при сл. входах усредняем с предыдущим результатом
      else for(i=1;i<count_param;i++) {del[i]++; out[count_out][i]=averaging(out[count_out][i],mas[j][i],&del[i]);}
     }
     j++;
    }

    //----------------------------------------------------------------------------------
    // теперь соберем новый выходный массив с приведением глубин к мертвым концам
    //----------------------------------------------------------------------------------
    //создадим массив смещений индексов для текущего набора параметров
    int *sm_index=new int[count_param];
    //расчитаем индексы смещений и заполним массив
    j=-1000; b=-1000;
    for(i=0;i<count_param;i++) {sm_index[i] = floor(mkmas[i]/0.2+0.5); if(mkmas[i]>b) {b=mkmas[i]; j=i;}}
    //теперь в j сидит индекс параметра с максимальным мертвым концом
    int max_sm_index = sm_index[j];
    //найдем минимальный мертвый конец
    b=1000;
    for(i=0;i<count_param;i++) {if(mkmas[i]<b) {b=mkmas[i]; j=i;}}
    int min_sm_index = sm_index[j];
    count_out_las = count_out + max_sm_index + 1;//увеличим строки на нужную величину индексов
    //теперь создаем двумерный динамический массив
    //первый индекс=глубины, второй=параметры
    float **out_las=new float*[count_out_las];
    for(i=0;i<count_out_las;i++) out_las[i]=new float[count_param];
    //заполним массив начальными значениями -9999
    for(j=0;j<count_out_las;j++) {for(i=0;i<count_param;i++) out_las[j][i] = -9999;}
    float new_begin_glub = out[0][0] - (float)max_sm_index*0.2;//делаем новую начальную глубину
    //теперь соберем новый выходной массив со смещениями глубин
    for(j=0;j<count_out_las;j++)
    {if(j < max_sm_index) out_las[j][0] = new_begin_glub + (float)j*0.2;//в начале - новые глубины
     else
     {out_las[j][0] = out[j-max_sm_index][0];//потом - старые глубины
      for(i=1;i<count_param;i++) out_las[j-sm_index[i]][i] = out[j-max_sm_index][i];
     }
    }
    count_out_las -= min_sm_index;//уберем снизу лишнее
    //----------------------------------------------------------------------------------
    //теперь определим границы выводимого участка
    //----------------------------------------------------------------------------------
    //глубина сидит в первом параметре = out[x][0]
    int begin_out = 0;//начальный индекс массива на выход
    int end_out = count_out_las-1;//конечный индекс массива на выход
    float glub_begin, glub_end;
    if(count_out_las < 2)//с одной строкой ничего не сделаешь
    {SaveStatusLog("Ошибка в количестве строк, LAS-файл не создан!", clRed);
     goto exit;
    }
    Fr45->EBegin->Text = FloatToStrF0(out_las[begin_out][0],7,2);//начало интервала
    Fr45->EEnd->Text = FloatToStrF0(out_las[end_out][0],7,2);//конец интервала
    FlagInterval=0;
    ShowPanelInterval(200,600);//покажем панель интервала
    while(FlagInterval==0) {Application->ProcessMessages();}//ждем завершения выбора интервала
    if(FlagInterval==2) goto exit;//выходим ничего не делая
    try{glub_begin = MyStrToFloat(Fr45->EBegin->Text);}catch(...){glub_begin = MyStrToFloat(out_las[begin_out][0]);}
    try{glub_end = MyStrToFloat(Fr45->EEnd->Text);}catch(...){glub_end = MyStrToFloat(out_las[end_out][0]);}
    //найдем индексы новых границ интервала в массиве
    for(i=0;i<count_out_las+1;i++)
      if((out_las[i][0]-glub_begin) > -0.0001) {begin_out = i; break;}
    for(i=0;i<count_out_las+1;i++)
      if((out_las[i][0]-glub_end) > -0.0001) {end_out = i; break;}
    if(begin_out>=end_out)
    {SaveStatusLog("Ошибка в границах интервала, LAS-файл не создан!", clRed);
     goto exit;
    }

    //изменим глубины в заголовке
    //будем искать строки старт-стоп по маске
    for(i=0;i<(count_string-1);i++)
    {
     s1=Fr45->MemoLas->Lines->Strings[i];
     //вытащим маску из строки
     j=AnsiString(s1).Pos("STRT.M");
     if(j!=0)
     {
      Fr45->MemoLas->Lines->Strings[i]="STRT.M"+PadLeft(FloatToStrF0(out_las[begin_out][0],5,2),31)+": START DEPTH";//Начальная глубина
      Fr45->MemoLas->Lines->Strings[i+1]="STOP.M"+PadLeft(FloatToStrF0(out_las[end_out][0],7,2),31)+": STOP DEPTH";//Последняя глубина
      Fr45->MemoLas->Lines->Strings[i+2]="STEP.M"+PadLeft("0.2",31)+": STEP";
      break;
     }
    } //ShowMessage(FloatToStrF0(out_las[end_out][0],5,2));
    //----------------------------------------------------------------------------------
    //теперь надо сохранить в выходной файл
    //----------------------------------------------------------------------------------
    if(count_string_out>0)
    {int flag_err=0;
     for(j=begin_out;j<end_out+1;j++)//между заданными границами интервала
     {
      s1="";
      for(i=0;i<count_param;i++)//создали строку
      try{s1=s1+PadRight(FloatToStrF0(out_las[j][i],DlinaParam-2,3),DlinaParam);}
      catch(...){flag_err=1;}
      Fr45->MemoLas->Lines->Add(s1);//добавили в memo
     }
    //удалим невыбранные параметры из заголовка
     int count_del=0;
     for(i=0;i<Fr45->LBLas->Items->Count;i++)
     {if(!Fr45->LBLas->Checked[i]) {Fr45->MemoLas->Lines->Delete(begin_param+i-count_del); count_del++;}
     }
     if(flag_err==0)
     {
        Fr45->MemoLas->Lines->SaveToFile(Flas);//записали в файл
        Fr45->StatusBar->SimpleText=" LAS-файл успешно создан! ("
        +FloatToStrF0(out_las[begin_out][0],5,2)+" - "+FloatToStrF0(out_las[end_out][0],5,2)
        +")";
     }
     else SaveStatusLog("LAS-файл содержит ошибки :(", clRed);
    }
    else SaveStatusLog("Не могу записать пустой LAS-файл!", clRed);
exit:
    //удалим динамическиe массивы
    if(count_string>0) for(i=0;i<count_string;i++) delete [] mas[i];
    if(count_string_out>0) for(i=0;i<count_string_out;i++) delete [] out[i];
    if(count_out_las>0) for(i=0;i<count_out_las;i++) delete [] out_las[i];
    delete del;
    delete mkmas;
    delete sm_index;
}
//====================================================================================
//Запретим снятие галочки с глубины
void __fastcall TFr45::LBLasClick(TObject *Sender)
{
    for(int i=0;i<Fr45->LBLas->Items->Count;i++)
    {if(Fr45->LBLas->Items->Strings[i]=="DEPTH")//ищем индекс глубины
     {if(Fr45->LBLas->Checked[i]!=true) Fr45->LBLas->Checked[i]=true;
      break;
     }
    }
}
//====================================================================================
//Показать панель Интервала
void ShowPanelInterval(int top, int left)
{
AnsiString ss;
  //Отформатируем Панель и расставим на ней все компоненты
  Fr45->PanelInterval->Top=top;
  Fr45->PanelInterval->Left=left;
  Fr45->PanelInterval->Width=200;
  Fr45->PanelInterval->Height=130;
  //кнопка Закрыть - правый верхний угол
  Fr45->CloseInterval->Top=7;//по вертикали
  Fr45->CloseInterval->Left=Fr45->PanelInterval->Width-Fr45->CloseInterval->Width-7;
  //Заголовок-шапка
  Fr45->StaticText2->Left=Fr45->PanelInterval->Width/2 - Fr45->StaticText2->Width/2;
  Fr45->StaticText2->Top=7;
  //кнопка Выбрать - внизу по центру
  Fr45->BitBtnIntervalSave->Top=Fr45->PanelInterval->Height-Fr45->BitBtnIntervalSave->Height-7;//по вертикали внизу слева
  Fr45->BitBtnIntervalSave->Left=Fr45->PanelInterval->Width/2-Fr45->BitBtnIntervalSave->Width/2;
  //Начало интервала
  Fr45->LBegin->Top=Fr45->StaticText2->Top+Fr45->StaticText2->Height+10;
  Fr45->LBegin->Left=9;
  Fr45->EBegin->Top=Fr45->LBegin->Top - 2;
  Fr45->EBegin->Left = Fr45->LBegin->Left + Fr45->LBegin->Width + 5;
  Fr45->EBegin->Width=80;
  //Конец интервала
  Fr45->LEnd->Top=Fr45->LBegin->Top+Fr45->LBegin->Height+15;
  Fr45->LEnd->Left=Fr45->LBegin->Left;
  Fr45->EEnd->Top=Fr45->LEnd->Top - 2;
  Fr45->EEnd->Left=Fr45->EBegin->Left;
  Fr45->EEnd->Width=80;

  Fr45->PanelInterval->Visible=true;//покажем панель
}
//====================================================================================
void __fastcall TFr45::BitBtnIntervalSaveClick(TObject *Sender)
{//
int now;
  if( (MyStrToFloat(EEnd->Text)-MyStrToFloat(EBegin->Text))<=0 || EEnd->Text=="" || EBegin->Text=="" )
  {	for(int i=0;i<5;i++)
    {PanelInterval->Left=PanelInterval->Left-10;
     now=clock()+100;
     while(now > clock()) Application->ProcessMessages();
     PanelInterval->Left=PanelInterval->Left+10;
     now=clock()+100;
     while(now > clock()) Application->ProcessMessages();
    }
  }
  else
  { Fr45->PanelInterval->Visible=false;
  	FlagInterval=1;
  }
}
//====================================================================================
void __fastcall TFr45::CloseIntervalClick(TObject *Sender)
{
	Fr45->PanelInterval->Visible=false;
    FlagInterval=2;
}
//====================================================================================
void __fastcall TFr45::PanelLasParamMouseMove(TObject *Sender, TShiftState Shift, int X, int Y)
{
 const int SC_DRAGMOVE=61458;//0xF012;
 ReleaseCapture();
 ((TControl *)Sender)->Perform(WM_SYSCOMMAND,SC_DRAGMOVE,0);
}
//====================================================================================
 