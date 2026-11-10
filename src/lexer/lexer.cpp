/** lexer的核心功能实现 */
/** 自定义系统头文件 */
#include <lexer/lexer.h>
#include <debugSystem/DebugSystem.h>
#include <SltStream/SltStream.h>

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
     * 使用 string.at 检测string中的每一个char
     * 需要注意长度检查需要在 at 之前防止 at 的报错
     */
    size_t fileTextLength = m_sSinglefileText.length();
    while (m_sSourcePos.m_sOffset < fileTextLength && m_sSinglefileText.at(m_sSourcePos.m_sOffset)){
        break;
        /** 这里要写哦，这里是死循环了 */
    }
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

/** 读取数字Token */
std::string SltLexer::ReadNumber(){
    return "";
}

/** 对外启动Lexer切词器,返回Vector<token>* */
std::vector<token>* SltLexer::GetLexerTokenVector(){
    SkipWhiteSpace();
    return &m_vTokenVector;
}

/** Lexer初始化 */
SltLexer::SltLexer() {
    /** 当前位置指针以及tokenVector */
    m_sSourcePos.m_sOffset = 0;
    m_sSourcePos.m_sLine = 0;
    m_vTokenVector.clear();
    m_sSinglefileText = "";
    /** 获取调试系统 */
    m_sDebugSystem = SltDebugSystem::GetInstance();
    m_sDebugSystem->DebugSystemConsoleWrite("Lexer", "初始化Lexer", DebugSystemKind::DEBUG_SYSTEM_DEBUG);
}

/** Lexer析构 */
SltLexer::~SltLexer(){
    /** 解构lexer内置变量 */
    /** 目前没有 */
    m_sDebugSystem->DebugSystemConsoleWrite("Lexer", "删除Lexer", DebugSystemKind::DEBUG_SYSTEM_DEBUG);
}
