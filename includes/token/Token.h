/** 这里是SLT语言的Token类 */
#ifndef SLT_TOKEN_H
#define SLT_TOKEN_H
/** 系统库文件 */

/** 这里是Slt库的文件 */
#include<lib/sltMap/SltMap.h>
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
    RCurlyBracket, Semicolon, Return,
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

#endif