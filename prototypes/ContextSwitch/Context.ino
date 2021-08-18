/**
 * \brief Class to hold data needed for context switching
 *
 * \see initContextAndSwitch
 * \see contextSwitch
 */
class Context {
    public:
        /**
         * \brief Switch from the current context to a new context
         *
         * This takes approximately the following steps:
         * \li Store current context into \c fromContext
         * \li Initialize \c toContext
         * \li Switch to \c toContext
         * \li Call \c func(arg)
         *
         * \param[in,out] fromContext The context before calling this function
         * \param[out] toContext The context after calling this function (the new context)
         * \param[in] func The function to start in the new context
         * \param[in] arg The argument to \c func
         */
        static void initAndSwitch(Context &fromContext, Context &toContext, void (*func)(void *arg), void *arg);

        /**
         * \brief Switch from one context to another
         *
         * \param[in,out] fromContext The context before calling this function
         * \param[in] toContext The context to switch to (must already be initialized by initAndSwitch)
         */
        static void contextSwitch(Context &fromContext, const Context &toContext);

    private:
        /**
         * \brief The address of the next instruction to run in the context
         */
        int returnAddress;
};

void Context::initAndSwitch(Context &fromContext, Context &toContext, void (*func)(void *arg), void *arg) {
    // TODO
    printf("\nBREAKPOINT: AA: fromContext: 0x%x \t toContext: 0x%x \t func: %p \t AB ADDRESS: 0x%x",fromContext.returnAddress, toContext.returnAddress, (*func), AddrChecker());
    __asm__ volatile( // Store current context into fromContext
      "mov %0, $a0\n"
      : "=r" (fromContext.returnAddress) 
    );
    func(arg); // Call func(arg)
}
//12 35 64 7
void Context::contextSwitch(Context &fromContext, const Context &toContext) {
    // TODO 
//    printf("\nBREAKPOINT: BA: fromContext: 0x%x \t toContext: 0x%x (%u) \t BA ADDRESS: 0x%x",fromContext.returnAddress, toContext.returnAddress, toContext.returnAddress, AddrChecker());
    __asm__ volatile(
      "mov %0, $a0\n"
      "mov $a0, %1\n"
      : "=&r" (fromContext.returnAddress) 
      : "r" (toContext.returnAddress)
    );
}
