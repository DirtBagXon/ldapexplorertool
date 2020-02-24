#ifdef __GNUG__
    #pragma implementation "ldapclass.h"
#endif
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include <wx/config.h>
#include <wx/fileconf.h>
#include <wx/wfstream.h>

#include "ldapentry.h"
#include "ldapclass.h"
#include "schema.h"

wxChar* DEFAULT_SUBSCHEMASUBENTRY=wxT( "cn=subschema" );


void SchemaUtil::SetConfig( const wxString& p_Name )
{
	m_ConfigName = p_Name ;
	m_SearchId = -1 ;
	m_HasSchema = false ;
	m_GuessBinary = false ;
}
SchemaUtil::SchemaUtil( void ) : LdapClass()
{
	m_SearchId = -1 ;
}
SchemaUtil::~SchemaUtil()
{
	Close();
}

bool SchemaUtil::HasSchema( void )
{
	return m_HasSchema;
}

void SchemaUtil::Close( void )
{
	LdapClass::Close();
	std::vector< LDAPObjectClass* >::iterator iter1 ;
	for( iter1 = m_ObjectClasses.begin(); iter1 != m_ObjectClasses.end(); iter1++ )
	{
		ldap_objectclass_free( (*iter1) );
	}
	m_ObjectClasses.clear();

	std::vector< LDAPAttributeType* >::iterator iter2 ;
	for( iter2 = m_AttributeTypes.begin(); iter2 != m_AttributeTypes.end(); iter2++ )
	{
		ldap_attributetype_free( (*iter2) );
	}
	m_AttributeTypes.clear();
	
	std::vector< LDAPSyntax* >::iterator iter3 ;
	for( iter3 = m_Syntaxes.begin(); iter3 != m_Syntaxes.end(); iter3++ )
	{
		ldap_syntax_free( (*iter3) );
	}
	m_Syntaxes.clear();
}

