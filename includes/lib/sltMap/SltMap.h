#pragma once
#ifndef SLT_MAP_H
#define SLT_MAP_H
#define SLT_MAP_VERSION "1.0.0"
#endif

/** 系统头文件,C++库 */
#include <unordered_set>
#include <string>
#include <vector>
#include <optional>
#include <initializer_list>
/** 自定义库 */
#include <debugSystem/DebugSystem.h>
#include <sltStream/SltStream.h>


/** SltMap映射的主要结构体 */
template <typename TypeKey, typename TypeValue>
struct SltMapElement{
    TypeKey m_tKey;
    TypeValue m_tValue;
};

/** SltMap主要实现类 */
template <typename TypeKey, typename TypeValue>
class SltMap{
private:
    /** 内部使用映射表 */
    std::vector<SltMapElement<TypeKey, TypeValue>> m_vElements;
    /** 调试系统 */
    SltDebugSystem* m_sDebugSystem;
    /** 辅助记录，用于检查key是否被使用 */
    std::unordered_set<TypeKey> m_sKeySet;
public:

    /** 映射类默认初始化, 使用 {} */
    SltMap() = default;
    SltMap(std::initializer_list<SltMapElement<TypeKey, TypeValue> > list){
        /** 空值化集合 */
        m_sKeySet = {};
        /** 获得调试系统 */
        m_sDebugSystem = SltDebugSystem::GetInstance();
        for (auto &&elem: list){
            /** 检查Key是否已经存在 */
            if (CheckExist(elem.m_tKey)){
                continue;
            }
            /** 不存在则添加键值对 */
            m_vElements.push_back(elem);
            m_sKeySet.insert(elem.m_tKey);
        }
    }

    /** 检查映射表中是否存在相同Key */
    bool CheckExist(const TypeKey &key) const{
        /** 检查key */
        if (m_sKeySet.count(key)){
            /** 调试系统提示 */
            m_sDebugSystem->DebugSystemConsoleWrite(
                "lib/SltMap/SltMap",
                SltStream::MakeFormatString(
                    "建立SltMap包含重复值,Key: ",
                    key
                ),
                DebugSystemKind::DEBUG_SYSTEM_DEBUG
                /** 存在返回true */
            );  
            return true;
        }
        /** 不存在返回false */
        return false;
    }

    /** 正向查找(遍历),检查key */
    std::optional<TypeValue> Find(const TypeKey &key) const{
         for (auto const &elem : m_vElements){
            if (elem.m_tKey == key){
                return elem.m_tValue;
            }
         }
         /** 无值状态返回 */
         m_sDebugSystem->DebugSystemConsoleWrite(
            "lib/SltMap/Find",
            SltStream::MakeFormatString(
                "SltMap不能找到key为",
                key,
                "的value"
            ),
            DebugSystemKind::DEBUG_SYSTEM_ERROR
         );
         return std::nullopt;
    } 

    /** 添加映射 */
    bool Insert(const TypeKey &key, const TypeValue &value){
        /** 先导检查映射表是否占用 */
        if (CheckExist(key)){
            /** 插入失败 */
            m_sDebugSystem->DebugSystemConsoleWrite(
                "lib/SltMap/Insert",
                SltStream::MakeFormatString("插入失败"),
                DebugSystemKind::DEBUG_SYSTEM_DEBUG                
            );
            /** 失败返回 */
            return false;
        }
        /** 不存在则添加键值对 */
        auto elem = SltMapElement<TypeKey, TypeValue>{std::move(key), std::move(value)};
        m_vElements.push_back(elem);
        m_sKeySet.insert(elem.m_tKey);
        return true;
    }

    /** 映射表默认析构 */
    ~SltMap() = default;
};