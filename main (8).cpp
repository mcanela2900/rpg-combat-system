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

// this will be used later to determine wether the game is over or not
int gamestate() {
    //if enemy health is zero, the game will end
    if (enemy_health[1] <= 0) {
        return 0;
    }
    else {
        //if player health is also zero, the game will end as well
        if (players_health[1] <= 0) {
            return 0;
        }
        //else, the game continues
        else {
            return 1;
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
    // this is the equation for damage from the player to the enemy
    int dmg = rand() % players_attack[1] + 1;

    return dmg;
}

int eattack()
{
    //this is the equation for damage from the enemy to the player
    int dmg = rand() % enemy_attack[1] + 1;
    
    return dmg;
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
//aditional initializations as to not repeat them in the loop
bool gamestatehealth = true, pturn, eturn;
int main() //first loop to keep the game going as long as at least one of them hits 0hp
{
    srand(time(NULL));
    do {
        string input;
        int turnorder = order();
        if (turnorder == 1) {
            pturn = true;
            eturn = false;
        }
        else
        {
            pturn = false;
            eturn = true;
        }
        while (pturn == true) {
            cin >> input;
            switch (input[0]) {
                case 'a': {
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
                    enemy_health[1] = enemy_health[1] - dmg;
                    cout << "enemy has " << enemy_health[1] << "/" << enemy_health[0] << "hp remaining\n";
                    pturn = false;
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
            cout << "You have " << players_health[1] << "/" << players_health[0] << "hp remaining\n";
            eturn = false;
        }
        if (gamestate() == 1) {
            gamestatehealth = true;
        }
        else {
            gamestatehealth = false;
        }
    } while (gamestatehealth == true);
}