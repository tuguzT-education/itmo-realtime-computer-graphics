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
        ${PROJECT_SOURCE_DIR}/include/borov_engine/window.hpp
        ${PROJECT_SOURCE_DIR}/include/borov_engine/game.hpp
        ${PROJECT_SOURCE_DIR}/include/borov_engine/component.hpp
        ../include/borov_engine/game.fwd
        ../include/borov_engine/timer.hpp)
set(SOURCE_LIST
        delegate/delegate_handle.cpp
        window.cpp
        game.cpp
        timer.cpp)

add_library(borov_engine ${SOURCE_LIST} ${HEADER_LIST})
target_include_directories(borov_engine PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_compile_features(borov_engine PUBLIC cxx_std_20)
