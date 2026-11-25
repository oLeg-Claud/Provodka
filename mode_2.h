//========= Вывод цифирей в режиме FORMAT_KADRA=2 =====================================================
if((FORMAT_KADRA==FORMAT_NDM_GK_RETR)&&(DCPt>=DCP))
{
   if(num_kod==FIRST_KOD_R2)
   {
    ClientSocket1->Socket->SendText("параметр09="+Eglubina->Text+" ");
    ClientSocket1->Socket->SendText("freg="+R4->Items->Strings[R4->ItemIndex]+" ");
   }
  //---------- зенит -------------------------------
  if( (num_kod==AZ_OTK7_H_R2) && (data_perc[AZ_OTK7_H_R2]>=DCP) && (data_perc[ZU_H_R2]>=DCP) && (data_perc[ZU_L_R2]>=DCP) )
  {L1->Cells[0][2]=FloatToStrF0(zenit,6,2);
   ClientSocket1->Socket->SendText("параметр01="+FloatToStrF0(zenit,6,2)+" ");
   write_tabl_dat("ЗУ_зтс", FloatToStrF0(zenit,6,2));//в черный ящик
   //if(SockNumber>0) SendToWits(zts_zu, FloatToStrF0(zenit,6,2));//Отправляем в Wits, если подключен
   //if(PB->Checked) SendOneQpb(wZU,zenit);//отправляем в Пульт Бурильщика, если выбран
   int j=0; Lz->Items->Clear();
   for(int i=0;i<L1->RowCount-2;i++) if(CompareText(L1->Cells[0][i+2],"")!=0) {Lz->Items->Strings[j]=L1->Cells[0][i+2];j=j+1;}
  }
  //---------------- азимут --------------------------
  if( //если проценты данных а порядке
     (num_kod==AZ_OTK7_H_R2)
     && (data_perc[HX_H_R2]>=DCP) && (data_perc[HX_M_R2]>=DCP) && (data_perc[HX_L_R2]>=DCP)
     && (data_perc[HY_H_R2]>=DCP) && (data_perc[GK3_H_R2]>=DCP) && (data_perc[GK3up_H_R2]>=DCP)
     && (data_perc[AZ_OTK3_H_R2]>=DCP) && (data_perc[GK3dn_H_R2]>=DCP) && (data_perc[HZ_H_R2]>=DCP) && (data_perc[HZ_L_R2]>=DCP)
     && (data_perc[ZUndm_H_R2]>=DCP)
    )
  {
   //сохраним признак качества
   if(!ctat)
   {if(!QUAL_STAT) masQuality[2]=0;//хорошо в Динамике
    else masQuality[2]=1;//плохо в Динамике
   }
   else
   {if(!QUAL_STAT) masQuality[2]=2;//хорошо в Статике
    else masQuality[2]=3;//плохо в Статике
   }
   ClientSocket1->Socket->SendText("параметр02="+FloatToStrF0(azimut,6,1)+" ");
   write_tabl_dat("Качество", IntToStr(QUAL_STAT));//в черный ящик
   write_tabl_dat("АЗ_зтс", FloatToStrF0(azimut,6,1));//в черный ящик
   //if(SockNumber>0) SendToWits(zts_az, FloatToStrF0(azimut,6,1));//Отправляем в Леузу, если подключен
   L1a->Cells[0][2]=FloatToStrF0(azimut,6,1);//выводим в ячейку
   L1->Cells[0][2]=L1->Cells[0][2];//перерисуем зенит с новым значением качества

   //Азимут корректированный в таблицу.
   if(MODE_AZ==0) Lakopp->Cells[0][2]=FloatToStrF0(Azk,6,1);//магнитный
   else Lakopp->Cells[0][2]=AZist_str;//истинный
   
   ClientSocket1->Socket->SendText("параметр16="+FloatToStrF0(Azk,6,1)+" ");
   write_tabl_dat("АЗ_корр", FloatToStrF0(Azk,6,1));//в черный ящик
   write_tabl_dat("Hx", IntToStr(HX));//в черный ящик
   write_tabl_dat("Hy", IntToStr(HY));//в черный ящик
   write_tabl_dat("Hz", IntToStr(HZ));//в черный ящик
   write_tabl_dat("Htotal", IntToStr(Htotal));//в черный ящик
   //if(SockNumber>0) SendToWits(zts_azk, FloatToStrF0(Azk,6,1));//Отправляем в Леузу, если подключен
   //if(PB->Checked) SendOneQpb(wAZ,Azk);//отправляем в Пульт Бурильщика, если выбран
   
   float rab=(float) Htotal/Hop;
   Lh->Cells[0][2]=IntToStr(Htotal)+"("+FloatToStrF(rab,ffFixed,4,2)+")";//Текущий модуль Н
   j=0; Lhh->Items->Clear();
   for(int i=0;i<Lh->RowCount-2;i++) if(CompareText(Lh->Cells[0][i+2],"")!=0)
                                     {Lhh->Items->Strings[j]=Lh->Cells[0][i+2];j=j+1;}
   j=0; La->Items->Clear();
   for(int i=0;i<L1a->RowCount-2;i++) if(CompareText(L1a->Cells[0][i+2],"")!=0)
                                      {La->Items->Strings[j]=L1a->Cells[0][i+2]; j=j+1;}
   j=0; Lkopp->Items->Clear();
   for(int i=0;i<Lakopp->RowCount-2;i++) if(CompareText(Lakopp->Cells[0][i+2],"")!=0)
                                      {Lkopp->Items->Strings[j]=Lakopp->Cells[0][i+2];j=j+1;}
   //добавляем строку всех углов в MemoUgol
   add_string_to_MemoUgol();
  }

  //------------- Номер режима и Роль прибора ----------------------------
   if((num_kod==GK2up_H_R2) && (data_perc[OTK2_H_R2]>=DCP) && (data_perc[GK2_H_R2]>=DCP) && (data_perc[GK2up_H_R2]>=DCP))
   {
    Spar->Cells[12][2]=IntToStr(num_mode) + RoleStr;//выводим в колонку Режим и Роль
    write_tabl_dat("Режим", Spar->Cells[12][2]);//в черный ящик
    if(num_mode != FORMAT_KADRA)//Если режимы не соответствуют друг другу
    {
     SaveStatusLog("Режим работы скважинного прибора("+IntToStr(num_mode)+")"+" не соответствует настройкам программы("+IntToStr(FORMAT_KADRA)+")", clRed);
     PlaySound("MYS2",0,SND_RESOURCE);
    }
   }

   //------------ Отклонители OTK --------------------------------------
   if(
      (  ((num_kod==OTK1_L_R2)&&(data_perc[OTK1_H_R2]>=DCP)&&(data_perc[OTK1_L_R2]>=DCP))
       ||((num_kod==OTK2_L_R2)&&(data_perc[OTK2_H_R2]>=DCP)&&(data_perc[OTK2_L_R2]>=DCP))
       ||((num_kod==OTK3_L_R2)&&(data_perc[OTK3_H_R2]>=DCP)&&(data_perc[OTK3_L_R2]>=DCP))
       ||((num_kod==OTK4_L_R2)&&(data_perc[OTK4_H_R2]>=DCP)&&(data_perc[OTK4_L_R2]>=DCP))
       ||((num_kod==OTK5_L_R2)&&(data_perc[OTK5_H_R2]>=DCP)&&(data_perc[OTK5_L_R2]>=DCP))
       ||((num_kod==OTK6_L_R2)&&(data_perc[OTK6_H_R2]>=DCP)&&(data_perc[OTK6_L_R2]>=DCP))
       ||((num_kod==OTK7_L_R2)&&(data_perc[OTK7_H_R2]>=DCP)&&(data_perc[OTK7_L_R2]>=DCP))
      )
     )//7 отклонителя
   {
    int dat_o;
    if(num_kod==OTK1_L_R2) dat_o=dat_o1;
    if(num_kod==OTK2_L_R2) dat_o=dat_o2;
    if(num_kod==OTK3_L_R2) dat_o=dat_o3;
    if(num_kod==OTK4_L_R2) dat_o=dat_o4;
    if(num_kod==OTK5_L_R2) dat_o=dat_o5;
    if(num_kod==OTK6_L_R2) dat_o=dat_o6;
    if(num_kod==OTK7_L_R2) dat_o=dat_o7;
    if(dat_o!=400)//коррекция ОТК на метки турбобура и пр
    {
     dat_o-=nulotk;
     if(CompareText(Sn->Cells[1][0],"")!=0) mrmt=StrToInt(Sn->Cells[1][0]);
     else mrmt=0;
     dat_o+=mrmt;
     while(dat_o>=360)dat_o-=360;
     while(dat_o<0)dat_o+=360;
    }
    //Вывод ОТК грав
    ClientSocket1->Socket->SendText("параметр03="+FloatToStrF0(dat_o,6,1)+" ");
    write_tabl_dat("ОТК_грав", FloatToStrF0(dat_o,6,1));//в черный ящик
    //SendToWits(zts_otk, FloatToStrF0(dat_o,6,1));//Отправляем в WITS, если подключен
    for(int i=L0->RowCount-2;i>=2;i--) for(int j=0;j<L0->ColCount;j++) L0->Cells[j][i+1]=L0->Cells[j][i];
    L0->Cells[0][2]="";
    L0->Cells[0][2]=IntToStr(dat_o);//Выводим ОТК в колонку
    if(num_kod==OTK1_L_R2){dat_o1=dat_o;}
    if(num_kod==OTK2_L_R2){dat_o2=dat_o;}
    if(num_kod==OTK3_L_R2){dat_o3=dat_o;}
    if(num_kod==OTK4_L_R2){dat_o4=dat_o;}
    if(num_kod==OTK5_L_R2){dat_o5=dat_o;}
    if(num_kod==OTK6_L_R2){dat_o6=dat_o;}
    if(num_kod==OTK7_L_R2){dat_o7=dat_o;}
    otktek=dat_o;//текущий общего пользования

    //отправляем в Пульт Бурильщика, если выбран
    if(PB->Checked && PBType->ItemIndex==0 && PBRadioGroup->ItemIndex==0)//если наш ПБ
    {SendOneQpb(wOTK,dat_o);//в круг ОТК
     SendOneQpb(wAZ,dat_o);//дублинуем цифрами в АЗ
    }
    if(PB->Checked && PBType->ItemIndex==1 && PBRadioGroup->ItemIndex==0)//если не наш ПБ
    {   SendByteQpb_rfd(SET_OTK_MODE,OTK_MODE_GRAV);//режим ОТК
        SendOneQpb_rfd(SET_OTK,(float)dat_o);//в круг ОТК
    }
    DrawOTK1(dat_o);//рисуем ОТК1
   }  //Конец 4 отклонителя

   //------------------- АЗ-ОТК -----------------------------------------
   if(    ((num_kod==AZ_OTK1_L_R2)&&(data_perc[AZ_OTK1_H_R2]>=DCP)&&(data_perc[AZ_OTK1_L_R2]>=DCP))
        ||((num_kod==AZ_OTK2_L_R2)&&(data_perc[AZ_OTK2_H_R2]>=DCP)&&(data_perc[AZ_OTK2_L_R2]>=DCP))
        ||((num_kod==AZ_OTK3_L_R2)&&(data_perc[AZ_OTK3_H_R2]>=DCP)&&(data_perc[AZ_OTK3_L_R2]>=DCP))
        ||((num_kod==AZ_OTK4_L_R2)&&(data_perc[AZ_OTK4_H_R2]>=DCP)&&(data_perc[AZ_OTK4_L_R2]>=DCP))
        ||((num_kod==AZ_OTK5_L_R2)&&(data_perc[AZ_OTK5_H_R2]>=DCP)&&(data_perc[AZ_OTK5_L_R2]>=DCP))
        ||((num_kod==AZ_OTK6_L_R2)&&(data_perc[AZ_OTK6_H_R2]>=DCP)&&(data_perc[AZ_OTK6_L_R2]>=DCP))
        ||((num_kod==AZ_OTK7_L_R2)&&(data_perc[AZ_OTK7_H_R2]>=DCP)&&(data_perc[AZ_OTK7_L_R2]>=DCP))
     )
   {
    if(num_kod==AZ_OTK1_L_R2) az_otk=dat_oa1;
    if(num_kod==AZ_OTK2_L_R2) az_otk=dat_oa2;
    if(num_kod==AZ_OTK3_L_R2) az_otk=dat_oa3;
    if(num_kod==AZ_OTK4_L_R2) az_otk=dat_oa4;
    if(num_kod==AZ_OTK5_L_R2) az_otk=dat_oa5;
    if(num_kod==AZ_OTK6_L_R2) az_otk=dat_oa6;
    if(num_kod==AZ_OTK7_L_R2) az_otk=dat_oa7;
    //если не Зарезка, а ОТК2
    if(!Caz_otk->Checked && az_otk!=400)
    {
     float otk2;
     float paz=MyStrToFloat(Fr45->Sn->Cells[1][1]);
     float mnr=mn*pi/180.,ZUtr=zu*pi/180.,AZtr=paz*pi/180.;
     float Mx=-BASEACC*cos(mnr*sin(AZtr))*sin(ZUtr-mnr*cos(AZtr));
     float My=BASEACC*sin(mnr*sin(AZtr));
     otk2=az_otk+ rotor(-My,Mx);
     az_otk=inta(otk2+360)%360;
    }

    //если Зарезка и режим Истинного Азимута
    if(Caz_otk->Checked && az_otk!=400 && MODE_AZ>0) az_otk=inta(get_ums()+az_otk)%360;

    //применим смещения меток
    if(az_otk!=400)
    {
     az_otk-=nulotk;
     az_otk+=mrmt;
     az_otk=inta(az_otk+360)%360;
    }
    if(num_kod==AZ_OTK1_L_R2) dat_oa1=az_otk;
    if(num_kod==AZ_OTK2_L_R2) dat_oa2=az_otk;
    if(num_kod==AZ_OTK3_L_R2) dat_oa3=az_otk;
    if(num_kod==AZ_OTK4_L_R2) dat_oa4=az_otk;
    if(num_kod==AZ_OTK5_L_R2) dat_oa5=az_otk;
    if(num_kod==AZ_OTK6_L_R2) dat_oa6=az_otk;
    if(num_kod==AZ_OTK7_L_R2) dat_oa7=az_otk;

    for(int i=Lo2->RowCount-2;i>=2;i--) for(int j=0;j<Lo2->ColCount;j++) Lo2->Cells[j][i+1]=Lo2->Cells[j][i];
    Lo2->Cells[0][2]="";
    az_otk=inta(az_otk);
    Lo2->Cells[0][2]=IntToStr(inta(az_otk));//выводим в колонку
    ClientSocket1->Socket->SendText("параметр10="+ FloatToStrF0(az_otk,6,1)+" ");//В Геосенсор AZ_OTK
    write_tabl_dat("ОТК_магн", FloatToStrF0(az_otk,6,1));//в черный ящик
    SendOtkToWits(L0->Cells[0][2], Lo2->Cells[0][2]);//Отправляем в WITS оба OTK
    //отправляем в Пульт Бурильщика, если выбран
    if(PB->Checked && PBType->ItemIndex==0 && PBRadioGroup->ItemIndex==1)//если наш ПБ
    {SendOneQpb(wOTK,az_otk);//в круг ОТК
     SendOneQpb(wAZ,az_otk);//дублинуем цифрами в АЗ
    }
    if(PB->Checked && PBType->ItemIndex==1 && PBRadioGroup->ItemIndex==1)//если не наш ПБ
    {   SendByteQpb_rfd(SET_OTK_MODE,OTK_MODE_MAG);//режим ОТК
        SendOneQpb_rfd(SET_OTK,(float)az_otk);//в круг ОТК
    }
    DrawOTK2(az_otk);//рисуем ОТК2
   }//Конец АЗ-ОТК

   //------------------- Вибрация -----------------------------------------
   if((num_kod==OTK3_H_R2)&&(data_perc[AZ_OTK2_H_R2]>=DCP)&&(data_perc[GK2dn_H_R2]>=DCP)&&(data_perc[OTK3_H_R2]>=DCP))
   {Cvib->Font->Color=clBlack;
    Cvib->Text=IntToStr(vib);
    for(int i=0;i<14;i++) Lvib->Items->Strings[14-i]=Lvib->Items->Strings[13-i];
    Lvib->Items->Strings[0]=Cvib->Text;
    ClientSocket1->Socket->SendText("параметр07="+FloatToStrF0(vib,6,1)+" ");
    write_tabl_dat("ВИБР_зтс", FloatToStrF0(vib,6,1));//в черный ящик
    Send09record(zts_vibr, IntToStr(vib));//отправляем в WITS
   }

   //------------------- модуль G -----------------------------------------
   if((num_kod==GMOD_L_R2)&&(data_perc[GMOD_H_R2]>=DCP)&&(data_perc[GMOD_L_R2]>=DCP))
   {CG->Font->Color=clBlack;
    CG->Text=FloatToStrF0(modg*1000,6,1);
    for(int i=0;i<14;i++)LG->Items->Strings[14-i]=LG->Items->Strings[13-i];
    LG->Items->Strings[0]=CG->Text;
    ClientSocket1->Socket->SendText("параметр12="+FloatToStrF0(modg,6,1)+" ");
    write_tabl_dat("Gtotal", FloatToStrF0(modg*1000,6,1));//в черный ящик
   }

   //------ ГК ЗТС -------------------------------------------------------
   switch(num_kod)
 	{
 	case(GK1_L_R2): if((data_perc[GK1_H_R2]>=DCP)&&(data_perc[GK1_L_R2]>=DCP))
                {
                 ClientSocket1->Socket->SendText("параметр19="+FloatToStrF0(gkzts,6,1)+" ");
                 Lgkzts->Caption="ГКзтс="+FloatToStrF0(gkzts,6,1);
                 write_tabl_dat("ГК_зтс", FloatToStrF0(gkzts,6,1));//в черный ящик
                 SendGkZtsToWits();//посылаем в wits
                 sendLogParam("GKzts");//посылаем в LOG ГКзтс
                }
                break;//ГK1
 	case(GK2_L_R2): if((data_perc[GK2_H_R2]>=DCP)&&(data_perc[GK2_L_R2]>=DCP))
                {
                 ClientSocket1->Socket->SendText("параметр19="+FloatToStrF0(gkzts,6,1)+" ");
                 write_tabl_dat("ГК_зтс", FloatToStrF0(gkzts,6,1));//в черный ящик
                 //if(SockNumber>0) SendToWits(gk1_raw, FloatToStrF0(gkzts,6,1));//Отправляем в Леузу, если подключен
                 Lgkzts->Caption="ГКзтс="+FloatToStrF0(gkzts,6,1);
                 SendGkZtsToWits();//посылаем в wits
                 sendLogParam("GKzts");//посылаем в LOG ГКзтс
                }
                break;//ГK2
    case(GK3_L_R2): if((data_perc[GK3_H_R2]>=DCP)&&(data_perc[GK3_L_R2]>=DCP))
                {
                 ClientSocket1->Socket->SendText("параметр19="+FloatToStrF0(gkzts,6,1)+" ");
                 write_tabl_dat("ГК_зтс", FloatToStrF0(gkzts,6,1));//в черный ящик
                 //if(SockNumber>0) SendToWits(gk1_raw, FloatToStrF0(gkzts,6,1));//Отправляем в Леузу, если подключен
                 Lgkzts->Caption="ГКзтс="+FloatToStrF0(gkzts,6,1);
                 SendGkZtsToWits();//посылаем в wits
                 sendLogParam("GKzts");//посылаем в LOG ГКзтс
                }
                break;//ГK3
    case(GK4_L_R2): if((data_perc[GK4_H_R2]>=DCP)&&(data_perc[GK4_L_R2]>=DCP))
                {
                 ClientSocket1->Socket->SendText("параметр19="+FloatToStrF0(gkzts,6,1)+" ");
                 write_tabl_dat("ГК_зтс", FloatToStrF0(gkzts,6,1));//в черный ящик
                 //if(SockNumber>0) SendToWits(gk1_raw, FloatToStrF0(gkzts,6,1));//Отправляем в Леузу, если подключен
                 Lgkzts->Caption="ГКзтс="+FloatToStrF0(gkzts,6,1);
                 SendGkZtsToWits();//посылаем в wits
                 sendLogParam("GKzts");//посылаем в LOG ГКзтс
                }
                break;//ГK4
    case(GK5_L_R2): if((data_perc[GK5_H_R2]>=DCP)&&(data_perc[GK5_L_R2]>=DCP))
                {
                 ClientSocket1->Socket->SendText("параметр19="+FloatToStrF0(gkzts,6,1)+" ");
                 write_tabl_dat("ГК_зтс", FloatToStrF0(gkzts,6,1));//в черный ящик
                 //if(SockNumber>0) SendToWits(gk1_raw, FloatToStrF0(gkzts,6,1));//Отправляем в Леузу, если подключен
                 Lgkzts->Caption="ГКзтс="+FloatToStrF0(gkzts,6,1);
                 SendGkZtsToWits();//посылаем в wits
                 sendLogParam("GKzts");//посылаем в LOG ГКзтс
                }
                break;//ГK5
    case(GK6_L_R2): if((data_perc[GK6_H_R2]>=DCP)&&(data_perc[GK6_L_R2]>=DCP))
                {
                 ClientSocket1->Socket->SendText("параметр19="+FloatToStrF0(gkzts,6,1)+" ");
                 //if(SockNumber>0) SendToWits(gk1_raw, FloatToStrF0(gkzts,6,1));//Отправляем в Леузу, если подключен
                 Lgkzts->Caption="ГКзтс="+FloatToStrF0(gkzts,6,1);
                 write_tabl_dat("ГК_зтс", FloatToStrF0(gkzts,6,1));//в черный ящик
                 SendGkZtsToWits();//посылаем в wits
                 sendLogParam("GKzts");//посылаем в LOG ГКзтс
                }
                break;//ГK6
    case(GK7_L_R2): if((data_perc[GK7_H_R2]>=DCP)&&(data_perc[GK7_L_R2]>=DCP))
                {
                 ClientSocket1->Socket->SendText("параметр19="+FloatToStrF0(gkzts,6,1)+" ");
                 //if(SockNumber>0) SendToWits(gk1_raw, FloatToStrF0(gkzts,6,1));//Отправляем в Леузу, если подключен
                 Lgkzts->Caption="ГКзтс="+FloatToStrF0(gkzts,6,1);
                 write_tabl_dat("ГК_зтс", FloatToStrF0(gkzts,6,1));//в черный ящик
                 SendGkZtsToWits();//посылаем в wits
                 sendLogParam("GKzts");//посылаем в LOG ГКзтс
                }
                break;//ГK7
 	}

    //------ ГК НДМ -------------------------------------------------------
    switch(num_kod)
    {
    case(GK1up_L_R2): if((data_perc[GK1up_H_R2]>=DCP)&&(data_perc[GK1up_L_R2]>=DCP))
                {ClientSocket1->Socket->SendText("параметр53="+FloatToStrF0(GKup,6,1)+" ");
                 //if(SockNumber>0) SendToWits(ndm_gk_up, FloatToStrF0(GKup,6,1));//Отправляем в Леузу, если подключен
                 Spar->Cells[2][2]=FloatToStrF0(GKup,6,1);
                 write_tabl_dat("ГКверх_ндм", FloatToStrF0(GKup,6,1));//в черный ящик
                }break;//ГК1верх
    case(GK1dn_L_R2): if((data_perc[GK1dn_H_R2]>=DCP)&&(data_perc[GK1dn_L_R2]>=DCP))
                {ClientSocket1->Socket->SendText("параметр54="+FloatToStrF0(GKdn,6,1)+" ");
                 //if(SockNumber>0) SendToWits(ndm_gk_dn, FloatToStrF0(GKdn,6,1));//Отправляем в Леузу, если подключен
                 Spar->Cells[3][2]=FloatToStrF0(GKdn,6,1);
                 write_tabl_dat("ГКниз_ндм", FloatToStrF0(GKdn,6,1));//в черный ящик
                 if((data_perc[GK1up_H_R2]>=DCP)&&(data_perc[GK1up_L_R2]>=DCP))
                 {
                  GKndm_int=(GKup+GKdn)*Kgk_ndm;
                  ClientSocket1->Socket->SendText("параметр41="+FloatToStrF0(GKndm_int,6,1)+" ");
                  Lgkndm->Caption="ГКндм="+FloatToStrF0(GKndm_int,6,1);//Выведем на панель ОТК1
                  write_tabl_dat("ГК_ндм", FloatToStrF0(GKndm_int,6,1));//в черный ящик
                  SendGkNdmToWits();
                  //if(SockNumber>0) SendToWits(ndm_gk, FloatToStrF0(GKndm_int,6,1));//Отправляем в Леузу, если подключен
                  sendLogParam("GKndm");//посылаем в LOG ГКндм
                 }
                } break;//ГК1низ
    case(GK2up_L_R2): if((data_perc[GK2up_H_R2]>=DCP)&&(data_perc[GK2up_L_R2]>=DCP))
                {ClientSocket1->Socket->SendText("параметр53="+FloatToStrF0(GKup,6,1)+" ");
                 //if(SockNumber>0) SendToWits(ndm_gk_up, FloatToStrF0(GKup,6,1));//Отправляем в Леузу, если подключен
                 Spar->Cells[2][2]=FloatToStrF0(GKup,6,1);
                 write_tabl_dat("ГКверх_ндм", FloatToStrF0(GKup,6,1));//в черный ящик
                } break;//ГК2верх
    case(GK2dn_L_R2): if((data_perc[GK2dn_H_R2]>=DCP)&&(data_perc[GK2dn_L_R2]>=DCP))
                {ClientSocket1->Socket->SendText("параметр54="+FloatToStrF0(GKdn,6,1)+" ");
                 //if(SockNumber>0) SendToWits(ndm_gk_dn, FloatToStrF0(GKdn,6,1));//Отправляем в Леузу, если подключен
                 Spar->Cells[3][2]=FloatToStrF0(GKdn,6,1);
                 write_tabl_dat("ГКниз_ндм", FloatToStrF0(GKdn,6,1));//в черный ящик
                 if((data_perc[GK2up_H_R2]>=DCP)&&(data_perc[GK2up_L_R2]>=DCP))
                 {GKndm_int=(GKup+GKdn)*Kgk_ndm;
                  ClientSocket1->Socket->SendText("параметр41="+FloatToStrF0(GKndm_int,6,1)+" ");
                  Lgkndm->Caption="ГКндм="+FloatToStrF0(GKndm_int,6,1);//Выведем на панель ОТК1
                  write_tabl_dat("ГК_ндм", FloatToStrF0(GKndm_int,6,1));//в черный ящик
                  SendGkNdmToWits();
                  sendLogParam("GKndm");//посылаем в LOG ГКндм
                  //if(SockNumber>0) SendToWits(ndm_gk, FloatToStrF0(GKndm_int,6,1));//Отправляем в Леузу, если подключен
                 }
                } break;//ГК2низ
    case(GK3up_L_R2): if((data_perc[GK3up_H_R2]>=DCP)&&(data_perc[GK3up_L_R2]>=DCP))
                {ClientSocket1->Socket->SendText("параметр53="+FloatToStrF0(GKup,6,1)+" ");
                 //if(SockNumber>0) SendToWits(ndm_gk_up, FloatToStrF0(GKup,6,1));//Отправляем в Леузу, если подключен
                 Spar->Cells[2][2]=FloatToStrF0(GKup,6,1);
                 write_tabl_dat("ГКверх_ндм", FloatToStrF0(GKup,6,1));//в черный ящик
                } break;//ГК3верх
    case(GK3dn_L_R2): if((data_perc[GK3dn_H_R2]>=DCP)&&(data_perc[GK3dn_L_R2]>=DCP))
                {ClientSocket1->Socket->SendText("параметр54="+FloatToStrF0(GKdn,6,1)+" ");
                 //if(SockNumber>0) SendToWits(ndm_gk_dn, FloatToStrF0(GKdn,6,1));//Отправляем в Леузу, если подключен
                 Spar->Cells[3][2]=FloatToStrF0(GKdn,6,1);
                 write_tabl_dat("ГКниз_ндм", FloatToStrF0(GKdn,6,1));//в черный ящик
                 if((data_perc[GK3up_H_R2]>=DCP)&&(data_perc[GK3up_L_R2]>=DCP))
                 {GKndm_int=(GKup+GKdn)*Kgk_ndm;
                  ClientSocket1->Socket->SendText("параметр41="+FloatToStrF0(GKndm_int,6,1)+" ");
                  Lgkndm->Caption="ГКндм="+FloatToStrF0(GKndm_int,6,1);//Выведем на панель ОТК1
                  write_tabl_dat("ГК_ндм", FloatToStrF0(GKndm_int,6,1));//в черный ящик
                  SendGkNdmToWits();
                  sendLogParam("GKndm");//посылаем в LOG ГКндм
                  //if(SockNumber>0) SendToWits(ndm_gk, FloatToStrF0(GKndm_int,6,1));//Отправляем в Леузу, если подключен
                 }
                } break;//ГК3низ
    case(GK4up_L_R2): if((data_perc[GK4up_H_R2]>=DCP)&&(data_perc[GK4up_L_R2]>=DCP))
                {ClientSocket1->Socket->SendText("параметр53="+FloatToStrF0(GKup,6,1)+" ");
                 //if(SockNumber>0) SendToWits(ndm_gk_up, FloatToStrF0(GKup,6,1));//Отправляем в Леузу, если подключен
                 Spar->Cells[2][2]=FloatToStrF0(GKup,6,1);
                 write_tabl_dat("ГКверх_ндм", FloatToStrF0(GKup,6,1));//в черный ящик
                } break;//ГК4верх
    case(GK4dn_L_R2): if((data_perc[GK4dn_H_R2]>=DCP)&&(data_perc[GK4dn_L_R2]>=DCP))
                {ClientSocket1->Socket->SendText("параметр54="+FloatToStrF0(GKdn,6,1)+" ");
                 //if(SockNumber>0) SendToWits(ndm_gk_dn, FloatToStrF0(GKdn,6,1));//Отправляем в Леузу, если подключен
                 Spar->Cells[3][2]=FloatToStrF0(GKdn,6,1);
                 write_tabl_dat("ГКниз_ндм", FloatToStrF0(GKdn,6,1));//в черный ящик
                 if((data_perc[GK4up_H_R2]>=DCP)&&(data_perc[GK4up_L_R2]>=DCP))
                 {GKndm_int=(GKup+GKdn)*Kgk_ndm;
                  ClientSocket1->Socket->SendText("параметр41="+FloatToStrF0(GKndm_int,6,1)+" ");
                  Lgkndm->Caption="ГКндм="+FloatToStrF0(GKndm_int,6,1);//Выведем на панель ОТК1
                  write_tabl_dat("ГК_ндм", FloatToStrF0(GKndm_int,6,1));//в черный ящик
                  SendGkNdmToWits();
                  sendLogParam("GKndm");//посылаем в LOG ГКндм
                  //if(SockNumber>0) SendToWits(ndm_gk, FloatToStrF0(GKndm_int,6,1));//Отправляем в Леузу, если подключен
                 }
                } break;//ГК4низ
    case(GK5up_L_R2): if((data_perc[GK5up_H_R2]>=DCP)&&(data_perc[GK5up_L_R2]>=DCP))
                {ClientSocket1->Socket->SendText("параметр53="+FloatToStrF0(GKup,6,1)+" ");
                 //if(SockNumber>0) SendToWits(ndm_gk_up, FloatToStrF0(GKup,6,1));//Отправляем в Леузу, если подключен
                 Spar->Cells[2][2]=FloatToStrF0(GKup,6,1);
                 write_tabl_dat("ГКверх_ндм", FloatToStrF0(GKup,6,1));//в черный ящик
                } break;//ГК4верх
    case(GK5dn_L_R2): if((data_perc[GK5dn_H_R2]>=DCP)&&(data_perc[GK5dn_L_R2]>=DCP))
                {ClientSocket1->Socket->SendText("параметр54="+FloatToStrF0(GKdn,6,1)+" ");
                 //if(SockNumber>0) SendToWits(ndm_gk_dn, FloatToStrF0(GKdn,6,1));//Отправляем в Леузу, если подключен
                 Spar->Cells[3][2]=FloatToStrF0(GKdn,6,1);
                 write_tabl_dat("ГКниз_ндм", FloatToStrF0(GKdn,6,1));//в черный ящик
                 if((data_perc[GK5up_H_R2]>=DCP)&&(data_perc[GK5up_L_R2]>=DCP))
                 {GKndm_int=(GKup+GKdn)*Kgk_ndm;
                  ClientSocket1->Socket->SendText("параметр41="+FloatToStrF0(GKndm_int,6,1)+" ");
                  Lgkndm->Caption="ГКндм="+FloatToStrF0(GKndm_int,6,1);//Выведем на панель ОТК1
                  write_tabl_dat("ГК_ндм", FloatToStrF0(GKndm_int,6,1));//в черный ящик
                  SendGkNdmToWits();
                  sendLogParam("GKndm");//посылаем в LOG ГКндм
                  //if(SockNumber>0) SendToWits(ndm_gk, FloatToStrF0(GKndm_int,6,1));//Отправляем в Леузу, если подключен
                 }
                } break;//ГК5низ
    case(GK6up_L_R2): if((data_perc[GK6up_H_R2]>=DCP)&&(data_perc[GK6up_L_R2]>=DCP))
                {ClientSocket1->Socket->SendText("параметр53="+FloatToStrF0(GKup,6,1)+" ");
                 //if(SockNumber>0) SendToWits(ndm_gk_up, FloatToStrF0(GKup,6,1));//Отправляем в Леузу, если подключен
                 Spar->Cells[2][2]=FloatToStrF0(GKup,6,1);
                 write_tabl_dat("ГКверх_ндм", FloatToStrF0(GKup,6,1));//в черный ящик
                } break;//ГК4верх
    case(GK6dn_L_R2): if((data_perc[GK6dn_H_R2]>=DCP)&&(data_perc[GK6dn_L_R2]>=DCP))
                {ClientSocket1->Socket->SendText("параметр54="+FloatToStrF0(GKdn,6,1)+" ");
                 //if(SockNumber>0) SendToWits(ndm_gk_dn, FloatToStrF0(GKdn,6,1));//Отправляем в Леузу, если подключен
                 Spar->Cells[3][2]=FloatToStrF0(GKdn,6,1);
                 write_tabl_dat("ГКниз_ндм", FloatToStrF0(GKdn,6,1));//в черный ящик
                 if((data_perc[GK6up_H_R2]>=DCP)&&(data_perc[GK6up_L_R2]>=DCP))
                 {GKndm_int=(GKup+GKdn)*Kgk_ndm;
                  ClientSocket1->Socket->SendText("параметр41="+FloatToStrF0(GKndm_int,6,1)+" ");
                  Lgkndm->Caption="ГКндм="+FloatToStrF0(GKndm_int,6,1);//Выведем на панель ОТК1
                  write_tabl_dat("ГК_ндм", FloatToStrF0(GKndm_int,6,1));//в черный ящик
                  SendGkNdmToWits();
                  sendLogParam("GKndm");//посылаем в LOG ГКндм
                  //if(SockNumber>0) SendToWits(ndm_gk, FloatToStrF0(GKndm_int,6,1));//Отправляем в Леузу, если подключен
                 }
                } break;//ГК6низ
    case(GK7up_L_R2): if((data_perc[GK7up_H_R2]>=DCP)&&(data_perc[GK7up_L_R2]>=DCP))
                {ClientSocket1->Socket->SendText("параметр53="+FloatToStrF0(GKup,6,1)+" ");
                 //if(SockNumber>0) SendToWits(ndm_gk_up, FloatToStrF0(GKup,6,1));//Отправляем в Леузу, если подключен
                 Spar->Cells[2][2]=FloatToStrF0(GKup,6,1);
                 write_tabl_dat("ГКверх_ндм", FloatToStrF0(GKup,6,1));//в черный ящик
                } break;//ГК4верх
    case(GK7dn_L_R2): if((data_perc[GK7dn_H_R2]>=DCP)&&(data_perc[GK7dn_L_R2]>=DCP))
                {ClientSocket1->Socket->SendText("параметр54="+FloatToStrF0(GKdn,6,1)+" ");
                 //if(SockNumber>0) SendToWits(ndm_gk_dn, FloatToStrF0(GKdn,6,1));//Отправляем в Леузу, если подключен
                 Spar->Cells[3][2]=FloatToStrF0(GKdn,6,1);
                 write_tabl_dat("ГКниз_ндм", FloatToStrF0(GKdn,6,1));//в черный ящик
                 if((data_perc[GK7up_H_R2]>=DCP)&&(data_perc[GK7up_L_R2]>=DCP))
                 {GKndm_int=(GKup+GKdn)*Kgk_ndm;
                  ClientSocket1->Socket->SendText("параметр41="+FloatToStrF0(GKndm_int,6,1)+" ");
                  Lgkndm->Caption="ГКндм="+FloatToStrF0(GKndm_int,6,1);//Выведем на панель ОТК1
                  write_tabl_dat("ГК_ндм", FloatToStrF0(GKndm_int,6,1));//в черный ящик
                  SendGkNdmToWits();
                  sendLogParam("GKndm");//посылаем в LOG ГКндм
                  //if(SockNumber>0) SendToWits(ndm_gk, FloatToStrF0(GKndm_int,6,1));//Отправляем в Леузу, если подключен
                 }
                } break;//ГК7низ
    }

    //------ добавляем НДМ ------------------------------
    switch(num_kod)
    {
    //ZUndm
    case(GK4_H_R2):
 		   if((data_perc[GK4_H_R2]>=DCP)&&(data_perc[ZUndm_H_R2]>=DCP)&&(data_perc[ZUndm_L_R2]>=DCP)&&(data_perc[OTK4_H_R2]>=DCP))//Зенит от НДМ
           {
            Spar->Cells[1][2]=FloatToStrF0(ZUndm,6,1);
            ClientSocket1->Socket->SendText("параметр40="+FloatToStrF0(ZUndm,6,1)+" ");
            write_tabl_dat("ЗУ_ндм", FloatToStrF0(ZUndm,6,1));//в черный ящик
            //if(SockNumber>0) SendToWits(ndm_zu, FloatToStrF0(ZUndm,6,1));//Отправляем в Леузу, если подключен
           }
           break;
    //Температура НДМ
    case(TEMPndm_L_R2): if((data_perc[TEMPndm_H_R2]>=DCP)&&(data_perc[TEMPndm_L_R2]>=DCP)&&(data_perc[DAVL_H_R2]>=DCP))
           {
            Spar->Cells[4][2]=FloatToStrF0(TEMPndm,6,1);
            ClientSocket1->Socket->SendText("параметр42="+FloatToStrF0(TEMPndm,6,1)+" ");
            write_tabl_dat("ТЕМП_ндм", FloatToStrF0(TEMPndm,6,1));//в черный ящик
            Send09record(ndm_temp, FloatToStrF0(TEMPndm,6,1));//отправляем в WITS
           }
           break;
    //IND
    case(TOK_H_R2): if((data_perc[OBORndm_L_R2]>=DCP)&&(data_perc[INDndm_H_R2]>=DCP)&&(data_perc[TOK_H_R2]>=DCP))
           {
            Spar->Cells[0][2]=IntToStr(indic);
            ClientSocket1->Socket->SendText("параметр46="+FloatToStrF0(indic,6,1)+" ");
            write_tabl_dat("Индикатор", IntToStr(indic));//в черный ящик
           }
           break;

    //dU
    case(GK7up_H_R2):
            if(
                (data_perc[TEMPndm_L_R2]>=DCP)&&(data_perc[dU_H_R2]>=DCP)&&(data_perc[OTK7_H_R2]>=DCP)
                &&(data_perc[GK7_H_R2]>=DCP)&&(data_perc[GK7up_H_R2]>=DCP)
            )//dU
           {
            Spar->Cells[9][2]=IntToStr((int)dUndm);
            ClientSocket1->Socket->SendText("параметр50="+FloatToStrF0(dUndm,6,1)+" ");
            write_tabl_dat("dU_ндм", FloatToStrF0(dUndm,6,1));//в черный ящик
            //КСндм
            float n1=PEAKndm;
            if(n1==0) n1=0.000001;
            KCndm=dUndm*Kgeo/n1;
            if(KCndm>10000) KCndm=10000;
            ClientSocket1->Socket->SendText("параметр48="+FloatToStrF0(KCndm,8,4)+" ");//КСндм
            write_tabl_dat("КС_ндм", FloatToStrF0(KCndm,8,4));//в черный ящик
            ClientSocket1->Socket->SendText("параметр49="+FloatToStrF0(Kgeo,6,1)+" ");//К геом.фактор
           }
           break;
    //Обороты
    case(OBORndm_L_R2):
            if((data_perc[GK4up_H_R2]>=DCP)&&(data_perc[AZ_OTK4_H_R2]>=DCP)&&(data_perc[GK4dn_H_R2]>=DCP)
                &&(data_perc[OBORndm_L_R2]>=DCP)
            )
           {
            Spar->Cells[5][2]=IntToStr((int)OBORndm);
            ClientSocket1->Socket->SendText("параметр43="+FloatToStrF0(OBORndm,6,1)+" ");
            write_tabl_dat("ОБОР_ндм", FloatToStrF0(OBORndm,6,1));//в черный ящик
            //if(SockNumber>0) SendToWits(ndm_obor, FloatToStrF0(OBORndm,6,1));//Отправляем в Леузу, если подключен
            if(PB->Checked) SendOneQpb(wOBOR,OBORndm);//отправляем в Пульт Бурильщика, если выбран
            Send09record(ndm_obor, IntToStr((int)OBORndm));//отправляем в WITS
           }
           break;
    //Амплитуда peak
    case(PEAK_L_R2): if((data_perc[PEAK_H_R2]>=DCP)&&(data_perc[PEAK_M_R2]>=DCP)&&(data_perc[PEAK_L_R2]>=DCP))
           {
            if(USIL) Spar->Cells[8][2]=FloatToStrF0(PEAKndm,6,3);
            else Spar->Cells[8][2]=FloatToStrF0(PEAKndm,6,2);
            ClientSocket1->Socket->SendText("параметр47="+FloatToStrF0(PEAKndm,6,3)+" ");
            write_tabl_dat("PEAK_ндм", FloatToStrF0(PEAKndm,6,3));//в черный ящик
           }
           break;
    //TOK
    case(OTK5_H_R2): if((data_perc[TOK_H_R2]>=DCP)&&(data_perc[TOK_L_R2]>=DCP)&&(data_perc[OTK5_H_R2]>=DCP))//ТОК
           {
            Spar->Cells[7][2]=IntToStr(TOKndm);
            ClientSocket1->Socket->SendText("параметр45="+FloatToStrF0(TOKndm,6,1)+" ");
            write_tabl_dat("ТОК_ндм", FloatToStrF0(TOKndm,6,1));//в черный ящик
           }
           break;
    //Нагрузка от НДМ
    case(MOMENT_H_R2):
            if((data_perc[GK5_H_R2]>=DCP)&&(data_perc[GK5up_H_R2]>=DCP)&&(data_perc[AZ_OTK5_H_R2]>=DCP)
                &&(data_perc[GK5dn_H_R2]>=DCP)&&(data_perc[MOMENT_H_R2]>=DCP)
            )
           {//Нагрузка в кН
            Spar->Cells[6][2]=FloatToStrF0(NAGRndm,6,1);
            ClientSocket1->Socket->SendText("параметр44="+FloatToStrF0(NAGRndm,6,1)+" ");
            write_tabl_dat("НАГР_ндм", FloatToStrF0(NAGRndm,6,1));//в черный ящик
            //if(SockNumber>0) SendToWits(ndm_nagr, FloatToStrF0(NAGRndm,6,1));//Отправляем в Леузу, если подключен
            if(PB->Checked) SendOneQpb(wNAGR,NAGRndm);//отправляем в Пульт Бурильщика, если выбран
            Send09record(ndm_nagr, FloatToStrF0(NAGRndm,6,1));//отправляем в WITS
           }
           break;

    //Момент
    case(GK6up_H_R2):
            if((data_perc[MOMENT_H_R2]>=DCP)&&(data_perc[OTK6_H_R2]>=DCP)&&(data_perc[GK6_H_R2]>=DCP)
                &&(data_perc[GK6up_H_R2]>=DCP)
            )
           {//момент в кН
            Spar->Cells[10][2]=FloatToStrF0(MOMndm,6,1);
            ClientSocket1->Socket->SendText("параметр51="+FloatToStrF0(MOMndm,6,1)+" ");
            write_tabl_dat("МОМЕНТ_ндм", FloatToStrF0(MOMndm,6,1));//в черный ящик
            //if(SockNumber>0) SendToWits(ndm_mom, FloatToStrF0(MOMndm,6,1));//Отправляем в Леузу, если подключен
            if(PB->Checked) SendOneQpb(wMOM,MOMndm);//отправляем в Пульт Бурильщика, если выбран
            Send09record(ndm_mom, FloatToStrF0(MOMndm,6,1));//отправляем в WITS
           }
           break;
    //Давление
    case(DAVL_H_R2):
            if((data_perc[GK6up_H_R2]>=DCP)&&(data_perc[AZ_OTK6_H_R2]>=DCP)&&(data_perc[GK6dn_H_R2]>=DCP)
                &&(data_perc[DAVL_H_R2]>=DCP)
            )
           {//давление в МПа
            Spar->Cells[11][2]=FloatToStrF0(DAVLndm,6,1);
            ClientSocket1->Socket->SendText("параметр52="+FloatToStrF0(DAVLndm,6,1)+" ");
            write_tabl_dat("ДАВЛ_ндм", FloatToStrF0(DAVLndm,6,1));//в черный ящик
            Send09record(ndm_davl, FloatToStrF0(DAVLndm,6,1));//отправляем в WITS
            if(PB->Checked) SendOneQpb(wDAVL,DAVLndm);//отправляем в Пульт Бурильщика, если выбран
           }
           break;
    }

    //на последнем коде определяем качество НДМ и отправляем в LOG
    if(num_kod==(DLINA_CODA_R2-1))
    {   if(data_perc[TEMPndm_L_R2]>=DCP)
    	{	masQualNDM[2]=QUAL_NDM_STAT;//качество НДМ
        	write_tabl_dat("QUAL_NDM_STAT", IntToStr(QUAL_NDM_STAT));//в черный ящик
    		//перерисовываем все столбцы верхней строки НДМ
        	for(int i=0;i<Spar->ColCount;i++) Spar->Cells[i][2] = Spar->Cells[i][2];
        }
        sendLogParam("NDMall");//посылаем в LOG все параметры НДМ
    }
}
//------- FORMAT_KADRA=2 -----------------------------------------------------
