cmake_minimum_required(VERSION 2.6)
 
PROJECT(sphere)
 
FIND_PACKAGE(VTK REQUIRED)
INCLUDE(${VTK_USE_FILE})
 
ADD_EXECUTABLE(sphere sphere.cxx)
 
TARGET_LINK_LIBRARIES(sphere vtkHybrid)
