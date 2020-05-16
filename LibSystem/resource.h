#ifndef _RESOURCE_H
#define _RESOURCE_H





//general
#define ZERO 0
#define LIMIT 20
#define LONGNAME 20
#define SHORTNAME 10
#define OPTION1 1
#define OPTION2 2
#define FILEMENU 0
#define BOOKMENU 1
#define IOMENU 2
#define ACCMENU 3
#define HELPMENU 4
#define OK 1
#define CANCEL -1
#define FILL 1
#define NOFILL 0
#define DETERMINE -2


//book view option
#define VIEWBOOKID 11
#define VIEWBOOKNAME 12
#define VIEWBOOKKEYWD 13
#define VIEWBOOKAUTHOR 14
#define VIEWBOOKPUB 15
#define VIEWBOOKPUBDATE 16
#define VIEWBOOKOUT 17


//menu/window related
#define STATUSSIZE 30
#define BKCOLOR "White"
#define FRAMECOLOR "Blue"
#define MAINFRAME 1
#define SECFRAME 2
#define THIRDFRAME 3
#define	FOURTHFRAME 4
#define FRAME(n) n
#define MENU(n) n-1
#define OPT(n) n


//AccountInfo related
#define PASSWORDSIZE 10
#define ACCIDUPPER 99999
#define ACCIDLOWER 0

//BookInfo related
#define BOOKIDUPPER 99999
#define BOOKIDLOWER 0
#define BOOKNAME 20
#define NUMOFKEY 5
#define NUMOFAUTHOR 3

//

struct menuControl
{
	int menuSelection;
	int optSelection;
	bool edited;
	int frameLvl;
	int priority;
	char status[STATUSSIZE];
	int buttonPressed;
	int fun;
};

typedef struct AccountInfo;

struct UserInfo
{
	char userName[LONGNAME];
	char gender;
	char work[LONGNAME];
	struct AccountInfo * acc;
};

struct AccountInfo
{
	char accName[SHORTNAME];
	char pwd[PASSWORDSIZE];
	int accId;
	struct UserInfo * usr;
	int booksBorrowed;
};

struct BookInfo
{
	int bookId;
	char bookName[LONGNAME];
	char keywords[NUMOFKEY][LONGNAME];
	char authors[NUMOFAUTHOR][LONGNAME];
	char publisher[LONGNAME];
	char pubDate[8];
	bool borrowed;
};

struct Record
{
	struct AccountInfo* acc;
	struct BookInfo* book;
	int borrowDate;
	int returnDate;
};

struct AccList
{
	struct AccountInfo* acc;
	struct AccList* next;
	struct AccList* last;
};

struct BookList
{
	struct BookInfo* book;
	struct BookList* next;
	struct BookList* last;
};

struct RecordList
{
	struct Record* record;
	struct RecordList* next;
	struct RecordList* last;
};

struct AccArr
{
	int n;
	int max;
	struct AccountInfo accs[100];
};

struct BookArr
{
	int n;
	int max;
	struct BookInfo books[100];
};

struct RecordArr
{
	int n;
	int max;
	struct Record records[100];
};


struct Library
{
	struct RecordList* records;
	struct BookList* books;
	struct AccList* accs;
};

struct winFrame
{
	double x;
	double y;
	double w;
	double h;
};
#endif