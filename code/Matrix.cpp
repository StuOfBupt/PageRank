//
// Created by 王尚荣 on 2020/11/7.
//

#include <iostream>
#include "Matrix.h"
#include <numeric>

Matrix::Matrix(int N): column_idx(vector<vector<int>>(N)), values(vector<vector<double>>(N)){
    UrlNum = N;
    base = 0;
}


/**
 * 向矩阵Matrix[i][j] 中添加元素
 * @param i
 * @param j
 * @param value
 */
void Matrix::add(int i, int j, double value) {
    if(i < 0 || j < 0 || i >= UrlNum || j >= UrlNum){
        printf("i, j > UrlNum\n");
        return;
    }
    int idx = _find(i, j);
    // 若 i,j 已存在值 则直接相加 否则创建新值
    if(idx == -1){
        column_idx[i].push_back(j);
        values[i].push_back(value);
    }else
        values[i][idx] += value;
}

double Matrix::get(int i, int j)const {
    if(i < 0 || j < 0 || i >= UrlNum || j >= UrlNum){
        printf("i, j > UrlNum\n");
        return 0.0;
    }
    int idx = _find(i, j);
    // 查不到，为base
    if(idx == -1)
        return base;
    else
        return values[i][idx] + base;
}

void Matrix::print() {
    printf("UrlNum:%d\n", UrlNum);
    printf("columns idx:\n");
    for(int i = 0; i < UrlNum; i++){
        for(int j = 0; j < column_idx[i].size(); j++){
            printf("%d ", column_idx[i][j]);
        }
        printf("\n");
    }
    printf("*****************************\n\nvalues idx:\n");
    for(int i = 0; i < UrlNum; i++){
        for(int j = 0; j < column_idx[i].size(); j++){
            printf("%lf ", values[i][j]);
        }
        printf("\n");
    }

}
/**
 * 矩阵保存在.txt文件中
 * 保存格式(三元组格式保存)：
 * LineNum
 * UrlNum
 * i    j     value
 * @param file_name
 */
void Matrix::save(const string &file_name) {
    ofstream out = ofstream(file_name, ofstream::out);
    out<<UrlNum<<endl;
    for(int i = 0; i < UrlNum; i++){
        for(int j = 0; j < column_idx[i].size(); j++){
            out << i << "\t" << column_idx[i][j] << "\t" <<  values[i][j] << endl;
        }
    }
    cout << "save matrix in " << file_name << " success" << endl;
    out.close();
}
/**
 * 用于直接设置值
 * @param i
 * @param j
 * @param value
 */
void Matrix::set(int i, int j, double value) {
    if(value != 0.0){
        column_idx[i].push_back(j);
        values[i].push_back(value);
    }
}

/*
 * 矩阵与常数相乘
 */
Matrix &Matrix::operator*(double alpha) {
    for(int i = 0; i < UrlNum; i++){
        for(int j = 0; j < values[i].size(); j++){
            values[i][j] *= alpha;
        }
    }
    return *this;
}

void Matrix::set_base(double b) {
    base = b;
}
/**
 * 优化：矩阵与列向量相乘
 * @param p 列向量
 * @return
 */
vector<double> Matrix::operator*(const vector<double>& p) const {
    vector<double> p1(UrlNum);
    double sum = accumulate(p.begin(), p.end(), 0.0);
    sum *= base;
    for(int i = 0; i < UrlNum; i++){
        double val = 0;
        const vector<int>& col = vector<int>(column_idx[i]);
        for(int j = 0; j < col.size(); j++){
            val += values[i][j] * p[col[j]];
        }
        p1[i] = val + base;
    }
    return p1;
}
/**
 * 查看一下第orw行有没有列号为col
 * @param row
 * @param col
 * @return idx or -1
 */
int Matrix::_find(int row, int col)const {
    for(int i = 0 ; i < column_idx[row].size(); i++)
        if(column_idx[row][i] == col)
            return i;
    return -1;
}

/**
 * 从格式化txt中加载矩阵
 * @param file_name
 * @return
 */
Matrix* load_matrix(const string &file_name) {
    ifstream in = ifstream(file_name, ifstream::in);
    int N;
    cout << "start read " << file_name << endl;
    in >> N;
    auto* M = new Matrix(N);
    int i, j;
    double value;
    string Line;
    while(in >> i >> j >> value){
        M->set(i, j, value);
    }
    in.close();
    cout << "load matrix from " << file_name << " success" << endl;
    return M;
}

/**
 * 保存urls信息
 * 格式：
 * UrlNum
 * @param file_name
 */
void save_urls(const string &file_name, const vector<string>& urls) {
    ofstream out = ofstream(file_name, ofstream::out);
    out << urls.size() << endl;
    for(int i = 0; i < urls.size(); i++)
        out << urls[i] << endl;
    out.close();
    cout << "save urls in " << file_name << " success" << endl;
}

vector<string> load_urls(const string &file_name) {
    ifstream in = ifstream(file_name, ifstream::in);
    int N = 0;
    cout << "start read file" << file_name << endl;
    in >> N;
    vector<string> urls(N);
    string Line;
    int i = 0;
    while(in >> Line){
        urls[i++] = Line;
    }
    cout << "load urls from " << file_name << " success" << endl;
    in.close();
    return urls;
}

/**
 * 两个列向量做差后取模的平方
 * @param v1
 * @param v2
 * @return
 */
double get_mod(const vector<double> &v1, const vector<double> &v2) {
    double sum = 0;
    for(int i =0; i < v1.size(); i++){
        double x = v1[i] - v2[i];
        sum += x * x;
    }
    return sum;
}

