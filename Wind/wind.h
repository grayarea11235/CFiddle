#ifndef WIND_H
#define WIND_H

#include "linked_list.h"

#define CT_FORM 0
#define CT_BUTTON 1
#define CT_CHECKBOX 2


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
  unsigned short lbtn : 1;
  unsigned short rbtn : 1;
  unsigned short mbtn : 1;
  unsigned short ctrl : 1;
  unsigned short shft : 1;
  unsigned short xbtn1 : 1;
  unsigned short xbtn2 : 1;
} mouse_move_event_t;



#define CONTROL_COMMON int type; \
  int id; \
  char *name; \  
  short x; \
  short y; \
  short width; \
  short height; \


typedef struct _form_t
{
  int type;
  int id;
  char *name;
  short x;
  short y;
  short width;
  short height;
  

  // Win32 specific data
  WNDCLASS wndclass;
  HWND hwnd;

  //control_t *controls;

  // Data
  char *title;



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

typedef struct _button_t
{
  int type;
  int id;
  char *name;
  short x;
  short y;
  short width;
  short height;
  
  // Win32 specific data
  WNDCLASS wndclass;
  HWND hwnd;
  
} button_t;

typedef struct _checkbox_t
{
  int type;
  int id;
  char *name;
  short x;
  short y;
  short width;
  short height;


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
  char *name;
  short x;
  short y;
  short width;
  short height;
  HWND parent;
  application_t *app;
} create_data_t;

// Global App 
application_t *app;

#endif
