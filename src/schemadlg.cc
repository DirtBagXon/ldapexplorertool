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
#include "schemadlg.h"


BEGIN_EVENT_TABLE( SchemaDlg, wxDialog )
EVT_BUTTON(XRCID( "button_search" ), SchemaDlg::OnObjectClassSelected) 
EVT_LISTBOX(XRCID( "attributes_listbox" ), SchemaDlg::OnAttributeSelected) 
END_EVENT_TABLE()
/*
long char_array_size( char ** p_array )
{
	long n = 0;
	assert( p_array != NULL );

	while( p_array[ n ] != NULL )
		n++;
	return n;
}
*/
SchemaDlg::SchemaDlg( wxWindow* p_Parent, SchemaUtil& p_Ldap )
{
	wxXmlResource::Get()->LoadDialog( this, p_Parent, wxT( "schemadlg" ) );
	InitControls();
	m_Ldap = &p_Ldap ;
	FillObjectClasses();
	FillAttributeTypes();
}
void SchemaDlg::FillAttributeTypes( void )
{
	wxArrayString Attributes;
	
	m_Ldap->GetAttributeTypes( Attributes );
	attributes_listbox->Append( Attributes );
}
void SchemaDlg::FillObjectClasses( void )
{
	wxArrayString ObjectClasses;
	m_Ldap->GetObjectClasses( ObjectClasses );
	objectclasses_listbox->Append( ObjectClasses );
}
void SchemaDlg::InitControls()
{
		objectclasses_listbox=(wxListBox*)FindWindowById( XRCID("objectclasses_listbox"),this );
		attributes_listbox=(wxListBox*)FindWindowById( XRCID("attributes_listbox"),this );
		objectclass_tree_ctrl =(wxTreeCtrl*)FindWindowById( XRCID("objectclass_tree_ctrl"),this );
		attributes_tree_ctrl =(wxTreeCtrl*)FindWindowById( XRCID("attributes_tree_ctrl"),this );		
}

void SchemaDlg::OnObjectClassSelected( wxCommandEvent& p_Event )
{
	LDAPObjectClass* theClass = NULL ;

	theClass = m_Ldap->FindObjectClass( objectclasses_listbox->GetStringSelection() );	
	if( theClass != NULL )
	{
		wxTreeItemId	RootId;
		wxTreeItemId	ParentId;
		wxString	Kind;
		objectclass_tree_ctrl->DeleteAllItems();
		RootId = objectclass_tree_ctrl->AddRoot( wxT("Properties") );
		
		ParentId = objectclass_tree_ctrl->AppendItem( RootId, wxT("OID") );
		objectclass_tree_ctrl->AppendItem( ParentId, wxString( theClass->oc_oid, wxConvUTF8 ) );
		
		ParentId = objectclass_tree_ctrl->AppendItem( RootId, wxT("Name") );
		for( long i = 0; theClass->oc_names[i] != NULL; i++ )
		{
			objectclass_tree_ctrl->AppendItem( ParentId, wxString( theClass->oc_names[i], wxConvUTF8 ) );
		}
		
		ParentId = objectclass_tree_ctrl->AppendItem( RootId, wxT("Description") );
		objectclass_tree_ctrl->AppendItem( ParentId, wxString( theClass->oc_desc, wxConvUTF8 ) );
		
		ParentId = objectclass_tree_ctrl->AppendItem( RootId, wxT("Obsolete") );
		objectclass_tree_ctrl->AppendItem( ParentId, (theClass->oc_obsolete==LDAP_SCHEMA_YES?wxT("yes"):wxT("no") ) );
		
		ParentId = objectclass_tree_ctrl->AppendItem( RootId, wxT("Kind") );
		switch( theClass->oc_kind )
		{
			case LDAP_SCHEMA_ABSTRACT:
				Kind = wxT("Abstract");
				break;
			case LDAP_SCHEMA_STRUCTURAL:
				Kind = wxT("Structural");
				break;
			case LDAP_SCHEMA_AUXILIARY:
				Kind = wxT("Auxiliary");
				break;		
		}		
		objectclass_tree_ctrl->AppendItem( ParentId, Kind );

		if( theClass->oc_sup_oids != NULL )
		{
			ParentId = objectclass_tree_ctrl->AppendItem( RootId, wxT("Parents") );
			for( long i = 0; theClass->oc_sup_oids[i] != NULL; i++ )
			{
				LDAPObjectClass* tClass = m_Ldap->FindObjectClassByOID( wxString( theClass->oc_sup_oids[i], wxConvUTF8 ) ) ;
				if( tClass != NULL )
				{
					objectclass_tree_ctrl->AppendItem( ParentId, wxString( tClass->oc_names[0], wxConvUTF8 ) );
				}
				else
				{
					objectclass_tree_ctrl->AppendItem( ParentId, wxString( theClass->oc_sup_oids[i], wxConvUTF8 ) );
				}
			}
		}
		
		objectclass_tree_ctrl->Expand( RootId );
		
		ParentId = objectclass_tree_ctrl->AppendItem( RootId, wxT("Mandatory Attributes") );
		if( theClass->oc_at_oids_must != NULL )
		{
			for( long i = 0; theClass->oc_at_oids_must[i] != NULL; i++ )
			{
				objectclass_tree_ctrl->AppendItem( ParentId, wxString( theClass->oc_at_oids_must[i], wxConvUTF8 ) );
			}
		}
		
		ParentId = objectclass_tree_ctrl->AppendItem( RootId, wxT("Optional Attributes") );
		if( theClass->oc_at_oids_may != NULL )
		{
			for( long i = 0; theClass->oc_at_oids_may[i] != NULL; i++ )
			{
				objectclass_tree_ctrl->AppendItem( ParentId, wxString( theClass->oc_at_oids_may[i], wxConvUTF8 ) );
			}
		}

	}
	else
	{
		::wxMessageBox( wxT( "Definition not found" ) );
	}
}

