set(HEADER_LIST
        game.hpp
        player.hpp)
set(SOURCE_LIST
        game.cpp
        main.cpp
        player.cpp)

add_executable(katamari ${SOURCE_LIST} ${HEADER_LIST})
target_compile_features(katamari PRIVATE cxx_std_20)
target_link_libraries(katamari PRIVATE borov_engine)

add_custom_command(TARGET katamari POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/resources/ $<TARGET_FILE_DIR:katamari>/resources/
        VERBATIM)
