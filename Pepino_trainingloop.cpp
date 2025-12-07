#include <iostream>
#include <string>
#include <iomanip>
#include <limits>
#include <cmath>
#include <algorithm>
using namespace std;


//Structures and variables

const int PlayerStatCount = 7;

const int StatATK = 0;
const int StatHP = 1;
const int StatHPMax = 2;
const int StatEND = 3;
const int StatEP = 4;
const int StatEPMax = 5;
const int StatEPRegen = 6;

//Player Array

double PlayerStats[PlayerStatCount] {
    10.0, 50.0, 50.0, 15.0, 5.0, 5.0, 0.0
};

//Training History

string TrainingHistory[48];

int SkillsUnlocked[8] = {0};

int SelfDiscovery = 0;

// Formulas for different stats

double ApplyStatGain(int statType, int n) {
    
    switch(statType) {
        case StatATK: {
            return 11 + 2 * (n - 1);
        }
        case StatHPMax: {
            return 21 + 4 * (n - 1);
        }
        case StatEND: {
            return 15 + 3 * (n - 1);
        }
        case StatEPMax: {
            return 15 + 3 * (n - 1);
        }
        case StatEPRegen: {
            return (double) (2.5 + 0.5 * (n - 1));
        }
    };
    return 0;
}

//DESCRIPTIONS

string skill_name[8] = {
    "Eight Gates - Gate of Opening(G1)",
    "Chain Handling",
    "kei-gan-barrage!",
    "Front Lotus",
    "Gate of Opening - Mastery",
    "Gate of Healing(G2)",
    "Chain Barrage",
    "Reverse Lotus"
};
string skill_type[8] = {
    "Stance",
    "Passive",
    "Combo Attack",
    "Finisher",
    "Passive",
    "Stance",
    "Combo Attack",
    "Finisher"
};
string skill_description[8] = {
    "Boosts ATK x 1.8, 35 EP on cast, 7 EP per turn",
    "Allows chain based moves, the lotus series",
    "3 hit combo using legs and fist, ATK x .45, ignores 5percent END",
    "chain assisted arial throw, ATK x .95, ignores 30percent END",
    "Reduces EP cost by -30 cast, -5 per turn, reduces crash penalty",
    "Increases ATK x 2.2, -45 EP cast, -9 per turn",
    "Enhanced 3 hit combo, ignores 10percent END",
    "4-hit finisher, ATK 1.25, ignores 35percent END"
};
string skill_notes[8] = {
    "unlocked by turn 3",
    "Non-combat prerequesite",
    "Core poke attack, CD 2",
    "Requires chains + gate",
    "Auto-upgrade after training",
    "Requires G1 mastery",
    "Stronger Kei-gan barrage",
    "Requires Gate + Front Lotus"
};
int skill_EP_cost[8] = {0, 0, 12, 13, 0, 0, 14, 25};

void displaytraininghistory(int TrainingHistoryCount) {
    for(int i=0; i<TrainingHistoryCount;i++) {
        if (TrainingHistory[i].empty()) {
            cout << "None";
        } else {
            cout << i + 1 << ". "<< TrainingHistory[i] << "," << endl;
        }
    }
};

bool GainSelfDiscovery() {
    cout << "---Skills---" << endl;

    if (SelfDiscovery >= 8) {
        cout << "Discovered all there is to discover!" << endl;
        return false;
    }

    int id = SelfDiscovery;
    SkillsUnlocked[id] = 1;
    SelfDiscovery++;
    cout << "Discovered " << skill_name[id] << " - " << skill_type[id] << " - " << skill_description[id] << " - " << skill_EP_cost[id] << " - " << skill_notes[id] << endl;

    return true;
}
void DisplaySkills() {
    bool none = true;
    cout << "---Discovered Skills---" << endl;
    for(int i=0;i<8;i++) {
        if (SkillsUnlocked[i] == 1) {
            cout << i+1 << ". " << skill_name[i] << "(" << skill_type[i] << ")" << endl;
            none = false;
        }
    }
    if (none) {
        cout << "No skills discovered yet" << endl;
    }
};

//TrainingLoop

