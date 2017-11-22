#include "substrateDB.h"
#include "common.h"
#include "message.h"

substrateDB::substrateDB ()
{
   theSubstrateList = new cloneList<substrateItem>;
}


substrateDB::~substrateDB ()
{
   delete theSubstrateList;
}


void substrateDB::Initialise(char *InputFilename)
{
  if (OpenInputFile(InputFilename))
  {
      char * InstanceName="Substrate";
      int first,num;
      GetSectionNumbers(InstanceName,&first,&num);
      Setcur_pos(0);
      for(int inx=first;inx<(first+num);inx++)
      {
        SetCritical();
        FindSection(InstanceName,inx);
        substrateItem *substrateInst=new substrateItem(InstanceName,inx,this);
        substrateInst->ReadParameters(file);
        theSubstrateList->InsertLast(substrateInst);
      }
		CloseInputFile();
	}
}

substrateItem * substrateDB::findSubstrate(int code)
{
   substrateItem *asubstrate;
   int i=0;
   do
   {
      asubstrate=theSubstrateList->ElementAtNumber(i);
      if (code == asubstrate->Getcode())
      	return asubstrate;
      i++;
   }
   while (i<theSubstrateList->NumOfNodes());
   if (!asubstrate)
   	theMessage->FatalError("Cannot find substrate with code");
  	return asubstrate;  //never gets here but this statement stops compiler generating a warning
};

