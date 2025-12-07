#include <iostream>
#include <string>
#include <iomanip>
#include <limits>
#include <cmath>
#include <algorithm>
using namespace std;

//MODIFIED THE DIFFERENT VALUES OF PLAYER TO SIMULATE BATTLELOOP




//Structures and variables

const int PlayerStatCount = 7;

const int StatATK = 0;
const int StatHP = 1;
const int StatHPMax = 2;
const int StatEND = 3;
const int StatEP = 4;
const int StatEPMax = 5;
const int StatEPRegen = 6;

//Enemy

const int EnemyStatCount = 6;

const int EStatATK = 0;
const int EStatHP = 1;
const int EStatHPMax = 2;
const int EStatEND = 3;
const int EStatPHASE = 4;
const int EStatStaggered = 5;

//Player  and Enemy Stats

double PlayerStats[PlayerStatCount] {
    500.0, 500.0, 500.0, 500.0, 500.0, 500.0, 500.0
};

double EnemyStats[EnemyStatCount] {
    520.0, 2500.0, 2500.0, 600, 0.0, 0.0
};

string TrainingHistory[48];

int SkillsUnlocked[8] = {0};

void unlockedall() {
    for (int i=0;i<8;i++) {
        SkillsUnlocked[i] = 1;
    }
    cout << "unlocked all";
}



int SelfDiscovery = 0;

//DAMAGE

double CalcDamage(double attackerATK, double DefenderEND);
double EnemyDamage();
double PlayerDamage();
double GetPlayerAtkforDamage();

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

//BATTLELOOP VARIABLES AND FUNCTIONS

//Status VARIABLES AND FUNCTIONS

const int MaxStatus = 30;
string StatusName[MaxStatus] = {""};
int StatusTurnsLeft[MaxStatus] = {0};
double StatusValue[MaxStatus];


void Addstatus(string name, int turns, double value = 0.0) {
    for (int i=0;i<MaxStatus;i++) {
        if (StatusTurnsLeft[i] == 0) {
            StatusName[i] = name;
            StatusTurnsLeft[i] = turns;
            StatusValue[i] = value;
            cout << name << "applied for " << turns << " turns." << endl;
            return;
        } 
    }
    cout << "No slots available" << endl;
}

bool StatusActive(string name) {
    for (int i=0; i<MaxStatus; i++) {
        if (StatusTurnsLeft[i] > 0 && StatusName[i] == name) {
            return true;
        }
    }
    return false;
}

void UpdateStatus() {
    for (int i = 0;i < MaxStatus;i++) {
        if (StatusTurnsLeft[i] > 0) {
            StatusTurnsLeft[i]--;
            if (StatusTurnsLeft[i] == 0) {
                cout << StatusName[i] << " expired" << endl;
                StatusName[i] = "";
                StatusValue[i] = 0;
            }
        } 
        
    }
}

int StatusRemaining(string name) {
    for (int i=0; i<MaxStatus;i++) {
        if (StatusTurnsLeft[i]>0 && StatusName[i]==name) {
            return StatusTurnsLeft[i];
        }
    }
    return 0;
}

void RemoveStatus(string name) {
    for (int i=0;i< MaxStatus; i++) {
         if (StatusTurnsLeft[i] > 0 && StatusName[i] == name) {
           
            for (int j = i; j < MaxStatus - 1; ++j) {
                StatusName[j] = StatusName[j + 1];
                StatusTurnsLeft[j] = StatusTurnsLeft[j + 1];
                StatusValue[j] = StatusValue[j + 1];
            }
            
            StatusName[MaxStatus - 1] = "";
            StatusTurnsLeft[MaxStatus - 1] = 0;
            StatusValue[MaxStatus - 1] = 0.0;
            return;
        }
    }
}

//GATE FUNCTIONS

bool Gateactive = false;
int CurrentGate = 0;
bool ChainHandling = false;
bool GateMastery = false;


