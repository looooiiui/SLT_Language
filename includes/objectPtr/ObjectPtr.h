/** 这里是SLT的内存分配器 */
#pragma once
#ifndef OBJECT_PTR_H
#define OBJECT_PTR_H
#define OBJECT_PTR_VERSION "1.0.0"
#endif

/** 自定义类，调试系统 */
#include <debugSystem/debugSystem.h>

/** SLT语言指针计数器 */
struct ObjectRef{
    int refcnt;
};

/** 
 * 这是SLT语言的指针自动管理系统
 * 这里是使用语言解释器切分语言创建内存的时候的内存分配器
 * 不是SLT语言系统的内存分配器
 * */
template <typename object>
class ObjectPtr{
private:
    /** 内部管理的指针 */
    object* m_pHoldPtr;
    ObjectRef m_oObjectCount;
    /** 调试系统，需要外部注入 */
public:
    /** 自动指针初始化 */
    ObjectPtr();
    explicit ObjectPtr(object* ptr);
    ~ObjectPtr();

    /** 重载指针操作 */
    const ObjectPtr* operator->() const;
    const ObjectPtr& operator*() const;

    /** 指针常规操作 */
    void MovePtrTo(const ObjectPtr& other);     // 移动指针到其他指针管理器
};