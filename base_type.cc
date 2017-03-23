/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "base_type.h"

BaseType::BaseType(int32_t id){
    this->type_id_ = id;
    value_ = 0;
    bomb_ = false;    
}

int32_t BaseType::get_type_id(){
    return type_id_;
}

void BaseType::set_bomb(){
    this->bomb_ = true;
}
bool BaseType::is_bomb(){
    return this->bomb_;
}
CardsIdx BaseType::get_matchcard_index(const CardsPile &cards){
    CardsIdx idx;
    for(int32_t c = 0; c < cards.size(); ++c){
        if(cards[c].match_card()){
            idx.push_back(c);            
        }
    }
    return idx;
}