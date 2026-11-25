#include<iostream>
/** SLT系统调试区，以后会作为主启动exe的主代码
 * 后续可能会加上SLT的DI容器I
 */
#include <debugSystem/debugSystem.h>
#include <objectPtr/ObjectPtr.h>
#include <lexer/lexer.h>
#include <utils/lexerUtil/LexerUtil.h>
#include <SltStream/SltStream.h>
#include <lib/sltMap/SltMap.h>

int main() {
    SltDebugSystem* pDebugSystem = SltDebugSystem::GetInstance();
    pDebugSystem->SetDebugSystemLogWriteEnable(false);
    SltLexer* lexer = new SltLexer(SltDebugSystem::GetInstance());
    /** 测试 lexer，给定源码 */
    lexer->SetAnalysisText(
        SltStream::MakeFormatString(
            "num UseNumA = 10;",
            "num UseNumB = 20;",
            "if (UseNumA < UseNumB){",
            "   num UseNumC = UseNumA + UseNumB;",
            "}" 
        )
    );
    std::vector<token> tokens = lexer->GetLexerTokenVector();
    /** 测试tokens的输出 */
    pDebugSystem->DebugSystemConsoleWrite(
        "主程序",
        LexerUtil::TokenVectorString(tokens),
        DebugSystemKind::DEBUG_SYSTEM_DEBUG
    );
    
    /** 测试映射 */
    // SltMap<int, int>* sltMap = new SltMap<int, int>{
    //     {10, 5},
    //     {11, 4},
    //     {10, 6}
    // };
    // auto result = sltMap->Find(11);
    // size_t index = sltMap->FindKeyIndex(10);
    // sltMap->Delete(11);
    // sltMap->CheckExist(11);
    // std::cout << result.value_or(0);
    /** 系统暂停 */
    pDebugSystem->DebugSystemConsolePauseRun();
    delete lexer;

    /** 测试区 */
    pDebugSystem->DebugSystemDestroy();
    system("pause");
    return EXIT_SUCCESS;
}