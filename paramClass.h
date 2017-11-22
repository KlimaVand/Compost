// Revised by JB 2/2-99

#ifndef paramClass_H  //Required for current class
  #define paramClass_H

#include "common.h"

class paramClass
{
   protected:
//parameters
   double microYieldCoeff;
   double microRecyclingCoeff;
   double MicrobialCtoNRatio;
   double ConsolidatedCtoNRatio;
   double refNitrificationRate;
   double ammoniaConst;
   double waterProdConst;
   double airflowConst;
   double refDenitrificationRate;

	public:

  		paramClass ();

  		//Destructor
  		~paramClass () {};

      double GetmicroYieldCoeff() const {return microYieldCoeff;};
		double GetmicroRecyclingCoeff() const {return microRecyclingCoeff;};
		double GetMicrobialCtoNRatio() const {return MicrobialCtoNRatio;};
		double GetConsolidatedCtoNRatio() const {return ConsolidatedCtoNRatio;};
		double GetrefNitrificationRate() const {return refNitrificationRate;};
   	double GetwaterProdConst() const {return waterProdConst;};
   	double GetammoniaConst() const {return ammoniaConst;};
	   double GetairflowConst() const {return airflowConst;};
		double GetrefDenitrificationRate()const {return refDenitrificationRate;};

      void GetParameters();
};

#endif

