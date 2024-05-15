#ifndef __INPUT_EVENT_LABE_H__
#define __INPUT_EVENT_LABE_H__
#include <view/keyevent.h>
#include <view/motionevent.h>
#include <string.h>
#include <stdint.h>

namespace cdroid{

struct InputEventLabel {
    const char *literal;
    int value;
};

#define DEFINE_KEY(key)   { #key, KeyEvent::KEYCODE_##key }
#define DEFINE_AXIS(axis) { #axis, MotionEvent::AXIS_##axis }
#define DEFINE_LED(led)   { #led, int(KeyEvent::LED_##led) }
#define DEFINE_FLAG(flag) { #flag, POLICY_FLAG_##flag }

static const InputEventLabel KEYCODES[] = {
    DEFINE_KEY(UNKNOWN),
    DEFINE_KEY(SOFT_LEFT),
    DEFINE_KEY(SOFT_RIGHT),
    DEFINE_KEY(HOME),
    DEFINE_KEY(BACK),
    DEFINE_KEY(CALL),
    DEFINE_KEY(ENDCALL),
    DEFINE_KEY(0),
    DEFINE_KEY(1),
    DEFINE_KEY(2),
    DEFINE_KEY(3),
    DEFINE_KEY(4),
    DEFINE_KEY(5),
    DEFINE_KEY(6),
    DEFINE_KEY(7),
    DEFINE_KEY(8),
    DEFINE_KEY(9),
    DEFINE_KEY(STAR),
    DEFINE_KEY(POUND),
    DEFINE_KEY(DPAD_UP),
    DEFINE_KEY(DPAD_DOWN),
    DEFINE_KEY(DPAD_LEFT),
    DEFINE_KEY(DPAD_RIGHT),
    DEFINE_KEY(DPAD_CENTER),
    DEFINE_KEY(VOLUME_UP),
    DEFINE_KEY(VOLUME_DOWN),
    DEFINE_KEY(POWER),
    DEFINE_KEY(CAMERA),
    DEFINE_KEY(CLEAR),
    DEFINE_KEY(A),
    DEFINE_KEY(B),
    DEFINE_KEY(C),
    DEFINE_KEY(D),
    DEFINE_KEY(E),
    DEFINE_KEY(F),
    DEFINE_KEY(G),
    DEFINE_KEY(H),
    DEFINE_KEY(I),
    DEFINE_KEY(J),
    DEFINE_KEY(K),
    DEFINE_KEY(L),
    DEFINE_KEY(M),
    DEFINE_KEY(N),
    DEFINE_KEY(O),
    DEFINE_KEY(P),
    DEFINE_KEY(Q),
    DEFINE_KEY(R),
    DEFINE_KEY(S),
    DEFINE_KEY(T),
    DEFINE_KEY(U),
    DEFINE_KEY(V),
    DEFINE_KEY(W),
    DEFINE_KEY(X),
    DEFINE_KEY(Y),
    DEFINE_KEY(Z),
    DEFINE_KEY(COMMA),
    DEFINE_KEY(PERIOD),
    DEFINE_KEY(ALT_LEFT),
    DEFINE_KEY(ALT_RIGHT),
    DEFINE_KEY(SHIFT_LEFT),
    DEFINE_KEY(SHIFT_RIGHT),
    DEFINE_KEY(CTRL_LEFT),
    DEFINE_KEY(CTRL_RIGHT),
    DEFINE_KEY(TAB),
    DEFINE_KEY(SPACE),
    DEFINE_KEY(SYM),
    DEFINE_KEY(EXPLORER),
    DEFINE_KEY(ENVELOPE),
    DEFINE_KEY(ENTER),
    DEFINE_KEY(DEL),
    DEFINE_KEY(GRAVE),
    DEFINE_KEY(MINUS),
    DEFINE_KEY(EQUALS),
    DEFINE_KEY(LEFTBRACE),
    DEFINE_KEY(RIGHTBRACE),
    DEFINE_KEY(BACKSLASH),
    DEFINE_KEY(SEMICOLON),
    DEFINE_KEY(APOSTROPHE),
    DEFINE_KEY(SLASH),
    DEFINE_KEY(AT),
    DEFINE_KEY(NUM),
    DEFINE_KEY(HEADSETHOOK),
    DEFINE_KEY(FOCUS),   // *Camera* focus
    DEFINE_KEY(PLUS),
    DEFINE_KEY(MENU),
    DEFINE_KEY(NOTIFICATION),
    DEFINE_KEY(SEARCH),
    DEFINE_KEY(MEDIA_PLAY_PAUSE),
    DEFINE_KEY(MEDIA_STOP),
    DEFINE_KEY(MEDIA_NEXT),
    DEFINE_KEY(MEDIA_PREVIOUS),
    DEFINE_KEY(MEDIA_REWIND),
    DEFINE_KEY(MEDIA_FAST_FORWARD),
    DEFINE_KEY(MUTE),
    DEFINE_KEY(PAGE_UP),
    DEFINE_KEY(PAGE_DOWN),
    DEFINE_KEY(PICTSYMBOLS),
    DEFINE_KEY(SWITCH_CHARSET),
    DEFINE_KEY(BUTTON_A),
    DEFINE_KEY(BUTTON_B),
    DEFINE_KEY(BUTTON_C),
    DEFINE_KEY(BUTTON_X),
    DEFINE_KEY(BUTTON_Y),
    DEFINE_KEY(BUTTON_Z),
    DEFINE_KEY(BUTTON_L1),
    DEFINE_KEY(BUTTON_R1),
    DEFINE_KEY(BUTTON_L2),
    DEFINE_KEY(BUTTON_R2),
    DEFINE_KEY(BUTTON_THUMBL),
    DEFINE_KEY(BUTTON_THUMBR),
    DEFINE_KEY(BUTTON_START),
    DEFINE_KEY(BUTTON_SELECT),
    DEFINE_KEY(BUTTON_MODE),
    DEFINE_KEY(ESCAPE),
    DEFINE_KEY(BACKSPACE),
    DEFINE_KEY(CTRL_LEFT),
    DEFINE_KEY(CTRL_RIGHT),
    DEFINE_KEY(CAPS_LOCK),
    DEFINE_KEY(SCROLL_LOCK),
    DEFINE_KEY(META_LEFT),
    DEFINE_KEY(META_RIGHT),
    DEFINE_KEY(FUNCTION),
    DEFINE_KEY(SYSRQ),
    DEFINE_KEY(BREAK),
    DEFINE_KEY(MOVE_HOME),
    DEFINE_KEY(MOVE_END),
    DEFINE_KEY(INSERT),
    DEFINE_KEY(FORWARD),
    DEFINE_KEY(MEDIA_PLAY),
    DEFINE_KEY(MEDIA_PAUSE),
    DEFINE_KEY(MEDIA_CLOSE),
    DEFINE_KEY(MEDIA_EJECT),
    DEFINE_KEY(MEDIA_RECORD),
    DEFINE_KEY(F1),
    DEFINE_KEY(F2),
    DEFINE_KEY(F3),
    DEFINE_KEY(F4),
    DEFINE_KEY(F5),
    DEFINE_KEY(F6),
    DEFINE_KEY(F7),
    DEFINE_KEY(F8),
    DEFINE_KEY(F9),
    DEFINE_KEY(F10),
    DEFINE_KEY(F11),
    DEFINE_KEY(F12),
    DEFINE_KEY(NUM_LOCK),
    DEFINE_KEY(NUMPAD_0),
    DEFINE_KEY(NUMPAD_1),
    DEFINE_KEY(NUMPAD_2),
    DEFINE_KEY(NUMPAD_3),
    DEFINE_KEY(NUMPAD_4),
    DEFINE_KEY(NUMPAD_5),
    DEFINE_KEY(NUMPAD_6),
    DEFINE_KEY(NUMPAD_7),
    DEFINE_KEY(NUMPAD_8),
    DEFINE_KEY(NUMPAD_9),
    DEFINE_KEY(NUMPAD_DIVIDE),
    DEFINE_KEY(NUMPAD_MULTIPLY),
    DEFINE_KEY(NUMPAD_SUBTRACT),
    DEFINE_KEY(NUMPAD_ADD),
    DEFINE_KEY(NUMPAD_DOT),
    DEFINE_KEY(NUMPAD_COMMA),
    DEFINE_KEY(NUMPAD_ENTER),
    DEFINE_KEY(NUMPAD_EQUALS),
    DEFINE_KEY(NUMPAD_LEFT_PAREN),
    DEFINE_KEY(NUMPAD_RIGHT_PAREN),
    DEFINE_KEY(VOLUME_MUTE),
    DEFINE_KEY(INFO),
    DEFINE_KEY(CHANNEL_UP),
    DEFINE_KEY(CHANNEL_DOWN),
    DEFINE_KEY(ZOOM_IN),
    DEFINE_KEY(ZOOM_OUT),
    DEFINE_KEY(TV),
    DEFINE_KEY(WINDOW),
    DEFINE_KEY(GUIDE),
    DEFINE_KEY(DVR),
    DEFINE_KEY(BOOKMARK),
    DEFINE_KEY(CAPTIONS),
    DEFINE_KEY(SETTINGS),
    DEFINE_KEY(TV_POWER),
    DEFINE_KEY(TV_INPUT),
    DEFINE_KEY(STB_POWER),
    DEFINE_KEY(STB_INPUT),
    DEFINE_KEY(AVR_POWER),
    DEFINE_KEY(AVR_INPUT),
    DEFINE_KEY(PROG_RED),
    DEFINE_KEY(PROG_GREEN),
    DEFINE_KEY(PROG_YELLOW),
    DEFINE_KEY(PROG_BLUE),
    DEFINE_KEY(APP_SWITCH),
    DEFINE_KEY(BUTTON_1),
    DEFINE_KEY(BUTTON_2),
    DEFINE_KEY(BUTTON_3),
    DEFINE_KEY(BUTTON_4),
    DEFINE_KEY(BUTTON_5),
    DEFINE_KEY(BUTTON_6),
    DEFINE_KEY(BUTTON_7),
    DEFINE_KEY(BUTTON_8),
    DEFINE_KEY(BUTTON_9),
    DEFINE_KEY(BUTTON_10),
    DEFINE_KEY(BUTTON_11),
    DEFINE_KEY(BUTTON_12),
    DEFINE_KEY(BUTTON_13),
    DEFINE_KEY(BUTTON_14),
    DEFINE_KEY(BUTTON_15),
    DEFINE_KEY(BUTTON_16),
    DEFINE_KEY(LANGUAGE_SWITCH),
    DEFINE_KEY(MANNER_MODE),
    DEFINE_KEY(3D_MODE),
    DEFINE_KEY(CONTACTS),
    DEFINE_KEY(CALENDAR),
    DEFINE_KEY(MUSIC),
    DEFINE_KEY(CALCULATOR),
    DEFINE_KEY(ZENKAKU_HANKAKU),
    DEFINE_KEY(EISU),
    DEFINE_KEY(MUHENKAN),
    DEFINE_KEY(HENKAN),
    DEFINE_KEY(KATAKANA_HIRAGANA),
    DEFINE_KEY(YEN),
    DEFINE_KEY(RO),
    DEFINE_KEY(KANA),
    DEFINE_KEY(ASSIST),
    DEFINE_KEY(BRIGHTNESS_DOWN),
    DEFINE_KEY(BRIGHTNESS_UP),
    DEFINE_KEY(MEDIA_AUDIO_TRACK),
    DEFINE_KEY(SLEEP),
    DEFINE_KEY(WAKEUP),
//////////////////////////////////////
//    DEFINE_KEY(OK),
//    DEFINE_KEY(EPG),
//    DEFINE_KEY(TELETEXT),
//    DEFINE_KEY(SUBTITLE),
//    DEFINE_KEY(AUDIO),
    DEFINE_KEY(PAIRING),
    DEFINE_KEY(MEDIA_TOP_MENU),
    DEFINE_KEY(11),
    DEFINE_KEY(12),
    DEFINE_KEY(LAST_CHANNEL),
    DEFINE_KEY(TV_DATA_SERVICE),
    DEFINE_KEY(VOICE_ASSIST),
    DEFINE_KEY(TV_RADIO_SERVICE),
    DEFINE_KEY(TV_TELETEXT),
    DEFINE_KEY(TV_NUMBER_ENTRY),
    DEFINE_KEY(TV_TERRESTRIAL_ANALOG),
    DEFINE_KEY(TV_TERRESTRIAL_DIGITAL),
    DEFINE_KEY(TV_SATELLITE),
    DEFINE_KEY(TV_SATELLITE_BS),
    DEFINE_KEY(TV_SATELLITE_CS),
    DEFINE_KEY(TV_SATELLITE_SERVICE),
    DEFINE_KEY(TV_NETWORK),
    DEFINE_KEY(TV_ANTENNA_CABLE),
    DEFINE_KEY(TV_INPUT_HDMI_1),
    DEFINE_KEY(TV_INPUT_HDMI_2),
    DEFINE_KEY(TV_INPUT_HDMI_3),
    DEFINE_KEY(TV_INPUT_HDMI_4),
    DEFINE_KEY(TV_INPUT_COMPOSITE_1),
    DEFINE_KEY(TV_INPUT_COMPOSITE_2),
    DEFINE_KEY(TV_INPUT_COMPONENT_1),
    DEFINE_KEY(TV_INPUT_COMPONENT_2),
    DEFINE_KEY(TV_INPUT_VGA_1),
    DEFINE_KEY(TV_AUDIO_DESCRIPTION),
    DEFINE_KEY(TV_AUDIO_DESCRIPTION_MIX_UP),
    DEFINE_KEY(TV_AUDIO_DESCRIPTION_MIX_DOWN),
    DEFINE_KEY(TV_ZOOM_MODE),
    DEFINE_KEY(TV_CONTENTS_MENU),
    DEFINE_KEY(TV_MEDIA_CONTEXT_MENU),
    DEFINE_KEY(TV_TIMER_PROGRAMMING),
    DEFINE_KEY(HELP),
//    DEFINE_KEY(NAVIGATE_PREVIOUS),
//    DEFINE_KEY(NAVIGATE_NEXT),
//    DEFINE_KEY(NAVIGATE_IN),
//    DEFINE_KEY(NAVIGATE_OUT),
    DEFINE_KEY(STEM_PRIMARY),
    DEFINE_KEY(STEM_1),
    DEFINE_KEY(STEM_2),
    DEFINE_KEY(STEM_3),
    DEFINE_KEY(DPAD_UP_LEFT),
    DEFINE_KEY(DPAD_DOWN_LEFT),
    DEFINE_KEY(DPAD_UP_RIGHT),
    DEFINE_KEY(DPAD_DOWN_RIGHT),
    DEFINE_KEY(MEDIA_SKIP_FORWARD),
    DEFINE_KEY(MEDIA_SKIP_BACKWARD),
    DEFINE_KEY(MEDIA_STEP_FORWARD),
    DEFINE_KEY(MEDIA_STEP_BACKWARD),
    DEFINE_KEY(SOFT_SLEEP),
    DEFINE_KEY(CUT),
    DEFINE_KEY(COPY),
    DEFINE_KEY(PASTE),
    DEFINE_KEY(SYSTEM_NAVIGATION_UP),
    DEFINE_KEY(SYSTEM_NAVIGATION_DOWN),
    DEFINE_KEY(SYSTEM_NAVIGATION_LEFT),
    DEFINE_KEY(SYSTEM_NAVIGATION_RIGHT),
    DEFINE_KEY(ALL_APPS),
    DEFINE_KEY(REFRESH),
    { NULL, 0 }
};

static const InputEventLabel AXES[] = {
    DEFINE_AXIS(X),
    DEFINE_AXIS(Y),
    DEFINE_AXIS(PRESSURE),
    DEFINE_AXIS(SIZE),
    DEFINE_AXIS(TOUCH_MAJOR),
    DEFINE_AXIS(TOUCH_MINOR),
    DEFINE_AXIS(TOOL_MAJOR),
    DEFINE_AXIS(TOOL_MINOR),
    DEFINE_AXIS(ORIENTATION),
    DEFINE_AXIS(VSCROLL),
    DEFINE_AXIS(HSCROLL),
    DEFINE_AXIS(Z),
    DEFINE_AXIS(RX),
    DEFINE_AXIS(RY),
    DEFINE_AXIS(RZ),
    DEFINE_AXIS(HAT_X),
    DEFINE_AXIS(HAT_Y),
    DEFINE_AXIS(LTRIGGER),
    DEFINE_AXIS(RTRIGGER),
    DEFINE_AXIS(THROTTLE),
    DEFINE_AXIS(RUDDER),
    DEFINE_AXIS(WHEEL),
    DEFINE_AXIS(GAS),
    DEFINE_AXIS(BRAKE),
    DEFINE_AXIS(DISTANCE),
    DEFINE_AXIS(TILT),
    DEFINE_AXIS(GENERIC_1),
    DEFINE_AXIS(GENERIC_2),
    DEFINE_AXIS(GENERIC_3),
    DEFINE_AXIS(GENERIC_4),
    DEFINE_AXIS(GENERIC_5),
    DEFINE_AXIS(GENERIC_6),
    DEFINE_AXIS(GENERIC_7),
    DEFINE_AXIS(GENERIC_8),
    DEFINE_AXIS(GENERIC_9),
    DEFINE_AXIS(GENERIC_10),
    DEFINE_AXIS(GENERIC_11),
    DEFINE_AXIS(GENERIC_12),
    DEFINE_AXIS(GENERIC_13),
    DEFINE_AXIS(GENERIC_14),
    DEFINE_AXIS(GENERIC_15),
    DEFINE_AXIS(GENERIC_16),

    // NOTE: If you add a new axis here you must also add it to several other files.
    //       Refer to frameworks/base/core/java/android/view/MotionEvent.java for the full list.
    { NULL, 0 }
};
static const InputEventLabel LEDS[] = {
    DEFINE_LED(NUM_LOCK),
    DEFINE_LED(CAPS_LOCK),
    DEFINE_LED(SCROLL_LOCK),
    DEFINE_LED(COMPOSE),
    DEFINE_LED(KANA),
    DEFINE_LED(SLEEP),
    DEFINE_LED(SUSPEND),
    DEFINE_LED(MUTE),
    DEFINE_LED(MISC),
    DEFINE_LED(MAIL),
    DEFINE_LED(CHARGING),
    DEFINE_LED(CONTROLLER_1),
    DEFINE_LED(CONTROLLER_2),
    DEFINE_LED(CONTROLLER_3),
    DEFINE_LED(CONTROLLER_4),

    // NOTE: If you add new LEDs here, you must also add them to Input.h
    { NULL, 0 }
};
/*static const InputEventLabel FLAGS[] = {
    DEFINE_FLAG(VIRTUAL),
    DEFINE_FLAG(FUNCTION),
    DEFINE_FLAG(GESTURE),

    { NULL, 0 }
};
*/
static int lookupValueByLabel(const char* literal, const InputEventLabel *list) {
    while (list->literal) {
        if (strcmp(literal, list->literal) == 0) {
            return list->value;
        }
        list++;
    }
    return list->value;
}

static const char* lookupLabelByValue(int value, const InputEventLabel* list) {
    while (list->literal) {
        if (list->value == value) {
            return list->literal;
        }
        list++;
    }
    return NULL;
}

static inline int32_t getKeyCodeByLabel(const char* label) {
    return int32_t(lookupValueByLabel(label, KEYCODES));
}

static inline const char* getLabelByKeyCode(int32_t keyCode) {
    for(int i=0;i<sizeof(KEYCODES)/sizeof(KEYCODES[0]);i++) {
        if(KEYCODES[i].value==keyCode)return KEYCODES[i].literal;
    }
    return NULL;
}

/*static inline uint32_t getKeyFlagByLabel(const char* label) {
    return uint32_t(lookupValueByLabel(label, FLAGS));
}*/

static inline int32_t getAxisByLabel(const char* label) {
    return int32_t(lookupValueByLabel(label, AXES));
}

static inline const char* getAxisLabel(int32_t axisId) {
    return lookupLabelByValue(axisId, AXES);
}

static inline int32_t getLedByLabel(const char* label) {
    return int32_t(lookupValueByLabel(label, LEDS));
}

}/*endof namespace*/
#endif