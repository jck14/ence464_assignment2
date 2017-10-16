#include <iostream>
#include <vector>
#include <thread>
#include <math.h>
using namespace std;

const int N = 11;
const size_t center = (N-1)/2;

class Array3D {
    size_t m_width, m_height;
    std::vector<float> m_data;
public:
    Array3D(size_t x, size_t y, size_t z, float init = 0):
            m_width(x), m_height(y), m_data(x*y*z, init)
    {}
    float& operator()(size_t x, size_t y, size_t z) {
        return m_data.at(x + y * m_width + z * m_width * m_height);
    }
};

void jacobian_algorithm(Array3D dataArray, Array3D tempArray, int rs, int re) {
    float f = 0.0;

    for (size_t Row = rs; Row < re; Row++) {
        for (size_t Col = 1; Col < N-1; Col++) {
            for (size_t Height = 1; Height < N-1; Height++) {
                if (Row == center && Col == center && Height == center) {
                    f = 1.0;
                } else {
                    f = 0.0;
                }
                tempArray(Row, Col, Height) =
                        1.0 / 6.0 * (dataArray(Row + 1, Col, Height) + dataArray(Row - 1, Col, Height) +
                                     dataArray(Row, Col + 1, Height) + dataArray(Row, Col - 1, Height) +
                                     dataArray(Row, Col, Height + 1) + dataArray(Row, Col, Height - 1) - f);
            }
        }
    }
}

int main() {

    unsigned int maxThreads = std::thread::hardware_concurrency(); // Use maximum number of threads

    Array3D dataArray(N, N, N, 0.0);
    Array3D tempArray(N, N, N, 0.0);
    dataArray(center,center,center) = 1.0;

    int Row = N/maxThreads + 1;

    std::cout << Row << '\n';

    for (size_t Row = 1; Row < N - 1; Row++){
        for (size_t Col = 1; Col < N - 1; Col++) {
            for (size_t Height = 1; Height < N - 1; Height++) {
                printf("%f, ", dataArray(Row, Col, Height));
            }
            printf("\n");
        }
        printf("\n***\n");
    }
    printf("\n%%%%%%%%%%%%\n");


    std::thread t1 (jacobian_algorithm,std::ref(dataArray),std::ref(tempArray),1,      Row);
    std::thread t2 (jacobian_algorithm,std::ref(dataArray),std::ref(tempArray),Row + 1,2*Row);
    std::thread t3 (jacobian_algorithm,std::ref(dataArray),std::ref(tempArray),2*Row+2,3*Row);
    std::thread t4 (jacobian_algorithm,std::ref(dataArray),std::ref(tempArray),3*Row,  N-1);

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    dataArray = tempArray;

    for (size_t Row = 1; Row < N - 1; Row++){
        for (size_t Col = 1; Col < N - 1; Col++) {
            for (size_t Height = 1; Height < N - 1; Height++) {
                printf("%f, ", dataArray(Row, Col, Height));
            }
            printf("\n");
        }
        printf("\n***\n");
    }
    printf("\n%%%%%%%%%%%%\n");

    return EXIT_SUCCESS;
}