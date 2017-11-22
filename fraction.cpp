#include "fraction.h"
#include "common.h"
#include "message.h"
#include "nixcbits.h"
#include "OMpool.h"

const double pH = 7.8;

stackFraction::stackFraction ()
:base()
{
   theComponents = new cloneList<substance>;
   water=new substance();
   theComponents->InsertLast(water); //store in cloneList
	nonVolatile=new substance();
   theComponents->InsertLast(nonVolatile);
	readilyDecomp=new OMpool();
   theComponents->InsertLast(readilyDecomp);
	resistant=new OMpool();
   theComponents->InsertLast(resistant);
	consolidated=new OMpool();
   theComponents->InsertLast(consolidated);
	ZeroVariables();
}

stackFraction::~stackFraction ()
{
   delete water;
   delete nonVolatile ;
	delete readilyDecomp;
   delete resistant;
   delete consolidated;
   theComponents->ForgetAll();
   delete theComponents;
}

/****************************************************************************\

\****************************************************************************/
stackFraction::stackFraction(char *aName, const int aIndex, const base * aOwner)
	:base(aName,aIndex,aOwner)
{
	ZeroVariables();
   theComponents = new cloneList<substance>;
}

void stackFraction::ZeroVariables()
{
	volume      = 0;
   CO2Closs = 0.0;
   aerationRate = 0.0;
   rateDenitrification = 0.0;
	nitrateN = 0.0;
	ammoniumN=0.0;
   temperature = 0.0;
   initialResistant=0.0;
   if (water)
		water->Setmass(0.0);
   if (nonVolatile)
		nonVolatile->Setmass(0.0);
   if (readilyDecomp)
   {
		readilyDecomp->SetC(0.0);
		readilyDecomp->SetN(0.0);
   }
   if (resistant)
   {
		resistant->SetC(0.0);
		resistant->SetN(0.0);
   }
   if (consolidated)
   {
		consolidated->SetC(0.0);
		consolidated->SetN(0.0);
   }
   startup=false;
	Cbudget.SetNames("stackFraction","C");
   Cbudget.Reset();
	NVSbudget.SetNames("stackFraction","NVS");
   NVSbudget.Reset();
	Waterbudget.SetNames("stackFraction","Water");
   Waterbudget.Reset();
   Nbudget.SetNames("stackFraction","N");
   Nbudget.Reset();
   Energybudget.SetNames("stackFraction","Energy");
   Energybudget.Reset();
}

/****************************************************************************\
\****************************************************************************/
stackFraction::stackFraction(const stackFraction& astackFraction)
//:base(astackFraction)
{
	volume      = astackFraction.volume;
   CO2Closs = astackFraction.CO2Closs;
   aerationRate = astackFraction.aerationRate;
   nitrateN = astackFraction.nitrateN;
   ammoniumN = astackFraction.ammoniumN;
   temperature = astackFraction.temperature;
   theComponents = new cloneList<substance>;
   water=new substance(*astackFraction.water);
   theComponents->InsertLast(water); //store in cloneList
	nonVolatile=new substance(*astackFraction.nonVolatile);
   theComponents->InsertLast(nonVolatile);
	readilyDecomp=new OMpool(*astackFraction.readilyDecomp);
   theComponents->InsertLast(readilyDecomp);
	resistant=new OMpool(*astackFraction.resistant);
   theComponents->InsertLast(resistant);
	consolidated=new OMpool(*astackFraction.consolidated);
   theComponents->InsertLast(consolidated);
   theparams = astackFraction.theparams;
	metdata =  astackFraction.metdata;
	startup=  astackFraction.startup;
   Cbudget =  astackFraction.Cbudget;
   NVSbudget =  astackFraction.NVSbudget;
   Waterbudget =  astackFraction.Waterbudget;
   Nbudget =  astackFraction.Nbudget;
   Energybudget =  astackFraction.Energybudget;
	temperature= astackFraction.temperature;
   initialResistant=astackFraction.initialResistant;
}

/****************************************************************************\
\****************************************************************************/
stackFraction* stackFraction::clone()
{
	stackFraction* p = new stackFraction(*this);
	return p;
}

/****************************************************************************\
\****************************************************************************/
stackFraction& stackFraction::operator-(const stackFraction &aFraction)
{
	for(int i=0;i<theComponents->NumOfNodes();i++)
	{
    substance * asubstance=theComponents->ElementAtNumber(i);
    substance * anotherSubstance=aFraction.GettheComponents()->ElementAtNumber(i);
    *asubstance - *anotherSubstance;
   }
   nitrateN -= aFraction.GetnitrateN();
   ammoniumN -= aFraction.GetammoniumN();
   initialResistant -= aFraction.GetinitialResistant();
   Cbudget.AddOutput(aFraction.GetmassC());
   Nbudget.AddOutput(aFraction.GetmassN());
   NVSbudget.AddOutput(aFraction.GetnonVolatile()->Getmass());
   Waterbudget.AddOutput(aFraction.Getwater()->Getmass());
   double energy = aFraction.GetTotalEnergy();
   Energybudget.AddOutput(energy);
	return *this;
}

