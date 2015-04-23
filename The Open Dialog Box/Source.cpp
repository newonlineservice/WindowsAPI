#include <Windows.h>
#include <ShObjIdl.h>
#include <atlbase.h>


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, INT nCmdShow)
{
	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

	if (SUCCEEDED(hr))
	{
		CComPtr<IFileOpenDialog> pDialog;
		hr = CoCreateInstance(__uuidof(FileOpenDialog), NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pDialog));

		if (SUCCEEDED(hr))
		{
			hr = pDialog->Show(NULL);

			if (SUCCEEDED(hr))
			{
				CComPtr<IShellItem> pItem;

				hr = pDialog->GetResult(&pItem);

				if (SUCCEEDED(hr))
				{
					LPWSTR pszFilePath;

					hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

					if (SUCCEEDED(hr))
					{
						MessageBox(NULL, pszFilePath, L"Selected Item", IDOK);
						CoTaskMemFree(pszFilePath);
					}
				}
			}
		}
		CoUninitialize();
	}
	return 0;
}