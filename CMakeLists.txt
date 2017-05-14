# Project #-----------------------------------------------------------------------------------------
project ( interceptorzz )

cmake_minimum_required ( VERSION 2.8.9 )

find_package ( Threads )
include      ( gtest.cmake )

# C++ Warning Level #-------------------------------------------------------------------------------
if ( CMAKE_COMPILER_IS_GNUCXX )
 set ( CMAKE_CXX_FLAGS "-Wall -pedantic ${CMAKE_CXX_FLAGS}" )
endif()

# C++11 #-------------------------------------------------------------------------------------------
include ( CheckCXXCompilerFlag )

check_cxx_compiler_flag ( "-std=gnu++11" COMPILER_SUPPORTS_CPP11 )
check_cxx_compiler_flag ( "-std=gnu++0x" COMPILER_SUPPORTS_CPP0X )

if ( COMPILER_SUPPORTS_CPP11 )
 set ( CMAKE_CXX_FLAGS "-std=gnu++11 ${CMAKE_CXX_FLAGS}" )
elseif( COMPILER_SUPPORTS_CPP0X )
 set ( CMAKE_CXX_FLAGS "-std=gnu++0x ${CMAKE_CXX_FLAGS}" )
else ()
 message ( STATUS "Compiler ${CMAKE_CXX_COMPILER} has no C++11 support." )
endif ()

message ( STATUS "Compiler flags: ${CMAKE_CXX_FLAGS}" )

# Debug
#set(CMAKE_BUILD_TYPE DEBUG)

# Release
set(CMAKE_BUILD_TYPE RELEASE)

# Sources #-----------------------------------------------------------------------------------------

include_directories ( sources/ )

set ( ZZ3_HEADERS
	  sources/core/depot.hpp
	  sources/heuristics/cache_policy.hpp
	  sources/heuristics/heuristic.hpp
	  sources/heuristics/heuristic_fastest.hpp
	  sources/heuristics/heuristic_sequence.hpp
	  sources/heuristics/heuristic_sequence_multisol.hpp
	  sources/core/interceptor.hpp
	  sources/core/mobile.hpp
	  sources/core/problem.hpp
	  sources/core/solution.hpp
	  sources/core/units.hpp
	  sources/export/tikzifyer.hpp
	  sources/export/tikzgrapher.hpp
	  sources/movements/move.hpp
	  sources/movements/movements.hpp
	  sources/movements/move_policies.hpp
	  sources/movements/move_insert.hpp
	  sources/movements/move_extract.hpp
	  sources/movements/move_move1route.hpp
	  sources/movements/move_move2routes.hpp
	  sources/movements/move_swap1route.hpp
	  sources/movements/move_swap2routes.hpp
	  sources/movements/move_replace.hpp
	  sources/movements/move_2opt.hpp
	  sources/metaheuristics/vnd.hpp
	  sources/metaheuristics/ms_els.hpp
	)

set ( ZZ3_SOURCES
	  sources/core/depot.cpp
	  sources/heuristics/cache_policy.cpp
	  sources/heuristics/heuristic.cpp
	  sources/heuristics/heuristic_sequence.cpp
	  sources/heuristics/heuristic_sequence_multisol.cpp
	  sources/core/interceptor.cpp
	  sources/core/mobile.cpp
	  sources/core/problem.cpp
	  sources/core/solution.cpp
	  sources/core/units.cpp
	  sources/export/tikzifyer.cpp
	  sources/export/tikzgrapher.cpp
	  sources/movements/move_policies.cpp
	)

set ( ZZ3_HEADERS_TEST
	  tests/depot_tests.hpp
	  tests/interceptor_tests.hpp
	  tests/mobile_tests.hpp
	  tests/problem_tests.hpp
	  tests/fastest_tests.hpp
	  tests/sequential_tests.hpp
	  tests/insertmove_tests.hpp
	  tests/extractmove_tests.hpp
	  tests/move2opt_tests.hpp
	  tests/moveswap1route_tests.hpp
	  tests/moveswap2routes_tests.hpp
	  tests/movemove1route_tests.hpp
	  tests/movereplace_tests.hpp
    )

