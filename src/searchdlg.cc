#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include <wx/xrc/xmlres.h>

#include "searchdlg.h"


BEGIN_EVENT_TABLE( SearchDlg, wxDialog )
EVT_BUTTON(XRCID( "button_search" ), SearchDlg::OnSearch) 
EVT_BUTTON(XRCID( "button_close" ), SearchDlg::OnClose) 
EVT_BUTTON(XRCID( "button_open" ), SearchDlg::OnOpen) 
END_EVENT_TABLE()

SearchDlg::SearchDlg( wxWindow *p_parent, SchemaUtil *p_ldap, const wxString& p_base )
{
	m_ldap = p_ldap;
	m_base = p_base;
	wxXmlResource::Get()->LoadDialog( this, p_parent, wxT( "search_dlg" ) );
	InitControls();
	searchbase_lbl->SetLabel( m_base );
}
SearchDlg::~SearchDlg()
{
}
void SearchDlg::InitControls( void )
{
		searchbase_lbl=(wxStaticText*)FindWindowById( XRCID("searchbase_lbl"),this );
		searchlevel_radio=(wxRadioBox*)FindWindowById( XRCID("searchlevel_radio"),this );
		searchfilter_textctrl=(wxTextCtrl*)FindWindowById( XRCID("searchfilter_textctrl"),this );
		searchresult_listbox=(wxListBox*)FindWindowById( XRCID("searchresult_listbox"),this );
}

void SearchDlg::OnSearch( wxCommandEvent& p_evt )
{
	int Ret;
	int scope ;
	
	wxString search_level = searchlevel_radio->GetStringSelection();

	searchresult_listbox->Clear();	
	if( search_level == wxT("One Sub Level") )
	{
		scope = LDAP_SCOPE_ONELEVEL;
	}
	else
	{
		scope = LDAP_SCOPE_SUBTREE;
	}	
	Ret = m_ldap->Search( Id, m_base, scope, searchfilter_textctrl->GetValue(), 
	NULL, false, LF_NONE );
	if( Ret != LDAP_SUCCESS )
	{
		return ;
	}
	

	int NbEntries = m_ldap->GetEntryCount( Id ) ;
	for( int i = 0; i < NbEntries; i++ )
	{
		LdapEntry Entry ;
		if( !m_ldap->GetEntry( Id, i, Entry ) )
		{
			::wxMessageBox( m_ldap->GetErrorStr(), _("Error"), wxOK|wxICON_ERROR );
		}
		searchresult_listbox->Append( Entry.GetDn() );
	}
//	m_ldap->CloseSearch( Id );
}
void SearchDlg::OnClose( wxCommandEvent& p_evt )
{
	m_ldap->CloseSearch( Id );
	EndModal( wxID_CANCEL );
}
void SearchDlg::OnOpen( wxCommandEvent& p_evt )
{
	EndModal( wxID_OK );
}
