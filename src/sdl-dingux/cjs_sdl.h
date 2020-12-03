#ifndef CJS_SDL_H_INCLUDED
#define CJS_SDL_H_INCLUDED

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

//#include "port.h"
//#include "cheats.h"
#include "cjs_font.h"
//#include "cjs.h"
//#include "../burn/burnint.h" //cheat
#include "burner.h"			//tchar

//文件更新函数
void chj_update(unsigned long key, int ec_index);

//菜单函数
//int ec_Settings();
void sdl_cheats();




#endif // CJS_SDL_H_INCLUDED
