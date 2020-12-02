//
// Created by 王尚荣 on 2020/11/8.
//

#ifndef BUILDGRAPH_AC_AUTOMACHINE_H
#define BUILDGRAPH_AC_AUTOMACHINE_H

#include <string.h>
#include "queue.h"
#include <iostream>
using namespace std;

/**
 * AC 自动机
 */
class AC_automachine {
public:
    AC_automachine();
    ~AC_automachine();
    void Insert(const string& pattern);
    int Build();
    Node* Match(char* text, Node* start=nullptr);
    vector<int>* MatchByFile(const string& filename, char* mode="rb");
    int findKey(const string&);
    void clear();
public:
    vector<string> urls;                 // 保存url
private:
    Node* root;                          // 根节点
    vector<int> match_result;            // 一个html中的匹配结果
    int idx = 0;
};

#endif //BUILDGRAPH_AC_AUTOMACHINE_H
