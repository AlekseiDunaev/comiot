cmake_minimum_required(VERSION 2.6)
project(Comiot)

add_executable(comiot main.c)

INSTALL(TARGETS comiot RUNTIME DESTINATION /usr/bin/)
