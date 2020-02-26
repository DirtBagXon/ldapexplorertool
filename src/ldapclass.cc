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


#include "ldapentry.h"
#include "ldapclass.h"

void char_array_free( char ** vals )
{
	long i = 0;
	while( vals[i] != NULL )
	{
		free( vals[i] );
	}
	free( vals );
}
LdapClass::LdapClass()
{
	m_LDAP = NULL ;
	m_Err = LDAP_SUCCESS ;
}

LdapClass::~LdapClass()
{
}

wxString LdapClass::GetErrorStr( void )
{
	
	wxString Ret ;
	Ret = wxString( ldap_err2string( m_Err ), wxConvUTF8 );
	
	return Ret ;
}

int LdapClass::Open( const wxString &p_Server, u_short p_Port )
{
    m_Err = LDAP_SUCCESS ;
	wxString Url ;

	Url = wxT("ldap://");
	Url += p_Server ;
	Url += wxString::Format( wxT(":%u"), p_Port );
    
    if( m_LDAP != NULL )
    {
	 	Close();
	}
    /*m_LDAP = ldap_open( p_Server.mb_str(wxConvUTF8), p_Port );
    if( m_LDAP == NULL )
    {
	 	m_Err = LDAP_CONNECT_ERROR ;
	}*/
    m_Err = ldap_initialize( &m_LDAP, Url.mb_str(wxConvUTF8) );

    return m_Err ;
}

int LdapClass::Init( const wxString &p_Server, u_short p_Port )
{
    m_Err = LDAP_SUCCESS ;
	wxString Url ;

	Url = wxT("ldap://");
	Url += p_Server ;
	Url += wxString::Format( wxT(":%u"), p_Port );
    
    if( m_LDAP != NULL )
    {
	 	Close();
	}
    /*m_LDAP = ldap_init( p_Server.mb_str(wxConvUTF8), p_Port );
    if( m_LDAP == NULL )
    {
	 	m_Err = LDAP_CONNECT_ERROR ;
	}*/

    m_Err = ldap_initialize( &m_LDAP, Url.mb_str(wxConvUTF8) );
    return m_Err ;
}

void LdapClass::Close( void )
{
	
	m_Err = LDAP_SUCCESS ;
	if( m_LDAP == NULL )
	 return;
	
	//ldap_unbind( m_LDAP );
	ldap_unbind_ext( m_LDAP, NULL, NULL );
	m_LDAP = NULL ;
}

int LdapClass::SetCACert( const wxString& p_CACertFile )
{
	m_Err = ldap_set_option( NULL, LDAP_OPT_X_TLS_CACERTFILE, p_CACertFile.mb_str(wxConvUTF8) );
	return m_Err ;
}

int LdapClass::SetClientCert( const wxString& p_CertFile )
{
	m_Err = ldap_set_option( NULL, LDAP_OPT_X_TLS_CERTFILE, p_CertFile.mb_str(wxConvUTF8) );
	return m_Err ;
}

int LdapClass::SetClientKey( const wxString& p_KeyFile )
{
	m_Err = ldap_set_option( NULL, LDAP_OPT_X_TLS_KEYFILE, p_KeyFile.mb_str(wxConvUTF8) );
	return m_Err ;
}

int LdapClass::StartTLS( void )
{
	assert( m_LDAP != NULL );

	m_Err = ldap_start_tls_s( m_LDAP, NULL, NULL );

	return m_Err ;
}
int	LdapClass::SimpleBind( const wxString &p_User, const wxString &p_Password )
{
 	assert( m_LDAP != NULL );

	m_Err = LDAP_SUCCESS;
	m_Err = ldap_simple_bind_s( m_LDAP, p_User.mb_str(wxConvUTF8), p_Password.mb_str(wxConvUTF8) );

	return m_Err ;
}

