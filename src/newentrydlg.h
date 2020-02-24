#ifndef __LET_NewEntryDlg_H
#define __LET_NewEntryDlg_H


//#include "../main_frame.h"
#include "ldapclass.h"
#include "schema.h"

class NewEntryDlg : public wxDialog
{
	public:
		NewEntryDlg( wxWindow* p_Parent );
		NewEntryDlg( wxWindow* p_Parent, const wxString& p_ParentDn, SchemaUtil& p_Ldap );
		wxString m_NewRdn ;
		wxString m_NewDn ;
	protected :
		SchemaUtil *m_Ldap;

		void OnMustAdd( wxCommandEvent& p_Event );
		void OnMayAdd( wxCommandEvent& p_Event );
		void OnObjectClassChanged( wxCommandEvent& p_Event );
		void OnRdnChanged( wxCommandEvent& p_Event );
		void OnTreeRightClick( wxTreeEvent& p_Event );
		void OnAddValue( wxCommandEvent& WXUNUSED( p_Event ) );
		void OnDelValue( wxCommandEvent& WXUNUSED( p_Event ) );
		void OnDeleteAttribute( wxCommandEvent& WXUNUSED( p_Event ) );
		void OnSave( wxCommandEvent& WXUNUSED( p_Event ) );
		void OnManualAdd( wxCommandEvent& p_Event );
		//void OnCancel( wxCommandEvent& WXUNUSED( p_Event ) );
		void SetManual( bool p_Manual );
	private:
		void InitControls();
	    wxTextCtrl* parentdn_text_ctrl;
		wxTextCtrl* entryrdn_text_ctrl;
		wxComboBox* objectclass_combo_box;
		wxTextCtrl* objectclass_text_ctrl;
		wxTreeCtrl* attributes_tree_ctrl;
		wxListBox* mustattributes_list_box;
		wxListBox* mayattributes_list_box;
		wxTextCtrl* attribute_text_ctrl;
		wxButton* button_add;
		wxButton* button_save;
		wxButton* button_cancel;

		DECLARE_EVENT_TABLE()
};

#endif
