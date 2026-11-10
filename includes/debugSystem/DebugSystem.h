#pragma once
#ifndef DEBUG_SYSTEM_H
#define DEBUG_SYSTEM_H
#define DEBUG_SYSTEM_VERSION "1.0.0"
#endif

/** 这里是调试系统的控制台颜色 */
#ifndef DEBUG_SYSTEM_COLOR
#define DEBUG_SYSTEM_COLOR
#define SLT_COLOR_RED       "\033[31m"
#define SLT_COLOR_GREEN     "\033[32m"
#define SLT_COLOR_YELLOW    "\033[33m"
#define SLT_COLOR_BLUE      "\033[34m"
#define SLT_COLOR_CYAN      "\033[36m"
#define SLT_COLOR_RESET     "\033[0m"
#endif

/** 调试系统属性定义 */
#ifndef DEBUG_SYSTEM_ATTRIBUTES
#define DEBUG_SYSTEM_ATTRIBUTES
#define DEBUG_SYSTEM_LOG_DIR "logs"
#define DEBUG_SYSTEM_LOG_NAME "logs.txt"
#endif

/** 系统头文件 */
#include <string>
#include <fstream>
#include <mutex>
#include <filesystem>
#include <sstream>

/** 这里是调试系统的日志类型枚举 */
enum class DebugSystemKind {
    DEBUG_SYSTEM_LOG,
    DEBUG_SYSTEM_DEBUG,
    DEBUG_SYSTEM_WARNING,
    DEBUG_SYSTEM_ERROR
};

/** 这里是SLT调试系统类 */
class SltDebugSystem{
private:
    /** 调试系统的单例指针以及线程锁 */
    static SltDebugSystem* m_pInstance;     // 调试系统的单例指针
    static std::mutex m_minstanceMtx;       // 调试系统的线程锁

    /** 调试系统各项属性的开关 */
    bool m_bDebugSystemEnable;              // 调试系统是否启用
    bool m_bDebugSystemLogEnable;           // 调试系统普通日志是否启用
    bool m_bDebugSystemDebugEnable;         // 调试系统调试日志是否启用
    bool m_bDebugSystemWarningEnable;       // 调试系统警告日志是否启用
    bool m_bDebugSystemErrorEnable;         // 调试系统错误日志是否启用
    bool m_bDebugSystemConsoleEnable;       // 调试系统控制台是否启用
    bool m_bDebugSystemFileEnable;          // 调试系统文件是否启用
    bool m_bDebugSystemConsolePauseEnable;  // 调试系统控制台是否暂停
    bool m_bDebugSystemShutdown;            // 调试系统是否在自销毁
    bool m_bConsoleVTEnabled;               // 控制台是否启用 VT (Virtual Terminal)
    

    /** WINDOWS CMD 控制台 */
    bool IsVTEnabled();
    bool TryEnableVT();
    void WindowsCMDConsoleUTF8();
    /** 调试系统日志函数以及其他函数 */
    void DebugSystemLogWrite(const std::string& logMessage);

    std::string GetDebugTimestamp();
    /** 返回exe程序路径 */
    std::filesystem::path GetLogBase();
public:
    SltDebugSystem();
    ~SltDebugSystem();

    /** 获取调试系统单例指针以及对应操作 */
    static SltDebugSystem* GetInstance(); 
    
    /** 设置调试系统是否启用 */
    void SetDebugSystemEnable(bool enable);
    void SetDebugSystemConsoleEnable(bool enable);
    void SetDebugSystemFileEnable(bool enable);
    void SetDebugSystemConsolePauseEnable(bool enable);
    /** 调试系统各个子模块 */
    void SetDebugSystemLogKindEnable(bool enable);
    void SetDebugSystemDebugKindEnable(bool enable);
    void SetDebugSystemWarningKindEnable(bool enable);
    void SetDebugSystemErrorKindEnable(bool enable);

    /** 获取调试系统是否启用 */
    bool GetDebugSystemEnable();
    bool GetDebugSystemLogEnable();
    bool GetDebugSystemConsoleEnable();
    bool GetDebugSystemFileEnable();
    bool GetDebugSystemConsolePauseEnable();

    /** 调试系统各个系统运行函数 */
    void DebugSystemRun();

    void DebugSystemConsoleWrite(const std::string& moduleName, const std::string& logMessage, DebugSystemKind logKind = DebugSystemKind::DEBUG_SYSTEM_DEBUG);
    ///** 调试系统字符串相加模板*/
    //template <typename... Args>
    //void DebugSystemConsoleWrite(const std::string& moduleName, DebugSystemKind logKind, Args&&... Args);

    void DebugSystemFileRead();
    void DebugSystemConsolePauseRun();

    /** 调试系统操作参数 */
    void DebugSystemDestroy();
    
};