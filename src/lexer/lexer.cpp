/** lexer的核心功能实现 */
/** 自带库文件 */
#include <string>
/** 自定义系统头文件 */
#include <lexer/lexer.h>
#include <debugSystem/DebugSystem.h>
#include <SltStream/SltStream.h>

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
    o_iRow = o_iRow;
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

/** lexer 用于获得当前位置的字符 */
std::optional<unsigned char> SltLexer::Peek() const{
    /** 先导检查当前偏移是否合法 */
    if (m_sSourcePos.m_sOffset >= m_sSinglefileText.size()){
        return std::nullopt;
    }
    /** 合法则获取当前的字符 */
    return static_cast<unsigned char>(m_sSinglefileText[m_sSourcePos.m_sOffset]);
}

/** 消费当前的字符，处理换行等 */
void SltLexer::Consume(bool nextLineFlag){
    /** 获取当前字符，先检查是否有字符可获取 */
    auto chOpt = Peek();
    if (chOpt == std::nullopt){
        return;
    }
    /** 检查是否启用换行 */
    if (nextLineFlag){
        /** 获取当前字符 */
        unsigned char currentChar = chOpt.value();
        /** 获取换行(CR + LF) */
        auto lineFeed = GetWhiteSpaceMappingTC().Find(WhiteSpaceKind::LineFeed).value();
        auto carriageReturn = GetWhiteSpaceMappingTC().Find(WhiteSpaceKind::CarriageReturn).value();
        /** LF + CR 全部都加，重复行添加交给 SkipWhiteSpace 处理 */
        if (currentChar == lineFeed){
            m_sSourcePos.m_sLine += 1;
        }
        else if (currentChar == carriageReturn){
            m_sSourcePos.m_sLine += 1;
        }
    }
    /** 最后默认偏移以及列号要加1 */
    m_sSourcePos.m_sOffset += 1;
    m_sSourcePos.m_iRow += 1;

}

/** Lexer 空白字符先导处理 */
void SltLexer::SkipWhiteSpace() {
    /** 跳过数量 */
    int m_iskipNum = 0;
    m_sDebugSystem->DebugSystemConsoleWrite(
        "lexer:空白跳跃", "跳跃空白字符", 
        DebugSystemKind::DEBUG_SYSTEM_DEBUG
    );

    /** 输出当前位置信息 */
    m_sDebugSystem->DebugSystemConsoleWrite(
        "lexer:空白跳跃", SltStream::MakeFormatString(
            "当前起始位置偏移: ", 
            m_sSourcePos.m_sOffset,
            " 行数: ",
            m_sSourcePos.m_sLine
        ), 
        DebugSystemKind::DEBUG_SYSTEM_DEBUG
    );

    /** 空白跳过实现 */
    m_sDebugSystem->DebugSystemConsoleWrite(
        "lexer:空白跳跃", 
        SltStream::MakeFormatString(
            "当前字符长度: ", 
            m_sSinglefileText.length()
        ), 
        DebugSystemKind::DEBUG_SYSTEM_DEBUG
    );

    /** 跳跃的主体功能
     * 使用 lexer::peer() 检测string中的每一个char
     * CR + LF 在这里处理
     * while 循环处理
     */
    while (auto chOpt = Peek()){
        // m_sDebugSystem->DebugSystemConsoleWrite(
        //     "lexer/SkipWhiteSpace",
        //     SltStream::MakeFormatString(
        //         "peek()切分当前的字符为: ",
        //         SltStream::MakeOutPutChar(Peek().value())
        //     ), 
        //     DebugSystemKind::DEBUG_SYSTEM_DEBUG
        // );
        /** 检查当前字符是否匹配空白符号 */
        auto kindOpt = GetWhiteSpaceMappingCT().Find(chOpt.value());
        /** 不匹配则结束，说明遇到了非空白 */
        if (!kindOpt.has_value()){
            m_sDebugSystem->DebugSystemConsoleWrite(
                "lexer:空白跳跃",
                SltStream::MakeFormatString(
                    "切分到第一个非跳跃字符: ",
                    SltStream::MakeOutPutChar(Peek().value())
                ), 
                DebugSystemKind::DEBUG_SYSTEM_DEBUG
            );
            break;
        }

        /** 匹配则检查 CR + LF */
        WhiteSpaceKind kind = kindOpt.value();
        auto lineFeed = GetWhiteSpaceMappingTC().Find(WhiteSpaceKind::LineFeed).value();
        /** 检查 CR */
        if (kind == WhiteSpaceKind::CarriageReturn){
            /** 消去CR (\r) */
            Consume(true);
            /** 检查LR (\n) */
            auto nextOpt = Peek();
            /** 如果是LR则这里处理重复加(减去) */
            if (nextOpt && nextOpt.value() == lineFeed){
                m_sSourcePos.m_sLine -= 1;
                Consume(true);
            }
            /** 这里处理列号 */
            m_sSourcePos.m_iRow = 0;
        }
        /** 其余空白字符直接吃掉 */
        else{
            Consume(true);
        }
    }
    /** 空白跳跃结尾输出数据 */
    m_sDebugSystem->DebugSystemConsoleWrite(
        "lexer:空白跳跃", SltStream::MakeFormatString(
            "当前结束位置偏移: ", 
            m_sSourcePos.m_sOffset,
            " 行数: ",
            m_sSourcePos.m_sLine,
            " 列号: ",
            m_sSourcePos.m_iRow
        ), 
        DebugSystemKind::DEBUG_SYSTEM_DEBUG
    );
}

