//========= Вывод цифирей в режиме FORMAT_KADRA=1 =====================================================
if((FORMAT_KADRA==FORMAT_STD_GK)&&(DCPt>=DCP))
{
   if(num_kod==FIRST_KOD_R1)
   {
    ClientSocket1->Socket->SendText("параметр09="+Eglubina->Text+" ");
    ClientSocket1->Socket->SendText("freg="+R4->Items->Strings[R4->ItemIndex]+" ");
   }
  //---------- зенит -------------------------------
  if( (num_kod==AZ_OTK4_H_R1) && (data_perc[AZ_OTK4_H_R1]>=DCP) && (data_perc[ZU_H_R1]>=DCP) && (data_perc[ZU_L_R1]>=DCP) )
  {L1->Cells[0][2]=FloatToStrF0(zenit,6,2);//выводим в колонку
   ClientSocket1->Socket->SendText("параметр01="+FloatToStrF0(zenit,6,2)+" ");
   write_tabl_dat("ЗУ_зтс", FloatToStrF0(zenit,6,2));//в черный ящик
   //if(SockNumber>0) SendToWits(zts_zu, FloatToStrF0(zenit,6,2));//Отправляем в Wits, если подключен
   //if(PB->Checked) SendOneQpb(wZU,zenit);//отправляем в Пульт Бурильщика, если выбран

   int j=0; Lz->Items->Clear();
   for(int i=0;i<L1->RowCount-2;i++) if(CompareText(L1->Cells[0][i+2],"")!=0) {Lz->Items->Strings[j]=L1->Cells[0][i+2];j=j+1;}
  }
  //---------------- азимут --------------------------
  if( //если проценты данных а порядке
     (num_kod==AZ_OTK4_H_R1)
     && (data_perc[HX_H_R1]>=DCP) && (data_perc[HX_M_R1]>=DCP) && (data_perc[HX_L_R1]>=DCP)
     && (data_perc[HY_H_R1]>=DCP) && (data_perc[HY_M_R1]>=DCP) && (data_perc[HY_L_R1]>=DCP)
     && (data_perc[HZ_H_R1]>=DCP) && (data_perc[HZ_L_R1]>=DCP)
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

   //Текущий модуль Н
   Lh->Cells[0][2]=IntToStr(Htotal)+"("+FloatToStrF((float)Htotal/Hop,ffFixed,4,2)+")";
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
   if((num_kod==GK3_H_R1)&& (data_perc[AZ_OTK2_H_R1]>=DCP) && (data_perc[OTK3_H_R1]>=DCP) && (data_perc[GK3_H_R1]>=DCP))
   {
    Spar->Cells[12][2]=IntToStr(num_mode) + RoleStr;//выводим в колонку Режим и Роль
    write_tabl_dat("Режим", Spar->Cells[12][2]);//в черный ящик
    if(num_mode != FORMAT_KADRA)//Если режимы не соответствуют друг другу
    {//ShowMessage("Режим работы скважинного прибора не соответствует приемнику");
     SaveStatusLog("Режим работы скважинного прибора("+IntToStr(num_mode)+")"+" не соответствует настройкам программы("+IntToStr(FORMAT_KADRA)+")", clRed);
     PlaySound("MYS2",0,SND_RESOURCE);
    }
   }

   //------------- ОТК угловой статический -------------------------
   if((num_kod==OTKu_L_R1) && (data_perc[OTKu_H_R1]>=DCP) && (data_perc[OTKu_L_R1]>=DCP))
   {write_tabl_dat("ОТК_стат", FloatToStrF0(otklonf,6,2));//в черный ящик
   }

   //------------ Отклонители --------------------------------------
   if(
      (  ((num_kod==OTK1_L_R1)&&(data_perc[OTK1_H_R1]>=DCP)&&(data_perc[OTK1_L_R1]>=DCP))
       ||((num_kod==OTK2_L_R1)&&(data_perc[OTK2_H_R1]>=DCP)&&(data_perc[OTK2_L_R1]>=DCP))
       ||((num_kod==OTK3_L_R1)&&(data_perc[OTK3_H_R1]>=DCP)&&(data_perc[OTK3_L_R1]>=DCP))
       ||((num_kod==OTK4_L_R1)&&(data_perc[OTK4_H_R1]>=DCP)&&(data_perc[OTK4_L_R1]>=DCP))
      )
     )//4 отклонителя
   {
    int dat_o;
    if(num_kod==OTK1_L_R1) dat_o=dat_o1;
    if(num_kod==OTK2_L_R1) dat_o=dat_o2;
    if(num_kod==OTK3_L_R1) dat_o=dat_o3;
    if(num_kod==OTK4_L_R1) dat_o=dat_o4;
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
    if(num_kod==OTK1_L_R1){dat_o1=dat_o;}
    if(num_kod==OTK2_L_R1){dat_o2=dat_o;}
    if(num_kod==OTK3_L_R1){dat_o3=dat_o;}
    if(num_kod==OTK4_L_R1){dat_o4=dat_o;}
    otktek=dat_o;//текущий общего пользования

    //отправляем в Пульт Бурильщика, если выбран
    if(PB->Checked && PBType->ItemIndex==0 && PBRadioGroup->ItemIndex==0)//если наш ПБ
    {SendOneQpb(wOTK,dat_o);//в круг ОТК
     SendOneQpb(wAZ,dat_o);//дублируем цифрами в АЗ
    }
    if(PB->Checked && PBType->ItemIndex==1 && PBRadioGroup->ItemIndex==0)//если не наш ПБ
    {   SendByteQpb_rfd(SET_OTK_MODE,OTK_MODE_GRAV);//режим ОТК
        SendOneQpb_rfd(SET_OTK,(float)dat_o);//в круг ОТК
    }
    DrawOTK1(dat_o);//рисуем ОТК1
   }  //Конец 4 отклонителя

   //------------------- АЗ-ОТК -----------------------------------------
   if(    ((num_kod==AZ_OTK1_L_R1)&&(data_perc[AZ_OTK1_H_R1]>=DCP)&&(data_perc[AZ_OTK1_L_R1]>=DCP))
        ||((num_kod==AZ_OTK2_L_R1)&&(data_perc[AZ_OTK2_H_R1]>=DCP)&&(data_perc[AZ_OTK2_L_R1]>=DCP))
        ||((num_kod==AZ_OTK3_L_R1)&&(data_perc[AZ_OTK3_H_R1]>=DCP)&&(data_perc[AZ_OTK3_L_R1]>=DCP))
        ||((num_kod==AZ_OTK4_L_R1)&&(data_perc[AZ_OTK4_H_R1]>=DCP)&&(data_perc[AZ_OTK4_L_R1]>=DCP))
     )
   {
    if(num_kod==AZ_OTK1_L_R1) az_otk=dat_oa1;
    if(num_kod==AZ_OTK2_L_R1) az_otk=dat_oa2;
    if(num_kod==AZ_OTK3_L_R1) az_otk=dat_oa3;
    if(num_kod==AZ_OTK4_L_R1) az_otk=dat_oa4;
    //если не Зарезка, а ОТК2
    if(!Caz_otk->Checked && az_otk!=400)
    {
     float otk2;
     float paz=MyStrToFloat(Fr45->Sn->Cells[1][1]);
     float mnr=mn*pi/180.,ZUtr=zu*pi/180.,AZtr=paz*pi/180.;
     float Mx=-BASEACC*cos(mnr*sin(AZtr))*sin(ZUtr-mnr*cos(AZtr));
     float My=BASEACC*sin(mnr*sin(AZtr));
     otk2=az_otk + rotor(-My,Mx);
     az_otk=inta(otk2+360)%360;
    }

    //если Зарезка и режим Истинного Азимута
    if(Caz_otk->Checked && az_otk!=400 && MODE_AZ>0) az_otk=inta(get_ums()+az_otk)%360;

    //применим смещения меток
    if(az_otk!=400)
    {
     az_otk-=nulotk;
     az_otk+=mrmt;
     az_otk=((int)az_otk+360)%360;
    }
    if(num_kod==AZ_OTK1_L_R1) dat_oa1=az_otk;
    if(num_kod==AZ_OTK2_L_R1) dat_oa2=az_otk;
    if(num_kod==AZ_OTK3_L_R1) dat_oa3=az_otk;
    if(num_kod==AZ_OTK4_L_R1) dat_oa4=az_otk;

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
   if(	(num_kod==GK4_H_R1)
   		&&(data_perc[AZ_OTK3_H_R1]>=DCP)&&(data_perc[OTK4_H_R1]>=DCP)&&(data_perc[GK4_H_R1]>=DCP)
     )
   {Cvib->Font->Color=clBlack;
    Cvib->Text=IntToStr(vib);
    for(int i=0;i<14;i++) Lvib->Items->Strings[14-i]=Lvib->Items->Strings[13-i];
    Lvib->Items->Strings[0]=Cvib->Text;
    ClientSocket1->Socket->SendText("параметр07="+FloatToStrF0(vib,6,1)+" ");
    write_tabl_dat("ВИБР_зтс", FloatToStrF0(vib,6,1));//в черный ящик
    Send09record(zts_vibr, IntToStr(vib));//отправляем в WITS
   }

   //------------------- модуль G -----------------------------------------
   if((num_kod==GMOD_L_R1)&&(data_perc[GMOD_H_R12]>=DCP)&&(data_perc[GMOD_L_R1]>=DCP))
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
 	case(GK1_L_R1): if((data_perc[GK1_H_R1]>=DCP)&&(data_perc[GK1_L_R1]>=DCP))
                {
                 ClientSocket1->Socket->SendText("параметр19="+FloatToStrF0(gkzts,6,1)+" ");
                 //if(SockNumber>0) SendToWits(gk1_raw, FloatToStrF0(gkzts,6,1));//Отправляем в Леузу, если подключен
                 Lgkzts->Caption="ГКзтс="+FloatToStrF0(gkzts,6,1);
                 write_tabl_dat("ГК_зтс", FloatToStrF0(gkzts,6,1));//в черный ящик
                 Lgkndm->Caption="";
                 SendGkZtsToWits();//посылаем в wits
                 sendLogParam("GKzts");//посылаем в LOG ГКзтс
                }
                break;//ГK1
 	case(GK2_L_R1): if((data_perc[GK2_H_R1]>=DCP)&&(data_perc[GK2_L_R1]>=DCP))
                {
                 ClientSocket1->Socket->SendText("параметр19="+FloatToStrF0(gkzts,6,1)+" ");
                 write_tabl_dat("ГК_зтс", FloatToStrF0(gkzts,6,1));//в черный ящик
                 //if(SockNumber>0) SendToWits(gk1_raw, FloatToStrF0(gkzts,6,1));//Отправляем в Леузу, если подключен
                 Lgkzts->Caption="ГКзтс="+FloatToStrF0(gkzts,6,1);
                 Lgkndm->Caption="";
                 SendGkZtsToWits();//посылаем в wits
                 sendLogParam("GKzts");//посылаем в LOG ГКзтс
                }
                break;//ГK2
    case(GK3_L_R1): if((data_perc[GK3_H_R1]>=DCP)&&(data_perc[GK3_L_R1]>=DCP))
                {
                 ClientSocket1->Socket->SendText("параметр19="+FloatToStrF0(gkzts,6,1)+" ");
                 write_tabl_dat("ГК_зтс", FloatToStrF0(gkzts,6,1));//в черный ящик
                 //if(SockNumber>0) SendToWits(gk1_raw, FloatToStrF0(gkzts,6,1));//Отправляем в Леузу, если подключен
                 Lgkzts->Caption="ГКзтс="+FloatToStrF0(gkzts,6,1);
                 Lgkndm->Caption="";
                 SendGkZtsToWits();//посылаем в wits
                 sendLogParam("GKzts");//посылаем в LOG ГКзтс
                }
                break;//ГK3
    case(GK4_L_R1): if((data_perc[GK4_H_R1]>=DCP)&&(data_perc[GK4_L_R1]>=DCP))
                {
                 ClientSocket1->Socket->SendText("параметр19="+FloatToStrF0(gkzts,6,1)+" ");
                 write_tabl_dat("ГК_зтс", FloatToStrF0(gkzts,6,1));//в черный ящик
                 //if(SockNumber>0) SendToWits(gk1_raw, FloatToStrF0(gkzts,6,1));//Отправляем в Леузу, если подключен
                 Lgkzts->Caption="ГКзтс="+FloatToStrF0(gkzts,6,1);
                 Lgkndm->Caption="";
                 SendGkZtsToWits();//посылаем в wits
                 sendLogParam("GKzts");//посылаем в LOG ГКзтс
                }
                break;//ГK4
 	}
}
//------- FORMAT_KADRA=1 -----------------------------------------------------
