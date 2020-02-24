#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include <wx/config.h>

#include "config_fct.h"

void GetConfigurations( wxString *&p_List, int &p_Size )
{
	wxString 		ConfigurationName ;
	bool			bRet ;
	long			Index,i ;
	p_Size = 0 ;
	i = 0 ;

	wxConfig::Get()->SetPath( wxT( "/configurations" ) );
	p_Size = wxConfig::Get()->GetNumberOfGroups();
	p_List = new wxString[ p_Size ];
	bRet = wxConfig::Get()->GetFirstGroup( ConfigurationName, Index );
	while( bRet )
	{
		p_List[ i ] = ConfigurationName ;
		i++;
		bRet = wxConfig::Get()->GetNextGroup( ConfigurationName, Index );
	}
}

int	WriteConfiguration( const wxString& p_ConfigName, const CnxConfiguration& p_Config )
{

	wxConfig::Get()->Write( wxString::Format( wxT( "/configurations/%s/userdn" ), p_ConfigName.c_str() ),
	p_Config.userdn );
	
	wxConfig::Get()->Write( wxString::Format( wxT( "/configurations/%s/storepwd" ), p_ConfigName.c_str() ),
	(long)p_Config.storepwd );

	wxConfig::Get()->Write( wxString::Format( wxT( "/configurations/%s/password" ), p_ConfigName.c_str() ),
	(p_Config.storepwd?p_Config.password:wxT("")) );

	wxConfig::Get()->Write( wxString::Format( wxT( "/configurations/%s/usessl" ), p_ConfigName.c_str() ),
	p_Config.usessl );
	
	wxConfig::Get()->Write( wxString::Format( wxT( "/configurations/%s/usetls" ), p_ConfigName.c_str() ),
	p_Config.usetls );

	wxConfig::Get()->Write( wxString::Format( wxT( "/configurations/%s/server" ), p_ConfigName.c_str() ),
	p_Config.server );

	wxConfig::Get()->Write( wxString::Format( wxT( "/configurations/%s/port" ), p_ConfigName.c_str() ),
	p_Config.port );

	wxConfig::Get()->Write( wxString::Format( wxT( "/configurations/%s/sslport" ), p_ConfigName.c_str() ),
	p_Config.sslport );

	wxConfig::Get()->Write( wxString::Format( wxT( "/configurations/%s/sizelimit" ), p_ConfigName.c_str() ),
	p_Config.sizelimit );

	wxConfig::Get()->Write( wxString::Format( wxT( "/configurations/%s/referals" ), p_ConfigName.c_str() ),
	p_Config.referals );

	wxConfig::Get()->Write( wxString::Format( wxT( "/configurations/%s/timelimit" ), p_ConfigName.c_str() ),
	p_Config.timelimit );

	wxConfig::Get()->Write( wxString::Format( wxT( "/configurations/%s/version" ), p_ConfigName.c_str() ),
	p_Config.version );

	wxConfig::Get()->Write( wxString::Format( wxT( "/configurations/%s/basedn" ), p_ConfigName.c_str() ),
	p_Config.basedn );
	
	wxConfig::Get()->Write( wxString::Format( wxT( "/configurations/%s/anonymous" ), p_ConfigName.c_str() ),
	p_Config.anonymous_login );

	wxConfig::Get()->Write( wxString::Format( wxT( "/configurations/%s/cacert" ), p_ConfigName.c_str() ),
	p_Config.cacert );

	wxConfig::Get()->Write( wxString::Format( wxT( "/configurations/%s/certfile" ), p_ConfigName.c_str() ),
	p_Config.certfile );

	wxConfig::Get()->Write( wxString::Format( wxT( "/configurations/%s/keyfile" ), p_ConfigName.c_str() ),
	p_Config.keyfile );

	wxConfig::Get()->Write( wxString::Format( wxT( "/configurations/%s/subschemasubentry" ), p_ConfigName.c_str() ),
	p_Config.subschemasubentry );
		
	wxConfig::Get()->Write( wxString::Format( wxT( "/configurations/%s/guessbinary" ), p_ConfigName.c_str() ),
	p_Config.guessbinary );
	return 0;
}

int	ReadConfiguration( const wxString& p_ConfigName, CnxConfiguration& p_Config )
{
	long lvalue;
	p_Config.userdn = wxConfig::Get()->Read( wxString::Format( wxT( "/configurations/%s/userdn" ), p_ConfigName.c_str() ) );

	wxConfig::Get()->Read( wxString::Format( wxT( "/configurations/%s/storepwd" ), p_ConfigName.c_str() ), &lvalue, false );
	p_Config.storepwd = (bool)lvalue;
	p_Config.password = wxConfig::Get()->Read( wxString::Format( wxT( "/configurations/%s/password" ), p_ConfigName.c_str() ) );

	p_Config.usessl = wxConfig::Get()->Read( wxString::Format( wxT( "/configurations/%s/usessl" ), p_ConfigName.c_str() ) );
	
	p_Config.usetls = wxConfig::Get()->Read( wxString::Format( wxT( "/configurations/%s/usetls" ), p_ConfigName.c_str() ) );

	p_Config.server = wxConfig::Get()->Read( wxString::Format( wxT( "/configurations/%s/server" ), p_ConfigName.c_str() ) );

	p_Config.port = wxConfig::Get()->Read( wxString::Format( wxT( "/configurations/%s/port" ), p_ConfigName.c_str() ) );

	p_Config.sslport = wxConfig::Get()->Read( wxString::Format( wxT( "/configurations/%s/sslport" ), p_ConfigName.c_str() ) );

	p_Config.sizelimit = wxConfig::Get()->Read( wxString::Format( wxT( "/configurations/%s/sizelimit" ), p_ConfigName.c_str() ) );

	p_Config.timelimit = wxConfig::Get()->Read( wxString::Format( wxT( "/configurations/%s/timelimit" ), p_ConfigName.c_str() ) );

	p_Config.referals = wxConfig::Get()->Read( wxString::Format( wxT( "/configurations/%s/referals" ), p_ConfigName.c_str() ) );
		
	p_Config.version = wxConfig::Get()->Read( wxString::Format( wxT( "/configurations/%s/version" ), p_ConfigName.c_str() ) );

	p_Config.basedn = wxConfig::Get()->Read( wxString::Format( wxT( "/configurations/%s/basedn" ), p_ConfigName.c_str() ) );

	p_Config.anonymous_login = wxConfig::Get()->Read( wxString::Format( wxT( "/configurations/%s/anonymous" ), p_ConfigName.c_str() ) );

	p_Config.cacert = wxConfig::Get()->Read( wxString::Format( wxT( "/configurations/%s/cacert" ), p_ConfigName.c_str() ) );

	p_Config.certfile = wxConfig::Get()->Read( wxString::Format( wxT( "/configurations/%s/certfile" ), p_ConfigName.c_str() ) );

	p_Config.keyfile = wxConfig::Get()->Read( wxString::Format( wxT( "/configurations/%s/keyfile" ), p_ConfigName.c_str() ) );
	
	p_Config.subschemasubentry = wxConfig::Get()->Read( wxString::Format( wxT( "/configurations/%s/subschemasubentry" ), p_ConfigName.c_str() ) );

	p_Config.guessbinary = wxConfig::Get()->Read( wxString::Format( wxT( "/configurations/%s/guessbinary" ), p_ConfigName.c_str() ) );
	
	return 0;
}
