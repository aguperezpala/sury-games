IF(NOT DEV_ROOT_PATH)
	message(SEND_ERROR "No esta seteado DEV_ROOT_PATH")
endif()

set(CP /core/resource_manager)

set(SRCS
	${SRCS}
	${DEV_ROOT_PATH}/core/resource_manager/ResourceManager.cpp
)

set(HDRS
	${HDRS}
	${DEV_ROOT_PATH}/core/resource_manager/ResourceManager.h
)

set(ACTUAL_DIRS
	${DEV_ROOT_PATH}/core/resource_manager
)

include_directories(${ACTUAL_DIRS})
