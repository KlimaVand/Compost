#include "common.h"
#include "compstack.h"
#include "substrateDB.h"
#include "bstime.h"
#include "message.h"
#include <fcntl.h>

bsTime  theTime;
message *theMessage = new message();

main()
{
   chdir("c://home//project//canada//compost//model");
   substrateDB *asubstrateDB = new substrateDB();
	asubstrateDB->Initialise("substrateDB2.txt");
   compostStack *theStack = new compostStack();
   bool abort = false;
   abort = theStack->Initialise(asubstrateDB);
	fstream *outfile = new fstream("comout.txt",ios::out);
   *outfile  << setw(15) << setprecision(6);
   *outfile << "time" << " \t";
   theStack->WriteStack(*outfile,true);
   *outfile << "0.0 "<< " \t";
   theStack->WriteStack(*outfile,false);
   double maxHour = 10 * 7 * 24;
//	maxHour = 2376.0;
   double hour = 0.0;
   double timeStep;
   int revCounter=0;
   while ((hour<maxHour)&& (!abort))
   {
     theStack->ReadMet();
     double oldHour=hour;
     timeStep=1.0;
     while ((hour-oldHour)<1.0)
     {
        if (theStack->CheckMix(hour))
        	timeStep=0.1;
        theStack->TimeRoutine(&timeStep, &abort);
        hour+=timeStep;
        revCounter++;
        if (revCounter>maxHour*10)
         abort=true;
        cout << hour << endl;
        if (hour>250.0)
        cout << " ";
		  *outfile  << setw(15);
        *outfile << setprecision(10) << hour << " \t";
        theStack->WriteStack(*outfile,false);
        timeStep=hour-oldHour;
     }
   }
   outfile->close();
   theStack->CheckBalances();
   delete theStack;
   delete asubstrateDB;
   delete outfile;
	delete theMessage;
}
