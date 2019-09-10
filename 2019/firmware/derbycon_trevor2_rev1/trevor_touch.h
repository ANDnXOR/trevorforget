#ifndef TREVOR_TOUCH_H_
#define TREVOR_TOUCH_H_

extern void trevor_touch_clear();
extern bool trevor_touch_eye0_get();
extern bool trevor_touch_eye1_get();
extern void trevor_touch_handler();
extern bool trevor_touch_eye0_wait(double max_ms);
extern bool trevor_touch_eye1_wait(double max_ms);
extern bool trevor_touch_wait(double max_ms);
extern void trevor_touch_wait_clear();
extern void trevor_touch_hug_set(bool hugged);
extern bool trevor_touch_hug_get();
extern bool trevor_touched();

#endif /* TREVOR_TOUCH_H_ */