/** Lexer 设置解析文本 */
void SltLexer::SetAnalysisText(const std::string& cinText){
    m_sSinglefileText = cinText;
    m_sDebugSystem->DebugSystemConsoleWrite(
        "Lexer", 
        SltStream::MakeFormatString("获得解析文本: ", cinText), 
        DebugSystemKind::DEBUG_SYSTEM_DEBUG
    );
}

/** 切词器读取数字Token */
token SltLexer::ReadNumber(){
    /** "."号标记,"."号只能出现一次 */
    bool dotFlag = false;
    /** 偏移标记，.不能作为数字的开头 */
    size_t startOffset = m_sSourcePos.m_sOffset;
    /** 起始行列记住，要放到返回值里面的 */
    size_t startLine = m_sSourcePos.m_sLine;
    size_t startRow = m_sSourcePos.m_iRow;
    /** buf缓冲string */
    std::string buf = "";
    /** 直接进入循环，当前的数字也要一起吃掉 */
    while (true){
        /** 获取当前数字的原始输出 */
        std::optional<unsigned char> originalNumChar = Peek();
        /** 检查当前的数字是否合法,或者说是否存在 */
        if (!originalNumChar.has_value()){
            m_sDebugSystem->DebugSystemConsoleWrite(
                "数字类型Token读取",
                "当前数字无法读取: 遭遇未知字符",
                DebugSystemKind::DEBUG_SYSTEM_ERROR,
                true
            );
            /** 当然这里的break没啥用，因为程序已经退出了 */
            break;
        }
        /** 获取当前的数字字符串(.号当然也要获取的) */
        unsigned char currentNumChar = originalNumChar.value();
        /** 检查是否是数字或者是 "." */
        if (isdigit(currentNumChar)){
            buf.push_back(currentNumChar);
            Consume(false);
            /** 下一次循环，不能去掉 */
            continue;
        }
        /** 如果不是字符串，检查是否是 "." 等特殊符号 */
        /** 先获取当前的类型 */
        std::optional<NormalConstKind> currentKind = GetNormalConstCT().Find(currentNumChar);
        /** 先判断一下类型存不存在 */ 
        if (!currentKind.has_value()){
            /** 不存在类型停止切词 */
            break;
        }
        /** 存在则对类型就行判断
         * 这里双向表要存的是一样的东西,不然就报错
         */
        switch (currentKind.value()){
            /** "." 号类型 */
            case NormalConstKind::Dot:
                buf.push_back(GetNormalConstTC().Find(currentKind.value()).value());
                /** 也要消费掉当前的 '.' */
                Consume(false);
                break;
            /** 其他没用的类型和这个无关的去除 */
            default:
                break;
        }
    }
    /** 拿到当前的行列的开头 */
    return token(TokenType::Number, buf, startLine, startRow);
}

/** 切词器读取字符串 */
token SltLexer::ReadString(){
    /** 吃掉当前的类型引号先 */
    unsigned char quote = Peek().value();
    /** 不带换行处理，当然默认也是不换行处理的 */
    Consume(false);
    /** 获取当前的引号类型 */
    NormalConstKind quoteKind = GetNormalConstCT().Find(quote).value();
    /** 获取当前起始的行列 */
    size_t startLine = m_sSourcePos.m_sLine;
    size_t startRow = m_sSourcePos.m_iRow;
    /** 获取字符串 */
    std::string buf = "";
    /** 循环获取字符 */
    while(true){
        /** 获取当前字符输出 */
        std::optional<unsigned char> originalChar = Peek();
        /** 如果当前文档没闭合，字符串遇到不是字符的玩意(我觉得一般来说字符不会莫名其妙的中断)就报错 */
        if (!originalChar){
            m_sDebugSystem->DebugSystemConsoleWrite(
                "字符串Token读取",
                "当前字符未闭合: 检测到非字符输入",
                DebugSystemKind::DEBUG_SYSTEM_ERROR,
                true
            );
            /** 当然这里的break没啥用，因为程序已经退出了 */
            break;
        }
        /** 获取当前字符的值 */
        unsigned char currentChar = originalChar.value();
        /** 检查闭合状态，这里是遇到另一个引号闭合 */
        std::optional<NormalConstKind> mayerQuote = GetNormalConstCT().Find(currentChar);
        /** 先检查是不是任意一种引号 */
        if (mayerQuote.has_value()){
            /** 检查当前引号是否是和初始的引号一致，一致就关闭 */
            if (quoteKind == mayerQuote.value()){
                // m_sDebugSystem->DebugSystemConsoleWrite(\
                //     "字符串Token读取",
                //     "读取到第二个闭合字符串",
                //     DebugSystemKind::DEBUG_SYSTEM_DEBUG
                // );
                /** 消费掉当前的第二个字符串 */
                Consume(false);
                break;
            }
        }
        /** 不是闭合引号直接吃下 */
        buf.push_back(currentChar);
        Consume(false);
    }
    /** 最后返回对应类型的Token
     * 行列用起始的引号开始
     */
    return token(TokenType::String, buf, startLine, startRow);
}

