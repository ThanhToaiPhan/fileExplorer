// TP04.cpp : Defines the entry point for the application.
//

#include "stdafx.h"

#include "TP04.h"
using namespace std;
#define lpnm   ((LPNMHDR)lParam)
#define lpnmTB ((LPNMTOOLBAR)lParam)

#define MAX_LOADSTRING 100
#define IDC_TREEVIEW 5000
#define IDC_TOOLBAR 5001
#define IDC_IMAGELIST 5002
#define IDB_ADD 5003
#define IDB_REFRESH 5004
#define IDB_HOME 5005
#define IDC_EDITBOX 5006
#define IDC_BUTTON 5007
#define IDC_LISTVIEW 5008
#define IDB_VIEW 5009
#define IDR_POPUP 5010
#define IDMENU_Details 5011
#define IDMENU_Icon 5012
#define IDMENU_List 5013
#define IDMENU_Tile 5014
#define IDB_UNDO 5015
#define IDB_REUNDO 5016
#define IDMENU_COPY 5017
#define IDMENU_PASTE 5018
#define IDMENU_DETELE 5019
#define IDMENU_CUT 5020
#define IDMENU_RENAME 5021
#define IDR_RICLKPOPUP 5022
#define IDB_FAVOR 5023
#define IDB_SHOWFAVOR 5024
// FAVORITES
#define IDMENU_FAVOR0 5025
#define IDMENU_FAVOR1 5026
#define IDMENU_FAVOR2 5027
#define IDMENU_FAVOR3 5028
#define IDMENU_FAVOR4 5029
#define IDMENU_FAVOR5 5030
#define IDMENU_FAVOR6 5031
#define IDMENU_FAVOR7 5032
#define IDMENU_FAVOR8 5033
#define IDMENU_FAVOR9 5034
#define IDC_BUTTON_SLIDESHOW 5035
#define ID_TIMER 5036
#define IDC_COMBOBOX 5037
#define IDMENU_Thumbnail 5038
#define IDMENU_OPEN 5039
#define IDC_SEARCH 5040
#define IDC_SEARCHBUTTON 5041
#define IDC_LISTVIEWSEARCH 5042


typedef struct FOLDER
{
	TCHAR name[100];
	TCHAR size[100];
	TCHAR type[100];
	TCHAR date[100];
};
int nfolder = 0;
FOLDER folder[1000];

INT DirectoryListingTV(TCHAR _szrDir[],HTREEITEM Root,HWND hWnd);

INT DirectoryListingLV(TCHAR _szrDir[],HWND hWnd);

VOID CollapseNode( HWND hTree, HTREEITEM hti );

VOID CollapseAllChildren(HWND treeWnd, HTREEITEM parent);

BOOL DirExists(TCHAR* path);

INT SplitPath(TCHAR *buffer, TCHAR **rslt, TCHAR delimenter);

BOOL GoToPath(TCHAR *path);

wchar_t *type(WIN32_FIND_DATA data);

INT SendToRecycleBin(TCHAR newpath[]); 

HTREEITEM GetHandleTV(TCHAR *path);

VOID UpDateRenameTV(HTREEITEM hCur,  TCHAR *path,TCHAR *name);

VOID UpDateTV(HTREEITEM hCur, TCHAR *path);

INT CALLBACK SortListViewFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);

VOID NotifyListView(LPNMLISTVIEW pNMLV);

HBITMAP GetThumbnail(PTSTR File);

INT ThumbnailListView(TCHAR _szrDir[],HWND hWnd);

BOOL GetImage(TCHAR Somewhere[], Gdiplus::Image* image[]);

VOID Draw(HWND hWndDraw, Gdiplus::Image* image);

LRESULT WndProcListView(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

VOID Search(TCHAR* pathNow, TCHAR* fileSearch);

BOOL KnuthMorrisPratt(TCHAR* Str1, TCHAR* Str2);

INT IsFolder(TCHAR* path);

VOID DeleteTVExits(HTREEITEM hCur);

// Global Variables:
HWND hWnd;
HWND hWndTreeView , hWndToolBar, hWndEditBox, hWndButton, hwndListView, hWndSlideShow, hWndButtonSlideShow;
HWND hwndView, hWndEditName, hWndSeconds, hWndSearchBox, hWndButtonSearch, hwndListViewSearch;
int iPos = -1;
WNDPROC winListView;
const CLSID CLSID_ImageThumbnailProvider = {0xC7657C4A, 0x9F68, 0x40fa, {0xA4, 0xDF, 0x96, 0xBC, 0x08, 0xEB, 0x35, 0x51}};
TV_INSERTSTRUCT tvinsert;
HTREEITEM Parent = NULL;           // Tree item handle
HTREEITEM Before;           // .......          
HTREEITEM Selected;
HINSTANCE hInst;
HIMAGELIST hImageListToolbar, hImageListView, hImageListTree, hImageListSearch, hImageListButtonGo, hImageListButtonSearch, hImageListThumbnail;
BUTTON_IMAGELIST ButtonGoImage, ButtonSearchImage;
HICON hIconAdd, hIconRefresh, hIconHome, hIconFavor, hIconShowFavor;
HICON hIconDisk, hIconFolder, hIconComputer;
HICON hIconGo, hIconItem, hIconUndo, hIconReundo, hIconSearch;
HICON hIconView, hIconLink;
HICON fileIcon[100];
int isCtrlDown = 0;
int nItemSelect;
int nFavor = 0;	
int checked = 0;
int nClick = -1;										//Use for button undo;
TCHAR* pathUndo[MAX_LOADSTRING], *pathFavor[10];				//
TCHAR pathCopy[MAX_LOADSTRING], pathPaste[MAX_LOADSTRING], fileName[MAX_LOADSTRING], TempFile[MAX_LOADSTRING];
TCHAR pathDelete[MAX_LOADSTRING],pathCut[MAX_LOADSTRING], pathNameExist[MAX_LOADSTRING], pathRename[MAX_LOADSTRING],reName[MAX_LOADSTRING];
TCHAR pathTemp[MAX_LOADSTRING], pathforUndo[MAX_LOADSTRING];
TCHAR pathLink[1000][MAX_LOADSTRING];
int cutChecked = 0;
int id_add, id_refresh, id_home, id_disk, id_computer, id_folder, id_go, id_view, id_undo, id_reundo;
int id_copy, id_paste, id_delete, id_cut, id_favor, id_showfavor, id_search, id_link;
int file[100], fileThumb[100];
TBBUTTON tbButton[10];
HTREEITEM myComputer;
SIZE size = {150, 75};
LVTILEVIEWINFO tileViewInfo = {0};
LVTILEINFO lvti = {0};
LVITEM lvI;
int order[3];
RECT rc, rcSubItem;
WNDPROC wpOrigEditProc;
int bEditing = 0;
HMENU hMenu, hSubMenu;
static BOOL isAscending = TRUE;
Gdiplus::GdiplusStartupInput gdiplusstartupinput;
ULONG_PTR gdiplusToken;
int index, numImage = 0;
PAINTSTRUCT ps;
Gdiplus::Image* img[1000];
BOOL isThumbnailling = FALSE;
BOOL Search_OK;
int numSearchResult;

// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];
TCHAR CurrentDir[MAX_LOADSTRING];
TCHAR Mask[MAX_LOADSTRING];
TCHAR Temp[MAX_LOADSTRING];
TCHAR listview_name[MAX_LOADSTRING];

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusstartupinput, NULL);
	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_TP04, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TP04));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	Gdiplus::GdiplusShutdown(gdiplusToken);
	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TP04));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//


BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	// HWND hWnd;
	hInst = hInstance; // Store instance handle in our global variable
	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPED|WS_SYSMENU,
		40, 40, 1150, 678, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}

	// TOOLBAR----------------------------------------------------------------------
	hWndToolBar = CreateWindow(TOOLBARCLASSNAME, _T("ToolBar"), WS_CHILD|WS_VISIBLE|TBSTYLE_WRAPABLE,
		0, 0, 10, 10, hWnd, (HMENU)IDC_TOOLBAR, hInst, NULL);
	hImageListToolbar = ImageList_Create(16, 16, ILC_COLOR32|ILC_MASK, 3, 10);
	hIconAdd = LoadIcon(hInst, MAKEINTRESOURCE(137));
	id_add = ImageList_AddIcon(hImageListToolbar, hIconAdd);
	hIconRefresh = LoadIcon(hInst, MAKEINTRESOURCE(138));
	id_refresh = ImageList_AddIcon(hImageListToolbar, hIconRefresh);
	hIconHome = LoadIcon(hInst, MAKEINTRESOURCE(144));
	id_home = ImageList_AddIcon(hImageListToolbar, hIconHome);
	hIconView = LoadIcon(hInst,MAKEINTRESOURCE(146));
	id_view = ImageList_AddIcon(hImageListToolbar,hIconView);
	hIconUndo = LoadIcon(hInst,MAKEINTRESOURCE(149));
	id_undo = ImageList_AddIcon(hImageListToolbar,hIconUndo);
	hIconReundo = LoadIcon(hInst,MAKEINTRESOURCE(148));
	id_reundo = ImageList_AddIcon(hImageListToolbar,hIconReundo);
	hIconFavor = LoadIcon(hInst, MAKEINTRESOURCE(186));
	id_favor = ImageList_AddIcon(hImageListToolbar, hIconFavor);
	hIconShowFavor = LoadIcon(hInst, MAKEINTRESOURCE(187));
	id_showfavor = ImageList_AddIcon(hImageListToolbar, hIconShowFavor);

	SendMessage(hWndToolBar, TB_SETIMAGELIST, (WPARAM)0, (LPARAM)hImageListToolbar);
	SendMessage(hWndToolBar, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);

	tbButton[0].fsState = TBSTATE_ENABLED;
	tbButton[0].fsStyle = TBSTYLE_BUTTON;
	tbButton[0].iBitmap = id_add;
	tbButton[0].idCommand = IDB_ADD;

	tbButton[1].fsState = TBSTATE_ENABLED;
	tbButton[1].fsStyle = TBSTYLE_BUTTON;
	tbButton[1].iBitmap = id_refresh;
	tbButton[1].idCommand = IDB_REFRESH;

	tbButton[2].fsState = TBSTATE_ENABLED;
	tbButton[2].fsStyle = TBSTYLE_BUTTON;
	tbButton[2].iBitmap = id_home;
	tbButton[2].idCommand = IDB_HOME;

	tbButton[3].fsState = TBSTATE_ENABLED;
	tbButton[3].fsStyle =  TBSTYLE_BUTTON;
	tbButton[3].iBitmap = id_view;
	tbButton[3].idCommand = IDB_VIEW;

	tbButton[4].fsState = TBSTATE_ENABLED;
	tbButton[4].fsStyle = TBSTYLE_BUTTON;
	tbButton[4].iBitmap = id_undo;
	tbButton[4].idCommand = IDB_UNDO;

	tbButton[5].fsState = TBSTATE_ENABLED;
	tbButton[5].fsStyle = TBSTYLE_BUTTON;
	tbButton[5].iBitmap = id_reundo;
	tbButton[5].idCommand = IDB_REUNDO;

	tbButton[6].fsState = TBSTATE_ENABLED;
	tbButton[6].fsStyle = TBSTYLE_BUTTON;
	tbButton[6].iBitmap = id_favor;
	tbButton[6].idCommand = IDB_FAVOR;

	tbButton[7].fsState = TBSTATE_ENABLED;
	tbButton[7].fsStyle = TBSTYLE_BUTTON;
	tbButton[7].iBitmap = id_showfavor;
	tbButton[7].idCommand = IDB_SHOWFAVOR;

	SendMessage(hWndToolBar, TB_ADDBUTTONS, (WPARAM)8, (LPARAM)&tbButton);

	// TREEVIEW----------------------------------------------------------------------
	InitCommonControls();
	hWndTreeView = CreateWindow(WC_TREEVIEW, _T("TreeView"), WS_VISIBLE|WS_CHILD|WS_BORDER|
		TVS_HASLINES|TVS_HASBUTTONS|TVS_LINESATROOT
		,0, 50, 300, 600, hWnd, (HMENU)IDC_TREEVIEW, hInst, NULL);
	hImageListTree = ImageList_Create(16, 16, ILC_COLOR32|ILC_MASK, 100, 100);

	hIconComputer = LoadIcon(hInst, MAKEINTRESOURCE(145));
	id_computer = ImageList_AddIcon(hImageListTree, hIconComputer);
	hIconDisk = LoadIcon(hInst, MAKEINTRESOURCE(140));
	id_disk = ImageList_AddIcon(hImageListTree, hIconDisk);
	hIconFolder = LoadIcon(hInst, MAKEINTRESOURCE(141));
	id_folder = ImageList_AddIcon(hImageListTree, hIconFolder);
	SendDlgItemMessage(hWnd, IDC_TREEVIEW, TVM_SETIMAGELIST, (WPARAM)0, (LPARAM)hImageListTree);

	wcscpy_s(CurrentDir,_T(""));
	tvinsert.item.pszText = _T("Computer");
	tvinsert.hParent = myComputer;
	tvinsert.item.mask=TVIF_TEXT|TVIF_IMAGE|TVIF_SELECTEDIMAGE; 
	tvinsert.hInsertAfter = TVI_ROOT;
	tvinsert.item.iImage = id_computer;
	tvinsert.item.iSelectedImage = id_computer;
	myComputer=(HTREEITEM)SendDlgItemMessage(hWnd,IDC_TREEVIEW,
		TVM_INSERTITEM,0,(LPARAM)&tvinsert);

	DirectoryListingTV(CurrentDir,myComputer,hWnd);

	//LISTVIEW----------------------------------------------------------------------
	hwndListView = CreateWindow(WC_LISTVIEW, _T(""), WS_CHILD|WS_VISIBLE|WS_BORDER|LVS_REPORT|LVS_EDITLABELS,
		300, 50, 593, 600,
		hWnd, (HMENU)IDC_LISTVIEW, hInst, NULL);
	hImageListView = ImageList_Create(32, 32, ILC_COLOR32|ILC_MASK, 100, 100);
	hImageListThumbnail = ImageList_Create(100, 100, ILC_COLOR32|ILC_MASK, 1000, 100);
	for (int i = 0; i <= 30; i++)
	{
		fileIcon[i] = LoadIcon(hInst, MAKEINTRESOURCE(150 + i));
		file[i] = ImageList_AddIcon(hImageListView, fileIcon[i]);
	}
	for (int i = 0; i <= 30; i++)
	{
		fileIcon[i] = LoadIcon(hInst, MAKEINTRESOURCE(150 + i));
		fileThumb[i] = ImageList_AddIcon(hImageListThumbnail, fileIcon[i]);
	}
	fileIcon[31] = LoadIcon(hInst, MAKEINTRESOURCE(140));
	file[31] = ImageList_AddIcon(hImageListView, fileIcon[31]);
	fileIcon[32] = LoadIcon(hInst, MAKEINTRESOURCE(141));
	file[32] = ImageList_AddIcon(hImageListView, fileIcon[32]);
	fileIcon[33] = LoadIcon(hInst, MAKEINTRESOURCE(182));
	file[33] = ImageList_AddIcon(hImageListView, fileIcon[33]);
	fileIcon[34] = LoadIcon(hInst, MAKEINTRESOURCE(183));
	file[34] = ImageList_AddIcon(hImageListView, fileIcon[34]);
	fileIcon[35] = LoadIcon(hInst, MAKEINTRESOURCE(185));
	file[35] = ImageList_AddIcon(hImageListView, fileIcon[35]);

	fileIcon[31] = LoadIcon(hInst, MAKEINTRESOURCE(140));
	fileThumb[31] = ImageList_AddIcon(hImageListThumbnail, fileIcon[31]);
	fileIcon[32] = LoadIcon(hInst, MAKEINTRESOURCE(141));
	fileThumb[32] = ImageList_AddIcon(hImageListThumbnail, fileIcon[32]);
	fileIcon[33] = LoadIcon(hInst, MAKEINTRESOURCE(182));
	fileThumb[33] = ImageList_AddIcon(hImageListThumbnail, fileIcon[33]);
	fileIcon[34] = LoadIcon(hInst, MAKEINTRESOURCE(183));
	fileThumb[34] = ImageList_AddIcon(hImageListThumbnail, fileIcon[34]);
	fileIcon[35] = LoadIcon(hInst, MAKEINTRESOURCE(185));
	fileThumb[35] = ImageList_AddIcon(hImageListThumbnail, fileIcon[35]);

	ListView_SetImageList(hwndListView, hImageListView, LVSIL_SMALL);
	LVCOLUMN lvc;
	int iCol;
	lvc.mask = LVCF_FMT|LVCF_WIDTH|LVCF_TEXT|LVCF_SUBITEM;
	lvc.cx = 150;
	for (iCol = 0; iCol < 4; iCol++)
	{
		lvc.iSubItem = iCol;
		switch(iCol)
		{
		case 0:
			lvc.pszText = _T("Name");
			break;
		case 1:
			lvc.pszText = _T("Size");
			break;
		case 2:
			lvc.pszText = _T("Type");
			break;
		case 3:
			lvc.pszText = _T("Date");
			break;
		}
		if (iCol < 1)
		{
			lvc.fmt = LVCFMT_LEFT;
		}
		else
		{
			lvc.fmt = LVCFMT_LEFT;
		}
		if (ListView_InsertColumn(hwndListView, iCol, &lvc) == -1)
		{
			return FALSE;
		}
	}
	winListView =(WNDPROC)SetWindowLong(hwndListView,GWLP_WNDPROC, (LONG)WndProcListView);

	//ADDRESS----------------------------------------------------------------------
	hWndEditBox = CreateWindow(WC_EDIT, _T(""), WS_VISIBLE|WS_CHILD|WS_BORDER,
		0, 25, 764, 24, hWnd, (HMENU)IDC_EDITBOX, hInst, NULL);
	CoInitialize(0);
	SHAutoComplete(hWndEditBox,SHACF_FILESYS_DIRS|SHACF_USETAB);
	CoUninitialize();
	hWndButton = CreateWindow(WC_BUTTON, _T(""), WS_VISIBLE|WS_CHILD|BS_PUSHBUTTON|BS_ICON,
		764, 25, 30, 25, hWnd, (HMENU)IDC_BUTTON, hInst, NULL);

	hImageListButtonGo = ImageList_Create(16, 16, ILC_COLOR32|ILC_MASK, 1, 10);
	hIconGo = LoadIcon(hInst, MAKEINTRESOURCE(142));
	ImageList_AddIcon(hImageListButtonGo, hIconGo);

	ButtonGoImage.himl = hImageListButtonGo;
	ButtonGoImage.uAlign = BUTTON_IMAGELIST_ALIGN_CENTER;
	Button_SetImageList(hWndButton, &ButtonGoImage);

	//SLIDESHOW----------------------------------------------------------------------
	hWndSlideShow = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPED|WS_SYSMENU,
		100, 100, 700, 500, NULL, NULL, hInstance, NULL);
	hWndButtonSlideShow = CreateWindow(WC_BUTTON, _T("Show Image"), WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,
		793, 25, 100, 25, hWnd, (HMENU)IDC_BUTTON_SLIDESHOW, hInst, NULL);
	hWndSeconds = CreateWindow(WC_COMBOBOX, _T(""), WS_CHILD|WS_VISIBLE|CBS_DROPDOWNLIST,
		550, 50, 100, 10, hWndSlideShow, (HMENU)IDC_COMBOBOX, hInst, NULL);
	SendMessage(hWndSeconds, CB_ADDSTRING, (WPARAM)0, (LPARAM)_T("1 second"));
	SendMessage(hWndSeconds, CB_ADDSTRING, (WPARAM)1, (LPARAM)_T("2 seconds"));
	SendMessage(hWndSeconds, CB_ADDSTRING, (WPARAM)2, (LPARAM)_T("3 seconds"));
	SendMessage(hWndSeconds, CB_ADDSTRING, (WPARAM)3, (LPARAM)_T("4 seconds"));
	SendMessage(hWndSeconds, CB_SETCURSEL, 0, 0);
	SetTimer(hWndSlideShow, ID_TIMER, 1000, (TIMERPROC)NULL);

	//SEARCH----------------------------------------------------------------------
	hwndListViewSearch = CreateWindow(WC_LISTVIEW, _T(""), WS_CHILD|WS_VISIBLE|WS_BORDER|LVS_LIST,
		893, 50, 251, 600,
		hWnd, (HMENU)IDC_LISTVIEW, hInst, NULL);
	hWndSearchBox = CreateWindow(WC_EDIT, _T(""), WS_VISIBLE|WS_CHILD|WS_BORDER,
		893, 25, 222, 24, hWnd, (HMENU)IDC_SEARCH, hInst, NULL);
	hWndButtonSearch = CreateWindow(WC_BUTTON, _T(""), WS_VISIBLE|WS_CHILD|BS_PUSHBUTTON|BS_ICON,
		1115, 25, 30, 25, hWnd, (HMENU)IDC_SEARCHBUTTON, hInst, NULL);
	hImageListButtonSearch = ImageList_Create(16, 16, ILC_COLOR32|ILC_MASK, 1, 10);
	hImageListSearch = ImageList_Create(16, 16,  ILC_COLOR32|ILC_MASK, 1, 10);
	hIconLink = LoadIcon(hInst, MAKEINTRESOURCE(189));
	id_link = ImageList_AddIcon(hImageListSearch, hIconLink);
	ListView_SetImageList(hwndListViewSearch, hImageListSearch, LVSIL_SMALL);
	hIconSearch = LoadIcon(hInst, MAKEINTRESOURCE(188));
	ImageList_AddIcon(hImageListButtonSearch, hIconSearch);
	ButtonSearchImage.himl = hImageListButtonSearch;
	ButtonSearchImage.uAlign = BUTTON_IMAGELIST_ALIGN_CENTER;
	Button_SetImageList(hWndButtonSearch, &ButtonSearchImage);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	HDC hdc;
	int num = 1;
	char count[10];
	WCHAR folder[10];
	TCHAR folderAdd[MAX_LOADSTRING];
	TCHAR itemName[MAX_PATH];
	HMENU hPopupMenu;
	HMENU hRightPop;
	POINT cursor , rightCursor;
	nItemSelect = ListView_GetItemCount(hwndListView);
	int itemIndex;
	RECT rc = {0};
	GetWindowRect(hWndSlideShow, &rc);
	rc.left = 0;
	rc.top = 0;
	rc.right = 500;
	rc.bottom = 500;
	DWORD  CharCount            = 0;
	WCHAR  DeviceName[MAX_PATH] = L"";
	DWORD  Error                = ERROR_SUCCESS;
	HANDLE FindHandle           = INVALID_HANDLE_VALUE;
	BOOL   Found                = FALSE;
	size_t Index                = 0;
	BOOL   Success              = FALSE;
	WCHAR  VolumeName[MAX_PATH] = L"";
	LVITEM itemSearch;
	itemSearch.mask = LVIF_TEXT|LVIF_IMAGE|LVIF_STATE;
	itemSearch.stateMask = 0;
	itemSearch.state = 0;


	switch (message)
	{
	case WM_TIMER:
		if (index == (numImage - 1))
		{
			index = 0;
		}
		else
		{
			index++;
		}
		InvalidateRect(hWndSlideShow, &rc, TRUE);
		Draw(hWndSlideShow, img[index]);
		UpdateWindow(hWndSlideShow);
		break;
	case WM_NOTIFY:
		if (LOWORD(wParam) == IDC_LISTVIEW)
		{
			NotifyListView((LPNMLISTVIEW)lParam);
		}
		if(((LPNMHDR)lParam)->hwndFrom == hWndTreeView)
		{
			if (((NMHDR*)lParam)->code == NM_DBLCLK || ((NMHDR*)lParam)->code == TVN_SELCHANGED)
			{
				checked = 2;
				if (((NMHDR*)lParam)->code == NM_DBLCLK && TreeView_GetCount(hWndTreeView) >0)
				{
					checked = 1;
					nClick ++;
				}
				Selected = TreeView_GetSelection(hWndTreeView);
				TCHAR buffer[200];
				HTREEITEM cha;
				TVITEM item, itemcha;
				item.hItem = Selected;
				item.mask = TVIF_TEXT;
				item.cchTextMax = 200;
				item.pszText = buffer;
				TreeView_GetItem(hWndTreeView, &item);
				wcscpy_s(Mask,_T(""));
				wcscpy_s(CurrentDir,item.pszText);
				cha = TreeView_GetParent(hWndTreeView,item.hItem);
				while (cha != NULL)
				{
					itemcha.hItem = cha;
					itemcha.mask = TVIF_TEXT;
					itemcha.cchTextMax = 200;
					itemcha.pszText = buffer;
					TreeView_GetItem(hWndTreeView, &itemcha);
					wcscpy_s(buffer, itemcha.pszText);
					if (wcscmp(buffer, _T("Computer")) == 0) 
					{
						break;
					}
					wcscat_s(buffer,_T("\\"));
					wcscat_s(buffer, CurrentDir);
					wcscpy_s(CurrentDir, buffer);
					cha = TreeView_GetParent(hWndTreeView,itemcha.hItem);
				}
				if (checked == 1 && (TreeView_GetChild(hWndTreeView, Selected) == NULL))
				{
					DirectoryListingTV(CurrentDir, Selected, hWnd);
				}
				if (checked == 2 && (TreeView_GetChild(hWndTreeView, Selected) == NULL))
				{
					ListView_DeleteAllItems(hwndListView);
					if (isThumbnailling)
					{
						ThumbnailListView(CurrentDir, hWnd);
					}
					else
					{
						DirectoryListingLV(CurrentDir, hWnd);
					}
				}
				//-----ListView cho nay ------------------------------
				if (TreeView_GetChild(hWndTreeView, Selected) != NULL)
				{
					ListView_DeleteAllItems(hwndListView);
					if (isThumbnailling)
					{
						ThumbnailListView(CurrentDir, hWnd);
					}
					else
					{
						DirectoryListingLV(CurrentDir, hWnd);
					}
				}

				if (wcscmp(CurrentDir, _T("Computer")) != 0)
				{
					SetWindowText(hWndEditBox, CurrentDir);
				}
				else
				{
					wcscpy(CurrentDir,_T(""));
					SetWindowText(hWndEditBox, CurrentDir);
				}
				if(((NMHDR*)lParam)->code == NM_DBLCLK && TreeView_GetCount(hWndTreeView) >0)
				{
					pathUndo[nClick] =new TCHAR[MAX_LOADSTRING];
					wcscpy(pathUndo[nClick],CurrentDir);
				}
			}
		}
		//--------Chon Item ListView
		if(((LPNMHDR)lParam)->hwndFrom == hwndListViewSearch)
		{
			if (((LPNMHDR)lParam)->code == NM_DBLCLK)
			{
				nClick ++;
				TCHAR buffer[MAX_LOADSTRING];
				iPos = ListView_GetNextItem(hwndListViewSearch, -1, LVNI_SELECTED);
				if (iPos == -1)
				{
					break;
				}
				lvI.iItem = iPos;
				lvI.iSubItem = 0;
				lvI.cchTextMax = MAX_LOADSTRING;
				lvI.pszText  = buffer;
				lvI.mask = LVIF_TEXT|LVIF_IMAGE|LVIF_STATE;
				ListView_GetItem(hwndListViewSearch,&lvI);
				wcscpy(CurrentDir,lvI.pszText);
				for (int i = wcslen(CurrentDir); i > 0; i--)
				{
					if (CurrentDir[i] == '\\')
					{
						CurrentDir[i] = NULL;
						break;
					}
				}
				GoToPath(CurrentDir);
				pathUndo[nClick] =new TCHAR[MAX_LOADSTRING];
				wcscpy(pathUndo[nClick],CurrentDir);
			}
		}
		if(((LPNMHDR)lParam)->hwndFrom == hwndListView)
		{
			if (((LPNMHDR)lParam)->code == NM_DBLCLK)
			{
				nClick ++;
				TCHAR buffer[MAX_LOADSTRING];
				iPos = ListView_GetNextItem(hwndListView, -1, LVNI_SELECTED);
				if (iPos == -1)
				{
					break;
				}
				lvI.iItem = iPos;
				lvI.iSubItem = 0;
				lvI.cchTextMax = MAX_LOADSTRING;
				lvI.pszText  = buffer;
				lvI.mask = LVIF_TEXT|LVIF_IMAGE|LVIF_STATE;
				ListView_GetItem(hwndListView,&lvI);

				wcscpy(Temp,lvI.pszText);
				wcscat_s(CurrentDir,_T("\\"));
				wcscat_s(CurrentDir,Temp);
				GoToPath(CurrentDir);
				pathUndo[nClick] =new TCHAR[MAX_LOADSTRING];
				wcscpy(pathUndo[nClick],CurrentDir);
			}
			if(((LPNMHDR)lParam)->code == NM_CLICK && nItemSelect > 0)
			{

				TCHAR buffer2[MAX_LOADSTRING];
				iPos = ListView_GetNextItem(hwndListView, -1, LVNI_SELECTED);

				lvI.iItem = iPos;
				lvI.iSubItem = 0;
				lvI.cchTextMax = MAX_LOADSTRING;
				lvI.pszText  = buffer2;
				lvI.mask = LVIF_TEXT|LVIF_IMAGE|LVIF_STATE;
				ListView_GetItem(hwndListView,&lvI);

				wcscpy(fileName,lvI.pszText);
			}
			if(((LPNMHDR)lParam)->code == NM_RCLICK && nItemSelect > 0)
			{

				TCHAR buffer2[MAX_LOADSTRING];
				iPos = ListView_GetNextItem(hwndListView, -1, LVNI_SELECTED);

				lvI.iItem = iPos;
				lvI.iSubItem = 0;
				lvI.cchTextMax = MAX_LOADSTRING;
				lvI.pszText  = buffer2;
				lvI.mask = LVIF_TEXT|LVIF_IMAGE|LVIF_STATE;
				ListView_GetItem(hwndListView,&lvI);

				wcscpy(fileName,lvI.pszText);

				//popup right click menu
				GetCursorPos(&rightCursor);
				hRightPop = CreatePopupMenu();
				InsertMenu(hRightPop, 0, MF_BYPOSITION|MF_STRING, IDMENU_RENAME, _T("Rename"));
				InsertMenu(hRightPop, 0, MF_BYPOSITION|MF_STRING, IDMENU_DETELE, _T("Delete"));
				InsertMenu(hRightPop, 0, MF_BYPOSITION|MF_STRING, IDMENU_PASTE, _T("Paste"));
				InsertMenu(hRightPop, 0, MF_BYPOSITION|MF_STRING, IDMENU_COPY, _T("Copy"));
				InsertMenu(hRightPop, 0, MF_BYPOSITION|MF_STRING, IDMENU_CUT, _T("Cut"));
				InsertMenu(hRightPop, 0, MF_BYPOSITION|MF_STRING, IDMENU_OPEN, _T("Open"));
				SetForegroundWindow(hWnd);
				TrackPopupMenu(hRightPop, TPM_TOPALIGN|TPM_LEFTALIGN, rightCursor.x, rightCursor.y, 0, hWnd, NULL);
				DestroyMenu(hRightPop);
			}
			if(((LPNMHDR)lParam)->code == NM_RCLICK && (nItemSelect == 0))
			{
				GetCursorPos(&rightCursor);
				hRightPop = CreatePopupMenu();
				InsertMenu(hRightPop, 0, MF_BYPOSITION|MF_STRING, IDMENU_PASTE, _T("Paste"));
				SetForegroundWindow(hWnd);
				TrackPopupMenu(hRightPop, TPM_TOPALIGN|TPM_LEFTALIGN, rightCursor.x, rightCursor.y, 0, hWnd, NULL);
				DestroyMenu(hRightPop);
			}
			if(((LPNMHDR)lParam)->code == LVN_BEGINLABELEDIT)
			{
				hWndEditName = ListView_GetEditControl(hwndListView);

			}
			if(((LPNMHDR)lParam)->code == LVN_ENDLABELEDIT)
			{		
				GetWindowText(hWndEditName,reName,MAX_PATH);

				// lay ten pathfile cu
				wcscpy(TempFile,CurrentDir);
				wcscat(CurrentDir,_T("\\"));
				wcscat(CurrentDir,fileName);
				wcscpy(pathNameExist,CurrentDir);
				wcscpy(CurrentDir,TempFile);
				wcscpy(TempFile,fileName);

				//lay pathfile moi
				wcscpy(TempFile,CurrentDir);
				wcscat(CurrentDir,_T("\\"));
				wcscat(CurrentDir,reName);
				wcscpy(pathRename,CurrentDir);
				wcscpy(CurrentDir,TempFile);
				wcscpy(TempFile,fileName);

				HTREEITEM checkedExist;
				checkedExist = GetHandleTV(pathRename);
				if(checkedExist != NULL)
				{
					MessageBox(hWnd, L"File has existed", L"ERROR", 0);
					break;
				}
				else
				{
					//Dung shel Rename
					SHFILEOPSTRUCT rename ;
					rename.hwnd = hWnd;
					rename.wFunc = FO_RENAME;
					rename.pFrom = pathNameExist;
					rename.pTo = pathRename;
					rename.fFlags = FOF_SILENT;
					SHFileOperation( &rename );
					// thay doi tren treeview VA LISTEVIEW
					HTREEITEM NewName;
					NewName = GetHandleTV(pathNameExist);
					UpDateRenameTV(NewName,CurrentDir,reName);
					ListView_DeleteAllItems(hwndListView);
					if (isThumbnailling)
					{
						ThumbnailListView(CurrentDir, hWnd);
					}
					else
					{
						DirectoryListingLV(CurrentDir, hWnd);
					}
				}
			}
		}
		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		if (wmEvent == CBN_SELCHANGE)
		{
			itemIndex = ComboBox_GetCurSel(hWndSeconds);
			if (itemIndex == 0)
			{
				KillTimer(hWndSlideShow, ID_TIMER);
				SetTimer(hWndSlideShow, ID_TIMER, 1000, (TIMERPROC)NULL);
			}
			if (itemIndex == 1)
			{
				KillTimer(hWndSlideShow, ID_TIMER);
				SetTimer(hWndSlideShow, ID_TIMER, 2000, (TIMERPROC)NULL);
			}
			if (itemIndex == 2)
			{
				KillTimer(hWndSlideShow, ID_TIMER);
				SetTimer(hWndSlideShow, ID_TIMER, 3000, (TIMERPROC)NULL);
			}
			if (itemIndex == 3)
			{
				KillTimer(hWndSlideShow, ID_TIMER);
				SetTimer(hWndSlideShow, ID_TIMER, 4000, (TIMERPROC)NULL);
			}
		}
		switch(wParam)
		{
		case IDMENU_Details:
			isThumbnailling = FALSE;
			ListView_DeleteAllItems(hwndListView);
			ListView_SetView(hwndListView, LV_VIEW_DETAILS);
			DirectoryListingLV(CurrentDir, hWnd);
			break;
		case IDMENU_Icon:
			isThumbnailling = FALSE;
			ListView_DeleteAllItems(hwndListView);
			ListView_SetImageList(hwndListView, hImageListView, LVSIL_NORMAL);
			ListView_SetView(hwndListView, LV_VIEW_ICON);
			DirectoryListingLV(CurrentDir, hWnd);
			break;
		case IDMENU_List:
			isThumbnailling = FALSE;
			ListView_DeleteAllItems(hwndListView);
			ListView_SetView(hwndListView, LV_VIEW_LIST);
			DirectoryListingLV(CurrentDir, hWnd);
			break;
		case IDMENU_Tile:
			isThumbnailling = FALSE;
			ListView_DeleteAllItems(hwndListView);
			ListView_SetImageList(hwndListView, hImageListView, LVSIL_NORMAL);
			tileViewInfo.cbSize   = sizeof(tileViewInfo);
			tileViewInfo.dwFlags  = LVTVIF_FIXEDSIZE;
			tileViewInfo.dwMask   = LVTVIM_COLUMNS|LVTVIM_TILESIZE;
			tileViewInfo.cLines   = 4;
			tileViewInfo.sizeTile = size;
			ListView_SetTileViewInfo(hwndListView, &tileViewInfo);
			ListView_SetView(hwndListView, LV_VIEW_TILE);
			DirectoryListingLV(CurrentDir, hWnd);
			break;
		case IDMENU_Thumbnail:
			isThumbnailling = TRUE;
			ListView_DeleteAllItems(hwndListView);
			ListView_SetImageList(hwndListView, hImageListThumbnail, LVSIL_NORMAL);
			ListView_SetView(hwndListView, LV_VIEW_ICON);
			ThumbnailListView(CurrentDir, hWnd);
			break;
		case IDMENU_COPY:
			cutChecked = 0 ;
			wcscpy(TempFile,CurrentDir);
			wcscat(CurrentDir,_T("\\"));
			wcscat(CurrentDir,fileName);
			wcscpy(pathCopy,CurrentDir);
			wcscpy(CurrentDir,TempFile);
			wcscpy(TempFile,fileName);
			wcscpy(pathTemp,pathCopy);
			wcscpy(pathforUndo,CurrentDir);
			break;
		case IDMENU_PASTE:
			nClick = 1;
			pathUndo[nClick-1] = new TCHAR[MAX_LOADSTRING];
			pathUndo[nClick] = new TCHAR[MAX_LOADSTRING];
			wcscpy(pathPaste,CurrentDir);
			wcscat(pathPaste,_T("\\"));
			wcscat(pathPaste,TempFile);
			HTREEITEM checkedExist;
			checkedExist = GetHandleTV(pathPaste);
			if(checkedExist != NULL)
			{
				MessageBox(hWnd, L"File has existed", L"ERROR", 0);
				break;
			}
			else
			{
				if (cutChecked == 0)			// neu nhan copy
				{	
					//Copy folder
					wcscpy(pathCopy,pathTemp);
					wcscpy(pathUndo[nClick-1],pathforUndo); // reload undo
					SHFILEOPSTRUCT copy ;
					copy.hwnd = hWnd;
					copy.wFunc = FO_COPY;
					copy.pFrom = pathCopy;
					copy.pTo = pathPaste;
					copy.fFlags = FOF_SILENT|FOF_NOCONFIRMMKDIR;

					if (IsFolder(pathPaste) == 0)
					{
						CopyFile(pathCopy,pathPaste,FALSE);
					}
					else
					{
						SHFileOperation( &copy );
					}
				}
				if (cutChecked == 1)				//neu nhan cut
				{
					wcscpy(pathCut,pathTemp);
					wcscpy(pathUndo[nClick-1],pathforUndo); // reload undo
					SHFILEOPSTRUCT cut ;
					cut.hwnd = hWnd;
					cut.wFunc = FO_MOVE;
					cut.pFrom = pathCut;
					cut.pTo = pathPaste;
					cut.fFlags = FOF_SILENT;

					if (IsFolder(pathPaste) == 0)
					{
						MoveFile(pathCut,pathPaste);
					}
					else
					{
						SHFileOperation( &cut );
						HTREEITEM deleted;
						deleted = GetHandleTV(pathCut);
						TreeView_DeleteItem(hWndTreeView,deleted);
					}
				}
				// load lai tree list
				wcscpy(pathUndo[nClick],CurrentDir); // reloadUndo
				HTREEITEM newRoot, preSibling ;
				newRoot = GetHandleTV(CurrentDir);
				if (IsFolder(CurrentDir) == 1)
				{
					UpDateTV(newRoot,CurrentDir);
					DirectoryListingTV(CurrentDir, newRoot, hWnd);
					DeleteTVExits(newRoot);
				}
				ListView_DeleteAllItems(hwndListView);
				if (isThumbnailling)
				{
					ThumbnailListView(CurrentDir, hWnd);
				}
				else
				{
					DirectoryListingLV(CurrentDir, hWnd);
				}
			}
			break;
		case IDMENU_DETELE:
			int msgboxID;
			msgboxID= MessageBox(
				NULL,
				L"Do you want to delete it?",
				L"Confirm Delete",
				MB_ICONEXCLAMATION | MB_YESNO
				);
			if (msgboxID == IDYES)
			{
				ListView_DeleteItem(hwndListView,iPos);
				wcscpy(pathDelete,CurrentDir);
				wcscat(pathDelete,_T("\\"));
				wcscat(pathDelete,fileName);
				DeleteFile(pathDelete);
				RemoveDirectory(pathDelete);
				SendToRecycleBin(pathDelete);
				// delete item
				HTREEITEM deleted;
				deleted = GetHandleTV(pathDelete);
				TreeView_DeleteItem(hWndTreeView,deleted);

				HTREEITEM PreRoot;
				PreRoot = GetHandleTV(CurrentDir);
				UpDateTV(PreRoot,CurrentDir);
				ListView_DeleteAllItems(hwndListView);
				if (isThumbnailling)
				{
					ThumbnailListView(CurrentDir, hWnd);
				}
				else
				{
					DirectoryListingLV(CurrentDir, hWnd);
				}
			}
			else
			{
				break;
			}
			break;
		case IDMENU_OPEN:
			wcscpy(TempFile, CurrentDir);
			wcscat(TempFile, _T("\\"));
			wcscat(TempFile, fileName);
			ShellExecute(0, L"open", TempFile, NULL, NULL, SW_SHOWDEFAULT);
			break;
		case IDMENU_CUT:
			wcscpy(TempFile,CurrentDir);
			wcscat(CurrentDir,_T("\\"));
			wcscat(CurrentDir,fileName);
			wcscpy(pathCut,CurrentDir);
			wcscpy(CurrentDir,TempFile);
			wcscpy(TempFile,fileName);
			wcscpy(pathTemp,pathCut);
			wcscpy(pathforUndo,CurrentDir);
			cutChecked = 1;		//Phan biet cut hay copy
			break;
		case IDMENU_RENAME:
			ListView_EditLabel(hwndListView, iPos);
			break;
			// Favoir -----------------------------------------------
		case IDMENU_FAVOR0:
			GoToPath(pathFavor[0]);
			break;
		case IDMENU_FAVOR1:
			GoToPath(pathFavor[1]);
			break;
		case IDMENU_FAVOR2:
			GoToPath(pathFavor[2]);
			break;
		case IDMENU_FAVOR3:
			GoToPath(pathFavor[3]);
			break;
		case IDMENU_FAVOR4:
			GoToPath(pathFavor[4]);
			break;
		case IDMENU_FAVOR5:
			GoToPath(pathFavor[5]);
			break;
		case IDMENU_FAVOR6:
			GoToPath(pathFavor[6]);
			break;
		case IDMENU_FAVOR7:
			GoToPath(pathFavor[7]);
			break;
		case IDMENU_FAVOR8:
			GoToPath(pathFavor[8]);
			break;
		case IDMENU_FAVOR9:
			GoToPath(pathFavor[9]);
			break;
		}	
		switch (wmId)
		{
		case IDB_VIEW:
			GetCursorPos(&cursor);
			hPopupMenu = CreatePopupMenu();
			InsertMenu(hPopupMenu, 0, MF_BYPOSITION|MF_STRING, IDMENU_Details, _T("Details"));
			InsertMenu(hPopupMenu, 0, MF_BYPOSITION|MF_STRING, IDMENU_List, _T("List"));
			InsertMenu(hPopupMenu, 0, MF_BYPOSITION|MF_STRING, IDMENU_Icon, _T("Icon"));
			InsertMenu(hPopupMenu, 0, MF_BYPOSITION|MF_STRING, IDMENU_Tile, _T("Tile"));
			InsertMenu(hPopupMenu, 0, MF_BYPOSITION|MF_STRING, IDMENU_Thumbnail, _T("Thumbnail"));
			SetForegroundWindow(hWnd);
			TrackPopupMenu(hPopupMenu, TPM_TOPALIGN|TPM_LEFTALIGN, cursor.x, cursor.y, 0, hWnd, NULL);
			DestroyMenu(hPopupMenu);
			break;
		case IDB_FAVOR:
			if (nFavor == 9)
			{
				nFavor = 0;
			}
			pathFavor[nFavor] =new TCHAR[MAX_LOADSTRING];
			wcscpy(pathFavor[nFavor],CurrentDir);
			nFavor++;
			break;
		case IDB_SHOWFAVOR:
			GetCursorPos(&cursor);
			hPopupMenu = CreatePopupMenu();
			InsertMenu(hPopupMenu, 0, MF_BYPOSITION|MF_STRING, IDMENU_FAVOR0, pathFavor[0]);
			InsertMenu(hPopupMenu, 0, MF_BYPOSITION|MF_STRING, IDMENU_FAVOR1, pathFavor[1]);
			InsertMenu(hPopupMenu, 0, MF_BYPOSITION|MF_STRING, IDMENU_FAVOR2, pathFavor[2]);
			InsertMenu(hPopupMenu, 0, MF_BYPOSITION|MF_STRING, IDMENU_FAVOR3, pathFavor[3]);
			InsertMenu(hPopupMenu, 0, MF_BYPOSITION|MF_STRING, IDMENU_FAVOR4, pathFavor[4]);
			InsertMenu(hPopupMenu, 0, MF_BYPOSITION|MF_STRING, IDMENU_FAVOR5, pathFavor[5]);
			InsertMenu(hPopupMenu, 0, MF_BYPOSITION|MF_STRING, IDMENU_FAVOR6, pathFavor[6]);
			InsertMenu(hPopupMenu, 0, MF_BYPOSITION|MF_STRING, IDMENU_FAVOR7, pathFavor[7]);
			InsertMenu(hPopupMenu, 0, MF_BYPOSITION|MF_STRING, IDMENU_FAVOR8, pathFavor[8]);
			InsertMenu(hPopupMenu, 0, MF_BYPOSITION|MF_STRING, IDMENU_FAVOR9, pathFavor[9]);
			SetForegroundWindow(hWnd);
			TrackPopupMenu(hPopupMenu, TPM_TOPALIGN|TPM_LEFTALIGN, cursor.x, cursor.y, 0, hWnd, NULL);
			DestroyMenu(hPopupMenu);
			break;
		case IDC_SEARCHBUTTON:
			numSearchResult = 0;
			Search_OK = FALSE;
			GetWindowText(hWndSearchBox, TempFile, MAX_LOADSTRING);
			ListView_DeleteAllItems(hwndListViewSearch);
			FindHandle = FindFirstVolumeW(VolumeName, ARRAYSIZE(VolumeName));
			if (FindHandle == INVALID_HANDLE_VALUE)
			{
				Error = GetLastError();
				wprintf(L"FindFirstVolumeW failed with error code %d\n", Error);
				return 0;
			}
			for (;;)
			{
				Index = wcslen(VolumeName) - 1;
				if (VolumeName[0]     != L'\\' ||
					VolumeName[1]     != L'\\' ||
					VolumeName[2]     != L'?'  ||
					VolumeName[3]     != L'\\' ||
					VolumeName[Index] != L'\\')
				{
					Error = ERROR_BAD_PATHNAME;
					wprintf(L"FindFirstVolumeW/FindNextVolumeW returned a bad path: %s\n", VolumeName);
					break;
				}
				VolumeName[Index] = L'\0';
				CharCount = QueryDosDeviceW(&VolumeName[4], DeviceName, ARRAYSIZE(DeviceName));
				VolumeName[Index] = L'\\';
				if (CharCount == 0)
				{
					Error = GetLastError();
					wprintf(L"QueryDosDeviceW failed with error code %d\n", Error);
					break;
				}
				DWORD  CharCount = MAX_PATH + 1;
				PWCHAR Names     = NULL;
				PWCHAR NameIdx   = NULL;
				BOOL   Success   = FALSE;
				for (;;)
				{
					Names = (PWCHAR) new BYTE [CharCount * sizeof(WCHAR)];
					if (!Names)
					{
						return 0;
					}
					Success = GetVolumePathNamesForVolumeNameW(VolumeName, Names, CharCount, &CharCount);
					if (Success)
					{
						break;
					}
					if ( GetLastError() != ERROR_MORE_DATA )
					{
						break;
					}
					delete [] Names;
					Names = NULL;
				}
				if (Success)
				{
					for (NameIdx = Names; NameIdx[0] != L'\0'; NameIdx += wcslen(NameIdx) + 1)
					{
						NameIdx[wcslen(NameIdx)-1]=L'\0';
					}
				}
				Search(Names, TempFile);
				if (Names != NULL)
				{
					delete [] Names;
					Names = NULL;
				}
				Success = FindNextVolumeW(FindHandle, VolumeName, ARRAYSIZE(VolumeName));
				if (!Success)
				{
					Error = GetLastError();
					if (Error != ERROR_NO_MORE_FILES)
					{
						wprintf(L"FindNextVolumeW failed with error code %d\n", Error);
						break;
					}
					Error = ERROR_SUCCESS;
					break;
				}
			}
			if (!Search_OK)
			{
				MessageBox(hWnd, _T("Sorry! We can't find your file..."), _T(":("), 0);
			}
			else
			{
				MessageBox(hWnd, _T("We got it!"), _T(":)"), 0);
				for (int i = 0; i < numSearchResult; i++)
				{
					itemSearch.iImage = id_link;
					itemSearch.iItem = i;
					itemSearch.iSubItem = 0;
					itemSearch.pszText = pathLink[i];
					ListView_InsertItem(hwndListViewSearch, &itemSearch);
				}
			}
			break;
		case IDC_BUTTON:
			GetWindowText(hWndEditBox, CurrentDir, MAX_LOADSTRING);
			GoToPath(CurrentDir);
			break;
		case IDC_BUTTON_SLIDESHOW:
			if (checked != 1 && checked != 2)
			{
				MessageBox(hWndSlideShow, _T("Choose a disk/folder to show image"), _T("ERROR"), 0);
				break;
			}

			if (!GetImage(CurrentDir, img))
			{
				MessageBox(hWndSlideShow, _T("Can't find image"), _T("ERROR"), 0);
				break;
			}
			index = 0;
			ShowWindow(hWndSlideShow, SW_NORMAL);
			Draw(hWndSlideShow, img[index]);
			break;
		case IDB_HOME:
			CollapseAllChildren(hWndTreeView,myComputer);
			SetWindowText(hWndEditBox, _T(""));
			nClick = -1;
			break;
		case IDB_UNDO:
			if(nClick > 0)
			{
				nClick--;
				GoToPath(pathUndo[nClick]);
			}
			break;
		case IDB_REUNDO:
			if(nClick >= 0)
			{		
				nClick++;
				GoToPath(pathUndo[nClick]);
			}
			break;
		case IDB_REFRESH:
			HTREEITEM update;
			update = GetHandleTV(CurrentDir);
			UpDateTV(update, CurrentDir);
			ListView_DeleteAllItems(hwndListView);
			if (isThumbnailling)
			{
				ThumbnailListView(CurrentDir, hWnd);
			}
			else
			{
				DirectoryListingLV(CurrentDir,hwndListView);
			}
			break;
		case IDB_ADD:
			if (checked == 1 || checked == 2)
			{
				wcscpy_s(folderAdd, _T("New Folder"));
				wcscpy_s(Temp, CurrentDir);
				wcscat_s(CurrentDir, _T("\\"));
				wcscat_s(CurrentDir, folderAdd);
				while (CreateDirectory(CurrentDir, NULL) == 0)
				{
					wcscpy_s(CurrentDir, Temp);
					wcscpy_s(folderAdd, _T("New Folder"));
					itoa(num, count, 10);
					count[strlen(count) + 1] = '\0';
					MultiByteToWideChar(0, 0, count, strlen(count) + 1, folder, MAX_LOADSTRING);
					wcscat_s(folderAdd, _T("("));
					wcscat_s(folderAdd, folder);
					wcscat_s(folderAdd, _T(")"));
					wcscat_s(CurrentDir, _T("\\"));
					wcscat_s(CurrentDir, folderAdd);
					num++;
				}
			}
			wcscpy_s(CurrentDir, Temp);
			ListView_DeleteAllItems(hwndListView);
			if (isThumbnailling)
			{
				ThumbnailListView(CurrentDir, hWnd);
			}
			else
			{
				DirectoryListingLV(CurrentDir, hWnd);
			}
			if (checked == 1)
			{
				HTREEITEM update;
				update = GetHandleTV(CurrentDir);
				UpDateTV(update, CurrentDir);
			}
			break;
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);

			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

INT DirectoryListingTV(TCHAR _szrDir[],HTREEITEM Root,HWND hWnd)
{
	myComputer = NULL;
	HTREEITEM Parent;
	WIN32_FIND_DATA ffd;
	TV_INSERTSTRUCT tvinsert;
	TCHAR szDir[MAX_PATH];
	HANDLE hFind = INVALID_HANDLE_VALUE;
	DWORD dwError=0;
	StringCchCopy(szDir, MAX_PATH,_szrDir);
	StringCchCat(szDir, MAX_PATH, TEXT("\\*"));
	hFind = FindFirstFile(szDir, &ffd);

	if (wcscmp(_szrDir,L"")==0)
	{
		DWORD  CharCount            = 0;
		WCHAR  DeviceName[MAX_PATH] = L"";
		DWORD  Error                = ERROR_SUCCESS;
		HANDLE FindHandle           = INVALID_HANDLE_VALUE;
		BOOL   Found                = FALSE;
		size_t Index                = 0;
		BOOL   Success              = FALSE;
		WCHAR  VolumeName[MAX_PATH] = L"";
		FindHandle = FindFirstVolumeW(VolumeName, ARRAYSIZE(VolumeName));
		if (FindHandle == INVALID_HANDLE_VALUE)
		{
			Error = GetLastError();
			wprintf(L"FindFirstVolumeW failed with error code %d\n", Error);
			return 0;
		}
		for (;;)
		{
			//
			//  Skip the \\?\ prefix and remove the trailing backslash.
			Index = wcslen(VolumeName) - 1;

			if (VolumeName[0]     != L'\\' ||
				VolumeName[1]     != L'\\' ||
				VolumeName[2]     != L'?'  ||
				VolumeName[3]     != L'\\' ||
				VolumeName[Index] != L'\\')
			{
				Error = ERROR_BAD_PATHNAME;
				wprintf(L"FindFirstVolumeW/FindNextVolumeW returned a bad path: %s\n", VolumeName);
				break;
			}

			//
			//  QueryDosDeviceW does not allow a trailing backslash,
			//  so temporarily remove it.
			VolumeName[Index] = L'\0';

			CharCount = QueryDosDeviceW(&VolumeName[4], DeviceName, ARRAYSIZE(DeviceName));

			VolumeName[Index] = L'\\';

			if ( CharCount == 0 )
			{
				Error = GetLastError();
				wprintf(L"QueryDosDeviceW failed with error code %d\n", Error);
				break;
			}
			// Processing Volume Name
			DWORD  CharCount = MAX_PATH + 1;
			PWCHAR Names     = NULL;
			PWCHAR NameIdx   = NULL;
			BOOL   Success   = FALSE;
			for (;;)
			{
				//
				//  Allocate a buffer to hold the paths.
				Names = (PWCHAR) new BYTE [CharCount * sizeof(WCHAR)];

				if ( !Names )
				{
					//
					//  If memory can't be allocated, return.
					return 0;
				}

				//
				//  Obtain all of the paths
				//  for this volume.
				Success = GetVolumePathNamesForVolumeNameW(
					VolumeName, Names, CharCount, &CharCount
					);

				if ( Success )
				{
					break;
				}

				if ( GetLastError() != ERROR_MORE_DATA )
				{
					break;
				}

				//
				//  Try again with the
				//  new suggested size.
				delete [] Names;
				Names = NULL;
			}

			if ( Success )
			{
				for ( NameIdx = Names;NameIdx[0] != L'\0';NameIdx += wcslen(NameIdx) + 1 )
				{
					NameIdx[wcslen(NameIdx)-1]=L'\0';
					tvinsert.item.pszText = NameIdx;
					tvinsert.hParent = Root;
					tvinsert.item.mask=TVIF_TEXT|TVIF_IMAGE|TVIF_SELECTEDIMAGE; 
					tvinsert.hInsertAfter = TVI_LAST;
					tvinsert.item.iImage = id_disk;
					tvinsert.item.iSelectedImage = id_disk;

					Parent = (HTREEITEM)SendDlgItemMessage(hWnd, IDC_TREEVIEW, TVM_INSERTITEM, 0, (LPARAM)&tvinsert);
				}
			}
			if ( Names != NULL )
			{
				delete [] Names;
				Names = NULL;
			}
			Success = FindNextVolumeW(FindHandle, VolumeName, ARRAYSIZE(VolumeName));

			if ( !Success )
			{
				Error = GetLastError();

				if (Error != ERROR_NO_MORE_FILES)
				{
					wprintf(L"FindNextVolumeW failed with error code %d\n", Error);
					break;
				}
				Error = ERROR_SUCCESS;
				break;
			}
		}
	}
	else
	{
		if (INVALID_HANDLE_VALUE == hFind) 
		{
			return dwError;
		} 
		do
		{
			if (
				(ffd.dwFileAttributes	&	FILE_ATTRIBUTE_DIRECTORY) 
				&&!	(wcscmp(ffd.cFileName,L".")==0) 
				&&!	(wcscmp(ffd.cFileName,L"..")==0)
				&&!	(ffd.dwFileAttributes	&	FILE_ATTRIBUTE_HIDDEN)
				&&!	(ffd.dwFileAttributes	&	FILE_ATTRIBUTE_SYSTEM)
				)
			{

				TCHAR buf[MAX_PATH];
				tvinsert.item.pszText = ffd.cFileName;
				tvinsert.hParent = Root;
				tvinsert.item.mask=TVIF_TEXT|TVIF_IMAGE|TVIF_SELECTEDIMAGE; 
				tvinsert.hInsertAfter = TVI_LAST;
				tvinsert.item.iImage = id_folder;
				tvinsert.item.iSelectedImage = id_folder;
				Parent=(HTREEITEM)SendDlgItemMessage(hWnd,IDC_TREEVIEW,
					TVM_INSERTITEM,0,(LPARAM)&tvinsert);

				swprintf_s(buf,_T("%s\\%s"),_szrDir,ffd.cFileName);
				::OutputDebugString(buf);
				::OutputDebugString(_T("\n"));
			}
		}
		while (FindNextFile(hFind, &ffd) != 0);
		FindClose(hFind);
		return dwError;
	}
}

VOID DisplayErrorBox(LPTSTR lpszFunction) 
{ 
	// Retrieve the system error message for the last-error code

	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
	DWORD dw = GetLastError(); 

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR) &lpMsgBuf,
		0, NULL );

	// Display the error message and clean up

	lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT, 
		(lstrlen((LPCTSTR)lpMsgBuf)+lstrlen((LPCTSTR)lpszFunction)+40)*sizeof(TCHAR)); 
	StringCchPrintf((LPTSTR)lpDisplayBuf, 
		LocalSize(lpDisplayBuf) / sizeof(TCHAR),
		TEXT("%s failed with error %d: %s"), 
		lpszFunction, dw, lpMsgBuf); 
	MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK); 

	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);
}

