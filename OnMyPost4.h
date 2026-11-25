// Сюда присылает сообшения поток 1 - NGT и MGT
void __fastcall TFr45::OnMyPost4(TMessage& a)
{
//тут помимо прочего происходит запись в файл черного ящика

     int num, indk, indl, indt;
    //получить новую цифру и подготовить ожидание следующей
    num = a.WParam;//номер вектора
    indk = a.LParam-iG;//индекс конца кода в массиве
    //StatusBar3->SimpleText="post4 NGT="+IntToStr(num)+", MGT="+IntToStr(indk);
    indl = indk-DLDIGr;//DLDIGr=Длина вектора данных в тиках УСО, indl=начало кода в массиве
    if(indl<0) indl+=2*DLKADRrp;//
    indt = indl-iG;//индекс начала кода в массиве со смещением
    if(indt<0)indt += 2*DLKADRrp;//DLKADRrp=Длина всего кадра в тиках УСО
    //Memo1->Lines->Add("post4 NGT="+IntToStr(num)+", MGT="+IntToStr(a.LParam)+", indt="+IntToStr(indt));

    //выдергиваем цифру длиной DLDIGr с краями из masc[] в mas1[] = это массив одной цифры
    //если индекс конца отрезка больше индекса начала, то напрямую
    if(indk+iG>indt) for(int i=0;i<DLDIGr+2*iG;i++) mas1[i]=masc[i+indt];
    //если индекс конца больше индекса начала, то с переходом через ноль
    if(indk+iG<indt)
    {
        int j=indt;
        for(int i=0;i<2*iG+DLDIGr;i++)
        {
         if(j==2*DLKADRrp) j=0;
         mas1[i]=masc[j];
         j=j+1;
        }
    }

    Sleep((unsigned long)2);//нафига тут слип?
    //Application->ProcessMessages();
    
    //Сообщение потоку 3--->там происходит корреляция одной цифры из массива mas1[]
    SendMessage(Fr45->Handle,WM_MyPost3,num,iG);

    //готовим следующий номер
    if(num<L-1)
    {
     NGT=num+1;
     MGT=indl+2*DLDIGr+iG+sdvig;
     fl_buf=false;
     if(MGT>=2*DLKADRrp) {MGT-=2*DLKADRrp; fl_buf=true;}
    }
    //Memo1->Lines->Add("post4 indl="+IntToStr(indl)+", DLDIGr="+IntToStr(DLDIGr)+", iG="+IntToStr(iG)+", sdvig="+IntToStr(sdvig)+", MGT="+IntToStr(MGT));
//-----------------------большой If до конца-----------------------------------
    if(num>=L-1)//Это последний код
    {
     //ShowMessage("Последний код="+IntToStr(l));
     C2->Title->Text->Strings[1]=C2->Title->Text->Strings[1]+")";
     MCP=indk+DLMASr+DELTAr;
     MCP=MCP;
     fl_buf=false;
     if(MCP>=2*DLKADRrp) {MCP=MCP-2*DLKADRrp;  fl_buf=true;}

     t0=t0+3;
//-----------------запись в файл (черный ящик)
    AnsiString t;
    DateTimeToString(t,"dd.mm.yy",Date());
    AnsiString dt=TimeToStr(Now());
    strcpy(Sdat.D,t.c_str());//заносим дату
    strcpy(Sdat.T,dt.c_str());//время
    Sdat.R[0]=FRE[freq];//частота
    Sdat.R[1]=MyStrToFloat(Eglubina->Text);//глубина
    AnsiString ddd="AMD";
    if(Caz_otk->Checked)ddd=ddd+", зарезка";
    strcpy(Sdat.R_MO,ddd.c_str());

    Sdat.SS[0]=MyStrToFloat(Sn->Cells[1][0]);//угол МПиТ
    Sdat.SS[1]=MyStrToFloat(Sn->Cells[1][1]);//АЗориент
    Sdat.SS[2]=MyStrToFloat(Sn->Cells[1][2]);//ЗУориент
    Sdat.SS[3]=get_umn();//УМН
    Sdat.SS[4]=MyStrToFloat(Sn->Cells[1][5]);//Hop
    Sdat.SS[6]=MyStrToFloat(Sn->Cells[1][8]);//НульОТК
    Sdat.O[4]=DCPt;//проценты корреляции
    Sdat.F=levelsig;   //мВ сигнала;
    //if(newr==0)//кадр УГЛЫ
     {
      Sdat.R[2]=zenit;//ЗУ
      Sdat.R[3]=azimut;//АЗсырой
      Sdat.R[4]=Azk;//АЗкорр
      Sdat.R[5]=(int)Htotal;//модуль H
      //ОТКстат
      int otk333;
      if((otklonf-(int)otklonf)>0.5) otk333=(otklonf+0.5); else otk333=otklonf;
      Sdat.O[0]=otk333;//ОТКстат
      Sdat.O[1]=dat_o1; Sdat.O[2]=dat_o2; Sdat.O[3]=dat_o3;//остальные поля ОТК
      //АЗ-ОТК
      Sdat.O2[0]=dat_oa1; Sdat.O2[1]=dat_oa2;Sdat.O2[2]=dat_oa3;   Sdat.O2[3]=dat_oa4;
      //Магнитное поле
      Sdat.HH[0]=HX;
      Sdat.HH[1]=HY;
      Sdat.HH[2]=HZ;
     }

    //Служебные параметры
    strcpy(Sdat.DOP,"-9999");
    if(iot==0)strcpy(Sdat.DOP,(IntToStr(obor)+" o").c_str());//обороты
    if(iot==1)strcpy(Sdat.DOP,(IntToStr(tem)+" t").c_str());//температура
    if(iot==2&&prib>=2)strcpy(Sdat.DOP,(FloatToStrF0(NAPR,4,1)+" v").c_str());//напряжение
    if(iot==3)strcpy(Sdat.DOP,(IntToStr(vib)+" g").c_str());//вибрация
    if(iot==5)strcpy(Sdat.DOP,(IntToStr(wr)+" ч").c_str());//время
    if(iot==4)Sdat.SS[7]=gk; else Sdat.SS[7]=NULL;//ГК
    if(iot==6)strcpy(Sdat.DOP,(FloatToStrF0(modg,8,4)+" m").c_str());//модуль G

    Sdat.F=levelsig;//уровень сигнала
    strcpy(Sdat.TAR,(tar+"("+IntToStr(nulotk)+") "+ver).c_str());//имя прибора
    strcpy(Sdat.RRR,"углы");

    //  запись leusa
    AnsiString SSS=TITLE+ver+ver2+"/AMD/",SSS1="",C="/";
    SSS=SSS+ckwName+C+Sdat.TAR+C+Sport->Text+C+IntToStr(1)+C;
    SSS=SSS+t+C+dt+C+IntToStr(R4->ItemIndex)+C+ IntToStr(Cuss->ItemIndex)+C;
    for (int i=0;i<8;i++)SSS=SSS+Sn->Cells[1][0]+C ;
    SSS=SSS+IntToStr(RadioGroup1->ItemIndex)+C+IntToStr(Caz_otk->Checked)+C+Czis->Text+C+Eglubina->Text+C;
    for (int i=2;i<6;i++)SSS1=SSS1+FloatToStrF0(Sdat.R[i],8,2)+C;
    for (int i=0;i<5;i++)SSS1=SSS1+IntToStr(Sdat.O[i])+C;
    SSS1=SSS1+FloatToStrF0(gk,5,1)+C;
    for (int i=0;i<4;i++)SSS1=SSS1+IntToStr(Sdat.O2[i])+C;
    for (int i=0;i<3;i++)SSS1=SSS1+IntToStr(Sdat.HH[i])+C;
    SSS1=SSS1+ IntToStr(obor)+C+IntToStr(tem)+C+IntToStr(vib)+C+IntToStr(wr)+C+FloatToStrF0(NAPR,4,1)+C;
    SSS1=SSS1+SPIK+C+FloatToStrF0(mgk,8,2)+C+FloatToStrF0(modg,8,4)+C;
    PutLeuza(SSS+SSS1+"конец");

 //-------------------------осреднение--------------
    if( DCPt>=DCP)
    {
        if(CompareText(Sn->Cells[1][1],"")!=0) azorien=StrToInt(Sn->Cells[1][1]);//ожидаемый Азимут
        else azorien=0;
        zuorien=MyStrToFloat(Sn->Cells[1][2]);//ожидаемый ЗУ
        L1a->Cells[0][1]=" - ";//не усредняем эту колонку
		Lakopp->Cells[0][1]=" - ";//не усредняем эту колонку
        L1->Cells[0][1]=" - ";//не усредняем эту колонку

        //проверяем выход за ожидаемые пределы
        RedC=0;//красним в настройках 1=азимут ориентировочно; 2=зенит ориентировочно; 3=оба
        if(ctat)//только в Статике
        {   //проверяем Азимут и ЗУ
        	float deltaZU, deltaAZ;
        	deltaZU=fabs(MyStrToFloat(L1->Cells[0][2])-zuorien);
        	deltaAZ=fabs(MyStrToFloat(Lakopp->Cells[0][2])-azorien);
        	if((deltaAZ > AZVOROTA) || (deltaZU > ZUVOROTA))
        	{	SaveStatusLog("Проверить ожидаемые значения", clRed);
         		PlaySound("MYS2",0,SND_RESOURCE);
         		if(deltaAZ > AZVOROTA) RedC=1;
         		if(deltaZU > ZUVOROTA) if(RedC==0) RedC=2; else RedC=3;
        	}
            //проверяем НУБТ по Зениту
            //если ЗУ > уровня и НУБТ не включен
            if(MyStrToFloat(L1->Cells[0][2]) > level_NUBT && Nubt->Checked==false)
            {	SaveStatusLog("Не включена коррекция НУБТ при ЗУ > "+FloatToStrF0(level_NUBT,4,1), clRed);
            	PlaySound("MYS2",0,SND_RESOURCE);
            }
            //если ЗУ < уровня и НУБТ включен
            if(MyStrToFloat(L1->Cells[0][2]) < level_NUBT && Nubt->Checked==true)
            {	SaveStatusLog("Включена коррекция НУБТ при ЗУ < "+FloatToStrF0(level_NUBT,4,1), clRed);
            	PlaySound("MYS2",0,SND_RESOURCE);
            }
        }
        Sn->Cells[1][1]=Sn->Cells[1][1];
        Sn->Cells[1][2]=Sn->Cells[1][2];

    }//конец if( DCPt>=DCP)
  }//конец if(l==L-1)
}  

 
