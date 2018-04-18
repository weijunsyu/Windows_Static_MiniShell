#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <stdio.h>

#define MAX_LEN 200


int main(int argc, char *argv[], char **envp) {
	
	FILE* file;
	char line[MAX_LEN];
	
	if (argc != 2) {
		printf("Please pass valid single argument\n");
		return 0;
	}

	
	file = fopen(argv[1], "r"); //open file
	while (fgets(line, sizeof(line), file)) { //iterate over file by line
		
		STARTUPINFO si;
		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);
		PROCESS_INFORMATION pi;
		ZeroMemory(&pi, sizeof(pi));

		if (CreateProcess(
			NULL, // Application name
			line, // Application arguments
			NULL, // Process handle not inheritable
			NULL, // Thread handle not inheritable
			FALSE, // Set handle inheritance
			CREATE_NEW_CONSOLE, // Creation flags
			NULL, // Use parent's environment block
			NULL, // Use parent's starting directory
			&si, // Pointer to STARTUPINFO structure
			&pi) // Pointer to PROCESS_INFORMATION structure
			== FALSE) {

			// most common problem is that the program was not found.
			printf("could not create process with path = (%s)\n", line);
			// send error information to stderr stream
			fprintf(stderr, "CreateProcess() failed on error %d\n", GetLastError());
		}
		// Wait until child process exits. (look this up on MSDN)
		WaitForSingleObject(pi.hProcess, INFINITE);
		// Close process and thread handles.
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}

	fclose(file);

	return 0;
}

