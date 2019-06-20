PriorityQueue Initialize(int MaxElements) {
    PriorityQueue H;
 
    /* 1*/ if (MaxElements < MinPQSize)         
    /* 2*/ Error("Priority queue size is too small");     
    /* 3*/ H = malloc(sizeof ( struct HeapStruct));     
    /* 4*/ if (H == NULL)         
    /* 5*/ FatalError("Out of space!!!");     /* Allocate the array plus one extra for sentinel */     /* 6*/ H->Elements = malloc((MaxElements + 1)
            * sizeof ( ElementType));
    /* 7*/ if (H->Elements == NULL)
        /* 8*/ FatalError("Out of space!!!");
 
    /* 9*/ H->Capacity = MaxElements;
    /*10*/ H->Size = 0;
    /*11*/ H->Elements[ 0 ] = MinData;
 
    /*12*/ return H;
}