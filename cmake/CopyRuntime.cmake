
if(NOT DEFINED DESTINATION)
    message(FATAL_ERROR "Destination not specified")
endif()

file(MAKE_DIRECTORY "${DESTINATION}")

foreach(dll IN LISTS RUNTIME_DLLS)
    if(EXISTS "${dll}")
        get_filename_component(dll_name "${dll}" NAME)

        file(COPY_FILE
            "${dll}"
            "${DESTINATION}/${dll_name}"
            ONLY_IF_DIFFERENT
        )

        message(STATUS "Copied runtime DLL: ${dll_name}")
    endif()
endforeach()
