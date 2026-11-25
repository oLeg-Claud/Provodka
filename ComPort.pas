unit ComPort;

interface

uses Windows,SysUtils;

type TComPort=class
     private
      PortName: string; {имя COM-порта}
      Hc: HWND; {дескриптор COM-порта}
      Cto: COMMTIMEOUTS; {структура тайм-аутов COM-порта}
      Cdcb: DCB; {блок параметров COM-порта}
      Bytes: DWORD; {кол-во переданных в/из COM-порта байт}
     public
      constructor Create(Port: string; Speed,TimeOut: integer; DataBit,Parity,
                         StopBit: byte; DlBuf: integer);
      destructor Destroy; override;
      function OutPort(Buf: array of byte): boolean;
      function InPort(var Buf: array of byte): boolean;
      function InName(var Buf: array of byte): boolean;
     end;

implementation

constructor TComPort.Create(Port: string; Speed,TimeOut: integer; DataBit,
                            Parity,StopBit: byte; DlBuf: integer);
begin
 inherited Create;
 PortName:=Port;
 Delete(PortName,Pos(' (USB)',PortName),6);
 Hc:=CreateFile(pchar('\\.\'+PortName),GENERIC_READ or GENERIC_WRITE,0,nil,
  OPEN_EXISTING,0,0);
 if Hc=INVALID_HANDLE_VALUE then
  raise Exception.Create('Ошибка открытия порта: '+Port+'');
 GetCommTimeouts(Hc,Cto);
 Cto.ReadTotalTimeoutConstant:=TimeOut;
 Cto.WriteTotalTimeoutConstant:=TimeOut;
 SetCommTimeouts(Hc,Cto);
 if not GetCommState(Hc,Cdcb) then
  raise Exception.Create('Ошибка чтения состояния порта: '+Port+'!');
 Cdcb.BaudRate:=Speed;
 Cdcb.ByteSize:=DataBit;
 Cdcb.Parity:=Parity;
 Cdcb.StopBits:=StopBit;
 if not SetCommState(Hc,Cdcb) then
  raise Exception.Create('Ошибка записи состояния порта: '+Port+'!');
 SetupComm(Hc,10*DlBuf,10*DlBuf);
 PurgeComm(Hc,PURGE_TXABORT or PURGE_RXABORT or PURGE_TXCLEAR or PURGE_RXCLEAR);
 Sleep(20);
end;

destructor TComPort.Destroy;
begin
 if Hc<>INVALID_HANDLE_VALUE then
 begin
  PurgeComm(Hc,PURGE_TXABORT or PURGE_RXABORT or PURGE_TXCLEAR or PURGE_RXCLEAR);
  CloseHandle(Hc);
  Hc:=0;
 end;
 inherited Destroy;
end;

function TComPort.OutPort(Buf: array of byte): boolean;
begin
 Result:=WriteFile(Hc,Buf,Length(Buf),Bytes,nil);
end;

function TComPort.InPort(var Buf: array of byte): boolean;
begin
 Result:=true;
 ReadFile(Hc,Buf,Length(Buf),Bytes,nil);
 PurgeComm(Hc,PURGE_TXABORT or PURGE_RXABORT or PURGE_TXCLEAR or PURGE_RXCLEAR);
 Sleep(20);
 if Length(Buf)>integer(Bytes) then
  Result:=false;
end;

function TComPort.InName(var Buf: array of byte): boolean;
var BufT, LBuf,i: byte;
begin
  LBuf:= Length(Buf);
  Buf[0]:=0;
  for i:=0 to LBuf do
  begin
  Result:=ReadFile(Hc,BufT,1,Bytes,nil);
  if integer(Bytes)=1 then
    begin
    if BufT=13 then
      begin
      Buf [i]:= 0;
      break;
      end;
    Buf [i]:= BufT
    end
  else
    begin
    Buf [i]:= 0;
    break ;
    end;
  end;
end;

end.
