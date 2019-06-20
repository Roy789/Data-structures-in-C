#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

// You need to include additional fields, data structures and functions  

// data type for heap nodes
typedef struct HeapNode { 
	// each node stores the priority (key), name, execution time,
	//  release time and deadline of one task
	int key; //key of this task
	int TaskName;  //task name 
	int Etime; //execution time of this task
	int Rtime; // release time of this task
	int Dline; // deadline of this task
	int degree; //Keeping the degree of the node.
	struct HeapNode *parent;  // Pointer to the parent. 
	struct HeapNode *child;   //Pointer to the child
	struct HeapNode *sibling; //Pointer to the sibling. 
} HeapNode;

//data type for a priority queue (heap) 
typedef struct BinomialHeap{ //this is heap header
	int  size;      // count of items in the heap
	struct HeapNode *root;
} BinomialHeap;

// create a new heap node to store an item (task) 
HeapNode *newHeapNode(int k, int n, int c, int r, int d)
{ // k:key, n:task name, c: execution time, r:release time, d:deadline
  // ... you need to define other parameters yourself) 	 
	HeapNode *new;
	new = malloc(sizeof(HeapNode));
	assert(new != NULL);
	new->key = k;		//Key of the heap 
	new->TaskName = n;	//Taskname that is being entered.
	new->Etime = c;		//Execution time
	new->Rtime = r; 	//Releasetime
	new->Dline = d;		//Deadline time
	new -> degree = 0;	//Degree of the node (Used to order the heap)
	new -> parent = NULL;//Parent pointer 
	new -> child = NULL; //Child pointer
	new -> sibling = NULL; // initialise other fields 
	return new;
}

// create a new empty heap-based priority queue
BinomialHeap *newHeap()
{ // this function creates an empty binomial heap-based priority queue
	BinomialHeap *T;
	T = malloc(sizeof(BinomialHeap));
	assert (T != NULL);  
	T -> size = 0;
	T -> root = NULL;
	return T;
}


//Merging of two binomial heaps.
//Helper function which takes two roots an merges the heaps. 
//This merges two heaps but doesnt check the degree of each of the binomial sub trees in it. 
/* This Function takes o(Log(n)) time to complete because of the while loop. */
HeapNode *merges_heaps_nodes(HeapNode *root_1,HeapNode *root_2)
{
	//Return node for our final function. 
	HeapNode *return_node;
	HeapNode *h1 = root_1; //Keeping track of the pointer for root of heap_1
	HeapNode *h2 = root_2; //Keeping track of the pointer for root of heap_2

	HeapNode *last_node;
	//Checks if the degree of the root 1 correponding to the Heap_1 has a higher degree than the h2 which is the root of the heap 2
	if (root_1 -> degree > root_2 -> degree){
		//Return node becomes root_2 if it has a lower degree
		return_node = root_2;
		h2 = h2 -> sibling;
	}
	else{
		return_node = root_1;
		h1 = h1 -> sibling;
	}
	//Keeping track of the return node. and using last_node to traverse over the heap. 
	last_node = return_node;
	//Making all the proper connections. 
	while(h1 != NULL && h2 != NULL){

		if(h1->degree > h2 -> degree){
			last_node -> sibling = h2;
			h2 = h2 ->sibling;
		}
		else{
			last_node -> sibling = h1;
			h1 = h1 -> sibling;
		}
		//Going to the next binomial tree of the heap.
		last_node = last_node -> sibling;
	}
	//if heap_1 is not Null thenit becomes a sibling of the last_node. same for heap 2.
	if (h1 != NULL){
		last_node -> sibling = h1;
	}
	else if(h1 == NULL){
		last_node -> sibling = h2;
	}
	return return_node;
}

//Main function. 
//This function just passes the root node to our helper function.
//This function takes the same time as our helper function that is O(Log(n)) time....
HeapNode *merge_heaps(BinomialHeap *heap_1, BinomialHeap *heap_2)
{
	//returning the root of heap_2 if the heap_1's root is NULL
	if ( heap_1 -> root == NULL){
		return heap_2 -> root;
	}
	if ( heap_2 -> root == NULL){
		return heap_1 -> root;
	}
	HeapNode *root; //For the new root that will be formed. 
	HeapNode *heap_root_1 = heap_1 -> root;
	HeapNode *heap_root_2 = heap_2 -> root;
	//Passing the roots to the merges_heaps_nodes which returns a node pointing to the root of the merged heap.  
	root = merges_heaps_nodes(heap_root_1, heap_root_2);
	return root;
}

