#ifndef LIGHT_CONTROL_H
#define LIGHT_CONTROL_H

// Poll the button, which controls the PWM duty cycle

#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>

#include "scheduler.h"
extern TASK_struct light_control_task;

#include "status_report.h"

#endif
