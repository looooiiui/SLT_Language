#ifndef SLT_MAP_H
#define SLT_MAP_H
#define SLT_MAP_VERSION "1.0.0"

/** 系统头文件,C++库 */
#include <unordered_set>
#include <string>
#include <vector>
#include <optional>
#include <initializer_list>
/** 自定义库 */
#include <debugSystem/DebugSystem.h>
#include <sltStream/SltStream.h>

/** index找不到对应的索引 */
inline size_t SLTMAP_NOT_FOUND = static_cast<size_t>(-1);

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
    SltDebugSystem* m_pDebugSystem;
    /** 辅助记录，用于检查key是否被使用 */
    std::unordered_set<TypeKey> m_sKeySet;
    /** 按照索引删除映射 */
    void EraseIndex(size_t index);
public:
    /** 映射类默认初始化, 使用 {} */
    SltMap() = default;
    /** 映射类初始化: 构造系统需要传入 */
    SltMap(
        std::initializer_list<SltMapElement<TypeKey, TypeValue> > list,
        SltDebugSystem* o_sDebugSystem = SltDebugSystem::GetInstance()
    );
    /** 检查映射表中是否存在相同Key */
    bool CheckExist(const TypeKey &key) const;
    /** 正向查找(遍历),检查key */
    std::optional<TypeValue> Find(const TypeKey &key) const;
    /** 正向查找, 返回索引位置 */
    size_t FindKeyIndex(const TypeKey &key) const;
    /** 添加映射 */
    bool Insert(const TypeKey &key, const TypeValue &value);
    /** 删除映射 */
    bool Delete(const TypeKey &key);
    /** 映射表默认析构 */
    ~SltMap() = default;
};

template <typename TypeKey, typename TypeValue>
inline SltMap<TypeKey, TypeValue>::SltMap(
    std::initializer_list<SltMapElement<TypeKey, TypeValue> > list,
    SltDebugSystem* o_sDebugSystem
){
    /** 空值化集合 */
    m_sKeySet = {};
    /** 获得调试系统 */
    m_pDebugSystem = o_sDebugSystem;
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

template <typename TypeKey, typename TypeValue>
inline bool SltMap<TypeKey, TypeValue>::CheckExist(const TypeKey &key) const{
    /** 检查key */
    if (m_sKeySet.count(key)){
        /** 调试系统提示 */
        // m_pDebugSystem->DebugSystemConsoleWrite(
        //     "lib/SltMap/CheckExist",
        //     SltStream::MakeFormatString(
        //         "SltMap包含键值,Key: ",
        //         key
        //     ),
        //     DebugSystemKind::DEBUG_SYSTEM_DEBUG
        //     /** 存在返回true */
        // );  
        return true;
    }
    /** 不存在返回false */
    return false;
}

template <typename TypeKey, typename TypeValue>
inline std::optional<TypeValue> SltMap<TypeKey, TypeValue>::Find(const TypeKey &key) const{
    for (auto const &elem : m_vElements){
        if (elem.m_tKey == key){
            return elem.m_tValue;
        }
    }
    /** 无值状态返回 */
    // m_pDebugSystem->DebugSystemConsoleWrite(
    //     "lib/SltMap/Find",
    //     SltStream::MakeFormatString(
    //         "SltMap不能找到key为",
    //         key,
    //         "的value"
    //     ),
    //     DebugSystemKind::DEBUG_SYSTEM_ERROR
    // );
    return std::nullopt;
} 

template <typename TypeKey, typename TypeValue>
inline size_t SltMap<TypeKey, TypeValue>::FindKeyIndex(const TypeKey &key) const {
    /** 记录索引位置 */
    size_t index = 0;
    /** 遍历寻找对应Key的索引 */
    for (auto const &elem : m_vElements){
        /** -1 索引开头,计算放开头 */
        if (elem.m_tKey == key){
            // m_pDebugSystem->DebugSystemConsoleWrite(
            //     "lib/SltMap/FindKeyIndex",
            //     SltStream::MakeFormatString(
            //         "SltMap键值,Key: ",
            //         key,
            //         " 位于索引: ",
            //         index
            //     ),
            //     DebugSystemKind::DEBUG_SYSTEM_DEBUG
            //     /** 存在返回index */
            // ); 
            return index;
        }
        index++;
    }
    /** 找不到索引,输出日志 */
    // m_pDebugSystem->DebugSystemConsoleWrite(
    //     "lib/SltMap/FindKeyIndex",
    //     SltStream::MakeFormatString(
    //         "SltMap键值,Key: ",
    //         key,
    //         " 找不到对应索引"
    //     ),
    //     DebugSystemKind::DEBUG_SYSTEM_DEBUG
    //     /** 存在返回index */
    // ); 
    /** 找不到索引返回 static_cast<size>(-1) */
    return static_cast<size_t>(-1);
}

template <typename TypeKey, typename TypeValue>
inline bool SltMap<TypeKey, TypeValue>::Insert(const TypeKey &key, const TypeValue &value){
    /** 先导检查映射表是否占用 */
    if (CheckExist(key)){
        /** 插入失败 */
        // m_pDebugSystem->DebugSystemConsoleWrite(
        //     "lib/SltMap/Insert",
        //     SltStream::MakeFormatString("插入失败"),
        //     DebugSystemKind::DEBUG_SYSTEM_ERROR                
        // );
        /** 失败返回 */
        return false;
    }
    /** 不存在则添加键值对 */
    auto elem = SltMapElement<TypeKey, TypeValue>{std::move(key), std::move(value)};
    m_vElements.push_back(elem);
    m_sKeySet.insert(elem.m_tKey);
    return true;
}

template <typename TypeKey, typename TypeValue>
inline void SltMap<TypeKey, TypeValue>::EraseIndex(size_t index){
    /** 先检查下标是否被找到与是否越界 */
    if (index == SLTMAP_NOT_FOUND || index >= m_vElements.size()){
        return;
    }
    /** 需要同步删除辅助集合中的 Key */
    auto& elem = m_vElements[index];
    m_sKeySet.erase(elem.m_tKey);
    /** 再删除数组中存储的键值对 */
    m_vElements.erase(m_vElements.begin() + index);
}

template <typename TypeKey, typename TypeValue>
inline bool SltMap<TypeKey, TypeValue>::Delete(const TypeKey &key){
    size_t keyIndex = FindKeyIndex(key);
    /** 先导检查映射表是否存在 */
    if (keyIndex == SLTMAP_NOT_FOUND){
        /** 不存在映射 */
        // m_pDebugSystem->DebugSystemConsoleWrite(
        //     "lib/SltMap/Insert",
        //     SltStream::MakeFormatString(
        //         "当前映射表不存在key: ",
        //         key
        //     ),
        //     DebugSystemKind::DEBUG_SYSTEM_ERROR          
        // );
        /** 失败返回 */
        return false;
    }
    /** 存在则删除键值对 */
    EraseIndex(keyIndex);
    return true;
}

#endif