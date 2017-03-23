/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cc
 * Author: admin
 *
 * Created on 2017年3月20日, 上午10:43
 */

#include <cstdlib>
#include <string>
#include <stack>
#include <vector>
#include "log.h"
#include "poker_types.h"
#include "game_world.h"
#include "iplayer.h"
using namespace std;

//向一个vector 插入空的vector 时会是什么样的
void test_vector_void(){
    using ivec = std::vector<int>;
    ivec data1;
    vector<ivec> p;
    p.push_back(data1);
    cout << "p.size()" << p.size() << endl;
    
    
}
void test(){
    GameWorld *world = new GameWorld();    
    
    
    IPlayer *p1 = new IPlayer(world);
    IPlayer *p2 = new IPlayer(world);
    IPlayer *p3 = new IPlayer(world);
    IPlayer *p4 = new IPlayer(world);
    IPlayer *p5 = new IPlayer(world);
    p1->sitdown();
    p1->ready();
    p2->sitdown();
    p3->sitdown();
    p4->sitdown();
    p2->ready();
    p3->ready();
    p4->ready();
    p5->sitdown();
    
    while(p1->status_ != PLAYER_STATUS_GAME_END)
    {
        p1->put_card();
        p2->put_card();
        p3->put_card();
        p4->put_card();
    }
    
    
    
    
    
    
}

void test_sub(){
    //这题涉及到编译原理的表达式匹配, 要用到栈
    //string s = "nvbjdghkafefvfsssszzzzqqq";
    string s = "aaabaaaaa";
    int start = 0;
    int end = 0;
    int sz = s.size() - 1;
    
    string v;
    for (int c = 0; c < s.size(); ++c){
        int endpos = sz;
        int startpos = c;
        start =c;
        end = sz;
        while( startpos <= endpos){
            if( s[startpos] != s[endpos] ){               
                startpos = c;
                --end;
                endpos = end;
                continue;
            }
            ++startpos;
            --endpos;
        }
        if(start <= end){            
            string tmp;
            tmp = s.substr(start, end - start + 1);
            if(tmp.size() > v.size()){
                v = tmp;
            }
        }
    }
    
    cout << v << endl;
}
    
    
/*
 * 
 */
int main(int argc, char** argv) {
    LOGInfo log;
    log.Init("main");
    test();
    LOG(INFO) << "game over";
    //test_vector_void();
    
    
    
    return 0;
}

