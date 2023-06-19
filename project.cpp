#include "project.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    Automaton a1;
    a1.setFlags(argc, argv);
    ifstream inFile(a1.getFilename());

    if (!inFile.fail() && a1.getVerboseFlag())
    {
        cout << "File accepted\n";
    }
    else if (inFile.fail())
    {
        cout << "File failed to open. Aborting.\n";
        return -1;
    }

    parseFile(inFile, a1);

    // cout << "automaton type: " << a1.getType() << endl;

    if (a1.getType() == d)
    {
        DFA d1(a1);
        d1.calcDFA();
    }
    else if (a1.getType() == n)
    {
        NFA n1(a1);
        n1.calcNFA();
    }

    inFile.close();
    return 0;
}

void parseFile(istream &inFile, Automaton &d1)
{
    string currentLine;
    string line, word, start;
    vector<string> accept;
    vector<tuple<string, string, char>> transitionTable;
    vector<char> alphabet;
    vector<string> inputStates;

    if (d1.getVerboseFlag())
    {
        cout << "---Defining Automaton from file---";
    }
    while (getline(inFile, currentLine))

    {
        istringstream lineString(currentLine);

        lineString >> word;
        if (word == "states:")
        {
            while (lineString >> word)
            {
                inputStates.push_back(word);
            }
            cout << endl;
        }
        if (word == "alphabet:")
        {
            while (lineString >> word)
            {
                alphabet.push_back(word.at(0));
            }
        }
        if (word == "startstate:")
        {
            lineString >> word;
            start = word;
        }
        if (word == "acceptstate:" || word == "finalstate:")
        {
            while (lineString >> word)
            {
                accept.push_back(word);
            }
        }
        if (word == "transition:")
        {
            string currState, transChar, moveState;
            lineString >> word;
            currState = word;
            lineString >> word;
            transChar = word;
            lineString >> word;
            moveState = word;

            transitionTable.push_back(tuple<string, string, char>(currState, moveState, transChar.at(0)));
        }
    }

    d1.updateAutomaton(inputStates, start, accept, alphabet, transitionTable);

    // print if -v
    if (d1.getVerboseFlag())
    {
        d1.verbosePrintData();
    }
}

Automaton::Automaton(vector<string> inputStates, string startState, vector<string> acceptState, vector<char> alphabet, vector<tuple<string, string, char>> trans)
{
    states = inputStates;
    start = startState;
    language = alphabet;
    currentState = startState;
    accept = acceptState;
    transitions = trans;
}
Automaton::Automaton(Automaton &init)
{
    states = init.states;
    start = init.start;
    accept = init.accept;
    language = init.language;
    currentState = init.currentState;
    transitions = init.transitions;
}
DFA::DFA(vector<string> inputStates, string startState, vector<string> acceptState, vector<char> alphabet, vector<tuple<string, string, char>> trans)
{
    states = inputStates;
    start = startState;
    language = alphabet;
    currentState = startState;
    accept = acceptState;
    transitions = trans;
}
DFA::DFA(Automaton &init)
{
    states = init.getStates();
    start = init.getStart();
    accept = init.getAccept();
    language = init.transferLanguage();
    currentState = init.getCurrent();
    transitions = init.transferTable();
    verbose = init.getVerboseFlag();
    help = init.getHelpFlag();
    dfaFile = init.getFilename();
}

NFA::NFA(Automaton &init)
{
    states = init.getStates();
    start = init.getStart();
    accept = init.getAccept();
    language = init.transferLanguage();
    currentState = init.getCurrent();
    transitions = init.transferTable();
    verbose = init.getVerboseFlag();
    help = init.getHelpFlag();
    nfaFile = init.getFilename();
}

void DFA::calcDFA()
{
    char curr;
    string input;

    while (getline(cin, input))
    {
        if (input == "")
        {
            break;
        }

        currentState = start;
        for (int i = 0; i < input.size(); ++i)
        {

            curr = input.at(i);

            if (find(language.begin(), language.end(), curr) == language.end())
            {
                cout << curr << " not recognized in the language. Aborting.\n"
                     << input << " <--- Reject" << endl;
                return;
            }

            if (getVerboseFlag())
            {
                cout << "Current State: " << currentState
                     << "  Symbol: " << curr;
            }
            for (auto state : transitions)
            {
                if (currentState == get<0>(state))
                {
                    if (curr == get<2>(state))
                    {
                        currentState = get<1>(state);
                        break;
                    }
                }
            }
            if (getVerboseFlag())
            {
                cout << "  New State: " << currentState << endl;
            }
        }
        cout << input;
        if (find(accept.begin(), accept.end(), currentState) != accept.end())
        {
            cout << " <--- Accept" << endl;
        }
        else
        {
            cout << " <--- Reject" << endl;
        }
        cin.clear();
    }
}

