#include <iostream>
#include"UrlParse.h"
#include"MutexRAII.h"

using namespace std;


int FinshSign;
int main() {
    clock_t start = clock();
    UrlParse m_UrlParse;
    m_UrlParse.Init("http://en.cppreference.com/w/");


   // while(1)
    {

        sleep(200);
       // cout<<"Main"<<endl;
    }
    clock_t ends = clock();
    cout <<"Running Time : "<<(double)(ends - start)/ CLOCKS_PER_SEC << endl;

    return 0;
}

