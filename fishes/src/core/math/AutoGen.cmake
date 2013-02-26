IF(NOT DEV_ROOT_PATH)
	message(SEND_ERROR "No esta seteado DEV_ROOT_PATH")
endif()

set(CP /core/math)

set(SRCS
	${SRCS}
	${DEV_ROOT_PATH}/core/math/Matrix4.cpp
)

set(HDRS
	${HDRS}
	${DEV_ROOT_PATH}/core/math/Matrix4.h
	${DEV_ROOT_PATH}/core/math/Vector2.h
	${DEV_ROOT_PATH}/core/math/AABB.h
)

set(ACTUAL_DIRS
	${DEV_ROOT_PATH}/core/math
)

include_directories(${ACTUAL_DIRS})
