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
}

void Context::contextSwitch(Context &fromContext, const Context &toContext) {
    // TODO
}
