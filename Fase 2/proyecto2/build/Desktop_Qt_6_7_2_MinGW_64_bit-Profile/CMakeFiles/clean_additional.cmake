# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "RelWithDebInfo")
  file(REMOVE_RECURSE
  "CMakeFiles\\proyecto2_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\proyecto2_autogen.dir\\ParseCache.txt"
  "proyecto2_autogen"
  )
endif()
