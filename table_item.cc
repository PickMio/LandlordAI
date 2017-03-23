/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "table_item.h"

TableItem::TableItem(CardsPile cards){      
    cards_pile_.swap(cards);    
    back_cards_pile_.assign(cards_pile_.begin(), cards_pile_.end());
    SingleType *st = new SingleType();
    DoubleType *dt = new DoubleType();
    TribleType *tt = new TribleType();
    TribleDoubleType *tdt = new TribleDoubleType();
    StraightType *sat = new StraightType();
    compare_.reg_compare( st );
    compare_.reg_compare( dt );
    compare_.reg_compare( tt );
    compare_.reg_compare( tdt );
    compare_.reg_compare( sat );
    reset_table();
}
TableItem::~TableItem(){
    
}
//整理手牌
void TableItem::sort_cards(CardsPile& cards){
    std::sort(cards.begin(), cards.end(), std::less<BaseCard>());    
}

//初始化桌子
void TableItem::reset_table(){ 
    max_players_ = TABLE_PLAYERS_COUNT;    
    table_status_ = TABLE_STATUS_NONE;   
    ready_count_ = 0;
    cur_pos_ = rand()%max_players_;
    last_pos_ = cur_pos_;
    //TODO 赢家位置应该可以修改
    win_pos_ = -1;
}

//发牌
void TableItem::deal_cards(){
    
    if( table_status_ != TABLE_STATUS_DEAL_CARD){
        LOG(ERROR) << "桌子状态不对, status[" << table_status_ << "]";
        return;
    }
    for(auto c = 0; c < cards_pile_.size(); ++c){
        auto pl = players_[c%players_.size()];
        pl->get_card(cards_pile_[c]);
    }
    table_status_ = TABLE_STATUS_PLAYING;
    LOG(INFO) << "游戏开始!";
    for(auto pl : players_){
        sort_cards(pl->hand_cards_);
        //备份手牌
        pl->back_hand_cards_.assign(pl->hand_cards_.begin(), pl->hand_cards_.end());
        //已经打出的手牌
        pl->out_cards_.clear();
        pl->status_ = PLAYER_STATUS_PLAYING;
        
        LOG(INFO) << "player[" << pl->identify_ << "] 手牌数[" << pl->hand_cards_.size() << "]的手牌为:";
        show_cards(pl->hand_cards_);
    }
}
//打印牌堆
void TableItem::show_cards(const CardsPile& cards){
    std::string str;    
    for(auto v: cards){
        str += v.get_name() + ",";
    }
    LOG(INFO) << str;
    
}

bool TableItem::player_sitdown(IPlayer* pl){
    if( nullptr == pl ){
        LOG(ERROR) << "玩家空指针";
    }
    if(PLAYER_STATUS_NONE != pl->status_){
        LOG(ERROR) << "玩家坐下失败, 玩家状态不对, status[" << pl->status_ << "]";
    }
    if( table_status_ != TABLE_STATUS_NONE && table_status_ != TABLE_STATUS_WAITING){
        LOG(INFO) << "玩家 pl.id[" << pl->identify_ << "] 开始围观";
        visitors_.push_back(pl);
        return true;        
    }
    for(auto v:players_){
        if(pl == v){
            LOG(ERROR) << "玩家准备失败, 玩家已经在游戏中了" << "玩家 pl.id[" << pl->identify_ ;
            return false;
        }
    }
    pl->direct_pos_ = players_.size();
    players_.push_back(pl);
    LOG(INFO) << "玩家 pl.id[" << pl->identify_ << "] 坐下成功";
    return true;
    
    
}
//玩家准备
bool TableItem::player_ready(IPlayer *pl){
    if( nullptr == pl ){
        LOG(ERROR) << "玩家空指针";
        return false;
    }
    if(PLAYER_STATUS_NONE != pl->status_){
        LOG(ERROR) << "玩家准备失败, 玩家状态不对, status[" << pl->status_ << "]";
        return false;
    }
    if(table_status_ != TABLE_STATUS_NONE && table_status_ != TABLE_STATUS_WAITING){
        //LOG error 桌子状态不对, 不能加入 
        LOG(ERROR) << "玩家准备失败, 桌子状态不对, status[" <<  table_status_ << "]";
        return false;        
    }
    table_status_ = TABLE_STATUS_WAITING;
    if(players_.size() <= pl->direct_pos_){
        //LOG error 玩家pos不对
        LOG(ERROR) << "玩家准备失败, 玩家pos 不对 pos[" << pl->direct_pos_;
        return false;
    }
    if(players_[pl->direct_pos_]->identify_ != pl->identify_){
        LOG(ERROR) << "玩家[" << pl->identify_ << "] 位置不对, pos[" << pl->direct_pos_ << "]"\
                "此处玩家id为[" << players_[pl->direct_pos_]->identify_ << "]";
    }
    
    pl->status_ = PLAYER_STATUS_READY;
    LOG(INFO) <<  "玩家[" << pl->identify_ << "] 准备ok";
    ++ready_count_;
    if(players_.size() >= max_players_ && ready_count_ >= max_players_ ){
        //人数够了开始游戏
        LOG(INFO) << "开始发牌!";
        table_status_ = TABLE_STATUS_DEAL_CARD;
        deal_cards();
    }
    return true;
    
}

