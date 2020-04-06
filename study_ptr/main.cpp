#include <iostream>
#include "study_ptr.h"

using namespace std;
using namespace Eglinux;

int main(int argc, const char *argv[])
{
    //-------------------------------------------------------------------------------------------------------------
    // 一些智能指针的使用方法
    shared_ptr<int> p1(new int(100));

    // shared_ptr<int> p2 = new int(200);   // 不能使用 = 赋值，智能指针是 explicit 的，不能进行隐式类型转换

    // 建议使用 make_shared，它是安全、高效的，它是函数模板
    shared_ptr<int> p3 = make_shared<int>(300);
    auto p4 = make_shared<string>(5, 'a');

    //-------------------------------------------------------------------------------------------------------------
    // 当默认删除器不能正确处理的时候需要我们自己指定删除器
    shared_ptr<int> p5(new int[10], [](int *p) {
        delete[] p;
    });

    // 也可以使用 default_delete 这个标准库中的模板类作为删除器
    shared_ptr<study_ptr> pclass(new study_ptr[10], default_delete<study_ptr[]>());

    // 如果实在不想处理删除器，就像下面这样写，这样还可以直接使用下标
    shared_ptr<int[]> p6(new int[10]);
    p6[1] = 12;

    //-------------------------------------------------------------------------------------------------------------
    // 创建 weak_ptr
    weak_ptr<int> p1w(p1);
    weak_ptr<int> p2w, p3w;
    p2w = p3;
    p3w = p1w;

    p3w.use_count();    // 返回强引用计数的值
    p3w.expired();      // 是否过期，如果强引用计数为 0，对象被释放，则返回真，对象过期
    p3w.reset();        // 弱引用计数 -1，不影响强引用计数

    auto p7 = p1w.lock();       // 检查 p1w 指向的对象是否存在，存在就返回一个该对象指向的 shared_ptr，引用计数 +1

    //-------------------------------------------------------------------------------------------------------------
    shared_ptr<study_ptr> pclass1(new study_ptr);
    shared_ptr<study_ptr> pclass2 = pclass1->get_self();

    return 0;
}