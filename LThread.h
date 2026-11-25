//---------------------------------------------------------------------------

#ifndef LThreadH
#define LThreadH
//---------------------------------------------------------------------------
#include <Classes.hpp>
//---------------------------------------------------------------------------
class LThread : public TThread
{            
private:
    void __fastcall glu();
    void __fastcall tmr();
    void __fastcall txt();
protected:
    void __fastcall Execute();
public:
    __fastcall LThread(bool CreateSuspended);
};
//---------------------------------------------------------------------------
#endif
