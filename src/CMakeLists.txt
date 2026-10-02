add_library(cpp_logger STATIC
    logger.cpp
)

target_compile_features(cpp_logger  # Set target specific standard
    PUBLIC
        cxx_std_20
)

target_include_directories(cpp_logger
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)
