# spacepi_dependencies(
#     [OPTIONAL]
#
#     [PROTOC]
#     [LIBPROTOBUF]
#     [LIBPROTOC]
#
#     [JAVA_JDK]
#
#     [DOXYGEN]
#     [DOXYGEN_DOT]
#
#     [PYTHON3]
#     [PYTHON3_PIP]
#
#     [GIT_EXE]
#     [GIT_LIB]
#
#     [BLKID_LIB]
#
#     [DEBOOTSTRAP]
#     [KPARTX]
#     [MKFS]
#     [SFDISK]
#     [SU]
#     [SUDO]
#     [USERADD]
#
#     [GPIOD]
#     [I2C]
#     [OPENSSL]
#     [OPENSSL_CRYPTO]
#     [SYSTEMCTL]
#     [DTC]
#     [SSH]
#
#     [QEMU_USER_STATIC <arch>]
#
#     [BOOST_ASIO]
#     [BOOST_BEAST]
#     [BOOST_CONFIG]
#     [BOOST_CONTEXT]
#     [BOOST_EXCEPTION]
#     [BOOST_FIBER]
#     [BOOST_FILESYSTEM]
#     [BOOST_PROCESS]
#     [BOOST_PROPERTY_TREE]
#     [BOOST_STACKTRACE_BACKTRACE]
#     [BOOST_SYSTEM]
#     [BOOST_THREAD]
# )
function (spacepi_dependencies)
    set(supportedPkgs
        PROTOC
        LIBPROTOBUF
        LIBPROTOC

        JAVA_JDK

        DOXYGEN
        DOXYGEN_DOT

        PYTHON3
        PYTHON3_PIP

        GIT_EXE
        GIT_LIB

        BLKID_LIB

        DEBOOTSTRAP
        KPARTX
        MKFS
        SFDISK
        SU
        SUDO
        USERADD

        GPIOD
        I2C
        OPENSSL
        OPENSSL_CRYPTO
        SYSTEMCTL
        DTC
        SSH

        BOOST_ASIO
        BOOST_BEAST
        BOOST_CONFIG
        BOOST_CONTEXT
        BOOST_EXCEPTION
        BOOST_FIBER
        BOOST_FILESYSTEM
        BOOST_PROCESS
        BOOST_PROPERTY_TREE
        BOOST_STACKTRACE_BACKTRACE
        BOOST_SYSTEM
        BOOST_THREAD
    )
    set(supportedTemplates
        QEMU_USER_STATIC
    )
    cmake_parse_arguments(SPACEPI_DEPS "OPTIONAL;${supportedPkgs}" "${supportedTemplates}" "" ${ARGN})

    set(optionalFlag)
    if (SPACEPI_DEPS_OPTIONAL)
        set(optionalFlag "OPTIONAL")
    endif()

    foreach (pkg IN LISTS SPACEPI_DEPS_UNPARSED_ARGUMENTS)
        message(SEND_ERROR "Unknown dependency: ${pkg}")
    endforeach()

    if (SPACEPI_DEPS_PROTOC)
        spacepi_dependency(
            protoc
            TARGET PROTOC
            PROGRAM_NAMES protoc
            PACKAGES protobuf-compiler
            ${optionalFlag}
        )
    endif()

    if (SPACEPI_DEPS_LIBPROTOBUF)
        spacepi_dependency(
            protobuf
            TARGET LIBPROTOBUF
            LIBRARY_NAMES protobuf
            PACKAGES libprotobuf-dev
            ${optionalFlag}
        )
    endif()

    if (SPACEPI_DEPS_LIBPROTOC)
        spacepi_dependency(
            libprotoc
            TARGET LIBPROTOC
            LIBRARY_NAMES protoc
            PACKAGES libprotoc-dev
            ${optionalFlag}
        )
    endif()

    if (SPACEPI_DEPS_JAVA_JDK)
        spacepi_dependency(
            java-jdk
            TARGET JAVA_JDK
            PROGRAM_NAMES javac
            PACKAGES openjdk-11-jdk-headless
            ${optionalFlag}
        )
    endif()

    if (SPACEPI_DEPS_DOXYGEN)
        spacepi_dependency(
            doxygen
            TARGET DOXYGEN
            PROGRAM_NAMES doxygen
            PACKAGES doxygen
            ${optionalFlag}
        )
    endif()

    if (SPACEPI_DEPS_DOXYGEN_DOT)
        spacepi_dependency(
            doxygen::dot
            TARGET DOXYGEN_DOT
            PROGRAM_NAMES dot
            PACKAGES graphviz
            ${optionalFlag}
        )
    endif()

    if (SPACEPI_DEPS_PYTHON3 OR SPACEPI_DEPS_PYTHON3)
        spacepi_dependency(
            python3
            TARGET PYTHON3
            PROGRAM_NAMES python3
            PACKAGES python3
            ${optionalFlag}
        )
    endif()

    if (SPACEPI_DEPS_PYTHON3_PIP AND TARGET PYTHON3 AND NOT TARGET PYTHON3_PIP)
        if (NOT SPACEPI_DEP_PYTHON3_PIP_FOUND)
            spacepi_dependency_message(CHECK_START "pip3")
            get_property(loc TARGET PYTHON3 PROPERTY IMPORTED_LOCATION)
            execute_process(
                COMMAND ${loc} -m pip help
                RESULT_VARIABLE found
                OUTPUT_QUIET
                ERROR_QUIET
            )
            if (found EQUAL 0)
                spacepi_dependency_message(CHECK_PASS "pip3")

                set(SPACEPI_DEP_PYTHON3_PIP_FOUND TRUE CACHE INTERNAL "")
            else()
                spacepi_dependency_message(CHECK_FAIL "pip3")
                
                spacepi_dependency_fail(
                    "pip3"
                    ${optionalFlag}
                    PACKAGES python3-pip
                )
            endif()
        endif()
                    
        if (SPACEPI_DEP_PYTHON3_PIP_FOUND)
            add_executable(PYTHON3_PIP IMPORTED GLOBAL)
        endif()
    endif()

    if (SPACEPI_DEPS_GIT_EXE)
        spacepi_dependency(
            git
            TARGET GIT_EXE
            PROGRAM_NAMES git
            PACKAGES git
            ${optionalFlag}
        )
    endif()

    if (SPACEPI_DEPS_GIT_LIB)
        spacepi_dependency(
            libgit2
            TARGET GIT_LIB
            LIBRARY_NAMES git2
            HEADERS git2.h
            PACKAGES libgit2-dev
            ${optionalFlag}
        )
    endif()

    if (SPACEPI_DEPS_BLKID_LIB)
        spacepi_dependency(
            libblkid
            TARGET BLKID_LIB
            LIBRARY_NAMES blkid
            HEADERS blkid/blkid.h
            PACKAGES libblkid-dev
            ${optionalFlag}
        )
    endif()

    if (SPACEPI_DEPS_DEBOOTSTRAP)
        spacepi_dependency(
            debootstrap
            TARGET DEBOOTSTRAP
            PROGRAM_NAMES debootstrap
            PACKAGES debootstrap
            ${optionalFlag}
        )
    endif()

    if (SPACEPI_DEPS_KPARTX)
        spacepi_dependency(
            kpartx
            TARGET KPARTX
            PROGRAM_NAMES kpartx
            PACKAGES kpartx
            ${optionalFlag}
        )
    endif()

    if (SPACEPI_DEPS_MKFS)
        spacepi_dependency(
            mkfs
            TARGET MKFS
            PROGRAM_NAMES mkfs
            PACAKGES util-linux
            ${optionalFlag}
        )
    endif()

    if (SPACEPI_DEPS_SFDISK)
        spacepi_dependency(
            sfdisk
            TARGET SFDISK
            PROGRAM_NAMES sfdisk
            PACKAGES sfdisk
            ${optionalFlag}
        )
    endif()

    if (SPACEPI_DEPS_SU)
        spacepi_dependency(
            su
            TARGET SU
            PROGRAM_NAMES su
            PACKAGES util-linux
            ${optionalFlag}
        )
    endif()

    if (SPACEPI_DEPS_SUDO)
        spacepi_dependency(
            sudo
            TARGET SUDO
            PROGRAM_NAMES sudo
            PACKAGES sudo
            ${optionalFlag}
        )
    endif()

    if (SPACEPI_DEPS_USERADD)
        spacepi_dependency(
            useradd
            TARGET USERADD
            PROGRAM_NAMES useradd
            PACKAGES passwd
            ${optionalFlag}
        )
    endif()

    if (SPACEPI_DEPS_GPIOD)
        spacepi_dependency(
            gpiod
            TARGET GPIOD
            LIBRARY_NAMES gpiodcxx
            HEADERS gpiod.hpp
            PACKAGES libgpiod-dev
            ${optionalFlag}
        )
    endif()

    if (SPACEPI_DEPS_I2C)
        spacepi_dependency(
            i2c
            TARGET I2C
            LIBRARY_NAMES i2c
            HEADERS i2c/smbus.h
            PACKAGES libi2c-dev
            ${optionalFlag}
        )
    endif()

    if (SPACEPI_DEPS_OPENSSL)
        spacepi_dependency(
            openssl
            TARGET OPENSSL
            LIBRARY_NAMES libssl.so ssl
            HEADERS openssl/ssl.h
            PACKAGES libssl-dev
            ${optionalFlag}
        )
    endif()

    if (SPACEPI_DEPS_OPENSSL_CRYPTO)
        spacepi_dependency(
            openssl::crypto
            TARGET OPENSSL_CRYPTO
            LIBRARY_NAMES libcrypto.so crypto
            HEADERS openssl/ssl.h
            PACKAGES libssl-dev
            ${optionalFlag}
        )
    endif()

    if (SPACEPI_DEPS_SYSTEMCTL)
        spacepi_dependency(
            systemctl
            TARGET SYSTEMCTL
            PROGRAM_NAMES systemctl
            PACKAGES systemd
            ${optionalFlag}
        )
    endif()

    if (SPACEPI_DEPS_DTC)
        spacepi_dependency(
            dtc
            TARGET DTC
            PROGRAM_NAMES dtc
            PACKAGES device-tree-compiler
            ${optionalFlag}
        )
    endif()

    if (SPACEPI_DEPS_SSH)
        spacepi_dependency(
            ssh
            TARGET SSH
            PROGRAM_NAMES ssh
            PACKAGES openssh-client
            ${optionalFlag}
        )
    endif()

    if (SPACEPI_DEPS_QEMU_USER_STATIC)
        spacepi_dependency(
            qemu-${SPACEPI_DEPS_QEMU_USER_STATIC}-static
            TARGET QEMU_USER_STATIC_${SPACEPI_DEPS_QEMU_USER_STATIC}
            PROGRAM_NAMES qemu-${SPACEPI_DEPS_QEMU_USER_STATIC}-static
            PACKAGES qemu-user-static
            ${optionalFlag}
        )
    endif()

    if (SPACEPI_DEPS_BOOST_ASIO)
        spacepi_dependency(
            boost::asio
            TARGET BOOST_ASIO
            HEADERS boost/asio.hpp
            PACKAGES libboost-dev
            ${optionalFlag}
        )
    endif()

    if (SPACEPI_DEPS_BOOST_BEAST)
        spacepi_dependency(
            boost::beast
            TARGET BOOST_BEAST
            HEADERS boost/beast.hpp
            PACKAGES libboost-dev
            ${optionalFlag}
        )
    endif()

    if (SPACEPI_DEPS_BOOST_CONFIG)
        spacepi_dependency(
            boost::config
            TARGET BOOST_CONFIG
            HEADERS boost/config.hpp
            PACKAGES libboost-dev
            ${optionalFlag}
        )
    endif()

    if (SPACEPI_DEPS_BOOST_CONTEXT)
        spacepi_dependency(
            boost::context
            TARGET BOOST_CONTEXT
            LIBRARY_NAMES boost_context
            HEADERS boost/context/all.hpp
            PACKAGES libboost-context-dev
            ${optionalFlag}
        )
    endif()

    if (SPACEPI_DEPS_BOOST_EXCEPTION)
        spacepi_dependency(
            boost::exception
            TARGET BOOST_EXCEPTION
            LIBRARY_NAMES boost_exception
            HEADERS boost/exception/all.hpp
            PACKAGES libboost-exception-dev
            ${optionalFlag}
        )
    endif()

    if (SPACEPI_DEPS_BOOST_FIBER)
        spacepi_dependency(
            boost::fiber
            TARGET BOOST_FIBER
            LIBRARY_NAMES boost_fiber
            HEADERS boost/fiber/all.hpp
            PACKAGES libboost-fiber-dev
            ${optionalFlag}
        )
    endif()

    if (SPACEPI_DEPS_BOOST_FILESYSTEM)
        spacepi_dependency(
            boost::filesystem
            TARGET BOOST_FILESYSTEM
            LIBRARY_NAMES boost_filesystem
            HEADERS boost/filesystem.hpp
            PACKAGES libboost-filesystem-dev
            ${optionalFlag}
        )
    endif()

    if (SPACEPI_DEPS_BOOST_PROCESS)
        spacepi_dependency(
            boost::process
            TARGET BOOST_PROCESS
            HEADERS boost/process.hpp
            PACKAGES libboost-dev
            ${optionalFlag}
        )
    endif()

    if (SPACEPI_DEPS_BOOST_PROPERTY_TREE)
        spacepi_dependency(
            boost::property_tree
            TARGET BOOST_PROPERTY_TREE
            HEADERS boost/property_tree/ptree.hpp boost/property_tree/xml_parser.hpp
            PACKAGES libboost-dev
            ${optionalFlag}
        )
    endif()

    if (SPACEPI_DEPS_BOOST_STACKTRACE_BACKTRACE)
        spacepi_dependency(
            boost::stacktrace::backtrace
            TARGET BOOST_STACKTRACE_BACKTRACE
            LIBRARY_NAMES boost_stacktrace_backtrace
            HEADERS boost/stacktrace.hpp
            PACKAGES libboost-stacktrace-dev
            ${optionalFlag}
        )
    endif()

    if (SPACEPI_DEPS_BOOST_SYSTEM)
        spacepi_dependency(
            boost::system
            TARGET BOOST_SYSTEM
            LIBRARY_NAMES boost_system
            HEADERS boost/system/error_code.hpp
            PACKAGES libboost-system-dev
            ${optionalFlag}
        )
    endif()

    if (SPACEPI_DEPS_BOOST_THREAD)
        spacepi_dependency(
            boost::thread
            TARGET BOOST_THREAD
            LIBRARY_NAMES boost_thread
            HEADERS boost/thread.hpp
            PACKAGES libboost-thread-dev
            ${optionalFlag}
        )
    endif()
endfunction()
