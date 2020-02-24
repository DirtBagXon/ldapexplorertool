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
#include "newentrydlg.h"


BEGIN_EVENT_TABLE( NewEntryDlg, wxDialog )
EVT_COMBOBOX(XRCID("objectclass_combo_box"), NewEntryDlg::OnObjectClassChanged)
EVT_TEXT_ENTER(XRCID("objectclass_combo_box"), NewEntryDlg::OnObjectClassChanged)
EVT_LISTBOX_DCLICK(XRCID("mustattributes_list_box"), NewEntryDlg::OnMustAdd) 
EVT_LISTBOX_DCLICK(XRCID("mayattributes_list_box"), NewEntryDlg::OnMayAdd) 
EVT_TEXT(XRCID("entryrdn_text_ctrl"), NewEntryDlg::OnRdnChanged) 
EVT_TREE_ITEM_RIGHT_CLICK(XRCID("attributes_tree_ctrl"), NewEntryDlg::OnTreeRightClick) 
EVT_MENU(XRCID( "value_add" ), NewEntryDlg::OnAddValue )
EVT_MENU(XRCID( "value_del" ), NewEntryDlg::OnDelValue )
EVT_MENU(XRCID( "delete_attribute" ), NewEntryDlg::OnDeleteAttribute )
EVT_BUTTON( XRCID( "button_save" ), NewEntryDlg::OnSave )
EVT_BUTTON( XRCID( "button_cancel" ), NewEntryDlg::OnCancel )
EVT_BUTTON( XRCID( "button_add" ), NewEntryDlg::OnManualAdd )

END_EVENT_TABLE()



NewEntryDlg::NewEntryDlg( wxWindow* p_Parent, const wxString& p_ParentDn, SchemaUtil& p_Ldap ) 
{
	
	int	Err ;
	wxXmlResource::Get()->LoadDialog( this, p_Parent, wxT( "entry_create" ) );
	InitControls();
	m_Ldap = &p_Ldap ;
	wxArrayString	ObjectClasses ;
	parentdn_text_ctrl->SetValue( p_ParentDn );
	attributes_tree_ctrl->AddRoot( p_ParentDn );
	
	if( !m_Ldap->HasSchema() )
	{
		SetManual( true );
	}
	else
	{
		m_Ldap->GetObjectClasses( ObjectClasses );
		for( long i = 0; i < ObjectClasses.GetCount(); i++ )
		{
			objectclass_combo_box->Append( ObjectClasses[i] );
		}
		SetManual( false );
	}
	
	
}

