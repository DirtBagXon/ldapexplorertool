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


#ifndef XRC_DIR
	#define XRC_DIR	wxT("./")
#endif

IMPLEMENT_APP(Application)

Application::Application()
{
}
Application::~Application()
{
}
bool Application::OnInit()
{

	wxConfig *ConfigFile = NULL ;
	wxString XrcFile = XRC_DIR;
	wxString ConfFile ;

	wxCmdLineParser CmdParser( argc, argv );
	
	CmdParser.AddOption( wxT("c"), wxT("config-file"), wxT("Configuration file to load") );
	CmdParser.AddOption( wxT("xrc"), wxT("xrc-dir"), wxT("Directory with Interface definition file to load") );

	CmdParser.AddSwitch( wxT("h"), wxT("help"), wxT("Usage of LDAPExplorerTool") );

	if( CmdParser.Parse() == 0 )
	{
		
		if( CmdParser.Found( wxT("h") ) )
		{
			CmdParser.Usage();
			return false;
		}
		if( !CmdParser.Found( wxT("c"), &ConfFile ) )
		{
			ConfFile = wxT(".ldapexplorertool2");
		}
		CmdParser.Found( wxT("xrc"), &XrcFile );
	}
	
	if( XrcFile.Last() != wxT('/') )
		XrcFile.Append( wxT( '/' ) );
	XrcFile.Append( wxT("ldapexplorertool2.xrc") );

	wxImage::AddHandler(new wxPNGHandler);
	
	wxXmlResource::Get()->InitAllHandlers();
	wxXmlResource::Get()->Load(XrcFile);
	
	ConfigFile = new wxConfig( wxT("ldapexplorertool2"),wxT("Laurent Ulrich"), ConfFile );
	wxConfig::Set( ConfigFile );
	
	MainFrame *frame = new MainFrame();
	frame->Show(true);
	return true;

}
