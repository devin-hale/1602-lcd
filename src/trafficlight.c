#include "trafficlight.h"
#include "lcd.h"
#include "led.h"
#include "stdlib.h"
#include "systick.h"
#include <stdio.h>

#define TLN_G PIN('G', 0)
#define TLN_Y PIN('D', 1)
#define TLN_R PIN('D', 0)

#define TLS_G PIN('F', 0)
#define TLS_Y PIN('F', 1)
#define TLS_R PIN('F', 2)

#define TLE_G PIN('A', 3)
#define TLE_Y PIN('C', 0)
#define TLE_R PIN('C', 3)

#define TLW_G PIN('B', 11)
#define TLW_Y PIN('B', 10)
#define TLW_R PIN('E', 15)

static bool event_buf_full(struct traffic_light *tl) {
    return ((tl->head + 1) % EVENT_BUF_SIZE) == tl->tail;
};

static bool event_buf_empty(struct traffic_light *tl) {
    return tl->head == tl->tail && !event_buf_full(tl);
}

uint8_t event_buf_push(struct traffic_light *tl, enum tl_event e) {
    if (event_buf_full(tl)) return 0;
    tl->event_buf[tl->head++] = e;
    if (tl->head >= EVENT_BUF_SIZE) { tl->head = (tl->head % EVENT_BUF_SIZE); }
    return 1;
}

enum tl_event event_buf_pop(struct traffic_light *tl) {
    if (event_buf_empty(tl)) {
        return TL_EVENT_NONE;
    } else {
        enum tl_event e = tl->event_buf[tl->tail++];
        if (tl->tail >= EVENT_BUF_SIZE) {
            tl->tail = (tl->tail % EVENT_BUF_SIZE);
        }
        return e;
    }
}

void event_buf_init(struct traffic_light *tl) {
    for (int i = 0; i < EVENT_BUF_SIZE; i++) {
        tl->event_buf[i] = TL_EVENT_NONE;
    }
}

void tl_init(struct traffic_light *tl, uint16_t red, uint16_t yellow,
             uint16_t green) {
    tl->r = init_led(red, LED_MODE_SOLID, LED_STATE_OFF);
    tl->y = init_led(yellow, LED_MODE_SOLID, LED_STATE_OFF);
    tl->g = init_led(green, LED_MODE_SOLID, LED_STATE_OFF);
    event_buf_init(tl);
    tl->head = 0;
    tl->tail = 0;
}

void tl_set_default_state(struct traffic_light *tl, enum tl_state s) {
    tl->state = s;
};

static void tl_off(struct traffic_light *tl) {
    if (tl->state != TL_STATE_GREEN) { led_set_state(tl->g, LED_STATE_OFF); }
    if (tl->state != TL_STATE_YELLOW) { led_set_state(tl->y, LED_STATE_OFF); }
    if (tl->state != TL_STATE_RED) { led_set_state(tl->r, LED_STATE_OFF); }
}

void tl_handle_state_green(struct traffic_light *tl);
void tl_handle_state_red(struct traffic_light *tl);
void tl_handle_state_yellow(struct traffic_light *tl);
void tl_handle_state_default(struct traffic_light *tl);

void (*tl_state_handlers[TL_STATE_COUNT])(struct traffic_light *tl) = {
    tl_handle_state_default, tl_handle_state_red, tl_handle_state_yellow,
    tl_handle_state_green, tl_handle_state_default};

void tl_handle_state(struct traffic_light *tl) {
    tl_off(tl);
    tl_state_handlers[tl->state](tl);
};

void tl_handle_state_green(struct traffic_light *tl) {
    led_set_state(tl->g, LED_STATE_ON);
};
void tl_handle_state_red(struct traffic_light *tl) {
    led_set_state(tl->r, LED_STATE_ON);
};
void tl_handle_state_yellow(struct traffic_light *tl) {
    led_set_state(tl->y, LED_STATE_ON);
};
void tl_handle_state_default(struct traffic_light *tl) { (void)tl; };

enum tl_event_err tl_handle_transition_none(struct traffic_light *);
enum tl_event_err tl_handle_transition_red(struct traffic_light *);
enum tl_event_err tl_handle_transition_green(struct traffic_light *);
enum tl_event_err tl_handle_transition_yellow(struct traffic_light *);
enum tl_event_err tl_handle_transition_flashing_yellow(struct traffic_light *);

