#ifndef _GLOBALS_H
#define _GLOBALS_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <gtk/gtk.h>
#include <glib/gi18n.h>
#include <glib.h>
#include <glib-unix.h>
#include <modbus-tcp.h>
#include <locale.h>
#include <signal.h>
#include <unistd.h>

//////////////////////////////////////////////
// All extern
extern GMutex terminal_mutex;

#endif //_GLOBALS_H
