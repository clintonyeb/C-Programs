#include <stdio.h>
#include <math.h>

#define COL 204
#define COL_P 2
#define ROWS_P 1020

int codecheck(int (*h)[2], int x[])
{
    int i, j, m, f; //flags
    int sum = 0;
    int y[COL];

    for (i = 0; i < COL; i++)
        y[i] = 0;
    m = 1;
    f = -1;
    for (i = 0; i < ROWS_P; i++)
    {
        for (j = 0; j < COL_P; j++)
        {
            if (h[j][0] == m)
            {
                y[m] = y[m] + x[h[j][1] - 1];
                f++;
            }
        }
        y[m] = y[m] % 2;
        m = m + 1;
        i = f + 1;
    }

    for (i = 0; i < COL; ++i)
    {
        sum = sum + y[i];
    }
    if (sum == 0)
    {
        return 1;
    }
    else
    {
        return -1;
    }
}

int codesum(int y[])
{
    int sum = 0;
    for (int i = 0; i < COL; i++)
        sum += y[i];
    //cout<< "sum="<< sum<<endl;
    if (sum > 0)
        return 0;
    else
        return 1;
}

/*void ModulationSchemeValues (int modulationScheme, int noOfSymbols, double var, float d, float rate, double snr)
{
           switch (modulationScheme)
            {
                case 0:
                    //cout<<"case 0"<<endl;
                    var=1/(2*rate*snr);
                    noOfSymbols = COL;
                    d=1;
                    break;
                case 1:
                    //cout<<"case 1"<<endl;
                    var=1/(4*rate*snr);
                    noOfSymbols = COL/2;
                    d=1/sqrt(2);
                    break;
                case 2:
                    //cout<<"case 2"<<endl;
                    var=1/(8*rate*snr);
                    noOfSymbols = COL/4;
                    d=1/sqrt(10);
                default:
                    var=1/(2*rate*snr);
                    noOfSymbols = COL;
                    d=1;
                    break;
            }
    printf("%d %d %f %f %f %d",modulationScheme,noOfSymbols, var, d,rate, snr);
}

void InitializeChannelLLR (double var, int modulationScheme, int noOfSymbols, int x[], float noise[], double L_ch[], float d, int (*h)[2], double LLR[])
{

    int j=0;   // temporary flags
   int k=0,t;
    for(int i=0;i<noOfSymbols;i++)                            // using randnoise function introducing noise to all zero codeword
    {

            x[j]=d;
            noise[j]=(x[j])+0.8;
            L_ch[j]= 2*noise[j]/var;
            j++;


    }


    for(int j=0;j<ROWS_P;j++)
    {
        //cout<<"enter loop"<<endl;
        //cout<< "j="<<j<<endl;  //<<j<<"\t";
        t=h[j][1]-1;
        LLR[j]=L_ch[t];
    }
    //cout<<"L_ch initialize done \n";

}*/