/** lexer解析标识符类型(其他乱七八糟的类型)
 * 标识符切词,语言默认类型比如 If, Else, Class等
 */
token SltLexer::ReadIdent(){
    /** 偏移标记，.不能作为数字的开头 */
    size_t startOffset = m_sSourcePos.m_sOffset;
    /** 起始行列记住，要放到返回值里面的 */
    size_t startLine = m_sSourcePos.m_sLine;
    size_t startRow = m_sSourcePos.m_iRow;
    /** buf缓冲string */
    std::string buf = "";
    /** 直接进入循环，当前的字符要一起吃掉 */
    while(true){
        /** 获取当前字符输出 */
        std::optional<unsigned char> originalChar = Peek();
        /** 如果当前文档没闭合，字符串遇到不是字符的玩意(我觉得一般来说字符不会莫名其妙的中断)就报错 */
        if (!originalChar){
            m_sDebugSystem->DebugSystemConsoleWrite(
                "标识符Token读取",
                "标识符非法: 检测到非字符输入",
                DebugSystemKind::DEBUG_SYSTEM_ERROR,
                true
            );
            /** 当然这里的break没啥用，因为程序已经退出了 */
            break;
        }
        /** 获取当前字符 */
        unsigned char currentChar = originalChar.value();
        /** 非法字符检测(先检查是不是字母) */
        if (!isalpha(currentChar)){
            /** 再检查是不是 '_' */
            if (!GetNormalConstTC().Find(NormalConstKind::UnderScore).value()){
                /** 不是'_' 直接报错 */
                /** 调试系统报错并停止程序 */
                m_sDebugSystem->DebugSystemConsoleWrite(
                    "标识符Token读取",
                    "标识符非法: 检测到合法字符，非法输入",
                    DebugSystemKind::DEBUG_SYSTEM_ERROR,
                    true
                );
            }
        }
        /** 检测退出条件: 含有空格跳跃字符类 */
        if (GetWhiteSpaceMappingCT().Find(currentChar).has_value()){
            /** 这里不要消费字符了，后面的留给空格跳跃 */
            break;
        }
        /** 合法添加字符 */
        buf.push_back(currentChar);
        /** 吃掉当前字符 */
        Consume(false);
    }
    /** 划分系统自定义类(就是乱七八糟类型的划分) */
    /** 先检查是否是系统类型之一 */
    std::optional<TokenType> mayerType = GetTokenTypeCT().Find(buf);
    /** 检查类型是否存在 */
    if (mayerType.has_value()){
        /** 类型存在则分配对应 Token */
        TokenType type = mayerType.value();
        return token(type, buf, startLine, startRow);
    }
    /** 类型不存在则默认返回Token(Ident)标识符 */
    return token(TokenType::Ident, buf, startLine, startRow);
}

