#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdlib.h>

class Util
{
    public:
    static void printErrorAndExit(const char *error)
    {
        printf("[Error] %s\n", error);
        exit(EXIT_FAILURE);
    }
    
    static void PrintWarning(const char *warning)
    {
        printf("[Warning] %s\n", warning);
    }

};

#endif // UTIL_H