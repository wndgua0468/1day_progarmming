# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\DBC_Edit_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\DBC_Edit_autogen.dir\\ParseCache.txt"
  "DBC_Edit_autogen"
  )
endif()