void NewEntryDlg::InitControls()
{
	parentdn_text_ctrl=(wxTextCtrl*)FindWindowById( XRCID("parentdn_text_ctrl"),this );
	entryrdn_text_ctrl=(wxTextCtrl*)FindWindowById( XRCID("entryrdn_text_ctrl"),this );
	objectclass_text_ctrl=(wxTextCtrl*)FindWindowById( XRCID("objectclass_text_ctrl"),this );
	attribute_text_ctrl=(wxTextCtrl*)FindWindowById( XRCID("attribute_text_ctrl"),this );
	attributes_tree_ctrl=(wxTreeCtrl*)FindWindowById( XRCID("attributes_tree_ctrl"),this );
	mustattributes_list_box=(wxListBox*)FindWindowById( XRCID("mustattributes_list_box"),this );
	mayattributes_list_box=(wxListBox*)FindWindowById( XRCID("mayattributes_list_box"),this );
	objectclass_combo_box=(wxComboBox*)FindWindowById( XRCID("objectclass_combo_box"),this );
	button_add=(wxButton*)FindWindowById( XRCID("button_add"),this );
	button_save=(wxButton*)FindWindowById( XRCID("button_save"),this );
	button_cancel=(wxButton*)FindWindowById( XRCID("button_cancel"),this );

}
void NewEntryDlg::OnObjectClassChanged( wxCommandEvent& p_Event )
{
	wxString ObjectClass = objectclass_combo_box->GetValue() ;
	
	wxArrayString	MustAt,MayAt ;
	
	m_Ldap->GetMUSTAttributeTypes( ObjectClass, MustAt );
	m_Ldap->GetMAYAttributeTypes( ObjectClass, MayAt );
	
	mustattributes_list_box->Clear();
	mustattributes_list_box->InsertItems( MustAt, 0 );
	mayattributes_list_box->Clear();
	mayattributes_list_box->InsertItems( MayAt, 0 );
}
void NewEntryDlg::OnMustAdd( wxCommandEvent& p_Event )
{
	wxString Attribute = mustattributes_list_box->GetStringSelection();
	wxTreeItemId RootId = attributes_tree_ctrl->GetRootItem();
	wxTreeItemId NewId ;
	NewId = attributes_tree_ctrl->AppendItem( RootId, Attribute );
	
	if( Attribute.CmpNoCase( wxT( "objectclass" ) ) == 0 )
	{
		if( objectclass_combo_box->GetStringSelection().Len() > 0 )
		{
			attributes_tree_ctrl->AppendItem( NewId, objectclass_combo_box->GetStringSelection() );
		}
	}
	attributes_tree_ctrl->EnsureVisible( NewId );
}
void NewEntryDlg::OnMayAdd( wxCommandEvent& p_Event )
{
	wxString Attribute = mayattributes_list_box->GetStringSelection();
	wxTreeItemId RootId = attributes_tree_ctrl->GetRootItem();
	wxTreeItemId NewId ;
	NewId = attributes_tree_ctrl->AppendItem( RootId, Attribute );
	attributes_tree_ctrl->EnsureVisible( NewId );
}
void NewEntryDlg::OnManualAdd( wxCommandEvent& p_Event )
{
	wxTreeItemId RootId = attributes_tree_ctrl->GetRootItem();
	wxTreeItemId NewId ;
	wxString NewAtt = attribute_text_ctrl->GetValue();
	
	if( NewAtt.Len() == 0 )
		return ;
		
	NewId = attributes_tree_ctrl->AppendItem( RootId, NewAtt );
	attributes_tree_ctrl->EnsureVisible( NewId );
}
void NewEntryDlg::OnRdnChanged( wxCommandEvent& p_Event )
{
	wxTreeItemId RootId = attributes_tree_ctrl->GetRootItem();
	wxString NewRdn = entryrdn_text_ctrl->GetValue() ;
	if( parentdn_text_ctrl->GetValue().Len() > 0 )
	{
		NewRdn.Append( wxT( "," ) );
		NewRdn.Append( parentdn_text_ctrl->GetValue() );
	}
	attributes_tree_ctrl->SetItemText( RootId, NewRdn );		
}

void NewEntryDlg::OnTreeRightClick( wxTreeEvent& p_Event )
{
	wxTreeItemId Id = p_Event.GetItem() ;
	wxTreeItemId RootId ;
	if( !Id.IsOk() )
		return ;
	
	RootId = attributes_tree_ctrl->GetRootItem();
	if( Id == RootId )
		return ;
	wxMenu *Popup = new wxMenu( _( "Attribute" ) );
	if( Popup == NULL )
		return ;

	if( attributes_tree_ctrl->GetItemParent( Id ) == RootId )
	{
		Popup->Append( XRCID( "delete_attribute" ), _( "Remove attribute" ) );
		Popup->AppendSeparator();
		Popup->Append( XRCID( "value_add" ), _( "Add value" ) );
	}
	else
	{
		Popup->Append( XRCID( "value_add" ), _( "Add value..." ) );
		Popup->Append( XRCID( "value_del" ), _( "Remove value" ) );
	}
	
	PopupMenu( Popup, wxDefaultPosition );
}

