#include "graphics.h"
#include "extgraph.h"
#include "genlib.h"
#include "simpio.h"
#include "conio.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#include <windows.h>
#include <olectl.h>
#include <mmsystem.h>
#include <wingdi.h>
#include <ole2.h>
#include <ocidl.h>
#include <winuser.h>
#include <filesystem>
#include <fileapi.h>

#include <time.h>

#include "imgui.h"
#include "LibGUI.h"
#include "LibFunc.h"

char* menuListFile[] = { "文件","图书库新建","打开","保存","退出" };	//menu1
char* menuListInventory[] = { "图书","新增","显示","修改","删除图书" };//menu2
char* menuListIO[] = { "借还","借书","还书" };						//menu3
char* menuListAccount[] = { "用户","登录","注册","审核","修改","删除" };//menu4
char* menuListHelp[] = { "帮助","关于本软件","使用方法" };				//menu5
char** menus[] = { menuListFile,menuListInventory,menuListIO,menuListAccount,menuListHelp };
const int menu1Count = 5;
const int menu2Count = 5;
const int menu3Count = 3;
const int menu4Count = 7;
const int menu5Count = 3;
char ex1[] = "样板";
char ex2[] = "最长的样板|ctrl + x";



//standard window setting
/*
double width = GetWindowWidth();
double height = GetWindowHeight();
double x = width / 5;
double y = height / 4;
double w = 3 * x;
double h = 2 * y;
*/




void drawMenu(double width, double height, double fH, struct menuControl* ctrl)
{
	double x = 0;
	double y = height;
	double h = fH * 1.5;
	double w = TextStringWidth(ex1) * 2; //控件宽度
	double wlist = TextStringWidth(ex2) * 1.2;
	int selection;
	// menu bar
	drawMenuBar(0, y - h, width-.01, h);

	if (ctrl->frameLvl==MAINFRAME){
		setMenuColors("Blue", "Blue", "Red", "Red", 0);
		//文件菜单
		selection = menuList(GenUIID(0), x, y - h, w, wlist, h, menuListFile, menu1Count);
		if (selection > 0 && selection < menu1Count)
		{
			ctrl->menuSelection = FILEMENU;
			ctrl->optSelection = selection;
		}
		//图书菜单
		selection = menuList(GenUIID(0), x + w, y - h, w, wlist, h, menuListInventory, menu2Count);
		if (selection > 0 && selection < menu2Count)
		{
			ctrl->menuSelection = BOOKMENU;
			ctrl->optSelection = selection;
		}

		//借还菜单
		selection = menuList(GenUIID(0), x + 2 * w, y - h, w, wlist, h, menuListIO, menu3Count);
		if (selection > 0 && selection < menu3Count)
		{
			ctrl->menuSelection = IOMENU;
			ctrl->optSelection = selection;
		}

		//用户菜单
		selection = menuList(GenUIID(0), x + 3 * w, y - h, w, wlist, h, menuListAccount, menu4Count);
		if (selection > 0 && selection < menu4Count)
		{
			ctrl->menuSelection = ACCMENU;
			ctrl->optSelection = selection;
		}

		//帮助菜单
		selection = menuList(GenUIID(0), x + 4 * w, y - h, w, wlist, h, menuListHelp, menu5Count);
		if (selection > 0 && selection < menu5Count)
		{
			ctrl->menuSelection = HELPMENU;
			ctrl->optSelection = selection;
		}
	}
	else
	{
		setMenuColors("Blue", "Blue", "Blue", "Blue", 0);
		//文件菜单
		menuList(GenUIID(0), x, y - h, w, wlist, h, menuListFile, 1);

		//图书菜单
		menuList(GenUIID(0), x + w, y - h, w, wlist, h, menuListInventory, 1);

		//借还菜单
		menuList(GenUIID(0), x + 2 * w, y - h, w, wlist, h, menuListIO, 1);

		//用户菜单
		menuList(GenUIID(0), x + 3 * w, y - h, w, wlist, h, menuListAccount, 1);

		//帮主菜单
		menuList(GenUIID(0), x + 4 * w, y - h, w, wlist, h, menuListHelp, 1);
	}
}

