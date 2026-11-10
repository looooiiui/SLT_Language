#include<iostream>
/** SLT系统调试区，以后会作为主启动exe的主代码
 * 后续可能会加上SLT的DI容器I
 */
#include "debugSystem/debugSystem.h"
#include "objectPtr/ObjectPtr.h"
#include "lexer/lexer.h"
#include "SltStream/SltStream.h"
#include "lib/sltMap/SltMap.h"

int main() {
    SltLexer* lexer = new SltLexer();
    /** 测试 lexer，给定源码 */
    lexer->SetAnalysisText("num a = 5487; num test{ str b = \"haha\"; return 54 }");
    lexer->GetLexerTokenVector();
    /** 测试映射 */
    SltMap<int, int>* sltMap = new SltMap<int, int>{
        {10, 5},
        {11, 4},
        {10, 6}
    };
    
    auto result = sltMap->Find(20);
    std::cout << result.value_or(0);

    /** 系统暂停 */
    SltDebugSystem* pDebugSystem = SltDebugSystem::GetInstance();
    pDebugSystem->DebugSystemConsolePauseRun();
    delete lexer;

    /** 测试区 */
    pDebugSystem->DebugSystemDestroy();
    system("pause");
    return EXIT_SUCCESS;
}