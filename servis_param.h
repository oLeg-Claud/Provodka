//------------- Выводим Служебные параметры ------------------------
if(
   /* режим 1 */
   ((FORMAT_KADRA==FORMAT_STD_GK)&&(num_kod==OTK3_H_R1)&&(DCPt>=DCP)&&(data_perc[OTK1_H_R1]>=DCP)&&(data_perc[AZ_OTK1_H_R1]>=DCP)&&(data_perc[OTK2_H_R1]>=DCP)&&(data_perc[AZ_OTK2_H_R1]>=DCP)&&(data_perc[OTK3_H_R1]>=DCP))
   ||
   /* режим 2 */
   ((FORMAT_KADRA==FORMAT_NDM_GK_RETR)&&(num_kod==OTK3_H_R2)&&(DCPt>=DCP)&&(data_perc[OTK1_H_R2]>=DCP)&&(data_perc[AZ_OTK1_H_R2]>=DCP)&&(data_perc[OTK2_H_R2]>=DCP)&&(data_perc[AZ_OTK2_H_R2]>=DCP)&&(data_perc[OTK3_H_R2]>=DCP))
   ||
   /* режим 11 */
   ((FORMAT_KADRA==FORMAT_GK_RETR)&&(num_kod==OTK3_H_R11)&&(DCPt>=DCP)&&(data_perc[OTK1_H_R11]>=DCP)&&(data_perc[AZ_OTK1_H_R11]>=DCP)&&(data_perc[OTK2_H_R11]>=DCP)&&(data_perc[AZ_OTK2_H_R11]>=DCP)&&(data_perc[OTK3_H_R11]>=DCP))
   ||
   /* режим 12 */
   ((FORMAT_KADRA==FORMAT_STD)&&(num_kod==OTK3_H_R12)&&(DCPt>=DCP)&&(data_perc[OTK1_H_R12]>=DCP)&&(data_perc[AZ_OTK1_H_R12]>=DCP)&&(data_perc[OTK2_H_R12]>=DCP)&&(data_perc[AZ_OTK2_H_R12]>=DCP)&&(data_perc[OTK3_H_R12]>=DCP))
   ||
   /* режим 13 */
   ((FORMAT_KADRA==FORMAT_STD_RETR)&&(num_kod==OTK3_H_R13)&&(DCPt>=DCP)&&(data_perc[OTK1_H_R13]>=DCP)&&(data_perc[AZ_OTK1_H_R13]>=DCP)&&(data_perc[OTK2_H_R13]>=DCP)&&(data_perc[AZ_OTK2_H_R13]>=DCP)&&(data_perc[OTK3_H_R13]>=DCP))
   ||
   /* режим 30 */
   ((FORMAT_KADRA==FORMAT_GEOMASH)&&(num_kod==OBOR_L_R30)&&(DCPt>=DCP)&&(data_perc[OBOR_H_R30]>=DCP)&&(data_perc[OBOR_L_R30]>=DCP))
   ||
   /* режим 31 */
   ((FORMAT_KADRA==FORMAT_GEOMASH_RETR)&&(num_kod==OBOR_L_R31)&&(DCPt>=DCP)&&(data_perc[OBOR_H_R31]>=DCP)&&(data_perc[OBOR_L_R31]>=DCP))
  )
{
  //делаем все параметры серыми
  Cobor->Font->Color=clGray;
  Ctem->Font->Color=clGray;
  Cnapr->Font->Color=clGray;
  //Cvib->Font->Color=clGray;
  Cgk->Font->Color=clGray;
  Cwr->Font->Color=clGray;
  //CG->Font->Color=clGray;

  switch(iot)
  {
  case(0)://Обороты
    Label4->Caption = "Обор";//переименуем ячейку оборотов в Обор
    Cobor->Font->Color=clBlack;//черним параметр
    Cobor->Text=IntToStr(obor);
    for(int i=0;i<14;i++) Lo->Items->Strings[14-i]=Lo->Items->Strings[13-i];
    Lo->Items->Strings[0]=Cobor->Text;
    ClientSocket1->Socket->SendText("параметр04="+FloatToStrF0(obor,6,1)+" ");
    Send09record(zts_obor, IntToStr(obor));//отправляем в WITS
    write_tabl_dat("ОБОР_зтс", IntToStr(obor));//в черный ящик
    break;
  case(1)://Температура
    Ctem->Font->Color=clBlack;//черним параметр
    Ctem->Text=IntToStr(tem);
    for(int i=0;i<14;i++)Lte->Items->Strings[14-i]=Lte->Items->Strings[13-i];
    Lte->Items->Strings[0]=Ctem->Text;
    ClientSocket1->Socket->SendText("параметр05="+FloatToStrF0(tem,6,1)+" ");
    //if(SockNumber>0) SendToWits(zts_temp, FloatToStrF0(tem,6,1));//Отправляем в Леузу, если подключен
    write_tabl_dat("ТЕМП_зтс", FloatToStrF0(tem,6,1));//в черный ящик
    break;
  case(2)://Напряжение
    Cnapr->Font->Color=clBlack;//черним параметр
    Cnapr->Text=FloatToStrF0(NAPR,6,1);
    for(int i=0;i<14;i++)Lnapr->Items->Strings[14-i]=Lnapr->Items->Strings[13-i];
    Lnapr->Items->Strings[0]=Cnapr->Text;
    ClientSocket1->Socket->SendText("параметр06="+FloatToStrF0(NAPR,6,1)+" ");
    write_tabl_dat("НАПР_зтс", FloatToStrF0(NAPR,6,1));//в черный ящик
    break;
  /*case(3)://Вибрация
    Cvib->Font->Color=clBlack;//черним параметр
    Cvib->Text=IntToStr(vib);
    for(int i=0;i<14;i++) Lvib->Items->Strings[14-i]=Lvib->Items->Strings[13-i];
    Lvib->Items->Strings[0]=Cvib->Text;
    ClientSocket1->Socket->SendText("параметр07="+FloatToStrF0(vib,6,1)+" ");
    Send09record(zts_vibr, IntToStr(vib));//отправляем в WITS
    break;*/
  case(4)://Ток
    Cgk->Font->Color=clBlack;//черним параметр
    Cgk->Text=FloatToStrF0(gk,5,1);
    for(int i=0;i<14;i++)Lgk->Items->Strings[14-i]=Lgk->Items->Strings[13-i];
    Lgk->Items->Strings[0]=Cgk->Text;
    ClientSocket1->Socket->SendText("параметр08="+FloatToStrF0(gk,6,1)+" ");
    write_tabl_dat("ТОК_зтс", FloatToStrF0(gk,6,1));//в черный ящик
    break;
  case(5)://Время
    Label15->Font->Color = clBlack;
    Label15->Caption = "Время";//переименуем ячейку времени во Время
    Cwr->Font->Color=clBlack;//черним параметр
    Cwr->Text=IntToStr(wr);
    for(int i=0;i<14;i++)Lwr->Items->Strings[14-i]=Lwr->Items->Strings[13-i];
    Lwr->Items->Strings[0]=Cwr->Text;
    ClientSocket1->Socket->SendText("параметр11="+FloatToStrF0(wr,6,1)+" ");
    write_tabl_dat("Время", FloatToStrF0(wr,6,1));//в черный ящик
    //if(ctat && wr>10) SaveStatusLog("Внимание! Обнаружен режим переключения частот!", clRed);
    break;
  /*case(6)://МодульG
    CG->Font->Color=clBlack;//черним параметр
    CG->Text=FloatToStrF0(modg,8,4);
    for(int i=0;i<14;i++)LG->Items->Strings[14-i]=LG->Items->Strings[13-i];
    LG->Items->Strings[0]=CG->Text;
    ClientSocket1->Socket->SendText("параметр12="+FloatToStrF0(modg,8,4)+" ");
    break;*/
  case(7)://MAHndm, A*ч НДМ с шагом 0.1, выводим в ячейку времени
    Label15->Font->Color = clBlack;
    Label15->Caption = "АчНдм";//переименуем ячейку времени в Ампер/час НДМ
    Cwr->Font->Color=clBlack;//черним параметр
    Cwr->Text=FloatToStrF0(MAHndm,4,1);//выводим в ячейку времени
    for(int i=0;i<14;i++)Lwr->Items->Strings[14-i]=Lwr->Items->Strings[13-i];
    Lwr->Items->Strings[0]=Cwr->Text;
    //ClientSocket1->Socket->SendText("параметр11="+FloatToStrF0(MAHndm,6,1)+" ");
    write_tabl_dat("АЧасы_ндм", FloatToStrF0(MAHndm,4,1));//в черный ящик
    break;
  case(8)://сигнал ККС, мВ с шагом 0.1, выводим в ячейку Обор
    Label4->Caption = "ККС";//переименуем ячейку оборотов в ККС
    Cobor->Font->Color=clBlack;//черним параметр
    Cobor->Text=FloatToStrF0(sigKKS,4,1);//выводим в ячейку Обор
    for(int i=0;i<14;i++) Lo->Items->Strings[14-i]=Lo->Items->Strings[13-i];
    Lo->Items->Strings[0]=Cobor->Text;
    //ClientSocket1->Socket->SendText("параметр04="+FloatToStrF0(sigKKS,6,1)+" ");
    write_tabl_dat("ККС", FloatToStrF0(sigKKS,4,1));//в черный ящик
    break;
  case(9)://MAHbatt, А*ч батарейки с шагом 0.1 выводим в ячнйку Времени
    Label15->Font->Color = clBlack;
    Label15->Caption = "Бат%";//переименуем ячейку времени в Батарея
    Cwr->Font->Color=clBlack;//черним параметр
    //Cwr->Text=FloatToStrF0(MAHbatt,4,1);//выводим в ячейку времени
    Cwr->Text=IntToStr((int)MAHbatt);//потраченная емкость батерей в %
    for(int i=0;i<14;i++)Lwr->Items->Strings[14-i]=Lwr->Items->Strings[13-i];
    Lwr->Items->Strings[0]=Cwr->Text;
    //ClientSocket1->Socket->SendText("параметр11="+FloatToStrF0(,6,1)+" ");
    write_tabl_dat("Емк_бат%", FloatToStrF0(MAHbatt,4,1));//в черный ящик
    break;
  case(10)://Маркер
    Label15->Font->Color = clRed;//название красним
    Label15->Caption = "Маркер";//переименуем ячейку времени в Маркер
    Cwr->Font->Color = clRed;//красним параметр
    Cwr->Text=IntToStr(MarkerFreq);
    for(int i=0;i<14;i++)Lwr->Items->Strings[14-i]=Lwr->Items->Strings[13-i];
    Lwr->Items->Strings[0]=Cwr->Text;
    SaveStatusLog("Маркер частоты = " + IntToStr(MarkerFreq), 0);//без красной полосы
    write_tabl_dat("Маркер", IntToStr(MarkerFreq));//в черный ящик
    break;
  }
  //StatusBar->SimpleText="iot="+IntToStr(iot);
}