int SPADecoder(double LLR[], int (*h)[2], double LLR_ret[], double L_ch[], double L_dec[], int y[])
{
    double temp;
    int z;
    int i, j, k;
    for (i = 0; i < ROWS_P; i++)
    {
        temp = 1;
        for (j = 0; j < ROWS_P; j++)
        {
            if (j != i && h[j][0] == h[i][0])
            {
                temp = temp * tanhf(LLR[j] / 2);
            }
        }

        LLR_ret[i] = log((1 + temp) / (1 - temp)); // lower and upper bounding the LLR

        if (LLR_ret[i] > 40)
        {
            LLR_ret[i] = 40;
        }
        else if (LLR_ret[i] < -40)
        {
            LLR_ret[i] = -40;
        }
    }

    for (i = 0; i < COL; i++) // L_dec array used to decide the value of each bit of codeword here it is initalized to LLr value received from channel
    {
        L_dec[i] = L_ch[i];
    }

    for (i = 1; i < COL + 1; i++) // LLR values received by the variable nodes are added and stored in L_dec for decision making
    {
        for (j = 0; j < ROWS_P; j++)
        {
            if (h[j][1] == i)
            {
                L_dec[i - 1] = L_dec[i - 1] + LLR_ret[j];
            }
        }

        if (L_dec[i - 1] <= 0) // decision about the bit values of the received codeword is done her and stored in Y.
        {
            y[i - 1] = 1;
        }
        else
        {
            y[i - 1] = 0;
        }
    }

    //z = codesum(y,col);
    z = codecheck(h, y); // checking whether the decoded codeword is all zero or not

    if (z == 1) // if decoded perfectly exit the deoding loop
    {
        return z;
    }
    // updating LLR values on variable nodes side for each variable and check node
    for (i = 0; i < ROWS_P; i++)
    {
        temp = 0;
        for (j = 0; j < ROWS_P; j++)
        {
            if (j != i && h[j][1] == h[i][1])
            {
                temp = temp + LLR_ret[j];
                k = h[i][1];
            }
        }
        LLR[i] = temp + L_ch[k - 1];
    }
    return z;
}

