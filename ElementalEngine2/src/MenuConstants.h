#ifndef MENU_CONSTANTS_H_INCLUDED
#define MENU_CONSTANTS_H_INCLUDED


////////////////////////////////////////////////////////////////////////////////////
// This header defines some menu system constants which are not covered by any
// VMenuSystem_cl::SetStandardXXX function
////////////////////////////////////////////////////////////////////////////////////

#define USE_SCREENMASK_FILTERING    TRUE
#define USE_SCREENMASK_WRAPPING     FALSE,FALSE

///////////////////////////////////////////////////////////////////////////
// Xbox
///////////////////////////////////////////////////////////////////////////
#ifdef _XBOX
#define XBOX_BUTTON_THRESHOLD   128
#endif


///////////////////////////////////////////////////////////////////////////
// modal message box related :
///////////////////////////////////////////////////////////////////////////

// modulation colors for fading out the background when messagebox is opened
#define MB_BACKGROUND_FADE_START       V_RGBA_WHITE
#define MB_BACKGROUND_FADE_END         V_RGBA(64, 64, 64, 255)
#define MB_BACKGROUND_FADE_TIME        0.5f

// the height of the title bar
#define WINDOW_TITLE_BAR_HEIGHT           24
// y-offset of message box title from top border
#define WINDOW_TITLE_Y_OFS                4
// standard window border width to add left, right and at bottom
#define WINDOW_BORDER_WIDTH               12

// distance from MB title to text
#define MB_TEXT_TITLE_Y_OFS           24
// distance between text and buttons
#define MB_BUTTONS_TEXT_Y_OFS         24



///////////////////////////////////////////////////////////////////////////
// text edit control related :
///////////////////////////////////////////////////////////////////////////

// width of edit cursor (height=font height)
#define EDITCTRL_CURSOR_WIDTH       3
// color of edit cursor
#define EDITCTRL_CURSOR_COLOR       V_RGBA(200,120,0,255)

// key repeat and deleay times (in seconds)
#define EDITCTRL_KEYREPEAT_DELAYTIME     0.25f
#define EDITCTRL_KEYREPEAT_FREQUENCY     0.05f


#endif

