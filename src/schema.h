#ifndef _LETSCHEMA_H
#define _LETSCHEMA_H

#include <vector>
#include <map>
#include <ldap.h>
#include <ldap_schema.h>
#include "ldapclass.h"

WX_DECLARE_STRING_HASH_MAP( wxString, StringMap );


class SchemaUtil: public LdapClass
{
	public:
		SchemaUtil();
		~SchemaUtil();
		bool 				HasSchema( void );
		void 				SetConfig( const wxString& p_Name );
		int 				GetSchema( const wxString& p_BaseDN );
		int 				GetObjectClasses( wxArrayString& p_Array );
		int					GetAttributeTypes( wxArrayString& p_Array );
		int 				GetSyntaxes( wxArrayString& p_Array );
		LDAPObjectClass* 	FindObjectClass( const wxString& p_Name );
		LDAPObjectClass* 	FindObjectClassByOID( const wxString& p_Oid );

		int 				GetMUSTAttributeTypes( const wxString& p_ObjectClass, wxArrayString& p_Array );
		int 				GetMAYAttributeTypes( const wxString& p_ObjectClass, wxArrayString& p_Array );
		LDAPAttributeType* 	GetAttributeTypeByOID( const wxString& p_OID );
		LDAPAttributeType* 	GetAttributeTypeByName( const wxString& p_OID );
		LDAPSyntax* 		GetSyntaxByOID( const wxString& p_OID );
		LDAPSyntax* 		GetSyntaxByName( const wxString& p_Name );
		bool 				IsBinaryAttribute( const wxString& p_Name );
		void				GuessBinary( bool p_Guess );
		bool				ReplaceAttributeType( LDAPAttributeType* p_New );
		bool 				AddAttributeType( LDAPAttributeType* p_New );
	private:
		int FindSchemaEntry( const wxString& p_BaseDN );
		void Close( void );

		int			m_SearchId ;
		wxString	m_ConfigName;
		bool		m_HasSchema ;
		bool		m_GuessBinary ;
		std::vector< LDAPAttributeType* >	m_AttributeTypes ;
		std::vector< LDAPObjectClass* >		m_ObjectClasses ;
		std::vector< LDAPSyntax* >			m_Syntaxes ;
		wxArrayString						m_BinarySyntaxes ;
		StringMap							m_BinaryExtensions;
		long								m_CheckAttDescription;
		long								m_CheckAttExtension;
};

#endif
