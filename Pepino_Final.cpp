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
    10.0, 50.0, 50.0, 15.0, 5.0, 5.0, 0.0
};

double EnemyStats[EnemyStatCount] {
    520.0, 2500.0, 2500.0, 600, 0.0, 0.0
};

string TrainingHistory[48];

int SkillsUnlocked[8] = {0};

int SelfDiscovery = 0;

//DAMAGE

double CalcDamage(double attackerATK, double DefenderEND);
double EnemyDamage();
double PlayerDamage();
double GetPlayerAtkforDamage();


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

    
// Story Functions and Training Loop    

void Prologue() {

    cout << "Starting Game....." << endl;
    cout << "---------------------------------" << endl;
    cout << "[Opening Scene]" << endl << endl;
    cin.get();
    cout << "Time: 8pm" << endl;
    cout << "*Clicking Keyboard*" << endl;
    cout << "Lee: I have to finish this project,  I can't afford to slack off. (Click enter to continue)" << endl;
    cin.get();
    cout << "*Vision begins to slowly fade*" << endl;
    cout << "Whats happening to me? I'm feeling sooo...." << endl;
    cout << "*Fade to black*" << endl;
    cin.get();
    cout << endl;
    cout << "You arrive in a village found in the northwestern part of East Blue," << endl;
    cout << "this island/village is called Cocoyashi Village" << endl;
    cin.get();
    cout << "Keigan, a quiet and brooding boy whose parents had passed a year ago" << endl;
    cout << "lives with his inheritance under the care of Genzo and Bell-mere." << endl;
    cin.get();
    cout << "On his fifth birthday, Bell-mere brings her daughters, Nojiko and Nami," << endl;
    cout << "to a small home celebration. Keigan has little experience interacting with children his age." << endl;
    cin.get();
    cout << "*Bell-mere teases with a smile*\n";
    cout << "Bell-mere: Keigan, meet Nojiko and Nami!\n";
    cin.get();
    cout << "*Keigan blushes, shyly looking at the girls*\n";
    cout << "Keigan: Uh… h-hi… Im Keigan.\n";
    cin.get();
    cout << "*Nami smiles brightly*\n";
    cout << "Nami: Yes, Keigan-kun?\n";
    cin.get();
    cout << "*Keigan fidgets under the table, voice whispering*\n";
    cout << "Keigan: Would you… uhm… want to be my friend?\n";
    cin.get();
    cout << "*Nami grins and shakes Keigan's hand*\n";
    cout << "Nami: Of course, Keigan-kun! Let's be friends!\n";
    cin.get();
    cout << "*Keigan smiles for the first time since his parents' death*\n";
    cout << "*This marks the start of new friendships and his journey*\n";
    cin.get();
    cout << "[A dream to be great]" << endl;
    cout << "Nami: Keigan, what are you reading?" << endl;
    cout << "Keigan: Maps. Want to see?" << endl;
    cin.get();
    cout << "Nami: Yes! Teach me!" << endl;
    cout << "Keigan: Let's learn together." << endl;
    cin.get();
    cout << "Nami: Someday I want to draw the whole world!" << endl;
    cout << "Keigan: I'll help you!" << endl;
    cin.get();
    cout << endl;   
}

