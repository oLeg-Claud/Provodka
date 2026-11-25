// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'ComPort.pas' rev: 6.00

#ifndef ComPortHPP
#define ComPortHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <SysUtils.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Comport
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TComPort;
class PASCALIMPLEMENTATION TComPort : public System::TObject 
{
	typedef System::TObject inherited;
	
private:
	AnsiString PortName;
	HWND Hc;
	_COMMTIMEOUTS Cto;
	#pragma pack(push, 1)
	_DCB Cdcb;
	#pragma pack(pop)
	
	unsigned Bytes;
	
public:
	__fastcall TComPort(AnsiString Port, int Speed, int TimeOut, Byte DataBit, Byte Parity, Byte StopBit, int DlBuf);
	__fastcall virtual ~TComPort(void);
	bool __fastcall OutPort(const Byte * Buf, const int Buf_Size);
	bool __fastcall InPort(Byte * Buf, const int Buf_Size);
	bool __fastcall InName(Byte * Buf, const int Buf_Size);
};


//-- var, const, procedure ---------------------------------------------------

}	/* namespace Comport */
using namespace Comport;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// ComPort
