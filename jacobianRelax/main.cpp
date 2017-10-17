#include <iostream>
#include <stdio.h>
#include <pthread.h>


int LENGTH_OF_MATRIX;
int CENTER_OF_MATRIX;

struct thread_data{
    int start_index;
    int end_index;
};

// Create 3D array
float*** create_3D_array(int n){
    float ***array = nullptr;
    array = new float**[n];

    for(int i = 0; i < n; i++){
        array[i] = new float*[n];

        for(int j = 0; j < n; j++){
            array[i][j] = new float[n];

            for(int k = 0; k < n; k++){
                array[i][j][k] = 0;
            }
        }
    }

    return array;
}

float*** tempArray;
float*** dataArray;

// Delete 3D array
void delete_3D_array(float ***array, int n){

    for(int i = 0; i < n; i++){

        for(int j = 0; j < n; j++){
            delete[] array[i][j];
        }
        delete[] array[i];
    }
    delete[] array;
}

//  Print 3D array
void print_array(float ***array, int n){
    for (int Row = 1; Row < n - 1; Row++){
        for (int Col = 1; Col < n - 1; Col++) {
            for (int Height = 1; Height < n - 1; Height++) {
                printf("%f, ", array[Row][Col][Height]);
            }
            printf("\n");
        }
        printf("\n***\n");
    }
    printf("\n%%%%%%%%%%%%\n");
}

void* jacobianIt(void* arg){
    struct thread_data *arg_struct = (struct thread_data*) arg;
    for (int Row = 1; Row < LENGTH_OF_MATRIX - 1; Row++) {
        for (int Col = 1; Col < LENGTH_OF_MATRIX - 1; Col++) {
            for (int Height = arg_struct->start_index; Height < (arg_struct->end_index); Height += 1) {
                if(Row == CENTER_OF_MATRIX && Col == CENTER_OF_MATRIX && Height == CENTER_OF_MATRIX){
                    tempArray[Row][Col][Height] = 1;
                }else{
                    // Compute algorithm and store value in temp array
                    tempArray[Row][Col][Height] = (dataArray[Row + 1][Col][Height] +
                                                   dataArray[Row - 1][Col][Height] +
                                                   dataArray[Row][Col + 1][Height] +
                                                   dataArray[Row][Col - 1][Height] +
                                                   dataArray[Row][Col][Height + 1] +
                                                   dataArray[Row][Col][Height - 1] )/6;
                }

            }
        }
    }
    pthread_exit(nullptr);
}

// Main thread
int main(int argc, char **argv) {

    //Check if input is valid
    if(argc > 4){
        printf("Usage: %s <num> <num>\n", argv[0]);
        exit(-1);
    }

    // Get number of threads and cube size
    int num_threads = atoi(argv[1]);
    LENGTH_OF_MATRIX = atoi(argv[2]);
    int num_iterations = atoi(argv[3]);
    CENTER_OF_MATRIX = (LENGTH_OF_MATRIX-1)/2;

    // Check if len_array is odd
    if(LENGTH_OF_MATRIX%2 != 1){
        printf("argv[2] must be odd\n");
        exit(-1);
    }

    // Create 3D array with point change in center
    dataArray = create_3D_array(LENGTH_OF_MATRIX);
    tempArray = create_3D_array(LENGTH_OF_MATRIX);
    dataArray[CENTER_OF_MATRIX][CENTER_OF_MATRIX][CENTER_OF_MATRIX] = 1;

    clock_t timePassed;
    double secondsPassed;
    clock_t startTime = clock(); //Start timer

    // Create list of thread data, 1 entry for each thread
    struct thread_data args[num_threads];

    // Iteration loop
    for(int j = 0; j < num_iterations; j++) {
        int sliced_index_start = 1;
        int sliced_index_end;
        int array_slice_len = LENGTH_OF_MATRIX / num_threads + LENGTH_OF_MATRIX % num_threads / num_threads + 1;
//        printf("array_slice_len = %d\n", array_slice_len);

        pthread_t tid[num_threads];
        for (int i = 0; i < num_threads; i++) {

            // Calculate data for threads
            sliced_index_end = sliced_index_start + array_slice_len;
            if (sliced_index_end > LENGTH_OF_MATRIX) {
                sliced_index_end = LENGTH_OF_MATRIX;
            }

//            printf("Thread %d array_slice_start = %d\n", i, sliced_index_start);
//            printf("Thread %d array_slice_end = %d\n", i, sliced_index_end);

            args[i].start_index = sliced_index_start;
            args[i].end_index = sliced_index_end;

            pthread_attr_t attr;
            pthread_attr_init(&attr);
            pthread_create(&tid[i], &attr, jacobianIt, &args[i]);

            // Set endpoint of last thread to start point of next thread
            sliced_index_start = sliced_index_end;

        }


        // TODO: Could do other thing here as main thread has to wait for other threads!!

        // Wait until thread is done
        for (int i = 0; i < num_threads; i++) {
            pthread_join(tid[i], nullptr);
        }

        // Set the answers to be used next iteration
        delete_3D_array(dataArray,LENGTH_OF_MATRIX);
        dataArray = tempArray;

        tempArray = create_3D_array(LENGTH_OF_MATRIX);

    }

    clock_t endTime = clock(); //Start timer
    timePassed = endTime - startTime;
    secondsPassed = timePassed / (double)CLOCKS_PER_SEC;

    std::cout << secondsPassed << "\n";
//    print_array(dataArray, LENGTH_OF_MATRIX);
    printf("%f\n", dataArray[CENTER_OF_MATRIX][CENTER_OF_MATRIX][CENTER_OF_MATRIX]);

    return EXIT_SUCCESS;
}