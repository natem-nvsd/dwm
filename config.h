/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx	= 2;	/* border pixel of windows */
static const unsigned int snap		= 32;	/* snap pixel */
static const int showbar		= 1;	/* 0 means no bar */
static const int topbar			= 1;	/* 0 means bottom bar */
static const char *fonts[]		= { "TerminusTTF:size=12" };
static const char dmenufont[]		= "TerminusTTF:size=12";
static const char col_gray1[]		= "#121212";
static const char col_gray2[]		= "#444444";
static const char col_gray3[]		= "#bbbbbb";
static const char col_gray4[]		= "#ffffff";
static const char col_cyan[]		= "#e95420";
static const char *colors[][3]		= {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };
static const char *tagsalt[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	*	WM_CLASS(STRING) = instance, class
	*	WM_NAME(STRING) = title
	*
	* class		instance,	title,	tags mask,	isfloating,	isterminal,	monitor */
	{ "Firefox",	NULL,		NULL,	2,		0,		0 },
	{ "firefox",	NULL,		NULL,	2,		0,		0 },
	{ "KeepassXC",	NULL,		NULL,	3,		0,		0 },
	{ "keepassxc",	NULL,		NULL,	3,		0,		0 },
}; 

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

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
static const char *dmenucmd[]	= { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
//static const char *termcmd[]	= { "xterm", NULL };
static const char *termcmd[]	= { "st", NULL };
//static const char *tabbedcmd[]	= { "/bin/sh", "-c", "tabbed -c xterm -into", NULL };
static const char *tabbedcmd[]	= { "/bin/sh", "-c", "tabbed -c st -w", NULL };
static const char *volupcmd[]	= { "/bin/sh", "-c", "mixer -s vol +1", NULL };
static const char *voldwncmd[]	= { "/bin/sh", "-c", "mixer -s vol -1", NULL };
static const char *volmtecmd[]	= { "/bin/sh", "-c", "mixer -s vol ^", NULL };
//static const char *volupcmd[]	= { "/bin/sh", "-c", "amixer -D pulse sset Master 2%+", NULL };
//static const char *voldwncmd[]	= { "/bin/sh", "-c", "amixer -D pulse sset Master 2%-", NULL };
//static const char *volmtecmd[]	= { "/bin/sh", "-c", "amixer -D pulse sset Master toggle", NULL };
//static const char *micmtecmd[]	= { "amixer -D pulse sset Master toggle", NULL };
static const char *brtupcmd[]	= { "xbacklight -inc 2", NULL };
static const char *brtdwncmd[]	= { "xbacklight -dec 2", NULL };
static const char *prntcmd[]	= { "/bin/sh", "-c", "scrot -z --quality 100 '%Y-%m-%d_%H:%M:%S.png' -e 'mv $f ~/Pictures/Screenshots/'", NULL };
static const char *prntfocmd[]	= { "/bin/sh", "-c", "scrot -z -u -b --quality 100 '%Y-%m-%d_%H:%M:%S.png' -e 'mv $f ~/Pictures/Screenshots/'", NULL };
static const char *fmcmd[]	= { "pcmanfm", NULL };
static const char *webcmd[]	= { "firefox", NULL};
static const char *gimpcmd[]	= { "gimp", NULL };
static const char *vboxcmd[]	= { "/bin/sh", "-c","virtualbox", NULL };
static const char *leftcmd[]	= { "left_vga", NULL };
static const char *rightcmd[]	= { "right_vga", NULL };
static const char *upcmd[]	= { "up_vga", NULL };
static const char *downcmd[]	= { "down_vga", NULL };
static const char *xautocmd[]	= { "xrandr --auto", NULL };
static const char *inkcmd[]	= { "inkscape", NULL };
static const char *blendcmd[]	= { "blender", NULL };
static const char *xmppcmd[]	= { "dino", NULL };
static const char *cmusprev[]	= { "/bin/sh", "-c", "cmus-remote -r &", NULL };
static const char *cmusplay[]	= { "/bin/sh", "-c", "cmus-remote -u & ", NULL };
static const char *cmusnext[]	= { "/bin/sh", "-c", "cmus-remote -n &", NULL }; 
static const char *boorucmd[]	= { "atsugami", NULL };
static const char *gladecmd[]	= { "glade", NULL };
static const char *pdfcmd[]	= { "zathura", NULL };
static const char *keecmd[]	= { "keepassxc", NULL };

static Key keys[] = {
	/* modifier                     key				function        argument
	 *=======================================================================================*/
	{ MODKEY,                       XK_d,				spawn,		{.v = dmenucmd	} },
	{ MODKEY, 	                XK_Return,			spawn,		{.v = termcmd	} },
	{ MODKEY|ShiftMask,		XK_Return,			spawn,		{.v = tabbedcmd	} },
	{ MODKEY,                       XK_b,				togglebar,	{0} },
	{ MODKEY,                       XK_j,				focusstack,	{.i = +1 } },
	{ MODKEY,                       XK_k,				focusstack,	{.i = -1 } },
	{ MODKEY,                       XK_i,				incnmaster,	{.i = +1 } },
	{ MODKEY,                       XK_o,				incnmaster,	{.i = -1 } },
	{ MODKEY,                       XK_h,				setmfact,	{.f = -0.05} },
	{ MODKEY,                       XK_l,				setmfact,	{.f = +0.05} },
	{ MODKEY,                       XK_t,				setlayout,	{.v = &layouts[0]} },
	{ MODKEY|ControlMask|ShiftMask,	XK_f,				setlayout,	{.v = &layouts[1]} },
	{ MODKEY,                       XK_m,				setlayout,	{.v = &layouts[2]} },
	{ MODKEY,                       XK_space,			setlayout,	{0		} },
	{ MODKEY|ShiftMask,             XK_space,			togglefloating,	{0		} },
	{ MODKEY,                       XK_comma,			focusmon,	{.i = -1 } },
	{ MODKEY,			XK_period,			focusmon,	{.i = +1 } }, /* Last tabbed line */
	{ MODKEY|ShiftMask,		XK_comma,			tagmon,		{.i = -1 } },
	{ MODKEY|ShiftMask,		XK_period,			tagmon,		{.i = +1 } },
	TAGKEYS(			XK_1,				0)
	TAGKEYS(			XK_2,				1)
	TAGKEYS(			XK_3,				2)
	TAGKEYS(			XK_4,				3)
	TAGKEYS(			XK_5,				4)
	TAGKEYS(			XK_6,				5)
	TAGKEYS(			XK_7,				6)
	TAGKEYS(			XK_8,				7)
	TAGKEYS(			XK_9,				8)
	{ MODKEY|ControlMask|ShiftMask,	XK_Escape,			quit,		{0} },
	{ MODKEY|ShiftMask,		XK_q,				killclient,	{0} },
	{ 0,				XF86XK_AudioLowerVolume,	spawn,		{.v = voldwncmd	} },
	{ 0,				XF86XK_AudioRaiseVolume,	spawn,		{.v = volupcmd	} },
	{ 0,				XF86XK_AudioMute,		spawn,		{.v = volmtecmd	} },
	{ 0,				XF86XK_AudioMicMute,		spawn,		{.v = micmtecmd	} },
	{ 0,				XF86XK_MonBrightnessUp,		spawn,		{.v = brtupcmd	} },
	{ 0,				XF86XK_MonBrightnessDown,	spawn, 		{.v = brtdwncmd	} },
	{ 0,				XF86XK_KbdBrightnessUp,		spawn,		{.v = brtupcmd	} },
	{ 0,				XF86XK_KbdBrightnessDown,	spawn, 		{.v = brtdwncmd	} },
	{ 0,				XK_Print, 			spawn,		{.v = prntcmd	} },
	{ 0,				XK_Sys_Req,			spawn,		{.v = prntfocmd	} },
	{ MODKEY,			XK_Print,			spawn,		{.v = prntfocmd	} },
	{ MODKEY,			XK_p,				spawn,		{.v = fmcmd	} },
	{ MODKEY,			XK_w,				spawn,		{.v = webcmd	} },
	{ MODKEY,			XK_v,				spawn,		{.v = vboxcmd	} },
	{ MODKEY,			XK_g,				spawn,		{.v = gimpcmd	} },
	{ MODKEY|ShiftMask,		XK_Left,			spawn,		{.v = leftcmd	} },
	{ MODKEY|ShiftMask,		XK_Right,			spawn,		{.v = rightcmd	} },
	{ MODKEY|ShiftMask,		XK_Up,				spawn,		{.v = upcmd	} },
	{ MODKEY|ShiftMask,		XK_Down,			spawn,		{.v = downcmd	} },
	{ Mod4Mask,			XK_p,				spawn,		{.v = xautocmd	} },
	{ MODKEY|ShiftMask,		XK_I,				spawn,		{.v = inkcmd	} },
	{ MODKEY|ShiftMask,		XK_b,				spawn,		{.v = blendcmd	} },
	{ MODKEY,			XK_x,				spawn,		{.v = xmppcmd	} },
	{ MODKEY,			XK_grave,			zoom,		{0		} },
	{ MODKEY,			XK_minus,			incnmaster,	{.i = -1	} },
	{ MODKEY,			XK_equal,			incnmaster,	{.i = +1	} },
	{ 0,				XF86XK_AudioPrev,		spawn,		{.v = cmusprev	} },
	{ 0,				XF86XK_AudioPlay,		spawn,		{.v = cmusplay	} },
	{ 0,				XF86XK_AudioNext,		spawn,		{.v = cmusnext	} }, 
	{ 0,				XF86XK_Search,			spawn,		{.v = cmusprev	} },
	{ 0,				XF86XK_LaunchA,			spawn,		{.v = cmusplay	} },
	{ 0,				XF86XK_Explorer,		spawn,		{.v = cmusnext	} }, 
	{ MODKEY, 			XK_a,				spawn,		{.v = boorucmd	} },
	{ MODKEY|ShiftMask,		XK_g,				spawn,		{.v = gladecmd	} },
	{ MODKEY,			XK_z,				spawn,		{.v = pdfcmd	} },
	{ MODKEY,			XK_u,				spawn,		{.v = keecmd	} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};
