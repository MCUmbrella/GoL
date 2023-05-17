//
// Created by mcumbrella on 23-5-10.
//

#include <fstream>
#include <sstream>
#include "GoL.h"
#include "Cell.h"
#include "CommonUtil.h"

// 设置CommonUtil::transparent的别名为t，节省字数
#define t CommonUtil::transparent

using namespace std;

GoL& GoL::getInstance()
{
    static GoL instance; // 引擎的唯一实例在程序开始运行时被自动初始化
    return instance;
}

GoL& GoL::init(const int& initLines, const int& initRows)
{
    if (initLines < 2 || initRows < 2) throw runtime_error("行数和列数必须大于2");

    // 在输入大小的基础上添加1细胞宽的边界，方便后期处理
    lines = initLines + 2;
    rows = initRows + 2;

    cout << "培养皿正在初始化，大小: " << initRows << " * " << initLines << endl;
    cells = vector<vector<Cell>>(lines);
    for (int i = 0; i != lines; ++i)
        for (int j = 0; j != rows; ++j)
            if (i == 0 || i == lines - 1)
                cells[i].emplace_back(STATE_BORDER); // 上下边界，初始化为边界细胞
            else
                cells[i].emplace_back(j == 0 || j == rows - 1 ? STATE_BORDER : STATE_DEAD); // 左右边界初始化为边界细胞，其余初始化为死细胞

    cacheCellNeighbours();

    cout << "培养皿初始化完成" << endl;
    return *this;
}

GoL& GoL::init(const string& initFilePath)
{
    cout << "使用存档文件: " << initFilePath << endl;
    ifstream in(initFilePath); // 以指定路径创建一个文件输入流
    if (!in) // 文件输入流无效？
        throw runtime_error(string("读取失败: ").append(initFilePath));

    // 读取存档的头两个数
    int l = 0, r = 0;
    in >> l >> r;
    // 初始化培养皿大小
    init(l, r);

    // 从存档应用培养皿状态
    cout << "正在读取培养皿状态" << endl;
    string line;
    for (int i = 0; i != getLines(); ++i)
    {
        if (in >> line)
        {
            if (line.length() != getRows()) // 行长和开头记录的列数不一致？
            {
                stringstream msg;
                msg << "行长不匹配: 行 " << i + 1 << " 长度应为 " << getRows() << " 实际为 " << line.length();
                throw runtime_error(msg.str());
            }
            for (int j = 0; j != getRows(); ++j)
                setStateOf(i + 1, j + 1, CommonUtil::parseCellState(line[j]));
        }
        else // 数据行数和开头记录的行数不一致？
        {
            stringstream msg;
            msg << "行数不匹配: 应为 " << getLines() << " 实际为 " << i;
            throw runtime_error(msg.str());
        }
    }
    cout << "培养皿状态加载完成" << endl;
    in.close(); // 及时关闭输入流，释放资源
    cout << "初始化完成" << endl;
    return *this;
}

GoL& GoL::save(const string& filePath)
{
    ofstream out(filePath); // 以指定路径构建文件输出流
    if (out.good()) // 输出流正常？
    {
        out << getLines() << ' ' << getRows() << endl;
        for (int i = 1; i <= getLines(); ++i)
        {
            for (int j = 1; j <= getRows(); ++j)
                out << cells[i][j].toChar();
            out << endl;
        }
        out.close();
    }
    return *this;
}

GoL& GoL::run()
{
    previousCells.push(vector<vector<Cell>>(cells)); // 把当前培养皿状态复制一份压入历史记录顶端
    calculateNextGeneration();
    applyNextGeneration();
    ++currentGeneration;
    return *this;
}

void GoL::calculateNextGeneration()
{
    for (int i = 1; i <= getLines(); ++i)
    {
        for (int j = 1; j <= getRows(); ++j)
        {
            Cell& c = getCell(i, j);
            c.setNextState(c.calculateNextState());
        }
    }
}

void GoL::applyNextGeneration()
{
    for (int i = 1; i <= getLines(); ++i)
    {
        for (int j = 1; j <= getRows(); ++j)
        {
            Cell& c = getCell(i, j);
            c.setState(c.getNextState());
        }
    }
}

