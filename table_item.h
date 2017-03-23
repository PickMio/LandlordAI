/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   table_item.h
 * Author: admin
 *
 * Created on 2017年3月23日, 下午5:47
 */

#ifndef TABLE_ITEM_H
#define TABLE_ITEM_H
#include "game_def.h"
#include "iplayer.h"
#include "base_card.h"
#include "poker_types.h"
class IPlayer;
class TableItem{
public:
    //初始化牌堆
    TableItem(CardsPile cards);
    ~TableItem();    
    
    //收拾桌子 
    virtual void reset_table();
    //发牌
    virtual void deal_cards();    
    //打印牌堆
    virtual void show_cards(const CardsPile& cards);  
    //玩家坐下
    virtual bool player_sitdown(IPlayer *pl);
    //玩家准备
    virtual bool player_ready(IPlayer *pl);
    
    //玩家出牌
    virtual bool player_discard(IPlayer *pl, const NetCards& cards);
    //整理手牌
    virtual void sort_cards(CardsPile& cards);
    
    //游戏结束
    virtual void game_end();
    std::vector<IPlayer* > players_;
    //围观玩家
    std::vector<IPlayer* > visitors_;
public:
    //发牌的牌堆,每次洗完牌后下次洗牌以上次洗牌的位置再洗
    CardsPile cards_pile_; 
    //玩家出牌的牌堆
    std::vector<CardsPile> stack_cards_;
    
    int32_t max_players_;
    //当前出牌玩家位置
    int32_t cur_pos_;
    //上家出牌管牌的位置
    int32_t last_pos_;
    CardsPile last_cards_;
    //赢家的位置
    int32_t win_pos_; 
    //已经准备好的数量
    int32_t ready_count_;
    
    CardsPile back_cards_pile_;
    TableStatus table_status_;
    BaseCompare compare_;
    
private:
};


#endif /* TABLE_ITEM_H */

