#ifndef WIND_H
#define WIND_H

#include <stdint.h>

#include "linked_list.h"

// Colors

#define COL_AQUA       0xFFFF00 	
#define COL_BLACK      0x000000 	
#define COL_BLUE       0xFF0000 	
#define COL_CREAM      0xF0FBFF 	
#define COL_DKGRAY     0x808080 	
#define COL_FUCHSIA    0xFF00FF 	
#define COL_GRAY       0x808080 	
#define COL_GREEN      0x008000 	
#define COL_LIME       0x00FF00 	
#define COL_LTGRAY     0xC0C0C0 	
#define COL_MAROON     0x000080 	
#define COL_MEDGRAY    0xA4A0A0 	
#define COL_MONEYGREEN 0xC0DCC0 	
#define COL_NAVY       0x800000 	
#define COL_OLIVE      0x008080 	
#define COL_PURPLE     0x800080 	
#define COL_RED        0x0000FF 	
#define COL_SILVER     0xC0C0C0 	
#define COL_SKYBLUE    0xF0CAA6 	
#define COL_TEAL       0x808000 	
#define COL_WHITE      0xFFFFFF 	
#define COL_YELLOW     0x00FFFF 	

typedef struct _color_t
{
  uint32_t color;
} color_t;


#define CT_FORM 0
#define CT_BUTTON 1
#define CT_CHECKBOX 2
#define CT_DROPDOWN 3


typedef struct _event_t
{


} event_t;

typedef struct _point_t
{
  int x;
  int y;
} point_t;

typedef struct _mouse_button_status_t
{
  unsigned short lbtn : 1;
  unsigned short rbtn : 1;
  unsigned short mbtn : 1;
  unsigned short ctrl : 1;
  unsigned short shft : 1;
} mouse_button_status_t;

typedef struct _mouse_click_event_t
{
  point_t pnt;
  mouse_button_status_t btn;
} mouse_click_event_t;

typedef struct _mouse_move_event_t
{
  point_t pnt;
  mouse_button_status_t btn;
  unsigned short xbtn1 : 1;
  unsigned short xbtn2 : 1;
} mouse_move_event_t;

#define CONTROL_COMMON int type;\
  int id;\
  char *name;\
  short x;\
  short y;\
  short width;\
  short height;\
  HWND hwnd;\
  HWND parent;


typedef struct _form_t
{
  CONTROL_COMMON

  // Win32 specific data
  WNDCLASS wndclass;

  //control_t *controls;

  // Data
  char *title;
  
  list_t *control_list;
  
  // Events
  void (*mouse_move_ptr)(void *event);  
  
} form_t;

typedef struct _application_t
{
  char *name;
  
  HINSTANCE hInstance;
  HINSTANCE hPrevInstance;
  PSTR szCmdLine;
  int iCmdShow;

  // list of forms here
  list_t *form_list;
} application_t;

typedef struct _checkbox_t
{
  CONTROL_COMMON

} checkbox_t;


typedef struct _control_t
{
  int type;
  int id;
  char *name;
  short x;
  short y;
  short width;
  short height;
  void *other;
  
  //form_t *form;
  //button_t *button;
} control_t;


typedef struct _create_data_t
{
  CONTROL_COMMON

  application_t *app;
} create_data_t;

// Global App 
application_t *app;

#endif
