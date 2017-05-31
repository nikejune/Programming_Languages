// PL homework: hw2
// regexp_matcher.h

#ifndef _PL_HOMEWORK_REGEXP_MATCHER_H_
#define _PL_HOMEWORK_REGEXP_MATCHER_H_

#include <map>
#include <set>

struct RegExpMatcher {
  // Design your RegExpMatcher structure.
    std::multimap<std::pair<int, char>, int > table_element;
    int accept_state;
    
    //nfa to dfa
    std::set<std::set<int> > state_set;
    std::map<std::pair<std::set<int>, char>, std::set<int> > table_map;

};

int RegExpToNFA(const char *regexp, RegExpMatcher *regexp_matcher);

// Homework 1.3
bool BuildRegExpMatcher(const char* regexp, RegExpMatcher* regexp_matcher);

// Homework 1.3
bool RunRegExpMatcher(const RegExpMatcher& regexp_matcher, const char* str);

#endif  //_PL_HOMEWORK_REGEXP_MATCHER_H_