/****************************************************************************\
\****************************************************************************/
stackFraction& stackFraction::operator+(const stackFraction &aFraction)
{
   double newSensibleEnergy = GetSensibleEnergy() + aFraction.GetSensibleEnergy();
	for(int i=0;i<theComponents->NumOfNodes();i++)
	{
    substance * asubstance=theComponents->ElementAtNumber(i);
    substance * anotherSubstance=aFraction.GettheComponents()->ElementAtNumber(i);
    *asubstance + *anotherSubstance;
   }
   nitrateN += aFraction.GetnitrateN();
   ammoniumN += aFraction.GetammoniumN();
   initialResistant+= aFraction.initialResistant;
   Cbudget.AddInput(aFraction.GetmassC());
   Nbudget.AddInput(aFraction.GetmassN());
   NVSbudget.AddInput(aFraction.GetnonVolatile()->Getmass());
   Waterbudget.AddInput(aFraction.Getwater()->Getmass());
   double energy = aFraction.GetTotalEnergy();
   Energybudget.AddInput(energy);
   temperature = (1E6 * newSensibleEnergy/GetHeatCapacity())- 273.0;
	return *this;
}

void stackFraction::AdjustFraction(stackFraction * aFraction)
{
   double originalMass = Getmass();
	for(int i=0;i<theComponents->NumOfNodes();i++)
	{
    substance * asubstance=theComponents->ElementAtNumber(i);
    substance * anotherSubstance=aFraction->GettheComponents()->ElementAtNumber(i);
    *asubstance + *anotherSubstance;
   }
   nitrateN += aFraction->GetnitrateN();
   ammoniumN += aFraction->GetammoniumN();
   initialResistant+= aFraction->initialResistant;

   Cbudget.AddInput(aFraction->GetmassC());
   Nbudget.AddInput(aFraction->GetmassN());
   NVSbudget.AddInput(aFraction->GetnonVolatile()->Getmass());
   Waterbudget.AddInput(aFraction->Getwater()->Getmass());
}

void stackFraction::Initialise(substanceDB *asubstanceDB, substrateItem * anItem, double amount,
      				paramClass *params, compMetClass * themetdata)
{
	theparams = params;
   metdata = themetdata;
   water = new substance(*asubstanceDB->FindSubstance("water"));
   water->Setmass(amount * anItem->GetwaterConc());  //initialise mass from substrate item
   theComponents->InsertLast(water); //store in cloneList
   Waterbudget.AddInput(water->Getmass());

   nonVolatile = new substance(*asubstanceDB->FindSubstance("nvs"));
   nonVolatile->Setmass(amount * anItem->GetnonVolatileConc());
   theComponents->InsertLast(nonVolatile);
   NVSbudget.AddInput(nonVolatile->Getmass());

   readilyDecomp = new OMpool(*(OMpool*)asubstanceDB->FindSubstance("readilyDecomp"));
	readilyDecomp->SetC(amount * anItem->GetreadilyDecompConc() *GetCconcInOrgmatter()); //GetCconcInOrgmatter() C in all OM
   Cbudget.AddInput(readilyDecomp->GetC());
   readilyDecomp->SetN(readilyDecomp->GetC()/anItem->GetreadilyCtoN());
   Nbudget.AddInput(readilyDecomp->GetN());
   theComponents->InsertLast(readilyDecomp);

   resistant = new OMpool(*(OMpool*)asubstanceDB->FindSubstance("resistant"));
   resistant->SetC(amount * anItem->GetresistantConc()*GetCconcInOrgmatter()); //GetCconcInOrgmatter() C in all OM
   Cbudget.AddInput(resistant->GetC());
   resistant->SetN(resistant->GetC()/anItem->GetresistantCtoN());
   Nbudget.AddInput(resistant->GetN());
   theComponents->InsertLast(resistant);
   initialResistant=resistant->GetC();

   consolidated = new OMpool(*(OMpool*)asubstanceDB->FindSubstance("consolidated"));
   consolidated->Setmass(0.0);
   theComponents->InsertLast(consolidated);

   nitrateN=amount * anItem->GetnitrateConc();
	ammoniumN=amount * anItem->GetammoniumConc();
   Nbudget.AddInput(nitrateN + ammoniumN);

   Energybudget.AddInput(GetTotalEnergy());
   CalcVolume();
}

void stackFraction::Initialise()
{
   water->Setmass(0.0);
   nonVolatile->Setmass(0.0);
	readilyDecomp->SetC(0.0);
   readilyDecomp->SetN(0.0);
   resistant->SetC(0.0);
   resistant->SetN(0.0);
   consolidated->SetC(0.0);
   consolidated->SetN(0.0);
   nitrateN=0.0;
	ammoniumN=0.0;
   initialResistant = 0.0;
}

/************************************
Get mass of fraction (kg)
************************************/
double stackFraction::Getmass() const
{
	double ret_val=0.0;
	for(int i=0;i<theComponents->NumOfNodes();i++)
	{
    substance * asubstance=theComponents->ElementAtNumber(i);
    ret_val+=asubstance->Getmass();
   }
   return ret_val;
}

/************************************
Get mass of C in fraction (kg)
************************************/
double stackFraction::GetmassC() const
{
	double ret_val=resistant->GetC() + readilyDecomp->GetC() + consolidated->GetC();
   return ret_val;
}

