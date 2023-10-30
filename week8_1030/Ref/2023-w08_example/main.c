#include "common.h"
#include "global.h"

//#pragma message "Compiling"  

int main(void)
{ 
    temp = 3;
    printf("temp %d\n", temp);

    change_temp();
    printf("temp %d\n", temp);

    printf("Runing %s, line: %d, on %s, at %s, STDC=%d \n",
           __FILE__, __LINE__, __DATE__, __TIME__, __STDC__);

}