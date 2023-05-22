//
// Created by Alex on 5/20/2023.
//

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

class Points {
public:
    struct Point {
        int x, y;
    };
    struct Route {
        vector<int> points; //list of all points from the route
        double distance; //total distance of route
    };
    Points(const char *file) //reading the file directly after the class is initialized
    {
        try {
            ifstream fin(file);
            if(!fin.good())
                throw invalid_argument("Error while reading.");
            int ok = 0; //checking if the line contains points or not
            while (!fin.eof()) {
                string line;
                getline(fin, line);
                if (!ok) {
                    if (line.find("NODE_COORD_SECTION") != string::npos) ok = 1; //start reading the points
                    else if (line.find("DIMENSION") != string::npos) //reading the dimension
                    {
                        //DIMENSION : 51 | DIMENSION: 318 => split ":"
                        vector<string> words;
                        string word;
                        istringstream is(line);
                        while (getline(is, word, ':')) //splitting text by ':', => DIMENSION, value
                            words.push_back(word);
                        this->x = stoi(words[1]); //words[0] is "DIMENSION", words[1] is value
                    }
                }
                else
                {
                    if(line.find("GTSP_SET_SECTION") != string::npos || line.find("EOF") != string::npos) break; // end of reading
                    istringstream is(line);
                    //poz, (x, y)
                    int i, x_coord, y_coord;
                    is >> i >> x_coord >> y_coord;

                    Point p = {x_coord, y_coord};
                    this->points.push_back(p); //adding points in the list
                }
            }
//            cout<<"Dimensiune: "<<this->x<<endl;
//            for(pair<int, int> i : points)
//                cout<<i.first<<" "<<i.second<<endl;
            fin.close();
            generateMatrix(this->matrix); //generating the matrix of distances
//            for(int i = 0; i<this->x; i++)
//            {
//                for(int j = 0; j<this->x; j++)
//                    cout<<matrix[i][j].first << ":"<<matrix[i][j].second << " ";
//                cout<<endl;
//            }
        }
        catch(invalid_argument& ex)
        {
            cout<<ex.what();
            exit(0);
        }
    }

    Route generateRoute(int start, int n_points) //generate a possible route using a start point
    {
        Route minimum = {{}, INT_MAX}; //define the minimum route for line K.
        for(int col = 1; col < this->x; col ++) {
            Route route;
            route.points.push_back(start); //first point will be the start
            int point, last = start;
            for (int i = 0; i < n_points; i++) {
                int k = 1; //k is the column of matrix.
                if(i == 0) k = col;
                do {
                    point = matrix[last][k].first; //assign to point the closer point
                    k++;
                } while (isElementInVector(point, route.points) &&
                         k < this->x); //check if point is already in list, if yes, then check the next one
                route.points.push_back(point); //add the found point
                last = point; //remember the last point to go to next closer point
            }
            route.points.push_back(start); // add at the end of list the starting point.
            route.distance = distanceTotal(route.points); //calculate the total distance of route
            if(route.distance < minimum.distance)
            {
                minimum.distance = route.distance;
                minimum.points = route.points;
            }
        }
        return minimum;
    }

    void solve(int n, const char *file)
    {
        Route min_route = {{}, INT_MAX}; //declare min_route variable
        for(int i = 0; i<this->x; i++)
        {
            Route route = generateRoute(i, n);
            if(route.distance < min_route.distance) //check if generatedRoute is minimum, if yes memorize it
            {
                min_route.distance = route.distance;
                min_route.points = route.points;
            }
        }

        //rewriting the output file name
        vector<string> words;
        string word;
        istringstream is(file);
        while (getline(is, word, '.')) //splitting text by '.', => name, gtsp
            words.push_back(word);
        //output the data
        ofstream fout((words[0] + ".out"));
        fout << "--- Puncte vizitate" << endl << n <<endl;
        fout << "--- Ordinea de vizitare" << endl;
        for(int i = 0; i<min_route.points.size() - 2; i++)
            fout<< min_route.points[i] + 1 << ", ";
        fout << min_route.points[min_route.points.size() - 1] + 1 << endl;
        fout << "--- Distanța totală calculată" << endl << (int)(min_route.distance + 0.5) << endl;
        fout.close(); //closing the file
    }

    vector<Point> points;
    int x; //x is the dimension
    vector<vector<pair<int, double>>> matrix; //matrix of dimensions, which is sorted
private:
    double distanceCalculator(Point p1, Point p2) //calculate the distance between two points
    {
        int dx = p2.x - p1.x, dy = p2.y - p1.y;
        return sqrt(dx*dx + dy*dy); //distance = sqrt(dx^2 + dy^2)
    }

    double distanceTotal(vector<int> pts) //pts = points
    {
        double dist = 0;
        for(int i = 0; i<pts.size()-2; i++)
            dist += distanceCalculator(this->points[pts[i]], this->points[pts[i+1]]);
        return dist;
    }

    static bool cmp(const pair<int, int> &a, const pair<int, int> b) //compare distances between two points for sorting
    {
        return (a.second < b.second); //returns true if a < b or false else.
    }

    bool isElementInVector(int value, vector<int> vect)
    {
        return (find(vect.begin(), vect.end(), value) != vect.end());
    }

    void generateMatrix(vector<vector<pair<int, double>>> &matrix) {
        //allocate and initialize the matrix with distances
        vector<vector<pair<int, double>>> tab(this->x, vector<pair<int, double>>(this->x));
        for (int i = 0; i < this->x; i++)
        {
            for (int j = 0; j < this->x; j++) {
                //tab[i][j] will have the structure x: value where x is number of point and value is distance, for example: 1: 30
                tab[i][j].first = j;
                tab[i][j].second = distanceCalculator(this->points[i], this->points[j]);
            }
            sort(tab[i].begin(), tab[i].end(), cmp); //sort the vector
        }
        matrix = tab;
    }
};