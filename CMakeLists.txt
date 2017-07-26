cmake_minimum_required(VERSION 3.0)
project(kasan-notes)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/3rdparty/cmake")

include_directories(${CMAKE_SOURCE_DIR}/3rdparty)
include_directories(${CMAKE_SOURCE_DIR}/3rdparty/doctest)

find_package(Qt5Widgets)
find_package(Qt5Gui)
find_package(Qt5Quick)
find_package(Qt5Qml)
find_package(Qt5Network)
find_package(Qt5Xml)

include_directories(${Qt5Widgets_INCLUDE_DIRS})
include_directories(${Qt5Gui_INCLUDE_DIRS})
include_directories(${Qt5Network_INCLUDE_DIRS})
include_directories(${Qt5Xml_INCLUDE_DIRS})

set (Boost_USER_MULTITHREADED ON)

find_package (Boost COMPONENTS filesystem system)
add_definitions(-DBOOST_THREAD_USE_LIB)
include_directories(${Boost_INCLUDE_DIRS})

set(CMAKE_CXX_FLAGS "-g -std=gnu++14 -fPIC")
add_definitions(-DELPP_QT_LOGGING)

add_definitions(-DELPP_NO_DEFAULT_LOG_FILE=1 -DELPP_NO_CHECK_MACROS=1)
#add_definitions(-DDOCTEST_CONFIG_NO_SHORT_MACRO_NAMES=1)

find_library(QEVERCLOUD qt5qevercloud PATHS /usr/local/lib)

set(3rdparty-sources
	3rdparty/easylogging/easylogging++.cc
)

set(sources
	log.cpp

	app/desktop/kasannotesapplication.cpp
	app/desktop/desktopfilelocator.cpp
	
	core/backends/common/attachment.cpp
	core/backends/common/backend.cpp
	core/backends/common/note.cpp
	core/backends/common/notebook.cpp

	core/backends/evernote/evernotebackend.cpp
	core/backends/evernote/evernotenote.cpp
	
	core/notestorage/notestorage.cpp
	core/notestorage/notestoragemodel.cpp
	
	ui/desktop/mainwindow.cpp
)

add_executable(kasan-notes main.cpp ${sources} ${3rdparty-sources} ${test_sources})
target_link_libraries(kasan-notes ${Boost_LIBRARIES} -lqt5qevercloud)
qt5_use_modules(kasan-notes Widgets Gui Network Xml)

add_executable(kasan-notes-tests test.cpp ${sources} ${3rdparty-sources} ${test_sources})
target_link_libraries(kasan-notes-tests ${Boost_LIBRARIES} -lqt5qevercloud)
qt5_use_modules(kasan-notes-tests Widgets Gui Network Xml)

