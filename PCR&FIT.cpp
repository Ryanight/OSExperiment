//
//  main.cpp
//  PCR
//
//  Created by Ryan on 2017/12/27.
//  Copyright © 2017年 Ryan. All rights reserved.
//


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <unistd.h>
#include <vector>
#include <list>

#define TRUE 1
#define FALSE 0
#define OK 1
#define Datatype int
#define MaxLength 255

using namespace std;

static int realtime = 0;
static int timeslice_origin = 2;
static int timeslice = timeslice_origin;
static int OccupySize;

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
    int memory_start;
    int memory_length;
    struct node *next;
}PCB;

static vector<int> MEMORY;
static vector<int> StartPoint;
static vector<int> Length;
static vector<int> Tag;

//定义事件节点
typedef struct QueueNode
{
    Datatype data;
    PCB *process;
    struct QueueNode *next;
}QueueNode;

typedef list<QueueNode> LISTQ;

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
    return (Q->front == NULL && Q->rear == NULL);
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
static LISTQ MemoryList;


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
    int w = 0;
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
        word>>w;
        p->process->ID = x;
        p->data = y;
        p->process->cputime = z;
        p->process->memory_length = w;
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

//获得空闲表
static void GetFreeChart()
{
    StartPoint.clear();
    Length.clear();
    Tag.clear();
    for (int i = 0; i < MEMORY.size(); i++) {
        if (MEMORY[i] == 0) {
            int count = 1;
            while (MEMORY[i+count] == 0) {
                count++;
            }
            StartPoint.push_back(i);
            Length.push_back(count);
            Tag.push_back(0);
            i = i + count - 1;
        }
    }
    
}

//内存剩余空间
static int MemoryLeft()
{
    int Space = 0;
    for (int i = 0; i < MEMORY.size(); i++) {
        if (MEMORY[i] == 0) {
            Space++;
        }
        else
            Space = Space;
    }
    return Space;
}


//初始化内存
static void InitMemory(int MemorySize, int OccupySize)
{
    MEMORY.resize(MemorySize);
    for (int i = 0; i < OccupySize; i++) {
        MEMORY[i] = 1;
    }
    for (int j = OccupySize; j < MemorySize; j++) {
        MEMORY[j] = 0;
    }
}

//程序进入内存
static void EnMemory(int i, QueueNode *qnode)
{
    qnode->process->memory_start = StartPoint[i];
    Tag[i] = 1;
    for (int j = StartPoint[i]; j < StartPoint[i] + qnode->process->memory_length; j++){
        MEMORY[j] = 1;
    }
    MemoryList.push_back(*qnode);
}

//程序弹出内存
static void DeMemory(QueueNode *qnode)
{
    LISTQ::iterator p = MemoryList.begin();
    for (int i = qnode->process->memory_start; i < qnode->process->memory_start + qnode->process->memory_length; i++) {
        MEMORY[i] = 0;
    }
    for (p = MemoryList.begin(); p != MemoryList.end(); p++) {
        if (p->process == qnode->process) {
            MemoryList.erase(p);
            cout<<"Process Pop Out of Memory."<<endl;
            break;
        }
    }
}

