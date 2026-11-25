//==========================================================================
void DrawOTK1(float otk)
{
    nulotk=StrToInt(Fr45->Sn->Cells[1][8]);//обновляем значения
	mrmt=StrToInt(Fr45->Sn->Cells[1][0]);
    AnsiString str="";
    if(mrmt != 0) str+="снос метки ОТК = "+IntToStr(mrmt);
	if(nulotk!=0)
    {if(str != "") str+="\r\n";
     str+="нульОТК ="+IntToStr(nulotk);
    }
	Fr45->ZeroOTK->Caption=str;

    int newotk=inta(otk);
    int flag400=0;
    if(newotk==400) flag400=1;
    while(newotk>=360) newotk-=360;
    iotk++;
    if(iotk<5) newot[4-iotk]=newotk;
    else //помещаем в стек
    { for(int i=3;i>=0;i--) newot[i+1]=newot[i];
      newot[0]=newotk;
      iotk=4;
    }
    //-------------------------
    BM->Canvas->FillRect(Rect(0,0,BM->Width,BM->Height));
    BM->Width=min(Fr45->Potk->Height-Fr45->OBar->Height,Fr45->Potk->Width);
    BM->Height= BM->Width;
    BM->Canvas->Pen->Width=2;
    if(flag400) BM->Canvas->Pen->Color=clRed; else BM->Canvas->Pen->Color=clBlack;
    int j=0,xo,yo;
    for(int i=4-iotk;i<5;i++)
    {
     if(napr==1)
     {
      xo=ox-((5-j)*rad/5)*sin(M_PI*(360-newot[i])/180) ;
      yo=oy-((5-j)*rad/5)*cos(M_PI*(360-newot[i])/180);
     }
     if(napr==-1)
     {
      xo=ox-((5-j)*rad/5)*sin(M_PI*(newot[i])/180) ;
      yo=oy-((5-j)*rad/5)*cos(M_PI*(newot[i])/180);
     }
     if(i>4-iotk)BM->Canvas->Pen->Width=1;
     BM->Canvas->MoveTo(ox,oy);
     BM->Canvas->LineTo(xo,yo);
     j++;
    }
    BM->Canvas->Pen->Width=1;
    BM->Canvas->Pen->Style=psSolid;
    Fr45->I2->Picture->Assign(BM);
    //---------------------------------
    if(otkmin<otkmax) if(newotk<otkmin || newotk>otkmax) Beep();
    if(otkmin>otkmax) if(newotk>otkmax&& newotk<otkmin)  Beep();
}
//==========================================================================
void DrawOTK2(float otk)
{
    if(!Fr45->Caz_otk->Checked) Fr45->Label14->Caption="  Для ZU="+Fr45->Sn->Cells[1][2]+"   AZ="+Fr45->Sn->Cells[1][1]+"   маг. накл.="+Fr45->Sn->Cells[1][4];
    if(Fr45->Caz_otk->Checked) Fr45->Label14->Caption="                               Зарезка";
    int newotkm=inta(otk);
    int flag400=0;
    if(newotkm==400) flag400=1;
    while(newotkm>=360) newotkm-=360;
    while(newotkm<0) newotkm+=360;
    iotkm++;
    if(iotkm<5) newotm[4-iotkm] = newotkm;
    else
    { for(int i=3;i>=0;i--) newotm[i+1] = newotm[i];
      newotm[0] = newotkm;
      iotkm=4;
    }
    //-------------------------
    BM->Canvas->FillRect(Rect(0,0,BM->Width,BM->Height));
    BM->Width=Fr45->Pi->Width; BM->Height= BM->Width;
    BM->Canvas->Pen->Width=2;
    if(flag400) BM->Canvas->Pen->Color=clRed; else BM->Canvas->Pen->Color=clBlack;
    int j=0,xo,yo;
    for(int i=4-iotkm;i<5;i++)
    {
     if(naprm==1)
     {
      xo=om-((5-j)*radm/5)*sin(M_PI*(360-newotm[i])/180) ;
      yo=om-((5-j)*radm/5)*cos(M_PI*(360-newotm[i])/180);
     }
     if(naprm==-1)
     {
      xo=om-((5-j)*radm/5)*sin(M_PI*(newotm[i])/180) ;
      yo=om-((5-j)*radm/5)*cos(M_PI*(newotm[i])/180);
     }
     if(i>4-iotkm) BM->Canvas->Pen->Width=1;
     BM->Canvas->MoveTo(om,om);
     BM->Canvas->LineTo(xo,yo);
     j++;
    }
    BM->Canvas->Pen->Width=1;
    BM->Canvas->Pen->Style=psSolid;
    Fr45->I2m->Picture->Assign(BM);
    //---------------------------------
    if(otkminm<otkmaxm)if(newotkm<otkminm || newotkm>otkmaxm)Beep();
    if(otkminm>otkmaxm)if(newotkm>otkmaxm&& newotkm<otkminm)Beep();
}
//==========================================================================
