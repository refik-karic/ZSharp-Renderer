#pragma once

// Even though this header only includes the minimum requirements at any given time.
// It is still useful to ignore excess things in the event where another header, such as objidl.h from GDI+/COM, include windows.h themselves.

// Exclude rarely-used stuff from Windows headers.
#define WIN32_LEAN_AND_MEAN

// Ignore parts of Windows headers that are not being used.
#define NOGDICAPMASKS
//#define NOVIRTUALKEYCODES
//#define NOWINMESSAGES
//#define NOWINSTYLES
#define NOSYSMETRICS
#define NOMENUS
#define NOICONS
//#define NOKEYSTATES
#define NOSYSCOMMANDS
#define NORASTEROPS
#define NOSHOWWINDOW
#define OEMRESOURCE
#define NOATOM
#define NOCLIPBOARD
#define NOCOLOR
#define NOCTLMGR
#define NODRAWTEXT
//#define NOGDI
#define NOKERNEL
//#define NOUSER
#define NONLS
#define NOMB
#define NOMEMMGR
//#define NOMETAFILE
//#define NOMINMAX
//#define NOMSG
#define NOOPENFILE
#define NOSCROLL
#define NOSERVICE
#define NOSOUND
#define NOTEXTMETRIC
#define NOWH
#define NOWINOFFSETS
#define NOCOMM
#define NOKANJI
#define NOHELP
#define NOPROFILER
#define NODEFERWINDOWPOS
#define NOMCX

// Thanks to Aras for the tip on reducing Windows includes.
// https://aras-p.info/blog/2018/01/12/Minimizing-windows.h/
#define _AMD64_
#include <windef.h>
#include <winbase.h>
#include <winuser.h>