//压缩内存
static void CompressMemory()
{
    if (MemoryList.size() == 0) {
        cout<<"No Progress in the Memory."<<endl;
    }
    else{
        QueueNode q = MemoryList.front();
        QueueNode *p = &q;
        int start = p->process->memory_start;
        int length = p->process->memory_length;
        if (q.process->memory_start != OccupySize) {
            for (int k = p->process->memory_start; k < p->process->memory_start + p->process->memory_length; k++) {
                MEMORY[k] = 0;
            }
            p->process->memory_start = OccupySize;
            start = p->process->memory_start;
            for (int i = OccupySize; i < OccupySize + p->process->memory_length; i++) {
                MEMORY[i] = 1;
            }
            p = p->next;
        }
        else
            p = p->next;
        for (int j = 0; j < MemoryList.size() - 1; j++) {
            for (int k = p->process->memory_start; k < p->process->memory_start + p->process->memory_length; k++) {
                MEMORY[k] = 0;
            }
            p->process->memory_start = start + length;
            for (int i = start + length; i < start + length + p->process->memory_length; i++) {
                MEMORY[i] = 1;
            }
            length = p->process->memory_length;
            start = p->process->memory_start;
            p = p->next;
        }
        cout<<"Compress Complete."<<endl;
    }
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
            DeMemory(Finished.rear);
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
            DeMemory(Finished.rear);
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
        //瞬间完成
        if (Running.rear->process->needtime == 0)
        {
            EnLinkQueue(&Finished, Running.rear);
            Finished.rear->process->state = 3;
            Finished.rear->process->finishtime = realtime;
            DeMemory(Finished.rear);
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
            DeMemory(Finished.rear);
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



//最先适应分配算法
static int FirstFit(QueueNode *p)
{
ProcessADD:
    GetFreeChart();
    int count = 0;
    //    LISTINT::iterator i;
    //准备队列和运行队列被调度进了程序
    if (p != NULL) {
        for (int i = 0; i < Length.size(); i++) {
            //当空间足够而且没有被占用时，直接放入
            if (Length[i] >= p->process->memory_length && Tag[i] == 0) {
                EnMemory(i, p);
                //ExLinkQueue(&Ready, &Event);
                //p = p->next;
                return 1;
            }
            else
                count++;
        }
        //没有足够的空间
        if (count >= Length.size()) {
            //总空间不够
            if (MemoryLeft() < p->process->memory_length) {
                cout<<"Out of Memory Space. PID: "<<p->process->ID<<endl;
                //                DeLinkQueue(&Event);
                //                EnLinkQueue(&Event, p);
                //p = p->next;
                return 0;
            }
            //总空间够，但是有碎片
            else{
                cout<<"Memory Left: "<<MemoryLeft()<<endl;
                cout<<"Need Memory: "<<p->process->memory_length<<endl;
                cout<<"Enough Memory Space, But Can't Push in PID: "<<p->process->ID<<endl;
                CompressMemory();   //压缩空间
                goto ProcessADD;
            }
            
        }
    }
    return 0;
}

//最优适应分配方法
static int BestFit(QueueNode *p)
{

    int temp = MEMORY.size();
    int temp_i = Length.size();
Bestfit:
    GetFreeChart();
    int count = 0;
    if (p != NULL) {
        for (int i = 0; i < Length.size(); i++) {
            //当空间足够而且没有被占用时，直接放入
            if (Length[i] >= p->process->memory_length && Tag[i] == 0) {
                if (Length[i] - p->process->memory_length < temp) {
                    temp = Length[i] - p->process->memory_length;
                    temp_i = i;
                }
                //ExLinkQueue(&Ready, &Event);
                //p = p->next;
                //return 1;
            }
            else
                count++;
        }
        //没有足够的空间
        if (count >= Length.size()) {
            //总空间不够
            if (MemoryLeft() < p->process->memory_length) {
                cout<<"Out of Memory Space. PID: "<<p->process->ID<<endl;
                //                DeLinkQueue(&Event);
                //                EnLinkQueue(&Event, p);
                //p = p->next;
                return 0;
            }
            //总空间够，但是有碎片
            else{
                cout<<"Memory Left: "<<MemoryLeft()<<endl;
                cout<<"Need Memory: "<<p->process->memory_length<<endl;
                cout<<"Enough Memory Space, But Can't Push in PID: "<<p->process->ID<<endl;
                CompressMemory();   //压缩空间
                goto Bestfit;
            }
            
        }
        else{
            EnMemory(temp_i, p);
            return 1;
        }
    }
    return 0;
    
}

//最差适应分配方法
static int WorstFit(QueueNode *p)
{
    
    int temp = 0;
    int temp_i = 0;
Worstfit:
    GetFreeChart();
    int count = 0;
    if (p != NULL) {
        for (int i = 0; i < Length.size(); i++) {
            //当空间足够而且没有被占用时，直接放入
            if (Length[i] >= p->process->memory_length && Tag[i] == 0) {
                if (Length[i] - p->process->memory_length > temp) {
                    temp = Length[i] - p->process->memory_length;
                    temp_i = i;
                }
                //ExLinkQueue(&Ready, &Event);
                //p = p->next;
                //return 1;
            }
            else
                count++;
        }
        //没有足够的空间
        if (count >= Length.size()) {
            //总空间不够
            if (MemoryLeft() < p->process->memory_length) {
                cout<<"Out of Memory Space. PID: "<<p->process->ID<<endl;
                //                DeLinkQueue(&Event);
                //                EnLinkQueue(&Event, p);
                //p = p->next;
                return 0;
            }
            //总空间够，但是有碎片
            else{
                cout<<"Memory Left: "<<MemoryLeft()<<endl;
                cout<<"Need Memory: "<<p->process->memory_length<<endl;
                cout<<"Enough Memory Space, But Can't Push in PID: "<<p->process->ID<<endl;
                CompressMemory();   //压缩空间
                goto Worstfit;
            }
            
        }
        else{
            EnMemory(temp_i, p);
            return 1;
        }
    }
    return 0;
    
}


int main()
{
    // cout<<"请输入系统进程数： ";
    // cin>>n;
    // int m=n;
    // int timeslice = 2;
    // node *p = head->next;
    // if(n==0)
    //    cout<<"此时没有就绪进程"<<endl;
    // else
    // {
    // 	create(n);
    // 	cout<<endl;
    // 	cout<<"先来先服务调度"<<endl;
    // 	FCFS();
    // 	cout<<endl;
    // 	cout<<"时间片轮转"<<endl;
    // 	RR(timeslice);
    // }
    int MemorySize = 64;
    OccupySize = 12;
    InitMemory(MemorySize, OccupySize);
    GetFreeChart();
    string METHOD;
    cout<<"Input Method: ";
    cin>>METHOD;
    InitQueue(&Event);
    InitQueue(&Ready);
    InitQueue(&Finished);
    InitQueue(&Running);
    EventCreate();
    QueueNode *p = Event.front;
ADD:
    while(!QueueEmpty(&Event) || !QueueEmpty(&Ready) || !QueueEmpty(&Running))
    {
        p = Event.front;
        for (int i = 0; i < Event.count; i++){
            if (p != NULL && realtime >= p->data)
            {
                int Mem = FirstFit(p);
                if (Mem == 1) {
                    p = p->next;
                    ExLinkQueue(&Ready, &Event);
                    Ready.rear->process->state = 1;
                }
                else
                    p = p->next;
            }
            else if(p != NULL && p->next != NULL)
                p = p->next;
        }
        if (!QueueEmpty(&Ready) || !QueueEmpty(&Running))
        {
            if (METHOD == "FCFS")
            {
            FCFSAG:
                FCFS();
                p = Event.front;
                for (int i = 0; i < Event.count; i++){
                    if (p != NULL && realtime >= p->data)
                    {
                        int Mem = WorstFit(p);
                        if (Mem == 1) {
                            p = p->next;
                            ExLinkQueue(&Ready, &Event);
                            Ready.rear->process->state = 1;
                        }
                        else
                            p = p->next;
                    }
                    else if(p != NULL && p->next != NULL)
                        p = p->next;
                }
                p = Event.front;
                for (int i = 0; i < Event.count; i++){
                    if (p != NULL && realtime >= p->data && MemoryLeft() >= p->process->memory_length) {
                        goto FCFSAG;
                    }
                    else
                        p = p->next;
                }
                if (QueueEmpty(&Running) && !QueueEmpty(&Ready)) {
                    goto RRAG;
                }
                cout<<"time: "<<realtime<<endl;
                cout<<"Running ";
                ReadLinkQueue(&Running);
                cout<<"Ready ";
                ReadLinkQueue(&Ready);
                cout<<"Not Created ";
                ReadLinkQueue(&Event);
                cout<<"Finished ";
                ReadLinkQueue(&Finished);
                realtime++;
                goto ADD;
            }
            else if (METHOD == "RR")
            {
            RRAG:
                RR();
                p = Event.front;
                for (int i = 0; i < Event.count; i++){
                    if (p != NULL && realtime >= p->data)
                    {
                        int Mem = FirstFit(p);
                        if (Mem == 1) {
                            p = p->next;
                            ExLinkQueue(&Ready, &Event);
                            Ready.rear->process->state = 1;
                        }
                        else
                            p = p->next;
                    }
                    else if(p != NULL && p->next != NULL)
                        p = p->next;
                }
                p = Event.front;
                for (int i = 0; i < Event.count; i++){
                    if (p != NULL && realtime >= p->data && MemoryLeft() >= p->process->memory_length) {
                        goto FCFSAG;
                    }
                    else
                        p = p->next;
                }
                if (QueueEmpty(&Running) && !QueueEmpty(&Ready)) {
                    goto RRAG;
                }
                cout<<"time: "<<realtime<<endl;
                cout<<"Running ";
                ReadLinkQueue(&Running);
                cout<<"Ready ";
                ReadLinkQueue(&Ready);
                cout<<"Not Created ";
                ReadLinkQueue(&Event);
                cout<<"Finished ";
                ReadLinkQueue(&Finished);
                realtime++;
                goto ADD;
            }
            else
                cout<<"Wrong Method"<<endl;
        }
    }
    //getchar();
    return 0;
}


