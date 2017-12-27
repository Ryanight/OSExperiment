#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
using namespace std;
#define getspace(type) (type*)malloc(sizeof(type)) //声明空间

typedef struct node {
	int start;
	int length;
	char tag;
	node *next;
	node *last;
}job;

//worst-fit
void insertjob3(job *head, job *jobhead, int length, char name) {
	cout << "add task " << name << "----------------------------" << "\n" << endl;

	job *temp;
	job *temp1;
	temp = head->next;
	temp1 = jobhead;
	job *max = getspace(job);
	max->length = 0;
	while (temp != NULL) {
		if (max->length < temp->length&& temp->length>length) {
			max = temp;
		}
		else if (temp->length - length == 0) {
			max->start = temp->start;
			max->length = temp->length;
			break;
		}
		temp = temp->next;
	}
	if (max->length == 0) {
		cout << "Error.Waiting for recycling." << "\n" << endl;
	}
	else if (max->length == length) {
		max->start = max->start + length;
		if (temp->next != NULL) {
			temp->last->next = temp->next;
			temp->next->last = temp->last;
		}
		else {
			temp->last->next = NULL;
		}
		free(temp);
	}
	else {
		max->start = max->start + length;
		max->length = max->length - length;
		max->tag = '1';
	}
	if (temp1 != NULL) {
		while (temp1->next != NULL) {
			temp1 = temp1->next;
		}
	}
	job* addjob;
	addjob = getspace(job);
	addjob->start = max->start - length;
	addjob->length = length;
	addjob->tag = name;
	addjob->next = NULL;
	addjob->last = temp1;
	temp1->next = addjob;

}

//best-fit
void insertjob2(job *head, job *jobhead, int length, char name) {
	cout << "add task " << name << "----------------------------" << "\n" << endl;

	job *temp;
	job *temp1;
	temp = head->next;
	temp1 = jobhead;
	job *min = getspace(job);
	min->length = 1000000;
	while (temp != NULL) {
		if (min->length - length > temp->length - length && temp->length>length) {
			min = temp;
		}
		else if (temp->length - length == 0) {
			min->start = temp->start;
			min->length = temp->length;
			break;
		}
		temp = temp->next;
	}
	if (min->length == 1000000) {
		cout << "Error.Waiting for recycling." << "\n" << endl;
	}
	else if (min->length == length) {
		min->start = min->start + length;
		if (temp->next != NULL) {
			temp->last->next = temp->next;
			temp->next->last = temp->last;
		}
		else {
			temp->last->next = NULL;
		}
		free(temp);
	}
	else {
		min->start = min->start + length;
		min->length = min->length - length;
		min->tag = '1';
	}
	if (temp1 != NULL) {
		while (temp1->next != NULL) {
			temp1 = temp1->next;
		}
	}
	job* addjob;
	addjob = getspace(job);
	addjob->start = min->start - length;
	addjob->length = length;
	addjob->tag = name;
	addjob->next = NULL;
	addjob->last = temp1;
	temp1->next = addjob;

}


//first-fit
void insertjob1(job *head, job *jobhead, int length, char name) {
	cout << "add task " << name << "----------------------------" << "\n" << endl;
	int i = 0;
	int flag = 0;
	job *temp;
	job *temp1;
	temp = head->next;
	temp1 = jobhead;
	while (temp != NULL) {
		if (temp->length > length) {
			temp->length = temp->length - length;
			temp->start = temp->start + length;
			temp->tag = '1';
			flag = 1;
			if (temp1 != NULL) {
				while (temp1->next != NULL) {
					temp1 = temp1->next;
				}
			}
			job* addjob;
			addjob = getspace(job);
			addjob->start = temp->start - length;
			addjob->length = length;
			addjob->tag = name;
			addjob->next = NULL;
			addjob->last = temp1;
			temp1->next = addjob;
			break;
		}
		if (temp->length == length) {
			flag = 1;

			if (temp1 != NULL) {
				while (temp1->next != NULL) {
					temp1 = temp1->next;
				}
			}
			job* addjob;
			addjob = getspace(job);
			addjob->start = temp->start;
			addjob->length = length;
			addjob->tag = name;
			addjob->next = NULL;
			addjob->last = temp1;
			temp1->next = addjob;
			if (temp->next != NULL) {
				temp->last->next = temp->next;
				temp->next->last = temp->last;
			}
			else {
				temp->last->next = NULL;
			}
			free(temp);
			break;
		}
		temp = temp->next;
	}
	if (flag == 0) {
		cout << "Error. Waiting for recycling." << "\n" << endl;
	}
}

