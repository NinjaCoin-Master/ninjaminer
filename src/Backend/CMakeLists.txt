# Add the files we want to link against
set(backend_source_files
)

add_library(Backend INTERFACE ${backend_source_files})

add_subdirectory(CPU)
target_link_libraries(Backend INTERFACE CPUBackend)

if (ENABLE_NVIDIA)
    add_subdirectory(Nvidia)
    target_link_libraries(Backend INTERFACE NvidiaBackend)
endif()

if (ENABLE_AMD)
    add_subdirectory(AMD)
    target_link_libraries(Backend INTERFACE AMDBackend)
endif()
