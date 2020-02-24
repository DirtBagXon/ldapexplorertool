#ifndef __LET_LDAP_ENTRY__
#define __LET_LDAP_ENTRY__

#include <ldap.h>
#include <wx/string.h>
#include <wx/file.h>
#include <vector>

typedef std::vector< berval* > BervalArray;
class LdapAttribute
{
	public:
		LdapAttribute();
		virtual ~LdapAttribute();
		void SetName( const wxString& p_Name );

		const wxString& GetName( void );
		void AddValue( const wxString& p_Value );
		void AddBinaryValue( const berval* p_Value );
		wxString GetValue( int p_Index );
		berval* GetBinaryValue( int p_Index );
		long CountValues( void );
		bool IsBinary( void );
	private:
		bool					m_IsBinary ;
		int					m_Operation ;
		wxString			m_Name ;
		wxArrayString	m_Values ;
		BervalArray		m_BerValues;
};

WX_DECLARE_HASH_MAP( wxString, LdapAttribute *, wxStringHash, wxStringEqual, LdapAttributes );

class LdapEntry
{
	public:
		LdapEntry();
		virtual ~LdapEntry();
		void SetDn( const wxString& p_Dn );
		const wxString& GetDn( void );
		wxString GetEndOfDn( void );
		void AddValue( const wxString& p_Attribute, const wxString& p_Value );
		void AddBinaryValue( const wxString& p_Attribute, const berval* p_Value );
		long CountAttributes( void );
		LdapAttribute* GetAttribute( long p_Index );
		LdapAttribute* GetAttribute( const wxString& p_Name, bool p_IgnoreCase );
		void write_ldif( wxFile& p_File );
		
	private:
		int 			m_Operation ;
		wxString			m_Dn ;
		LdapAttributes	m_Attributes;
};

#endif
