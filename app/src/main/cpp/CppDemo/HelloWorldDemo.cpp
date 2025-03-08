//
// Created by ljh on 2025/2/24.
//

#include "HelloWorldDemo.h"
#include <limits>
#include <iostream>
#include <array>
#include <cctype>

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

    char a[] = "Hello" "World";

    LogUtils::info(a, "What");

    struct Data {
        char buffer[32]; // 定长字符数组
    };

    Data b;
    strcpy(b.buffer, "Hello");

    Data c = b; // 结构体赋值（逐成员拷贝）
    strcpy(c.buffer, "World");

    LogUtils::info(TAG, string(b.buffer) + " -- " + string(c.buffer));

}

void HelloWorldDemo::pointerDemo() {
    int matrix[2][5] {
            {0,1,2,3,4},
            {5,6,7,8,9}
    };

    pointerDemo1(matrix, 2);
}

void HelloWorldDemo::pointerDemo1(int (*matrix)[5], int rows) {
    ostringstream oss;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < 5; j++) {
            oss << matrix[i][j] << ", ";
        }
        oss << "\n";
    }

    LogUtils::info(TAG, oss.str());


}

void HelloWorldDemo::stringDemo() {
    char food[20] {"Helloworld"};

    ostringstream oss;
    oss << "food before " << food << endl;
    strcpy(food, "What?");
    oss << "food after " << food << endl;

    strncpy(food, "What123456789012334556789123123123", 19);
    food[19] = '\0';
    oss << "food after 1" << food << endl;

    LogUtils::info(TAG, oss.str());
}

void HelloWorldDemo::vectorArrayDemo() {
    array<int, 10> intArr{1,2,3,4,5,6,7,8,9,0};
    array<int, 10> copyArr = intArr;
    intArr[0] = 100;

    LogUtils::info(TAG, printArray(intArr));
    LogUtils::info(TAG, printArray(copyArr));

    ostringstream oss;
    oss << "oss:" << boolalpha << true << " " << false << " " << true << " " << noboolalpha << true << " " << false << endl;
    LogUtils::info(TAG, oss.str());

    oss.clear();
    string strInfo = "Helloworld";
    for (char ch : strInfo) {
        oss << ch << ' ';
    }

    strInfo[3] = 'W';

    for (int i = 0; i < strInfo.size(); i++) {
        oss << strInfo[i] << ' ';
    }

    LogUtils::info(TAG, oss.str());


    oss.clear();

    oss << boolalpha;
    oss << (strInfo=="Helloworld") << " " << (strInfo != "Helloworld") << endl;
    LogUtils::info(TAG, oss.str());


    char a[10] = "Hello";
    a[3] = 'W';
    oss.clear();
    oss << string(a) << endl;
    LogUtils::info(TAG, oss.str());


    char charValue = '2';
    oss.clear();
    oss << boolalpha;
    oss <<(isdigit(charValue) == 1);
    LogUtils::info(TAG, oss.str());
}



string HelloWorldDemo::printArray(const array<int, 10>& arr) {
    ostringstream oss;
    for (int i = 0; i < arr.size(); i++) {
        oss << arr.at(i);
        if (i < arr.size() - 1) {
            oss << ",";
        }
    }

    return oss.str();
}


void HelloWorldDemo::print2DArr(int (*arr)[5], int size) {
    ostringstream oss;
    ostringstream oss1;
    ostringstream oss2;
    for (int i = 0; i < size; i++) {
        int* subArr = arr[i];
        for (int j = 0; j < 5; j++) {
            oss << subArr[j] << ", ";
            oss1 << arr[i][j] << ", ";
        }

        oss2 << **(arr + i) << "\t";

        oss << "\t--\t";
        oss1 << "\t--\t";
    }

    LogUtils::info(TAG, oss.str());
    LogUtils::info(TAG, oss1.str());
    LogUtils::info(TAG, oss2.str());


}

void HelloWorldDemo::arrDemo() {
    int arr[][5] = {
            {1,2,3,4,5},
            {2,3,4,5,6},
            {3,4,5,6,7}
    };

    print2DArr(arr, 3);
}

double add(double a, double b) {
    ostringstream oss;
    oss << "add " << a << " + " << b << " = " << a + b;
    LogUtils::info(TAG, oss.str());
    return a + b;
}

void HelloWorldDemo::funcDemo() {

    // 函数指针
    double (*calFunc)(double, double) = add;
    ostringstream oss;
    oss << "FuncTools finish:" << funcTools(calFunc, 10, 10);
    LogUtils::info(TAG, oss.str());
}


double HelloWorldDemo::funcTools(double(*cal)(double, double), double x, double y) {
    LogUtils::info(TAG, "funcTools before");
    double res = cal(x, y);
    LogUtils::info(TAG, "funcTools after");
    return res;
}