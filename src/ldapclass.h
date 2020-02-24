#ifndef __LET_LDAPCLASS__
#define __LET_LDAPCLASS__

#include <ldap.h>
#include <wx/string.h>
#include <wx/hashmap.h>
#include "ldapentry.h"

#define LF_OPERATIONAL 1
#define LF_ALL	2
#define LF_ALL_OPERATIONAL	3
#define LF_NONE	4

WX_DECLARE_HASH_MAP( int, LDAPMessage*, wxIntegerHash, wxIntegerEqual, Search_Results_Map );

class LdapClass
{
	public:
		LdapClass();
		virtual ~LdapClass();
		
		wxString GetErrorStr( void );
		int Open( const wxString& p_Server, u_short p_Port = LDAP_PORT );
		int Init( const wxString& p_Server, u_short p_Port = LDAP_PORT );
		virtual void Close( void );
		
		int SetCACert( const wxString& p_CACertFile );
		int SetClientCert( const wxString& p_CertFile );
		int SetClientKey( const wxString& p_KeyFile );
		int StartTLS( void );
		
		int	SimpleBind( const wxString& p_User, const wxString& p_Password );
		int	SimpleBind( void );
		int SetVersion( u_short p_Version );
		int GetVersion( u_short& p_Version );
		int SetOption( int p_Option, void* p_Value );
		int GetOption( int p_Option, void* p_Value );
		int Referals( bool p_Follow );
		int SetSizeLimit( int p_Size );
		int GetSizeLimit( int& p_Size );
		int SetTimeLimit( int p_Time );
		int GetTimeLimit( int& p_Time );
		int Search( int& p_RequestId, const wxString& p_Base, int p_Scope, const wxString& p_Filter,
			wxArrayString* p_Attributes = NULL, bool p_GetValues = true, u_short p_AttFlags = 0, 
			long p_Timeout = 0, int p_SizeLimit = 0 );
		bool GetEntry( int p_RequestId, int p_Index, LdapEntry& p_Entry );
		int GetEntryCount( int p_RequestId );
		int GetResultCount( int p_RequestId );
		void CloseSearch( int p_RequestId );
		
		int	AddEntry( LdapEntry& p_Entry );
		int	ReplaceEntry( LdapEntry& p_Entry );
		int	DeleteEntry( LdapEntry& p_Entry );
		int	Rename( const wxString& p_OldDn, const wxString& p_NewRDn );
		
		int	AddAttribute( LdapEntry& p_Entry );
		int	DeleteAttribute( const wxString& p_Dn, const wxString& p_AttributeName );
		int AddAttributeValue( const wxString& p_Dn, const wxString& p_AttributeName, const wxString& p_Value );
		int AddAttributeValue( const wxString& p_Dn, const wxString& p_AttributeName, const berval& p_Value );
		int DeleteAttributeValue( const wxString& p_Dn, const wxString& p_AttributeName, const wxString& p_Value );
		int DeleteAttributeValue( const wxString& p_Dn, const wxString& p_AttributeName, const berval& p_Value );
		
		int CopyTo( int p_Id, const wxString& p_ToDn, const wxString& p_OldDn );

		wxArrayString Explode_Dn( const wxString& p_Dn );
		
		virtual bool IsBinaryAttribute( const wxString& p_Name )=0;
		
		protected:
		
		LDAPMod** Attributes2Mods( LdapEntry& p_Entry, int p_Operation );
		void FillEntry( LDAPMessage* p_Msg, LdapEntry& p_Entry );
		char** Build_Attribute_Array( const char*p_Array1, wxArrayString* p_Array2, char* p_Str );
		void Free_Attribute_Array( char** p_Array );
		int CopyEntry( LDAPMessage* p_Entry, const wxString& p_ToDn, const wxString& p_OldDn );
		
		int		m_Err ;
	private:
		LDAP*	m_LDAP ;
		
		Search_Results_Map	m_ResultsMap ;
};
#endif
