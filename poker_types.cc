/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "poker_types.h"


/*-------------------------单张牌型-------------------------------------------*/
SingleType::SingleType() : BaseType(1){
        
}

bool SingleType::is_type(const CardsPile &cards){
    value_ = 0;
    if( cards.size() != 1 ){
        return false;
    }
    value_ = cards[0].get_value();
    return true;
}

int32_t SingleType::get_value(const CardsPile &cards){
    return value_;
}
/*----------------------------------------------------------------------------*/



/*-------------------------对牌-----------------------------------------------*/
DoubleType::DoubleType() : BaseType(2){
        
}

bool DoubleType::is_type(const CardsPile &cards){
    value_ = 0;
    if( cards.size() != 2 ){
        return false;
    }
    
    //是对子
    if( cards[0].get_index() == cards[1].get_index()){
        value_ = cards[1].get_value();
        return true;
    }    
    return false;
}

int32_t DoubleType::get_value(const CardsPile &cards){    
    return value_;
}
/*----------------------------------------------------------------------------*/


/*-------------------------三张-----------------------------------------------*/
TribleType::TribleType() : BaseType(3){
        
}

bool TribleType::is_type(const CardsPile &cards){
    value_ = 0;
    if( cards.size() != 3 ){
        return false;
    }
    
    //是三张(三张牌的index一样)
    if( cards[0].get_index() == cards[1].get_index()  && 
           cards[0].get_index() == cards[3].get_index()){
        value_ = cards[0].get_value();
        return true;
    }
    return false;
}

int32_t TribleType::get_value(const CardsPile &cards){
    //如果有配牌, 配牌肯定排在最后, 其value最大 
    return value_;
}
/*----------------------------------------------------------------------------*/



/*-------------------------三连对---------------------------------------------*/
TribleDoubleType::TribleDoubleType() : BaseType(3){
        
}

bool TribleDoubleType::is_type(const CardsPile &cards){
    value_ = 0;
    if( cards.size() != 6 ){
        return false;
    }
    
    for( int c = 0; c < cards.size() - 2; c+=2 ){
        if( cards[c].get_index() != cards[c + 1].get_index() ){
            return false;
        }
    }
    //对A, 对2, 对3
    if( cards[0].get_index() ==  CARD_INDEX_THREE && 
        cards[2].get_index() ==  CARD_INDEX_ACE && 
        cards[4].get_index() ==  CARD_INDEX_TWO){
        value_ = cards[0].get_value();
        return true;
    }
    
    //对2, 对3, 对4
    if( cards[0].get_index() ==  CARD_INDEX_THREE && 
        cards[2].get_index() ==  CARD_INDEX_FOUR && 
        cards[4].get_index() ==  CARD_INDEX_TWO){
        value_ = cards[2].get_value();
        return true;
    }
    //通用三连对
    if( cards[0].get_index() + 1 ==  cards[2].get_index() && 
        cards[2].get_index() + 1==  cards[4].get_index()){
        value_ = cards[4].get_index();
        return true;
    }    
    return false;
}

int32_t TribleDoubleType::get_value(const CardsPile &cards){
    //如果有配牌, 配牌肯定排在最后, 其value最大 
    return value_;
}
/*----------------------------------------------------------------------------*/



/*-------------------------顺子,只能有五张牌----------------------------------*/
StraightType::StraightType() : BaseType(3){
        
}

//检查同花顺
bool StraightType::check_same_color(const CardsPile &cards){
    
    
    CardColor color = CARD_COLOR_NONE;
    for(int32_t c = 0;  c < cards.size() - 1; ++c ){
        //配牌无视花色
        if(cards[c].match_card()){
            continue;
        }
        //不是第一张牌并且有花色不同的
        if( CARD_COLOR_NONE != color && color != cards[c].get_color()){
            return false;
        }
        color = cards[c].get_color();        
    }
    return true;
}

bool StraightType::is_type(const CardsPile &cards){
    value_ = 0;
    if( cards.size() != 5 ){
        return false;
    }    
    
    //炸弹偏移量加上1000 
    int32_t add_value = 0;
    //A,2,3,4,5
    if( cards[0].get_index() ==  CARD_INDEX_THREE && 
        cards[1].get_index() ==  CARD_INDEX_FOUR && 
        cards[2].get_index() ==  CARD_INDEX_FIVE && 
        cards[3].get_index() ==  CARD_INDEX_ACE && 
        cards[4].get_index() ==  CARD_INDEX_TWO){
        if(check_same_color(cards)){
            set_bomb();
            add_value = SAME_COLOR_OFFSET;
        }
        value_ = cards[2].get_value() + add_value;
        
        return true;
    }
    
    //2,3,4,5,6
    if( cards[0].get_index() ==  CARD_INDEX_THREE && 
        cards[1].get_index() ==  CARD_INDEX_FOUR && 
        cards[2].get_index() ==  CARD_INDEX_FIVE && 
        cards[3].get_index() ==  CARD_INDEX_SIX && 
        cards[4].get_index() ==  CARD_INDEX_TWO){
        if(check_same_color(cards)){
            set_bomb();
            add_value = SAME_COLOR_OFFSET;
        }
        value_ = cards[3].get_value();
        return true;
    }
    //通用五张
    for( int c = 0; c < cards.size() - 1; ++c ){
        if( cards[c].get_index() + 1 != cards[c + 1].get_index() ){
            return false;
        }
    }
    value_ = cards[5].get_value();
    if(check_same_color(cards)){
        set_bomb();
        add_value = SAME_COLOR_OFFSET;
    }
    return true;
}

int32_t StraightType::get_value(const CardsPile &cards){
    //如果有配牌, 配牌肯定排在最后, 其value最大 
    return value_;
}
/*----------------------------------------------------------------------------*/