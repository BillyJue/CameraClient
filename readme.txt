1.search_sdk_define.h 头文件注释以下代码，并再其添加头文件#include<Windows.h>

// 结构体定义

// // Windows OS下字节紧缩对齐
 #ifdef WIN32
 #pragma pack( push )
 #pragma pack( 1 )
#endif
// #define window( x )	x
// #else
// #include <netinet/in.h>
// #define window( x )
// #endif

2.如果依赖项目的版本过低，可能会报错，先升级到当前的编译器就可以了