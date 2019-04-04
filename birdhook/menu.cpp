/* nuklear - 1.32.0 - public domain */
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include <cstdlib>

#include <iostream>
#include <direct.h>
#include <io.h>
#include <algorithm> 
#include <vector>

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_IMPLEMENTATION
#define NK_GDI_IMPLEMENTATION
#include "nuklear/nuklear.h"
#include "nuklear_gdi.h"
#include "Configuration.hpp"
#include "MiscScript.h"
#include "menu.h"
#include "gettext/libgnuintl.h"


int nWidth, nHeight;
static int menuindex = 0, aimbotindex = 0;
static char* MenuTitle = "Paste2Cheats.com    " "build :" __DATE__ " " __TIME__;
static std::vector<std::string> files;//存放文件名
char configname[32] = { 0 };
int confignamelen = 20;
static bool deltip = false;
static int configsindex = 0, Lweaponsindex = 0;
static bool Visible = true;
NK_Keybind Ktrigger;


void mainmenu(struct nk_context *ctx);
void visualmenu(struct nk_context *ctx);
void aimbotmenu(struct nk_context *ctx);
void configmenu(struct nk_context *ctx);
void flashconfiglist();
void miscmenu(struct nk_context *ctx);
void Init_Language();

namespace Setting
{
	int VGlowEsp = { 0 };
	int Atriggerbot = { 0 };
	int AtriggerbotKey = { 0 };
	int Atriggerbotdelay = { 0 };
	int AtriggerbotfovBased = { 0 };
	int MBunnyhop = { 0 };
	int VGlowEspAplha = { 255 };
	int VGlowOnlyVisible = { 0 };
	int AAimbotenable = { 0 };
	int AHitboxList[34] = { 0 };
	int AFOVList[34] = { 0 };
	int ADelayList[34] = { 0 };
	int ArcsList[34] = { 0 };
	int ASmoothList[34] = { 0 };
	int AKillDelayList[34] = { 0 };
	int AStartBList[34] = { 0 };
	int AendbulletList[34] = { 0 };

}

namespace menu_string
{
	char *GlowEsp;
	char *GlowEspAlpah;
	char *AutoJump;
}

char **SLWeaponsList = new char*[34]{ u8"沙鹰", u8"双枪", "CN57","Tec-9", u8"洛洛克", "USP-S", u8"CZ75 自动手枪", u8"R8 左轮", "P2000", "P250",
		"AK-47", "AUG", u8"加利尔 AR", u8"法玛斯", "M4A4","M4A1","SG 553",
		"MAC-10","MP5", "MP7", "MP9", "P90", "UMP-45", u8"PP-野牛",
		"MAG-7", "XM1014",u8"内格夫", u8"截短散弹枪", u8"新星", "M249",
		 "AWP","SSG 08","SCAR-20", "G3SG1" };

char **SLHitboxo = new char*[5]{ u8"头",u8"脖子",u8"胸部",u8"胃",u8"最近" };

static LRESULT CALLBACK
WindowProc(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

	if (Visible)
	{
		if (nk_gdi_handle_event(wnd, msg, wparam, lparam))
			return 0;
	}
    return DefWindowProcW(wnd, msg, wparam, lparam);
}

wchar_t *rand_str(const int len)
{
	srand((unsigned)time(NULL));
	wchar_t *str = new wchar_t[len + 8];
	int i;
	for (i = 0; i < len; ++i)
		str[i] = 'A' + std::rand() % 26;
	str[++i] = '\0';
	str[++i] = '\0';
	str[++i] = '\0';
	str[++i] = '\0';
	return str;
}

