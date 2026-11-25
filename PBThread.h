//---------------------------------------------------------------------------

#ifndef PBThreadH
#define PBThreadH
//---------------------------------------------------------------------------
#include <Classes.hpp>
//---------------------------------------------------------------------------
class PBThread : public TThread
{            
private:
    void __fastcall txt();
    void __fastcall qtxt();
    void __fastcall ledON();
    void __fastcall ledOFF();
    void __fastcall pause();
    void __fastcall send_response();
protected:
    void __fastcall Execute();
public:
    __fastcall PBThread(bool CreateSuspended);
};
//---------------------------------------------------------------------------
#endif
