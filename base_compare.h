/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   base_compare.h
 * Author: admin
 *
 * Created on 2017年3月23日, 下午5:31
 */

#ifndef BASE_COMPARE_H
#define BASE_COMPARE_H
#include "base_type.h"
#include "base_card.h"


using TYPE_MAP = std::map<int32_t, BaseType*> ;
//比牌的基类
class BaseCompare{
public:
    BaseCompare();
    ~BaseCompare();    
public:
    //高16位为1 表示炸弹, 可以和任何牌型比较
    const int32_t CARD_TYPE_BOMB = 1 << 16;
public:    
    bool reg_compare(BaseType* type);
    //返回-1 表示不能比较大小, 牌型不对,  返回0表示右边牌大于左边牌, 可以管, 返回1 表示不能管
    int32_t compare(CardsPile& lsh, CardsPile& rsh);
    
    
private:
    TYPE_MAP cmp_map_;
    
};

#endif /* BASE_COMPARE_H */

