#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

#define THREAD_STACK_SIZE 1024

/**
 * Each message queue message contains:
 * 1) Timestamp (ms)
 * 2) Sequence number (proves what # message this is)
 * 3) Sensor data 0
 * 4) Sensor data 1
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

volatile uint32_t drops_put = 0;

extern "C" void app_main(void)
{
    printk("boot OK: rtos-mission-sim\n");

    static struct k_thread sensor_thread_data;

    k_thread_create(&sensor_thread_data, sensor_stack, THREAD_STACK_SIZE, sensor_thread, NULL, NULL, NULL, 1, K_ESSENTIAL, K_NO_WAIT);

    TelemetryMessage msg;
    bool have_msg = false;

    while (true)
    {
        have_msg = false;

        while (k_msgq_get(&telemetry_msgq, &msg, K_NO_WAIT) == 0)
        {
            have_msg = true;
        }

        if (have_msg)
        {
            printk("Telemetry Data: ts %llu seq %u s0 %u s1 %u drops %u\n", (unsigned long long)msg.ts_ms, msg.seq, msg.s0, msg.s1, drops_put);
        }

        k_sleep(K_MSEC(100));
    }
}

void sensor_thread(void *p1, void *p2, void *p3)
{
    uint32_t seq = 0;

    while (true)
    {
        TelemetryMessage msg;

        msg.ts_ms = k_uptime_get();
        msg.seq = seq;
        msg.s0 = seq % 1000;
        msg.s1 = (seq * 2) % 1000;

        if (k_msgq_put(&telemetry_msgq, &msg, K_NO_WAIT) != 0)
        {
            drops_put++;
        }

        seq++;

        // Sleep for 10ms; simulate 100 Hz sensor
        k_sleep(K_MSEC(10));
    }
}