//玩家出牌
bool TableItem::player_discard(IPlayer *pl, const NetCards& cards){
    if( table_status_ != TABLE_STATUS_PLAYING){
        LOG(ERROR) << "桌子游戏状态不对! status[" << table_status_ << "]";
        return false;
    }
    if ( pl->status_ != PLAYER_STATUS_PLAYING ){
         LOG(ERROR) << "玩家游戏状态不对! status[" << pl->status_ << "]";
        return false;
    }
    if(cur_pos_ != pl->direct_pos_){
        LOG(ERROR) << "玩家 id[" << pl->identify_ <<"] pos[" << pl->direct_pos_ <<"]出牌顺序不对, 当前出牌位置为[" << cur_pos_ <<"]";
        return false;
    }
    CardsPile pile;
   
    for(auto v : cards){
        if(v >= back_cards_pile_.size()){
            LOG(ERROR) << "玩家[" << pl->identify_ << "] 打出的牌ID[" << v << "] 不对";
            return false;
        }
        pile.push_back(back_cards_pile_[v]);
    }
    //排序出的牌    
    sort_cards(pile);
    if(pile.size() > 0 && last_pos_ != cur_pos_){
        //需要出的牌大于上家出的牌
        if( 0 != compare_.compare(last_cards_, pile)){
            LOG(ERROR) << "玩家[" << pl->identify_ << "] 打出的牌管不了上家!";
            show_cards(pile);
            LOG(ERROR) << "上家的牌:";
            show_cards(last_cards_);
            return false;
        }     
        last_pos_ = cur_pos_;        
    }    
   
    if(!pl->del_card(cards)){
        LOG(ERROR) <<"玩家[" << pl->identify_ << "] 手上没有此牌!";
        LOG(ERROR) << "玩家手牌:";
        show_cards(pl->hand_cards_);
        LOG(ERROR) << "玩家打出的牌:";
        show_cards(pile);
        return false;        
    }
    stack_cards_.push_back(pile);
    if(pile.size() > 0){
        last_cards_ = pile;
        LOG(INFO) << "玩家[" << pl->identify_ << "] 打出的牌为:";
        show_cards(pile);    
        if(pl->hand_cards_.empty()){
            table_status_ = TABLE_STATUS_CLOSING;
            for(auto p : players_){
                p->status_ = PLAYER_STATUS_GAME_END;
            }
            //游戏结束
            win_pos_ = pl->direct_pos_;
            game_end();        
        }
    }else{
        LOG(INFO) << "玩家[" << pl->identify_ << "] 过牌";
    }
    
    cur_pos_ = (cur_pos_ + 1) % max_players_;
    return true;
}
void TableItem::game_end(){
    if(table_status_ != TABLE_STATUS_CLOSING){
         LOG(ERROR) << "桌子游戏状态不对! status[" << table_status_ << "]";
         return;
    }
     for(auto p : players_){
        if(p->status_ != PLAYER_STATUS_GAME_END ){
            LOG(ERROR) << "玩家" << p->identify_  << "游戏状态不对! status[" << table_status_ << "]";
            return;            
        }
    }
    if (win_pos_ < 0 || win_pos_ >= players_.size()){
        LOG(ERROR) << "当前没有赢家, 错误! win_pos_[" << win_pos_ << "]";
         return;
    }
    LOG(INFO) << "赢了的玩家[" << players_[win_pos_]->identify_ << "]";
    
}
