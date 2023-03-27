option(GIT_SUBMODULE "Check submodules during build" ON)

function(download_submodules)
    if(GIT_SUBMODULE)
        if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
            message(STATUS "Submodule update")
            execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                RESULT_VARIABLE GIT_SUBMOD_RESULT)

            if(NOT GIT_SUBMOD_RESULT EQUAL "0")
                message(FATAL_ERROR "Command git submodule update --init --recursive failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
            endif()
        else()
            message(WARNING "Git not found, please checkout submodules")
        endif()
    else()
        message(STATUS "The submodules were not downloaded! GIT_SUBMODULE was turned off. Please update submodules and try again.")
    endif()
endfunction()