/************************************
Get mass of N in fraction (kg)
************************************/
double stackFraction::GetmassN()  const
{
	double ret_val=resistant->GetN() + readilyDecomp->GetN() + consolidated->GetN();
   ret_val+=ammoniumN + nitrateN;
   return ret_val;
}

/************************************
Get total energy in fraction (MJ)
************************************/
double stackFraction::GetTotalEnergy() const
{
	double ret_val;
   double chemicalEnergy=GetChemicalEnergy();  //potential combustion
	double sensibleHeat=GetSensibleEnergy();;
   ret_val= chemicalEnergy + sensibleHeat;
   return ret_val;
};

/************************************
Chemical energy in organic matter  in MJ
************************************/
double stackFraction::GetChemicalEnergy()  const
{
	double ret_val=readilyDecomp->GetC()*readilyDecomp->GetheatOfCombustion()
   		+ resistant->GetC()*resistant->GetheatOfCombustion()
         + consolidated->GetC() * consolidated->GetheatOfCombustion();
   return ret_val;
};

/************************************
Get latent heat energy  in MJ
************************************/
double stackFraction::GetLatentEnergy()
{
   double latentHeat=water->Getmass() * GetLatentHeatVaporisationWater(40.0) * 1000.0/1E6; //(temperature);
   return latentHeat;
};

/************************************
Get sensible heat energy in MJ
************************************/
double stackFraction::GetSensibleEnergy() const
{
	double sensibleHeat=0.0;
  	for(int i=0;i<theComponents->NumOfNodes();i++)
	{
    substance * asubstance=theComponents->ElementAtNumber(i);
    sensibleHeat+=(temperature + 273) * asubstance->Getmass()
    		* asubstance->Getspecific_heat()/1E6;
   }
   return sensibleHeat;
};

/************************************
Use to change budget due to changes in sensible heats
************************************/
void stackFraction::ChangeEnergyBudget(double aValue)
{
	Energybudget.AddInput(aValue);
};

/************************************
Get dry bulk density of fraction (kg per cubic metre)
************************************/
double stackFraction::GetdryBulkDensity()
{
	double ret_val;
   double massSolids  = GetmassSolids();
   ret_val = massSolids/volume;
   return ret_val;
}

/************************************
Get mean specific gravity of solids in substrate in fraction ()
************************************/
double stackFraction::GetmeanSGsolids()
{
 double meanSGOMsolids =	(readilyDecomp->Getmass() * readilyDecomp->Getspecific_gravity()
   				+ resistant->Getmass() * resistant->Getspecific_gravity()
               + consolidated->Getmass() * consolidated->Getspecific_gravity())/GetmassOM();
 return meanSGOMsolids;
}

/************************************
Get porosity of substrate in fraction
************************************/
double stackFraction::Getporosity()
{
	double ret_val;
   double meanSGOMsolids =	GetmeanSGsolids();
   ret_val = 1-GetdryBulkDensity()/(1000.0 * meanSGOMsolids);
   return ret_val;
}

/************************************
Get proportion of dry matter in fraction (kg per cubic metre)
************************************/
double stackFraction::GetpropDM()
{
 double mass_water = water->Getmass();
 double mass_solids =GetmassSolids();
 double ret_val = mass_solids/(mass_solids + mass_water);
 return ret_val;
}
/************************************
Get wet bulk density of fraction (kg per cubic metre)
************************************/
double stackFraction::GetwetBulkDensity()
{
	double ret_val;
   if (GetFreeAirSpace()==0)
   {
   	double DM=GetpropDM();
   	ret_val = (1-DM) + GetmeanSGsolids() * DM;
   }
   else
   	ret_val = Getmass()/volume;
//   	ret_val = GetdryBulkDensity() * (1 + GetmoistureContdb());
   return ret_val;
}

/************************************
************************************/
double stackFraction::GetvolumetricWaterContent()
{
//	double ret_val = GetmoistureContdb() * GetdryBulkDensity()/1000.0;
	double ret_val = GetmoistureContdb();
   ret_val *= GetdryBulkDensity()/1000.0;
   return ret_val;
}

/************************************
Get free air space of fraction ()
************************************/
double stackFraction::GetFreeAirSpace()
{
	double ret_val;
   ret_val = Getporosity()-GetvolumetricWaterContent();
//   cout << "Porosity " << Getporosity() << " Vol water " << GetvolumetricWaterContent() << endl;
   if (ret_val<0.0)
   	ret_val = 0.0;
   return ret_val;
}

/************************************
************************************/
double stackFraction::GetmassSolids()
{
	return GetmassOM() + nonVolatile->Getmass();
}

/************************************
************************************/
double stackFraction::GetmassNVS()
{
	return nonVolatile->Getmass();
}
/************************************
Get specific gravity of fraction ()
************************************/
double stackFraction::GetSpecificGravity()
{
	double ret_val;
   double massOM=GetmassOM();
   double massSolids=GetmassSolids();
   ret_val = massOM/(massSolids * GetspecificGravityOM())
					+ (1-(massOM/massSolids))/nonVolatile->Getspecific_gravity();
   ret_val=1/ret_val;
   return ret_val;
}


/************************************
Get specific gravity of OM in fraction ()
************************************/
double stackFraction::GetspecificGravityOM()
{
	double ret_val=(readilyDecomp->Getmass()*readilyDecomp->Getspecific_gravity()
   		+ resistant->Getmass()*resistant->Getspecific_gravity()
         + consolidated->Getmass() * consolidated->Getspecific_gravity())
         /GetmassOM();
   return ret_val;
}

