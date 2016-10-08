/* Config for my 2x 1920x1080 VDU
/* See LICENSE file for copyright and license details. */
#include "gaplessgrid.c"
#include "selfrestart.c"
#include "zoomswap.c"



/* appearance */
/*static const char *fonts[] = {
        "pango:EnvyCodeR:size=9",
        "pango:EnvyCodeR:size=9"
};
*/
static const char *fonts[] = {
        "pango:EnvyCodeR:size=9",
        "Siji"
};

static const unsigned int borderpx 			= 1.5;	/* border pixel of windows */
static const unsigned int snap 				= 3;	/* snap pixel */
static const unsigned int tagpadding 		= 3;	/* inner padding of tags */
static const unsigned int tagspacing 		= 5;	/* inner padding of tags */
static const unsigned int gappx			= 15;	/* gap pixel between windows */
static const unsigned int systraypinning 	= 0;	/* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayspacing 	= 0;	/* systray spacing */
static const int systraypinningfailfirst 	= 0;	/* 1: if pinning fails, display systray on the first monitor, 0: display systray on the last monitor*/
static const int showsystray 				= 1;	/* 0 means no systray */
static const int showbar 					= 1;	/* 0 means no bar */
static const int topbar 					= 1;	/* 0 means bottom bar */
static const char dmenufont[]       = "EnvyCodeR:size=10";


#define NUMCOLORS 9
static const char colors[NUMCOLORS][MAXCOLORS][9] = {
	// border	 foreground	background
	{ "#cb4b16", "#cccccc", "#191919" }, // 0 = normal
	{ "#cb4b16", "#cb4b16", "#3e3e3e" }, // 1 = selected
	{ "#b43030", "#b43030", "#0b0606" }, // 2 = red / urgent
	{ "#191919", "#2aa198", "#222222" }, // 3 = green / occupied
	{ "#191919", "#ab7438", "#0b0606" }, // 4 = yellow
	{ "#191919", "#475971", "#0b0606" }, // 5 = blue
	{ "#191919", "#694255", "#0b0606" }, // 6 = cyan
	{ "#191919", "#3e6868", "#0b0606" }, // 7 = magenta
	{ "#191919", "#cfa696", "#0b0606" }, // 8 = grey
};

/* tagging */
static const char *tags[] = {
/*		"web",
		"term",
		"code",
		"media",
		"game",
		"play",
		"misc"
*/
		"\ue1d7",
		"\ue1ef",
		"\ue269",
		"\ue1dd",
		"\ue1c6",
		"\ue1df",
		"\ue1e0"
};

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class		instance	title		tags mask	isfloating		monitor */
	 { "Gimp",     NULL,       NULL,       0,            True,        -0 },
	 { "Firefox",  NULL,       NULL,       1 << 8,       False,        0 },
	 { "Tickr",    NULL,       NULL,       1 << 8,       True,         0 },
	 { "chromium", "crx_kghagoclbibnecmngoilbmkiedggcick", "Mobdro", 0,  True,  0 },
         { "chromium", "crx_kfopknjoohhpjblfhjggahliccjkjcgm", "TV",     0,  True,  0 },
	 { "MPlayer",  NULL,       NULL,       0,            True,        -1 },
	 { "mpv",      NULL,       NULL,       0,            True,        -1 },
	 { "Termite",  NULL,       NULL,       1,            False,        2 },
	 { "Terminator",  NULL,    NULL,       2 << 0,       False,        2 },
	 { "XClock",      NULL,    NULL,       2 << 0,       True,         2 },
	 { "XTerm",      NULL,      NULL,    0,            True,        -0 },
	 { "feh",      NULL,      NULL,       0,            True,         2 },  
	 { "Dwb",      NULL,      NULL,       0,            True,         2 },  
	 { "Vimb",      NULL,      NULL,       0,            True,         2 },  
	 { "tabbed",    NULL,      NULL,     0,            True,        -0 },
	 { "sublime_text",   NULL, "Sublime Text 2",    4,   False,        0 },
	 { "Orage",    NULL,       NULL,       0,            True,        -0 },
	 { "Xsensors",  NULL,       NULL,       1,            True,        -0 },
	 { "Roxterm",     NULL,    NULL,       0,            True,        -0 },
	 { "Tilda",   NULL,       "tilda",   2 << 0,         False,        0 },
};

/* layout(s) */
static const float mfact 		= 0.55;	/* factor of master area size [0.05..0.95] */
static const int nmaster 		= 1;	/* number of clients in master area */
static const int resizehints 	= 1;	/* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "\ue002",      tile },    /* first entry is default */
	{ "\ue006",      NULL },    /* no layout function means floating behavior */
	{ "\ue000",      monocle },
	{ "\ue003",      htile },
	{ "\ue00a",      gaplessgrid },
};