INT DirectoryListingLV(TCHAR _szrDir[],HWND hWnd)
{
	WIN32_FIND_DATA ffd;
	TCHAR szDir[MAX_PATH];
	HANDLE hFind = INVALID_HANDLE_VALUE;
	DWORD dwError = 0;
	StringCchCopy(szDir, MAX_PATH,_szrDir);
	StringCchCat(szDir, MAX_PATH, TEXT("\\*"));
	hFind = FindFirstFile(szDir, &ffd);

	order[0] = 1;
	order[1] = 2;
	order[2] = 3;
	lvti.cbSize = sizeof(LVTILEINFO);
	lvti.cColumns = 4;
	lvti.piColFmt =  LVCFMT_LEFT;
	lvti.puColumns = PUINT(order);

	lvI.mask = LVIF_TEXT|LVIF_IMAGE|LVIF_STATE;
	lvI.stateMask = 0;
	lvI.state = 0;
	int index = 0;

	// Search and add item to ListView
	do
	{
		if (!(wcscmp(ffd.cFileName,L".") == 0)
			&&	!(wcscmp(ffd.cFileName,L"..") == 0)
			&&	!(ffd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)
			&&	!(ffd.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM))
		{
			TCHAR buf[MAX_PATH];
			lvti.iItem = index;
			if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				lvI.iImage = file[32];
				lvI.iItem = index;
				// FILE NAME:
				lvI.iSubItem = 0;
				lvI.pszText = ffd.cFileName;
				ListView_InsertItem(hwndListView, &lvI);

				//FILE TYPE:
				lvI.iSubItem = 2;
				lvI.pszText = L"File Folder";
				ListView_SetItem(hwndListView, &lvI);

				//FILE DATE:
				lvI.iSubItem = 3;
				LPSYSTEMTIME system = new SYSTEMTIME();
				FILETIME* filetime = new FILETIME(ffd.ftCreationTime);
				FileTimeToSystemTime(filetime,system);
				wchar_t temp[MAX_PATH];
				swprintf_s(temp,_T("%d/%d/%d"),system->wDay,system->wMonth,system->wYear);
				lvI.pszText = temp;
				ListView_SetItem(hwndListView, &lvI);
			}
			else
			{
				// Choose IMAGE by FILETYPE
				lvI.iImage = file[0];
				wchar_t fileType[MAX_PATH];
				swprintf_s(fileType, L"%s",type(ffd));
				if (wcscmp(fileType, L"EXE") == 0)
				{
					lvI.iImage = file[1];
				}
				if (wcscmp(fileType, L"7Z") == 0)
				{
					lvI.iImage = file[2];
				}
				if (wcscmp(fileType, L"ASC") == 0)
				{
					lvI.iImage = file[3];
				}
				if (
					(wcscmp(fileType, L"AVI") == 0)
					||	(wcscmp(fileType, L"MP4") == 0)
					||	(wcscmp(fileType, L"M4A") == 0)
					||	(wcscmp(fileType, L"M4V") == 0)
					||	(wcscmp(fileType, L"3GP") == 0)
					||	(wcscmp(fileType, L"3G2") == 0)
					||	(wcscmp(fileType, L"ASF") == 0)
					||	(wcscmp(fileType, L"WMA") == 0)
					||	(wcscmp(fileType, L"WMV") == 0)
					||	(wcscmp(fileType, L"FLV") == 0)
					||	(wcscmp(fileType, L"MPG") == 0)
					)
				{
					lvI.iImage = file[34];
				}
				if (wcscmp(fileType, L"CSV") == 0)
				{
					lvI.iImage = file[5];
				}
				if (wcscmp(fileType, L"DLL") == 0)
				{
					lvI.iImage = file[6];
				}
				if (wcscmp(fileType, L"DMG") == 0)
				{
					lvI.iImage = file[7];
				}
				if (
					(wcscmp(fileType, L"GIF") == 0)
					||	(wcscmp(fileType, L"JPG") == 0)
					||	(wcscmp(fileType, L"JPEG") == 0)
					||	(wcscmp(fileType, L"PNG") == 0)
					||	(wcscmp(fileType, L"BMP") == 0)
					||	(wcscmp(fileType, L"ICO") == 0)
					||	(wcscmp(fileType, L"EMF") == 0)
					||	(wcscmp(fileType, L"TIF") == 0)
					||	(wcscmp(fileType, L"WMF") == 0)
					||	(wcscmp(fileType, L"MOV") == 0)
					)
				{
					lvI.iImage = file[35];
				}
				if (wcscmp(fileType, L"GIS") == 0)
				{
					lvI.iImage = file[10];
				}
				if (wcscmp(fileType, L"GPX") == 0)
				{
					lvI.iImage = file[11];
				}
				if (wcscmp(fileType, L"KML") == 0)
				{
					lvI.iImage = file[13];
				}
				if (wcscmp(fileType, L"KMZ") == 0)
				{
					lvI.iImage = file[14];
				}
				if (
					(wcscmp(fileType, L"MP3") == 0)
					||	(wcscmp(fileType, L"FLAC") == 0)
					||	(wcscmp(fileType, L"OGG") == 0)
					||	(wcscmp(fileType, L"AAC") == 0)
					||	(wcscmp(fileType, L"AMR") == 0)
					||	(wcscmp(fileType, L"QCP") == 0)
					||	(wcscmp(fileType, L"WAV") == 0)
					||	(wcscmp(fileType, L"MID") == 0)
					)
				{
					lvI.iImage = file[33];
				}
				if (wcscmp(fileType, L"NMEA") == 0)
				{
					lvI.iImage = file[18];
				}
				if (wcscmp(fileType, L"OSM") == 0)
				{
					lvI.iImage = file[20];
				}
				if (wcscmp(fileType, L"PDF") == 0)
				{
					lvI.iImage = file[21];
				}
				if ((wcscmp(fileType, L"PPT") == 0) || (wcscmp(fileType, L"PPTX") == 0))
				{
					lvI.iImage = file[23];
				}
				if (wcscmp(fileType, L"PSD") == 0)
				{
					lvI.iImage = file[24];
				}
				if (wcscmp(fileType, L"RAR") == 0)
				{
					lvI.iImage = file[25];
				}
				if (wcscmp(fileType, L"TXT") == 0)
				{
					lvI.iImage = file[26];
				}
				if ((wcscmp(fileType, L"DOC") == 0) || (wcscmp(fileType, L"DOCX") == 0))
				{
					lvI.iImage = file[28];
				}
				if ((wcscmp(fileType, L"XLS") == 0) || (wcscmp(fileType, L"XLSX") == 0))
				{
					lvI.iImage = file[29];
				}
				if (wcscmp(fileType, L"ZIP") == 0)
				{
					lvI.iImage = file[30];
				}

				lvI.iItem = index;
				// FILE NAME:
				lvI.iSubItem = 0;
				lvI.pszText = ffd.cFileName;
				ListView_InsertItem(hwndListView, &lvI);

				// FILE SIZE:
				lvI.iSubItem = 1;
				LARGE_INTEGER filesize;
				filesize.LowPart = ffd.nFileSizeLow;
				filesize.HighPart = ffd.nFileSizeHigh;
				wchar_t kichco[MAX_PATH];
				swprintf_s(kichco,L"%s",(to_wstring(filesize.QuadPart/1024)).c_str());
				wcscat_s(kichco, L" KB");
				lvI.pszText = kichco;
				ListView_SetItem(hwndListView, &lvI);

				//FILE TYPE:
				lvI.iSubItem = 2;
				wcscat_s(fileType, L" File");
				lvI.pszText = fileType;
				ListView_SetItem(hwndListView, &lvI);

				//FILE DATE:
				lvI.iSubItem = 3;
				LPSYSTEMTIME system = new SYSTEMTIME();
				FILETIME* filetime = new FILETIME(ffd.ftCreationTime);
				FileTimeToSystemTime(filetime,system);
				wchar_t temp[MAX_PATH];
				swprintf_s(temp,_T("%d/%d/%d"),system->wDay,system->wMonth,system->wYear);
				lvI.pszText = temp;
				ListView_SetItem(hwndListView, &lvI);
			}
			ListView_SetTileInfo(hwndListView, &lvti);
			index++;
		}
	}
	while (FindNextFile(hFind, &ffd) != 0);
	FindClose(hFind);
	return dwError;
}

