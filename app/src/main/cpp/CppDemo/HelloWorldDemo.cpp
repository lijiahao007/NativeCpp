//
// Created by ljh on 2025/2/24.
//

#include "HelloWorldDemo.h"
#include <limits>
#include <iostream>

using namespace NativeCpp;
using namespace CppDemo;
using namespace Utils;
using namespace std;

namespace {
    const char* const TAG = "HelloWorldDemo";
}

void HelloWorldDemo::logHelloWorld() {
    LogUtils::info(TAG, "HelloWorld!!!");
}

void HelloWorldDemo::基本类型最大最小值() {
    int maxValue = numeric_limits<int>::max();
    int minValue = numeric_limits<int>::min();
    float maxFloatValue = numeric_limits<float>::max();
    float minFloatValue = numeric_limits<float>::min();
    float lowestFloatValue = numeric_limits<float>::lowest();

    LogUtils::info(TAG, "max[" + to_string(maxValue) + "] min[" + to_string(minValue) + "]");
    LogUtils::info(TAG, "maxF[" + to_string(maxFloatValue) + "] minF[" + to_string(minFloatValue) + "] lowestF[" +
            to_string(lowestFloatValue));
}
