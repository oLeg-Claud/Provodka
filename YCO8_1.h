//---------------------------------------------------------------------------

#ifndef YCO8_1H
#define YCO8_1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#define WM_MyPost1  WM_USER
#define WM_MyPost2  WM_USER+1
#define WM_MyPost4  WM_USER+3
#define WM_MyPost5  WM_USER+4
#define WM_MyPost6  WM_USER+5
//---------------------------------------------------------------------------
class sic4 : public TThread
{
private:
         void __fastcall st();
         void __fastcall st0();
         void __fastcall sterr( );
         void __fastcall sterr1( );
          void __fastcall kih0( );
protected:
         void __fastcall Execute();
public:
        _fastcall sic4(bool CreateSuspended);
};
//---------------------------------------------------------------------------
#endif
