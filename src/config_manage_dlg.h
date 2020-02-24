#ifndef __CONFIG_MANAGE_DLG
#define __CONFIG_MANAGE_DLG

class Config_Manage_Dlg : public wxDialog
{
	public:
		Config_Manage_Dlg(); 		
		~Config_Manage_Dlg(); 		
		wxString ConfigName;
	private:
		void FillList( void );
		void OnOpen( wxCommandEvent& p_Event );
		void OnNew( wxCommandEvent& p_Event );
		void OnEdit( wxCommandEvent& p_Event );
		void OnDelete( wxCommandEvent& p_Event );
		void InitControls();
		wxListBox* config_list_ctrl;
	DECLARE_EVENT_TABLE()
};
#endif
