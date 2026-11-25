/** 这里是给lexer特化的工具类 */
#include <utils/lexerUtil/LexerUtil.h>
#include <lexer/lexer.h>
#include <sltStream/SltStream.h>

std::string LexerUtil::TokenVectorString(std::vector<token>& vec){
    /** 检查数组长度先 */
    size_t vecSize = vec.size();
    /** 要输出的结果字符串缓冲 */
    std::string buf = "";
    /** 循环按照格式填入字符串缓冲区 */
    for (size_t index = 0; index < vecSize; index++){
        /** 格式化当前Token */
        std::string currentToken = SltStream::MakeFormatString(
            " [", index, "]:",
            vec.at(index).m_stext,
            "\n"
        );
        /** 填入缓冲 */
        buf += currentToken;
    }
    /** 返回格式化后的数据 */
    return buf;
}