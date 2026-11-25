/** 这里是SLT语言的Lexer词法切分器 */
#ifndef SLT_LEXERL_H
#define SLT_LEXERL_H
#define SLT_LEXERL_VERSION "1.0.0"

/** 原生库(系统头) */
#include <string>
#include <vector>
/** 自定义类，调试系统 */
#include <debugSystem/debugSystem.h>
#include <SltStream/SltStream.h>
#include <lib/sltMap/SltMap.h>
#include <token/Token.h>
#include <optional>


/** 源码位置器, 第一个记录当前位置在总文本中的位置，第二个记录当前的行数 */
struct SourcePos {
    size_t m_sOffset;
    size_t m_sLine;
    size_t m_iRow;
};

/** SLT 核心词法解析器 */
class SltLexer{
private:
    /** 解析器运行时使用以及产出的参数,格式: [token...] */
    std::vector<token> m_vTokenVector;
    /** m_sSourcePos 第一个记录当前位置在总文本中的位置，第二个记录当前的行数 */
    SourcePos m_sSourcePos;
    std::string m_sSinglefileText;

    /** 其他系统 */
    SltDebugSystem* m_pDebugSystem;

    /** lexer peek, 拿取下一个字符*/
    std::optional<unsigned char> Peek() const;
    /** lexer 字符消费器，用于处理当前位置信息 */
    void Consume(bool nextLineFlag = false);
    /** lexer 空白解析器*/
    void SkipWhiteSpace();
    /** lexer 解析数字类型 */
    token ReadNumber();
    /** lexer解析字符串类型 */
    token ReadString();
    /** lexer解析标识符类型(其他乱七八糟的类型) */
    token ReadIdent();
    /** lexer解析运算符号类型 */
    token ReadOperator();
    /** 综合，结合所有切词类型，通用返回 */
    token NextToken();
    /** 语句切词器 */
    void ReadUntilSemi(const std::string& line);
    /** 总切词器(单脚本) */
    void LexerGenerateVector(const std::string& page);
public:
    SltLexer(SltDebugSystem* o_sDebugSystem);
    ~SltLexer();

    /** lexer功能 */
    std::vector<token> GetLexerTokenVector();
    void SetAnalysisText(const std::string& cinText);
};


#endif