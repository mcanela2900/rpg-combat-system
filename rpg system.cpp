#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

using namespace std;

//array creations, array 0 are base stats, array 1-3 are modifiable stats
int players_attack[2] = { 6,6 };
int players_defense[2] = { 5,5 };
int players_speed[2] = { 8, 8 };
int players_health[2] = { 20, 20 };
int enemy_attack[2] = { 9, 9 };
int enemy_defense[2] = { 8, 8 };
int enemy_speed[2] = { 5, 5 };
int enemy_health[2] = { 30, 30 };

//aditional initializations as to not repeat them in the loop
bool gamestatehealth, pturn, eturn, has_defended = false, ehas_defended = false, has_speed = true, has_axe = true, has_potion = true;

void ChangeColor(char color)
{
    switch (color)
    {
    case '0': SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);  break;
    case '1': SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);  break;
    case '2': SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);  break;
    case '3': SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 3);  break;
    case '4': SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);  break;
    case '5': SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 5);  break;
    case '6': SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 6);  break;
    case '7': SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);  break;
    case '8': SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);  break;
    case '9': SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);  break;
    case 'a': SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10); break;
    case 'b': SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11); break;
    case 'c': SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12); break;
    case 'd': SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13); break;
    case 'e': SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14); break;
    case 'f': SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15); break;
    default:  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);  break;
    }
}
// this will be used later to determine wether the game is over or not
void gamestate() {
    //if enemy health is zero, the game will end
    if (enemy_health[1] <= 0) {
        ChangeColor('a');
        cout << "You have defeated the enemy!\n";
        gamestatehealth = false;
    }
    else {
        //if player health is also zero, the game will end as well
        if (players_health[1] <= 0) {
            ChangeColor('4');
            cout << "The enemy has defeated you...\n";
            ChangeColor('4');
            cout << "Game over!\n";
            gamestatehealth = false;
        }
        //else, the game continues
        else {
            gamestatehealth = true;
            ChangeColor('8');
            cout << "~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.~.";
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
    int roll = diceroll();
    if (roll == 20) {
        dmg = dmg * 1.5;
        ChangeColor('c');
        cout << "critical hit!\n";
    }
    else
        if (roll == 1) {
            ChangeColor('4');
            cout << "critical failure! attack missed\n";
            dmg = 0;
        }
    if (ehas_defended) {
        enemy_defense[1] -= dmg; // decrease damage by half if player defended
        ChangeColor('4');
        cout << "You've hit the enemy's armor, it has " << enemy_defense[1] << " durability remaining\n";
        dmg -= enemy_defense[0] - enemy_defense[1];
        ehas_defended = false; // reset has_defended for next turn
    }

    if (dmg < 0) {
        dmg = 0;
        ChangeColor('1');
        cout << "Your attack was successfully parried by the enemy\n";
    }

    return dmg;
}

int eattack()
{
    //this is the equation for damage from the enemy to the player
    int dmg = rand() % enemy_attack[1] + 1;
    int roll = diceroll();
    if (roll == 20) {
        dmg = dmg * 1.5;
        ChangeColor('4');
        cout << "critical hit!\n";
    }
    else
        if (roll == 1) {
            ChangeColor('c');
            cout << "critical failure! enemy attack missed\n";
            dmg = 0;
        }
    if (has_defended) {
        players_defense[1] -= dmg;
        dmg -= players_defense[0] - players_defense[1];
        players_defense[0] = players_defense[1];
        ChangeColor('1');
        cout << "The enemy has hit your armor, it has " << players_defense[1] << " durability remaining\n";
        has_defended = false; // reset has_defended for next turn
    }

    if (dmg < 0) {
        dmg = 0;
        ChangeColor('3');
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
        ChangeColor('e');
        cout << "You already drank the only speed potion you had!\n";
    }
    players_speed[0] += 2;
    ChangeColor('e');
    cout << "You feel energized by the effect of the speed potion \n";
    ChangeColor('3');
    cout<<" + 2 speed \n";

}

void battle_axe()
{
    axe += 1;
    if (axe == 2) {
        has_axe = false;
        ChangeColor('e');
        cout << "You're already holding your axe!\n";
    }
    else {
        players_attack[0 - 1] += 3;
        ChangeColor('e');
        cout << "You draw your battle axe from your bag and feel like youll just hit the enemy a little harder\n";
        ChangeColor('c');
        cout<<" + 3 attack \n";
    }
}

void health_potion()
{
    potion += 1;
    if (potion == 3) {
        ChangeColor('e');
        cout << "You search your bag for another potion, but you seem to have run out of them\n";
        has_potion = false;
    }
    else {
        players_health[1] = players_health[1] + players_health[0] / 2;
        ChangeColor('e');
        cout << "You drink the potion and feel like you can fight a little longer \n";
        ChangeColor('d');
        cout<<" + 10 health\n";
        if (players_health[1] > players_health[0]) {
            players_health[1] = players_health[0];
        }
    }
}

void wrong_input() {
    //function to allow for additional inputs
    ChangeColor('e');
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
    ChangeColor('9');
    cout << "You've encountered an enemy!\n";
    do {
        string input, ch;
        int turnorder = order();
        if (turnorder == 1) {
            cout << "\n";
            ChangeColor('2');
            cout << "Your turn!\n";
            cout << "\n";
            pturn = true;
            eturn = false;
        }
        else
        {
            cout << "\n";
            ChangeColor('8');
            cout << "Enemy's turn!\n";
            cout << "\n";
            pturn = false;
            eturn = true;
        }
        while (pturn == true) {
            ChangeColor('a');
            cout << "You have the following actions: \n";
            ChangeColor('6');
            cout << "attack\n";
            ChangeColor('6');
            cout << "defend\n";
            ChangeColor('6');
            cout << "items (view)\n";
            ChangeColor('a');
            cout << "Type the action to perform it\n";
            cout << "\n";
            ChangeColor('7');
            cin >> input;
            cout << "\n";
            switch (input[0]) {
            case 'a': {
                ChangeColor('c');
                cout << "You rush in to attack!\n";
                int dmg = attack();
                if (enemy_health[1] < 0) {
                    enemy_health[1] = 0;
                }
                enemy_health[1] = enemy_health[1] - dmg;
                ChangeColor('9');
                cout << "enemy has " << enemy_health[1] << "/" << enemy_health[0] << "hp remaining\n";
                pturn = false;
                break;
            }
            case 'd': {
                if (has_defended) {
                    ChangeColor('3');
                    cout << "You have already defended this turn.\n";
                    pturn = true;
                    break;
                }
                ChangeColor('3');
                cout << "You prepare to defend.\n";
                has_defended = true;
                pturn = false;
                break;
            }
            case 'i': {
                ChangeColor('a');
                cout << "You open your bag and find the following items:\n";
                cout << "\n";
                ChangeColor('9');
                cout << "Speed potion: Increase your speed by 2\n";
                ChangeColor('9');
                cout << "Axe: Increase your attack by 3\n";
                ChangeColor('9');
                cout << "Health potion: Increase your health by 50%, max 2 uses\n";
                cout << "\n";
                ChangeColor('a');
                cout << "type the name of the item you wish to use. type 'c' to return\n";
                cout << "\n";
                ChangeColor('7');
                cin >> ch;
                cout << "\n";
                switch (ch[0]) {
                    case 's': {
                        if (has_speed == false) {
                            ChangeColor('e');
                            cout << "You already did this action!\n";
                        }
                        else {
                            speed_potion();
                            pturn = false;
                        }
                        break;
                    }
                    case 'a': {
                        if (has_axe == false) {
                            ChangeColor('e');
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
                            ChangeColor('e');
                            cout << "You already did this action!\n";
                        }
                        else {
                            health_potion();
                            pturn = false;
                        }
                        break;
                    }
                    case 'c': {
                        ChangeColor('e');
                        cout << "You decide to leave them for later and face the enemy again\n";
                        cout << "\n";
                        break;
                    }
                 break;
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
            int enemy_choice = rand() % 2;
            if (enemy_choice == 1) {
                ChangeColor('4');
                cout << "The enemy rushes in to attack!\n";
                int dmg = eattack();
                players_health[1] = players_health[1] - dmg;
                if (players_health[1] < 0) {
                    players_health[1] = 0;
                }
                ChangeColor('2');
                cout << "You have " << players_health[1] << "/" << players_health[0] << "hp remaining\n";
                eturn = false;
            }
            else
            {
                if (ehas_defended) {
                    eturn = true;
                }
                else {
                    ChangeColor('3');
                    cout << "The enemy prepares to defend from your next attack!\n";
                    ehas_defended = true;
                    eturn = false;
                }
            }
        }
        gamestate();
    } while (gamestatehealth == true);
}