set ( ZZ3_SOURCES_TEST
    )

set ( ZZ3_BENCHMARK_HEADERS
	  sources/VND_benchmark/benchmarkresult.hpp
	  sources/VND_benchmark/VNDBenchmark.hpp
	)

set ( ZZ3_BENCHMARK_SOURCES
	  sources/VND_benchmark/benchmarkresult.cpp
	)

set ( ZZ3_BRKGA_HEADERS
	  sources/BRKGA/BRKGA.h
	  sources/BRKGA/Population.h
	  sources/BRKGA/decoder.h
	  sources/BRKGA/solver.h
	)

set ( ZZ3_BRKGA_SOURCES
	  sources/BRKGA/Population.cpp
	  sources/BRKGA/decoder.cpp
	)

set ( ZZ3_PARETO_HEADERS
	  sources/Pareto/datapoint.h
	  sources/Pareto/paretofrontsolver.h
	  sources/Pareto/paretofront.h
	)


# Executables #-------------------------------------------------------------------------------------
add_executable ( ${CMAKE_PROJECT_NAME}_exec
				 sources/main.cpp
                 ${ZZ3_HEADERS}
                 ${ZZ3_SOURCES}
               )

add_executable ( gen
				 sources/gen.cpp
				 ${ZZ3_HEADERS}
				 ${ZZ3_SOURCES}
			   )

add_executable ( ${CMAKE_PROJECT_NAME}_test
                 tests/tests.cpp
                 ${ZZ3_HEADERS}
                 ${ZZ3_SOURCES}
                 ${ZZ3_HEADERS_TEST}
                 ${ZZ3_SOURCES_TEST}
               )

add_executable ( vnd_benchmark
				sources/VND_benchmark/main.cpp
				${ZZ3_BENCHMARK_HEADERS}
				${ZZ3_HEADERS}
				${ZZ3_PARETO_HEADERS}
				${ZZ3_BENCHMARK_SOURCES}
				${ZZ3_SOURCES}
			   )

add_executable ( brkga_exec
				sources/BRKGA/main.cpp
				${ZZ3_BRKGA_HEADERS}
				${ZZ3_HEADERS}
				${ZZ3_PARETO_HEADERS}
				${ZZ3_BRKGA_SOURCES}
				${ZZ3_SOURCES}
			   )
# Build #-------------------------------------------------------------------------------------------
set_target_properties ( ${CMAKE_PROJECT_NAME}_exec PROPERTIES LINKER_LANGUAGE C )
target_link_libraries ( ${CMAKE_PROJECT_NAME}_exec ${CMAKE_THREAD_LIBS_INIT} )

set_target_properties ( gen PROPERTIES LINKER_LANGUAGE C )
target_link_libraries ( gen ${CMAKE_THREAD_LIBS_INIT} )

set_target_properties ( brkga_exec PROPERTIES LINKER_LANGUAGE C )
target_link_libraries ( brkga_exec ${CMAKE_THREAD_LIBS_INIT} )

set_target_properties ( ${CMAKE_PROJECT_NAME}_test PROPERTIES LINKER_LANGUAGE C )
target_link_libraries ( ${CMAKE_PROJECT_NAME}_test ${CMAKE_THREAD_LIBS_INIT} )
add_dependencies      ( ${CMAKE_PROJECT_NAME}_test googletest )
target_link_libraries ( ${CMAKE_PROJECT_NAME}_test gtest pthread )

# add a target to generate API documentation with Doxygen
find_package(Doxygen)
if(DOXYGEN_FOUND)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
add_custom_target(doc
${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
COMMENT "Generating API documentation with Doxygen" VERBATIM
)
endif(DOXYGEN_FOUND)
