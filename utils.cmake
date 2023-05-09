function(SetWarningAsError)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        # Using GCC or Clang
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror" PARENT_SCOPE)
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
        # Using MSVC
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX" PARENT_SCOPE)
    endif()
endfunction()