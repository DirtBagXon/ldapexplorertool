#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include <wx/xrc/xmlres.h>
#include <wx/splitter.h>
#include <wx/treectrl.h>
#include <wx/config.h>

#include "ldapentry.h"
#include "ldapclass.h"
#include "schema.h"
#include "config_fct.h"
#include "configurationdlg.h"


BEGIN_EVENT_TABLE( ConfigurationDlg, wxDialog )
	EVT_BUTTON( XRCID( "button_ok" ), ConfigurationDlg::OnOK ) 
	EVT_BUTTON(XRCID( "test_server_button" ), ConfigurationDlg::OnTestServerConnection ) 
	EVT_BUTTON(XRCID( "test_connection_button" ), ConfigurationDlg::OnTestConnection ) 
	EVT_BUTTON(XRCID( "basedn_guess_button" ), ConfigurationDlg::OnGuessBaseDN ) 
	EVT_BUTTON(XRCID( "button_ca" ), ConfigurationDlg::OnCAClick ) 
	EVT_BUTTON(XRCID( "button_cert" ), ConfigurationDlg::OnCertClick ) 
	EVT_BUTTON(XRCID( "button_key" ), ConfigurationDlg::OnKeyClick ) 
	EVT_CHECKBOX(XRCID( "usedefport_checkbox" ), ConfigurationDlg::OnUseDefPortClick ) 
	EVT_CHECKBOX(XRCID( "usedefsslport_checkbox" ), ConfigurationDlg::OnUseDefSSLPortClick ) 
	EVT_CHECKBOX(XRCID( "storepwd_checkbox" ), ConfigurationDlg::OnStorePwdClick ) 
	EVT_RADIOBOX( XRCID( "usessl_radio_box" ), ConfigurationDlg::OnUseSSL )
END_EVENT_TABLE()


ConfigurationDlg::ConfigurationDlg( wxWindow* p_Parent )
{
	wxXmlResource::Get()->LoadDialog( this, p_Parent, wxT("configurationdlg") );
	InitControls();
	port_text_ctrl->SetValidator( wxTextValidator( wxFILTER_NUMERIC ) );
	sslport_text_ctrl->SetValidator( wxTextValidator( wxFILTER_NUMERIC ) );
}

