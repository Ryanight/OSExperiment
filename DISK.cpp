//
//  main.cpp
//  DISK
//
//  Created by Ryan on 2017/12/29.
//  Copyright © 2017年 Ryan. All rights reserved.
//

#include <iostream>
#include<stdio.h>
#include<algorithm>
#include<math.h>
#include<cstdlib>
#include<unistd.h>
#include<iostream>
#include <sstream>
#include <fstream>
#include <string>
using namespace std;

#define getspace(type) (type*)malloc(sizeof(type)) //·ÖÅä¿Õ¼ä
//定义磁盘号的范围（0 <= 磁盘号 < MAX_DISK_NUMBER)
const int MAX_DISK = 200;
//定义一个数组存储要访问的磁盘的序列号
int Disk[MAX_DISK + 1];
//DISK_NUMBER要访问的磁道的数量，
int DISK_NUMBER = 0;
//保存磁道的访问方向 DIRECTION 为 1 时从小到大， 为 0 时 从大到小；
int DIRECTION = 1;
//定义当前磁道号
int CURRENT_DISK = 0;
int timer = 0;
int nowpoint;
char flash[300];
char frontflash[300];
char null[300];
char point[300];
int finish = 0;
int count1 = 0;

#define random(a,b) (rand()%(b-a+1)+a);


typedef struct QNode {
    int id;
    int arrivetime;
    struct QNode *next;
}*QueuePtr;




struct LinkQueue
{
    QueuePtr front, rear; // 队头、队尾指针
};


LinkQueue q1;

void InitQueue(LinkQueue &Q)
{ // 构造一个空队列Q。
    Q.front = Q.rear = (QueuePtr)malloc(sizeof(QNode)); // 生成头结点
    if (!Q.front) // 生成头结点失败
        exit(OVERFLOW);
    Q.front->next = NULL; // 头结点的next域为空
}

void DestroyQueue(LinkQueue &Q)
{ // 销毁队列Q(无论空否均可)。
    while (Q.front) // Q.front不为空
    {
        Q.rear = Q.front->next; // Q.rear指向Q.front的下一个结点
        free(Q.front); // 释放Q.front所指结点
        Q.front = Q.rear; // Q.front指向Q.front的下一个结点
    }
}

void ClearQueue(LinkQueue &Q)
{ // 将队列Q清为空队列
    DestroyQueue(Q); // 销毁队列Q
    InitQueue(Q); // 重新构造空队列Q
}

bool QueueEmpty(LinkQueue Q)
{ // 若队列Q为空队列，则返回TRUE；否则返回FALSE
    if (Q.front->next == NULL)
        return true;
    else
        return false;
}

int QueueLength(LinkQueue Q)
{ // 求队列Q的长度
    int i = 0; // 计数器，初值为0
    QueuePtr p = Q.front; // p指向头结点
    while (Q.rear != p) // p所指不是尾结点
    {
        i++; // 计数器+1
        p = p->next; // p指向下一个结点
    }
    return i;
}

void GetHead(LinkQueue Q, QueuePtr e)
{ // 若队列Q不空，则用e返回Q的队头元素
    QueuePtr p;
    p = Q.front->next; // p指向队头结点
    e = p; // 将队头元素的值赋给e
}

void EnQueue(LinkQueue &Q, QueuePtr e)
{ // 插入元素e为队列Q的新的队尾元素。
    QueuePtr p;
    p = (QueuePtr)malloc(sizeof(QNode)); // 动态生成新结点
    if (!p)
        exit(OVERFLOW); // 失败则退出
    p = e; // 将值e赋给新结点
    p->next = NULL; // 新结点的指针域为空
    Q.rear->next = p; // 原队尾结点的指针指向新结点
    Q.rear = p; // 尾指针指向新结点
}

void DeQueue(LinkQueue &Q, QueuePtr &e)
{ // 若队列Q不空，删除Q的队头元素，用e返回其值，
    // 并返回OK；否则返回ERROR。
    QueuePtr p;
    p = Q.front->next; // p指向队头结点
    // 将队头元素的值赋给e
    e->id = p->id;
    e->arrivetime = p->arrivetime;
    Q.front->next = p->next; // 头结点指向下一个结点
    if (Q.rear == p) // 删除的是队尾结点
        Q.rear = Q.front; // 修改队尾指针指向头结点(空队列)
    free(p); // 释放队头结点
}



