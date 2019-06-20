
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <assert.h>
#define COUNT 10 

// all the basic data structures and functions are included in this template
// you can add your own auxiliary functions as you like 

// data type for avl tree nodes
typedef struct AVLTreeNode {
	int key; //key of this item
	int  value;  //value (int) of this item 
	int height; //height of the subtree rooted at this node
	struct AVLTreeNode *parent; //pointer to parent
	struct AVLTreeNode *left; //pointer to left child
	struct AVLTreeNode *right; //pointer to right child
} AVLTreeNode;

//data type for AVL trees
typedef struct AVLTree{
	int  size;      // count of items in avl tree
	AVLTreeNode *root; // root
} AVLTree;

// create a new AVLTreeNode
AVLTreeNode *newAVLTreeNode(int k, int v )
{
	AVLTreeNode *new;
	new = malloc(sizeof(AVLTreeNode));
	assert(new != NULL);
	new->key = k;
	new->value = v;
	new->height = 1; // height of this new node is set to 0
	new->left = NULL; // this node has no child
	new->right = NULL;
	new->parent = NULL; // no parent
	return new;
}

// create a new empty avl tree
AVLTree *newAVLTree()
{
	AVLTree *T;
	T = malloc(sizeof (AVLTree));
	assert (T != NULL);
	T->size = 0;
	T->root = NULL;
	return T;
}
// HELPER FUNCTIONS..
/*1) IS_EMPTY FUNCTION:
	Checks of the string inputed to the function is empty 
	or not, used for chcking for spaces in the input data.*/
int is_empty(const char *s) {
  while (*s != '\0') {
    if (!isspace((unsigned char)*s))
      return 0;
    s++;
  }
  return 1;
}

/* 2) Max checking function for height returns the maximum value out
of the two inputs. */

int max(int a, int b){
	if (a >= b){
		return a;
	}
	else
		return b;
}
/*3 Function for checking if the leaf node is NULL then return height of the node as 0
otherwise return theheight of the node. 
Node -> height will give an segmentation fault if node is NULL*/
int height(struct AVLTreeNode *node) 
{ 
    if (node == NULL) 
        return 0; 
    return node->height; 
} 

//4) Handling rotations of nodes due to get a balanced tree..
/*Left-rotate rotating the node if the height is imbalanced 
we take the right o the node and save in p_right and left of p_right in p_left 
and returning p_right instead of the node after updating the heights */
AVLTreeNode *left_rotate(AVLTreeNode *node)
{
	//intializing the variables. 
	AVLTreeNode *p_left, *p_right;
	p_right = node->right;
	p_left = p_right -> left;

	//Rotation operation.
	p_right->left = node;
	node->right = p_left;

	//updating the heights for everynode...
	node -> height = 1 + max(height(node->left), height(node -> right));
	p_right -> height = 1 + max(height(p_right-> left), height(p_right -> right));
	return p_right;
}

/* Right rotate of the node where we store left of the node in p_left
And then right of the p_left in right make adjustments to the particular nodes
and retuenp_left after updating heights.*/
AVLTreeNode *right_rotate(AVLTreeNode *node)
{
	//Intializing the variables. 
	AVLTreeNode *p_left, *p_right;

	p_left = node -> left;
	p_right = p_left -> right; 

	//Rotation operation.
	p_left->right = node; 
    node->left = p_right;

    //Updating the heights of the nodes...
    node-> height = 1 + max(height(node->right), height(node->left));
    return p_left;
}





//5)Storing the height of each node in the Tree getting the height of each of the node. 
void get_height( AVLTreeNode *node)
{
	if(node != NULL){
		node -> height = 1 + max(height(node->left), height(node -> right));
	}
}

/*6) Helper function for finding the height difference of node, but returning 0 if the 
node itself is NULL, this is used in Delete node..*/
int height_diff_function(AVLTreeNode *node)
{
	if (node == NULL){
		return 0;
	}
	else{
		int height_diff_var = height(node->left)- height(node->right);
		return height_diff_var;
	}
}
/* 6) Helper function for traversing through the tree and storing the keys, 
values in 2 arrays named arr_keys, arr_values
This algorithim is also known as Morris traversal. 
 O(n) time.*/
