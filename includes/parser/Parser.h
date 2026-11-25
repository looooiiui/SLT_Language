/** 这里是Slt解析器，语法分析器 */
#ifndef SLT_PARSER_H
#define SLT_PARSER_H
#define SLT_PARSER_VERSION

/** 系统库导入 */
#include<string>
#include<stddef.h>

/** Slt语言解释器的当前解析位置，用于控制Token消费
 * 这是给Slt内部用的位置，其余的代码位置已经在Token中标注
 */
struct ParserPos{
    size_t m_sOffset;
};



/** Slt语言的解释器，语法分析器 */
class SltParser{
private:
    ParserPos m_pParserPos;
public:


};

#endif