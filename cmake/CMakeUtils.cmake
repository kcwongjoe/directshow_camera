function(init_variables)
    # ---- Initialize the following variables ----
    # IS_BUILDING_X86 = TRUE if the project is building for x86, FALSE otherwise

    # ---- Check whether building x86 and return IS_BUILDING_X86----

    # Convert the platform string to lowercase
    set(BUILD_PLATFORM_STR "x64")
    if (${CMAKE_GENERATOR_PLATFORM})
        string(TOLOWER ${CMAKE_GENERATOR_PLATFORM} BUILD_PLATFORM_STR)
    endif()

    # Check if the platform is x86
    if ( (CMAKE_SIZE_OF_VOID_P EQUAL 4) OR ("${BUILD_PLATFORM_STR}" STREQUAL "win32") )
        set(IS_BUILDING_X86 TRUE PARENT_SCOPE)
    else()
        set(IS_BUILDING_X86 FALSE PARENT_SCOPE)
    endif()

endfunction()