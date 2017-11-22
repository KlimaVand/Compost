#ifndef stack_H  //Required for current class
  #define stack_H

#include "common.h"
#include "clonList.h"
#include "fraction.h"
#include "substrateDB.h"
#include "compMetClass.h"

class compostStack: public base
{
   protected:
		double temperature;
		double diameter;
      double length;
      double height;
      double surface_area;
      double aerobicDepth;
      double volume;
      double volumeAnaerobic;
      double volumeChange;
      double temperatureChange;
      double Gr;
   	cloneList <stackFraction> * theFractions; //
      stackFraction * aerobic;
      stackFraction * anaerobic;
      compMetClass *metData;
      paramClass *theparams;
      bool fixLength;

// management parameters
		int turningDate[20];
      int turningHour[20];
      bool doneTurn[20];
      double irrigationAmount[20];
      double aerationMultiplier;
      int shape;

	public:
  		compostStack ();
      //Copy constructor
//  		stack (const stack& astack );
  		//Destructor
  		~compostStack ( );

   	//Get accessor function for attributes
      double Gettemperature() const {return temperature;}
      double Getdiameter() const {return diameter;}
      double Getlength() const {return length;}
      double Getsurface_area() const {return surface_area;}
		double GetstackMass();
		double GetstackBulkDensity();
		double GetstackHeatCapacity();
      double GetstackVolume(stackFraction *anAerobic, stackFraction *anAnaerobic);
		double GetStackHeight(double volume);
		double GetaerobicDepth() {return aerobicDepth;}
		double Getvolume() {return volume;}
      double GetStackDiameter(double aVolume);
		double GetStackLength(double aVolume, double aDiameter);
      double GetVolumeAnaerobic(double aDiameter, double anAerobicDepth);
		double GetAeratedCrossSectionalArea();
      double GetStackArea();
      void CheckBalances();
//      void Settemperature(double aValue) {temperature=aValue;}

      void CarbonDynamics();
		void StartCompStack(stackFraction *aFraction);
		bool Initialise(substrateDB *thesubstrateDB);
      double GetdryBulkDensity();
		void ThermalDynamics();
		void DoMix(double anIrrigationAmount);
		bool CheckMix(double theTime);
		bool TimeRoutine(double *timeStep, bool *abort);

		void WriteStack(fstream& os, bool header);
      void ReadMet() {metData->readHourlyData();};

};

#endif