void NFA::calcNFA()
{
    char curr;

    string input;

    while ((getline(cin, input)))
    {
        if (input == "")
        {
            break;
        }

        nfaStates.push_back(start);

        for (int i = 0; i < input.size(); ++i)
        {
            curr = input.at(i);

            if (find(language.begin(), language.end(), curr) == language.end())
            {
                cout << curr << " not recognized in the language. Aborting.\n"
                     << input << " <--- Reject" << endl;
                return;
            }
            if (getVerboseFlag())
            {
                cout << "Current State(s): ";
                for (auto i : nfaStates)
                {
                    cout << i << " ";
                }
                cout << "  Symbol: " << curr;
            }

            for (auto i : nfaStates)
            {

                for (auto state : transitions)
                {
                    nfaStates
                    int symbol = get<2>(state);
                    string next;

                    if (i == get<0>(state))
                    {

                        if (curr == symbol)
                        {
                            next = get<1>(state);
                            // cout << "added: " << get<1>(state) << endl;
                            if (next != get<0>(state))
                            {
                                nfaStates.push_back(next);
                            }
                        }
                    }
                }
            }

            if (getVerboseFlag())
            {
                cout << "  New State(s): ";
                for (auto i : nfaStates)
                {
                    cout << i << " ";
                }
                cout << endl;
            }
        }
        cout << input;

        // loop to find acceptable final state
        for (auto i : accept)
        {
            if (find(nfaStates.begin(), nfaStates.end(), i) != nfaStates.end())
            {
                cout << " <--- Accept" << endl;
                break;
            }
            if (find(nfaStates.begin(), nfaStates.end(), i) != nfaStates.end())
            {
                cout << " <--- Reject" << endl;
            }
        }

        cin.clear();
        nfaStates.clear();
    }
}

void Automaton::updateAutomaton(vector<string> inputStates, string startState, vector<string> acceptState, vector<char> alphabet, vector<tuple<string, string, char>> trans)
{
    states = inputStates;
    start = startState;
    currentState = startState;
    language = alphabet;
    accept = acceptState;
    transitions = trans;
}

vector<string> Automaton::getAccept()
{
    return accept;
}

string Automaton::getStart()
{
    return this->start;
}

void Automaton::printStates()
{
    cout << "States: \n";
    for (auto printState : states)
    {
        cout << printState << ", ";
    }
    cout << endl;
}

void Automaton::printLanguage()
{
    cout << "Language: \n";
    for (auto printLang : language)
    {
        cout << printLang << ", ";
    }
    cout << endl;
}

string Automaton::getCurrent()
{
    return this->currentState;
}

string Automaton::getFilename()
{
    return this->AutomatonFile;
}

bool Automaton::getVerboseFlag()
{
    return this->verbose;
}

bool Automaton::getHelpFlag()
{
    return this->help;
}
void Automaton::printTransitionList()
{
    cout << "Transitions: \n";
    for (auto printTrans : transitions)
    {
        cout << get<0>(printTrans) << " " << get<2>(printTrans) << " " << get<1>(printTrans) << endl;
    }
}

void Automaton::verbosePrintData()
{
    this->printStates();
    this->printLanguage();
    cout << "Start State: \n"
         << this->getStart() << endl
         << "Accept State: \n";
    for (auto acc : accept)
    {
        cout << acc << ", ";
    }
    cout << endl;
    this->printTransitionList();
    cout << endl;
}

void Automaton::setFlags(int argc, char *argv[])
{
    char **originalArgv = argv;
    // cout << "argc: " << argc << endl;
    //  no arguments
    if (argc == 1)
    {
        // default is b.dfa
        AutomatonFile = "x.nfa";
    }
    for (int i = 0; i < argc; ++i)
    {
        // cout << "argv: " << argv[i] << endl;

        // Help flag
        if (strcmp(argv[i], "-h") == 0)
        {
            help = true;
        }
        // Verbose flag
        else if (strcmp(argv[i], "-v") == 0)
        {
            verbose = true;
        }
        // Automaton flag
        else if (strcmp(argv[i], "-d") == 0)
        {
            setType(d);
            // assign filename to argument after -d
            AutomatonFile = argv[i + 1];
            i++;
        }
        // NFA flag
        else if (strcmp(argv[i], "-n") == 0)
        {
            setType(n);
            // assign filename to argument after -n
            AutomatonFile = argv[i + 1];
            i++;
        }
    }

    // output argument values
    /* cout << "automaton type: " << getType() << endl;

    for (int i = 0; i < argc; ++i)
    {
        cout << "Argument #" << i << ": " << *originalArgv << endl;
        originalArgv++;
    } */
}

vector<char> Automaton::transferLanguage()
{
    return language;
}

vector<string> Automaton::getStates()
{
    return this->states;
}

vector<tuple<string, string, char>> Automaton::transferTable()
{
    return transitions;
}