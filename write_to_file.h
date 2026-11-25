//==========Запись в файл скважины inc=======================================
void __fastcall TFr45::Panel6Click(TObject *Sender)//Кнопка ОК глубины
{
    //если глубина пустая - выходим
    if(CompareText(Fr45->Eglubina->Text,"")==0)
    {	//ShowMessage("Введите глубину");
    	return;
    }
    //делаем 2 знака после запятой
    glubina=MyStrToFloat(Fr45->Eglubina->Text);
    Eglubina->Text=FloatToStrF0(glubina,7,2);
    glubina=MyStrToFloat(Fr45->Eglubina->Text);

    if(glubina<=glubina_last)//Если текущая глубина меньше или равна предыдущей
 	{   //выводим последнюю глубину красным
  		Fr45->Eglubina->Font->Color=clRed;
        Fr45->Eglubina->Text=FloatToStrF0(glubina_last,7,2);
        //Если глубина Ручная, то выводим предложение исправить файл ink
        //если глубина Внешняя, то просто выходим
        if(RadioGroup1->ItemIndex == 1)
        {	char *dd="Текущая глубина меньше или равна последней записанной!\r\nХотите исправить файл *.ink?";
  			if(Application->MessageBox(dd, "Ошибка глубины", MB_YESNO)== IDYES)
  			{PanelINKClick(Sender);
            }
  			else
            {Fr45->Eglubina->Font->Color=clBlack;
            }
        }
  		return;
 	}
    //если все в порядке, то пишем новую глубину в файл *.ink
 	Fr45->Eglubina->Font->Color=clBlack;
    write_to_file_inc();
}
//====================================================================================
void write_to_file_inc(void)
{
float zens; int azs;
AnsiString s1,ss;

  glubina_last=MyStrToFloat(Fr45->Eglubina->Text);
  glubina_last=roundf(glubina_last,2);
  ss="";

  //Кол-во колонок должно совпадать с составом заголовка

  //Собираем строку текущих параметров
  //Глубина
  s1=Fr45->Eglubina->Text;
  ss+=PadRight(s1,DlinaParam);
  //Зенит
  if(zenit<0) s1=IntToStr(-9999);
  else s1=FloatToStrF0(zenit,6,2);
  ss+=PadRight(s1,DlinaParam);
  //АЗ сырой ЗТС
  s1=FloatToStrF0(azimut,6,1);
  ss+=PadRight(s1,DlinaParam);
  //Азимут корр.
  if(Azk==-9999) s1="-9999";
  else s1=FloatToStrF0(Azk,6,1);
  ss+=PadRight(s1,DlinaParam);
  //Азимут истинный
  s1=AZist_str;
  ss+=PadRight(s1,DlinaParam);
  //Модуль H
  s1=IntToStr(Htotal);
  ss+=PadRight(s1,DlinaParam);
  //ОТКграв
  s1=IntToStr(otktek);//изменили с версии 062
  ss+=PadRight(s1,DlinaParam);
  //ОТКмагн
  s1=FloatToStrF0(az_otk,4,0);
  ss+=PadRight(s1,DlinaParam);
  //Обороты
  s1=IntToStr(obor);
  ss+=PadRight(s1,DlinaParam);
  //Температура
  s1=IntToStr(tem);
  ss+=PadRight(s1,DlinaParam);
  //Напряжение ЗТС
  if(NAPR==-9999) s1=IntToStr(-9999);
  else s1=FloatToStrF0(NAPR,6,1);
  ss+=PadRight(s1,DlinaParam);
  //Вибрация
  s1=IntToStr(vib);
  ss+=PadRight(s1,DlinaParam);
  //ГК зтс
  if(gkzts==-9999) s1=IntToStr(-9999);
  else s1=FloatToStrF0(gkzts,5,1);
  ss+=PadRight(s1,DlinaParam);

  //Закончились параметры от ЗТС, далее параметры от НДМ
  //Зенит НДМ
  if(ZUndm==-9999) s1=IntToStr(-9999);
  else s1=FloatToStrF0(ZUndm,6,1);
  ss+=PadRight(s1,DlinaParam);
  //ГК верх
  if(GKup==-9999) s1=IntToStr(-9999);
  else s1=FloatToStrF0(GKup,5,1);
  ss+=PadRight(s1,DlinaParam);
  //ГК низ
  if(GKdn==-9999) s1=IntToStr(-9999);
  else s1=FloatToStrF0(GKdn,5,1);
  ss+=PadRight(s1,DlinaParam);
  //Температура
  if(TEMPndm==-9999) s1=IntToStr(-9999);
  else s1=FloatToStrF0(TEMPndm,5,1);
  ss+=PadRight(s1,DlinaParam);
  //Обороты долота
  s1=IntToStr((int)OBORndm);
  ss+=PadRight(s1,DlinaParam);
  //Нагрузка
  if(NAGRndm==-9999) s1=IntToStr(-9999);
  else s1=FloatToStrF0(NAGRndm,5,1);
  ss+=PadRight(s1,DlinaParam);
  //ТОК ндм
  s1=IntToStr(TOKndm);
  ss+=PadRight(s1,DlinaParam);
  //Амплитуда peak
  if(PEAKndm==-9999) s1=IntToStr(-9999);
  else s1=FloatToStrF0(PEAKndm,6,2);
  ss+=PadRight(s1,DlinaParam);
  //КС, расчетный
  if(KCndm==-9999) s1=IntToStr(-9999);
  else s1=FloatToStrF0(KCndm,4,3);
  ss+=PadRight(s1,DlinaParam);
  //Индикатор
  s1=IntToStr((int)indic);
  ss+=PadRight(s1,DlinaParam);
  //dU
  s1=FloatToStrF0(dUndm,6,3);
  ss+=PadRight(s1,DlinaParam);
  //Момент, кН
  if(MOMndm==-9999) s1=IntToStr(-9999);
  else s1=FloatToStrF0(MOMndm,6,1);
  ss+=PadRight(s1,DlinaParam);
  //MPa
  if(DAVLndm==-9999) s1=IntToStr(-9999);
  else s1=FloatToStrF0(DAVLndm,6,1);
  ss+=PadRight(s1,DlinaParam);
  //А*ч
  if(MAHndm==-9999) s1=IntToStr(-9999);
  else s1=FloatToStrF0(MAHndm,3,1);
  ss+=PadRight(s1,DlinaParam);
  //Сигнал ККС, мВ
  if(sigKKS==-9999) s1=IntToStr(-9999);
  else s1=FloatToStrF0(sigKKS,4,1);
  ss+=PadRight(s1,DlinaParam);
  // Дата_Время
  s1=get_StringFromUTime(time(NULL));//текущее время
  ss+=PadRight(s1,DlinaParam*2);
  // Gtotal
  if(modg==-9999) s1=IntToStr(-9999);
  else s1=FloatToStrF0(modg*1000,6,1);// в mg
  ss+=PadRight(s1,DlinaParam);

  ss=StringReplace(ss,",",".",TReplaceFlags()<<rfIgnoreCase);
  
  LoadToMemoInkFromFile(Fink);//загружаем в мемо из файла
  Fr45->MemoInc->Lines->Add(ss);
  Fr45->MemoInc->Lines->SaveToFile(Fink);//записали в файл INK
  Fr45->StatusBar->Color=clWhite;
  Fr45->StatusBar->SimpleText="Для губины "+Fr45->Eglubina->Text+" запись в файл "+Fink+" проведена";
  Fr45->Gll->Caption="забой "+FloatToStrF0(glubina_last,6,2);
}
//====================================================================================
//это файл Черного ящика
void write_tabl_dat(AnsiString key, AnsiString val)
{
AnsiString ss="";
	//Собираем строку текущих параметров
    if(key != "String") val = StringReplace(val," ","_",TReplaceFlags()<<rfIgnoreCase);//заменяем пробелы, если есть
  	AnsiString mdate=get_DateFromUTime(time(NULL));// Дата
  	AnsiString mtime=get_TimeFromUTime(time(NULL));// Время
  	AnsiString glub=Fr45->Eglubina->Text;//Глубина
    if(key != "String") ss += PadRight(key,DlinaParam*2)+PadRight(mdate,DlinaParam)+PadRight(mtime,DlinaParam)+PadRight(glub,DlinaParam)+PadRight(val,DlinaParam);
    else ss += PadRight(key,DlinaParam*2)+PadRight(mdate,DlinaParam)+PadRight(mtime,DlinaParam)+PadRight(glub,DlinaParam)+val;

    FILE *outt;
  	outt=fopen(Fdat.c_str(),"a+b");
  	fprintf(outt,"%s",ss+"\r\n");
  	fclose(outt);
  	outt=NULL;
}
//====================================================================================
//Добавляем пробелы слева до нужной длины строки
AnsiString PadLeft(AnsiString input, unsigned int len)
{
AnsiString temp;
int delta, i;
    delta=len-StrLen(input.c_str());//нужное кол-во пробелов
    if(delta<=0)
    {
     input.Delete(len,abs(delta)+1);//обрежем строку справа
     delta=len-StrLen(input.c_str());//нужное кол-во пробелов
    }
    for(i=0;i<delta;i++) temp=temp+" ";//добавляем пробелы в начало
    temp=temp+input;//соединяем строки
    return temp;
}
//====================================================================================
//Добавляем пробелы справа до нужной длины строки
AnsiString PadRight(AnsiString input, unsigned int len)
{
AnsiString temp;
int delta, i;
    delta=len-StrLen(input.c_str());//нужное кол-во пробелов
    if(delta<=0)
    {
     input.Delete(len,abs(delta)+1);//обрежем строку справа
     delta=len-StrLen(input.c_str());//нужное кол-во пробелов
    }
    temp=input;
    for(i=0;i<delta;i++) temp=temp+" ";//добавляем пробелы в конец
    return temp;
}
//====================================================================================
double interpol(double begin, double end, double ratio)
{
    if(begin==-9999 && end==-9999) return -9999;
    if(ratio<1) if(begin == -9999 || end==-9999) return begin;
    else if(begin == -9999 || end==-9999) return end;
    return begin+(end-begin)*ratio;
}
//====================================================================================
double averaging(double one, double two, int *ptr)
{
    if(one==-9999 && two==-9999) {(*ptr)--; return -9999;}
    if(one==-9999 && two != -9999) {(*ptr)--; return two;}
    if(one != -9999 && two==-9999) {(*ptr)--; return one;}
    return (one*(*ptr)+two)/(1+(*ptr));
}
//====================================================================================
int get_ABACUS_rev(void)
{	if(Fr45->RGAbacus->ItemIndex == 0) return 5;//Rev-5
	if(Fr45->RGAbacus->ItemIndex == 1) return 4;//Rev-4
}
//====================================================================================
//запись значений осей в файл *.in для Baker  по новому
void write_to_file_in(void)
{
    if(get_ABACUS_rev() == 5) write_to_file_in_rev5();//Rev-5
	if(get_ABACUS_rev() == 4) write_to_file_in_rev4();//Rev-4


//Справка
//ShowMessage('Диск       = '+ExtractFileDrive (fullFileName));//C:
//ShowMessage('Каталог    = '+ExtractFileDir   (fullFileName));//C:\Program Files
//ShowMessage('Путь       = '+ExtractFilePath  (fullFileName));//C:\Program Files\
//ShowMessage('Имя        = '+ExtractFileName  (fullFileName));//file.txt
//ShowMessage('Расширение = '+ExtractFileExt   (fullFileName));//.txt

}
//====================================================================================
//запись значений осей в файл *.in для Baker  по Rev-4
/*
ОПИСАНИЕ и ПРИМЕР:
заголовок,С.Ш.,В.Д.,альтитуда ротора,номер рейса,название ЗТС,номер инклинометра ЗТС,Заказчик,Месторождение,Куст,Скважина,Секция
UnixTime,Глубина,Gx,Gy,Gz,Bx,By,Bz,RefGt,RefBt,RefDip,RefNC
- знак окончания заголовка / больше не используется

ABACUS_Rev-4,58.3564789,75.38654985,71.58,5,СИБ2.2,12598,RN-NyaganNeftegaz,Krasnoleninskoe,132,1630,220.7 //12шт.
1700710725,1245.25,-0.1234,0.2548,0.9857,-0.12589,0.25478,0.59857,1.00256,0.59865,78.21,16.25
*/
void write_to_file_in_rev4(void)
{
AnsiString ss,s1,s2;
int i,j,count_string,pos,count;
float gl_in,Gx_in,Gy_in,Gz_in,Bx,By,Bz,RefBt,RefDip=0,RefNC=0;
double ff, RefGt, Gt;
__int64 time;

  //определим стандартные параметры, общие
  //УМН
  //try{ff=get_umn();} catch(...){ff=-9999.0;}//проверяем на число
  //if(ff!=-9999.0) RefDip=90.0-ff; else RefDip=ff;//90-УМН
  //УМСклон
  //try{ff=get_ums();} catch(...){ff=-9999.0;}//проверяем на число
  //RefNC=ff;

  //надо собирать все в одну строку с разделителем 0x0A
   ss="";
   //положим строку заголовка
   ss+="ABACUS_Rev-4,"+Fr45->TablePasport->Cells[1][4]+",";//С.Ш.
   ss+=Fr45->TablePasport->Cells[1][5]+",";//В.Д.
   ss+=Fr45->TablePasport->Cells[1][6]+",";//Адьтитуда ротора
   ss+=Fr45->TablePasport->Cells[1][8]+",";//Номер рейса
   ss+=Fr45->Cname->Text+",";//Имя телесистемы
   //вытащим номер прибора, если есть
   AnsiString str = Fr45->Cdat->Text, p="";
   for(int i=0;i<str.Length();i++) if(isdigit(str[1+i]))
   {    while(i<str.Length() && isdigit(str[1+i])) p+= str[++i];
        break;
   }
   if(p=="") p="0";
   ss+= p + ",";//Номер прибора
   ss+=Fr45->TablePasport->Cells[1][0]+",";//Заказчик
   ss+=Fr45->TablePasport->Cells[1][1]+",";//Месторождение
   ss+=Fr45->TablePasport->Cells[1][2]+",";//Куст
   ss+=Fr45->Ckw->Text+",";//Скважина
   ss+=Fr45->TablePasport->Cells[1][3];//Секция
   ss+="\n";

  //Парсим строки Статики  из MemoStat
  count_string=Fr45->MemoStat->Lines->Count;//кол-во строк в мемо
  //пройдемся по строкам мемо и выделим нужные параметры в общую строку
  for(i=1;i<count_string;i++)//начинаем после заголовка
  {
   s1=Fr45->MemoStat->Lines->Strings[i];//возьмем строку
   s1=StringReplace(s1,",",".",TReplaceFlags()<<rfIgnoreCase);//замена запятой на точку в строке
   s1=s1.Trim();//уберем пробелы слева и справа
   j=0; count=0; RefGt=-9999; time=0; Gt=-9999;
   //парсим очередную строку по параметрам
   while(j==0)
   {
    pos=AnsiString(s1).Pos(" ");//узнаем позицию ближайшего пробела
    if(pos!=0)
    {
     s2=s1.SubString(1,pos);//получили параметр
     s1.Delete(1,pos);//удалим параметр из строки
     s1=s1.TrimLeft();//удалим пробелы слева
    }
    else {j=1; s2=s1;}//дошли до последнего числа
    count++;//увеличиваем номер параметра в строке
    if(s2.Pos("_") > 0) {time = get_UTimeFromString(s2); s2="-9999";}//это время
    else try{ff=MyStrToFloat(s2);} catch(...){ff=-9999.0;}//проверяем на число
    //нужный параметр положим в нужную переменную
    switch(count)
    {
     //Глубина float, 2 знака после запятой
     case(1): gl_in=ff-get_mkonec_zts(); break;//Приведем глубину к мертвому концу ЗТС_Инк
     //Gx float, 5 знака после запятой
     case(8): Gx_in=ff; break;
     //Gy float, 5 знака после запятой
     case(9): Gy_in=ff; break;
     //Gz float, 5 знака после запятой
     case(10): Gz_in=ff; break;
     //Hx float, 5 знаков после запятой
     case(11): Bx=ff; break;
     //Hy float, 5 знаков после запятой
     case(12): By=ff; break;
     //Hz float, 5 знаков после запятой
     case(13): Bz=ff; break;
     //Htotal float, 5 знаков после запятой
     case(14): RefBt=ff; break;//Htotal
     //RefGt из паспорта прибора
     case(15): Gt = ff; break;
     //RefGt на скважине
     case(18): RefGt = ff; break;
     //RefDip на скважине
     case(19): RefDip = ff; break;
     //RefNC на скважине
     case(20): RefNC = ff; break;
    }
   }//строка распарсена
   //первым теперь пойдет время
   ss+=IntToStr(time)+",";
   //Глубина,Gx,Gy,Gz,Bx,By,Bz,RefGt,RefBt,RefDip,RefNC
   ss+=FloatToStrF0(gl_in,8,2)+",";//Глубина float, 2 знака после запятой
   //Gx float, 5 знака после запятой
   ss+=FloatToStrF0(Gx_in,7,5)+",";
   //Gy float, 5 знака после запятой
   ss+=FloatToStrF0(Gy_in,7,5)+",";
   //Gz float, 5 знака после запятой
   ss+=FloatToStrF0(Gz_in,7,5)+",";
   //Hx float, 5 знаков после запятой
   ss+=FloatToStrF0(Bx/100000.,7,5)+",";
   //Hy float, 5 знаков после запятой
   ss+=FloatToStrF0(By/100000.,7,5)+",";
   //Hz float, 5 знаков после запятой
   ss+=FloatToStrF0(Bz/100000.,7,5)+",";
   //RefGt float, 6 знаков после запятой
   if(Gt==0 || Gt==-9999) RefGt = 0;
   else if(RefGt==0 || RefGt==-9999) RefGt = (double)MyStrToFloat(Fr45->TablePasport->Cells[1][7]) / Gt;
   else RefGt = RefGt / Gt; //RefGt на скважине/Gt из паспорта прибора
   ss+=FloatToStrF0(RefGt,7,6)+",";
   //RefBt float, 5 знаков после запятой
   ss+=FloatToStrF0(RefBt/100000.,7,5)+",";
   //RefDip float, 2 знака после запятой
   ss+=FloatToStrF0(RefDip,6,2)+",";
   //RefNC float, 2 знака после запятой
   ss+=FloatToStrF0(RefNC,6,2)+(char)0x0A;//конец подстроки
  }

  FILE *out1;
  if((out1=fopen(Fin.c_str(),"wb"))==NULL) {ShowMessage("Не удается открыть файл "+Fin);}
  else
  {
   fprintf(out1,"%s",ss);
   fclose(out1); out1=NULL;
  }

  out1=NULL;
  if((out1=fopen(Fmagcorr.c_str(),"wb"))==NULL) {ShowMessage("Не удается открыть файл "+Fmagcorr);}
  else
  {
   fprintf(out1,"%s",ss);
   fclose(out1); //out1=NULL;
   //ShowMessage("Записал файл "+Fmagcorr);
  }
}
//====================================================================================
//запись значений осей в файл raw4.in для Baker  по Rev-4
/*
ОПИСАНИЕ и ПРИМЕР:
заголовок,С.Ш.,В.Д.,альтитуда ротора,номер рейса,название ЗТС,номер инклинометра ЗТС,Заказчик,Месторождение,Куст,Скважина,Секция
UnixTime,Глубина,Gx,Gy,Gz,Bx,By,Bz,RefGt,RefBt,RefDip,RefNC
- знак окончания заголовка / больше не используется

ABACUS_Rev-4,58.3564789,75.38654985,71.58,5,СИБ2.2,12598,RN-NyaganNeftegaz,Krasnoleninskoe,132,1630,220.7 //12шт.
1700710725,1245.25,-0.1234,0.2548,0.9857,-0.12589,0.25478,0.59857,1.00256,0.59865,78.21,16.25
*/
void write_to_file_raw4(AnsiString str)
{
AnsiString ss,s1,s2;
int i,j,count_string,pos,count;
float gl_in,Gx_in,Gy_in,Gz_in,Bx,By,Bz,RefBt,RefDip=0,RefNC=0;
double ff, RefGt, Gt;
__int64 time;

  //надо собирать все в одну строку с разделителем 0x0A
   ss="";
  //Парсим строку Статики  из str
  count_string=1;//кол-во строк
  //пройдемся по строкам и выделим нужные параметры в общую строку
  for(i=0;i<count_string;i++)
  {
   s1=str;//возьмем строку
   s1=StringReplace(s1,",",".",TReplaceFlags()<<rfIgnoreCase);//замена запятой на точку в строке
   s1=s1.Trim();//уберем пробелы слева и справа
   j=0; count=0; RefGt=-9999; time=0; Gt=-9999;
   //парсим очередную строку по параметрам
   while(j==0)
   {
    pos=AnsiString(s1).Pos(" ");//узнаем позицию ближайшего пробела
    if(pos!=0)
    {
     s2=s1.SubString(1,pos);//получили параметр
     s1.Delete(1,pos);//удалим параметр из строки
     s1=s1.TrimLeft();//удалим пробелы слева
    }
    else {j=1; s2=s1;}//дошли до последнего числа
    count++;//увеличиваем номер параметра в строке
    if(s2.Pos("_") > 0) {time = get_UTimeFromString(s2); s2="-9999";}//это время
    else try{ff=MyStrToFloat(s2);} catch(...){ff=-9999.0;}//проверяем на число
    //нужный параметр положим в нужную переменную
    switch(count)
    {
     //Глубина float, 2 знака после запятой
     case(1): gl_in=ff-get_mkonec_zts(); break;//Приведем глубину к мертвому концу ЗТС_Инк
     //Gx float, 5 знака после запятой
     case(8): Gx_in=ff; break;
     //Gy float, 5 знака после запятой
     case(9): Gy_in=ff; break;
     //Gz float, 5 знака после запятой
     case(10): Gz_in=ff; break;
     //Hx float, 5 знаков после запятой
     case(11): Bx=ff; break;
     //Hy float, 5 знаков после запятой
     case(12): By=ff; break;
     //Hz float, 5 знаков после запятой
     case(13): Bz=ff; break;
     //Htotal float, 5 знаков после запятой
     case(14): RefBt=ff; break;//Htotal
     //RefGt из паспорта прибора
     case(15): Gt = ff; break;
     //RefGt на скважине
     case(18): RefGt = ff; break;
     //RefDip на скважине
     case(19): RefDip = ff; break;
     //RefNC на скважине
     case(20): RefNC = ff; break;
    }
   }//строка распарсена
   //первым теперь пойдет время
   ss+=IntToStr(time)+",";
   //Глубина,Gx,Gy,Gz,Bx,By,Bz,RefGt,RefBt,RefDip,RefNC
   ss+=FloatToStrF0(gl_in,8,2)+",";//Глубина float, 2 знака после запятой
   //Gx float, 5 знака после запятой
   ss+=FloatToStrF0(Gx_in,7,5)+",";
   //Gy float, 5 знака после запятой
   ss+=FloatToStrF0(Gy_in,7,5)+",";
   //Gz float, 5 знака после запятой
   ss+=FloatToStrF0(Gz_in,7,5)+",";
   //Hx float, 5 знаков после запятой
   ss+=FloatToStrF0(Bx/100000.,7,5)+",";
   //Hy float, 5 знаков после запятой
   ss+=FloatToStrF0(By/100000.,7,5)+",";
   //Hz float, 5 знаков после запятой
   ss+=FloatToStrF0(Bz/100000.,7,5)+",";
   //RefGt float, 6 знаков после запятой
   if(Gt==0 || Gt==-9999) RefGt = 0;
   else if(RefGt==0 || RefGt==-9999) RefGt = (double)MyStrToFloat(Fr45->TablePasport->Cells[1][7]) / Gt;
   else RefGt = RefGt / Gt; //RefGt на скважине/Gt из паспорта прибора
   ss+=FloatToStrF0(RefGt,7,6)+",";
   //RefBt float, 5 знаков после запятой
   ss+=FloatToStrF0(RefBt/100000.,7,5)+",";
   //RefDip float, 2 знака после запятой
   ss+=FloatToStrF0(RefDip,6,2)+",";
   //RefNC float, 2 знака после запятой
   ss+=FloatToStrF0(RefNC,6,2)+(char)0x0A;//конец подстроки
  }

  FILE *out1;
  if((out1=fopen(Fraw4.c_str(),"ab+"))==NULL) {ShowMessage("Не удается открыть файл "+Fraw4);}
  else
  {
   fprintf(out1,"%s",ss);
   fclose(out1); out1=NULL;
  }

  out1=NULL;
}
//====================================================================================
//запись значений осей в файл *.in для Baker  по Rev-5
/*
Заголовок теперь с версией 5, номер рейса из заголовка убран, теперь у каждого замера указывается номер рейса,
в котором он принят.
ОПИСАНИЕ и ПРИМЕР:
заголовок,С.Ш.,В.Д.,альтитуда ротора,название ЗТС,номер инклинометра ЗТС,Заказчик,Месторождение,
Куст,Скважина,Секция
UnixTime,Рейс,Глубина,Gx,Gy,Gz,Bx,By,Bz,RefGt,RefBt,RefDip,RefNC
*/
void write_to_file_in_rev5(void)
{
AnsiString ss,s1,s2;
int i,j,count_string,pos,count;
float gl_in,Gx_in,Gy_in,Gz_in,Bx,By,Bz,RefBt,RefDip=0,RefNC=0;
double ff, RefGt, Gt;
__int64 time;

  //надо собирать все в одну строку с разделителем 0x0A
   ss="";
   //положим строку заголовка
   ss+="ABACUS_Rev-5,"+Fr45->TablePasport->Cells[1][4]+",";//С.Ш.
   ss+=Fr45->TablePasport->Cells[1][5]+",";//В.Д.
   ss+=Fr45->TablePasport->Cells[1][6]+",";//Адьтитуда ротора
   //ss+=Fr45->TablePasport->Cells[1][8]+",";//Номер рейса теперь перенесен в данные
   ss+=Fr45->Cname->Text+",";//Имя телесистемы
   //вытащим номер прибора, если есть
   AnsiString str = Fr45->Cdat->Text, p="";
   for(int i=0;i<str.Length();i++) if(isdigit(str[1+i]))
   {    while(i<str.Length() && isdigit(str[1+i])) p+= str[++i];
        break;
   }
   if(p=="") p="0";
   ss+= p + ",";//Номер прибора
   ss+=Fr45->TablePasport->Cells[1][0]+",";//Заказчик
   ss+=Fr45->TablePasport->Cells[1][1]+",";//Месторождение
   ss+=Fr45->TablePasport->Cells[1][2]+",";//Куст
   ss+=Fr45->Ckw->Text+",";//Скважина
   ss+=Fr45->TablePasport->Cells[1][3];//Секция
   ss+="\n";

  //Парсим строки Статики  из MemoStat
  count_string=Fr45->MemoStat->Lines->Count;//кол-во строк в мемо
  //пройдемся по строкам мемо и выделим нужные параметры в общую строку
  for(i=1;i<count_string;i++)//начинаем после заголовка
  {
   s1=Fr45->MemoStat->Lines->Strings[i];//возьмем строку
   s1=StringReplace(s1,",",".",TReplaceFlags()<<rfIgnoreCase);//замена запятой на точку в строке
   s1=s1.Trim();//уберем пробелы слева и справа
   j=0; count=0; RefGt=-9999; time=0; Gt=-9999;
   //парсим очередную строку по параметрам
   while(j==0)
   {
    pos=AnsiString(s1).Pos(" ");//узнаем позицию ближайшего пробела
    if(pos!=0)
    {
     s2=s1.SubString(1,pos);//получили параметр
     s1.Delete(1,pos);//удалим параметр из строки
     s1=s1.TrimLeft();//удалим пробелы слева
    }
    else {j=1; s2=s1;}//дошли до последнего числа
    count++;//увеличиваем номер параметра в строке
    if(s2.Pos("_") > 0) {time = get_UTimeFromString(s2); s2="-9999";}//это время
    else try{ff=MyStrToFloat(s2);} catch(...){ff=-9999.0;}//проверяем на число
    //нужный параметр положим в нужную переменную
    switch(count)
    {
     //Глубина float, 2 знака после запятой
     case(1): gl_in=ff-get_mkonec_zts(); break;//Приведем глубину к мертвому концу ЗТС_Инк
     //Gx float, 5 знака после запятой
     case(8): Gx_in=ff; break;
     //Gy float, 5 знака после запятой
     case(9): Gy_in=ff; break;
     //Gz float, 5 знака после запятой
     case(10): Gz_in=ff; break;
     //Hx float, 5 знаков после запятой
     case(11): Bx=ff; break;
     //Hy float, 5 знаков после запятой
     case(12): By=ff; break;
     //Hz float, 5 знаков после запятой
     case(13): Bz=ff; break;
     //Htotal float, 5 знаков после запятой
     case(14): RefBt=ff; break;//Htotal
     //RefGt из паспорта прибора
     case(15): Gt = ff; break;
     //RefGt на скважине
     case(18): RefGt = ff; break;
     //RefDip на скважине
     case(19): RefDip = ff; break;
     //RefNC на скважине
     case(20): RefNC = ff; break;
    }
   }//строка распарсена
   //UnixTime,Рейс,Глубина,Gx,Gy,Gz,Bx,By,Bz,RefGt,RefBt,RefDip,RefNC
   //первым теперь пойдет время
   ss+=IntToStr(time)+",";
   //Номер рейса теперь перенесен сюда
   ss+=Fr45->TablePasport->Cells[1][8]+",";
   //Глубина float, 2 знака после запятой
   ss+=FloatToStrF0(gl_in,8,2)+",";
   //Gx float, 5 знака после запятой
   ss+=FloatToStrF0(Gx_in,7,5)+",";
   //Gy float, 5 знака после запятой
   ss+=FloatToStrF0(Gy_in,7,5)+",";
   //Gz float, 5 знака после запятой
   ss+=FloatToStrF0(Gz_in,7,5)+",";
   //Hx float, 5 знаков после запятой
   ss+=FloatToStrF0(Bx/100000.,7,5)+",";
   //Hy float, 5 знаков после запятой
   ss+=FloatToStrF0(By/100000.,7,5)+",";
   //Hz float, 5 знаков после запятой
   ss+=FloatToStrF0(Bz/100000.,7,5)+",";
   //RefGt float, 6 знаков после запятой
   if(Gt==0 || Gt==-9999) RefGt = 0;
   else if(RefGt==0 || RefGt==-9999) RefGt = (double)MyStrToFloat(Fr45->TablePasport->Cells[1][7]) / Gt;
   else RefGt = RefGt / Gt; //RefGt на скважине/Gt из паспорта прибора
   ss+=FloatToStrF0(RefGt,7,6)+",";
   //RefBt float, 5 знаков после запятой
   ss+=FloatToStrF0(RefBt/100000.,7,5)+",";
   //RefDip float, 2 знака после запятой
   ss+=FloatToStrF0(RefDip,6,2)+",";
   //RefNC float, 2 знака после запятой
   ss+=FloatToStrF0(RefNC,6,2)+"\n";//конец подстроки
  }

  ss = AnsiToUtf8(ss);//преобразуем строку в UTF-8
  //ShowMessage(Utf8ToAnsi(ss));

  FILE *out1;
  if((out1=fopen(Fin.c_str(),"wb"))==NULL) {ShowMessage("Не удается открыть файл "+Fin);}
  else
  {
   //uint8_t buf[3] = {0xEF,0xBB,0xBF};
   //fwrite(buf,1,sizeof(buf),out1);
   fprintf(out1,ss.c_str());
   fclose(out1); out1=NULL;
  }

  out1=NULL;
  if((out1=fopen(Fmagcorr.c_str(),"wb"))==NULL) {ShowMessage("Не удается открыть файл "+Fmagcorr);}
  else
  {
   //uint8_t buf[3] = {0xEF,0xBB,0xBF};//BOM
   //fwrite(buf,1,sizeof(buf),out1);//записываем BOM
   fprintf(out1,ss.c_str());
   fclose(out1); //out1=NULL;
  } 
}
//====================================================================================
//запись значений осей в файл raw5.in для Baker  по Rev-5
/*
Заголовок теперь с версией 5, номер рейса из заголовка убран, теперь у каждого замера указывается номер рейса,
в котором он принят.
ОПИСАНИЕ и ПРИМЕР:
заголовок,С.Ш.,В.Д.,альтитуда ротора,название ЗТС,номер инклинометра ЗТС,Заказчик,Месторождение,
Куст,Скважина,Секция
UnixTime,Рейс,Глубина,Gx,Gy,Gz,Bx,By,Bz,RefGt,RefBt,RefDip,RefNC

ABACUS_Rev-5,58.3564789,75.38654985,71.58,СИБ2.2,12598,RN-NyaganNeftegaz,Krasnoleninskoe,132,1630,220.7 //12шт.
1700710725,5,1245.25,-0.1234,0.2548,0.9857,-0.12589,0.25478,0.59857,1.00256,0.59865,78.21,16.25

но в этот файл пишем без заголовка
*/
void write_to_file_raw5(AnsiString str)
{
AnsiString ss,s1,s2;
int i,j,count_string,pos,count;
float gl_in,Gx_in,Gy_in,Gz_in,Bx,By,Bz,RefBt,RefDip=0,RefNC=0;
double ff, RefGt, Gt;
__int64 time;

  //надо собирать все в одну строку с разделителем 0x0A
   ss="";
  //Парсим строку Статики  из str
  count_string=1;//кол-во строк
  //пройдемся по строкам и выделим нужные параметры в общую строку
  for(i=0;i<count_string;i++)
  {
   s1=str;//возьмем строку
   s1=StringReplace(s1,",",".",TReplaceFlags()<<rfIgnoreCase);//замена запятой на точку в строке
   s1=s1.Trim();//уберем пробелы слева и справа
   j=0; count=0; RefGt=-9999; time=0; Gt=-9999;
   //парсим очередную строку по параметрам
   while(j==0)
   {
    pos=AnsiString(s1).Pos(" ");//узнаем позицию ближайшего пробела
    if(pos!=0)
    {
     s2=s1.SubString(1,pos);//получили параметр
     s1.Delete(1,pos);//удалим параметр из строки
     s1=s1.TrimLeft();//удалим пробелы слева
    }
    else {j=1; s2=s1;}//дошли до последнего числа
    count++;//увеличиваем номер параметра в строке
    if(s2.Pos("_") > 0) {time = get_UTimeFromString(s2); s2="-9999";}//это время
    else try{ff=MyStrToFloat(s2);} catch(...){ff=-9999.0;}//проверяем на число
    //нужный параметр положим в нужную переменную
    switch(count)
    {
     //Глубина float, 2 знака после запятой
     case(1): gl_in=ff-get_mkonec_zts(); break;//Приведем глубину к мертвому концу ЗТС_Инк
     //Gx float, 5 знака после запятой
     case(8): Gx_in=ff; break;
     //Gy float, 5 знака после запятой
     case(9): Gy_in=ff; break;
     //Gz float, 5 знака после запятой
     case(10): Gz_in=ff; break;
     //Hx float, 5 знаков после запятой
     case(11): Bx=ff; break;
     //Hy float, 5 знаков после запятой
     case(12): By=ff; break;
     //Hz float, 5 знаков после запятой
     case(13): Bz=ff; break;
     //Htotal float, 5 знаков после запятой
     case(14): RefBt=ff; break;//Htotal
     //RefGt из паспорта прибора
     case(15): Gt = ff; break;
     //RefGt на скважине
     case(18): RefGt = ff; break;
     //RefDip на скважине
     case(19): RefDip = ff; break;
     //RefNC на скважине
     case(20): RefNC = ff; break;
    }
   }//строка распарсена
   //UnixTime,Рейс,Глубина,Gx,Gy,Gz,Bx,By,Bz,RefGt,RefBt,RefDip,RefNC
   //первым теперь пойдет время
   ss+=IntToStr(time)+",";
   //Номер рейса теперь перенесен сюда
   ss+=Fr45->TablePasport->Cells[1][8]+",";
   //Глубина float, 2 знака после запятой
   ss+=FloatToStrF0(gl_in,8,2)+",";
   //Gx float, 5 знака после запятой
   ss+=FloatToStrF0(Gx_in,7,5)+",";
   //Gy float, 5 знака после запятой
   ss+=FloatToStrF0(Gy_in,7,5)+",";
   //Gz float, 5 знака после запятой
   ss+=FloatToStrF0(Gz_in,7,5)+",";
   //Hx float, 5 знаков после запятой
   ss+=FloatToStrF0(Bx/100000.,7,5)+",";
   //Hy float, 5 знаков после запятой
   ss+=FloatToStrF0(By/100000.,7,5)+",";
   //Hz float, 5 знаков после запятой
   ss+=FloatToStrF0(Bz/100000.,7,5)+",";
   //RefGt float, 6 знаков после запятой
   if(Gt==0 || Gt==-9999) RefGt = 0;
   else if(RefGt==0 || RefGt==-9999) RefGt = (double)MyStrToFloat(Fr45->TablePasport->Cells[1][7]) / Gt;
   else RefGt = RefGt / Gt; //RefGt на скважине/Gt из паспорта прибора
   ss+=FloatToStrF0(RefGt,7,6)+",";
   //RefBt float, 5 знаков после запятой
   ss+=FloatToStrF0(RefBt/100000.,7,5)+",";
   //RefDip float, 2 знака после запятой
   ss+=FloatToStrF0(RefDip,6,2)+",";
   //RefNC float, 2 знака после запятой
   ss+=FloatToStrF0(RefNC,6,2)+"\n";//конец подстроки
  }

  
  FILE *out1;
  if((out1=fopen(Fraw5.c_str(),"ab+"))==NULL) {ShowMessage("Не удается открыть файл "+Fraw5);}
  else
  {
   //uint8_t buf[3] = {0xEF,0xBB,0xBF};//BOM
   //fwrite(buf,1,sizeof(buf),out1);//записываем BOM
   fprintf(out1,ss.c_str());
   fclose(out1); out1=NULL;
  }

  out1=NULL;
}
//====================================================================================
//запись значений осей в файл *.sur для SUCOP
//собираем чисто текстовый файл с разделителем = пробелы, как в LAS
void write_to_file_sur(void)
{
AnsiString ss,s1,s2;
int i,j,count_string,pos,count;
float ff;

  //Перекодируем значения из MemoStat
  count_string=Fr45->MemoStat->Lines->Count;//кол-во строк в мемо
  ss="";//надо собирать все в одну строку с пробелами
  //положим строку заголовка
  //1 - Hop/1000, мкТл
  ff=get_Hop()/1000; if(ff<0) ff=0;
  ss+=PadRight(FloatToStrF0(ff,7,3),DlinaParam);
  //2 - УМН
  ff=get_umn();//УМН
  ss+=PadRight(FloatToStrF0(ff,6,2),DlinaParam);
  //3 - УМC
  ff=get_ums();
  ss+=PadRight(FloatToStrF0(ff,6,2),DlinaParam);
  //4 - Gtotal
  ff=MyStrToFloat(Fr45->TablePasport->Cells[1][7]);
  ss+=PadRight(FloatToStrF0(ff,7,6),DlinaParam);
  //5 - 12 = 0
  for(int i=0;i<8;i++) ss+=PadRight(FloatToStrF0(0,7,2),DlinaParam);
  //13 - АЗ ожидаемый
  ff=0;//MyStrToFloat(Fr45->Sn->Cells[1][1]);
  ss+=PadRight(FloatToStrF0(ff,7,1),DlinaParam);
  //14 - Допуск на УМН, град
  ss+=PadRight(FloatToStrF0(0.5,7,1),DlinaParam);
  //15 - погрешность измеренного G, g
  ss+=PadRight(FloatToStrF0(0.005,7,3),DlinaParam);
  //16 - допуск на Btotal, мкТл
  ss+=PadRight(FloatToStrF0(0.3,7,3),DlinaParam);
  //17 - длина тяжести собак, м
  ss+=PadRight(FloatToStrF0(10,5,2),DlinaParam);
  //18 - допуск на тяжесть собак, град
  ss+=PadRight(FloatToStrF0(5,5,1),DlinaParam);
  ss+="\r\n";

  //пройдемся по строкам мемо и выделим нужные параметры в общую строку
  for(i=1;i<count_string;i++)
  {
   s1=Fr45->MemoStat->Lines->Strings[i];//возьмем строку
   s1=StringReplace(s1,",",".",TReplaceFlags()<<rfIgnoreCase);//замена запятой на точку в строке
   s1=s1.Trim();//уберем пробелы слева и справа
   j=0; count=0;
   while(j==0)//пройдемся по строке и выделим параметры
   {
    pos=AnsiString(s1).Pos(" ");//узнаем позицию ближайшего пробела
    if(pos!=0)
    {
     s2=s1.SubString(1,pos);//получили параметр
     s1.Delete(1,pos);//удалим параметр из строки
     s1=s1.TrimLeft();//удалим пробелы слева
    }
    else {j=1; s2=s1;}//дошли до последнего числа
    if(s2.Pos("_") > 0) {s2="-9999";}//это время
    //проверяем на число, и если ок, то увеличиваем count
    try{ff=MyStrToFloat(s2); count++;} catch(...){continue;}//count=номер параметра
    //нужные параметры преобразуем и положим в общую строку
    //только глубина и составляющие Gx,Gy,Gz,Hx,Hy,Hz
    switch(count)
    {
     //Глубина float, 2 знака после запятой
     //Приведем глубину к мертвому концу ЗТС_Инк
     case(1): ff=MyStrToFloat(s2)-get_mkonec_zts(); ss+=PadRight(FloatToStrF0(ff,8,2),DlinaParam); break;
     //Gx float, 4 знака после запятой
     case(8): ff=MyStrToFloat(s2); ss+=PadRight(FloatToStrF0(ff,6,4),DlinaParam); break;
     //Gy float, 4 знака после запятой
     case(9): ff=MyStrToFloat(s2); ss+=PadRight(FloatToStrF0(ff,6,4),DlinaParam); break;
     //Gz float, 4 знака после запятой
     case(10): ff=MyStrToFloat(s2); ss+=PadRight(FloatToStrF0(ff,6,4),DlinaParam); break;
     //Hx float, 3 знаков после запятой, мкТл
     case(11): ff=MyStrToFloat(s2); ss+=PadRight(FloatToStrF0(ff/1000.,7,3),DlinaParam); break;
     //Hy float, 3 знаков после запятой, мкТл
     case(12): ff=MyStrToFloat(s2); ss+=PadRight(FloatToStrF0(ff/1000.,7,3),DlinaParam); break;
     //Hz float, 3 знаков после запятой , мкТл
     case(13): ff=MyStrToFloat(s2); ss+=PadRight(FloatToStrF0(ff/1000.,7,3),DlinaParam);
                ss+="\r\n";//конец подстроки
     			break;
    }
   }
  }

  FILE *out1;
  if((out1=fopen(Fsur.c_str(),"wb"))==NULL) {ShowMessage("Не удается открыть файл "+Fsur);}
  else
  {
   fprintf(out1,"%s",ss);
   fclose(out1); out1=NULL;
  }
}
//====================================================================================
//запись значений осей в файл *.txt для GeoTrack
//собираем чисто текстовый файл с разделителем = пробелы, как в LAS
void write_to_file_geotrack(void)
{
AnsiString ss,s1,s2;
int i,j,count_string,pos,count;
float ff;

  //Перекодируем значения из MemoStat
  count_string=Fr45->MemoStat->Lines->Count;//кол-во строк в мемо
  ss="";//надо собирать все в одну строку с пробелами

  //пройдемся по строкам мемо и выделим нужные параметры в общую строку
  for(i=1;i<count_string;i++)
  {
   s1=Fr45->MemoStat->Lines->Strings[i];//возьмем строку
   s1=StringReplace(s1,",",".",TReplaceFlags()<<rfIgnoreCase);//замена запятой на точку в строке
   s1=s1.Trim();//уберем пробелы слева и справа
   j=0; count=0;
   while(j==0)//пройдемся по строке и выделим параметры
   {
    pos=AnsiString(s1).Pos(" ");//узнаем позицию ближайшего пробела
    if(pos!=0)
    {
     s2=s1.SubString(1,pos);//получили параметр
     s1.Delete(1,pos);//удалим параметр из строки
     s1=s1.TrimLeft();//удалим пробелы слева
    }
    else {j=1; s2=s1;}//дошли до последнего числа
    if(s2.Pos("_") > 0) {s2="-9999";}//это время
    //проверяем на число, и если ок, то увеличиваем count
    try{ff=MyStrToFloat(s2); count++;} catch(...){continue;}//count=номер параметра
    //нужные параметры преобразуем и положим в общую строку
    //только глубина и составляющие Gx,Gy,Gz,Hx,Hy,Hz
    switch(count)
    {
     //Глубина float, 2 знака после запятой
     //Приведем глубину к мертвому концу ЗТС_Инк
     case(1): ff=MyStrToFloat(s2)-get_mkonec_zts(); ss+=PadRight(FloatToStrF0(ff,8,2),DlinaParam); break;
     //Gx float, 4 знака после запятой
     case(8): ff=MyStrToFloat(s2); ss+=PadRight(FloatToStrF0(ff,6,4),DlinaParam); break;
     //Gy float, 4 знака после запятой
     case(9): ff=MyStrToFloat(s2); ss+=PadRight(FloatToStrF0(ff,6,4),DlinaParam); break;
     //Gz float, 4 знака после запятой
     case(10): ff=MyStrToFloat(s2); ss+=PadRight(FloatToStrF0(ff,6,4),DlinaParam); break;
     //Hx float, 0 знаков после запятой, нТл
     case(11): ff=MyStrToFloat(s2); ss+=PadRight(FloatToStrF0(ff,6,0),DlinaParam); break;
     //Hy float, 0 знаков после запятой, нТл
     case(12): ff=MyStrToFloat(s2); ss+=PadRight(FloatToStrF0(ff,6,0),DlinaParam); break;
     //Hz float, 0 знаков после запятой, нТл
     case(13): ff=MyStrToFloat(s2); ss+=PadRight(FloatToStrF0(ff,6,0),DlinaParam);
                ss+="\r\n";//конец подстроки
     			break;
    }
   }
  }

  FILE *out1;
  if((out1=fopen(Fgeotrack.c_str(),"wb"))==NULL) {ShowMessage("Не удается открыть файл "+Fgeotrack);}
  else
  {
   fprintf(out1,"%s",ss);
   fclose(out1); out1=NULL;
  }
}
//====================================================================================
void add_string_to_memo(TMemo *memo, AnsiString glub)
{
  memo->Lines->Add(get_string_for_stat(glub));
}
//========================================================================
//Собирает строку из замеров для записи в массив строк углов
AnsiString get_string_for_stat(AnsiString glub)
{
extern float GXout, GYout, GZout;
extern int HXout, HYout, HZout;
AnsiString ss,s1;

  ss="";
  //Глубина
  s1=glub;
  ss+=PadRight(s1,DlinaParam);
  //Зенит ТС
  if(zenit<0) s1=IntToStr(-9999);
  else s1=FloatToStrF0(zenit,6,2);
  ss+=PadRight(s1,DlinaParam);
  //Зенит НДМ
  if(ZUndm==-9999) s1=IntToStr(-9999);
  else s1=FloatToStrF0(ZUndm,6,1);
  ss+=PadRight(s1,DlinaParam);
  //АЗ сырой ЗТС
  s1=FloatToStrF0(azimut,6,1);
  ss+=PadRight(s1,DlinaParam);
  //Азимут корр.
  if(Azk==-9999) s1="-9999";
  else s1=FloatToStrF0(Azk,6,1);
  ss+=PadRight(s1,DlinaParam);
  //Азимут истинный
  s1=AZist_str;
  ss+=PadRight(s1,DlinaParam);
  //ОТКграв, статический
  s1=FloatToStrF0(otklonf,5,1);
  ss+=PadRight(s1,DlinaParam);
  //Gx
  s1=FloatToStrF0(GXout,7,5);
  ss+=PadRight(s1,DlinaParam);
  //Gy
  s1=FloatToStrF0(GYout,7,5);
  ss+=PadRight(s1,DlinaParam);
  //Gz
  s1=FloatToStrF0(GZout,7,5);
  ss+=PadRight(s1,DlinaParam);
  //Hx
  s1=IntToStr(HXout);
  ss+=PadRight(s1,DlinaParam);
  //Hy
  s1=IntToStr(HYout);
  ss+=PadRight(s1,DlinaParam);
  //Hz
  s1=IntToStr(HZout);
  ss+=PadRight(s1,DlinaParam);
  //Hop
  s1=Fr45->Sn->Cells[1][5];//Hop
  ss+=PadRight(s1,DlinaParam);
  //Gref из паспорта прибора
  s1=Fr45->Sn->Cells[1][20];
  ss+=PadRight(s1,DlinaParam);
  //UTime
  s1=get_StringFromUTime(time(NULL));
  ss+=PadRight(s1,DlinaParam*2);
  //Gtotal, mg
  s1=FloatToStrF0(modg*1000,6,1);
  ss+=PadRight(s1,DlinaParam);
  //RefGt на скважине
  s1=Fr45->TablePasport->Cells[1][7];
  ss+=PadRight(s1,DlinaParam);
  //RefDip текущий из настроек
  float ff;
  try{ff=get_umn();} catch(...){ff=-9999.0;}//проверяем на число
  s1=FloatToStrF0(ff,6,2);
  ss+=PadRight(s1,DlinaParam);
  //RefNC текущий из настроек
  try{ff=get_ums();} catch(...){ff=-9999.0;}//проверяем на число
  s1=FloatToStrF0(ff,6,2);
  ss+=PadRight(s1,DlinaParam);

  ss=StringReplace(ss,",",".",TReplaceFlags()<<rfIgnoreCase);//замена запятой на точку в строке
  return ss;
}
//========================================================================
/*void SendToWits(AnsiString adr, AnsiString param)
{
AnsiString s1;
    s1="";
    s1=s1+"&&&&\r\n"+adr+param+"\r\n"+"!!\r\n";

    //WITS-сервер
    if(Fr45->ServerSocket1->Active==true) Fr45->ServerSocket1->Socket->Connections[0]->SendText(s1);
    //WITS-клиент
    else if(Fr45->ClientSocket2->Active==true) Fr45->ClientSocket2->Socket->SendText(s1);
}*/
//========================================================================
AnsiString CreateTopicWits(AnsiString zap, float glub_sens, float glub_bit)
{
AnsiString str="";
  CountKadrWits++;//счетчик посылок
  str="&&\r\n";//Начало
  str=str + zap + "01" + Fr45->Ckw->Text + "\r\n";//Идентификатор скважины
  str=str + zap + "02" + "0" + "\r\n";//Номер ствола, 0=один ствол
  str=str + zap + "03" + "159" + "\r\n";//150+идентификатор записи
  str=str + zap + "04" + IntToStr(CountKadrWits) + "\r\n";//Счетчик записей, порядковый номер по выходу
  str=str + zap + "05" + FormatDateTime("yymmdd", DateToStr(Date())) + "\r\n";//дата
  str=str + zap + "06" + FormatDateTime("hhmmss", TimeToStr(Time())) + "\r\n";//время
  str=str + zap + "07" + "2" + "\r\n";//Код проводимой работы, 2 - бурение
  str=str + zap + "08" + FloatToStrF0(glub_bit,6,2) + "\r\n";//Глубина долота
  //для записи "07" параметр 10 пропускаем
  if(zap != "07") str=str + zap + "10" + FloatToStrF0(glub_sens,6,2) + "\r\n";//Глубина долота
  return str;
}
//========================================================================
// параметры 09
//========================================================================
void Send09record(AnsiString kod, AnsiString param)
{
AnsiString s1="";
char buf[1000];
float glub_sens, glub_bit;
  try{
    if(WitsServer_ON==false && WitsClient_ON==false) return;
    //кадр 09
    glub_sens=MyStrToFloat(Fr45->Eglubina->Text) - MyStrToFloat(Fr45->Sn->Cells[1][14]);//глубина датчика ГК ЗТС
    glub_bit=MyStrToFloat(Fr45->Eglubina->Text);//глубина долота
    //заголовок
    s1=CreateTopicWits("09",glub_sens,glub_bit);
    //параметры
    s1=s1 + kod + param+"\r\n";
    s1=s1 + "!!\r\n";//Конец

    strcpy(buf, s1.c_str());
    //WITS-сервер
    try{
    if(Fr45->ServerSocket1->Active==true) Fr45->ServerSocket1->Socket->Connections[0]->SendBuf(buf, strlen(s1.c_str()));
    } catch(...){s1+="Ошибка catch Send09record: сервер wits\r\n";}
    //WITS-клиент
    try{
    if(Fr45->ClientSocket2->Active==true) Fr45->ClientSocket2->Socket->SendBuf(buf, strlen(s1.c_str()));
    } catch(...){s1+="Ошибка catch Send09record: клиент wits\r\n";}
    SaveWitsLog(s1);
  } catch(...){SaveWitsLog("Ошибка catch: Send09record()\r\n");}
}
//========================================================================
// ГК НДМ
//========================================================================
void SendGkNdmToWits(void)
{
AnsiString s1="";
char buf[1000];
float glub_sens, glub_bit;
  try{
    if(WitsServer_ON==false && WitsClient_ON==false) return;
	//кадр 08
    glub_sens=MyStrToFloat(Fr45->Eglubina->Text) - MyStrToFloat(Fr45->Sn->Cells[1][14]);//глубина датчика ГК ЗТС
    glub_bit=MyStrToFloat(Fr45->Eglubina->Text);//глубина долота
    //заголовок
    s1=CreateTopicWits("08",glub_sens,glub_bit);
    //параметры
    glub_sens=MyStrToFloat(Fr45->Eglubina->Text) - MyStrToFloat(Fr45->Sn->Cells[1][15]);//глубина датчика ГК НДМ
    s1=s1 + gk2_depth + FloatToStrF0(glub_sens,6,2) + "\r\n";//Глубина по стволу датчика ГК НДМ
    s1=s1 + gk2_raw + FloatToStrF0(GKndm_int,6,1)+"\r\n";//ГК НДМ сырой
    s1=s1 + "!!\r\n";//Конец

    strcpy(buf, s1.c_str());
    //WITS-сервер
    try{
    if(Fr45->ServerSocket1->Active==true) Fr45->ServerSocket1->Socket->Connections[0]->SendBuf(buf, strlen(s1.c_str()));
    } catch(...){s1+="Ошибка catch SendGkNdmToWits: сервер wits\r\n";}
    //WITS-клиент
    try{
    if(Fr45->ClientSocket2->Active==true) Fr45->ClientSocket2->Socket->SendBuf(buf, strlen(s1.c_str()));
    } catch(...){s1+="Ошибка catch SendGkNdmToWits: клиент wits\r\n";}
    SaveWitsLog(s1);
  } catch(...){SaveWitsLog("Ошибка catch: SendGkNdmToWits()\r\n");}
}
//========================================================================
// ГК ЗТС
//========================================================================
void SendGkZtsToWits(void)
{
AnsiString s1="";
char buf[1000];
float glub_sens, glub_bit;
  try{
    if(WitsServer_ON==false && WitsClient_ON==false) return;
	//кадр 08
    glub_sens=MyStrToFloat(Fr45->Eglubina->Text) - MyStrToFloat(Fr45->Sn->Cells[1][14]);//глубина датчика ГК ЗТС
    glub_bit=MyStrToFloat(Fr45->Eglubina->Text);//глубина долота
    //заголовок
    s1=CreateTopicWits("08",glub_sens,glub_bit);
    //параметры
    s1=s1 + gk1_depth + FloatToStrF0(glub_sens,6,2) + "\r\n";//Глубина по стволу датчика ГК ЗТС
    s1=s1 + gk1_raw + FloatToStrF0(gkzts,6,1) + "\r\n";//ГК ЗТС сырой
    s1=s1 + "!!\r\n";//Конец

    strcpy(buf, s1.c_str());
    //WITS-сервер
    try{
    if(Fr45->ServerSocket1->Active==true) Fr45->ServerSocket1->Socket->Connections[0]->SendBuf(buf, strlen(s1.c_str()));
    } catch(...){s1+="Ошибка catch SendGkZtsToWits: сервер wits\r\n";}
    //WITS-клиент
    try{
    if(Fr45->ClientSocket2->Active==true) Fr45->ClientSocket2->Socket->SendBuf(buf, strlen(s1.c_str()));
    } catch(...){s1+="Ошибка catch SendGkZtsToWits: клиент wits\r\n";}
    SaveWitsLog(s1);
  } catch(...){SaveWitsLog("Ошибка catch: SendGkZtsToWits()\r\n");}
}
//========================================================================
//запись 07
//========================================================================
void SendStaticToWits(void)
{
AnsiString s1="", str="", zundm, err="";
char buf[1000];
int index, flag=0;
float glub_sens, glub_bit, fl;
extern int HXout, HYout, HZout;
extern float GXout, GYout, GZout;
    if(WitsServer_ON==false && WitsClient_ON==false) return;
    if(Fr45->MemoStat->Lines->Count < 2) {SaveWitsLog("Статика пока пустая!"); return;}
    
    //забираем последнюю строку из MemoStat
  	str = Fr45->MemoStat->Lines->Strings[Fr45->MemoStat->Lines->Count-1];
    if(str=="") {err+="Ошибка SendStaticToWits = строка из Статики пустая!\r\n"; flag=1;}
    index=str.Pos(",");//проверим строку на наличие запятых
    if(index>0) {err+="Ошибка SendStaticToWits = строка Статики содержит запятые!\r\n"; flag=1;}

    //парсим глубину долота при замере
    str = str.TrimLeft();//удаляем пробелы слева
    index=str.Pos(" ");//ближайший пробел справа
    try{
    if(index>0) {glub_bit=MyStrToFloat(str.SubString(1,index)); str=str.Delete(1,index);}
    else {err+="Ошибка SendStaticToWits = глубина долота отсутствует!\r\n"; flag=1;}
    //глубина датчика ЗТС
    glub_sens=glub_bit - MyStrToFloat(Fr45->Sn->Cells[1][13]);//вычитаем мертвую зону
    } catch(...){err+="Ошибка catch SendStaticToWits: глубина\r\n"; flag=1;}

    //заголовок кадра 07
    s1=CreateTopicWits("07",glub_sens,glub_bit);
    //параметры
    //парсим ЗУзтс
    str = str.TrimLeft();//удаляем пробелы слева
  	index=str.Pos(" ");//ближайший пробел справа
    if(index>0) {s1=s1 + zts_zu+str.SubString(1,index) + "\r\n"; str=str.Delete(1,index);}
    else {err+="Ошибка = ЗУзтс отсутствует!\r\n"; flag=1;}
    //парсим ЗУндм, но не добавляем пока
    str = str.TrimLeft();//удаляем пробелы слева
  	index=str.Pos(" ");//ближайший пробел справа
    if(index>0) {zundm=str.SubString(1,index); str=str.Delete(1,index);}
    else {err+="Ошибка = ЗУндм отсутствует!\r\n"; flag=1;}
    //парсим АЗсырой
    str = str.TrimLeft();//удаляем пробелы слева
  	index=str.Pos(" ");//ближайший пробел справа
    if(index>0) {s1=s1 + zts_az + str.SubString(1,index) + "\r\n"; str=str.Delete(1,index);}
    else {err+="Ошибка = АЗсырой отсутствует!\r\n"; flag=1;}
    //парсим АЗкорр
    str = str.TrimLeft();//удаляем пробелы слева
  	index=str.Pos(" ");//ближайший пробел справа
    if(index>0) {s1=s1 + zts_azk + str.SubString(1,index) + "\r\n"; str=str.Delete(1,index);}
    else {err+="Ошибка = АЗкорр отсутствует!\r\n"; flag=1;}
    //парсим ОТКстат в никуда
    str = str.TrimLeft();//удаляем пробелы слева
  	index=str.Pos(" ");//ближайший пробел справа
    if(index>0) {str=str.Delete(1,index);}
    else {err+="Ошибка = ОТКстат отсутствует!\r\n"; flag=1;}
    //парсим Gx
    str = str.TrimLeft();//удаляем пробелы слева
  	index=str.Pos(" ");//ближайший пробел справа
    if(index>0) {s1=s1 + zts_Gx + str.SubString(1,index) + "\r\n"; str=str.Delete(1,index);}
    else {err+="Ошибка = Gx отсутствует!\r\n"; flag=1;}
    //парсим Gy
    str = str.TrimLeft();//удаляем пробелы слева
  	index=str.Pos(" ");//ближайший пробел справа
    if(index>0) {s1=s1 + zts_Gy + str.SubString(1,index) + "\r\n"; str=str.Delete(1,index);}
    else {err+="Ошибка = Gy отсутствует!\r\n"; flag=1;}
    //парсим Gz
    str = str.TrimLeft();//удаляем пробелы слева
  	index=str.Pos(" ");//ближайший пробел справа
    if(index>0) {s1=s1 + zts_Gz + str.SubString(1,index) + "\r\n"; str=str.Delete(1,index);}
    else {err+="Ошибка = Gz отсутствует!\r\n"; flag=1;}
    //парсим Hx
    str = str.TrimLeft();//удаляем пробелы слева
  	index=str.Pos(" ");//ближайший пробел справа
    try{
    if(index>0) {fl=MyStrToFloat(str.SubString(1,index)); str=str.Delete(1,index);}
    else {err+="Ошибка = Hx отсутствует!\r\n"; flag=1;}
    s1=s1 + zts_Hx + FloatToStrF0(fl*0.00001,7,5) + "\r\n";//Hx в Гауссах
    } catch(...){err+="Ошибка catch SendStaticToWits: Hx\r\n"; flag=1;}
    //парсим Hy
    str = str.TrimLeft();//удаляем пробелы слева
  	index=str.Pos(" ");//ближайший пробел справа
    try{
    if(index>0) {fl=MyStrToFloat(str.SubString(1,index)); str=str.Delete(1,index);}
    else {err+="Ошибка = Hy отсутствует!\r\n"; flag=1;}
    s1=s1 + zts_Hy + FloatToStrF0(fl*0.00001,7,5) + "\r\n";//Hy в Гауссах
    } catch(...){err+="Ошибка catch SendStaticToWits: Hy\r\n"; flag=1;}
    //парсим Hz
    str = str.TrimLeft();//удаляем пробелы слева
  	index=str.Pos(" ");//ближайший пробел справа
    try{
    if(index>0) {fl=MyStrToFloat(str.SubString(1,index)); str=str.Delete(1,index);}
    else {err+="Ошибка = Hz отсутствует!\r\n"; flag=1;}
    s1=s1 + zts_Hz + FloatToStrF0(fl*0.00001,7,5) + "\r\n";//Hz в Гауссах
    } catch(...){err+="Ошибка catch SendStaticToWits: Hz\r\n"; flag=1;}
    //Gtotal опорный
    try{
    s1=s1 + zts_Gt + FloatToStrF0(MyStrToFloat(Fr45->TablePasport->Cells[1][7]),7,6) + "\r\n";
    } catch(...){err+="Ошибка catch SendStaticToWits: Gtotal\r\n"; flag=1;}
    //Температура ЗТС
    try{
    s1=s1 + zts_temp + FloatToStrF0(tem,4,1) + "\r\n";
    } catch(...){err+="Ошибка catch SendStaticToWits: Temp\r\n"; flag=1;}

    //во 2-м режиме добавляем ЗУ НДМ
    if(FORMAT_KADRA==FORMAT_NDM_GK_RETR)
    {
     try{
     glub_sens=glub_bit - MyStrToFloat(Fr45->Sn->Cells[1][15]);//вычисляем глубину НДМ
     s1=s1 + ndm_depth + FloatToStrF0(glub_sens,6,2) + "\r\n";//Глубина по стволу датчика ЗУ НДМ
     s1=s1 + ndm_zu + zundm + "\r\n";//ЗУ НДМ
     } catch(...){err+="Ошибка catch SendStaticToWits: zundm+depth\r\n"; flag=1;}
    }
    s1=s1+"!!\r\n";//Конец
    strcpy(buf, s1.c_str());
    //WITS-сервер
    try{
    if(Fr45->ServerSocket1->Active==true && !flag) Fr45->ServerSocket1->Socket->Connections[0]->SendBuf(buf, strlen(s1.c_str()));
    } catch(...){err+="Ошибка catch SendStaticToWits: сервер wits\r\n"; flag=1;}
    //WITS-клиент
    try{
    if(Fr45->ClientSocket2->Active==true && !flag) Fr45->ClientSocket2->Socket->SendBuf(buf, strlen(s1.c_str()));
    } catch(...){err+="Ошибка catch SendStaticToWits: клиент wits\r\n"; flag=1;}

    if(flag) SaveWitsLog(err);
  	else SaveWitsLog(s1);
}
//========================================================================
void SendOtkToWits(AnsiString otk_g, AnsiString otk_h)
{
AnsiString s1="";
char buf[1000];
float glub_sens, glub_bit;
    if(WitsServer_ON==false && WitsClient_ON==false) return;
try{
    //в кадре 07
    glub_sens=MyStrToFloat(Fr45->Eglubina->Text) - MyStrToFloat(Fr45->Sn->Cells[1][13]);//глубина датчика ЗТС
    glub_bit=MyStrToFloat(Fr45->Eglubina->Text);
    //заголовок
    s1=CreateTopicWits("07",glub_sens,glub_bit);
    //параметры
    if(otk_g!="") s1=s1 + zts_otk + otk_g + "\r\n";//ОТК гравитационный
    if(otk_h!="") s1=s1 + zts_az_otk + otk_h+"\r\n";//ОТК магнитный
    s1=s1+"!!\r\n";//Конец

    strcpy(buf, s1.c_str());
    //WITS-сервер
    try{
    if(Fr45->ServerSocket1->Active==true) Fr45->ServerSocket1->Socket->Connections[0]->SendBuf(buf, strlen(s1.c_str()));
    } catch(...){s1+="Ошибка catch SendOtkToWits: сервер wits\r\n";}
    //WITS-клиент
    try{
    if(Fr45->ClientSocket2->Active==true) Fr45->ClientSocket2->Socket->SendBuf(buf, strlen(s1.c_str()));
    } catch(...){s1+="Ошибка catch SendOtkToWits: клиент wits\r\n";}
    //ShowMessage("Посылаем OTK в лог");
    SaveWitsLog(s1);
  } catch(...){SaveWitsLog("Ошибка catch SendOtkToWits()\r\n");}
}
//========================================================================
void SaveWitsLog(AnsiString s)
{
FILE *log;
AnsiString dt, file=CoreDir+"\\wits.log";//в корневой папке программы
 log=NULL;
 while((log=fopen(file.c_str(),"ab+"))==NULL) Application->ProcessMessages();//b=бинарный
 DateTimeToString(dt,"dd.mm.yy=hh:nn:ss ",Now());
 fprintf(log,"-----------------------------------------------------------\r\n");
 fprintf(log,"%s\r\n",dt);
 fprintf(log,"%s",s);
 fflush(log);
 fclose(log);
 //ShowMessage("Запись произведена");
}
//========================================================================
void LoadToMemoStatFromFile(AnsiString file)
{
	TStringList *list = new TStringList();//создаем список строк
    list->LoadFromFile(file);
    if(list->Count > 0)
    {   //удаляем строки мертвых концов из list
        while(list->Count>0 && list->Strings[0].Pos("Мертвый")!=0) list->Delete(0);//строки мертвых концов
        //удаляем заголовок из list
    	while(list->Count>0 && list->Strings[0].Pos("Глуб")!=0) list->Delete(0);//строки заголовка
    }
  	Fr45->MemoStat->Lines->Clear();
    Fr45->MemoStat->Lines->Add(ZagolForStatic);//первая строка - текущий заголовок
    Fr45->MemoStat->Lines->AddStrings(list);
	delete list;//удаляем временный список
}
//========================================================================
void LoadToMemoInkFromFile(AnsiString file)
{
    FILE *F=fopen(file.c_str(),"a");//если файла нет, то создаем новый
    fclose(F);//закрываем файл
    Fr45->MemoInc->Lines->Clear();//чистим
    Fr45->MemoInc->Lines->LoadFromFile(file);//загружаем в memo из файла
    if(Fr45->MemoInc->Lines->Count<1)//если было пусто
	{	Fr45->MemoInc->Lines->Add(ZagolForInk);//пишем заголовок в мемо
    	glubina_last=0;//последняя глубина
        Fr45->MemoInc->Lines->SaveToFile(file);//записали мемо в файл
	}
}
//========================================================================
//запись ошибок в лог-файл
void SaveStatusLog(AnsiString str, TColor color)
{
FILE *log;
AnsiString dt, status;
 
 //при color=0 - не выводить на экран, только писать в файл
 if(color != 0)
 {	//сначала выведем сообщение в StatusBar
 	Fr45->StatusBar->SimpleText = str;
 	Fr45->StatusBar->Color = color;
	//выставим значок Внимание! на верхнюю панель, если не белый
    if(color != clWhite)
    {if(Fr45->Label8->Caption=="") Fr45->Label8->Caption="( ! )";//если пусто, то ставим Внимание!
 	 Fr45->Label8->Color=color;
    }
 }

 //собираем строку и пишем в файл
 DateTimeToString(status,"dd.mm.yy=hh:nn:ss ",Now());
 if(color==clWhite || color==0) status += "[MESSAGE] ";
 else status += "[ERROR] ";
 status += str+"\n";
 log=NULL;
 while((log=fopen(Fstatus.c_str(),"a+"))==NULL) Application->ProcessMessages();//b=бинарный
 fprintf(log,"%s",status);
 fflush(log);
 fclose(log);
}
//========================================================================
//запись Настроек в лог файл
void SaveNastrLog(AnsiString File)
{
FILE *log;
AnsiString str, dt;
 log=NULL;
 while((log=fopen(File.c_str(),"a+b"))==NULL) Application->ProcessMessages();//b=бинарный
 str = getNastrString();
 fprintf(log, "%s", str);
 fflush(log);
 fclose(log);
}
//========================================================================
AnsiString getNastrString(void)
{
AnsiString str="", dt;
 //время
 str += "-----------------------------------------------------------\r\n";
 str += "Проводка "+ver+"\r\n";
 DateTimeToString(dt,"dd.mm.yy=hh:nn:ss ",Now());
 str += dt+"\r\n";
 str += "-----------------------------------------------------------\r\n";
 //порт УСО
 str += Fr45->Label3->Caption+": "+Fr45->Sport->Text+"\r\n";
 //НУБТ
 if(Fr45->Nubt->Checked) str += "НУБТ: +++\r\n";
 else str += "НУБТ: ---\r\n";
 //таблица
 for(int i=0; i<Fr45->Sn->RowCount; i++) {str += Fr45->Sn->Cells[0][i]+": "+Fr45->Sn->Cells[1][i]+"\r\n";}
 //частота
 str += "Частота: "+Fr45->R4->Items->Strings[Fr45->R4->ItemIndex]+"\n";
 //модули
 str += "Модули: ЗТС";
 if(Fr45->CBgkzts->Checked) str += "+ГКзтс";
 if(Fr45->CBndm->Checked) str += "+НДМ";
 if(Fr45->CBretr->Checked) str += "+Ретр";
 if(Fr45->CBnnk->Checked) str += "+ННК";
 if(Fr45->CBgeomash->Checked) str += "+Геомаш";
 str += "\r\n";
 //кодировка
 str += "Кодировка: "+Fr45->RGcoder->Items->Strings[Fr45->RGcoder->ItemIndex]+"\r\n";
 //Усо32
 if(Fr45->u32->Checked) str += "Усо32: +++\r\n";
 else str += "Усо32: ---\r\n";
 //WITS сервер
 if(Fr45->Sv2->Checked) str += "WITS сервер: +++\r\n";
 else str += "WITS сервер: ---\r\n";
 //WITS клиент
 if(Fr45->Cl2->Checked) str += "WITS клиент: +++, ("+Fr45->Cwits->Text+")\r\n";
 else str += "WITS клиент: ---\r\n";
 //REC сервер
 if(Fr45->CBLog->Checked) str += "REC сервер: +++\r\n";
 else str += "REC сервер: ---\r\n";
 //внешняя глубина
 if(Fr45->Sv->Checked)
 {  str += "Внешняя глубина:\r\n";
    str += "  Источник: ";
    if(Fr45->SourceGlub->ItemIndex==0) str += "Геосенсор="+Fr45->Czis->Text+"\r\n";
    else if(Fr45->SourceGlub->ItemIndex==1) str += "Леуза="+Fr45->Cleusa->Text+"\r\n";
    else if(Fr45->SourceGlub->ItemIndex==2) str += "ГТИ(wits)="+Fr45->Cgti->Text+"\r\n";
    else str += "\r\n";
 }
 else str += "Внешняя глубина: ---\r\n";
 //пульт бурильщика
 if(Fr45->PB->Checked) str += "Пульт бурильщика: порт="+Fr45->Sport2->Text+", "+Fr45->PBType->Items->Strings[Fr45->PBType->ItemIndex]+", "+Fr45->PBRadioGroup->Items->Strings[Fr45->PBRadioGroup->ItemIndex]+"\r\n";
 else str += "Пульт бурильщика: ---\r\n";
 //номер прибора
 str += "Номер прибора: "+Fr45->Cdat->Text+"\r\n";
 //имя телесистемы
 str += "Имя системы: "+Fr45->Cname->Text+"\r\n";
 //ID скважины
 str += "ID скважины: "+Fr45->Ckw->Text+"\r\n";
 //путь к скважине
 str += "Путь к скважине: "+tpathckw+"\r\n";
 //Версия Проводки
 str += "Версия Проводки: "+ver+"\r\n";
 return str;
}
//========================================================================
//возращает строку заголовка для ink
AnsiString getZagolInk(void)
{
AnsiString str="", par="";
int len=(sizeof(Zag_global)/sizeof(*Zag_global));
 for(int i=0;i<len;i++)
 {par = Zag_global[i][2];
  if(par.Pos("Дата/Время")==0) str += PadRight(par,DlinaParam);//одинарная длина
  else str += PadRight(par,DlinaParam*2);//дата, удвоенная длина
 }
 return str.Trim();
}
//========================================================================
//возращает строку заголовка для dat
AnsiString getZagolDat(void)
{
AnsiString str="";
/*str = PadRight("Дата",DlinaParam) + PadRight("Время",DlinaParam) + PadRight("Глуб.(м)",DlinaParam)
  + PadRight("ЗУзтс",DlinaParam) + PadRight("АЗсырой",DlinaParam) + PadRight("АЗкорр",DlinaParam)
  + PadRight("АЗист",DlinaParam) + PadRight("Htotal",DlinaParam) + PadRight("ОТКграв",DlinaParam)
  + PadRight("ОТКмагн",DlinaParam) + PadRight("Обороты",DlinaParam) + PadRight("Тзтс",DlinaParam)
  + PadRight("Напр.зтс",DlinaParam) + PadRight("Вибр.зтс",DlinaParam) + PadRight("ГКзтс",DlinaParam)
  + PadRight("ЗУндм",DlinaParam) + PadRight("ГКверх",DlinaParam) + PadRight("ГКниз",DlinaParam)
  + PadRight("Тндм",DlinaParam) + PadRight("ОБОРндм",DlinaParam) + PadRight("НАГРндм",DlinaParam)
  + PadRight("ТОКндм",DlinaParam) + PadRight("PEAKндм",DlinaParam) + PadRight("КСндм",DlinaParam)
  + PadRight("Индик",DlinaParam) + PadRight("dU",DlinaParam) + PadRight("МОМндм",DlinaParam)
  + PadRight("ДАВЛндм",DlinaParam) + PadRight("А*ч",DlinaParam) + PadRight("сигККС",DlinaParam)
  + PadRight("Gtotal",DlinaParam); */
str = PadRight("Параметр",DlinaParam*2)+PadRight("Дата",DlinaParam)+PadRight("Время",DlinaParam)+PadRight("Глубина",DlinaParam)
		+PadRight("Значение",DlinaParam);

 	return str.Trim();
}
//========================================================================
//добавляем строку всех углов в MemoUgol
void add_string_to_MemoUgol(void)
{
AnsiString s1="", s2;
   s2 = get_string_for_stat(Fr45->Eglubina->Text);
   if(ctat)
   {s1="*";
    if(get_ABACUS_rev() == 5) write_to_file_raw5(s2);//Rev-5
    if(get_ABACUS_rev() == 4) write_to_file_raw4(s2);//Rev-4
   }
   if(Fr45->MemoUgol->Lines->Count > 0) Fr45->MemoUgol->Lines->Strings[0]=s1+s2;
   else Fr45->MemoUgol->Lines->Add(s2);
}
//========================================================================
// Посылаем пакет в Геомаш через WITS сокет-клиент
//========================================================================
void SendToWitsSocket(uint8_t* buf, uint32_t len)
{
AnsiString s1="";
    if(WitsClient_ON==false) return;

    //WITS-клиент
    try{
    if(Fr45->ClientSocket2->Active==true) Fr45->ClientSocket2->Socket->SendBuf(buf, len);
    } catch(...){s1+="Ошибка catch SendToGeomash: клиент wits\r\n"; SaveWitsLog(s1);}
}
//========================================================================
/*void send_test(void)
{
extern GEOMASH_type GEOMASH;
        //if(percent==0)//если все принято корректно
        {	GEOMASH.Header = 0xFF;
        	GEOMASH.Version = 0x01;
            GEOMASH.Number = Count_Geomash;
            GEOMASH.Moment = getUnixTimeMs();
            //соединим 5-ки бит в байты
            int res = join_NBbit_arr(&dig[BEGIN_PLD_R31], DLINA_CODA_R31-1-BEGIN_PLD_R31, &GEOMASH.PayLoad[0]);
            if(res != sizeof(GEOMASH.PayLoad)) ShowMessage("Ошибка: кол-во векторов в Payload="+IntToStr(res));
            //перед посылкой в сокет свапим большие слова
            GEOMASH.Number = swap32(GEOMASH.Number);//Big endian
            GEOMASH.Moment = swapDouble(GEOMASH.Moment);//Big endian
            //вычислим CRC32
            GEOMASH.CRC32 = crc32_poly((uint8_t*)&GEOMASH.Header, sizeof(GEOMASH_type)-sizeof(GEOMASH.CRC32));
            GEOMASH.CRC32 = swap32(GEOMASH.CRC32);//Big endian
            //соберем строку для ЧЯ
            AnsiString str = "{\"Geomash_struct\":{\"Header\":"+IntToStr(GEOMASH.Header)+", \"Version\":"+IntToStr(GEOMASH.Version)+", ";
            str += "\"Number\":"+IntToStr(GEOMASH.Number)+", ";
            str += "\"Moment\":"+IntToStr(GEOMASH.Moment)+", ";
            str += "\"PayLoad\":[";
            for(int i=0;i<sizeof(GEOMASH.PayLoad);i++)
            {str += "0x" + IntToHex(GEOMASH.PayLoad[i], 2);
             if(i<(sizeof(GEOMASH.PayLoad)-1)) str += ",";
            }
            str += "], ";
            str += "\"CRC32\":0x" + IntToHex((int)GEOMASH.CRC32, 8);
            str += "}}";

			write_tabl_dat("String", str);//в черный ящик


    		//SendToWitsSocket(&GEOMASH.Header, sizeof(GEOMASH));//в Геомаш через WITS-клиент
        }
}*/
