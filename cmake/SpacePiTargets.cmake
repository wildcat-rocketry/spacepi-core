# spacepi_default_targets(
#     [CORE]
#     [DOCS]
#     [EXAMPLES]
#     [LIBRARIES]
#     [MESSAGES]
#     [MODULES]
#     [PACKAGES]
#     [TARGETS]
#     [TESTS]
# )
function (spacepi_default_targets)
    cmake_parse_arguments(SPACEPI_TARGET "CORE;DOCS;EXAMPLES;LIBRARIES;MESSAGES;MODULES;PACKAGES;TARGETS;TESTS" "" "" ${ARGN})

    if (CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
        set(deps)

        if (SPACEPI_TARGET_CORE)
            list(APPEND deps core)
        endif()
        if (SPACEPI_TARGET_DOCS)
            list(APPEND deps docs)
        endif()
        if (SPACEPI_TARGET_EXAMPLES)
            list(APPEND deps examples)
        endif()
        if (SPACEPI_TARGET_LIBRARIES)
            list(APPEND deps libraries)
        endif()
        if (SPACEPI_TARGET_MESSAGES)
            list(APPEND deps messages)
        endif()
        if (SPACEPI_TARGET_MODULES)
            list(APPEND deps modules)
        endif()
        if (SPACEPI_TARGET_PACKAGES)
            list(APPEND deps packages)
        endif()
        if (SPACEPI_TARGET_TARGETS)
            list(APPEND deps targets)
        endif()
        if (SPACEPI_TARGET_TESTS)
            list(APPEND deps tests)
        endif()

        add_custom_target(
            defaults ALL
            DEPENDS ${deps}
        )
    endif()
endfunction()

include(SpacePiOnce)
spacepi_once(SPACEPI_TARGETS)

add_custom_target(core)      # Core library
add_custom_target(docs)      # Documentation
add_custom_target(examples)  # Example code
add_custom_target(full)      # Everything
add_custom_target(libraries) # Module Libraries
add_custom_target(messages)  # Module Messages
add_custom_target(modules)   # Module Executables
add_custom_target(packages)  # Packaged Environments
add_custom_target(targets)   # Packaging Helper Code
add_custom_target(tests)     # Unit Tests

add_dependencies(full
    core
    docs
    examples
    libraries
    messages
    modules
    packages
    targets
    tests
)
