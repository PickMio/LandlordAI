/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   poker.h
 * Author: admin
 *
 * Created on 2017年3月20日, 上午10:44
 */

#ifndef POKER_H
#define POKER_H
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
using CardsIdx = std::vector<int32_t>;
class BaseCompare;
//牌型的基类
class BaseType{
public:
    BaseType(int32_t id);
    virtual ~BaseType() {};
public:
    //默认认为传入的牌值都是排序好的
    //是否是这种类型
    virtual bool is_type(const CardsPile &cards) = 0;
    //获取这个牌型的值, 用来做比较, 最先比较weight, 再比较这个
    //除炸弹外, 这个数值就是该数值的最大一张牌的值, 炸弹应该加上偏移量
    virtual int32_t get_value(const CardsPile &cards) = 0; 
    int32_t get_type_id();
    //设置为炸弹, 可以管任何牌型
    void set_bomb();
    //是否是炸弹
    bool is_bomb();
    //获取配牌的idx, 没有配牌返回空数组
    CardsIdx get_matchcard_index(const CardsPile &cards);
protected:
    int32_t value_;//牌型的值, 在判断牌型的时候记录下来, 每次计算牌型重新初始化
private:    
    int32_t type_id_;
    
    bool    bomb_;
    BaseCompare *base_cmp_;
    
};



using TYPE_MAP = std::map<int32_t, BaseType*> ;
//比牌的基类
class BaseCompare{
public:
    BaseCompare();
    ~BaseCompare();    
public:
    //高16位为1 表示炸弹, 可以和任何牌型比较
    const int32_t CARD_TYPE_BOMB = 1 << 16;
public:    
    bool reg_compare(BaseType* type);
    //返回-1 表示不能比较大小, 牌型不对,  返回0表示右边牌大于左边牌, 可以管, 返回1 表示不能管
    int32_t compare(CardsPile& lsh, CardsPile& rsh);
    
    
private:
    TYPE_MAP cmp_map_;
    
};


//单张牌
class SingleType : public BaseType{
public:
    SingleType();
    
    virtual bool is_type(const CardsPile &cards);
    //获取这个牌型的值, 用来做比较, 最先比较weight, 再比较这个
    virtual int32_t get_value(const CardsPile &cards); 
};


//对牌
class DoubleType : public BaseType{
public:
    DoubleType();
    
    virtual bool is_type(const CardsPile &cards);
    //获取这个牌型的值, 用来做比较, 最先比较weight, 再比较这个
    virtual int32_t get_value(const CardsPile &cards); 
};





//三张, 三张牌一样的
class TribleType : public BaseType{
public:
    TribleType();
    
    virtual bool is_type(const CardsPile &cards);
    //获取这个牌型的值, 用来做比较, 最先比较weight, 再比较这个
    virtual int32_t get_value(const CardsPile &cards); 
};


//三连对
class TribleDoubleType : public BaseType{
public:
    TribleDoubleType();
    
    virtual bool is_type(const CardsPile &cards);
    //获取这个牌型的值, 用来做比较, 最先比较weight, 再比较这个
    virtual int32_t get_value(const CardsPile &cards); 
};


//顺子, 可能是同花顺
class StraightType : public BaseType{
public:
    StraightType();
    
    virtual bool is_type(const CardsPile &cards);
    //获取这个牌型的值, 用来做比较, 最先比较weight, 再比较这个
    virtual int32_t get_value(const CardsPile &cards); 
    bool check_same_color(const CardsPile &cards);
    const int32_t SAME_COLOR_OFFSET = 1000;//同花顺的偏移量
};

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

//传输用的牌值
using NetCards = std::vector<int32_t>;
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


//当前牌局最多有多少玩家
const int32_t TABLE_PLAYERS_COUNT = 4;
//最大洗牌次数, 随机交换两张牌
const int32_t MAX_FHUFFLE_COUNT = 1000;
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

#endif /* POKER_H */

