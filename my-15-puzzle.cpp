#include <iostream>
#include <vector>
#include <stdlib.h>
#include <algorithm>
#include <ctime>

using namespace std;

class Puzzle{
private:
    const static int size = 16;
    int board[size];
    int posX = 3, posY = 3;

    void makeMove(int d){
        int t = posX + posY * 4;
        switch (d) {
        case 1:
            posX--;
            break;
        case 2:
            posY--;
            break;
        case 3:
            posX++;
            break;
        case 4:
            posY++;
            break;
        }
        board[t] = board[posX + posY * 4];
        board[posX + posY * 4] = 0;
    }

    void createBoard(){
        // default values
        int i;
        vector<int> v;
        for (i = 0; i < size - 1; i++) {
            board[i] = i + 1;
        }
        board[15] = 0;

        srand(time(0));
        for (i = 0; i < 1000; i++) {
            getPossibleMoves(v);
            int d = v[rand() % v.size()];
            makeMove(d);
            v.clear();
        }
    }

    void getPossibleMoves(vector<int>& v){
        if (posX > 0) v.push_back(1);
        if (posY > 0) v.push_back(2);
        if (posX < 3) v.push_back(3);
        if (posY < 3) v.push_back(4);
   }

    void drawBoard(){
        cout << "\n\n";

        for (int i = 0; i < 4; i++) {
            cout << "+---+---+---+---+\n";
            for (int j = 0; j < 4; j++) {
                int r = board[j + i * 4];
                cout << "|";
                if (r < 10) cout << " ";
                if (!r) cout << "  ";
                else cout << r << " ";
            }
            cout << "|\n";
        }
        cout << "+---+---+---+---+\n\n";
    }

    bool isDone(){
        for (int i = 0; i < size; i++) {
            if (board[i] != i + 1) return false;
        }
        return true;
    }

    int getMove(){
        vector<int> v;
        vector<int> m;
        int ind = -1;
        getPossibleMoves(v);
        do{
            printMoveMess(v, m);
            int s;
            cout << "Make move >> ";
            cin >> s;
            ind = findIndex(m, s);
            cout << "Inde" << ind << "  ";
        }while(ind == -1);
        cout << "H";
        return v[ind];

    }

    int findIndex(vector<int> v, int p){
        int i;
        for (i = 0; i < v.size(); ++i) {
            if (v[i] == p) return i;
        }
        return -1;
    }

    void printMoveMess(vector<int>& v, vector<int>& p){
        int i, b, x, y, t;
        cout << "Possible moves: ";

        for (i = 0; i < v.size(); ++i) {
            x = posX;
            y = posY;

            b = v[i];
            switch (b) {
            case 1:
                x--;
                break;
            case 2:
                y--;
                break;
            case 3:
                x++;
                break;
            case 4:
                y++;
                break;
            }

            t = board[x + y * 4];
            p.push_back(t);
            cout << t << " ";
        }

        cout << endl;
    }

public:
    void play(){
        char o;
        int move;

        while(true){
            createBoard();
             while(!isDone()){
                 drawBoard();
                 move = getMove();
                 cout << "Move " << move;
                 makeMove(move);
             }

             drawBoard();
             cout << "Congratulations you completed the game\n";
             cout << "Play Again: Y/N -> ";

             cin >> o;
            if (o != 'Y') break;
        }
    }
};

int main(){
    Puzzle p;
    p.play();
}