void Act1() {
        cout << "[Act 1: Arlong's Arrival]" << endl << endl;
    cin.get();
    cout << "*Smoke fills the air, the sounds of bombs and fire echoing the entire village*" << endl;
    cin.get();
    cout << "Keigan sees Nami and Nojiko running, fear on their faces." << endl;
    cin.get();
    cout << "Genzo: Quick, hide in the woods!" << endl;
    cin.get();
    cout << "Keigan: Stay close! I won't let anything happen to you!" << endl;
    cin.get();
    cout << "Nojiko: Kei! Be careful!" << endl;
    cin.get();
    cout << "Bell-mere fights Arlong at the front of the house, rifle in hand." << endl;
    cin.get();
    cout << "Bell-mere: You'll have to go through me first!" << endl;
    cin.get();
    cout << "Arlong: Hah! Humans are weak. Do you really think you can stop us?" << endl;
    cin.get();
    cout << "Bell-mere kicks Arlong, but the fishmen just laugh." << endl;
    cin.get();
    cout << "Arlong bites her rifle, grinning: You're powerless against us!" << endl;
    cin.get();
    cout << "At the back, Nako grabs the kids." << endl;
    cin.get();
    cout << "Nako: Run! Escape now while you can!" << endl;
    cin.get();
    cout << "Keigan: No! We can't leave her! Ohana means nobody gets left behind!" << endl;
    cin.get();
    cout << "Nami & Nojiko: Yes! We won't leave her!" << endl;
    cin.get();
    cout << "Bell-mere is beaten, rifle destroyed, bloodied but defiant." << endl;
    cin.get();
    cout << "Bell-mere: My daughters... my family... they come first!" << endl;
    cin.get();
    cout << "Keigan: Bell-mere-okasan! Stay strong!" << endl;
    cin.get();
    cout << "Arlong notices a map in Bell-mere's pocket." << endl;
    cin.get();
    cout << "Arlong: This map… take the girl!" << endl;
    cin.get();
    cout << "Keigan rushes in front of Nami." << endl;
    cin.get();
    cout << "Keigan: Get your hands off her, you fish freak!" << endl;
    cin.get();
    cout << "Arlong: You're just a child. Do you really think you can stop me?" << endl;
    cin.get();
    cout << "Keigan: I don't care! I'd rather die than let you touch her!" << endl;
    cin.get();
    cout << "Arlong punches Keigan three times. He falls, bloodied, but still glares defiantly." << endl;
    cin.get();
    cout << "Keigan: Take me instead! I don't care about the life tax!" << endl;
    cin.get();
    cout << "Arlong: Hah! Brave, little human. Fine, I'll throw you to the ocean!" << endl;
    cin.get();
    cout << "Keigan whispers to Nami as the octopus fishman holds her:" << endl;
    cin.get();
    cout << "Keigan: Nami, survive. Live happily even if I can't. Don't seek revenge." << endl;
    cin.get();
    cout << "Keigan: Na-chan, I loved you since my birthday. You were my happiness." << endl;
    cin.get();
    cout << "Keigan: If I survive, we'll get married." << endl;
    cin.get();
    cout << "Keigan: NAMI, I LOVE YOU!" << endl;
    cin.get();
    cout << "Nami: KEIGAN, I LOVE YOU TOO!" << endl;
    cin.get(); 
    cout << "Arlong hurls Keigan toward the ocean at full force, wind whipping around him." << endl;
    cout << "Keigan is flying, powerless, toward the water at nearly 200 miles per hour." << endl;
    cin.get(); 
    cout << "Keigan should be gone, forever." << endl << endl;
    cin.get(); 
    cout << "right???" << endl;
    }