/************************************
 moisture content on dry weight basis
************************************/
double stackFraction::GetmoistureContdb()
{
 double mass_water = water->Getmass();
 double mass_solids =GetmassSolids();
 double ret_val = mass_water/mass_solids;
 return ret_val;
}
/************************************
Get mass of organic matter in fraction (kg)
************************************/
double stackFraction::GetmassOM()
{
	double ret_val=readilyDecomp->Getmass() + resistant->Getmass() + consolidated->Getmass();
   return ret_val;
}

/************************************
Get heat capacity of fraction kJ/C
************************************/
double stackFraction::GetHeatCapacity()
{
	double ret_val=0.0;
	for(int i=0;i<theComponents->NumOfNodes();i++)
	{
    substance * asubstance=theComponents->ElementAtNumber(i);
    ret_val+=asubstance->Getmass() * asubstance->Getspecific_heat();
   }
   return ret_val;
};


/************************************
Get depth of aerobic layer (m)
************************************/
double stackFraction::GetAeratedDepth()
{
	double ret_val;
   double FAS = GetFreeAirSpace();
   ret_val = 1.0 * pow(FAS,1.5);
   return ret_val;
};


/************************************
Get mass of air passing per hour (kg)
************************************/
double stackFraction::CalcFlowRate(double VolumeAerationRate)
{
 aerationRate=VolumeAerationRate * GetdensityAir(metdata->GetatmosphericPress(),
	 metdata->GetairTemperature(),metdata->GetwaterVapourPressure());
 return aerationRate;
}

double stackFraction::CalcVolume()
{
	volume=0.0;
	double val=0.0;
// 	val=nonVolatile->Getmass()/nonVolatile->GetdryBulkDensity();
//   if (val>volume)
  // 	volume=val;
 	val=readilyDecomp->Getmass()/readilyDecomp->GetdryBulkDensity();
   if (val>volume)
   	volume=val;
 	val=consolidated->Getmass()/consolidated->GetdryBulkDensity();
   if (val>volume)
   	volume=val;

   double km=0.95;
   double propDegResistant = 1-resistant->GetC()/initialResistant;
//   double drybdresist=resistant->GetdryBulkDensity() + (consolidated->GetdryBulkDensity() - resistant->GetdryBulkDensity())
  // 					 *propDegResistant /(propDegResistant+km);
   double drybdresist=resistant->GetdryBulkDensity() + propDegResistant * (consolidated->GetdryBulkDensity() - resistant->GetdryBulkDensity());
 	val=resistant->Getmass()/drybdresist;
   if (val>volume)
   	volume=val;
      cout << "vol " << volume << "deg " << propDegResistant << "drybd " << drybdresist << endl;
	return volume;
}

/************************************
Get cubic metres of air passing per hour
************************************/
double stackFraction::CalcAerationRate(double aeratedCrossSectionalArea,
				double airTempOut, double aerobicHt, double *Grashof)
{
	double ret_val;
   double FAS=GetFreeAirSpace();
   double airTempIn=metdata->GetairTemperature();
   double waterVapourPressIn=metdata->GetwaterVapourPressure();
   double airDensityIn =GetdensityAir(metdata->GetatmosphericPress(),airTempIn,waterVapourPressIn);
   double waterVapourPressOut=GetsaturatedWaterVapourPressure(airTempOut);
   double airDensityOut =GetdensityAir(metdata->GetatmosphericPress(),airTempOut,waterVapourPressOut);
   airTempOut+=273;
   airTempIn+=273;
   double temperature_diff = airTempOut-airTempIn;
//   double aerobicHt=PI*(radius-depth/2)/2;
   *Grashof =  (aerobicHt * FAS *pow(airDensityIn,2)* GetgravitationalAcceleration()*temperature_diff/((airTempOut+airTempIn)/2))
   							/pow(GetdynamicViscosityAir(),2);
   *Grashof *=1E-10;
   double airbag = theparams->GetairflowConst();

   ret_val = airbag * (*Grashof)/1000.0;
   cout << ret_val << endl;
   ret_val*=aeratedCrossSectionalArea;
   if (ret_val<0.0)
   	ret_val=0.0;
   else
   	ret_val*=3600;
   return ret_val;
}

/****************************************************************************\
\****************************************************************************/
double stackFraction::GetTemperatureFunc()
{
	double ret_val = pow(1.066,(temperature-20)) - pow(1.21,(temperature-60));
   if (ret_val<0.0)
   	ret_val=0.0;
   return ret_val;
}

/****************************************************************************\
\****************************************************************************/
double stackFraction::GetThermalConductivity()  //from van Ginkel et al (2002) Biosyst Engineering v81, p113-125
{
	double ret_val = 0.1 + 0.0375 * GetvolumetricWaterContent()/(1-Getporosity());
//   ret_val =
   return ret_val;
}

/****************************************************************************\
\****************************************************************************/
double stackFraction::GetMoistureFunc()
{
	double ret_val = 1/(exp(7.0622-17.3648*water->Getmass()/Getmass()) +1.0);
   if (ret_val<0.0)
   	ret_val=0.0;
   return ret_val;
}