int menuinit(void)
{
	nWidth = GetSystemMetrics(SM_CXSCREEN);  //屏幕宽度    
	nHeight = GetSystemMetrics(SM_CYSCREEN); //屏幕高度

    GdiFont* font;
    struct nk_context *ctx;

    WNDCLASSW wc;
    ATOM atom;
    RECT rect = { 0, 0,  nWidth, nHeight };
    HWND wnd;
    HDC dc;
    int running = 1;
    int needs_refresh = 1;

    /* Win32 */
    memset(&wc, 0, sizeof(wc));
    wc.style = CS_DBLCLKS;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandleW(0);
	wc.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(0, 0, 0));
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = rand_str(std::rand() % 26);
    atom = RegisterClassW(&wc);

    //AdjustWindowRectEx(&rect, style, FALSE, exstyle);
    wnd = CreateWindowExW(WS_EX_TOPMOST | WS_EX_LAYERED, wc.lpszClassName, rand_str(std::rand() % 26),
		WS_POPUP, CW_USEDEFAULT, CW_USEDEFAULT,
        rect.right - rect.left, rect.bottom - rect.top,
        NULL, NULL, wc.hInstance, NULL); //| WS_EX_TRANSPARENT
    dc = GetDC(wnd);

	//SetLayeredWindowAttributes(wnd, 0, 1.f, LWA_ALPHA);

	SetLayeredWindowAttributes(wnd, 0, RGB(0, 0, 0), LWA_COLORKEY);

	ShowWindow(wnd, SW_SHOW);
    /* GUI */
    font = nk_gdifont_create("simfang", 15);
    ctx = nk_gdi_init(font, dc, nWidth, nHeight);

	flashconfiglist();
    /* style.c */
    #ifdef INCLUDE_STYLE
    /*set_style(ctx, THEME_WHITE);*/
    /*set_style(ctx, THEME_RED);*/
    /*set_style(ctx, THEME_BLUE);*/
    /*set_style(ctx, THEME_DARK);*/
    #endif

	/*putenv("LANG = zh_CN");
	setlocale(LC_ALL, "");
	bindtextdomain("zh_CN", "C:\\privatecheats\\");
	textdomain("zh_CN");*/
	Init_Language();

	printf("version :%d", libintl_version);

	while (running)
	{
		{
			static time_t oldtime = 0;

			static DWORD oldkeytime = 0;

			if ((GetTickCount() - oldkeytime) > 15)
			{
				if (GetAsyncKeyState(VK_INSERT) && (time(0) - oldtime) > 1)
				{
					Visible = !Visible;
					oldtime = time(0);

					if (!Visible)
					{
						/* 画背景 */
						nk_gdi_render(nk_rgb(0, 0, 0));
					}
				}
			}
			oldkeytime = GetTickCount();


			MSG msg;
			nk_input_begin(ctx);

			while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE)) {
				if (msg.message == WM_QUIT)
					running = 0;
				TranslateMessage(&msg);
				DispatchMessageW(&msg);
				needs_refresh = 1;
			}
			nk_input_end(ctx);
			/* GUI */
			if (Visible)
			{
				mainmenu(ctx);

				/* 画背景 */
				nk_gdi_render(nk_rgb(0, 0, 0));
			}
		}
	}

	nk_gdifont_del(font);
	ReleaseDC(wnd, dc);
	UnregisterClassW(wc.lpszClassName, wc.hInstance);

    return 0;
}

#pragma region KeyString

