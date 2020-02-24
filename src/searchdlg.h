#ifndef __SEARCH_DLG
#define __SEARCH_DLG

#include "ldapclass.h"
#include "schema.h"

class SearchDlg : public wxDialog
{
	public:
		SearchDlg( wxWindow *p_parent, SchemaUtil *p_ldap, const wxString& p_base );
		~SearchDlg();
		int Id;
	private:
		wxString m_base;	
		SchemaUtil *m_ldap;
		wxTextCtrl* searchfilter_textctrl;
		wxListBox* searchresult_listbox;
		wxStaticText* searchbase_lbl;
		wxRadioBox* searchlevel_radio;
		void InitControls(void);
		void OnSearch( wxCommandEvent &p_evt );
		void OnClose( wxCommandEvent &p_evt );
		void OnOpen( wxCommandEvent &p_evt );
		DECLARE_EVENT_TABLE()
};
#endif
