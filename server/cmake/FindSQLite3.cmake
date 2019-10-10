find_path(SQLite3_INCLUDE_DIRS NAMES sqlite3.h)

find_library(SQLite3_LIBRARIES NAMES sqlite3 sqlite)

if(SQLite3_INCLUDE_DIR)
	file(STRINGS ${SQLite3_INCLUDE_DIRS}/sqlite3.h _ver_line
		REGEX "^#define SQLITE_VERSION  *\"[0-9]+\\.[0-9]+\\.[0-9]+\""
		LIMIT_COUNT 1)
	string(REGEX MATCH "[0-9]+\\.[0-9]+\\.[0-9]+"
		SQLite3_VERSION "${_ver_line}")
	unset(_ver_line)
endif()

if(SQLite3_FOUND)
	set(SQLite3_INCLUDE_DIRS ${SQLite3_INCLUDE_DIRS})
	set(SQLite3_LIBRARIES ${SQLite3_LIBRARIES})
	if(NOT TARGET SQLite::SQLite3)
		add_library(SQLite::SQLite3 UNKNOWN IMPORTED)
		set_target_properties(SQLite::SQLite3 PROPERTIES
			IMPORTED_LOCATION             "${SQLite3_LIBRARIES}"
			INTERFACE_INCLUDE_DIRECTORIES "${SQLite3_INCLUDE_DIRS}")
	endif()
endif()
