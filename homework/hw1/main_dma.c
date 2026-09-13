#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
// #include "leak_detector_c.h" for memory leak detector.

#define MAX 51

/* This structure stores information about a monster.
The name and element are dynamically allocated strings, 
while population stores the monster's population value. */
typedef struct monster
{
    char *name;
    char *element;
    int population;
} monster;

/* This structure stores information about a region.
The monsters member is an array of pointers to monsters
that were already created in the Monster Index. */
typedef struct region
{
    char *name;
    int monster_cnt;
    int total_population;
    monster **monsters;
} region;

/* This structure stores information about a trainer's itinerary.
The regions member is an array of pointers to regions
that were already created in the Region Atlas. */
typedef struct itinerary
{
    int region_cnt;
    region **regions;
    int captures;
} itinerary;

/* This structure stores information about a trainer's name and itinerary. */
typedef struct trainer
{
    char *name;
    itinerary *visits;
} trainer;

/* This function returns a dynamically allocated monster 
filled with the provided parameters */
monster* makeMonster(char *name, char *element, int population) 
{
    monster *m; 

    // Allocate memory for the monster structure 
    m = malloc(sizeof(monster));

    // Allocate exact memory for the monster name and copy it
    m->name = malloc((strlen(name) + 1) * sizeof(char));
    strcpy(m->name, name);

    // Allocate exact memory for the monster element and copy it
    m->element = malloc((strlen(element) + 1) * sizeof(char));
    strcpy(m->element, element);

    // Store the population of the monster
    m->population = population;

    return m;
}

/* This function returns an array of monster pointers
where each monster pointer points to the dynamically allocated 
monsters with fill-up information from the provided inputs. */
monster** readMonsters(int *monsterCount) 
{
    monster **list;
    char name[MAX];
    char element[MAX];
    int population;

    // Read the number of monsters and allocate the pointer array
    scanf("%d", monsterCount);
    list = malloc(*monsterCount * sizeof(monster *));

    // Read each monster and create it dynamically
    for (int i = 0; i < *monsterCount; i++)
    {
        scanf("%50s %50s %d", name, element, &population);
        list[i] = makeMonster(name, element, population);
    }

    return list;
}

/* This function searches the monster list for a monster with the given name
and returns a pointer to the existing monster if it is found. */
monster* findMonsters(monster **monsterList, int monsterCount, char *name) 
{
    // Compare the requested name with each monster's name
    for (int i = 0; i < monsterCount; i++)
    {
        if(strcmp(monsterList[i]->name, name) == 0)
        {
            return monsterList[i];
        }
    }

    return NULL;
}

/* This function returns an array of region pointers 
where each region pointer points to a dynamically allocated region, filled 
up with the information from the inputs, and the region’s monsters member 
points to an appropriate list of monsters from the monsterList passed to this function*/
region** readRegions(int *countRegions, monster** monsterList, int monsterCount) 
{
    region **regions;
    char regionName[MAX];
    char monsterName[MAX];
    int rmcount;

    // Read the number of regions and allocate the region pointer array
    scanf("%d", countRegions);
    regions = malloc(*countRegions * sizeof(region *));

    // Read and create each region
    for (int i = 0; i < *countRegions; i++)
    {
        // Allocate memory for the region structure 
        regions[i] = malloc(sizeof(region));   
        // Read, allocate, and copy the region name
        scanf("%50s", regionName);
        regions[i]->name = malloc((strlen(regionName) + 1) * sizeof(char));
        strcpy(regions[i]->name, regionName);

        // Read the number of monsters found in the region
        scanf("%d", &rmcount);
        regions[i]->monster_cnt = rmcount;
        
        // Initialize the region's total population to zero
        regions[i]->total_population = 0;

        // Allocate an array of monster pointers for the region
        regions[i]->monsters = malloc(rmcount * sizeof(monster *));

        // Read each monster name and find the existing monster
        for (int j = 0; j < rmcount; j++)
        {
            scanf("%50s", monsterName);

            // Store a pointer to the existing monster instead of creating another copy of the monster
            regions[i]->monsters[j] = findMonsters(monsterList, monsterCount, monsterName);

            // Add the monster's population to the region total
            regions[i]->total_population += regions[i]->monsters[j]->population;
        }
    }
    
    return regions;
}

/* This function searches the region list for a region with the given name
and returns a pointer to the existing region if it is found. */
region* findRegions(region **regionList, int regionCount, char *name)
{
    // Compare the requested name with each region's name
    for (int i = 0; i < regionCount; i++)
    {
        if(strcmp(regionList[i]->name, name) == 0)
        {
            return regionList[i];
        }
    }

    return NULL;
}

