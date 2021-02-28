get_property(hasProp GLOBAL PROPERTY SPACEPI_ONCE_CALLED DEFINED)
if (NOT hasProp)
    define_property(GLOBAL PROPERTY SPACEPI_ONCE_CALLED BRIEF_DOCS "." FULL_DOCS ".")
    set_property(GLOBAL PROPERTY SPACEPI_ONCE_CALLED "")
    
    # spacepi_once(<name>)
    macro (spacepi_once SPACEPI_ONCE_NAME)
        get_property(callList GLOBAL PROPERTY SPACEPI_ONCE_CALLED)
        list(FIND callList "${SPACEPI_ONCE_NAME}" res)
        if (res LESS 0)
            list(APPEND callList "${SPACEPI_ONCE_NAME}")
            set_property(GLOBAL PROPERTY SPACEPI_ONCE_CALLED ${callList})
        else()
            return()
        endif()
    endmacro()
endif()