void TrainingLoop() {
    int HP_count = 0, ATK_count = 0, END_count = 0, EP_count = 0, Regen_count = 0;
    int turns = 48;
    int TrainingHistoryCount = 0;
    
    for(int i = 0; i < turns; i++) {
        cout << "Turn " << i+1 << " out of 48" << endl;
        cout << "------------------------------" << endl;
        cout << "Present Stats" << endl;
        cout << "HP: " << PlayerStats[StatHP] << "/" << PlayerStats[StatHPMax] << endl;
        cout << "ATK: " << PlayerStats[StatATK] << endl;
        cout << "END: " << PlayerStats[StatEND] << endl;
        cout << "EP: " << PlayerStats[StatEP] << "/" << PlayerStats[StatEPMax] << endl;
        cout << "EP regen: " << PlayerStats[StatEPRegen] << endl;
        cout << "-----TRAINING HISTORY-----" << endl;
        displaytraininghistory(TrainingHistoryCount);

        cout << "Choose (1)HP / (2)ATK / (3)END / (4)EP / (5)EP regen / (6)Self Discovery / (7)Display Skills: ";
        string confirm;
        int choice;
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input! Enter a NUMBER." << endl;
            cin.get(); 
            i--;      
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (choice == 1) {
            int preview = ApplyStatGain(StatHPMax, HP_count + 1);
            cout << "New HP -- +" << preview << endl;
            cout << "Continue? y/n: ";
            getline(cin, confirm);

            if (confirm == "y" || confirm == "Y") {
                HP_count++;
                PlayerStats[StatHPMax] += preview;
                PlayerStats[StatHP] = PlayerStats[StatHPMax];
                TrainingHistory[TrainingHistoryCount] = "HP";
                TrainingHistoryCount++;
                cout << "New HP -- " << PlayerStats[StatHP] << endl;
                cout << "Press enter...";
                cin.get();
                continue;
            } else if (confirm == "n" || confirm == "N") {
                cout << "Cancelling, choose again" << endl;
                cout << "Press enter...";
                cin.get();
                i--;
            } else {
                cout << "Please enter y/n" << endl;
                i--; 
                continue;
            }

        } else if (choice == 2) {
            int preview = ApplyStatGain(StatATK, ATK_count + 1);
            cout << "New ATK -- +" << preview << endl;
            cout << "Continue? y/n: ";
            getline(cin, confirm);

            if (confirm == "y" || confirm == "Y") {
                ATK_count++;
                PlayerStats[StatATK] += preview;
                TrainingHistory[TrainingHistoryCount] = "ATK";
                TrainingHistoryCount++;
                cout << "New ATK -- " << PlayerStats[StatATK] << endl;
                cout << "Press enter...";
                cin.get();
                continue;
            } else if (confirm == "n" || confirm == "N") {
                cout << "Cancelling, choose again" << endl;
                cout << "Press enter...";
                cin.get();
                i--;
            } else {
                cout << "Please enter y/n" << endl;
                i--; 
                continue;
            }

        } else if (choice == 3) {
            int preview = ApplyStatGain(StatEND, END_count + 1);
            cout << "New END -- +" << preview << endl;
            cout << "Continue? y/n: ";
            getline(cin, confirm);

            if (confirm == "y" || confirm == "Y") {
                END_count++;
                PlayerStats[StatEND] += preview;
                TrainingHistory[TrainingHistoryCount] = "END";
                TrainingHistoryCount++;
                cout << "New END -- " << PlayerStats[StatEND] << endl;
                cout << "Press enter...";
                cin.get();
                continue;
            } else if (confirm == "n" || confirm == "N") {
                cout << "Cancelling, choose again" << endl;
                cout << "Press enter...";
                cin.get();
                i--;
            } else {
                cout << "Please enter y/n" << endl;
                i--; 
                continue;
            }

        } else if (choice == 4) {
            int preview = ApplyStatGain(StatEPMax, EP_count + 1);
            cout << "New EP -- +" << preview << endl;
            cout << "Continue? y/n: ";
            getline(cin, confirm);

            if (confirm == "y" || confirm == "Y") {
                EP_count++;
                PlayerStats[StatEPMax] += preview;
                PlayerStats[StatEP] = PlayerStats[StatEPMax];
                TrainingHistory[TrainingHistoryCount] = "EP";
                TrainingHistoryCount++;
                cout << "New EP -- " << PlayerStats[StatEP] << endl;
                cout << "Press enter...";
                cin.get();
                continue;
            } else if (confirm == "n" || confirm == "N") {
                cout << "Cancelling, choose again" << endl;
                cout << "Press enter...";
                cin.get();
                i--;
            } else {
                cout << "Please enter y/n" << endl;
                i--; 
                continue;
            }

        } else if (choice == 5) {
            int preview = ApplyStatGain(StatEPRegen, Regen_count + 1);
            cout << "New EP Regen -- +" << preview << endl;
            cout << "Continue? y/n: ";
            getline(cin, confirm);

            if (confirm == "y" || confirm == "Y") {
                Regen_count++;
                PlayerStats[StatEPRegen] += preview;
                TrainingHistory[TrainingHistoryCount] = "EP Regen";
                TrainingHistoryCount++;
                cout << "New EP Regen -- " << PlayerStats[StatEPRegen] << endl;
                cout << "Press enter...";
                cin.get();
                continue;
            } else if (confirm == "n" || confirm == "N") {
                cout << "Cancelling, choose again" << endl;
                cout << "Press enter...";
                cin.get();
                i--;
            } else {
                cout << "Please enter y/n" << endl;
                i--; 
                continue;
            }

        } else if (choice == 6) {
            if (GainSelfDiscovery()) {
                TrainingHistory[TrainingHistoryCount] = "Self Discovery";
                TrainingHistoryCount++;
            } else {
                cout << "Press enter...";
                cin.get();
                i--;
                continue;
            }
            cout << "Press enter...";
            cin.get();
            continue;

        } else if (choice == 7) {
            DisplaySkills();
            cout << "Press enter...";
            cin.get();
            i--;
        } else {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid Input!" << endl;
            cout << "Press enter...";
            cin.get();
            i--;
        } 
    }
}

int main() {
    
    TrainingLoop();

    return  0;
}