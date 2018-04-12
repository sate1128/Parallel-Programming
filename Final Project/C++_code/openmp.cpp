#include <iostream>
#include <math.h>
#include <fstream>
#include <string.h>
#include <string>
#include <vector>
#include "omp.h"
#include <time.h>
#define N 304713

using namespace std;



vector<float> calculate_idf(vector<string>input, vector<vector<string> > line){
    vector<float> idf;
    int count_word;
    int count_sum;
    unsigned int v;
    unsigned int i;
    unsigned int j;
    for(v=0;v<296;v++){ //input.size()==300
        count_sum=0;
        #pragma omp parallel for private(i,j,count_word) reduction (+:count_sum)  num_threads(1)
        for(int i=0;i<N;i++){
            count_word=0;
            for(int j=0;j<line[i].size();j++){
                if(input[v]==line[i][j]){
                    count_word=1;
                }
            }
            count_sum+=count_word;
        }
        idf.push_back(log10(N/count_sum));
    }

    return idf;
}


vector<int> calculate_tf(vector<string>input, vector<string> task){
    vector<int> tf;
    int count_word;


    for(int j=0;j<296;j++){  //input.size()==300
        count_word=0;
        for (unsigned int k=0;k<task.size();k++){
            if(input[j]==task[k]){
                count_word+=1;
            }
        }
        tf.push_back(count_word);
    }

    return tf;
}
vector<float> calculate_tfidf(vector<int>tf, vector<float> idf){
    vector<float> tfidf;
    for(int i=0;i<296;i++){  //input.size()==300
        tfidf.push_back(tf[i]*idf[i]);
    }

    return tfidf;
}



int main(){
    clock_t tStart = clock();
    fstream input_file,word_file,task_file;

    input_file.open("token.txt",ios::in);
    word_file.open("300input.txt",ios::in);
    task_file.open("task1.txt",ios::in);

    string buff;
    char* buff2;

    vector<vector<string> > line;
    line.resize(N);

    vector<string> task[10];
    vector<string> input;
    vector<int> tf[10];
    vector<float> idf;
    vector<float> tfidf[10];

    //read test.txt
    int i=0;

    while(getline(input_file,buff)){
        char *word = new char[buff.length() + 1];
        strcpy(word, buff.c_str());
        buff2=strtok(word," ");
        while(buff2){
            line[i].push_back(buff2);
            buff2=strtok(NULL," ");
        }

        i=i+1;
    }

    //read 300input
    while(getline(word_file,buff)){
        char *word = new char[buff.length() + 1];
        strcpy(word, buff.c_str());
        buff2=strtok(word," ");
        while(buff2){
            input.push_back(buff2);
            buff2=strtok(NULL," ");
        }
    }
    //read task
    i=0;
    while(getline(task_file,buff)){
        char *word = new char[buff.length() + 1];
        strcpy(word, buff.c_str());
        buff2=strtok(word," ");
        while(buff2){
            task[i].push_back(buff2);
            buff2=strtok(NULL," ");
        }
        i=i+1;
    }
    int line_number=i;
    clock_t tStart2 = clock();
    idf = calculate_idf(input,line);
    printf("PTime taken: %.2fs\n", (double)(clock() - tStart2)/CLOCKS_PER_SEC);
    for(i=0;i<line_number;i++){
        tf[i] = calculate_tf(input,task[i]);
        tfidf[i] = calculate_tfidf(tf[i],idf);
    }
    
    
    printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
    return 0;


}
