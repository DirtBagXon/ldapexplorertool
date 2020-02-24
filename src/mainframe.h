#ifndef __LET_FRAME_H
#define __LET_FRAME_H

#include <wx/splitter.h>
#include <wx/treectrl.h>
#include <wx/listctrl.h>
#include <wx/wizard.h>
#include <ldap.h>
#include "ldapentry.h"
#include "ldapclass.h"
#include "schema.h"

#define BINARY_VALUES	1
#define STRING_VALUES	2
class LDAPItemData: public wxTreeItemData
{	
	public:
		LDAPItemData( const wxString& p_Value) : wxTreeItemData()
		{
			m_Value = p_Value ;
		}
	
		wxString m_Value;
		
};

class ValueItemData: public wxTreeItemData
{
	public:
		ValueItemData( int p_ValueType, berval* p_BerVal = NULL )
		{
			m_Type = p_ValueType ;
			m_BerVal = p_BerVal ;
		}
		~ValueItemData( void )
		{
			if( m_BerVal != NULL )
				ber_bvfree( m_BerVal );
		}
		int m_Type ;
		berval* m_BerVal ;
};
class MainFrame: public wxFrame
{
	public:
		MainFrame();
		virtual ~MainFrame();

	private:
		void OnQuit( wxCommandEvent& WXUNUSED( p_Event ) ) ;
		void OnViewSchema( wxCommandEvent& WXUNUSED( p_Event ) );
		void OnOpen( wxCommandEvent& WXUNUSED( p_Event ) );
		void OnOpenLast( wxCommandEvent& WXUNUSED( p_Event ) );
		void OnNewConnection( wxCommandEvent& WXUNUSED( p_Event ) );
		void OnDeleteConnection( wxCommandEvent& WXUNUSED( p_Event ) );

		void OnOpenConnection( wxCommandEvent& WXUNUSED( p_Event ) );
		void OnEditConnection( wxCommandEvent& WXUNUSED( p_Event ) );
		void OnLeftSelectionChanged( wxTreeEvent& WXUNUSED( p_Event ) );
		void OnRightSelectionChanged( wxTreeEvent& WXUNUSED( p_Event ) );
		void OnRight_PopupMenu( wxTreeEvent& WXUNUSED( p_Event ) );
		void OnLeftMove( wxCommandEvent& WXUNUSED( p_Event ) );
		void OnLeft_PopupMenu( wxTreeEvent& WXUNUSED( p_Event ) );

		void OnAddEntry( wxCommandEvent& WXUNUSED( p_Event ) );
		
		void OnCopyEntry( wxCommandEvent& WXUNUSED( p_Event ) );
		void OnRenameEntry( wxCommandEvent& WXUNUSED( p_Event ) );
		void OnDeleteEntry( wxCommandEvent& WXUNUSED( p_Event ) );
		void OnAddAttribute( wxCommandEvent& WXUNUSED( p_Event ) );
		void OnDeleteAttribute( wxCommandEvent& WXUNUSED( p_Event ) );
		void OnAddValue( wxCommandEvent& WXUNUSED( p_Event ) );
		void OnAddBinValue( wxCommandEvent& WXUNUSED( p_Event ) );
		void OnModifyValue( wxCommandEvent& WXUNUSED( p_Event ) );
		void OnDeleteValue( wxCommandEvent& WXUNUSED( p_Event ) );
		void OnSearch( wxCommandEvent& WXUNUSED( p_Event ) );
		
		
		//void GetConfigurations( wxArrayString& p_List );
		
		void OnConnect( const wxString& p_Config );
		
		void Left_Display( wxTreeItemId p_ParentId, const wxString& p_Url );
		void Right_Display( wxString p_Url );

		SchemaUtil 	m_Ldap ;
		wxString	m_ConfigName ;
		
		wxTreeCtrl* left_tree_ctrl;

		wxTreeCtrl* right_tree_ctrl;

		DECLARE_EVENT_TABLE()
};
#endif