void traversal_final(AVLTreeNode* root, int arr_keys[],int arr_values[], int size) 
{ 
	int i = 0;
    struct AVLTreeNode *current, *parent; 
  	//Condition where the root is null and then return and not do anything. 
    if (root == NULL) 
        return; 
  
    current = root; 
    /*Finding the inorder traversal of the root which will be the right most child on 
    the left node of the root. */
    while (current != NULL) { 
    	//if left is NULL that means that the root is the first node of the inorder traversal. 
        if (current->left == NULL) { 

            arr_keys[i] = current -> key;
            arr_values[i] = current -> value;
            i++;
            /*changing the link to current right so that it goes 
            to the right node and performs the same check again that it did on the node*/
            current = current->right; 
        } 
        else { 
        	//This loops through when the node as current has no left node. 
  			//Setting the node left of the root to parent.
            parent = current->left; 
            //Checking if right of the parent is not the current itself, if it is current that means that this node has been traversed already and obviously non NULL.
            while (parent->right != NULL && parent->right != current) 
                parent = parent->right; 
  
            // Making current as right node of its  parent, creating a temporary link for finding the inorder traversal 
            if (parent->right == NULL) { 
                parent->right = current; 
                current = current->left; 
            } 
  
            // Getting back to the original tree by breaking the temporary loop.
            else { 
                parent->right = NULL; 
                arr_keys[i] = current -> key;
                arr_values[i] = current -> value;
            	i++;
            	//Breaks the link and sets the correct, current->right
                current = current->right; 
            } 
        } 
    } 
} 

//Declaration of printing functions..
void PrintAVLTree(AVLTree *T);
AVLTreeNode *Search(AVLTree *T, int k, int v);  
int InsertNode(AVLTree *T, int k, int v);

//O(nlog(n))
//1) Create AVL from a file...
// put your time complexity analysis of CreateAVLTree() here
/*All the standard library functions take o(1) time as soecified in the assignment..
1)while loop with strsep takes O(cn) where c is a constant
2)For loop with a nested while loop also takes O(cn) time becuase the digits of a input will 
always be constant.
3)Inserting a node takes Log(n) time for every n therefore total time of this function becaomes
O(nlog(n)) time.  */
AVLTree *CreateAVLTree(const char *filename)
{

	if ((strcmp(filename, "stdin")) == 0){
		AVLTree *final_return_tree;	
		final_return_tree = newAVLTree();
	    int l = 0, c, bytes; // Numbers is to store the input as ints in the array 
	    char input[1000];					//character string to store the chars.
	    fgets(input, sizeof(input), stdin); // fgets to get the inout, size and from stdin.
	    char *string,*found;
	    string = strdup(input);
	    char* final[1000];
	    int i = 0;
		while((found = strsep(&string,"()")) != NULL ){ //Seperating the string on "()" so that we only get the numbers inside.
		  if (is_empty(found) == 0){
		  	 //Storing each set in final. 
		     final[i] = found;
		     i++;
		  }	  
		}
		int j = 0;

		int looper = 0;
		int numbers_1[1000];
		int length = 0;
		//Loop to convert chars to numbers using atoi function.
		for( j = 0; j < i; j++){
		  while((found = strsep(&final[j], ","))){
		     numbers_1[length] = atoi(found);
		     length++;
		  }
		}


		while (looper < length)
		{
			//printf("%d, %d\n",numbers[looper], numbers[looper+1]);
			InsertNode(final_return_tree, numbers_1[looper], numbers_1[looper+1]);
			looper = looper +2;
			
		}
		final_return_tree -> size = length/2;
		return final_return_tree;
	}
	//case where the input is from a file and not stdin. 
	//Input for a file here...
	//AVL tree intialisation.
	AVLTree *final_return_tree;
	final_return_tree = newAVLTree();
	//File type. 
	FILE *fp;
	// For saving the characters in the array. 
    char output[1000];
    //Opening the file for reading and saving it in fp.
    fp =fopen(filename,"r");   
    //Storing in a string format in the char array.

    int bytes_read = fread(output,1,1000000,fp);
    fclose(fp);

	char *string,*found;
	//Making a duplicate the string pointer to point towards the ouput.
	string = strdup(output);
	char* final[1000];
	int i = 0;
	//seperating a string based on '()'

	while((found = strsep(&string,"()")) != NULL ){
	  if (is_empty(found) == 0){

	     final[i] = found;
	     i++;
	  }
	}

	int j=0 ;
	while(j<i-1){
		j++;
	}
	int numbers[1000];
	int length = 0;
	for(int j = 0; j < i-1; j++){
	  while((found = strsep(&final[j], ","))){
	     numbers[length] = atoi(found);
	     length++;
	  }
	}

	int looper = 0;
	while (looper < length)
	{
		//Inserting the nodes into a tree. 
		InsertNode(final_return_tree, numbers[looper], numbers[looper+1]);
		looper = looper +2;
		
	}
	final_return_tree -> size = length/2;
	//Returning the final_return_tree 
	return final_return_tree;

}

