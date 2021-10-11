# CMakeList.txt: проект CMake для deque; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#
cmake_minimum_required (VERSION 3.8)

project ("deque")

# Добавьте источник в исполняемый файл этого проекта.
add_executable (deque  "deque.h" "main.cpp" "allocator.h" )

# TODO: Добавьте тесты и целевые объекты, если это необходимо.
