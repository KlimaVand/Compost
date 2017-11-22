#ifndef MESSAGE_H
#define MESSAGE_H

#include "common.h"
#include <string>
#include <stdlib>
using namespace std;
class message
{
private:
	static fstream * fw;
	static fstream * fl;
	static fstream * fd; //debug text file
	static char * WarnFileName;
	static char * LogFileName;
	static char * DebugFileName;
	static int Warnings;
	static int Copies;

	message& operator=(const message& m); // Dissable the compilers generation of default assignment operator.
	message(const message& m);            // Dissable the compilers generation of default copy constructor.

	char * AssignString(char * c);

public:
	message();
	~message();
	void InitMessage(char * WarnFN,char * LogFN, char *DebugFN);
	void ChangeLogfilename(char * cp);
	void Warning(const char * cp1,const char * cp2="",const char * cp3="");
	void Warning(const string st1, const string st2, const string st3);
   void Warning(const string st1, int anint);
   void Warning(const char * cp1, const double aNumber, const char * cp3="");
	void WarningWithDisplay(const char * cp1,const char * cp2="",const char * cp3="");
   void WarningWithDisplay(const string st1, const string st2="", const string st3="");
	void WarningWithDisplay(const char * cp1, const double aNumber, const char * cp3="");
	void FatalError(const char * cp1,const char * cp2="",const char * cp3="");
	void LogEvent(const char * cp1);
   void LogEvent(const char * cp, const double number);
	int NumOfWarnings();
	fstream * GiveHandle();
	void CloseHandle();
	fstream * GiveDebugHandle();
	void CloseDebugHandle();
	void LogEvent(const char * cp1, int anint, const char *cp2, double number);
	void LogEventWithTime(const char * cp1, int anint, const char *cp2, double number);
	void LogEventWithTime(const char * cp1);
   void LogEventWithTime(const char * cp, const double number);
};

extern message * theMessage;

#endif
