# -*- coding: utf-8 -*-
#
# jomiel-examples
#
# Copyright
#  2021 Toni Gündoğdu
#
#
# SPDX-License-Identifier: Apache-2.0
#

# protoc_gen_bindings
#   - Compile the protobuf declarations for jomiel messages.
#   Args:
#       _langid (str): "cpp" or "c"
#       _protodir (str): path to search (recursively) for the .proto files
#       _destdir (str): path for the generated files be written to
#   Returns:
#       PROTOC_GEN_SRCFILES: a list of the generated source files
#   Notes:
#       - Works with/without conan; if used, the protoc from the
#       `protobuf` conan package will be used instead of the protoc in
#       PATH.
#
function(protoc_gen_bindings _langid _protodir _destdir)
    # Determine the file extension of the generated (impl.) files.
    #
    if(${_langid} STREQUAL "cpp")
        set(_ext "cc")
    elseif(${_langid} STREQUAL "c")
        set(_ext "c")
    else()
        message(FATAL_ERROR "invalid value: ${_langid} (_langid)")
    endif()

    # Locate protoc(1).
    #   - Either the one installed from the `protobuf` conan package
    #   - Or the one found in the PATH
    #
    find_program(_protoc "protoc" REQUIRED)
    # cmake_print_variables(_protoc)

    # Find .proto files from the specified path.
    #
    # cmake_print_variables(_protodir)
    file(GLOB_RECURSE _protofiles ${_protodir}/*.proto)
    # cmake_print_variables(_protofiles)

    # Make destination directory for the generated files.
    #   - protoc does not seem to do this.
    #
    file(MAKE_DIRECTORY ${_destdir})

    # Notify user.
    #
    message(STATUS
        "Compiling the protobuf declarations for jomiel messages...")

    # Invoke protoc.
    #
    set(_langarg --${_langid}_out=${_destdir})

    foreach(_protofile ${_protofiles})
        set(_args -I${_protodir} ${_langarg} ${_protofile})
        execute_process(COMMAND
            ${_protoc} ${_args}
            OUTPUT_VARIABLE _protoc_invoke_result
            RESULT_VARIABLE _protoc_failed
            OUTPUT_STRIP_TRAILING_WHITESPACE
        )
        if (_protoc_failed)
            cmake_print_variables(_protofile)
            cmake_print_variables(_protoc)
            foreach(_arg ${_args})
                cmake_print_variables(_arg)
            endforeach()
            cmake_print_variables(_protoc_invoke_result)
            cmake_print_variables(_protoc_failed)
            message(FATAL_ERROR "${_protoc} failed")
        else()
            message(STATUS "  Compiled ${_protofile}")
        endif()
    endforeach()

    # Find the generated files.
    #   - Return file paths in PROTOC_GEN_SRCFILES.
    #
    file(GLOB_RECURSE
        _protoc_gen_src_files
        ${_destdir}/*.${_ext}
        ${_destdir}/*.h
    )
    # cmake_print_variables(_protoc_gen_src_files)

    set(PROTOC_GEN_SRCFILES ${_protoc_gen_src_files} PARENT_SCOPE)
endfunction()
