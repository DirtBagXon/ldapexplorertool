#ifdef __GNUG__
    #pragma implementation "mainframe.h"
#endif


#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include <wx/xrc/xmlres.h>
#include <wx/config.h>
#include <wx/file.h>


#include "config_fct.h"
#include "mainframe.h"
#include "configurationdlg.h"
#include "newentrydlg.h"
#include "schemadlg.h"
#include "attribute_add_dlg.h"
#include "config_manage_dlg.h"
#include "searchdlg.h"



BEGIN_EVENT_TABLE( MainFrame, wxFrame )
    EVT_MENU(	XRCID( "file_quit" ), MainFrame::OnQuit )
    EVT_MENU(	XRCID( "file_open_lastconfig" ), MainFrame::OnOpenLast )
    EVT_MENU(	XRCID( "file_open" ), MainFrame::OnOpen )
    EVT_MENU(	XRCID( "new_configuration" ), MainFrame::OnNewConnection )
    EVT_MENU(	XRCID( "edit_configuration" ), MainFrame::OnEditConnection )
    EVT_MENU(	XRCID( "open_configuration" ), MainFrame::OnOpenConnection )
    EVT_MENU(	XRCID( "delete_configuration" ), MainFrame::OnDeleteConnection )
    EVT_MENU(	XRCID( "schema_view" ), MainFrame::OnViewSchema )
    EVT_MENU(	XRCID( "left_popup_add" ), MainFrame::OnAddEntry )
    EVT_MENU(	XRCID( "left_popup_copy" ), MainFrame::OnCopyEntry )
    EVT_MENU(	XRCID( "left_popup_rename" ), MainFrame::OnRenameEntry )
    EVT_MENU(	XRCID( "left_popup_delete" ), MainFrame::OnDeleteEntry )
    EVT_MENU(	XRCID( "left_popup_search" ), MainFrame::OnSearch )
    EVT_MENU(	XRCID( "right_popup_add_att" ), MainFrame::OnAddAttribute )
    EVT_MENU(	XRCID( "left_popup_del_att" ), MainFrame::OnDeleteAttribute )
    EVT_MENU(	XRCID( "right_popup_add_value" ), MainFrame::OnAddValue )
    EVT_MENU(	XRCID( "right_popup_add_binvalue" ), MainFrame::OnAddBinValue )
    EVT_MENU(	XRCID( "right_popup_mod_value" ), MainFrame::OnModifyValue )
    EVT_MENU(	XRCID( "right_popup_del_value" ), MainFrame::OnDeleteValue )
    EVT_TREE_SEL_CHANGED(XRCID( "left_tree_ctrl" ), MainFrame::OnLeftSelectionChanged ) 
    EVT_TREE_SEL_CHANGED(XRCID( "right_tree_ctrl" ), MainFrame::OnRightSelectionChanged ) 
    EVT_TREE_ITEM_ACTIVATED(XRCID( "left_tree_ctrl" ), MainFrame::OnLeftSelectionChanged )
    EVT_TREE_ITEM_RIGHT_CLICK( XRCID( "left_tree_ctrl" ), MainFrame::OnLeft_PopupMenu )
    EVT_TREE_ITEM_RIGHT_CLICK( XRCID( "right_tree_ctrl" ), MainFrame::OnRight_PopupMenu )    
END_EVENT_TABLE()

MainFrame::MainFrame()
{
	wxXmlResource::Get()->LoadFrame( this, NULL, wxT("main_frame") );
	left_tree_ctrl = (wxTreeCtrl*)FindWindowById( XRCID("left_tree_ctrl"), this) ;
	right_tree_ctrl = (wxTreeCtrl*)FindWindowById( XRCID("right_tree_ctrl"), this) ;
}
MainFrame::~MainFrame()
{
}

void MainFrame::OnQuit( wxCommandEvent& WXUNUSED( p_Event ) )
{
    Close(true);
}

void MainFrame::OnViewSchema( wxCommandEvent& WXUNUSED( p_Event ) )
{
	int	Ret;
	SchemaDlg Dlg( this, m_Ldap ) ;
	
	Ret = Dlg.ShowModal( );
	if( Ret == wxID_OK )
	{
	}
}