/****************************************************************************\
\****************************************************************************/
double stackFraction::GetNitrificationTemperatureFunc()
{
	double ret_val;
   if ((temperature<=0.0)||(temperature>40.0))
   ret_val=0.0;
   if ((temperature>0.0)&&(temperature<32.5))
   ret_val=temperature/32.5;
   if ((temperature>32.5)&&(temperature<40.0))
   ret_val=(40.0-temperature)/(40.0-32.5);
   return ret_val;
}


double stackFraction::GetNitrificationRate()
{
 double thenitrificationRate= theparams->GetrefNitrificationRate() * GetMoistureFunc()
			* GetNitrificationTemperatureFunc() * ammoniumN;
 if (GetO2SupplyRate()<=O2demandRate)
 	thenitrificationRate=0.0;
 else
 {
 	double NitrificO2DemandRate = thenitrificationRate * 3.0;  //3 molecules O2 per molecule N?
	if ((GetO2SupplyRate()- O2demandRate)<NitrificO2DemandRate)
   	thenitrificationRate*=(GetO2SupplyRate()- O2demandRate)/NitrificO2DemandRate;
 }
 return thenitrificationRate;
}

/********************************************************
Only occurs in anaerobic pool
********************************************************/
void  stackFraction::DoDenitrification(double *timeStep)
{
 mineralisation=0.0;
 rateNitrification=0.0;
 rateDenitrification = 0.0;
 if (nitrateN>0.0)
 {
/* 	 rateDenitrification = theparams->GetrefDenitrificationRate() * GetMoistureFunc()
			* GetNitrificationTemperatureFunc() * nitrateN;                       //assume same moisture and temperature dependency of denitrification as nitrification
    deNitrificLoss=*timeStep * rateDenitrification;
    if (nitrateN >= deNitrificLoss)
    {
      nitrateN-= deNitrificLoss;
    }
    else
    {
      deNitrificLoss=nitrateN;
      SetnitrateN(0.0);
    }
    */
    deNitrificLoss=nitrateN;
    nitrateN=0.0;
    rateDenitrification=deNitrificLoss;
    Nbudget.AddOutput(deNitrificLoss);
 }
}


/****************************************************************************\
\****************************************************************************/
double stackFraction::GetAmmoniaLossRate()
{
 double lossRate;//= (temperature) * ammoniumN/(GetKHenry(temperature)
 	//	* GetDissociationConst(temperature));
  double KN =GetKHenry(temperature);
  double Kh=exp(160.559-(8621.06/(temperature+273)) - 25.6767*log(temperature+273)+0.035388*(temperature+273));
  double R=0.0000820575;
  double surfaceNH3Factor=Kh/(R*(temperature+273)*(1+pow(10,-pH)/KN));
  lossRate = theparams->GetammoniaConst() * surfaceNH3Factor * ammoniumN/water->Getmass();
//   cout << temp << " " << KN;

 return lossRate;
}
/****************************************************************************\
Returns net long wave heat loss from stack (J per sec)
\****************************************************************************/
double stackFraction::GetSurfaceHeatLoss(double stackArea)
{
	double ret_val = stackArea * (GetlongWaveUp(1,(temperature+273))
   						- GetlongWaveUp(1,(-10+273)));
   return ret_val;
}
/****************************************************************************\
\****************************************************************************/
void stackFraction::CalcMineralisation(double degrateReadilyDecomp,
				double *degrateResistant, double *timestep)
{
 double rateNdegraded = degrateReadilyDecomp * readilyDecomp->GetN()
 					+  *degrateResistant * resistant->GetN();
 double rateNewMicrobialC = theparams->GetmicroYieldCoeff()
 		* (degrateReadilyDecomp * readilyDecomp->GetC()
    	+ (*degrateResistant) * resistant->GetC());
 double rateNimmobilised= rateNewMicrobialC * ((theparams->GetmicroRecyclingCoeff()/theparams->GetMicrobialCtoNRatio())
      +  (1-theparams->GetmicroRecyclingCoeff())/theparams->GetConsolidatedCtoNRatio());

 mineralisation = rateNdegraded - rateNimmobilised;
 if (mineralisation<0.0)
 {
 	if ((ammoniumN + nitrateN) == 0.0)
   {
   	double X = (theparams->GetmicroRecyclingCoeff()/theparams->GetMicrobialCtoNRatio()) +
     					(1-theparams->GetmicroRecyclingCoeff())/theparams->GetConsolidatedCtoNRatio();
      double numerator = degrateReadilyDecomp * readilyDecomp->GetC()
         * (theparams->GetmicroYieldCoeff() *  X
         - 1/(readilyDecomp->GetC()/readilyDecomp->GetN()));
      double denominator = resistant->GetC() * (1/(resistant->GetC()/resistant->GetN())
         				- theparams->GetmicroYieldCoeff() * X);
         *degrateResistant=numerator/denominator;
   }
  	else if (ammoniumN > 0.0)
   {
      double rateNitrification = GetNitrificationRate();
      double rateNH3Loss = GetAmmoniaLossRate();
      double rateChangeAmmonium = mineralisation - (rateNitrification+ rateNH3Loss);
      if (fabs(rateChangeAmmonium) > ammoniumN)   //demand for ammonium would deplete pool within tisme step
      {
         double newTimeStep=-ammoniumN/rateChangeAmmonium;
         if (newTimeStep<*timestep)
             *timestep = newTimeStep;
      }
   }
  	else if (nitrateN < fabs(mineralisation)) //demand for nitrate would deplete pool within timestep
      {
         double newTimeStep=nitrateN/fabs(mineralisation);
         if (newTimeStep<*timestep)
             *timestep = newTimeStep;
      }
 }
}