int SchemaUtil::FindSchemaEntry( const wxString& p_BaseDN )
{
	int Ret ;
	wxString	SchemaEntry ;	
	int	Id ;	
	
	/*
	*	Is a subschema subentry configured
	*/
	SchemaEntry = wxConfig::Get()->Read( wxString::Format( wxT( "/configurations/%s/subschemasubentry" ), 
	m_ConfigName.c_str() ) );

	if( SchemaEntry.Len() == 0 )
	{	
		/*
		*	Try to find the subschemasubentry attribute in the baseDN
		*/
		Ret = Search( Id, p_BaseDN, LDAP_SCOPE_BASE, wxT( "objectclass=*" ), 
		NULL, true, LF_ALL_OPERATIONAL );
		if( Ret == LDAP_SUCCESS )
		{

			int NbEntries = GetEntryCount( Id ) ;
			if( NbEntries > 0 )
			{
				LdapEntry Entry ;
				if( !GetEntry( Id, 0, Entry ) )
				{
					::wxLogError( wxT( "FindSchemaEntry: %d - %s" ),m_Err, GetErrorStr().c_str() );
				}
				else
				{
					LdapAttribute *Attribute = Entry.GetAttribute( wxT( "subschemasubentry" ), true );
					if( Attribute != NULL )
					{
						SchemaEntry = Attribute->GetValue( 0 ) ;
					}
				}
			}
			CloseSearch( Id );
		}
	}
	if( SchemaEntry.Len() == 0 )
	{	
		/*
		*	Try to find the subschemasubentry attribute in the rootDN
		*/
		Ret = Search( Id, wxT( "" ), LDAP_SCOPE_BASE, wxT( "objectclass=*" ), 
		NULL, true, LF_ALL_OPERATIONAL );
		if( Ret != LDAP_SUCCESS )
		{
			SchemaEntry = DEFAULT_SUBSCHEMASUBENTRY ;
		}
		else
		{
			int NbEntries = GetEntryCount( Id ) ;
			if( NbEntries > 0 )
			{
				LdapEntry Entry ;
				if( !GetEntry( Id, 0, Entry ) )
				{
					::wxLogError( wxT( "FindSchemaEntry: %d - %s" ),m_Err, GetErrorStr().c_str() );
				}
				LdapAttribute *Attribute = Entry.GetAttribute( wxT( "subschemasubentry" ), true );
				if( Attribute != NULL )
				{
					SchemaEntry = Attribute->GetValue( 0 ) ;
				}
				else
				{
					SchemaEntry = DEFAULT_SUBSCHEMASUBENTRY ;
				}
			}
			else
			{
				/*
				* Try the default value for subschemasubentry
				*/
				SchemaEntry = DEFAULT_SUBSCHEMASUBENTRY ;
			}
			CloseSearch( Id );
		}
	}
	/*
	*	get the schema entry
	*/
	Ret = Search( m_SearchId, SchemaEntry, LDAP_SCOPE_BASE, wxT( "objectclass=*" ), 
	NULL, true, LF_ALL_OPERATIONAL );
	if( Ret != LDAP_SUCCESS )
	{
		::wxLogError( wxT( "FindSchemaEntry: %d - %s" ),m_Err, GetErrorStr().c_str() );
		return Ret ;
	}
	return LDAP_SUCCESS ;
}
int SchemaUtil::GetSchema( const wxString& p_BaseDN )
{
	
	
	m_Err = FindSchemaEntry( p_BaseDN );
	if( m_Err != LDAP_SUCCESS )
	{
		m_HasSchema = false ;
		return m_Err;
	}
	

	int NbEntries = GetEntryCount( m_SearchId ) ;
	if( NbEntries <= 0 )
	{
		m_HasSchema = false ;
		return LDAP_SUCCESS ;
	}
	
	LdapEntry Entry ;
	
	GetEntry( m_SearchId, 0, Entry );
	LdapAttribute *ObjectClasses = Entry.GetAttribute( wxT( "objectClasses" ), true );
	LdapAttribute *AttributeTypes = Entry.GetAttribute( wxT( "attributeTypes" ), true );
	LdapAttribute *Syntaxes = Entry.GetAttribute( wxT( "ldapSyntaxes" ), true );
	
	long nValues ;
	long i;
	wxString Value ;
	int	Err;
	const char *Errp ;
	if( ObjectClasses != NULL )
	{
		nValues = ObjectClasses->CountValues() ;
		for( i = 0; i < nValues ; i++ )
		{
			Value = ObjectClasses->GetValue( i ) ;
			LDAPObjectClass *Oc = ldap_str2objectclass( Value.mb_str(wxConvUTF8), &Err, &Errp, LDAP_SCHEMA_ALLOW_ALL );
			if( Oc != NULL )
			{
				m_ObjectClasses.insert( m_ObjectClasses.end(), Oc );
			}
		}
	}

	if( AttributeTypes != NULL )
	{
		nValues = AttributeTypes->CountValues() ;
		for( i = 0; i < nValues ; i++ )
		{
			Value = AttributeTypes->GetValue( i ) ;
			LDAPAttributeType *At = ldap_str2attributetype( Value.mb_str(wxConvUTF8), &Err, &Errp, LDAP_SCHEMA_ALLOW_ALL );
			if( At != NULL )
			{
				m_AttributeTypes.insert( m_AttributeTypes.end(), At );
			}
		}
	}
	if( Syntaxes != NULL )
	{
		nValues = Syntaxes->CountValues() ;
		for( i = 0; i < nValues ; i++ )
		{
			Value = Syntaxes->GetValue( i ) ;
			LDAPSyntax *Syn = ldap_str2syntax( Value.mb_str(wxConvUTF8), &Err, &Errp, LDAP_SCHEMA_ALLOW_ALL );
			if( Syn != NULL )
			{
				m_Syntaxes.insert( m_Syntaxes.end(), Syn );
				/*::wxMessageBox( wxString( Syn->syn_names[0], wxConvUTF8 ) );*/
			}
		}
	}
	m_HasSchema = true ;
	return LDAP_SUCCESS ;
}

int SchemaUtil::GetObjectClasses( wxArrayString& p_Array )
{
	std::vector< LDAPObjectClass* >::iterator iter ;
	
	p_Array.Clear();
	for( iter = m_ObjectClasses.begin(); iter != m_ObjectClasses.end(); iter++ )
	{
		LDAPObjectClass* Class = (*iter);
		if( Class->oc_names == NULL )
			continue;
			
		for( long i = 0; Class->oc_names[ i ] != NULL; i++ )
		{
			p_Array.Add( wxString( Class->oc_names[ i ], wxConvUTF8 ) );
		}
	}
	p_Array.Sort();
	return p_Array.Count() ;
}
int SchemaUtil::GetAttributeTypes( wxArrayString& p_Array )
{
	std::vector< LDAPAttributeType* >::iterator iter ;
	
	p_Array.Clear();
	for( iter = m_AttributeTypes.begin(); iter != m_AttributeTypes.end(); iter++ )
	{
		LDAPAttributeType* Type = (*iter);
		if( Type->at_names == NULL )
			continue;
			
		for( long i = 0; Type->at_names[ i ] != NULL; i++ )
		{
			p_Array.Add( wxString( Type->at_names[ i ], wxConvUTF8 ) );
		}
	}
	p_Array.Sort();
	return p_Array.Count() ;
}

