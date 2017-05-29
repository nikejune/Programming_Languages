// PL homework: hw1
// fsa.cc

#include <iostream>

#include "fsa.h"
#include <stdio.h>
#define DISABLE_LOG true
#define LOG \
    if (DISABLE_LOG) {} else std::cerr

using namespace std;

bool RunFSA(const FiniteStateAutomaton& fsa, const string& str) {
   // Implement this function.

    set<int> tmpStates;

    set<int>::iterator iter1, iter2, iter_Si;
    tmpStates = fsa.start_states;

    int i, j;
    bool next =false;

    for(i = 0; i < str.length(); i++)
    {

        next = false;
        for(j = 0; j < fsa.table.size(); j++)
        {
            if((tmpStates == fsa.table[j].states) && (str.at(i) == fsa.table[j].input_char))
            {
                tmpStates = fsa.table[j].next_states;
                next =true;
                break;
            }

        }
   
        if(!next)
            return false;
    }


    for(iter1 = fsa.accept_states.begin(); iter1 != fsa.accept_states.end(); iter1++)
    {
        for(iter2 = tmpStates.begin(); iter2 != tmpStates.end(); iter2++)
        {
            if(*iter1 == *iter2)
                return true;
        }
    }
    
  return false;
   
}

bool BuildFSA(const std::vector<FSATableElement>& elements,
              const std::vector<int>& accept_states,
              FiniteStateAutomaton* fsa) {
    
   LOG << "num_elements: " << elements.size()
       << ", accept_states: " << accept_states.size() << endl;

   set<int>::iterator iter_Si;
   set<char>::iterator iter_Sc;
   pair< set<int>::iterator, bool> ret;
   pair< set< set<int> >::iterator ,bool> ret2;
   set < set<int> > subset;
   set < set<int> >::iterator iter_SSi, iter_SSi2;
   set < set<int> > subset2;

   bool check =false;
   bool isNew = true;
   TableSet tempTS;


   // Find start states and Add to fsa->start_states
   fsa->start_states.insert(elements[0].state);
   for(int i = 0; i < elements.size(); i++)
   {
       for(iter_Si = fsa->start_states.begin(); iter_Si != fsa->start_states.end(); iter_Si++)
       {
           // # && elements[i].state == start_states
           if(elements[i].str.length() == 0 && elements[i].state == *iter_Si)
           {   
               ret = fsa->start_states.insert(elements[i].next_state);
               if(ret.second)
               {
                   i = 0;
                   iter_Si = fsa->start_states.begin();
               }
           }

       }
   }
    /*  
   for(iter_Si = fsa->start_states.begin(); iter_Si != fsa->start_states.end(); iter_Si++)
   {
       cout << *iter_Si << endl;
   }
     */


   // Add accept_states into fsa->accept_states 
   for(int i = 0; i < accept_states.size(); i++)
   {
       fsa->accept_states.insert(accept_states[i]);
   }

   /*
   for(iter_Si = fsa->accept_states.begin(); iter_Si != fsa->accept_states.end(); iter_Si++)
   {
       cout << *iter_Si << endl;
   }*/

  
   // Add all input_chars into fsa->input_chars
   for(int i = 0; i < elements.size(); i++)
   {
       for(int j = 0; j < elements[i].str.length(); j++)
       {
           fsa->input_chars.insert(elements[i].str.at(j));
       }
   }

   /*
   for(iter_Sc = fsa->input_chars.begin(); iter_Sc != fsa->input_chars.end(); iter_Sc++)
   {
       cout << *iter_Sc << endl;
   }
   */
/*
   // Add all states into fsa->all_states
   for(int i = 0; i < elements.size(); i++)
   {
       fsa->all_states.insert(elements[i].state);
       fsa->all_states.insert(elements[i].next_state);
   }
*/
   /*
   for(iter_Si = fsa->all_states.begin(); iter_Si != fsa->all_states.end(); iter_Si++)
   {
       cout << *iter_Si << endl;
   }

    */

  /* 
    
   for(iter_Si = tempTS.states.begin(); iter_Si != tempTS.states.end(); iter_Si++)
   {
       cout << *iter_Si << endl;
   }

   for(iter_SSi = subset.begin(); iter_SSi !=  subset.end(); iter_SSi++)
   {  
       for(iter_Si = iter_SSi->begin(); iter_Si != iter_SSi->end(); iter_Si++)
       {
           cout << *iter_Si << endl;
       }
   }

   */
   tempTS.states = fsa->start_states;
   subset.insert(fsa->start_states);


   while(isNew)
   {
       
       isNew = false;
  
    //   cout << "while intro " << endl;
       // seearch next_states in elements[i].next_state
       for(iter_Sc = fsa->input_chars.begin(); iter_Sc != fsa->input_chars.end(); iter_Sc++)
       {

       //    cout << "for iter_Sc intro " << endl;
           for(iter_Si = tempTS.states.begin(); iter_Si != tempTS.states.end(); iter_Si++)
           {
               for(int i = 0; i < elements.size(); i++)
               {
                   for(int j = 0; j < elements[i].str.length(); j++)
                   {
                       if(*iter_Si == elements[i].state && *iter_Sc == elements[i].str.at(j))
                       {
                           tempTS.next_states.insert(elements[i].next_state);

                       }

                   }
               }

           }


           // add temp.next_states to #
           for(int i = 0; i < elements.size(); i++)
           {
               for(iter_Si = tempTS.next_states.begin(); iter_Si != tempTS.next_states.end(); iter_Si++)
               {
                   // # && elements[i].state == tempTS_states
                   if(elements[i].str.length() == 0 && elements[i].state == *iter_Si)
                   {   
                       ret = tempTS.next_states.insert(elements[i].next_state);
                       if(ret.second)
                       {
                           i = 0;
                           iter_Si = tempTS.next_states.begin();
                       }
                   }
               }
           }

           tempTS.input_char = *iter_Sc;
           fsa->table.push_back(tempTS);
/*
           for(iter_SSi = subset.begin(); iter_SSi != subset.end(); iter_SSi++)
            {
                    if(*iter_SSi == tempTS.next_states)
                    {
                        check = true;
                        break;
                    }
                    else
                    {
                        check = false;
                    }
        
            }
  */      
           ret2 = subset.insert(tempTS.next_states);
           
           tempTS.next_states.clear();
           if(ret2.second){
               isNew =true;
           }
    //   cout << "for iter_Sc  outro" <<endl;
       }
       subset2.insert(tempTS.states);
       if(subset == subset2)
           break;
       for(iter_SSi = subset.begin(); iter_SSi !=subset.end(); iter_SSi++)
       {
           bool check2 = false;
           for(iter_SSi2 = subset2.begin(); iter_SSi2 != subset2.end() ; iter_SSi2++)
           {
               if(*iter_SSi == *iter_SSi2)
                   check2 = true;
           }
           if(!check2)
           {
               tempTS.states = *iter_SSi;
               break;
           }
       }

   //    cout << "while outro" <<endl;
   }

/* table all state, next state, input char search 
   for(int i = 0; i < fsa->table.size(); i++)
   {
       cout << "states ";
       for(iter_Si = fsa->table[i].states.begin(); iter_Si != fsa->table[i].states.end(); iter_Si++)
       {
           cout << *iter_Si << endl;
       }
       cout << "next_states ";

       for(iter_Si = fsa->table[i].next_states.begin(); iter_Si != fsa->table[i].next_states.end(); iter_Si++)
       {
           cout << *iter_Si << endl;
       }
       cout << "input_char "<< fsa->table[i].input_char << endl;
   }
*/

/*   tempTS.states.clear();
   tempTS.states.insert(1);
   tempTS.states.insert(2);
   tempTS.states.insert(3);
  subset2.insert(tempTS.states);

  for(iter_Si = tempTS.states.begin(); iter_Si != tempTS.states.end() ; iter_Si++)
  {
      cout <<"tempTS" <<*iter_Si << endl;
  }

        cout << "+=============================" <<endl;
        cout << "+=============================" <<endl;
  for(iter_SSi = subset2.begin(); iter_SSi != subset2.end(); iter_SSi++)
    {
        for(iter_Si = iter_SSi->begin(); iter_Si != iter_SSi->end() ; iter_Si++)
        {
            cout <<"subset2" <<*iter_Si << endl;
        }
        
        cout << "+=============================" <<endl;
    }


        cout << "+=============================" <<endl;
        cout << "+=============================" <<endl;
   tempTS.states.clear();
   tempTS.states.insert(1);
   tempTS.states.insert(3);
  subset2.insert(tempTS.states);

  for(iter_Si = tempTS.states.begin(); iter_Si != tempTS.states.end() ; iter_Si++)
  {
      cout <<"tempTS" <<*iter_Si << endl;
  }
        cout << "+=============================" <<endl;
        cout << "+=============================" <<endl;

  for(iter_SSi = subset2.begin(); iter_SSi != subset2.end(); iter_SSi++)
    {
        for(iter_Si = iter_SSi->begin(); iter_Si != iter_SSi->end() ; iter_Si++)
        {
            cout <<"subset2" <<*iter_Si << endl;
        }
        
        cout << "+=============================" <<endl;
    }


        cout << "+=============================" <<endl;
        cout << "+=============================" <<endl;

   tempTS.states.clear();
   tempTS.states.insert(2);
   tempTS.states.insert(3);
  subset2.insert(tempTS.states);

  for(iter_Si = tempTS.states.begin(); iter_Si != tempTS.states.end() ; iter_Si++)
  {
      cout <<"tempTS" <<*iter_Si << endl;
  }

        cout << "+=============================" <<endl;
        cout << "+=============================" <<endl;
  for(iter_SSi = subset2.begin(); iter_SSi != subset2.end(); iter_SSi++)
    {
        for(iter_Si = iter_SSi->begin(); iter_Si != iter_SSi->end() ; iter_Si++)
        {
            cout <<"subset2" <<*iter_Si << endl;
        }
        
        cout << "+=============================" <<endl;
    }


        cout << "+=============================" <<endl;
        cout << "+=============================" <<endl;

   tempTS.states.clear();
   tempTS.states.insert(1);
  ret2 = subset2.insert(tempTS.states);
   if(ret2.second)
       cout << "success" << endl; 
   else
       cout << "fail" << endl; 
 
  for(iter_Si = tempTS.states.begin(); iter_Si != tempTS.states.end() ; iter_Si++)
  {
      cout <<"tempTS" <<*iter_Si << endl;
  }

        cout << "+=============================" <<endl;
        cout << "+=============================" <<endl;
  for(iter_SSi = subset2.begin(); iter_SSi != subset2.end(); iter_SSi++)
    {
        for(iter_Si = iter_SSi->begin(); iter_Si != iter_SSi->end() ; iter_Si++)
        {
            cout <<"subset2" <<*iter_Si << endl;
        }
        
        cout << "+=============================" <<endl;
    }


        cout << "+=============================" <<endl;
        cout << "+=============================" <<endl;
 
   tempTS.states.clear();
   tempTS.states.insert(1);
  ret2 = subset2.insert(tempTS.states);
   if(ret2.second)
       cout << "success" << endl; 
   else
       cout << "fail" << endl; 
  for(iter_Si = tempTS.states.begin(); iter_Si != tempTS.states.end() ; iter_Si++)
  {
      cout <<"tempTS" <<*iter_Si << endl;
  }

        cout << "+=============================" <<endl;
        cout << "+=============================" <<endl;
  for(iter_SSi = subset2.begin(); iter_SSi != subset2.end(); iter_SSi++)
    {
        for(iter_Si = iter_SSi->begin(); iter_Si != iter_SSi->end() ; iter_Si++)
        {
            cout <<"subset2" <<*iter_Si << endl;
        }
        
        cout << "+=============================" <<endl;
    }


        cout << "+=============================" <<endl;
        cout << "+=============================" <<endl;
 
  tempTS.states.clear();
   tempTS.states.insert(2);
   tempTS.states.insert(3);


    for(iter_SSi = subset2.begin(); iter_SSi != subset2.end(); iter_SSi++)
    {
            if(*iter_SSi == tempTS.states)
            {
                check = true;
                break;
            }
            else
            {
                check = false;
            }

    }
   if(check)
       cout << "success" << endl; 
   else
       cout << "fail" << endl; 
*/
   return true;

   
   
    // Implement this function.
}

