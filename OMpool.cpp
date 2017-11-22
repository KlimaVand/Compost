#include "OMpool.h"
#include "common.h"
#include "message.h"

OMpool::OMpool ()
{
	ZeroValues();
}

OMpool::OMpool(char *aName, const int aIndex, const base * aOwner)
	:substance(aName,aIndex,aOwner)
{
	ZeroValues();
}

void OMpool::ZeroValues()
{
	N      				= 0;
   C     				= 0;
   refDecompRate  	= 0;
   dryBulkDensity		= 0.0;
	heatOfCombustion	= 0.0;
}

OMpool::OMpool (const OMpool& aOMpool )
:substance (aOMpool)
{
	N      			= aOMpool.N;
   C     			= aOMpool.C;
   refDecompRate  = aOMpool.refDecompRate;
   dryBulkDensity	= aOMpool.dryBulkDensity;
   heatOfCombustion	= aOMpool.heatOfCombustion;
}

/****************************************************************************\
\****************************************************************************/
OMpool* OMpool::clone()
{
	OMpool* aOMpool;
	(OMpool*)aOMpool= new OMpool;
	(*aOMpool)=(*this);
	return aOMpool;
}


/****************************************************************************\
Only mass, C and N are added
\****************************************************************************/
substance& OMpool::operator+(const substance &someOMpool)
{
 OMpool* aOMpool = (OMpool*)(&someOMpool);
 N += aOMpool->GetN();
 C	+= aOMpool->GetC();
 return *this;
}

/****************************************************************************\
Only mass, C and N are subtracted
\****************************************************************************/
substance& OMpool::operator-(const substance &someOMpool)
{
 OMpool* aOMpool = (OMpool*)(&someOMpool);
 N -= aOMpool->GetN();
 C	-= aOMpool->GetC();
 if ((C<0.0)||(N<0.0))
    	theMessage->FatalError("C or N below zero in ", GetName());

 return *this;
}

void OMpool::Initialise(string filename)
{
//	substance::Initialise(string filename);

}

void OMpool::ReadParameters(fstream * file)
{
	substance::ReadParameters(file);
   Setfile(file);
   SetCritical();
   GetParameter("refDecompRate",&refDecompRate);
   GetParameter("dryBulkDensity",&dryBulkDensity);
   GetParameter("heatOfCombustion",&heatOfCombustion);
   Setfile(NULL);
}

double OMpool::Decompose(double timeStep)
{
	return timeStep * refDecompRate * mass;
}

/****************************************************************************\
	Adjust amount to the proportion indicated
\****************************************************************************/
void OMpool::CreateProportion(double aProportion)
{
	C*=aProportion;
   N*=aProportion;
};
/****************************************************************************\
\****************************************************************************/
/*
double OMpool::Getmass()
{
	return C/GetCconcInOrgmatter();
}
  */

void OMpool::WriteSubstance(fstream& os,bool header)
{
  substance::WriteSubstance(os,header);
  if (header)
  {
  os << setiosflags(ios::left) << " C " << " \t";
  os << setiosflags(ios::left) << " N " << " \t";
  os << setiosflags(ios::left) << " refDecompRate "  << " \t";
  os << setiosflags(ios::left) << " heatOfCombustion "  << " \t";
  }
  else
  {
  os << setiosflags(ios::left) << " " << GetC() << " \t";
  os << setiosflags(ios::left) << " " << GetN() << " \t";
  os << setiosflags(ios::left) << " " << GetrefDecompRate() << " \t";
  os << setiosflags(ios::left) << " " << GetheatOfCombustion() << " \t";
  }
}


