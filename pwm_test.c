/* Very basic example that just demonstrates we can run at all!
 */
#include "espressif/esp_common.h"
#include "esp/uart.h"
#include "FreeRTOS.h"
#include "task.h"
#include "pwm.h"

const int pwm_freq = 46000;
const int sample_freq = 8000;

static volatile uint16_t frc2_count;

void frc2_interrupt_handler(void)
{
    /* FRC2 needs the match register updated on each timer interrupt */
    timer_set_frequency(FRC2, sample_freq);
    frc2_count++;
    pwm_set_duty(frc2_count);
}

void user_init(void)
{
    uint8_t pins[1];
    uart_set_baud(0, 115200);

    printf("SDK version:%s\n", sdk_system_get_sdk_version());

    pins[0] = 14;
    pwm_init(1, pins);
    /* frequency can only be set once */
    pwm_set_freq(pwm_freq);
    pwm_set_duty(UINT16_MAX/2);
    pwm_start();

    /* stop both timers and mask their interrupts as a precaution */
    timer_set_interrupts(FRC2, false);
    timer_set_run(FRC2, false);

    /* set up ISRs */
    _xt_isr_attach(INUM_TIMER_FRC2, frc2_interrupt_handler);

    /* configure timer frequencies */
    timer_set_frequency(FRC2, sample_freq);

    /* unmask interrupts and start timers */
    timer_set_interrupts(FRC2, true);
    timer_set_run(FRC2, true);
}
