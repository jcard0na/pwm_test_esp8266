/* Very basic example that just demonstrates we can run at all!
 */
#include "espressif/esp_common.h"
#include "esp/uart.h"
#include "FreeRTOS.h"
#include "task.h"
#include "pwm.h"

void task1(void *pvParameters)
{
    printf("Hello from task1!\r\n");
    uint32_t count = 0;
    while(1) {
        vTaskDelay(100);
        pwm_set_duty(count++);
        if (count > UINT16_MAX)
            count = 0;
    }
}

void user_init(void)
{
    uint8_t pins[1];
    uart_set_baud(0, 115200);

    printf("SDK version:%s\n", sdk_system_get_sdk_version());

    printf("pwm_init(1, [14])\n");
    pins[0] = 14;
    pwm_init(1, pins);
    printf("pwm_set_freq(1000)     # 1 kHz\n");
    pwm_set_freq(1000);
    printf("pwm_set_duty(UINT16_MAX/2)    # 50%%\n");
    pwm_set_duty(UINT16_MAX/2);

    xTaskCreate(task1, (signed char *)"tsk1", 256, NULL, 2, NULL);
}
