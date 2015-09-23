# 
# alps-python cmake configuration script.
# 

cmake_minimum_required (VERSION 2.8.12)

# define project name
project (alps-python CXX)
message ("\nALPSCore module : ${PROJECT_NAME}\n")

# add common modules from ../common/cmake
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/../common/cmake)

# add common definitions
include(ALPSCommonModuleDefinitions)

gen_documentation()

if (DocumentationOnly)
  return()
endif (DocumentationOnly)

# search for python
find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})
link_libraries(${PYTHON_LIBRARIES})

# find numpy
find_package(NumPy REQUIRED)
include_directories(${NUMPY_INCLUDE_DIRS})

find_package(ALPSCore REQUIRED)
link_libraries(${ALPSCore_LIBRARIES})

add_this_package(python dict_to_params)
add_boost(python)

add_testing()

set (libs_ "")
foreach (name    
        pyrandom01 
	pyhdf5
	api
	pymcbase)
    
    add_library(${name}_c MODULE src/${name}.cpp src/import_numpy.cpp)
    list(APPEND libs_ ${name}_c)

    target_compile_definitions(${name}_c PUBLIC "${ALPS_SHARED_CPPFLAGS}") # FIXME: public or private?

    set_target_properties(${name}_c PROPERTIES PREFIX "")
    target_link_libraries(${name}_c ${PYTHON_LIBRARIES} ${Boost_LIBRARIES} ${ALPSCore_LIBRARIES})

    install(TARGETS ${name}_c 
            EXPORT ${PROJECT_NAME}
            DESTINATION python
            INCLUDES DESTINATION include)
endforeach(name)
install(EXPORT ${PROJECT_NAME} NAMESPACE alps-python:: DESTINATION share/${PROJECT_NAME})

# FIXME: modify this code (transplanted from ALPSCommonModuleDefinitions.cmake) to make it work:
# set(tgt_list_ "")
# foreach(tgt_ ${ARGV})
#     list(APPEND tgt_list_ "alps::${tgt_}")
# endforeach()
# if (tgt_list_)
#     gen_cfg_module(DEPENDS ${${PROJECT_NAME}_DEPENDS} EXPORTS ${tgt_list_})
# else()
#     gen_cfg_module(DEPENDS ${${PROJECT_NAME}_DEPENDS})
# endif()



gen_pkg_config()
#gen_find_module(alps/utilities/boost_python.hpp HEADER_ONLY) 
