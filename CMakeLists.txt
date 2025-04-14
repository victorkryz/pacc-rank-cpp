cmake_minimum_required(VERSION 3.14)

set(app pacc-rank)
project(${app})

set(CMAKE_GENERATOR "Ninja")
message("generator is set to ${CMAKE_GENERATOR}")

set(CMAKE_CXX_STANDARD 20)

set(CURL_INCLUDE_DIRS "")
set(CURL_LIBRARIES "")	

file(GLOB source_files RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} src/*.h src/*.hpp src/*.cpp src/*.c)

if (WIN32)

	include(FetchContent)
	FetchContent_Declare(
	  curl
	  URL https://curl.se/windows/dl-8.13.0_1/curl-8.13.0_1-win64-mingw.zip
	  DOWNLOAD_EXTRACT_TIMESTAMP ON
	  )

	FetchContent_MakeAvailable(curl)

	set(CURL_INCLUDE_DIRS "${FETCHCONTENT_BASE_DIR}/curl-src/include")
	set(CURL_LIBRARIES "${FETCHCONTENT_BASE_DIR}/curl-src/lib/libcurl.dll.a")

else()

	set(CURL_LIBRARIES curl)

endif()

add_executable(${app} ${source_files})

target_include_directories(pacc-rank PRIVATE ${CURL_INCLUDE_DIRS})
target_link_libraries(${app} ${CURL_LIBRARIES})

