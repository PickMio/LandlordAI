/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "iplayer.h"
int32_t IPlayer::user_count_ = 0;


IPlayer::IPlayer(GameWorld *w){
    identify_ = ++user_count_;
    status_ = PLAYER_STATUS_NONE;
    world_ = w;

}

IPlayer::~IPlayer(){
    
}
void IPlayer::sitdown(){
    world_->on_player_sitdonw(this);
}
void IPlayer::ready(){
    world_->on_player_ready(this);
}
bool IPlayer::put_card(){
    NetCards cards;
    if(!hand_cards_.empty()){
        cards.push_back(hand_cards_[rand() % hand_cards_.size()].get_id());        
    }
    if(false == world_->on_player_discard(this, cards) ){
        cards.clear();
        world_->on_player_discard(this, cards);
    }
    return true;
    
}



void IPlayer::get_card(BaseCard c){
    hand_cards_.push_back(c);
}
bool IPlayer::del_card(const NetCards& cards){
    for( auto v : cards){
        auto it = hand_cards_.begin();
        for(; it != hand_cards_.end(); ++it){
            if((*it).get_id() == v){
                break;
            }
        }
        //有卡牌不在手牌中 
        if( it == hand_cards_.end() ){
            return false;
        }
    }
    for( auto v : cards){
        for(auto it = hand_cards_.begin(); it != hand_cards_.end(); ++it){
            if((*it).get_id() == v){
                hand_cards_.erase(it);
                break;
            }
        }
    }
    return true;
    
}
