#include <iostream>
#include <math.h>
#include <fstream>
#include <string.h>
#include <string>
#include <vector>
#include <future>
#include <time.h>

#define N 304713
#define W 296
#define thread_num 1

using namespace std;

struct input{
    vector<string> input;
    vector<vector<string> > line;
}arg;


vector<float> thread_idf(int number){

    vector<float> idf;
    int count_word;
    int count_sum;
    int bound;
    int start=number*W/thread_num;
    int stop=(number+1)*W/thread_num;

    for(int v=start;v<stop;v++){ //input.size()==300
        count_sum=0;
        for(int i=0;i<N;i++){
            count_word=0;
            for(int j=0;j<arg.line[i].size();j++){
                if(arg.input[v]==arg.line[i][j]){
                    count_word=1;
                }
            }
            count_sum+=count_word;
        }

        idf.push_back(log10(N/count_sum));


    }
    return idf;
}
vector<int> calculate_tf(vector<string> task){
    vector<int> tf;
    int count_word;


    for(int j=0;j<300;j++){  //input.size()==300
        count_word=0;
        for (int k=0;k<task.size();k++){
            if(arg.input[j]==task[k]){
                count_word+=1;
            }
        }
        tf.push_back(count_word);
    }

    return tf;
}
vector<float> calculate_tfidf(vector<int>tf, vector<float> idf){
    vector<float> tfidf;
    for(int i=0;i<300;i++){  //input.size()==300
        tfidf.push_back(tf[i]*idf[i]);
    }

    return tfidf;
}



int main(){
    fstream input_file,word_file,task_file;

    input_file.open("token.txt",ios::in);
    word_file.open("300input.txt",ios::in);
    task_file.open("task1.txt",ios::in);

    string buff;
    char* buff2;
    arg.line.resize(N);
    vector<string> task[10];

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
            arg.line[i].push_back(buff2);
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
            arg.input.push_back(buff2);
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

    /////////////////////////////////////

    int seconds1 = time((time_t*)NULL);
    future<vector<float> > fu0 =async(launch::async, thread_idf, 0);
    //future<vector<float> > fu1 =async(launch::async, thread_idf, 1);
    //future<vector<float> > fu2 =async(launch::async, thread_idf, 2);
    //future<vector<float> > fu3 =async(launch::async, thread_idf, 3);
    //future<vector<float> > fu4 =async(launch::async, thread_idf, 4);
    //future<vector<float> > fu5 =async(launch::async, thread_idf, 5);
    //future<vector<float> > fu6 =async(launch::async, thread_idf, 6);
    //future<vector<float> > fu7 =async(launch::async, thread_idf, 7);
    vector<float> test0=fu0.get();
    //vector<float> test1=fu1.get();
    //vector<float> test2=fu2.get();
    //vector<float> test3=fu3.get();
    //vector<float> test4=fu4.get();
    //vector<float> test5=fu5.get();
    //vector<float> test6=fu6.get();
    //vector<float> test7=fu7.get();
    idf.insert(idf.end(),test0.begin(),test0.end());
    //idf.insert(idf.end(),test1.begin(),test1.end());
    //idf.insert(idf.end(),test2.begin(),test2.end());
    //idf.insert(idf.end(),test3.begin(),test3.end());
    //idf.insert(idf.end(),test4.begin(),test4.end());
    //idf.insert(idf.end(),test5.begin(),test5.end());
    //idf.insert(idf.end(),test6.begin(),test6.end());
    //idf.insert(idf.end(),test7.begin(),test7.end());
    //////////////////////////////////////////////
    int seconds2 = time((time_t*)NULL);
    cout<<seconds2-seconds1<<" sec"<<endl;

    for(i=0;i<line_number;i++){
        tf[i] = calculate_tf(task[i]);
        tfidf[i] = calculate_tfidf(tf[i],idf);
    }

   /** for (i=0;i<10;i++){
        for(int j=0;j<tfidf[i].size();j++){
            cout<<tfidf[i][j]<<" ";
        }
        cout<<endl;
    }**/



    return 0;
}

