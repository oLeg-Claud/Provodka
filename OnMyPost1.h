//Сюда присылает сообщение поток УСО sic4 = номера массива
void __fastcall TFr45::OnMyPost1(TMessage& a)
{
 if(iprov!=0)
 {int numt= a.WParam;
  ytek=(int) a.LParam;
  //StatusBar2->SimpleText="post1 numt="+IntToStr(numt)+", ytek="+IntToStr(ytek)+", fl_buf="+IntToStr(fl_buf)+", NGT="+IntToStr(NGT)+", MGT="+IntToStr(MGT);//------------->>>>>>>>>>>>>
  //Memo1->Lines->Add("post1 numt="+IntToStr(numt)+", ytek="+IntToStr(ytek)+", fl_buf="+IntToStr(fl_buf)+", NGT="+IntToStr(NGT)+", MGT="+IntToStr(MGT));//добавили в memo

  //отрисовываем новую точку из сырого массива mast[numt]
  //если нет флага фильтрованной осциллограммы
  if(!fl_fil)
  {if(uco32)    DIGIT( mast[numt]*((KmB/256)*1000/(pow(2,ur)-1)));//УСО32
   else         DIGIT( mast[numt]*(KmB*1000/(pow(2,ur)-1)));      //УСО24
  }
  //иначе отрисовываем новую точку из фильтрованного массива masf[numt]
  else
  {if(uco32)    DIGIT(masf[numt]*((KmB/256)*1000/(pow(2,ur)-1)));//УСО32
   else         DIGIT( masf[numt]*(KmB*1000/(pow(2,ur)-1)));     //УСО24
  }

  //если включена запись в файл, то будем писать
  if(StatusRecorder==true)
  {
   if(FileSignal==NULL)//откроем файл сигнала, если не открыт еще
   {AnsiString dt,str,path;
	path = tpathckw+"\\smp";
    if(!DirectoryExists(path)) {CreateDirectory(path.c_str(),NULL);}
    DateTimeToString(dt,"yy-mm-dd hh-nn-ss",Now());
    str=path+"\\"+dt+".smp";
    FileSignal = fopen(str.c_str(),"wb");
    //заголовок
    char Zag[2048];
    str = getNastrString();
    strcpy(Zag,str.c_str());
    fwrite(&Zag, sizeof(char),sizeof(Zag),FileSignal);//место под заголовок
    //запишем настройки
    //str=path+"\\"+dt+".txt";//куда писать
    //SaveNastrLog(str);
   }
   int sig = (int)mast[numt];//сэмпл сигнала
   if(!uco32) sig = sig<<8;//из 24 делаем 32
   fwrite(&sig, sizeof(sig),1,FileSignal);//записываем в файл
  }//конец записи сигнала

  //запись сигнала в текстовом виде для Маткада
  /*if(FileSignal2==NULL)
  {AnsiString dt,str,path;
	path = tpathckw+"\\smp";
    if(!DirectoryExists(path)) {CreateDirectory(path.c_str(),NULL);}
    DateTimeToString(dt,"yy-mm-dd hh-nn-ss",Now());
    str=path+"\\"+dt+".smp.txt";
    FileSignal2 = fopen(str.c_str(),"wb");
  }
   int sig = (int)mast[numt];//сэмпл сигнала
   if(!uco32) sig = sig<<8;//из 24 делаем 32
   AnsiString str = IntToStr(sig) + "\r\n";
   //str = "-444\r\n";
   char Str2[20];
   strcpy(Str2,str.c_str());
   fwrite(Str2, str.Length(),1,FileSignal2);//записываем в файл
   */

  // NGT = номер ожидаемого вектора цифры, MGT = ожидаемый индекс в массиве
  // ждем, когда ytek доберется до MGT
  if(NGT!=-1 && ytek>MGT)
  {
   //NGT=номер вектора
   //StatusBa1->SimpleText="NGT="+IntToStr(NGT)+"   ytek="+IntToStr(ytek)+"   MGT="+IntToStr(MGT);
   if(!fl_buf)//похоже, это признак перехода в буфере по кольцу
   {
    //-----------Сообщение потоку 4--------------------
    // от сюда посылаются номер кода, когда все в порядке, настройка произошла
    PostMessage(Fr45->Handle,WM_MyPost4,NGT,MGT);
    //StatusBar1->SimpleText="post1 номер1 NGT="+IntToStr(NGT)+", MGT="+IntToStr(MGT)+", ytek="+IntToStr(ytek)+", DLKADRrp="+IntToStr(DLKADRrp);//------------->>>>>>>>>>>>>
    //Memo1->Lines->Add("post1 номер1 NGT="+IntToStr(NGT)+", MGT="+IntToStr(MGT)+", ytek="+IntToStr(ytek));
    NGT=-1;
    MGT=-1;
   }
   if(fl_buf)
   {if(ytek<DLKADRrp)
    {
     //-----------Сообщение потоку 4--------------------
     PostMessage(Fr45->Handle,WM_MyPost4,NGT,MGT);
     //StatusBar1->SimpleText="post1 номер2 NGT="+IntToStr(NGT)+", MGT="+IntToStr(MGT);//------------->>>>>>>>>>>>>
     //Memo1->Lines->Add("post1 номер2 NGT="+IntToStr(NGT)+", MGT="+IntToStr(MGT));
     NGT=-1;
     MGT=-1;
    }
   }
  }
  //если сообщение не послано
  if(NGT!=-1) if(ytek<DLKADRrp && MGT>DLKADRrp) if(!fl_buf) if(MGT-ytek>DLKADRrp)
  {
   //-----------Сообщение потоку 4--------------------
   PostMessage(Fr45->Handle,WM_MyPost4,NGT,MGT);
   //StatusBar1->SimpleText="post1 номер3 NGT="+IntToStr(NGT)+", MGT="+IntToStr(MGT);//------------->>>>>>>>>>>>>
   NGT=-1;
   MGT=-1;
  }

  if(MCP!=-1 && ytek>MCP)
  {
   //-----------Сообщение потоку 5--------------------
   if(!fl_buf)                  {MCP=-1;PostMessage(Fr45->Handle,WM_MyPost5,numt,ytek);}
   //-----------Сообщение потоку 5--------------------
   if(fl_buf) if(ytek<DLKADRrp) {MCP=-1;PostMessage(Fr45->Handle,WM_MyPost5,numt,ytek);}
  }
  //-----------Сообщение потоку 5----------------
  if(MCP!=-1) if(ytek<DLKADRrp&&MCP>DLKADRrp) if(!fl_buf) if(MCP-ytek>DLKADRrp)
  {MCP=-1;PostMessage(Fr45->Handle,WM_MyPost5,numt,ytek);}
 }
}
