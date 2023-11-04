#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

int processNum = 5;  // 记录进程个数
int resourceNum = 3; // 记录资源类型个数
int requireNum = 0;  // 申请进程号

// 使用的数据结构
int Max[20][20];             // Max[i][j]表示进程i对j类资源的最大需求
int Allocation[20][20];      // Allocation[i][j]表示已分配给进程i的j类资源数
int Need[20][20];            // Need[i][j]表示进程i当前所需的j类资源数
int Available[20];           // Available[i]表示第i类资源现有资源数
int request[20];             // 申请的资源情况
int WorkStatus[20];          // 用于检查安全性的工作矩阵
bool Finished[20] = {false}; // 用于记录进程的完成情况
int Safety[20];              // 安全序列

// 获取最大需求矩阵 Max
void getMax()
{
    printf("请输入最大需求矩阵 Max：\n");
    for (int i = 0; i < processNum; i++)
    {
        printf("%2d号进程：\n", i + 1);
        for (int j = 0; j < resourceNum; j++)
        {
            // int maxij=0;
            scanf("%d", &Max[i][j]);
        }
        // printf("\n");
    }
}

// 获取已分配资源
void getAllocation()
{
    printf("请输入已分配资源矩阵 Allocation：\n");
    for (int i = 0; i < processNum; i++)
    {
        printf("%d号进程已分配：\n", i + 1);
        for (int j = 0; j < resourceNum; j++)
        {
            // int maxij=0;
            scanf("%d", &Allocation[i][j]);
        }
        // printf("\n");
    }
}

// 计算需求矩阵，这一部分可以和获取资源分配同时进行，减少循环次数，这里分开写显示逻辑性
void getNeed()
{
    for (int i = 0; i < processNum; i++)
    {
        for (int j = 0; j < resourceNum; j++)
        {
            Need[i][j] = Max[i][j] - Allocation[i][j];
        }
    }
    printf("Need矩阵计算完毕\n");
}

void getAvailable()
{
    printf("请输入可分配资源矩阵：\n");
    for (int i = 0; i < resourceNum; i++)
    {
        scanf("%d", &Available[i]);
    }
    printf("\n");
}

// 接收资源请求
void getRequirement()
{
    printf("请输入申请请求的进程号：");
    // int pnum=0;
    scanf("%d", &requireNum);
    Safety[0] = requireNum;
    for (int i = 0; i < resourceNum; i++)
    {
        printf("%d类资源需要：\n", i + 1);
        // int num=0;
        scanf("%d", &request[i]);
        if (request[i] > Need[requireNum - 1][i])
        {
            printf("\n出错了，申请资源数超过当前所需！重新申请。\n");
            getRequirement();
        }
        if (request[i] > Available[i])
        {
            printf("进程%d已被阻塞", requireNum);
        }
        else
        {
            // 拷贝到工作矩阵，方便后面恢复状态
            // WorkStatus[i]=Available[i];
            // 试分配给进程requireNum资源
            // WorkStatus[i] -= request[i];
            Available[i] -= request[i];
            Allocation[requireNum - 1][i] += request[i];
            Need[requireNum - 1][i] -= request[i];
            Finished[requireNum - 1] = true;
        }
    }
}

bool is_safe() // 安全性判断
{
    int i, j, i1 = 0;
    bool t = false;

    for (i = 0; i < resourceNum; i++) // 把Available赋值给Work
    {
        WorkStatus[i] = Available[i];
    }

    for (i = 0; i < processNum; i++) // 初始化所有进程未完成
    {
        Finished[i] = false;
    }

    while (1)
    {
        for (j = 0; j < processNum; j++) // 寻找Need小于等于Work的进程
        {
            if (Finished[j])
                continue;
            t = true;
            for (i = 0; i < resourceNum; i++)
            {
                if (Need[j][i] <= WorkStatus[i])
                    continue;
                else
                {
                    t = false;
                    break;
                }
            }
            if (t)
                break; // 判断第j个进程是否满足Need小于等于Work
        }

        if (t && (Finished[j] == false)) // 是否找到Need小于等于Work的进程并且进程未完成
        {
            for (i = 0; i < resourceNum; i++)
            {
                WorkStatus[i] = WorkStatus[i] + Allocation[j][i];
            }
            Finished[j] = true; // 进程完成
            Safety[i1] = j+1; // 安全序列
            i1++;
        }
        else
        {
            for (i = 0; i < processNum; i++)
            {
                if (Finished[i])
                    continue;
                else
                    return false;
            }
            return true;
        }
    }
    return true;
}

int main()
{
    // 初始化
    printf("请输入进程总数和资源类的个数：\n");
    while (1)
    {
        scanf("%d %d", &processNum, &resourceNum);
        // 针对本次实验的参数要求验证
        if (processNum < 5)
        {
            printf("进程个数至少为5，请重新输入\n");
            continue;
        }
        if (resourceNum < 3)
        {
            printf("资源类别至少为3，请重新输入\n");
            continue;
        }
        break;
    }
    // 获取最大需求矩阵 Max
    getMax();
    // 获取分配矩阵 Allocation
    getAllocation();
    // 计算需求矩阵 Need
    getNeed();
    // 获取当前可用资源
    getAvailable();
    // 申请前检查一次
    // if (!VerifySafety())
    if(!is_safe())
    {
        printf("当前系统不安全，暂时无法申请资源！");
    }
    else
    {
        // 获取请求信息
        getRequirement();
        // 检查分配后的安全性
        // if (!VerifySafety())
        if(!is_safe())
        {
            printf("分配后系统不安全，不予分配，已恢复原状态！");
        }
        else
        {
            printf("申请有效，给予资源分配");
            printf("\n一个可行的安全序列为：\n");
            for (int i = 0; i < processNum; i++)
            {
                printf("%d ", Safety[i]);
            }
        }
    }
    return 0;
}