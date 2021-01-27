#ifndef SPACEPI_TARGETLIB_LINUX_FSOPERATION_HPP
#define SPACEPI_TARGETLIB_LINUX_FSOPERATION_HPP

namespace spacepi {
    namespace spacepictl {
        class FSTransaction;

        class FSOperation {
            friend class FSTransaction;

            public:
                FSOperation() = default;
                FSOperation(FSOperation &) = delete;
                FSOperation &operator =(FSOperation &) = delete;

            protected:
                void handle(const std::string &msg, int res);

            private:
                virtual void perform() = 0;
                virtual void undo() = 0;
                virtual void finalize() = 0;
        };
    }
}

#endif