//Union of two heaps used for joining the heaps.
//Forming the necessary links for the nodes. 
//This function just takes O(1) time for completion. 
int Linking_two_nodes(HeapNode *node_1, HeapNode *node_2){
	//Setting node_1 as the parent of node_2 and all the other links.
	//Also increasing the degree of node_2. 
	node_1 -> parent = node_2;
	node_1 -> sibling = node_2->child;
	node_2 -> child = node_1;
	node_2 -> degree++;
	return 1;
}

//Union of two heaps. 
//This union of two heaps takes O(Log(n)) time. 
HeapNode *Heap_union(BinomialHeap *heap_1, BinomialHeap *heap_2)
{
	//New return root.
	HeapNode *new_root;
	//Calls th merging function and just merges
	new_root = merge_heaps(heap_1, heap_2);
	//Check for null case.
	if (new_root == NULL){
		return NULL;
	}
	//Setting a pointer to the new_root for returning the value. 
	HeapNode *Auxilary_node = new_root;
	//Keeping track of the previous sibling for connection.
	HeapNode *previous = NULL;
	//And next root will be the sibling of the new_root ( Condition for the while loop)
	HeapNode *next = new_root -> sibling;
	//Iterating over all the siblings. 
	while(next != NULL){
		//Case 1:
		/*if the degree of The Auxilary root is not the same as the degree of the next sibling. 
		Or it is not the last sibling of the Heap and the next to next sibling of the tree has the
		 same degree as the Auxilary node's degree go to the next sbling and adjust the previous and 
		 the Auxilary node*/ 
		if (Auxilary_node -> degree != next -> degree || (next->sibling != NULL && next -> sibling-> degree == Auxilary_node -> degree)){
			previous = Auxilary_node;
			Auxilary_node = next;
		}
		//Everyother case
		else
		{
			//Comparing the keys to arrange the nodes into the heap. Linking the nodes using our link function. 
			if (Auxilary_node -> key <= next -> key){
				Auxilary_node -> sibling = next -> sibling;
				Linking_two_nodes(next, Auxilary_node);
			}
			else{
				//If the previous is NULL.
				if (previous == NULL){
					new_root = next;
				}
				else{
					//Previous sibling is been set to the Auxilary_node -> sibling which is over current pointer. 
					previous -> sibling = next;
				}
				//Linking the Auxilary_node and the sibling but as a parent and creating the child link as well. 
				Linking_two_nodes(Auxilary_node, next);
				Auxilary_node = next;
			}
		}
		//moving ahead in the loop. 
		next = Auxilary_node -> sibling;
	}
	//The new root to the Binomial heap is been returned. 
	return new_root;
}


//Formation of the heap. we need heap insert for this. 
// put the time complexity analysis for Insert() here 
//The insert function takes O(1) time for creating a new node and Log(n) time for finding the union of heaps. 
void Insert(BinomialHeap *T, int k, int n, int c, int r, int d)
{ // k: key, n: task name, c: execution time, r: release time, d:deadline 
  // You don't need to check if this task already exists in T 	 
	HeapNode *node_1;
	node_1 = newHeapNode(k,n,c,r,d);
	//Making a new heap with this individual node and then doing the union operation 
	//With this new heap and the orignal Heap.
	BinomialHeap *Auxilary_heap;
	Auxilary_heap = newHeap();
	Auxilary_heap -> root = node_1;
	T -> root = Heap_union(T, Auxilary_heap); //Heap Union
	T -> size++; //Incrementing the size if a node is being added.
}



