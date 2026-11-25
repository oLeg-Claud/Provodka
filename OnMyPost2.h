//—юда присылает сообщение поток ”—ќ sic4 = нули
void __fastcall TFr45::OnMyPost2(TMessage& a)
{
dp=0;
dpp=0;
Panel2->Width=0;
DateTimeToString(dtime,"h:nn:ss",Now()-T0);
Fr45->StatusBar->Color=clWhite;
StatusBar->SimpleText=date0+"      ¬рем€ с начала проводки: "+dtime;

for (int i=0;i<DLKADRrp+2*DLMASr;i++) mas1[i]=masc[i];
//    спектр и фильтраци€ сигнала

//------------- коррел€ци€ —ѕ
	if(CompareText(Sn->Cells[1][3],"")!=0) DCP=StrToInt(Sn->Cells[1][3]);
	else DCP=0;
	levelsig=0;
	for (int i=0;i<DLKADRrp+2*DLMASr;i++) if(levelsig< mas1[i]) levelsig=mas1[i];
	ampl=levelsig;
	levelsig=levelsig*(KmB*1000/8388607.) ;
	if(uco32)levelsig=levelsig*100/256/1943 ;   //ShowMessage(levelsig);
	int jj,j0;
	float xkv;
	ind=0;
	maxind=0;
	minind=0;
	if(dst==0)
    {	for(int i=0;i<DLKADRrp+DLMASr;i++)
    	{	korr[i]=0; xkv=0;
          	for(int j=0;j<DLMASr;j++)
        	{	korr[i]=korr[i]+mas[j]*mas1[j+i];
        		xkv=xkv+mas1[j+i]*mas1[j+i];
        	}
     		if(xkv==0)korr[i]=0;
  			else korr[i]=korr[i]/(sqrt(xkm)*sqrt(xkv));
    		if(korr[i]>korr[maxind]) maxind=i;
			if(korr[i]<korr[minind]) minind=i;
        }
    }
	else
    {	for(int i=0;i<DLKADRrp+DLMASr;i++)
       	{	Mul((__int16) DLMASr,1,mas1,mas1,i,DLKADRrp+2*DLMASr,&xkv);
			if(xkv==0) korr[i]=0;
  			else
            {	Mul((__int16) DLMASr,0,mas,mas1,i,DLKADRrp+2*DLMASr,&korr[i]);
				korr[i]=korr[i]/(sqrt(xkm)*sqrt(xkv));
			}
			if(korr[i]>korr[maxind]) maxind=i;
			if(korr[i]<korr[minind]) minind=i;
       	}
    }
    if(fabs(korr[maxind])>fabs(korr[minind])) ind=maxind;
    else  ind=minind;
    if(korr[ind]>=0)faza=1;
    else faza=-1;
	float h=(float)DLKADRf/DLKADRr; // ShowMessage(ind);
	PCP=ind*h;
    if(PCP>=DLKADR) PCP-=DLKADR;
    indt0=ind;
	DCPt=abs((int)(korr[ind]*100));
	DCPt=DCPt+1;

	if(DCPt>=DCP)Panel2->Color=clGreen;
	else  Panel2->Color=clRed;

    //рисование коррел€ции красным
	Series3->Clear();
	if(ind<DLMASr) for(int j=0;j<DLKADRrp;j++) Series3->AddXY(j,korr[j],"",clRed);
	if(ind>=DLMASr) for(int j=DLMASr;j<DLKADRrp+DLMASr;j++) Series3->AddXY(j-DLMASr,korr[j],"",clRed);

	if( DCPt<DCP)
    {	C2->Title->Font->Color=clRed;
		C2->Title->Text->Strings[1]="—п "+IntToStr(DCPt)+"%(";
    }
	else
    {	C2->Title->Font->Color=clBlack;
		C2->Title->Text->Strings[1]="—п "+IntToStr(DCPt)+"%(";
    }

// коррел€ци€ данных --------------------------------
sdvig=0;
int il=0;
  indl=ind+DLMASr;
  for(int l=0;l<L;l++)
  {if(indl+DLDIGr+iG<DLKADRrp)
   {sdvig=0;
    SendMessage(Fr45->Handle,WM_MyPost3,l,indl);//-----------—ообщение потоку 3--------------------
    il=il+1;
    indl=indl+DLDIGr+sdvig;
   }
  }
NGO=-1;
if(il<L)
{NGO=il;
 NGT=NGO;
 MGT=indl+DLDIGr+iG;
 fl_buf=false;
 if(MGT>=2*DLKADRrp){MGT-=2*DLKADRrp; fl_buf=true;}
}
else
{
 C2->Title->Text->Strings[1]=C2->Title->Text->Strings[1]+")";
 MCP=ind+DLKADRrp+DELTAr;
 fl_buf=false;
 if(MCP>=2*DLKADRrp){MCP=MCP-2*DLKADRrp;fl_buf=true;}
// ShowMessage(MCP);
}

}

 