ConfigurationDlg::ConfigurationDlg( wxWindow* p_Parent, const wxString& p_ConfigName )
{
	
	wxString StringValue;
	CnxConfiguration	Config ;
	wxXmlResource::Get()->LoadDialog( this, p_Parent, wxT("configurationdlg") );
	InitControls();
	
	configname_text_ctrl->SetValue( p_ConfigName );
	configname_text_ctrl->SetEditable( false );
	configname_text_ctrl->Disable();
	
	ReadConfiguration( p_ConfigName, Config );
	
	
	userdn_text_ctrl->SetValue( Config.userdn ) ;

	password_text_ctrl->SetValue( Config.password );
	
	storepwd_checkbox->SetValue( Config.storepwd );
	password_text_ctrl->Enable( Config.storepwd );
	
	usessl_radio_box->SetStringSelection( Config.usessl );
	usetls_radio_box->SetStringSelection( Config.usetls );
	
	if( Config.usessl == _("Yes") )
	{
		usetls_radio_box->Enable( false );
	}
	server_text_ctrl->SetValue( Config.server );

	port_text_ctrl->SetValue( Config.port );
	port_text_ctrl->SetValidator( wxTextValidator( wxFILTER_NUMERIC ) );

	sslport_text_ctrl->SetValue( Config.sslport );
	sslport_text_ctrl->SetValidator( wxTextValidator( wxFILTER_NUMERIC ) );

	sizelimit_text_ctrl->SetValue( Config.sizelimit );

	timelimit_text_ctrl->SetValue( Config.timelimit );

	referals_radio_box->SetStringSelection( Config.referals );
		
	version_radio_box->SetStringSelection( Config.version );

	basedn_text_ctrl->SetValue( Config.basedn );
	
	if( Config.anonymous_login == wxT( "1" ) )
	{
		anonymous_checkbox->SetValue( true );
	}
	else
	{
		anonymous_checkbox->SetValue( false );
	}
	cacert_text_ctrl->SetValue( Config.cacert );
	cert_text_ctrl->SetValue( Config.certfile );
	key_text_ctrl->SetValue( Config.keyfile );
	
	subschemasubentry_text_ctrl->SetValue( Config.subschemasubentry );
	
	guessbinary_radio_box->SetStringSelection( Config.guessbinary );
}
void ConfigurationDlg::InitControls()
{
	configname_text_ctrl=(wxTextCtrl*)FindWindowById( XRCID("configname_text_ctrl"), this );
	server_text_ctrl=(wxTextCtrl*)FindWindowById( XRCID("server_text_ctrl"), this );
	port_text_ctrl=(wxTextCtrl*)FindWindowById( XRCID("port_text_ctrl"), this );
	sslport_text_ctrl=(wxTextCtrl*)FindWindowById( XRCID("sslport_text_ctrl"), this );
	userdn_text_ctrl=(wxTextCtrl*)FindWindowById( XRCID("userdn_text_ctrl"), this );
	password_text_ctrl=(wxTextCtrl*)FindWindowById( XRCID("password_text_ctrl"), this );
	basedn_text_ctrl=(wxTextCtrl*)FindWindowById( XRCID("basedn_text_ctrl"), this );
	timelimit_text_ctrl=(wxTextCtrl*)FindWindowById( XRCID("timelimit_text_ctrl"), this );
	sizelimit_text_ctrl=(wxTextCtrl*)FindWindowById( XRCID("sizelimit_text_ctrl"), this );
	subschemasubentry_text_ctrl=(wxTextCtrl*)FindWindowById( XRCID("subschemasubentry_text_ctrl"), this );
	cacert_text_ctrl=(wxTextCtrl*)FindWindowById( XRCID("cacert_text_ctrl"), this );
	cert_text_ctrl=(wxTextCtrl*)FindWindowById( XRCID("cert_text_ctrl"), this );
	key_text_ctrl=(wxTextCtrl*)FindWindowById( XRCID("key_text_ctrl"), this );
	
	storepwd_checkbox=(wxCheckBox*)FindWindowById( XRCID("storepwd_checkbox"), this );
	usedefport_checkbox=(wxCheckBox*)FindWindowById( XRCID("usedefport_checkbox"), this );
	anonymous_checkbox=(wxCheckBox*)FindWindowById( XRCID("anonymous_checkbox"), this );
	usedefsslport_checkbox=(wxCheckBox*)FindWindowById( XRCID("usedefsslport_checkbox"), this );
	
	version_radio_box=(wxRadioBox*)FindWindowById( XRCID("version_radio_box"), this );
	usessl_radio_box=(wxRadioBox*)FindWindowById( XRCID("usessl_radio_box"), this );
	usetls_radio_box=(wxRadioBox*)FindWindowById( XRCID("usetls_radio_box"), this );
	referals_radio_box=(wxRadioBox*)FindWindowById( XRCID("referals_radio_box"), this );
	guessbinary_radio_box=(wxRadioBox*)FindWindowById( XRCID("guessbinary_radio_box"), this );



	basedn_guess_button=(wxButton*)FindWindowById( XRCID("basedn_guess_button"), this );
	test_connection_button=(wxButton*)FindWindowById( XRCID("test_connection_button"), this );
	test_server_button=(wxButton*)FindWindowById( XRCID("test_server_button"), this );
	button_ca=(wxButton*)FindWindowById( XRCID("button_ca"), this );
	button_cert=(wxButton*)FindWindowById( XRCID("button_cert"), this );
	button_key=(wxButton*)FindWindowById( XRCID("button_key"), this );
	button_ok=(wxButton*)FindWindowById( XRCID("button_ok"), this );
	wxID_CANCEL=(wxButton*)FindWindowById( XRCID("wxID_CANCEL"), this );

	assert( storepwd_checkbox != NULL );
	assert( configname_text_ctrl != NULL );
	assert( server_text_ctrl != NULL );
	assert( port_text_ctrl != NULL );
	assert( sslport_text_ctrl != NULL );
	assert( userdn_text_ctrl != NULL );
	assert( password_text_ctrl != NULL );
	assert( basedn_text_ctrl != NULL );
	assert( timelimit_text_ctrl != NULL );
	assert( sizelimit_text_ctrl != NULL );
	assert( subschemasubentry_text_ctrl != NULL );
	assert( cacert_text_ctrl != NULL );
	assert( cert_text_ctrl != NULL );
	assert( key_text_ctrl != NULL );
	assert( usedefport_checkbox != NULL );
	assert( anonymous_checkbox != NULL );
	assert( usedefsslport_checkbox != NULL );
	assert( version_radio_box != NULL );
	assert( usessl_radio_box != NULL );
	assert( usetls_radio_box != NULL );
	assert( referals_radio_box != NULL );
	assert( guessbinary_radio_box != NULL );
	assert( basedn_guess_button != NULL );
	assert( test_connection_button != NULL );
	assert( test_server_button != NULL );
	assert( button_ca != NULL );
	assert( button_cert != NULL );
	assert( button_key != NULL );
	assert( button_ok != NULL );
	assert( wxID_CANCEL != NULL );


}
void ConfigurationDlg::OnOK( wxCommandEvent& p_Event )
{
	
	CnxConfiguration Config;	
	wxString ConfigName = configname_text_ctrl->GetValue();
	
	if( ConfigName.Len() == 0 )
	{
		::wxMessageBox( _( "Configuration name is empty" ), _( "Error" ), wxOK | wxICON_ERROR );
		return ;
	}
	Config.server =server_text_ctrl->GetValue();
	if( Config.server.Len() == 0 )
	{
		::wxMessageBox( _( "Server name or IP is empty" ), _( "Error" ), wxOK | wxICON_ERROR );
		return ;
	}
	
	
	Config.userdn = userdn_text_ctrl->GetValue() ;
	Config.password = password_text_ctrl->GetValue();
	Config.usessl = usessl_radio_box->GetStringSelection();
	Config.usetls = usetls_radio_box->GetStringSelection();
	
	Config.referals = referals_radio_box->GetStringSelection() ;
	Config.port = port_text_ctrl->GetValue();
	Config.sslport = sslport_text_ctrl->GetValue();
	Config.sizelimit = sizelimit_text_ctrl->GetValue();
	Config.timelimit = timelimit_text_ctrl->GetValue();
	Config.version = version_radio_box->GetStringSelection();
	Config.basedn = basedn_text_ctrl->GetValue();
	Config.cacert = cacert_text_ctrl->GetValue();
	Config.certfile = cert_text_ctrl->GetValue();
	Config.keyfile = key_text_ctrl->GetValue();
	Config.subschemasubentry = subschemasubentry_text_ctrl->GetValue();
	if( anonymous_checkbox->IsChecked() )
	{
		Config.anonymous_login = wxT( "1" );
	}
	else
	{
		Config.anonymous_login = wxT( "0" );
	}
	Config.guessbinary = guessbinary_radio_box->GetStringSelection();
	Config.storepwd = storepwd_checkbox->GetValue();
	WriteConfiguration( ConfigName, Config );
	
	Name = ConfigName ;
	SetReturnCode( wxID_OK );
	EndModal( wxID_OK );
}

