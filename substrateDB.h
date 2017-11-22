
#ifndef substrateDB_H  //Required for current class
  #define substrateDB_H

#include "common.h"
#include "substrateItem.h"
#include "base.h"
#include "clonlist.h"



class substrateDB: public base
{
   protected:

   cloneList<substrateItem> *theSubstrateList;

	public:

  		substrateDB ();

  		//Destructor
  		~substrateDB();

	void Initialise(char *InputFilename);
   substrateItem * findSubstrate(int code);
   	//Get accessor function for attributes
//      double Getresistant() const {return resistant;}

};

#endif

