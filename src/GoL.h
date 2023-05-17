//
// Created by mcumbrella on 23-5-10.
//

#ifndef GOL_GOL_H
#define GOL_GOL_H

#include <iostream>
#include <vector>
#include <stack>
#include "Cell.h"

using std::vector;
using std::stack;

/**
 * Game of Life模拟引擎，采用单例模式设计
 */
class GoL
{
private:
    bool flNoBorder = false; // 透明边界状态
    int
        currentGeneration = 0, // 当前迭代数。0表示初始状态
        rows = 0, // 培养皿列数
        lines = 0; // 培养皿行数
    vector<vector<Cell>> cells; // 培养皿，存储所有细胞实例
    stack<vector<vector<Cell>>> previousCells; // 存储所有培养皿之前的状态

    GoL() = default; // 设置默认实现为private，确保GoL不能在外部被其它代码实例化

    ~GoL() = default; // 设置默认解构操作为private，确保GoL不能在外部被其它代码销毁

    /**
     * 让每个细胞计算和设置其下次迭代时的状态
     */
    void calculateNextGeneration();

    /**
     * 让每个细胞应用其下次迭代时的状态，最后让迭代计数+1，即完成一次迭代
     */
    void applyNextGeneration();

    /**
     * 设置每个细胞的周边缓存
     */
    void cacheCellNeighbours();

public:
    GoL(const GoL&) = delete; // 阻止拷贝，防止通过拷贝构造函数创建新实例

    GoL& operator =(const GoL&) = delete; // 阻止赋值，防止通过赋值创建新实例

    /**
     * 获取模拟引擎的唯一实例
     */
    static GoL& getInstance();

    /**
     * 初始化一个指定大小的培养皿
     * @param initLines 培养皿的行数（不包括上下边界的两行）
     * @param initRows 培养皿的列数（不包括左右边界的两行）
     */
    GoL& init(const int& initLines, const int& initRows);

    /**
     * 使用指定路径的存档文件初始化培养皿
     * @param initFilePath 文件的路径
     */
    GoL& init(const std::string& initFilePath);

    /**
     * 保存当前培养皿的状态到指定文件
     * @param filePath 文件的路径
     */
    GoL& save(const std::string& filePath);

    /**
     * 进行一次迭代
     */
    GoL& run();

    /**
     * 输出当前培养皿的状态到控制台
     * @param border 输出是否包括培养皿边界
     */
    GoL& display(const bool& border);

    /**
     * 获取当前迭代次数
     */
    int getCurrentGeneration() const;

    /**
     * 获取培养皿的行数（不包括上下边界）
     */
    int getLines() const;

    /**
     * 获取培养皿的列数（不包括左右边界）
     */
    int getRows() const;

    /**
     * 透明边界是否被启用？
     */
    bool isNoBorder() const;

    /**
     * 获取指定位置的细胞的引用
     * @param line 行数（从1开始）
     * @param row 列数（从1开始）
     */
    Cell& getCell(const int& line, const int& row);

    /**
     * 获取指定位置的细胞的状态
     * @param line 行数（从1开始）
     * @param row 列数（从1开始）
     */
    CellState getStateOf(const int& line, const int& row);

    /**
     * 设置指定位置的细胞的状态
     * @param line 行数（从1开始）
     * @param row 列数（从1开始）
     * @param state 要设置成什么状态？
     */
    void setStateOf(const int& line, const int& row, CellState state);

    /**
     * 设置透明边界功能的状态。当透明边界功能启用时，培养皿的对边会互通。
     * 假设有数组A{a, b, c, d, e}，当透明边界开启时，A[-1]=A[4]=e，
     * A[-2]=A[3]=d，A[5]=A[0]=a，A[6]=A[1]=b，以此类推。
     * 对于培养皿的典型表现就是一组移动的细胞在到达一边的边界后会从
     * 对面的边界处出现
     * @param status 新的透明边界状态
     */
    GoL& toggleBorder(const bool& status);

    /**
     * 撤销指定次迭代
     */
    GoL& revert(const int& steps);

    /**
     * 一次性进行指定次迭代
     */
    GoL& forward(const int& steps);
};

#endif //GOL_GOL_H