/****************************************************************************\
\****************************************************************************/
void stackFraction::KickstartO2Demand(double CO2ProdRate, double *timeStep)
{
	O2demandRate = 1.5 * CO2ProdRate/GetCconcInOrgmatter();   //convert to DM and multiply by O2 demand per unit mass
   double O2Supply = GetVolume() * GetFreeAirSpace() * 0.21 * 32.0/29.0;
   if (O2Supply<(*timeStep * O2demandRate))
      *timeStep= O2Supply/O2demandRate;
}

/****************************************************************************\
\****************************************************************************/
double stackFraction::GetO2SupplyRate()
{
   double O2SupplyRate = aerationRate * 0.21 * 32.0/29.0;
   return O2SupplyRate;
}

/****************************************************************************\
\****************************************************************************/
double stackFraction::CalcO2Demand(double CO2ProdRate)
{
	double ret_val=1.0;
	O2demandRate = 1.5 * CO2ProdRate/GetCconcInOrgmatter();   //convert to DM and multiply by O2 demand per unit mass
   if (GetO2SupplyRate()<O2demandRate)
      ret_val= GetO2SupplyRate()/O2demandRate;
   return ret_val;
}

/****************************************************************************\
\****************************************************************************/
void stackFraction::UpdateBudgets()
{
 Cbudget.AddOutput(CO2Closs);
 Nbudget.AddOutput(NH3Nloss);
 Waterbudget.AddOutput(ventWaterLoss);
 Waterbudget.AddInput(biolWaterGen);
 Energybudget.AddOutput(energyChange);
}

/****************************************************************************\
Handle C dynamics. Note that this function stores changes in a copy of the stackFraction object,
so that state variables remain constant until the end of computation
\****************************************************************************/
void stackFraction::CarbonDynamics(stackFraction *aFraction,
				 double *timeStep, double *degrateReadilyDecomp, double *degrateResistant)
{
   moistureFunc = GetMoistureFunc();
   temperatureFunc = GetTemperatureFunc();
   double changeConsolidatedC;
   double changeReadilyDecompC;
   double changeResistantC;
   double degradedReadilyDecompC;
   double degradedResistantC;
	double CO2CProdRate;
   double O2function;
   *degrateReadilyDecomp=moistureFunc * temperatureFunc *	readilyDecomp->GetrefDecompRate();
   *degrateResistant=moistureFunc * temperatureFunc *	resistant->GetrefDecompRate();
   CO2CProdRate= (1-theparams->GetmicroYieldCoeff())*(*degrateReadilyDecomp * readilyDecomp->GetC()
                        + (*degrateResistant) * resistant->GetC());
   if (startup)
   {
      KickstartO2Demand(CO2CProdRate, timeStep);
      O2function=1.0;
   }
   else
      O2function=CalcO2Demand(CO2CProdRate);
   if (O2function<1.0)
   {
      *degrateReadilyDecomp*=O2function;
      *degrateResistant*=O2function;
      CO2CProdRate*=O2function;
   }
   CalcMineralisation(*degrateReadilyDecomp, degrateResistant, timeStep);
   degradedReadilyDecompC=*degrateReadilyDecomp * readilyDecomp->GetC() *(*timeStep);
   degradedResistantC=*degrateResistant * resistant->GetC() *(*timeStep);
   changeResistantC=-degradedResistantC;
   changeReadilyDecompC=-degradedReadilyDecompC + theparams->GetmicroYieldCoeff() *
                   theparams->GetmicroRecyclingCoeff()* (degradedReadilyDecompC + degradedResistantC);
   changeConsolidatedC = theparams->GetmicroYieldCoeff() * (1 - theparams->GetmicroRecyclingCoeff()) *
                           (degradedReadilyDecompC + degradedResistantC);

   CO2Closs=CO2CProdRate*(*timeStep);
   aFraction->GetreadilyDecomp()->SetC(changeReadilyDecompC);
   aFraction->Getresistant()->SetC(changeResistantC);
   aFraction->Getconsolidated()->SetC(changeConsolidatedC);
}

