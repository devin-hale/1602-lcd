#pragma once

#include "led.h"
#include <stdint.h>

enum tl_state {
    TL_STATE_RED,
    TL_STATE_YELLOW,
    TL_STATE_GREEN,
    TL_STATE_YELLOW_FLASHING,
};

enum tl_event {
    TL_EVENT_NONE,
    TL_EVENT_TRANSITION_RED,
    TL_EVENT_TRANSITION_GREEN,
    TL_EVENT_TRANSITION_YELLOW,
	TL_EVENT_TRANSITION_FLASHING_YELLOW,
	TL_EVENT_COUNT,
};

enum tl_event_err { 
	TL_EVENT_ERR_NONE ,
	TL_EVENT_ERR_GREEN_TO_RED,
	TL_EVENT_ERR_YELLOW_TO_GREEN,
	TL_EVENT_ERR_RED_TO_YELLOW
};

struct traffic_light {
    enum tl_state state;

    led_typedef *r;
    led_typedef *y;
    led_typedef *g;

    enum tl_event event_buf[32];
    uint8_t head;
    uint8_t tail;

    uint8_t timer;
};

void tl_init(struct traffic_light *tl, uint16_t pin_red, uint16_t pin_yellow, uint16_t pin_green);
void tl_set_default_state(struct traffic_light *tl, enum tl_state s);
void tl_handle_state(struct traffic_light *tl);
uint8_t event_buf_push(struct traffic_light *tl, enum tl_event e);
enum tl_event_err tl_handle_next_event(struct traffic_light *tl);
