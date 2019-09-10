#ifndef STATE_H_
#define STATE_H_

extern uint8_t state_get();
extern void state_set(uint8_t state);
extern void state_load();
extern void state_save();

#endif /* STATE_H_ */