IF(NOT DEV_ROOT_PATH)
	message(SEND_ERROR "No esta seteado DEV_ROOT_PATH")
endif()

set(CP /common/configuration)

set(SRCS
	${SRCS}
	${DEV_ROOT_PATH}/common/configuration/Config.cpp
)

set(HDRS
	${HDRS}
	${DEV_ROOT_PATH}/common/configuration/Config.h
)

set(ACTUAL_DIRS
	${DEV_ROOT_PATH}/common/configuration
)

include_directories(${ACTUAL_DIRS})
