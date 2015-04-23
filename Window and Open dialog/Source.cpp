#include <Windows.h>
#include <atlbase.h>
#include <ShObjIdl.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PTSTR pszCmdLine, int nCmdShow)
{
	CoInitialize(NULL);

	CComPtr<IFileOpenDialog> pDialog;
	HRESULT hr = CoCreateInstance(__uuidof(FileOpenDialog), NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pDialog));

	if (SUCCEEDED(hr))
	{
		hr = pDialog->Show(NULL);

		if (SUCCEEDED(hr))
		{
			CComPtr<IShellItem> pItem;
			hr = pDialog->GetResult(&pItem);

			if (SUCCEEDED(hr))
			{
				PWSTR pFileName;
				hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pFileName);

				if (SUCCEEDED(hr))
				{
					MessageBox(NULL, pFileName, L"Result", IDOK);
					CoTaskMemFree(pFileName);
				}
			}
		}
	}


	PTSTR ClassName = L"MainWindow";

	WNDCLASS wc = {};

	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = ClassName;

	ATOM result = RegisterClass(&wc);

	if (result)
	{
		HWND hwnd = CreateWindow(
			ClassName,
			ClassName,
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
			NULL, NULL, hInstance, NULL);

		if (hwnd)
		{
			BOOL result = ShowWindow(hwnd, nCmdShow);

			if (result)
			{
				MSG msg;
				while (GetMessage(&msg, NULL, 0, 0))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}
		}
	}
	CoUninitialize();
	return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);
		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
		EndPaint(hwnd, &ps);
		return S_OK;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		return S_OK;
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}