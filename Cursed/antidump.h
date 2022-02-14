#define ENV64BIT
namespace antidump {
	void ErasePEHeaderFromMemory() {
		spdlog::debug("Erasing PE header from memory");
		DWORD OldProtect = 0;

		char* pBaseAddr = (char*)GetModuleHandle(NULL);

		VirtualProtect(pBaseAddr, 4096, PAGE_READWRITE, &OldProtect);
		SecureZeroMemory(pBaseAddr, 4096);
	}
	void SizeOfImage() {
#if defined(ENV64BIT)
		PPEB pPeb = (PPEB)__readgsqword(0x60);
#elif defined(ENV32BIT)
		PPEB pPeb = (PPEB)__readfsdword(0x30);
#endif

		spdlog::debug("Increasing SizeOfImage in PE Header to: 0x1000000");

		PLIST_ENTRY InLoadOrderModuleList = (PLIST_ENTRY)pPeb->Ldr->Reserved2[1];
		PLDR_DATA_TABLE_ENTRY tableEntry = CONTAINING_RECORD(InLoadOrderModuleList, LDR_DATA_TABLE_ENTRY, Reserved1[0]);
		PULONG pEntrySizeOfImage = (PULONG)&tableEntry->Reserved3[1];
		*pEntrySizeOfImage = (ULONG)((INT_PTR)tableEntry->DllBase + 0x1000000);
	}
}