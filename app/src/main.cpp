#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

extern "C" void app_main(void)
{
    printk("boot OK: rtos-mission-sim\n");
    uint64_t seq;

    while (true)
    {
        seq = k_uptime_get();
        printk("TLM ts_ms<%d>\n", seq);

        // Wait 100 ms
        k_sleep(K_MSEC(100));
    }
}