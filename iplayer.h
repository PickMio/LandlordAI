/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   iplayer.h
 * Author: admin
 *
 * Created on 2017年3月23日, 下午5:34
 */

#ifndef IPLAYER_H
#define IPLAYER_H
#include "game_def.h"
#include "base_card.h"
#include "game_world.h"

class GameWorld;
class IPlayer{
public:
    IPlayer(GameWorld *w);
    ~IPlayer();
    void get_card(BaseCard c);
    
    //出牌
    bool del_card(const NetCards& cards);
    void sitdown();
    void ready();
    //自动出一张牌
    bool put_card();
    

    int32_t identify_;     //用户id
    int32_t direct_pos_;    //在桌子上的pos
    int32_t table_pos_;     //房间pos
    CardsPile hand_cards_; //手牌
    CardsPile out_cards_;  //已经出过的牌
    CardsPile back_hand_cards_;//手牌的备份
    PlayerStatus status_;
    static int32_t user_count_;
    GameWorld *world_;
    
};


#endif /* IPLAYER_H */

