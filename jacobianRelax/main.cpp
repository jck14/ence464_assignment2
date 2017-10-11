#include <iostream>
#include <vector>

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

int main() {

    const int N = 5;
    float f = 0;

    Array3D dataArray(N, N, N, 0.0);
    Array3D tempArray(N, N, N, 0.0);
    dataArray(2,2,2) = 1.0;

    printf("Jacobian Relaxation");

    for(int i = 0; i < 50; i++) {
        for (int Row = 1; Row < N - 1; Row++) {
            for (int Col = 1; Col < N - 1; Col++) {
                printf("\n----\n");
                for (int Height = 1; Height < N - 1; Height++) {
                    if (Row == 2 && Col == 2 && Height == 2) {
                        f = 0.0;
                    } else {
                        f = 0.0;
                    }
                    tempArray(Row, Col, Height) =
                            (dataArray(Row + 1, Col, Height) + dataArray(Row - 1, Col, Height) +
                                               dataArray(Row, Col + 1, Height) + dataArray(Row, Col - 1, Height) +
                                               dataArray(Row, Col, Height + 1) +
                                               dataArray(Row, Col, Height - 1) - f)/6.0;
                    printf("\n%f", dataArray(Row, Col, Height));
                }
            }
        }
        printf("-------------------------------\n\n");
        dataArray = tempArray;
    }

    printf("\n\n");
    return EXIT_SUCCESS;
}