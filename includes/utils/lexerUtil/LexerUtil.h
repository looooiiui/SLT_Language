/** lexer对应的工具 */
#ifndef LEXER_UTIL_H
#define LEXER_UTIL_H
/** 系统库文件 */
#include <string>
#include <vector>
/** 导入自己要联动的头文件 */
#include <lexer/lexer.h>

class LexerUtil{
private:
    
public:
    /** 转换 Token Vector 数组为可输出字符串 */
    static std::string TokenVectorString(std::vector<token>& vec);
};


#endif