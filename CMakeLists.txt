cmake_minimum_required(VERSION 3.23)



###############################################
## Tricks
###############################################
# Generate the make files:
#	cd bin
#	cmake ../
# Run the unit tests
#	cd bin
#	cmake ../
#	make
#	ctest


###############################################
## Definitions
###############################################
set(                        DESCRIPTION                         "thread-safe bounded blocking queue")
project(                    test                                VERSION 1.1
                            DESCRIPTION                         ${DESCRIPTION}
                            LANGUAGES                           CXX)
set(                        CMAKE_CXX_STANDARD                  20)
set(                        CMAKE_CXX                           g++)
set(                        CMAKE_CXX_STANDARD_REQUIRED         ON)
set(                        CMAKE_CXX_EXTENSIONS                OFF)

set(                        EXTENSION_SRC                       .cpp)
set(                        EXTENSION_HEADER                    .h)
set(                        EXTENSION_TEST_INPUT                .in)
set(                        EXTENSION_TEST_OUTPUT               .out)

set(                        DIR_APP                             ${CMAKE_SOURCE_DIR}/app)
set(                        DIR_CODE                            ${CMAKE_SOURCE_DIR}/code)
set(                        DIR_SRC                             ${DIR_CODE}/src)
set(                        DIR_HEADER                          ${DIR_CODE}/header)
set(                        DIR_TEST                            ${CMAKE_SOURCE_DIR}/test)

set(                        DIR_RESOURCE                        ${CMAKE_SOURCE_DIR}/resource)
set(                        DIR_TEST_INPUT                      ${DIR_RESOURCE}/input)
set(                        DIR_TEST_OUTPUT                     ${DIR_RESOURCE}/output)

set(                        TEST_KIND_VALGRIND                  valgrind)
set(                        TEST_KIND_LIST                      non_regression
                                                                unit
                                                                ${TEST_KIND_VALGRIND}
                                                                performance)

set(                        EXEC                                main)
set(                        EXEC_DEBUG                          main_debug)

set(                        CMAKE_CXX_FLAGS                     "-O3 -Wall -g") #-Werror -Wextra
SET(                        FLAG_PROD                           "")
SET(                        FLAG_VERBOSE                        "-DLOG")
SET(                        FLAG_TEST                           "-DTEST -DLOG") # TODO pb: only the first flag is considered



#=======================================================
# Manage the config of the project
#=======================================================
message(STATUS "==================================")
message(STATUS "Configurations : " ${DESCRIPTION})
message(STATUS "==================================")
message(STATUS "C++ standard          : " ${CMAKE_CXX_STANDARD})
message(STATUS "C++ compiler          : " ${CMAKE_CXX})
message(STATUS "Flags                 : " ${CMAKE_CXX_FLAGS})