int main()
{

    // flags and counters
    int z, i, j, N, k, ber_count, looping, t, sum;
    float d = 1;
    float rate = 0.5;

    int maxTransmissions = 20, decoderIterations = 10, maxErrors = 100, errorsAccumulated = 0;
    int y[COL]; // iterations gives the maximum number of time the decoder is run for each value of SNR
    int L, O;

    double var, snr;
    double temp; //temp variable

    int M = 0; // temporary flags
    int R;

    float noise[COL];
    double L_ch[COL], L_dec[COL];
    double LLR[ROWS_P], LLR_ret[ROWS_P];
    double BER[11];
    float snr_db[] = {0, 1, 2, 3, 4};
    int snr_range = 5, snr_count;
    int x[] = {0, 1, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 1, 1, 1};
    int ret, row = 0;
    double error_bits, error;
    // Set Field Separator here
    printf("encoder \n");
    //array x
    for (L = 0; L < 204; L++)
    {
        printf("%d", x[L]);
    }

    int modulationScheme = 0, noOfSymbols = 204;

    int h[ROWS_P][COL_P] = {{1, 1}, {1, 18}, {1, 27}, {1, 82}, {1, 102}, {1, 103}, {1, 130}, {1, 177}, {1, 190}, {1, 203}, {2, 2}, {2, 15}, {2, 75}, {2, 80}, {2, 86}, {2, 104}, {2, 121}, {2, 141}, {2, 144}, {2, 191}, {3, 3}, {3, 31}, {3, 32}, {3, 42}, {3, 101}, {3, 105}, {3, 153}, {3, 159}, {3, 178}, {3, 199}, {4, 4}, {4, 37}, {4, 71}, {4, 75}, {4, 85}, {4, 106}, {4, 111}, {4, 114}, {4, 127}, {4, 164}, {5, 5}, {5, 17}, {5, 42}, {5, 72}, {5, 87}, {5, 107}, {5, 141}, {5, 158}, {5, 193}, {5, 197}, {6, 4}, {6, 6}, {6, 17}, {6, 78}, {6, 102}, {6, 108}, {6, 162}, {6, 165}, {6, 167}, {6, 174}, {7, 7}, {7, 23}, {7, 73}, {7, 80}, {7, 83}, {7, 109}, {7, 118}, {7, 168}, {7, 177}, {7, 178}, {8, 8}, {8, 33}, {8, 37}, {8, 73}, {8, 100}, {8, 110}, {8, 156}, {8, 173}, {8, 192}, {8, 197}, {9, 9}, {9, 40}, {9, 41}, {9, 88}, {9, 92}, {9, 103}, {9, 111}, {9, 173}, {9, 181}, {9, 199}, {10, 10}, {10, 14}, {10, 23}, {10, 53}, {10, 93}, {10, 107}, {10, 112}, {10, 137}, {10, 167}, {10, 175}, {11, 11}, {11, 19}, {11, 26}, {11, 28}, {11, 88}, {11, 110}, {11, 113}, {11, 116}, {11, 170}, {11, 191}, {12, 1}, {12, 12}, {12, 25}, {12, 86}, {12, 97}, {12, 108}, {12, 114}, {12, 124}, {12, 161}, {12, 192}, {13, 4}, {13, 13}, {13, 33}, {13, 34}, {13, 50}, {13, 115}, {13, 137}, {13, 140}, {13, 146}, {13, 160}, {14, 14}, {14, 31}, {14, 40}, {14, 58}, {14, 95}, {14, 116}, {14, 121}, {14, 166}, {14, 174}, {14, 203}, {15, 15}, {15, 68}, {15, 70}, {15, 87}, {15, 101}, {15, 109}, {15, 113}, {15, 117}, {15, 165}, {15, 180}, {16, 4}, {16, 16}, {16, 59}, {16, 67}, {16, 92}, {16, 118}, {16, 125}, {16, 131}, {16, 138}, {16, 151}, {17, 15}, {17, 17}, {17, 44}, {17, 71}, {17, 73}, {17, 119}, {17, 161}, {17, 190}, {17, 194}, {17, 198}, {18, 18}, {18, 47}, {18, 62}, {18, 85}, {18, 93}, {18, 120}, {18, 122}, {18, 125}, {18, 146}, {18, 181}, {19, 19}, {19, 22}, {19, 23}, {19, 37}, {19, 57}, {19, 121}, {19, 126}, {19, 130}, {19, 131}, {19, 143}, {20, 1}, {20, 20}, {20, 50}, {20, 51}, {20, 99}, {20, 106}, {20, 122}, {20, 169}, {20, 179}, {20, 200}, {21, 21}, {21, 32}, {21, 62}, {21, 72}, {21, 98}, {21, 123}, {21, 126}, {21, 154}, {21, 168}, {21, 204}, {22, 22}, {22, 31}, {22, 44}, {22, 89}, {22, 98}, {22, 124}, {22, 141}, {22, 160}, {22, 169}, {22, 176}, {23, 23}, {23, 32}, {23, 61}, {23, 65}, {23, 99}, {23, 125}, {23, 132}, {23, 133}, {23, 147}, {23, 152}, {24, 16}, {24, 24}, {24, 42}, {24, 73}, {24, 77}, {24, 126}, {24, 142}, {24, 145}, {24, 146}, {24, 147}, {25, 5}, {25, 24}, {25, 25}, {25, 55}, {25, 67}, {25, 127}, {25, 133}, {25, 165}, {25, 178}, {25, 185}, {26, 10}, {26, 26}, {26, 27}, {26, 40}, {26, 85}, {26, 128}, {26, 147}, {26, 155}, {26, 160}, {26, 200}, {27, 22}, {27, 27}, {27, 42}, {27, 50}, {27, 74}, {27, 108}, {27, 129}, {27, 154}, {27, 170}, {27, 195}, {28, 2}, {28, 28}, {28, 63}, {28, 67}, {28, 94}, {28, 130}, {28, 150}, {28, 171}, {28, 187}, {28, 192}, {29, 29}, {29, 48}, {29, 69}, {29, 79}, {29, 90}, {29, 116}, {29, 131}, {29, 159}, {29, 176}, {29, 200}, {30, 7}, {30, 30}, {30, 53}, {30, 58}, {30, 94}, {30, 127}, {30, 132}, {30, 140}, {30, 162}, {30, 189}, {31, 29}, {31, 31}, {31, 64}, {31, 77}, {31, 94}, {31, 117}, {31, 133}, {31, 167}, {31, 183}, {31, 190}, {32, 20}, {32, 30}, {32, 32}, {32, 74}, {32, 78}, {32, 115}, {32, 134}, {32, 151}, {32, 181}, {32, 194}, {33, 11}, {33, 33}, {33, 47}, {33, 76}, {33, 81}, {33, 109}, {33, 135}, {33, 154}, {33, 155}, {33, 179}, {34, 2}, {34, 7}, {34, 34}, {34, 71}, {34, 76}, {34, 136}, {34, 138}, {34, 159}, {34, 182}, {34, 183}, {35, 27}, {35, 29}, {35, 35}, {35, 60}, {35, 86}, {35, 123}, {35, 137}, {35, 149}, {35, 166}, {35, 189}, {36, 17}, {36, 35}, {36, 36}, {36, 48}, {36, 100}, {36, 138}, {36, 139}, {36, 146}, {36, 163}, {36, 168}, {37, 5}, {37, 34}, {37, 37}, {37, 38}, {37, 69}, {37, 117}, {37, 139}, {37, 145}, {37, 169}, {37, 203}, {38, 38}, {38, 56}, {38, 76}, {38, 92}, {38, 101}, {38, 106}, {38, 123}, {38, 140}, {38, 161}, {38, 170}, {39, 39}, {39, 41}, {39, 60}, {39, 76}, {39, 78}, {39, 125}, {39, 141}, {39, 142}, {39, 175}, {39, 198}, {40, 2}, {40, 35}, {40, 40}, {40, 43}, {40, 93}, {40, 114}, {40, 135}, {40, 142}, {40, 148}, {40, 158}, {41, 1}, {41, 2}, {41, 13}, {41, 41}, {41, 59}, {41, 109}, {41, 119}, {41, 132}, {41, 143}, {41, 157}, {42, 11}, {42, 42}, {42, 43}, {42, 92}, {42, 95}, {42, 115}, {42, 117}, {42, 144}, {42, 156}, {42, 204}, {43, 7}, {43, 9}, {43, 43}, {43, 45}, {43, 56}, {43, 122}, {43, 145}, {43, 192}, {43, 194}, {43, 201}, {44, 10}, {44, 44}, {44, 75}, {44, 83}, {44, 95}, {44, 110}, {44, 135}, {44, 146}, {44, 157}, {44, 180}, {45, 45}, {45, 63}, {45, 79}, {45, 81}, {45, 97}, {45, 127}, {45, 139}, {45, 147}, {45, 148}, {45, 195}, {46, 34}, {46, 46}, {46, 61}, {46, 66}, {46, 77}, {46, 105}, {46, 144}, {46, 148}, {46, 197}, {46, 201}, {47, 14}, {47, 20}, {47, 46}, {47, 47}, {47, 83}, {47, 124}, {47, 128}, {47, 132}, {47, 149}, {47, 159}, {48, 8}, {48, 13}, {48, 26}, {48, 48}, {48, 62}, {48, 149}, {48, 150}, {48, 174}, {48, 193}, {48, 195}, {49, 15}, {49, 21}, {49, 25}, {49, 49}, {49, 51}, {49, 145}, {49, 149}, {49, 151}, {49, 177}, {49, 188}, {50, 8}, {50, 49}, {50, 50}, {50, 54}, {50, 89}, {50, 105}, {50, 126}, {50, 152}, {50, 164}, {50, 165}, {51, 43}, {51, 51}, {51, 66}, {51, 68}, {51, 91}, {51, 131}, {51, 133}, {51, 153}, {51, 187}, {51, 189}, {52, 1}, {52, 39}, {52, 45}, {52, 52}, {52, 54}, {52, 110}, {52, 140}, {52, 150}, {52, 154}, {52, 163}, {53, 8}, {53, 53}, {53, 60}, {53, 64}, {53, 90}, {53, 138}, {53, 155}, {53, 157}, {53, 194}, {53, 204}, {54, 12}, {54, 28}, {54, 54}, {54, 60}, {54, 82}, {54, 112}, {54, 139}, {54, 151}, {54, 156}, {54, 183}, {55, 3}, {55, 17}, {55, 37}, {55, 55}, {55, 99}, {55, 112}, {55, 148}, {55, 157}, {55, 172}, {55, 189}, {56, 20}, {56, 31}, {56, 56}, {56, 80}, {56, 91}, {56, 152}, {56, 156}, {56, 158}, {56, 162}, {56, 180}, {57, 28}, {57, 57}, {57, 66}, {57, 81}, {57, 89}, {57, 134}, {57, 137}, {57, 159}, {57, 172}, {57, 196}, {58, 5}, {58, 58}, {58, 70}, {58, 78}, {58, 91}, {58, 118}, {58, 149}, {58, 154}, {58, 160}, {58, 201}, {59, 18}, {59, 24}, {59, 59}, {59, 88}, {59, 96}, {59, 150}, {59, 152}, {59, 161}, {59, 179}, {59, 183}, {60, 21}, {60, 38}, {60, 55}, {60, 59}, {60, 60}, {60, 162}, {60, 176}, {60, 187}, {60, 190}, {60, 197}, {61, 61}, {61, 69}, {61, 74}, {61, 93}, {61, 101}, {61, 111}, {61, 128}, {61, 163}, {61, 193}, {61, 196}, {62, 6}, {62, 7}, {62, 35}, {62, 62}, {62, 96}, {62, 103}, {62, 113}, {62, 129}, {62, 164}, {62, 200}, {63, 16}, {63, 27}, {63, 44}, {63, 63}, {63, 88}, {63, 121}, {63, 134}, {63, 165}, {63, 175}, {63, 188}, {64, 4}, {64, 26}, {64, 64}, {64, 79}, {64, 84}, {64, 119}, {64, 134}, {64, 136}, {64, 166}, {64, 168}, {65, 12}, {65, 13}, {65, 52}, {65, 57}, {65, 65}, {65, 106}, {65, 138}, {65, 142}, {65, 167}, {65, 176}, {66, 24}, {66, 30}, {66, 52}, {66, 66}, {66, 102}, {66, 112}, {66, 117}, {66, 122}, {66, 135}, {66, 168}, {67, 6}, {67, 57}, {67, 67}, {67, 85}, {67, 97}, {67, 140}, {67, 169}, {67, 182}, {67, 184}, {67, 186}, {68, 6}, {68, 58}, {68, 68}, {68, 71}, {68, 90}, {68, 134}, {68, 143}, {68, 145}, {68, 152}, {68, 170}, {69, 54}, {69, 57}, {69, 59}, {69, 69}, {69, 77}, {69, 103}, {69, 104}, {69, 114}, {69, 136}, {69, 171}, {70, 19}, {70, 24}, {70, 56}, {70, 70}, {70, 100}, {70, 105}, {70, 114}, {70, 128}, {70, 129}, {70, 172}, {71, 14}, {71, 52}, {71, 63}, {71, 70}, {71, 71}, {71, 120}, {71, 169}, {71, 173}, {71, 178}, {71, 196}, {72, 11}, {72, 25}, {72, 39}, {72, 66}, {72, 72}, {72, 120}, {72, 130}, {72, 132}, {72, 174}, {72, 199}, {73, 48}, {73, 49}, {73, 64}, {73, 73}, {73, 98}, {73, 111}, {73, 175}, {73, 179}, {73, 201}, {73, 202}, {74, 9}, {74, 15}, {74, 58}, {74, 74}, {74, 84}, {74, 107}, {74, 120}, {74, 135}, {74, 164}, {74, 176}, {75, 28}, {75, 41}, {75, 70}, {75, 75}, {75, 96}, {75, 133}, {75, 155}, {75, 166}, {75, 177}, {75, 202}, {76, 26}, {76, 45}, {76, 65}, {76, 76}, {76, 94}, {76, 144}, {76, 158}, {76, 172}, {76, 178}, {76, 203}, {77, 10}, {77, 13}, {77, 77}, {77, 84}, {77, 87}, {77, 127}, {77, 151}, {77, 163}, {77, 179}, {77, 191}, {78, 49}, {78, 55}, {78, 65}, {78, 78}, {78, 90}, {78, 122}, {78, 130}, {78, 139}, {78, 180}, {78, 182}, {79, 33}, {79, 43}, {79, 67}, {79, 79}, {79, 99}, {79, 123}, {79, 141}, {79, 181}, {79, 188}, {79, 202}, {80, 18}, {80, 19}, {80, 35}, {80, 46}, {80, 80}, {80, 153}, {80, 167}, {80, 182}, {80, 188}, {80, 193}, {81, 9}, {81, 16}, {81, 36}, {81, 55}, {81, 81}, {81, 143}, {81, 160}, {81, 183}, {81, 186}, {81, 191}, {82, 10}, {82, 11}, {82, 79}, {82, 82}, {82, 96}, {82, 108}, {82, 111}, {82, 126}, {82, 172}, {82, 184}, {83, 12}, {83, 36}, {83, 69}, {83, 83}, {83, 87}, {83, 148}, {83, 174}, {83, 185}, {83, 202}, {83, 204}, {84, 45}, {84, 47}, {84, 48}, {84, 75}, {84, 84}, {84, 108}, {84, 123}, {84, 142}, {84, 153}, {84, 186}, {85, 9}, {85, 46}, {85, 53}, {85, 54}, {85, 85}, {85, 115}, {85, 121}, {85, 129}, {85, 177}, {85, 187}, {86, 22}, {86, 64}, {86, 83}, {86, 86}, {86, 91}, {86, 107}, {86, 116}, {86, 171}, {86, 188}, {86, 199}, {87, 8}, {87, 22}, {87, 61}, {87, 81}, {87, 87}, {87, 104}, {87, 120}, {87, 182}, {87, 189}, {87, 198}, {88, 32}, {88, 33}, {88, 39}, {88, 51}, {88, 88}, {88, 129}, {88, 143}, {88, 171}, {88, 180}, {88, 190}, {89, 3}, {89, 21}, {89, 52}, {89, 89}, {89, 100}, {89, 103}, {89, 109}, {89, 115}, {89, 125}, {89, 191}, {90, 20}, {90, 40}, {90, 44}, {90, 61}, {90, 90}, {90, 118}, {90, 137}, {90, 153}, {90, 184}, {90, 192}, {91, 6}, {91, 23}, {91, 29}, {91, 84}, {91, 91}, {91, 144}, {91, 155}, {91, 173}, {91, 185}, {91, 193}, {92, 14}, {92, 49}, {92, 72}, {92, 92}, {92, 102}, {92, 147}, {92, 171}, {92, 185}, {92, 186}, {92, 194}, {93, 5}, {93, 21}, {93, 29}, {93, 65}, {93, 93}, {93, 105}, {93, 110}, {93, 119}, {93, 184}, {93, 195}, {94, 39}, {94, 80}, {94, 82}, {94, 94}, {94, 98}, {94, 107}, {94, 113}, {94, 131}, {94, 166}, {94, 196}, {95, 30}, {95, 47}, {95, 50}, {95, 63}, {95, 95}, {95, 104}, {95, 118}, {95, 161}, {95, 163}, {95, 197}, {96, 25}, {96, 34}, {96, 56}, {96, 68}, {96, 96}, {96, 116}, {96, 181}, {96, 186}, {96, 196}, {96, 198}, {97, 36}, {97, 38}, {97, 46}, {97, 89}, {97, 97}, {97, 113}, {97, 136}, {97, 150}, {97, 157}, {97, 199}, {98, 3}, {98, 12}, {98, 18}, {98, 30}, {98, 98}, {98, 136}, {98, 170}, {98, 187}, {98, 198}, {98, 200}, {99, 53}, {99, 62}, {99, 74}, {99, 97}, {99, 99}, {99, 104}, {99, 119}, {99, 156}, {99, 185}, {99, 201}, {100, 16}, {100, 68}, {100, 82}, {100, 95}, {100, 100}, {100, 106}, {100, 124}, {100, 158}, {100, 195}, {100, 202}, {101, 19}, {101, 36}, {101, 41}, {101, 72}, {101, 101}, {101, 112}, {101, 124}, {101, 162}, {101, 164}, {101, 203}, {102, 3}, {102, 38}, {102, 51}, {102, 86}, {102, 102}, {102, 128}, {102, 173}, {102, 175}, {102, 184}, {102, 204}};

    printf(" \n Matrix \n");
    /*for(M=0; M<ROWS_P; M++)
{
   loop for second dimension of array which is 4 in this example
     for(O=0;O<COL_P;O++)
     {
         printf("%d \t",h[M][O]);
     }
     printf("\n");
}*/

    for (snr_count = 0; snr_count < snr_range; snr_count++)
    {
        //  cout<<"Decoder run for SNR in db=" <<snr_db[snr_count]<<endl;
        snr = pow(10, (snr_db[snr_count] / 10));

        printf(" \n ModulationSchemeValue \n");

        //ModulationSchemeValues (modulationScheme, noOfSymbols, var, d, rate, snr);  // determining various values based on modulation scheme

        var = 1 / (2 * rate * snr);
        noOfSymbols = COL;
        d = 1;
        printf(" \n%d %d %f %f %f %d\n", modulationScheme, noOfSymbols, var, d, rate, snr);

        printf("\n");
        // calulating noise variance from SNR
        BER[snr_count] = 0; // initializing BER
                            //   normal_distribution<double> dist(0.0, 1);
        N = 0;
        errorsAccumulated = 0;

        printf(" \n InitializeChannelLLR\n");
        while (errorsAccumulated <= maxErrors && N <= maxTransmissions)
        {
            N++;

            for (int i = 0; i < noOfSymbols; i++) // using randnoise function introducing noise to all zero codeword
            {

                x[M] = d;
                noise[M] = (x[M]) + 0.8;
                L_ch[M] = 2 * noise[M] / var;
                M++;
            }

            for (int M = 0; M < ROWS_P; M++)
            {
                //cout<<"enter loop"<<endl;
                //cout<< "j="<<j<<endl;  //<<j<<"\t";
                R = h[M][1] - 1;
                LLR[M] = L_ch[R];
            }
            printf(" %d", LLR[M]);
            // InitializeChannelLLR( var, modulationScheme, noOfSymbols, x, noise,  L_ch, d, h, LLR);

            for (looping = 0; looping < decoderIterations; looping++)
            {

                z = SPADecoder(LLR, h, LLR_ret, L_ch, L_dec, y);
            }

            if (z == 1) // if decoded correctly BER for that codeword is zero
            {
                BER[snr_count] = BER[snr_count] + 0;
            }
            else // if not decoded properly calulating the BER
            {
                error_bits = 0;
                for (i = 0; i < COL; i++)
                {
                    if (y[i] != 0)
                    {
                        error_bits++;
                    }
                }
                errorsAccumulated += error_bits;
                error = error_bits / (double)COL;
                BER[snr_count] = BER[snr_count] + error;
            }
            if (N % 300 == 0)
                printf("iteration=%d \t errors accumulated=%d \n", N, errorsAccumulated);
        }

        BER[snr_count] = BER[snr_count] / (double)N; // averaging BER over all decoding iterations.
        printf("snr=%lf\tBER[%d]=%lf \n", snr_db[snr_count], snr_count, BER[snr_count]);
    }
    return 0;
}