void MainFrame::OnOpen( wxCommandEvent& WXUNUSED( p_Event ) )
{
	Config_Manage_Dlg Dlg;
	if( Dlg.ShowModal() == wxID_OK )
	{	
		wxConfig::Get()->Write( wxT( "/lastconfiguration" ), Dlg.ConfigName );
		OnConnect( Dlg.ConfigName );
	}
}
void MainFrame::OnOpenLast( wxCommandEvent& WXUNUSED( p_Event ) )
{
	wxString ConfigName ;
	
	ConfigName = wxConfig::Get()->Read( wxT( "/lastconfiguration" ), wxT( "" ) );
	if( ConfigName.Len() == 0 )
	{
		::wxMessageBox( _( "No connection defined" ), _("Error"), wxOK| wxICON_ERROR );
		return ;
	}
	OnConnect( ConfigName );
}
void MainFrame::OnNewConnection( wxCommandEvent& WXUNUSED( p_Event ) )
{
	int	Ret;
	ConfigurationDlg Dlg(this) ;
	
	Ret = Dlg.ShowModal();
	if( Ret == wxID_OK )
	{
		wxConfig::Get()->Write( wxT( "/lastconfiguration" ), Dlg.Name );
		OnConnect( Dlg.Name );
	}
}

void MainFrame::OnDeleteConnection( wxCommandEvent& WXUNUSED( p_Event ) )
{
	//wxArrayString	Configurations;
	wxString	*Configurations;
	int		nConf;
	long			Index ;
	int				Ret ;
	

	GetConfigurations( Configurations, nConf );	

	wxSingleChoiceDialog Dlg( this, _( "Choose a configuration to delete" ), _( "Configurations" ), 
	nConf, Configurations );
	
	Ret = Dlg.ShowModal();
	
	delete []Configurations;
	
	if( Ret == wxID_CANCEL )
	{
		return ;
	}
	wxString Config = wxString::Format( wxT( "/configurations/%s" ), Dlg.GetStringSelection().c_str() ) ;

	wxConfig::Get()->DeleteGroup( Config );
}

void MainFrame::OnOpenConnection( wxCommandEvent& WXUNUSED( p_Event ) )
{
	//wxArrayString	Configurations;
	wxString	*Configurations;
	int		nConf;
	long			Index ;
	int				Ret ;
	

	GetConfigurations( Configurations, nConf );	

	wxSingleChoiceDialog Dlg( this, _( "Choose a configuration to open" ), _( "Configurations" ), 
	nConf, Configurations );
	
	Ret = Dlg.ShowModal();
	
	delete []Configurations;
	
	if( Ret == wxID_CANCEL )
	{
		return ;
	}
	wxConfig::Get()->Write( wxT( "/lastconfiguration" ), Dlg.GetStringSelection() );
	OnConnect( Dlg.GetStringSelection() );
}

void MainFrame::OnEditConnection( wxCommandEvent& WXUNUSED( p_Event ) )
{
	//wxArrayString	Configurations;
	wxString	*Configurations;
	int		nConf;
	
	int				Ret ;
	
	
	GetConfigurations( Configurations, nConf );
	
	wxSingleChoiceDialog Dlg( this, _( "Choose a configuration to open" ), _( "Configurations" ), 
	nConf, Configurations );
	
	Ret = Dlg.ShowModal();
	
	delete []Configurations;
	
	if( Ret == wxID_CANCEL )
	{
		return ;
	}
	
	ConfigurationDlg DlgConf( this, Dlg.GetStringSelection() );
	
	Ret = DlgConf.ShowModal();
	if( Ret == wxID_OK )
	{
		wxConfig::Get()->Write( wxT( "/lastconfiguration" ), DlgConf.Name );
		OnConnect( DlgConf.Name );
	}
}

void MainFrame::OnLeftSelectionChanged( wxTreeEvent& p_Event )
{
	wxTreeItemId Id = p_Event.GetItem() ;
	if( Id > 0 )
	{
		LDAPItemData* data = ( LDAPItemData* )left_tree_ctrl->GetItemData( p_Event.GetItem() ) ;

		Right_Display( data->m_Value );
		if( left_tree_ctrl->GetChildrenCount( Id, false ) == 0 )
		{
			Left_Display( Id, data->m_Value );
		}
	}
	left_tree_ctrl->Toggle( Id );
}

void MainFrame::OnRightSelectionChanged( wxTreeEvent& p_Event )
{
	wxTreeItemId Id = p_Event.GetItem() ;
	right_tree_ctrl->Expand( Id );
}

