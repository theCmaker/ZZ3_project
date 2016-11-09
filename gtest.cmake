include ( ExternalProject )

set_directory_properties ( PROPERTIES EP_PREFIX ${CMAKE_BINARY_DIR}/ThirdParty )

externalproject_add (
 googletest

 SVN_REPOSITORY https://github.com/stp/googletest/trunk/

 TIMEOUT 10

 CMAKE_ARGS -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG:PATH=DebugLibs
            -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE:PATH=ReleaseLibs
            -Dgtest_force_shared_crt=ON

 INSTALL_COMMAND ""

 LOG_DOWNLOAD ON
 LOG_CONFIGURE ON
 LOG_BUILD ON
)

externalproject_get_property ( googletest source_dir )
include_directories ( ${source_dir}/include )

externalproject_get_property ( googletest binary_dir )
link_directories ( ${binary_dir} )