void Act2() {
    cout << "[Act 2: The Eight Year Training Arc]" << endl << endl;
    cin.get();
    cout << "Watcher: Oi! Something fell from the sky!" << endl;
    cin.get();
    cout << "Watcher: Captain, it's a kid! He's heading for the water!" << endl;
    cin.get();
    cout << "Captain: Good Heart Pirates! Move! We're saving that boy!" << endl;
    cin.get();

    cout << "--- Three Days Later ---" << endl;
    cin.get();
    cout << "Quack: You're finally awake. Good. Eat first, talk later." << endl;
    cin.get();
    cout << "Keigan: ...How long was I out?" << endl;
    cin.get();
    cout << "Quack: Three days. Honestly, kid, you should be dead." << endl;
    cin.get();

    cout << "Keigan: Doc, where are we?" << endl;
    cin.get();
    cout << "Quack: Near an uninhabited island. We'll dock there so you can rest." << endl;
    cin.get();

    cout << "--- On the Island ---" << endl;
    cin.get();
    cout << "Captain: Alright kid, name's not important. We're the Good Heart Pirates." << endl;
    cin.get();
    cout << "Keigan: *cringes slightly*" << endl;
    cin.get();
    cout << "Captain: Yeah, yeah, I know. Dumb name. But we don't hurt innocents." << endl;
    cin.get();
    cout << "Keigan: So what happens to me now?" << endl;
    cin.get();
    cout << "Captain: First, how'd you end up falling from Cocoyashi's direction?" << endl;
    cin.get();
    cout << "Keigan: Because I came from Cocoyashi. Before a tyrant took over." << endl;
    cin.get();
    cout << "*The captain looks at him with pity, pausing for a moment*" << endl;
    cin.get();
    cout << "Live first. Then choose what to do with the life you still have." << endl;
    cin.get();
    cout << "Left alone on the island, Keigan eventually uncovered an old scroll hidden under driftwood—" << endl;
    cin.get();
    cout << "its pages worn thin by salt and time." << endl;
    cin.get();
    cout << "It spoke of a forbidden discipline known as the Eight Inner Gates." << endl;
    cin.get();
    cout << "The method was dangerous, but for him… it was the only direction left." << endl;
    cin.get();
    cout << "From that day, everything he did revolved around training." << endl;
    cin.get();
    cout << "At dawn, he hammered his fists into tree trunks until they numbed," << endl;
    cin.get();
    cout << "by midday he forced his body through cliffs, waves, and sheer exhaustion," << endl;
    cin.get();
    cout << "and when night fell, he sat beneath crashing waterfalls to keep his energy steady." << endl;
    cin.get();
    cout << "Weeks bled into months; months grew into years." << endl;
    cin.get();
    cout << "The island slowly turned into a monument of his struggle—split rocks, marked trees, broken ground." << endl;
    cin.get();
    cout << "The Eight Gates slowly shaped him, testing both his strength and the life he still held on to." << endl;
    cin.get();
}

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

void Act3() {
    cout << "[Act 3: Return to Cocoyashi]" << endl;
    cout << "---------TIME SKIP---------" << endl;
    cout << "-------8 Years Later-------" << endl << endl;
    cin.get();
    cout << "After eight years of grueling training on the island, Keigan sets sail again—hardened, focused, and carrying Bell-mere's faded headband as a promise." << endl;
    cout << "His path leads him to Baratie, the restaurant ship, where laughter and chaos greet him." << endl;
    cin.get();
    cout << "He watches a lively crew move like a storm: the Straw Hat Pirates." << endl;
    cin.get();
    cout << "There he notices a sharp-eyed navigator whose manner and guarded expression stir something in him." << endl;
    cout << "Recognition aches without a name. He keeps his distance." << endl;
    cin.get();
    cout << "The peace is shattered when Don Krieg and his armada attack." << endl;
    cout << "In the chaos, Dracule Mihawk appears, Zoro challenges him, and the fight ends with Zoro beaten but unbroken." << endl;
    cin.get();
    cout << "During the scramble, the navigator and her crew steal away aboard the Going Merry." << endl;
    cout << "Keigan hears her name for the first time—Nami—spoken by the fleeing pirates." << endl;
    cin.get();
    cout << "Keigan (quiet, to himself): Nami…?" << endl;
    cin.get();
    cout << "The Straw Hats decide to pursue. Luffy and Sanji remain at Baratie for their own reasons, while Zoro, Usopp, Yosaku, and others give chase." << endl;
    cin.get();
    cout << "Keigan, whose gut tells him where Nami will head, volunteers to join the pursuit." << endl;
    cout << "He rides with the group toward the Conomi Islands, steering them by instincts buried for eight years." << endl;
    cin.get();
    cout << "The group reaches Cocoyashi Village, finding a village hollowed by fear." << endl;
    cout << "They meet Nojiko, who—together with Luffy and Sanji (who arrive with the Baratie party) \nexplains Nami's past: Bell-mère's death, the tribute, Arlong's rule, and Nami's forced servitude." << endl;
    cin.get();
    cout << "Nojiko mentions a name from the old days—Keigan." << endl;
    cin.get();
    cout << "Usopp: You—Keigan—the one Arlong tossed in the water?" << endl;
    cout << "Zoro: And you survived. Not many would." << endl;
    cout << "Luffy (to Keigan): So you're here to help us?" << endl;
    cout << "Keigan (soft): I promised I would come back. I'm here to end it." << endl;
    cin.get();
    cout << "Keigan still doesn't confront Nami immediately; the recognition comes later, in the heat of the battle with Arlong." << endl;
    cout << "Nojiko's telling anchors the crew's purpose: this fight is about freeing Cocoyashi and saving Nami." << endl;
    cin.get();
    cout << "United, the Straw Hats and Keigan move toward Arlong Park." << endl;
    cin.get();
    cout << "The corrupt local Marines and Fishman sentries try to block them, but the crew cuts through the bureaucracy with blunt force and blunt words." << endl;
    cin.get();
    cout << "As they approach Arlong Park, the gates loom like a promise and a threat: everything Keigan trained for every two months, every year, has built to this single moment." << endl;
    cin.get();
    cout << "Keigan (quiet, under his breath): This time I bring what I should have had eight years ago." << endl;
    cin.get();
    cout << "The encounter begins. Nami, the villagers, and the pirates brace for the storm." << endl;
    cin.get();

}

