/** SLT流式处理类的主要实现 */
#include <sltStream/SltStream.h>

/** 系统自带库 */
#include <sstream>

/** 将特殊字符例如 /n /r 转化为 // 可输出字符 */
std::string SltStream::MakeOutPutChar(unsigned char normalChar){
    /** 这里对已经识别的字符进行转化 */
    switch (normalChar){
        case '\n': return "\\n";
        case '\r': return "\\r";
        case '\t': return "\\t";
        case '\v': return "\\v";
        case '\f': return "\\f";
        default:
            /** 其余的转化为字符串类型返回 */
            if (std::isprint(normalChar)){
                return std::string(1, static_cast<char>(normalChar));
            }
            /** 不能输出的翻译成 0x + 10进制 */
            else{
                return "0x" + std::to_string(normalChar);
            }
    }
}