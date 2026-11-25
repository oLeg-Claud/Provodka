//сюда присылает сообщение поток 2 и поток 4
void __fastcall TFr45::OnMyPost3(TMessage& a)
{
 static int minus=0;
 int j, k;
 int num_kod=a.WParam;//номер вектора в абсолютах
 int indl=a.LParam;
 int skv=0,dpsk=0;
 float tokndm1;
 sdvig=0;
 int indt=indl-iG;
 if(indt<0) indt+=DLKADRr;//DLKADRr=длина кадра в индексах кольцевого массива
 mn=90.0-get_umn();
 zu=MyStrToFloat(Sn->Cells[1][2]);
 Hop=StrToInt(Sn->Cells[1][5]);
 Lh->Cells[0][1]=IntToStr(Hop);
 Kgeo=MyStrToFloat(Sn->Cells[1][9]);
 Kgk_ndm=MyStrToFloat(Sn->Cells[1][11]);
 Kgk_zts=MyStrToFloat(Sn->Cells[1][17]);
 maxgk=(int)(mgk*26);
 if(num_kod==0) minus=0;//самый первый вектор
 int sir=num_kod;
 extern GEOMASH_type GEOMASH;

    //max_pause_format[] уже приведен к абсолютной индексации в кадре
    if(FORMAT_KADRA==FORMAT_NDM_GK_RETR)
    {for(int i=0;i<NUM_PAUSE_R2;i++)//кол-во блоков пауз в кадре
     {if(num_kod > mas_pause_format2[i] && num_kod < (mas_pause_format2[i]+num_pause_vectors))//индекс начала пауз в кадре
      {minus++; break;}
     }
    }
    if(FORMAT_KADRA==FORMAT_GK_RETR)
    {for(int i=0;i<NUM_PAUSE_R11;i++)//кол-во блоков пауз в кадре
     {if(num_kod > mas_pause_format11[i] && num_kod < (mas_pause_format11[i]+num_pause_vectors))//индекс начала пауз в кадре
      {minus++; break;}
     }
    }
    if(FORMAT_KADRA==FORMAT_STD_RETR)
    {for(int i=0;i<NUM_PAUSE_R13;i++)//кол-во блоков пауз в кадре
     {if(num_kod > mas_pause_format13[i] && num_kod < (mas_pause_format13[i]+num_pause_vectors))//индекс начала пауз в кадре
      {minus++; break;}
     }
    }
    if(FORMAT_KADRA==FORMAT_GEOMASH_RETR)
    {for(int i=0;i<NUM_PAUSE_R31;i++)//кол-во блоков пауз в кадре
     {if(num_kod > mas_pause_format31[i] && num_kod < (mas_pause_format31[i]+num_pause_vectors))//индекс начала пауз в кадре
      {minus++; break;}
     }
    }
    num_kod -= minus;
    //StatusBar->SimpleText="indl="+IntToStr(indl);
    //StatusBar->SimpleText="num_kod="+IntToStr(num_kod)+", minus="+IntToStr(minus);

 kordat4_8(indt,num_kod,&zenit,&azimut,&otklonf,&obor,&d1,&tem,&sdvig,&d2,&vib,&gk,&tok,&zu,&mn,&az_otk,&modg,&skv,&dpsk);

 if(Sv->Checked&&DCPt>=DCP&&dpsk>=DCP)  ClientSocket1->Socket->SendText("skv"+IntToStr(num_kod)+"="+IntToStr(skv)+" ");
 C2->Title->Text->Strings[1]=C2->Title->Text->Strings[1]+IntToStr(dproc[num_kod+1])+",";//Выводим проценты данных

 //------------проверка напряжения
 nap=MyStrToFloat(Sn->Cells[1][6]);
 //определяем приход Напряжения
 if((FORMAT_KADRA==FORMAT_STD_GK)&&((num_kod==OTK3_H_R1&&iot==2)&&(DCPt>=DCP)&&(data_perc[OTK1_H_R1]>=DCP)&&(data_perc[OTK2_H_R1]>=DCP)&&(data_perc[OTK3_H_R1]>=DCP)&&(data_perc[AZ_OTK1_H_R1]>=DCP)&&(data_perc[AZ_OTK2_H_R1]>=DCP))/*режим 1*/
    ||
    (FORMAT_KADRA==FORMAT_NDM_GK_RETR)&&((num_kod==OTK3_H_R2&&iot==2)&&(DCPt>=DCP)&&(data_perc[OTK1_H_R2]>=DCP)&&(data_perc[OTK2_H_R2]>=DCP)&&(data_perc[OTK3_H_R2]>=DCP)&&(data_perc[AZ_OTK1_H_R2]>=DCP)&&(data_perc[AZ_OTK2_H_R2]>=DCP))/*режим 2*/
    ||
    (FORMAT_KADRA==FORMAT_GK_RETR)&&((num_kod==OTK3_H_R11&&iot==2)&&(DCPt>=DCP)&&(data_perc[OTK1_H_R11]>=DCP)&&(data_perc[OTK2_H_R11]>=DCP)&&(data_perc[OTK3_H_R11]>=DCP)&&(data_perc[AZ_OTK1_H_R11]>=DCP)&&(data_perc[AZ_OTK2_H_R11]>=DCP))/*режим 1*/
    ||
    (FORMAT_KADRA==FORMAT_STD)&&((num_kod==OTK3_H_R12&&iot==2)&&(DCPt>=DCP)&&(data_perc[OTK1_H_R12]>=DCP)&&(data_perc[OTK2_H_R12]>=DCP)&&(data_perc[OTK3_H_R12]>=DCP)&&(data_perc[AZ_OTK1_H_R12]>=DCP)&&(data_perc[AZ_OTK2_H_R12]>=DCP))/*режим 12*/
    ||
    (FORMAT_KADRA==FORMAT_STD_RETR)&&((num_kod==OTK3_H_R13&&iot==2)&&(DCPt>=DCP)&&(data_perc[OTK1_H_R13]>=DCP)&&(data_perc[OTK2_H_R13]>=DCP)&&(data_perc[OTK3_H_R13]>=DCP)&&(data_perc[AZ_OTK1_H_R13]>=DCP)&&(data_perc[AZ_OTK2_H_R13]>=DCP))/*режим 13*/
   )
 {
  if(NAPR < nap)//если меньше порога
  {SaveStatusLog("НЕДОПУСТИМОЕ НАПРЯЖЕНИЕ СТАТИКИ!", clRed);
   PlaySound("MYS1",0,SND_RESOURCE);
  }
 }
 //--------------- коррекция
 Panel2->Width=dcpm+dcpl*(num_kod+1);//Бегунок
 if(CompareText(Sn->Cells[1][0],"")!=0) mrmt=StrToInt(Sn->Cells[1][0]);
 else mrmt=0;

 if( DCPt<DCP)//если слабый сигнал
 {Label7->Color=clRed;//красный
  Label7->Caption="Низкое качество сигнала";
  Label7->Visible=true;
 }
 else
 {
  Label7->Color=clLime;//зеленый
  Label7->Caption="СТАТИКА!";
  if(ctat) Label7->Visible=true;//видимость
  else Label7->Visible=false;
  Label6->Caption="Напр";
 }
//==================================================================================================
 //Пережвижка строк таблицы НДМ во всех режимах
 if((num_kod==0)&&(DCPt>=DCP))
 {for(j=0;j<Spar->ColCount;j++) for(int i=Spar->RowCount-1;i>=1;i--) Spar->Cells[j][i+1]=Spar->Cells[j][i];
  for(int i=0;i<Spar->ColCount;i++) Spar->Cells[i][1]="";//очищаем колонки
  for(int i=Spar->RowCount-2;i>=1;i--) masQualNDM[i+1]=masQualNDM[i];//передвигаем массив качества НДМ
  masQualNDM[1]=0;//чистим верхний элемент
 }
//==================================================================================================
 //Передвижка строк в колонках угловых параметров во всех режимах
 if((num_kod==0)&&(DCPt>=DCP))
 {	for(int i=L1->RowCount-2;i>=2;i--) masQuality[i+1]=masQuality[i];//массив качества
    for(int i=L1->RowCount-2;i>=2;i--) {L1->Cells[0][i+1]=L1->Cells[0][i];}//ЗУ
    for(int i=L1a->RowCount-2;i>=2;i--) {L1a->Cells[0][i+1]=L1a->Cells[0][i];}//АЗ
    for(int i=Lakopp->RowCount-2;i>=2;i--) {Lakopp->Cells[0][i+1]=Lakopp->Cells[0][i];}//АЗкорр
    for(int i=Lh->RowCount-2;i>=2;i--) {Lh->Cells[0][i+1]=Lh->Cells[0][i];}//модуль H
    MemoUgol->Lines->Add("Конец");
    for(int i=MemoUgol->Lines->Count-1;i>0;i--)//строки Все углы
    {   AnsiString ss=MemoUgol->Lines->Strings[i-1];
    	MemoUgol->Lines->Strings[i]=ss;
    }
    if(MemoUgol->Lines->Count >= rowCount) MemoUgol->Lines->Delete(MemoUgol->Lines->Count-1);//удаляем превышение

    //очистим верхние ячейки
    if(ctat) masQuality[2]=2; else masQuality[2]=0;//до прихода Качества - по статике
    L1->Cells[0][2]="";
    L1a->Cells[0][2]="";
    Lakopp->Cells[0][2]="";
    Lh->Cells[0][2]="";
    //L0->Cells[0][2]="";
    MemoUgol->Lines->Strings[0]="";
    if(ctat==1) Label7->Visible=true; else Label7->Visible=false;
 }
//==================================================================================================
//далее идет вывод цифирей
#include "mode_1.h"
#include "mode_2.h"
#include "mode_11.h"
#include "mode_12.h"
#include "mode_13.h"
#include "mode_30.h"
#include "mode_31.h"
#include "servis_param.h"//Тут обрабатываются сервисный параметр
//==================================================================================================
//----------------запись в файл по глубине
//проверяем прирост текущей глубины
    Edz->Text=FloatToStrF0(MyStrToFloat(Edz->Text),5,1);//шаг по глубине, округляем до 1 знака после точки
    if((RadioGroup1->ItemIndex==0) && (DCPt>=DCP))//если глубина ВНЕШНЯЯ и процент достоверности приличный
     {float temp1=MyStrToFloat(Eglubina->Text);
      if(temp1-glubina_last >= MyStrToFloat(Edz->Text)) //если разница старой и текущей глубин больше, чем шаг в окошке.
      {TObject *Sender; Panel6Click(Sender);//Запишем значения в файл inc
      }
     }
}
