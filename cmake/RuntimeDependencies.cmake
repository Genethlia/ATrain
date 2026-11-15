# Copy Windows runtime DLLs beside the executable after linking.
if(WIN32)
    add_custom_command(
        TARGET ATrain POST_BUILD
        COMMAND ${CMAKE_COMMAND}
            "-DRUNTIME_DLLS=$<TARGET_RUNTIME_DLLS:ATrain>"
            "-DDESTINATION=$<TARGET_FILE_DIR:ATrain>"
            -P "${CMAKE_CURRENT_LIST_DIR}/CopyRuntime.cmake"
        VERBATIM
    )
endif()
