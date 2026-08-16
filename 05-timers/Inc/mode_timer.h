typedef enum {
    MODE_TOGGLE,
    MODE_BREATHING
} LedMode_t;

extern volatile LedMode_t current_led_mode;

void mode_timer_init(void);