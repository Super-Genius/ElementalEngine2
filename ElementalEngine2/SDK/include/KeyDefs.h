///============================================================================
/// \file		KeyDefs.h
/// \brief		Header file for <insert brief description here>
/// \date		04-23-2005
/// \author		Josh Lulewicz
/// \note   Elemental Engine
///         Copyright (c)  2005-2008 Signature Devices, Inc.
///
///         This code is redistributable under the terms of the EE License.
///
///         This code is distributed without warranty or implied warranty of
///			merchantability or fitness for a particular purpose. See the 
///			EE License for more details.
///
///         You should have received a copy of the EE License along with this
///			code; If not, write to Signature Devices, Inc.,
///			3200 Bridge Parkway Suite 102, Redwood City, CA 94086 USA.
///============================================================================

//
//                                *** KEY CODES ***
//

// Generic
#define EEK_UNKNOWN    0 // Unknown key

// Control keys
#define EEK_LSHIFT     2 // Left shift (or shift, if no right shift exists)
#define EEK_RSHIFT     3 // Right shift
#define EEK_LCTRL      29 // Left control (or control, if no right control exists)
#define EEK_RCTRL      157 // Right control

#define EEK_LALT       6 // Left alt (or alt, if no right alt exists)
#define EEK_RALT       7 // Right Alt
#define EEK_ALTGR      7 // AltGr (Right Alt on German keyboards)

#define EEK_BACKSP     14 // Backspace (ASCII compliant)
#define EEK_TAB        15 // 

#define EEK_LWIN      219 // Left Windows key
#define EEK_RWIN      220 // Right Windows key
#define EEK_APPS      12 // Windows menu (application) key  //Used to be called EEK_MENU incorrectly
#define EEK_ENTER     28 // 
#define EEK_LCOM      14 // Left command (Mac)
#define EEK_RCOM      15 // Right command (Mac)
#define EEK_MAC       16 // Special mac key (will come later :)
#define EEK_LMETA     17 // Left meta key
#define EEK_RMETA     18 // Right meta key

// Non-numerical edit and cursor movement keys
#define EEK_INS       210 // Insert
#define EEK_DEL       211 // Delete
#define EEK_HOME      199 // Home
#define EEK_END       207 // End
#define EEK_PGUP      201 // PageUp
#define EEK_PGDN      209 // PageDown

#define EEK_ESC       1	// 

#define EEK_UP        200 // Up key
#define EEK_DOWN      208 // Down key
#define EEK_LEFT      203 // Left key
#define EEK_RIGHT     205 // Right key

#define EEK_SPACE     32 // Space bar (ASCII compliant)

//////////////////////////////////////////////////////////////////////////
//The following keys might not be available on all keyboard layouts:
// (They work on US and German layouts)
//////////////////////////////////////////////////////////////////////////



#define EEK_GRAVE  36 // Grave ` on american keyboard (same key as tilde)

#define EEK_EQUAL  43 // Equal "=" on american keyboard
#define EEK_COMMA  44 // Comma "," on american keyboard
#define EEK_MINUS  45 // Minus on american keyboard
#define EEK_PERIOD 46 // Period "." on american keyboard
#define EEK_SLASH  47 // Slash "/" on american keyboard
/////////////////////////////////////////////
// Standard number codes comply with ASCII codes
/////////////////////////////////////////////
#define EEK_0         48
#define EEK_1         49
#define EEK_2         50
#define EEK_3         51
#define EEK_4         52
#define EEK_5         53
#define EEK_6         54
#define EEK_7         55
#define EEK_8         56
#define EEK_9         57

/////////////////////////////////////////////
// Special keys
/////////////////////////////////////////////
#define EEK_CAPS      58 // Caps Lock
#define EEK_SCROLL    59 // Scroll Lock
#define EEK_NUM       60 // Num Lock
#define EEK_PRSCR     61 // Print screen
#define EEK_PAUSE     62 // Pause key

/////////////////////////////////////////////
// Standard letter codes comply with ASCII codes
/////////////////////////////////////////////
#define EEK_A         65
#define EEK_B         66
#define EEK_C         67
#define EEK_D         68
#define EEK_E         69
#define EEK_F         70
#define EEK_G         71
#define EEK_H         72
#define EEK_I         73
#define EEK_J         74
#define EEK_K         75
#define EEK_L         76
#define EEK_M         77
#define EEK_N         78
#define EEK_O         79
#define EEK_P         80
#define EEK_Q         81
#define EEK_R         82
#define EEK_S         83
#define EEK_T         84
#define EEK_U         85
#define EEK_V         86
#define EEK_W         87
#define EEK_X         88
#define EEK_Y         89
#define EEK_Z         90

