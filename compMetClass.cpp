#include "compMetClass.h"
#include "NixCbits.h"
#include "message.h"

compMetClass::compMetClass ()
{
	airTemperature      = 0;
   humidity     = 0;
   waterVapourPressure  = 0;
}

compMetClass::~compMetClass ()
{
	infile.close();
};


void compMetClass::openFile(char *infileName)
{
   infile.open(infileName,ios::in | ios::nocreate);
   if (!infile)
   	theMessage->FatalError("Cannot find ",infileName);
   char buffer[500];
   infile.getline(buffer,500);
}

void compMetClass::readHourlyData()
{
 int hour;
 double dum;
 infile >> hour >> dum >> airTemperature >> dum >> dum >> dum >> dum >> dum >> dum
 			>> waterVapourPressure >> dum >>dum >> atmosphericPress >> dum >> dum >> precip;
 char buffer[500];
 infile.getline(buffer,500);
// waterVapourPressure=humidity
  //      *GetsaturatedWaterVapourPressure(airTemperature)/100.0;
 waterVapourPressure*=1000.0;
 atmosphericPress*=100.0;
 if (atmosphericPress<=70000.0)
        atmosphericPress=90500.0;
//   atmosphericPress=GetAirPressure(1000.0,airTemperature);
}

void compMetClass::WriteMet(fstream& os, bool header)
{
 if (header)
 {
  os << setiosflags(ios::left) << " airTemperature " << " \t" ;
  os << setiosflags(ios::left) << " waterVapourPressure "<< " \t";
  os << setiosflags(ios::left) << " atmosphericPress "<< " \t";
  os << setiosflags(ios::left) << " precip "<< " \t";
 }
 else
 {
  os << setiosflags(ios::left) << " " << airTemperature << " \t";
  os << setiosflags(ios::left) << " " << waterVapourPressure << " \t";
  os << setiosflags(ios::left) << " " << atmosphericPress << " \t";
  os << setiosflags(ios::left) << " " << precip << " \t";
 }
}