void MainFrame::OnRight_PopupMenu( wxTreeEvent& p_Event )
{
	wxTreeItemId Id = right_tree_ctrl->GetSelection() ;
	if( !Id.IsOk() )
		return ;
	wxTreeItemId RootId = right_tree_ctrl->GetRootItem();
	
	wxMenu *Popup = new wxMenu( _( "Entry Attributes" ) );
	if( Popup == NULL )
		return ;
	if( Id == RootId )
	{
		Popup->Append( XRCID( "right_popup_add_att" ), _( "Add Attribute..." ) );
	}
	else if( right_tree_ctrl->GetItemParent( Id ) == RootId )
	{
		Popup->Append( XRCID( "left_popup_del_att" ), _( "Delete Attribute..." ) );
		Popup->AppendSeparator();
		Popup->Append( XRCID( "right_popup_add_value" ), _( "Add value..." ) );
		Popup->Append( XRCID( "right_popup_add_binvalue" ), _( "Add binary value..." ) );
	}
	else
	{
		Popup->Append( XRCID( "right_popup_mod_value" ), _( "Modifiy value..." ) );		
		Popup->Append( XRCID( "right_popup_del_value" ), _( "Delete value" ) );		
		
	}
	PopupMenu( Popup, wxDefaultPosition );
}



void MainFrame::OnLeft_PopupMenu( wxTreeEvent& p_Event )
{
	wxTreeItemId Id = p_Event.GetItem() ;
	if( !Id.IsOk() )
		return ;
	wxTreeItemId RootId = left_tree_ctrl->GetRootItem();

	wxMenu *Popup = new wxMenu( _( "Entry" ) );
	if( Popup == NULL )
		return ;

	Popup->Append( XRCID( "left_popup_add" ), _( "Add..." ) );
	Popup->Append( XRCID( "left_popup_copy" ), _( "Copy to..." ) );
	Popup->Append( XRCID( "left_popup_rename" ), _( "Rename..." ) );
	Popup->Append( XRCID( "left_popup_delete" ), _( "Delete..." ) );
	Popup->AppendSeparator();
	Popup->Append( XRCID( "left_popup_search" ), _( "Search..." ) );		
	
	PopupMenu( Popup, wxDefaultPosition );
}

void MainFrame::OnConnect( const wxString& p_Config )
{
	CnxConfiguration Config ;
	u_long			Version;
	u_long			Port;
	u_long			SSLPort;
	int				Err;
	bool			UseSSL = false ;
	bool			UseTLS = false ;
	int 			Ret ;

	left_tree_ctrl->DeleteAllItems();
	right_tree_ctrl->DeleteAllItems();
	

	m_ConfigName = p_Config ;		
	ReadConfiguration( p_Config, Config );
	
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
		Ret = m_Ldap.SetCACert( Config.cacert );
		if( Ret != LDAP_SUCCESS )
		{
			::wxMessageBox( m_Ldap.GetErrorStr(), _("Set CA Cert" ) );
		}
		if( Config.certfile.Len() > 0 && Config.keyfile.Len() > 0 )
		{
			Ret = m_Ldap.SetClientCert( Config.certfile );
			Ret = m_Ldap.SetClientKey( Config.keyfile );
		}
	}
	Ret = m_Ldap.Init( Config.server, Port );
	if( Ret != LDAP_SUCCESS )
	{
		::wxMessageBox( wxString::Format( wxT( "LDAP open:%s" ), m_Ldap.GetErrorStr().c_str() ) );
		return;
	}
	if( m_Ldap.SetVersion( Version ) != LDAP_SUCCESS )
	{
		::wxMessageBox( wxString::Format( wxT( "LDAP SetVersion:%s" ), m_Ldap.GetErrorStr().c_str() ) );
		return;
	}
	
	if( UseTLS )
	{	
		Ret = m_Ldap.StartTLS( );
		if( Ret != LDAP_SUCCESS )
		{
			::wxMessageBox( wxString::Format( wxT( "LDAP StartTLS:%s" ), m_Ldap.GetErrorStr().c_str() ) );
			return;
		}
	}
	if( UseSSL )
	{
		int SSLArg = LDAP_OPT_X_TLS_HARD ;
		Ret = m_Ldap.SetOption( LDAP_OPT_X_TLS, &SSLArg );
		if( Ret != LDAP_SUCCESS )
		{
			::wxMessageBox( m_Ldap.GetErrorStr().c_str(), _( "SetOption:LDAP_OPT_X_TLS" )  );
			return;
		}
	}

	unsigned long SizeLimit ;
	unsigned long TimeLimit ;
	
	Config.sizelimit.ToULong( &SizeLimit );
	Config.timelimit.ToULong( &TimeLimit );
	
	if( Config.sizelimit.Len() > 0 )
	{
		Err = m_Ldap.SetSizeLimit( SizeLimit );
	}
	
	if( Config.timelimit.Len() > 0 )
	{
		Err = m_Ldap.SetTimeLimit( TimeLimit );
	}
	
	if( Config.referals == _( "Yes" ) )
	{
		Err = m_Ldap.Referals( true ) ;
	}
	else
	{
		Err = m_Ldap.Referals( false ) ;
	}
	
	if( Config.anonymous_login == wxT( "1" ) )
	{
		Err = m_Ldap.SimpleBind();
	}
	else
	{
		if( Config.storepwd )
		{
			Err = m_Ldap.SimpleBind( Config.userdn, Config.password );
		}
		else
		{
			//wxPasswordEntryDialog Dlg( this, wxT("Enter user password") );
			wxTextEntryDialog Dlg( this, wxT("Enter user password"),wxT("Enter password"), wxT(""), wxOK|wxCANCEL|wxTE_PASSWORD );
			Dlg.ShowModal();
			Err = m_Ldap.SimpleBind( Config.userdn, Dlg.GetValue());
		}
	}
	
	if( Err != LDAP_SUCCESS )
	{
		::wxMessageBox( wxString::Format( wxT( "LDAP Bind:%s" ), m_Ldap.GetErrorStr().c_str() ) );
		return;
	}
	
	m_Ldap.SetConfig( m_ConfigName );
	if( Config.guessbinary.CmpNoCase( _("Yes" ) ) == 0 )
		m_Ldap.GuessBinary( true );
	else
		m_Ldap.GuessBinary( false );
	
	m_Ldap.GetSchema( Config.basedn );
	
	wxTreeItemId RootID = left_tree_ctrl->AddRoot( wxString::Format( _("Root-%s"), Config.basedn.c_str() ) );
	left_tree_ctrl->SetItemData( RootID, new LDAPItemData( Config.basedn ) );
	Left_Display( RootID, Config.basedn );
	left_tree_ctrl->Expand( RootID );

}

