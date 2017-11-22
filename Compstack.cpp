#include "compstack.h"
#include "common.h"
#include "fraction.h"
#include "substanceDB.h"
#include "substrateItem.h"
#include "message.h"
#include "NixCbits.h"
#include "pi.h"

// extern pi;

compostStack::compostStack ()
{
	temperature		= 0.0;
   diameter     	= 0.0;
   length  			= 0.0;
   surface_area 	= 0.0;
   aerobicDepth	= 0.0;
   temperatureChange = 0.0;
   Gr					= 0.0;
   shape 			= 0.0;
	aerobic			= NULL;
   anaerobic		= NULL;
   theFractions	= new cloneList<stackFraction>;
}

compostStack::~compostStack ()
{
	delete aerobic;
   delete anaerobic;
	theFractions->ForgetAll();
	delete theFractions;
   delete metData;
   delete theparams;
}

/****************************************************************************\
\****************************************************************************/
double compostStack::GetstackMass()
{
	double ret_val=0.0;
	for(int i=0;i<theFractions->NumOfNodes();i++)
	{
    stackFraction * afraction=theFractions->ElementAtNumber(i);
    ret_val+=afraction->Getmass();
   }
   return ret_val;
}

/****************************************************************************\
\****************************************************************************/
double compostStack::GetstackVolume(stackFraction *anAerobic, stackFraction *anAnaerobic)
{
 double ret_val;
 ret_val = anAerobic->CalcVolume();
 if (anAnaerobic)
   ret_val+=anAnaerobic->CalcVolume();
 return ret_val;
}

/****************************************************************************\
\****************************************************************************/
double compostStack::GetstackHeatCapacity()
{
	double ret_val;
   ret_val=aerobic->GetHeatCapacity();
   if (anaerobic)
	   ret_val+=anaerobic->GetHeatCapacity();
   return ret_val;
}

/****************************************************************************\
\****************************************************************************/
double compostStack::GetStackDiameter(double aVolume)
{
 double ret_val;
 double a = (PI/12.0);     //cubic
 double b = PI*length/8.0;    //quadratic
 double c = 0.0;           //linear
 double d = -aVolume;       //constant
 int solutions=0;
 double x[3];            //returned values
 SolveCubic(a,b,c,d,&solutions,x);
 if (solutions==1)
	 ret_val=x[0];
 else
	 ret_val=x[1];
 return ret_val;
// double test = a*pow(x[0],3)+b*pow(x[0],2)+d;
// cout << test << " " << x[1] ;
}

/****************************************************************************\
\****************************************************************************/
double compostStack::GetStackHeight(double volume)
{
	double ret_val = volume/length;
   return ret_val;
}
/****************************************************************************\
\****************************************************************************/
double compostStack::GetStackLength(double aVolume, double aDiameter)
{
 double ret_val;
 ret_val = 8*(aVolume - PI*pow(aDiameter,3)/12.0)/(PI*pow(aDiameter,2));
 return ret_val;
}

/****************************************************************************\
\****************************************************************************/
double compostStack::GetVolumeAnaerobic(double aDiameter, double anAerobicDepth)
{
	double ret_val = 0.0;
   if (aDiameter>anAerobicDepth)
			   ret_val=PI*((2*pow(((aDiameter/2)-anAerobicDepth),3)/3)
   						 + length*pow(((aDiameter/2)-anAerobicDepth)/3,2)/2);
   return ret_val;
};

/****************************************************************************\
\****************************************************************************/
double compostStack::GetAeratedCrossSectionalArea()
{
	double ret_val;
   if (shape == 1)
	   ret_val=PI*(pow((diameter/2),2)-pow(((diameter/2)-aerobicDepth),2))
   						 + (2*length*aerobicDepth);
   else
      ret_val=length * length;
   return ret_val;
};

/****************************************************************************\
\****************************************************************************/
double compostStack::GetStackArea()
{
	double ret_val;
   if (shape==1)
	   ret_val=(PI*diameter/2)*(diameter + length);
   else
   	ret_val = length * length;
   return ret_val;
};