wchar_t *type(WIN32_FIND_DATA data)
{
	wchar_t result[MAX_LOADSTRING];
	int i;
	for (i = wcslen(data.cFileName); i >= 0; i--)
	{
		if (data.cFileName[i] == '.')
		{
			break;
		}
	}
	int j = 0;
	i = i + 1;
	while (i < wcslen(data.cFileName))
	{
		result[j] = data.cFileName[i];
		if (result[j] >='a' && result[j] <= 'z')
		{
			result[j] = result[j] - 32;
		}
		i++;
		j++;
	}
	result[j] = NULL;
	result[j + 1] = NULL;
	return result;
}

VOID CollapseNode( HWND hTree, HTREEITEM hti )
{
	if( TreeView_GetChild( hTree, hti ) != NULL )
	{
		TreeView_Expand( hTree, hti, TVE_COLLAPSE );
		hti = TreeView_GetChild( hTree, hti );
		do
		{
			CollapseNode( hTree, hti );
		}
		while( ( hti = TreeView_GetNextSibling( hTree, hti ) ) != NULL  );
	}
}

VOID CollapseAllChildren(HWND treeWnd, HTREEITEM parent)
{
	HTREEITEM curNode;

	curNode = TreeView_GetChild(treeWnd, parent);
	if (curNode != NULL)
	{
		CollapseNode(treeWnd, curNode);

		while ((curNode = TreeView_GetNextSibling(treeWnd, curNode)) != NULL)
		{
			CollapseNode(treeWnd, curNode);
		}
	}
}

