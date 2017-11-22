/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#ifndef budget_H
   #define budget_H
#include <string.h>
#include <stdlib.h>
#include <string>
#include <stdlib>
class budget
{
   private:
		double input;
		double output;
      double lastError;
      bool monitor; // Remove after debugging !!!
      string ClassName;
      string BudgetName;

	public:
  		budget ();
      //Copy constructor
  		budget (const budget& abudget);
      //Assignment operator
    	budget& operator=(const budget& f);
  		//Destructor
  		~budget () {}

   	//Get accessor function for attributes
   	double GetInput() const {return input;}
   	double GetOutput() const {return output;}
      string GetClassName() const {return ClassName;}
      string GetBudgetName() const {return BudgetName;}

   	//Set accessor function for attributes
   	void SetInput (double aInput);
   	void SetOutput (double aOutput);
   	void SetNames(string aClassName, string aBudgetName);
   	void SetMonitor (bool m) {monitor=m;};

   	void AddInput (double aInput);
   	void AddOutput (double aOutput);

   	bool Balance (double Remain);
      void Reset ();

   	budget * clone() const;
   	void Add(budget* aBudget, double fraction);
   	void Add(budget  aBudget, double fraction);

		friend ostream& operator<< (ostream& os,const budget& s);
};

#endif

