add_library(net SHARED
    ${CMAKE_CURRENT_LIST_DIR}/extension.cpp
    ${CMAKE_CURRENT_LIST_DIR}/common/url.cpp
    ${CMAKE_CURRENT_LIST_DIR}/common/io_context.cpp
    ${CMAKE_CURRENT_LIST_DIR}/detail/cmd.cpp
    ${CMAKE_CURRENT_LIST_DIR}/detail/ext.cpp
    ${CMAKE_CURRENT_LIST_DIR}/json/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/json/native.cpp
    ${CMAKE_CURRENT_LIST_DIR}/json/sm_handler.cpp
    ${CMAKE_CURRENT_LIST_DIR}/net/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/net/http/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/net/http/native.cpp
    ${CMAKE_CURRENT_LIST_DIR}/net/http/sm_handler.cpp
    ${CMAKE_CURRENT_LIST_DIR}/net/websocket/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/net/websocket/native.cpp
    ${CMAKE_CURRENT_LIST_DIR}/net/websocket/sm_forward.cpp
    ${CMAKE_CURRENT_LIST_DIR}/net/websocket/sm_handler.cpp
)

target_include_directories(net PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/json
    ${CMAKE_CURRENT_LIST_DIR}/common
    ${CMAKE_CURRENT_LIST_DIR}/detail
    ${CMAKE_CURRENT_LIST_DIR}/net
    ${CMAKE_CURRENT_LIST_DIR}/net/http
    ${CMAKE_CURRENT_LIST_DIR}/net/websocket
)

target_link_libraries(net PUBLIC amsdk)
target_link_libraries(net PUBLIC Boost::boost fmt::fmt-header-only OpenSSL::SSL OpenSSL::Crypto)

target_compile_options(net PUBLIC /bigobj)

set_target_properties(net PROPERTIES PREFIX "" SUFFIX ".ext${LIB_SHARED_EXT}")