/* This function returns a dynamically allocated array of trainers,
filled with information from the inputs. The trainer's visits field
points to a dynamically allocated itinerary based on the regionList. */
trainer* loadTrainers(int *trainerCount, region** regionList, int countRegions) 
{
    trainer *trainers;
    char trainerName[MAX];
    char regionName[MAX];
    int captures;
    int trcount;

    // Read the number of trainers and allocate the trainer array 
    scanf("%d", trainerCount);
    trainers = malloc(*trainerCount * sizeof(trainer));

    // Read the information for each trainer
    for (int i = 0; i < *trainerCount; i++) 
    {
        // Read, allocate, and copy the trainer's name
        scanf("%50s", trainerName);
        trainers[i].name = malloc((strlen(trainerName) + 1) * sizeof(char));
        strcpy(trainers[i].name, trainerName);

        // Read the number of captures and regions in the itinerary
        scanf("%d", &captures);
        scanf("%d", &trcount);

        // Allocate and initialize the trainer's itinerary
        trainers[i].visits = malloc(sizeof(itinerary));

        trainers[i].visits->captures = captures;
        trainers[i].visits->region_cnt = trcount;
        // Allocate an array of pointers to the region
        trainers[i].visits->regions = malloc(trcount * sizeof(region *));

        // Find and store pointers to the existing regions
        for (int j = 0; j < trcount; j++)
        {
            scanf("%50s", regionName);
            trainers[i].visits->regions[j] = findRegions(regionList, countRegions, regionName);
        }
    }

    return trainers;
}

/* This function processes all the data and produce the output. */
void processInputs(monster** monsterList, int monsterCount, region** regionList,
int regionCount, trainer* trainerList, int trainerCount ) 
{
    int estimated;

    // Process each trainer in the order they appeared in the input
    for (int i = 0; i < trainerCount; i++) 
    {
        printf("%s\n", trainerList[i].name);

        // Process each region in. the trainer's itinerary 
        for (int j = 0; j < trainerList[i].visits->region_cnt; j++)
        {
            region *currentRegion;
            currentRegion = trainerList[i].visits->regions[j];
            printf("%s\n", currentRegion->name);

            // Process each monster in the current region
            for (int k = 0; k < currentRegion->monster_cnt; k++)
            {
                monster *currentMonster;
                currentMonster = currentRegion->monsters[k];
                
                // Calculate the estimated number of captures for the current monster and round the result
                estimated = (int)round(
                    ((double)currentMonster->population /
                    currentRegion->total_population)
                    * trainerList[i].visits->captures);
            
                // Only print monsters with at least one estimated capture
                if (estimated > 0)
                {
                    printf("%d-%s\n", estimated, currentMonster->name);
                }
            }
        }

        // Print a blank line between trainers
        if (i < trainerCount - 1)
        {
            printf("\n");
        }
    }
}

/* This function takes all the dynamically allocated arrays and free-up all the memory. */
void releaseMemory(monster** monsterList, int monsterCount, region** regionList,
int regionCount, trainer* trainerList, int trainerCount )
{
    // Free each trainer's dynamically allocated memory
    for (int i = 0; i < trainerCount; i++)
    {
        free(trainerList[i].name);
        free(trainerList[i].visits->regions);
        free(trainerList[i].visits);
    }
    // Free the trainer array 
    free(trainerList);

    // Free each region and its dynamically allocated members
    for (int i = 0; i < regionCount; i++)
    {
        free(regionList[i]->name);
        free(regionList[i]->monsters);
        free(regionList[i]);
    } 
    // Free the region pointer array 
    free(regionList);

    // Free each monster and its dynamically allocated strings
    for (int i = 0; i < monsterCount; i++)
    {
        free(monsterList[i]->name);
        free(monsterList[i]->element);
        free(monsterList[i]);
    } 
    // Free the monster pointer array 
    free(monsterList);
}

int main(void) 
{
    // atexit(report_mem_leak); for memory leak detector.
    int monsterCount;
    int regionCount;
    int trainerCount;

    monster **monsterList;
    region **regionList;
    trainer *trainerList;

    // Read and dynamically create the Monster Index
    monsterList = readMonsters(&monsterCount);
    // Read and dynamically create the Region Atlas
    regionList = readRegions(&regionCount, monsterList, monsterCount);
    // Read and dynamically create the List of Trainers
    trainerList = loadTrainers(&trainerCount, regionList, regionCount);

    // Calculate and print the expected monster captures 
    processInputs(monsterList, monsterCount, regionList, regionCount, trainerList, trainerCount);

    // Free all dynamically allocated memory before the program ends
    releaseMemory(monsterList, monsterCount, regionList, regionCount, trainerList, trainerCount);

    return 0;
}
