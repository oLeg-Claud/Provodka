//====================================================================================
void __fastcall TFr45::PBClick(TObject *Sender)
{
    PbCheck=PB->Checked;
    if(PbCheck)
    {Lport2->Visible=true;
     Sport2->Visible=true;
     PBRadioGroup->Visible=true;
     PBType->Visible=true;
     if(PBType->ItemIndex == 1) RotateButton->Visible=true;
     Label31->Caption = IntToStr(PBrotation);
     Label31->Visible=true;
    }
    else
    {Lport2->Visible=false;
     Sport2->Visible=false;
     PBRadioGroup->Visible=false;
     PBType->Visible=false;
     RotateButton->Visible=false;
     Label31->Visible=false;
     Diod->Visible=false;
    }
}
//====================================================================================
void __fastcall TFr45::PBTypeClick(TObject *Sender)
{
if(PBType->ItemIndex == 1) {RotateButton->Visible=true; Label31->Visible=true;}
else {RotateButton->Visible=false; Label31->Visible=false;}
}
//====================================================================================
//кнопка вращения дисплея
void __fastcall TFr45::RotateButtonClick(TObject *Sender)
{
	PBrotation = ++PBrotation%4;//следующий поворот на 90
    Label31->Caption = IntToStr(PBrotation);
    SendByteQpb_rfd(SET_DISP_ROT, PBrotation);//посылаем команду вращения
}
//====================================================================================
void __fastcall TFr45::Sport2Change(TObject *Sender)
{
    NCOMPB=Sport2->Text;
    int j=LastDelimiter("(",NCOMPB);//найдем индекс скобки
    if(j>0) NCOMPBh=Trim(NCOMPB.SubString(1,j-1));//обрежем до скобки и уберем пробелы
    else  NCOMPBh=NCOMPB;
}
//====================================================================================
void __fastcall TFr45::Sport2DropDown(TObject *Sender)
{
int ssn=0;
int i,ncom;

 Fr45->StatusBar->Color=clWhite;//Очищаем статус строку
 Fr45->StatusBar->SimpleText="";
 Sport2->Items->Clear();

 int majorVersion = Win32MajorVersion;//определяем версию windows, 5=XP
 //если старше XP, то через cmd
 if(majorVersion>5) ncom=port_list_cmd();//получим имена портов через cmd, лежат в MemoLas
 else ncom=port_list_reg();//иначе через реестр

 for (int i = 0; i< ncom; i++)
 {
   N_USO2(MemoLas->Lines->Strings[i],&ssn);
   if(ssn==0xFD) Sport2->Items->Add(MemoLas->Lines->Strings[i]+" (усоПБ)");
   else if(ssn==0xAA) Sport2->Items->Add(MemoLas->Lines->Strings[i]+" (RFD)");
   else  Sport2->Items->Add(MemoLas->Lines->Strings[i]+" (?)");
 }
 Fr45->MemoLas->Lines->Clear();

 for(int i=0;i<Sport2->Items->Count;i++)
 if(CompareStr(NCOMPB.SubString(1,8),Sport2->Items->Strings[i].SubString(1,8)) == 0)
 {
   Sport2->ItemIndex=i;
   Sport2->Text=Sport2->Items->Strings[i];
   NCOMPB=Sport2->Items->Strings[i];
   int j=LastDelimiter("(",NCOMPB);//найдем индекс скобки
   if(j>0) NCOMPBh=Trim(NCOMPB.SubString(1,j-1));//обрежем до скобки и уберем пробелы
   else  NCOMPBh=NCOMPB;
 }
}
//====================================================================================
//проба COM-порта на свой-чужой
//====================================================================================
void N_USO2(AnsiString w,int *ssn)
{
HANDLE iport=NULL;
int iierr;
COMMTIMEOUTS  cto;// {структура тайм-аутов COM-порта}
DCB    cdcb;// {блок параметров COM-порта}
unsigned char buf[]={255,255,255,253};//FF FF FF FD
unsigned char buf2[]={0x6A,0x00,0x06,0x00,0x8F,0xFF};//получить позицию экрана
//unsigned char buf[]={1,1,0,2,40,1,94,135,255,255,255,253};// -> AZ=350
unsigned long kolb;
typedef union u16 {unsigned short s; char c[2];} u16;
u16 num;

 iierr=0;
 *ssn=0;
 while(PotokPBON || BusyPB) Application->ProcessMessages();//ждем, если поток работает
 BusyPB = 1;//занимаем порт
 //инициализация порта w
 if(iport=initPortPB(w))
 {
    Fr45->Diod->Color=clRed;
    Fr45->Diod->Visible=true;//зажжем индикатор

    if(Fr45->PBType->ItemIndex == 0)//если наш ПБ
    {   WriteFile(iport,buf,sizeof(buf),&kolb,NULL);
   	    if(kolb!=sizeof(buf)) ShowMessage("нет передачи "+w);
   	    buf[3]=0;
   	    int count=0;
   	    while(buf[3]!=0xFD)//ловим пачку конца пакета в ответе
   	    {	ReadFile(iport,buf,sizeof(buf),&kolb,NULL);
            if(kolb!=sizeof(buf)) {*ssn=0; break;} //если вообще неответ
                if((buf[sizeof(buf)-1]==0xFD)) {*ssn=0xFD; break;} else {*ssn=0;}
            if(++count>=5) break;//не более 5ти раз
   	    }
    }
    else if(Fr45->PBType->ItemIndex == 1)//если не наш ПБ
    {   //адрес, мл. вперед
    	uint16_t comm = GET_DISP_POS;//прочитать позицию дисплея
    	num.s = comm;
    	buf2[0] = num.c[0];
    	buf2[1] = num.c[1];
    	//длина, мл. вперед
    	num.s = sizeof(buf2);
    	buf2[2] = num.c[0];
    	buf2[3] = num.c[1];
    	//CRC
    	num.s = crc16_simple (buf2, sizeof(buf2)-2);
    	buf2[4] = num.c[0];
    	buf2[5] = num.c[1];
    	WriteFile(iport,buf2,sizeof(buf2),&kolb,NULL);
   	    if(kolb!=sizeof(buf2)) ShowMessage("нет передачи "+w);
        kolb = 0;
   	    ReadFile(iport,buf,6,&kolb,NULL);
        if(kolb != 6) {*ssn=0;} //если вообще неответ
        else if((buf[0]==(char)comm)) {PBposition = buf[1]&0x0F; *ssn=0xAA;}//позиция дисплея
        else {*ssn=0;}
    }

   	Fr45->Diod->Visible=false;//потушим индикатор

 	PurgeComm(iport,PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);//очистка буферов
  	CloseHandle(iport);
 }
 BusyPB = 0;//освобождаем порт
}
//====================================================================================
//инициализировать COM-порт
//====================================================================================
HANDLE initPortPB(AnsiString NUM)
{
//int ssn=0;
HANDLE iport;
COMMTIMEOUTS  cto;// {структура тайм-аутов COM-порта}
DCB    cdcb;// {блок параметров COM-порта}
    //N_USO2(NUM,&ssn);
    //if(ssn != 0xFD) return NULL;
    NUM= "\\\\.\\"+NUM;
    iport=CreateFile(NUM.c_str(),GENERIC_READ | GENERIC_WRITE ,0,NULL,OPEN_EXISTING,0,NULL);
    if(iport==INVALID_HANDLE_VALUE) return NULL;
    if(!GetCommTimeouts(iport,&cto)) return NULL;
    if(Fr45->PBType->ItemIndex == 0) cto.ReadTotalTimeoutConstant=100; else cto.ReadTotalTimeoutConstant=1000;
    cto.WriteTotalTimeoutConstant=100;
    if(!SetCommTimeouts(iport,&cto)) return NULL;
    if (!GetCommState(iport,&cdcb)) return NULL;
    cdcb.BaudRate=CBR_9600;//скорость передачи данных
    cdcb.ByteSize=8; //число информационных бит
    cdcb.Parity=NOPARITY;//бит четности отсутствует
    cdcb.StopBits=ONESTOPBIT;//один стоnовый бит
    if (!SetCommState(iport,&cdcb)) return NULL;
    PurgeComm(iport,PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);//очистить все
    SetCommMask(iport,EV_TXEMPTY);//отслеживать последний переданный символ
    return iport;
}
//====================================================================================
//Передать один параметр по новому в наш пульт
//====================================================================================
void SendOneQpb(unsigned char adr, float val)
{
//unsigned char buf[12]={1,1,0,2,0,0,0,0,255,255,255,253};
qStruc buf1[3];
qStruc comm={1,1,0,2};
qStruc end={255,255,255,253};
qStruc len={255,3,255,3};//так будем передавать длину = 3
//1 1 0 2 2A 01 0E 39 FF FF FF FD -> OTK=270
//1 1 0 2 28 01 5E 87 FF FF FF FD -> AZ=350
//1 1 0 2 29 00 62 8B FF FF FF FD -> ZU=98
typedef union u16 {unsigned short s; char c[2];} u16;
u16 num;
    if(adr>3 && adr<40) return;//левый адрес
    if(adr>42) return;//левый адрес
    //extern int error_portPB;
    //if(error_portPB) {error_portPB=0; while(!Qpb.empty()) Qpb.pop();}

     switch(adr)
     {
      //Момент, кН - float
      case(wMOM): num.s=round(val*10); break;//делаем с одним знаком после запятой
      //Обороты, об/мин - беззнаковое целое 9 разрядов
      case(wOBOR): num.s=(int)val; break;
      //Давление, Мпа - float
      case(wDAVL): num.s=round(val*9.8692); break;//переводим в целые атм
      //Нагрузка, тонны - float
      case(wNAGR): num.s=round(val*10); break;//делаем один знак после запятой
      //Азимут - float
      case(wAZ): num.s=(int)val; break;
      //Зенит - float
      case(wZU): num.s=round(val*10); break;//делаем один знак после запятой
      //ОТК - int
      case(wOTK): num.s=(int)val; break;
     }
     //num.s=(int)val;

     //загружаем команду 4 байта
     buf1[0]=comm;
     //загружаем параметр = 4 байта
     buf1[1].mas[0]=adr;//адрес
     buf1[1].mas[1]=num.c[1];//старшая часть числа
     buf1[1].mas[2]=num.c[0];//младшая часть числа
     buf1[1].mas[3]=adr+num.c[0]+num.c[1];//КС
     //загружаем конец 4 байта
     buf1[2]=end;

     /*AnsiString str="";
     for(int n=0;n<4;n++) str+=" "+IntToHex(buf1[0].mas[n], 2);
     for(int n=0;n<4;n++) str+=" "+IntToHex(buf1[1].mas[n], 2);
     for(int n=0;n<4;n++) str+=" "+IntToHex(buf1[2].mas[n], 2);
     SaveStatusLog("собрали пакет в очередь: "+str, clWhite);*/

     //Теперь отправим массив в очередь на передачу
     EnterCriticalSection( &cs );
     //------------------------------------
     Qpb.push(len);//первый элемент - кол-во параметров, включая команду и конец
     for(int n=0;n<(sizeof(buf1)/sizeof(qStruc));n++) Qpb.push(buf1[n]);
     //------------------------------------
     LeaveCriticalSection( &cs );
     //запускаем поток, если закрыт
     if(PotokPBON==0) {PBThr = new PBThread(false); while(!PotokPBON) Application->ProcessMessages();}
}
//====================================================================================
//Передать один параметр через очередь в чужой пульт
//====================================================================================
void SendOneQpb_rfd(unsigned short adr, float val)
{
qStruc buf1[10];
qStruc len={255,10,255,10};//так будем передавать длину = 10
uint8_t bufbyte[10];
typedef union u16 {unsigned short s; char c[2];} u16;
typedef union u32 {float f; char c[4];} u32;
u16 num;
u32 par;
    if(adr>SET_OTK_MODE && adr<SET_ZU) return;//левый адрес
    //extern int error_portPB;
    //if(error_portPB) {error_portPB=0; while(!Qpb.empty()) Qpb.pop();}

    //сложим в байтовый массив
    //адрес, мл. вперед
    num.s = adr;
    bufbyte[0] = num.c[0];
    bufbyte[1] = num.c[1];
    //длина, мл. вперед
    num.s = sizeof(bufbyte);
    bufbyte[2] = num.c[0];
    bufbyte[3] = num.c[1];
    //параметр, мл. вперед
    par.f = val;
    bufbyte[4] = par.c[0];
    bufbyte[5] = par.c[1];
    bufbyte[6] = par.c[2];
    bufbyte[7] = par.c[3];
    //CRC
    num.s = crc16_simple (bufbyte, sizeof(bufbyte)-2);
    bufbyte[8] = num.c[0];
    bufbyte[9] = num.c[1];

    //Теперь отправим массив в очередь на передачу
    EnterCriticalSection( &cs );
    //------------------------------------
    Qpb.push(len);//первый элемент - кол-во параметров, включая команду и конец
    //переносим байты в очередь по одному в эл-т
    for(int i=0;i<(sizeof(bufbyte));i++)
    {
        buf1[i].mas[0]=0;
        buf1[i].mas[1]=0;
        buf1[i].mas[2]=0;
        buf1[i].mas[3]=bufbyte[i];
        Qpb.push(buf1[i]);//кладем в очередь
    }
    //------------------------------------
    LeaveCriticalSection( &cs );
    //запускаем поток, если закрыт
    if(PotokPBON==0) {PBThr = new PBThread(false); while(!PotokPBON) Application->ProcessMessages();}

    //спросим положение дисплея, и если не то, то повернем в нужную позицию
    if(PBposition != PBrotation)//если позиция экрана не та
    {	SendByteQpb_rfd(SET_DISP_ROT, PBrotation);//поворачиваем экран куда надо
    }
    SendCommQpb_rfd(GET_DISP_POS);//обновить текущую позицию дисплея для сл.раза
}
//====================================================================================
//Передать один байт через очередь в чужой пульт
//====================================================================================
void SendByteQpb_rfd(uint16_t adr, uint8_t val)
{
qStruc buf1[7];
qStruc len={255,7,255,7};//так будем передавать длину = 7
uint8_t bufbyte[7];
typedef union u16 {unsigned short s; char c[2];} u16;
u16 num;
    //extern int error_portPB;
    //if(error_portPB) {error_portPB=0; while(!Qpb.empty()) Qpb.pop();}

    //сложим в байтовый массив
    //адрес, мл. вперед
    num.s = adr;
    bufbyte[0] = num.c[0];
    bufbyte[1] = num.c[1];
    //длина, мл. вперед
    num.s = sizeof(bufbyte);
    bufbyte[2] = num.c[0];
    bufbyte[3] = num.c[1];
    //параметр, мл. вперед
    bufbyte[4] = val;
    //CRC
    num.s = crc16_simple (bufbyte, sizeof(bufbyte)-2);
    bufbyte[5] = num.c[0];
    bufbyte[6] = num.c[1];

    //Теперь отправим массив в очередь на передачу
    EnterCriticalSection( &cs );
    //------------------------------------
    Qpb.push(len);//первый элемент - кол-во параметров, включая команду и конец
    //переносим байты в очередь по одному в эл-т
    for(int i=0;i<(sizeof(bufbyte));i++)
    {
        buf1[i].mas[0]=0;
        buf1[i].mas[1]=0;
        buf1[i].mas[2]=0;
        buf1[i].mas[3]=bufbyte[i];
        Qpb.push(buf1[i]);//кладем в очередь
    }
    //------------------------------------
    LeaveCriticalSection( &cs );
    //запускаем поток, если закрыт
    if(PotokPBON==0) {PBThr = new PBThread(false); while(!PotokPBON) Application->ProcessMessages();}
}
//====================================================================================
//Передать только команду через очередь в чужой пульт
//====================================================================================
void SendCommQpb_rfd(uint16_t adr)
{
qStruc buf1[6];
qStruc len={255,6,255,6};//так будем передавать длину = 7
uint8_t bufbyte[6];
typedef union u16 {unsigned short s; char c[2];} u16;
u16 num;
    //extern int error_portPB;
    //if(error_portPB) {error_portPB=0; while(!Qpb.empty()) Qpb.pop();}

    //сложим в байтовый массив
    //адрес, мл. вперед
    num.s = adr;
    bufbyte[0] = num.c[0];
    bufbyte[1] = num.c[1];
    //длина, мл. вперед
    num.s = sizeof(bufbyte);
    bufbyte[2] = num.c[0];
    bufbyte[3] = num.c[1];
    //CRC
    num.s = crc16_simple (bufbyte, sizeof(bufbyte)-2);
    bufbyte[4] = num.c[0];
    bufbyte[5] = num.c[1];

    //Теперь отправим массив в очередь на передачу
    EnterCriticalSection( &cs );
    //------------------------------------
    Qpb.push(len);//первый элемент - кол-во параметров, включая команду и конец
    //переносим байты в очередь по одному в эл-т
    for(int i=0;i<(sizeof(bufbyte));i++)
    {
        buf1[i].mas[0]=0;
        buf1[i].mas[1]=0;
        buf1[i].mas[2]=0;
        buf1[i].mas[3]=bufbyte[i];
        Qpb.push(buf1[i]);//кладем в очередь
    }
    //------------------------------------
    LeaveCriticalSection( &cs );
    //запускаем поток, если закрыт
    if(PotokPBON==0) {PBThr = new PBThread(false); while(!PotokPBON) Application->ProcessMessages();}
}
//====================================================================================
//Чистить Пульт Бурильщика по новому
//====================================================================================
void clearPB(void)
{
unsigned char adr[]={wMOM,wOBOR,wDAVL,wNAGR,wAZ,wZU,wOTK};
	//отправим во все параметры по очереди
    int data0[]={0,0,0,0,0,0,0};//числа, которые будем отправлять (очистка)
    for(int i=0;i<sizeof(adr);i++) SendOneQpb(adr[i],(float)data0[i]);
    //int data1[]={10,20,30,40,50,60,70};//числа, которые будем отправлять (очистка)
    //for(int i=0;i<sizeof(adr);i++) SendOneQpb(adr[i],(float)data1[i]);
}
//====================================================================================
void setNastrBufPB(qStruc* buf, uint16_t min, uint16_t max, uint8_t point, uint8_t bright)
{
typedef union u16 {unsigned short s; char c[2];} u16;
u16 num;
int j=0;
    //минимум
	num.s=min; buf[j].mas[0]=0; buf[j].mas[1]=num.c[1]; buf[j].mas[2]=num.c[0]; buf[j].mas[3]=buf[j].mas[0]+buf[j].mas[1]+buf[j].mas[2]; j++;
    //максимум
    num.s=max; buf[j].mas[0]=1; buf[j].mas[1]=num.c[1]; buf[j].mas[2]=num.c[0]; buf[j].mas[3]=buf[j].mas[0]+buf[j].mas[1]+buf[j].mas[2]; j++;
    //точка
    num.s=point; buf[j].mas[0]=2; buf[j].mas[1]=num.c[1]; buf[j].mas[2]=num.c[0]; buf[j].mas[3]=buf[j].mas[0]+buf[j].mas[1]+buf[j].mas[2]; j++;
    //множитель
    num.s=0; buf[j].mas[0]=3; buf[j].mas[1]=num.c[1]; buf[j].mas[2]=num.c[0]; buf[j].mas[3]=buf[j].mas[0]+buf[j].mas[1]+buf[j].mas[2]; j++;
    //смещение
    num.s=0; buf[j].mas[0]=4; buf[j].mas[1]=num.c[1]; buf[j].mas[2]=num.c[0]; buf[j].mas[3]=buf[j].mas[0]+buf[j].mas[1]+buf[j].mas[2]; j++;
    //яркость
    buf[j].mas[0]=5; buf[j].mas[1]=bright; buf[j].mas[2]=0; buf[j].mas[3]=buf[j].mas[0]+buf[j].mas[1]+buf[j].mas[2]; j++;
    //режим
    num.s=0; buf[j].mas[0]=6; buf[j].mas[1]=num.c[1]; buf[j].mas[2]=num.c[0]; buf[j].mas[3]=buf[j].mas[0]+buf[j].mas[1]+buf[j].mas[2]; j++;
}
//====================================================================================
//Настраиваем масштабы и точки в дисплее ПБ
//эта команда пока не работает корректно
void setDisplayPB(void)
{
uint8_t adr[]={wMOM,wOBOR,wDAVL,wNAGR,wAZ,wZU,wOTK};
//uint8_t adr[]={wDAVL};
qStruc buf[9];
qStruc comm={0x0A,0,0,0};
qStruc end={255,255,255,253};
qStruc len={255,9,255,9};//так будем передавать длину = comm+7data+end=9
    //extern int error_portPB;
    //if(error_portPB) {error_portPB=0; while(!Qpb.empty()) Qpb.pop();}

	for(int i=0;i<sizeof(adr);i++)
    {   buf[0].mas[0]=0x0A; buf[0].mas[1]=adr[i];//команда WIN, остальное после
        switch(adr[i])
     	{
      	//Момент, кН - делаем с одним знаком после запятой
      	case(wMOM): setNastrBufPB(&buf[1],0,30,2,255); break;
      	//Обороты, об/мин - беззнаковое целое 9 разрядов
      	case(wOBOR): setNastrBufPB(&buf[1],0,50,0,255); break;//индикатор Расхода
      	//Давление, Мпа - переводим в целые атм
      	case(wDAVL): setNastrBufPB(&buf[1],0,250,0,0); break;
      	//Нагрузка, тонны - делаем один знак после запятой
      	case(wNAGR): setNastrBufPB(&buf[1],0,30,2,255); break;
      	//Азимут - целое
      	case(wAZ): setNastrBufPB(&buf[1],0,400,0,255); break;
      	//Зенит - целое
      	case(wZU): setNastrBufPB(&buf[1],0,400,0,255); break;
      	//ОТК - int
      	case(wOTK): setNastrBufPB(&buf[1],0,400,0,255); break;
     	}
        //КС по данным
        buf[0].mas[2]=0;
        for(int n=0;n<7;n++) buf[0].mas[2] += buf[n+1].mas[3];
        //КС команды
        buf[0].mas[3]= buf[0].mas[0]+buf[0].mas[1]+buf[0].mas[2];
        //конец
        buf[8]=end;
        //Теперь отправим массив в очередь на передачу
        EnterCriticalSection( &cs );
        //------------------------------------
     	Qpb.push(len);//первый элемент - кол-во параметров, включая команду и конец
     	for(int n=0;n<(sizeof(buf)/sizeof(qStruc));n++) Qpb.push(buf[n]);
        //------------------------------------
        LeaveCriticalSection( &cs );
     	//запускаем поток, если закрыт
     	if(PotokPBON==0) {PBThr = new PBThread(false); while(!PotokPBON) Application->ProcessMessages();}
    }
}
//====================================================================================
//пауза между передачами в ПБ
//====================================================================================
void __fastcall TFr45::Timer9Timer(TObject *Sender)
{
  Timer9->Enabled=false;
  BusyPB=0;
  //if(Fr45->Diod->Color == clRed) Diod->Visible=false;//выключим индикатор
}
//====================================================================================
void __fastcall TFr45::Sport2KeyPress(TObject *Sender, char &Key)
{
Fr45->StatusBar->Color=clWhite;
Fr45->StatusBar->SimpleText="";    
}
//====================================================================================
//получает список портов, возвращает кол-во ком-портов, имена портов - в MemoLas
//====================================================================================
int port_list_cmd(void)
{
int count_string, p, i, count_com=0;

    HANDLE iport=NULL;
    //создаем временный файл
    iport=CreateFile("out.txt",
                     GENERIC_READ | GENERIC_WRITE,
                     FILE_SHARE_READ | FILE_SHARE_WRITE,
                     NULL,
                     CREATE_ALWAYS,
                     FILE_ATTRIBUTE_TEMPORARY, //| FILE_FLAG_DELETE_ON_CLOSE,
                     NULL);
    if(iport!=NULL)
    {CloseHandle(iport);
     while(FileExists("out.txt")==false) Application->ProcessMessages();
    }

    ShellExecute(0, "open", "cmd.exe", "/c mode >> out.txt", 0, SW_HIDE);//добавляем в файл
    Sleep((unsigned long)300);
    FILE *out1;
    while((out1=fopen("out.txt","a"))==NULL) Application->ProcessMessages();//ждем освобождения
    fclose(out1);
    Fr45->MemoLas->Lines->Clear();
    Fr45->MemoLas->Lines->LoadFromFile("out.txt");
    DeleteFile("out.txt");
    count_string=Fr45->MemoLas->Lines->Count;//кол-во строк
    AnsiString s1, s2="";
    for(i=0;i<count_string;i++)
    {
     s1=Fr45->MemoLas->Lines->Strings[i];
     p=s1.Pos("COM");
     if(p>0)
     {
      s1.Delete(1,p-1);
      p=s1.Pos(":");//узнаем, где заканчивается имя порта
      s2+=s1.SubString(1,p);//соберем имена в строку с разделителем
      count_com++;
     }
    }
    Fr45->MemoLas->Lines->Clear();
    for(i=0;i<count_com;i++)
    {
     while(s2.Pos(":")==1) s2.Delete(1,1);//удалим разделитель слева
     p=s2.Pos(":");//узнаем, где заканчивается имя
     Fr45->MemoLas->Lines->Add(s2.SubString(1,p-1));//сохраняем строки с именами портов
     s2.Delete(1,p);
    }
    return count_com;
}
//====================================================================================
//получает список портов, возвращает кол-во ком-портов, имена портов - в MemoLas
//====================================================================================
int port_list_reg(void)
{
  TRegistry* Reg = new TRegistry;
  TStringList *List = new TStringList;
  Reg->RootKey=HKEY_LOCAL_MACHINE;
  Reg->OpenKey("HARDWARE\\DEVICEMAP\\SERIALCOMM", false);
  Reg->GetValueNames(List);//загрузим все строки
  int ncom=List->Count;//кол-во строк
  Fr45->MemoLas->Lines->Clear();
  for (int i = 0; i< ncom; i++)
  {
   if(CompareText(List->Strings[i].SetLength(8),"\\Device\\")==0 && CompareText(Reg->ReadString(List->Strings[i]).SetLength(3),"COM")==0)//если значение имеет COM
    Fr45->MemoLas->Lines->Add(Reg->ReadString(List->Strings[i]));
  }
  Reg->CloseKey();
  List->Free();
  Reg->Free();
  return Fr45->MemoLas->Lines->Count;
}
//====================================================================================