int SchemaUtil::GetSyntaxes( wxArrayString& p_Array )
{
	std::vector< LDAPSyntax* >::iterator iter ;
	
	p_Array.Clear();
	for( iter = m_Syntaxes.begin(); iter != m_Syntaxes.end(); iter++ )
	{
		LDAPSyntax* Syn = (*iter);
		if( /*Syn->syn_names == NULL && */Syn->syn_desc == NULL )
			continue;
			
		p_Array.Add( wxString( Syn->syn_desc, wxConvUTF8 ) );	
		/*
		for( long i = 0; Syn->syn_names[ i ] != NULL; i++ )
		{
			p_Array.Add( wxString( Syn->syn_names[ i ], wxConvUTF8 ) );
		}
		*/
	}
	p_Array.Sort();
	return p_Array.Count() ;
}

LDAPObjectClass* SchemaUtil::FindObjectClass( const wxString& p_Name )
{
	LDAPObjectClass* Ret = NULL ;
	std::vector< LDAPObjectClass* >::iterator iter ;
	
	for( iter = m_ObjectClasses.begin(); iter != m_ObjectClasses.end(); iter++ )
	{
		LDAPObjectClass* Class = (*iter);
		if( Class->oc_names == NULL )
			continue;
			
		for( long i = 0; Class->oc_names[ i ] != NULL; i++ )
		{
			if( p_Name.Cmp( wxString( Class->oc_names[ i ], wxConvUTF8 ) ) == 0 )
			{
				Ret = Class ;
				return Ret;
			}
		}
	}
	return Ret ;
}

LDAPObjectClass* SchemaUtil::FindObjectClassByOID( const wxString& p_Oid )
{
	LDAPObjectClass* Ret = NULL ;
	std::vector< LDAPObjectClass* >::iterator iter ;
	
	for( iter = m_ObjectClasses.begin(); iter != m_ObjectClasses.end(); iter++ )
	{
		LDAPObjectClass* Class = (*iter);
		if( Class->oc_oid == NULL )
			continue;
			
		if( p_Oid.Cmp( wxString( Class->oc_oid, wxConvUTF8 ) ) == 0 )
		{
			Ret = Class ;
			return Ret;
		}
	}
	return Ret ;
}

LDAPAttributeType* SchemaUtil::GetAttributeTypeByOID( const wxString& p_OID )
{
	LDAPAttributeType* Ret = NULL ;
	std::vector< LDAPAttributeType* >::iterator iter ;
	
	
	for( iter = m_AttributeTypes.begin(); iter != m_AttributeTypes.end(); iter++ )
	{
		LDAPAttributeType* Type = (*iter);
		
		if( p_OID.Cmp( wxString( Type->at_oid, wxConvUTF8 ) ) == 0 )
		{
			Ret = Type ;
			break;
		}
	}
	
	return Ret;
}

LDAPSyntax* SchemaUtil::GetSyntaxByOID( const wxString& p_OID )
{
	LDAPSyntax* Ret = NULL ;
	std::vector< LDAPSyntax* >::iterator iter ;
	
	
	for( iter = m_Syntaxes.begin(); iter != m_Syntaxes.end(); iter++ )
	{
		LDAPSyntax* Syn = (*iter);
		
		if( p_OID.Cmp( wxString( Syn->syn_oid, wxConvUTF8 ) ) == 0 )
		{
			Ret = Syn ;
			break;
		}
	}
	
	return Ret;
}

