// PL homework: hw2
// regexp_matcher.cc

#include <iostream>
#include "regexp_matcher.h"

using namespace std;

#define SUCCESS	999

int state = 1;
int exp_idx = 0;
int count_char = 0;
bool union_check = false;

// set state, char, next_state in table_element
void AddToTable(char character, RegExpMatcher *regexp_matcher)
{
    pair<int, char> key;
    
    key = make_pair(state, character);
    regexp_matcher->table_element.insert(make_pair(key, ++state));
}

// handle the union "|"
void HandleUnion(const char *regexp, RegExpMatcher *regexp_matcher)
{
    AddToTable('#', regexp_matcher);
    
    int junc_state = state - 1;
    
    // save regexp in table, until regexp[exp_idx] =='|' is ture
    while (regexp[exp_idx] != '|')
        RegExpToNFA(regexp, regexp_matcher);
    
    int upWay_state = state;
    
    pair<int, char> key = make_pair(junc_state, '#');
    regexp_matcher->table_element.insert(make_pair(key, ++state));
    
    exp_idx++;
    while ((regexp[exp_idx] != ')') && (regexp[exp_idx] != '\0'))
        RegExpToNFA(regexp, regexp_matcher);
    
    key = make_pair(upWay_state, '#');
    regexp_matcher->table_element.insert(make_pair(key, state + 1));
    AddToTable('#', regexp_matcher);
}

// handle the asterisk "*"
void HandleAster(int count_char, RegExpMatcher *regexp_matcher)
{
    //pair<char, int> value;
    pair<int, char> key;
    
    key = make_pair((state - count_char), '#');
    regexp_matcher->table_element.insert(make_pair(key, state));
    key = make_pair(state, '#');
    regexp_matcher->table_element.insert(make_pair(key, (state - count_char)));
}

// find the "("
int Find_lp(const char *regexp, int idx)
{
    int paren_match = 0;
    int num_paren = 0;
    
    while (idx >= 0)
    {
        if (regexp[idx] == ')')
        {
            num_paren++;
            paren_match++;
        }
        else if (regexp[idx] == '(')
        {
            num_paren++;
            paren_match--;
            if (paren_match == -1)
                return idx;
        }
        idx--;
    }
    if (num_paren == 0)
        return 0;
    return -1;
}

// check the union "|"
bool CheckUnion(const char *regexp)
{
    int i = 0;
    union_check = true;
    
    while (regexp[i] != 0)
    {
        if (regexp[i] == '|')
        {
            int idx = Find_lp(regexp, i);
            if (idx == 0)
                return true;
            else
                return false;
        }
        i++;
    }
    return false;
}

int RegExpToNFA(const char *regexp, RegExpMatcher *regexp_matcher)
{
    //[,] X
    if (regexp[exp_idx] == '[' || regexp[exp_idx] == ']')
        return -1;
    
    if (!union_check && CheckUnion(&regexp[exp_idx]))
        HandleUnion(regexp, regexp_matcher);
    
    if (regexp[exp_idx] == 0)
    {
        regexp_matcher->accept_state = state;
        return SUCCESS;
    }
    
    if (regexp[exp_idx] == ')')
    {
        exp_idx++;
        return 0;
    }
    
    switch (regexp[exp_idx])
    {
        case '*':
            if (regexp[exp_idx - 1] == ')')
                HandleAster(count_char, regexp_matcher);
            else
                HandleAster(1, regexp_matcher);
            exp_idx++;
            break;
            
        case '(':
            union_check = false;
            count_char = 0;
            exp_idx++;
            RegExpToNFA(regexp, regexp_matcher);
            break;
            
        default:
            count_char++;
            AddToTable(regexp[exp_idx++], regexp_matcher);
            break;
    }
    
    return 0;
}

//nfa to dfa
void MakeStateSet_Eps(RegExpMatcher *regexp_matcher, int chk_state, set<int> *states)
{
    if (chk_state != 0)
    {
        multimap<pair<int, char>, int >::iterator iter;
        iter = regexp_matcher->table_element.begin();
        while ((iter != regexp_matcher->table_element.end()) && (iter->first.first != chk_state))
            iter++;
        while ((iter != regexp_matcher->table_element.end()) && (iter->first.first == chk_state))
        {
            if (iter->first.second == '#')
                states->insert(iter->second);
            iter++;
        }
        return;
    }
    //if it has epsilon "#"
    else {
        chk_state = 1;
        for (state = chk_state; state <= regexp_matcher->accept_state; state++)
        {
            bool exist = false;
            pair<int, char> key = make_pair(state, '#');
            multimap<pair<int, char>, int >::iterator iter_map;
            set<int> states;
            
            states.insert(state);
            iter_map = regexp_matcher->table_element.find(key);
            if (iter_map != regexp_matcher->table_element.end())
                while ((iter_map != regexp_matcher->table_element.end()) && (iter_map->first == key))
                {
                    exist = true;
                    int next_state = iter_map->second;
                    MakeStateSet_Eps(regexp_matcher, next_state, &states);
                    states.insert(next_state);
                    iter_map++;
                }
            if (exist)
                regexp_matcher->state_set.insert(states);
        }
    }
}