//O(N)
//2) Cloning of an AVL tree...
// put your time complexity analysis for CloneAVLTree() here
/*With constant time functions, taking O(1) time the recursion goes over every node therefore takes O(n) time*/

//Helper function takes in a node and returns a node with all the links as the input node.
AVLTreeNode *CloneAVLTree_node(AVLTreeNode *node){
	if (node == NULL){
		return NULL;
	}
	//Creating the clone_node for cloning the original.
	AVLTreeNode *temp; 
	//Assigning Heap space for the node. 
	temp = malloc(sizeof(AVLTreeNode));
	temp -> key = node -> key;
	temp -> value = node -> value;
	temp -> height = node -> height;
	temp -> left = CloneAVLTree_node(node -> left);
	temp -> right = CloneAVLTree_node(node -> right);
	return temp;
}
//Main function 
AVLTree *CloneAVLTree(AVLTree *T)
{
	if (T == NULL){
		return NULL;
	}
	else{
		AVLTree *final_return_tree;
		final_return_tree= newAVLTree();
		final_return_tree -> root = CloneAVLTree_node(T->root);
		final_return_tree -> size = T -> size;
		return final_return_tree;
	}
}

//O((m+n)log(m+n))
//3) UNION of 2 treees.
// Helper function taking O(m+n) time to complete the traversal....
int union_arrays(int keys_1[], int vals_1[], int keys_2[], int vals_2[], int final_keys[], int final_vals[], int size_1, int size_2)
{
	//intializing the variables 
	int i =0; //Keeps track of the loop for Tree 1. 
	int j =0; //Keeps track of the loop for Tree 2.
	int counter = 0;
	int length = 0;
	//Maximum loop would be m + n. 
	while(counter < size_1 +size_2 ){
		/*If the key and values of both the nodes are equal storing 1 value in the 
		final_keys array with the value in final keys and increasing i and j by 1*/
		if (keys_1[i] == keys_2[j] && vals_1[i] == vals_2[j] && (i < size_1 && j< size_2)){
			final_keys[length] = keys_1[i];
			final_vals[length] = vals_1[i];
			length ++;
			i++;
			j++;
		}
		/* Comparing the keys of the two trees that are stored in an array
		Increasing the loop of the first variable if the key is not there in
		the second array of the tree2*/
		else if (keys_1[i] < keys_2[j] && (i < size_1 && j< size_2)){
			final_keys[length] = keys_1[i];
			final_vals[length] = vals_1[i];
			length++;
			i++;
		}
		//comparing the keys and increasing the one which has a lower key 
		else if (keys_1[i] > keys_2[j] && (i < size_1 && j< size_2)){
			final_keys[length] = keys_2[j];
			final_vals[length] = vals_2[j];
			length++;
			j++;
		}
		//Comparing values now.
		else if(keys_1[i] == keys_2[j] && vals_1[i] < vals_2[j] && (i < size_1 && j< size_2)){
			final_keys[length] = keys_1[i];
			final_vals[length] = vals_1[i];
			length++;
			i++;			
		}
		//Comparing values now
		else if(keys_1[i] == keys_2[j] && vals_1[i] > vals_2[j] && (i < size_1 && j< size_2)){
			final_keys[length] = keys_2[j];
			final_vals[length] = vals_2[j];
			length++;
			j++;			
		}
		// Keeping the track of the outer loop. 
		counter++;
		/*Storing The rest of the elements of in the final value if one of the trees reach its
		last value while taking union we still need to store the elements of the other tree which
		might not have reached it's end*/
		//Case:1
		if (i == size_1-1 && j!= size_2-1){
			while(j != size_2){
				final_keys[length] = keys_2[j];
				final_vals[length] = vals_2[j];
				j++;
				length++;
			}
			break;
		}
		//Case:2 
		if (j == size_2-1 && i!= size_1-1){ //Change -1 to 0 for testing code.. 
			while(i != size_1){
				final_keys[length] = keys_1[i];
				final_vals[length] = vals_1[i];
				i++;
				length++;
			}
			break;
		}
	}

	//RETURNING THE LENGTH OF THE NEW TREE..
	return length;
}


