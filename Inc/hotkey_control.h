#ifndef __HOTKEY_CONTROL_H
#define __HOTKEY_CONTROL_H

#define INT_EVENT_CLASS         0x06        //Event class 6
#define hotkey_trigger_timermax 10          //base on 10ms task event => 10*10 = 150ms
#define HOTKEY_BUTTON_UP        0x00
#define HOTKEY_HOME             0x01
#define HOTKEY_KEY_MENU         0x02
#define HOTKEY_BACK             0x04
#define HOTKEY_DOWN             0x08
#define HOTKEY_UP               0x10
#define COMB_BACK_UP            0x80

#define KEYBOARD_A              0x04
#define KEYBOARD_B              0x05
#define KEYBOARD_C              0x06
#define KEYBOARD_D              0x07
#define KEYBOARD_E              0x08
#define KEYBOARD_F              0x09
#define KEYBOARD_G              0x0A
#define KEYBOARD_H              0x0B
#define KEYBOARD_I              0x0C
#define KEYBOARD_J              0x0D
#define KEYBOARD_K              0x0E
#define KEYBOARD_L              0x0F
#define KEYBOARD_M              0x10
#define KEYBOARD_N              0x11
#define KEYBOARD_O              0x12
#define KEYBOARD_P              0x13
#define KEYBOARD_Q              0x14
#define KEYBOARD_R              0x15
#define KEYBOARD_S              0x16
#define KEYBOARD_T              0x17
#define KEYBOARD_U              0x18
#define KEYBOARD_V              0x19
#define KEYBOARD_W              0x1A
#define KEYBOARD_X              0x1B
#define KEYBOARD_Y              0x1C
#define KEYBOARD_Z              0x1D
#define KEYBOARD_1              0x1E
#define KEYBOARD_2              0x1F
#define KEYBOARD_3              0x20
#define KEYBOARD_4              0x21
#define KEYBOARD_5              0x22
#define KEYBOARD_6              0x23
#define KEYBOARD_7              0x24
#define KEYBOARD_8              0x25
#define KEYBOARD_9              0x26
#define KEYBOARD_0              0x27
#define KEYBOARD_F1             0x3A
#define KEYBOARD_F2             0x3B
#define KEYBOARD_F3             0x3C
#define KEYBOARD_F4             0x3D
#define KEYBOARD_F5             0x3E
#define KEYBOARD_F6             0x3F
#define KEYBOARD_F7             0x40
#define KEYBOARD_F8             0x41
#define KEYBOARD_F9             0x42
#define KEYBOARD_F10            0x43
#define KEYBOARD_F11            0x44
#define KEYBOARD_F12            0x45
#define KEYBOARD_ESC            0x29
#define KEYBOARD_GRAVE_ACCENT   0x35
#define KEYBOARD_TAB            0x2B
#define KEYBOARD_CAPS_LOCK      0x39
#define KEYBOARD_L_SHIFT        0xE1
#define KEYBOARD_L_CTRL         0xE0
#define KEYBOARD_L_GUI          0xE3
#define KEYBOARD_L_ALT          0xE2
#define KEYBOARD_MINUS          0x2D
#define KEYBOARD_EQUAL          0x2E
#define KEYBOARD_BACKSPACE      0x2A
#define KEYBOARD_L_QUOTATION    0x2F
#define KEYBOARD_R_QUOTATION    0x30
#define KEYBOARD_BACKSLASH      0x31
#define KEYBOARD_SEMICOLON      0x33
#define KEYBOARD_APOSTROPHE     0x34
#define KEYBOARD_ENTER          0x28
#define KEYBOARD_COMMA          0x36
#define KEYBOARD_PERIOD         0x37
#define KEYBOARD_SLASH          0x38
#define KEYBOARD_R_SHIFT        0xE5
#define KEYBOARD_R_CTRL         0xE4
#define KEYBOARD_APPLICATION    0x65
#define KEYBOARD_R_GUI          0xE7
#define KEYBOARD_R_ALT          0xE6
#define KEYBOARD_SPACEBAR       0x2C
#define KEYBOARD_PRINT_SCREEN   0x46
#define KEYBOARD_SCROLL_LOCK    0x84
#define KEYBOARD_PAUSE          0x48
#define KEYBOARD_INSERT         0x49
#define KEYBOARD_HOME           0x4A
#define KEYBOARD_PAGEUP         0x4B
#define KEYBOARD_DELETE         0x4C
#define KEYBOARD_END            0x4D
#define KEYBOARD_PAGEDOWN       0x4E
#define KEYBOARD_LEFT_ARROW     0x50
#define KEYBOARD_UP_ARROW       0x52
#define KEYBOARD_RIGHT_ARROW    0x4F
#define KEYBOARD_DOWN_ARROW     0x51
#define KEYBOARD_NUM_LOCK       0x83


void USB_KEYBOARD_TRANSFER(void);
void TASK_KEYBOARD_SCAN(void);

#endif