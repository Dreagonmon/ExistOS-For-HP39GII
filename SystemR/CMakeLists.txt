
include_directories(./)
include_directories(./Config)
include_directories(./RTOSCore)
include_directories(./RTOSCore/include)
include_directories(./RTOSCore/porting)
include_directories(./Fs)
include_directories(./Fs/LittleFs)
include_directories(./ExistOS)
include_directories(./ExistOS/Time)
include_directories(./ExistOS/Settings)
include_directories(./Graphic)
include_directories(./Graphic/UI)
include_directories(./Keyboard)
include_directories(./Utils)
include_directories(./Utils/Collectc)
include_directories(./Apps)

aux_source_directory(.                                             SYS_SRCS)
aux_source_directory(./RTOSCore                                    SYS_SRCS)
aux_source_directory(./RTOSCore/porting                            SYS_SRCS)
aux_source_directory(./Fs                                          SYS_SRCS)
aux_source_directory(./Fs/LittleFs                                 SYS_SRCS)
aux_source_directory(./ExistOS                                     SYS_SRCS)
aux_source_directory(./ExistOS/Time                                SYS_SRCS)
aux_source_directory(./ExistOS/Settings                            SYS_SRCS)
aux_source_directory(./Graphic                                     SYS_SRCS)
aux_source_directory(./Graphic/UI                                  SYS_SRCS)
aux_source_directory(./Keyboard                                    SYS_SRCS)
aux_source_directory(./Utils                                       SYS_SRCS)
aux_source_directory(./Utils/Collectc                              SYS_SRCS)
aux_source_directory(./Apps                                        SYS_SRCS)

# ==== add apps here ====
add_subdirectory(./Apps/Settings)

add_executable(sys.elf ${SYS_SRCS})

target_link_libraries(sys.elf -lm -lc -lrdimon -lgcc -nostartfiles
# ==== add apps here ====
app_settings
)

set(LINKER_SCRIPT 
    ${CMAKE_SOURCE_DIR}/Script/sys_ld.script
)
set(LINKER_FLAGS "SHELL:-T${LINKER_SCRIPT} -Wl,--wrap=malloc -Wl,--wrap=free")
target_link_options(sys.elf PRIVATE ${LINKER_FLAGS}) 

target_compile_options(sys.elf PRIVATE -mtune=arm926ej-s -mcpu=arm926ej-s -mlittle-endian -mfloat-abi=soft -marm
    -Os -pipe
    -DHP39
    -fexec-charset=UTF-8
)

add_custom_command(TARGET sys.elf  POST_BUILD
    COMMAND arm-none-eabi-size ${CMAKE_CURRENT_BINARY_DIR}/sys.elf 
)

add_custom_target(
    ExistOS.sys ALL
    COMMAND ${CMAKE_OBJCOPY} -I elf32-little -O binary -R .note -R .comment -S ${CMAKE_CURRENT_BINARY_DIR}/sys.elf ${CMAKE_CURRENT_BINARY_DIR}/ExistOS.sys 
    COMMAND ${CMAKE_NM} -n ${CMAKE_CURRENT_BINARY_DIR}/sys.elf > ${CMAKE_CURRENT_BINARY_DIR}/sys_symtab.txt 
    COMMAND ${CMAKE_SOURCE_DIR}/tools/sysigner ${CMAKE_CURRENT_BINARY_DIR}/sys_symtab.txt ${CMAKE_CURRENT_BINARY_DIR}/ExistOS.sys 
    DEPENDS sys.elf 
)

add_custom_target(
    edb_flash_sysr 
    COMMAND sudo ${CMAKE_SOURCE_DIR}/tools/edb -r -s -f ${CMAKE_CURRENT_BINARY_DIR}/ExistOS.sys 1984
    DEPENDS ExistOS.sys
)
