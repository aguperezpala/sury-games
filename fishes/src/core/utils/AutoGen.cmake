IF(NOT DEV_ROOT_PATH)
	message(SEND_ERROR "No esta seteado DEV_ROOT_PATH")
endif()

set(CP /core/utils)

set(SRCS
	${SRCS}
	${DEV_ROOT_PATH}/core/utils/AnimatedSprite.cpp
	${DEV_ROOT_PATH}/core/utils/TextureAtlas.cpp
)

set(HDRS
	${HDRS}
	${DEV_ROOT_PATH}/core/utils/TextureAtlas.h
	${DEV_ROOT_PATH}/core/utils/AnimatedSprite.h
)

set(ACTUAL_DIRS
	${DEV_ROOT_PATH}/core/utils
)

include_directories(${ACTUAL_DIRS})