enum tl_event_err (*tl_event_handlers[TL_EVENT_COUNT])(struct traffic_light *) =
    {tl_handle_transition_none, tl_handle_transition_red,
     tl_handle_transition_green, tl_handle_transition_yellow,
     tl_handle_transition_flashing_yellow};

enum tl_event_err tl_handle_next_event(struct traffic_light *tl) {
    enum tl_event e = event_buf_pop(tl);
    return tl_event_handlers[e](tl);
}

enum tl_event_err tl_handle_transition_none(struct traffic_light *tl) {
    (void)tl;
    return TL_EVENT_ERR_NONE;
};

enum tl_event_err tl_handle_transition_red(struct traffic_light *tl) {
    if (tl->state == TL_STATE_RED) return TL_EVENT_ERR_NONE;
    if (tl->state == TL_STATE_GREEN) return TL_EVENT_ERR_GREEN_TO_RED;
    tl->state = TL_STATE_RED;
    return TL_EVENT_ERR_NONE;
}

enum tl_event_err tl_handle_transition_green(struct traffic_light *tl) {
    if (tl->state == TL_STATE_GREEN) return TL_EVENT_ERR_NONE;
    if (tl->state == TL_STATE_YELLOW) return TL_EVENT_ERR_YELLOW_TO_GREEN;
    tl->state = TL_STATE_GREEN;
    return TL_EVENT_ERR_NONE;
}

enum tl_event_err tl_handle_transition_yellow(struct traffic_light *tl) {
    if (tl->state == TL_STATE_YELLOW) return TL_EVENT_ERR_NONE;
    if (tl->state == TL_STATE_RED) return TL_EVENT_ERR_RED_TO_YELLOW;
    tl->state = TL_STATE_YELLOW;
    return TL_EVENT_ERR_NONE;
}

enum tl_event_err
tl_handle_transition_flashing_yellow(struct traffic_light *tl) {
    if (tl->state == TL_STATE_YELLOW_FLASHING) return TL_EVENT_ERR_NONE;
    tl->state = TL_STATE_YELLOW_FLASHING;
    return TL_EVENT_ERR_NONE;
}

// traffic light

void tlc_reset(struct traffic_light_controller *tlc) {
    tl_init(&tlc->tl_north, TLN_R, TLN_Y, TLN_G);
    tlc->lights[0] = &tlc->tl_north;
    tl_init(&tlc->tl_south, TLS_R, TLS_Y, TLS_G);
    tlc->lights[1] = &tlc->tl_south;
    tl_init(&tlc->tl_east, TLE_R, TLE_Y, TLE_G);
    tlc->lights[2] = &tlc->tl_west;
    tl_init(&tlc->tl_west, TLW_R, TLW_Y, TLW_G);
    tlc->lights[3] = &tlc->tl_east;
};

static char flow_label_none[16] = "";
static char flow_label_nts[16] = "NTS";
static char flow_label_etw[16] = "ETW";
static char flow_label_transition[16] = "TRANSITION";
static char flow_label_flashing[16] = "FLASHING";

static char *flow_label[] = {flow_label_none, flow_label_nts, flow_label_etw,
                             flow_label_transition, flow_label_flashing};

void tlc_handle_flow_none(struct traffic_light_controller *tlc);
void tlc_handle_flow_nts(struct traffic_light_controller *tlc);
void tlc_handle_flow_etw(struct traffic_light_controller *tlc);
void tlc_handle_flow_transition(struct traffic_light_controller *tlc);

void (*tlc_flow_handlers[])(struct traffic_light_controller *tlc) = {
    tlc_handle_flow_none, tlc_handle_flow_nts, tlc_handle_flow_etw,
    tlc_handle_flow_transition, tlc_handle_flow_none};

void tlc_set_flow(struct traffic_light_controller *tlc, enum flow_state f) {
    printf("Set flow: %s\r\n", flow_label[f]);
    tlc->previous_flow = tlc->flow;
    tlc->flow = f;
    tlc->seconds =
        f == FLOW_TRANSITION ? MIN_TRANSITION_SECONDS : MIN_GREEN_SECONDS;
}

