#ifndef _CJS_FONT_H
#define _CJS_FONT_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

//#include "minilogger.h"
//#include<iostream>
//#include<string>
//#include "sal.h" //SDL_Surface *mScreen = NULL;
//#include "port.h"
//#include "cheats.h"
#include "cjs.h"
//#include "cjs_sdl.h"

//extern SDL_Surface *mScreen;

typedef uint32_t u32;
typedef int32_t  s32;
typedef uint16_t u16;
typedef int16_t  s16;
typedef uint8_t  u8;
typedef char     s8;



int InitFont();
void KillFont();
int SFC_Get_SCREEN();
void draw_bg(SDL_Surface *bg);                       //设置背景
void DrawText2(const char *textmsg, int x, int y);   //SDL_ttf
void DrawText3(const char *textmsg, int x, int y);   //内存操作不刷新
void SFC_Flip();                                     //刷新显示
void SDL_TEST();

void str_log(char *info);

//矩形填充，用于着重显示选择项
void drawrect(Uint8 R, Uint8 G, Uint8 B,int x,int y,int width,int heigth);
//日志

//void sal_VideoPrint2(8,50,message1,SAL_RGB(31,31,31));
//void sal_VideoPrint(s32 x, s32 y, const char *buffer, u32 color);

//按键比对
void readkey();
int parsekey(unsigned long code);
//按键定义
#define SDL_INPUT_UP SDLK_UP
#define SDL_INPUT_DOWN SDLK_DOWN
#define SDL_INPUT_LEFT SDLK_LEFT
#define SDL_INPUT_RIGHT SDLK_RIGHT
#define SDL_INPUT_A SDLK_LCTRL
#define SDL_INPUT_B SDLK_LALT
#define SDL_INPUT_X SDLK_SPACE
#define SDL_INPUT_Y SDLK_LSHIFT
#define SDL_INPUT_L SDLK_TAB
#define SDL_INPUT_R SDLK_BACKSPACE
#define SDL_INPUT_START SDLK_RETURN
#define SDL_INPUT_SELECT SDLK_ESCAPE
#define SDL_INPUT_MENU SDLK_HOME
#endif
