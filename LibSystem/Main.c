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

#include "imgui.h"
#include "LibGUI.h"
#include "LibFunc.h"
#include "resource.h"


#define ADMIN 1
#define USER 2
#define GUEST 3


struct menuControl ctrl = { MENU(0), OPT(0), FALSE, MAINFRAME, GUEST, "", NOPRESS, 0};
struct menuControl* ctrlPtr = &ctrl;

struct AccArr accArr;
struct BookArr bookArr;
struct RecordArr recArr;

struct winFrame frame1;
struct winFrame frame2;
struct winFrame frame3;
struct winFrame frame4;

struct winFrame *mainFrame = &frame1;
struct winFrame *secFrame = &frame2;
struct winFrame *thirdFrame = &frame3;
struct winFrame *fourthFrame = &frame4;


static double winwidth, winheight;   // 窗口尺寸
// 清屏函数，provided in libgraphics
//void DisplayClear(void);
// 用户的显示函数
void display();//struct Library* libPtr
//initialize menuControl
void InitMenuControl();
void CharEventProcess(char ch);
void KeyboardEventProcess(int key, int event);
void MouseEventProcess(int x, int y, int button, int event);
void TimerEventProcess(int timerID);
void setFrame(double x, double y, double w, double h, struct winFrame* frame);
void InitProgram();
void exitFrame(string s);
void InitBook(struct BookInfo* book);


void nextFrame(string s);
void fileMenu();
void changeStatus(string s);
void openFile(char* dir);
bool createNewLib(char* dir);
bool newLibFun1(struct winFrame* frame);
bool openFileFun1(struct winFrame* frame);
void bookMenu();




void Main()
{

	SetWindowTitle("Library System");
	//SetWindowSize(10, 10); // 单位 - 英寸
	//SetWindowSize(15, 10);
	//SetWindowSize(10, 20);  // 如果屏幕尺寸不够，则按比例缩小
	

	InitGraphics();
	
	winwidth = GetWindowWidth();
	winheight = GetWindowHeight();
	InitProgram();
	// 注册时间响应函数
	registerCharEvent(CharEventProcess);        // 字符
	registerKeyboardEvent(KeyboardEventProcess);// 键盘
	registerMouseEvent(MouseEventProcess);      // 鼠标
	registerTimerEvent(TimerEventProcess);      // 定时器
}



//struct Library* libPtr
void display()
{
	// 清屏
	DisplayClear();
	
	drawMenu(winwidth,winheight,GetFontHeight(),ctrlPtr);
	switch (ctrlPtr->menuSelection)
	{
	case FILEMENU:
		fileMenu();
		break;
	case BOOKMENU:
		bookMenu();
		break;
	case IOMENU:
		IOMenu(ctrlPtr);
		break;
	case ACCMENU:
		accountMenu(ctrlPtr);
		break;
	case HELPMENU:
		helpMenu(ctrlPtr);
		break;
	}
	drawStatus(ctrlPtr);

}
void InitBook(struct BookInfo* book)
{
	book->bookId = 0;
	strcpy(book->bookName, "");
	for (int i = 0; i < 3; i++)strcpy(book->authors[i], "");
	book->borrowed = FALSE;
	for (int i = 0; i < 5; i++)strcpy(book->keywords[i], "");
	strcpy(book->pubDate, "");
	strcpy(book->publisher, "");
}
void bookMenu()
{
	static struct BookInfo book;
	int option = ctrlPtr->optSelection;
	if (option == 1) {
		if (ctrlPtr->frameLvl == 1)
			nextFrame("图书->新增");
		newBookUI(ctrlPtr, secFrame, &book);
		if (ctrlPtr->frameLvl == 2)
		{
			if (ctrlPtr->buttonPressed == OK) {
				//add book to library function
				//remember to clear static book
				//to change edited to true
				//set book id
				exitFrame("主界面：成功新增图书");
			}
			else if (ctrlPtr->buttonPressed == CANCEL) {
				exitFrame("主界面：取消新增图书");
			}
			if (ctrlPtr->frameLvl == 1)InitBook(&book);
		}
	}
	else if (option == 2) {
		if (ctrlPtr->frameLvl == 1) 
			nextFrame("图书->显示");
		showBookUI(ctrlPtr, secFrame, &bookArr, bookArr.n);
		if (ctrlPtr->frameLvl == 2) {
			if (ctrlPtr->buttonPressed != NOPRESS) {
				exitFrame("主界面：退出\"图书->显示\"");
			}
			//if(ctrlPtr->buttonPressed == view options)
		}
	}
	else if (option == 3) {
		if (ctrlPtr->frameLvl == 1) {
			nextFrame("图书->修改");
		}
	}
	else if (option == 4) {
		if (ctrlPtr->frameLvl == 1) {
			nextFrame("图书->删除图书");
		}
	}
}




// 用户的字符事件响应函数
void CharEventProcess(char ch)
{
	uiGetChar(ch); // GUI字符输入
	display(); //刷新显示
}

// 用户的键盘事件响应函数
void KeyboardEventProcess(int key, int event)
{
	uiGetKeyboard(key, event); // GUI获取键盘
	display(); // 刷新显示
}

// 用户的鼠标事件响应函数
void MouseEventProcess(int x, int y, int button, int event)
{
	uiGetMouse(x, y, button, event); //GUI获取鼠标
	display(); // 刷新显示
}

// 用户的计时器时间响应函数
void TimerEventProcess(int timerID)
{
	if (timerID == 1)
	{
		display(); // 刷新显示
	}
}

