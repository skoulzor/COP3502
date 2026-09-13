//THIS IS A CODE TEMPLATE FOR LAB 2 - FOR PRACTICE//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "leak_detector_c.h"

#define NAMEMAX 51

typedef struct student
{
     char *lname;
     int assignment;
     int finalExam;
     int total;
     int *quizzes;
}student;


student** readCourses(int *C, int *N, int *M)
{
    student** courses;

    char lname[NAMEMAX];

    //read the first line for C N and M
    scanf("%d %d %d", C, N, M);

     // Allocates the memory for C number of courses //Array of arrays of students aka 2D array of students.
     //so courses is an array of students pointers
    courses = malloc(*C * sizeof(student*));

     //Now read C number of courses

     //allocate memory for the list of students for each course
     for(int cn = 0; cn < *C; cn++)
     {
      //Array of students for cnth course
      courses[cn] = malloc(*N * sizeof(student));
       
         //now access each students of course[cn]
         for(int sn = 0; sn < *N; sn++)
         {
             int total = 0; //total for the student

             //read name and get the lenght of the name
              //1 extra space for NULL char as we will allocate memory
              scanf("%s", lname);
              int nameLength = strlen(lname) + 1;
              courses[cn][sn].lname = malloc(nameLength * sizeof(char));
              strcpy(courses[cn][sn].lname, lname);

             //take assignment score
             scanf("%d", &courses[cn][sn].assignment);
             total += courses[cn][sn].assignment;
             
             //allocate memory for M number of quizzes for that particular student
            courses[cn][sn].quizzes = malloc(*M * sizeof(int));

           //take all quiz scores
             for(int qn = 0; qn < *M; qn++)
             {
                scanf("%d", &courses[cn][sn].quizzes[qn]);
                total += courses[cn][sn].quizzes[qn];
             }

             //input the final exam score
             scanf("%d", &courses[cn][sn].finalExam);
             total += courses[cn][sn].finalExam;
             courses[cn][sn].total = total;
         }
     }
     return courses;
}

void printHighestTotal(student** courses, int C, int N, int M)
{
    //we need to know which course and the which student so that we can print the details.
    int maxTotal = -1;
    int maxCourseIdx = 0;
    int maxStudentIdx = 0;
    
    for(int cn = 0; cn<C; cn++)
    {
        for(int sn = 0; sn<N; sn++)
        {
            if(courses[cn][sn].total>maxTotal)
            {
                maxTotal = courses[cn][sn].total;
                maxCourseIdx = cn;
                maxStudentIdx = sn;
            }

        }

    }

    //print info of the stident with the highest total
    printf("Name: %s\n", courses[maxCourseIdx][maxStudentIdx].lname);
    printf("Assignment: %d\n", courses[maxCourseIdx][maxStudentIdx].assignment);
    printf("Quizzes:");
    for (int qn = 0; qn < M; qn++)
    {
        printf(" %d", courses[maxCourseIdx][maxStudentIdx].quizzes[qn]);
    }
    printf(" \n");
    printf("Final exam: %d\n", courses[maxCourseIdx][maxStudentIdx].finalExam);
    printf("Total: %d\n", courses[maxCourseIdx][maxStudentIdx].total);
    printf("Course number: %d", maxCourseIdx + 1);
}

void release_memroy(student ** st, int C, int N, int M)
{
    //we need to release each quizzes array for each student, their name and then release the students and then courses.

    for(int cn = 0; cn < C; cn++)
    {
        for(int sn = 0; sn < N; sn++)
        {
            //freeing name and quizzes
            free(st[cn][sn].lname);
            free(st[cn][sn].quizzes);
        }
      
        //freeing the students in that course
        free(st[cn]);
    }
  
    //freeing the double array
    free(st);
    
}

int main()
{
     atexit(report_mem_leak); //for memory leak detector.
     student **courseList;
     int C, N, M;
    

     //passing reference of C, N, and M so that we get to read input to them
     courseList = readCourses(&C, &N, &M);

     printHighestTotal(courseList, C, N, M);

     release_memroy(courseList, C, N, M);

     //free(ptr);

    return 0;
}