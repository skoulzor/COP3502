/* COP 3502C PA0
This program is written by: Hailey Simpson */

#define MAXSTRLEN 20
#define MAX_MONSTERS 100

typedef struct {
    char name[MAXSTRLEN+1];
    char type[MAXSTRLEN+1];
    int level;
    int hp;
    int attack;
    int defense;
} Monster;

void readAllMonsters(Monster monsters[], int *count);
void trainMonster(Monster *monster);
int findStrongest(Monster monsters[], int count);