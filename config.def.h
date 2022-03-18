/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 5;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int focusonwheel       = 0;
static const int horizpadbar        = 0;        /* horizontal padding for statusbar */
static const int vertpadbar         = 7;        /* vertical padding for statusbar */
static const Gap default_gap        = {.isgap = 1, .realgap = 10, .gappx = 10};
static const char *fonts[]          = {"JetBrainsMono Nerd Font:size=13"};
static const char dmenufont[]       = "JetBrainsMono Nerd Font:size=12";
static const char col_gray1[]       = "#282a36";
static const char col_gray2[]       = "#282a36";
static const char col_gray3[]       = "#96b5b4";
static const char col_gray4[]       = "#d7d7d7";
static const char col_cyan[]        = "#282a36";
static const char col_black[]       = "#000000";
static const char col_white[]       = "#ffffff";
static const char col_red[]         = "#ff5370";
static const char col_darkred[]     = "#db1616";
static const char col_yellow[]      = "#ffee75";
static const char col_green[]       = "#c3e88d";
static const char col_orange[]      = "#f07178";
static const char col_purple[]      = "#c792ea";
static const char col_pink[]        = "#FD9CAC";
static const char col_blue[]        = "#89ddff";

static const char *colors[][3]      = {
  /*          fg         bg          border   */
    [SchemeNorm]   = { col_gray3,  col_gray1,  col_gray2 },
    [SchemeSel]    = { col_red,  col_cyan,   col_cyan  },
    [SchemeRed]    = { col_red,    col_gray1,  col_gray2 },
    [SchemeYellow] = { col_yellow, col_gray1,  col_gray2 },
    [SchemeOrange] = { col_orange, col_gray1,  col_gray2 },
    [SchemeGreen]  = { col_green,  col_gray1,  col_gray2 },
    [SchemePurple] = { col_purple, col_gray1,  col_gray2 },
    [SchemePink]   = { col_pink,   col_gray1,  col_gray2 },
    [Schemeblue]   = { col_blue,   col_gray1,  col_gray2 },
    [SchemeDarkRed]= { col_darkred,col_gray1,  col_gray2 },
};

/* staticstatus */
static const int statmonval = 0;

/* tagging */
static const char *tags[] = { " ", " ", " ", " ", " ", " ", "ﭮ ", " "};

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
  { "Binance",  NULL,       NULL,       1 << 5,       0,            1},
  { "discord",  NULL,       NULL,       1 << 6,       0,            0},
  { "Spotify",  NULL,       NULL,       1 << 7,       0,            1},
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "st", NULL };
static const char *web[] = {"brave", NULL};

static const char *mute[] = {"pactl", "set-sink-mute", "@DEFAULT_SINK@", "toggle", NULL };
static const char *volup[] = { "pactl", "set-sink-volume", "@DEFAULT_SINK@", "+5%", NULL };
static const char *voldown[] = { "pactl", "set-sink-volume", "@DEFAULT_SINK@", "-5%", NULL };

static const char *play_pause_spotify[] = { "playerctl", "play-pause", "-p", "spotify", NULL};
static const char *next_spotify[] = { "playerctl", "next","-p", "spotify", NULL};
static const char *previous_spotify[] = { "playerctl", "previous","-p", "spotify", NULL};

static const char *play_pause[] = { "playerctl", "play-pause", "-i", "spotify", NULL};
static const char *next[] = { "playerctl", "next","-i", "spotify", NULL};
static const char *previous[] = { "playerctl", "previous","-i", "spotify", NULL};

static const char *kitty[] = {"kitty", NULL};
static const char *audio[] = {"/home/anurag/.program/audio.sh", NULL};
static const char *colpick[] = {"/home/anurag/.program/colpick.sh", NULL};
static const char *code[] = {"code", NULL};
static const char *mon_off[] = {"xset", "dpms", "force", "off",  NULL};

const char screensot_script[] = "var=~/pix/$(date '+%d-%m-%y-%H_%M_%S').png; maim -s -u -m 10 $var; cat $var | xclip -selection clipboard -t image/png; notify-send 'Screenshot captured' -t 3000 -i \"$var\"";

static Key keys[] = {
  /* modifier                     key        function        argument */
  { MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
  { MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
  { MODKEY,                       XK_b,      togglebar,      {0} },
  { MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
  { MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
  { MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
  { MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
  { MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
  { MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
  { MODKEY,                       XK_Return, zoom,           {0} },
  { MODKEY,                       XK_Tab,    view,           {0} },
  { MODKEY,                       XK_x,      killclient,     {0} },
  { MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
  { MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
  { MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
  { MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
  { MODKEY,                       XK_0,      view,           {.ui = ~0 } },
  { MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
  { MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
  { MODKEY,                       XK_period, focusmon,       {.i = +1 } },
  { MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
  { MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
  { MODKEY,                       XK_minus,  setgaps,        {.i = -5 } },
  { MODKEY,                       XK_equal,  setgaps,        {.i = +5 } },
  { MODKEY|ShiftMask,             XK_minus,  setgaps,        {.i = GAP_RESET } },
  { MODKEY|ShiftMask,             XK_equal,  setgaps,        {.i = GAP_TOGGLE} },
  TAGKEYS(                        XK_1,                      0)
  TAGKEYS(                        XK_2,                      1)
  TAGKEYS(                        XK_3,                      2)
  TAGKEYS(                        XK_4,                      3)
  TAGKEYS(                        XK_5,                      4)
  TAGKEYS(                        XK_6,                      5)
  TAGKEYS(                        XK_7,                      6)
  TAGKEYS(                        XK_8,                      7)
  TAGKEYS(                        XK_9,                      8)
  { MODKEY|ShiftMask,             XK_q,      quit,           {0} },
  { 0,        XF86XK_AudioMute,              spawn,          {.v = mute } },
  { 0,        XF86XK_AudioLowerVolume,       spawn,          {.v = voldown } },
  { 0,        XF86XK_AudioRaiseVolume,       spawn,          {.v = volup } },
  { 0,        XF86XK_AudioPlay,              spawn,          {.v = play_pause_spotify } },
  { 0,        XF86XK_AudioPrev,              spawn,          {.v = previous_spotify } },
  { 0,        XF86XK_AudioNext,              spawn,          {.v = next_spotify } },
  { ShiftMask,    XF86XK_AudioPlay,          spawn,          {.v = play_pause } },
  { ShiftMask,    XF86XK_AudioPrev,          spawn,          {.v = previous } },
  { ShiftMask,    XF86XK_AudioNext,          spawn,          {.v = next } },
  { MODKEY,       XK_space,                  spawn,          {.v = kitty} },
  { 0,            XK_Print,                  spawn,          SHCMD(screensot_script)},
  { 0,            XK_F8,                     spawn,          {.v = audio} },
  { ControlMask|MODKEY,           XK_c,      spawn,          {.v = colpick} },
  { MODKEY,                       XK_w,      spawn,          {.v = web }},
  { MODKEY,                       XK_c,      spawn,          {.v = code}},
  { ControlMask,                  XK_F1,     spawn,          {.v = mon_off} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
  /* click                event mask      button          function        argument */
  { ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
  { ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
  { ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
  { ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
  { ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
  { ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
  { ClkTagBar,            0,              Button1,        view,           {0} },
  { ClkTagBar,            0,              Button3,        toggleview,     {0} },
  { ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
  { ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