//Helper function to remove a minimum node from the tree..
//This function takes 3 inputs The heap , the minimum node_which is found by the get minimum, previous node for connecting the children of the minimum node back to the heap.
//This function takes O(Log(n)) time to complete. 
int removing_node(BinomialHeap *Heap_1, HeapNode *min_node, HeapNode *previous_node){
	//if the minimum_node is the root itself just set the root to its sibling. 
	if(min_node == Heap_1 -> root){
		Heap_1 -> root = min_node -> sibling; // Setting the new root to the sibling. 
	}
	else{
		//Breaking the link to the minimum node. 
		previous_node -> sibling = min_node -> sibling;
	}
	//Setting a new root Heap_node which will be our root for our temp heap for union. 
	HeapNode *new_root = NULL;
	//
	HeapNode *node_child = min_node -> child;
	while(node_child != NULL){
		HeapNode *next_node;
		next_node = node_child -> sibling;
		node_child -> sibling = new_root;
		node_child -> parent = NULL; // After creating a link to the sibling of the node_child we can free its parent. 
		new_root = node_child; //Setting the new root to the child.
		node_child = next_node; //iterating over to the next node. 

	}
	//Auxilary heap for union 
	BinomialHeap *Auxilary_heap;
	Auxilary_heap = newHeap(); // Creating a new heap for storing the Auxilary heap. 
	Auxilary_heap -> root = new_root;
	//Union of the heap and the auxilary heap which points to the new_root. 
	Heap_1 -> root = Heap_union(Heap_1, Auxilary_heap);
	return 0;
}
//Helper function for getting pointer to the minimum first. And then using the minimum node and the previous minimum node for removal of the minium node and also maintaining the heap. 
//This takes O(Log(n)) time to be completed. 
HeapNode *get_minimum(BinomialHeap *Heap)
{
	if (Heap -> root == NULL){
		return NULL;
	}
	HeapNode *minimum_node;
	minimum_node = Heap -> root;
	HeapNode *previous_minimum_node; //Keeping track so that We can use it in the removal function.
	HeapNode *next_previous_node = minimum_node;
	HeapNode *next_node = minimum_node -> sibling; //Keeping track of the next node. 
	while(next_node != NULL){
		if (next_node -> key< minimum_node -> key){
			minimum_node = next_node; //Setting the new minimum. 
			previous_minimum_node = next_previous_node; //Keeping track of the ast node that was visited to join the previous and the next node together using our function.
		}
		next_previous_node = next_node; //Traversing through the sibling o the roots. 
		next_node = next_node -> sibling;

	}
	//Removal of the minimum node. 
	removing_node(Heap, minimum_node, previous_minimum_node);
	Heap -> size--; //Subtracting the size as we remove one node from the heap using the remove min helper.
	return minimum_node;
}


//***Code for priority queue of cores this is the same as the Binomial heap***
//New Binomial Heap for storing the core times and the core name. 
//Creating a binomial heap for the cores..
typedef struct heap_core_node { 
	// each node stores the priority (key), name, execution time,
	//  release time and deadline of one task
	int key; //Time of the core at present.
	int core_name;  //core name 
	int degree;
	struct heap_core_node *parent;
	struct heap_core_node *child;
	struct heap_core_node *sibling;
} heap_core_node;

//Intialising the new core_node...
heap_core_node *new_heap_core_node(int k, int n)
{ // k:key, n:task name, c: execution time, r:release time, d:deadline
  // ... you need to define other parameters yourself) 	 
	heap_core_node *new;
	new = malloc(sizeof(heap_core_node));
	assert(new != NULL);
	new->key = k;
	new->core_name = n;
	new -> degree = 0;
	new -> parent = NULL;
	new -> child = NULL;
	new -> sibling = NULL; // initialise other fields 
	return new;
}


//New structure for the Binomial_core_heap.
typedef struct BinomialHeap_core{ //this is heap header
	int  size;      // count of items in the heap
	struct heap_core_node *root;
} BinomialHeap_core;

//New priority queue for the core. 
BinomialHeap_core *newHeap_core()
{ // this function creates an empty binomial heap-based priority queue
	BinomialHeap_core *T;
	T = malloc(sizeof(BinomialHeap_core));
	assert (T != NULL);
	T -> size = 0;
	T -> root = NULL;
	return T;
}