int deletejob(job *head, job *jobhead, char name, int count[], int count1[]) {
	cout << "release task " << name << "----------------------------" << "\n" << endl;
	job *temp;
	int countnum = -1;
	temp = jobhead->next;
	job *temp1;
	temp1 = head;
	int flag = 0;
	while (temp != NULL) {

		if (temp->tag == name) {
			while (temp1->next->start <temp->start) {
				temp1 = temp1->next;
				countnum++;
			}
			if (temp1->start + temp1->length == temp->start && temp->start + temp->length != temp1->next->start) {
				temp1->length += temp->length;
				if (temp1->start == count[countnum] && temp1->length == count1[countnum]) {
					temp1->tag = '0';
				}
				else {
					temp1->tag = '1';
				}
			}
			else if (temp->start + temp->length == temp1->next->start && temp1->start + temp1->length != temp->start) {
				temp1->next->length += temp->length;
				temp1->next->start = temp->start;
				if (temp1->next->start == count[countnum + 1] && temp1->next->length == count1[countnum + 1]) {
					temp1->next->tag = '0';
				}
				else {
					temp1->next->tag = '1';
				}
			}
			else if (temp->start + temp->length == temp1->next->start && temp1->start + temp1->length == temp->start) {
				job *freenode = temp1->next;
				temp1->length = temp->length + temp1->next->length + temp1->length;
				if (temp1->next == NULL) {
					temp1->last->next = NULL;
				}
				else if (temp1->next->next == NULL) {
					temp1->next = NULL;
				}
				else {
					temp1->next = temp1->next->next;
					temp1->next->next->last = temp1;
				}

				free(freenode);
				temp1->tag = '0';

			}
			else {
				job *addnode;
				addnode = getspace(job);
				addnode->length = temp->length;
				addnode->start = temp->start;
				addnode->tag = '0';
				addnode->last = temp1;
				addnode->next = temp1->next;
				temp1->next = addnode;

			}
			if (temp->next != NULL) {
				temp->last->next = temp->next;
				temp->next->last = temp->last;
			}
			else {
				temp->last->next = NULL;
			}
			free(temp);
			temp->last = NULL;
			flag = 1;
			return 0;
		}
		temp = temp->next;
	}
	if (flag == 0) {
		cout << "Error. Can't find job " << name << "\n" << endl;
	}
	return 0;
}

void display(job *head) {
	job *temp;
	temp = head->next;
	cout << "   " << "start" << " " << "length" << "    " << "tag" << "\n" << endl;
	while (temp != NULL) {
		cout << "   " << temp->start << " " << temp->length << "    " << temp->tag << "\n" << endl;
		temp = temp->next;
	}
}

int main() {
	job *head;
	job *rear;
	job *jobhead;
	job *jobrear;
	int count[20];
	int count1[20];
	head = getspace(job);
	jobhead = getspace(job);
	jobrear = getspace(job);
	jobhead->next = jobrear;
	jobrear->last = jobhead;
	jobrear->next = NULL;
	rear = head;
	std::ifstream fin("/Users/Ryan/Documents/C++ Project/data.txt", std::ios::in);
	char line[1024] = { 0 };
	int x = 0;
	int y = 0;
	int i = 0;
	while (fin.getline(line, sizeof(line)))
	{
		job *temp;
		temp = getspace(job);

		std::stringstream word(line);
		word >> x;
		word >> y;
		count[i] = x;
		count1[i] = y;
		i++;
		temp->start = x;
		temp->length = y;
		temp->tag = '0';
		rear->next = temp;
		temp->next = NULL;
		temp->last = rear;
		rear = temp;

	}
	fin.clear();
	fin.close();
	display(head);
	insertjob3(head, jobhead, 5, 'A');
	display(head);
	insertjob3(head, jobhead, 10, 'B');
	display(head);
	insertjob3(head, jobhead, 3, 'C');
	display(head);
	deletejob(head, jobhead, 'A', count, count1);
	display(head);
	deletejob(head, jobhead, 'B', count, count1);
	display(head);
	deletejob(head, jobhead, 'C', count, count1);
	display(head);
	getchar();
	return 0;
}