/////////////////////////////////////////////////////////////////////
// Numeric keypad keys. Some of these keys are doubled for your leisure :)
/////////////////////////////////////////////////////////////////////
#define EEK_KP_SLASH  91 // Slash (or divide) key on the numeric keypad
#define EEK_KP_MUL    92 // Multiply (or asterisk) key on the numeric keypad
#define EEK_KP_MINUS  93 // Minus on the numeric keypad
#define EEK_KP_PLUS   94 // Plus...
#define EEK_KP_ENTER  95 
#define EEK_KP_PERIOD 96 // Period (or comma or del) on the numeric keypad
#define EEK_KP_DEL    96
#define EEK_KP_0      97 // Insert or 0
#define EEK_KP_INS    97
#define EEK_KP_1      98 // End or 1
#define EEK_KP_END    98
#define EEK_KP_2      99 // Down or 2
#define EEK_KP_DOWN   99
#define EEK_KP_3     100 // PgDn or 3
#define EEK_KP_PGDN  100
#define EEK_KP_4     101 // Left or 4
#define EEK_KP_LEFT  101
#define EEK_KP_5     102 // Keypad 5
#define EEK_KP_6     103 // Right or 6
#define EEK_KP_RIGHT 103
#define EEK_KP_7     104 // Home or 7
#define EEK_KP_HOME  104
#define EEK_KP_8     105 // Up or 8
#define EEK_KP_UP    105
#define EEK_KP_9     106 // PgUp or 9
#define EEK_KP_PGUP  106

//////////////////////
// Function keys
//////////////////////
#define EEK_F1       110
#define EEK_F2       111
#define EEK_F3       112
#define EEK_F4       113
#define EEK_F5       114
#define EEK_F6       115
#define EEK_F7       116
#define EEK_F8       117
#define EEK_F9       118
#define EEK_F10      119
#define EEK_F11      120
#define EEK_F12      121


////////////////////////////////
// Keys only available on specific layouts
////////////////////////////////

//US (and English)

#define EEK_LSQBRK   131 // Left square bracket "[" on american keyboard
#define EEK_RSQBRK   132 // Right square bracket "]" on american keyboard
#define EEK_SEMICL   133 // Semicolon ";" on american keyboard
#define EEK_APOSTR   134 // Apostrophe key "'" on american keyboard
#define EEK_BACKSL   135 // Backslash "\" on american keyboard



//German keyboard
#define EEK_DE_SS     140 // Double-S (or Beta) "ß" on german keyboard
#define EEK_DE_ACCENT 141 // Accent key "´" on german keyboard
#define EEK_DE_UE     142 // U with umlaut "Ü" on german keyboard
#define EEK_DE_PLUS   143 // Plus on german keyboard
#define EEK_DE_OE     144 // O with umlaut "Ö" on german keyboard
#define EEK_DE_AE     145 // A with umlaut "Ä" on german keyboard
#define EEK_DE_HASH   146 // Hash "#" on german keyboard
#define EEK_DE_LT     147 // Less than "<" key on german keyboard
#define EEK_DE_CIRC   148 // Circumflex "^"on german keyboard

#define EEK_TILDE  EEK_GRAVE     // Tilde ~ is just here because some people call the grave key the tilde key

//US
#define EEK_US_EQUAL  EEK_EQUAL  // Equal "=" on american keyboard
#define EEK_US_LSQBRK EEK_LSQBRK // Left square bracket "[" on american keyboard
#define EEK_US_RSQBRK EEK_RSQBRK // Right square bracket "]" on american keyboard
#define EEK_US_SEMICL EEK_SEMICL // Semicolon ";" on american keyboard
#define EEK_US_APOSTR EEK_APOSTR // Apostrophe key "'" on american keyboard
#define EEK_US_BACKSL EEK_BACKSL // Backslash "\" on american keyboard
#define EEK_US_SLASH  EEK_SLASH  // Slash "/" on american keyboard

#define EEK_US_COMMA  EEK_COMMA  // Comma "," on american keyboard
#define EEK_US_PERIOD EEK_PERIOD // Period "." on american keyboard
#define EEK_US_MINUS  EEK_MINUS  // Minus on american keyboard

#define EEK_US_GRAVE  EEK_TILDE  // Grave ` on american keyboard
#define EEK_US_TILDE  EEK_TILDE  // Tilde ~ is just because some people call the grave key the tilde key

