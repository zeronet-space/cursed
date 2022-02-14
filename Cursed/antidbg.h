namespace antidebug {
    void debug_break() {
        __try {
            DebugBreak();
            //exit(EXIT_FAILURE);
        }
        __except (GetExceptionCode() == EXCEPTION_BREAKPOINT ?
            EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH) {}
    }
}