//Merging of two binomial heaps of core_nodes. 
//Merging of two binomial heaps.
//Helper function which takes two roots an merges the heaps. 
//This merges two heaps but doesnt check the degree of each of the binomial sub trees in it. 
heap_core_node *merges_heaps_nodes_core(heap_core_node *root_1,heap_core_node *root_2)
{
	//Return node for our final function. 
	heap_core_node *return_node;
	heap_core_node *h1 = root_1; //Keeping track of the pointer for root of heap_1
	heap_core_node *h2 = root_2; //Keeping track of the pointer for root of heap_2

	heap_core_node *last_node;
	//Checks if the degree of the root 1 correponding to the Heap_1 has a higher degree than the h2 which is the root of the heap 2
	if (root_1 -> degree > root_2 -> degree){
		//Return node becomes root_2 if it has a lower degree
		return_node = root_2;
		h2 = h2 -> sibling;
	}
	else{
		return_node = root_1;
		h1 = h1 -> sibling;
	}
	//Keeping track of the return node. and using last_node to traverse over the heap. 
	last_node = return_node;
	//Making all the proper connections. 
	while(h1 != NULL && h2 != NULL){

		if(h1->degree > h2 -> degree){
			last_node -> sibling = h2;
			h2 = h2 ->sibling;
		}
		else{
			last_node -> sibling = h1;
			h1 = h1 -> sibling;
		}
		//Going to the next binomial tree of the heap.
		last_node = last_node -> sibling;
	}
	//if heap_1 is not Null thenit becomes a sibling of the last_node. same for heap 2.
	if (h1 != NULL){
		last_node -> sibling = h1;
	}
	else if(h1 == NULL){
		last_node -> sibling = h2;
	}
	return return_node;
}

heap_core_node *merge_heaps_core(BinomialHeap_core *heap_1, BinomialHeap_core *heap_2)
{
	//returning the root of heap_2 if the heap_1's root is NULL
	if ( heap_1 -> root == NULL){
		return heap_2 -> root;
	}
	if ( heap_2 -> root == NULL){
		return heap_1 -> root;
	}
	heap_core_node *root; //For the new root that will be formed. 
	heap_core_node *heap_root_1 = heap_1 -> root;
	heap_core_node *heap_root_2 = heap_2 -> root;
	//Passing the roots to the merges_heaps_nodes which returns a node pointing to the root of the merged heap.  
	root = merges_heaps_nodes_core(heap_root_1, heap_root_2);
	return root;
}
void DISPLAY(heap_core_node * H);
//Union of two core_heaps
//Necessary link for core-nodes.
int Linking_two_nodes_core(heap_core_node *node_1, heap_core_node *node_2){
	//Setting node_1 as the parent of node_2 and all the other links.
	//Also increasing the degree of node_2. 
	node_1 -> parent = node_2;
	node_1 -> sibling = node_2->child;
	node_2 -> child = node_1;
	node_2 -> degree++;
	return 1;
}

//Union of two heaps. 
heap_core_node *Heap_union_core(BinomialHeap_core *heap_1, BinomialHeap_core *heap_2)
{
	//New return root.
	heap_core_node *new_root;
	//Calls th merging function and just merges
	new_root = merge_heaps_core(heap_1, heap_2);
	//Check for null case.
	if (new_root == NULL){
		return NULL;
	}
	//Setting a pointer to the new_root for returning the value. 
	heap_core_node *Auxilary_node = new_root;
	//Keeping track of the previous sibling for connection.
	heap_core_node *previous = NULL;
	//And next root will be the sibling of the new_root ( Condition for the while loop)
	heap_core_node *next = new_root -> sibling;
	//Iterating over all the siblings. 
	while(next != NULL){
		//Case 1:
		/*if the degree of The Auxilary root is not the same as the degree of the next sibling. 
		Or it is not the last sibling of the Heap and the next to next sibling of the tree has the
		 same degree as the Auxilary node's degree go to the next sbling and adjust the previous and 
		 the Auxilary node*/ 
		if (Auxilary_node -> degree != next -> degree || (next->sibling != NULL && next -> sibling-> degree == Auxilary_node -> degree)){
			previous = Auxilary_node;
			Auxilary_node = next;
		}
		//Everyother case
		
		else
		{
			//Comparing the keys to arrange the nodes into the heap. Linking the nodes using our link function. 
			if (Auxilary_node -> key <= next -> key){
				Auxilary_node -> sibling = next -> sibling;
				Linking_two_nodes_core(next, Auxilary_node);
			}
			else{
				//If the previous is NULL.
				if (previous == NULL){
					new_root = next;
				}
				else{
					//Previous sibling is been set to the Auxilary_node -> sibling which is over current pointer. 
					previous -> sibling = next;
				}
				//Linking the Auxilary_node and the sibling but as a parent and creating the child link as well. 
				Linking_two_nodes_core(Auxilary_node, next);
				Auxilary_node = next;
			}
		}
		//moving ahead in the loop. 
		next = Auxilary_node -> sibling;
	}
	//The new root to the Binomial heap is been returned. 
	return new_root;
}


