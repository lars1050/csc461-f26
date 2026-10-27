#ifndef STATUS_REPORT_H
#define STATUS_REPORT_H

// Display to the screen the status of the system



#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>

#include "scheduler.h"


void log_lighting_status(int);
extern TASK_struct status_report_task;

#endif
