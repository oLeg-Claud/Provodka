//---------------------------------------------------------------------------
//исправить INK
void __fastcall TFr45::PanelINKClick(TObject *Sender)
{
AnsiString s1;
int count_string;
    //если файл *.ink пустой, но выходим
    LoadToMemoInkFromFile(Fink);//загружаем в мемо из файла
    if(Fr45->MemoInc->Lines->Count<2) {ShowMessage("Файл "+Fink+" пустой!"); return;}
    //если глубина Внешняя, то останавливаем проводку кнопкой СТОП
    if(RadioGroup1->ItemIndex == 0)
    {	stop->Down=true;//нажимаем кнопку СТОП
    	stopClick(Sender);//вызываем событие
        Application->ProcessMessages();
    }
    //сначала загрузим значения глубин из мемо
    //вытащим первую глубину из строки, с 1-го символа до пробела
    s1=Fr45->MemoInc->Lines->Strings[1];//Строка сразу после заголовка
    EditBegin->Text = Trim(AnsiString(s1).SubString(1,AnsiString(s1).Pos(" ")-1));
    EditBegin2->Text = EditBegin->Text;
    //вытащим глубину из последней  строки, с 1-го символа до пробела
    s1=Fr45->MemoInc->Lines->Strings[Fr45->MemoInc->Lines->Count-1];
    EditEnd->Text = Trim(AnsiString(s1).SubString(1,AnsiString(s1).Pos(" ")-1));
    EditEnd2->Text = EditEnd->Text;
    EditLast->Text = EditEnd->Text;//последняя введенная глубина
	//сначала настраиваем панель и показываем ее
    RadioGroup2->ItemIndex=0;//в начале всегда Последняя глубина
    RadioGroup2Click(Sender);//вызовем событие выбора действия
    PanelEditInk->Top = 220;
    PanelEditInk->Left = 630;
    PanelEditInk->Visible = true;//показываем панель
}
//---------------------------------------------------------------------------
void __fastcall TFr45::RadioGroup2Click(TObject *Sender)
{
	if(RadioGroup2->ItemIndex==0)//Последняя глубина, остальное гасим
    {	Label29->Visible = true;
    	EditLast->Visible = true;
        Label27->Visible = false;
    	EditBegin->Visible = false;
        EditBegin2->Visible = false;
        Label28->Visible = false;
    	EditEnd->Visible = false;
    	EditEnd2->Visible = false;
    }
    else//во всех остальных случаях оставляем интервал только
    {	Label29->Visible = false;
    	EditLast->Visible = false;
        Label27->Visible = true;
    	EditBegin->Visible = true;
        EditBegin2->Visible = true;
        Label28->Visible = true;
    	EditEnd->Visible = true;
    	EditEnd2->Visible = true;
        //Удалить интервал
        if(RadioGroup2->ItemIndex==3) EditEnd2->Enabled = false;
        else EditEnd2->Enabled = true;
    }
}
//---------------------------------------------------------------------------
//Закрыть панель
void __fastcall TFr45::Image2Click(TObject *Sender)
{
	PanelEditInk->Visible = false;
}
//---------------------------------------------------------------------------
//Выполнить
void __fastcall TFr45::BitBtnRunClick(TObject *Sender)
{
float a,b,a1,b1;
AnsiString s1,s2;
    //Перед коррекцией сохраним бэкап-файл. В мемо уже сидят строки
    Fr45->MemoInc->Lines->SaveToFile(tpathckw+"\\"+ckwName+".bkp");//записали в бэкап файл

    //Последняя глубина
	if(RadioGroup2->ItemIndex==0)
    {   try {a=MyStrToFloat(EditLast->Text);}//новая глубина
        catch(...){ShowMessage("Ошибка в последней глубине!"); goto exit;}
        //если строка с глубиной не единственная, то сравниваем новую глубину с предыдущей
        if(Fr45->MemoInc->Lines->Count>2)
        {   s1 = Fr45->MemoInc->Lines->Strings[Fr45->MemoInc->Lines->Count-2];
            s1 = Trim(AnsiString(s1).SubString(1,AnsiString(s1).Pos(" ")-1));
            b = MyStrToFloat(s1);//предыдущая глубина
            if(b>=a) {ShowMessage("Введена некорректная последняя глубина!"); goto exit;}
            s1 = Fr45->MemoInc->Lines->Strings[Fr45->MemoInc->Lines->Count-1];
            s1 = Trim(AnsiString(s1).SubString(1,AnsiString(s1).Pos(" ")-1));
            b = MyStrToFloat(s1);//последняя глубина в файле
            if(b==a) {ShowMessage("Вы не исправили последнюю глубину, все остается как прежде!"); goto exit;}
        }
        //меняем последнюю глубину, если все ок
        s1 = Fr45->MemoInc->Lines->Strings[Fr45->MemoInc->Lines->Count-1];
        s1 = Trim(AnsiString(s1).SubString(1,AnsiString(s1).Pos(" ")-1));//число глубины
        s2 = Fr45->MemoInc->Lines->Strings[Fr45->MemoInc->Lines->Count-1];
        s2 = StringReplace(s2,s1,"",TReplaceFlags()<<rfIgnoreCase);//удаляем число глубины из строки
        s2 = s2.TrimLeft();//удаляем пробелы слева, осталась строка без глубины
        s1 = PadRight(FloatToStrF0(a,7,2),DlinaParam)+s2;//собираем строку
        Fr45->MemoInc->Lines->Strings[Fr45->MemoInc->Lines->Count-1] = s1;//заменили строку
        Fr45->MemoInc->Lines->SaveToFile(Fink);//записали в файл INK
    }
    //Сжать и растянуть интервал = функционал одинаковый
    else if(RadioGroup2->ItemIndex==1 || RadioGroup2->ItemIndex==2)
    {   a=roundf(MyStrToFloat(Fr45->EditBegin->Text),2);//старое начало
        b=roundf(MyStrToFloat(Fr45->EditEnd->Text),2);//старый конец
        try{a1=roundf(MyStrToFloat(Fr45->EditBegin2->Text),2);} catch(...){ShowMessage("Некорректная начальная глубина!"); goto exit;}
        try{b1=roundf(MyStrToFloat(Fr45->EditEnd2->Text),2);} catch(...){ShowMessage("Некорректная конечная глубина!"); goto exit;}
        if(a1<a) {ShowMessage("Начало интервала меньше допустимого!"); goto exit;}
        if(RadioGroup2->ItemIndex==1 && b1>b) {ShowMessage("При сжатии конец интервала больше допустимого!"); goto exit;}
        if(RadioGroup2->ItemIndex==2 && b1<b) {ShowMessage("При растяжении конец интервала меньше допустимого!"); goto exit;}
        if(b1==b) {goto exit;}//сжатие-растяжение ненужно
        if(a1==a && b1==b) {goto exit;}//сжатие-растяжение ненужно
        if(a==b || a1==b1) {ShowMessage("Ошибка! Интервал равен нулю!"); goto exit;}
        //подтверждение
        if(RadioGroup2->ItemIndex==1) s1="Вы уверены, что хотите поднять забой до ";
        if(RadioGroup2->ItemIndex==2) s1="Вы уверены, что хотите опустить забой до ";
        s1+=FloatToStrF0(b1,7,2)+"м";
        if(Application->MessageBox(s1.c_str(), "Внимание!", MB_YESNO) == IDYES) {}
        else goto exit;
        //если интервал правильно определен, то вычислим уравнение прямой y(x)=k*x+c
        float x1=a1, y1=a1, x2=b, y2=b1, k, c, d;
        k = (y2-y1)/(x2-x1);
        c = y2-k*x2;
        //теперь пройдемся по строкам снизу вверх и заменим глубины, которые лежат в интервале a1-b
        int count=Fr45->MemoInc->Lines->Count-1;//индекс последней строки в мемо
        while(count>0)
        {	s1 = Fr45->MemoInc->Lines->Strings[count];
            s1 = Trim(AnsiString(s1).SubString(1,AnsiString(s1).Pos(" ")-1));//символы глубины
            d = roundf(MyStrToFloat(s1),2);//глубина из строки
            if((d-a1)>-0.0001)//если в интервале (ниже начала по глубине и до конца)
            {	s2 = Fr45->MemoInc->Lines->Strings[count];
        		s2 = StringReplace(s2,s1,"",TReplaceFlags()<<rfIgnoreCase);//удаляем число глубины из строки
        		s2 = s2.TrimLeft();//удаляем пробелы слева, осталась строка без глубины
                d = k*d+c;//вычисляем новую глубину
                Fr45->MemoInc->Lines->Strings[count] = PadRight(FloatToStrF0(d,7,2),DlinaParam)+s2;//собрали новую строку
            }
            else break;//выходим
			count--;
        }
        //запишем полученный мемо в файл INK
        Fr45->MemoInc->Lines->SaveToFile(Fink);
        Application->ProcessMessages();
        //перезагрузим массивы и переменные через вызов события
        CkwChange(Sender);
    }
    //Удалить интервал
    else if(RadioGroup2->ItemIndex==3)
    {	a=roundf(MyStrToFloat(Fr45->EditBegin->Text),2);//старое начало
        b=roundf(MyStrToFloat(Fr45->EditEnd->Text),2);//старый конец
        try{a1=roundf(MyStrToFloat(Fr45->EditBegin2->Text),2);} catch(...){ShowMessage("Некорректная начальная глубина!"); goto exit;}
        try{b1=roundf(MyStrToFloat(Fr45->EditEnd2->Text),2);} catch(...){ShowMessage("Некорректная конечная глубина!"); goto exit;}
        if(a1<a) {ShowMessage("Начало интервала меньше допустимого!"); goto exit;}
        if(b1!=b) {ShowMessage("Конец интервала ошибочен!"); goto exit;}
        //подтверждение
        s1="Вы уверены в том, что делаете?";
        if(Application->MessageBox(s1.c_str(), "Внимание!", MB_YESNO) == IDYES) {}
        else goto exit;
        //теперь пройдемся по строкам снизу вверх и удалим строки, которые лежат в интервале a1-b1
        while(Fr45->MemoInc->Lines->Count>1)//до заголовка
        {	s1 = Fr45->MemoInc->Lines->Strings[Fr45->MemoInc->Lines->Count-1];//последняя строка
            s1 = Trim(AnsiString(s1).SubString(1,AnsiString(s1).Pos(" ")-1));//символы глубины
            float d = roundf(MyStrToFloat(s1),2);//глубина из строки
            if((d-a1)>-0.0001)//если в интервале (ниже начала по глубине и до конца)
            {Fr45->MemoInc->Lines->Delete(Fr45->MemoInc->Lines->Count-1);}//удаляем последнюю строку
            else break;
        }
        //запишем полученный мемо в файл INK
        Fr45->MemoInc->Lines->SaveToFile(Fink);
        Application->ProcessMessages();
        //перезагрузим массивы и переменные через вызов события
        CkwChange(Sender);
    }
exit:
    PanelEditInk->Visible = false;//закрываем панель
}
//---------------------------------------------------------------------------

 