//Formation of the heap. we need heap insert for this. 
// put the time complexity analysis for Insert() here    
void insert_core(BinomialHeap_core *T, int k, int core_name)
{ // k: key, n: task name, c: execution time, r: release time, d:deadline 
  // You don't need to check if this task already exists in T 	 
	heap_core_node *node_1;
	node_1 = new_heap_core_node(k, core_name);
	//Making a new heap with this individual node and then doing the union operation 
	//With this new heap and the orignal Heap.
	BinomialHeap_core *Auxilary_heap;
	Auxilary_heap = newHeap_core();
	Auxilary_heap -> root = node_1;
	T -> root = Heap_union_core(T, Auxilary_heap);
	T -> size++;
}
//Removing a node from a core_heap.

int removing_node_core(BinomialHeap_core *Heap_1, heap_core_node *min_node, heap_core_node *previous_node){
	//if the minimum_node is the root itself just set the root to its sibling. 
	if(min_node == Heap_1 -> root){
		Heap_1 -> root = min_node -> sibling; // Setting the new root to the sibling. 
	}
	else{
		//Breaking the link to the minimum node. 
		previous_node -> sibling = min_node -> sibling;
	}
	//Setting a new root Heap_node which will be our root for our temp heap for union. 
	heap_core_node *new_root = NULL;
	//
	heap_core_node *node_child = min_node -> child;
	while(node_child != NULL){
		heap_core_node *next_node;
		next_node = node_child -> sibling;
		node_child -> sibling = new_root;
		node_child -> parent = NULL; // After creating a link to the sibling of the node_child we can free its parent. 
		new_root = node_child; //Setting the new root to the child.
		node_child = next_node; //iterating over to the next node. 

	}
	//Auxilary heap for union 
	BinomialHeap_core *Auxilary_heap;
	Auxilary_heap = newHeap_core(); // Creating a new heap for storing the Auxilary heap. 
	Auxilary_heap -> root = new_root;
	//Union of the heap and the auxilary heap which points to the new_root. 
	Heap_1 -> root = Heap_union_core(Heap_1, Auxilary_heap);
	//printf("This is core_time: %d\n",Heap_1->root -> key );
	return 0;
}

//Helper function for getting the minimum first. And then using the minimum node and the previous minimum node for removal of the minium node and also maintaining the heap. 
heap_core_node *get_minimum_core(BinomialHeap_core *Heap)
{
	if (Heap -> root == NULL){
		return NULL;
	}
	heap_core_node *minimum_node;
	minimum_node = Heap -> root;
	heap_core_node *previous_minimum_node;
	heap_core_node *next_previous_node = minimum_node;
	heap_core_node *next_node = minimum_node -> sibling; //Keeping track of the next node. 
	while(next_node != NULL){
		if (next_node -> key< minimum_node -> key){
			minimum_node = next_node; //Setting the new minimum. 
			previous_minimum_node = next_previous_node; //Keeping track of the ast node that was visited to join the previous and the next node together using our function.
		}
		next_previous_node = next_node; //Traversing through the sibling o the roots. 
		next_node = next_node -> sibling;

	}
	//Removal of the minimum node. 
	removing_node_core(Heap, minimum_node, previous_minimum_node);
	Heap -> size--;
	return minimum_node;
}

/*Defining a function which takes in a Binomial heap of tasks and core_heaps and returns length if 
A feasible schedule is found and length(But this length is smaller than the number of nodes which
is checked in our main funcction) if it is not found. Outputs an array which has all the 
times of the scheduled task which will be used for outputting in a file. */