void MainFrame::Left_Display( wxTreeItemId p_ParentId, const wxString& p_Url )
{
	int Id;
	int Ret;
	wxSortedArrayString DN;
	wxSortedArrayString EndOfDn;
	
	Ret = m_Ldap.Search( Id, p_Url, LDAP_SCOPE_ONELEVEL, wxT( "objectclass=*" ), 
	NULL, false, LF_NONE );
	if( Ret != LDAP_SUCCESS )
	{
		return ;
	}

	int NbEntries = m_Ldap.GetEntryCount( Id ) ;
	for( int i = 0; i < NbEntries; i++ )
	{
		LdapEntry Entry ;
		if( !m_Ldap.GetEntry( Id, i, Entry ) )
		{
			::wxMessageBox( m_Ldap.GetErrorStr(), _("Error"), wxOK|wxICON_ERROR );
		}

		EndOfDn.Add(Entry.GetEndOfDn());
		DN.Add(Entry.GetDn());
	}

	m_Ldap.CloseSearch( Id );

	for ( int j = 0; j < EndOfDn.Count(); j++ )
	{
		wxTreeItemId NewId = left_tree_ctrl->AppendItem( p_ParentId, EndOfDn.Item( j ) );
		left_tree_ctrl->SetItemData( NewId, new LDAPItemData( DN.Item( j ) ) );
	}
}