void newBookUI(struct menuControl* ctrl, struct winFrame* frame, struct BookInfo* book)
{
	double x = frame->x;
	double y = frame->y;
	double w = frame->w;
	double h = frame->h;
	double yindent = GetFontHeight() * 1.5;
	static int pressed = NOPRESS;
	char msg[] = "出版日期(YYYYMMDD)：";
	double xindent = TextStringWidth(msg)*1.1;
	int id = 1;
	if(pressed == NOPRESS ) pressed = drawOperationRectangle(x, y, w, h, "", "确定", "取消");
	else drawOperationRectangle(x, y, w, h, "", "确定", "取消");
	if (ctrl->frameLvl == 2) {
		if (pressed != NOPRESS) {
			ctrl->buttonPressed = pressed;
			pressed = NOPRESS;
		}
		x = x + w / 8;
		y = y + h - yindent * 3;
		w = w / 2;
		h = yindent;

		char msg1[]="书名:";
		MovePen(x, y);
		DrawTextString(msg1);
		textbox(id++, x + xindent, y, w, h, book->bookName,20);
		SetPenColor(FRAMECOLOR);

		char msg2[]="关键词:";
		string s;
		for (int i = 0; i < 5; i++) {
			//strcat(msg2);
			y -= yindent*1.1;
			MovePen(x, y);
			DrawTextString(msg2);
			textbox(id++, x + xindent, y, w, h, book->keywords[i], 20);
			SetPenColor(FRAMECOLOR);
		}
		char msg3[] = "作者:";
		for (int i = 0; i < 3; i++) {
			y -= yindent*1.1;
			MovePen(x, y);
			DrawTextString(msg3);
			textbox(id++, x + xindent, y, w, h, book->authors[i], 20);
			SetPenColor(FRAMECOLOR);
		}
		char msg4[] = "出版社：";
		y -= yindent*1.1;
		MovePen(x, y);
		DrawTextString(msg4);
		textbox(id++, x + xindent, y, w, h, book->publisher, 20);
		SetPenColor(FRAMECOLOR);

		char msg5[] = "出版日期(YYYYMMDD)：";
		y -= yindent*1.1;
		MovePen(x, y);
		DrawTextString(msg5);
		textbox(id++, x + xindent, y, w, h, book->pubDate, 8);
		SetPenColor(FRAMECOLOR);
	}
}

void showBookUI(struct menuControl* ctrl, struct winFrame* frame, struct BookArr* books, int n)
{
	double x = frame->x;
	double y = frame->y;
	double w = frame->w;
	double h = frame->h;
	double yindent = GetFontHeight() * 1.5;
	double xindent = w / 16;
	char *views[] = { "id", "书名", "关键词", "作者", "出版社", "出版日期", "已借出" };
	double buttonWidth = TextStringWidth(views[5]);
	double buttonHeight = GetFontHeight() * 2;
	int limit = 10;
	static int pressed = NOPRESS;
	static int view = VIEWBOOKID;
	static int page = 0;
	int maxPage = n / limit;
	if (n % limit)maxPage++;
	if (pressed == NOPRESS) pressed = drawOperationRectangle(x, y, w, h, "", "退出", "退出");
	else drawOperationRectangle(x, y, w, h, "", "退出", "退出");
	if (ctrl->frameLvl == 2) {
		if (pressed != NOPRESS) {
			ctrl->buttonPressed = pressed;
			view = VIEWBOOKID;
			pressed = NOPRESS;
			page = 0;
		}
		y = y + h - buttonHeight*2;
		for (int i = 0; i < 7; i++) {
			if (i == 0)x = x + xindent;
			else x = x + 2 * xindent;
			if (button(GenUIID(0) + i, x, y, buttonWidth, buttonHeight, views[i])) {
				view = i + VIEWBOOKID;
				page = 0;
			}
			SetPenColor(FRAMECOLOR);
		}
		x = frame->x + xindent;
		y -= yindent*1.5;
		if (button(GenUIID(0), x, y, buttonWidth, buttonHeight, "上一页"))
				if (page > 0)page--;
		x = x + xindent * 2;
		if (button(GenUIID(0), x, y, buttonWidth, buttonHeight, "下一页"))
			if (page < maxPage - 1)page++;
		SetPenColor(FRAMECOLOR);

		x = frame->x + xindent;
		char id[8];
		MovePen(frame->x, frame->y);
		char pageNum[10]="page";
		DrawTextString(strcat(pageNum,itoa(page+1,id,10)));
		for (int i = 0,j=page*limit; i < limit && ((j + i) < n); i++) {
			y -= yindent;
			MovePen(x, y);
			switch (view) {
			case VIEWBOOKAUTHOR:
				DrawTextString(books->books[j+i].authors[0]);
				break;
			case VIEWBOOKID:
				//itoa
				DrawTextString(itoa(books->books[j+i].bookId,id,10));
				break;
			case VIEWBOOKKEYWD:
				DrawTextString(books->books[j+i].keywords[0]);
				break;
			case VIEWBOOKNAME:
				DrawTextString(books->books[i+j].bookName);
				break;
			case VIEWBOOKOUT:
				//if(true) out ...
				//else out...
				if (books->books[i+j].borrowed)DrawTextString("true");
				else DrawTextString("false");
				break;
			case VIEWBOOKPUB:
				DrawTextString(books->books[i+j].publisher);
				break;
			case VIEWBOOKPUBDATE:
				DrawTextString(books->books[i+j].pubDate);
				break;
			}
		}


	}
	//x = x + xindent;
	
}

