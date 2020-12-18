#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<string.h>
#include<unistd.h>
#include"util.h"
typedef int bool;
#define true 1
#define false 0
struct Node{
	int data;
	struct Node *next;
}*head;
int num, pdim;
Point * pPoints, * b;

void add(int num) {
	struct Node * temp;
	temp = (struct Node * ) malloc(sizeof(struct Node));
	temp->data = num;
	temp->next = head;
	head = temp;
}

int delete(int num) {
	struct Node * temp, * prev;
	temp = head;
	while (temp != NULL) 
		if (temp->data == num) {
			if (temp == head) {
				head = temp->next;
				free(temp);
				return 1;
			} else {
				prev->next = temp->next;
				free(temp);
				return 1;
			}
		} else {
			prev = temp;
			temp = temp->next;
		}
	return 0;
}

void merging(int low, int mid, int high) {
	int l1, l2, i;
	for (l1 = low, l2 = mid + 1, i = low; l1 <= mid && l2 <= high; i++) 
		if (pPoints[l1].values[0] <= pPoints[l2].values[0])
			b[i] = pPoints[l1++];
		else
			b[i] = pPoints[l2++];
	while (l1 <= mid)
		b[i++] = pPoints[l1++];
	while (l2 <= high)
		b[i++] = pPoints[l2++];
	for (i = low; i <= high; i++)
		pPoints[i] = b[i];
}
void sort(int low, int high) {
	int mid;
	if (low < high) {
		mid = (low + high) / 2;
		sort(low, mid);
		sort(mid + 1, high);
		merging(low, mid, high);
	} else 
	return;
}

bool dominated(Point A, Point B) {
	int eq = 0, i;
	for (i = 0; i < pdim; ++i) {
		if (A.values[i] <= B.values[i]) {
			if (A.values[i] == B.values[i]) 
				eq++;
			
		} else
		break;
	}
	if (eq < pdim && i == pdim) 
		return true;
	return false;
}

void * checker(void * x) {
	int pos = * (int * ) x;
	struct Node * r, * r2;
	if (pos == 0) {
		for (int i = 0; i < num; i++) {
			r = head;
			r2 = NULL;
			while (r != NULL) {
				r2 = r->next;
				if (dominated(pPoints[r->data], pPoints[i]))
					break;
				if (dominated(pPoints[i], pPoints[r->data]))
					delete(r->data);
				r = r2;
			}
			if (r == NULL)
				add(i);
		}
	}else
	for (int i = 0; i < 100; ++i)
		i+=2;
	pthread_exit(NULL);
}

int asgn2_pthread(Point * points, Point * * pPermissiblePoints, int number, int dim, int thread_number) {
	int permissiblePointNum = 0, bound[thread_number], t[thread_number];
	Point * permissiblePoints = malloc(sizeof(Point) * number);
	void * status;
	b = malloc(sizeof(Point) * number);
	pPoints = points;
	num = number;
	pdim = dim;
	sort(0, number - 1);
	struct Node *r;
	head = NULL;
	pthread_t threads[thread_number];
	pthread_attr_t attr;
	pthread_attr_init( & attr);
	pthread_attr_setdetachstate( & attr, PTHREAD_CREATE_JOINABLE);
	for (int i = 0; i < thread_number; i++) {
		t[i] = i;
		pthread_create( & threads[i], & attr, checker, & t[i]);
		if ((i + 1) == thread_number)
			for (int j = 0; j < thread_number; j++)
				pthread_join(threads[j], & status);
		}
		r = head;
		while (r != NULL) {
			permissiblePoints[permissiblePointNum++] = pPoints[r->data];
			r = r->next;
		}
		* pPermissiblePoints = realloc(permissiblePoints, sizeof(Point) * permissiblePointNum);
		return permissiblePointNum;
	}