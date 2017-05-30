// PL homework: hw2
// lr_parser.cc

#include <assert.h>

#include <iostream>
#include <vector>
#include <stack>

#include "lr_parser.h"

#define DISABLE_LOG false
#define LOG \
    if (DISABLE_LOG) {} else std::cerr

using namespace std;

bool BuildLRParser(const std::vector<LRTableElement>& elements,
                   const std::vector<LRRule>& rules,
                   LRParser* lr_parser) {
    for (int i = 0; i < elements.size(); i++)
    {
        
        //build action table
        if (elements[i].action != GOTO)
        {
            pair<int, int> key;
            pair<LRAction, int> value;
            
            key = make_pair(elements[i].state, elements[i].symbol);
            value = make_pair(elements[i].action, elements[i].next_state);
            lr_parser->action_table[key] = value;
        }
        //build goto table
        else
        {
            pair<int, int> key;
            
            key = make_pair(elements[i].state, elements[i].symbol);
            lr_parser->goto_table[key] = elements[i].next_state;
        }
    }
    
    //build rule_table
    for (int i = 0; i < rules.size(); i++)
    {
        pair<int, int> value;
        
        value = make_pair(rules[i].lhs_symbol, rules[i].num_rhs);
        lr_parser->rule_table[rules[i].id] = value;
    }
    
    return true;
}

bool RunLRParser(const LRParser& lr_parser, const std::string& str) {
    
    map<pair<int, int>, pair<LRAction, int> > action_table = lr_parser.action_table;
    map<pair<int, int>, int > goto_table = lr_parser.goto_table;
    map<int, pair<int, int> > rule_table = lr_parser.rule_table;
    stack<int> stk;
    
    int state = 0;
    int strIdx = 0;
    stk.push(state);
    
    while (true)
    {
        map<pair<int, int>, pair<LRAction, int> >::iterator iter_action;
        map<pair<int, int>, int >::iterator iter_goto;
        map<int, pair<int, int> >::iterator iter_rule;
        pair<LRAction, int> action;
        pair<int, int> key(state, str.at(strIdx));
        pair<int, int> goto_key;

        iter_action = action_table.find(key);
        if (iter_action == action_table.end())
            return false;
        action = iter_action->second;
        
        if (action.first == SHIFT) // if action is SHIFT, push character and number by table.
        {
            stk.push(str.at(strIdx));
            stk.push(action.second);
            strIdx++;
        }
        else if (action.first == REDUCE) // if action is REDUCE, reduce by num_rhs, use rule table
        {
            int lhs_symbol;
            int num_rhs;
            int key = action.second;
            
            iter_rule = rule_table.find(key);
            if (iter_rule == rule_table.end())
                return false;
            lhs_symbol = iter_rule->second.first;
            num_rhs = iter_rule->second.second;
            
            for (int i = 0; i < num_rhs * 2; i++)
                stk.pop();
            
            state = stk.top();
            goto_key.first = state;
            goto_key.second = lhs_symbol;
            iter_goto = goto_table.find(goto_key);
            if (iter_goto == goto_table.end())
                return false;
            state = iter_goto->second;
            
            stk.push(lhs_symbol);
            stk.push(state);
        }
        else if (action.first == ACCEPT) // if action is ACCEPT, loop is end
            return true;
        else if (action.first == INVALID)// if action is INVALID, error, return false
            return false;
        
        state = stk.top();
    }
    
    return false;
}

