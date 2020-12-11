
#include "cjs_sdl.h"




//全局变量
int cheat_menu_yes = 0; //定义chj文件是否读取过
int all_ec_nums = -2; //定义chj文件有效的数量，-2没有文件，-1文件格式有问题，从0开始计数
extern unsigned long g_key;
extern SDL_Surface *g_bg;
char rom_name[1024];



extern CheatInfo* pCheatInfo;
CheatInfo* pCheat_index=NULL;
static int* nPrevCheatSettings = NULL;

/***
更新cheats
***/
extern bool bCheatsAllowed;
void cheats_mem()
{


	CheatInfo* pCurrentCheat = pCheatInfo;
	int nCurrentCheat = 0;
	while (pCurrentCheat) {
		bCheatsAllowed = true;
		CheatEnable(nCurrentCheat, nPrevCheatSettings[nCurrentCheat]);
		pCurrentCheat = pCurrentCheat->pNext;
		nCurrentCheat++;
	}

}
/***
文件更新函数
***/
void chj_update(unsigned long key, int ec_index)
{
	if(pCheat_index==NULL){
		return;
	}
	INT32 val=pCheat_index->nCurrent;
	INT32 i,max_val;
	for(i=0;i<CHEAT_MAX_OPTIONS;i++)
	{
		if(pCheat_index->pOption[i]->szOptionName){
			max_val=i;
		}
		else{
			break;
		}
	}
	if (key == SDL_INPUT_RIGHT){
		if (val < max_val){
			val ++;
			}
		}
	else if (key == SDL_INPUT_LEFT){
		if (val > 0){
			val --;
			}
		}
	nPrevCheatSettings[ec_index] = val;
	cheats_mem();
}



int get_ec_infos()
{
	TCHAR szFilename[MAX_PATH] = _T("");
	_stprintf(szFilename, _T("%s%s.ini"), szAppCheatsPath, BurnDrvGetText(DRV_NAME));
	FILE *fp=fopen(szFilename,"r");
	if(!fp) {
		return -2;
	}
	else{
		fclose (fp);
	}

	ConfigCheatLoad();
	int nCurrentCheat = 0;
	CheatInfo* pCurrentCheat;
	pCurrentCheat = pCheatInfo;
	// Save old cheat settings
	while (pCurrentCheat) {
		nCurrentCheat++;
		pCurrentCheat = pCurrentCheat->pNext;
	}
	nPrevCheatSettings = (int*)malloc(nCurrentCheat * sizeof(int));
	
	pCurrentCheat = pCheatInfo;
	nCurrentCheat = 0;
	while (pCurrentCheat) {
		nPrevCheatSettings[nCurrentCheat] = pCurrentCheat->nCurrent;
		pCurrentCheat = pCurrentCheat->pNext;
		nCurrentCheat++;
	}
	
	return (nCurrentCheat-1);
}



