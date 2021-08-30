#include "light_simulation.h"
#include "FastLED.h"

CRGBPalette16 bb_color;

void LightSimulation::perform_surge(unsigned long now) {
    if (!is_surging & now > event_start) {
        event_end = now + 1000* (unsigned long)random16(min_surge_duration,max_surge_duration);
        uint8_t ttemp = current_temp - random8(min_temp_drop,max_temp_drop);
        start_temp = current_temp;
        target_temp = max(temp_min,ttemp);
        is_surging = true;
    }
    if (!is_surging) {
        return;
    }

    if (now > event_end) {
        is_surging = false;
        current_temp = target_temp;
        last_upd = event_end;
        event_start = get_next_surge_event();
    } else {
        unsigned long current_position = now-event_start;

        unsigned long total_duration = event_end-event_start;

        uint8_t completion_frac = (UINT8_MAX * current_position)/total_duration;
        current_temp = lerp8by8(start_temp,target_temp,completion_frac);
        last_upd=now;
    }
}

void LightSimulation::do_charge(unsigned long now) {
    if (!is_surging) {
        unsigned long time_since_last_upd = min(now,event_start) - last_upd;
        uint8_t recharge = 0;

        if (time_since_last_upd * recharge_per_ms / 100000 > ((unsigned long) UINT8_MAX)) {
            recharge = UINT8_MAX;
        } else {
            recharge = time_since_last_upd * recharge_per_ms /100000;
        }
        if ((uint16_t)current_temp + (uint16_t)recharge > temp_max) {
            current_temp = temp_max;
        } else {
            current_temp += recharge;
        }
        last_upd = min(now,event_start);
    }
}

unsigned long LightSimulation::get_next_surge_event() {
    return last_upd + 1000 * (unsigned long) random16(min_time_until_surge,max_time_until_surge);
}

void LightSimulation::update(unsigned long now) {
    //execute one step
    if (now > event_start) {
        if (!is_surging) {
            do_charge(now);
        }
        perform_surge(now);
    }

    if (!is_surging) {  
        do_charge(now);
    } 

    Serial.println(current_temp);
    (*led) = ColorFromPalette(bb_color,current_temp);
}