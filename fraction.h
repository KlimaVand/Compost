#ifndef fraction_H  //Required for current class
  #define fraction_H

#include "common.h"
#include "clonList.h"
#include "substance.h"
#include "substanceDB.h"
#include "OMpool.h"
#include "substrateItem.h"
#include "compMetClass.h"
#include "paramClass.h"
#include "budget.h"

class stackFraction: public base
{
   protected:
		double volume;
      double depth;
   	cloneList <substance> * theComponents; //
      substance * water;
      substance * nonVolatile;
      OMpool * readilyDecomp;
      OMpool * resistant;
      OMpool * consolidated;
      double nitrateN;
      double ammoniumN;

      double aerationRate;   //in kg per hour
      double CO2Closs;
      double NH3Nloss;
      double deNitrificLoss;
      double O2demandRate;
	 	double ventWaterLoss;
 		double biolWaterGen;
      double biologicalEnergy;
      double sensibleHeatAir;
      double latentHeatLoss;
      double surfaceHeatLoss;
      double radiativeHeatLoss;
		double temperatureFunc;
      double moistureFunc;
		double energyChange;
		double temperature;
      double mineralisation;
		double rateNitrification;
      double rateDenitrification;
      double initialResistant;
      paramClass *theparams;
      compMetClass *metdata;
      bool startup;   //used to indicate when starting (and so use O2 in FAS)
      budget Cbudget;
      budget NVSbudget;
      budget Waterbudget;
      budget Nbudget;
      budget Energybudget;

	public:
  		stackFraction ();
      //Copy constructor
		stackFraction(char *aName, const int aIndex, const base * aOwner);
		stackFraction* clone();
      stackFraction(const stackFraction& astackFraction);
		void ZeroVariables();
  		//Destructor
  		~stackFraction ();

      OMpool * GetreadilyDecomp() {return readilyDecomp;}
      OMpool * Getresistant() {return resistant;}
      OMpool * Getconsolidated() {return consolidated;}
      substance * Getwater() const {return water;}
      substance * GetnonVolatile() const {return nonVolatile;}

		double GetFreeAirSpace();
      void Setdepth(double aValue) {depth = aValue;}
      double Getdepth() {return depth;}
		double GetdryBulkDensity();
   	double Getmass() const;
		double GetmassOM();
      double GetWaterMass() const {return water->Getmass();}
		double GetmassSolids();
		double GetmassNVS();
		double GetmassC() const ;
		double GetmassN() const;
      double GetTotalEnergy() const;
		double GetSensibleEnergy() const;
		double GetLatentEnergy();
		double GetmeanSGsolids();
		double Getporosity();
		double GetpropDM();
		double GetwetBulkDensity();
		double GetvolumetricWaterContent();
      double GetSpecificGravity();
		double GetspecificGravityOM();
      double GetmoistureContdb();
      double GetnitrateN() const {return nitrateN;}
      double GetammoniumN() const {return ammoniumN;}
      double GetCO2Closs() const {return CO2Closs;}
      double GetaerationRate() const {return aerationRate;}
      double GetNH3Nloss() {return NH3Nloss;};
      double GetinitialResistant(){return initialResistant;};
      void SetnitrateN(double aValue) {nitrateN = aValue;}
      void SetammoniumN(double aValue) {ammoniumN = aValue;}
   	void Setstartup(bool aValue) {startup = aValue;}
      void Settemperature(double aValue) {temperature=aValue;}
      void ChangeEnergyBudget(double aValue);
      double GetChemicalEnergy() const;
		stackFraction& operator-(const stackFraction &aFraction);
		stackFraction& operator+(const stackFraction &aFraction);
      void AdjustFraction(stackFraction * aFraction);
   	cloneList <substance> * GettheComponents() const {return theComponents;} //
		void Initialise(substanceDB *asubstanceDB, substrateItem * anItem, double amount,
      				paramClass *params,compMetClass * themetdata);
		void Initialise();
//		double Getmass() {return (GetWaterMass()+GetmassOM()+GetmassSolids());}
		double GetHeatCapacity();
		double GetAeratedDepth();
      double CalcVolume();
      double GetVolume() {return volume;}
		double CalcAerationRate(double aeratedCrossSectionalArea, double airTempOut, double aerobicHt, double *Grashof);
		double CalcFlowRate(double VolumeAerationRate);
		double GetTemperatureFunc();
		double GetNitrificationTemperatureFunc();
      double GetThermalConductivity();
		double GetMoistureFunc();
		double GetNitrificationRate();
		void DoDenitrification(double *timeStep);
		double GetAmmoniaLossRate();
      double GetSurfaceHeatLoss(double stackArea);
		void CalcMineralisation(double degrateReadilyDecomp,
				double *degrateResistant, double *timestep);
		void KickstartO2Demand(double CO2ProdRate, double *timeStep);
		double GetO2SupplyRate();
		double CalcO2Demand(double CO2ProdRate);
		void UpdateBudgets();
		void CarbonDynamics(stackFraction *aFraction,
				 double *timeStep, double *degrateReadilyDecomp, double *degrateResistant);
		void NitrogenDynamics(stackFraction *aFraction, double *timeStep,
			 	double degrateReadilyDecomp, double degrateResistant);
		double ThermalDynamics(stackFraction *aFraction, double massFlowRate,
      				double stackArea, double *energyLost,  double *timeStep);
		double WaterDynamics(double deltaHumidity, double massFlowRate,
		double OMdegraded, stackFraction *aFraction, double *timeStep);
		void Dynamics(int pool, stackFraction *aFraction,double *timeStep);
		void Update();
      void CreateProportion(double aProportion);
		void WriteFraction(fstream& os, bool header);
		void CheckBalances();
};

#endif