//O((m+n)log(m+n))
// put your time complexity for ALVTreesUNion() here: 
/*the whole function takes O(m+n) time. completes in one whole loop nut inserting every node takes log(m+n)
time therefore the whole function (m+n)log(m+n) time. 
*/
void *AVLTreesUnion(AVLTree *T1, AVLTree *T2)
{
	//Making a new tree.
	AVLTree *final_return_tree;
	final_return_tree = newAVLTree();
	//Making array to store the values inthe arrays as input for our helper function
	int array_1_keys[T1->size], array_1_values[T1->size];
	int array_2_keys[T2->size],array_2_values[T2->size];
	int final_keys[T1->size + T2->size], final_vals[T1->size + T2->size];
	int length_final = 0;
	traversal_final(T1->root, array_1_keys, array_1_values,T1->size);   //Helper function takes o(n) time  
	traversal_final(T2->root, array_2_keys, array_2_values,T2->size);   //Helper function takes o(m) time
	length_final = union_arrays(array_1_keys, array_1_values,array_2_keys, array_2_values,final_keys, final_vals, T1->size, T2->size); // Helper function takes O(m+n) time
	//Inserting nodes in the final tree.
	for (int i = 0; i < length_final; i++)
	{
		InsertNode(final_return_tree, final_keys[i], final_vals[i]);
	}
	final_return_tree -> size = length_final;
	return final_return_tree;										
}
 

//O((m+n)log(m+n))
//4) Intersection of 2 trees. 
/*Helper function for intersection of nodes for findig the intersections when we enter 2 
4 arrays, 2 with keys and values from 1st tree and the other for the second tree.*/

int intersection_arrays(int keys_1[], int vals_1[], int keys_2[], int vals_2[], int final_keys[], int final_vals[], int size_1, int size_2)
{
	int i =0;
	int j =0;
	int counter = 0;
	int length = 0;
	while (counter < size_1 +size_2){
		/*Storing the values in the array final_keys and final_vals
		 If they are common in both the tree - arrays and then increasing 
		 both i and j loopers.  */
		if (keys_1[i] == keys_2[j] && vals_1[i] == vals_2[j]){
			final_keys[length] = keys_1[i];
			final_vals[length] = vals_1[j];
			i++;
			j++;
			length++;
		}
		else if(keys_1[i] < keys_2[j]){
			i++;
		}
		else if(keys_1[i] > keys_2[j]){
			j++;
		}
		else if(keys_1[i] == keys_2[j] && vals_1[i] < vals_2[j]){
			i++;
		}
		else if(keys_1[i] == keys_2[j] && vals_1[i] > vals_2[j]){
			j++;
		}
		if (i == size_1  || j ==size_2){
			break;
		}
		counter++;
	}
	return length;
}	

//O((m+n)log(m+n))
// put your time complexity for ALVTreesIntersection() here 
/*This function takes O(m+n) time for finding the intersection in arrays and log(m+n) to enter every node. */
void *AVLTreesIntersection(AVLTree *T1, AVLTree *T2)
{
	AVLTree *final_return_tree;
	final_return_tree = newAVLTree();
	//arrays for storing the values of the intersection in the final_keys and final_vals.
	int array_1_keys[T1->size], array_1_values[T1->size];
	int array_2_keys[T2->size],array_2_values[T2->size];
	int final_keys[T1->size + T2->size], final_vals[T1->size + T2->size];
	//Function for storing values in the arrays.
	traversal_final(T1->root, array_1_keys, array_1_values,T1->size);   //Helper function takes o(n) time  
	traversal_final(T2->root, array_2_keys, array_2_values,T2->size);   //Helper function takes o(m) time	 
	int length_final;
	//Calling the helper functions and inputting the keys and the vals of the tree_1 and tree_2 and storing in the final_keys and final_vals.
	length_final = intersection_arrays(array_1_keys,array_1_values,array_2_keys, array_2_values, final_keys, final_vals, T1 -> size, T2 ->size);
	for (int i = 0; i < length_final; i++)
	{
		InsertNode(final_return_tree, final_keys[i], final_vals[i]);
	}	
	final_return_tree -> size = length_final;
	return final_return_tree;	
}