char* MyKeyStrings[254] = { "[ _ ]", "[M1]", "[M2]", "[BRK]", "[M3]", "[M4]", "[M5]",
"[ _ ]", "[BSPC]", "[TAB]", "[ _ ]", "[ _ ]", "[ _ ]", "[ENTER]", "[ _ ]", "[ _ ]", "[SHI]",
"[CTRL]", "[ALT]","[PAU]","[CAPS]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]",
"[ESC]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[SPACE]","[PGUP]", "[PGDOWN]", "[END]", "[HOME]", "[LEFT]",
"[UP]", "[RIGHT]", "[DOWN]", "[ _ ]", "[PRNT]", "[ _ ]", "[PRTSCR]", "[INS]","[DEL]", "[ _ ]", "[0]", "[1]",
"[2]", "[3]", "[4]", "[5]", "[6]", "[7]", "[8]", "[9]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]",
"[ _ ]", "[A]", "[B]", "[C]", "[D]", "[E]", "[F]", "[G]", "[H]", "[I]", "[J]", "[K]", "[L]", "[M]", "[N]", "[O]", "[P]", "[Q]", "[R]", "[S]", "[T]", "[U]",
"[V]", "[W]", "[X]","[Y]", "[Z]", "[LFTWIN]", "[RGHTWIN]", "[ _ ]", "[ _ ]", "[ _ ]", "[NUM0]", "[NUM1]",
"[NUM2]", "[NUM3]", "[NUM4]", "[NUM5]", "[NUM6]","[NUM7]", "[NUM8]", "[NUM9]", "[*]", "[+]", "[_]", "[-]", "[.]", "[/]", "[F1]", "[F2]", "[F3]",
"[F4]", "[F5]", "[F6]", "[F7]", "[F8]", "[F9]", "[F10]", "[F11]", "[F12]","[F13]", "[F14]", "[F15]", "[F16]", "[F17]", "[F18]", "[F19]", "[F20]", "[F21]",
"[F22]", "[F23]", "[F24]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]","[ _ ]", "[ _ ]", "[ _ ]",
"[NUM LOCK]", "[SCROLL LOCK[", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]",
"[ _ ]", "[ _ ]","[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[LSHFT]", "[RSHFT]", "[LCTRL]",
"[RCTRL]", "[LMENU]", "[RMENU]", "[ _ ]","[ _ ]", "[ _ ]","[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]",
"[ _ ]", "[ _ ]", "[ _ ]", "[NTRK]", "[PTRK]", "[STOP]", "[PLAY]", "[ _ ]", "[ _ ]",
"[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[;]", "[+]", "[,]", "[-]", "[.]", "[/?]", "[~]", "[ _ ]", "[ _ ]",
"[ _ ]", "[ _ ]","[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]",
"[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]","[ _ ]",
"[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[{]", "[\\|]", "[}]", "['\"]", "[ _ ]",
"[ _ ]", "[ _ ]", "[ _ ]","[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]",
"[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]",
"[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]",
"[ _ ]", "[ _ ]" };

#pragma endregion


void NK_Keybind::Run(struct nk_context *ctx, int *BindKey, char *KeyName)
{
	nk_labelf(ctx, NK_TEXT_LEFT, u8"key :%s", MyKeyStrings[*BindKey]);
	if (StartBind)
	{
		for (int i = 2; i < 255; i++)
		{
			if (GetAsyncKeyState(i) & 0x8000)
			{
				if (i == VK_ESCAPE)
				{
					StartBind = false;
					*BindKey = 0;
					break;
				}

				*BindKey = i;
				StartBind = false;
				break;
			}
		}
	}
	if (nk_button_label(ctx, KeyName))
		StartBind = true;
}

void mainmenu(struct nk_context *ctx)
{
	if (nk_begin(ctx, MenuTitle, nk_rect(50, 50, nWidth/2, nHeight/2),
		NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_TITLE))
	{
		nk_layout_row_dynamic(ctx, nHeight / 22, 4);
		if (nk_select_label(ctx, u8"视觉", NK_TEXT_CENTERED, menuindex == 0))
			menuindex = 0;
		if (nk_select_label(ctx, u8"瞄准", NK_TEXT_CENTERED, menuindex == 1))
			menuindex = 1;
		if (nk_select_label(ctx, u8"杂项", NK_TEXT_CENTERED, menuindex == 2))
			menuindex = 2;
		if (nk_select_label(ctx, u8"配置", NK_TEXT_CENTERED, menuindex == 3))
			menuindex = 3;

		nk_layout_row_dynamic(ctx, 15, 1);
		nk_label(ctx, "", NK_TEXT_CENTERED);

		switch (menuindex)
		{
		case 0:
		{
			visualmenu(ctx);
		}break;

		case 1:
		{
			aimbotmenu(ctx);
		}break;

		case 2:
		{
			miscmenu(ctx);
		}break;

		case 3:
		{
			configmenu(ctx);
		}break;
		}

	nk_end(ctx);
	}
	
}

