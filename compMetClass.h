// Revised by JB 2/2-99

#ifndef compMetClass_H  //Required for current class
  #define compMetClass_H

#include "common.h"

class compMetClass
{
   protected:
		double airTemperature;
		double humidity;
      double waterVapourPressure;
      double atmosphericPress;
      double precip;
      fstream infile;
	public:

  		compMetClass ();

  		//Destructor
  		~compMetClass ( );

   	//Get accessor function for attributes
      double GetairTemperature() const {return airTemperature;}
      double Gethumidity() const {return humidity;}
      double GetwaterVapourPressure() const {return waterVapourPressure;}
      double GetatmosphericPress() const {return atmosphericPress;}
      double Getprecip() const {return precip;}
		void openFile(char *infileName);
  		void readHourlyData();
		void WriteMet(fstream& os, bool header);

};

#endif

