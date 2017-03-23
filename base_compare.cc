/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "base_compare.h"

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