void Timer() {
    usleep(20000);  //0.002s
    timer++;
    /*
     int q = 0;
     while (q <= count1 &&q1.front->next != NULL) {
     Timer();
     QueuePtr e = new QNode;
     DeQueue(q1, e);
     if (e->arrivetime >= timer) {
     Disk[DISK_NUMBER] = e->id;
     DISK_NUMBER++;
     }
     else {
     EnQueue(q1, e);
     }
     q++;
     }*/
}

//比较大小的方法，用于对 Disk[] 的排序
int compare(int a, int b) {
    return a > b;
}

void reset() {
    
    for (int i = 0; i < 300; i++) {
        flash[i] = null[i];
        frontflash[i] = null[i];
        point[i/10] = ' ';
    }
}


//先来先服务（FIFO）
void FIFO() {
    double length = 0, avg_length = 0;
    nowpoint = DISK_NUMBER;
    for (int j = 0; j < Disk[nowpoint]; j++) {
        flash[j/ 10] = ' ';
    }
    cout << flash << Disk[nowpoint] << endl;
    reset();
    length += abs(Disk[DISK_NUMBER] - Disk[0]);
    //从当前位置到第一个
    while(Disk[nowpoint] != Disk[0]){
        Timer();
        if (Disk[nowpoint] > Disk[0]) {
            //left
            Disk[nowpoint] = Disk[nowpoint] - 1;
            if (Disk[nowpoint] % 10 == 0) {
                point[Disk[nowpoint] / 10] = '/';
                cout << point << endl;
                reset();
            }
        }
        else {
            //right
            Disk[nowpoint] = Disk[nowpoint] + 1;
            if (Disk[nowpoint] % 10 == 0) {
                point[Disk[nowpoint] / 10] = '\\';
                cout << point << endl;
                reset();
            }
        }
    }
    if (flash[Disk[nowpoint] + 1] == ' ') {
        for (int j = 0; j < Disk[nowpoint]; j++) {
            flash[j / 10] = ' ';
        }
        cout << flash << Disk[nowpoint] << endl;
    }
    else {
        
        for (int j = 0; j < Disk[nowpoint]; j++) {
            flash[j / 10] = ' ';
        }
        cout << flash << Disk[nowpoint] << endl;
    }
    reset();
    nowpoint = 0;
    //printf("%d", Disk[0]);
    
    //按顺序依次访问的总时间
    for (int i = 1; i < DISK_NUMBER; ++i)
    {
        length += abs(Disk[i] - Disk[i - 1]);
        while (Disk[nowpoint] != Disk[i]) {
            Timer();
            if (Disk[nowpoint] > Disk[i]) {
                //left
                Disk[nowpoint] = Disk[nowpoint] - 1;
                if (Disk[nowpoint] % 10 == 0) {
                    point[Disk[nowpoint]/ 10] = '/';
                    cout << point << endl;
                    reset();
                }
            }
            else {
                //right
                Disk[nowpoint] = Disk[nowpoint] + 1;
                if (Disk[nowpoint] % 10 == 0) {
                    point[Disk[nowpoint]/ 10] = '\\';
                    cout << point << endl;
                    reset();
                }
            }
        }
        
        if (flash[Disk[nowpoint] + 1] == ' ') {
            for (int j = 0; j < Disk[nowpoint]; j++) {
                flash[j / 10] = ' ';
            }
            cout << flash << Disk[nowpoint] << endl;
        }
        else {
            
            for (int j = 0; j < Disk[nowpoint]; j++) {
                flash[j / 10] = ' ';
            }
            cout <<flash << Disk[nowpoint]<< endl;
        }
        reset();
        nowpoint = i;
        //printf(" -> %d ", Disk[i]);
    }
    avg_length = 0.002 * timer / DISK_NUMBER;
    printf("平均寻道时间为：%lf  s\n", avg_length);
}