int	LdapClass::SimpleBind( void )
{	
	assert( m_LDAP != NULL );
	
	m_Err = LDAP_SUCCESS ;
	
	m_Err = ldap_simple_bind_s( m_LDAP, NULL, NULL );

	return m_Err ;
}
int LdapClass::SetVersion( u_short p_Version )
{
	assert( m_LDAP != NULL );
	int	Version = LDAP_VERSION3;

	if( p_Version == 2 )
	{
		Version = LDAP_VERSION2 ;
	}
	
	m_Err = SetOption( LDAP_OPT_PROTOCOL_VERSION, &Version );
	return m_Err ;
}
int LdapClass::GetVersion( u_short& p_Version )
{
	assert( m_LDAP != NULL );
	int	Version ;
	if( p_Version == 2 )
	{
		Version = LDAP_VERSION2 ;
	}
	
	m_Err = GetOption( LDAP_OPT_PROTOCOL_VERSION, &Version );
	if( m_Err == LDAP_SUCCESS && Version == LDAP_VERSION3 )
	{
		p_Version = 3 ;
	}
	else if( m_Err == LDAP_SUCCESS )
	{
		p_Version = 2 ;
	}
	return m_Err ;
}

int LdapClass::SetOption( int p_Option, void *p_Value )
{
	assert( m_LDAP != NULL );
	
	m_Err = ldap_set_option( m_LDAP, p_Option, p_Value );
	
	return m_Err ;
}
int LdapClass::GetOption( int p_Option, void *p_Value )
{
	assert( m_LDAP != NULL && p_Value != NULL );
	
	m_Err = ldap_get_option( m_LDAP, p_Option, p_Value );
	
	return m_Err ;
}
int LdapClass::Referals( bool p_Follow )
{
	assert( m_LDAP != NULL );

	if( p_Follow )
	{
		m_Err = SetOption( LDAP_OPT_REFERRALS, LDAP_OPT_ON );
	}
	else
	{
		m_Err = SetOption( LDAP_OPT_REFERRALS, LDAP_OPT_OFF );
	}
	return m_Err ;
}
int LdapClass::SetSizeLimit( int p_Size )
{
	assert( m_LDAP != NULL );
	
	m_Err = SetOption( LDAP_OPT_SIZELIMIT, &p_Size );
	return m_Err ;
}

int LdapClass::GetSizeLimit( int &p_Size )
{
	assert( m_LDAP != NULL && &p_Size != NULL );

	m_Err = GetOption( LDAP_OPT_SIZELIMIT, &p_Size );
	return m_Err ;
}
int LdapClass::SetTimeLimit( int p_Time )
{
	assert( m_LDAP != NULL );
	m_Err = SetOption( LDAP_OPT_TIMELIMIT, &p_Time );
	return m_Err ;
}
int LdapClass::GetTimeLimit( int &p_Time )
{
	assert( m_LDAP != NULL && &p_Time != NULL );
	m_Err = GetOption( LDAP_OPT_TIMELIMIT, &p_Time );
	return m_Err ;
}

int LdapClass::GetEntryCount( int p_RequestId )
{	
	assert( m_ResultsMap.find( p_RequestId ) != m_ResultsMap.end() ) ;
	assert( m_LDAP != NULL );
	int Ret = ldap_count_entries( m_LDAP, m_ResultsMap[ p_RequestId ] );
	return Ret;
}
int LdapClass::GetResultCount( int p_RequestId )
{
	
	assert( m_LDAP != NULL );
	int Ret ;
	m_Err = LDAP_SUCCESS ;
	u_short Version ;
	
	if( m_ResultsMap.find( p_RequestId ) == m_ResultsMap.end() )
	{
		return -1;
	}
	GetVersion( Version );
	if( Version == 2 )
		Ret = ldap_count_entries( m_LDAP, m_ResultsMap[ p_RequestId ] );
	else
		Ret = ldap_count_messages( m_LDAP, m_ResultsMap[ p_RequestId ] );
	
	return Ret ;
}

