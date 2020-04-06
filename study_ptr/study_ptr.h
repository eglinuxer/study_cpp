#ifndef __STUDY_PTR_H__
#define __STUDY_PTR_H__
#include <memory>
/**************************************************************************
 * 
 * 
 * 
 * 
 * ************************************************************************/

namespace Eglinux
{
    class study_ptr : public std::enable_shared_from_this<study_ptr>
    {
    public:
        study_ptr(/* args */);
        virtual ~study_ptr();

    public:
        std::shared_ptr<study_ptr> get_self()
        {
            // 要返回自身的智能指针，必须通过这种方法才是安全的，如果直接返回 this 指针，则相当于返回的裸指针，是不安全的。
            // enable_shared_from_this 的 shared_from_this 方法内部实际上是调用了 weak_ptr 的 lock() 方法，返回一个 shared_ptr，并且引用计数 +1
            return shared_from_this();
        }

    private:
        /* data */
    };
}

#endif