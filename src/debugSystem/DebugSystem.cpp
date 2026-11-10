/** 调试系统的cpp源文件 */
/** WINDOWS 的头文件 */
#ifdef _WIN32
#include <windows.h>
#endif
/** 调试系统的系统头 */
#include <iostream>
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <mutex>
#include <filesystem>
/** 调试系统的自定义头文件 */
#include "debugSystem/debugSystem.h"

/** 初始化静态变量 */
SltDebugSystem* SltDebugSystem::m_pInstance = nullptr;
std::mutex SltDebugSystem::m_minstanceMtx;

/** 获取调试系统单例指针 */
SltDebugSystem* SltDebugSystem::GetInstance() {
    // 线程锁，防止多线程创建单例
    std::lock_guard<std::mutex> lock(m_minstanceMtx);
    if (SltDebugSystem::m_pInstance == nullptr) {
        SltDebugSystem::m_pInstance = new SltDebugSystem();
    }
    return SltDebugSystem::m_pInstance;
}

/** 设置调试系统是否启用 */
void SltDebugSystem::SetDebugSystemEnable(bool enable){
    m_bDebugSystemEnable = enable;
}
void SltDebugSystem::SetDebugSystemConsoleEnable(bool enable){
    m_bDebugSystemConsoleEnable = enable;
}
void SltDebugSystem::SetDebugSystemFileEnable(bool enable){
    m_bDebugSystemFileEnable = enable;
}
void SltDebugSystem::SetDebugSystemConsolePauseEnable(bool enable){
    m_bDebugSystemConsolePauseEnable = enable;
}
void SltDebugSystem::SetDebugSystemLogKindEnable(bool enable) {
    m_bDebugSystemLogEnable = enable;
}
void SltDebugSystem::SetDebugSystemDebugKindEnable(bool enable) {
    m_bDebugSystemDebugEnable = enable;
}
void SltDebugSystem::SetDebugSystemWarningKindEnable(bool enable) {
    m_bDebugSystemWarningEnable = enable;
}
void SltDebugSystem::SetDebugSystemErrorKindEnable(bool enable) {
    m_bDebugSystemErrorEnable = enable;
}

/** 调试系统自销毁 */
void SltDebugSystem::DebugSystemDestroy(){
    std::lock_guard<std::mutex> lock(m_minstanceMtx);
    DebugSystemConsoleWrite("调试系统", "收到自销毁请求", DebugSystemKind::DEBUG_SYSTEM_DEBUG);
    // 标记调试系统自毁
    m_bDebugSystemShutdown = true;
    // 全局调用剪短，置空单例(注意这里单例也会被置空)
    SltDebugSystem* self = this;
    m_pInstance = nullptr;
    delete self;
}

/** 获得调试时间戳 */
std::string SltDebugSystem::GetDebugTimestamp(){
    auto current_time = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(current_time);
    std::tm tm = *std::localtime(&t);
    // 格式转换为字符串类型用于输出
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y%m%d_%H%M%S");
    return oss.str();
}

/** exe 所在位置 */
std::filesystem::path GetExeDir(){
    // windows系统与linux系统exe?路径
    #ifdef _WIN32
        wchar_t buf[1024]{};
        if (!GetModuleFileNameW(nullptr, buf, static_cast<DWORD>(std::size(buf)))){
            throw std::runtime_error("GetModuleFileNamew failed");
        }
        return std::filesystem::path(buf).parent_path();
    #else
        char buf[1024]{};
        ssize_t len = readlink("/proc/self/exe", buf, sizeof(buf) - 1);
        if (len <= 0){
            throw std::runtime_error("readlink /proc/self/exe failed");
        }
        buf[len] = '\0';
        return std::filesystem::path(buf).parent_path();
    #endif
    }

/** 日志写入系统 */
void SltDebugSystem::DebugSystemLogWrite(const std::string& logMessage){
    /** CMake 项目获得路径为根目录路径 */
    std::filesystem::path currentDir = GetExeDir();
    std::filesystem::path logOutDir = currentDir / DEBUG_SYSTEM_LOG_DIR;
    /** 日志文件创建路径 */
    if (!std::filesystem::exists(logOutDir)){
        std::filesystem::create_directories(logOutDir);
    }
    /** 日志文件写入 */
    std::filesystem::path logOutfilePath = logOutDir / DEBUG_SYSTEM_LOG_NAME;
    std::ofstream fout(logOutfilePath.string(), std::ios::app);
    // 检查文件打开状态
    if (!fout.is_open()){
        return;
    }
    fout << logMessage << std::endl;
}

/** 调试控制台功能
 *  调试系统控制台暂停 
 *  调试控制台普通输出
*/
void SltDebugSystem::DebugSystemConsolePauseRun() {
    if (this->m_bDebugSystemConsolePauseEnable) {
        this->DebugSystemConsoleWrite("调试系统", "调试系统控制台暂停运行，请按任意键继续...", DebugSystemKind::DEBUG_SYSTEM_LOG);
        system("pause");
    }
    this->DebugSystemConsoleWrite("调试系统", "调试系统控制台运行中...", DebugSystemKind::DEBUG_SYSTEM_LOG);
}

