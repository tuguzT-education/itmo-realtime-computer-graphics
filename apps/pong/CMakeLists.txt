set(HEADER_LIST
        direction.hpp
        player.hpp
        ball.hpp
        game.hpp)
set(SOURCE_LIST
        player.cpp
        ball.cpp
        game.cpp
        main.cpp)

add_executable(pong ${SOURCE_LIST} ${HEADER_LIST})
target_compile_features(pong PRIVATE cxx_std_20)
target_link_libraries(pong PRIVATE borov_engine)

add_custom_command(TARGET pong POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/resources/ $<TARGET_FILE_DIR:pong>/resources/
        VERBATIM)
