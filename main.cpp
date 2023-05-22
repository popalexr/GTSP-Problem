#include "Points.cpp"

using namespace std;

void consoleInput(int &p, string &file) //reading the data
{
    cout << "Introdu o valoare intreaga pentru p" << endl;
    cin >> p;
    if(p > 100) { //p is a number between 0 and 100
        cout << "Numarul trebuie sa fie cuprins intre 0 si 100" << endl;
        exit(0); //exit the program
    }
    cout << "Introdu numele fisierului (exemplu: \"11eil51.gtsp\")" << endl;
    cin >> file; //name of the file should be "<name>.<extension>"
}

int main() {
    int p; string file; //console input
    consoleInput(p, file); //reading p and the file name
    Points points = Points(file.c_str()); //reading the points from file
    int n = (int)(1.0*points.x * p / 100 + 0.5); //n points to be visited (formula)
    points.solve(n, file.c_str());
    return 0;
}
