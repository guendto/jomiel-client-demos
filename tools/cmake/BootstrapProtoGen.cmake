# -*- coding: utf-8 -*-
#
# jomiel-examples
#
# Copyright
#  2020 Toni Gündoğdu
#
#
# SPDX-License-Identifier: Apache-2.0
#
include(BuildWithDocker)

macro(_bootstrap_invoke _langid _outdir)
    set(_protodir ${BOOTSTRAP_PROTO_DIR})
    set(_bootstrap_exec ${_protodir}/bin/bootstrap)
    set(_bindir ${CMAKE_CURRENT_BINARY_DIR})
    set(_destdir ${_bindir}/${_outdir})

    if(WITH_BOOTSTRAP_DEBUG)
        cmake_print_variables(_protodir)
        cmake_print_variables(_bootstrap_exec)
        cmake_print_variables(_bindir)
        cmake_print_variables(_destdir)
        cmake_print_variables(_langid)
    endif()

    execute_process(COMMAND
        ${_bootstrap_exec} -p ${_protodir} -l ${_langid} -d ${_destdir}
        OUTPUT_VARIABLE _bootstrap_invoke_result
        RESULT_VARIABLE _bootstrap_failed
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    if(_bootstrap_failed)
        cmake_print_variables(_bootstrap_invoke_result)
        cmake_print_variables(_bootstrap_failed)
        message(FATAL_ERROR "${_bootstrap_exec} failed")
    endif()
endmacro()

macro(_detect_fileext _langid)
    if(${_langid} STREQUAL "c")
        set(_val "c")
    elseif(${_langid} STREQUAL "cpp")
        set(_val "cc")
    else()
        message(FATAL_ERROR "invalid value: _langid=${_langid}")
    endif()
    set(_fileext ${_val})
endmacro()

# bootstrap_gen_bindings
#   - Generate lang bindings usig the bootstrap script from jomiel-proto
#   Args:
#       _langid (str): "c" or "cpp"
#
#       _outdir (str): the output dir for the generated bindings, note
#           that this will be appended to the CMAKE_CURRENT_BINARY_DIR value
#   Sets:
#       SRC_BOOTSTRAP_FILES: a list of the generated files
#
function(bootstrap_gen_bindings _langid _outdir)
    # Check if building in docker image.
    #
    build_with_docker()

    # Options.
    #
    option(WITH_BOOTSTRAP_DEBUG
        "display bootstrap (proto-gen) status" OFF)

    message(STATUS
        "Compile the protobuf declarations for jomiel messages")

    # Invoke the bootstrap script.
    #   - Detect file extension from the given language ID string
    #   - Locate all generated files
    #
    _bootstrap_invoke(${_langid} ${_outdir})
    _detect_fileext(${_langid})

    file(GLOB_RECURSE
        _bootstrap_src_files
        ${_destdir}/*.${_fileext}
        ${_destdir}/*.h
    )

    if(WITH_BOOTSTRAP_DEBUG)
        foreach(_arg ${_bootstrap_src_files})
            message(STATUS ${_arg})
        endforeach()
    endif()

    # Set SRC_BOOTSTRAP_FILES.
    #
    set(SRC_BOOTSTRAP_FILES ${_bootstrap_src_files} PARENT_SCOPE)
endfunction()

# vim: set ts=4 sw=4 tw=72 expandtab:
