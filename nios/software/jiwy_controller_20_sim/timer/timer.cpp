#include "timer.hpp"

uint32_t Timer::timer_isr_context;
std::function<void()> Timer::callback;

void Timer::timer_0_isr(void* timer_isr_context)
{
    // Answer interrupt, clear STATUS register.
	IOWR_ALTERA_AVALON_TIMER_STATUS(TIMER_0_BASE,
		~ ALTERA_AVALON_TIMER_STATUS_TO_MSK);

	// Call callback.
	callback();
}

Timer::Timer(std::function<void ()> func)
{
	callback = func;
}

void Timer::enable()
{
	IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_0_BASE,
		ALTERA_AVALON_TIMER_CONTROL_START_MSK | // START = 1
		ALTERA_AVALON_TIMER_CONTROL_CONT_MSK  | // CONT  = 1
		ALTERA_AVALON_TIMER_CONTROL_ITO_MSK);   // ITO   = 1

	void* isr_context_ptr = (void*) &timer_isr_context;

	alt_ic_isr_register(
		TIMER_0_IRQ_INTERRUPT_CONTROLLER_ID, // interrupt controller label, copied from system.h
		TIMER_0_IRQ, // hardware interrupt number, copied from system.h
		timer_0_isr, // interrupt service subfunction
		isr_context_ptr, // points to the data structure related to the device driver instance
		0x0); // flags, reserved unused
}
