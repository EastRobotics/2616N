#pragma once

typedef enum {
    BLEFT_MTR_INDEX = 0,
    FLEFT_MTR_INDEX,
    BRIGHT_MTR_INDEX,
    FRIGHT_MTR_INDEX,
    LIFT_MTR_INDEX,
    TRAY_MTR_INDEX,
    L_INTAKE_MTR_INDEX,
    R_INTAKE_MTR_INDEX
} allMotorIndexEnum;

void recordRerun(void* a);
void replayRerun();