/** lexer解析运算符 
 * 比如 + - * / 等运算符号
 * 这里就吃到二元符号
*/
token SltLexer::ReadOperator(){
    /** 偏移标记，.不能作为数字的开头 */
    size_t startOffset = m_sSourcePos.m_sOffset;
    /** 起始行列记住，要放到返回值里面的 */
    size_t startLine = m_sSourcePos.m_sLine;
    size_t startRow = m_sSourcePos.m_iRow;
    /** buf缓冲string */
    std::string buf = "";
    /** 这里只吃到二元符号 */
    /** 获取当前字符输出 */
    std::optional<unsigned char> originalChar = Peek();
    /** 遇到不知道什么玩意的字符报错 */
    if (!originalChar){
        m_sDebugSystem->DebugSystemConsoleWrite(
            "运算符Token读取",
            "运算符非法: 检测到非字符输入",
            DebugSystemKind::DEBUG_SYSTEM_ERROR,
            true
        );
    }
    /** 先吃掉当前的运算 */
    Consume(false);
    /** 再将当前运算符加入缓冲区 */
    unsigned char currentChar = originalChar.value();
    buf.push_back(currentChar);

    /** 预吃下一个字符，用于检查是否是双运算符号 */
    std::optional<unsigned char> secondOriginalChar = Peek(); 
    /** 遇到不知道什么玩意的字符报错 */
    if (!originalChar){
        m_sDebugSystem->DebugSystemConsoleWrite(
            "运算符Token读取",
            "运算符非法: 检测到非字符输入",
            DebugSystemKind::DEBUG_SYSTEM_ERROR,
            true
        );
    }
    /** 拼接优先检测双运算符号 */
    unsigned char secondChar = secondOriginalChar.value();
    /** 先拼接进去 */
    buf.push_back(secondChar);
    /** 用当前缓冲字符检查是否是双运算符 */
    std::optional<TokenType> maybeType = GetTokenTypeCT().Find(buf);
    /** 检查，如果有的话就用双字符 */
    if (maybeType.has_value()){
        TokenType returnType = maybeType.value();
        /** 消费并返回双字符类型 */
        Consume(false);
        token returnToken = token(returnType, buf, startLine, startRow);
        return returnToken;
    }
    /** 没有的话就返回单字符(除非单字符也查不到) */
    buf.pop_back();
    /** 先查单字符存在状态 */
    maybeType = GetTokenTypeCT().Find(buf);
    /** 如果没有则报错 */
    if (!maybeType.has_value()){
        m_sDebugSystem->DebugSystemConsoleWrite(
            "运算符Token读取",
            SltStream::MakeFormatString("运算符: ", buf, " 不存在"),
            DebugSystemKind::DEBUG_SYSTEM_ERROR,
            true
        );
    }
    TokenType returnType = maybeType.value();
    /** 有就返回 */
    token returnToken = token(returnType, buf, startLine, startRow);
}

/** 读取下一Token */
token SltLexer::NextToken(){
    /** 先跳过空格等无关字符 */
    SkipWhiteSpace();
    /** 检查下一个token的首字符 */
    std::optional<unsigned char> originalChar = Peek();
    /** 检查空值返回EOF结尾 */
    if (!originalChar.has_value()){
        return token(TokenType::Eof);
    }
    /** 获取当前字符用于下一步判断token类型 */
    unsigned char nextChar = originalChar.value();
    
    /** 
     * 类型判断检查下一个token类型并返回 
     * 数字
     * 字符串 */
    if (isdigit(nextChar)){
        return ReadNumber();
    }
    /** 字符串 */
    else if (
        nextChar == GetNormalConstTC().Find(NormalConstKind::CurlyQuote) 
        || 
        nextChar == GetNormalConstTC().Find(NormalConstKind::StraightQuote)
    ){
        return ReadString();
    }
    /** 标识符类型识别 */
    else if (
        isalpha(nextChar)
        ||  
        nextChar == GetNormalConstTC().Find(NormalConstKind::UnderScore)
    ){
        return ReadIdent();
    }
    /** 返回符号类型(剩下的全是符号) */
    else{
        return ReadOperator();
    }
    // /** 啥都没匹配上就报错 */
    // m_sDebugSystem->DebugSystemConsoleWrite(
    //     "lexer/Token获取",
    //     "未知字符",
    //     DebugSystemKind::DEBUG_SYSTEM_ERROR,
    //     true
    // );
}
/** 对外启动Lexer切词器,返回Vector<token>* */
std::vector<token>* SltLexer::GetLexerTokenVector(){
    /** 循环求到末尾，当然这里还没有 */
    NextToken();
    NextToken();
    NextToken();
    token token_now = NextToken();
    return &m_vTokenVector;
}

/** Lexer初始化 */
SltLexer::SltLexer(SltDebugSystem* o_sDebugSystem) {
    /** 当前位置指针以及tokenVector */
    m_sSourcePos.m_sOffset = 0;
    m_sSourcePos.m_sLine = 1;
    m_vTokenVector.clear();
    m_sSinglefileText = "";
    /** 获取调试系统 */
    m_sDebugSystem = o_sDebugSystem;
    m_sDebugSystem->DebugSystemConsoleWrite("Lexer", "初始化Lexer", DebugSystemKind::DEBUG_SYSTEM_DEBUG);
}

/** Lexer析构 */
SltLexer::~SltLexer(){
    /** 解构lexer内置变量 */
    /** 目前没有 */
    m_sDebugSystem->DebugSystemConsoleWrite("Lexer", "删除Lexer", DebugSystemKind::DEBUG_SYSTEM_DEBUG);
}
