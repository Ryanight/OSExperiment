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


#define TRUE 1
#define FALSE 0
#define OK 1
#define Datatype int



using namespace std;

char method;
int realtime = 0;
int timeslice_origin = 2;
int timeslice = timeslice_origin;
//定义进程节点
typedef struct node
{
    int ID;
    int prio;
    int round;
    int cputime;
    int needtime;
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
void InitQueue(LinkQueue *Q)
{
    Q->front = Q->rear = NULL;
}

//判断链队列是否为空
int QueueEmpty(LinkQueue *Q)
{
    return(Q->front == NULL && Q->rear == NULL);
}

// 事件入队
void EnLinkQueue(LinkQueue *Q, QueueNode *p)
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
}

//交换队列
void ExLinkQueue(LinkQueue *Q, LinkQueue *P)
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
    //free(p);
}
//出队
Datatype DeLinkQueue(LinkQueue *Q)
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
    return i;
    
}

//读取队列头元素，不改变队列状态
Datatype ReadLinkQueue(LinkQueue *Q)
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

// int main()
// {
//     LinkQueue Q;
//     Datatype i = 1;
//     InitQueue(&Q);
//     while(i<=6)
//     {
//         EnLinkQueue(&Q,i);      //将1-6入队
//         i++;
//     }

//     printf("DeLinkQueue: %d\n", DeLinkQueue(&Q));
//     printf("DeLinkQueue: %d\n", DeLinkQueue(&Q));
//     printf("ReadLinkQueue: %d\n", ReadLinkQueue(&Q));
//     printf("ReadLinkQueue: %d\n", ReadLinkQueue(&Q));

//     EnLinkQueue(&Q,9);
//     printf("The all number of the linkqueue:\n");
//     while(!QueueEmpty(&Q))
//         printf("%d\t",DeLinkQueue(&Q));         //输出队列中所有数据

//     return 0;
// }

QueueNode *QueueHead = (QueueNode *)malloc(sizeof(node));
PCB *head = (node *)malloc(sizeof(node));		//声明头程序空间
LinkQueue Ready;
LinkQueue Running;
LinkQueue Finished;
LinkQueue Event;


// int n;
void EventCreate()
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
    
    // FILE *fp;
    // int i, no;
    // if ((fp = fopen("/Users/Ryan/Documents/C++ Project/PCR.txt", "r")) == NULL)
    // {
    // 	printf("Can't open the file!\n");
    // 	exit(0);
    // }
    // no = fscanf(fp, "%c", &method);
    // no++;
    // no = fscanf(fp, "%d", &Event.count);
    // no++;
    // for (i = 0; no != NULL; i++)
    // {
    // 	QueueNode *p;
    // 	p = (QueueNode *)malloc(sizeof(QueueNode));
    // 	p->process = (node *)malloc(sizeof(node));
    // 	no = fscanf(fp, "%d %d %d", &p->process->ID, &p->data, &p->process->cputime);
    // 	no++;
    // 	p->process->needtime = p->process->cputime;
    // 	p->process->state = 0;
    // 	p->next = NULL;
    // 	q->next = p;
    // 	q = q->next;
    // 	EnLinkQueue(&Event, p);
    // }
    // fclose(fp);
    // int i=1;
    //    head->next=NULL;
    // node *q=head;
    // cout<<"    任务     优先级    NeedTime   State "<<endl;
    //    while(i<=n)
    // {
    //     node *p=(node *)malloc(sizeof(node));
    // 	p->ID=i;
    // 	p->cputime=0;
    // 	p->state=0;
    // 	p->prio=rand()%10+1;
    // 	p->needtime=rand()%8+1;
    // 	cout<<"     "<<p->ID<<"        "<<p->prio<<"            "<<p->needtime<<"         "<<p->state<<endl;
    // 	p->next=NULL;
    // 	q->next=p;
    // 	q=q->next;
    // 	i++;
    // }
}