void MainFrame::Right_Display( wxString p_Url )
{
	int Id;
	int Ret;
	
	right_tree_ctrl->DeleteAllItems();
	
	Ret = m_Ldap.Search( Id, p_Url, LDAP_SCOPE_BASE, wxT( "objectclass=*" ), 
	NULL, true, LF_ALL_OPERATIONAL );
	if( Ret != LDAP_SUCCESS )
	{
		::wxMessageBox( wxString::Format( wxT( "Search:%s" ),m_Ldap.GetErrorStr().c_str() ), _("Error"), wxOK|wxICON_ERROR );
		return ;
	}
	

	LdapEntry Entry ;
	if( !m_Ldap.GetEntry( Id, 0, Entry ) )
	{
		::wxMessageBox( m_Ldap.GetErrorStr(), _("Error"), wxOK|wxICON_ERROR );
	}
	wxTreeItemId NewId = right_tree_ctrl->AddRoot( Entry.GetDn() );
	right_tree_ctrl->SetItemData( NewId, new LDAPItemData( Entry.GetDn() ) );
	
	long Nb = Entry.CountAttributes();
	for( long i = 0; i < Nb ; i++ )
	{
		LdapAttribute* Attribute = Entry.GetAttribute( i );
		
		wxTreeItemId ItemId = right_tree_ctrl->AppendItem( NewId, Attribute->GetName() );
		long NbVals = Attribute->CountValues();
		
		for( long j = 0; j < NbVals; j++ )
		{
			wxTreeItemId AttValId = right_tree_ctrl->AppendItem( ItemId, Attribute->GetValue( j ) );
			if( Attribute->IsBinary() )
			{
				right_tree_ctrl->SetItemData( AttValId, 
				new ValueItemData( BINARY_VALUES, ber_bvdup( Attribute->GetBinaryValue( j ) ) ) );
			}
			else
			{
				right_tree_ctrl->SetItemData( AttValId, new ValueItemData( STRING_VALUES ) );
			}
		}
		right_tree_ctrl->SortChildren( ItemId );
	}
	right_tree_ctrl->SortChildren( NewId );
	right_tree_ctrl->Expand( NewId );
	m_Ldap.CloseSearch( Id );	
}

void MainFrame::OnAddEntry( wxCommandEvent& WXUNUSED( p_Event ) )
{
	wxTreeItemId Id ;
	wxTreeItemId NewId ;
	int		Ret;
	
	Id = left_tree_ctrl->GetSelection();
	if( !Id.IsOk() )
		return ;
	LDAPItemData* Data = (LDAPItemData*)left_tree_ctrl->GetItemData( Id ) ;
	
	NewEntryDlg	Dlg( this, Data->m_Value, m_Ldap );

		
	Ret = Dlg.ShowModal();
	if( Ret == wxID_OK )
	{
		NewId = left_tree_ctrl->AppendItem( Id, Dlg.m_NewRdn );
		left_tree_ctrl->SetItemData( NewId, new LDAPItemData( Dlg.m_NewDn ) );
	}
}

void MainFrame::OnCopyEntry( wxCommandEvent& WXUNUSED( p_Event ) )
{
	wxTreeItemId 	Id ;
	int				SId;
	int				Ret;
	int				Choice ;
	int				Scope ;
	wxString		NewRDn;
	LdapEntry		Entry;
	wxArrayString	Choices;
	
	Choices.Add( _( "Selected entry" ) );
	Choices.Add( _( "Selected entry sub-level" ) );
	Choices.Add( _( "Selected entry and all sub-levels" ) );

	Id = left_tree_ctrl->GetSelection();
	if( !Id.IsOk() )
		return ;

	Choice = ::wxGetSingleChoiceIndex( _( "Select the entries you wish to copy" ), 
	_( "Copy To..." ), Choices );
	if( Choice == -1 )
		return ;
		
	switch( Choice )
	{
		case 0:
			Scope = LDAP_SCOPE_BASE ;
		break;
		case 1:
			Scope = LDAP_SCOPE_ONELEVEL ;
		break;
		case 2:
			Scope = LDAP_SCOPE_SUBTREE;
		break;
	}
	LDAPItemData* Data = (LDAPItemData*)left_tree_ctrl->GetItemData( Id ) ;
	
	Ret = m_Ldap.Search( SId, Data->m_Value, Scope, wxT( "objectClass=*" ), NULL,
	true, LF_ALL );
	if( Ret != LDAP_SUCCESS )
	{
		::wxMessageBox( m_Ldap.GetErrorStr(), Data->m_Value );
		return;
	}
	
	wxString NewDn = ::wxGetTextFromUser( _( "Enter the new parent DN" ), Data->m_Value, wxT( "" ) );
	if( NewDn == wxT( "" ) )
		return ;

	wxString OldDn;
	if( Choice == 1 )
	{
		OldDn = Data->m_Value;
	}
	else
	{
		OldDn = Data->m_Value.AfterFirst( ',' );
	}
	Ret = m_Ldap.CopyTo( SId, NewDn, OldDn );
	if( Ret != LDAP_SUCCESS )
	{
		::wxMessageBox( m_Ldap.GetErrorStr(), _( "Error" ) );
	}
	
	m_Ldap.CloseSearch( SId );
}