void drawStatus(struct menuControl* menuCtrl)
{
	double h = GetFontHeight() * 1.5;
	double w = GetWindowWidth();
	char recentCtrlMsg[] = { "status: " };
	double y = h;
	double x = 0;
	MovePen(x, y);
	SetPenColor("Blue");
	DrawLine(w, 0);
	y = h / 4;
	drawLabel(x, y, recentCtrlMsg);
	SetPenColor("Brown");
	drawLabel(x + TextStringWidth(recentCtrlMsg), y, menuCtrl->status);
}






void displayBooksUI(struct menuControl* menuCtrl, struct BookList* books) {}
void editBookUI(struct menuControl* menuCtrl, struct BookInfo* book) {}
void deleteBookUI(struct menuControl* menuCtrl, struct BookInfo* book) {}


void IOMenu(struct menuControl* menuCtrl) {}
void accountMenu(struct menuControl* menuCtrl) {}
void helpMenu(struct menuControl* menuCtrl) {}

int drawOperationRectangle(double x, double y, double w, double h,char * title, char * option1, char * option2)
{
	char buttonStr[] = "@BUTTON@";
	double buttonWidth = TextStringWidth(buttonStr);
	double buttonHeight = GetFontHeight() * 2;
	bool ok = FALSE, cancel = FALSE;

	drawBackGround(x, y, w, h, "White");
	drawRectangle(x, y, w, h, NOFILL);
	double xindent = w / 10;
	ok = button(GenUIID(0), x + xindent, y, buttonWidth, buttonHeight, option1);
	SetPenColor("Blue");
	cancel = button(GenUIID(0), x + w - xindent - buttonWidth, y, buttonWidth, buttonHeight, option2);
	SetPenColor("Blue");
	if (ok)return OK;
	else if (cancel)return CANCEL;
	else return NOPRESS;
}

int drawMsgRectangle(double x, double y, double w, double h, char* title, char* msg, char* option)
{
	char buttonStr[] = "@BUTTON@";
	double buttonWidth = TextStringWidth(buttonStr);
	double buttonHeight = GetFontHeight() * 2;
	bool ok = FALSE;
	//setMenuColors("Grey", "Black", "Grey", "Black", NOFILL);
	//drawRectangle(x, y, w, h, NOFILL);
	SetPenColor("Light Gray");
	drawRectangle(x, y, w, h, FILL);
	setMenuColors("Blue", "Blue", "Red", "Red", NOFILL);
	drawBox(x, y, w, h, NOFILL, msg, 'L', "Black");
	ok = button(GenUIID(0), (x+w/2)-buttonWidth/2, y*1.1, buttonWidth, buttonHeight, option);
	if (ok)return OK;
	else return NOPRESS;
}