void tlc_transition(struct traffic_light_controller *tlc) {
    switch (tlc->flow) {
    case FLOW_NTS:
    case FLOW_ETW:
        tlc_set_flow(tlc, FLOW_TRANSITION);
        break;
    case FLOW_TRANSITION:
        if (tlc->previous_flow == FLOW_NTS) {
            tlc_set_flow(tlc, FLOW_ETW);
        } else if (tlc->previous_flow == FLOW_ETW) {
            tlc_set_flow(tlc, FLOW_NTS);
        } else {
            // handle error
        }
        break;
    default:
        break;
    }
}

void tlc_handle_state(struct traffic_light_controller *tlc) {
    if (timer_expired_ms(&tlc->timer, TIMER_PERIOD_MS)) {
        if (tlc->seconds != 0) {
            tlc->seconds--;
        } else {
            tlc_transition(tlc);
        }
        char str[8];
        clear_and_write(tlc->lcd, itoa((int)tlc->seconds, str, 10));
        printf("Seconds: %lu\r\n", (unsigned long)tlc->seconds);
    }
    tlc_flow_handlers[tlc->flow](tlc);
    for (int i = 0; i < LIGHT_COUNT; i++) {
        struct traffic_light *tl = tlc->lights[i];
        tl_handle_next_event(tl);
        tl_handle_state(tl);
    }
};

void tlc_handle_flow_none(struct traffic_light_controller *tlc) { (void)tlc; };

void tlc_handle_flow_nts(struct traffic_light_controller *tlc) {
    if (tlc->seconds > MIN_YELLOW_SECONDS) {
        if (tlc->tl_north.state != TL_STATE_GREEN) {
            event_buf_push(&tlc->tl_north, TL_EVENT_TRANSITION_GREEN);
        }
        if (tlc->tl_south.state != TL_STATE_GREEN) {
            event_buf_push(&tlc->tl_south, TL_EVENT_TRANSITION_GREEN);
        }
    } else {
        if (tlc->tl_north.state != TL_STATE_YELLOW) {
            event_buf_push(&tlc->tl_north, TL_EVENT_TRANSITION_YELLOW);
        }
        if (tlc->tl_south.state != TL_STATE_YELLOW) {
            event_buf_push(&tlc->tl_south, TL_EVENT_TRANSITION_YELLOW);
        }
    }
    if (tlc->tl_east.state != TL_STATE_RED) {
        event_buf_push(&tlc->tl_east, TL_EVENT_TRANSITION_RED);
    }
    if (tlc->tl_west.state != TL_STATE_RED) {
        event_buf_push(&tlc->tl_west, TL_EVENT_TRANSITION_RED);
    }
};

void tlc_handle_flow_etw(struct traffic_light_controller *tlc) {
    if (tlc->seconds > MIN_YELLOW_SECONDS) {
        if (tlc->tl_east.state != TL_STATE_GREEN) {
            event_buf_push(&tlc->tl_east, TL_EVENT_TRANSITION_GREEN);
        }
        if (tlc->tl_west.state != TL_STATE_GREEN) {
            event_buf_push(&tlc->tl_west, TL_EVENT_TRANSITION_GREEN);
        }
    } else {
        if (tlc->tl_east.state != TL_STATE_YELLOW) {
            event_buf_push(&tlc->tl_east, TL_EVENT_TRANSITION_YELLOW);
        }
        if (tlc->tl_west.state != TL_STATE_YELLOW) {
            event_buf_push(&tlc->tl_west, TL_EVENT_TRANSITION_YELLOW);
        }
    }
    if (tlc->tl_north.state != TL_STATE_RED) {
        event_buf_push(&tlc->tl_north, TL_EVENT_TRANSITION_RED);
    }
    if (tlc->tl_south.state != TL_STATE_RED) {
        event_buf_push(&tlc->tl_south, TL_EVENT_TRANSITION_RED);
    }
};

void tlc_handle_flow_transition(struct traffic_light_controller *tlc) {
    if (tlc->tl_north.state != TL_STATE_RED) {
        event_buf_push(&tlc->tl_north, TL_EVENT_TRANSITION_RED);
    }
    if (tlc->tl_south.state != TL_STATE_RED) {
        event_buf_push(&tlc->tl_south, TL_EVENT_TRANSITION_RED);
    }
    if (tlc->tl_east.state != TL_STATE_RED) {
        event_buf_push(&tlc->tl_east, TL_EVENT_TRANSITION_RED);
    }
    if (tlc->tl_west.state != TL_STATE_RED) {
        event_buf_push(&tlc->tl_west, TL_EVENT_TRANSITION_RED);
    }
};
