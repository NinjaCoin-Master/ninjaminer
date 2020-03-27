# Add the files we want to link against
set(cpu_backend_source_files
    CPU.cpp
)

add_library(CPUBackend ${cpu_backend_source_files})

target_link_libraries(CPUBackend ArgonVariants Argon2)
