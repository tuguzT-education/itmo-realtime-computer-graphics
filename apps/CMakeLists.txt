set(HEADER_LIST
        clear_screen_component.hpp)
set(SOURCE_LIST
        app.cpp)

add_executable(app ${SOURCE_LIST} ${HEADER_LIST})
target_compile_features(app PRIVATE cxx_std_20)
target_link_libraries(app PRIVATE borov_engine)

add_custom_command(TARGET app PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/resources/ $<TARGET_FILE_DIR:app>/resources)