void GoL::cacheCellNeighbours()
{
    if (flNoBorder) // 透明边界开启，使用t函数进行坐标映射
        for (int i = 1; i <= getLines(); ++i)
            for (int j = 1; j <= getRows(); ++j) // 边界细胞不参与迭代，无需设置周边缓存
            {
                Cell& c = cells[i][j];
                c.setNeighbour(&(cells[t(i - 1, getLines())][t(j - 1, getRows())]))
                 .setNeighbour(&(cells[t(i - 1, getLines())][t(j, getRows())]))
                 .setNeighbour(&(cells[t(i - 1, getLines())][t(j + 1, getRows())]))
                 .setNeighbour(&(cells[t(i, getLines())][t(j - 1, getRows())]))
                 .setNeighbour(&(cells[t(i, getLines())][t(j + 1, getRows())]))
                 .setNeighbour(&(cells[t(i + 1, getLines())][t(j - 1, getRows())]))
                 .setNeighbour(&(cells[t(i + 1, getLines())][t(j, getRows())]))
                 .setNeighbour(&(cells[t(i + 1, getLines())][t(j + 1, getRows())]));
            }
    else
        for (int i = 1; i <= getLines(); ++i)
            for (int j = 1; j <= getRows(); ++j)
            {
                Cell& c = cells[i][j];
                c.setNeighbour(&(cells[i - 1][j - 1]))
                 .setNeighbour(&(cells[i - 1][j]))
                 .setNeighbour(&(cells[i - 1][j + 1]))
                 .setNeighbour(&(cells[i][j - 1]))
                 .setNeighbour(&(cells[i][j + 1]))
                 .setNeighbour(&(cells[i + 1][j - 1]))
                 .setNeighbour(&(cells[i + 1][j]))
                 .setNeighbour(&(cells[i + 1][j + 1]));
            }
}

GoL& GoL::display(const bool& border)
{
    if (border) // 需要显示边界？
        for (int i = 0; i != lines; ++i)
        {
            for (int j = 0; j != rows; ++j)
                cout << cells[i][j].toString();
            cout << endl;
        }
    else // 跳过边界索引
        for (int i = 1; i <= getLines(); ++i)
        {
            for (int j = 1; j <= getRows(); ++j)
                cout << cells[i][j].toString();
            cout << endl;
        }
    return *this;
}

int GoL::getCurrentGeneration() const
{
    return currentGeneration;
}

int GoL::getLines() const
{
    return lines - 2;
}

int GoL::getRows() const
{
    return rows - 2;
}

bool GoL::isNoBorder() const
{
    return flNoBorder;
}

Cell& GoL::getCell(const int& line, const int& row)
{
    if (flNoBorder) // 透明边界开启，使用t函数进行坐标映射
        return cells[t(line, getLines())][t(row, getRows())];

    if ((line < 1 || line > getLines()) || (row < 1 || row > getRows()))
        throw out_of_range("getCell(): 坐标越界");
    return cells[line][row];
}

CellState GoL::getStateOf(const int& line, const int& row)
{
    if (flNoBorder)
        return cells[t(line, getLines())][t(row, getRows())].getState();

    if ((line < 1 || line > getLines()) || (row < 1 || row > getRows()))
        return STATE_BORDER;
    return cells[line][row].getState();
}

void GoL::setStateOf(const int& line, const int& row, CellState state)
{
    if (flNoBorder)
        cells[t(line, getLines())][t(row, getRows())].setState(state);

    if ((line < 1 || line > getLines()) || (row < 1 || row > getRows()))
        throw out_of_range("setStateOf(): 坐标越界");
    cells[line][row].setState(state);
}

GoL& GoL::toggleBorder(const bool& status)
{
    flNoBorder = status;
    return *this;
}

GoL& GoL::revert(const int& steps)
{
    if (steps < 1) return *this; // 至少回退1代，否则不做任何操作
    for (int i = 0; i != steps; ++i)
    {
        if (previousCells.empty()) // 历史记录空了？
            break; // 提前结束回退操作
        cells = vector<vector<Cell>>(previousCells.top()); // 用历史记录最顶上的状态覆盖当前培养皿的状态
        previousCells.pop(); // 删除最顶上的历史记录
        --currentGeneration; // 更新迭代计数
    }
    cacheCellNeighbours(); // 重新给每个（非边界）细胞编制缓存，因为内存地址有变
    return *this;
}

GoL& GoL::forward(const int& steps)
{
    if (steps > 0)
        for (int i = 0; i != steps; ++i)
            run();
    return *this;
}