//Log(N)
// 5)  Insertion of a node...
//Helper function for inserting..
AVLTreeNode *InsertNode_node_AUX_1(AVLTreeNode *node, int k, int v)
{
	//If node is null make a new node.
	if (node == NULL){
		node = newAVLTreeNode(k, v);
		return node;
	}
	//node's key is less than k entered so go right. 
	if (node-> key < k){
		node->right = InsertNode_node_AUX_1(node -> right, k,v);
	}
	//node's key is greater than k therfore it recurses towards the left
	else if (node -> key > k){
		node -> left = InsertNode_node_AUX_1(node -> left, k, v);
	}
	//comparing the values now. 
	else if(node ->key == k && node-> value < v){
		node->right = InsertNode_node_AUX_1(node -> right, k,v);
	}
	//Comparing values now. 
	else if(node->key == k && node -> value > v){
		node -> left = InsertNode_node_AUX_1(node -> left, k, v);
	}

	//Height diff variable for storing the height of each code and balancing the trees if needed 
	int height_diff;
	node -> height = 1 + max(height(node->left), height(node->right));
	height_diff = height(node->left) - height(node -> right);
	//1 of the 2 conditions in an imbalanced AVL tree..
	//If the Balance of the node is less than -1, comparing keys and values will make them into 4 cases.

	if(height_diff <  -1){
		// This consists of 4 conditions 
		//case 1
		if (k > node -> right-> key){
			return left_rotate(node);
		}
		//case 2
		if (k < node->right->key){
			node->right = right_rotate(node->right);
			return left_rotate(node);
		}

		else if (k == node->right->key){
			//case 3
			if (v > node -> right->value){
				return left_rotate(node);
			}
			//case 4
			else if(v < node->right->value){
				node->right = right_rotate(node->right);
				return left_rotate(node);				
			}
		}
	}

	//2nd condition when balance is more than equal to 1..
	// 4 more cases to be considered in this
	if (height_diff > 1){
		if (k < node->left->key){
			return right_rotate(node);
		}
		if (k > node->left->key){
			node -> left = left_rotate(node->left);
			return right_rotate(node);
		}
		else if (k == node -> left -> key){
			if (v < node->left->value){
				return right_rotate(node);
			}
			else if (v > node -> left->key){
				node -> left = left_rotate(node->left);
				return right_rotate(node);				
			}
		}
	}
	return node;
}
// Main Function for inserting nodes into the binary tree.
int InsertNode(AVLTree *T, int k, int v)
{	
	if (Search(T, k,v) == NULL){
	T->root = InsertNode_node_AUX_1(T->root, k, v);	
	T->size ++;
	
	return 1;
	}
	else{
		return 0;
	}
}

//Log(N)
//6) Deletion of nodes..
/*Finding the right most node of the array in the current node;
Used when we need to delete a node which has a right subtree. */
AVLTreeNode *right_most_node(AVLTreeNode *node)
{
	AVLTreeNode *right_most_node;
	right_most_node = node;
	while(right_most_node -> right != NULL){
		right_most_node = right_most_node -> right;
	}
	return right_most_node;
}


