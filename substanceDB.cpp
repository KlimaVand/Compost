#include "substanceDB.h"
#include "common.h"
#include "message.h"
#include "OMpool.h"

substanceDB::substanceDB ()
:base()
{
   theSubstanceList = new cloneList<substance>;
}

substanceDB::substanceDB(char *aName, const int aIndex, const base * aOwner)
	:base(aName,aIndex,aOwner)
{
   theSubstanceList = new cloneList<substance>;
}
substanceDB::~substanceDB ()
{
   delete theSubstanceList;
}


void substanceDB::Initialise(char *InputFilename)
{
  if (OpenInputFile(InputFilename))
  {
      SetCritical();
      Setcur_pos(0);
      char *InstName = "water";
      substance *water = new substance(InstName,NULL,0);
      water->ReadParameters(file);              //read basic parameters from file
      theSubstanceList->InsertLast(water); //store in linklist

      strcpy(InstName,"nvs");
      substance *nonVolatile = new substance(InstName,NULL,0);
      nonVolatile->ReadParameters(file);
      theSubstanceList->InsertLast(nonVolatile);

      strcpy(InstName,"readilyDecomp");
      OMpool *readilyDecomp = new OMpool(InstName,NULL,0);
      readilyDecomp->ReadParameters(file);
      theSubstanceList->InsertLast(readilyDecomp);

      strcpy(InstName,"resistant");
      OMpool *resistant = new OMpool(InstName,NULL,0);
      resistant->ReadParameters(file);
      theSubstanceList->InsertLast(resistant);

      strcpy(InstName,"consolidated");
      OMpool *consolidated = new OMpool(InstName,NULL,0);
      consolidated->ReadParameters(file);
      consolidated->Setmass(0.0);
      theSubstanceList->InsertLast(consolidated);
      CloseInputFile();
   }
   else
   	cout << "Error opening basic.dat ";
}

substance *substanceDB::FindSubstance(char * name)
{
   substance *asubstance;
   int i=0;
   do
   {
      asubstance=theSubstanceList->ElementAtNumber(i);
      if (strcmp(name,asubstance->GetName())==0)
      	return asubstance;
      i++;
   }
   while (i<theSubstanceList->NumOfNodes());
   if (!asubstance)
   	theMessage->FatalError("Cannot find substrate with this name");

  	return asubstance;
};

