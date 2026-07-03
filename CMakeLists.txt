cmake_minimum_required(VERSION 3.5)

project(shelly-1pm-mini-rainmaker)

set(EXTRA_COMPONENT_DIRS "$ENV{IDF_PATH}/components/esp-rainmaker/components")

include($ENV{IDF_PATH}/tools/cmake/project.cmake)

project_goals()