void newLibraryUI(struct menuControl* ctrl,const struct winFrame* frame, char* dir)
{
	char* label = menuListFile[ctrl->optSelection];
	static char newLibName[20];
	char newLibMsg[]="新建图书库名：";
	static int pressed = NOPRESS;
	drawBackGround(frame->x, frame->y, frame->w, frame->h, BKCOLOR);
	SetPenColor("Blue");

	pressed = drawOperationRectangle(frame->x, frame->y, frame->w, frame->h, label, "确定", "取消");

	MovePen(frame->x * 2 - TextStringWidth(newLibMsg), frame->y * 2 + GetFontHeight() / 2);
	DrawTextString(newLibMsg);
	if (ctrl->frameLvl == 2) {
		setTextBoxColors("Blue", "Blue", "Red", "Red", 0);
		if (pressed != NOPRESS)
		{
			ctrl->buttonPressed = pressed;
			strcpy(dir, newLibName);
			strcpy(newLibName, "");
			pressed = NOPRESS;
		}
		textbox(GenUIID(0), frame->x * 2, frame->y * 2, frame->w / 2, GetFontHeight() * 2, newLibName, 19);
		SetPenColor("Blue");
	}
}

void openFileUI(struct menuControl* menuCtrl,struct winFrame* frame, char * dir )
{
	double x = frame->x;
	double y = frame->y;
	double w = frame->w;
	double h = frame->h;
	char* label = menuListFile[menuCtrl->optSelection];
	static int pressed=NOPRESS;
	double fontHeight = GetFontHeight();
	double xindent;
	char buttonMsg[] = "选择";
	string prompt1 = "选择文件";
	string prompt2 = "打开文件:";
	double promptWidth = TextStringWidth(prompt2);
	double buttonWidth = TextStringWidth(buttonMsg);
	int id = 0;
	//to handle files
	HANDLE hFind;
	WIN32_FIND_DATA findData;	
	char dirNew[100] = {""};
	static char fileSelected[100] = {""};

	//operation window
	if(pressed == NOPRESS) pressed = drawOperationRectangle(x, y, w, h, label, "打开", "取消");
	else drawOperationRectangle(x, y, w, h, label, "打开", "取消");
	SetPenColor("Blue");

	//open default directory
	GetCurrentDirectoryA(100,dirNew);
	strcat(dirNew, "\\Data\\library*");
	hFind = FindFirstFile(dirNew, &findData);

	//display window headder
	MovePen(x, y + h - fontHeight * 1.5);
	DrawLine(w, 0);
	x = x + w / 50;
	y = y + h - fontHeight;
	MovePen(x, y);
	DrawTextString(prompt1);

	//display first file
	y = y - fontHeight*2;
	MovePen(x, y);
	DrawTextString(findData.cFileName);
	xindent = TextStringWidth(findData.cFileName);
	if (button(GenUIID(id++), x + xindent, y, buttonWidth, fontHeight, buttonMsg) != NOPRESS)
		strcpy(fileSelected, findData.cFileName);
	SetPenColor(FRAMECOLOR);

	//display consecutive files
	while (FindNextFile(hFind, &findData))
	{
		y -= fontHeight;
		MovePen(x, y);
		DrawTextString(findData.cFileName);
		xindent = TextStringWidth(findData.cFileName);
		if (button(GenUIID(id++), x + xindent, y, buttonWidth, fontHeight, buttonMsg) != NOPRESS)
			strcpy(fileSelected, findData.cFileName);
		SetPenColor(FRAMECOLOR);
	}

	//display "打开文件" selection
	x = frame->x + frame->w / 6;
	y = frame->y + buttonWidth*1.5;
	MovePen(x, y);
	DrawTextString(prompt2);
	MovePen(x + promptWidth, y);
	DrawTextString(fileSelected);

	if (pressed == CANCEL){
		strcpy(fileSelected, "");
		pressed = NOPRESS;
		menuCtrl->buttonPressed = CANCEL;
	}
	else if (pressed == OK){
		GetCurrentDirectoryA(100, dirNew);
		strcat(dirNew, "\\Data\\");
		strcat(dirNew, fileSelected);
		if (strcmp(fileSelected, "") == 0) {
			strcpy(dirNew, "");
		}
		strcpy(dir, dirNew);
		strcpy(fileSelected, "");
		pressed = NOPRESS;
		menuCtrl->buttonPressed = OK;
	}
}

void returnToMainFrame(struct menuControl *menuCtrl)
{


}

bool validNewLibrary(const string s, int size) {
	int len = strlen(s);
	if (len <= 0 || len >= size)return FALSE;
	else return TRUE;
}

void drawBackGround(double x, double y, double w, double h, string color)
{
	SetPenColor(color);
	drawRectangle(x, y, w, h, FILL);
	SetPenColor(FRAMECOLOR);
}