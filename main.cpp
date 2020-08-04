//
//  hw04.cpp
//  hw04: dynamic arrays
//
//  Created by Jensine Sultana on 10/14/18.
//  Copyright © 2018 Jensine. All rights reserved.
//
// large function

#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

void openFile(ifstream& commandSheet) {
    commandSheet.open("nobleWarriors.txt");
    
    if (!commandSheet) {
        cerr << "Could not open the file.\n";
        exit(1);
    }
}

class Warrior {
// Warrior class
    friend ostream& operator<<(ostream& os, const Warrior& rhs) {
        os << rhs.name << ": " << rhs.strength;
        return os;
    }
public:
    Warrior(const string& name, float strength) : name(name), strength(strength) {}

    int getStrength() const {
        return strength;
    }
    string getName() const {
        return name;
    }
    
    float checkStrength() const{
        return strength;
    }
    
    void changeStrength(float& factor) {
        if (factor == 0) {
            strength = 0;
        }
        else {
            strength = strength - (strength * factor);
        }
    }
    
    bool checkIfHired() const {
        return isHired;
    }
    
    void hire() {
        isHired = true;
    }
    
    void fire() {
        isHired = false;
    }
    
private:
    float strength;
    string name;
    bool isHired = false;
};

class Noble {
    friend ostream& operator<<(ostream& os, const Noble& rhs) {
        if (rhs.army.size() == 0) {
            os << "NONE" << endl;
        }
        else {
            os << rhs.name << " has an army of " << rhs.army.size() << endl;
            for (int i = 0; i < rhs.army.size(); ++i) {
                os << "     " << *(rhs.army[i]) << endl;
            }
        }
        return os;
    }
public:
    Noble(const string& name) : name(name) {}
    
    
    
    // fire
    void fire (string& warriorName) {
        for (int i = 0; i < army.size(); ++i) {
            if (army[i]->getName() == warriorName) {
                cout << "You don't work for me anymore " << warriorName << "! -- " << name << "." << endl;
                army[i]->fire();
                for (int j = i; j < (army.size() - 1); ++j) {
                    Warrior* moveBack = army[j];
                    Warrior* moveFront = army[j+1];
                    army[j] = moveFront;
                    army[j+1] = moveBack;
                }
                army.pop_back();
                break;
            }
        }
    }
    
    bool isDead() const {
        if (army.size() == 0) {
            return false;
        }
        for (int i = 0; i < army.size(); ++i) {
            if ((*army[i]).checkStrength() != 0) {
                return false;
            }
        }
        return true;
    }
    
    int totalStrength() const {
        int total = 0;
        for (int i = 0; i < army.size(); ++i) {
            total = total + army[i]->checkStrength();
        }
        return total;
    }
    
    void changeStrength(float factor) {
        for (int i = 0; i < army.size(); ++i) {
            // changes strength
            (army[i]->changeStrength(factor));
        }
    }
    
    // hire
    void hire(Warrior& warrior) {
        warrior.hire();
        army.push_back(&warrior);
    }
    
    // battle
    void battle(Noble& challenger) {
        cout << name << " battles " << challenger.getName() << endl;
        if (isDead() && challenger.isDead()) {
            cout << "Oh, NO! They're both dead! Yuck!" << endl;
        }
        else if (isDead()) {
            cout << "He's dead, " << challenger.getName() << endl;
        }
        else if (challenger.isDead()) {
            cout << "He's dead, " << name << endl;
        }
        else if (totalStrength() > challenger.totalStrength()) {
            float factor = (float(challenger.totalStrength()) / float(totalStrength()));
            changeStrength(factor);
            challenger.changeStrength(0);
            cout << name << " defeats " << challenger.getName() << endl;
        }
        else if (totalStrength() < challenger.totalStrength()) {
            float factor = float(totalStrength()) / float(challenger.totalStrength());
            changeStrength(0);
            challenger.changeStrength(factor);
            cout << challenger.getName() << " defeats " << name << endl;
        }
        else if (totalStrength() == challenger.totalStrength()) {
            changeStrength(0);
            challenger.changeStrength(0);
            cout << "Mutual Annihilation: " << name << " and " << challenger.getName() << " die at each other's hands" << endl;
        }
    }
    
    // get name
    string getName() const {
        return name;
    }
    
    bool inArmy(string& warriorName) const {
        for (int i = 0; i < army.size(); ++i) {
            if (army[i]->getName() == warriorName) {
                return true;
            }
        }
        return false;
    }
    
private:
    vector<Warrior*> army;
    string name;
};

