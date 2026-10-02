add_executable(logger_example
    basic.cpp
)

target_link_libraries(logger_example
    PRIVATE
        cpp_logger
)
