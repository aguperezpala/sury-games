IF(NOT DEV_ROOT_PATH)
	message(SEND_ERROR "No esta seteado DEV_ROOT_PATH")
endif()

set(CP /core/system)

set(SRCS
	${SRCS}
	${DEV_ROOT_PATH}/core/system/GlobalConfig.cpp
)

set(HDRS
	${HDRS}
	${DEV_ROOT_PATH}/core/system/GlobalConfig.h
)

set(ACTUAL_DIRS
	${DEV_ROOT_PATH}/core/system
)

include_directories(${ACTUAL_DIRS})
