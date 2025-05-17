#include "trafficlight.h"
#include "led.h"

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

void tl_init(struct traffic_light *tl, uint16_t red, uint16_t yellow, uint16_t green) {
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

void tl_handle_state(struct traffic_light *tl) {
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

enum tl_event_err tl_handle_transition_none(struct traffic_light *);
enum tl_event_err tl_handle_transition_red(struct traffic_light *);
enum tl_event_err tl_handle_transition_green(struct traffic_light *);
enum tl_event_err tl_handle_transition_yellow(struct traffic_light *);
enum tl_event_err tl_handle_transition_flashing_yellow(struct traffic_light *);

enum tl_event_err (*tl_event_handlers[TL_EVENT_COUNT])(struct traffic_light *) = {
    tl_handle_transition_none, tl_handle_transition_red,
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

enum tl_event_err tl_handle_transition_flashing_yellow(struct traffic_light *tl) {
    if (tl->state == TL_STATE_YELLOW_FLASHING) return TL_EVENT_ERR_NONE;
    tl->state = TL_STATE_YELLOW_FLASHING;
    return TL_EVENT_ERR_NONE;
}
