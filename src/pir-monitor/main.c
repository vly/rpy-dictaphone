#include <bcm2835.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#undef NA_PIN RPI_GPIO_P1_07
#define NA_POLL_DELAY 33
#define NA_PIN_STILL_VALUE 1

int main(int argc, char **argv)
{
    if (!bcm2835_init()) {
        printf("Can't access hardware, did you use sudo?\n");
        return EXIT_FAILURE;
    }

    // Setup pin to use
    bcm2835_gpio_fsel(NA_PIN, BCM2835_GPIO_FSEL_INPT);

    // Use pull up
    bcm2835_gpio_set_pud(NA_PIN, BCM2835_GPIO_PUD_UP);

    uint8_t value = NA_PIN_STILL_VALUE;

    while (1) {
        // Read pin value
        value = bcm2835_gpio_lev(NA_PIN);

        if(value != NA_PIN_STILL_VALUE){
            printf("%d\n", (int)time(NULL));
        }

        // Pause before polling again
        delay(NA_POLL_DELAY);
    }

    bcm2835_close();

    return EXIT_SUCCESS;
}

#undef NA_PIN
#undef NA_POLL_DELAY
#undef NA_PIN_STILL_VALUE