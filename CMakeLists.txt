##---------------------------------------------------------------------------
## Author:      nohal aka. Pavel Kalian  Copyright: License:     wxWidgets License
##---------------------------------------------------------------------------
# Frontend2 Author: Jon Gough  
# Frontend2: v1.0.132		
##---------------------------------------------------------------------------
## ----- When changing this file do NOT change the order in which sections occur        ----- ##
## ----- Changes should only be made between the section blocks that identify where     ----- ##
## ----- these changes should be. The whole configuration process relies on this        ----- ##
## ----- sequence to be successful                                                      ----- ##
##
##
## ----- Modify section below to include all the details for your plugin ----- ##
##

set(CMLOC "CMakeLists: ")

# define minimum cmake version
cmake_minimum_required(VERSION 3.1.1)
if(COMMAND cmake_policy)
    if(POLICY CMP0043)
        cmake_policy(SET CMP0043 NEW)
    endif(POLICY CMP0043)
    cmake_policy(SET CMP0048 NEW)
    if(POLICY CMP0077)
        cmake_policy(SET CMP0077 NEW)
    endif(POLICY CMP0077)
endif(COMMAND cmake_policy)

# define plugin name, owner and versions
set(VERBOSE_NAME "FindIt")
set(COMMON_NAME "FindIt")
set(TITLE_NAME "FINDIT")
set(PACKAGE_CONTACT "Del Elson")
set(PACKAGE "findit")
set(SHORT_DESCRIPTION "Stowage PlugIn for OpenCPN")
set(LONG_DESCRIPTION "Stowage PlugIn for OpenCPN")

set(VERSION_MAJOR "1")
set(VERSION_MINOR "2")
set(VERSION_PATCH "6")
set(VERSION_TWEAK "0")
set(VERSION_DATE "08/09/2020")
set(OCPN_MIN_VERSION "ov50")
set(OCPN_API_VERSION_MAJOR "1")
set(OCPN_API_VERSION_MINOR "16")
set(TP_COMMENT "  * Release for O5 using CI")

set(PARENT "opencpn")

# The next line allows for setting up your own local webserver with git on it for testing purposes, the default is github.com
#set(GIT_REPOSITORY_SERVER "github.com")

# Specifies cloudsmith repository sufficies
set(PROD "prod") # Standard Repos
set(BETA "beta") # Standard Repos
set(ALPHA "alpha") # Standard Repos

# set(CLOUDSMITH_USER "rick-gleasoh")
set(CLOUDSMITH_USER "opencpn")
# set this if your cloudsmith repository name does not match your git repository name set(CLOUDSMITH_BASE_REPOSITORY "testplugin_pi")
set(CLOUDSMITH_BASE_REPOSITORY "findit")  #without the pi

set(XML_INFO_URL "https://opencpn.org/OpenCPN/plugins/findit.html")
set(XML_SUMMARY ${SHORT_DESCRIPTION})
set(XML_DESCRIPTION ${LONG_DESCRIPTION})

##
## ----- Modify section above to include all the details for your plugin ----- ##
## ----- Modify section below if there are special requirements for the plugin ----- ##
##

option(PLUGIN_USE_SVG "Use SVG graphics" ON)

set(CMAKE_CXX_STANDARD 11)

# Prefer libGL.so to libOpenGL.so, see CMP0072
set(OpenGL_GL_PREFERENCE "LEGACY")
# Don't use local version of GLU library
set(USE_LOCAL_GLU FALSE)
option(USE_GL "Enable OpenGL support" OFF)
message(STATUS "${CMLOC}USE_GL: ${USE_GL}")

if("${CMAKE_BUILD_TYPE}" STREQUAL "")
    set(CMAKE_BUILD_TYPE
        "Release"
        CACHE STRING "Choose the type of build" FORCE)
endif("${CMAKE_BUILD_TYPE}" STREQUAL "")
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "" "Debug" "Release" "RelWithDebInfo" "MinSizeRel")
message(STATUS "${CMLOC}Build type: ${CMAKE_BUILD_TYPE}")

##
## ----- The statements below are used to setup standard variables that are required by the CMAKE process - do not remove ----- ##
##

project(${PACKAGE})
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake;")

#========================================================
include("PluginSetup")
#========================================================

##
## ----- The above is used to setup standard variables that are required by the CMAKE process - do not remove ----- ##
## ----- Modify section below if there are special requirements for the plugin ----- ##
##

set(CMAKE_VERBOSE_MAKEFILE ON)

option(Plugin_CXX11 "Use c++11" OFF)

##
## ----- Modify section above if there are special requirements for the plugin ----- ##
## ----- do not change next section - needed to configure build process ----- ##
##

