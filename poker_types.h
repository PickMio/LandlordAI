/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   poker_types.h
 * Author: admin
 *
 * Created on 2017年3月20日, 上午10:44
 */

#ifndef POKER_H
#define POKER_H

#include "game_def.h"
#include "base_card.h"
#include "base_type.h"




//单张牌
class SingleType : public BaseType{
public:
    SingleType();
    
    bool is_type(const CardsPile &cards) override;
    //获取这个牌型的值, 用来做比较, 最先比较weight, 再比较这个
    int32_t get_value(const CardsPile &cards) override; 
};


//对牌
class DoubleType : public BaseType{
public:
    DoubleType();
    
    bool is_type(const CardsPile &cards) override;
    //获取这个牌型的值, 用来做比较, 最先比较weight, 再比较这个
    int32_t get_value(const CardsPile &cards) override; 
};





//三张, 三张牌一样的
class TribleType : public BaseType{
public:
    TribleType();
    
    bool is_type(const CardsPile &cards) override;
    //获取这个牌型的值, 用来做比较, 最先比较weight, 再比较这个
    int32_t get_value(const CardsPile &cards) override; 
};


//三连对
class TribleDoubleType : public BaseType{
public:
    TribleDoubleType();
    
    bool is_type(const CardsPile &cards) override;
    //获取这个牌型的值, 用来做比较, 最先比较weight, 再比较这个
    int32_t get_value(const CardsPile &cards) override; 
};


//顺子, 可能是同花顺
class StraightType : public BaseType{
public:
    StraightType();
    
    bool is_type(const CardsPile &cards) override;
    //获取这个牌型的值, 用来做比较, 最先比较weight, 再比较这个
    int32_t get_value(const CardsPile &cards) override; 
    bool check_same_color(const CardsPile &cards);
    const int32_t SAME_COLOR_OFFSET = 1000;//同花顺的偏移量
};





#endif /* POKER_H */

