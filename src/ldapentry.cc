#ifdef __GNUG__
    #pragma implementation "ldapentry.h"
#endif

#include "wx/wxprec.h"
#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include <ldap.h>
/*#include <ldif.h>*/
#include "ldapentry.h"

LdapAttribute::LdapAttribute()
{
	m_IsBinary = false ;
}
LdapAttribute::~LdapAttribute()
{
	BervalArray::iterator beriter ;
	
	for( beriter = m_BerValues.begin(); beriter != m_BerValues.end(); beriter++ )
	{
		ber_bvfree( (*beriter) );
	}
	m_BerValues.clear();
}
void LdapAttribute::SetName( const wxString& p_Name )
{
	m_Name = p_Name ;
}
const wxString& LdapAttribute::GetName( void )
{
	return m_Name ;
}

void LdapAttribute::AddValue( const wxString& p_Value )
{
	m_Values.Add( p_Value );
}
void LdapAttribute::AddBinaryValue( const berval* p_Value )
{
	
	m_IsBinary = true ;
	berval *newber = ber_bvdup( ( berval* )p_Value );
	memcpy( newber->bv_val, p_Value->bv_val, p_Value->bv_len );
	m_BerValues.insert( m_BerValues.end(), newber );
}

wxString LdapAttribute::GetValue( int p_Index )
{
	wxString Ret ;
	if( m_IsBinary )
	{
		berval* TheBer;
		if( p_Index < m_BerValues.size() )
		{
			TheBer = m_BerValues[ p_Index ];
			for( long i = 0; i < TheBer->bv_len && i < 255; i++ )
			{
				Ret.Append( wxString::Format( wxT( "%x" ), TheBer->bv_val[ i ] ) );
			}
		}
	}
	else
	{
		if( p_Index < m_Values.Count() )
		{
			Ret = m_Values[ p_Index ] ;
		}
	}
	return Ret;
}

berval* LdapAttribute::GetBinaryValue( int p_Index )
{
	berval* TheBer = NULL;
	if( p_Index < m_BerValues.size() )
	{
		TheBer = m_BerValues[ p_Index ];
	}
	return TheBer;
}

bool LdapAttribute::IsBinary( void )
{
	return m_IsBinary ;
}


long LdapAttribute::CountValues( void )
{
	if( m_IsBinary )
		return m_BerValues.size() ;
	else
		return m_Values.Count();
}


LdapEntry::LdapEntry()
{
}

LdapEntry::~LdapEntry()
{
}

void LdapEntry::SetDn( const wxString& p_Dn )
{
	m_Dn = p_Dn ;
}
const wxString& LdapEntry::GetDn( void )
{
	return m_Dn;
}
wxString LdapEntry::GetEndOfDn( void )
{
	char **Values = NULL ;
	wxString Ret ;
	
	Ret = m_Dn.BeforeFirst( wxT( ',' ) );
	return Ret;
}
void LdapEntry::AddValue( const wxString& p_Attribute, const wxString& p_Value )
{
	LdapAttribute* Attribute = NULL ;
	
	if( m_Attributes.find( p_Attribute ) == m_Attributes.end() )
	{
		Attribute = new LdapAttribute() ;
		Attribute->SetName( p_Attribute );
		Attribute->AddValue( p_Value );
		m_Attributes[ p_Attribute ] = Attribute ;
	}
	else
	{
		m_Attributes[ p_Attribute ]->AddValue( p_Value ) ;
	}
}

void LdapEntry::AddBinaryValue( const wxString& p_Attribute, const berval* p_Value )
{
	LdapAttribute* Attribute = NULL ;
	
	if( m_Attributes.find( p_Attribute ) == m_Attributes.end() )
	{
		Attribute = new LdapAttribute() ;
		Attribute->SetName( p_Attribute );
		Attribute->AddBinaryValue( p_Value );
		m_Attributes[ p_Attribute ] = Attribute ;
	}
	else
	{
		m_Attributes[ p_Attribute ]->AddBinaryValue( p_Value ) ;
	}
	
	
}

long LdapEntry::CountAttributes( void )
{
	return m_Attributes.size() ;
}

LdapAttribute* LdapEntry::GetAttribute( long p_Index )
{
	assert( p_Index < m_Attributes.size() );
	long i = 0 ;
	LdapAttribute* Attribute = NULL ;
	LdapAttributes::iterator iter ;
	
	for( iter = m_Attributes.begin(); iter != m_Attributes.end(); iter ++ )
	{
		if( i == p_Index )
		{
			Attribute = iter->second;
			break;
		}
		i++;
	}
	
	
	return Attribute ;
}

LdapAttribute* LdapEntry::GetAttribute( const wxString& p_Name, bool p_IgnoreCase )
{


	LdapAttribute* Attribute = NULL ;
	LdapAttributes::iterator iter ;
	
	for( iter = m_Attributes.begin(); iter != m_Attributes.end(); iter ++ )
	{
		if( p_IgnoreCase && p_Name.CmpNoCase( iter->second->GetName() ) == 0 )
		{
			Attribute = iter->second;
			break;
		}
		else if( !p_IgnoreCase && p_Name.Cmp( iter->second->GetName() ) == 0 )
		{
			Attribute = iter->second;
			break;
		}
		
	}
	
	
	return Attribute ;	
}
void LdapEntry::write_ldif( wxFile& p_File )
{
/*
	char *ldif_buffer;
	ldif_buffer = ldif_put( type, name, value, vallen ) ;
	if( ldif_buffer != NULL )
	{
		ber_memfree( ldif );
	}
*/
}
