// EzChord keyboard firmware
//   ___ _   _ ____  _   _ __  __ __  __
// .' __| |_| |  _ '| | | |  \/  |  \/  |
// | |__|  _  | |_) | |_| | |\/| | |\/| |
// '.___|_| |_|_| \_\.___.|_|  |_|_|  |_|
//
// Copyright 2023 Severin Meyer
// Licensed under CERN-OHL-W v2 or later

#include "EzChord/config.h"
#include "EzChord/midi.h"
#include "EzChord/encoder.h"
#include "EzChord/led.h"
#include "EzChord/chord.h"
#include "EzChord/quality.h"
#include "EzChord/oled.h"
#include "EzChord/bass.h"

#include <tusb.h>
#include <hardware/clocks.h>
#include <hardware/watchdog.h>
#include <pico/stdlib.h>


int main()
{
    set_sys_clock_48mhz();
    // clock_stop(clk_peri);
    // clock_stop(clk_adc);
    stdio_init_all();

    oled_init();
    quality_init();
    bass_init();
    chord_init();
    encoder_init();
    //led_init();
    tusb_init();
    //led_blink(0b10100101);
    //printf("Init done");


    watchdog_enable(WATCHDOG_TIMEOUT_MS, false);

    while (true) {
        absolute_time_t timeout = make_timeout_time_us(TICK_INTERVAL_US);

        //printf("Tick \n");
        quality_tick();
        bass_tick();
        chord_tick();
        oled_tick();
        //led_tick();
        encoder_tick();
        tud_task();
        watchdog_update();

        sleep_until(timeout);
    }
}

//--------------------------------------------------------------------+
// Device callbacks
//--------------------------------------------------------------------+

// Invoked when device is mounted
void tud_mount_cb(void)
{
  
}

// Invoked when device is unmounted
void tud_umount_cb(void)
{
  
}

// Invoked when usb bus is suspended
// remote_wakeup_en : if host allow us  to perform remote wakeup
// Within 7ms, device must draw an average of current less than 2.5 mA from bus
void tud_suspend_cb(bool remote_wakeup_en)
{
  (void) remote_wakeup_en;
  
}

// Invoked when usb bus is resumed
void tud_resume_cb(void)
{
  
}