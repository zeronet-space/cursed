extern "C" {
	void __AsmConstantCondition();
	void __AsmJmpSameTarget();
	void __AsmImpossibleDisassm();
	void __AsmFunctionPointer(DWORD);
	void __AsmReturnPointerAbuse(DWORD64);
#ifndef _WIN64
	void __AsmSEHMisuse();
#endif
};

namespace antidisassm {
	void ConstantCondition() {
		__AsmConstantCondition();
	}
	void AsmJmpSameTarget() {
		__AsmJmpSameTarget();
	}
	void ImpossibleDiasassm() {
		__AsmImpossibleDisassm();
	}
	void ReturnPointerAbuse() {
		__AsmReturnPointerAbuse(666);
	}
	void FunctionPointer() {
		DWORD Number = 2;
		__AsmFunctionPointer(Number);
	}
#ifndef _WIN64
	void AsmSEHMisuse() {
		__AsmSEHMisuse();
	}
#endif
}