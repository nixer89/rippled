#[===================================================================[
   read version from source
#]===================================================================]

file (STRINGS src/xrpl/protocol/impl/BuildInfo.cpp BUILD_INFO)
foreach (line_ ${BUILD_INFO})
  if (line_ MATCHES "versionString[ ]*=[ ]*\"(.+)\"")
    set (rippled_version ${CMAKE_MATCH_1})
  endif ()
endforeach ()
if (rippled_version)
  message (STATUS "xrpld version: ${rippled_version}")
else ()
  message (FATAL_ERROR "unable to determine xrpld version")
endif ()