/** 调试系统控制台日志输出+日志处理流程 */
void SltDebugSystem::DebugSystemConsoleWrite(const std::string& moduleName, const std::string& logMessage, DebugSystemKind logKind) {
    std::lock_guard<std::mutex> lock(m_minstanceMtx);
    /** 调试系统销毁状态则停止运行 */
    if (m_bDebugSystemShutdown){
        return;
    }
    /** 调试系统状态检查并选择对应调试类型 */
    std::string colorCode;
    bool isOutput = true;
    switch (logKind) {
        case DebugSystemKind::DEBUG_SYSTEM_LOG:
            colorCode = SLT_COLOR_GREEN;
            isOutput = this->m_bDebugSystemLogEnable;
            break;
        case DebugSystemKind::DEBUG_SYSTEM_DEBUG:
            colorCode = SLT_COLOR_YELLOW;
            isOutput = this->m_bDebugSystemDebugEnable;
            break;
        case DebugSystemKind::DEBUG_SYSTEM_WARNING:
            colorCode = SLT_COLOR_YELLOW;
            isOutput = this->m_bDebugSystemWarningEnable;
            break;
        case DebugSystemKind::DEBUG_SYSTEM_ERROR:
            colorCode = SLT_COLOR_RED;
            isOutput = this->m_bDebugSystemErrorEnable;
            break;
        default:
            colorCode = SLT_COLOR_RESET;
            break;
    }
    std::string debugOutput = "";

    /** 调试输出头处理 */
    /** 先处理事件戳，再处理模块，最后其他事件 */
    std::string outTimeStamp = "[" + GetDebugTimestamp() + "]";
    std::string outModuleName = "[" + moduleName + "]";
    // 时间戳添加,检测VT启用状态
    if (this->m_bConsoleVTEnabled){
        debugOutput += SLT_COLOR_CYAN;
    }
    debugOutput += outTimeStamp;
    if (this->m_bConsoleVTEnabled){
        debugOutput += SLT_COLOR_RESET;
    }
    // 检测VT是否启用，如果启用则使用颜色输出，否则不使用颜色输出
    if (this->m_bConsoleVTEnabled) {
            debugOutput += colorCode;
    }
    debugOutput += outModuleName;
    // 尾部中止颜色，防止控制台后文全部变色
    if (this->m_bConsoleVTEnabled) {
            debugOutput += SLT_COLOR_RESET;
    }
    debugOutput += logMessage;

    /** 日志中间处理流程 */
    // 添加进日志文件,这里去除VT颜色
    std::string writeInLog = outTimeStamp + outModuleName + logMessage;
    DebugSystemLogWrite(writeInLog);

    /** 控制台输出 */
    if (this->m_bDebugSystemConsoleEnable && isOutput) {
        std::cout << debugOutput << std::endl;
    }
}

/** 检查 WINDWOS VT (Virtual Terminal) 是否启用 */
bool SltDebugSystem::IsVTEnabled() {
#ifdef _WIN32
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hStdOut == INVALID_HANDLE_VALUE) {
        return false;
    }
    // 检查当前是否为控制台设备
    DWORD consoleMode = 0;
    if (!GetConsoleMode(hStdOut, &consoleMode)) {
        return false;
    }
    // 检测 VT 标记，如果已启用，则返回 true
    const DWORD VT_FLAG = ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    return (consoleMode & VT_FLAG) != 0;
#else
    return isatty(STDOUT_FILENO);
#endif
}

/** 尝试启用 WINDOWS VT (Virtual Terminal) */
bool SltDebugSystem::TryEnableVT() {
#ifdef _WIN32
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hStdOut == INVALID_HANDLE_VALUE) {
        return false;
    }
    DWORD consoleMode = 0;
    if (!GetConsoleMode(hStdOut, &consoleMode)) {
        return false;
    }
    // 尝试启用 VT 标记
    const DWORD VT_FLAG = ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    consoleMode |= VT_FLAG;
    if (!SetConsoleMode(hStdOut, consoleMode)) {
        return false;
    }
    GetConsoleMode(hStdOut, &consoleMode);
    return (consoleMode & VT_FLAG) != 0;
#else
    return isatty(STDOUT_FILENO);
#endif
}

/** WINDOWS CMD 控制台 UTF 8*/
void SltDebugSystem::WindowsCMDConsoleUTF8() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    if (!this->m_bConsoleVTEnabled){
        return;
    }
    // ANSI/VT100 CMD 控制台颜色
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hStdOut != INVALID_HANDLE_VALUE) {
        DWORD consoleMode;
        if (GetConsoleMode(hStdOut, &consoleMode)) {
            consoleMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
            SetConsoleMode(hStdOut, consoleMode);
        }
    }
#endif
}

/** 调试系统的构造函数 */
SltDebugSystem::SltDebugSystem() {
    this->m_bDebugSystemConsoleEnable       = true;
    this->m_bDebugSystemConsolePauseEnable  = true;
    this->m_bDebugSystemEnable              = true;
    this->m_bDebugSystemFileEnable          = true;
    /** 各项日志输出初始化 */
    this->m_bDebugSystemLogEnable           = true;
    this->m_bDebugSystemDebugEnable         = true;
    this->m_bDebugSystemWarningEnable       = true;
    this->m_bDebugSystemErrorEnable         = true;
    this->m_bDebugSystemShutdown            = false;
    /** 初始化调试系统状态 */
    this->m_bConsoleVTEnabled = this->TryEnableVT();
    this->WindowsCMDConsoleUTF8();
}

/** 调试系统的析构函数 */
SltDebugSystem::~SltDebugSystem() {
    if (SltDebugSystem::m_pInstance != nullptr) {
        this->DebugSystemConsoleWrite("调试系统", "正在析构调试系统", DebugSystemKind::DEBUG_SYSTEM_DEBUG);
        SltDebugSystem::m_pInstance = nullptr;
    }
}