void GateActivate1() {
    if (CurrentGate >= 1) {
        cout << "gate already activated" << endl;
        return;
    }

    int epCost = GateMastery ? 30 : 35;
    int epDrain = GateMastery ? 5 : 7;

    if (PlayerStats[StatEP] < epCost) {
        cout << "not enough EP" << endl;
        return;
    }

    PlayerStats[StatEP] -= 35;

    Gateactive = true;
    CurrentGate = 1;

    Addstatus("Gate1Drain", epDrain);

    cout << "GATE OPENED! ATK 1.8 -- EP: 35" << endl;
}

void GateActivate2() {
    if (!GateMastery) {
        cout << "needs G1 Mastery!" << endl;
        return;
    }
    if (CurrentGate != 1) {
        cout <<"requires G1 to be open to access this gate!" << endl;
        return;
    }
    if (PlayerStats[StatEP] < 10) {
        cout << "not enough EP" << endl;
        return;
    }
    PlayerStats[StatEP] -= 10;
    RemoveStatus("Gate1Drain");
    Addstatus("Gate2Drain", 999);
    CurrentGate = 2;
    
    cout << "Gate of Healing Activated!" << endl;
    cout << "Your power grows at a massive level!" << endl;


}

void CheckChainHandling() {
    if (SkillsUnlocked[2] == 1) {
        ChainHandling = true;
    }
}

void GateCheckMastery() {
    if (SkillsUnlocked[5] == 1) {
        GateMastery = true;
    }
}



void DeactivateGate(bool crashed) {
    RemoveStatus("Gate1Drain");
    RemoveStatus("Gate2Drain");
    Gateactive = false;

    double PenaltyCrash = GateMastery ? 1.4 : 1.5;
    
    if (crashed) {
        PlayerStats[StatATK] = max(1.0, PlayerStats[StatATK] / PenaltyCrash);
        PlayerStats[StatEND] = max(1.0, PlayerStats[StatEND] / PenaltyCrash);
        Addstatus("GateCrashPenalty", 1, 0.0);
        cout << "Gate Crashed!" << endl;
    } else {
        cout << "Gate closed safely" << endl;
    }

    CurrentGate = 0;
    
}

// PLAYER Damage

double CalcDamage(double attackerATK, double DefenderEND) {
    double raw = (attackerATK - DefenderEND) / 2.0; 
    if (raw < 1.0) raw = 1.0;
    return max(1.0, ceil(raw));
}

double GetPlayerAtkforDamage() {
    double atk = PlayerStats[StatATK];
    if (CurrentGate == 1) atk *= 1.8;
    if (CurrentGate == 2) atk *= 2.2;
    if (StatusActive("GateCrashPenalty")) atk /= 1.5;
    return atk;
}

double EnemyDamage() {
    double Damage = max(1.0, ceil((EnemyStats[EStatATK] - PlayerStats[StatEND]) / 2.0));
    PlayerStats[StatHP] -= Damage;
    return Damage;
}

double PlayerDamage() {
    double atk = GetPlayerAtkforDamage();
    double damage = CalcDamage(atk, EnemyStats[EStatEND]);
    EnemyStats[EStatHP] -= damage;
    return damage;
}



//Turn Start Functions

void PlayerEPRegen() {
    cout << "Increasing EP by " << PlayerStats[StatEPRegen] << endl;
    PlayerStats[StatEP] += PlayerStats[StatEPRegen];
    if (PlayerStats[StatEP] > PlayerStats[StatEPMax]) {
        PlayerStats[StatEP] = PlayerStats[StatEPMax];
    }
    cout << "New EP: " << PlayerStats[StatEP] << "/" << PlayerStats[StatEPMax] << endl;
}

void EnemyPhase() {
    if(EnemyStats[EStatHP] >= 1601) {
        cout << "Arlong: YOU ARE NO MATCH FOR ME WEAKLING!\n";
        EnemyStats[EStatATK] = 550;
    } else if (EnemyStats[EStatHP] < 1600) {
        cout << "Arlong: Very well weakling, you have the privilige TO FEEL MY BLADE!\n";
        EnemyStats[EStatATK] = 600;
    }else if (EnemyStats[EStatHP] < 900) {
        cout << "Arlong: YOU BASTARD! I will crush you with my bare hands\n";
        EnemyStats[EStatEND] = 660;
    } else if (EnemyStats[EStatHP] < 200) {
        cout << "Arlong: IMPOSSIBLE, how are you doing this???\n";
    }
        
}

