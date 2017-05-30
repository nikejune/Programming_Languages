// PL homework: hw1
// fsa.h

#ifndef _PL_HOMEWORK_FSA_H_
#define _PL_HOMEWORK_FSA_H_

#include <vector>
#include <string>

#include <map>
#include <set>

using namespace std;

// Valid characters are alphanumeric and underscore (A-Z,a-z,0-9,_).
// Epsilon moves in NFA are represented by empty strings.

struct FSATableElement {
  int state;
  int next_state;
  std::string str;
};

struct TableSet {

    set <int> states;
    char input_char;
    set<int> next_states;

};

struct FiniteStateAutomaton {
  // Make your ow FSA struct here.
    set<int> start_states;
    set<int> accept_states;
    set<char> input_chars;
    vector<TableSet> table;
};

// Run FSA and return true if str is matched by fsa, and false otherwise.
bool RunFSA(const FiniteStateAutomaton& fsa, const std::string& str);

bool BuildFSA(const std::vector<FSATableElement>& elements,
              const std::vector<int>& accept_states,
              FiniteStateAutomaton* fsa);

#endif //_PL_HOMEWORK_FSA_H_

