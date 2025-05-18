#pragma once

#include "lcd.h"
#include "led.h"
#include <stdint.h>

enum tl_state {
    TL_STATE_OFF,
    TL_STATE_RED,
    TL_STATE_YELLOW,
    TL_STATE_GREEN,
    TL_STATE_YELLOW_FLASHING,
    TL_STATE_COUNT,
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
    TL_EVENT_ERR_NONE,
    TL_EVENT_ERR_GREEN_TO_RED,
    TL_EVENT_ERR_YELLOW_TO_GREEN,
    TL_EVENT_ERR_RED_TO_YELLOW
};

#define EVENT_BUF_SIZE 32

struct traffic_light {
    enum tl_state state;

    led_typedef *r;
    led_typedef *y;
    led_typedef *g;

    enum tl_event event_buf[EVENT_BUF_SIZE];
    uint8_t head;
    uint8_t tail;
};

void tl_init(struct traffic_light *tl, uint16_t pin_red, uint16_t pin_yellow,
             uint16_t pin_green);
void tl_set_default_state(struct traffic_light *tl, enum tl_state s);
void tl_handle_state(struct traffic_light *tl);
uint8_t event_buf_push(struct traffic_light *tl, enum tl_event e);
enum tl_event_err tl_handle_next_event(struct traffic_light *tl);

#define MIN_GREEN_SECONDS 10
#define MIN_YELLOW_SECONDS 3
#define MIN_TRANSITION_SECONDS 3
#define TIMER_PERIOD_MS 1000
#define LIGHT_COUNT 4

enum flow_state {
    FLOW_NONE,
    FLOW_NTS,
    FLOW_ETW,
    FLOW_TRANSITION,
    FLOW_FLASHING,
	FLOW_COUNT,
};

struct traffic_light_controller {
    enum flow_state flow;
    enum flow_state previous_flow;

    struct traffic_light tl_north;
    struct traffic_light tl_south;
    struct traffic_light tl_east;
    struct traffic_light tl_west;

	struct traffic_light *lights[LIGHT_COUNT];

    volatile uint32_t traffic_density_nts;
    volatile uint32_t traffic_density_etw;

    volatile uint32_t seconds;
    volatile uint32_t timer;

	lcd_typedef *lcd;
};

void tlc_reset(struct traffic_light_controller *tlc);
void tlc_handle_state(struct traffic_light_controller *tlc);
void tlc_set_flow(struct traffic_light_controller *tlc, enum flow_state f);
