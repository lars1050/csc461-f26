#ifndef SCHEDULER_H
#define SCHEDULER_H

typedef struct {
  void (*initialize)(void);
  void (*run)(void);
  uint16_t period;
  uint8_t released;
  unsigned long expiry;
} TASK_struct;

#endif