void status(vector<Noble*>& nobles, vector<Warrior*>& warriors) {
    cout << "Status" << endl;
    cout << "======" << endl;
    cout << "Nobles:" << endl;
    if (nobles.size() == 0) {
        cout << "NONE" << endl;
    }
    else {
        for (int i = 0; i < nobles.size(); ++i) {
            cout << *nobles[i];
        }
    }
    cout << "Unemployed Warriors:" << endl;
    bool allEmployed = true;
    for (int i = 0; i < warriors.size(); ++i) {
        if (warriors[i]->checkIfHired() == false) {
            allEmployed = false;
        }
    }
    for (int i = 0; i < warriors.size(); ++i) {
        if (warriors[i]->checkIfHired() == false) {
            cout << *warriors[i] << endl;
        }
    }
    if (allEmployed == true) {
        cout << "NONE" << endl;
    }
}

void clear(vector<Noble*>& nobles, vector<Warrior*>& warriors) {
    // goes through the vectors of warriors and nobles and calls destructor for each of the warriors and nobles created
    for (size_t i = 0; i < warriors.size(); ++i) {
        delete warriors[i];
    }
    warriors.clear();
    
    for (size_t i = 0; i < nobles.size(); ++i) {
        delete nobles[i];
    }
    nobles.clear();
}

void readFile(ifstream& commandSheet, vector<Noble*>& nobles, vector<Warrior*>& warriors) {
// open file -> make nobles and warriors on the heap, per command
    string command;
    while (commandSheet >> command) {
        if (command == "Noble"){
            string noble;
            commandSheet >> noble;
            bool nobleExists = false;
            for (int i = 0; i < nobles.size(); ++i) {
                if (nobles[i]->getName() == noble) {
                    nobleExists = true;
                    break;
                }
            }
            if (!nobleExists) {
                Noble* newNoble = new Noble(noble);
                nobles.push_back(newNoble);
            }
        }
        else if (command == "Warrior") {
            string warrior;
            float strength;
            commandSheet >> warrior >> strength;
            bool warriorExists = false;
            for (int i = 0; i < warriors.size(); ++i) {
                if (warriors[i]->getName() == warrior) {
                    warriorExists = true;
                    break;
                }
            }
            if (!warriorExists) {
                Warrior* newWarrior = new Warrior(warrior, strength);
                warriors.push_back(newWarrior);
            }
        }
        else if (command ==  "Hire")  {
            string noble;
            string warrior;
            int indNoble;
            int indWarrior;
            commandSheet >> noble >> warrior;
            bool nobleExists = false;
            bool warriorExists = false;
            for (int i = 0; i < nobles.size(); ++i) { // check if Noble is in the heap
                if (nobles[i]->getName() == noble) {
                    nobleExists = true;
                    indNoble = i;
                    break;
                }
            }
            if (nobleExists) {
                for (int i = 0; i < warriors.size(); ++i) { // check if Warrior is in the heap
                    if (warriors[i]->getName() == warrior) {
                        warriorExists = true;
                        indWarrior = i;
//                        Warrior isHired = *warriors[i];
                        break;
                    }
                }
            }
            if (warriorExists) {
                nobles[indNoble]->hire(*warriors[indWarrior]);
            }
            else {
                cout << "The noble or warrior does not exist." << endl;
            }
        }
        else if (command == "Status") {
            status(nobles, warriors);
        }
        else if (command == "Fire") {
            string noble;
            string warrior;
            int indNoble;
            commandSheet >> noble >> warrior;
            bool nobleExists = false; // check if noble exists
            for (int i = 0; i < nobles.size(); ++i) {
                if (nobles[i]->getName() == noble) {
                    nobleExists = true;
                    indNoble = i;
                    break;
                }
            }
            if (nobleExists) { // check if warrior is in noble's army
                if (nobles[indNoble]->inArmy(warrior)) {
                    nobles[indNoble]->fire(warrior); // fire
                }
                else {
                    cout << "You can't fire someone who doesn't work for you, " << nobles[indNoble]->getName() << "!" << endl;
                }
            }
            else {
                cout << "The noble does not exist" << endl;
            }
        }
        else if (command == "Battle") {
            string noble;
            string challenger;
            int indNoble;
            int indChallenger;
            commandSheet >> noble >> challenger;
            bool bothExist = false;
            for (int i = 0; i < nobles.size(); ++i) {
                if (nobles[i]->getName() == noble) {
                    indNoble = i;
                    for (int j = 0; j < nobles.size(); ++j) {
                        if (nobles[j]->getName() == challenger) {
                            indChallenger = j;
                            bothExist = true;
                            break;
                        }
                    }
                }
            }
            if (bothExist) {
                nobles[indNoble]->battle(*nobles[indChallenger]);
            }
            else {
                cout << "If a noble doesn't exist, he cannot battle, fool" << endl;
            }
        }
        else if (command == "Clear") {
            clear(nobles, warriors);
        }
    }
    
// close file
    commandSheet.close();
}

int main() {
    ifstream commandSheet;
    openFile(commandSheet);
    vector<Noble*> nobles;
    vector<Warrior*> warriors;
    readFile(commandSheet, nobles, warriors);
}
