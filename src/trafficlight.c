#include "trafficlight.h"
#include "led.h"

#define EVENT_BUF_SIZE 32

static tl_event ebuf_n[EVENT_BUF_SIZE];
static tl_event ebuf_s[EVENT_BUF_SIZE];
static tl_event ebuf_e[EVENT_BUF_SIZE];
static tl_event ebuf_w[EVENT_BUF_SIZE];

static tl_typedef tl_n = {.event_buf = ebuf_n};

static tl_typedef tl_s = {.event_buf = ebuf_s};

static tl_typedef tl_e = {.event_buf = ebuf_e};

static tl_typedef tl_w = {.event_buf = ebuf_w};

static tl_typedef *get_instance(tl_instance tl) {
    switch (tl) {
    case TL_NORTH:
        return &tl_n;
    case TL_SOUTH:
        return &tl_s;
    case TL_EAST:
        return &tl_e;
    case TL_WEST:
        return &tl_w;
    default:
        return NULL;
    }
}

static bool event_buf_full(tl_typedef *tl) {
    return ((tl->head + 1) % EVENT_BUF_SIZE) == tl->tail;
};

static bool event_buf_empty(tl_typedef *tl) {
    return tl->head == tl->tail && !event_buf_full(tl);
}

uint8_t event_buf_push(tl_instance i, tl_event e) {
    tl_typedef *tl = get_instance(i);
    if (event_buf_full(tl)) return 0;
    tl->event_buf[tl->head++] = e;
    if (tl->head >= EVENT_BUF_SIZE) { tl->head = (tl->head % EVENT_BUF_SIZE); }
    return 1;
}

tl_event event_buf_pop(tl_typedef *tl) {
    if (event_buf_empty(tl)) {
        return TL_EVENT_NONE;
    } else {
        tl_event e = tl->event_buf[tl->tail++];
        if (tl->tail >= EVENT_BUF_SIZE) {
            tl->tail = (tl->tail % EVENT_BUF_SIZE);
        }
        return e;
    }
}

void event_buf_init(tl_typedef *tl) {
    for (int i = 0; i < EVENT_BUF_SIZE; i++) {
        tl->event_buf[i] = TL_EVENT_NONE;
    }
}

void tl_init(tl_instance i, uint16_t red, uint16_t yellow, uint16_t green) {
    tl_typedef *tl = get_instance(i);
    tl->r = init_led(red, LED_MODE_SOLID, LED_STATE_OFF);
    tl->y = init_led(yellow, LED_MODE_SOLID, LED_STATE_OFF);
    tl->g = init_led(green, LED_MODE_SOLID, LED_STATE_OFF);
    event_buf_init(tl);
    tl->head = 0;
    tl->tail = 0;
}

void tl_set_default_state(tl_instance i, tl_state s) {
    tl_typedef *tl = get_instance(i);
    tl->state = s;
};

static void tl_off(tl_typedef *tl) {
    if (tl->state != TL_STATE_GREEN) { led_set_state(tl->g, LED_STATE_OFF); }
    if (tl->state != TL_STATE_YELLOW) { led_set_state(tl->y, LED_STATE_OFF); }
    if (tl->state != TL_STATE_RED) { led_set_state(tl->r, LED_STATE_OFF); }
}

void tl_handle_state(tl_instance i) {
    tl_typedef *tl = get_instance(i);
	tl_off(tl);
    switch (tl->state) {
    case TL_STATE_GREEN:
        led_set_state(tl->g, LED_STATE_ON);
		return;
    case TL_STATE_RED:
        led_set_state(tl->r, LED_STATE_ON);
		return;
	case TL_STATE_YELLOW:
		led_set_state(tl->y, LED_STATE_ON);
		return;
    default:
        return;
    }
};

tl_event_error tl_handle_transition_none(tl_typedef *);
tl_event_error tl_handle_transition_red(tl_typedef *);
tl_event_error tl_handle_transition_green(tl_typedef *);
tl_event_error tl_handle_transition_yellow(tl_typedef *);
tl_event_error tl_handle_transition_flashing_yellow(tl_typedef *);

tl_event_error (*tl_event_handlers[TL_EVENT_LENGTH])(tl_typedef *) = {
    tl_handle_transition_none, tl_handle_transition_red,
    tl_handle_transition_green, tl_handle_transition_yellow,
    tl_handle_transition_flashing_yellow};

tl_event_error tl_handle_next_event(tl_instance i) {
    tl_typedef *tl = get_instance(i);
    tl_event e = event_buf_pop(tl);
    return tl_event_handlers[e](tl);
}

tl_event_error tl_handle_transition_none(tl_typedef *tl) {
    (void)tl;
    return TL_EVENT_ERR_NONE;
};

tl_event_error tl_handle_transition_red(tl_typedef *tl) {
    if (tl->state == TL_STATE_RED) return TL_EVENT_ERR_NONE;
    if (tl->state == TL_STATE_GREEN) return TL_EVENT_ERR_GREEN_TO_RED;
    tl->state = TL_STATE_RED;
    return TL_EVENT_ERR_NONE;
}

tl_event_error tl_handle_transition_green(tl_typedef *tl) {
    if (tl->state == TL_STATE_GREEN) return TL_EVENT_ERR_NONE;
    if (tl->state == TL_STATE_YELLOW) return TL_EVENT_ERR_YELLOW_TO_GREEN;
    tl->state = TL_STATE_GREEN;
    return TL_EVENT_ERR_NONE;
}

tl_event_error tl_handle_transition_yellow(tl_typedef *tl) {
    if (tl->state == TL_STATE_YELLOW) return TL_EVENT_ERR_NONE;
    if (tl->state == TL_STATE_RED) return TL_EVENT_ERR_RED_TO_YELLOW;
    tl->state = TL_STATE_YELLOW;
    return TL_EVENT_ERR_NONE;
}

tl_event_error tl_handle_transition_flashing_yellow(tl_typedef *tl) {
    if (tl->state == TL_STATE_YELLOW_FLASHING) return TL_EVENT_ERR_NONE;
    tl->state = TL_STATE_YELLOW_FLASHING;
    return TL_EVENT_ERR_NONE;
}