BOOL DirExists(TCHAR* path){
	DWORD ftyp = GetFileAttributesW( path );
	if (ftyp == INVALID_FILE_ATTRIBUTES)
		return FALSE;  // wrong path :-?
	if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
		return TRUE;   // this is a directory!
	else
		return FALSE;    // this is not a directory!
}

INT SplitPath(TCHAR *buffer, TCHAR **rslt, TCHAR delimenter){
	int iBuff, iSubPath, nPart;

	nPart = 0;
	iBuff = 0;
	iSubPath = 0;

	rslt[0] = new TCHAR[MAX_LOADSTRING];
	while( iBuff <= wcslen(buffer)-1 ){
		if( buffer[iBuff] == NULL )
			break;
		if( buffer[iBuff] == delimenter ){
			// end old path
			rslt[nPart][iSubPath] = NULL;
			if( iBuff == wcslen(buffer)-1 ) // if delimenter is end-character
				break;
			// split new path
			nPart++;
			rslt[nPart] = new TCHAR[MAX_LOADSTRING];
			iSubPath = 0;
		}else{
			rslt[nPart][iSubPath] = buffer[iBuff];
			iSubPath++;
		}
		iBuff++;
	}
	rslt[nPart][iSubPath] = NULL;

	nPart++;
	return nPart;
}

