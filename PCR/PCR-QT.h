#ifndef PCR_H
#define PCR_H

#include <QMainWindow>
#include <QTimer>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <unistd.h>

namespace Ui {
class PCR;
}

class PCR : public QMainWindow
{
    Q_OBJECT

public:
    explicit PCR(QWidget *parent = 0);
    ~PCR();


private slots:

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void RefreshTable();

    void on_pushButton_3_clicked();

    void on_radioButton_2_clicked();

    void on_radioButton_clicked();

private:
    Ui::PCR *ui;
    QTimer *timer;
    QTableWidget *tableWidget;
    QTableWidget *tableWidget_2;
    QTableWidget *tableWidget_3;
    QTableWidget *tableWidget_4;
};

#endif // PCR_H

//
//  main.cpp
//  PCR
//
//  Created by Ryan on 2017/12/27.
//  Copyright © 2017年 Ryan. All rights reserved.
//



#define TRUE 1
#define FALSE 0
#define OK 1
#define Datatype int

using namespace std;

static int realtime = 0;
static int timeslice_origin = 2;
static int timeslice = timeslice_origin;
//定义进程节点
typedef struct node
{
    int ID;
    int prio;
    int round;
    int cputime;
    int needtime;
    int finishtime;
    int count;
    int state;
    struct node *next;
}PCB;

//定义事件节点
typedef struct QueueNode
{
    Datatype data;
    PCB *process;
    struct QueueNode *next;
}QueueNode;

//定义头节点
typedef struct{
    QueueNode *front;
    QueueNode *rear;
    int count;
}LinkQueue;

//初始化链队列，头节点置空
static void InitQueue(LinkQueue *Q)
{
    Q->front = Q->rear = NULL;
}

//判断链队列是否为空
static int QueueEmpty(LinkQueue *Q)
{
    return(Q->front == NULL && Q->rear == NULL);
}

// 事件入队
static void EnLinkQueue(LinkQueue *Q, QueueNode *p)
{
    // QueueNode *p;
    // p = (QueueNode *)malloc(sizeof(QueueNode));//为新的节点分配空间
    // p->data = v;
    p->next = NULL;
    if(QueueEmpty(Q))
        Q->front = Q->rear = p;
    else
    {
        Q->rear->next = p;  //将新的节点连接到队列
        Q->rear = p;             //指向队列尾
    }
    Q->count++;
}

//交换队列
static void ExLinkQueue(LinkQueue *Q, LinkQueue *P)
{
    QueueNode *p = P->front;
    if (QueueEmpty(Q))
    {
        Q->front = Q->rear = p;
    }
    else
    {
        Q->rear->next = p;
        Q->rear = p;
    }
    if(P->front == P->rear)   //判断队列是否只有一个节点
        P->front = P->rear = NULL;
    else
        P->front = p->next;
    p->next = NULL;
    Q->count++;
    P->count--;
    //free(p);
}
//出队
static Datatype DeLinkQueue(LinkQueue *Q)
{
    Datatype i;
    QueueNode *s;
    if(QueueEmpty(Q))     //判断队列是否为空
        printf("Error,the linkqueue is empty!");
    s = Q->front;
    i = s->data;
    if(Q->front == Q->rear)   //判断队列是否只有一个节点
        Q->front = Q->rear = NULL;
    else
        Q->front = s->next;
    //free(s);
    Q->count--;
    return i;

}

//读取队列头元素，不改变队列状态
static Datatype ReadLinkQueue(LinkQueue *Q)
{
    Datatype i = 0;
    QueueNode *p = Q->front;
    if(QueueEmpty(Q))     //判断队列是否为空
        cout<<"-> NULL";
    while (p != NULL)
    {
        i = p->data;
        cout<<"-> "<<p->process->ID;
        p = p->next;
    }
    cout<<endl;
    return i;
}

static QueueNode *QueueHead = (QueueNode *)malloc(sizeof(node));
static PCB *head = (node *)malloc(sizeof(node));		//声明头程序空间
static LinkQueue Ready;
static LinkQueue Running;
static LinkQueue Finished;
static LinkQueue Event;


// int n;
static void EventCreate()
{
    //std::ifstream fin("/Users/Ryan/Documents/C++ Project/data.txt", std::ios::in);
    QueueHead->next = NULL;
    QueueNode *q = QueueHead;
    std::ifstream fin1("/Users/Ryan/Documents/C++ Project/PCR.txt",std::ios::in);
    char line1[1024] = {0};
    int x = 0;
    int y = 0;
    int z = 0;
    while(fin1.getline(line1,sizeof(line1)))
    {
        QueueNode *p = nullptr;
        p = (QueueNode *)malloc(sizeof(QueueNode));
        node *pro = (node *)malloc(sizeof(node));
        p->process = pro;
        std::stringstream word(line1);
        word>>x;
        word>>y;
        word>>z;
        p->process->ID = x;
        p->data = y;
        p->process->cputime = z;
        p->process->needtime = p->process->cputime;
        p->process->state = 0;
        p->next = NULL;
        q->next = p;
        q = q->next;
        EnLinkQueue(&Event, p);
    }
    fin1.clear();
    fin1.close();
}

//先来先服务
static void FCFS()
{
FCFS:
    if (QueueEmpty(&Running) && !QueueEmpty(&Ready))
    {
        ExLinkQueue(&Running, &Ready);
        //Running.front->process->needtime--;
        Running.rear->process->state = 2;
        if (Running.rear->process->needtime == 0)
        {
            EnLinkQueue(&Finished, Running.rear);
            Finished.rear->process->state = 3;
            Finished.rear->process->finishtime = realtime;
            DeLinkQueue(&Running);
            goto FCFS;
        }
    }
    else if (!QueueEmpty(&Running))
    {
        Running.rear->process->needtime--;
        if (Running.rear->process->needtime == 0)
        {
            EnLinkQueue(&Finished, Running.front);
            Finished.rear->process->state = 3;
            Finished.rear->process->finishtime = realtime;
            DeLinkQueue(&Running);
            goto FCFS;
        }
    }

    else
        return;
}

//时间片轮转
static void RR()
{
RR:
    //添加进程
    if (QueueEmpty(&Running) && !QueueEmpty(&Ready))
    {
        ExLinkQueue(&Running, &Ready);
        Running.rear->process->state = 2;
        if (Running.rear->process->needtime == 0)
        {
            EnLinkQueue(&Finished, Running.rear);
            Finished.rear->process->state = 3;
            Finished.rear->process->finishtime = realtime;
            DeLinkQueue(&Running);
            goto RR;
        }
    }
    //运行
    else if (!QueueEmpty(&Running))
    {
        Running.front->process->needtime--;
        timeslice--;
        cout<<"using timeslice: "<<timeslice<<endl;
        //完成
        if (Running.front->process->needtime == 0)
        {
            EnLinkQueue(&Finished, Running.front);
            Finished.rear->process->state = 3;
            Finished.rear->process->finishtime = realtime;
            DeLinkQueue(&Running);
            timeslice = timeslice_origin;
            //添加新的进程
            goto RR;
        }
        //时间片耗尽
        if (!QueueEmpty(&Running) && timeslice == 0)
        {
            ExLinkQueue(&Ready, &Running);
            Ready.rear->process->state = 1;
            timeslice = timeslice_origin;
            goto RR;
        }
    }
    else
        return;
}


