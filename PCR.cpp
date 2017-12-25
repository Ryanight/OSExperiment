#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include <iostream>
#include <string>

#define TRUE 1
#define FALSE 0
#define OK 1

using namespace std;

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

PCB *head=(node *)malloc(sizeof(node));		//声明头程序空间

int n;
void create(int n)
{
	int i=1;	
    head->next=NULL;
	node *q=head;
	cout<<"    任务     优先级    NeedTime   State "<<endl;
    while(i<=n)
	{
	    node *p=(node *)malloc(sizeof(node));
		p->ID=i;
		p->cputime=0;
		p->state=0;
		p->prio=rand()%10+1;
		p->needtime=rand()%8+1;
		cout<<"     "<<p->ID<<"        "<<p->prio<<"            "<<p->needtime<<"         "<<p->state<<endl;
		p->next=NULL;
		q->next=p;
		q=q->next;
		i++;
	}
}

void FCFS()
{
	int time = 0;
	node *p = head->next;
	while(p!=NULL)
	{
		cout<<"执行进程"<<p->ID<<"     ";
		cout<<"耗费时间"<<p->needtime<<endl;
		p->cputime = p->needtime;	//程序运行时间等于需要时间
		time += p->needtime;		//将任务耗时加入总时间
		p->state = 1;
		p = p->next;	//切换到下一个任务
	}
	cout<<endl;
	cout<<"所有进程都执行完成"<<endl;
	cout<<"消耗的总时间为"<<time<<endl;
}

void RR(int timeslice)
{
	int time = 0;
	node *p = head->next;
	while(p != NULL)
	{
		p->state = 0;
		p->cputime = 0;
		p = p->next;
	}
Loop:
	while(p != NULL)
	{
		if (p-> state != 1)
		{
			cout<<"执行进程"<<p->ID <<"    ";
			if ((p->needtime - p->cputime)<timeslice)
			{
				cout<<"运行时间"<<(p->needtime - p->cputime)<<"    ";
				time += (p->needtime - p->cputime);
				p->cputime += (p->needtime - p->cputime);
			}
			else
			{
				cout<<"运行时间"<<timeslice<<"    ";
				p->cputime += timeslice;
				time += timeslice;
			}
			cout<<"剩余时间"<<(p->needtime - p->cputime)<<endl;
			if (p->cputime == p->needtime)
			{
				p->state = 1;
			}
			p = p->next;
		}
		else
			p = p->next;

	}
	p = head->next;
	while(p != NULL)
	{
		if (p->state == 1)
			p = p->next;
		else{
			p = head->next;
			goto Loop;
		}
	}
	cout<<endl;
	cout<<"所有进程都执行完成"<<endl;
	cout<<"消耗的总时间为"<<time<<endl;
}


int main()
{
	cout<<"请输入系统进程数： ";
	cin>>n;
	int m=n;
	int timeslice = 2;
	node *p = head->next;
	if(n==0)
    cout<<"此时没有就绪进程"<<endl;
	else
	{
		create(n);
		cout<<endl;
		cout<<"先来先服务调度"<<endl;
		FCFS();
		cout<<endl;
		cout<<"时间片轮转"<<endl;
		RR(timeslice);
	}
}