void GateDrain() {
    if (!Gateactive) return;
    if (CurrentGate == 1) {
        PlayerStats[StatEP] -= 7;
        if (PlayerStats[StatEP] <= 0) {
            cout << "Out of EP, Gate Crashed!";
            DeactivateGate(true);
            
        }
    } else if (CurrentGate == 2) {
        PlayerStats[StatEP] -= 9;
        if (PlayerStats[StatEP] <= 0) {
            cout << "Out of EP, Gate Crashed!";
            DeactivateGate(true);
            
        }
    }
}

void TurnStart () {
    EnemyPhase();
    PlayerEPRegen();
    GateDrain();
    
}

//SKILLS

void keiganbarrage() {
    
    if(StatusActive("Barrage")) {
        int turns = StatusRemaining("Barrage");
        cout << "Cooldown! " << turns << " turns remaining" << endl;
        return;
    }
    if (PlayerStats[StatEP] < 12) {
        cout << "Not enough EP" << endl;
        return;
    }

    PlayerStats[StatEP] -= 12;

    cout << "3 hit combo KEIGAN BARRAGE!" << endl;
    cout << "Click Enter...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();

    for(int i=0;i<3;i++) {
        double damage = max(1.0, ceil((PlayerStats[StatATK] * 0.45) - (EnemyStats[EStatEND] * 0.05)));
        EnemyStats[EStatHP] -= damage;
        cout << "Hit " << i+1 << ": " << damage << " Damage" << endl;
    };
    Addstatus("UsedBarrage", 1);

    Addstatus("Barrage", 2);
};

void frontlotus() {
    if (!Gateactive) {
        cout << "requires Gate to be active" << endl;
        return;
    }
    if (!StatusActive("UsedBarrage")) {
        cout << "requires barrage first" << endl;
        return;
    }
    if (!ChainHandling) {
        cout << "requires chain handling to use" << endl;
        return;
    }

    cout << "3 hit combo FRONT LOTUS!" << endl;
    cout << "Click Enter...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();

    for (int i=0;i<3;i++) {
        double damage = max(1.0, ceil((PlayerStats[StatATK] * 0.95) - (EnemyStats[EStatEND] * 0.3)));
        EnemyStats[EStatHP] -= damage;
        cout << "Hit " << i+1 << ": " << damage << " Damage" << endl;
    };

    Addstatus("FrontLotusReady", 1);
    RemoveStatus("UsedBarrage");

}
void ChainBarrage() {
    if(StatusActive("CBarrage")) {
        int turns = StatusRemaining("CBarrage");
        cout << "Cooldown! " << turns << " turns remaining" << endl;
        return;
    }
    if (PlayerStats[StatEP] < 14) {
        cout << "Not enough EP" << endl;
        return;
    }

    cout << "Using CHAIN BARRAGE!";
    cout << "Press Enter...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();

    PlayerStats[StatEP] -= 14;
    for (int i=0; i<3;i++) {
        double damage = max(1.0, ceil((PlayerStats[StatATK] * 0.5) - (EnemyStats[EStatEND] * 0.1)));
        EnemyStats[EStatHP] -= damage;
        cout << "Hit " << i+1 << ": " << damage << " Damage!" << endl;
    }

    Addstatus("UsedCBarrage", 1);
    Addstatus("CBarrage", 2);
}

void reverseLotus() {

    if (!Gateactive) {
        cout << "Reverse Lotus requires a Gate to be active!" << endl;
        return;
    }

    if (!StatusActive("FrontLotusReady")) {
        cout << "Reverse Lotus requires using Front Lotus first!" << endl;
        return;
    }

    if (StatusActive("Cooldown_ReverseLotus")) {
        cout << "Reverse Lotus is on cooldown (" << StatusRemaining("Cooldown_ReverseLotus") << " turns left)!" << endl;
        return;
    }

    if (PlayerStats[StatEP] < 25) {
        cout << "Not enough EP!" << endl;
        return;
    }
    PlayerStats[StatEP] -= 25;

    int hits = 4;
    if (CurrentGate == 2) hits++;

    cout << "REVERSE LOTUS!!! " << hits << " hit finisher!" << endl;
    cout << "Press Enter...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();

    for (int i = 0; i < hits; i++) {
        double atk = GetPlayerAtkforDamage() * 1.25;
        double end = EnemyStats[EStatEND] * 0.65;

        double dmg = CalcDamage(atk, end);
        EnemyStats[EStatHP] -= dmg;

        cout << "Hit " << (i+1) << ": " << dmg << " Damage!" << endl;
    }

    Addstatus("Cooldown_ReverseLotus", 5);

    RemoveStatus("FrontLotusReady");
}

