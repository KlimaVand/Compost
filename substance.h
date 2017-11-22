// Revised by JB 2/2-99

#ifndef substance_H  //Required for current class
  #define substance_H

#include "common.h"
#include "base.h"

class substance: public base
{
   protected:
		double mass;
      double specific_heat;
      double specific_gravity;

	public:

  		substance ();

      //Copy constructor
  		substance (const substance& asubstance );
		substance(char *aName, const int aIndex, const base * aOwner);
		virtual substance* clone();

  		//Destructor
  		~substance ( );

   	//Get accessor function for attributes
      double Getspecific_heat() const {return specific_heat;}
      double Getspecific_gravity() const {return specific_gravity;}
      virtual double Getmass() const {return mass;}
      void Setmass(double amass) {mass=amass;}
      virtual double GetdryBulkDensity() {return 0.0;}

      virtual substance& operator+(const substance &somesubstance);
		virtual substance& operator-(const substance &somesubstance);
      virtual void ReadParameters(fstream * file);
		virtual void CreateProportion(double aProportion);

		void WriteSubstance(fstream& os,bool header);

};

#endif