void compostStack::CheckBalances()
{
   aerobic->CheckBalances();
   if (anaerobic)
   	   anaerobic->CheckBalances();
};
/****************************************************************************\
\****************************************************************************/
bool compostStack::Initialise(substrateDB *thesubstrateDB)
{
//get management info
//	int shape;
	fstream manfile;
   manfile.open("compman.dat",ios::in | ios::nocreate);
   if (!manfile)
   	theMessage->FatalError("Cannot find compman.dat");
   char buffer[500];
   manfile.getline(buffer,500);
   double aRadius;
   double aLength;
   manfile >> aRadius >> aLength >> shape;
   manfile.getline(buffer,500);
   manfile.getline(buffer,500);
	if ((aRadius>0.0)&& (aLength>0.0) && (shape==1))
   	theMessage->FatalError("Both diameter and length cannot be fixed");
   int turnNumber;
   int aNum1,aNum2,aNum3;   //needed because for some reason, direct reading does not work
   do    // read list of feed used
   {
    turnNumber=0;
    manfile >> turnNumber >> aNum1 >> aNum2>> aNum3;
    turningDate[turnNumber-1] = aNum1; turningHour[turnNumber-1]  = aNum2; irrigationAmount[turnNumber-1] = aNum3;
    manfile.getline(buffer,500);
   }while (turnNumber!=0);
   manfile.close();

//get substrate database
   substanceDB *asubstanceDB = new substanceDB("SubstanceDB",0,NULL);
	asubstanceDB->Initialise("basic.dat");

//get environmental info
	metData = new compMetClass();
   metData->openFile("metdata.dat");
   metData->readHourlyData();
   theparams = new paramClass();
   theparams->GetParameters();

//get details of substrate amounts added
	fstream infile;
   infile.open("compost.dat",ios::in | ios::nocreate);
   if (!infile)
   	theMessage->FatalError("Cannot find compost.dat");
   infile.getline(buffer,500);
   substrateItem *bulkSubstrate = new substrateItem();
   int code;
   double cumulativeMass=0.0;
   double amount=0.0;
   do    // read list of feed used
   {
	 code = 0;
    infile >> code >> amount;
    infile.getline(buffer,500);
    if (code>0)
    {
      substrateItem *asubstrateItem = thesubstrateDB->findSubstrate(code);
     	double proportion =amount/(amount + cumulativeMass);
           //load into a container
     	bulkSubstrate->Add(asubstrateItem,proportion);
      cumulativeMass+=amount;
    }
   } while (infile);
	infile.close();
	stackFraction *temp = new stackFraction("temp",0,NULL);
   temperature=metData->GetairTemperature();
   temp->Settemperature(temperature);
   temp->Initialise(asubstanceDB,bulkSubstrate,cumulativeMass,
   						theparams, metData);
   if ((aLength>0.0)|| ((aLength==0.0)&&(aRadius==0.0)))
   {
   	length=aLength;
      fixLength=true;
   }
   else
   {
   	fixLength=false;
      diameter = 2 * aRadius;
   }
   StartCompStack(temp);
   delete temp;
	aerobic->Setstartup(true);
   delete asubstanceDB;
   delete bulkSubstrate;
   if (aerobicDepth>0.0)
   	return false;
   else
   	return true;
}