void InitMenuControl()
{
	ctrlPtr->menuSelection = MENU(0);
	ctrlPtr->optSelection = OPT(0);
	strcpy(ctrlPtr->status, "主界面");
	ctrlPtr->edited = FALSE;
	ctrlPtr->frameLvl = MAINFRAME;
	ctrlPtr->priority = GUEST;
	ctrlPtr->buttonPressed = NOPRESS;
	ctrlPtr->fun = 0;
}

void setFrame(double x, double y, double w, double h,struct winFrame* frame)
{
	frame->x = x;
	frame->y = y;
	frame->w = w;
	frame->h = h;
}

void InitProgram()
{
	double x = winwidth / 10;
	double y = winheight / 6;
	setFrame(0, 0, winwidth, winheight, mainFrame);
	setFrame(x * 2, y, x * 6, y * 4, secFrame);
	setFrame(x * 3, y, x * 4, y * 3, thirdFrame);
	setFrame(x * 4, y * 2, x * 2, y * 2, fourthFrame);
	InitMenuControl();
	accArr.n = 0;
	accArr.max = 100;
	bookArr.n = 20;
	bookArr.max = 100;
	//example
	char author[] = "author";
	char holder[20];
	char num[5];
	for (int i = 0; i < 20; i++){
		strcpy(holder, author);
		strcat(holder, itoa(i+1,num,10));
		strcpy(bookArr.books[i].authors[0], holder);
		bookArr.books[i].bookId = i+1;
		strcpy(bookArr.books[i].bookName, strcat(holder,(strcpy(holder, "book"), num)));
		if (i % 2)bookArr.books[i].borrowed = FALSE;
		else bookArr.books[i].borrowed = TRUE;
		strcpy(bookArr.books[i].keywords[0], num);
		strcpy(bookArr.books[i].pubDate, strcat(holder, (strcpy(holder, "date"), num)));
		strcpy(bookArr.books[i].publisher, strcat(holder, (strcpy(holder, "publisher"), num)));
	}
	//
	recArr.n = 0;
	recArr.max = 100;
}

void exitFrame(string s)
{
	switch (ctrlPtr->frameLvl) {
	case 1:
		InitMenuControl();
		break;
	case 2:
		ctrl.frameLvl = 1;
		ctrl.menuSelection = MENU(0);
		ctrl.optSelection = OPT(0);
		break;
	case 3:
		ctrl.frameLvl = 2;
		break;
	}
	ctrl.buttonPressed = NOPRESS;
	changeStatus(s);
}


void nextFrame(string s)
{
	switch (ctrlPtr->frameLvl) {
	case 1:
		ctrlPtr->frameLvl = 2;
		break;
	case 2:
		ctrlPtr->frameLvl = 3;
		break;
	case 3:
		ctrlPtr->frameLvl = 4;
		break;
	case 4:
		break;
	}
	ctrlPtr->buttonPressed = NOPRESS;

	changeStatus(s);
}

void changeStatus(string s)
{
	strcpy(ctrl.status, s);
}



void fileMenu()
{
	static char dir[100];
	int option = ctrlPtr->optSelection;
	if (option == 1) {
		if (ctrlPtr->frameLvl == 1)
			nextFrame("新建图书库");
		newLibraryUI(ctrlPtr, secFrame, dir);
		if (ctrlPtr->frameLvl == 2) {
			if (ctrlPtr->buttonPressed == OK) {
				if (createNewLib(dir)) {
					ctrlPtr->edited = TRUE;
					exitFrame("主界面：成功建图书库");
				}
				else {
					nextFrame("创建失败");
					ctrlPtr->fun = 1;
				}
			}
			else if (ctrlPtr->buttonPressed == CANCEL) {
				exitFrame("主界面：取消创建图书库");
			}
		}
		if (ctrlPtr->frameLvl == 1)strcpy(dir, "");
		if (ctrlPtr->frameLvl == 3) {
			if (ctrlPtr->fun == 1) {
				if (newLibFun1(fourthFrame)) {
					exitFrame("创建图书库：重新输入");
					ctrlPtr->fun = 0;
				}
			}
		}
	}
	else if (option == 2) {
		if (ctrlPtr->frameLvl == 1)
			nextFrame("打开文件");
		openFileUI(ctrlPtr, secFrame, dir);
		if (ctrlPtr->frameLvl == 2) {
			if (ctrlPtr->buttonPressed == OK) {
				if (strcmp(dir, "") == 0) {
					nextFrame("未选择文件");
					ctrlPtr->fun = 1;
				}
				else {
					openFile(dir);
					exitFrame("主界面：已打开文件");
				}
			}
			else if (ctrlPtr->buttonPressed == CANCEL)
			{
				exitFrame("主界面：退出打开文件");
			}
		}
		if (ctrlPtr->frameLvl == 1)strcpy(dir, "");
		if (ctrlPtr->frameLvl == 3) {
			if (ctrlPtr->fun == 1) {
				if (openFileFun1(fourthFrame)) {
					exitFrame("打开文件：重新选择文件");
				}
			}
		}
	}
	else if (option == 3) {
		//saveFile();
		changeStatus("已保存");
	}
	else if (option == 4)exit(-1); //exit program
}
bool createNewLib(char* dir)//implement
{
	return FALSE;
}
bool newLibFun1(struct winFrame* f)
{
	if (drawMsgRectangle(f->x, f->y, f->w, f->h, "", "创建失败请重新输入", "确定") == OK) {
		return TRUE;
	}
	return FALSE;	
}

bool openFileFun1(struct winFrame* f)
{
	if (drawMsgRectangle(f->x, f->y, f->w, f->h, "", "请选择文件", "确定") == OK) {
		return TRUE;
	}
	return FALSE;
}
void openFile(char* dir) { }//implement