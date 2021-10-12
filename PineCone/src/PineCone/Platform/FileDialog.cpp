#include "pcpch.h"
#include "FileDialog.h"

namespace Pine {

    #ifdef WIN32

    std::filesystem::path FileDialog::OpenFile(const std::vector<Filter>& filters)
    {
        std::filesystem::path result;

        HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
            COINIT_DISABLE_OLE1DDE);
        if (SUCCEEDED(hr))
        {
            IFileOpenDialog* pFileOpen;

            // Create the FileOpenDialog object.
            hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
                IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

            if (SUCCEEDED(hr))
            {
                COMDLG_FILTERSPEC* winFilter = new COMDLG_FILTERSPEC[filters.size()];
                if (filters.size() > 0) {
                    // Setup filter
                    for (uint32_t i = 0; i < filters.size(); i++) {
                        winFilter[i] = { filters[i].HumanReadableName.c_str(), filters[i].AcceptedTypes.c_str() };
                    }
                    hr = pFileOpen->SetFileTypes(filters.size(), winFilter);
                    pFileOpen->SetDefaultExtension(filters[0].AcceptedTypes.c_str());
                }

                // Show the Open dialog box.
                hr = pFileOpen->Show(NULL);

                // Get the file name from the dialog box.
                if (SUCCEEDED(hr))
                {
                    IShellItem* pItem;
                    hr = pFileOpen->GetResult(&pItem);
                    if (SUCCEEDED(hr))
                    {
                        PWSTR pszFilePath;
                        hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

                        // Display the file name to the user.
                        if (SUCCEEDED(hr))
                        {
                            result = pszFilePath;
                            CoTaskMemFree(pszFilePath);
                        }
                        pItem->Release();
                    }
                }

                delete[] winFilter;

                pFileOpen->Release();
            }
            CoUninitialize();
        }

        return result;
    }

    std::filesystem::path FileDialog::SaveFile(const std::vector<Filter>& filters)
    {
        std::filesystem::path result;

        HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
            COINIT_DISABLE_OLE1DDE);
        if (SUCCEEDED(hr))
        {
            IFileSaveDialog* pFileSave;

            // Create the FileSaveDialog object.
            hr = CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_ALL,
                IID_IFileSaveDialog, reinterpret_cast<void**>(&pFileSave));

            if (SUCCEEDED(hr))
            {
                COMDLG_FILTERSPEC* winFilter = new COMDLG_FILTERSPEC[filters.size()];
                if (filters.size() > 0) {
                    // Setup filter
                    for (uint32_t i = 0; i < filters.size(); i++) {
                        winFilter[i] = { filters[i].HumanReadableName.c_str(), filters[i].AcceptedTypes.c_str() };
                    }
                    pFileSave->SetFileTypes(filters.size(), winFilter);
                    pFileSave->SetDefaultExtension(filters[0].AcceptedTypes.c_str());
                }

                // Show the Open dialog box.
                hr = pFileSave->Show(NULL);

                // Get the file name from the dialog box.
                if (SUCCEEDED(hr))
                {
                    IShellItem* pItem;
                    hr = pFileSave->GetResult(&pItem);
                    if (SUCCEEDED(hr))
                    {
                        PWSTR pszFilePath;
                        hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

                        // Display the file name to the user.
                        if (SUCCEEDED(hr))
                        {
                            result = pszFilePath;
                            CoTaskMemFree(pszFilePath);
                        }
                        pItem->Release();
                    }
                }

                delete[] winFilter;

                pFileSave->Release();
            }
            CoUninitialize();
        }

        return result;
    }

    #endif

}