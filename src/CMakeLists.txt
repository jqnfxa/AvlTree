set(BINARY ${CMAKE_PROJECT_NAME})

file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true *.hpp *.cpp)

#set(CMAKE_BUILD_TYPE Release)
#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2")
set(SOURCES ${SOURCES})
add_executable(${BINARY}_run ${SOURCES})
add_library(${BINARY}_lib STATIC ${SOURCES}
        algorithm/AvlTreeNode.cpp
        algorithm/AvlTreeNode.hpp
        algorithm/AvlTreeHeader.cpp
        algorithm/AvlTreeHeader.hpp
        algorithm/AvlTreeHelper.cpp
        algorithm/AvlTreeHelper.hpp
        algorithm/AvlTreeIterator.hpp
        algorithm/AvlTreeDeclaration.hpp)