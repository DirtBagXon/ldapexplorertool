#ifndef __LET_ConfigurationDlg_H
#define __LET_ConfigurationDlg_H


//#include "../main_frame.h"

class ConfigurationDlg : public wxDialog
{
	public:
		ConfigurationDlg( wxWindow* p_Parent );
		ConfigurationDlg( wxWindow* p_Parent, const wxString& p_ConfigName );
		wxString Name ;
	protected :
		void OnTestServerConnection( wxCommandEvent& p_Event );
		void OnTestConnection( wxCommandEvent& p_Event );
		void OnGuessBaseDN( wxCommandEvent& p_Event );
		void OnStorePwdClick( wxCommandEvent& p_Event );
		void OnUseDefSSLPortClick( wxCommandEvent& p_Event );
		void OnUseDefPortClick( wxCommandEvent& p_Event );
		void OnUseSSL( wxCommandEvent& p_Event );
		void OnOK( wxCommandEvent& p_Event );
		void OnCAClick( wxCommandEvent& p_Event );
		void OnCertClick( wxCommandEvent& p_Event );
		void OnKeyClick( wxCommandEvent& p_Event );
		void Test( bool p_GetBaseDN );
	
	private:
		void InitControls();
		wxTextCtrl* configname_text_ctrl;
		wxTextCtrl* server_text_ctrl;
		wxTextCtrl* port_text_ctrl;
		wxCheckBox* storepwd_checkbox;
		wxCheckBox* usedefport_checkbox;
		wxTextCtrl* sslport_text_ctrl;
		wxCheckBox* usedefsslport_checkbox;
		wxRadioBox* version_radio_box;
		wxButton* test_server_button;
		wxTextCtrl* userdn_text_ctrl;
		wxCheckBox* anonymous_checkbox;
		wxTextCtrl* password_text_ctrl;
		wxRadioBox* usessl_radio_box;
		wxRadioBox* usetls_radio_box;
		wxTextCtrl* basedn_text_ctrl;
		wxButton* basedn_guess_button;
		wxButton* test_connection_button;
		wxTextCtrl* timelimit_text_ctrl;
		wxTextCtrl* sizelimit_text_ctrl;
		wxRadioBox* referals_radio_box;
		wxTextCtrl* subschemasubentry_text_ctrl;
		wxRadioBox* guessbinary_radio_box;
		wxTextCtrl* cacert_text_ctrl;
		wxButton* button_ca;
		wxTextCtrl* cert_text_ctrl;
		wxButton* button_cert;
		wxTextCtrl* key_text_ctrl;
		wxButton* button_key;
		wxButton* button_ok;
	    wxButton* wxID_CANCEL;

		DECLARE_EVENT_TABLE()
};

#endif