//最短寻道时间优先（SSTF）
//将当前的磁道号放入要访问的磁道号 Disk[] 中，排序后便可轻易找到其左右的要访问的磁道号
void SSTF() {
    //定义三个变量，left标记 当前磁道 左边第一个未调度的磁道号在数组中的序号
    //right 标记 当前磁道 右边第一个未调度的磁道号在数组中的序号
    //now 当前磁道；
    int left, right, now;
    //定义寻道长度和平均寻道长度
    double length = 0, avg_length = 0;
    //根据磁道移动的方向不同对磁道号进行排序
    if (DIRECTION == 1)
        sort(Disk, Disk + DISK_NUMBER + 1);
    else
        sort(Disk, Disk + DISK_NUMBER + 1, compare);
    
    for (int i = 0; i <= DISK_NUMBER; i++)
    {
        //找到当前磁道 左右边第一个未调度的磁道号在数组中的序号
        if (Disk[i] == CURRENT_DISK)
        {
            now = i;
            left = i - 1;
            right = i + 1;
        }
    }
    
    for (int j = 0; j < Disk[now]; j++) {
        flash[j / 10] = ' ';
    }
    cout << flash << Disk[now] << endl;
    reset();
    
    //不断寻找距离 now 近的并输出，相应改变 now left right
    while (!(left == -1 && right == DISK_NUMBER + 1))
    {
        //如果左侧到了 Disk[] 外则把右边的顺序输出
        if (left <= -1)
        {
            for (int i = right; i <= DISK_NUMBER; i++) {
                while (Disk[now] != Disk[right]) {
                    Timer();
                    
                    Disk[now] = Disk[now] + 1;
                    if (Disk[now] % 10 == 0) {
                        point[Disk[now] / 10] = '\\';
                        cout << point << endl;
                        reset();
                    }
                }
                if (flash[Disk[now] + 1] == ' ') {
                    for (int j = 0; j < Disk[now]; j++) {
                        flash[j / 10] = ' ';
                    }
                    cout << flash << Disk[now] << endl;
                }
                else {
                    
                    for (int j = 0; j < Disk[now]; j++) {
                        flash[j / 10] = ' ';
                    }
                    cout << flash << Disk[now] << endl;
                }
                reset();
                now = right;
                right++;
            }
            break;
        }
        //如果右侧到了 Disk[] 外则把左边的顺序输出
        if (right == DISK_NUMBER + 1)
        {
            for (int i = left; i >= 0; i--)
            {
                length += abs(Disk[now] - Disk[left]);
                while (Disk[now] != Disk[left]) {
                    Timer();
                    
                    Disk[now] = Disk[now] - 1;
                    if (Disk[now] % 10 == 0) {
                        point[Disk[now] / 10] = '/';
                        cout << point << endl;
                        reset();
                    }
                }
                if (flash[Disk[now] + 1] == ' ') {
                    for (int j = 0; j < Disk[now]; j++) {
                        flash[j / 10] = ' ';
                    }
                    cout << flash << Disk[now] << endl;
                }
                else {
                    
                    for (int j = 0; j < Disk[now]; j++) {
                        flash[j / 10] = ' ';
                    }
                    cout << flash << Disk[now] << endl;
                }
                reset();
                now = left;
                left--;
            }
            break;
        }
        //right 距离 <= left 向右走（不管方向为什么，都是往右走，因为方向不同时的排序方法不同，
        //根据方向区分左右距离相等时向左还是向右的话，效果与排序叠加，恰好得到了错误的序列）
        if (abs(Disk[right] - Disk[now]) <= abs(Disk[now] - Disk[left]))
        {
            while (Disk[now] != Disk[right]) {
                Timer();
                
                Disk[now] = Disk[now] + 1;
                if (Disk[now] % 10 == 0) {
                    point[Disk[now] / 10] = '\\';
                    cout << point << endl;
                    reset();
                }
            }
            for (int j = 0; j < Disk[now]; j++) {
                flash[j / 10] = ' ';
            }
            cout << flash << Disk[now] << endl;
            
            reset();
            now = right;
            right++;
        }
        else if (abs(Disk[right] - Disk[now]) > abs(Disk[now] - Disk[left]))
        {
            while (Disk[now] != Disk[left]) {
                Timer();
                
                Disk[now] = Disk[now] -1;
                if (Disk[now] % 10 == 0) {
                    point[Disk[now] / 10] = '/';
                    cout << point << endl;
                    reset();
                }
            }
            
            
            for (int j = 0; j < Disk[now]; j++) {
                flash[j / 10] = ' ';
            }
            cout << flash << Disk[now] << endl;
            reset();
            now = left;
            left--;
        }
    }
    //计算平均寻道时间
    avg_length = 0.002*timer / DISK_NUMBER;
    printf("平均寻道时间为：%lf s", avg_length);
}

