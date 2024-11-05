#pragma once 

#include "esp_timer.h"
#include "esp_err.h"
#include <stdio.h>

typedef struct {
    double value;
    double target;
    double step;
    bool finished;
} hp_stepping_float_t;

const hp_stepping_float_t HP_STEPPING_FLOAT_INIT = {
    .value = 0,
    .target = 0,
    .step = 1,
    .finished = true
};

void hp_stepping_float_init(hp_stepping_float_t * hp_stepping_float, double start_value) {
    hp_stepping_float->value = start_value;
    hp_stepping_float->finished = true;
};

void hp_stepping_float_target(hp_stepping_float_t * hp_stepping_float, double target, int steps) {
    hp_stepping_float->target = target;
    hp_stepping_float->step = (target- hp_stepping_float->value)/steps;
    hp_stepping_float->finished = false;
};

bool hp_stepping_float_step(hp_stepping_float_t * hp_stepping_float) {
    if(hp_stepping_float->finished) return true;

    hp_stepping_float->value+=hp_stepping_float->step;

    if( ( (hp_stepping_float->step > 0) && (hp_stepping_float->value > hp_stepping_float->target) ) ||
        ( (hp_stepping_float->step < 0) && (hp_stepping_float->value < hp_stepping_float->target) ) ) {
            hp_stepping_float->value = hp_stepping_float->target;
            hp_stepping_float->finished = true;
        }

    return hp_stepping_float->finished;
};
