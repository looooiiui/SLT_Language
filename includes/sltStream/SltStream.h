/** SLT 定制流式处理 */
#pragma once
#ifndef SLT_STREAM_H
#define SLT_STREAM_H
#define SLT_STREAM_VERSION "1.0.0"
#endif
/** 乱七八糟的stream和系统类的导入 */
#include <sstream>
#include <string>

/** 这里是流式处理的主类 */
class SltStream{
private:
    /** 待功能写入 */
public:
    /** 无参递归底层，最后返回格式化完成的string */
    static void stringFormat(std::ostringstream& oss);
    /** 递归拆包层，反复拆包解析数据类型 */
    template<typename T, typename... Args>
    static void stringFormat(std::ostringstream& oss, T&& first, Args&&... args);
    /** 最后获得格式化的字符串 */
    template<typename... Args>
    static std::string MakeFormatString(Args&&... args);
};

inline void SltStream::stringFormat(std::ostringstream& oss){
    return;
}

template<typename T, typename... Args>
inline void SltStream::stringFormat(std::ostringstream& oss, T&& first, Args&&... args){
    oss << std::forward<T>(first);
    /** 切除first后继续递归 */
    SltStream::stringFormat(oss, std::forward<Args>(args)...);
}

template<typename... Args>
inline std::string SltStream::MakeFormatString(Args&&... args){
    /** 最终要返回的oss */
    std::ostringstream oss;
    /** 这里进行递归写入 */
    SltStream::stringFormat(oss, std::forward<Args>(args)...);
    return oss.str();
}



