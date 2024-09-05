cmake_minimum_required(VERSION 3.10)

# Nome do projeto
project(SchedulerRateMonotonic)

# Definir o padrão do compilador C como C11
set(CMAKE_C_STANDARD 11)

# Adicionar o executável
add_executable(scheduler main.c)

# Incluir o diretório onde a biblioteca json-c está instalada
find_package(PkgConfig REQUIRED)
pkg_check_modules(JSON_C REQUIRED json-c)

# Adicionar diretórios de include e ligar a biblioteca json-c
include_directories(${JSON_C_INCLUDE_DIRS})
target_link_libraries(scheduler ${JSON_C_LIBRARIES} m)