LDAPSyntax* SchemaUtil::GetSyntaxByName( const wxString& p_Name )
{
	LDAPSyntax* Ret = NULL ;
	std::vector< LDAPSyntax* >::iterator iter ;
	
	
	for( iter = m_Syntaxes.begin(); iter != m_Syntaxes.end(); iter++ )
	{
		LDAPSyntax* Syn = (*iter);
		
		if( Syn->syn_names != NULL && p_Name.Cmp( wxString( Syn->syn_names[0], wxConvUTF8 ) ) == 0 )
		{
			Ret = Syn ;
			break;
		}
		if( Syn->syn_desc != NULL && p_Name.Cmp( wxString( Syn->syn_desc, wxConvUTF8 ) ) == 0 )
		{
			Ret = Syn ;
			break;
		}
	}
	
	return Ret;
}

LDAPAttributeType* SchemaUtil::GetAttributeTypeByName( const wxString& p_OID )
{
	LDAPAttributeType* Ret = NULL ;
	std::vector< LDAPAttributeType* >::iterator iter ;
	
	
	for( iter = m_AttributeTypes.begin(); iter != m_AttributeTypes.end(); iter++ )
	{
		LDAPAttributeType* Type = (*iter);
		if( Type->at_names == NULL )
			continue;
		
		for( long i = 0; Type->at_names[ i ] != NULL; i++ )
		{
			if( p_OID.Cmp( wxString( Type->at_names[ i ], wxConvUTF8 ) ) == 0 )
			{
				Ret = Type ;
				return Ret;
			}
		}
	}
	
	return Ret;
}

int SchemaUtil::GetMUSTAttributeTypes( const wxString& p_ObjectClass, wxArrayString& p_Array )
{
	LDAPObjectClass* Class = FindObjectClass( p_ObjectClass );
	if( Class != NULL )
	{
		if( Class->oc_at_oids_must != NULL )
		{
			for( long i = 0; Class->oc_at_oids_must[ i ] != NULL; i++ )
			{
				p_Array.Add( wxString( Class->oc_at_oids_must[ i ], wxConvUTF8 ) );
			}
		}
		if( Class->oc_sup_oids != NULL )
		{
			for( long i = 0; Class->oc_sup_oids[ i ] != NULL; i++)
			{
				GetMUSTAttributeTypes( wxString( Class->oc_sup_oids[ i ], wxConvUTF8 ), p_Array );
			}
		}
	}	
	return p_Array.Count() ;
}

int SchemaUtil::GetMAYAttributeTypes( const wxString& p_ObjectClass, wxArrayString& p_Array )
{
	LDAPObjectClass* Class = FindObjectClass( p_ObjectClass );
	if( Class != NULL )
	{
		if( Class->oc_at_oids_may != NULL )
		{
			for( long i = 0; Class->oc_at_oids_may[ i ] != NULL; i++ )
			{
				p_Array.Add( wxString( Class->oc_at_oids_may[ i ], wxConvUTF8 ) );
			}
		}
		if( Class->oc_sup_oids != NULL )
		{
			for( long i = 0; Class->oc_sup_oids[ i ] != NULL; i++)
			{
				GetMAYAttributeTypes( wxString( Class->oc_sup_oids[ i ], wxConvUTF8 ), p_Array );
			}
		}
	}	
	return p_Array.Count() ;
}