void StartBattle() {
    cout << "[StartBattle: Approach to Arlong Park]" << endl << endl;
    cin.get();

    cout << "Keigan steps forward, eyes burning with a familiar fire, staring at Arlong." << endl;
    cout << "Keigan: Long time no see, Arlong. Or should I say… Arlong time no see, fish freak." << endl;
    cin.get();

    cout << "Arlong sneers: You? Who are you supposed to be? Some weak little brat from the past?" << endl;
    cin.get();

    cout << "Keigan: Same brat who promised Nami-chan I'd end you. And I don't break promises." << endl;
    cin.get();

    cout << "Ard lunges at Keigan, but Keigan sidesteps effortlessly." << endl;
    cin.get();

    cout << "Zoro, Sanji, and Usopp face their opponents, tension rising." << endl;
    cin.get();

    cout << "Luffy, freed from the stone by Genzo, emerges from the water, determination blazing." << endl;
    cin.get();

    cout << "Keigan: Arlong. This ends with just you and me." << endl;
    cin.get();

    cout << "Arlong smirks, acknowledging the seriousness in Keigan's eyes." << endl;
    cout << "Arlong: So be it. Let's see if you've grown beyond a memory." << endl;
    cin.get();

    cout << "Keigan plants his feet, fists ready." << endl;
    cout << "Keigan: I promised Nami-chan. I'll keep that promise no matter what." << endl;
    cin.get();

    cout << "The crew braces. Villagers watch, hope rising." << endl;
    cout << "Zoro: Keep them busy. Don't let Luffy drown again." << endl;
    cout << "Sanji nods silently, eyes locked on his opponent." << endl;
    cin.get();

    cout << "Keigan inhales deeply, focusing on Arlong." << endl;
    cout << "Keigan: Enough talk. Let's finish this." << endl;
    cin.get();

    cout << "Everything seems to pause… tension is electric." << endl;
    cin.get();

    cout << "--------KEIGAN VS ARLONG--------" << endl;
    cout << "Now begins the full Keigan vs Arlong fight sequence." << endl;
    cin.get();
};