/****************************************************************************\
\****************************************************************************/
void stackFraction::NitrogenDynamics(stackFraction *aFraction, double *timeStep,
	 double degrateReadilyDecomp, double degrateResistant)
{
 double rateNewMicrobialC = theparams->GetmicroYieldCoeff()
      * (degrateReadilyDecomp * readilyDecomp->GetC()
    	+ degrateResistant * resistant->GetC());
 double rateChangeReadilyDecompN=-degrateReadilyDecomp * readilyDecomp->GetN()
 	 + theparams->GetmicroRecyclingCoeff()* rateNewMicrobialC/theparams->GetMicrobialCtoNRatio();
 double ratechangeResistantN=-degrateResistant * resistant->GetN();
 double rateChangeConsolidatedN=(1-theparams->GetmicroRecyclingCoeff()) * rateNewMicrobialC
 								/theparams->GetConsolidatedCtoNRatio();
 double rateChangeAmmonium = 0.0;
 double rateChangeNitrate = 0.0;
 double rateNH3Loss = 0.0;
 double newTimeStep = *timeStep;
 if (ammoniumN>0.0)
 {
 	rateNitrification = GetNitrificationRate();
   rateNH3Loss = GetAmmoniaLossRate();
   rateChangeAmmonium = mineralisation - (rateNitrification+ rateNH3Loss);
   if ((*timeStep)* rateChangeAmmonium + ammoniumN<0.0)
   {
    	newTimeStep=-ammoniumN/rateChangeAmmonium;
    	if (newTimeStep<*timeStep)
          *timeStep = newTimeStep;
   }
 }
 else if (mineralisation>0.0)
 		rateChangeAmmonium = mineralisation;

 if ((nitrateN>0.0) && (mineralisation < 0.0)&&(ammoniumN==0.0))   //immobilisation from nitrate
 {
  rateChangeNitrate=mineralisation;  //mineralisation is negative here
  if ((*timeStep)* rateChangeNitrate + nitrateN<0.0)
  {
    	newTimeStep=-nitrateN/rateChangeNitrate;
    	if (newTimeStep<*timeStep)
          *timeStep = newTimeStep;
  }
 }
 else
 	rateChangeNitrate= rateNitrification;
 aFraction->GetreadilyDecomp()->SetN(*timeStep * rateChangeReadilyDecompN);
 aFraction->Getresistant()->SetN(*timeStep * ratechangeResistantN);
 aFraction->Getconsolidated()->SetN(*timeStep * rateChangeConsolidatedN);
 aFraction->SetnitrateN(*timeStep * rateChangeNitrate);
 aFraction->SetammoniumN(*timeStep * rateChangeAmmonium);
 NH3Nloss = *timeStep * rateNH3Loss;
}

/****************************************************************************\
\****************************************************************************/
double stackFraction::ThermalDynamics(stackFraction *aFraction, double massFlowRate,
					double stackArea, double *energyLost, double *timeStep)
{
//Calculate energy generation by decomposition
 double readilyCDecomposed = -aFraction->GetreadilyDecomp()->GetC();
 double resistantCDecomposed = -aFraction->Getresistant()->GetC();
 double consolidatedCreated = aFraction->Getconsolidated()->GetC();
 biologicalEnergy=readilyCDecomposed * 1E6 * readilyDecomp->GetheatOfCombustion();
 biologicalEnergy+=resistantCDecomposed * 1E6 * resistant->GetheatOfCombustion();
 biologicalEnergy-=consolidatedCreated * 1E6 * consolidated->GetheatOfCombustion();
 // Calculate energy exchange in ventilation air
 double airPressure = metdata->GetatmosphericPress();
 double waterVapourPressure = metdata->GetwaterVapourPressure();
 double incomingSpecHumidity= GetSpecificHumidity(airPressure, waterVapourPressure);
 waterVapourPressure = GetsaturatedWaterVapourPressure(temperature);
 double outgoingSpecHumidity= GetSpecificHumidity(airPressure, waterVapourPressure);
//  outgoingSpecHumidity*= GetMoistureFunc();
//  if (outgoingSpecHumidity<incomingSpecHumidity)
  //		outgoingSpecHumidity=incomingSpecHumidity;
 double sensibleHeatWater = *timeStep * massFlowRate * water->Getspecific_heat()
 			*(outgoingSpecHumidity*(temperature + 273.0)
         - incomingSpecHumidity * (metdata->GetairTemperature()+273.0));
 double deltaHumidity = outgoingSpecHumidity - incomingSpecHumidity;
 double specHeatAir = GetspecificHeatCapAir();
 sensibleHeatAir = (*timeStep) * massFlowRate * specHeatAir
 				* (temperature - metdata->GetairTemperature());

 // Calculate heat loss through surface
 surfaceHeatLoss = 3600.0 * (*timeStep) * stackArea * GetThermalConductivity() //theparams->GetthermaTransConst() *
				  *	(temperature - metdata->GetairTemperature())/(depth/2);
  if (surfaceHeatLoss<0.0)
  cout << "" ;

 // do water dynamics
 double OMdegraded = (readilyCDecomposed + resistantCDecomposed)/GetCconcInOrgmatter();
 WaterDynamics(deltaHumidity, massFlowRate, OMdegraded,
 										aFraction, timeStep);

 latentHeatLoss = ventWaterLoss * GetLatentHeatVaporisationWater(40.0)  * 1000.0; //(temperature)

 *energyLost = sensibleHeatWater + sensibleHeatAir + latentHeatLoss + surfaceHeatLoss;
 energyChange = biologicalEnergy - *energyLost;
 Energybudget.AddOutput(*energyLost/1E6);
 return energyChange;
}

