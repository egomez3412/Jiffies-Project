#include <stdio.h>
#include <stdlib.h>

int main(void){
    int num = 321;
    char buffer[10];
    sprintf(buffer, "%d", num);

    puts(buffer);


    return 0;
}