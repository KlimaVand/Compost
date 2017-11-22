
#ifndef substanceDB_H  //Required for current class
  #define substanceDB_H

#include "common.h"
#include "substance.h"
#include "clonlist.h"

class substanceDB: public base
{
   protected:

   cloneList<substance> *theSubstanceList;

	public:

  		substanceDB ();

		substanceDB(char *aName, const int aIndex, const base * aOwner);
  		//Destructor
  		~substanceDB();

	void Initialise(char *InputFilename);
   	//Get accessor function for attributes
	substance * FindSubstance(char * name);

};

#endif

