#include "arch/arch_core_timer.h"

typedef struct TICK_CONTEXT  {
    long counter;
} tc_t;

tc_t timer_context;

long c;
void callback(uint32_t st,uintptr_t cxt){
  ((tc_t *) cxt)->counter++;
  c++;
} 

void systick_init(void) {
    arch_core_timer_init();
    arch_core_timer_set_callback(&callback, (uintptr_t)&timer_context);
}
