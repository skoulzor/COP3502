/* COP 3502C PA0
This program is written by: Hailey Simpson */

#include <stdio.h>
#include "main.h"


void readAllMonsters(Monster monsters[], int *count)
{
    scanf("%d", count);

    for (int i = 0; i < *count; i++) {
        scanf("%s %s %d %d %d %d", 
            monsters[i].name, 
            monsters[i].type, 
            &monsters[i].level,
            &monsters[i].hp,
            &monsters[i].attack,
            &monsters[i].defense
            );
    }
}


void printMonster(Monster monster)
{
    printf("Name: %s\n", monster.name);
    printf("Type: %s\n", monster.type);
    printf("Level: %d\n", monster.level);
    printf("Hp: %d\n", monster.hp);
    printf("Attack: %d\n", monster.attack);
    printf("Defense: %d\n", monster.defense);
}


void trainMonster(Monster *monster)
{
    monster->level +=1;
    monster->attack +=5;
    monster->defense +=3;

    int hpIncrease = monster->name[0] - 'A' + 1;
    monster->hp += hpIncrease;
}


int findStrongest(Monster monsters[], int count)
{
    int strongestIdx = 0;
    int strongestScore = monsters[0].level + monsters[0].attack + monsters[0].defense;

    for (int i = 0; i < count; i++) {
        
        int currentScore = monsters[i].level + monsters[i].attack + monsters[i].defense;

        if (currentScore > strongestScore) {
            strongestScore = currentScore;
            strongestIdx = i;
        }

    }

    return strongestIdx;

}


int main()
{
    Monster monsters [MAX_MONSTERS];

    int count;
    readAllMonsters(monsters, &count);

    printf("=== MONSTER TEAM ===\n");
    for (int i = 0; i < count; i++){
        printf("Monster %d:\n", i);
        printMonster(monsters[i]);
        printf("\n");
    }

    int trainIdx;
    scanf("%d", &trainIdx);
    printf("=== TRAINING ===\n");
    printf("Training Monster %d...\n", trainIdx);
    trainMonster(&monsters[trainIdx]);
    printMonster(monsters[trainIdx]);
    printf("\n");

    printf("=== STRONGEST MONSTER ===\n");
    int strongestIdx = findStrongest(monsters, count);
    printMonster(monsters[strongestIdx]);
    printf("\n");

    return 0;
}