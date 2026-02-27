#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

#define THREAD_STACK_SIZE 1024

/*
Each message queue message contains:
1) Timestamp (ms)
2) Sequence number (proves what # message this is)
3) Sensor data 0
4) Sensor data 1
*/
struct TelemetryMessage
{
    uint64_t ts_ms;
    uint32_t seq;
    uint16_t s0;
    uint16_t s1;
};

// Bounded stack/msgq in static memory
K_THREAD_STACK_DEFINE(sensor_stack, THREAD_STACK_SIZE);
K_MSGQ_DEFINE(telemetry_msgq, sizeof(TelemetryMessage), 4, 4);

void sensor_thread(void *p1, void *p2, void *p3);

extern "C" void app_main(void)
{
    printk("boot OK: rtos-mission-sim\n");

    static struct k_thread sensor_thread_data;
    uint64_t ts_ms;

    k_thread_create(&sensor_thread_data, sensor_stack, THREAD_STACK_SIZE, sensor_thread, NULL, NULL, NULL, 1, K_ESSENTIAL, K_NO_WAIT);

    while (true)
    {
        ts_ms = k_uptime_get();

        printk("TLM ts_ms<%llu>\n", ts_ms);

        k_sleep(K_MSEC(100));
    }
}

void sensor_thread(void *p1, void *p2, void *p3)
{
}