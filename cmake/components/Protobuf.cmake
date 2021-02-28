# SPACEPI_MLIB_JAVA: If the Java sources should be generated
# SPACEPI_MLIB_PROTOC: The protoc executable
# SPACEPI_MLIB_OUTDIR: Output directory for generated sources
# SPACEPI_MLIB_INCLUDE: List of include directories for protoc
# SPACEPI_MLIB_SOURCES: List of protobuf source files

file(MAKE_DIRECTORY "${SPACEPI_MLIB_OUTDIR}/java")

set(outputs "--cpp_out" "${SPACEPI_MLIB_OUTDIR}")
if (SPACEPI_MLIB_JAVA)
    list(APPEND outputs "--java_out" "${SPACEPI_MLIB_OUTDIR}/java")
endif()

file(TOUCH "${SPACEPI_MLIB_OUTDIR}/protobuf.test")

execute_process(
    COMMAND "${SPACEPI_MLIB_PROTOC}" ${outputs} ${SPACEPI_MLIB_INCLUDE} ${SPACEPI_MLIB_SOURCES}
    RESULT_VARIABLE result
)

if (NOT ${result} EQUAL 0)
    message(FATAL_ERROR "Failed to run protobuf compiler: ${result}")
endif()

file(GLOB_RECURSE files
    "${SPACEPI_MLIB_OUTDIR}/*.java"
    "${SPACEPI_MLIB_OUTDIR}/*.pb.cc"
    "${SPACEPI_MLIB_OUTDIR}/*.pb.h"
)
foreach (file IN LISTS files)
    if ("${SPACEPI_MLIB_OUTDIR}/protobuf.test" IS_NEWER_THAN "${file}")
        file(REMOVE "${file}")
    endif()
endforeach()

file(TOUCH "${SPACEPI_MLIB_OUTDIR}/protobuf.stamp")