void Act4() {
        cout << "[Act 4: Cocoyashi's Freedom and Beyond - Interactive]" << endl << endl;
        cin.get();

        // Scene 1: Nami and Keigan's Freedom
        cout << "*The dust clears over Arlong Park, smoke curling into the sky, buildings collapsing in the background*" << endl;
        cin.get();
        cout << "Villagers emerge, chains broken. Tears of joy and laughter fill the air." << endl;
        cin.get();
        cout << "Keigan collapses from exhaustion after opening the Gates." << endl;
        cin.get();
        cout << "Nami rushes to him, panic in her eyes:" << endl;
        cin.get();
        cout << "Nami: Kei! Stay with me! Don't you dare leave me here!" << endl;
        cin.get();
        
        int choice1;
        cout << "Keigan, what do you say?" << endl;
        cout << "1. I will survive, for you and everyone!" << endl;
        cout << "2. I am done… everything is over…" << endl;
        cin >> choice1;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if(choice1 == 1) {
            cout << "Keigan: I will survive, for you and everyone!" << endl;
            cout << "Nami: *smiling through tears* You better!" << endl;
        } else {
            cout << "Keigan: I am done… everything is over…" << endl;
            cout << "Nami: No! You are not allowed to give up now!" << endl;
        }
        cin.get();

        cout << "Luffy lifts Keigan up effortlessly, grinning." << endl;
        cin.get();
        cout << "Luffy: Hahaha! You are alive! Let us celebrate later, but first, get on your feet!" << endl;
        cin.get();
        
        // Scene 2: Nami and Keigans Reunion at Bell-meres grave
        cout << "*Morning after the battle, Bell-mère's grave*" << endl;
        cin.get();
        cout << "Nami places tangerine blossoms on the grave." << endl;
        cin.get();

        int choice2;
        cout << "Keigan, what do you whisper to Bell-mère?" << endl;
        cout << "1. We did it, Bell-mère. Your daughters are safe." << endl;
        cout << "2. I couldnt protect everyone, but I will try from now on." << endl;
        cin >> choice2;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if(choice2 == 1) {
            cout << "Keigan: We did it, Bell-mère. Your daughters are safe." << endl;
        } else {
            cout << "Keigan: I couldnt protect everyone, but Ill try from now on." << endl;
        }
        cin.get();
        
        cout << "Nojiko teases softly:" << endl;
        cin.get();
        cout << "Nojiko: Kei, dont get too sappy! Namis blushing!" << endl;
        cin.get();

        // Scene 3: Nami's Farewell
        cout << "*Three days later, festival winds down*" << endl;
        cin.get();
        cout << "Doctor Nako checks Namis tattoo. Keigan and Genzo walk to the grave one last time." << endl;
        cin.get();

        int choice3;
        cout << "Keigan, how do you feel about Nami leaving with the Straw Hats?" << endl;
        cout << "1. Proud and happy for her." << endl;
        cout << "2. Nervous and worried." << endl;
        cin >> choice3;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if(choice3 == 1) {
            cout << "Keigan: She deserves this… Im happy for her." << endl;
        } else {
            cout << "Keigan: I hope she stays safe.. I cant help but worry." << endl;
        }
        cin.get();
        
        cout << "Luffy: Alright, everyone! The ship wont wait!" << endl;
        cin.get();
        
        // Scene 4: Luffy Recruits Keigan
        cout << "*Cocoyashi docks, the Going Merry ready to sail*" << endl;
        cin.get();
        cout << "Luffy points at Keigan:" << endl;
        cin.get();
        cout << "Luffy: Hey! You! Come sail with us! Youre strong!" << endl;
        cin.get();

        int choice4;
        cout << "Keigan, do you accept Luffys invitation?" << endl;
        cout << "1. Yes, Ill sail with you!" << endl;
        cout << "2. No, I need to stay and protect my village." << endl;
        cin >> choice4;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if(choice4 == 1) {
            cout << "Keigan: Alright... Ill sail with you!" << endl;
            cout << "Crew: Yay! Welcome aboard, Jet Keigan!" << endl;
        } else {
            cout << "Keigan: I must stay and protect my home for now." << endl;
            cout << "Luffy: Hahaha, your choice, but the sea will always call!" << endl;
        }
        cin.get();

        // Scene 5: Bounty Reveal
        cout << "*Days later, aboard the Going Merry*" << endl;
        cin.get();
        cout << "Nami flips through the newspaper; wanted posters slip out. Keigan catches them." << endl;
        cin.get();
        cout << "Keigan sees the posters: *Straw Hat* Luffy — 30,000,000 Beli, *Jet* Lee Keigan — 5,000,000 Beli" << endl;
        cin.get();

        int choice5;
        cout << "Keigan, how do you react to your bounty?" << endl;
        cout << "1. Laugh it off with the crew." << endl;
        cout << "2. Take it seriously, time to prepare." << endl;
        cin >> choice5;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if(choice5 == 1) {
            cout << "Keigan: Hahaha! Looks like Jets got a price on his head now!" << endl;
            cout << "Usopp: Careful, Jet! Soon the Marines will be after you!" << endl;
        } else {
            cout << "Keigan: A bounty of 5,000,000… I need to be careful." << endl;
            cout << "Luffy: Hah! Careful or not, youre one of us!" << endl;
        }
        cin.get();

        cout << "*The crew laughs, sails catch the wind, and a new adventure begins for Keigan and the Straw Hats.*" << endl;
        cin.get();
}

