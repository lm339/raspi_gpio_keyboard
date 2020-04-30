cmake_minimum_required(VERSION 2.8)

SET(COMPILE_DEFINITIONS -Werror)

include_directories(/media/lm339/lm339/Lakka-LibreELEC/build.Lakka-RPi.arm-2.2-devel/toolchain/armv6zk-libreelec-linux-gnueabi/sysroot/usr/include/glib-2.0)
include_directories(/media/lm339/lm339/Lakka-LibreELEC/build.Lakka-RPi.arm-2.2-devel/toolchain/armv6zk-libreelec-linux-gnueabi/sysroot/usr/lib/glib-2.0/include)

link_directories(/media/lm339/lm339/Lakka-LibreELEC/build.Lakka-RPi.arm-2.2-devel/toolchain/armv6zk-libreelec-linux-gnueabi/sysroot/usr/lib)

add_executable(uinput-key main.c gpio_key.c uinput_keyboard.c)

target_link_libraries(uinput-key libwiringPi.so glib-2.0 pthread)