/****************************************************************************\
\****************************************************************************/
double stackFraction::WaterDynamics(double deltaHumidity, double massFlowRate,
		double OMdegraded, stackFraction *aFraction, double *timeStep)
{
 ventWaterLoss = *timeStep * massFlowRate* deltaHumidity;
 biolWaterGen = OMdegraded * (1-theparams->GetmicroYieldCoeff())* theparams->GetwaterProdConst();
 double changeWater = biolWaterGen - ventWaterLoss;
 aFraction->Getwater()->Setmass(changeWater);
 return biolWaterGen;
}
/****************************************************************************\
\****************************************************************************/
void stackFraction::Dynamics(int pool, stackFraction *aFraction, double *timeStep)
{
 switch (pool) {
 	case 1:double degrateReadilyDecomp;
          double degrateResistant;
          double newTimestep;
          do
          {
             newTimestep=*timeStep;
             CarbonDynamics(aFraction, timeStep,&degrateReadilyDecomp,
             		&degrateResistant);
             NitrogenDynamics(aFraction, timeStep, degrateReadilyDecomp, degrateResistant);
          } while (newTimestep!=*timeStep);
          aFraction->GetnonVolatile()->Setmass(0.0);
          break;
 	case 2:break;
   {
//	 double
   }
   default:theMessage->FatalError("Pool number >2");

   }
 if ((startup) && (temperature>metdata->GetairTemperature()+5))
    startup=false;
};

void stackFraction::Update()
{

}
/****************************************************************************\
\****************************************************************************/
void stackFraction::CreateProportion(double aProportion)
{
	for(int i=0;i<theComponents->NumOfNodes();i++)
	{
    substance * asubstance=theComponents->ElementAtNumber(i);
    asubstance->CreateProportion(aProportion);
   }
   ammoniumN*=aProportion;
   nitrateN*=aProportion;
   initialResistant*=aProportion;
}

/****************************************************************************\
\****************************************************************************/

void stackFraction::WriteFraction(fstream& os, bool header)
{
 if (header)
 {
  os << setiosflags(ios::left) << " CO2Closs " << " \t" ;
  os << setiosflags(ios::left) << " aerationRate "<< " \t";
  os << setiosflags(ios::left) << " FAS "<< " \t";
  os << setiosflags(ios::left) << " nitrateN "<< " \t";
  os << setiosflags(ios::left) << " ammoniumN "<< " \t";
  os << setiosflags(ios::left) << " ammoniaVolat "<< " \t";
  os << setiosflags(ios::left) << " mineralisation " << " \t";
  os << setiosflags(ios::left) << " rateNitrification " << " \t";
  os << setiosflags(ios::left) << " rateDenitrification " << " \t";
  os << setiosflags(ios::left) << " ventWaterLoss "<< " \t";
  os << setiosflags(ios::left) << " biolWaterGen "<< " \t";
  os << setiosflags(ios::left) << " biologicalEnergy "<< " \t";
  os << setiosflags(ios::left) << " sensibleHeatAir "<< " \t";
  os << setiosflags(ios::left) << " latentHeatLoss "<< " \t";
  os << setiosflags(ios::left) << " surfaceHeatLoss "<< " \t";
  os << setiosflags(ios::left) << " radiativeHeatLoss "<< " \t";
  os << setiosflags(ios::left) << " temperatureFunc "<< " \t";
  os << setiosflags(ios::left) << " moistureFunc "<< " \t";
 }
 else
 {
  os << setiosflags(ios::left) << " " << GetCO2Closs() << " \t";
  os << setiosflags(ios::left) << " " << GetaerationRate() << " \t";
  os << setiosflags(ios::left) << " " << GetFreeAirSpace() << " \t";
  os << setiosflags(ios::left) << " " << GetnitrateN() << " \t";
  os << setiosflags(ios::left) << " " << GetammoniumN() << " \t";
  os << setiosflags(ios::left) << " " << GetNH3Nloss() << " \t";
  os << setiosflags(ios::left) << " " << mineralisation << " \t";
  os << setiosflags(ios::left) << " " << rateNitrification << " \t";
  os << setiosflags(ios::left) << " " << rateDenitrification << " \t";
  os << setiosflags(ios::left) << " " << ventWaterLoss << " \t";
  os << setiosflags(ios::left) << " " << biolWaterGen << " \t";
  os << setiosflags(ios::left) << " " << biologicalEnergy << " \t";
  os << setiosflags(ios::left) << " " << sensibleHeatAir << " \t";
  os << setiosflags(ios::left) << " " << latentHeatLoss << " \t";
  os << setiosflags(ios::left) << " " << surfaceHeatLoss << " \t";
  os << setiosflags(ios::left) << " " << radiativeHeatLoss << " \t";
  os << setiosflags(ios::left) << " " << temperatureFunc << " \t";
  os << setiosflags(ios::left) << " " << moistureFunc << " \t";
 }
 water->WriteSubstance(os,header);
 nonVolatile->WriteSubstance(os,header);
 readilyDecomp->WriteSubstance(os,header);
 resistant->WriteSubstance(os,header);
 consolidated->WriteSubstance(os,header);
}

/****************************************************************************\
\****************************************************************************/
void stackFraction::CheckBalances()
{
	double rem;
   rem=GetmassC();
   Cbudget.Balance(rem);
   rem=GetmassN();
   Nbudget.Balance(rem);
   rem=GetWaterMass();
   Waterbudget.Balance(rem);
   rem=GetmassNVS();
   NVSbudget.Balance(rem);
   rem=GetTotalEnergy();
   Energybudget.Balance(rem);
   if (nitrateN<1E-8)
   	nitrateN=0.0;
   if (ammoniumN<1E-8)
   	ammoniumN=0.0;
}