void visualmenu(struct nk_context *ctx)
{
	nk_layout_row_dynamic(ctx, 20, 1);
	nk_checkbox_label(ctx, menu_string::GlowEsp, &Setting::VGlowEsp);

	nk_labelf(ctx, NK_TEXT_LEFT, menu_string::GlowEspAlpah, Setting::VGlowEspAplha);
	nk_slider_int(ctx, 0, &Setting::VGlowEspAplha, 255, 1);

	nk_checkbox_label(ctx, u8"只显示可见部分", &Setting::VGlowOnlyVisible);
}

void aimbotmenu(struct nk_context *ctx)
{

	nk_layout_row_dynamic(ctx, 25, 2);
	if (nk_select_label(ctx, u8"自瞄", NK_TEXT_CENTERED, aimbotindex == 0))
		aimbotindex = 0;
	if (nk_select_label(ctx, u8"扳机", NK_TEXT_CENTERED, aimbotindex == 1))
		aimbotindex = 1;

	switch (aimbotindex)
	{
	case 0:
	{
		nk_layout_row_dynamic(ctx, 20, 1);
		nk_checkbox_label(ctx, u8"自瞄启用", &Setting::AAimbotenable);

		nk_layout_row_dynamic(ctx, 16, 1);
		nk_label(ctx, u8"武器列表", NK_TEXT_LEFT);
		Lweaponsindex = nk_combo(ctx, (const char **)SLWeaponsList, 34, Lweaponsindex, 20, nk_vec2(200, 300));
		nk_layout_row_dynamic(ctx, 260, 1);
		nk_group_begin(ctx, "function", NK_WINDOW_BORDER);
		{
			nk_layout_row_dynamic(ctx, 16, 1);
			nk_label(ctx, u8"瞄准部位", NK_TEXT_LEFT);
			Setting::AHitboxList[Lweaponsindex] = nk_combo(ctx, (const char **)SLHitboxo, 5, Setting::AHitboxList[Lweaponsindex], 20, nk_vec2(200, 200));

			nk_labelf(ctx, NK_TEXT_LEFT, u8"平滑度:%d", Setting::ASmoothList[Lweaponsindex]);
			nk_slider_int(ctx, 0, &Setting::ASmoothList[Lweaponsindex], 75, 1);

			nk_labelf(ctx, NK_TEXT_LEFT, u8"压枪:%d", Setting::ArcsList[Lweaponsindex]);
			nk_slider_int(ctx, 0, &Setting::ArcsList[Lweaponsindex], 200, 1);

			nk_labelf(ctx, NK_TEXT_LEFT, u8"FOV:%d", Setting::AFOVList[Lweaponsindex]);
			nk_slider_int(ctx, 0, &Setting::AFOVList[Lweaponsindex], 100, 1);

			nk_labelf(ctx, NK_TEXT_LEFT, u8"射击延迟:%d", Setting::ADelayList[Lweaponsindex]);
			nk_slider_int(ctx, 0, &Setting::ADelayList[Lweaponsindex], 100, 1);

			nk_labelf(ctx, NK_TEXT_LEFT, u8"击杀后延迟:%d", Setting::AKillDelayList[Lweaponsindex]);
			nk_slider_int(ctx, 0, &Setting::AKillDelayList[Lweaponsindex], 100, 1);

			nk_labelf(ctx, NK_TEXT_LEFT, u8"%d 次射击后开始瞄准", Setting::AStartBList[Lweaponsindex]);
			nk_slider_int(ctx, 0, &Setting::AStartBList[Lweaponsindex], 30, 1);

			nk_labelf(ctx, NK_TEXT_LEFT, u8"%d 次射击后停止瞄准", Setting::AendbulletList[Lweaponsindex]);
			nk_slider_int(ctx, 0, &Setting::AendbulletList[Lweaponsindex], 30, 1);

			nk_group_end(ctx);
		}
	}break;

	case 1:
	{
		nk_layout_row_dynamic(ctx, 25, 1);
		nk_checkbox_label(ctx, u8"自动扳机启用", &Setting::Atriggerbot);

		nk_layout_row_dynamic(ctx, 25, 2);
		Ktrigger.Run(ctx, &Setting::AtriggerbotKey, u8"自动扳机键");

		nk_layout_row_dynamic(ctx, 25, 1);
		nk_checkbox_label(ctx, u8"基于FOV的自动扳机", &Setting::AtriggerbotfovBased);

		nk_labelf(ctx, NK_TEXT_LEFT, u8"自动扳机延时:%d ms", Setting::Atriggerbotdelay);
		nk_slider_int(ctx, 0, &Setting::Atriggerbotdelay, 45, 1);
	}break;
	}

}