//Log(N)
//This is a helper function and takes a node as an input and recursively reaches the node and then deletes it. 
//This takes log(n) time same as the insert node because it is similar in running to the insert node func
/*Helper function goes through the tree until it finds the */
AVLTreeNode *DEL_node_AUX(AVLTreeNode *node, int k, int v)
{

	if (node-> key < k){
		node->right = DEL_node_AUX(node -> right, k,v);
	}
	else if (node -> key > k){
		node -> left = DEL_node_AUX(node -> left, k, v);
	}

	else if(node ->key == k && node-> value < v){
		node->right = DEL_node_AUX(node -> right, k,v);
	}
	else if(node->key == k && node -> value > v){
		node -> left = DEL_node_AUX(node -> left, k, v);
	}

	else if (node -> key == k && node -> value == v){
		//Case where both left and righ subtrees are null
		if ( node -> right == NULL && node -> left == NULL){
			AVLTreeNode *temp;
			temp = node;
			node = NULL;
			free(temp);
		}
		else if (node -> left == NULL || node -> right == NULL){
			AVLTreeNode *temp = node->left ? node->left : node->right; 
			*node = *temp;
			free(temp);
		}
		//Case when no subtrees are NULL...
		else if(node -> left != NULL && node ->right != NULL){
			AVLTreeNode *temp;
			temp = right_most_node(node -> left);
			node -> key = temp ->key;
			node -> value = temp -> value;
			node -> left = DEL_node_AUX(node -> left, temp -> key, temp -> value);

		}


	}
	if (node == NULL){
		return node;
	}
	//Height diff variable for storing the height of each code and balancing the trees if needed 
	int height_diff;
	//Updating the height of the node. 
	node -> height = 1 + max(height(node->left), height(node->right));
	height_diff = height(node->left) - height(node -> right);
	//2 Variables for storing height difference of the children of the particular node.
	int height_diff_left, height_diff_right;
	height_diff_left = height_diff_function(node->left);
	height_diff_right = height_diff_function(node->right);
	//Rotating to balance the trees.
	if (height_diff < -1){
		if(height_diff_right <= 0){
			AVLTreeNode *temp;
			temp = left_rotate(node);
			return temp;
		}

		if (height_diff > 0){
			AVLTreeNode *temp;
			node -> right = right_rotate(node ->right);
			temp = left_rotate(node);
			return temp;
		}
	}

	if (height_diff > 1){
		if (height_diff_left >= 0){
			AVLTreeNode *temp;
			temp = right_rotate(node);
			return temp;
		}

		if (height_diff_left < 0){
			AVLTreeNode *temp;
			node -> left = left_rotate(node -> left);
			temp = right_rotate(node);
			return temp;
		}
	}

	return node;
}

//Main function for deleting a particular node from the tree.
int DeleteNode(AVLTree *T, int k, int v)
{
	//if node is null then just return 0
	AVLTreeNode *root_1;
	if (T-> root == NULL || T == NULL){
		return 0;
	}	
	if (Search(T, k, v) == NULL){
		return 0;
	}
	//if the recursion has reached the node it wants to delete. 
	if(T-> root -> key == k && T -> root -> value == v){
		//If the left subtree is not null and the right is also not null..
		if(T-> root -> left != NULL && T-> root ->right != NULL){
			AVLTreeNode *temp;
			//Finding the right most node and saving it as the root.
			temp = right_most_node(T-> root -> left);
			T-> root -> key = temp ->key;
			T-> root -> value = temp -> value;
			//And saving the rest of the connected subtree to this. and deleting the repeatitive node. 
			T-> root -> left = DEL_node_AUX(T-> root -> left, temp -> key, temp -> value);

		}
		//Right subtree is NULL
		else if (T-> root -> left != NULL && T-> root -> right == NULL){
					AVLTreeNode  *temp = T-> root -> left;
					free(T-> root);
					T -> root =  temp;
				}
		//Leftsubtree is null.
		else if (T-> root -> right != NULL && T-> root -> left == NULL){
			AVLTreeNode  *temp = T-> root ->right;
			free(T-> root);
			T-> root = temp;
		}
		//Is a leap node and has no subtrees.
		else if (T-> root -> right == NULL && T-> root -> left == NULL){
			AVLTreeNode *temp;
			temp = T-> root;
			T-> root = NULL;
			free(temp);
		}
		return 1;
	}
	else{
		DEL_node_AUX(T->root, k, v);
	 	return 1;
	}	
}

//Log(N)
//7) search of nodes is in tree
//Auxilary function which takes the node as the input. 
/*This function takes log(n) time for completion because this goes over a balanced binary tree.*/

AVLTreeNode *Search(AVLTree *T, int k, int v)
{	
	//Return NULL if input is NULL
	if (T-> root == NULL || T == NULL){
		return NULL;
	}	
	//Recursive function for searching through the nodes. 
	else{
		AVLTreeNode *pointer, *root;
		root = T->root;
		pointer = T-> root;
		//While loop to see if the node is not null.
		while (pointer != NULL){
			//Comparing the keys and the values of the node and then getting the value.
			if ( pointer -> key > k){
				pointer = pointer -> left;
			}
			else if (pointer -> key < k){
				pointer = pointer -> right;
			}
			else if ( pointer -> key == k && pointer -> value > v){
				pointer = pointer -> left;
			}
			else if ( pointer -> key == k && pointer -> value  < v){
				pointer = pointer -> right;
			}

			else{
				return pointer;
			}
		}

	}
	return NULL;
}


