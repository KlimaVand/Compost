#include "substance.h"
#include "common.h"
#include "message.h"

substance::substance ()
: base()
{
	mass      = 0;
   specific_heat  = 0;
   specific_gravity  = 0;
}

substance::substance(char *aName, const int aIndex, const base * aOwner)
	:base(aName,aIndex,aOwner)
{
	mass      = 0;
   specific_heat  = 0;
   specific_gravity  = 0;
}

substance::~substance ()
{
};


substance::substance (const substance& asubstance )
:base(asubstance)
{
	mass      = asubstance.mass;
   specific_heat  = asubstance.specific_heat;
   specific_gravity  = asubstance.specific_gravity;
}

/****************************************************************************\
\****************************************************************************/
substance* substance::clone()
{
	substance* asubstance;
	(substance*)asubstance= new substance;
	(*asubstance)=(*this);
	return asubstance;
}

/****************************************************************************\
\****************************************************************************/
substance& substance::operator+(const substance &somesubstance)
{
 mass	+= somesubstance.Getmass();
 return *this;
}

/****************************************************************************\
\****************************************************************************/
substance& substance::operator-(const substance &somesubstance)
{
 mass	-= somesubstance.Getmass();
 if (mass<0.0)
    	theMessage->FatalError("mass below zero in ", GetName());
 return *this;
}

void substance::ReadParameters(fstream * file)
{
   Setfile(file);
   SetCritical();
   FindSection(GetName(),GetIndex());
   GetParameter("name",Name);
   GetParameter("specific_heat",&specific_heat);
   GetParameter("specific_gravity",&specific_gravity);
   Setfile(NULL);
}

void substance::CreateProportion(double aProportion)
{
 mass *=aProportion;
}

/****************************************************************************\
	Operator << for output
\****************************************************************************/

void substance::WriteSubstance(fstream& os,bool header)
{
 if (header)
 {
  os << setiosflags(ios::left) << " Name "  << " \t";
  os << setiosflags(ios::left) << " specific_heat " << " \t";
  os << setiosflags(ios::left) << " specifc_gravity " << " \t";
  os << setiosflags(ios::left) << " mass "  << " \t";
 }
 else
 {
  os << setiosflags(ios::left) << " " << GetName() << " \t";
  os << setiosflags(ios::left) << " " << Getspecific_heat() << " \t";
  os << setiosflags(ios::left) << " " << Getspecific_gravity() << " \t";
  os << setiosflags(ios::left) << " " << Getmass() << " \t";
 }
}


