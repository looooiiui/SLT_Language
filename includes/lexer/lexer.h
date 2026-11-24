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
#include <optional>


/** 词元类型, 切词使用 ";" 作为语句结束, 这里都是特定符号 */
enum class TokenType {
    /** 普通数字, 字符串
     * 标识符
     * 逻辑
     * 运算
     * 系统符
     */
    Number, String, KindNumber, KindString, Class,
    Ident,
    /** 符号表逻辑 */
    If, Else, LParen, RParen, LBracket, RBracket, LCurlyBracket, 
    RCurlyBracket, Semicolon,
    /* 符号表运算 */
    Plus, Minus, Star, Slash, Inc, Dec, Assign,
    Lt, Gt, Percent, Bang, Amp, Pipe, Caret,
    Tilde, Comma, Dot, Colon, Question, Equal, NEqual,
    LE, GE, Amp_Amp, Pipe_Pipe, Plue_Eq, Minus_Eq, Star_Eq,
    Slash_Eq, Percent_Eq, Amp_Eq, Pipe_Eq, Caret_Eq, LShift, RShift,
    /** 其他系统符号 */
    Eof,
};

/** 标记类Token,不吃,比如空格 */
enum class WhiteSpaceKind {
    Space, LineFeed, CarriageReturn, VTab, FFeed, HTab
};
/** 标记类Token,这里要吃掉的,比如引号 */
enum class NormalConstKind {
    StraightQuote, CurlyQuote, Dot, UnderScore
};

/** 下面把映射表写成函数主要是因为SltMap存有调试系统，会在静态期间调用线程锁，这时候锁没加载好 */
/** 词元类型映射表(字符串->词元类型) */
SltMap<unsigned char, WhiteSpaceKind>& GetWhiteSpaceMappingCT();
/** 词元类型映射表(词元类型->字符串)，给空白字符用的 */
SltMap<WhiteSpaceKind, unsigned char>& GetWhiteSpaceMappingTC();
/** 词元类型映射表(可以吃掉的字符串->词元类型) */
SltMap<unsigned char, NormalConstKind>& GetNormalConstCT();
/** 词元类型映射表(可以吃掉的词元类型->字符串)*/
SltMap<NormalConstKind, unsigned char>& GetNormalConstTC();
/** 词元类型映射表(系统识别字符->词元类型) */
SltMap<std::string, TokenType>& GetTokenTypeCT();
/** 词元类型映射表(词元类型->系统识别字符) */
SltMap<TokenType, std::string>& GetTokenTypeTC();

/** 词法解析器产出的词结构体 */
struct token{
    /** 词元类型
     * 词元内容
     * 词元行
     * 词元列
     */
    TokenType m_ttype;
    std::string m_stext;
    size_t m_iline;
    size_t m_iRow;

    /** 结构体默认构造 */
    token() = default;
    /** 结构体初始化 */
    token(TokenType o_tType);
    token(
        TokenType o_tType, 
        std::string o_sText, 
        size_t o_iLine, 
        size_t o_iRow
    );
};

/** 源码位置器, 第一个记录当前位置在总文本中的位置，第二个记录当前的行数 */
struct SourcePos {
    size_t m_sOffset;
    size_t m_sLine;
    size_t m_iRow;
};

/** SLT 核心词法解析器 */
class SltLexer{
private:
    /** 解析器运行时使用以及产出的参数,格式: [[一句内的词元]...] */
    std::vector<token> m_vTokenVector;
    /** m_sSourcePos 第一个记录当前位置在总文本中的位置，第二个记录当前的行数 */
    SourcePos m_sSourcePos;
    std::string m_sSinglefileText;

    /** 其他系统 */
    SltDebugSystem* m_sDebugSystem;

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
    std::vector<token>* GetLexerTokenVector();
    void SetAnalysisText(const std::string& cinText);
};


#endif