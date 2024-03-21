CMAKE_MINIMUM_REQUIRED(VERSION 2.8.12)

elements_subdir(GrowiLa)

elements_depends_on_subdirs(Linx)

find_package(Boost) # test

elements_add_library(GrowiLa src/lib/*.cpp
                     INCLUDE_DIRS Linx
                     LINK_LIBRARIES Linx
                     PUBLIC_HEADERS GrowiLa)

elements_add_executable(GrowLaplacian src/program/GrowLaplacian.cpp
                     INCLUDE_DIRS GrowiLa
                     LINK_LIBRARIES GrowiLa)

elements_add_unit_test(Pipeline tests/src/Pipeline_test.cpp 
                     EXECUTABLE GrowiLa_Pipeline_test
                     LINK_LIBRARIES GrowiLa
                     TYPE Boost)
