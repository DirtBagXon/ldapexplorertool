#ifndef __LET_Attribute_Add_Dlg_H
#define __LET_Attribute_Add_Dlg_H


//#include "../main_frame.h"
#include "ldapclass.h"
#include "schema.h"

class Attribute_Add_Dlg : public wxDialog
{
	public:
//		Attribute_Add_Dlg();
		Attribute_Add_Dlg( wxWindow* p_Parent, const wxString& p_Dn, wxArrayString p_ObjectClasses, SchemaUtil& p_Ldap ) ;

		
	protected :

		wxString m_Dn ;
		
		SchemaUtil*	m_Ldap ;
		void OnSave( wxCommandEvent& WXUNUSED( p_Event ) );
		void SetManual( bool p_Manual );
	private:
		void InitControls();
	    wxListBox* attributes_list_box;
		wxTextCtrl* attribute_text_ctrl;
		wxTextCtrl* value_text_ctrl;
		wxButton* button_save;
		wxButton* button_cancel;

		DECLARE_EVENT_TABLE()
};

#endif