/****************************************************************************\
\****************************************************************************/
void compostStack::StartCompStack(stackFraction *aFraction)
{
   volume = GetstackVolume(aFraction,NULL);
   if (fixLength)
   {
   	if (shape == 1)
		   diameter = GetStackDiameter(volume);
      else
      	height = GetStackHeight(volume);
   }
   else
   	length = GetStackLength(volume, diameter);
   if (length<0.0)
   	theMessage->FatalError("Radius too big - length is less than zero");

	if (shape == 1)
   {
   	aerobicDepth=aFraction->GetAeratedDepth();
      if (aerobicDepth>=diameter)
        aerobicDepth=diameter;
   }
   else
   	aerobicDepth=length;
   double volumeAnaerobic = 0.0;
   double proportionAerobic = 1.0;
	if (shape == 1)
   {
   	volumeAnaerobic=GetVolumeAnaerobic(diameter,aerobicDepth);
	   if ((diameter/2)>aerobicDepth)
   		proportionAerobic=(volume-volumeAnaerobic)/volume;
   }
	stackFraction *temp = new stackFraction(*aFraction);
   temp->CreateProportion(proportionAerobic);
	aerobic = new stackFraction(*aFraction);
   aerobic->ZeroVariables();
   *aerobic + *temp;                 // do it this way to ensure budgets are correct
   delete temp;
   aerobic->Setdepth(aerobicDepth);
   aerobic->CheckBalances();
   aerobic->CalcVolume();
	double newSensible=aerobic->GetSensibleEnergy()*1E6;
   theFractions->InsertLast(aerobic);
   if (proportionAerobic<1.0)
   {
      temp = new stackFraction(*aFraction);
      temp->CreateProportion(1-proportionAerobic);
		anaerobic = new stackFraction(*aFraction);
   	anaerobic->ZeroVariables();
      *anaerobic + *temp;                 // do it this way to ensure budgets are correct
      delete temp;
	   theFractions->InsertLast(anaerobic);
      anaerobic->CalcVolume();
		anaerobic->CheckBalances();
      newSensible+=anaerobic->GetSensibleEnergy()*1E6;
   }
   temperature =(newSensible/GetstackHeatCapacity())-273.0;
}

/****************************************************************************\
\****************************************************************************/
void compostStack::DoMix(double anIrrigationAmount)
{
 stackFraction *temp = new stackFraction(*aerobic);
 if (anaerobic)
 {
	 *temp + *anaerobic;
	 delete anaerobic;
 }
 delete aerobic;
 if (anIrrigationAmount>0.0)
 {
	 stackFraction *irrigationWater = new stackFraction(*aerobic);
    irrigationWater->ZeroVariables();
 	 irrigationWater->Getwater()->Setmass(anIrrigationAmount);
    irrigationWater->Settemperature(metData->GetairTemperature());
	 *temp + *irrigationWater;
    delete irrigationWater;
 }

 StartCompStack(temp);
 delete temp;
}
/****************************************************************************\
\****************************************************************************/
bool compostStack::CheckMix(double theTime)
{
 bool ret_val =false;
 for (int i=0;i<20;i++)
 {
 	int hours =turningDate[i]*24 + turningHour[i];
   if ((theTime > hours)&& (hours>0)&&(!doneTurn[i]))
   {
	   DoMix(irrigationAmount[i]);
      aerationMultiplier=15.0;
   	doneTurn[i]=true;
      ret_val =true;
   }
 }
 return ret_val;
}
/****************************************************************************\
\****************************************************************************/
bool compostStack::TimeRoutine(double *timeStep, bool *abort)
{
 double maxTimeStep;
 if (((temperature>70.0)||(temperatureChange/(*timeStep)>5.0)) && ((temperatureChange/(*timeStep)<10.0)))
 	maxTimeStep=0.1;
 else if (temperatureChange/(*timeStep)>=10.0)
 	maxTimeStep=0.1;
    else
 	  maxTimeStep=1.0;
 if (*timeStep>maxTimeStep)
 		*timeStep=maxTimeStep;

 if (anaerobic)
	 anaerobic->DoDenitrification(timeStep);
 aerobic->Settemperature(temperature);
 double aerobicHt = 0.0;
 if (shape == 1)
 	aerobicHt=PI*((diameter/2) - (aerobic->Getdepth()/2))/2;
 else
	aerobicHt = height;
 double VolumeAerationRate= aerobic->CalcAerationRate(GetAeratedCrossSectionalArea(),temperature, aerobicHt, &Gr);
 double massFlowRate=aerationMultiplier * aerobic->CalcFlowRate(VolumeAerationRate);
 stackFraction *changeInAerobic = new stackFraction(*aerobic);
 changeInAerobic->Initialise();
 aerobic->CheckBalances();
 aerobic->Dynamics(1,changeInAerobic, timeStep);
 double stackArea = GetStackArea();
 double energyLoss;
 double energyChange = aerobic->ThermalDynamics(changeInAerobic, massFlowRate,
 						stackArea, &energyLoss, timeStep);
 double oldSensible=aerobic->GetSensibleEnergy();
 aerobic->AdjustFraction(changeInAerobic);
 double newSensible=aerobic->GetSensibleEnergy();
 double changesensibleHeat = 1E6*(oldSensible - newSensible) + energyChange;
 delete changeInAerobic;

 temperatureChange =changesensibleHeat/GetstackHeatCapacity();

 double newVolume = GetstackVolume(aerobic,anaerobic);
 volume = newVolume;
 if (shape == 1)
 {
	 aerobicDepth=aerobic->GetAeratedDepth();
    double newDiameter = GetStackDiameter(newVolume);
    diameter = newDiameter;
    if (aerobicDepth>=(diameter/2))
      aerobicDepth=diameter/2;
    if (aerobicDepth == 0.0)
      *abort = true;
    aerobic->Setdepth(aerobicDepth);
    double proportionToTransfer;
    if (anaerobic)
    {
       double newVolumeAnaerobic=GetVolumeAnaerobic(newDiameter,aerobicDepth);
       double currentVolumeAnaerobic=anaerobic->GetVolume();
       proportionToTransfer=(currentVolumeAnaerobic-newVolumeAnaerobic)
                                       /currentVolumeAnaerobic;

      if (proportionToTransfer>0.0)
      {
         volumeAnaerobic = newVolumeAnaerobic;
         anaerobic->CalcVolume();
         stackFraction *transferToAerobic = new stackFraction(*anaerobic);
         transferToAerobic->CreateProportion(proportionToTransfer);
         *anaerobic - *transferToAerobic;
         *aerobic + *transferToAerobic;
         delete transferToAerobic;
      }
      if (proportionToTransfer==1.0)
      {
         delete anaerobic;
         anaerobic = NULL;
      }
	 };
 }
 else
 	height = GetStackHeight(volume);
 temperature +=temperatureChange;
 cout << " temp " << temperature << endl;
 double propEnergyToAerobic = aerobic->GetHeatCapacity()/GetstackHeatCapacity();
 aerobic->Settemperature(temperature);
 if (anaerobic)
 {
	 anaerobic->Settemperature(temperature);
 	 anaerobic->ChangeEnergyBudget((1-propEnergyToAerobic) * changesensibleHeat/1E6);
 	 aerobic->ChangeEnergyBudget(-(1-propEnergyToAerobic) * changesensibleHeat/1E6);
	 anaerobic->CheckBalances();
 }
 aerobic->CheckBalances();
 aerationMultiplier=1.0;
 if (fabs(temperature-metData->GetairTemperature())<0.1)
	 return true;
 else
	 return false;
}


