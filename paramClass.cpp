#include "paramClass.h"
#include "message.h"
#include "common.h"

paramClass::paramClass ()
{
    microYieldCoeff=0.0;
    microRecyclingCoeff=0.0;
    MicrobialCtoNRatio=0.0;
    ConsolidatedCtoNRatio=0.0;
    refNitrificationRate=0.0;
    ammoniaConst=0.0;
    waterProdConst=0.0;
    airflowConst = 0.0;
}


/****************************************************************************\
\****************************************************************************/
void paramClass::GetParameters()
{
	fstream infile;
   infile.open("parameters.dat",ios::in | ios::nocreate);
   if (!infile)
   	theMessage->FatalError("Cannot find parameters.dat");
   char buffer[500];
   infile.getline(buffer,500);
   infile.getline(buffer,500);
   infile >> microRecyclingCoeff >> microYieldCoeff >> MicrobialCtoNRatio >> ConsolidatedCtoNRatio
   			>> ammoniaConst >> refNitrificationRate >> waterProdConst >> airflowConst >> refDenitrificationRate;
   infile.getline(buffer,500);
}


