/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   game_world.h
 * Author: admin
 *
 * Created on 2017年3月23日, 下午5:36
 */

#ifndef GAME_WORLD_H
#define GAME_WORLD_H
#include "game_def.h"
#include "base_compare.h"
#include "iplayer.h"
#include "base_card.h"
#include "table_item.h"

class TableItem;
class IPlayer;


class GameWorld{
public:
    //洗牌
    //发牌
    //定时器
    GameWorld();
    //准备开桌
    void ready_game(TableItem *table);    
    //玩家坐下
    void on_player_sitdonw(IPlayer *pl); 
    //玩家准备
    void on_player_ready(IPlayer *pl);
    //玩家离开
    void on_player_leave(IPlayer *pl);
    bool on_player_discard(IPlayer *pl, const NetCards& cards);
    
    //洗牌
    void shuffle();
    
private:
    //初始化牌堆
    void init_cards();
private:    
    //是否开启配牌
    bool open_matching_;   
    //每次都要使用的初始化好的牌堆, 新牌堆复制它
    CardsPile const_cards_pile_;
    //是否是两副牌 
    bool double_cards_;
    int32_t shuff_times_;
    TableItem *table_;
    
    
};

#endif /* GAME_WORLD_H */

