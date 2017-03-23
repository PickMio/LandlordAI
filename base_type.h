/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   base_type.h
 * Author: admin
 *
 * Created on 2017年3月23日, 下午5:28
 */

#ifndef BASE_TYPE_H
#define BASE_TYPE_H

#include "base_card.h"

//牌型的基类
class BaseType{
public:
    BaseType(int32_t id);
    virtual ~BaseType() {};
public:
    //默认认为传入的牌值都是排序好的
    //是否是这种类型
    virtual bool is_type(const CardsPile &cards) = 0;
    //获取这个牌型的值, 用来做比较, 最先比较weight, 再比较这个
    //除炸弹外, 这个数值就是该数值的最大一张牌的值, 炸弹应该加上偏移量
    virtual int32_t get_value(const CardsPile &cards) = 0; 
    int32_t get_type_id();
    //设置为炸弹, 可以管任何牌型
    void set_bomb();
    //是否是炸弹
    bool is_bomb();
    //获取配牌的idx, 没有配牌返回空数组
    CardsIdx get_matchcard_index(const CardsPile &cards);
protected:
    int32_t value_;//牌型的值, 在判断牌型的时候记录下来, 每次计算牌型重新初始化
private:    
    int32_t type_id_;
    
    bool    bomb_;
    
    
};


#endif /* BASE_TYPE_H */

