set(HEADER_LIST
        objects/apricot.hpp
        objects/axe.hpp
        objects/bulb.hpp
        objects/cake.hpp
        objects/hog.hpp
        field.hpp
        player.hpp
        game.hpp)
set(SOURCE_LIST
        objects/apricot.cpp
        objects/axe.cpp
        objects/bulb.cpp
        objects/cake.cpp
        objects/hog.cpp
        field.cpp
        player.cpp
        game.cpp
        main.cpp)

add_executable(katamari ${SOURCE_LIST} ${HEADER_LIST})
target_compile_features(katamari PRIVATE cxx_std_20)
target_link_libraries(katamari PRIVATE borov_engine)

add_custom_command(TARGET katamari POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/resources/ $<TARGET_FILE_DIR:katamari>/resources/
        VERBATIM)