BOOL GoToPath(TCHAR path[])
{
	if(!DirExists(path))
	{
		return FALSE;
	}

	HTREEITEM hCur ;
	hCur= TreeView_GetChild(hWndTreeView, myComputer);
	HTREEITEM hChild;
	TVITEM item;
	TCHAR **subPath;
	TCHAR Pathcur[MAX_PATH], buffer[MAX_PATH];
	struct stat fileInfo;

	subPath = new TCHAR*;
	int nPart = SplitPath(path, subPath, L'\\');

	memset(Pathcur, 0, MAX_PATH);
	memset(buffer, 0, MAX_PATH);

	item.hItem = hCur;
	item.mask = TVIF_TEXT;
	item.cchTextMax = 200;
	item.pszText = buffer;

	for( int i = 0; i <= nPart-1; i++ )
	{
		hCur = TreeView_GetChild(hWndTreeView, hCur);
		item.hItem = hCur;
		TreeView_GetItem(hWndTreeView, &item);

		StringCchCat(Pathcur, MAX_PATH, subPath[i]);
		if( wcscmp(item.pszText, subPath[i]) != 0 )
		{
			do
			{
				hCur = TreeView_GetNextSibling(hWndTreeView, hCur);
				item.hItem = hCur;
				TreeView_GetItem(hWndTreeView, &item);
			}
			while( wcscmp(item.pszText, subPath[i]) != 0 );
		}

		hChild = TreeView_GetChild(hWndTreeView, hCur);

		if( hChild == NULL )
		{
			DirectoryListingTV(Pathcur,hCur,hWnd);
		}
		else
		{
			CollapseAllChildren(hWndTreeView,myComputer);
		}

		TreeView_Expand(hWndTreeView, hCur, TVE_EXPAND);
		StringCchCat(Pathcur, MAX_PATH, _T("\\"));

	}
	TreeView_SelectItem(hWndTreeView, hCur);
	SetWindowText(hWndEditBox, CurrentDir);
	ListView_DeleteAllItems(hwndListView);
	if (isThumbnailling)
	{
		ThumbnailListView(CurrentDir, hWnd);
	}
	else
	{
		DirectoryListingLV(CurrentDir, hWnd);
	}
	return TRUE;
}

INT SendToRecycleBin(TCHAR newpath[]) 
{          
	_tcscat_s(newpath, MAX_PATH,_T("|"));
	TCHAR* Lastptr = _tcsrchr(newpath, _T('|'));
	*Lastptr = _T('\0');                                 // Replace last pointer with Null for double null termination
	SHFILEOPSTRUCT shFileStruct; 
	ZeroMemory(&shFileStruct,sizeof(shFileStruct)); 
	shFileStruct.hwnd=NULL; 
	shFileStruct.wFunc= FO_DELETE; 
	shFileStruct.pFrom= newpath;
	shFileStruct.fFlags = FOF_ALLOWUNDO | FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_SILENT;
	return SHFileOperation(&shFileStruct);
}

HTREEITEM GetHandleTV(TCHAR *path){

	HTREEITEM hCur , hChild;
	hCur  =TreeView_GetChild(hWndTreeView, myComputer);
	TVITEM item;
	TCHAR **subPath;
	TCHAR Pathcur[MAX_PATH], buffer[MAX_PATH];

	// split path
	subPath = new TCHAR*;
	int nPart = SplitPath(path, subPath, L'\\');

	memset(Pathcur, 0, MAX_PATH);   //      prepare blank strings
	memset(buffer, 0, MAX_PATH);

	//      prepare handle
	item.hItem = hCur;
	item.mask = TVIF_TEXT;
	item.cchTextMax = 200;
	item.pszText = buffer;

	for( int i = 0; i <= nPart-1; i++ ){

		hCur = TreeView_GetChild(hWndTreeView, hCur);
		if(hCur == NULL) return NULL;
		DWORD err = GetLastError();
		item.hItem = hCur;
		TreeView_GetItem(hWndTreeView, &item);

		StringCchCat(Pathcur, MAX_PATH, subPath[i]);
		if( wcscmp(item.pszText, subPath[i]) != 0 ){
			do{
				hCur = TreeView_GetNextSibling(hWndTreeView, hCur);
				item.hItem = hCur;
				TreeView_GetItem(hWndTreeView, &item);
				if( hCur == NULL ) return NULL;
			}while( wcscmp(item.pszText, subPath[i]) != 0 );
		}

		StringCchCat(Pathcur, MAX_PATH, _T("\\"));
	}

	return hCur;
}

VOID UpDateRenameTV(HTREEITEM hCur,  TCHAR *path, TCHAR *name)
{
	TCHAR buffer[200];
	HTREEITEM cha;
	TVITEM item, itemcha;
	item.hItem = hCur;
	item.mask = TVIF_TEXT;
	item.cchTextMax = 200;
	item.pszText = name;
	TreeView_SetItem(hWndTreeView,&item);
	wcscpy_s(Mask,_T(""));
	wcscpy(path,item.pszText);
	cha = TreeView_GetParent(hWndTreeView,item.hItem);
	while (cha != NULL)
	{
		itemcha.hItem = cha;
		itemcha.mask = TVIF_TEXT;
		itemcha.cchTextMax = 200;
		itemcha.pszText = buffer;
		TreeView_GetItem(hWndTreeView, &itemcha);
		wcscpy_s(buffer, itemcha.pszText);
		if (wcscmp(buffer, _T("Computer")) == 0) 
		{
			break;
		}
		wcscat_s(buffer,_T("\\"));
		wcscat_s(buffer, path);
		wcscpy(path, buffer);
		cha = TreeView_GetParent(hWndTreeView,itemcha.hItem);
	}
	if (checked == 1 && (TreeView_GetChild(hWndTreeView, hCur) == NULL))
	{
		DirectoryListingTV(path, hCur, hWnd);
	}
}

VOID UpDateTV(HTREEITEM hCur, TCHAR *path)
{
	TCHAR buffer[200];
	HTREEITEM cha;
	TVITEM item, itemcha;
	item.hItem = hCur;
	item.mask = TVIF_TEXT;
	item.cchTextMax = 200;
	item.pszText = buffer;
	TreeView_GetItem(hWndTreeView,&item);
	wcscpy_s(Mask,_T(""));
	wcscpy(path,item.pszText);
	cha = TreeView_GetParent(hWndTreeView,item.hItem);
	while (cha != NULL)
	{
		itemcha.hItem = cha;
		itemcha.mask = TVIF_TEXT;
		itemcha.cchTextMax = 200;
		itemcha.pszText = buffer;
		TreeView_GetItem(hWndTreeView, &itemcha);
		wcscpy_s(buffer, itemcha.pszText);
		if (wcscmp(buffer, _T("Computer")) == 0) 
		{
			break;
		}
		wcscat_s(buffer,_T("\\"));
		wcscat_s(buffer, path);
		wcscpy(path, buffer);
		cha = TreeView_GetParent(hWndTreeView,itemcha.hItem);
	}
	if (checked == 1 && (TreeView_GetChild(hWndTreeView, hCur) == NULL))
	{
		DirectoryListingTV(path, hCur, hWnd);
	}
}

VOID NotifyListView(LPNMLISTVIEW pNMLV)
{
	switch (pNMLV->hdr.code)
	{
	case LVN_COLUMNCLICK:
		pNMLV->lParam = isAscending;
		isAscending = !isAscending;
		ListView_SortItemsEx(pNMLV->hdr.hwndFrom, (PFNLVCOMPARE)SortListViewFunc, pNMLV);
		break;
	}
}

INT CALLBACK SortListViewFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	LPNMLISTVIEW pNMLV = (LPNMLISTVIEW)lParamSort;
	TCHAR szItemText1[MAX_PATH] = {0};
	TCHAR szItemText2[MAX_PATH] = {0};
	int size1, size2;
	int number[6];
	int i = 0;
	int cDate = 0, cSplit = 0;
	int count = 0;
	TCHAR split[MAX_PATH] = {0};
	ListView_GetItemText(pNMLV->hdr.hwndFrom, (INT)lParam1, pNMLV->iSubItem, szItemText1, MAX_PATH);
	ListView_GetItemText(pNMLV->hdr.hwndFrom, (INT)lParam2, pNMLV->iSubItem, szItemText2, MAX_PATH);
	switch (pNMLV->iSubItem)
	{
	case 0: 
	case 2:
		if (pNMLV->lParam == 1)
		{
			return _tcscmp(szItemText1, szItemText2);
		}
		return _tcscmp(szItemText2, szItemText1);
		break;
	case 1:
		size1 = _ttoi(szItemText1);
		size2 = _ttoi(szItemText2);
		if (pNMLV->lParam == 1)
		{
			return (size1 > size2);
		}
		return (size2 > size1);
		break;
	case 3:
		while (count < 3)
		{
			while ((szItemText1[cDate] != '/') && (szItemText1[cDate] != NULL))
			{
				split[cSplit] = szItemText1[cDate];
				cDate++;
				cSplit++;
			}
			split[cSplit] = 0;
			cDate++;
			cSplit = 0;
			number[i] = _ttoi(split);
			i++;
			count++;
		}
		count = 0;
		cDate = 0;
		while (count < 3)
		{
			while ((szItemText2[cDate] != '/') && (szItemText2[cDate] != NULL))
			{
				split[cSplit] = szItemText2[cDate];
				cDate++;
				cSplit++;
			}
			split[cSplit] = 0;
			cDate++;
			cSplit = 0;
			number[i] = _ttoi(split);
			i++;
			count++;
		}
		if (pNMLV->lParam == 1)
		{
			if (number[2] == number[5])
			{
				if (number[1] == number[4])
				{
					return (number[0] > number[3]);
				}
				return (number[1] > number[4]);
			}
			return (number[2] > number[5]);
		}
		if (number[2] == number[5])
		{
			if (number[1] == number[4])
			{
				return (number[0] < number[3]);
			}
			return (number[1] < number[4]);
		}
		return (number[2] < number[5]);
		break;
	}
}

HBITMAP GetThumbnail(PTSTR File)
{
	HBITMAP BitmapThumbnail = NULL;
	IThumbnailProvider* pThumbnailProvider;
	IInitializeWithFile* pInitFile;
	HRESULT hresult = CoCreateInstance(CLSID_ImageThumbnailProvider,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&pThumbnailProvider));
	if (SUCCEEDED(hresult))
	{
		pThumbnailProvider->QueryInterface(IID_PPV_ARGS(&pInitFile));
		pInitFile->Initialize(File, STGM_READ);
		WTS_ALPHATYPE alphaType;
		pThumbnailProvider->GetThumbnail(100, &BitmapThumbnail, &alphaType);
		return BitmapThumbnail;
	}
	return NULL;
}

