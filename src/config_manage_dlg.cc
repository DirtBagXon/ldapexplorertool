#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include <wx/xrc/xmlres.h>
#include <wx/config.h>

#include "config_fct.h"
#include "config_manage_dlg.h"
#include "configurationdlg.h"


BEGIN_EVENT_TABLE( Config_Manage_Dlg, wxDialog )
	EVT_BUTTON( XRCID( "button_open" ), Config_Manage_Dlg::OnOpen )
	EVT_BUTTON( XRCID( "button_new" ), Config_Manage_Dlg::OnNew )
	EVT_BUTTON( XRCID( "button_edit" ), Config_Manage_Dlg::OnEdit )
	EVT_BUTTON( XRCID( "button_delete" ), Config_Manage_Dlg::OnDelete )
END_EVENT_TABLE()

Config_Manage_Dlg::Config_Manage_Dlg()
{

	wxXmlResource::Get()->LoadDialog( this, NULL, wxT( "config_manage_dlg" ) );
	InitControls();

	FillList();
	SetReturnCode( wxID_CANCEL );
	
}
Config_Manage_Dlg::~Config_Manage_Dlg()
{
}
void Config_Manage_Dlg::FillList(void)
{
	wxString	*Configurations;
	int		nConf;

	GetConfigurations( Configurations, nConf );	
	config_list_ctrl->Set(nConf, Configurations );
	delete []Configurations;
}
void Config_Manage_Dlg::InitControls()
{
	config_list_ctrl=( wxListBox* )FindWindowById( XRCID("config_list_ctrl"), this );
}
void Config_Manage_Dlg::OnOpen( wxCommandEvent& p_Event )
{
	wxString Name ;
	wxArrayInt Selection;

	Name = config_list_ctrl->GetStringSelection();
	if( Name.Length() == 0 )
	{
		::wxMessageBox( wxT( "Choose a configuration to open" ), wxT( "Error" ) );
		return;
	}
	ConfigName = Name;
	SetReturnCode( wxID_OK );
	EndModal( wxID_OK );
}
void Config_Manage_Dlg::OnNew( wxCommandEvent& p_Event )
{
	int	Ret;
	ConfigurationDlg Dlg(this) ;
	
	Ret = Dlg.ShowModal();
	if( Ret == wxID_OK )
	{
		//wxConfig::Get()->Write( wxT( "/lastconfiguration" ), Dlg.Name );
		FillList();
	}
}
void Config_Manage_Dlg::OnEdit( wxCommandEvent& p_Event )
{
	ConfigurationDlg Dlg( this, config_list_ctrl->GetStringSelection() );
	int Ret;
	
	Ret = Dlg.ShowModal();
	if( Ret == wxID_OK )
	{
		//wxConfig::Get()->Write( wxT( "/lastconfiguration" ), f.Name );
		FillList();
	}
}
void Config_Manage_Dlg::OnDelete( wxCommandEvent& p_Event )
{
	wxString Config = wxString::Format( wxT( "/configurations/%s" ), config_list_ctrl->GetStringSelection().c_str() ) ;
	if ( ::wxMessageBox( wxT("Really delete this configuration"),Config,wxYES_NO ) == wxYES )
	{
		wxConfig::Get()->DeleteGroup( Config );
		FillList();
	}
}
