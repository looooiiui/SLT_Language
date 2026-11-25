/** 这里是Slt的Token类的具体实现 */
#include <token/Token.h>
/** 这里是系统库文件 */

/** 这里是Slt库的文件 */
#include<lib/sltMap/SltMap.h>

/** 结构体初始化 */
token::token(TokenType o_tType){
    m_ttype = o_tType;
}
/** token带参数初始化 */
token::token(
    TokenType o_tType,
    std::string o_sText,
    size_t o_iLine, 
    size_t o_iRow
){
    m_ttype = o_tType;
    m_stext = o_sText;
    m_iline = o_iLine;
    m_iRow = o_iRow;
}

/** 词元类型映射表(字符串->词元类型) */
SltMap<unsigned char, WhiteSpaceKind>& GetWhiteSpaceMappingCT(){
    static SltMap<unsigned char, WhiteSpaceKind> inst{
        {' ',    WhiteSpaceKind::Space},
        {'\n',   WhiteSpaceKind::LineFeed},
        {'\r',   WhiteSpaceKind::CarriageReturn},
        {'\v',   WhiteSpaceKind::VTab},
        {'\f',   WhiteSpaceKind::FFeed},
        {'\t',   WhiteSpaceKind::HTab},        
    };
    return inst;
}

/** 词元类型映射表(词元类型->字符串)，给空白字符用的 */
SltMap<WhiteSpaceKind, unsigned char>& GetWhiteSpaceMappingTC(){
    static SltMap<WhiteSpaceKind, unsigned char> inst {
        {WhiteSpaceKind::Space,             ' '},
        {WhiteSpaceKind::LineFeed,          '\n'},
        {WhiteSpaceKind::CarriageReturn,    '\r'},
        {WhiteSpaceKind::VTab,              '\v'},
        {WhiteSpaceKind::FFeed,             '\f'},
        {WhiteSpaceKind::HTab,              '\t'},
    };
    return inst;
}

/** 词元类型映射表(可以吃掉的字符串->词元类型,当然拿去判断也行) */
SltMap<unsigned char, NormalConstKind>& GetNormalConstCT(){
    static SltMap<unsigned char, NormalConstKind> inst{       
        {'\'',      NormalConstKind::StraightQuote},
        {'"',       NormalConstKind::CurlyQuote},
        {'.',       NormalConstKind::Dot},
        {'_',       NormalConstKind::UnderScore}
    };
    return inst;
}
/** 词元类型映射表(可以吃掉的词元类型->字符串,当然拿去判断也行)*/
SltMap<NormalConstKind, unsigned char>& GetNormalConstTC(){
    static SltMap<NormalConstKind, unsigned char> inst{       
        {NormalConstKind::StraightQuote,        '\''},
        {NormalConstKind::CurlyQuote,           '"'},
        {NormalConstKind::Dot,                  '.'},
        {NormalConstKind::UnderScore,           '_'}
    };
    return inst;
}
/** 词元类型映射表(系统识别字符->词元类型) */
SltMap<std::string, TokenType>& GetTokenTypeCT(){
    static SltMap<std::string, TokenType> inst{
        {"num",             TokenType::KindNumber},
        {"str",             TokenType::KindString},
        {"if",              TokenType::If},
        {"else",            TokenType::Else},
        {"class",           TokenType::Class},
        {"return",          TokenType::Return}, 
        {"+",               TokenType::Plus},
        {"-",               TokenType::Minus},
        {"*",               TokenType::Star},
        {"/",               TokenType::Slash},
        {"(",               TokenType::LParen},
        {")",               TokenType::RParen},
        {"[",               TokenType::LBracket},
        {"]",               TokenType::RBracket},
        {"{",               TokenType::LCurlyBracket},
        {"}",               TokenType::RCurlyBracket},
        {"=",               TokenType::Assign},
        {"<",               TokenType::Lt},
        {">",               TokenType::Gt},
        {"%",               TokenType::Percent},
        {"!",               TokenType::Bang},
        {"&",               TokenType::Amp},
        {"|",               TokenType::Pipe},
        {"^",               TokenType::Caret},
        {"~",               TokenType::Tilde},
        {",",               TokenType::Comma},
        {";",               TokenType::Semicolon},
        {".",               TokenType::Dot},
        {"?",               TokenType::Question},
        {":",               TokenType::Colon},
        {"++",              TokenType::Inc},
        {"--",              TokenType::Dec},
        {"==",              TokenType::Equal},
        {"!=",              TokenType::NEqual},
        {"<=",              TokenType::LE},
        {">=",              TokenType::GE},
        {"&&",              TokenType::Amp_Amp},
        {"||",              TokenType::Pipe_Pipe},
        {"+=",              TokenType::Plue_Eq},
        {"-=",              TokenType::Minus_Eq},
        {"*=",              TokenType::Star_Eq},
        {"/=",              TokenType::Slash_Eq},
        {"%=",              TokenType::Percent_Eq},
        {"&=",              TokenType::Amp_Eq},
        {"|=",              TokenType::Pipe_Eq},
        {"^=",              TokenType::Caret_Eq},
        {"<<",              TokenType::LShift},
        {">>",              TokenType::RShift}
    };
    return inst;
}
/** 词元类型映射表(词元类型->系统识别字符) */
SltMap<TokenType, std::string>& GetTokenTypeTC(){
    static SltMap<TokenType, std::string> inst{
        {TokenType::KindNumber,       "num"},
        {TokenType::KindString,       "str"},
        {TokenType::If,               "if"},
        {TokenType::Else,             "else"},
        {TokenType::Class,            "class"},
        {TokenType::Return,           "return"},  
        {TokenType::Plus,             "+"},
        {TokenType::Minus,            "-"},
        {TokenType::Star,             "*"},
        {TokenType::Slash,            "/"},
        {TokenType::LParen,           "("},
        {TokenType::RParen,           ")"},
        {TokenType::LBracket,         "["},
        {TokenType::RBracket,         "]"},
        {TokenType::LCurlyBracket,    "{"},
        {TokenType::RCurlyBracket,    "}"},
        {TokenType::Assign,           "="},
        {TokenType::Lt,               "<"},
        {TokenType::Gt,               ">"},
        {TokenType::Percent,          "%"},
        {TokenType::Bang,             "!"},
        {TokenType::Amp,              "&"},
        {TokenType::Pipe,             "|"},
        {TokenType::Caret,            "^"},
        {TokenType::Tilde,            "~"},
        {TokenType::Comma,            ","},
        {TokenType::Semicolon,        ";"},
        {TokenType::Dot,              "."},
        {TokenType::Question,         "?"},
        {TokenType::Colon,            ":"},
        {TokenType::Inc,              "++"},
        {TokenType::Dec,              "--"},
        {TokenType::Equal,            "=="},
        {TokenType::NEqual,           "!="},
        {TokenType::LE,               "<="},
        {TokenType::GE,               ">="},
        {TokenType::Amp_Amp,          "&&"},
        {TokenType::Pipe_Pipe,        "||"},
        {TokenType::Plue_Eq,          "+="},
        {TokenType::Minus_Eq,         "-="},
        {TokenType::Star_Eq,          "*="},
        {TokenType::Slash_Eq,         "/="},
        {TokenType::Percent_Eq,       "%="},
        {TokenType::Amp_Eq,           "&="},
        {TokenType::Pipe_Eq,          "|="},
        {TokenType::Caret_Eq,         "^="},
        {TokenType::LShift,           "<<"},
        {TokenType::RShift,           ">>"},
    };
    return inst;
}