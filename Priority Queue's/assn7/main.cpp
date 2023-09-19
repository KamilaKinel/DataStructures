// ******************************************************
// Name: Kamila Kinel
// NSHE id: 5005951543
// Assignment number: assingment7
// Description: Program that reads into files to get
//              information and uses priorityQs
//              to create a create a sort of simulated
//              gameplay of Resident Evil 3: Nemesis
// ******************************************************
#include <iostream>
#include <fstream>
#include <vector>
#include "priorityQ.h"
#include <iomanip>

using namespace std;

struct mercType {
    // tracks how many of each monsters is
    // defeated and civilian saved for a
    // mercenary
    int zombies;
    int spiders;
    int drainDeimos;
    int zombieDogs;
    int hunters;
    int nemesis;
    int civilians;
    // default constructor
    mercType() {
        zombies = 0;
        spiders = 0;
        drainDeimos = 0;
        zombieDogs = 0;
        hunters = 0;
        nemesis = 0;
        civilians = 0;
    }
    double calScore() const{
        return zombies * 1.0 + spiders * 1.1 + drainDeimos * 1.2 +
                zombieDogs * 1.3 + hunters * 1.4 + nemesis * 1.5 +
                civilians * 2.0;
    }
    // do not actual try to overload the
    //  question mark operator , you can overload
    // any comparison operator you like and it will
    // be used in your bubbleUp and bubbleDown
    bool operator<(const mercType& rhs) const {
        return calScore() < rhs.calScore();
    }

    // You might need to overload + or +=
    // operators , would be needed in the
    //  increaseKey function
    mercType& operator+=(const mercType& rhs) {
        this->zombies = zombies + rhs.zombies;
        this->spiders = spiders + rhs.spiders;
        this->drainDeimos = drainDeimos + rhs.drainDeimos;
        this->zombieDogs = zombieDogs + rhs.zombieDogs;
        this->hunters = hunters + rhs.hunters;
        this->nemesis = nemesis + rhs.nemesis;
        this->civilians = civilians + rhs.civilians;
        return *this;
    }
    
};

/**
 * =Initalizes monsters ans sets amount
 *
 * @param monsterStr monster string from file
 * @param monsterStr amount from file
 * @return N/A
 */
mercType createMercType(std::string monsterStr, int amount){
    mercType m;   // crate object of merctype
    if (monsterStr == "zombies") { 
        m.zombies = amount;
    } else if (monsterStr == "spiders") { 
        m.spiders = amount;
    } else if (monsterStr == "drainDeimos") { 
        m.drainDeimos = amount;
    } else if (monsterStr == "zombieDogs") { 
        m.zombieDogs = amount;
    } else if (monsterStr == "hunters") { 
        m.hunters = amount;
    } else if (monsterStr == "nemesis") { 
        m.nemesis = amount;
    } else if (monsterStr == "civilians") { 
        m.civilians = amount;
    }
    return m;
}

/**
 * Helper function that erases leading/trailing spaces adn
 * newline chars
 *
 * @param line line from file
 * @return N/A
 */
std::string trim(std::string line){
    int i = line.find(" ");
    while (i == 0){
        line.erase(0 , i + 1);
        i = line.find(" ");
    }
    i = line.rfind(" ");
    while (i == line.length() - 1){
        line.erase(i);
        i = line.rfind(" ");
    }
    i = line.rfind("\n");
    while (i == line.length() - 1){
        line.erase(i);
        i = line.rfind("\n");
    }
    return line;   // returns string w ignored spaces/ newline
}

/**
 * Helper function that helps us easily retrieve information from file.
 * gets the next word in a string.
 *
 * @param line line from file
 * @return N/A
 */
std::string getNext(std::string &line){
    int i = line.find(" ");  // finds the index of the next word
    std::string item = line.substr(0 , i);  // gets word
    line.erase(0 , i + 1);    // updates string 
    i = line.find(" ");
    line = trim(line);   // gets rid of leading/trailing spaces
    return item;   // word is returned
}

/**
 * Reads mercenary file and 
 *
 * @param mercenariesFilename file of mercinaries
 * @param mercenaries mercinaries that will be retreieved from file
 * @return N/A
 */
void readMercenaries(std::string mercenariesFilename, priorityQ<std::string, mercType> &mercenaries){
    std::fstream file (mercenariesFilename);
    std:: string line, item;
    // reads into file
	if(file.is_open()){
		while(std::getline(file, line)){
            // saves mercinary information in sorted order(priorQ)
			mercenaries.push_back(line, createMercType("",0));
		}
	}
}

/**
 * Reads simulation data file
 *
 * @param simFilename file name
 * @param simStepsMercenary vec w mercenaries
 * @param simStepsMonster vec w monster info
 * @return N/A
 */
void readSimulationData(std::string simFilename, vector<string> &simStepsMercenary, vector<mercType> &simStepsMonster){
    std::fstream file (simFilename);
    std:: string line, item;
    // read into file
	if(file.is_open()){
		while(std::getline(file, line)){
			string mercName = getNext(line);     // getNext will give us the first word in line
            string monsterName = getNext(line);   // gets the mosters name, second in line
            int amount = stoi(getNext(line));    // gets amount, last in line
            // saves/ sorts into vectors
            simStepsMercenary.push_back(mercName);
            simStepsMonster.push_back(createMercType(monsterName, amount));
		}
	}
}

int main() { 
    // For setting decimal
    cout << fixed;
    cout << setprecision(1);

    priorityQ<std::string, mercType> mercenaries; 
    vector<string> simStepsMercenary;    // vector for merc 
    vector<mercType> simStepsMonster;    // vector for diff monsters

    std::string mercenariesFilename;
    std::string simFilename;
    cout << endl;

    cout << "Enter mercenaries file: ";
    getline(cin, mercenariesFilename);
    cout << endl << endl;
    cout << "Enter simulation file: ";
    getline(cin, simFilename);

    // mercenariesFilename = "mercenaries.txt";
    // simFilename = "simulation03.txt";
    readMercenaries(mercenariesFilename, mercenaries);
    readSimulationData(simFilename,simStepsMercenary, simStepsMonster);
    std::string prevLeader = "";
    for(int i = 0; i < simStepsMercenary.size(); i++){
        mercenaries.increaseKey(simStepsMercenary[i], simStepsMonster[i]);
        if (prevLeader != mercenaries.get_front_key()){
            cout << endl;
            cout << "We have a new leader: " << mercenaries.get_front_key() << " Score: " << mercenaries.get_front_priority().calScore() << endl;
            prevLeader = mercenaries.get_front_key();  // prev leader= key feild of root elem
        }
    }
    
    int i = 0;    // counter
    // Uses while to print all ranks and names
    while (!mercenaries.isEmpty()){
        i++;
        cout << "Rank " << i << endl;
        cout << "Name: " << mercenaries.get_front_key() << " Score: " << mercenaries.get_front_priority().calScore() << endl;
        cout << endl;
        mercenaries.pop_front();   // assigns last ele to root position
    }
    return 0;

}