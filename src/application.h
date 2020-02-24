#ifndef	__LET_APPLICATION_H
#define __LET_APPLICATION_H

class Application: public wxApp
{
	public:
		Application();
		virtual ~Application();
		
		bool OnInit();
	private:

};
#endif
