#include "substrateItem.h"
#include "common.h"
#include "message.h"

substrateItem::substrateItem ()
{
	ZeroValues();
}

substrateItem::substrateItem(char *aName, const int aIndex, const base * aOwner)
	:base(aName,aIndex,aOwner)
{
	ZeroValues();
}

/****************************************************************************\
\****************************************************************************/
substrateItem* substrateItem::clone()
{
	substrateItem* asubstrateItem;
	(substrateItem*)asubstrateItem= new substrateItem;
	(*asubstrateItem)=(*this);
	return asubstrateItem;
}

substrateItem::~substrateItem ()
{
};

void substrateItem::ZeroValues()
{
	waterConc      = 0;
   nonVolatileConc     = 0;
   readilyDecompConc  = 0;
   resistantConc  = 0;
   readilyCtoN				= 0.0;
   resistantCtoN		= 0.0;
   nitrateConc=0.0;
   ammoniumConc=0.0;
};
void substrateItem::ReadParameters(fstream * file)
{
   Setfile(file);
   SetCritical();
   FindSection(GetName(),GetIndex());
   double DM=0.0;
   GetParameter("dryMatter",&DM);
   waterConc = 1-DM;
   GetParameter("nonVolatileConc",&nonVolatileConc);
   GetParameter("readilyDecompConc",&readilyDecompConc);
   GetParameter("resistantConc",&resistantConc);
   double test = nonVolatileConc + readilyDecompConc + resistantConc;
   if (test!=1.0)
   	theMessage->FatalError("substrateItem:: OM components do not sum to 1.0");

   nonVolatileConc*=DM;
   readilyDecompConc*=DM;
   resistantConc*=DM;
   GetParameter("nitrateConc",&nitrateConc);
   GetParameter("ammoniumConc",&ammoniumConc);
   GetParameter("code",&code);
   UnsetCritical();
   GetParameter("readilyC:N",&readilyCtoN);
   GetParameter("resistantC:N",&resistantCtoN);
   Setfile(NULL);
   double total = waterConc+nonVolatileConc+readilyDecompConc+resistantConc;
   if ((total>1.0)||(total<1.0))
   	theMessage->FatalError("substrateItem:: components do not sum to 1.0");
}


void substrateItem::Add(substrateItem * asubstrateItem, double proportion)
{
	waterConc           =   (1-proportion)*waterConc + proportion * asubstrateItem->GetwaterConc();
   nonVolatileConc     =   (1-proportion)*nonVolatileConc + proportion * asubstrateItem->GetnonVolatileConc();
   readilyDecompConc   =   (1-proportion) * readilyDecompConc + proportion * asubstrateItem->GetreadilyDecompConc();
   resistantConc       = 	(1-proportion) *resistantConc + proportion * asubstrateItem->GetresistantConc();
   readilyCtoN			  = 	(1-proportion) *readilyCtoN + proportion * asubstrateItem->GetreadilyCtoN();
   resistantCtoN		  = 	(1-proportion) *resistantCtoN + proportion * asubstrateItem->GetresistantCtoN();
   nitrateConc			  = 	(1-proportion) *nitrateConc + proportion * asubstrateItem->GetnitrateConc();
   ammoniumConc		  = 	(1-proportion) *ammoniumConc + proportion * asubstrateItem->GetammoniumConc();
//
}




