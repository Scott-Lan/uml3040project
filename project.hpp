#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <map>
#include <vector>
#include <tuple>
#include <algorithm>

enum automaton_type { d, n };

using namespace std;

class Automaton
{
public:
    Automaton(){};
    Automaton(vector<string> inputStates, string startState, vector<string> acceptState, vector<char> alphabet, vector<tuple<string, string, char>> trans);
    Automaton(Automaton &init);
    //void calcAutomaton();
    void updateAutomaton(vector<string> inputStates, string startState, vector<string> acceptState, vector<char> alphabet, vector<tuple<string, string, char>> trans);
    string getStart();
    void setStart(string inStart) { start = inStart; };
    vector<string> getAccept();
    void setAccept(string inAccept){accept.push_back(inAccept);};
    void printStates();
    void setStates(vector<string> inStates) { states = inStates; };
    void printLanguage();
    void setLang(vector<char> inLang){language = inLang;};
    vector<char> transferLanguage();
    string getCurrent();
    string getFilename();
    bool getVerboseFlag();
    bool getHelpFlag();
    void printTransitionList();
    void verbosePrintData();
    void setInput(string inValue) { inputValue = inValue; };
    string getInput(){return inputValue;};
    void setFlags(int argc, char *argv[]);
    vector<string> getStates();
    vector<tuple<string, string, char>> transferTable();
    void setType(automaton_type in){a_t = in;};
    automaton_type getType(){return a_t;};

private: 
automaton_type a_t = n;

protected:
    vector<string> states;
    string start;
    vector<string> accept;
    vector<char> language;
    string inputValue;
    string currentState;
    string AutomatonFile;
    bool verbose = true, help = false;
    

    vector<tuple<string, string, char>> transitions;
};

// NFA class definition
class NFA : public Automaton
{

public:
    NFA(){};
    NFA(Automaton &init);

    void calcNFA();

private:
    vector<string> nfaStates;
    string nfaFile;
    vector<string> nfaNextStates;
};

// DFA class definition
class DFA : public Automaton
{

public:
    DFA(){};
    DFA(vector<string> inputStates, string startState, vector<string> acceptState, vector<char> alphabet, vector<tuple<string, string, char>> trans);

    DFA(Automaton &init);

    void calcDFA();

private:
    string dfaFile;
};

void parseFile(istream &inFile, Automaton &copy);