bool LdapClass::GetEntry( int p_RequestId, int p_Index, LdapEntry &p_Entry )
{
	assert( m_ResultsMap.find( p_RequestId ) != m_ResultsMap.end() );
	assert( m_LDAP != NULL );
	
	bool Ret = false ;
	LDAPMessage	*Current ;
	
	LDAPMessage *Res = NULL ;
	
	Res = m_ResultsMap[ p_RequestId ] ;

	u_short Version ;
	
	GetVersion( Version );
	
	if( p_Index >= GetEntryCount( p_RequestId ) )
	{
		return false ;
	}
	
	int	i = 0 ;

	for( Current = ldap_first_entry( m_LDAP, Res ); Current != NULL ;
	Current = ldap_next_entry( m_LDAP, Current ) )
	{
		if( i == p_Index )
		{
			Ret = true ;
			FillEntry( Current, p_Entry );
			break;
		}
		i++ ;
	}
	return Ret ;
}
void LdapClass::FillEntry( LDAPMessage *p_Msg, LdapEntry& p_Entry )
{
	char *Dn = ldap_get_dn( m_LDAP, p_Msg );
	char *Attribute ;
	BerElement	*Ber = NULL ;
	
	p_Entry.SetDn( wxString( Dn, wxConvUTF8 ) ) ;
	
	ldap_memfree( Dn );
	
	for( Attribute = ldap_first_attribute( m_LDAP, p_Msg, &Ber );
	Attribute != NULL; Attribute = ldap_next_attribute( m_LDAP, p_Msg, Ber ) )
	{
		bool IsBinary = false ;

		IsBinary = IsBinaryAttribute( wxString( Attribute, wxConvUTF8 ) );
		if( !IsBinary )
		{
			berval **Values = NULL ;
			Values = ldap_get_values_len( m_LDAP, p_Msg, Attribute );
			if( Values != NULL )
			{	
				for( int i=0; Values[i] != NULL; i++ )
				{
					p_Entry.AddValue( wxString( Attribute, wxConvUTF8 ), wxString( Values[i]->bv_val, wxConvUTF8, Values[i]->bv_len ) );
				}
				ldap_value_free_len( Values );
			}
			/*char **Values = NULL ;
			Values = ldap_get_values( m_LDAP, p_Msg, Attribute );
			if( Values != NULL )
			{
				
				for( int i = 0; Values[ i ] != NULL ; i++ )
				{
					p_Entry.AddValue( wxString( Attribute, wxConvUTF8 ), wxString( Values[ i ], wxConvUTF8 ) );
				}
				ldap_value_free( Values );
			}*/
		}
		else
		{
			berval **bValues = NULL ;
			bValues = ldap_get_values_len( m_LDAP, p_Msg, Attribute );
			if( bValues != NULL )
			{
				for( int i = 0; bValues[ i ] != NULL; i++ )
				{
					p_Entry.AddBinaryValue( wxString( Attribute, wxConvUTF8 ), bValues[ i ] );
				}
				ldap_value_free_len( bValues );
			}
		}
		ldap_memfree( Attribute );
		
	}
	
	if( Ber != NULL )
		ber_free( Ber, 0 );
	
}
void LdapClass::CloseSearch( int p_RequestId )
{
	assert( m_ResultsMap.find( p_RequestId ) != m_ResultsMap.end() ) ;
	ldap_msgfree( m_ResultsMap[ p_RequestId ] );
	m_ResultsMap.erase( p_RequestId ) ;
}