void MainFrame::OnRenameEntry( wxCommandEvent& WXUNUSED( p_Event ) )
{
	wxTreeItemId 	Id ;
	int				Ret;
	wxString		NewRDn ;
	Id = left_tree_ctrl->GetSelection();
	if( !Id.IsOk() )
		return ;
		
	LDAPItemData* Data = (LDAPItemData*)left_tree_ctrl->GetItemData( Id ) ;

	NewRDn = ::wxGetTextFromUser( _("Enter the new RDN of the entry" ), Data->m_Value ) ;
	if( NewRDn.Len() == 0 )
		return;
		
	Ret = m_Ldap.Rename( Data->m_Value, NewRDn );
	if( Ret != LDAP_SUCCESS )
	{
		::wxMessageBox( m_Ldap.GetErrorStr(), Data->m_Value ) ;
	}
	else
	{
		
		int Pos = Data->m_Value.Find( ',' );
		if ( Pos == -1 )
		{
			Data->m_Value = NewRDn ;
			::wxMessageBox ( wxT( "Assigned new RDN as absolute" ) );
		}
		else
		{
			Data->m_Value = (Data->m_Value.Right( ( Data->m_Value).Length() - Pos )).Prepend(NewRDn);
		}
		left_tree_ctrl->SetItemData( Id, Data );
		left_tree_ctrl->SetItemText( Id, NewRDn );
	}
}

void MainFrame::OnSearch( wxCommandEvent& WXUNUSED( p_Event ) )
{

	wxTreeItemId Id = left_tree_ctrl->GetSelection();
	if( !Id.IsOk() )
		return ;
		
	
	LDAPItemData* Data = (LDAPItemData*)left_tree_ctrl->GetItemData( Id ) ;
	
	SearchDlg Dlg( this, &m_Ldap, Data->m_Value );
	
	if( Dlg.ShowModal() == wxID_OK )
	{
		left_tree_ctrl->DeleteAllItems();
		right_tree_ctrl->DeleteAllItems();

		wxTreeItemId RootID = left_tree_ctrl->AddRoot( Data->m_Value );
		left_tree_ctrl->SetItemData( RootID, new LDAPItemData( Data->m_Value ) );
		int NbEntries = m_Ldap.GetEntryCount( Dlg.Id );
		for( int i = 0; i < NbEntries; i++ )
		{
			LdapEntry Entry ;
			if( !m_Ldap.GetEntry( Dlg.Id, i, Entry ) )
			{
				::wxMessageBox( m_Ldap.GetErrorStr(), _("Error"), wxOK|wxICON_ERROR );
			}
			
			wxTreeItemId NewId = left_tree_ctrl->AppendItem( RootID, Entry.GetDn() );
			left_tree_ctrl->SetItemData( NewId, new LDAPItemData( Entry.GetDn() ) );
		}
		m_Ldap.CloseSearch( Dlg.Id );
	}
}

void MainFrame::OnDeleteEntry( wxCommandEvent& WXUNUSED( p_Event ) )
{
	LdapEntry Entry ;
	wxTreeItemId 	Id ;
	int				Ret;
	int				SId;
		
	Id = left_tree_ctrl->GetSelection();
	if( !Id.IsOk() )
		return ;
		
	LDAPItemData* Data = (LDAPItemData*)left_tree_ctrl->GetItemData( Id ) ;
	
	if( ::wxMessageBox( _("Do you really want to delete this entry and all its sub-entries?"), Data->m_Value, wxYES_NO ) == wxNO )
	{
		return;
	}
	
	Ret = m_Ldap.Search( SId, Data->m_Value, LDAP_SCOPE_SUBTREE, wxT( "objectClass=*" ), NULL,
	true, LF_NONE );
	if( Ret != LDAP_SUCCESS )
	{
		::wxMessageBox( m_Ldap.GetErrorStr(), Data->m_Value );
		return;
	}
		
	
	int nEntries = m_Ldap.GetEntryCount( SId );
	
	for( long i = nEntries - 1; i >= 0; i-- )
	{
		m_Ldap.GetEntry( SId, i, Entry );
		Ret = m_Ldap.DeleteEntry( Entry );
		if( Ret != LDAP_SUCCESS )
		{
			::wxMessageBox( m_Ldap.GetErrorStr(), Data->m_Value ) ;
			break;
		}
	}
	if( Ret == LDAP_SUCCESS )
	{
		left_tree_ctrl->Delete( Id );
	}
}

