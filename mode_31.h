//========= Вывод цифирей в режиме FORMAT_KADRA=31 =====================================================
if((FORMAT_KADRA==FORMAT_GEOMASH_RETR)&&(DCPt>=DCP))
{
   if(num_kod==FIRST_KOD_R31)
   {
    ClientSocket1->Socket->SendText("параметр09="+Eglubina->Text+" ");
    ClientSocket1->Socket->SendText("freg="+R4->Items->Strings[R4->ItemIndex]+" ");
   }
   //------------- Номер режима и Роль прибора ----------------------------
   if((num_kod==VIBR_H_R31)&& (data_perc[VIBR_H_R31]>=DCP) && (data_perc[FORMAT_R31]>=DCP))
   {
    Spar->Cells[12][2]=IntToStr(num_mode) + RoleStr;//выводим в колонку Режим и Роль
    write_tabl_dat("Режим", Spar->Cells[12][2]);//в черный ящик
    if(num_mode != FORMAT_KADRA)//Если режимы не соответствуют друг другу
    {//ShowMessage("Режим работы скважинного прибора не соответствует приемнику");
     SaveStatusLog("Режим работы скважинного прибора("+IntToStr(num_mode)+")"+" не соответствует настройкам программы("+IntToStr(FORMAT_KADRA)+")", clRed);
     PlaySound("MYS2",0,SND_RESOURCE);
    }
   }
   //------------------- Вибрация -----------------------------------------
   if(	(num_kod==VIBR_L_R31)
   		&&(data_perc[VIBR_L_R31]>=DCP)&&(data_perc[VIBR_H_R31]>=DCP)
     )
   {Cvib->Font->Color=clBlack;
    Cvib->Text=IntToStr(vib);
    for(int i=0;i<14;i++) Lvib->Items->Strings[14-i]=Lvib->Items->Strings[13-i];
    Lvib->Items->Strings[0]=Cvib->Text;
    ClientSocket1->Socket->SendText("параметр07="+FloatToStrF0(vib,6,1)+" ");
    write_tabl_dat("ВИБР_зтс", FloatToStrF0(vib,6,1));//в черный ящик
    Send09record(zts_vibr, IntToStr(vib));//отправляем в WITS
   }
   //------------------- Счетчик кадров -----------------------------------------
   if((num_kod==COUNT_L_R31)&&(data_perc[COUNT_H_R31]>=DCP)&&(data_perc[COUNT_L_R31]>=DCP))
   {//посадим счетчик в колонку H
    Lh->Cells[0][2]=IntToStr(Count_Geomash);//Текущий модуль Н
    j=0; Lhh->Items->Clear();
    for(int i=0;i<Lh->RowCount-2;i++) if(CompareText(Lh->Cells[0][i+2],"")!=0)
                                     {Lhh->Items->Strings[j]=Lh->Cells[0][i+2];j=j+1;}
    j=0; La->Items->Clear();
    for(int i=0;i<L1a->RowCount-2;i++) if(CompareText(L1a->Cells[0][i+2],"")!=0)
                                      {La->Items->Strings[j]=L1a->Cells[0][i+2]; j=j+1;}
    j=0; Lkopp->Items->Clear();
    for(int i=0;i<Lakopp->RowCount-2;i++) if(CompareText(Lakopp->Cells[0][i+2],"")!=0)
                                      {Lkopp->Items->Strings[j]=Lakopp->Cells[0][i+2];j=j+1;}

    write_tabl_dat("Htotal", IntToStr(Count_Geomash));//в черный ящик
    //ClientSocket1->Socket->SendText("параметр07="+FloatToStrF0(vib,6,1)+" ");
    //Send09record(zts_vibr, IntToStr(vib));//отправляем в WITS
   }
   //------------------- Payload Геомаш -----------------------------------------
   //на последнем коде
   if(num_kod==(DLINA_CODA_R31-1-1))
   {	int percent=0;
   		if(data_perc[COUNT_H_R31] < DCP) percent++;
        if(data_perc[COUNT_L_R31] < DCP) percent++;
        for(int i=BEGIN_PLD_R31; i<DLINA_CODA_R31-1; i++) if(data_perc[i] < DCP) percent++;
        if(percent==0)//если все принято корректно
        {	GEOMASH.Header = 0xFF;
        	GEOMASH.Version = 0x01;
            GEOMASH.Number = Count_Geomash;
            long long time = getUnixTimeMs();//получаем Unix в мс
			GEOMASH.Moment = time - getTimeShiftGeomash(FORMAT_KADRA, freq);//вычитаем время кадра
            //соединим 5-ки бит в байты
            int res = join_NBbit_arr(&dig[BEGIN_PLD_R31], DLINA_CODA_R31-1-BEGIN_PLD_R31, &GEOMASH.PayLoad[0]);
            if(res != sizeof(GEOMASH.PayLoad)) ShowMessage("Ошибка: кол-во векторов в Payload="+IntToStr(res));
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

            //перед посылкой в сокет свапим большие слова
            GEOMASH.Number = swap32(GEOMASH.Number);//Big endian
            GEOMASH.Moment = swapDouble(GEOMASH.Moment);//Big endian
            //вычислим CRC32
            GEOMASH.CRC32 = crc32_poly((uint8_t*)&GEOMASH.Header, sizeof(GEOMASH_type)-sizeof(GEOMASH.CRC32));
            GEOMASH.CRC32 = swap32(GEOMASH.CRC32);//Big endian

    		SendToWitsSocket(&GEOMASH.Header, sizeof(GEOMASH));//в Геомаш через WITS-клиент
        }
   }
}
//------- FORMAT_KADRA=31 -----------------------------------------------------


