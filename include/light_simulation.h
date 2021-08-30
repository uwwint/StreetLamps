#ifndef LIGHT_SIMULATION_H
#define LIGHT_SIMULATION_H

#include <Arduino.h>
#include <FastLED.h>

class LightSimulation {
    public:
        LightSimulation() {};
        LightSimulation(unsigned long now, CRGB* gled) : temp_min(100), temp_max(140), current_temp(100), last_upd(now), led(gled) {
            event_start = get_next_surge_event();
        };

        void update(unsigned long now);
        void perform_surge(unsigned long now);
        void do_charge(unsigned long now);
        unsigned long get_next_surge_event();

    private:
        uint8_t temp_min;
        uint8_t temp_max;

        uint8_t min_temp_drop = 5;
        uint8_t max_temp_drop = 20;

        uint16_t min_time_until_surge = 100;
        uint16_t max_time_until_surge = 3000;
        uint16_t min_surge_duration = 300;
        uint16_t max_surge_duration = 500;
        uint16_t recharge_per_ms = 5;

        //current status
        uint8_t current_temp;
        unsigned long last_upd;

        //future status
        uint8_t start_temp;
        uint8_t target_temp;
        unsigned long event_end;
        unsigned long event_start;
        bool is_surging;
        CRGB* led;
        
        
};

#endif
