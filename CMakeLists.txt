cmake_minimum_required(VERSION 3.0)
project(kasan-notes)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/3rdparty/cmake")

include_directories(${CMAKE_SOURCE_DIR}/3rdparty)

find_package(Qt5Widgets)
find_package(Qt5Gui)
find_package(Qt5Quick)
find_package(Qt5Qml)
find_package(Qt5Network)

include_directories(${Qt5Widgets_INCLUDE_DIRS})
include_directories(${Qt5Gui_INCLUDE_DIRS})
include_directories(${Qt5Quick_INCLUDE_DIRS})
include_directories(${Qt5Qml_INCLUDE_DIRS})
include_directories(${Qt5Network_INCLUDE_DIRS})

set (Boost_USER_MULTITHREADED ON)

find_package (Boost COMPONENTS filesystem system)
add_definitions(-DBOOST_THREAD_USE_LIB)
include_directories(${Boost_INCLUDE_DIRS})

set(CMAKE_CXX_FLAGS "-g -std=gnu++14 -fPIC")
add_definitions(-DELPP_QT_LOGGING)

find_library(QEVERCLOUD qt5qevercloud PATHS /usr/local/lib)

set(sources
	log.cpp

	app/desktop/kasannotesapplication.cpp
	app/desktop/desktopfilelocator.cpp
	app/desktop/mainwindowcontroller.cpp
	
	core/backends/common/backend.cpp
	core/backends/common/note.cpp

	core/backends/evernote/evernotebackend.cpp
	core/backends/evernote/evernotenote.cpp
)
add_executable(kasan-notes main.cpp ${sources})
target_link_libraries(kasan-notes ${Boost_LIBRARIES} -lqt5qevercloud)

qt5_use_modules(kasan-notes Widgets Gui Quick Qml Network)