bool SchemaUtil::IsBinaryAttribute( const wxString& p_Name )
{
	LDAPAttributeType* Type = NULL ;
	LDAPSyntax *Syntax = NULL ;
	
	if( !m_GuessBinary )
		return false ;
	
	if( p_Name.Contains( wxT( ";binary" ) ) )
	{
		::wxMessageBox( wxT( "Name Contains ;binary" ) );
	}
	Type = GetAttributeTypeByName( p_Name );
	if( Type != NULL )
	{
		if( m_CheckAttDescription == 1 )
		{
			/*
			*	Search for the word binary in the description
			*/
			wxString Desc = wxString( Type->at_desc, wxConvUTF8 ) ;
			Desc.MakeLower();
			if( Desc.Contains( wxT( "binary" ) ) )
			{
				::wxMessageBox( wxT( "Desc Contains binary" ) );
				return true ;

			}
		}
		
		if( m_CheckAttExtension == 1 )
		{
			/*
			*	Search for the word binary in the attribute's extensions
			*/
			for( long i = 0; Type->at_extensions != NULL && Type->at_extensions[ i ] != NULL ; i++ )
			{
				wxString Extension = wxString( Type->at_extensions[ i ]->lsei_name, wxConvUTF8 ) ;
				if( Extension.CmpNoCase( wxT( "binary" ) ) == 0 )
				{
					::wxMessageBox( wxT( "Att Ext Contains binary" ) );
					return true;
				}
			}
		}
		if( Type->at_syntax_oid != NULL )
		{
			Syntax = GetSyntaxByOID( wxString( Type->at_syntax_oid, wxConvUTF8 ) );
			if( Syntax != NULL )
			{
				wxString Desc = wxString( Syntax->syn_desc, wxConvUTF8 ) ;
				Desc.MakeLower();
				for( long i= 0; i < m_BinarySyntaxes.Count(); i++ )
				{
					if( Desc.Contains( m_BinarySyntaxes[i] ) )
					{
						::wxMessageBox( wxString::Format( wxT( "Syntax Contains %s" ), m_BinarySyntaxes[i].c_str() ) );
						return true;
					}
				}
				
				for( long i = 0; Syntax->syn_extensions != NULL && Syntax->syn_extensions[ i ] != NULL; i++ )
				{
					wxString Name,Value ;
					Name = wxString( Syntax->syn_extensions[ i ]->lsei_name, wxConvUTF8 ) ;
					
					StringMap::iterator iter ;
					for( iter = m_BinaryExtensions.begin(); iter != m_BinaryExtensions.end(); iter ++ )
					{
						if( Name.CmpNoCase( (*iter).first ) == 0 )
						{
							for( long j = 0; Syntax->syn_extensions[ i ]->lsei_values != NULL &&
							Syntax->syn_extensions[ i ]->lsei_values[ j ] != NULL; j++ )
							{
								Value = wxString( Syntax->syn_extensions[ i ]->lsei_values[ j ], wxConvUTF8 ) ;
								if( Value.CmpNoCase( (*iter).second ) )
								{
									::wxMessageBox( wxString::Format( wxT( "Syntax Ext Contains Contains %s" ), (*iter).second.c_str() ) );
									return true;
								}
							}
						}
					}
				}
			}
		}
	}
	
	return false ;
}

void SchemaUtil::GuessBinary( bool p_Guess )
{
	m_GuessBinary = p_Guess ;
	if( m_GuessBinary )
	{
		m_CheckAttDescription = 1;
		m_CheckAttExtension = 1;
	}
}


bool SchemaUtil::ReplaceAttributeType( LDAPAttributeType* p_New )
{
	wxString Oid( p_New->at_oid, wxConvUTF8 ) ;
	
	LdapEntry Entry ;
	LdapEntry NewEntry ;
	
	GetEntry( m_SearchId, 0, Entry );

	LdapAttribute *AttributeTypes = Entry.GetAttribute( wxT( "attributeTypes" ), true );
	
	long nValues ;
	long i;
	wxString Value ;
	int	Err;
	const char *Errp ;

	if( AttributeTypes == NULL )
	{
		return false ;
	}
	
	NewEntry.SetDn( Entry.GetDn() );
	
	nValues = AttributeTypes->CountValues() ;
	for( i = 0; i < nValues ; i++ )
	{
		Value = AttributeTypes->GetValue( i ) ;
		LDAPAttributeType *At = ldap_str2attributetype( Value.mb_str(wxConvUTF8), &Err, &Errp, LDAP_SCHEMA_ALLOW_ALL );
		if( At != NULL && strcmp( At->at_oid, p_New->at_oid) == 0 )
		{
			char* NewValue = ldap_attributetype2str( p_New );
			NewEntry.AddValue( wxT("attributeTypes"),wxString( NewValue, wxConvUTF8 ) );
			free( NewValue );	
		}
		else
		{
			NewEntry.AddValue( wxT("attributeTypes"),Value );
		}
	}
	
	if( ReplaceEntry( NewEntry ) != LDAP_SUCCESS )
	{
		return false ;
	}
	return true ;
}

bool SchemaUtil::AddAttributeType( LDAPAttributeType* p_New )
{
	LdapEntry Entry ;
	int Err ;


	GetEntry( m_SearchId, 0, Entry );

	
	char* NewValue = ldap_attributetype2str( p_New );

	if( NewValue != NULL )
	{
		::wxMessageBox( wxString( NewValue, wxConvUTF8 ) );
		Err = AddAttributeValue( Entry.GetDn(), wxT("attributeTypes"), wxString( NewValue,wxConvUTF8 ) );
		free( NewValue );
		if( Err != LDAP_SUCCESS )
		{
			return false;
		}
	}
	return true ;
}

