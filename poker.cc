/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "poker.h"
int32_t IPlayer::user_count_ = 0;
BaseCard::BaseCard() {
    card_value_ = CARD_VALUE_LITTLE;
    card_color_   = CARD_COLOR_NONE;
    card_index_ = CARD_INDEX_NONE;
    
}

BaseCard::~BaseCard() {  
    
}

void BaseCard::set_card(CardValue v, CardIndex i, CardColor c, int32_t id){
    card_value_ = v;
    card_color_ = c;
    card_index_ = i;
    card_id_ = id;
}
//判断牌是否正确
bool BaseCard::valid() const{
    if( card_index_ < CARD_INDEX_END  && card_color_ < CARD_COLOR_END ){
        return true;
    }
    
    return false;
}
bool BaseCard::match_card() const{
    return is_matching_card_;
}

std::string BaseCard::get_name() {  
    if( !valid() ){
        //LOG ERROR
        return nullptr;
    }
    
    return colors[card_color_] + cards[card_index_];    
}

CardValue BaseCard::get_value() const{  
    if( !valid() ){
        //LOG ERROR
        return CARD_VALUE_LITTLE;
    }
    
    return card_value_;    
}

CardIndex BaseCard::get_index() const {  
    if( !valid() ){
        //LOG ERROR
        return CARD_INDEX_NONE;
    }
    
    return card_index_;    
}
int32_t BaseCard::get_id() const{
    return card_id_;
}

CardColor BaseCard::get_color() const{  
    if( !valid() ){
        //LOG ERROR
        return CARD_COLOR_NONE;
    }
    
    return card_color_;    
}


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
BaseCompare::BaseCompare(){
    
}
BaseCompare::~BaseCompare(){
    
}
bool BaseCompare::reg_compare(BaseType* type){
    auto cmp = cmp_map_.find( type->get_type_id() );    
    if(cmp != cmp_map_.end()) {
        //LOG ERROR 提示有重复的类型!!!
        cmp_map_.erase(cmp);
    }
    cmp_map_.insert(std::make_pair(type->get_type_id(), type));
    
}

int32_t BaseCompare::compare(CardsPile& lsh, CardsPile& rsh){
    if( lsh.size() == 0 ){
        //第一轮出牌
        return 0;
    }
    std::sort(lsh.begin(), lsh.end(), std::less<BaseCard>());
    std::sort(rsh.begin(), rsh.end(), std::less<BaseCard>());
    for(auto v : cmp_map_){        
        if( !v.second->is_type(lsh) ){
            //类型不符合
            continue;
        }
        int32_t lvalue = v.second->get_value(lsh);
        if( !v.second->is_type(rsh) && !v.second->is_bomb()) {
            return -1; //不能比较大小, 牌型不对, 并且右边出的牌不是炸弹
        }
        
        
        int32_t rvalue = v.second->get_value(rsh);
        if(rvalue <= lvalue){
            //右边牌型比较小, 管不了
            return 1;            
        }        
        //可以管
        return 0;
        
    }
}

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