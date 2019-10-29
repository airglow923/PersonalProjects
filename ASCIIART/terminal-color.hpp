/*
 * Usage:
 * 
 * X - ANY PREDEFINED MACROS FROM:
 *    - FOREGROUND
 *    - BACKGROUND
 *    - FONT STYLE
 * 
 * RST - RST DEFINED BELOW
 * 
 * Ex.
 *    std::cout << X + std::string(TEXT) + RST;
 * 
 * RGB values for supported colors:
 * 
 *    - black   (  0,   0,   0)
 *    - red     (255,   0,   0)
 *    - green   (  0, 255,   0)
 *    - yellow  (255, 255,   0)
 *    - blue    (  0,   0, 255)
 *    - magenta (255,   0, 255)
 *    - cyan    (  0. 255, 255)
 *    - white   (255, 255, 255)
 */

#ifndef _TERMINAL_COLORS_
#define _TERMINAL_COLORS_

// RESET
#define RST      "\x1B[0m"

// FOREGROUND
#define FBLK     "\x1B[30m"   // Foreground black
#define FRED     "\x1B[31m"   // Foreground red
#define FGRN     "\x1B[32m"   // Foreground green
#define FYEL     "\x1B[33m"   // Foreground yellow
#define FBLU     "\x1B[34m"   // Foreground blue
#define FMAG     "\x1B[35m"   // Foreground magenta
#define FCYN     "\x1B[36m"   // Foreground cyan
#define FWHT     "\x1B[37m"   // Foreground white

// BACKGROUND
#define BBLK     "\x1B[40m"   // Background black
#define BRED     "\x1B[41m"   // Background red
#define BGRN     "\x1B[42m"   // Background green
#define BYEL     "\x1B[43m"   // Background yellow
#define BBLU     "\x1B[44m"   // Background blue
#define BMAG     "\x1B[45m"   // Background magenta
#define BCYN     "\x1B[46m"   // Background cyan
#define BWHT     "\x1B[47m"   // Background white

// FONT STYLE
#define BOLD     "\x1B[1m"
#define UNDL     "\x1B[4m"
#define INVS     "\x1B[7m"
#define BOLDOFF  "\x1B[21m"
#define UNDLOFF  "\x1B[24m"
#define INVSOFF  "\x1B[27m"

#endif