/****************************************************************************\
\****************************************************************************/
double compostStack::GetdryBulkDensity()
{
 double ret_val=aerobic->GetdryBulkDensity();
 if (anaerobic)
  ret_val+=anaerobic->GetdryBulkDensity();
 return ret_val;
};

/****************************************************************************\
\****************************************************************************/

void compostStack::WriteStack(fstream& os, bool header)
{
 metData->WriteMet(os,header);
 if(header)
 {
  os << setiosflags(ios::left) << " temperature "  << " \t";
  os << setiosflags(ios::left) << " volume " << " \t";
  os << setiosflags(ios::left) << " volumeAnaerobic " << " \t";
  os << setiosflags(ios::left) << " diameter " << " \t";
  os << setiosflags(ios::left) << " aerobicDepth " << " \t";
  os << setiosflags(ios::left) << " surface_area " << " \t";
  os << setiosflags(ios::left) << " AerateArea " << " \t";
  os << setiosflags(ios::left) << " Grashof " << " \t";
  }
  else
  {
  os << setiosflags(ios::left) << " " << Gettemperature() << " \t";
  os << setiosflags(ios::left) << " " << Getvolume() << " \t";
  os << setiosflags(ios::left) << " " << volumeAnaerobic << " \t";
  os << setiosflags(ios::left) << " " << Getdiameter() << " \t";
  os << setiosflags(ios::left) << " " << aerobic->Getdepth() << " \t";
  os << setiosflags(ios::left) << " " << GetStackArea() << " \t";
  os << setiosflags(ios::left) << " " << GetAeratedCrossSectionalArea() << " \t";
  os << setiosflags(ios::left) << " " << Gr << " \t";

  }
  aerobic->WriteFraction(os,header);
  if (anaerobic)
	  anaerobic->WriteFraction(os,header);;
  os << endl;
}


