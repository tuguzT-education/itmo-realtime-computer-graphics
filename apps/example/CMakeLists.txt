set(HEADER_LIST
        clear_screen_component.hpp
        window_title_component.hpp
        square_component.hpp)
set(SOURCE_LIST
        example.cpp)

add_executable(example ${SOURCE_LIST} ${HEADER_LIST})
target_compile_features(example PRIVATE cxx_std_20)
target_link_libraries(example PRIVATE borov_engine)

add_custom_command(TARGET example PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/resources/ $<TARGET_FILE_DIR:example>/resources)
