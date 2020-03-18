#ifdef __GNUG__
    #pragma implementation "application.h"
#endif

#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include <wx/image.h>
#include <wx/xrc/xmlres.h>
#include <wx/config.h>
#include <wx/cmdline.h>

#include "mainframe.h"
#include "application.h"


extern void InitXmlResource();

IMPLEMENT_APP(Application)

Application::Application()
{
}
Application::~Application()
{
}
bool Application::OnInit()
{
	wxCmdLineParser CmdParser( argc, argv );
	
	CmdParser.AddSwitch( wxT("h"), wxT("help"), wxT("Usage of LDAPExplorerTool") );

	if( CmdParser.Parse() == 0 )
	{
		
		if( CmdParser.Found( wxT("h") ) )
		{
			CmdParser.Usage();
			return false;
		}
	}

	wxXmlResource::Get()->InitAllHandlers();
	InitXmlResource();

	wxImage::AddHandler(new wxPNGHandler);

	MainFrame *frame = new MainFrame();
	frame->Show(true);
	return true;
}
