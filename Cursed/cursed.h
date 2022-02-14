namespace cursed {
    SOCKET connect(const char* host, int port) {
        WSADATA WSAData;
        SOCKET server;
        SOCKADDR_IN addr;
        WSAStartup(MAKEWORD(2, 0), &WSAData);
        if ((server = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
            spdlog::error("Socket creation failed with error: {}", WSAGetLastError());
            return -1;
        }

        addr.sin_addr.s_addr = inet_addr(host);
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        if (connect(server, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR) {
            spdlog::error("Server connection failed with error: {}", WSAGetLastError());
            return -1;
        }

        spdlog::debug("succefully connected to server.\nhost: {}\nport: {}", host, port);

        return server;
    }

    void destroy(LPVOID lpParam) {
        SOCKET server = *(SOCKET*)lpParam;
        closesocket(server);
        WSACleanup();
    }

    char* c_recv(LPVOID lpParam) {
        char buffer[BUFFER_SIZE] = { NULL };
        SOCKET server = *(SOCKET*)lpParam;
        while (true) {
            if (recv(server, buffer, sizeof(buffer), 0) == SOCKET_ERROR) {
                spdlog::info("recv function failed with error: {}", WSAGetLastError());
                exit(EXIT_FAILURE);
            }
            if (strcmp(buffer, "exit\n") == 0) {
                spdlog::warn("Server disconnected.");
                exit(EXIT_FAILURE);
            }
            return buffer;
        }
    }

    DWORD WINAPI send(LPVOID lpParam, const char* data) {
        SOCKET server = *(SOCKET*)lpParam;
        while (true) {
            if (::send(server, data, sizeof(data), 0) == SOCKET_ERROR) {
                spdlog::error("send failed with error: {}", WSAGetLastError());
                return -1;
            }
        }
        return 1;
    }
}

char* unconstchar(const char* s) {
    if (!s) return NULL;
    int i;
    char* res = NULL;
    res = (char*)malloc(strlen(s) + 1);
    if (!res) {
        fprintf(stderr, "Memory Allocation Failed! Exiting...\n");
        exit(1);
    }
    else {
        for (i = 0; s[i] != '\0'; i++) {
            res[i] = s[i];
        }
        res[i] = '\0';
        return res;
    }
}

DWORD GetMainThreadId(DWORD pid) {
	HANDLE h = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	if (h != INVALID_HANDLE_VALUE) {
		THREADENTRY32 te;
		te.dwSize = sizeof(te);
		if (Thread32First(h, &te)) {
			do {
				if (te.dwSize >= FIELD_OFFSET(THREADENTRY32, th32OwnerProcessID) + sizeof(te.th32OwnerProcessID)) {
					if (te.th32OwnerProcessID == pid) {
						HANDLE hThread = OpenThread(READ_CONTROL, FALSE, te.th32ThreadID);
						if (!hThread) printf("OpenThread");
						else {
							CloseHandle(hThread);
							CloseHandle(h);
							return te.th32ThreadID;
						}
					}
				}
			} while (Thread32Next(h, &te));
		}
		CloseHandle(h);
	}

	return (DWORD)0;
}