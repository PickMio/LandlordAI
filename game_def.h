/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   game_def.h
 * Author: admin
 *
 * Created on 2017年3月23日, 下午5:24
 */

#ifndef GAME_DEF_H
#define GAME_DEF_H

#include <stdint.h>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <iostream>
#include <glog/logging.h>
enum CardIndex{
    CARD_INDEX_NONE    = 0,    
    CARD_INDEX_THREE   = 1,    //3
    CARD_INDEX_FOUR    = 2,    //4
    CARD_INDEX_FIVE    = 3,    //5
    CARD_INDEX_SIX     = 4,    //6
    CARD_INDEX_SEVEN   = 5,    //7
    CARD_INDEX_EIGHT   = 6,    //8
    CARD_INDEX_NINE    = 7,    //9
    CARD_INDEX_TEN     = 8,    //10
    CARD_INDEX_JACK    = 9,    //J
    CARD_INDEX_QUEEN   = 10,   //Q
    CARD_INDEX_KING    = 11,   //K
    CARD_INDEX_ACE     = 12,   //A
    CARD_INDEX_TWO     = 13,   //2
    CARD_INDEX_LJOKER  = 14,   //大王
    CARD_INDEX_JOKER   = 15,   //小王
    CARD_INDEX_END     = 16,   
};

enum CardValue{
    CARD_VALUE_LITTLE    = 0,    
    CARD_VALUE_THREE   = 1,    //3
    CARD_VALUE_FOUR    = 2,    //4
    CARD_VALUE_FIVE    = 3,    //5
    CARD_VALUE_SIX     = 4,    //6
    CARD_VALUE_SEVEN   = 5,    //7
    CARD_VALUE_EIGHT   = 6,    //8
    CARD_VALUE_NINE    = 7,    //9
    CARD_VALUE_TEN     = 8,    //10
    CARD_VALUE_JACK    = 9,    //J
    CARD_VALUE_QUEEN   = 10,   //Q
    CARD_VALUE_KING    = 11,   //K
    CARD_VALUE_ACE     = 12,   //A
    CARD_VALUE_TWO     = 13,   //2
    CARD_VALUE_OTHER   = 14,   //配牌大小
    CARD_VALUE_LJOKER  = 15,   //大王
    CARD_VALUE_JOKER   = 16,   //小王
    CARD_VALUE_END     = 17,   
};

enum CardColor{
    CARD_COLOR_NONE    = 0,
    CARD_COLOR_DIAMOND = 1,     //方块
    CARD_COLOR_CLUB    = 2,     //梅花
    CARD_COLOR_SPADE   = 3,     //黑桃
    CARD_COLOR_HEART   = 4,     //红桃    
    CARD_COLOR_END     = 5,     
};
const CardValue card_value[] = { CARD_VALUE_LITTLE, CARD_VALUE_THREE, CARD_VALUE_FOUR, CARD_VALUE_FIVE, 
                                CARD_VALUE_SIX,  CARD_VALUE_SEVEN, CARD_VALUE_EIGHT,
                                CARD_VALUE_NINE, CARD_VALUE_TEN, CARD_VALUE_JACK,
                                CARD_VALUE_QUEEN, CARD_VALUE_KING, CARD_VALUE_ACE,
                                CARD_VALUE_TWO, CARD_VALUE_LJOKER, CARD_VALUE_JOKER,                                 
                        };
const CardIndex card_index[] = { CARD_INDEX_NONE, CARD_INDEX_THREE,  CARD_INDEX_FOUR, CARD_INDEX_FIVE, 
                                CARD_INDEX_SIX,  CARD_INDEX_SEVEN, CARD_INDEX_EIGHT,
                                CARD_INDEX_NINE, CARD_INDEX_TEN, CARD_INDEX_JACK,
                                CARD_INDEX_QUEEN, CARD_INDEX_KING, CARD_INDEX_ACE,
                                CARD_INDEX_TWO, CARD_INDEX_LJOKER, CARD_INDEX_JOKER,                                 
                        };

const CardColor card_color[] = { CARD_COLOR_NONE, CARD_COLOR_DIAMOND, CARD_COLOR_CLUB,
                            CARD_COLOR_SPADE, CARD_COLOR_HEART,    
                        };

    
            
const std::string cards[] = { "空" , "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A", "2", "小王", "大王"};
const std::string colors[] = { "空" ,"方块", "梅花", "黑桃", "红桃" };
 
using CardsIdx = std::vector<int32_t>;
//传输用的牌值
using NetCards = std::vector<int32_t>;

enum TableStatus {
    TABLE_STATUS_NONE = 0,    
    //等待来人
    TABLE_STATUS_WAITING = 1, 
    //人齐了发牌
    TABLE_STATUS_DEAL_CARD = 2,
    //发完牌后开始游戏
    TABLE_STATUS_PLAYING = 3,
    //打完后结算
    TABLE_STATUS_CLOSING = 4,
};

enum PlayerStatus {
    PLAYER_STATUS_NONE = 0,
    PLAYER_STATUS_SITDOWN = 1,
    PLAYER_STATUS_READY = 2,
    PLAYER_STATUS_PLAYING = 3,
    PLAYER_STATUS_GAME_END = 4,
    
};



//当前牌局最多有多少玩家
const int32_t TABLE_PLAYERS_COUNT = 4;
//最大洗牌次数, 随机交换两张牌
const int32_t MAX_FHUFFLE_COUNT = 1000;

#endif /* GAME_DEF_H */