void MakeStateSet(RegExpMatcher *regexp_matcher)
{
    multimap<pair<int, char>, int >::iterator iter_map;
    iter_map = regexp_matcher->table_element.begin();
    while (iter_map != regexp_matcher->table_element.end())
    {
        bool insert = true;
        char character = iter_map->first.second;
        if (character != '#')
        {
            int state = iter_map->first.first;
            set<set<int> >::iterator iter_ss = regexp_matcher->state_set.begin();
            while (iter_ss != regexp_matcher->state_set.end())
            {
                set<int>::iterator iter_s = iter_ss->begin();
                while (iter_s != iter_ss->end())
                {
                    if (*iter_s == state)
                    {
                        insert = false;
                        break;
                    }
                    iter_s++;
                }
                iter_ss++;
            }
            if (insert)
            {
                set<int> states;
                states.insert(state);
                regexp_matcher->state_set.insert(states);
            }
        }
        iter_map++;
    }
    
    //is it accepted?
    bool insert = true;
    int state = regexp_matcher->accept_state;
    set<set<int> >::iterator iter_ss = regexp_matcher->state_set.begin();
    while (iter_ss != regexp_matcher->state_set.end())
    {
        set<int>::iterator iter_s = iter_ss->begin();
        while (iter_s != iter_ss->end())
        {
            if (*iter_s == state)
            {
                insert = false;
                break;
            }
            iter_s++;
        }
        iter_ss++;
    }
    if (insert)
    {
        set<int> states;
        states.insert(state);
        regexp_matcher->state_set.insert(states);
    }
}

void BuildDFA(RegExpMatcher *regexp_matcher)
{
    set<set<int> >::iterator iter_ss;
    iter_ss = regexp_matcher->state_set.begin();
    while (iter_ss != regexp_matcher->state_set.end())
    {
        set<int>::iterator iter_s;
        iter_s = iter_ss->begin();
        while (iter_s != iter_ss->end())
        {
            multimap<pair<int, char>, int>::iterator iter_map;
            iter_map = regexp_matcher->table_element.begin();
            while (iter_map != regexp_matcher->table_element.end())
            {
                char character = iter_map->first.second;
                int state = iter_map->first.first;
                if ((character != '#') && (state == *iter_s))
                {
                    pair<set<int>, char> key;
                    key.first = *iter_ss;
                    key.second = character;
                    //find next_state set
                    set<int> next_state_set;
                    int next_state = iter_map->second;
                    set<set<int> >::iterator iter_nss = regexp_matcher->state_set.begin();
                    while (iter_nss != regexp_matcher->state_set.end())
                    {
                        set<int>::iterator iter_ns = iter_nss->begin();
                        while (iter_ns != iter_nss->end())
                        {
                            if (*iter_ns == next_state)
                            {
                                next_state_set = *iter_nss;
                                break;
                            }
                            iter_ns++;
                        }
                        iter_nss++;
                    }//find next_state set END
                    regexp_matcher->table_map[key] = next_state_set;
                }
                iter_map++;
            }
            iter_s++;
        }
        iter_ss++;
    }
}

bool BuildRegExpMatcher(const char* regexp, RegExpMatcher* regexp_matcher) {
    
    while (true)
    {
        int succ = RegExpToNFA(regexp, regexp_matcher);
        if (succ == SUCCESS)
        {
            MakeStateSet_Eps(regexp_matcher, 0, NULL);
            MakeStateSet(regexp_matcher);
            BuildDFA(regexp_matcher);
            return true;
        }
        if (succ == -1)
            break;
    }
    return false;
}

bool RunRegExpMatcher(const RegExpMatcher& regexp_matcher, const char* str) {
    
    set<int> state;
    set<int>::iterator iter_s;
    set<set<int> >::iterator iter_ss;
    map<pair<set<int>, char>, set<int> >::iterator iter_map;
    map<pair<set<int>, char>, set<int> > map = regexp_matcher.table_map;
    
    iter_ss = regexp_matcher.state_set.begin();
    //start state set
    state = *iter_ss;
    while (true)
    {
        bool find = false;
        pair<set<int>, char> key;
        
        if (*str == 0)
            return false;
        
        key.first = state;
        key.second = *str;
        
        iter_map = map.find(key);
        if (iter_map == map.end())
        {
            key.first = state;
            key.second = '.';
            iter_map = map.find(key);
            if (iter_map != map.end())
                find = true;
        }
        else
            find = true;
        
        if (!find)
            return false;
        else
        {
            set<int> next_state = iter_map->second;
            state = next_state;
            iter_s = state.begin();
            while (iter_s != state.end())
            {
                if (*iter_s == regexp_matcher.accept_state && *(str+1)=='\0')
                    return true;
                iter_s++;
            }
        }
        str++;
    }
    
    return false;
}