// O(N).
//8) Freeing of the memory of a BST
/*Freeing of the nodes, Helper functions takes the root of the tree as an arguement
goes to every node in a post order fashion and the frees the nodes in O(N) time then 
the main function frees the tree as well. */
void FreeAVLTree_node(AVLTreeNode *node)
{
    if (node == NULL) return; 
    FreeAVLTree_node(node->left); 
    FreeAVLTree_node(node->right); 
    
    AVLTreeNode *temp;
    temp = node; 
    free(temp); 
}
//Main function....
void FreeAVLTree(AVLTree *T)
{
	if (T == NULL){
		return;
	}
	else{
		FreeAVLTree_node(T-> root);
		//free the tree ...
		free(T);
	}
}

//O(N)
//9) Printing the tree in order..
//Helper function for printing the tree inorder. 
// put your time complexity analysis for PrintAVLTree() here
/*Printing the nodes checking if node is NULL then this function won't print anything
Goes to the leftmost node first prints the node and then goes to the right node recursively.
This function as O(n) time. */

//Helper function 
void PrintAVLTree_node(AVLTreeNode *node)
{
	if (node != NULL){
		//going for the left node first 
		PrintAVLTree_node(node -> left);
		printf("(%d, %d) %d\n", node-> key, node -> value, node->height-1);
		//going for the right node next.
		PrintAVLTree_node(node -> right);
	}
}


//Main function for printing the tree. With O(n) time. 
void PrintAVLTree(AVLTree *T)
{
	if (T == NULL){
		return;
	}
	PrintAVLTree_node(T->root);
}

int main() //sample main for testing 
{ int i,j;
 AVLTree *tree1, *tree2, *tree3, *tree4, *tree5, *tree6, *tree7, *tree8;
 AVLTreeNode *node1;
 
 tree1=CreateAVLTree("stdin");
 PrintAVLTree(tree1);
 printf("\n");
 FreeAVLTree(tree1);
 //you need to create the text file file1.txt
 // to store a set of items without duplicate items
 tree2=CreateAVLTree("file1.txt"); 
 PrintAVLTree(tree2);
 printf("\n");
 tree3=CloneAVLTree(tree2);
 PrintAVLTree(tree3);
 printf("\n");
 FreeAVLTree(tree2);
 FreeAVLTree(tree3);
 //Create tree4 
 tree4=newAVLTree();
 j=InsertNode(tree4, 10, 10);
 for (i=0; i<15; i++)
  {
   j=InsertNode(tree4, i, i);
   if (j==0) printf("(%d, %d) already exists\n", i, i);
  }
  PrintAVLTree(tree4);
  printf("\n");
  node1=Search(tree4,20,20);
  if (node1!=NULL)
    printf("key= %d value= %d\n",node1->key,node1->value);
  else 
    printf("Key 20 does not exist\n");
  
  for (i=17; i>0; i--)
  {
  	/*Please see that my delete function works for half the tree and loses the left 
  	tree for some reason.. I tried but was not able to get the correct output*/
    j=DeleteNode(tree4, i, i);
	if (j==0) 
	  printf("Key %d does not exist\n",i);  
    PrintAVLTree(tree4);
  }
 FreeAVLTree(tree4);
 //Create tree5
 tree5=newAVLTree();
 j=InsertNode(tree5, 6, 25);
 j=InsertNode(tree5, 6, 10);
 j=InsertNode(tree5, 6, 12);
 j=InsertNode(tree5, 6, 20);
 j=InsertNode(tree5, 9, 25);
 j=InsertNode(tree5, 10, 25);
 PrintAVLTree(tree5);
 printf("\n");
 //Create tree6
 tree6=newAVLTree();
 j=InsertNode(tree6, 6, 25);
 j=InsertNode(tree6, 5, 10);
 j=InsertNode(tree6, 6, 12);
 j=InsertNode(tree6, 6, 20);
 j=InsertNode(tree6, 8, 35);
 j=InsertNode(tree6, 10, 25);
 PrintAVLTree(tree6);
 printf("\n");
 tree7=AVLTreesIntersection(tree5, tree6);
 tree8=AVLTreesUnion(tree5,tree6);
 printf("This is tree intersection: \n");
 PrintAVLTree(tree7);
 printf("\n");
 printf("This is the tree union\n" );
 PrintAVLTree(tree8);
 printf("\n");
 return 0; 
}
