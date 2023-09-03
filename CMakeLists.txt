# 这是cmake文件
cmake_minimum_required(VERSION 3.0)
project(parseweb)
aux_source_directory(${PROJECT_SOURCE_DIR}/src SRC)
add_executable(parseweb ${SRC})
#add_library(parseweb ${SRC})

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
include_directories(${PROJECT_SOURCE_DIR}/include)

# 链接库文件到你的可执行文件
find_package(CURL REQUIRED)
include_directories(${CURL_INCLUDE_DIRS})
target_link_libraries(parseweb  ${CURL_LIBRARIES})
message(${CURL_INCLUDE_DIRS})

# ICU
include_directories(/usr/local/opt/icu4c/include)
target_link_libraries(parseweb /usr/local/opt/icu4c/lib/libicui18n.dylib)

# link iconv
include_directories(/usr/local/opt/libiconv/include)
target_link_libraries(parseweb /usr/local/opt/libiconv/lib/libiconv.dylib)


