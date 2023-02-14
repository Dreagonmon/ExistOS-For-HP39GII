#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

// ┌─────┬───────┬───────┬────────┬──────────┬────────┐
// |     | 0     | 1     | 2      | 3        | 4      |
// ├─────┼───────┼───────┼────────┼──────────┼────────┤
// | 0   | f1    | f3    | f4     | f5       | up     |
// ├─────┼───────┼───────┼────────┼──────────┼────────┤
// | 1   | symb  | f2    | num    | f6       | right  |
// ├─────┼───────┼───────┼────────┼──────────┼────────┤
// | 2   | home  | plot  | views  | x,t,Φ,n  | left   |
// ├─────┼───────┼───────┼────────┼──────────┼────────┤
// | 3   | vars  | apps  | a b/c  | ←        | down   |
// ├─────┼───────┼───────┼────────┼──────────┼────────┤
// | 4   | sin   | math  | tan    | ln       | log    |
// ├─────┼───────┼───────┼────────┼──────────┼────────┤
// | 5   | x^2   | cos   | (      | )        | ÷      |
// ├─────┼───────┼───────┼────────┼──────────┼────────┤
// | 6   | ,     | x^y   | 8      | 9        | x      |
// ├─────┼───────┼───────┼────────┼──────────┼────────┤
// | 7   | alpha | 7     | 5      | 6        | -      |
// ├─────┼───────┼───────┼────────┼──────────┼────────┤
// | 8   | shift | 4     | 2      | 3        | +      |
// ├─────┼───────┼───────┼────────┼──────────┼────────┤
// | 9   | 0     | 1     | .      | (-)      | enter  |
// ├─────┼───────┼───────┼────────┼──────────┼────────┤
// | 10  | on/c  |       |        |          |        |
// └─────┴───────┴───────┴────────┴──────────┴────────┘

#define kbd_K_F1                  ((0 << 3) + 0)
#define kbd_K_F2                  ((1 << 3) + 1)
#define kbd_K_F3                  ((0 << 3) + 1)
#define kbd_K_F4                  ((0 << 3) + 2)
#define kbd_K_F5                  ((0 << 3) + 3)
#define kbd_K_F6                  ((1 << 3) + 3)
#define kbd_K_UP                  ((0 << 3) + 4)
#define kbd_K_RIGHT               ((1 << 3) + 4)
#define kbd_K_LEFT                ((2 << 3) + 4)
#define kbd_K_DOWN                ((3 << 3) + 4)
#define kbd_K_SYMB                ((1 << 3) + 0)
#define kbd_K_NUM                 ((1 << 3) + 2)
#define kbd_K_HOME                ((2 << 3) + 0)
#define kbd_K_PLOT                ((2 << 3) + 1)
#define kbd_K_VIEWS               ((2 << 3) + 2)
#define kbd_K_XTPHIN              ((2 << 3) + 3)
#define kbd_K_VARS                ((3 << 3) + 0)
#define kbd_K_APPS                ((3 << 3) + 1)
#define kbd_K_ABC                 ((3 << 3) + 2)
#define kbd_K_BACKSPACE           ((3 << 3) + 3)
#define kbd_K_SIN                 ((4 << 3) + 0)
#define kbd_K_MATH                ((4 << 3) + 1)
#define kbd_K_TAN                 ((4 << 3) + 2)
#define kbd_K_LN                  ((4 << 3) + 3)
#define kbd_K_LOG                 ((4 << 3) + 4)
#define kbd_K_X2                  ((5 << 3) + 0)
#define kbd_K_COS                 ((5 << 3) + 1)
#define kbd_K_LEFTBRACKET         ((5 << 3) + 2)
#define kbd_K_RIGHTBRACKET        ((5 << 3) + 3)
#define kbd_K_DIVISION            ((5 << 3) + 4)
#define kbd_K_COMMA               ((6 << 3) + 0)
#define kbd_K_XY                  ((6 << 3) + 1)
#define kbd_K_8                   ((6 << 3) + 2)
#define kbd_K_9                   ((6 << 3) + 3)
#define kbd_K_MULTIPLICATION      ((6 << 3) + 4)
#define kbd_K_ALPHA               ((7 << 3) + 0)
#define kbd_K_7                   ((7 << 3) + 1)
#define kbd_K_5                   ((7 << 3) + 2)
#define kbd_K_6                   ((7 << 3) + 3)
#define kbd_K_SUBTRACTION         ((7 << 3) + 4)
#define kbd_K_SHIFT               ((8 << 3) + 0)
#define kbd_K_4                   ((8 << 3) + 1)
#define kbd_K_2                   ((8 << 3) + 2)
#define kbd_K_3                   ((8 << 3) + 3)
#define kbd_K_PLUS                ((8 << 3) + 4)
#define kbd_K_0                   ((9 << 3) + 0)
#define kbd_K_1                   ((9 << 3) + 1)
#define kbd_K_DOT                 ((9 << 3) + 2)
#define kbd_K_NEGATIVE            ((9 << 3) + 3)
#define kbd_K_ENTER               ((9 << 3) + 4)
#define kbd_K_ON                  ((10 << 3) + 0)

#define kbd_ACTION_NOP       0x0
#define kbd_ACTION_DOWN      0x1
#define kbd_ACTION_UP        0x2

#define kbd_action(x) (x & 0xFFFF)
#define kbd_value(x) ((x >> 16) & 0xFFFF)

uint8_t kbd_check_key(uint16_t key_to_check);
uint32_t kbd_query_event();

#endif