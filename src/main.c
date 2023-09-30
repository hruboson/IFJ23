/**
 * 
*/
#include "example.h"
#include "stdio.h"

int main(int argc, char const *argv[])
{
    printf("argc: %d\nargv: %s\n", argc, argv[0]);
    printf("average of 3, 4, 8 = %d\n", AverageThreeBytes(3, 4, 8));
    return 0;
}