int LdapClass::Search( int &p_RequestId, const wxString &p_Base, int p_Scope, const wxString &p_Filter,
wxArrayString *p_Attributes, bool p_GetValues, u_short p_AttFlags, long p_Timeout , int p_SizeLimit )
{
	assert( ( p_Scope == LDAP_SCOPE_BASE || p_Scope == LDAP_SCOPE_ONELEVEL ||
	p_Scope == LDAP_SCOPE_SUBTREE ) && m_LDAP != NULL );
	
	char** Attributes = NULL ;
	
	int AttOnly = ( p_GetValues ? 0 : 1 );
	
	struct timeval *Timeout = NULL ;
	int SizeLimit = ( p_SizeLimit == 0 ? LDAP_NO_LIMIT : p_SizeLimit ) ;
	LDAPMessage    *Results = NULL ;
	
	switch( p_AttFlags )
	{
		case LF_OPERATIONAL :
			Attributes = Build_Attribute_Array( LDAP_ALL_OPERATIONAL_ATTRIBUTES, p_Attributes, NULL );
		break;
		case LF_ALL_OPERATIONAL :
			Attributes = Build_Attribute_Array( LDAP_ALL_OPERATIONAL_ATTRIBUTES, NULL, (char*)LDAP_ALL_USER_ATTRIBUTES );
		break;
		case LF_ALL :
			Attributes = NULL ;
		break;
		case LF_NONE :
			Attributes = Build_Attribute_Array( NULL, NULL, (char *)LDAP_NO_ATTRS );
		break;

		default:
			if( p_Attributes == NULL )
				Attributes = Build_Attribute_Array( NULL, NULL, (char *)LDAP_NO_ATTRS );
			else
				Attributes = Build_Attribute_Array( NULL, p_Attributes, NULL );
		break;
	}
	switch( p_Timeout )
	{
		case -1 : // Session timeout
			// Do nothing Timeout is already NULL ;
		break;
		case 0 : // infinite timeout
		break;
			Timeout = ( struct timeval *) malloc( sizeof( struct timeval ) ) ;
			Timeout->tv_sec = Timeout->tv_usec = 0 ;
		default:
			Timeout = ( struct timeval *) malloc( sizeof( struct timeval ) ) ;
			Timeout->tv_usec = 0 ;
			Timeout->tv_sec = p_Timeout ;
		break;
	}
	
	
	
	u_short Version ;
	GetVersion( Version );
	if( Version == 3 )
	{
		m_Err = ldap_search_ext_s( m_LDAP, p_Base.mb_str(wxConvUTF8) , p_Scope, p_Filter.mb_str(wxConvUTF8),
		Attributes, AttOnly, NULL, NULL, Timeout, SizeLimit, &Results );
	}
	else
	{
		int tSizeLimit ;
		GetSizeLimit( tSizeLimit );
		SetSizeLimit( SizeLimit );
		/*m_Err = ldap_search_st( m_LDAP, p_Base.mb_str(wxConvUTF8), p_Scope, p_Filter.mb_str(wxConvUTF8),
		Attributes, AttOnly, Timeout, &Results );*/
		m_Err = ldap_search_ext_s( m_LDAP, p_Base.mb_str(wxConvUTF8), p_Scope, p_Filter.mb_str(wxConvUTF8),
		Attributes, AttOnly, NULL, NULL, Timeout,0, &Results );
		SetSizeLimit( tSizeLimit );
	}
	if( m_Err == LDAP_SUCCESS )
	{
		p_RequestId = ldap_msgid( Results );
		m_ResultsMap[ p_RequestId ] = Results ;
	}
	if( Attributes != NULL )
	{
		Free_Attribute_Array( Attributes );
	}
	if( Timeout != NULL )
		free( Timeout );
	return m_Err ;
}

char** LdapClass::Build_Attribute_Array( const char *p_Array1, wxArrayString *p_Array2, char *p_Str )
{
	char **Ret = NULL ;
	long Count = 0;
	long Index = 0;
	
	if( p_Str != NULL )
		Count ++ ;
	if( p_Array1 != NULL )
	{
		Count += 1;
	}
	if( p_Array2 != NULL )
	{
		Count += p_Array2->Count() ;
	}
	
	Count ++ ;
	
	Ret = ( char ** )malloc( Count * sizeof( char * ) ) ;
	
	if( p_Array1 != NULL )
	{
	/*
		for( long i = 0; p_Array1[ i ] != NULL; i++ )
		{
			Ret[ Index ] = strdup( p_Array1[ i ] );
			Index ++ ;
		}
	*/
		Ret[ Index ] = strdup( p_Array1 );
		Index ++;
	}
	if( p_Array2 != NULL )
	{
		for( long i = 0; i < p_Array2->Count(); i++ )
		{
			//wxString UTF8Value( p_Array2->Item( i ), wxConvUTF8 );
			Ret[ Index ] =  strdup( p_Array2->Item( i ).mb_str(wxConvUTF8) );
			Index ++ ;
		}
	}
	if( p_Str != NULL )
	{
		Ret[ Index ] = strdup( p_Str ) ;
		Index ++;
	}
	Ret[ Index ] = NULL ;
	
	return Ret ;
}

void LdapClass::Free_Attribute_Array( char** p_Array )
{
	assert( p_Array != NULL );
	for( long i = 0; p_Array[ i ] != NULL ; i++ )
	{
		free( p_Array[ i ] );
	}
	free( p_Array );
}

