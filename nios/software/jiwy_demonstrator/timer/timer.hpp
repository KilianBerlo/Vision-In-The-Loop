#ifndef TIMER_TIMER_HPP_
#define TIMER_TIMER_HPP_

#include <stdio.h>
#include <functional>

#include "system.h"
#include "sys/alt_irq.h"
#include "altera_avalon_timer_regs.h"

class Timer
{
public:
	Timer(std::function<void ()> callback);

	void enable();

private:
	// Variable to store isr_context pointer.
	static uint32_t timer_isr_context;
	static std::function<void ()> callback;

	static void timer_0_isr(void* timer_isr_context);
};

#endif /* TIMER_TIMER_HPP_ */
