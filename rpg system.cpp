#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <time.h>

using namespace std;

//array creations, array 0 are base stats, array 1-3 are modifiable stats
int players_attack[2] = {6,6};
int players_defense[2] = {5,5};
int players_speed[2] = {8, 8};
int players_health[2] = {20, 20};
int enemy_attack[2] = {9, 9};
int enemy_defense[2] = {8, 8};
int enemy_speed[2] = {5, 5};
int enemy_health[2] = {30, 30};

//aditional initializations as to not repeat them in the loop
bool gamestatehealth, pturn, eturn, has_defended, has_speed = true, has_axe = true, has_potion = true;

// this will be used later to determine wether the game is over or not
void gamestate() {
    //if enemy health is zero, the game will end
    if (enemy_health[1] <= 0) {
        cout << "You have defeated the enemy!\n";
        gamestatehealth = false;
    }
    else {
        //if player health is also zero, the game will end as well
        if (players_health[1] <= 0) {
            cout << "The enemy has defeated you...\n";
            cout << "Game over!\n";
            gamestatehealth = false;
        }
        //else, the game continues
        else {
            gamestatehealth = true;
        }
    }
}

//this will be used to determine crits and misses
int diceroll() {
    int roll = rand() % 20 + 1;
    
    return roll;
}

int attack()
{
    int dmg = rand() % players_attack[1] + 1;

    return dmg;
}

int eattack()
{
    //this is the equation for damage from the enemy to the player
    int dmg = rand() % enemy_attack[1] + 1;
    
    if (has_defended) {
        dmg -= rand() % players_defense[1] + 1; // decrease damage by half if player defended
        has_defended = false; // reset has_defended for next turn
    }
    
    if (dmg < 0) {
        dmg = 0;
        cout << "Enemy's attack was successfully parried\n";
    }
    
    return dmg;
}

//items

int speedp = 0, axe = 0, potion = 0;

void speed_potion()
{
    speedp += 1;
    if (speedp == 2) {
        has_speed = false;
        cout << "You already drank the only speed potion you had!\n";
    }
    players_speed[0] += 2;
    cout << "You feel energized by the effect of the speed potion \n +2 speed \n";
    
}

void battle_axe()
{
    axe += 1;
    if (axe == 2) {
        has_axe = false;
        cout << "You're already holding your axe!\n";
    }
    else {
    players_attack[0-1] += 3;
    cout << "You draw your battle axe from your bag and feel like youll just hit the enemy a little harder\n +3 attack \n";
    }
}

void health_potion()
{
    potion += 1;
    if (potion == 3){
        cout << "You search your bag for another potion, but you seem to have run out of them\n";
        has_potion = false;
    }
    else {
        players_health[1] = players_health[1] + players_health[0] / 2;
        cout << "You drink the potion and feel like you can fight a little longer \n +10 health\n";
        if (players_health[1] > players_health[0]) {
            players_health[1] = players_health[0];
        }
    }
}

void wrong_input() {
    //function to allow for additional inputs
    cout << "please try your input again\n";
}

//this will be used to determine turn order
int order() {
    if (players_speed[1] >= enemy_speed[1]) {
        enemy_speed[1] = enemy_speed[1] + enemy_speed[0];
        return 1;
    }
    else
    {
        players_speed[1] = players_speed[1] + players_speed[0];
        return 0;
    }
}

int main() //first loop to keep the game going as long as at least one of them hits 0hp
{
    srand(time(NULL));
    cout << "You've encountered an enemy!\n";
    do {
        string input, ch;
        int turnorder = order();
        if (turnorder == 1) {
            cout << "\n";
            cout << "Your turn!\n";
            cout << "\n";
            pturn = true;
            eturn = false;
        }
        else
        {
            cout << "\n";
            cout << "Enemy's turn!\n";
            cout << "\n";
            pturn = false;
            eturn = true;
        }
        while (pturn == true) {
            cout << "You have the following actions: \n";
            cout << "attack\n";
            cout << "defend\n";
            cout << "items (view)\n";
            cout << "Type the action to perform it\n";
            cout << "\n";
            cin >> input;
            cout << "\n";
            switch (input[0]) {
                case 'a': {
                    cout << "You rush in to attack!\n";
                    cout << "\n";
                    int roll = diceroll();
                    int dmg = attack();
                    if (roll == 20) {
                        dmg = dmg * 1.5;
                        cout << "critical hit!\n";
                    }
                    else
                        if (roll == 1) {
                            cout << "critical failure! attack missed\n";
                            dmg = 0;
                        }
                    if (enemy_health[1] < 0) {
                        enemy_health[1] = 0;
                    }
                    enemy_health[1] = enemy_health[1] - dmg;
                    cout << "enemy has " << enemy_health[1] << "/" << enemy_health[0] << "hp remaining\n";
                    pturn = false;
                    break;
                }
                case 'd': {
                    if (has_defended) {
                        cout << "You have already defended this turn.\n";
                        pturn = true;
                        break;
                    }
                    players_defense[1] += 2; // increase defense by 2 for the rest of the turn
                    cout << "You prepare to defend.\n";
                    has_defended = true;
                    pturn = false;
                    break;
                }
                case 'i': {
                    cout << "You open your bag and find the following items:\n";
                    cout << "Speed potion: Increase your speed by 2\n";
                    cout << "Battle axe: Increase your attack by 3\n";
                    cout << "Health potion: Increase your health by 50%, max 2 uses\n";
                    cout << "type the name of the item you wish to use. type 'c' to return\n";
                    cin >> ch;
                    switch (ch[0]) {
                        case 's': {
                            if (has_speed ==false) {
                                cout << "You already did this action!\n";
                            }
                            else {
                                speed_potion();
                                pturn = false;
                            }
                            break;
                        }
                        case 'b': {
                            if (has_axe == false) {
                                cout << "You already did this action!\n";
                            }
                            else {
                                battle_axe();
                                pturn = false;
                            }
                            break;
                        }
                        case 'h': {
                            if (has_potion == false) {
                                cout << "You already did this action!\n";
                            }
                            else {
                                health_potion();
                                pturn = false;
                            }
                            break;
                        }
                        case 'c': {
                            cout << "You decide to leave them for later and face the enemy again\n";
                            break;
                        }
                    }
                break;
                }
                default: {
                    pturn = true;
                    wrong_input();
                    break;
                }
            }
        }
        while (eturn == true) {
            cout << "The enemy rushes in to attack!\n";
            int roll = diceroll();
            int dmg = eattack();
            if (roll == 20) {
                dmg = dmg * 1.5;
                cout << "critical hit!\n";
            }
            else
                if (roll == 1) {
                    cout << "critical failure! enemy attack missed\n";
                    dmg = 0;
                }
            players_health[1] = players_health[1] - dmg;
            if (players_health[1] < 0) {
                players_health[1] = 0;
            }
            cout << "You have " << players_health[1] << "/" << players_health[0] << "hp remaining\n";
            eturn = false;
        }
        gamestate();
    } while (gamestatehealth == true);
}