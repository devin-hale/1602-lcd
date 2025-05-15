#pragma once

#include "led.h"
#include <stdint.h>

typedef enum {
	TL_NORTH,
	TL_SOUTH,
	TL_EAST,
	TL_WEST
} tl_instance;

typedef enum {
    TL_STATE_RED,
    TL_STATE_YELLOW,
    TL_STATE_GREEN,
    TL_STATE_YELLOW_FLASHING,
    TLS_LENGTH
} tl_state;

typedef enum {
    TL_EVENT_NONE,
    TL_EVENT_TRANSITION_RED,
    TL_EVENT_TRANSITION_GREEN,
    TL_EVENT_TRANSITION_YELLOW,
	TL_EVENT_TRANSITION_FLASHING_YELLOW,
    TL_EVENT_LENGTH
} tl_event;

typedef enum { 
	TL_EVENT_ERR_NONE ,
	TL_EVENT_ERR_GREEN_TO_RED,
	TL_EVENT_ERR_YELLOW_TO_GREEN,
	TL_EVENT_ERR_RED_TO_YELLOW
} tl_event_error;

typedef struct {
    tl_state state;

    led_typedef *r;
    led_typedef *y;
    led_typedef *g;

    tl_event *event_buf;
    uint8_t head;
    uint8_t tail;

    uint8_t timer;
} tl_typedef;

typedef struct {
    tl_typedef *north;
    tl_typedef *south;
    tl_typedef *east;
    tl_typedef *west;
} tl_controller;

void tl_init(tl_instance i, uint16_t pin_red, uint16_t pin_yellow, uint16_t pin_green);
void tl_set_default_state(tl_instance i, tl_state s);
void tl_handle_state(tl_instance i);
uint8_t event_buf_push(tl_instance i, tl_event e);
tl_event_error tl_handle_next_event(tl_instance i);