int schedule_of_tasks(BinomialHeap *nodes_heap, BinomialHeap_core *core_heap, int return_array[]){
	int length = 0 ;
	int start_time; 
	int present_core_time;
	//Loop for checking if all the tasks have been completed (If the heap is empty)
	while(nodes_heap -> size != 0){
		HeapNode *node_1;
		heap_core_node *core;
		//Getting the mi from the binomial_heap and the core heap.
		node_1 = get_minimum(nodes_heap);
		//popping minimum from the core_node
		core = get_minimum_core(core_heap);
		//Core's key is greater than the nodes meaning if core_time is less than the release time we will 
		//Add Release time + the execution time. 
		if (core -> key < node_1 -> Rtime){
			present_core_time = node_1 -> Rtime + node_1 -> Etime;
			start_time = node_1 -> Rtime;
		}
		//If Core time is more the task will only go into the core after the core_time(Which is the finishing of the last task)
		else{
			present_core_time = core -> key + node_1 -> Etime;
			start_time = core -> key;
		}
		//If it passes the deadline. 
		if (present_core_time > node_1 -> Dline){
			return_array[length] = node_1 -> TaskName;
			return_array[length+1] = core -> core_name;
			return_array[length+2] = start_time;
			length = length +3;
			return length/3;
		}
		//Storing the numbers in an array for printing the file in the main function. 
		else{
			insert_core(core_heap, present_core_time, core -> core_name);
			return_array[length] = node_1 -> TaskName;
			return_array[length+1] = core -> core_name;
			return_array[length+2] = start_time;
			length = length +3;
		}
	}
	//Returning the total number of tasks that were evaluated. 
	return length/3;
}