/* key definitions */
#define MODKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/zsh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] 			= "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = {"dmenu_run -nb '#100' -nf '#2aa198' -sb '#000' -sf '#cb4b16' -p dmenu:", NULL};
static const char *termcmd[] 		= { "termite", NULL };
static const char *screenshot[] 	= { "screenshot", NULL};


static Key keys[] = {
	/* modifier						key							function			argument */
	{ MODKEY,						XK_d,						spawn,				{.v = dmenucmd } },
	{ MODKEY,						XK_Return,					spawn,				{.v = termcmd } },
	{ MODKEY|ShiftMask,				XK_b,						togglebar,			{0} },
	{ MODKEY,						XK_Right,					focusstack,			{.i = +1 } },
	{ MODKEY,						XK_Left,					focusstack,			{.i = -1 } },
	{ MODKEY,						XK_i,						incnmaster,			{.i = +1 } },
	{ MODKEY,						XK_p,						incnmaster,			{.i = -1 } },
	{ MODKEY|ShiftMask,				XK_Left,					setmfact,			{.f = -0.05} },
	{ MODKEY|ShiftMask,				XK_Right,					setmfact,			{.f = +0.05} },
	{ MODKEY|ShiftMask,				XK_Up,						setcfact,			{.f = +0.25} },
	{ MODKEY|ShiftMask,				XK_Down,					setcfact,			{.f = -0.25} },
	{ MODKEY|ShiftMask,				XK_o,						setcfact,			{.f =  0.00} },
	{ MODKEY,						XK_Down,					pushdown,			{0} },
	{ MODKEY,						XK_Up,						pushup,				{0} },
	{ MODKEY|ShiftMask,				XK_Return,					zoom,				{0} },
	{ MODKEY,						XK_Tab,						view,				{0} },
	{ MODKEY|ShiftMask,				XK_c,						killclient,			{0} },
	{ MODKEY,						XK_t,						setlayout,			{.v = &layouts[0]} },
	{ MODKEY,						XK_f,						setlayout,			{.v = &layouts[1]} },
	{ MODKEY,						XK_m,						setlayout,			{.v = &layouts[2]} },
	{ MODKEY,						XK_b,						setlayout,			{.v = &layouts[3]} },
	{ MODKEY,						XK_g,						setlayout,			{.v = &layouts[4]} },
	{ MODKEY,						XK_space,					setlayout,			{0} },
	{ MODKEY,						XK_u,						togglefullscreen,	{0} },
	{ MODKEY|ShiftMask,				XK_space,					togglefloating,		{0} },
	{ MODKEY,						XK_0,						view,				{.ui = ~0 } },
	{ MODKEY|ShiftMask,				XK_0,						tag,				{.ui = ~0 } },
	{ MODKEY,						XK_comma,					focusmon,			{.i = -1 } },
	{ MODKEY,						XK_period,					focusmon,			{.i = +1 } },
	{ MODKEY|ShiftMask,				XK_comma,					tagmon,				{.i = -1 } },
	{ MODKEY|ShiftMask,				XK_period,					tagmon,				{.i = +1 } },
	TAGKEYS(						XK_1,						0)
	TAGKEYS(						XK_2,						1)
	TAGKEYS(						XK_3,						2)
	TAGKEYS(						XK_4,						3)
	TAGKEYS(						XK_5,						4)
	TAGKEYS(						XK_6,						5)
	TAGKEYS(						XK_7,						6)
	TAGKEYS(						XK_8,						7)
	TAGKEYS(						XK_9,						8)
	{ MODKEY|ShiftMask,				XK_q,						quit,				{0} },
	{ MODKEY|ShiftMask,				XK_r,						self_restart,		{0} },
    { 0,							XK_Print,					spawn,				{.v = screenshot } },

};

/* button definitions */
/* click can be ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click					event mask		button			function			argument */
	{ ClkLtSymbol,					0,			Button1,		setlayout,			{0} },
	{ ClkLtSymbol,					0,			Button3,		setlayout,			{.v = &layouts[2]} },
	{ ClkStatusText,				0,			Button2,		spawn,				{.v = termcmd } },
	{ ClkClientWin,					MODKEY,		Button1,		movemouse,			{0} },
	{ ClkClientWin,					MODKEY,		Button2,		togglefloating,		{0} },
	{ ClkClientWin,					MODKEY,		Button3,		resizemouse,		{0} },
	{ ClkTagBar,					0,			Button1,		view,				{0} },
	{ ClkTagBar,					0,			Button3,		toggleview,			{0} },
	{ ClkTagBar,					MODKEY,		Button1,		tag,				{0} },
	{ ClkTagBar,					MODKEY,		Button3,		toggletag,			{0} },
};