void getFiles(std::string cate_dir)
{

	files.clear();

	_finddata_t file;
	long lf;
	//输入文件夹路径
	if ((lf = _findfirst(cate_dir.c_str(), &file)) == -1) {
		return;
	}
	else {
		while (_findnext(lf, &file) == 0) {
			//输出文件名
			//cout<<file.name<<endl;
			if (strcmp(file.name, ".") == 0 || strcmp(file.name, "..") == 0)
				continue;
			files.push_back(file.name);
		}
	}
	_findclose(lf);


	//排序，按从小到大排序
	std::sort(files.begin(), files.end());

	//return files;
}

void flashconfiglist()
{
	getFiles("C:\\privatecheats\\*");
}

void configmenu(struct nk_context *ctx)
{
	//nk_layout_row_static(ctx, 300, 400, 1);
	nk_layout_row_dynamic(ctx, nHeight / 4, 1);
	if (nk_group_begin(ctx, "configs", NK_WINDOW_BORDER))
	{
		nk_layout_row_dynamic(ctx, 20, 1);
		int i = 0;
		for (auto conf : files)
		{
			if (nk_select_label(ctx, conf.c_str(), NK_TEXT_CENTERED, i == configsindex))
				configsindex = i;
			i++;
		}

		nk_group_end(ctx);
	}

	nk_layout_row_dynamic(ctx, 20, 3);
	if (nk_button_label(ctx, u8"载入"))
	{
		static TCHAR luapath[MAX_PATH];
		sprintf(luapath, "C:\\privatecheats\\%s", files[configsindex].c_str());
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)execluafile, (LPVOID)luapath, 0, NULL);
		//execluafile((LPVOID *)luapath);
	}
	if (nk_button_label(ctx, u8"删除"))
		deltip = true;

	if (deltip)
	{
		static struct nk_rect s = { nWidth / 4, nHeight / 4, 400, 120 };
		if (nk_popup_begin(ctx, NK_POPUP_STATIC, "delete", 0, s))
		{
			nk_layout_row_dynamic(ctx, 25, 1);
			nk_label(ctx, u8"删除", NK_TEXT_LEFT);
			nk_label(ctx, files[configsindex].c_str(), NK_TEXT_LEFT);
			nk_layout_row_dynamic(ctx, 25, 2);
			if (nk_button_label(ctx, u8"是")) {
				static TCHAR delpath[MAX_PATH];
				sprintf(delpath, "C:\\privatecheats\\%s", files[configsindex].c_str());
				remove(delpath);
				deltip = 0;
				nk_popup_close(ctx);
				flashconfiglist();
			}
			if (nk_button_label(ctx, u8"返回")) {
				deltip = 0;
				nk_popup_close(ctx);
			}
			nk_popup_end(ctx);
		}
		else deltip = false;
	};

	if (nk_button_label(ctx, u8"刷新列表"))
		flashconfiglist();


	nk_layout_row_dynamic(ctx, 25, 1);
	nk_edit_string(ctx, NK_EDIT_SIMPLE, configname, &confignamelen, 32, nk_filter_default);
	if (nk_button_label(ctx, u8"保存配置"))
	{
		Config->Save(configname);
		flashconfiglist();
	}
}

void miscmenu(struct nk_context *ctx)
{
	nk_layout_row_dynamic(ctx, 25, 1);
	nk_checkbox_label(ctx, menu_string::AutoJump, &Setting::MBunnyhop);
}

void Init_Language()
{
	menu_string::GlowEsp = "Glow ESP";
	menu_string::GlowEspAlpah = "Glow ESP Alpha";
	menu_string::AutoJump = "Auto Jump";
}