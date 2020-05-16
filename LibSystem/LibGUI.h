#ifndef _MENUGUI_H
#define _MENUGUI_H

#include "resource.h"



#define NOPRESS 0
#define INVALID 0
#define VALID 1

#define DEFAULTCOLOR "Blue","Blue","Red","Red",NOFILL
#define NAMESIZE 21

void drawMenu(double width, double height, double fH, struct menuControl* ctrl);
void drawStatus(struct menuControl* menuCtrl);

void newBookUI(struct menuControl* ctrl, struct winFrame* frame, struct BookInfo* book);
void showBookUI(struct menuControl* ctrl, struct winFrame* frame, struct BookArr* books,int n);
void displayBooksUI(struct menuControl* menuCtrl, struct BookList* books);
void editBookUI(struct menuControl* menuCtrl, struct BookInfo* book);
void deleteBookUI(struct menuControl* menuCtrl, struct BookInfo* book);
void IOMenu(struct menuControl* menuCtrl);
void accountMenu(struct menuControl* menuCtrl);
void helpMenu(struct menuControl* menuCtrl);
int drawOperationRectangle(double x, double y, double w, double h, char* title, char* option1, char* option2);
int drawMsgRectangle(double x, double y, double w, double h, char* title, char* msg, char* option);
void newLibraryUI(struct menuControl* menuCtrl, const struct winFrame* frame, char* dir);
bool validNewLibrary(const string s, int size);
void openFileUI(struct menuControl* menuCtrl, struct winFrame* frame,char *dir);
void returnToMainFrame(struct menuControl* menuCtrl);
void drawBackGround(double x, double y, double w, double h, string color);

#endif