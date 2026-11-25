//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
USEFORM("TRAEKT2k.cpp", Fr45);
//---------------------------------------------------------------------------
extern AnsiString ver, HELP_FILE, TITLE;
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
        try
        {
                 Application->Initialize();
                 Application->Title = TITLE + ver;
                 Application->HelpFile = HELP_FILE;
		Application->CreateForm(__classid(TFr45), &Fr45);
		Application->HintHidePause = 10000;
               Application->Run();
        }
        catch (Exception &exception)
        {
                 Application->ShowException(&exception);
        }
        return 0;
}
//---------------------------------------------------------------------------