// PLAYER ENEMY TURN

void PlayerTurn(int currentturn) {
    while (true) {
        int choice;
        
        cout << "Basic Attack(1), Use Skill(2), Toggle Gate(3): ";
            cin >> choice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (choice == 1) {
                double damagedealt = PlayerDamage();
                cout << "You have attacked!" << endl;
                cout << "You have dealt " << damagedealt << " damage!" << endl;
                cout << "Press enter...";
                cin.get();

                
                break;
                
            } else if (choice == 2) {
                if (SkillsUnlocked[2]) cout << "1. Keigan Barrage" << endl;
                if (SkillsUnlocked[3]) cout << "2. Front Lotus" << endl;
                if (SkillsUnlocked[6]) cout << "3. Chain Barrage" << endl;
                if (SkillsUnlocked[7]) cout << "4. Reverse Lotus" << endl;
                int skillchoice;
                    cout << "Choose Skill: ";
                    cin >> skillchoice;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                switch(skillchoice) {
                    case 1: keiganbarrage(); break;
                    case 2: frontlotus(); break;
                    case 3: ChainBarrage(); break;
                    case 4: reverseLotus(); break;
                    default: cout << "invalid" << endl; break;
                }

                break;

            } else if (choice == 3) {
                if (currentturn < 3) {
                    cout << "Cannot use until turn 3" << endl;
                    cout << "Click enter..";
                    cin.get();
                    return;
                }
                
                if (!Gateactive) {
                    GateActivate1();        
                    cout << "Activated Gate 1!" << endl;
                } else if (CurrentGate == 1 && GateMastery) {
                    GateActivate2();          
                    cout << "Activated Gate 2!" << endl;
                } else {
                    DeactivateGate(false);    
                    cout << "Manually closed Gate!" << endl;
                }
                cin.get();
                break;
            } else {
                cout << "invalid choice!" << endl;
                return;
                
            };
    }

}

void EnemyTurn() {
    cout << "You are no match for me!" << endl;
    double damagedealt = EnemyDamage();
    cout << "Arlong has dealt " << damagedealt << " damage!" << endl;
    cout << "New health " << PlayerStats[StatHP] << endl;
    cout << "Press enter...";
    cin.get();
}

//BATTLELOOP

void BattleLoop() {

    int turns=0;
    GateCheckMastery();
    CheckChainHandling(); 

    while (PlayerStats[StatHP] > 0 && EnemyStats[EStatHP] > 0) {
       //start turn
       cout << endl << endl;
        turns++;
        int choice;
        cout << "Turn --- " << turns << " ---" << endl;
        cout << "Keigan health: " << PlayerStats[StatHP] << "/" << PlayerStats[StatHPMax] << endl;
        cout << "Arlong health: " << EnemyStats[EStatHP] << "/" << EnemyStats[EStatHPMax] << endl;
    
        TurnStart();
      
        //Player
        PlayerTurn(turns);

        if(EnemyStats[EStatHP] <= 0) {
        break;
        }
        
       //Enemy
       EnemyTurn();

       if(PlayerStats[StatHP] <= 0) {
        cout << "You were defeated by Arlong!";
        break;
       }

       //End
       cout << endl << endl;
       cout << "------DIFFERENT EFFECTS------" << endl << endl;

       UpdateStatus();
    };

    if (PlayerStats[StatHP] <= 0) {
        cout << "You were defeated by Arlong!" << endl << endl;
        //AlternateEnding
        
    } else if (EnemyStats[EStatHP] <= 0) {
        cout << "You have defeated Arlong!" << endl << endl;
        //NormalEnding
    }
}

//main

int main() {
    unlockedall();
    BattleLoop();
    
    return 0;
}