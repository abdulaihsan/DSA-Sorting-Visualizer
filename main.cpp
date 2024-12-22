#include<SDL2/SDL.h>
#include<iostream>
#include<limits>
#include<time.h>
#include<string.h>


using namespace std;

const int SCREEN_WIDTH=910;
const int SCREEN_HEIGHT=750;

const int arrSize=130;
const int rectSize=7;

int arr[arrSize];
int Barr[arrSize];


struct Node {
    int data;
    Node* next;
};


SDL_Window* window=NULL;
SDL_Renderer* renderer=NULL;

bool complete=false;


//INITIALIZE SDL
bool init()
{
    bool success=true;
    if(SDL_Init(SDL_INIT_VIDEO)<0)
    {
        cout<<"Couldn't initialize SDL. SDL_Error: "<<SDL_GetError();
        success=false;
    }
    else
    {
        if(!(SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")))
        {
            cout<<"Warning: Linear Texture Filtering not enabled.\n";
        }

        window=SDL_CreateWindow("Sorting Visualizer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if(window==NULL)
        {
            cout<<"Couldn't create window. SDL_Error: "<<SDL_GetError();
            success=false;
        }
        else
        {
            renderer=SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
            if(renderer==NULL)
            {
                cout<<"Couldn't create renderer. SDL_Error: "<<SDL_GetError();
                success=false;
            }
        }
    }

    return success;
}

//CLOSE SDL
void close()
{
    SDL_DestroyRenderer(renderer);
    renderer=NULL;

    SDL_DestroyWindow(window);
    window=NULL;

    
    SDL_Quit();
}

//VISUALIZATION FOR ARRAYS
void visualize(int x=-1, int y=-1, int z=-1)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    int j=0;
    for(int i=0; i<=SCREEN_WIDTH-rectSize; i+=rectSize)
    {
        SDL_PumpEvents();

        SDL_Rect rect={i, 0, rectSize, arr[j]};
        if(complete)
        {
            
            SDL_SetRenderDrawColor(renderer, 100, 180, 100, 0);
            SDL_RenderDrawRect(renderer, &rect);
        }
        else if(j==x || j==z)
        {
          
            SDL_SetRenderDrawColor(renderer, 100, 180, 100, 0);
            SDL_RenderFillRect(renderer, &rect);
        }
        else if(j==y)
        {
            
            SDL_SetRenderDrawColor(renderer, 165, 105, 189, 0);
            SDL_RenderFillRect(renderer, &rect);
        }
        else
        {
            
            SDL_SetRenderDrawColor(renderer, 170, 183, 184, 0);
            SDL_RenderDrawRect(renderer, &rect);
        }
        j++;
    }
    SDL_RenderPresent(renderer);
}

//VISUALIZATION FOR LINKED LIST
void visualizeLinkedList(Node* head, Node* x = nullptr, Node* y = nullptr, bool sorted = false)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    Node* temp = head;
    int j = 0;
    while (temp != nullptr)
    {
        SDL_PumpEvents();

        SDL_Rect rect = {j * rectSize, 0, rectSize, temp->data};
        if (sorted)
        {
            SDL_SetRenderDrawColor(renderer, 100, 180, 100, 0);
            SDL_RenderDrawRect(renderer, &rect);
        }
        else if (temp == x || temp == y)
        {
            SDL_SetRenderDrawColor(renderer, 165, 105, 189, 0);
            SDL_RenderFillRect(renderer, &rect);
        }
        else
        {
            SDL_SetRenderDrawColor(renderer, 170, 183, 184, 0);
            SDL_RenderDrawRect(renderer, &rect);
        }

        temp = temp->next;
        j++;
    }
    SDL_RenderPresent(renderer);
}

//BUBBLE SORT LINKED LIST
void bubbleSortLinkedList(Node* head)
{
    if (head == nullptr)
        return;

    bool swapped;
    Node* ptr1;
    Node* lptr = nullptr;

    do
    {
        swapped = false;
        ptr1 = head;

        while (ptr1->next != lptr)
        {
            if (ptr1->data > ptr1->next->data)
            {
                swap(ptr1->data, ptr1->next->data);
                swapped = true;
            }

            visualizeLinkedList(head, ptr1, ptr1->next);
            SDL_Delay(5);
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while (swapped);

    
    visualizeLinkedList(head, nullptr, nullptr, true);
}

//SELECTION SORT LINKED LIST
void selectionSortLinkedList(Node* head)
{
    if (!head) return;

    Node* current = head;
    while (current)
    {
        Node* minNode = current;
        Node* checker = current->next;

        while (checker)
        {
            if (checker->data < minNode->data)
            {
                minNode = checker;
            }
            checker = checker->next;
        }

        if (minNode != current)
        {
            swap(current->data, minNode->data);
            visualizeLinkedList(head, current, minNode);
            SDL_Delay(50);
        }

        current = current->next;
    }
}

//MERGE SORT LINKED LIST
void splitLinkedList(Node* head, Node** front, Node** back)
{
    if (!head || !head->next)
    {
        *front = head;
        *back = nullptr;
        return;
    }

    Node* slow = head;
    Node* fast = head->next;

    while (fast && fast->next)
    {
        slow = slow->next;
        fast = fast->next->next;
    }

    *front = head;
    *back = slow->next;
    slow->next = nullptr;

    visualizeLinkedList(*front, *back, nullptr);
    SDL_Delay(20); 
}
Node* mergeSortedLists(Node* a, Node* b)
{
    if (!a) return b;
    if (!b) return a;

    Node* result = nullptr;

    if (a->data <= b->data)
    {
        result = a;
        result->next = mergeSortedLists(a->next, b);
    }
    else
    {
        result = b;
        result->next = mergeSortedLists(a, b->next);
    }

    visualizeLinkedList(result, a, b); 
    SDL_Delay(20); 
    return result;
}
void mergeSortLinkedList(Node** headRef)
{
    Node* head = *headRef;

    if (!head || !head->next)
    {
        return;
    }

    Node* front = nullptr;
    Node* back = nullptr;

    splitLinkedList(head, &front, &back);

    mergeSortLinkedList(&front);
    mergeSortLinkedList(&back);

    *headRef = mergeSortedLists(front, back);

    visualizeLinkedList(*headRef, nullptr, nullptr); 
    SDL_Delay(20);
}

//QUICK SORT LINKED LIST
Node* getLastNode(Node* head) {
    while (head != nullptr && head->next != nullptr) {
        head = head->next;
    }
    return head;
}
Node* getPreviousNode(Node* start, Node* target) {
    Node* prev = nullptr;
    while (start != nullptr && start != target) {
        prev = start;
        start = start->next;
    }
    return prev;
}
Node* partition(Node* low, Node* high, Node* head) {
    int pivot = high->data; // Choose pivot as the last element
    Node* i = nullptr;      // Pointer for elements smaller than pivot
    Node* j = low;

    while (j != high) {
        if (j->data <= pivot) {
            if (i == nullptr) {
                i = low; // Initialize i at low for the first swap
            } else {
                i = i->next;
            }

            // Swap data between i and j
            swap(i->data, j->data);
            visualizeLinkedList(head, i, j);
            SDL_Delay(10);
        }
        j = j->next;
    }
    // Place the pivot in its correct position
    if (i == nullptr) {
        i = low;
    } else {
        i = i->next;
    }
    swap(i->data, high->data);
    visualizeLinkedList(head, i, high);
    SDL_Delay(10);

    return i; // Return pivot node
}
void quickSortLinkedList(Node* low, Node* high, Node* head) {
    if (low != nullptr && high != nullptr && low != high && low != high->next) {
        Node* p = partition(low, high, head);

        // Recursive calls for left and right parts
        quickSortLinkedList(low, getPreviousNode(low, p), head);
        quickSortLinkedList(p->next, high, head);
    }
}
void startQuickSort(Node* head) {
    Node* last = getLastNode(head);
    quickSortLinkedList(head, last, head);
    complete = true;
    visualizeLinkedList(head, nullptr, nullptr, true);
    SDL_Delay(30);
}

//HEAP SORT ARRAY
void inplaceHeapSort(int* input, int n)
{
    for(int i=1; i<n; i++)
    {
       int childIndex=i;
       int parentIndex=(childIndex-1)/2;

       while(childIndex>0)
       {
           if(input[childIndex]>input[parentIndex])
           {
               int temp=input[parentIndex];
               input[parentIndex]=input[childIndex];
               input[childIndex]=temp;

           }
           else
           {
               break;
           }

           visualize(parentIndex, childIndex);
           SDL_Delay(40);

           childIndex=parentIndex;
           parentIndex=(childIndex-1)/2;
       }
    }

    for(int heapLast=n-1; heapLast>=0 ; heapLast--)
    {
        int temp=input[0];
        input[0]=input[heapLast];
        input[heapLast]=temp;

        int parentIndex=0;
        int leftChildIndex=2*parentIndex + 1;
        int rightChildIndex=2*parentIndex + 2;

        while(leftChildIndex<heapLast)
        {
            int maxIndex=parentIndex;

            if(input[leftChildIndex]>input[maxIndex])
            {
                maxIndex=leftChildIndex;
            }
            if(rightChildIndex<heapLast && input[rightChildIndex]>input[maxIndex])
            {
                maxIndex=rightChildIndex;
            }
            if(maxIndex==parentIndex)
            {
                break;
            }

            int temp=input[parentIndex];
            input[parentIndex]=input[maxIndex];
            input[maxIndex]=temp;

            visualize(maxIndex, parentIndex, heapLast);
            SDL_Delay(40);

            parentIndex=maxIndex;
            leftChildIndex=2*parentIndex + 1;
            rightChildIndex=2*parentIndex + 2;
        }
    }
}

//QUICK SORT ARRAY
int partition_array(int a[], int si, int ei)
{
    int count_small=0;

    for(int i=(si+1);i<=ei;i++)
    {
        if(a[i]<=a[si])
        {
            count_small++;
        }
    }
    int c=si+count_small;
    int temp=a[c];
    a[c]=a[si];
    a[si]=temp;
    visualize(c, si);

    int i=si, j=ei;

    while(i<c && j>c)
    {
        if(a[i]<= a[c])
        {
            i++;
        }
        else if(a[j]>a[c])
        {
            j--;
        }
        else
        {
            int temp_1=a[j];
            a[j]=a[i];
            a[i]=temp_1;

            visualize(i, j);
            SDL_Delay(70);

            i++;
            j--;
        }
    }
    return c;
}
void quickSort(int a[], int si, int ei)
{
    if(si>=ei)
    {
        return;
    }

    int c=partition_array(a, si, ei);
    quickSort(a, si, c-1);
    quickSort(a, c+1, ei);

}

//MERGE SORT ARRAY
void merge2SortedArrays(int a[], int si, int ei)
{
    int size_output=(ei-si)+1;
    int* output=new int[size_output];

    int mid=(si+ei)/2;
    int i=si, j=mid+1, k=0;
    while(i<=mid && j<=ei)
    {
        if(a[i]<=a[j])
        {
            output[k]=a[i];
            visualize(i, j);
            i++;
            k++;
        }
        else
        {
            output[k]=a[j];
            visualize(i, j);
            j++;
            k++;
        }

    }
    while(i<=mid)
    {
        output[k]=a[i];
        visualize(-1, i);
        i++;
        k++;
    }
    while(j<=ei)
    {
        output[k]=a[j];
        visualize(-1, j);
        j++;
        k++;
    }
    int x=0;
    for(int l=si; l<=ei; l++)
    {
        a[l]=output[x];
        visualize(l);
        SDL_Delay(15);
        x++;
    }
    delete []output;
}
void mergeSort(int a[], int si, int ei)
{
    if(si>=ei)
    {
        return;
    }
    int mid=(si+ei)/2;

    mergeSort(a, si, mid);
    mergeSort(a, mid+1, ei);

    merge2SortedArrays(a, si, ei);
}

//BUBBLE SORT ARRAY
void bubbleSort()
{
    for(int i=0; i<arrSize-1; i++)
    {
        for(int j=0; j<arrSize-1-i; j++)
        {
            if(arr[j+1]<arr[j])
            {
                int temp=arr[j];
                arr[j]=arr[j+1];
                arr[j+1]=temp;

                visualize(j+1, j, arrSize-i);
            }
            SDL_Delay(1);
        }
    }
}

//INSERTION SORT ARRAY
void insertionSort()
{
    for(int i=1; i<arrSize; i++)
    {
        int j=i-1;
        int temp=arr[i];
        while(j>=0 && arr[j]>temp)
        {
            arr[j+1]=arr[j];
            j--;

            visualize(i, j+1);
            SDL_Delay(5);
        }
        arr[j+1]=temp;
    }
}

//SELECTION SORT ARRAY
void selectionSort()
{
    int minIndex;
    for(int i=0;i<arrSize-1;i++)
    {
        minIndex=i;
        for(int j=i+1;j<arrSize;j++)
        {
            if(arr[j]<arr[minIndex])
            {
                minIndex=j;
                visualize(i, minIndex);
            }
            SDL_Delay(1);
        }
        int temp=arr[i];
        arr[i]=arr[minIndex];
        arr[minIndex]=temp;
    }
}

//FLASH SORT ARRAY
void flashSort()
{
    int n = arrSize;
    int m = int(0.43 * n);
    int* L = new int[m];
    memset(L, 0, m * sizeof(int));

    int minVal = arr[0], maxIdx = 0;

    for (int i = 1; i < n; i++) {
        if (arr[i] < minVal) {
            minVal = arr[i];
        }
        if (arr[i] > arr[maxIdx]) {
            maxIdx = i;
        }
    }

    if (arr[maxIdx] == minVal) {
        delete[] L;
        return; 
    }

    double c = (double)(m - 1) / (arr[maxIdx] - minVal);
    for (int i = 0; i < n; i++) {
        int k = int(c * (arr[i] - minVal));
        L[k]++;
        visualize(i, k);
        SDL_Delay(30);
    }

    for (int i = 1; i < m; i++) {
        L[i] += L[i - 1];
    }

    int count = 0, i = 0;
    while (count < n) {
        int k = int(c * (arr[i] - minVal));
        while (i >= L[k]) {
            k = int(c * (arr[++i] - minVal));
        }
        int temp = arr[i];
        while (i < L[k]) {
            k = int(c * (temp - minVal));
            int t = arr[--L[k]];
            arr[L[k]] = temp;
            temp = t;
            visualize(i, L[k]);
            SDL_Delay(30);
            count++;
        }
    }

    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
            visualize(i, j);
            SDL_Delay(20);
        }
        arr[j + 1] = key;
    }

    delete[] L;
}

//COUNTING SORT ARRAY
void countingSort()
{
    int maxVal = arr[0];
    for (int i = 1; i < arrSize; i++)
    {
        if (arr[i] > maxVal)
            maxVal = arr[i];
    }

    int* count = new int[maxVal + 1]();
    for (int i = 0; i < arrSize; i++)
    {
        count[arr[i]]++;
        visualize(i);
        SDL_Delay(30);
    }

    int index = 0;
    for (int i = 0; i <= maxVal; i++)
    {
        while (count[i] > 0)
        {
            arr[index++] = i;
            visualize(index - 1);
            SDL_Delay(30);
            count[i]--;
        }
    }
    delete[] count;
}

//INITIALIZE ARRAYS
void loadArr()
{
    memcpy(arr, Barr, sizeof(int)*arrSize);
}
void randomizeAndSaveArray()
{
    unsigned int seed=(unsigned)time(NULL);
    srand(seed);
    for(int i=0; i<arrSize; i++)
    {
        int random=rand()%(SCREEN_HEIGHT-50);
        Barr[i]=random;
    }
}

//INITIALIZE LINKED LIST
Node* createLinkedList()
{
    Node* head = nullptr;
    Node* tail = nullptr;

    for (int i = 0; i < arrSize; i++)
    {
        Node* newNode = new Node{Barr[i], nullptr};
        if (!head)
        {
            head = tail = newNode;
        }
        else
        {
            tail->next = newNode;
            tail = newNode;
        }
    }
    return head;
}
void convertSortedLinkedListToArray(Node* head, int arr[], int maxSize) {
    int index = 0;
    Node* temp = head;

    while (temp != nullptr && index < maxSize) {
        arr[index++] = temp->data;
        temp = temp->next;
    }
}


//SORTING EXECUTION
void execute()
{
    if (!init()) {
        cout << "SDL Initialization Failed.\n";
    } else {
        randomizeAndSaveArray();
        loadArr();

        SDL_Event e;
        bool quit = false;
        while (!quit) {
             Node* bubbleHead = nullptr;      
             Node* selectionHead = nullptr;
             Node* mergeHead = nullptr;
             Node* quickHead = nullptr;

            while (SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_QUIT) {
                    quit = true;
                    complete = false;
                } else if (e.type == SDL_KEYDOWN) {
                    switch (e.key.keysym.sym) {
                        case (SDLK_q):
                            quit = true;
                            complete = false;
                            cout << "\nEXITING SORTING VISUALIZER.\n";
                            break;
                        case (SDLK_0):
                            randomizeAndSaveArray();
                            complete = false;
                            loadArr();
                            cout << "\nNEW RANDOM LIST GENERATED.\n";
                            break;
                        case (SDLK_1):
                            loadArr();
                            cout << "\nSELECTION SORT STARTED.\n";
                            complete = false;
                            selectionSort();
                            complete = true;
                            cout << "\nSELECTION SORT COMPLETE.\n";
                            break;
                        case (SDLK_2):
                            loadArr();
                            cout << "\nINSERTION SORT STARTED.\n";
                            complete = false;
                            insertionSort();
                            complete = true;
                            cout << "\nINSERTION SORT COMPLETE.\n";
                            break;
                        case (SDLK_3):
                            loadArr();
                            cout << "\nBUBBLE SORT STARTED.\n";
                            complete = false;
                            bubbleSort();
                            complete = true;
                            cout << "\nBUBBLE SORT COMPLETE.\n";
                            break;
                        case (SDLK_4):
                            loadArr();
                            cout << "\nMERGE SORT STARTED.\n";
                            complete = false;
                            mergeSort(arr, 0, arrSize - 1);
                            complete = true;
                            cout << "\nMERGE SORT COMPLETE.\n";
                            break;
                        case (SDLK_5):
                            loadArr();
                            cout << "\nQUICK SORT STARTED.\n";
                            complete = false;
                            quickSort(arr, 0, arrSize - 1);
                            complete = true;
                            cout << "\nQUICK SORT COMPLETE.\n";
                            break;
                        case (SDLK_6):
                            loadArr();
                            cout << "\nHEAP SORT STARTED.\n";
                            complete = false;
                            inplaceHeapSort(arr, arrSize);
                            complete = true;
                            cout << "\nHEAP SORT COMPLETE.\n";
                            break;
                        case (SDLK_7):
                            loadArr();
                            cout << "\nFLASH SORT STARTED.\n";
                            complete = false;
                            flashSort();
                            complete = true;
                            cout << "\nFLASH SORT COMPLETE.\n";
                            break;
                        case (SDLK_8):
                            loadArr();
                            cout << "\nCOUNTING SORT STARTED.\n";
                            complete = false;
                            countingSort();
                            complete = true;
                            cout << "\nCOUNTING SORT COMPLETE.\n";
                            break;
                        case (SDLK_9):
                            bubbleHead =createLinkedList();
                            cout << "\nLINKED LIST BUBBLE SORT STARTED.\n";
                            complete = false;
                            bubbleSortLinkedList(bubbleHead);
                            convertSortedLinkedListToArray(bubbleHead, arr, arrSize);
                            complete = true;
                            cout << "\nLINKED LIST BUBBLE SORT COMPLETE.\n";
                            break;
                        case (SDLK_a):
                            selectionHead = createLinkedList();
                            cout << "\nLINKED LIST SELECTION SORT STARTED.\n";
                            complete = false;
                            selectionSortLinkedList(selectionHead);
                            convertSortedLinkedListToArray(selectionHead, arr, arrSize);
                            complete = true;
                            cout << "\nLINKED LIST SELECTION SORT COMPLETE.\n";
                            break;
                        case (SDLK_s):
                            mergeHead = createLinkedList();
                            cout << "\nLINKED LIST MERGE SORT STARTED.\n";
                            complete = false;
                            mergeSortLinkedList(&mergeHead);
                            convertSortedLinkedListToArray(mergeHead, arr, arrSize);
                            complete = true;
                            cout << "\nLINKED LIST MERGE SORT COMPLETE.\n";
                            break;
                        case (SDLK_d):
                            quickHead = createLinkedList();
                            cout << "\nLINKED LIST MERGE SORT STARTED.\n";
                            complete = false;
                            startQuickSort(quickHead);
                            convertSortedLinkedListToArray(quickHead, arr, arrSize);
                            complete = true;
                            cout << "\nLINKED LIST MERGE SORT COMPLETE.\n";
                            break;

                        break;
                    }
                }
            }
            visualize();
            
        }
        close();
    }
}

