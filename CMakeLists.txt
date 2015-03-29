cmake_minimum_required(VERSION 3.0)
project(kasan-notes)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

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

find_package (Boost COMPONENTS system)
add_definitions(-DBOOST_THREAD_USE_LIB)
include_directories(${Boost_INCLUDE_DIRS})

set(CMAKE_CXX_FLAGS "-g -std=gnu++11 -fPIC")
add_definitions(-DELPP_QT_LOGGING)

set(sources
	log.cpp

	app/desktop/kasannotesapplication.cpp
	app/desktop/desktopfilelocator.cpp
	
	core/backends/kasandb/bmxppacket.cpp
	core/backends/kasandb/bmxpsocket.cpp
	core/backends/kasandb/kasandbauthenticator.cpp
	core/backends/kasandb/kasandbbackend.cpp
)

add_executable(kasan-notes main.cpp ${sources})
target_link_libraries(kasan-notes)

qt5_use_modules(kasan-notes Widgets Gui Quick Qml Network)