//先来先服务
void FCFS()
{
    //int time = 0;
    // node *p = head->next;
    // while(p!=NULL)
    // {
    // 	cout<<"执行进程"<<p->ID<<"     ";
    // 	cout<<"耗费时间"<<p->needtime<<endl;
    // 	p->cputime = p->needtime;	//程序运行时间等于需要时间
    // 	time += p->needtime;		//将任务耗时加入总时间
    // 	p->state = 1;
    // 	p = p->next;	//切换到下一个任务
    // }
    // cout<<endl;
    // cout<<"所有进程都执行完成"<<endl;
    // cout<<"消耗的总时间为"<<time<<endl;
    if (QueueEmpty(&Running) && !QueueEmpty(&Ready))
    {
        ExLinkQueue(&Running, &Ready);
        //Running.front->process->needtime--;
        Running.front->process->state = 2;
        if (Running.front->process->needtime == 0)
        {
            EnLinkQueue(&Finished, Running.front);
            Finished.front->process->state = 3;
            DeLinkQueue(&Running);
            if (QueueEmpty(&Running) && !QueueEmpty(&Ready))
            {
                ExLinkQueue(&Running, &Ready);
                //Running.front->process->needtime--;
                Running.front->process->state = 2;
            }
        }
    }
    else if (!QueueEmpty(&Running))
    {
        Running.front->process->needtime--;
        if (Running.front->process->needtime == 0)
        {
            EnLinkQueue(&Finished, Running.front);
            Finished.front->process->state = 3;
            DeLinkQueue(&Running);
            if (QueueEmpty(&Running) && !QueueEmpty(&Ready))
            {
                ExLinkQueue(&Running, &Ready);
                //Running.front->process->needtime--;
                Running.front->process->state = 2;
            }
        }
    }
    
    else
        return;
}

//时间片轮转
void RR()
{
RR:
    if (QueueEmpty(&Running) && !QueueEmpty(&Ready))
    {
        ExLinkQueue(&Running, &Ready);
        Running.front->process->state = 2;
        if (Running.front->process->needtime == 0)
        {
            EnLinkQueue(&Finished, Running.front);
            Finished.front->process->state = 3;
            DeLinkQueue(&Running);
            if (QueueEmpty(&Running) && !QueueEmpty(&Ready))
            {
                ExLinkQueue(&Running, &Ready);
                //Running.front->process->needtime--;
                Running.front->process->state = 2;
            }
        }
    }
    else if (!QueueEmpty(&Running))
    {
        Running.front->process->needtime--;
        timeslice--;
        if (Running.front->process->needtime == 0)
        {
            EnLinkQueue(&Finished, Running.front);
            Finished.front->process->state = 3;
            DeLinkQueue(&Running);
            timeslice = timeslice_origin;
            if (QueueEmpty(&Running) && !QueueEmpty(&Ready))
            {
                ExLinkQueue(&Running, &Ready);
                //Running.front->process->needtime--;
                Running.front->process->state = 2;
            }
        }
        if (!QueueEmpty(&Running) && timeslice == 0)
        {
            ExLinkQueue(&Ready, &Running);
            Ready.front->process->state = 1;
            timeslice = timeslice_origin;
            goto RR;
        }

    }
//    int time = 0;
//    node *p = head->next;
//    //重置程序信息
//    while(p != NULL)
//    {
//        p->state = 0;
//        p->cputime = 0;
//        p = p->next;
//    }
//Loop:	//循环执行程序
//    while(p != NULL)
//    {
//        if (p-> state != 1)
//        {
//            cout<<"执行进程"<<p->ID <<"    ";
//            //如果程序完成需要时间小于时间片
//            if ((p->needtime - p->cputime) < timeslice)
//            {
//                cout<<"运行时间"<<(p->needtime - p->cputime)<<"    ";
//                time += (p->needtime - p->cputime);
//                p->cputime += (p->needtime - p->cputime);
//            }
//            //程序完成需要时间大于时间片
//            else
//            {
//                cout<<"运行时间"<<timeslice<<"    ";
//                p->cputime += timeslice;
//                time += timeslice;
//            }
//            cout<<"剩余时间"<<(p->needtime - p->cputime)<<endl;
//            //程序完成
//            if (p->cputime == p->needtime)
//            {
//                p->state = 1;
//            }
//            p = p->next;
//        }
//        else
//            //进行下个程序
//            p = p->next;
//        
//    }
//    //回到第一个程序
//    p = head->next;
//    //检查还有没有未完成的程序
//    while(p != NULL)
//    {
//        if (p->state == 1)
//            p = p->next;
//        else{
//            p = head->next;
//            goto Loop;
//        }
//    }
//    cout<<endl;
//    cout<<"所有进程都执行完成"<<endl;
//    cout<<"消耗的总时间为"<<time<<endl;
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
    string METHOD;
    METHOD = method;
    METHOD = "RR";
    InitQueue(&Event);
    InitQueue(&Ready);
    InitQueue(&Finished);
    InitQueue(&Running);
    EventCreate();
ADD:
    while(!QueueEmpty(&Event) || !QueueEmpty(&Ready) || !QueueEmpty(&Running))
    {
        if (Event.front != NULL && realtime == Event.front->data)
        {
            ExLinkQueue(&Ready, &Event);
            Ready.front->process->state = 1;
            goto ADD;
        }
        else
        {
            if (METHOD == "FCFS")
            {
                FCFS();
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
                RR();
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
