#include "antidbg.h"
#include "antiinject.h"
#include "antidisassm.h"
#include "antidump.h"

namespace security {
	void protect() {
		spdlog::debug("running antidump::ErasePEHeaderFromMemory");
		antidump::ErasePEHeaderFromMemory();
		spdlog::debug("running antidump::SizeOfImage");
		antidump::SizeOfImage();

		spdlog::debug("running antidisassm::ConstantCondition");
		antidisassm::ConstantCondition();
		spdlog::debug("running antidisassm::AsmJmpSameTarget");
		antidisassm::AsmJmpSameTarget();
		spdlog::debug("running antidisassm::ImpossibleDiasassm");
		antidisassm::ImpossibleDiasassm();
		spdlog::debug("running antidisassm::ReturnPointerAbuse");
		antidisassm::ReturnPointerAbuse();
		spdlog::debug("running antidisassm::FunctionPointer");
		antidisassm::FunctionPointer();

#ifndef _WIN64
		spdlog::debug("running antidisassm::SEHMisuse");
		antidisassm::SEHMisuse();
#endif

		spdlog::debug("checking for debugger");
		if (IsDebuggerPresent()) {
			SetLastError(0);
			OutputDebugStringA("Hello, debugger :)\n");
			if (GetLastError() == 0) {
				antidebug::debug_break();
			}
		}
		spdlog::debug("no debugger detected");

		spdlog::debug("creating antiinject::AntiInject thread");
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)AntiInject, 0, 0, 0);
	}
}