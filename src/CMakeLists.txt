set(HEADER_LIST
        ${PROJECT_SOURCE_DIR}/include/borov_engine/Window.hpp
        ${PROJECT_SOURCE_DIR}/include/borov_engine/Delegates.hpp)
set(SOURCE_LIST
        Window.cpp
        Delegates.cpp)

add_library(borov_engine ${SOURCE_LIST} ${HEADER_LIST})
target_include_directories(borov_engine PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_compile_features(borov_engine PUBLIC cxx_std_20)
