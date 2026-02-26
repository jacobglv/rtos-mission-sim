#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

extern "C" void app_main(void) {
    printk("boot OK: rtos-mission-sim\n");
    int counter = 0;
    while (true) {
        printk("tick %d\n", counter++);
        k_sleep(K_SECONDS(1));
    }
}