INT ThumbnailListView(TCHAR _szrDir[],HWND hWnd)
{
	WIN32_FIND_DATA ffd;
	TCHAR szDir[MAX_PATH];
	HANDLE hFind = INVALID_HANDLE_VALUE;
	DWORD dwError = 0;
	StringCchCopy(szDir, MAX_PATH,_szrDir);
	StringCchCat(szDir, MAX_PATH, TEXT("\\*"));
	hFind = FindFirstFile(szDir, &ffd);
	TCHAR pathThumbnail[MAX_LOADSTRING];
	lvI.mask = LVIF_TEXT|LVIF_IMAGE|LVIF_STATE;
	lvI.stateMask = 0;
	lvI.state = 0;
	int index = 0;

	// Search and add item to ListView
	do
	{
		if (!(wcscmp(ffd.cFileName,L".") == 0) 
			&&	!(wcscmp(ffd.cFileName,L"..") == 0)
			&&	!(ffd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)
			&&	!(ffd.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM))
		{
			TCHAR buf[MAX_PATH];
			if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				lvI.iImage = fileThumb[32];
				lvI.iItem = index;
				// FILE NAME:
				lvI.iSubItem = 0;
				lvI.pszText = ffd.cFileName;
				ListView_InsertItem(hwndListView, &lvI);

				//FILE TYPE:
				lvI.iSubItem = 2;
				lvI.pszText = L"File Folder";
				ListView_SetItem(hwndListView, &lvI);

				//FILE DATE:
				lvI.iSubItem = 3;
				LPSYSTEMTIME system = new SYSTEMTIME();
				FILETIME* filetime = new FILETIME(ffd.ftCreationTime);
				FileTimeToSystemTime(filetime,system);
				wchar_t temp[MAX_PATH];
				swprintf_s(temp,_T("%d/%d/%d"),system->wDay,system->wMonth,system->wYear);
				lvI.pszText = temp;
				ListView_SetItem(hwndListView, &lvI);
			}
			else
			{
				// Choose IMAGE by FILETYPE
				lvI.iImage = fileThumb[0];
				wchar_t fileType[MAX_PATH];
				swprintf_s(fileType, L"%s",type(ffd));
				if (wcscmp(fileType, L"EXE") == 0)
				{
					lvI.iImage = fileThumb[1];
				}
				if (wcscmp(fileType, L"7Z") == 0)
				{
					lvI.iImage = fileThumb[2];
				}
				if (wcscmp(fileType, L"ASC") == 0)
				{
					lvI.iImage = fileThumb[3];
				}
				if (
					(wcscmp(fileType, L"AVI") == 0)
					||	(wcscmp(fileType, L"MP4") == 0)
					||	(wcscmp(fileType, L"M4A") == 0)
					||	(wcscmp(fileType, L"M4V") == 0)
					||	(wcscmp(fileType, L"3GP") == 0)
					||	(wcscmp(fileType, L"3G2") == 0)
					||	(wcscmp(fileType, L"ASF") == 0)
					||	(wcscmp(fileType, L"WMA") == 0)
					||	(wcscmp(fileType, L"WMV") == 0)
					||	(wcscmp(fileType, L"FLV") == 0)
					||	(wcscmp(fileType, L"MPG") == 0)
					)
				{
					lvI.iImage = fileThumb[34];
				}
				if (wcscmp(fileType, L"CSV") == 0)
				{
					lvI.iImage = fileThumb[5];
				}
				if (wcscmp(fileType, L"DLL") == 0)
				{
					lvI.iImage = fileThumb[6];
				}
				if (wcscmp(fileType, L"DMG") == 0)
				{
					lvI.iImage = fileThumb[7];
				}
				if (
					(wcscmp(fileType, L"GIF") == 0)
					||	(wcscmp(fileType, L"JPG") == 0)
					||	(wcscmp(fileType, L"JPEG") == 0)
					||	(wcscmp(fileType, L"PNG") == 0)
					||	(wcscmp(fileType, L"BMP") == 0)
					||	(wcscmp(fileType, L"ICO") == 0)
					||	(wcscmp(fileType, L"EMF") == 0)
					||	(wcscmp(fileType, L"TIF") == 0)
					||	(wcscmp(fileType, L"WMF") == 0)
					||	(wcscmp(fileType, L"MOV") == 0)
					)
				{
					HBITMAP hBitmapThumbnail;
					wcscpy(pathThumbnail, CurrentDir);
					wcscat(pathThumbnail, _T("\\"));
					wcscat(pathThumbnail, ffd.cFileName);
					hBitmapThumbnail = GetThumbnail(pathThumbnail);
					lvI.iImage = ImageList_Add(hImageListThumbnail, hBitmapThumbnail, NULL);
				}
				if (wcscmp(fileType, L"GIS") == 0)
				{
					lvI.iImage = fileThumb[10];
				}
				if (wcscmp(fileType, L"GPX") == 0)
				{
					lvI.iImage = fileThumb[11];
				}
				if (wcscmp(fileType, L"KML") == 0)
				{
					lvI.iImage = fileThumb[13];
				}
				if (wcscmp(fileType, L"KMZ") == 0)
				{
					lvI.iImage = fileThumb[14];
				}
				if (
					(wcscmp(fileType, L"MP3") == 0)
					||	(wcscmp(fileType, L"FLAC") == 0)
					||	(wcscmp(fileType, L"OGG") == 0)
					||	(wcscmp(fileType, L"AAC") == 0)
					||	(wcscmp(fileType, L"AMR") == 0)
					||	(wcscmp(fileType, L"QCP") == 0)
					||	(wcscmp(fileType, L"WAV") == 0)
					||	(wcscmp(fileType, L"MID") == 0)
					)
				{
					lvI.iImage = fileThumb[33];
				}
				if (wcscmp(fileType, L"NMEA") == 0)
				{
					lvI.iImage = fileThumb[18];
				}
				if (wcscmp(fileType, L"OSM") == 0)
				{
					lvI.iImage = fileThumb[20];
				}
				if (wcscmp(fileType, L"PDF") == 0)
				{
					lvI.iImage = fileThumb[21];
				}
				if ((wcscmp(fileType, L"PPT") == 0) || (wcscmp(fileType, L"PPTX") == 0))
				{
					lvI.iImage = fileThumb[23];
				}
				if (wcscmp(fileType, L"PSD") == 0)
				{
					lvI.iImage = fileThumb[24];
				}
				if (wcscmp(fileType, L"RAR") == 0)
				{
					lvI.iImage = fileThumb[25];
				}
				if (wcscmp(fileType, L"TXT") == 0)
				{
					lvI.iImage = fileThumb[26];
				}
				if ((wcscmp(fileType, L"DOC") == 0) || (wcscmp(fileType, L"DOCX") == 0))
				{
					lvI.iImage = fileThumb[28];
				}
				if ((wcscmp(fileType, L"XLS") == 0) || (wcscmp(fileType, L"XLSX") == 0))
				{
					lvI.iImage = fileThumb[29];
				}
				if (wcscmp(fileType, L"ZIP") == 0)
				{
					lvI.iImage = fileThumb[30];
				}

				lvI.iItem = index;
				// FILE NAME:
				lvI.iSubItem = 0;
				lvI.pszText = ffd.cFileName;
				ListView_InsertItem(hwndListView, &lvI);

				// FILE SIZE:
				lvI.iSubItem = 1;
				LARGE_INTEGER filesize;
				filesize.LowPart = ffd.nFileSizeLow;
				filesize.HighPart = ffd.nFileSizeHigh;
				wchar_t kichco[MAX_PATH];
				swprintf_s(kichco,L"%s",(to_wstring(filesize.QuadPart/1024)).c_str());
				wcscat_s(kichco, L" KB");
				lvI.pszText = kichco;
				ListView_SetItem(hwndListView, &lvI);

				//FILE TYPE:
				lvI.iSubItem = 2;
				wcscat_s(fileType, L" File");
				lvI.pszText = fileType;
				ListView_SetItem(hwndListView, &lvI);

				//FILE DATE:
				lvI.iSubItem = 3;
				LPSYSTEMTIME system = new SYSTEMTIME();
				FILETIME* filetime = new FILETIME(ffd.ftCreationTime);
				FileTimeToSystemTime(filetime,system);
				wchar_t temp[MAX_PATH];
				swprintf_s(temp,_T("%d/%d/%d"),system->wDay,system->wMonth,system->wYear);
				lvI.pszText = temp;
				ListView_SetItem(hwndListView, &lvI);
			}
			index++;
		}
	}
	while (FindNextFile(hFind, &ffd) != 0);
	FindClose(hFind);
	return dwError;
}

VOID Draw(HWND hWndDraw, Gdiplus::Image* image)
{
	HDC hdc_image = BeginPaint(hWndDraw, &ps);
	Gdiplus::Graphics grp(hdc_image);
	grp.DrawImage(image, 0, 0, 500, 500);
	EndPaint(hWndDraw, &ps);
}

BOOL GetImage(TCHAR Somewhere[], Gdiplus::Image* image[])
{
	HANDLE imageHDL;
	WIN32_FIND_DATA fileData;
	TCHAR imageLink[MAX_LOADSTRING];
	TCHAR OOO[MAX_LOADSTRING];
	BOOL png = TRUE, jpg = TRUE, bmp = TRUE, ico = TRUE, tiff = TRUE, gif = TRUE, wmf = TRUE, emf = TRUE;
	StringCchCopy(OOO, MAX_LOADSTRING, Somewhere);
	StringCchCat(OOO, MAX_LOADSTRING, _T("\\*.png"));
	imageHDL = FindFirstFile(OOO, &fileData);
	if (imageHDL == INVALID_HANDLE_VALUE)
	{
		png = FALSE;
	}
	else
	{
		index = numImage;
		do
		{
			StringCchCopy(imageLink, MAX_LOADSTRING, Somewhere);
			StringCchCat(imageLink, MAX_LOADSTRING, _T("\\"));
			StringCchCat(imageLink, MAX_LOADSTRING, fileData.cFileName);
			image[index] = Gdiplus::Image::FromFile(imageLink);
			index++;
		}
		while (FindNextFile(imageHDL, &fileData) != 0);
		numImage = index;
	}
	StringCchCopy(OOO, MAX_LOADSTRING, Somewhere);
	StringCchCat(OOO, MAX_LOADSTRING, _T("\\*.jpg"));
	imageHDL = FindFirstFile(OOO, &fileData);
	if (imageHDL == INVALID_HANDLE_VALUE)
	{
		jpg = FALSE;
	}
	else
	{
		index = numImage;
		do
		{
			StringCchCopy(imageLink, MAX_LOADSTRING, Somewhere);
			StringCchCat(imageLink, MAX_LOADSTRING, _T("\\"));
			StringCchCat(imageLink, MAX_LOADSTRING, fileData.cFileName);
			image[index] = Gdiplus::Image::FromFile(imageLink);
			index++;
		}
		while (FindNextFile(imageHDL, &fileData) != 0);
		numImage = index;
	}
	StringCchCopy(OOO, MAX_LOADSTRING, Somewhere);
	StringCchCat(OOO, MAX_LOADSTRING, _T("\\*.tif"));
	imageHDL = FindFirstFile(OOO, &fileData);
	if (imageHDL == INVALID_HANDLE_VALUE)
	{
		tiff = FALSE;
	}
	else
	{
		index = numImage;
		do
		{
			StringCchCopy(imageLink, MAX_LOADSTRING, Somewhere);
			StringCchCat(imageLink, MAX_LOADSTRING, _T("\\"));
			StringCchCat(imageLink, MAX_LOADSTRING, fileData.cFileName);
			image[index] = Gdiplus::Image::FromFile(imageLink);
			index++;
		}
		while (FindNextFile(imageHDL, &fileData) != 0);
		numImage = index;
	}
	StringCchCopy(OOO, MAX_LOADSTRING, Somewhere);
	StringCchCat(OOO, MAX_LOADSTRING, _T("\\*.gif"));
	imageHDL = FindFirstFile(OOO, &fileData);
	if (imageHDL == INVALID_HANDLE_VALUE)
	{
		gif = FALSE;
	}
	else
	{
		index = numImage;
		do
		{
			StringCchCopy(imageLink, MAX_LOADSTRING, Somewhere);
			StringCchCat(imageLink, MAX_LOADSTRING, _T("\\"));
			StringCchCat(imageLink, MAX_LOADSTRING, fileData.cFileName);
			image[index] = Gdiplus::Image::FromFile(imageLink);
			index++;
		}
		while (FindNextFile(imageHDL, &fileData) != 0);
		numImage = index;
	}
	StringCchCopy(OOO, MAX_LOADSTRING, Somewhere);
	StringCchCat(OOO, MAX_LOADSTRING, _T("\\*.bmp"));
	imageHDL = FindFirstFile(OOO, &fileData);
	if (imageHDL == INVALID_HANDLE_VALUE)
	{
		bmp = FALSE;
	}
	else
	{
		index = numImage;
		do
		{
			StringCchCopy(imageLink, MAX_LOADSTRING, Somewhere);
			StringCchCat(imageLink, MAX_LOADSTRING, _T("\\"));
			StringCchCat(imageLink, MAX_LOADSTRING, fileData.cFileName);
			image[index] = Gdiplus::Image::FromFile(imageLink);
			index++;
		}
		while (FindNextFile(imageHDL, &fileData) != 0);
		numImage = index;
	}
	StringCchCopy(OOO, MAX_LOADSTRING, Somewhere);
	StringCchCat(OOO, MAX_LOADSTRING, _T("\\*.ico"));
	imageHDL = FindFirstFile(OOO, &fileData);
	if (imageHDL == INVALID_HANDLE_VALUE)
	{
		ico = FALSE;
	}
	else
	{
		index = numImage;
		do
		{
			StringCchCopy(imageLink, MAX_LOADSTRING, Somewhere);
			StringCchCat(imageLink, MAX_LOADSTRING, _T("\\"));
			StringCchCat(imageLink, MAX_LOADSTRING, fileData.cFileName);
			image[index] = Gdiplus::Image::FromFile(imageLink);
			index++;
		}
		while (FindNextFile(imageHDL, &fileData) != 0);
		numImage = index;
	}
	StringCchCopy(OOO, MAX_LOADSTRING, Somewhere);
	StringCchCat(OOO, MAX_LOADSTRING, _T("\\*.wmf"));
	imageHDL = FindFirstFile(OOO, &fileData);
	if (imageHDL == INVALID_HANDLE_VALUE)
	{
		wmf = FALSE;
	}
	else
	{
		index = numImage;
		do
		{
			StringCchCopy(imageLink, MAX_LOADSTRING, Somewhere);
			StringCchCat(imageLink, MAX_LOADSTRING, _T("\\"));
			StringCchCat(imageLink, MAX_LOADSTRING, fileData.cFileName);
			image[index] = Gdiplus::Image::FromFile(imageLink);
			index++;
		}
		while (FindNextFile(imageHDL, &fileData) != 0);
		numImage = index;
	}
	StringCchCopy(OOO, MAX_LOADSTRING, Somewhere);
	StringCchCat(OOO, MAX_LOADSTRING, _T("\\*.emf"));
	imageHDL = FindFirstFile(OOO, &fileData);
	if (imageHDL == INVALID_HANDLE_VALUE)
	{
		emf = FALSE;
	}
	else
	{
		index = numImage;
		do
		{
			StringCchCopy(imageLink, MAX_LOADSTRING, Somewhere);
			StringCchCat(imageLink, MAX_LOADSTRING, _T("\\"));
			StringCchCat(imageLink, MAX_LOADSTRING, fileData.cFileName);
			image[index] = Gdiplus::Image::FromFile(imageLink);
			index++;
		}
		while (FindNextFile(imageHDL, &fileData) != 0);
		numImage = index;
	}
	if (png || jpg || bmp || tiff || gif || ico || wmf || emf)
	{
		return TRUE;
	}
	return FALSE;
}

