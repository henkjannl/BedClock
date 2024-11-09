#pragma once

#include "esp_timer.h"
#include "esp_err.h"
#include <stdio.h>
#include <math.h>

typedef enum {
    ST_LINEAR,
    ST_CYCLOID,
    ST_SPRING,
    ST_CLICK,
    ST_ACCEL,
    ST_DAMPED
 } hp_stepping_interpolation_t;

typedef struct {
    float start;
    float target;
    int steps;
    hp_stepping_interpolation_t interpolation;

    // Internal values
    int step;    // Current step, incremented by float hp_step(hp_stepping_float_t * hp_stepping_float) {
    float value; // Calculated by float hp_stepping_value(hp_stepping_float_t * hp_stepping_float);
} hp_stepping_float_t;

const hp_stepping_float_t HP_STEPPING_FLOAT_INIT = {
    .start  = 0,
    .target = 0,
    .steps  = 100,
    .interpolation = ST_CYCLOID,
    .step   = 0,
    .value  = 0
};

void hp_stepping_float_init(hp_stepping_float_t * hp_stepping_float,
                            float target,
                            int steps,
                            hp_stepping_interpolation_t interpolation) {
    hp_stepping_float->start = hp_stepping_float->value;
    hp_stepping_float->target = target;
    hp_stepping_float->steps = steps;
    hp_stepping_float->interpolation = interpolation;
    hp_stepping_float->step = 0;
};

void hp_stepping_float_target(hp_stepping_float_t * hp_stepping_float, float target, int steps) {
    hp_stepping_float->start = hp_stepping_float->value;
    hp_stepping_float->target = target;
    hp_stepping_float->steps = steps;
    hp_stepping_float->step = 0;
};

float hp_stepping_float_interpolation(float x, hp_stepping_interpolation_t interpolation) {

    // Translates (0..1) input to (0..1) output according to selected interpolation method

    switch(interpolation) {
        case ST_LINEAR:
    		return x;
        break;

        case ST_CYCLOID:
    		return x - 1/(2*M_PI)*sin(2*M_PI*x);
        break;

        case ST_SPRING:
            return (441.8214882*pow(x,8) - 1703.935071*pow(x,7) + 2530.182847*pow(x,6) -
                1771.030096*pow(x,5) + 561.4457127*pow(x,4) - 62.98225215*pow(x,3) +
                5.597813989*pow(x,2) - 0.09284320409*x - 0.000440360649);
        break;

        case ST_CLICK:
    		return x-0.4052443*sin(2*M_PI*x)+0.1453085*sin(4*M_PI*x);
        break;

        case ST_ACCEL:
    		return pow(x,2);
        break;

        case ST_DAMPED:
    		return 1-pow(1-x,2);
        break;

        default:
            // Also ST_LINEAR
    		return x;
    }
}

float hp_stepping_value(hp_stepping_float_t * hp_stepping_float) {
    hp_stepping_float->value = hp_stepping_float->start +
        (hp_stepping_float->target - hp_stepping_float->start) *
        hp_stepping_float_interpolation(1.0*hp_stepping_float->step / hp_stepping_float->steps, hp_stepping_float->interpolation);

    return hp_stepping_float->value;
}

float hp_stepping_float_step(hp_stepping_float_t * hp_stepping_float) {
    if(hp_stepping_float->step<hp_stepping_float->steps) hp_stepping_float->step++;
    return hp_stepping_value(hp_stepping_float);
}

bool hp_stepping_float_finished(hp_stepping_float_t * hp_stepping_float) {
    return (hp_stepping_float->step >= hp_stepping_float->steps);
}