//MENU
bool controls()
{
     
        cout<<"\n"
        <<"Sorting Visualizer:-\n"
        <<"    0- Generate a different randomized list.\n"
        <<"    1- Selection Sort Algorithm.\n"
        <<"    2- Insertion Sort Algorithm.\n"
        <<"    3- Bubble Sort Algorithm.\n"
        <<"    4- Merge Sort Algorithm.\n"
        <<"    5- Quick Sort Algorithm.\n"
        <<"    6- Heap Sort Algorithm.\n"
        <<"    7- Flash Sort Algorithm.\n"
        <<"    8- Counting Sort Algorithm.\n"
        <<"    9- Linked List Bubble Sort Algorithm.\n"
        <<"    a- Linked List Selection Sort Algorithm.\n"
        <<"    s- Linked List Merge Sort Algorithm.\n"
        <<"    d- Linked List Quick Sort Algorithm.\n"
        <<"    q- Exit out of Sorting Visualizer\n\n"
        <<"PRESS ENTER TO START SORTING VISUALIZER...\n\n"
        <<"Or type -1 and press ENTER to quit the program.";


    string s;
    getline(cin, s);
    if(s=="-1")
    {
        return false;
    }
    
    return true;
}
void intro()
{
    cout<<"==============================Sorting Visualizer==============================\n\n"
        <<"Visualization of different sorting algorithms in C++ with SDL2 Library. A sorting algorithm is an algorithm that puts the elements of a list in a certain order. While there are a large number of sorting algorithms, in practical implementations a few algorithms predominate.\n"
        <<"In this implementation of sorting visualizer, we'll be looking at some of these sorting algorithms and visually comprehend their working.\n"
        <<"The sorting algorithms covered here are Selection Sort, Insertion Sort, Bubble Sort, Merge Sort, Quick Sort, Flash Sort, Counting Sort, and Heap Sort.\n"
        <<"You can input the number of elements you want in the array or linked list. You can randomize the list and select any type of sorting algorithm to call on the list from the given options. Here, all sorting algorithms will sort the elements in ascending order. The sorting time being visualized for an algorithm is not exactly same as their actual time complexities. The relatively faster algorithms like Merge Sort, etc., have been delayed so that they could be properly visualized.\n\n"
        <<"Press ENTER to show controls...";
    
    string s;
    getline(cin, s);
    if(s=="\n")
    {
        return;
    }
}

//MAIN
int main(int argc, char* args[])
{
    
    intro();

    while(1)
    {
        cout<<'\n';
        if(controls())
            execute();
        else
        {
            cout<<"\nEXITING PROGRAM.\n";
            break;
        }
    }

    return 0;
}