LDAPMod** LdapClass::Attributes2Mods( LdapEntry& p_Entry, int p_Operation )
{
	/*
	*	Convertir en UTF-8 les valeurs des attributs.
	*/
	LDAPMod** Attributes = NULL ;
	long	  nAttributes ;
	long	  nValues ;
	
	nAttributes = p_Entry.CountAttributes() ;
	
	Attributes = new LDAPMod*[ nAttributes + 1 ] ;
	Attributes[ nAttributes ] = NULL ;
	
	for( long i = 0; i < nAttributes ; i++ )
	{
		LdapAttribute* Attribute = p_Entry.GetAttribute( i );
		nValues = Attribute->CountValues() ;
		
		LDAPMod* NewMod = new LDAPMod ;
		NewMod->mod_op = p_Operation ;

		NewMod->mod_type = strdup( Attribute->GetName().mb_str(wxConvUTF8) ) ;
		if( Attribute->IsBinary() )
		{
			NewMod->mod_op = NewMod->mod_op | LDAP_MOD_BVALUES ;
			NewMod->mod_bvalues = new berval* [ nValues + 1 ];
			NewMod->mod_bvalues[ nValues ] = NULL ;
			for( long j = 0; j < nValues; j++ )
			{
				NewMod->mod_bvalues[ j ] = ber_bvdup( Attribute->GetBinaryValue( j ) ) ;
			}
		}
		else
		{
			NewMod->mod_values = new char* [ nValues + 1 ] ;
			NewMod->mod_values[ nValues ] = NULL ;
			for( long j = 0; j < nValues; j++ )
			{
				NewMod->mod_values[ j ] = strdup( Attribute->GetValue( j ).mb_str(wxConvUTF8) ) ;
			}
		}
		Attributes[ i ] = NewMod ;
	}
	return Attributes ;
}
int	LdapClass::AddEntry( LdapEntry& p_Entry )
{
	/*
	* 	Convertir le DN en UTF-8
	*/
	LDAPMod** Attributes = NULL;
	Attributes = Attributes2Mods( p_Entry, LDAP_MOD_ADD );
	m_Err = ldap_add_ext_s( m_LDAP, p_Entry.GetDn().mb_str(wxConvUTF8), Attributes, NULL, NULL );
	
	ldap_mods_free( Attributes, true );
	return m_Err ;
}

int	LdapClass::ReplaceEntry( LdapEntry& p_Entry )
{
	/*
	* 	Convertir le DN en UTF-8
	*/
	LDAPMod** Attributes = NULL;
	Attributes = Attributes2Mods( p_Entry, LDAP_MOD_REPLACE );
	m_Err = ldap_add_ext_s( m_LDAP, p_Entry.GetDn().mb_str(wxConvUTF8), Attributes, NULL, NULL );
	
	ldap_mods_free( Attributes, true );
	return m_Err ;
}

int	LdapClass::DeleteEntry( LdapEntry& p_Entry )
{
	/*
	* 	Convertir le DN en UTF-8
	*/
	m_Err = ldap_delete_ext_s( m_LDAP, p_Entry.GetDn().mb_str(wxConvUTF8), NULL, NULL );
	return m_Err ;
}
int	LdapClass::Rename( const wxString& p_OldDn, const wxString& p_NewRDn )
{
	/*
	* 	Convertir le DN en UTF-8
	*/
	//m_Err = ldap_modrdn2_s( m_LDAP, p_OldDn.mb_str(wxConvUTF8), p_NewRDn.mb_str(wxConvUTF8), 0 );
	m_Err = ldap_rename_s( m_LDAP, p_OldDn.mb_str(wxConvUTF8), p_NewRDn.mb_str(wxConvUTF8), NULL, 0, NULL, NULL );
	return m_Err ;
}

int	LdapClass::AddAttribute( LdapEntry& p_Entry )
{
	/*
	* 	Convertir le DN en UTF-8
	*/
	LDAPMod** Attributes = NULL;
	Attributes = Attributes2Mods( p_Entry, LDAP_MOD_ADD );
	
	m_Err = ldap_modify_ext_s( m_LDAP, p_Entry.GetDn().mb_str(wxConvUTF8), Attributes, NULL, NULL );
	
	ldap_mods_free( Attributes, true );
	return m_Err ;
}


