cmake_minimum_required(VERSION 3.13)

add_library(
    PluginHandler SHARED
    plugin_handler.cpp
)

add_library(
    Plugin SHARED
    plugin.cpp
)

target_include_directories(Plugin PUBLIC ${SIMULATOR_ROOT})
target_include_directories(PluginHandler PUBLIC ${SIMULATOR_ROOT})

set(SOURCES plugin.cpp
            plugin_handler.cpp
)

add_library(plugin_impl OBJECT ${SOURCES})
target_include_directories(plugin_impl PUBLIC ${SIMULATOR_ROOT})