void ConfigurationDlg::OnStorePwdClick( wxCommandEvent& p_Event )
{
	password_text_ctrl->Enable( storepwd_checkbox->GetValue() );
}

void ConfigurationDlg::OnUseDefPortClick( wxCommandEvent& p_Event )
{
	if( usedefport_checkbox->IsChecked() )
	{
		port_text_ctrl->SetValue( wxT( "389" ) );
		port_text_ctrl->SetEditable( false );
	}
	else
	{
		port_text_ctrl->SetValue( wxT( "" ) );
		port_text_ctrl->SetEditable( true );
	}
}
void ConfigurationDlg::OnUseSSL( wxCommandEvent& p_Event )
{
	if( usessl_radio_box->GetStringSelection() == _("Yes") )
	{
		usetls_radio_box->Enable( false );
	}
	else
	{
		usetls_radio_box->Enable( true );
	}
}


void ConfigurationDlg::OnUseDefSSLPortClick( wxCommandEvent& p_Event )
{
	if( usedefsslport_checkbox->IsChecked() )
	{
		sslport_text_ctrl->SetValue( wxT( "636" ) );
		sslport_text_ctrl->SetEditable( false );
	}
	else
	{
		sslport_text_ctrl->SetValue( wxT( "" ) );
		sslport_text_ctrl->SetEditable( true );
	}
}

