/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   base_card.h
 * Author: admin
 *
 * Created on 2017年3月23日, 下午5:26
 */

#ifndef BASE_CARD_H
#define BASE_CARD_H

#include "game_def.h"
class BaseCard{
public:
    BaseCard();
    ~BaseCard();
public:
    //获取牌名
    void set_card(CardValue v, CardIndex i, CardColor c , int32_t id);
    std::string get_name();
    CardValue get_value() const;
    CardColor get_color() const;
    CardIndex get_index() const;
    int32_t get_id() const;
    bool valid() const;
    //是否是配牌
    bool match_card() const;
    bool operator < (const BaseCard &m) const {
                return card_value_ < m.card_value_;
        }
    
private:
    //牌值, 用来比较牌值大小的
    CardValue card_value_;
    //牌的花色
    CardColor card_color_;    
    // 牌的索引, 用来指明是哪张牌的
    CardIndex card_index_;
    //是否是配牌
    bool      is_matching_card_; 
    //每副牌用来唯一标识一张牌的id,当有多副牌时用来区别牌
    int32_t card_id_;
    
};

//牌堆
using CardsPile = std::vector<BaseCard>;


#endif /* BASE_CARD_H */