#========================================================
include("PluginConfigure")
#========================================================

##
## ----- do not change section above - needed to configure build process ----- ##
## ----- Change below to match project requirements for source, headers, etc. ----- ##
##

add_definitions(-DUSE_S57)

SET(SRC_FINDIT
	src/findit_pi.cpp
	src/findit_pi.h
	src/findit.cpp
	src/findit.h
	src/icons.cpp
	src/icons.h
	src/gui.cpp
	src/gui.h
	src/json_defs.h
	src/jsonreader.h
	src/jsonval.h
   	src/jsonwriter.h
   	src/jsonreader.cpp
   	src/jsonval.cpp
   	src/jsonwriter.cpp
	api-16/ocpn_plugin.h
)

IF(WIN32)
SET(SRC_TINYXML
	src/tinyxml/tinyxml.cpp
	src/tinyxml/tinyxml.h
	src/tinyxml/tinyxmlerror.cpp
	src/tinyxml/tinyxmlparser.cpp
	src/tinyxml/tinystr.cpp
	src/tinyxml/tinystr.h 
)
ENDIF(WIN32)

add_definitions(-DPLUGIN_USE_SVG)

if(APPLE)
    option(PI_JSON_SCHEMA_VALIDATOR "Use JSON Schema validator" OFF)
else(APPLE)
    option(PI_JSON_SCHEMA_VALIDATOR "Use JSON Schema validator" ON)
endif(APPLE)

if(PI_JSON_SCHEMA_VALIDATOR)
    message(STATUS "${CMLOC}Using JSON Schema validation - warning this changes the 'flavour' of the executable and it may not be compatible with OpenCPN")
    add_definitions(-DPI_JSON_SCHEMA_VALIDATOR)
    message(STATUS "${CMLOC}Adding JSON Schema validation")

    set(EXTSRC ${EXTSRC} extsrc/json-schema-validator/json-schema-draft7.json.cpp extsrc/json-schema-validator/json-uri.cpp extsrc/json-schema-validator/json-validator.cpp)
    set(EXTINCLUDE ${EXTINCLUDE} extinclude/nlohmann/json.hpp extinclude/json-schema-validator/json-schema.hpp)
    include_directories(BEFORE ${PROJECT_SOURCE_DIR}/extinclude/json-schema-validator)
else(PI_JSON_SCHEMA_VALIDATOR)
    message(STATUS "${CMLOC}Not using JSON Schema validation - warning invalid JSON 'may' cause unexpected crashes")
endif(PI_JSON_SCHEMA_VALIDATOR)

#include_directories(BEFORE ${PROJECT_SOURCE_DIR}/ocpnsrc/bitmaps)
include_directories(BEFORE ${PROJECT_SOURCE_DIR}/api-16)

# add_library(${PACKAGE_NAME} SHARED ${SRCS} ${HDRS} ${OCPNSRC} ${EXTSRC} ${LIBSSRC})

add_library(${PACKAGE_NAME} SHARED ${SRC_FINDIT} ${SRC_TINYXML})

add_definitions(-DTIXML_USE_STL)

if(UNIX AND NOT APPLE)
    if(PI_JSON_SCHEMA_VALIDATOR)
        # Change OS/ABI to be UNIX - System V to make it run with OCPN
        set(lib_name lib${PACKAGE_NAME}.so)
        message(STATUS "${CMLOC}lib-name: ${lib_name}")
        find_file(elfedit elfedit)
        if(NOT (${elfedit} STREQUAL "elfedit-NOTFOUND"))
            message(STATUS "${CMLOC}Will ensure OS/ABI is set to 0 to match OCPN")
            add_custom_command(
                TARGET ${PACKAGE_NAME}
                POST_BUILD
                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                DEPENDS ${PACKAGE_NAME}
                COMMENT " Running post build action on ${lib_name}."
                COMMAND sh -c 'elfedit --input-osabi=Linux --output-osabi=none ${lib_name} || (echo 0) ')
        else(NOT (${elfedit} STREQUAL "elfedit-NOTFOUND"))
            message(STATUS "${CMLOC}Cannot correct OS/ABI to match OCPN if it is generated incorrectly. Package binutils needed")
        endif(NOT (${elfedit} STREQUAL "elfedit-NOTFOUND"))
    endif(PI_JSON_SCHEMA_VALIDATOR)
endif(UNIX AND NOT APPLE)

##
## ----- Change above to match project requirements ----- ##
##

##
## ----- do not change next section - needed to configure build process ----- ##
##
#========================================================
include("PluginInstall")
include("PluginLocalization")
include("PluginPackage")
#========================================================
##
## ----- do not change section above - needed to configure build process ----- ##
##