int	LdapClass::DeleteAttribute( const wxString& p_Dn, const wxString& p_AttributeName )
{
	LDAPMod 	Mod ;
	LDAPMod* 	Mods[ 2 ] ;
	
	Mod.mod_op = LDAP_MOD_DELETE ;
	Mod.mod_type = strdup( p_AttributeName.mb_str(wxConvUTF8) ) ;
	Mod.mod_values = NULL ;
	
	Mods[ 0 ] = &Mod ;
	Mods[ 1 ] = NULL ;
	
	m_Err = ldap_modify_ext_s( m_LDAP, p_Dn.mb_str(wxConvUTF8), Mods, NULL, NULL );
	
	free( Mod.mod_type );
	
	return m_Err ;
}

int LdapClass::AddAttributeValue( const wxString& p_Dn, const wxString& p_AttributeName, const wxString& p_Value )
{
	
	LDAPMod 	Mod ;
	LDAPMod* 	Mods[ 2 ] ;
	
	Mod.mod_op = LDAP_MOD_ADD ;
	Mod.mod_type = strdup( p_AttributeName.mb_str(wxConvUTF8) ) ;
	Mod.mod_values = new char*[ 2 ] ;
	
	Mod.mod_values[ 0 ] = strdup( p_Value.mb_str(wxConvUTF8) ) ;
	Mod.mod_values[ 1 ] = NULL ;
	
	Mods[ 0 ] = &Mod ;
	Mods[ 1 ] = NULL ;
	
	m_Err = ldap_modify_ext_s( m_LDAP, p_Dn.mb_str(wxConvUTF8), Mods, NULL, NULL );
	
	free( Mod.mod_type );
	free( Mod.mod_values[ 0 ] );
	delete []Mod.mod_values ;
	
	return m_Err ;
}

int LdapClass::AddAttributeValue( const wxString& p_Dn, const wxString& p_AttributeName, const berval& p_Value )
{
	LDAPMod 	Mod ;
	LDAPMod* 	Mods[ 2 ] ;
	
	Mod.mod_op = LDAP_MOD_ADD | LDAP_MOD_BVALUES;
	Mod.mod_type = strdup( p_AttributeName.mb_str(wxConvUTF8) ) ;
	Mod.mod_bvalues = new berval*[ 2 ] ;
	
	Mod.mod_bvalues[ 0 ] = ( berval* )&p_Value ;
	Mod.mod_bvalues[ 1 ] = NULL ;
	
	Mods[ 0 ] = &Mod ;
	Mods[ 1 ] = NULL ;
	
	m_Err = ldap_modify_ext_s( m_LDAP, p_Dn.mb_str(wxConvUTF8), Mods, NULL, NULL );

	
	free( Mod.mod_type );
	
	delete []Mod.mod_bvalues ;
	
	return m_Err ;
}

int LdapClass::DeleteAttributeValue( const wxString& p_Dn, const wxString& p_AttributeName, const wxString& p_Value )
{
	LDAPMod 	Mod ;
	LDAPMod* 	Mods[ 2 ] ;
	
	Mod.mod_op = LDAP_MOD_DELETE ;
	Mod.mod_type = strdup( p_AttributeName.mb_str(wxConvUTF8) ) ;
	Mod.mod_values = new char*[ 2 ] ;
	
	Mod.mod_values[ 0 ] = strdup( p_Value.mb_str(wxConvUTF8) ) ;
	Mod.mod_values[ 1 ] = NULL ;
	
	Mods[ 0 ] = &Mod ;
	Mods[ 1 ] = NULL ;
	
	m_Err = ldap_modify_ext_s( m_LDAP, p_Dn.mb_str(wxConvUTF8), Mods, NULL, NULL );
	
	free( Mod.mod_type );
	free( Mod.mod_values[ 0 ] );
	delete []Mod.mod_values ;
	
	return m_Err ;

}

