#ifndef SPACEPI_CORE_JOURNALER_JOURNAL_HPP
#define SPACEPI_CORE_JOURNALER_JOURNAL_HPP

#include <memory>
#include <spacepi/messaging/JournalEntry.pb.h>
#include <spacepi/resource/Filesystem.hpp>
#include <spacepi/resource/Stream.hpp>
#include <spacepi/resource/ResourcePtr.hpp>
#include <spacepi/util/Command.hpp>
#include <spacepi/util/CommandConfigurable.hpp>

namespace spacepi {
    namespace journaler {
        class Journal : private spacepi::util::CommandConfigurable {
            public:
                explicit Journal(spacepi::util::Command &cmd) noexcept;

                void open(bool write);
                bool read(spacepi::messaging::detail::JournalEntry &ent) const;
                void write(const spacepi::messaging::detail::JournalEntry &ent) const;
                void flush() const;

            private:
                void runCommand();

                spacepi::resource::ResourcePtr<spacepi::resource::Filesystem> fs;
                std::shared_ptr<spacepi::resource::Stream> stream;
        };
    }
}

#endif
