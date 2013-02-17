IF(NOT DEV_ROOT_PATH)
	message(SEND_ERROR "No esta seteado DEV_ROOT_PATH")
endif()

set(CP /core/ui)

set(SRCS
	${SRCS}
	${DEV_ROOT_PATH}/core/ui/AnimatedSprite.cpp
)

set(HDRS
	${HDRS}
	${DEV_ROOT_PATH}/core/ui/AnimatedSprite.h
)

set(ACTUAL_DIRS
	${DEV_ROOT_PATH}/core/ui
)

include_directories(${ACTUAL_DIRS})
