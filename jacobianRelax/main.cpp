#include <iostream>

int main() {

    const int MAX_ROW = 5;
    const int MAX_COL = 5;
    const int MAX_HEIGHT = 5;

    unsigned float dataArray[MAX_ROW][MAX_COL][MAX_HEIGHT]

    for(int Row = 1; Row < MAX_ROW; ++Row) {
        for (int Col = 1; Col < MAX_COL; ++Col) {
            for (int Height = 1; Height < MAX_HEIGHT; ++Height) {
                dataArray[Row][Col][Height] = 1/6 * (dataArray[Row+1][Col][Height] + dataArray[Row-1][Col][Height]+
                        dataArray[Row][Col+1][Height] + dataArray[Row][Col-1][Height]+dataArray[Row][Col][Height+1] +
                        dataArray[Row][Col][Height-1]);
                printf("%f",dataArray[Row][Col][Height]);
            }
        }
    }

    return 0;
}