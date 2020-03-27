include_directories("${CMAKE_CURRENT_SOURCE_DIR}/Argon2/src")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

if (ENABLE_NVIDIA)
    # Only include directories if any were found
    if (NOT "${CMAKE_CUDA_TOOLKIT_INCLUDE_DIRECTORIES}" STREQUAL "")
        include_directories("${CMAKE_CUDA_TOOLKIT_INCLUDE_DIRECTORIES}")
    endif()
endif()

# Add the ArgonVariants directory to add that library
add_subdirectory(ArgonVariants)

# Add the Argon2 directory to add that library
add_subdirectory(Argon2)

# Add the Backend directory to add that library
add_subdirectory(Backend)

# Add the Config directory to add that library
add_subdirectory(Config)

# Add the Miner directory to add the actual executable
add_subdirectory(Miner)

# Add the MinerManager directory to add that library
add_subdirectory(MinerManager)

# Add the PoolCommunication directory to add that library
add_subdirectory(PoolCommunication)

# Add the Types directory to add that library
add_subdirectory(Types)

# Add the Utilities directory to add that library
add_subdirectory(Utilities)
