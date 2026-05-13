#include <Arduino.h>
#include "hardware/dma.h"
#include "hardware/timer.h"

// Data to transfer
const char *src_str = "PACED DMA EXAMPLE";
char dst_str[32] = {0};

void setup() {
    Serial1.begin(115200);
    while (!Serial1);
    Serial1.println("DMA Pacing Example Started");

    // 1. Configure a DMA Pacing Timer
    // The RP2040 has 4 pacing timers (0-3).
    // They are configured as a fraction X/Y.
    // X is in [15:0], Y is in [31:16]
    // Transfer rate is X/Y transfers per clock cycle.
    volatile uint32_t *timer0_reg = (volatile uint32_t *)(0x50000000 + 0x420);
    // X=1, Y=10000. This results in 1 transfer every 10,000 cycles.
    *timer0_reg = (10000 << 16) | 1;

    // 2. Claim and configure a DMA channel
    int dma_chan = dma_claim_unused_channel(true);
    dma_channel_config c = dma_channel_get_default_config(dma_chan);
    channel_config_set_transfer_data_size(&c, DMA_SIZE_8);
    channel_config_set_read_increment(&c, true);
    channel_config_set_write_increment(&c, true);

    // Set DREQ to use TIMER0 (DREQ index 59)
    channel_config_set_dreq(&c, 0x3b);

    Serial1.printf("Starting paced transfer on channel %d...\n", dma_chan);

    // 3. Start the transfer
    dma_channel_configure(
        dma_chan,
        &c,
        dst_str,    // Destinaton
        src_str,    // Source
        strlen(src_str), // Count
        true        // Start immediately
    );

    // 4. Wait for completion
    dma_channel_wait_for_finish_blocking(dma_chan);

    // 5. Verify
    if (strcmp(src_str, dst_str) == 0) {
        Serial1.print("DMA Pacing Success: ");
        Serial1.println(dst_str);
    } else {
        Serial1.print("DMA Pacing Failed. Got: ");
        Serial1.println(dst_str);
    }

    dma_channel_unclaim(dma_chan);
}

void loop() {
    // Keep alive
    static uint32_t last_msg = 0;
    if (millis() - last_msg > 2000) {
        Serial1.println("DMA Pacing Example Running");
        last_msg = millis();
    }
}
