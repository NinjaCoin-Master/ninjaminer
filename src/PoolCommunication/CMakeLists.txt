# Add the files we want to link against
set(pool_communication_source_files
    PoolCommunication.cpp
)

# Add the library to be linked against, with the previously specified source files
add_library(PoolCommunication ${pool_communication_source_files})

if (OPENSSL_FOUND)
    target_link_libraries(PoolCommunication ${OPENSSL_LIBRARIES})

    if (MSVC)
        target_link_libraries(PoolCommunication ws2_32 gdi32 advapi32 crypt32 user32)
    endif()
endif()

