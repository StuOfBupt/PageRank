#include <iostream>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include "Matrix.h"
#include "AC_automachine.h"
#define MODE_INSERT 1
#define MODE_MATCH  2
Matrix* matrix = nullptr;
AC_automachine* ac = nullptr;
string graph_file = "graph.txt";
string urls_file = "urls.txt";
double alpha = 0.85;
/**
 * 匹配文件中的url
 * @param file_name 文件名
 * @param path      模式串
 */
void match_url(char* file_name, const string& pattern){
    vector<int>* result = ac->MatchByFile(string(file_name), "rb");
    if(result){
        int src = ac->findKey(pattern);
        int length = (*result).size();
        for(int i = 0 ;i < length; i++){
            // 消除自引用导致的排名上升问题
//            if(src == (*result)[i])
//                continue;
            matrix->add((*result)[i], src, 1.0 / length);
        }
    }
}
/**
 * 递归读取目录,获得所有模式串
 * @param dirname 目录名
 * @param path    当前路径
 */
void read_dir(const char* dirname, const string& path, int mode){
    DIR *dir;
    struct dirent *entry;
    struct stat statbuf;
    if((dir = opendir(dirname)) == NULL){
        printf("Can't open dir:%s\n", dirname);
        return;
    }
    chdir(dirname);
    while((entry = readdir(dir)) != NULL){
        lstat(entry->d_name, &statbuf);
        if(S_ISDIR(statbuf.st_mode)){
            if((strcmp(entry->d_name, ".") == 0 ) || (strcmp(entry->d_name, "..") == 0))
                continue;
//            printf("dir  name:%s%s\n",path.c_str(), entry->d_name);
            read_dir(entry->d_name, path + string(entry->d_name) + "/", mode);
        }else{
//            printf("file name:%s%s\n",path.c_str(), entry->d_name);
            string file_name = path + string(entry->d_name);
            if(mode == MODE_INSERT){
                ac->Insert(file_name);
            }
            else{
                match_url(entry->d_name, file_name);
            }

        }
    }
    closedir(dir);
    chdir("..");
}
void generate_graph(Matrix*& mat, const string& mat_txt, const string& url_txt){
    ac = new AC_automachine();
    read_dir("webdir", "http://news.sohu.com/", MODE_INSERT);
    int N = ac->Build();
    mat = new Matrix(N);
    read_dir("webdir", "http://news.sohu.com/", MODE_MATCH);
    save_urls(url_txt, ac->urls);
    mat->save(mat_txt);

    delete ac;
    ac = nullptr;
}
vector<string> LoadData(Matrix*& mat, const string& mat_txt, const string& url_txt){
    if(mat == nullptr){
        mat = load_matrix(mat_txt);
    }
    return load_urls(url_txt);
}
int main() {
    printf("pid:%d\n", getpid());
    chdir("..");
    //generate_graph(matrix, graph_file, urls_file);
    chdir("..");
    vector<string> urls = LoadData(matrix, graph_file, urls_file);
    (*matrix)*alpha;
    matrix->set_base((1.0 - alpha) / matrix->UrlNum);
    double e = 1e-8;
    vector<double> p(matrix->UrlNum, 1.0 / matrix->UrlNum);
    int count = 1;
    while(true){
        cout << "iter: " << count << endl;
        vector<double> p1 = (*matrix) * p;
        if(get_mod(p1, p) < e)
            break;
        p = p1;
        count++;
    }
    cout << "finish" <<endl;
    vector<pair<double, int>> url_idx;
    for(int i = 0; i < p.size(); i++){
        url_idx.emplace_back(p[i], i);
    }
    sort(url_idx.begin(), url_idx.end(), [](pair<double, int>& o1, pair<double, int>& o2){ return o2.first < o1.first;});
    for(int i = 0 ; i < 20; i++){
        cout << i << ": " << urls[url_idx[i].second] << " PR:" << url_idx[i].first << endl;
    }
    return 0;

}
