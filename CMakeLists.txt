cmake_minimum_required(VERSION 3.16)
project(ClusteringRows LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Find system libraries
find_package(Boost REQUIRED COMPONENTS program_options system)
find_package(OpenCV REQUIRED)
find_package(CURL REQUIRED)

# Fetch nanoflann (header-only)
include(FetchContent)
FetchContent_Declare(
  nanoflann
  GIT_REPOSITORY https://github.com/jlblancoc/nanoflann.git
  GIT_TAG master
)
FetchContent_MakeAvailable(nanoflann)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS include/*.h)

add_executable(ClusteringRows ${SOURCES} ${HEADERS})

target_include_directories(ClusteringRows PRIVATE
    ${Boost_INCLUDE_DIRS}
    ${nanoflann_SOURCE_DIR}/include
    ${OpenCV_INCLUDE_DIRS}
    ${CURL_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(ClusteringRows PRIVATE
    Boost::program_options
    Boost::system
    CURL::libcurl
    ${OpenCV_LIBS}
)
