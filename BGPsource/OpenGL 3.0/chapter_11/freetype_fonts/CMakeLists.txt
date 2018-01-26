CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

SET(APP_NAME freetype_fonts)

PROJECT(${APP_NAME})

IF(WIN32)
	ADD_DEFINITIONS(-D_WIN32)
    SET(SOURCE_FILES 
        src/main.cpp
        src/example.cpp
        src/glwindow.cpp
		src/freetypefont.cpp
		src/glee/GLee.c
    )
ELSE(WIN32)
    SET(SOURCE_FILES 
		src/main.cpp
		src/freetypefont.cpp
        src/example.cpp
        src/glxwindow.cpp
		src/glee/GLee.c
    )
ENDIF(WIN32)

IF (WIN32)
INCLUDE_DIRECTORIES( ${PROJECT_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/src/freetype/include ${CMAKE_SOURCE_DIR}/src/freetype/include/freetype2)
LINK_DIRECTORIES( ${PROJECT_SOURCE_DIR}/src/freetype/lib )
ELSE (WIN32)
INCLUDE_DIRECTORIES( ${PROJECT_SOURCE_DIR}/include /usr/include/freetype2)
ENDIF (WIN32)

IF(WIN32)
	ADD_EXECUTABLE(${APP_NAME} WIN32 ${SOURCE_FILES})
ELSE(WIN32)
	ADD_EXECUTABLE(${APP_NAME} ${SOURCE_FILES})
ENDIF(WIN32)

IF(WIN32)
	SET(LIBRARIES OPENGL32 GLU32 freetype)
ELSE(WIN32)
	SET(LIBRARIES GL GLU Xxf86vm freetype)
ENDIF(WIN32)

TARGET_LINK_LIBRARIES(${APP_NAME} ${LIBRARIES})
