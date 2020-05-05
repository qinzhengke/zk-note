// normal_distribution
#include <iostream>
#include <string>
#include <random>
#include "matplotlibcpp.h"

using namespace std;
namespace plt = matplotlibcpp;

float func_f(float a, float b, float x){
    return exp(a * x * x + b * x);
}

float func_s(float a, float b, vector<float> x, vector<float> y){
    float sum = 0.0;
    for(int i=0; i<x.size(); i++){
        sum += (y[i] - func_f(a,b,x[i])) * (y[i] - func_f(a,b,x[i]));
    }

    return sum;
}


float ga(float a, float b, vector<float> x, vector<float> y){
    float sum = 0.0;
    for(int i=0; i<x.size(); i++){
        sum += (y[i] - func_f(a,b,x[i]))*(-func_f(a,b,x[i]))*x[i]*x[i];
    }
    sum *= 2.0;

    return sum;
}

float gb(float a, float b, vector<float> x, vector<float> y){
    float sum = 0.0;
    for(int i=0; i<x.size(); i++){
        sum += (y[i] - func_f(a,b,x[i]))*(-func_f(a,b,x[i]))*x[i];
    }
    sum *= 2.0;

    return sum;
}


int generate_data(float a, float b,
                            float min, float max, int N, float noise_mean,
                            float noise_cov,
                            vector<float> &x, vector<float> &y){
    x.clear();
    y.clear();

    std::default_random_engine generator;
    std::normal_distribution<double> distribution(noise_mean, noise_cov);

    float step = (max-min)/(float)N;
    for(int i=0; i<N; i++){
        float x_ = min + i*step;
        double noise = distribution(generator);
        x.push_back(x_);
        y.push_back(func_f(a,b,x_)+noise);
    }

    return 0;
}

int generate_s_map(const vector<float> &x, const vector<float> &y,
                   float a_min, float a_max, int a_num,
                   float b_min, float b_max, int b_num,
                   vector<vector<float>> &ma,
                   vector<vector<float>> &mb,
                   vector<vector<float>> &s){
    ma.clear();
    mb.clear();
    s.clear();

    float a_step = (a_max-a_min)/(float)a_num;
    float b_step = (b_max-b_min)/(float)b_num;
    for(int r=0; r<a_num; r++){
        float a = a_min + r*a_step;
        vector<float> a_row, b_row, s_row;
        for(int c=0; c<b_num; c++){
            float b = b_min + c * b_step;
            a_row.push_back(a);
            b_row.push_back(b);
            s_row.push_back(func_s(a, b, x, y));
        }
        ma.push_back(a_row);
        mb.push_back(b_row);
        s.push_back(s_row);
    }
    return 0;
}

void plot_data(vector<float> x, vector<float> y, vector<float> y_ref,
               vector<float> y_target){
    plt::scatter(x,y);
    plt::plot(x,y_ref);
    plt::plot(x,y_target);
    plt::show();
}

void plot_map(vector<vector<float>> a, vector<vector<float>> b,
              vector<vector<float>> s){
    plt::plot_surface(a, b, s);
    plt::show();
}


int opt_gradient(float step, float delta, int max_iter_num,
                 vector<float> x, vector<float> y,
                 float &a, float &b,
                 bool &success_flag, int &final_iter_num, float &final_err){

    success_flag = false;

    int i;
    for(i=0; i<max_iter_num && func_s(a,b,x,y)>delta; i++){
        a += ga(a,b,x,y)*step;
        b += gb(a,b,x,y)*step;
        #if 1   // Debug code.

        #endif
    }

    final_iter_num = i;
    final_err = func_s(a,b,x,y);
    success_flag = final_err <= delta ? true : false;

#if 1   // Debug code.
    cout<<"result:"<<success_flag<<", a:"<<a<<",b="<<b;
    cout<<", final_iter_num:"<<final_iter_num<<", final_err:"<<final_err<<endl;
#endif

    return 0;
}

int main(int argc, char** argv){
    vector<float> x,y,y_t;
    float a = 1.0, b = 2.0;
    generate_data(a, b, -1.0, 1.0, 100, 0.0, 0.1, x, y);
    generate_data(a, b, -1.0, 1.0, 100, 0.0, 0.0, x, y_t);
    float a_=0.0, b_=0.0;
    bool success_flag;
    float final_err;
    int final_iter_num;
    opt_gradient(0.01, 0.001, 100, x, y, a_, b_, success_flag, final_iter_num,
                 final_err);
    vector<float> y1;
    generate_data(a_, b_, -1.0, 1.0, 100, 0.0, 0.0, x, y1);

    vector<vector<float>> ma, mb, ms;
    generate_s_map(x, y, a-0.5, a+0.1, 100, b-5.0, b+1.0, 100, ma, mb, ms);

//    cout<<"s(a,b)="<<func_s(a,b,x,y)<<endl;
//    cout<<"s(a-0.5,b-0.5)="<<func_s(a-0.5,b-0.5,x,y)<<endl;
//    cout<<"s(a+0.5,b+0.5)="<<func_s(a+0.5,b+0.5,x,y)<<endl;

//    plot_data(x, y, y_t, y1);
    plot_map(ma, mb, ms);

    return 0;
}