void MainFrame::OnAddAttribute( wxCommandEvent& WXUNUSED( p_Event ) )
{
	int Id;
	int Ret;
	int Count;
	wxTreeItemId RootId;
	LDAPItemData *Data ;
	LdapEntry	Entry, NewEntry ;
	LdapAttribute *Attribute ;
	wxString ObjectClass;
	wxArrayString ObjectClasses;
	
	RootId = right_tree_ctrl->GetRootItem();
	Data = (LDAPItemData*)right_tree_ctrl->GetItemData( RootId );
	
	Ret = m_Ldap.Search( Id, Data->m_Value, LDAP_SCOPE_BASE, wxT( "objectclass=*" ), 
	NULL, true, LF_ALL_OPERATIONAL );
	if( Ret != LDAP_SUCCESS )
	{
		::wxMessageBox( m_Ldap.GetErrorStr(), Data->m_Value );
		return;
	}
	
	if( !m_Ldap.GetEntry( Id, 0, Entry ))
	{
		::wxMessageBox( m_Ldap.GetErrorStr(), Data->m_Value );
		m_Ldap.CloseSearch( Id );
		return;
	}
	
	Attribute = Entry.GetAttribute( wxT( "objectclass" ), true );

	if( Attribute != NULL )
	{
		Count = Attribute->CountValues();

		for( int i = 0; i < Count; i++ ) {
			ObjectClass = Attribute->GetValue( i );
			ObjectClasses.Add( ObjectClass );
		}
	}

	m_Ldap.CloseSearch( Id );
	
	Attribute_Add_Dlg Dlg( this, Data->m_Value, ObjectClasses, m_Ldap );
	
	Ret = Dlg.ShowModal();
	if( Ret == wxID_OK )
	{
		Right_Display( Data->m_Value );
	}
	
}

void MainFrame::OnDeleteAttribute( wxCommandEvent& WXUNUSED( p_Event ) )
{
	wxTreeItemId Id = right_tree_ctrl->GetSelection(); 
	wxTreeItemId RootId = right_tree_ctrl->GetRootItem();
	if( !Id.IsOk() )
		return ;
	
	wxString Attribute = right_tree_ctrl->GetItemText( Id );
	if( ::wxMessageBox( _("Do you really want to delete this attribute?"), Attribute, wxYES_NO ) == wxYES )
	{
		//LdapEntry Entry ;
		LDAPItemData* Data = ( LDAPItemData* )right_tree_ctrl->GetItemData( RootId ) ;
		/*
		Entry.SetDn( Data->m_Value );
		Entry.AddValue( Attribute, "" );
		*/
		int Err = m_Ldap.DeleteAttribute( Data->m_Value, Attribute );
		if( Err != LDAP_SUCCESS )
		{
			::wxMessageBox( m_Ldap.GetErrorStr(), Attribute );
		}
		else
		{
			right_tree_ctrl->Delete( Id );
		}
	}	
	
	
	
}

void MainFrame::OnAddValue( wxCommandEvent& WXUNUSED( p_Event ) )
{
	wxTreeItemId 	Id ;
	wxTreeItemId 	RootId ;
	wxTreeItemId 	ParentId ;
	LDAPItemData* 	data;
	wxString		Value ;
	wxString		Attribute ;
	int				Err ;
	
	Id = right_tree_ctrl->GetSelection();
	if( !Id.IsOk() )
		return ;

	ParentId = right_tree_ctrl->GetItemParent( Id ) ;
	RootId = right_tree_ctrl->GetRootItem();
	
	if( ParentId != RootId )
	{
		Id = ParentId;
	}
	Attribute = right_tree_ctrl->GetItemText( Id ) ;
	data = ( LDAPItemData* )right_tree_ctrl->GetItemData( RootId ) ;
	
	
	Value = ::wxGetTextFromUser( _( "Enter a value for the item" ), Attribute , wxT( "" ) );
	if( Value.Len() == 0 )
	{
		return ;
	}
	
	Err = m_Ldap.AddAttributeValue( data->m_Value, Attribute, Value );
	if( Err != LDAP_SUCCESS )
	{
		::wxMessageBox( m_Ldap.GetErrorStr(), _( "Error" ) );
	}
	else
	{
		Right_Display( data->m_Value );
	}
			
}
void MainFrame::OnAddBinValue( wxCommandEvent& WXUNUSED( p_Event ) )
{
	wxTreeItemId 	Id ;
	wxTreeItemId 	RootId ;
	wxTreeItemId 	ParentId ;
	LDAPItemData* 	data;
	wxString		Attribute ;
	int				Err ;
	
	Id = right_tree_ctrl->GetSelection();
	if( !Id.IsOk() )
		return ;

	ParentId = right_tree_ctrl->GetItemParent( Id ) ;
	RootId = right_tree_ctrl->GetRootItem();
	
	if( ParentId != RootId )
	{
		Id = ParentId;
	}
	Attribute = right_tree_ctrl->GetItemText( Id ) ;
	data = ( LDAPItemData* )right_tree_ctrl->GetItemData( RootId ) ;
	
	
	wxString Filename = wxFileSelector( _( "Choose a file to open" ));
	if ( Filename.empty() )
	{
		return ;
	}

	wxFile File( Filename );
	if( !File.IsOpened() )
	{
		::wxMessageBox( _( "File could not be opened" ), Filename );
		return;
	}
	
	berval	Ber ;
	
	Ber.bv_len = File.Length() ;
	Ber.bv_val = new char[ Ber.bv_len ] ;
	File.Read( Ber.bv_val, Ber.bv_len );

	
	Err = m_Ldap.AddAttributeValue( data->m_Value, Attribute, Ber );
	
	delete []Ber.bv_val	;
	if( Err != LDAP_SUCCESS )
	{
		::wxMessageBox( m_Ldap.GetErrorStr(), _( "Error" ) );
	}
	else
	{
		Right_Display( data->m_Value );
	}
			
}

