set(HEADER_LIST
        ${PROJECT_SOURCE_DIR}/include/borov_engine/alloc/inline_allocator.hpp
        ${PROJECT_SOURCE_DIR}/include/borov_engine/alloc/inline_allocator.inl
        ${PROJECT_SOURCE_DIR}/include/borov_engine/delegate/member_function.hpp
        ${PROJECT_SOURCE_DIR}/include/borov_engine/delegate/delegate_kind.hpp
        ${PROJECT_SOURCE_DIR}/include/borov_engine/delegate/delegate_kind.inl
        ${PROJECT_SOURCE_DIR}/include/borov_engine/delegate/static_delegate.hpp
        ${PROJECT_SOURCE_DIR}/include/borov_engine/delegate/static_delegate.inl
        ${PROJECT_SOURCE_DIR}/include/borov_engine/delegate/raw_delegate.hpp
        ${PROJECT_SOURCE_DIR}/include/borov_engine/delegate/raw_delegate.inl
        ${PROJECT_SOURCE_DIR}/include/borov_engine/delegate/lambda_delegate.hpp
        ${PROJECT_SOURCE_DIR}/include/borov_engine/delegate/lambda_delegate.inl
        ${PROJECT_SOURCE_DIR}/include/borov_engine/delegate/shared_ptr_delegate.hpp
        ${PROJECT_SOURCE_DIR}/include/borov_engine/delegate/shared_ptr_delegate.inl
        ${PROJECT_SOURCE_DIR}/include/borov_engine/delegate/delegate_handle.hpp
        ${PROJECT_SOURCE_DIR}/include/borov_engine/delegate/delegate_handle.inl
        ${PROJECT_SOURCE_DIR}/include/borov_engine/delegate/delegate.hpp
        ${PROJECT_SOURCE_DIR}/include/borov_engine/delegate/multicast_delegate.hpp
        ${PROJECT_SOURCE_DIR}/include/borov_engine/delegate/multicast_delegate.inl
        ${PROJECT_SOURCE_DIR}/include/borov_engine/detail/erased_unique_ptr.hpp
        ${PROJECT_SOURCE_DIR}/include/borov_engine/detail/err_handling_api.hpp
        ${PROJECT_SOURCE_DIR}/include/borov_engine/detail/string_api_set.hpp
        ${PROJECT_SOURCE_DIR}/include/borov_engine/detail/check_result.hpp
        ${PROJECT_SOURCE_DIR}/include/borov_engine/detail/check_result.inl
        ${PROJECT_SOURCE_DIR}/include/borov_engine/detail/d3d_ptr.hpp
        ${PROJECT_SOURCE_DIR}/include/borov_engine/math.hpp
        ${PROJECT_SOURCE_DIR}/include/borov_engine/collision_primitive.hpp
        ${PROJECT_SOURCE_DIR}/include/borov_engine/collision_primitive.inl
        ${PROJECT_SOURCE_DIR}/include/borov_engine/window.hpp
        ${PROJECT_SOURCE_DIR}/include/borov_engine/input_key.hpp
        ${PROJECT_SOURCE_DIR}/include/borov_engine/input.hpp
        ${PROJECT_SOURCE_DIR}/include/borov_engine/game.hpp
        ${PROJECT_SOURCE_DIR}/include/borov_engine/game.inl
        ${PROJECT_SOURCE_DIR}/include/borov_engine/timer.hpp
        ${PROJECT_SOURCE_DIR}/include/borov_engine/component.hpp
        ${PROJECT_SOURCE_DIR}/include/borov_engine/camera.hpp
        ${PROJECT_SOURCE_DIR}/include/borov_engine/camera_manager.hpp
        ${PROJECT_SOURCE_DIR}/include/borov_engine/spectator_camera_manager.hpp
        ${PROJECT_SOURCE_DIR}/include/borov_engine/orbit_camera_manager.hpp
        ${PROJECT_SOURCE_DIR}/include/borov_engine/viewport.hpp
        ${PROJECT_SOURCE_DIR}/include/borov_engine/viewport_manager.hpp
        ${PROJECT_SOURCE_DIR}/include/borov_engine/transform.hpp
        ${PROJECT_SOURCE_DIR}/include/borov_engine/scene_component.hpp
        ${PROJECT_SOURCE_DIR}/include/borov_engine/geometric_primitive_component.hpp
        ${PROJECT_SOURCE_DIR}/include/borov_engine/triangle_component.hpp
        ${PROJECT_SOURCE_DIR}/include/borov_engine/box_component.hpp)
set(SOURCE_LIST
        delegate/delegate_kind.cpp
        delegate/delegate_handle.cpp
        delegate/delegate.cpp
        detail/err_handling_api.cpp
        detail/string_api_set.cpp
        detail/check_result.cpp
        math.cpp
        collision_primitive.cpp
        window.cpp
        input.cpp
        game.cpp
        timer.cpp
        component.cpp
        camera.cpp
        camera_manager.cpp
        spectator_camera_manager.cpp
        orbit_camera_manager.cpp
        viewport.cpp
        viewport_manager.cpp
        transform.cpp
        scene_component.cpp
        geometric_primitive_component.cpp
        triangle_component.cpp
        box_component.cpp)

find_package(directxtk CONFIG REQUIRED)
find_package(range-v3 CONFIG REQUIRED)

add_library(borov_engine ${SOURCE_LIST} ${HEADER_LIST})
target_include_directories(borov_engine PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_compile_features(borov_engine PUBLIC cxx_std_20)
target_link_libraries(borov_engine PUBLIC
        d3d11.lib dxgi.lib d3dcompiler.lib dxguid.lib
        Microsoft::DirectXTK range-v3)

message(STATUS "Using toolchain file: ${CMAKE_TOOLCHAIN_FILE}")
