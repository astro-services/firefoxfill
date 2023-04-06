#include <Windows.h>
#include <commctrl.h>
#include <exdisp.h>
#include <mshtml.h>
#include <string>
#include <iostream>

// Global variables
HWND g_hwnd;
HWND g_editName;
HWND g_editEmail;
HWND g_editMessage;
HWND g_buttonSubmit;

// Function to initialize the GUI
void InitGUI()
{
    // Create the main window
    g_hwnd = CreateWindowEx(
        0,
        L"STATIC",
        L"AutoFill",
        WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT,
        300, 200,
        NULL, NULL,
        GetModuleHandle(NULL), NULL);

    // Create the "Name" label
    HWND labelName = CreateWindowEx(
        0,
        L"STATIC",
        L"Name:",
        WS_CHILD | WS_VISIBLE,
        10, 10,
        50, 20,
        g_hwnd, NULL,
        GetModuleHandle(NULL), NULL);

    // Create the "Name" edit box
    g_editName = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        L"EDIT",
        L"",
        WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL,
        70, 10,
        200, 20,
        g_hwnd, NULL,
        GetModuleHandle(NULL), NULL);

    // Create the "Email" label
    HWND labelEmail = CreateWindowEx(
        0,
        L"STATIC",
        L"Email:",
        WS_CHILD | WS_VISIBLE,
        10, 40,
        50, 20,
        g_hwnd, NULL,
        GetModuleHandle(NULL), NULL);

    // Create the "Email" edit box
    g_editEmail = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        L"EDIT",
        L"",
        WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL,
        70, 40,
        200, 20,
        g_hwnd, NULL,
        GetModuleHandle(NULL), NULL);

    // Create the "Message" label
    HWND labelMessage = CreateWindowEx(
        0,
        L"STATIC",
        L"Message:",
        WS_CHILD | WS_VISIBLE,
        10, 70,
        50, 20,
        g_hwnd, NULL,
        GetModuleHandle(NULL), NULL);

    // Create the "Message" edit box
    g_editMessage = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        L"EDIT",
        L"",
        WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL,
        70, 70,
        200, 60,
        g_hwnd, NULL,
        GetModuleHandle(NULL), NULL);

    // Create the "Submit" button
    g_buttonSubmit =
CreateWindowEx(
    0,
    L"BUTTON",
    L"Submit",
    WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
    100, 140,
    100, 30,
    g_hwnd, NULL,
    GetModuleHandle(NULL), NULL);

// Show the window
ShowWindow(g_hwnd, SW_SHOW);

// Get the handle to the browser object
IWebBrowser2* browser;
HRESULT hr = AccessibleObjectFromWindow(ieHwnd, OBJID_CLIENT, IID_IWebBrowser2, (void**)&browser);
if (FAILED(hr) || !browser)
{
    std::cerr << "Error getting browser object" << std::endl;
    return;
}

// Get the HTML document object
IDispatch* dispatch;
hr = browser->get_Document(&dispatch);
if (FAILED(hr) || !dispatch)
{
    std::cerr << "Error getting HTML document object" << std::endl;
    return;
}

// Get the HTML document interface
IHTMLDocument2* document;
hr = dispatch->QueryInterface(IID_IHTMLDocument2, (void**)&document);
if (FAILED(hr) || !document)
{
    std::cerr << "Error getting HTML document interface" << std::endl;
    return;
}

// Get the form elements
IHTMLElement* elementName = NULL;
IHTMLElement* elementEmail = NULL;
IHTMLElement* elementMessage = NULL;
hr = document->getElementById(L"name", &elementName);
hr = document->getElementById(L"email", &elementEmail);
hr = document->getElementById(L"message", &elementMessage);

// Fill in the form elements
std::wstring name = L"";
std::wstring email = L"";
std::wstring message = L"";
int nameLength = GetWindowTextLength(g_editName) + 1;
int emailLength = GetWindowTextLength(g_editEmail) + 1;
int messageLength = GetWindowTextLength(g_editMessage) + 1;
wchar_t* nameBuffer = new wchar_t[nameLength];
wchar_t* emailBuffer = new wchar_t[emailLength];
wchar_t* messageBuffer = new wchar_t[messageLength];
GetWindowText(g_editName, nameBuffer, nameLength);
GetWindowText(g_editEmail, emailBuffer, emailLength);
GetWindowText(g_editMessage, messageBuffer, messageLength);
name = std::wstring(nameBuffer);
email = std::wstring(emailBuffer);
message = std::wstring(messageBuffer);
elementName->put_value(name.c_str());
elementEmail->put_value(email.c_str());
elementMessage->put_value(message.c_str());