void MainFrame::OnModifyValue( wxCommandEvent& WXUNUSED( p_Event ) )
{
	wxTreeItemId 	Id ;
	wxTreeItemId 	RootId ;
	wxTreeItemId 	ParentId ;
	LDAPItemData* 	data;
	wxString		OldValue ;
	wxString		NewValue;
	wxString		Attribute ;
	int				Err ;
	
	Id = right_tree_ctrl->GetSelection();
	if( !Id.IsOk() )
		return ;

	ParentId = right_tree_ctrl->GetItemParent( Id ) ;
	RootId = right_tree_ctrl->GetRootItem();
	
	if( ParentId == RootId )
	{
		return ;
	}
	Attribute = right_tree_ctrl->GetItemText( ParentId ) ;
	data = ( LDAPItemData* )right_tree_ctrl->GetItemData( RootId ) ;
	OldValue = right_tree_ctrl->GetItemText( Id );
	
	NewValue = ::wxGetTextFromUser( _( "Modify value for this item" ), Attribute , OldValue );
	if( NewValue.Len() == 0 )
	{
		return ;
	}
	
	Err = m_Ldap.ModifyAttributeValue( data->m_Value, Attribute, NewValue );
	if( Err != LDAP_SUCCESS )
	{
		::wxMessageBox( m_Ldap.GetErrorStr(), _( "Error" ) );
		return ;
	}
	Right_Display( data->m_Value );
}

void MainFrame::OnDeleteValue( wxCommandEvent& WXUNUSED( p_Event ) )
{
	wxTreeItemId 	Id ;
	wxTreeItemId	ParentId;
	wxTreeItemId 	RootId ;
	LDAPItemData* 	data;
	ValueItemData*	valuedata ;
	LdapEntry		Entry ;
	wxString		Value ;
	wxString		Attribute ;
	int				Err ;
	
	Id = right_tree_ctrl->GetSelection();
	if( !Id.IsOk() )
		return ;
	
	RootId = right_tree_ctrl->GetRootItem();
	ParentId = right_tree_ctrl->GetItemParent( Id ) ;
	if( ParentId == RootId )
	{
		return ;
	}
	
	Attribute = right_tree_ctrl->GetItemText( ParentId ) ;
	Value = right_tree_ctrl->GetItemText( Id ) ;
	
	data = ( LDAPItemData* )right_tree_ctrl->GetItemData( RootId ) ;
	
	valuedata = ( ValueItemData* )right_tree_ctrl->GetItemData( Id ) ;
	
	if( valuedata != NULL && valuedata->m_Type == BINARY_VALUES )
	{
		Err = m_Ldap.DeleteAttributeValue( data->m_Value, Attribute, *( valuedata->m_BerVal ) );
	}
	else
	{
		Err = m_Ldap.DeleteAttributeValue( data->m_Value, Attribute, Value );
	}
	if( Err != LDAP_SUCCESS )
	{
		::wxMessageBox( m_Ldap.GetErrorStr(), _( "Error" ) );
	}
	else
	{
		Right_Display( data->m_Value );
	}
	
}
