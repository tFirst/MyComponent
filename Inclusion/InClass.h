#include <vector>
#include <iostream>
#include <math.h>

using namespace std;

class InterBi{
private:
    double x1;
    double y1;

    double x2;
    double y2;

    vector<double> x;
    vector<double> y;

public:
    InterBi(double x1, double x2, double y1, double y2, int units){
        this->x1 = x1;
        this->x2 = x2;
        this->y1 = y1;
        this->y2 = y2;
        this->x.resize(units);
        for(int i = 0; i < units; i++){
            this->x[i] = x1 + i * (x2-x1)/(units - 1);
        }
        this->y.resize(units);
        for(int i = 0; i < units; i++){
            this->y[i] = y1 + i * (y2-y1)/(units - 1);
        }
    }

    vector<double> getX(){
        return this->x;
    }

    vector<double> getY(){
        return this->y;
    }

    //функция
    double function(double x, double y){
        return (sin(x*y) * y);
    }

    vector< vector<double> > interpol(vector<double> x, vector<double> y){
        double R1, R2, Q11, Q12, Q21, Q22;

        vector< vector<double> > result;
        result.resize(x.size());
        for(int i = 0; i < x.size(); i ++){
            result[i].resize(y.size());
        }

        for(int i = 0; i < x.size(); i++){
            for(int j = 0; j < y.size(); j++)
                result[i][j] = function(x[i], y[j]);
            }

        for(int i = 1; i < x.size() - 1; i ++){
            for(int j = 1; j < y.size() - 1; j ++){
                Q11 = function(x[i - 1], y[j - 1]);
                Q12 = function(x[i - 1], y[j + 1]);
                Q21 = function(x[i + 1], y[j - 1]);
                Q22 = function(x[i + 1], y[j + 1]);
                R1 = Q11 * (x[i + 1] - x[i])/(x[i + 1] - x[i - 1]) + Q21 * (x[i] - x[i - 1])/(x[i + 1] - x[i - 1]);
                R2 = Q12 * (x[i + 1] - x[i])/(x[i + 1] - x[i - 1]) + Q22 * (x[i] - x[i - 1])/(x[i + 1] - x[i - 1]);
                result[i][j] = R1 * (y[j + 1] - y[j])/(y[j + 1] - y[j - 1]) + R2 * (y[j] - y[j - 1])/(y[j + 1] - y[j - 1]);
            }
        }
        return result;
    }
};


class InterTri{
private:
    double x1;
    double y1;
    double z1;

    double x2;
    double y2;
    double z2;

    vector<double> x;
    vector<double> y;
    vector<double> z;

public:
    InterTri(double x1, double x2, double y1, double y2, double z1, double z2, double units){
        this->x1 = x1;
        this->x2 = x2;
        this->y1 = y1;
        this->y2 = y2;
        this->z1 = z1;
        this->z2 = z2;

        this->x.resize(units);
        for(int i = 0; i < units; i++){
            this->x[i] = x1 + i * (x2-x1)/(units - 1);
        }
        this->y.resize(units);
        for(int i = 0; i < units; i++){
            this->y[i] = y1 + i * (y2-y1)/(units - 1);
        }
        this->z.resize(units);
        for(int i = 0; i < units; i++){
            this->z[i] = z1 + i * (z2-z1)/(units - 1);
        }
    }

    vector<double> getX(){
        return this->x;
    }

    vector<double> getY(){
        return this->y;
    }
    vector<double> getZ(){
        return this->z;
    }

    //функция
    double function(double x, double y, double z){
        return (sin(x*y) * y + cos(z));
    }

     vector< vector< vector<double> > > interpol(vector<double> x, vector<double> y, vector<double> z){

        vector< vector< vector<double> > > result;
        result.resize(x.size());
        for(int i = 0; i < x.size(); i ++){
            result[i].resize(y.size());
            for (int j = 0; j < y.size(); j++){
                result[i][j].resize(z.size());
            }
        }

        for(int i = 0; i < x.size(); i++){
            for(int j = 0; j < y.size(); j++){
                for(int k = 0; k < z.size(); k++)
                    result[i][j][k] = function(x[i], y[j], z[k]);
            }
        }

        for(int i = 1; i < x.size() - 1; i ++){
            for(int j = 1; j < y.size() - 1; j ++){
                    for(int k = 1; k < z.size() - 1; k ++){
                        result[i][j][k] = function(x[i-1], y[j-1], z[k-1]) * (x[i+1] - x[i]) * (y[j+1] - y[j]) * (z[k+1] - z[k]) +
                                          function(x[i-1], y[j-1], z[k+1]) * (x[i+1] - x[i]) * (y[j+1] - y[j]) * (z[k] - z[k-1]) +
                                          function(x[i-1], y[j+1], z[k-1]) * (x[i+1] - x[i]) * (y[j] - y[j-1]) * (z[k+1] - z[i]) +
                                          function(x[i-1], y[j+1], z[k+1]) * (x[i+1] - x[i]) * (y[j] - y[j-1]) * (z[k] - z[k-1]) +
                                          function(x[i+1], y[j-1], z[k-1]) * (x[i] - x[i-1]) * (y[j+2] - y[j]) * (z[k+1] - z[k]) +
                                          function(x[i+1], y[j-1], z[k+1]) * (x[i] - x[i-1]) * (y[j+2] - y[j]) * (z[k] - z[k-1]) +
                                          function(x[i+1], y[j+1], z[k-1]) * (x[i] - x[i-1]) * (y[j] - y[j-1]) * (z[k+1] - z[k]) +
                                          function(x[i+1], y[j+1], z[k+1]) * (x[i] - x[i-1]) * (y[j] - y[j-1]) * (z[k] - z[k-1]);
                        result[i][j][k] /= (x[i+1] - x[i-1]) * (y[j+1] - y[j-1]) * (z[k+1] - z[k-1]);
                    }
            }
        }
        return result;
    }
};