/***
显示信息
***/
int ec_Settings()
{
	SFC_Get_SCREEN();
	
	char ecname[1024];



	int start_y = 40; //开始高度
	int start_w = 15; //间距
	static int index = 0;
	static int offset_start = 0;
	static int start_now = index*start_w+start_y;
	

	if(cheat_menu_yes == 0)
	{
		//str_log("start");
		cheat_menu_yes=1;
		index = 0;
		offset_start = 0;
		start_now = index*start_w+start_y;
		all_ec_nums = get_ec_infos();
		
	}
	
	CheatInfo* pCurrentCheat = pCheatInfo;
	CheatAddressInfo* pAddressInfo;

	
	int done = 0, y, i;
	int max_entries = 12;
	int menu_size = all_ec_nums+1;
	
	static int offset_end = menu_size > max_entries ? max_entries : menu_size;

	char tmp[32];
	int cheat_index = 0;


	//先显示菜单，再更新菜单
	char cheat_msg[] = "金手指菜单";
	draw_bg(g_bg);
	DrawText3(cheat_msg, 0, 14);
	drawrect(255, 0, 0,  0,start_now,320,start_w);

	if (all_ec_nums > -1)
	{
		// Draw menu
		i = 0;
		y = 40;
		pCurrentCheat = pCheatInfo;
		while (pCurrentCheat && i < offset_end) {
			if(i>=offset_start){
				DrawText3(pCurrentCheat->szCheatName, 30, y);
				cheat_index=pCurrentCheat->nCurrent;
				DrawText3(pCurrentCheat->pOption[cheat_index]->szOptionName, 210, y);
				y += 15;
				if(i==index){
					pCheat_index=pCurrentCheat;
				}
			}
			i++;
			pCurrentCheat = pCurrentCheat->pNext;
		}
		// Update real screen
		SDL_Delay(16);
		SFC_Flip();




		while (!done) {
			// Parse input
			readkey();
			if (parsekey(SDL_INPUT_B))
				done = 1;
			if (parsekey(SDL_INPUT_UP)) {
				if (index > 0)
				{
					index--;
					if (offset_start == 0)
					{
						start_now=start_now-start_w;
					}
					if ((offset_start > 0) && (index < offset_start)  )
					{
						offset_start--;
						offset_end--;
						start_now=0*start_w+start_y;
					}
					else if ((offset_start > 0) && (index >= offset_start)  )
					{
						start_now=start_now-start_w;
					}
				} else
				{
					index = menu_size-1;
					offset_end = menu_size;
					offset_start = menu_size <= max_entries ? 0 : offset_end - max_entries;
					start_now=menu_size <= max_entries ? (menu_size-1)*start_w+start_y:(max_entries-1)*start_w+start_y;

				}
			}
			if (parsekey(SDL_INPUT_DOWN)) {
				if (index < (menu_size - 1))
				{
					index++;
					if (index < offset_end)
					{
						start_now=start_now+start_w;
					}
					if ((index >= offset_end) && (offset_end < menu_size)  )
					{
						offset_end++;
						offset_start++;
						start_now=(max_entries-1)*start_w+start_y;
					}
				}
				else
				{
					index = 0;
					offset_start = 0;
					offset_end = menu_size <= max_entries ? menu_size : max_entries;
					start_now=index*start_w+start_y;

				}
			}

			if (parsekey(SDL_INPUT_LEFT) || parsekey(SDL_INPUT_RIGHT) || parsekey(
					SDL_INPUT_A)){
				chj_update( g_key, index);
			}

			/***
			汉字处理
			***/
			draw_bg(g_bg);
			DrawText3(cheat_msg, 0, 14);
			drawrect(255, 0, 0,  0,start_now,320,start_w);




			// Draw menu
			i = 0;
			y = 40;
			pCurrentCheat = pCheatInfo;
			while (pCurrentCheat && i < offset_end) {
				if(i>=offset_start){
					DrawText3(pCurrentCheat->szCheatName, 30, y);
					cheat_index=pCurrentCheat->nCurrent;
					DrawText3(pCurrentCheat->pOption[cheat_index]->szOptionName, 210, y);
					y += 15;
					if(i==index){
						pCheat_index=pCurrentCheat;
					}
				}
				i++;
				pCurrentCheat = pCurrentCheat->pNext;
			}
			// Draw offset marks

			// Update real screen
			SDL_Delay(16);
			SFC_Flip();
		}
		//cheats_mem();
	}
	else if(all_ec_nums == -2)
	{
		DrawText3("无金手指文件,按B退出", 0, start_now);
		DrawText3(ecname, 0, start_now+start_w);
		SDL_Delay(16);
		SFC_Flip();
		while (!done) {
			readkey();
			if (parsekey(SDL_INPUT_B))
				done = 1;
		}
	}
	else if(all_ec_nums == -1)
	{
		DrawText3("金手指文件未发现有效数据,按B退出", 0, start_now);
		SDL_Delay(16);
		SFC_Flip();
		while (!done) {
			readkey();
			if (parsekey(SDL_INPUT_B))
				done = 1;
		}
	}

	// Clear screen
	return 0;
}

void sdl_cheats()
{
	//SDL_TEST();
	ec_Settings();
	
}
void cheats_exit()
{
	CheatExit();
	cheat_menu_yes = 0;

}