#=======================================================
# Include the headers of the project
#=======================================================
message(STATUS "\n\nInclude custom libraries")
MACRO(HEADER_DIR_LIST lib)
    message(STATUS "\tAdd sub directory header: " ${lib})
    include_directories(${lib})

    FILE(GLOB children ${lib}/*)
    SET(dirlist "")
    FOREACH(child ${children})
        IF(IS_DIRECTORY ${child})
            HEADER_DIR_LIST(${child})
        ENDIF()
    ENDFOREACH()
ENDMACRO()

HEADER_DIR_LIST(${DIR_HEADER})


#=======================================================
# List custom code to compile
#=======================================================
SET(libCustomList "")

message(STATUS "\n\nList custom libraries to compile: ")
file(GLOB_RECURSE libFileList ${DIR_SRC}/*${EXTENSION_SRC})                 # Gather the source files of the custom libraries
message(STATUS "\tAdd source files")
foreach(sourceFile ${libFileList})
    message(STATUS "\t\t${sourceFile}")
    LIST(APPEND libCustomList ${sourceFile})
endforeach()

message(STATUS "\n\nList custom tests to compile:")
foreach(testKind ${TEST_KIND_LIST})                                         # Gather the test source files
    message(STATUS "\tTest kind: ${testKind}")
    file(GLOB_RECURSE testFileList ${DIR_TEST}/${testKind}/*${EXTENSION_SRC})
    foreach(sourceFile ${testFileList})
        message(STATUS "\t\t${sourceFile}")
    endforeach()
endforeach()


#=======================================================
# Compile custom applications
#=======================================================
message(STATUS "\n\nGenerate app executables:")

message(STATUS "\t\t\"${EXEC}\" (no logs, no routines for tests)")
SET (execSourceFile ${DIR_APP}/${EXEC}${EXTENSION_SRC})
add_executable(${EXEC} ${libCustomList} ${execSourceFile})
target_compile_options( ${EXEC} PUBLIC ${FLAG_PROD})

message(STATUS "\t\t\"${EXEC_DEBUG}\" (logs, routines for tests)")
SET (execSourceFile ${DIR_APP}/${EXEC}${EXTENSION_SRC})
add_executable(${EXEC_DEBUG} ${libCustomList} ${execSourceFile})
target_compile_options( ${EXEC_DEBUG} PUBLIC ${FLAG_VERBOSE})


#=======================================================
# Compile custom tests
#=======================================================
MACRO(ADD_CUSTOM_TEST execTest execName FLAG_INPUT_FILE FLAG_OUTPUT_FILE)
    message(STATUS "\t\t\t\"Add test: ${execName}\"")
    add_executable(${execName} ${libCustomList} ${execTest})
    target_compile_options( ${execName} PUBLIC ${FLAG_TEST} ${FLAG_INPUT_FILE} ${FLAG_OUTPUT_FILE})

    IF(${testKind} STREQUAL ${TEST_KIND_VALGRIND})
        add_test(NAME ${execName} COMMAND valgrind --leak-check=full --error-exitcode=1 ${execName})
    ELSE()
        add_test(NAME ${execName} COMMAND ${execName})
    ENDIF()
ENDMACRO()

MACRO(GENERATE_TESTS testFileList)
    foreach(execTest ${testFileList})
        message(STATUS "\t\tTest exec: \"${execTest}\"")
        get_filename_component(testName ${testKind}/${execTest} NAME_WE)
        file(GLOB_RECURSE inputFileList ${DIR_TEST_INPUT}/${testKind}/*${EXTENSION_TEST_INPUT})

        IF(${inputFileList})
            foreach(inputFile ${inputFileList})
                get_filename_component(inputName ${inputFile} NAME_WE)
                set(outputFile ${DIR_TEST_OUTPUT}/${testKind}/${inputName}${EXTENSION_TEST_OUTPUT})

                file(RELATIVE_PATH inputFileRelative  ${CMAKE_SOURCE_DIR} ${inputFile})
                file(RELATIVE_PATH outputFileRelative ${CMAKE_SOURCE_DIR} ${outputFile})

                set(FLAG_INPUT_FILE  "-DTEST_INPUT_FILE=\"${inputFileRelative}\"")
                set(FLAG_OUTPUT_FILE "-DTEST_OUTPUT_FILE=\"${outputFileRelative}\"")

                set(execName "${testName}_${inputName}")
                ADD_CUSTOM_TEST (${execTest} ${execName} ${FLAG_INPUT_FILE} ${FLAG_OUTPUT_FILE})
            endforeach()
        ELSE()
            set(execName "${testName}")
            ADD_CUSTOM_TEST (${execTest} ${execName} "" "")
        ENDIF()
    endforeach()
ENDMACRO()


include(CTest)                                                              # Part of CMake, no extra frameworks / lib
enable_testing()

message(STATUS "\n\nGenerate CTest executables:")
foreach(testKind ${TEST_KIND_LIST})

    message(STATUS "\tTest kind: \"${testKind}\"")
    file(GLOB_RECURSE testFileList ${DIR_TEST}/${testKind}/*${EXTENSION_SRC})
    if (testFileList)
        GENERATE_TESTS("${testFileList}")
    else()
        message(STATUS "\t\t*****No test class found")
    endif()
endforeach()

