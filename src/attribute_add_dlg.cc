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
#include <wx/listctrl.h>
#include <wx/config.h>

#include "ldapentry.h"
#include "ldapclass.h"
#include "config_fct.h"
#include "attribute_add_dlg.h"


BEGIN_EVENT_TABLE( Attribute_Add_Dlg, wxDialog )
EVT_BUTTON( XRCID( "button_save" ), Attribute_Add_Dlg::OnSave )

END_EVENT_TABLE()


Attribute_Add_Dlg::Attribute_Add_Dlg( wxWindow* p_Parent, const wxString& p_Dn, wxArrayString p_ObjectClasses, SchemaUtil& p_Ldap )
{
	int	Err ;
	SetEscapeId( XRCID("button_cancel") );
	wxXmlResource::Get()->LoadDialog( this, p_Parent, wxT( "attribute_add" ) );
	InitControls();
	m_Ldap = &p_Ldap ;

	m_Dn = p_Dn ;
	
	if( !m_Ldap->HasSchema() )
	{
		SetManual( true );
	}
	else
	{
		SetManual( false );
		wxSortedArrayString MayAt ;
		attributes_list_box->Clear();

		for( int i = 0; i < p_ObjectClasses.Count(); i++ ) {

			m_Ldap->GetMAYAttributeTypes( p_ObjectClasses.Item(i), MayAt );
		}

		attributes_list_box->InsertItems( MayAt, 0 );
	}
}

void Attribute_Add_Dlg::InitControls()
{
	attributes_list_box=(wxListBox*)FindWindowById( XRCID("attributes_list_box"),this );
	attribute_text_ctrl=(wxTextCtrl*)FindWindowById( XRCID("attribute_text_ctrl"),this );
	value_text_ctrl=(wxTextCtrl*)FindWindowById( XRCID("value_text_ctrl"),this );
	button_save=(wxButton*)FindWindowById( XRCID("button_save"),this );
	button_cancel=(wxButton*)FindWindowById( XRCID("button_cancel"),this );

}

void Attribute_Add_Dlg::OnSave( wxCommandEvent& WXUNUSED( p_Event ) )
{
	LdapEntry	Entry ;
	wxString 	Att;
	Entry.SetDn( m_Dn );
	wxString	Value ;
	
	if( attributes_list_box->IsEnabled() )
	{
		Att = attributes_list_box->GetStringSelection();
	}
	else
	{
		Att = attribute_text_ctrl->GetValue() ;
	}

	if( Att.Len() == 0 )
	{
		::wxMessageBox( _("You must select an attribute"), _( "Error" ) ) ;
		return ;
	}
	Value =  value_text_ctrl->GetValue();
	if( Value.Len() == 0 )
	{
		::wxMessageBox( _("You must enter a value for the attribute"), Att ) ;
		return ;
	}
	
	Entry.AddValue( Att, Value );
	int Err = m_Ldap->AddAttribute( Entry );
	if( Err != LDAP_SUCCESS )
	{
		::wxMessageBox( m_Ldap->GetErrorStr(), Att );
		return ;
	}
	EndModal( wxID_OK );
}
void Attribute_Add_Dlg::SetManual( bool p_Manual )
{
	attributes_list_box->Enable( !p_Manual );
	
	attribute_text_ctrl->Enable( p_Manual );
}
