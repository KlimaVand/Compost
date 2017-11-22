/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#ifndef CLONLIST_H
   #define CLONLIST_H

template <class T>
class cloneList
{
protected:
	struct Snode
   {
		T * element;
		Snode * next;
	};
public:
	typedef Snode * PS;
protected:
	PS Head,Tail;
	int numOfNodes;
	void MakeFirstNode(PS &P,T * E);
public:
   void CheckHeadAndTail(char * c);
	cloneList();
	~cloneList();
	void PeekHead(PS &P);
	void PeekTail(PS &P);
	int NumOfNodes();
	void OneStep(PS &P);
	void Remove(PS &R);
	void Release(PS &R);
	void InsertBefore(PS &S,PS &P,T * E);
	void InsertAfter(PS &S,PS &P,T * E);
	void InsertLast(T * E);
   void InsertFirst(T * E);
	void Reset();
	void ForEach(void (*funcp)(T*));
   double AddForEach(double (*funcp)(T*));
   void FirstThat(PS &FT,int (*funcp)(T*));
	void LastThat(PS &FT,int (*funcp)(T*));
	void SearchByNumber(PS &FT,int num);
   T* ElementAtNumber(int num);
   cloneList<T>* clone() const;
   void ForgetAll();
};

#ifndef __ANSICPP__
   #pragma option -Jgx
#endif

template<class T> void cloneList<T>::CheckHeadAndTail(char * c)
{
   if (NumOfNodes()>0 && Head==Tail)
   {
      cout << c << " violated Head or Tail pointer";
      cout << endl;
   }
}

template<class T> cloneList<T>::cloneList()
{
	Head=NULL;
	Tail=NULL;
	numOfNodes=0;
}

template<class T> cloneList<T>::~cloneList()
{
	Reset();
}

template<class T> void cloneList<T>::MakeFirstNode(PS &P,T * E)
{
	P=new Snode;
	P->next=NULL;
	P->element=E;
	Head=P;
	Tail=P;
	numOfNodes++;
}

template<class T> void cloneList<T>::PeekHead(PS &P)
{
	P=Head;
}

template<class T> void cloneList<T>::PeekTail(PS &P)
{
	P=Tail;
}

template<class T> int cloneList<T>::NumOfNodes()
{
	return numOfNodes;
}

// This function is quite dangerous if S is invalid !!!
template<class T> void cloneList<T>::InsertAfter(PS &S,PS &P,T * E)
{
	if (Head==NULL)
		MakeFirstNode(P,E);
	else
	{
      if (NULL==S)
      {
         cout << "cloneList<T>::InsertAfter - function called with NULL, element inserted as last!" << endl;
         S=Tail;
      }
		P=new Snode;
		if (S==Tail)
			Tail=P;
		P->next=S->next;
		S->next=P;
		P->element=E;
		numOfNodes++;
	}
}

template<class T> void cloneList<T>::InsertLast(T * E)
{
	PS S1,S2;
	PeekTail(S1);
	InsertAfter(S1,S2,E);
}

template<class T> void cloneList<T>::InsertFirst(T * E)
{
	PS S1,S2;
	PeekHead(S1);
	InsertBefore(S1,S2,E);
}

template<class T> void cloneList<T>::OneStep(PS &P)
{
	if (Head==NULL)
		P=NULL;
	else
      if (P!=NULL)
	   	P=P->next;
}

// Ved afslutning peger R p� elementet efter det slettede.
template<class T> void cloneList<T>::Remove(PS &R)
{
	PS P=Head;
	if ((R==Head) && (Head!=NULL))
   { // R peger p� "Head"
      if (R==Tail)
         Tail=NULL;
		Head=R->next;
		delete R->element;
		delete R;
		numOfNodes--;
		R=Head;
	}
	else
   {
		while ((P!=NULL) && (R!=P->next))
			P=P->next;
		if (P!=NULL)
      {
         if (R==Tail)
            Tail=P;
			P->next=P->next->next;
			delete R->element;
			delete R;
			numOfNodes--;
			R=P->next;
		} // Ellers er knuden ikke fundet
	}
}