//This function takes O(nLog(n)) time to be completed. The Insert function takes nLog(n) time and the 
//Insert inheap takes m(log(m)) time but the total Time complexity of the function remains O(nLog(n)) time 
//because in a real world scenario number of tasks(n) will always be greater than number of cores(m). (n > m)
//Main function of time scheduling. 
int TaskScheduler(char *f1,char *f2, int m )
{
	FILE *fp;
	char ch;
	int n;
	//This has all the numbers that have been read from the file. 
	int numbers[1000];
	int length =0;
	//Opening the file. 
	fp =fopen(f1,"r");
	while (fscanf(fp, " %d", &n) == 1) {
		numbers[length] = n;
	    length ++;
	}

	int j = 0;
	fclose(fp);
	j = 0;
	int num_nodes = 0;		//Storing the number of nodes in the binomial heap.
	//Inserting the nodes for the first binomial tree having release times as their keys. 
	BinomialHeap *heap_release_time;

	//Making a Binomial heal of all the tasks which will take O(nLog(n)) time.  
	heap_release_time = newHeap();
	while(j < length-3){
		Insert(heap_release_time,numbers[j+2],numbers[j], numbers[j+1], numbers[j+2], numbers[j+3]);
		j = j+4;
		num_nodes++;
	}
	//Creating a heap for storing the dead line times as keys. 
	//This will take O(nLog(n)) time for inserting n number of nodes. 
	BinomialHeap *heap_deadline_time;
	heap_deadline_time = newHeap();
	j = 0;
	while(j < length-3){
		HeapNode *pointer;
		Insert(heap_deadline_time,numbers[j+3],numbers[j], numbers[j+1], numbers[j+2], numbers[j+3]);
		j = j+4;
	}
	//Creating a heap for storing the average of the release and deadline_time. 
	//This will take O(nLog(n)) time for inserting n number of nodes. 
	BinomialHeap *heap_avg;
	heap_avg = newHeap();
	j = 0;
	while(j < length-3){
		Insert(heap_avg,numbers[j]+numbers[j+2],numbers[j], numbers[j+1], numbers[j+2], numbers[j+3]);
		j = j+4;
	}
	//Making a Binomial heap for the cores. 
	int array_tasks[1000];
	int iter = 0;
	BinomialHeap_core *heap_of_cores_1;
	heap_of_cores_1 = newHeap_core();
	//1st core heap for binomial heaps using starttime as their key. 
	//Takes mlog(m) time to insert but considering that the number of tasks will always be greater than the 
	//number of cores total running time wont be affected. 3 heaps like this have been made. 
	while(iter < m){
		insert_core(heap_of_cores_1, 0, iter);
		iter++;
	}
	//Making the array for returning the numbers to be outputted on the file.
	int time_release;
	int file2_array[1000];
	int file2_array_1[1000];
	int file2_array2[1000];
	iter = 0;
	//2nd core_heap for binomial heaps using Deadline time as their keys. 
	BinomialHeap_core *heap_of_cores_2;
	heap_of_cores_2 = newHeap_core();
	while(iter < m){
		insert_core(heap_of_cores_2, 0, iter);
		iter++;
	}
	iter =0;
	//3rd heap which will use the average of start and end times as the key. 
	BinomialHeap_core *heap_of_cores_3;
	heap_of_cores_3 = newHeap_core();
	while(iter < m){
		insert_core(heap_of_cores_3, 0, iter);
		iter++;
	}
	//Passing the heaps to our helper function and then returns the number of tasks that were completed. 
	int time_deadline, time_avg;
	time_release = schedule_of_tasks(heap_release_time, heap_of_cores_1, file2_array);
	time_deadline = schedule_of_tasks(heap_deadline_time, heap_of_cores_2, file2_array_1);
	time_avg = schedule_of_tasks(heap_avg, heap_of_cores_3,file2_array2);
	iter = 0;

	//Checking if the time release is not equal to the number of node for all our test cases than means our edf was not able to find the perfect schedule. 
	if (time_release != num_nodes && time_deadline != num_nodes && time_avg != num_nodes){
		iter = 0;
		FILE *ffinal;
		ffinal = fopen(f2, "w");
		while(iter < time_release*3){
			//If the Binomial heap with release times finds a feasible solution then this gets printed. 
			fprintf(ffinal,"%d ", file2_array[iter++]);
			fprintf(ffinal,"Core%d ", file2_array[iter++]+1);
			fprintf(ffinal,"%d ", file2_array[iter++]);
		}
		//returning a failed case, because none of the heaps were able to find a schedule. 
		return 0;
	}
	//If a feasible schedule was found with release times as their keys. 
	else if (time_release == num_nodes){
		iter = 0;
		FILE *ffinal;
		ffinal = fopen(f2, "w");
		while(iter < time_release*3){
			fprintf(ffinal,"%d ", file2_array[iter++]);		  //Prints the taskname
			fprintf(ffinal,"Core%d ", file2_array[iter++]+1); //Prints the Core name.
			fprintf(ffinal,"%d ", file2_array[iter++]);		  //Printis the start time in the core. 
		}
		return 1;
	}
	//If a feasible schedule was found using Average_time time. 
	else if (time_deadline == num_nodes){
		iter = 0;
		FILE *ffinal;
		ffinal = fopen(f2, "w");
		while(iter < time_deadline*3){
			fprintf(ffinal,"%d ", file2_array_1[iter++]);
			fprintf(ffinal,"Core%d ", file2_array_1[iter++]+1);
			fprintf(ffinal,"%d ", file2_array_1[iter++]);			
		}
		//Found a schedule
		return 1;
	}
	//If schedule was found using the average time of the start and the task_name. 
	else if (time_avg == num_nodes){
		iter = 0;
		FILE *ffinal;
		ffinal = fopen(f2, "w");
		while(iter < time_avg*3){
			fprintf(ffinal,"%d ", file2_array2[iter++]);
			fprintf(ffinal,"Core%d ", file2_array2[iter++]+1);
			fprintf(ffinal,"%d ", file2_array2[iter++]);			
		}
		//Found a schedule
		return 1;
	}

	return 0;
}

 

int main() //sample main for testing 
{ 
  int i;
  i=TaskScheduler("samplefile1.txt", "feasibleschedule1.txt", 4);
  if (i==0) printf("No feasible schedule!\n");
  /* There is a feasible schedule on 4 cores */
  i=TaskScheduler("samplefile1.txt", "feasibleschedule2.txt", 3);
  if (i==0) printf("No feasible schedule!\n");
  /* There is no feasible schedule on 3 cores */
  i=TaskScheduler("samplefile2.txt", "feasibleschedule3.txt", 5);
  if (i==0) printf("No feasible schedule!\n");
  /* There is a feasible schedule on 5 cores */
  i=TaskScheduler("samplefile2.txt", "feasibleschedule4.txt", 4);
  if (i==0) printf("No feasible schedule!\n");
  /* There is no feasible schedule on 4 cores */
  i=TaskScheduler("samplefile3.txt", "feasibleschedule5.txt", 2);
  if (i==0) printf("No feasible schedule!3\n");
  /* There is no feasible schedule on 2 cores */
  i=TaskScheduler("samplefile4.txt", "feasibleschedule6.txt", 2);
  if (i==0) printf("No feasible schedule!\n");
  return 0; 
}