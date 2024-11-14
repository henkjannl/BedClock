#pragma once

#include "esp_timer.h"
#include "esp_err.h"
#include <stdio.h>
#include <math.h>

/**
 * @file hp_stepping_float.h
 * @brief generates smoothly transitioning values for animations and transitions
 *
 * Can be used for single shot timers or for looping (auto reset) timers
 * Beware that an auto reset timer will only be reset if and when
 * hp_timer_has_lapsed() or hp_timer_lapsed() are called.
 *
 * For details on the interpolation profiles see:
 * https://github.com/henkjannl/BedClock/blob/master/01%20System/Move%20profiles/Move.png
*/

typedef enum {
    ST_LINEAR,  // Moves from start to target linearly
    ST_CYCLOID, // Smooth transition without overshoot
    ST_SPRING,  // Fast begin and some overshoot before target
    ST_CLICK,   // Overshoot at start and end
    ST_ACCEL,   // Move to target with increasing speed
    ST_DAMPED,  // Move to target with decreasing speed
    ST_BOUNCE,  // Move to target and then bounce back twice
    ST_BOW,     // Overshoot at beginning and then accelerate to target
    ST_STEP     // Quick movement to target with overshoot twice
 } hp_stepping_interpolation_t;

typedef struct {
    float start;
    float target;
    int steps;
    hp_stepping_interpolation_t interpolation;

    // Internal values
    int step;    // Current step, incremented by float hp_step(hp_stepping_float_t * hp_stepping_float)
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

/**
 * @brief Initializes the stepper with an interval in microseconds
 *
 * @param hp_stepping_float stepper handle
 * @param start returned value at the beginning of the interval
 * @param target returned value at the end of the interval
 * @param steps number of discrete steps that the interval takes
 * @param interpolation type of trajectory
 */
void hp_stepping_float_init(hp_stepping_float_t * hp_stepping_float,
                            float start,
                            float target,
                            int steps,
                            hp_stepping_interpolation_t interpolation) {
    hp_stepping_float->start = start;
    hp_stepping_float->target = target;
    hp_stepping_float->steps = steps;
    hp_stepping_float->interpolation = interpolation;
    hp_stepping_float->value = start;
    hp_stepping_float->step = 0;
};

/**
 * @brief Change the target value of the stepper
 *
 * @param hp_stepping_float stepper handle
 * @param target returned value at the end of the interval
 * @param steps number of discrete steps that the interval takes
 */
void hp_stepping_float_target(hp_stepping_float_t * hp_stepping_float, float target, int steps) {
    hp_stepping_float->start = hp_stepping_float->value;
    hp_stepping_float->target = target;
    hp_stepping_float->steps = steps;
    hp_stepping_float->step = 0;
};

/**
 * @brief Internal function that returns a value in a 0<=x<=1 0<=y<=1 domain
 *
 * @param x input value
 * @param interpolation type of trajectory
 * @return output value
 */
float hp_stepping_float_interpolation(float x, hp_stepping_interpolation_t interpolation) {

    // Translates (0..1) input to (0..1) output according to selected interpolation method
    float x_;

    switch(interpolation) {
        case ST_LINEAR:
    		return x;

        case ST_CYCLOID:
    		return x - 1/(2*M_PI)*sin(2*M_PI*x);

        case ST_SPRING:
            if(x<0.65) {
                float x_=x/0.65;
                return 3.6*pow(x_,2)*(1-x_) + 1.2*pow(x_,3);
            }

            x_ = (x-0.65)/0.35;
            return 1.2*pow(1-x_,3) + 3.6*x_*pow(1-x_,2) + 3*pow(x_,2)*(1-x_) + pow(x_,3);

        case ST_CLICK:
    		return x-0.4052443*sin(2*M_PI*x)+0.1453085*sin(4*M_PI*x);

        case ST_ACCEL:
    		return pow(x,2);

        case ST_DAMPED:
    		return 1-pow(1-x,2);

        case ST_BOUNCE:
            if(x<0.5) return pow(2*x,3);

            if(x<0.8) {
                x_ = (x-0.5)/0.3;
                return pow(1-x_,3)+3*x_*pow(1-x_,2)*0.75+3*pow(x_,2)*(1-x_)*0.75+pow(x_,3);
            }

            x_ = (x-0.8)/0.2;
            return pow(1-x_,3)+3*x_*pow(1-x_,2)*0.9+3*pow(x_,2)*(1-x_)*0.9+pow(x_,3);

        case ST_BOW:
            return 3*x*pow(1-x,2)*-0.4+pow(x,3);

        case ST_STEP:
            if(x<0.4) {
                x_ = x/0.4;
                return 3.3*pow(x_,2)*(1-x_) + 1.1*pow(x_,3);
            }

            if(x<0.6) {
                x_ = (x-0.4)/0.2;
                return 1.1* pow(1-x_,3) + 3.3*x_*pow(1-x_,2) + 2.85*pow(x_,2)*(1-x_) + 0.95*pow(x_,3);
            }

            if(x<0.8) {
                x_ = (x-0.6)/0.2;
                return 0.95*pow(1-x_,3) + 2.85*x_*pow(1-x_,2) + 3.15*pow(x_,2)*(1-x_) + 1.05*pow(x_,3);
            }

            x_ = (x-0.8)/0.2;
            return 1.05*pow(1-x_,3) + 3.15*x_*pow(1-x_,2) + 3*pow(x_,2)*(1-x_) + pow(x_,3);

        default:
            // Also ST_LINEAR
    		return x;
    }
}

/**
 * @brief Calculates and returns the new return value
 *
 * @param hp_stepping_float stepper handle
 * @return output value
 */
float hp_stepping_value(hp_stepping_float_t * hp_stepping_float) {
    hp_stepping_float->value = hp_stepping_float->start +
        (hp_stepping_float->target - hp_stepping_float->start) *
        hp_stepping_float_interpolation(1.0*hp_stepping_float->step / hp_stepping_float->steps, hp_stepping_float->interpolation);

    return hp_stepping_float->value;
}

/**
 * @brief Increases the step and calculates and returns the new return value
 *
 * @param hp_stepping_float stepper handle
 * @return output value
 */
float hp_stepping_float_step(hp_stepping_float_t * hp_stepping_float) {
    if(hp_stepping_float->step<hp_stepping_float->steps) hp_stepping_float->step++;
    return hp_stepping_value(hp_stepping_float);
}

/**
 * @brief Indicates if the end of the trajectory is reached
 *
 * @param hp_stepping_float stepper handle
 * @return end of trajectory is reached
 */
bool hp_stepping_float_finished(hp_stepping_float_t * hp_stepping_float) {
    return (hp_stepping_float->step >= hp_stepping_float->steps);
}