void ConfigurationDlg::OnTestServerConnection( wxCommandEvent& p_Event )
{
	Test( false );			
}
void ConfigurationDlg::OnCAClick( wxCommandEvent& p_Event )
{
	wxString File = wxFileSelector(_("Choose the CA Certificate"), wxT( "." ) );
	if( !File.empty() )
	{
		cacert_text_ctrl->SetValue( File );
	}

}

void ConfigurationDlg::OnCertClick( wxCommandEvent& p_Event )
{
	wxString File = wxFileSelector(_("Choose the Client Certificate"), wxT( "." ) );
	if( !File.empty() )
	{
		cert_text_ctrl->SetValue( File );
	}
}

void ConfigurationDlg::OnKeyClick( wxCommandEvent& p_Event )
{
	wxString File = wxFileSelector(_("Choose the Client Key"), wxT( "." ) );
	if( !File.empty() )
	{
		key_text_ctrl->SetValue( File );
	}
}

void ConfigurationDlg::OnTestConnection( wxCommandEvent& p_Event )
{
	Test( false );	
}
void ConfigurationDlg::OnGuessBaseDN( wxCommandEvent& p_Event )
{
	Test( true );	
}


void ConfigurationDlg::Test( bool p_GetBaseDN )
{
	CnxConfiguration Config;	
	
	u_long			Version;
	u_long			Port;
	u_long			SSLPort;
	int				Err;
	bool			UseSSL = false ;
	bool			UseTLS = false ;
	int 			Ret ;
	int 			Id;
	SchemaUtil		hLdap ;

	Config.server =server_text_ctrl->GetValue();
	if( Config.server.Len() == 0 )
	{
		::wxMessageBox( _( "Server name or IP is empty" ), _( "Error" ), wxOK | wxICON_ERROR );
		return ;
	}
	
	Config.userdn = userdn_text_ctrl->GetValue() ;
	Config.password = password_text_ctrl->GetValue();
	Config.storepwd = storepwd_checkbox->GetValue();
	Config.usessl = usessl_radio_box->GetStringSelection();
	Config.usetls = usetls_radio_box->GetStringSelection();
	
	Config.referals = referals_radio_box->GetStringSelection() ;
	Config.port = port_text_ctrl->GetValue();
	Config.sslport = sslport_text_ctrl->GetValue();
	Config.sizelimit = sizelimit_text_ctrl->GetValue();
	Config.timelimit = timelimit_text_ctrl->GetValue();
	Config.version = version_radio_box->GetStringSelection();
	Config.basedn = basedn_text_ctrl->GetValue();
	Config.cacert = cacert_text_ctrl->GetValue();
	Config.certfile = cert_text_ctrl->GetValue();
	Config.keyfile = key_text_ctrl->GetValue();

	if( anonymous_checkbox->IsChecked() )
	{
		Config.anonymous_login = wxT( "1" );
	}
	else
	{
		Config.anonymous_login = wxT( "0" );
	}
		
	Config.port.ToULong( &Port );
	Config.sslport.ToULong( &SSLPort );
	Config.version.ToULong( &Version );
	
	
	if( Config.usessl == _("Yes") )
	{
		UseSSL = true ;
		Port = SSLPort;
	}
	else if( Config.usetls == _("Yes") )
	{
		UseTLS = true ;
		
	}
	
	if( UseSSL || UseTLS )
	{
		Ret = hLdap.SetCACert( Config.cacert );
		if( Ret != LDAP_SUCCESS )
		{
			::wxMessageBox( hLdap.GetErrorStr(), _("Set CA Cert" ) );
		}
		if( Config.certfile.Len() > 0 && Config.keyfile.Len() > 0 )
		{
			Ret = hLdap.SetClientCert( Config.certfile );
			Ret = hLdap.SetClientKey( Config.keyfile );
		}
	}
	Ret = hLdap.Init( Config.server, Port );
	if( Ret != LDAP_SUCCESS )
	{
		::wxMessageBox( wxString::Format( wxT( "LDAP open:%s" ), hLdap.GetErrorStr().c_str() ) );
		return;
	}
	if( hLdap.SetVersion( Version ) != LDAP_SUCCESS )
	{
		::wxMessageBox( wxString::Format( wxT( "LDAP SetVersion:%s" ), hLdap.GetErrorStr().c_str() ) );
		return;
	}
	
	if( UseTLS )
	{	
		Ret = hLdap.StartTLS( );
		if( Ret != LDAP_SUCCESS )
		{
			::wxMessageBox( wxString::Format( wxT( "LDAP StartTLS:%s" ), hLdap.GetErrorStr().c_str() ) );
			return;
		}
	}
	if( UseSSL )
	{
		int SSLArg = LDAP_OPT_X_TLS_HARD ;
		Ret = hLdap.SetOption( LDAP_OPT_X_TLS, &SSLArg );
		if( Ret != LDAP_SUCCESS )
		{
			::wxMessageBox( hLdap.GetErrorStr().c_str(), _( "SetOption:LDAP_OPT_X_TLS" )  );
			return;
		}
	}

	unsigned long SizeLimit ;
	unsigned long TimeLimit ;
	
	Config.sizelimit.ToULong( &SizeLimit );
	Config.timelimit.ToULong( &TimeLimit );
	
	if( Config.sizelimit.Len() > 0 )
	{
		Err = hLdap.SetSizeLimit( SizeLimit );
	}
	
	if( Config.timelimit.Len() > 0 )
	{
		Err = hLdap.SetTimeLimit( TimeLimit );
	}
	
	if( Config.referals == _( "Yes" ) )
	{
		Err = hLdap.Referals( true ) ;
	}
	else
	{
		Err = hLdap.Referals( false ) ;
	}

	if( Config.anonymous_login == wxT( "1" ) )
	{
		Err = hLdap.SimpleBind();
	}
	else
	{
		if( Config.storepwd )
		{
			Err = hLdap.SimpleBind( Config.userdn, Config.password );
		}
		else
		{
			//wxPasswordEntryDialog Dlg(this, wxT("Enter user password") );
			wxTextEntryDialog Dlg(this, wxT("Enter user password"),wxT("Enter password"),wxT(""),wxOK|wxCANCEL|wxTE_PASSWORD );
			Dlg.ShowModal();
			Err = hLdap.SimpleBind( Config.userdn, Dlg.GetValue());
		}
	}
	
	if( Err != LDAP_SUCCESS )
	{
		::wxMessageBox( wxString::Format( wxT( "LDAP Bind:%s" ), hLdap.GetErrorStr().c_str() ) );
		return;
	}

	if( p_GetBaseDN )
	{
		Ret = hLdap.Search( Id, wxT( "" ), LDAP_SCOPE_BASE, wxT( "objectclass=*" ), 
		NULL, true, LF_ALL_OPERATIONAL );
		if( Ret != LDAP_SUCCESS )
		{
			::wxMessageBox( hLdap.GetErrorStr().c_str() , _("Error"), wxOK|wxICON_ERROR );
			return ;
		}
		
	
		int NbEntries = hLdap.GetEntryCount( Id ) ;
		if( NbEntries > 0 )
		{
			LdapEntry Entry ;
			if( !hLdap.GetEntry( Id, 0, Entry ) )
			{
				::wxMessageBox( hLdap.GetErrorStr(), _("Error"), wxOK|wxICON_ERROR );
			}
			LdapAttribute *Attribute = Entry.GetAttribute( wxT( "namingcontexts" ), true );
			if( Attribute != NULL )
			{
				basedn_text_ctrl->SetValue( Attribute->GetValue( 0 ) );
			}
		}
		hLdap.CloseSearch( Id );
	}
	else
	{
		::wxMessageBox( _( "Test OK" ), _("Test Result" ) );
	}
}
