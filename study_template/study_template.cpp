#include "study_template.h"

namespace Eglinux
{
    // 普通方式
    int add_sum(int a, int b)
    {
        cout << "int add_sum(int a, int b)" << endl;
        return a + b;
    }

    double add_sum(double a, double b)
    {
        cout << "double add_sum(double a, double b)" << endl;
        return a + b;
    }

    // 模板方式的实现必须放在头文件中，否则链接的时候会到不到定义
}