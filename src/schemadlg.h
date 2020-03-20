#ifndef __LET_SchemaDlg_H
#define __LET_SchemaDlg_H


//#include "../main_frame.h"
#include "ldapclass.h"
#include "schema.h"

class SchemaDlg : public wxDialog
{
	public:
		SchemaDlg( wxWindow* p_Parent, SchemaUtil& p_Ldap );
	protected :		
		void OnObjectClassSelected( wxCommandEvent& p_Event );
		void OnAttributeSelected( wxCommandEvent& p_Event );
		void OnObjectClassSelectionChanged( wxTreeEvent& WXUNUSED( p_Event ) );
		void OnAttributeSelectionChanged( wxTreeEvent& WXUNUSED( p_Event ) );

		SchemaUtil *m_Ldap;

	private:
		void InitControls();
		wxListBox* objectclasses_listbox;
		wxListBox* attributes_listbox;
		wxTreeCtrl* attributes_tree_ctrl;
		wxTreeCtrl* objectclass_tree_ctrl;
		void FillObjectClasses( void );
		void FillAttributeTypes( void );
		DECLARE_EVENT_TABLE()
};

#endif