void AlternateRoute() {
    cout << "[Act 4: A Fading Dream - Keigans Defeat]" << endl << endl;
    cin.get();

    cout << "*The sea churns violently, waves crashing against each other*" << endl;
    cin.get();
    cout << "Keigan is hurled through the air, powerless, the wind screaming past him." << endl;
    cin.get();
    cout << "*Splash!* The ocean swallows him, darkness pressing in from all sides." << endl;
    cin.get();
    cout << "Everything goes silent… the world fades to black." << endl;
    cin.get();

    cout << "*A faint light pierces the darkness. Keigan blinks.*" << endl;
    cin.get();
    cout << "He opens his eyes to a familiar scene: the sun shining over Cocoyashi Village, peaceful and whole." << endl;
    cin.get();
    cout << "Bell-mère's house stands firm, the streets quiet. No signs of Arlongs terror anywhere." << endl;
    cin.get();
    cout << "*Keigan looks down at his small hands and realizes he is eight years old again.*" << endl;
    cin.get();

    cout << "Keigan (to himself, whispering): Was it… all a dream? Can I really defeat Arlong?" << endl;
    cin.get();
    cout << "*The village seems perfect, but a heavy, unshakable dread lingers in the air.*" << endl;
    cin.get();
    cout << "Keigan: Something… isnt right." << endl;
    cin.get();
    cout << "*The vision dissolves into silence, leaving only the sound of his heartbeat echoing in his ears.*" << endl;
    cin.get();

    cout << "*Keigan breathes deeply, determination flaring in his eyes.*" << endl;
    cin.get();
    cout << "Keigan: If this is a dream… then Ill make it reality. I will protect everyone. I will defeat him." << endl;
    cin.get();
    cout << "*Fade out.*" << endl;
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
        AlternateRoute();
        
    } else if (EnemyStats[EStatHP] <= 0) {
        cout << "You have defeated Arlong!" << endl << endl;
        Act4();
    }
}



//Initializing of Game

void initGame() {
    Prologue();
    Act1();
    Act2();
    TrainingLoop();
    Act3();
    StartBattle();
    BattleLoop();
};    

//Menu

int main() {
    int choices;

    while(true) {
        cout << "WELCOME TO Keigan's Journey -- The Cocoyashi Arc" << endl;
        cout << "------------------------------------------------" << endl;
        cout << "------------------MAIN MENU---------------------" << endl;
        cout << "1. Start Game" << endl;
        cout << "2. Quit" << endl;
        cout << "Choice: ";
        cin >> choices;

        if (choices==1) {
            initGame();
        }
        
        else if (choices==2) {
            break;
        }
        else {
            cout << "Invalid Choice" << endl;
        }
       
    }
    return 0;
}