void SchemaDlg::OnAttributeSelected( wxCommandEvent& p_Event )
{
	LDAPAttributeType* theAtt = NULL ;
	LDAPAttributeType* parent = NULL ;
	wxString Usage;
	
	theAtt = m_Ldap->GetAttributeTypeByName( attributes_listbox->GetStringSelection() );
	if( theAtt == NULL )
		return;

	wxTreeItemId	RootId;
	wxTreeItemId	ParentId;

	attributes_tree_ctrl->DeleteAllItems();
	RootId = attributes_tree_ctrl->AddRoot( wxT("Properties") );
	
	ParentId = attributes_tree_ctrl->AppendItem( RootId, wxT("OID") );
	attributes_tree_ctrl->AppendItem( ParentId, wxString( theAtt->at_oid, wxConvUTF8 ) );

	ParentId = attributes_tree_ctrl->AppendItem( RootId, wxT("Name") );
	for( long i = 0; theAtt->at_names[i] != NULL; i++ )
	{
		attributes_tree_ctrl->AppendItem( ParentId, wxString( theAtt->at_names[i], wxConvUTF8 ) );
	}				
	
	ParentId = attributes_tree_ctrl->AppendItem( RootId, wxT("Description") );
	attributes_tree_ctrl->AppendItem( ParentId, wxString( theAtt->at_desc, wxConvUTF8 ) );
	
	ParentId = attributes_tree_ctrl->AppendItem( RootId, wxT("Obsolete") );
	attributes_tree_ctrl->AppendItem( ParentId, (theAtt->at_obsolete==LDAP_SCHEMA_YES?wxT("yes"):wxT("no") ) );

	
	ParentId = attributes_tree_ctrl->AppendItem( RootId, wxT("Parent Type") );
	parent = m_Ldap->GetAttributeTypeByOID( wxString( theAtt->at_sup_oid, wxConvUTF8 ) ) ;
	if( parent != NULL )
	{
		attributes_tree_ctrl->AppendItem( ParentId, wxString( parent->at_names[0], wxConvUTF8 ) );
	}
	else
	{
		attributes_tree_ctrl->AppendItem( ParentId, wxString( theAtt->at_sup_oid, wxConvUTF8 ) );
	}

	ParentId = attributes_tree_ctrl->AppendItem( RootId, wxT("Equality") );
	if( theAtt->at_equality_oid != NULL )
	{
		attributes_tree_ctrl->AppendItem( ParentId, wxString(theAtt->at_equality_oid,wxConvUTF8) );
	}
	
	ParentId = attributes_tree_ctrl->AppendItem( RootId, wxT("Ordering") );
	if( theAtt->at_ordering_oid != NULL )
	{
		attributes_tree_ctrl->AppendItem( ParentId, wxString(theAtt->at_ordering_oid,wxConvUTF8) );
	}
	
	ParentId = attributes_tree_ctrl->AppendItem( RootId, wxT("Substring matching rule") );
	if( theAtt->at_substr_oid != NULL )
	{
		attributes_tree_ctrl->AppendItem( ParentId, wxString(theAtt->at_substr_oid,wxConvUTF8) );
	}
	
	ParentId = attributes_tree_ctrl->AppendItem( RootId, wxT("Syntax") );
	if( theAtt->at_syntax_oid != NULL )
	{
		attributes_tree_ctrl->AppendItem( ParentId, wxString(theAtt->at_syntax_oid,wxConvUTF8) );
	}
	
	ParentId = attributes_tree_ctrl->AppendItem( RootId, wxT("Syntax length") );
	attributes_tree_ctrl->AppendItem( ParentId, wxString::Format( wxT("%d"),theAtt->at_syntax_len) );
	
	ParentId = attributes_tree_ctrl->AppendItem( RootId, wxT("Single valued") );
	attributes_tree_ctrl->AppendItem( ParentId, ( theAtt->at_single_value==LDAP_SCHEMA_YES?wxT("yes"):wxT("no")));
	
	ParentId = attributes_tree_ctrl->AppendItem( RootId, wxT("Collective") );
	attributes_tree_ctrl->AppendItem( ParentId, ( theAtt->at_collective==LDAP_SCHEMA_YES?wxT("yes"):wxT("no")));
	
	ParentId = attributes_tree_ctrl->AppendItem( RootId, wxT("No user modification") );
	attributes_tree_ctrl->AppendItem( ParentId, ( theAtt->at_no_user_mod==LDAP_SCHEMA_YES?wxT("yes"):wxT("no")));

	switch( theAtt->at_usage )
	{
		case LDAP_SCHEMA_USER_APPLICATIONS:
			Usage= wxT( "LDAP_SCHEMA_USER_APPLICATIONS" );
			break;

		case LDAP_SCHEMA_DIRECTORY_OPERATION:
			Usage= wxT( "LDAP_SCHEMA_DIRECTORY_OPERATION" );
			break;
		case LDAP_SCHEMA_DISTRIBUTED_OPERATION:
			Usage= wxT( "LDAP_SCHEMA_DISTRIBUTED_OPERATION" );
			break;

		case LDAP_SCHEMA_DSA_OPERATION:
			Usage= wxT( "LDAP_SCHEMA_DSA_OPERATION" );
			break;
  	}
	ParentId = attributes_tree_ctrl->AppendItem( RootId, wxT("Usage") );
	attributes_tree_ctrl->AppendItem( ParentId, Usage );
  	
}
