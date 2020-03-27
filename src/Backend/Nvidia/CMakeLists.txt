# Add the files we want to link against
set(nvidia_backend_source_files
    Nvidia.cpp
    NvidiaHash.cpp
    NvidiaUtils.cu
)

add_library(NvidiaBackend ${nvidia_backend_source_files})

target_link_libraries(NvidiaBackend Nvidia)

target_compile_features(NvidiaBackend PUBLIC cxx_std_11)

set_target_properties(NvidiaBackend PROPERTIES CUDA_SEPARABLE_COMPILATION ON)
set_target_properties(NvidiaBackend PROPERTIES CUDA_STANDARD 11)

if (MSVC)
    set_target_properties(NvidiaBackend PROPERTIES CUDA_RESOLVE_DEVICE_SYMBOLS ON)
endif()
