set(HEADER_LIST
        celestials/sun.hpp
        celestials/mercury.hpp
        celestials/venus.hpp
        celestials/earth.hpp
        celestials/moon.hpp
        celestials/mars.hpp
        celestials/phobos.hpp
        celestials/deimos.hpp
        celestials/jupyter.hpp
        celestials/saturn.hpp
        celestials/uranus.hpp
        celestials/neptune.hpp
        viewport_manager.hpp
        game.hpp)
set(SOURCE_LIST
        celestials/sun.cpp
        celestials/mercury.cpp
        celestials/venus.cpp
        celestials/earth.cpp
        celestials/moon.cpp
        celestials/mars.cpp
        celestials/phobos.cpp
        celestials/deimos.cpp
        celestials/jupyter.cpp
        celestials/saturn.cpp
        celestials/uranus.cpp
        celestials/neptune.cpp
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
