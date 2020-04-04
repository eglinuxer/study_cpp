#include <iostream>

#include "study_template.h"
#include "macro.h"

using namespace std;
using namespace Eglinux;

int func(int a, int b)
{
    return 1;
}

int main(int argc, const char *argv[])
{
    cout << "hello world!" << endl;

    // 一、函数模板

    // 普通函数调用和函数模板调用的对比
    cout << add_sum(1, 2) << endl;
    cout << add_sum(1.1, 2.2) << endl;

    cout << add_sum(2.2222f, 5.5555f) << endl;

    // 显示的指定模板参数，在 <> 中提供额外的信息,模板函数体里面可以直接使用
    cout << add_sum<10, 11>() << endl;
    print_arg<const char*, 10, 11>("hello");
    print_arg<double, 10, 11>(10.9999);

    cout << compare_str("test1", "test2") << endl;
    cout << compare_str("test3", "test3") << endl;


    // 2、类模板
    study_template<int> my_temp;
    my_temp.mem_func(10);
    study_template1<int, 100> my_temp1;

    my_temp1.test_func();
    study_template1<int> my_temp12;
    my_temp12.test_func();

    // 3、函数指针在函数模板中的使用
    
    test_p_func(1, 3, func);

    // ４、可调用对象在函数模板中的使用
    tc obj;
    test_p_func(1, 3, obj);

    test_p_func1(1, 3);

    // 5、类型别名和类模板结合
    temp_name<int>::my_type map1;
    map1.insert({"a", 0});
    auto it = map1.find("a");
    cout << it->first << ":" << it->second << endl;

    str_map<int> map2;
    map2.insert({"b", 1});
    it = map2.find("b");
    cout << it->first << ":" << it->second << endl;

    // 6、可变参函数模板
    test_arg_func();
    test_arg_func(1);
    test_arg_func(1, 2);
    test_arg_func(1, "2", 3);

    // 在这里没有编译通过，但是单独测试是编译过了的，还没找到是什么原因
    //test_arg_func1(1, "a", 3, 4);

    // 7、可变参类模板
    test_arg_class<int, double, float> test(100, 30, 20.1f);

    // 8、模板模板参数
    tt_arg_class<int, my_vector> tt_v;
    tt_arg_class<int, my_list> tt_l;


    return 0;
}