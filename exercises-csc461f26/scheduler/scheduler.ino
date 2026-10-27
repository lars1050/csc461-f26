#include "scheduler.h"
#include "light_control.h"
#include "status_report.h"
#include "record_results.h"
#include "face_recognition.h"

// number of tasks being scheduled
const int task_count = 4;

// task schedule
TASK_struct tasks[] = {
  light_control_task,
  status_report_task,
  record_results_task,
  recognition_task
};

void setup() {
  // initialize each of the tasks
  int i;
  for (int i=0; i<task_count; i++) {
    tasks[i].initialize();
    tasks[i].expiry = millis() + tasks[i].period;
  }
}

void loop() {
  // determine which tasks are ready to run
  int i;
  for (int i=0; i<task_count; i++) {
    if (tasks[i].released && millis() >= tasks[i].expiry) {
      tasks[i].expiry = millis() + tasks[i].period;
      tasks[i].run();
    }
  }
}
