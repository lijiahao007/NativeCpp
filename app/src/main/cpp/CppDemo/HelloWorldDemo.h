//
// Created by ljh on 2025/2/24.
//

#ifndef NATIVECPP_CPPDEMO_HELLOWORLDDEMO_H
#define NATIVECPP_CPPDEMO_HELLOWORLDDEMO_H

#include "LogUtils.h"
#include <string>

namespace NativeCpp {
    namespace CppDemo {
        class HelloWorldDemo {
        public:
            static void pointerDemo();
            static void pointerDemo1(int (*matrix)[5], int rows);
            static void logHelloWorld();
            static void 基本类型最大最小值();
            static void stringDemo();
            static void vectorArrayDemo();
            static void print2DArr(int (*arr)[5], int size);
            static void arrDemo();
            static void funcDemo(); // 函数指针demo
        private:
            static std::string printArray(const std::array<int, 10>& array);
            static double funcTools(double (*cal)(double, double), double x, double y);
        };
    }
}
#endif //NATIVECPP_CPPDEMO_HELLOWORLDDEMO_H
