set(HEADER_LIST
        camera_manager.hpp
        viewport_manager.hpp
        game.hpp)
set(SOURCE_LIST
        camera_manager.cpp
        viewport_manager.cpp
        game.cpp
        main.cpp)

add_executable(solar_system ${SOURCE_LIST} ${HEADER_LIST})
target_compile_features(solar_system PRIVATE cxx_std_20)
target_link_libraries(solar_system PRIVATE borov_engine)

add_custom_command(TARGET solar_system POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/resources/ $<TARGET_FILE_DIR:solar_system>/resources/
        VERBATIM)