void NewEntryDlg::OnAddValue( wxCommandEvent& WXUNUSED( p_Event ) )
{
	wxTreeItemId Id ;
	wxTreeItemId AttId ;
	wxTreeItemId RootId ;

	Id = attributes_tree_ctrl->GetSelection();
	RootId = attributes_tree_ctrl->GetRootItem();
	if( Id == RootId )
		return ;
	if( attributes_tree_ctrl->GetItemParent( Id ) == RootId )
	{
		AttId = Id ;
	}
	else
	{
		AttId = attributes_tree_ctrl->GetItemParent( Id );
	}
	wxString Attribute = attributes_tree_ctrl->GetItemText( AttId );
	wxString NewValue = ::wxGetTextFromUser( _("Enter a new value" ), Attribute, wxT( "" ) );
	if( NewValue.Len() > 0 )
	{
		Id = attributes_tree_ctrl->AppendItem( AttId, NewValue );
		attributes_tree_ctrl->EnsureVisible( Id );
	}
}
void NewEntryDlg::OnDelValue( wxCommandEvent& WXUNUSED( p_Event ) )
{
	wxTreeItemId Id ;
	Id = attributes_tree_ctrl->GetSelection();
	
	attributes_tree_ctrl->Delete( Id );
}

void NewEntryDlg::OnDeleteAttribute( wxCommandEvent& WXUNUSED( p_Event ) )
{
	wxTreeItemId Id ;
	Id = attributes_tree_ctrl->GetSelection();
	
	attributes_tree_ctrl->Delete( Id );
}
void NewEntryDlg::OnSave( wxCommandEvent& WXUNUSED( p_Event ) )
{
	LdapEntry	Entry ;
	wxString Rdn = entryrdn_text_ctrl->GetValue();
	wxString ParentDn = parentdn_text_ctrl->GetValue() ;
	
	if( Rdn.Len() == 0 )
	{
		::wxMessageBox( _("You must enter the RDN of the entry" ), _("Error" ) );
		return;
	}
	/*
	if( ParentDn.Len() == 0 )
	{
		::wxMessageBox( _("You must enter the parent DN of the entry" ), _("Error" ) );
		return;
	}
	*/
	wxString Dn = Rdn ;
	if( ParentDn.Len() > 0 )
	{
		Dn.Append( wxT( "," ) );
		Dn.Append( ParentDn );
	}
	Entry.SetDn( Dn );
	
	wxTreeItemId RootId, CurId, CurId2 ;
	wxTreeItemIdValue Cookie, Cookie2;
	
	RootId = attributes_tree_ctrl->GetRootItem() ;
	CurId = attributes_tree_ctrl->GetFirstChild( RootId, Cookie );
	while( CurId.IsOk() )
	{
		wxString Att = attributes_tree_ctrl->GetItemText( CurId );
		CurId2 = attributes_tree_ctrl->GetFirstChild( CurId, Cookie2 );
		while( CurId2.IsOk() )
		{
			wxString Value = attributes_tree_ctrl->GetItemText( CurId2 );
			Entry.AddValue( Att, Value );
			CurId2 = attributes_tree_ctrl->GetNextChild( CurId, Cookie2 );
		}
		CurId = attributes_tree_ctrl->GetNextChild( RootId, Cookie );
	}
	
	int Err = m_Ldap->AddEntry( Entry );
	if( Err != LDAP_SUCCESS )
	{
		::wxMessageBox( m_Ldap->GetErrorStr(), _("Error") );
		return ;
	}
	m_NewRdn = Rdn ;
	m_NewDn = Dn ;
	EndModal( wxID_OK );
}

void NewEntryDlg::SetManual( bool p_Manual )
{
	objectclass_combo_box->Enable( !p_Manual );
	objectclass_text_ctrl->Enable( p_Manual );
	
	
	mustattributes_list_box->Enable( !p_Manual );
	mayattributes_list_box->Enable( !p_Manual );
	//attribute_text_ctrl->Enable( p_Manual );
	
	
	//button_add->Enable( p_Manual );

}
