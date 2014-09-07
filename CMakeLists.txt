cmake_minimum_required(VERSION 2.8)
project(wireless_turnstile)
find_package(OpenCV REQUIRED)
find_package(CURL REQUIRED)

# C++11
set(CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS}")

set(restclient_INCLUDEDIR
    lib/restclient-cpp/include
    )

set(restclient_SOURCES
    lib/restclient-cpp/source/restclient.cpp
    )

set(picojson_INCLUDEDIR
    lib/picojson
    )

set(apitest_SOURCES
    api.cpp
    apitest.cpp
    )

set(imgproctest_SOURCES
    imgproc.cpp
    imgproc_test.cpp
    )

set(app_SOURCES
    app.cpp
    imgproc.cpp
    api.cpp
    settings.cpp
    )

include_directories(${CURL_INCLUDE_DIRS} ${OpenCV_INCLUDE_DIR} ${restclient_INCLUDEDIR} ${picojson_INCLUDEDIR})
add_executable(apitest ${apitest_SOURCES} ${restclient_SOURCES})
target_link_libraries(apitest ${OpenCV_LIBS} ${CURL_LIBRARIES})

add_executable(imgproctest ${imgproctest_SOURCES})
target_link_libraries(imgproctest ${OpenCV_LIBS})

add_executable(app ${app_SOURCES} ${restclient_SOURCES})
target_link_libraries(app ${OpenCV_LIBS} ${CURL_LIBRARIES})
