
#ifndef substrateItem_H  //Required for current class
  #define substrateItem_H

#include "common.h"
#include "base.h"


class substrateItem: public base
{
   protected:
		double waterConc;
		double nonVolatileConc;
      double readilyDecompConc;
      double resistantConc;
      double nitrateConc;
      double ammoniumConc;
		double readilyCtoN;
		double resistantCtoN;
      int code;

	public:

  		substrateItem ();

      //Copy constructor
  		substrateItem (const substrateItem& asubstrateItem );
		substrateItem(char *aName, const int aIndex, const base * aOwner);

		substrateItem* clone();
  		//Destructor
  		~substrateItem ( );

   	//Get accessor function for attributes
      double GetresistantConc() const {return resistantConc;}
      double GetnonVolatileConc() const {return nonVolatileConc;}
      double GetreadilyDecompConc() const {return readilyDecompConc;}
      double GetwaterConc() const {return waterConc;}
      double GetnitrateConc() const {return nitrateConc;};
      double GetammoniumConc() const {return ammoniumConc;};
		double GetreadilyCtoN() const {return readilyCtoN;}
		double GetresistantCtoN() const {return resistantCtoN;}
      int Getcode() {return code;}
      void ZeroValues();
      void ReadParameters(fstream * file);
		void Add(substrateItem * asubstrateItem, double proportion);
};

#endif

