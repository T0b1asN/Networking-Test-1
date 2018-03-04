#pragma once

//Max length of name
//Will fit in all name boxes (15)
#define MAX_NAME_LENGTH 15

#define VERSION "v2.2.0"

//#define FONT_NORM "GOTHIC.TTF"
//#define FONT_BOLD "GOTHICB.TTF"
//#define FONT_IT "GOTHICI.TTF"
//#define FONT_BOLD_IT "GOTHICBI.TTF"

#define FONT_NORM "times.ttf"
#define FONT_BOLD "timesbd.ttf"
#define FONT_IT "HTOWERT.ttf"
#define FONT_BOLD_IT "HTOWERTI.ttf"
#define FONT_UNICODE "unifont-9.0.06.ttf"

//INFO: using whirlpool hash
//can be replaced with any string can only be used by server in code due to things that are added in user accesible functions
#define SHUTDOWN_MSG "53f433188a63f84e5486e5ab0118731b939fd3ac5990f9e1b46c162b50dc620832a11636331d22530e5689b4ae6f94972812834adf3151c9fddb65d85a03beac"

//send this at the start of the message, if the client should not play a sound on receiving
#define NO_SOUND_CHAR "┐"
