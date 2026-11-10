/** 这里是SLT语言的Lexer词法切分器 */
#pragma once
#ifndef SLT_LEXERL_H
#define SLT_LEXERL_H
#define SLT_LEXERL_VERSION "1.0.0"
#endif

/** 原生库(系统头) */
#include <string>
#include <vector>
/** 自定义类，调试系统 */
#include <debugSystem/debugSystem.h>
#include <SltStream/SltStream.h>

/** 词元类型, 切词使用 ";" 作为语句结束*/
enum class TokenType {
    /** 普通数字
     * 标识符
     * 逻辑
     * 运算
     * 系统符
     */
    Number,
    Ident,
    If, Else, LParen, RParen, Semi,
    Plus, Minus, Star, Slash,
    Eof
};

/** 词元类型映射表 */
struct tokenMapping {
    
};

/** 词法解析器产出的词结构体 */
struct token{
    /** 词元类型
     * 词元内容
     * 词元行
     * 词元列
     */
    TokenType m_ttype;
    std::string m_stext;
    int m_iline;
    int m_irow;
};

/** 源码位置器, 第一个记录当前位置在总文本中的位置，第二个记录当前的行数 */
struct SourcePos {
    size_t m_sOffset;
    size_t m_sLine;
};

/** SLT 核心词法解析器 */
class SltLexer{
private:
    /** 解析器运行时使用以及产出的参数 */
    std::vector<token> m_vTokenVector;
    /** m_sSourcePos 第一个记录当前位置在总文本中的位置，第二个记录当前的行数 */
    SourcePos m_sSourcePos;
    std::string m_sSinglefileText;

    /** 其他系统 */
    SltDebugSystem* m_sDebugSystem;

    /** lexer内部小功能，切词等 */
    void SkipWhiteSpace();
    std::string ReadNumber();
    std::string ReadString();
    std::string NextToken();
    /** 语句切词器 */
    void ReadUntilSemi(const std::string& line);
    /** 总切词器(单脚本) */
    void LexerGenerateVector(const std::string& page);
public:
    SltLexer();
    ~SltLexer();

    /** lexer功能 */
    std::vector<token>* GetLexerTokenVector();
    void SetAnalysisText(const std::string& cinText);
};