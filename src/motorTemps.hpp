#pragma once

#include <vector>
#include <string>
#include "main.h"


typedef struct {
    std::string motorCode;
    int temp;
} motorCodeTemp;

void overTempWarning(void);
void OTWarning_task(void * a);
void showTemps(void * a);