IF(NOT DEV_ROOT_PATH)
	message(SEND_ERROR "No esta seteado DEV_ROOT_PATH")
endif()

set(CP /core/scene)

set(SRCS
	${SRCS}
	${DEV_ROOT_PATH}/core/scene/Entity.cpp
	${DEV_ROOT_PATH}/core/scene/SceneManager.cpp
)

set(HDRS
	${HDRS}
	${DEV_ROOT_PATH}/core/scene/LayerLevel.h
	${DEV_ROOT_PATH}/core/scene/SceneManager.h
	${DEV_ROOT_PATH}/core/scene/Entity.h
)

set(ACTUAL_DIRS
	${DEV_ROOT_PATH}/core/scene
)

include_directories(${ACTUAL_DIRS})
