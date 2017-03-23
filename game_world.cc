/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "game_world.h"
GameWorld::GameWorld(){
    //这里还需要调整, 只有一桌, 卡牌最好要桌子自己来管理
   shuff_times_ = MAX_FHUFFLE_COUNT;
   table_ = nullptr;
   double_cards_ = true;
    srand(time(NULL));
    init_cards();
    table_ = new TableItem(const_cards_pile_);
    ready_game(table_);
    
}
void GameWorld::ready_game(TableItem *table){
    table->reset_table();
    shuffle();
}

//准备扑克
void GameWorld::init_cards(){
    const_cards_pile_.clear();
    BaseCard card;
    for( int32_t c = CARD_INDEX_THREE; c < CARD_INDEX_LJOKER; ++c ){
        for(int32_t k = CARD_COLOR_DIAMOND; k < CARD_COLOR_END; ++k){
            card.set_card(card_value[c], card_index[c], card_color[k], const_cards_pile_.size());
            const_cards_pile_.push_back(card);
        }
    }
    //放入大小王
    card.set_card(CARD_VALUE_LJOKER, CARD_INDEX_LJOKER, CARD_COLOR_NONE, const_cards_pile_.size());
    const_cards_pile_.push_back(card);
    card.set_card(CARD_VALUE_JOKER, CARD_INDEX_JOKER, CARD_COLOR_NONE, const_cards_pile_.size());
    const_cards_pile_.push_back(card);    
    if(double_cards_){
        auto tmp = const_cards_pile_;
        const_cards_pile_.insert(const_cards_pile_.end(), tmp.begin(), tmp.end());        
    }
    
}
void GameWorld::shuffle(){
    int32_t count = table_->cards_pile_.size();
    int32_t start = 0;
    int32_t end = 0;    
    for(int c = 0; c < shuff_times_; ++c ){
        start = rand()%count;
        end = rand()%count;
        std::swap(table_->cards_pile_[start], table_->cards_pile_[end]);        
    }    
}

//玩家坐下
void GameWorld::on_player_sitdonw(IPlayer *pl){
    table_->player_sitdown(pl);
        
} 

//玩家准备
void GameWorld::on_player_ready(IPlayer *pl){
    table_->player_ready(pl);
        
        
}
//玩家离开
void GameWorld::on_player_leave(IPlayer *pl){
    
}
//玩家出牌
bool GameWorld::on_player_discard(IPlayer *pl, const NetCards& cards){
    return table_->player_discard(pl, cards);
    
}