//扫描算法（SCAN）
void SCAN()
{
    //定义三个变量，left标记 当前磁道 左边第一个微调度的磁道号在数组中的序号
    //right 标记 当前磁道 右边第一个未调度的磁道号在数组中的序号
    //now 当前磁道；
    int left = -1;
    int right = 1;
    int now = 0;
    //定义寻道长度和平均寻道长度
    double length = 0, avg_length = 0;
    //根据磁道移动的方向不同对磁道号进行排序
    if (DIRECTION == 1)
        sort(Disk, Disk + DISK_NUMBER + 1);
    else
        sort(Disk, Disk + DISK_NUMBER + 1, compare);
    
    //    //输出排序后的Disk[]
    //    for(int i = 0; i <= DISK_NUMBER; i++)
    //        printf("%d ",Disk[i]);
    for (int i = 0; i <= DISK_NUMBER; i++)
    {
        //找到当前磁道 左右边第一个未调度的磁道号在数组中的序号
        if (Disk[i] == CURRENT_DISK)
        {
            now = i;
            left = i - 1;
            right = i + 1;
        }
    }
    for (int j = 0; j < Disk[now]; j++) {
        flash[j / 10] = ' ';
    }
    cout << flash << Disk[now] << endl;
    reset();
    length += abs(Disk[now] - Disk[right]);
    //从now向DIRECTION方向移动
    for (int i = right; i <= DISK_NUMBER; i++)
    {
        finish++;
        while (Disk[now] != Disk[right]) {
            Timer();
            
            Disk[now] = Disk[now] + 1;
            if (Disk[now] % 10 == 0) {
                point[Disk[now] / 10] = '\\';
                cout << point << endl;
                reset();
            }
        }
        for (int j = 0; j < Disk[now]; j++) {
            flash[j / 10] = ' ';
        }
        cout << flash << Disk[now] << endl;
        
        reset();
        now = right;
        right++;
    }
    length += abs(Disk[DISK_NUMBER] - Disk[left]);
    //走到DIRECTION方向尽头，返回扫描
    for (int i = left; i >= 0; i--)
    {
        finish++;
        while (Disk[now] != Disk[left]) {
            Timer();
            
            Disk[now] = Disk[now] - 1;
            if (Disk[now] % 10 == 0) {
                point[Disk[now] / 10] = '/';
                cout << point << endl;
                reset();
            }
        }
        
        
        for (int j = 0; j < Disk[now]; j++) {
            flash[j / 10] = ' ';
        }
        cout << flash << Disk[now] << endl;
        reset();
        now = left;
        left--;
    }
    //计算平均寻道时间
    avg_length = 0.02*timer / DISK_NUMBER;
    printf("平均寻道时间为：%lf", avg_length);
}

int main()
{
    /*
     InitQueue(q1);
     int q = 0;
     
     
     std::ifstream fin("C:\\task9.txt", std::ios::in);
     char line[1024] = { 0 };
     int x = 0;
     int y = 0;
     while (fin.getline(line, sizeof(line)))
     {
     std::stringstream word(line);
     word >> x;
     word >> y;
     
     QueuePtr e = new QNode;
     e->id = x;
     e->arrivetime = y;
     EnQueue(q1, e);
     count1++;
     }
     fin.clear();
     fin.close();
     */
    cout << "input the number of the disk\n";
    cin >> DISK_NUMBER;
    cout << endl;
    cout << "the Disk[] is \n";
    for (int i = 0; i < DISK_NUMBER; i++) {
        Disk[i] = random(0, 200);
        cout << Disk[i] << "->";
    }
    cout << endl;
    cout << "input the direction of the disk , right = 1 and left = 0\n";
    cin >> DIRECTION;
    cout << endl;
    cout << "input the current disk\n";
    cin >> CURRENT_DISK;
    //°ÑCURRENT_DISK¼ÓÈëDisk[]
    Disk[DISK_NUMBER] = CURRENT_DISK;
    SCAN();
    /*
     QueuePtr e = new QNode;
     e = q1.front->next;
     while (e != NULL) {
     cout << e->id << "->";
     e = e->next;
     }
     cout << endl;
     
     e = q1.front;
     */
    
    
    /*
     while (finish < count1) {
     Timer();
     if (DISK_NUMBER > 0) {
     
     SCAN();
     for (int i = 0; i <= finish; i++) {
     Disk[i] = 0;
     }
     }
     
     q = 0;
     }
     //°ÑCURRENT_DISK¼ÓÈëDisk[]
     //Disk[DISK_NUMBER] = CURRENT_DISK;
     //FIFO();
     
     
     //SSTF();
     /*printf("\nthe answer of the SCAN is:\n");
     SCAN();
     */
    system("pause");
    return 0;
}