// Ved afslutning peger R p� elementet efter det flyttede.
template<class T> void cloneList<T>::Release(PS &R)
{
	PS P=Head;
	if ((R==Head) && (Head!=NULL))
   { // R peger p� "Head"
      if (R==Tail)
         Tail=NULL;
		Head=R->next;
		delete R;
		numOfNodes--;
		R=Head;
	}
	else
   {
		while ((P!=NULL) && (R!=P->next))
			P=P->next;
		if (P!=NULL)
      {
         if (R==Tail)
            Tail=P;
			P->next=P->next->next;
			delete R;
			numOfNodes--;
			R=P->next;
		} // Ellers er knuden ikke fundet
	}
}

template<class T> void cloneList<T>::InsertBefore(PS &S,PS &P,T * E)
{
	if (Head==NULL)
		MakeFirstNode(P,E);
	else
   {
      if (NULL==S)
      {
         cout << "cloneList<T>::InsertBefore - function called with NULL, element inserted as first!" << endl;
         S=Head;
      }
		if (Head==S)
      {
			P=new Snode;
			P->next=Head;
			Head=P;
			P->element=E;
			numOfNodes++;
		}
		else
      {
			PS PT=Head;
			while ((PT!=NULL) && (S!=PT->next))
				PT=PT->next;
			if (PT!=NULL)
         {
				P=new Snode;
				P->next=PT->next;
				PT->next=P;
				P->element=E;
				numOfNodes++;
			}
         else
            cout << "cloneList<T>::InsertBefore - function called with invalid pointer, element not inserted!" << endl;
		}
   }
}

template<class T> void cloneList<T>::Reset()
{
	PS P1;
	PS P=Head;
	while (P!=NULL)
   {
		P1=P->next;
		delete P->element;
		delete P;
		P=P1;
	}
	Head=NULL;
	Tail=NULL;
	numOfNodes=0;
}

template<class T> void cloneList<T>::ForEach(void (*funcp)(T*))
{
	PS P=Head;
	while (P!=NULL)
   {
		funcp(P->element);
		P=P->next;
	}
}

template<class T> double cloneList<T>::AddForEach(double (*funcp)(T*))
{
   double sum = 0.0;
	PS P=Head;
	while (P!=NULL)
   {
		sum += funcp(P->element);
		P=P->next;
	}
   return sum;
}

// Returnerer NULL n�r ikke fundet
template<class T> void cloneList<T>::FirstThat(PS &FT,int (*funcp)(T*))
{
	PS P=Head;
	while ((P!=NULL) && (!funcp(P->element)))
		P=P->next;
	FT=P;
}

// Returnerer NULL n�r ikke fundet
template<class T> void cloneList<T>::LastThat(PS &FT,int (*funcp)(T*))
{
	FT=NULL;
	PS P=Head;
	while (P!=NULL)
   {
		if (funcp(P->element))
			FT=P;
		P=P->next;
	}
}

// Returnerer NULL n�r ikke fundet.
// F�rste element har nummer 0.
template<class T> void cloneList<T>::SearchByNumber(PS &FT,int num)
{
	FT=Head;
	int i=0;
	while ((FT!=NULL) && (i<num))
   {
		FT=FT->next;
		i++;
	}
}

template<class T> T* cloneList<T>::ElementAtNumber(int num)
{
	PS FT=Head;
	int i=0;
	while ((FT!=NULL) && (i<num))
   {
		FT=FT->next;
		i++;
	}
      return FT->element;
}

template<class T> cloneList<T>* cloneList<T>::clone() const
{
	cloneList* aCloneList = new cloneList;
   PS P=Head;
   while (P!=NULL)
   {
      T * e=P->element->clone();
      aCloneList->InsertLast(e);
      P=P->next;
   }
	return aCloneList;
}

template<class T> void cloneList<T>::ForgetAll()
{
	PS P1;
	PS P=Head;
	while (P!=NULL)
   {
		P1=P->next;
		//delete P->element;
		delete P;
		P=P1;
	}
	Head=NULL;
	Tail=NULL;
	numOfNodes=0;
}

#ifndef __ANSICPP__
   #ifndef BUILDER
      #pragma option -Jg
   #endif
#endif

#endif

