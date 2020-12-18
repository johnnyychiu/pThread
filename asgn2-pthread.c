#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include"util.h"
#include <omp.h>
typedef int bool;
#define true 1
#define false 0
struct Node{
	int data;
	struct Node *next;
}*head,*temp;
int num, pdim, permissiblePointNum;
Point * pPoints, * b, * permissiblePoints ;

void add(int num) {
	temp = malloc(sizeof(struct Node));
	temp->data = num;
	temp->next = head;
	head = temp;
}

void merging(int low, int mid, int high) {
	int l1, l2, i;
	for (l1 = low, l2 = ((low + high) / 2) + 1, i = low; l1 <= ((low + high) / 2) && l2 <= high; i++) 
		b[i] = pPoints[l1].values[0] <= pPoints[l2].values[0] ? pPoints[l1++] : pPoints[l2++];
	while (l1 <= ((low + high) / 2))
		b[i++] = pPoints[l1++];
	while (l2 <= high)
		b[i++] = pPoints[l2++];
	for (i = low; i <= high; i++)
		pPoints[i] = b[i];
}

void sort(int low, int high) {
	if (low < high) {
        #pragma omp parallel 
        {
            #pragma omp single
            {
                #pragma omp task
                sort(low, ((low + high) / 2));

                #pragma omp task
                sort(((low + high) / 2) + 1, high);
            }
        }			
		merging(low, ((low + high) / 2), high);
	}
}

bool dominated(Point A, Point B) {
	for (int i = 0; i < pdim; i++) 
		if (A.values[i] > B.values[i]) 
			return false;
	return true;	
}

int checker(int pos) {
	// if() return 0;
	struct Node * r;		
	for (int i = 0; i < num && !pos; i++) {
		r = head;
		while (r != NULL) {
			if (dominated(pPoints[r->data], pPoints[i]))
				break;
			r = r->next;
		}
		if (r == NULL){
			permissiblePoints[permissiblePointNum++] = pPoints[i];
			add(i);
		}
	}
}

int asgn2_pthread(Point * points, Point ** pPermissiblePoints, int number, int dim, int thread_number) {
	permissiblePointNum = 0;
	permissiblePoints = malloc(sizeof(Point) * number);
	b = malloc(sizeof(Point) * number);
	pPoints = points;
	num = number;
	pdim = dim;
	head = NULL;
	sort(0, number - 1);
	#pragma omp parallel for 
	for (int i = 0; i < thread_number; ++i)
		checker(i);
	* pPermissiblePoints = realloc(permissiblePoints, sizeof(Point) * permissiblePointNum);
	return permissiblePointNum;
}