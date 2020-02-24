#ifndef __CONFIG_FCT_H
#define __CONFIG_FCT_H

struct CnxConfiguration
{
	wxString	userdn;
	bool		storepwd;
	wxString	password;
	wxString	usessl;
	wxString	usetls;
	wxString	server;
	wxString	port;
	wxString	sizelimit;
	wxString	timelimit;
	wxString	version;
	wxString	basedn;
	wxString	referals;
	wxString	sslport;
	wxString	anonymous_login;
	wxString	cacert;
	wxString	certfile;
	wxString	keyfile;
	wxString	subschemasubentry;
	wxString	guessbinary;
};

int	WriteConfiguration( const wxString& p_ConfigName, const CnxConfiguration& p_Config );
int	ReadConfiguration( const wxString& p_ConfigName, CnxConfiguration& p_Config );

void 	GetConfigurations( wxString *&p_List, int &p_Size );
	
#endif
