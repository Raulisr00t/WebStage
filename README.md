# Payload Downloader

This project is a simple example of downloading a payload from a given URL using the WinINet API in C. The code demonstrates how to initialize a WinINet session, open a URL, read the data from the URL, and handle errors appropriately.

## Prerequisites

- Windows Operating System
- Visual Studio or any other C compiler that supports Windows API
- Internet connection

## Building the Project

To compile the code, follow these steps:

1. Open your terminal or command prompt.
2. Navigate to the directory containing the `main.c` file.
3. Run the following command:

   ```sh
   gcc -o downloader main.c -lwininet
   ```

## Usage
Make sure your server is running and serving the payload file at http://127.0.0.1:8000/payload.exe.

Run the compiled executable:
```shell
./downloader
```
If the payload is downloaded successfully, you will see:
```shell
[+] Payload downloaded successfully.
```
Otherwise, you will see an error message indicating what went wrong.

## Code Explanation
### Includes and Definitions
```c
#include <stdio.h>
#include <windows.h>
#include <wininet.h>
```

### Function: GetPayloadFromUrl
This function handles the downloading process. It performs the following steps:

Initialize a WinINet session with InternetOpenW.
Open the specified URL with InternetOpenUrlW.
Allocate memory and read the data from the URL in chunks of 1024 bytes.
Resize the buffer as needed and copy the data into it.
Clean up resources and close handles.

### Function: main
This is the entry point of the program. It calls GetPayloadFromUrl and prints the result.
```c
int main(int argc, char *argv[]) {
    if (GetPayloadFromUrl()) {
        printf("[+] Payload downloaded successfully.\n");
    } else {
        printf("[!] Failed to download payload.\n");
    }
    return 0;
}
```

## Error Handling
The code includes error handling to provide informative messages if any WinINet functions fail. It uses GetLastError to retrieve and display the specific error code.

## Memory Management
The code allocates memory dynamically as it reads the data. It ensures that all allocated memory is freed before the program exits, preventing memory leaks.

## Security Considerations
Make sure the URL and server you are connecting to are trusted to avoid downloading malicious payloads.
Validate and sanitize any inputs if modifying this code for use with dynamic URLs.

##License
This project is licensed under the MIT License.
