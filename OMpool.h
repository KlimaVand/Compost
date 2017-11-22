// Revised by JB 2/2-99

#ifndef OMpool_H  //Required for current class
  #define OMpool_H

#include "common.h"
#include "substance.h"
#include "nixCbits.h"

class OMpool: public substance
{
   private:
		double C;
		double N;
      double refDecompRate;
      double dryBulkDensity;
      double heatOfCombustion;

	public:

  		OMpool ();
		OMpool(char *aName, const int aIndex, const base * aOwner);
      //Copy constructor
  		OMpool (const OMpool& aOMpool );
  		//Destructor
  		~OMpool ( ) {}
   	//Get accessor function for attributes
		void ZeroValues();
      double GetN() const {return N;}
      double GetC() const {return C;}
      double GetrefDecompRate() const {return refDecompRate;}
      double GetdryBulkDensity() {return dryBulkDensity;};
      double GetheatOfCombustion() const {return heatOfCombustion;}
//      virtual double Getmass();
      double Getmass() const {return C/GetCconcInOrgmatter();}

      void SetN(double aValue) {N=aValue;}
      void SetC(double aValue) {C=aValue;}
		OMpool* clone();

      substance& operator+(const substance &someOMpool);
      substance& operator-(const substance &someOMpool);
		void Initialise(string filename);
		void ReadParameters(fstream * file);
		double Decompose(double timeStep);
		void CreateProportion(double aProportion);

		void WriteSubstance(fstream& os,bool header);

};

#endif

