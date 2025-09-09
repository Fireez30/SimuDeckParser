# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/SimuDeckParser_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/SimuDeckParser_autogen.dir/ParseCache.txt"
  "SimuDeckParser_autogen"
  )
endif()