int LdapClass::DeleteAttributeValue( const wxString& p_Dn, const wxString& p_AttributeName, const berval& p_Value )
{
	LDAPMod 	Mod ;
	LDAPMod* 	Mods[ 2 ] ;
	
	Mod.mod_op = LDAP_MOD_DELETE | LDAP_MOD_BVALUES;
	Mod.mod_type = strdup( p_AttributeName.mb_str(wxConvUTF8) ) ;
	Mod.mod_bvalues = new berval*[ 2 ] ;
	
	Mod.mod_bvalues[ 0 ] = ( berval* )&p_Value ;
	Mod.mod_bvalues[ 1 ] = NULL ;
	
	Mods[ 0 ] = &Mod ;
	Mods[ 1 ] = NULL ;
	
	m_Err = ldap_modify_ext_s( m_LDAP, p_Dn.mb_str(wxConvUTF8), Mods, NULL, NULL );
	
	free( Mod.mod_type );
	
	delete []Mod.mod_bvalues ;
	
	return m_Err ;
}

int LdapClass::CopyTo( int p_Id, const wxString& p_ToDn, const wxString& p_OldDn )
{
	assert( m_ResultsMap.find( p_Id ) != m_ResultsMap.end() );
	assert( m_LDAP != NULL );
	
	LDAPMessage	*Current ;
	
	LDAPMessage *Entries = NULL ;
	
	Entries = m_ResultsMap[ p_Id ] ;

	for( Current = ldap_first_entry( m_LDAP, Entries ); Current != NULL ;
	Current = ldap_next_entry( m_LDAP, Current ) )
	{
			m_Err = CopyEntry( Current, p_ToDn, p_OldDn );
	}
	return m_Err ;
}
int LdapClass::CopyEntry( LDAPMessage* p_Entry, const wxString& p_ToDn, const wxString& p_OldDn )
{
	char* Attribute = NULL ;
	BerElement	*Ber = NULL ;
	std::vector< LDAPMod* > ModVector ;
	LDAPMod**	Mods ;
	
	
	for( Attribute = ldap_first_attribute( m_LDAP, p_Entry, &Ber );
	Attribute != NULL; Attribute = ldap_next_attribute( m_LDAP, p_Entry, Ber ) )
	{
		LDAPMod* NewMod = new LDAPMod ;
		NewMod->mod_op = LDAP_MOD_ADD | LDAP_MOD_BVALUES ;
		NewMod->mod_type = Attribute; 
		
		NewMod->mod_bvalues = ldap_get_values_len( m_LDAP, p_Entry, Attribute );
		
		ModVector.insert( ModVector.end(), NewMod );
		//ldap_value_free_len( bValues );
		//ldap_memfree( Attribute );
		
	}
	if( Ber != NULL )
		ber_free( Ber, 0 );
	
	Mods = new LDAPMod*[ ModVector.size() + 1 ];
	Mods[ ModVector.size() ] = NULL ;
	
	std::vector< LDAPMod* >::iterator iter ;
	
	long i = 0 ;
	for( iter = ModVector.begin(); iter != ModVector.end(); iter++ )
	{
		Mods[ i ] = *iter ;
		i++;
	}
	
	char *Dn = ldap_get_dn( m_LDAP, p_Entry );
	wxString	wDn = wxString( Dn, wxConvUTF8 ) ;
	
	ldap_memfree( Dn );
	
	wDn.Replace( p_OldDn, p_ToDn, false );
	
	m_Err = ldap_add_ext_s( m_LDAP, wDn.mb_str(wxConvUTF8), Mods, NULL, NULL );
	for( iter = ModVector.begin(); iter != ModVector.end(); iter++ )
	{
		ldap_value_free_len( (*iter)->mod_bvalues ) ;
		ldap_memfree( (*iter)->mod_type );
	}
	return m_Err ;
}
wxArrayString LdapClass::Explode_Dn( const wxString& p_Dn )
{
	wxArrayString	Ret; 
	char** Elems = ldap_explode_dn( p_Dn.mb_str(), 0 );
	if( Elems == NULL )
	{
		return Ret;
	}
	for( long i = 0 ; Elems[ i ] != NULL; i++ )
	{
		Ret.Add( wxString( Elems[ i ], wxConvUTF8 ) );
	}
	
	//ldap_value_free( Elems );
	char_array_free( Elems );
	return Ret;
}
