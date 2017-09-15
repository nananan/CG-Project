project(First_Project)
cmake_minimum_required(VERSION 2.8)

find_package(PkgConfig REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenGL  REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
#pkg_search_module(ASSIMP REQUIRED assimp)

include_directories(
  ${GLFW_INCLUDE_DIRS}
  ${GLEW_INCLUDE_DIRS}
  ${OPENGL_INCLUDE_DIRS}
  /usr/local/include/assimp/
  /usr/local/include/SOIL/
  /home/eliana/Scaricati/glad-master/include/
  )

aux_source_directory(. SRC_LIST)

if(NOT MSVC)
  find_library(M_LIB m)
else() # not needed on Windows
  set(M_LIB "")
endif()
file(GLOB SOURCES
    include/*.h
    source/*.cpp
    shaders/*.frag
    shaders/*.vs

)
add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(
  ${PROJECT_NAME}
#  ${ASSIMP_LIBRARIES}
  ${GLFW_LIBRARIES}
  ${GLEW_LIBRARIES}
  ${OPENGL_LIBRARIES}
  ${M_LIB}
  /usr/local/lib/libassimp.so
  /usr/local/lib/libSOIL.a
  /home/eliana/Scaricati/glad-master/libglad.a
  )


set_target_properties(First_Project PROPERTIES LINKER_LANGUAGE CXX)




