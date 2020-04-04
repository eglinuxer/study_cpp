#ifndef __STUDY_TEMPLATE__
#define __STUDY_TEMPLATE__
#include <cstring>
#include <string>
#include <map>
#include <vector>
#include <list>

#include <iostream>

using namespace std;

namespace Eglinux
{

    /*****************************************************************************************************
     * 
     * 为什么需要模板？
     *      1、实现泛型编程，重复的写法，只是参数类型不同的时候，使用模板代替是很好的选择
     * 
     * ***************************************************************************************************/

    //---------------------------------------------普通函数----------------------------------------------------------
    // 普通方法实现两数相加的例子, 可以看出仅仅是参数的类型不同而已，但是我们需要写两遍
    int add_sum(int a, int b);
    double add_sum(double a, double b);

    //---------------------------------------------函数模板----------------------------------------------------------
    // 函数模板的方式
    // 注意：把实现写在这里，多个文件包含头文件，不会造成重定义。
    // 如果实现写在 cpp 中，那编译的时候没有生成对应版本的实体，所以链接的时候就找不到实体的定义。
    // 模板定义并不会生成具体实例代码，只有在调用模板函数的时候，才会根据调用参数的类型生成具体实例代码。
    template<typename T>
    T add_sum(T a, T b)
    {
        cout << "T add_sum(T a, T b)" << endl;
        return a + b;
    }

    // 非类型模板参数
    template<int a, int b>
    int add_sum()
    {
        cout << "template<int a, int b>\nint add_sum(int a, int b)" << endl;
        return a + b;
    }

    template<typename T, int a, int b>
    void print_arg(T c)
    {
        cout << "template<typename T, int a, int b>\nvoid print_arg(T c)" << endl;
        cout << "a = " << a << " b =" << b << " c = " << c << endl;
    }

    template<unsigned l1, unsigned l2>
    int compare_str(const char (&p1)[l1], const char (&p2)[l2])
    {
        cout << "template<unsigned l1, unsigned l2>\nint compare_str(cosnt char (&p1)[l1], const char (&p2)[l2])" << endl;
        return strcmp(p1, p2);
    }

    //---------------------------------------------类模板----------------------------------------------------------
    // 类型模板参数
    template<typename T>
    class study_template
    {
    public:
        typedef T *iterater;    // 迭代器

    public:
        study_template()
        {

        }

        virtual ~study_template()
        {

        }

        study_template &operator=(const study_template &);   // 赋值运算符重载
        iterater begin();
        iterater end();

    public:
        void test_func();
    
    public:
        template<typename U>
        void mem_func(U u);

    private:
        /* data */
    };

    // 必须放在头文件中，否则链接的时候找不到定义
    template<typename T>
    void study_template<T>::test_func()
    {

    }

    template<typename T>
    study_template<T> &study_template<T>::operator=(const study_template &)
    {
        return *this;
    }

    // 这里的 typename 可以使用 class 代替，但意思不是类的意思
    template<typename T>
    // 这里 typename 的作用是告诉编译器 iterater 是一个 study_template<T>::iterater 类型成员
    typename study_template<T>::iterater study_template<T>::begin()
    {

    }

    template<typename T>
    typename study_template<T>::iterater study_template<T>::end()
    {

    }

    template<typename T>
    template<typename U>
    void study_template<T>::mem_func(U u)
    {
        cout << u << endl;
    }

    // 非类型模板参数
    /***********************************************
     * 限制：
     *      1、浮点型不能做非类型参数
     *      2、类类型也不能做非类型参数
     * 
     * ********************************************/
    template<typename T, int size = 10>
    class study_template1
    {
    public:
        study_template1()
        {

        }

        virtual ~study_template1()
        {

        }

    public:
        void test_func();

    private:
        T array[size];
    };

    // 必须放在头文件中，否则链接的时候找不到定义
    template<typename T, int size>
    void study_template1<T, size>::test_func()
    {
        cout << size << endl;
    }

    //---------------------------------------------可调用对象----------------------------------------------------------
    class tc
    {
    public:
        tc()
        {
            cout << "可调用对象构造函数 tc()" << endl;
        }

        tc(const tc &t)
        {
            cout << "可调用对象拷贝构造函数 tc()" << endl;
        }

        // 重载 () 云算法
        int operator()(int v1, int v2) const
        {
            return v1 + v2;
        }
    };

    //---------------------------------------------函数模板中使用函数指针----------------------------------------------------------
    template<typename T, typename F>
    void test_p_func(const T &a, const T &b, F p_func)
    {
        cout << p_func(a, b) << endl;
    }

    // 默认参数给对象名
    template<typename T, typename F = tc>
    void test_p_func1(const T &a, const T &b, F p_func = F())
    {
        cout << p_func(a, b) << endl;
    }

    //---------------------------------------------定义类型别名----------------------------------------------------------
    // 类模板和类型别名的结合
    template<typename tm>
    struct temp_name
    {
        typedef map<string, tm> my_type;
    };

    // 使用　using
    template<typename T>
    using str_map = map<string, T>;

    //---------------------------------------------可变参函数模板----------------------------------------------------------
    template<typename... T>
    void test_arg_func(T... args)
    {
        cout << sizeof...(args) << endl;
        cout << sizeof...(T) << endl;
    }

    // 建议使用这种方式使用可变参数模板，方便参数展开
    // 递归终止函数
    // void test_arg_func1()
    // {
    //     cout << "递归结束" << endl;
    // }

    // template<typename T, typename... U>
    // void test_arg_func1(const T& a, const U&... b)
    // {
    //     cout << "收到的参数为：" << a << endl;
    //     test_arg_func1(b...);   // 递归调用，就能每次取出第一个参数 a
    // }

    //---------------------------------------------可变参类模板----------------------------------------------------------
    template<typename... args>
    class test_arg_class {};

    template<>
    class test_arg_class<>
    {
    public:
        test_arg_class()
        {
            cout << "0 个参数的类模板" << endl;
        }
    };
    
    // 继承方式展开
    template<typename T, typename... U>
    class test_arg_class<T, U...> : private test_arg_class<U...>    // 偏特化
    {
    public:
        test_arg_class() : m_t(0)
        {
            cout << "n 个参数的类模板" << endl;
        }

        test_arg_class(T t, U... u) : m_t(t), test_arg_class<U...>(u...)
        {
            cout << m_t << endl;
        }

    public:
        T m_t;
    };

    // 组合方式展开
    // template<typename T, typename... U>
    // class test_arg_class<T, U...>
    // {
    // public:
    //     test_arg_class() : m_t(0)
    //     {
    //         cout << "n 个参数的类模板" << endl;
    //     }

    //     test_arg_class(T t, U... u) : m_t(t), m_o(u...)
    //     {
    //         cout << m_t << endl;
    //     }

    // public:
    //     T m_t;
    //     test_arg_class<U...> m_o
    // };


    //---------------------------------------------模板模板参数---------------------------------------------------------
    template<typename T>
    using my_vector = vector<T, allocator<T>>;

    template<typename T>
    using my_list = list<T, allocator<T>>;

    template<
                typename T,                         // 类型模板参数
                template<class> class container     // 模板模板参数
                //template<typename W> typename Container   // W 其实毫无用处
            >
    class tt_arg_class
    {
    public:
        tt_arg_class(): m_t(10)
        {
            for(int i = 0; i < 10; ++i) {
                m_con.push_back(i);
            }
        }

    public:
        T m_t;
        container<T> m_con;
    };

}

#endif