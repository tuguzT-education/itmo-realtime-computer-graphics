set(HEADER_LIST
        objects/apricot.hpp
        objects/axe.hpp
        objects/boat.hpp
        objects/bulb.hpp
        objects/cake.hpp
        objects/chair.hpp
        objects/cheese.hpp
        objects/concrete_barricade.hpp
        objects/die.hpp
        objects/hog.hpp
        objects/strawberry.hpp
        objects/tanto.hpp
        viewport_manager.hpp
        field.hpp
        player_child.hpp
        player.hpp
        game.hpp)
set(SOURCE_LIST
        objects/apricot.cpp
        objects/axe.cpp
        objects/boat.cpp
        objects/bulb.cpp
        objects/cake.cpp
        objects/chair.cpp
        objects/cheese.cpp
        objects/concrete_barricade.cpp
        objects/die.cpp
        objects/hog.cpp
        objects/strawberry.cpp
        objects/tanto.cpp
        viewport_manager.cpp
        field.cpp
        player_child.cpp
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
