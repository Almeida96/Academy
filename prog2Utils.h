#ifndef prog2utils_H
#define prog2utils_H

/*Edit text file*/
int editFile(char path[],char delim[],int id,int pos, char newVal[]);

/* get current day */
void getToday(int *day,int *month,int *year);
 
#endif