// Submit the form
IHTMLElement* elementSubmit = NULL;
hr = document->getElementById(L"submit", &elementSubmit);
elementSubmit->click();

// Clean up
delete[] nameBuffer;
delete[] emailBuffer;
delete[] messageBuffer;
document->Release();
dispatch->Release();
browser->Release();

// Create the main window
CreateMainWindow(hInstance, nCmdShow);

// Message loop
MSG msg;
while (GetMessage(&msg, NULL, 0, 0))
{
    TranslateMessage(&msg);
    DispatchMessage(&msg);
}

return msg.wParam;

    case WM_COMMAND:
        if (LOWORD(wParam) == ID_SUBMIT_BUTTON)
        {
            // Fill in the form and submit
            AutoFill();
        }
        break;

    case WM_DESTROY:
        // Clean up and exit
        PostQuitMessage(0);
        break;

    default:
        // Handle any other messages
        return DefWindowProc(hwnd, msg, wParam, lParam);
}

return 0;

// Create the window
HWND hwnd = CreateWindowEx(0, L"MainWindowClass", L"AutoFill", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 400, 240, NULL, NULL, hInstance, NULL);
if (!hwnd)
{
    std::cerr << "Error creating main window" << std::endl;
    return;
}

// Show the window
ShowWindow(hwnd, nCmdShow);
UpdateWindow(hwnd);

// Create the name edit box
g_editName = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", NULL, WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, 100, 20, 240, 20, hwnd, NULL, NULL, NULL);

// Create the email label
CreateWindowEx(0, L"STATIC", L"Email:", WS_CHILD | WS_VISIBLE, 20, 60, 80, 20, hwnd, NULL, NULL, NULL);

// Create the email edit box
g_editEmail = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", NULL, WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, 100, 60, 240, 20, hwnd, NULL, NULL, NULL);

// Create the message label
CreateWindowEx(0, L"STATIC", L"Message:", WS_CHILD | WS_VISIBLE, 20, 100, 80, 20, hwnd, NULL, NULL, NULL);

// Create the message edit box
g_editMessage = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", NULL, WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | ES_MULTILINE | ES_WANTRETURN, 100, 100, 240, 60, hwnd, NULL, NULL, NULL);

// Create the submit button
CreateWindowEx(0, L"BUTTON", L"Submit", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 100, 180, 100, 30, hwnd, (HMENU)ID_SUBMIT_BUTTON, NULL, NULL);

// Get the window handle for the Firefox browser
HWND hwndBrowser = FindWindow(L"MozillaWindowClass", NULL);
if (!hwndBrowser)
{
    std::cerr << "Firefox window not found" << std::endl;
    return;
}

// Get the child window handle for the web page
HWND hwndPage = FindWindowEx(hwndBrowser, NULL, L"MozillaContentWindowClass", NULL);
if (!hwndPage)
{
    std::cerr << "Firefox content window not found" << std::endl;
    return;
}

// Set focus to the web page
SetForegroundWindow(hwndPage);

// Fill in the name field
SendMessage(g_editName, WM_SETTEXT, 0, (LPARAM)L"John Doe");

// Fill in the email field
SendMessage(g_editEmail, WM_SETTEXT, 0, (LPARAM)L"johndoe@example.com");

// Fill in the message field
SendMessage(g_editMessage, WM_SETTEXT, 0, (LPARAM)L"This is an auto-filled message");

// Submit the form
keybd_event(VK_RETURN, 0, 0, 0);
keybd_event(VK_RETURN, 0, KEYEVENTF_KEYUP, 0);

// Create the main window
HWND hwndMain = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
if (!hwndMain)
{
    return FALSE;
}

// Show the main window
ShowWindow(hwndMain, SW_SHOW);
UpdateWindow(hwndMain);

// Enter the message loop
MSG msg = { 0 };
while (GetMessage(&msg, nullptr, 0, 0))
{
    TranslateMessage(&msg);
    DispatchMessage(&msg);
}

return (int)msg.wParam;

// Load the menu
HMENU hMenu = LoadMenu(hInstance, MAKEINTRESOURCE(IDR_MENU1));
if (!hMenu)
{
    return FALSE;
}

// Save the instance handle
hInst = hInstance;

// Create the main window
HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, hMenu, hInstance, nullptr);
if (!hWnd)
{
    return FALSE;
}

// Show the main window
ShowWindow(hWnd, nCmdShow);
UpdateWindow(hWnd);

// Enter the message loop
MSG msg = {};
while (GetMessage(&msg, nullptr, 0, 0))
{
    TranslateMessage(&msg);
    DispatchMessage(&msg);
}

return (int)msg.wParam;


