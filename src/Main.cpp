#include <csignal>
#include "GoL.h"
#include "CommonUtil.h"

using namespace std;

static bool
    flInfiniteGenerations = true, // 无尽模式
    flPause = false, // 暂停状态
    flNoBorder = false, // 透明边界开启状态
    flShowBorder = false; // 边界显示状态
static unsigned int sleepMs = 500; // 每次迭代时间间隔（毫秒）
static unsigned long targetGeneration; // 目标迭代次数

/**
 * 显示操作菜单。用户可以进行一些操作（比如退出、快进/快退、导出等）
 * 菜单会在无限模拟开始时、新培养皿创建后和每次按Ctrl+C时显示
 */
void showMenu(int);

/**
 * 无限循环，用来进行自动化模拟
 */
void mainLoop();

/**
 * 重置标准输入
 */
void resetStdin()
{
    // 清除标准输入流的缓冲区
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
    fflush(stdin);
    // 重设标准输入流的错误状态
    clearerr(stdin);
    cin.clear();
}

/**
 * 程序入口。两个参数由操作系统在运行程序时自动设置（具体值取决于运行程序的命令）
 * @param argc 程序的参数数量，至少为1。
 * @param argv C风格字符串数组，存储程序文件的路径(argv[0])以及参数
 */
int main(int argc, char** argv)
{
    if (argc < 2) // 参数不够，显示帮助信息后退出
    {
        cout << "用法: GoL <--new / 存档路径> [--targetGeneration={}] [--sleepMs={}] [--noBorder] [--showBorder]" << endl
             << "参数:" << endl
             << " new:              创建一个新培养皿" << endl
             << " 存档路径:         从指定的文件加载一个现有的培养皿" << endl
             << " targetGeneration: 目标迭代次数。不加此参数即为无限制" << endl
             << " sleepMs:          每次自动迭代的时间间隔(毫秒)。不加此参数即为500" << endl
             << " noBorder:         启用透明边界功能" << endl
             << " showBorder:       显示培养皿的边界" << endl;
        return 0;
    }

    // 处理命令行参数
    vector<string> args;
    for (int i = 1; i != argc; ++i) // argv[0]为程序文件的路径，跳过
        args.emplace_back(argv[i]);

    for (string& arg : args)
        if (arg.rfind("--targetGeneration=", 0) == 0) // 以“--targetGeneration=”开头
        {
            try
            {
                targetGeneration = stoul(arg.substr(19)); // 把“--targetGeneration=”后的内容解析为unsigned long
                flInfiniteGenerations = false; // 关闭无尽模式
            }
            catch (...) // 解析时出错，使用默认值
            {
                // flInfiniteGenerations = true
            }
        }
        else if (arg.rfind("--sleepMs=", 0) == 0)
            try
            {
                sleepMs = stoi(arg.substr(10));
            }
            catch (...)
            {
                // sleepMs = 500
            }
        else if (arg == "--noBorder")
            flNoBorder = true;
        else if (arg == "--showBorder")
            flShowBorder = true;

    // 初始化模拟引擎
    GoL& app = GoL::getInstance();
    app.toggleBorder(flNoBorder);
    if (args[0] == "--new") // 创建新培养皿
    {
        int lines = 0, rows = 0;
        cout << "输入: 培养皿大小 (X Y)" << endl << "? ";
        cout.flush();
        cin >> rows >> lines;
        app.init(lines, rows);
        showMenu(0);
    }
    else // 从现有存档文件加载培养皿
    {
        app.init(args[0]);
        // 如果指定了目标迭代次数，不显示菜单，直接开始自动模拟
        if (flInfiniteGenerations) showMenu(0);
    }

    signal(SIGINT, showMenu); // 监听Ctrl+C信号（SIGINT）
    mainLoop();

    return 0;
}

void mainLoop()
{
    GoL& app = GoL::getInstance();
    while (flInfiniteGenerations || app.getCurrentGeneration() != targetGeneration) // 是无尽模式或未达到目标迭代次数？
    {
        if (flPause) // 暂停？
        {
            CommonUtil::freeze(500); // 限制循环速率，避免占用大量cpu资源
            continue;
        }
        CommonUtil::clearScreen();
        app.run().display(flShowBorder); // 迭代一次，然后显示培养皿的新状态
        cout << "迭代次数: " << app.getCurrentGeneration()
             << ". 培养皿大小: " << app.getRows() << "*" << app.getLines() << endl
             << "[Ctrl+C]暂停" << endl;
        flush(cout); // 释放标准输出缓存，确保信息输出全
        CommonUtil::freeze(sleepMs); // 等待指定时长，避免运行速度过快
    }
    cout << "已到达指定迭代次数" << endl;
}

void showMenu(int)
{
    flPause = true; // 设置为暂停状态。Windows系统在新线程中调用该函数，不设置会导致模拟在显示操作菜单时仍然在后台运行
    GoL& app = GoL::getInstance();

    for (;;)
    {
        // 首先显示当前培养皿状态
        CommonUtil::clearScreen();
        app.display(flShowBorder);
        cout << "迭代次数: " << app.getCurrentGeneration()
             << ". 培养皿大小: " << app.getRows() << "*" << app.getLines() << endl;

        // ask for option
        cout << "[Q]退出 [W]开始/继续 [E]编辑 [R]上一代 [T]跳转 [Y]另存为" << endl << "? ";
        flush(cout);
        string s;
        cin >> s;
        // check option
        if (s == "q" || s == "Q") // 退出
        {
            cout << "退出" << endl;
            exit(0);
        }
        else if (s == "w" || s == "W") // 开始/继续
        {
            break; // 退出循环
        }
        else if (s == "e" || s == "E") // 编辑
        {
            cout << "输入: 横坐标 纵坐标 状态(1=活, 0=死)" << endl << "? ";
            flush(cout);
            int x, y;
            char state;
            if (cin >> x >> y >> state && x > 0 && x <= app.getRows() && y > 0 && y <= app.getLines())
                app.setStateOf(y, x, CommonUtil::parseCellState(state));
        }
        else if (s == "r" || s == "R") // 上一代
        {
            app.revert(1);
        }
        else if (s == "t" || s == "T") // 跳转
        {
            cout << "输入: 目标迭代次数" << endl << "? ";
            flush(cout);
            int g;
            if (cin >> g)
            {
                if (g == app.getCurrentGeneration()) continue; // 目标代和当前代相同
                if (g > app.getCurrentGeneration()) // 目标代在当前代之后，快进
                {
                    cout << "加载中";
                    flush(cout);
                    app.forward(g - app.getCurrentGeneration());
                }
                else // 目标代在当前代之前，快退
                {
                    cout << "加载中";
                    flush(cout);
                    app.revert(app.getCurrentGeneration() - g);
                }
            }
        }
        else if (s == "y" || s == "Y") // 另存为
        {
            cout << "输入: 文件路径" << endl << "? ";
            flush(cout);
            string path;
            cin >> path;
            if (!path.empty()) app.save(path);
        }
        resetStdin(); // 输入异常，重置标准输入流后要求再次输入
    }
    resetStdin(); // 清除任何残留输入，防止下次暂停时被误读

    // 继续
    signal(SIGINT, showMenu); // 重新设置为该函数处理Ctrl+C信号，因为在Windows系统中Ctrl+C信号被捕获一次后相应的处理函数会失效
    flPause = false; // 解除暂停状态
}