LRESULT WndProcListView(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	int msgboxID;
	char count[10];
	TCHAR folderAdd[MAX_LOADSTRING];
	int num = 1;
	WCHAR folder[10];
	switch (message)
	{
	case WM_GETDLGCODE:
		return (DLGC_WANTALLKEYS | CallWindowProc(winListView, hWnd, message, wParam, lParam));
	case WM_CHAR:
		if ((wParam == VK_RETURN) || (wParam == VK_TAB))
		{
			return 0;
		}
		else
		{
			return (CallWindowProc(winListView, hWnd, message, wParam, lParam));
		}
	case WM_KEYDOWN:
		switch( wParam )
		{
		case VK_TAB:
		case VK_RETURN:
			return FALSE;
			break;
		case VK_DELETE:
			int msgboxID;
			msgboxID= MessageBox(
				NULL,
				L"Do you want to delete it?",
				L"Confirm Delete",
				MB_ICONEXCLAMATION | MB_YESNO
				);
			if (msgboxID == IDYES)
			{
				ListView_DeleteItem(hwndListView,iPos);
				wcscpy(pathDelete,CurrentDir);
				wcscat(pathDelete,_T("\\"));
				wcscat(pathDelete,fileName);
				DeleteFile(pathDelete);
				RemoveDirectory(pathDelete);
				SendToRecycleBin(pathDelete);
				// delete item
				HTREEITEM deleted;
				deleted = GetHandleTV(pathDelete);
				TreeView_DeleteItem(hWndTreeView,deleted);

				HTREEITEM PreRoot;
				PreRoot = GetHandleTV(CurrentDir);
				UpDateTV(PreRoot,CurrentDir);
				ListView_DeleteAllItems(hwndListView);
				if (isThumbnailling)
				{
					ThumbnailListView(CurrentDir, hWnd);
				}
				else
				{
					DirectoryListingLV(CurrentDir, hWnd);
				}
			}
			else
			{
				break;
			}
			break;
		case VK_CONTROL:
			isCtrlDown = 1;
			break;
		case 'R':
			if (isCtrlDown == 1)
			{
				ListView_EditLabel(hwndListView, iPos);
			}
			isCtrlDown = 0;
			break;
		case 'N':			// new folder
			{
				if (checked == 1 || checked == 2)
				{
					wcscpy_s(folderAdd, _T("New Folder"));
					wcscpy_s(Temp, CurrentDir);
					wcscat_s(CurrentDir, _T("\\"));
					wcscat_s(CurrentDir, folderAdd);
					while (CreateDirectory(CurrentDir, NULL) == 0)
					{
						wcscpy_s(CurrentDir, Temp);
						wcscpy_s(folderAdd, _T("New Folder"));
						itoa(num, count, 10);
						count[strlen(count) + 1] = '\0';
						MultiByteToWideChar(0, 0, count, strlen(count) + 1, folder, MAX_LOADSTRING);
						wcscat_s(folderAdd, _T("("));
						wcscat_s(folderAdd, folder);
						wcscat_s(folderAdd, _T(")"));
						wcscat_s(CurrentDir, _T("\\"));
						wcscat_s(CurrentDir, folderAdd);
						num++;
					}
				}
				wcscpy_s(CurrentDir, Temp);
				ListView_DeleteAllItems(hwndListView);
				if (isThumbnailling)
				{
					ThumbnailListView(CurrentDir, hWnd);
				}
				else
				{
					DirectoryListingLV(CurrentDir, hWnd);
				}
				if (checked == 1)
				{
					HTREEITEM update;
					update = GetHandleTV(CurrentDir);
					UpDateTV(update, CurrentDir);
				}
				break; 
			}
		case 'C':
			if (isCtrlDown == 1)
			{
				cutChecked = 0 ;
				wcscpy(TempFile,CurrentDir);
				wcscat(CurrentDir,_T("\\"));
				wcscat(CurrentDir,fileName);
				wcscpy(pathCopy,CurrentDir);
				wcscpy(CurrentDir,TempFile);
				wcscpy(TempFile,fileName);
				wcscpy(pathTemp,pathCopy);
			}
			isCtrlDown = 0;
			break;
		case 'X':
			if (isCtrlDown == 1)
			{
				wcscpy(TempFile,CurrentDir);
				wcscat(CurrentDir,_T("\\"));
				wcscat(CurrentDir,fileName);
				wcscpy(pathCut,CurrentDir);
				wcscpy(CurrentDir,TempFile);
				wcscpy(TempFile,fileName);
				wcscpy(pathTemp,pathCut);
				cutChecked = 1;		//Phan biet cut hay copy
			}
			isCtrlDown = 0;
			break;
		case 'V':
			wcscpy(pathPaste,CurrentDir);
			wcscat(pathPaste,_T("\\"));
			wcscat(pathPaste,TempFile);
			HTREEITEM checkedExist;
			checkedExist = GetHandleTV(pathPaste);
			if(checkedExist != NULL)
			{
				MessageBox(hWnd, L"File has existed", L"ERROR", 0);
				break;
			}
			else
			{
				if (cutChecked == 0)			// neu nhan copy
				{	
					//Copy folder
					wcscpy(pathCopy,pathTemp);
					SHFILEOPSTRUCT copy ;
					copy.hwnd = hWnd;
					copy.wFunc = FO_COPY;
					copy.pFrom = pathCopy;
					copy.pTo = pathPaste;
					copy.fFlags = FOF_SILENT|FOF_NOCONFIRMMKDIR;

					if (IsFolder(pathPaste) == 0)
					{
						CopyFile(pathCopy,pathPaste,FALSE);
					}
					else
					{
						SHFileOperation( &copy );
					}
				}
				if (cutChecked == 1)				//neu nhan cut
				{
					wcscpy(pathCut,pathTemp);
					SHFILEOPSTRUCT cut ;
					cut.hwnd = hWnd;
					cut.wFunc = FO_MOVE;
					cut.pFrom = pathCut;
					cut.pTo = pathPaste;
					cut.fFlags = FOF_SILENT;

					if (IsFolder(pathPaste) == 0)
					{
						MoveFile(pathCut,pathPaste);
					}
					else
					{
						SHFileOperation( &cut );
						HTREEITEM deleted;
						deleted = GetHandleTV(pathCut);
						TreeView_DeleteItem(hWndTreeView,deleted);
					}
				}
				// load lai tree list
				HTREEITEM newRoot, preSibling ;
				newRoot = GetHandleTV(CurrentDir);
				if (IsFolder(CurrentDir) == 1)
				{
					UpDateTV(newRoot,CurrentDir);
					DirectoryListingTV(CurrentDir, newRoot, hWnd);
					DeleteTVExits(newRoot);
				}
				ListView_DeleteAllItems(hwndListView);
				if (isThumbnailling)
				{
					ThumbnailListView(CurrentDir, hWnd);
				}
				else
				{
					DirectoryListingLV(CurrentDir, hWnd);
				}
			}
			isCtrlDown = 0;
			break;
		}
		break;
	case WM_KEYUP:
		switch(wParam)
		{
		case VK_CONTROL:
			isCtrlDown = 0;
			break;
		}
		break;
	default:
		return CallWindowProc(winListView, hWnd, message, wParam, lParam);
	}
	return FALSE;
}

VOID Search(TCHAR* pathNow, TCHAR* fileSearch)
{
	WIN32_FIND_DATA ffd;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	TCHAR path[MAX_PATH], pathNext[MAX_PATH];
	StringCchCopy(path, MAX_PATH, pathNow);
	StringCchCat(path, MAX_PATH, TEXT("\\*"));
	if (wcscmp(pathNow, fileSearch) != 0)
	{
		hFind = FindFirstFile(path, &ffd);
		if (hFind != INVALID_HANDLE_VALUE)
		{
			do 
			{
				if (wcscmp(ffd.cFileName, TEXT(".")) != 0
					&& wcscmp(ffd.cFileName, TEXT("..")) != 0
					&&!	(ffd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN))
				{
					if (KnuthMorrisPratt(fileSearch, ffd.cFileName))
					{
						StringCchCopy(pathLink[numSearchResult], MAX_PATH, pathNow);
						StringCchCat(pathLink[numSearchResult], MAX_PATH, TEXT("\\"));
						StringCchCat(pathLink[numSearchResult], MAX_PATH, ffd.cFileName);
						numSearchResult++;
						Search_OK = TRUE;
					}
					if (ffd.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
					{
						StringCchCopy(pathNext, wcslen(path) , path);
						StringCchCat(pathNext, MAX_PATH, ffd.cFileName);

						Search(pathNext, fileSearch);
					}
				}
			}
			while (FindNextFile(hFind, &ffd));
		}
	}
	FindClose(hFind);
}

BOOL KnuthMorrisPratt(TCHAR* Str1, TCHAR* Str2)
{
	if (wcslen(Str2) < wcslen(Str1))
	{
		return FALSE;
	}
	int count = 0;
	BOOL result = FALSE;
	int NEXT[MAX_PATH];
	NEXT[0] = -1;
	NEXT[1] = 0;
	int sl = -1;

	for (int i = 2; i < wcslen(Str1); i++)
	{
		while ((sl >= 0) && (Str1[sl + 1] != Str1[i - 1]))
		{
			sl = NEXT[sl];
		}
		if (Str1[sl + 1] == Str1[i - 1])
		{
			sl++;
		}
		NEXT[i] = sl + 1;
	}
	int vt = 0;
	int runStr1 = 0;
	int runStr2 = 0;
	while (vt <= (wcslen(Str2) - wcslen(Str1)))
	{
		while (runStr1 < wcslen(Str1))
		{
			if (Str1[runStr1] ==Str2[runStr2])
			{
				{
					runStr2++;
					runStr1++;
				}
			}
			else
			{
				vt = vt + runStr1 - NEXT[runStr1];
				runStr1 = NEXT[runStr1];
				if (runStr1 == -1)
				{
					runStr1 = 0;
				}
				runStr2 = vt + runStr1;
				break;
			}
		}
		if (runStr1 == wcslen(Str1))
		{
			result = TRUE;
			return result;
		}
	}
	return result;
}

INT IsFolder(TCHAR* path)
{
	int result = 1;
	int n =  _tcslen(path);
	for (int i =0; i<n; i++)
	{
		if (path[i] =='.')
		{
			result = 0;
			break;
		}
	}
	return result;
}

VOID DeleteTVExits(HTREEITEM hCur)
{
	HTREEITEM hCurChild,hCurSibling;
	TCHAR buffer[200];
	TVITEM item, itemSibling;	
	hCurChild = TreeView_GetChild(hWndTreeView,hCur);
	item.hItem = hCurChild;
	item.mask = TVIF_TEXT;
	item.cchTextMax = 200;
	item.pszText = buffer;
	TreeView_GetItem(hWndTreeView,&item);

	hCurSibling = TreeView_GetNextSibling(hWndTreeView,hCurChild);
	itemSibling.hItem = hCurSibling;
	itemSibling.mask = TVIF_TEXT;
	itemSibling.cchTextMax = 200;
	itemSibling.pszText = buffer;
	TreeView_GetItem(hWndTreeView,&itemSibling);
	do 
	{
		if (wcscmp(item.pszText,itemSibling.pszText)==0)
		{
			TreeView_DeleteItem(hWndTreeView,hCurChild);
			//hCurChild = TreeView_GetNextSibling(hWndTreeView,hCurChild);
		}
		hCurSibling = TreeView_GetNextSibling(hWndTreeView,hCurSibling);
		TreeView_GetItem(hWndTreeView,&itemSibling);
		if (hCurSibling == NULL)
		{
			hCurChild = TreeView_GetNextSibling(hWndTreeView,hCurChild);
			TreeView_GetItem(hWndTreeView,&item);
			hCurSibling = TreeView_GetNextSibling(hWndTreeView,hCurChild);
			TreeView_GetItem(hWndTreeView,&itemSibling);
		}
	} 
	while (hCurChild != NULL);

}