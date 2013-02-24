IF(NOT DEV_ROOT_PATH)
	message(SEND_ERROR "No esta seteado DEV_ROOT_PATH")
endif()

set(CP /core/space_partition)

set(SRCS
	${SRCS}
	${DEV_ROOT_PATH}/core/space_partition/SpacePartition.cpp
)

set(HDRS
	${HDRS}
	${DEV_ROOT_PATH}/core/space_partition/TwoDimCell.h
	${DEV_ROOT_PATH}/core/space_partition/Object.h
	${DEV_ROOT_PATH}/core/space_partition/SpacePartition.h
	${DEV_ROOT_PATH}/core/space_partition/Defines.h
)

set(ACTUAL_DIRS
	${DEV_ROOT_PATH}/core/space_partition
)

include_directories(${ACTUAL_DIRS})
