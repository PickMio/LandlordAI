/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "base_card.h"
BaseCard::BaseCard() {
    card_value_ = CARD_VALUE_LITTLE;
    card_color_   = CARD_COLOR_NONE;
    card_index_ = CARD_INDEX_NONE;
    
}

BaseCard::~BaseCard() {  
    
}

void BaseCard::set_card(CardValue v, CardIndex i, CardColor c, int32_t id){
    card_value_ = v;
    card_color_ = c;
    card_index_ = i;
    card_id_ = id;
}
//判断牌是否正确
bool BaseCard::valid() const{
    if( card_index_ < CARD_INDEX_END  && card_color_ < CARD_COLOR_END ){
        return true;
    }
    
    return false;
}
bool BaseCard::match_card() const{
    return is_matching_card_;
}

std::string BaseCard::get_name() {  
    if( !valid() ){
        //LOG ERROR
        return nullptr;
    }
    
    return colors[card_color_] + cards[card_index_];    
}

CardValue BaseCard::get_value() const{  
    if( !valid() ){
        //LOG ERROR
        return CARD_VALUE_LITTLE;
    }
    
    return card_value_;    
}

CardIndex BaseCard::get_index() const {  
    if( !valid() ){
        //LOG ERROR
        return CARD_INDEX_NONE;
    }
    
    return card_index_;    
}
int32_t BaseCard::get_id() const{
    return card_id_;
}

CardColor BaseCard::get_color() const{  
    if( !valid() ){
        //LOG ERROR
        return CARD_COLOR_NONE;
    }
    
    return card_color_;    
}
