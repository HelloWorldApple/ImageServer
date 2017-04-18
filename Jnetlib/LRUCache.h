#ifndef LRUCACHE_H
#define LRUCACHE_H
#include<unordered_map>
#include<utility>
#include<list>
#include<iterator>
#include<typeinfo>
#include<iostream>
//#include<bits/stdc++.h>
using namespace  std ;



template<class T1,class T2>
class LRUCache{
    //notice use of "typename"
    using CACHE_ITERATOR=typename unordered_map<T1, pair<T2,typename list<T1>::iterator> >::iterator;
private:
    int _capacity;
    unordered_map<T1, pair<T2,typename list<T1>::iterator> > _cache;//{key,{value,key_iterator}}
    list<T1> _used;//key

    void touch(CACHE_ITERATOR it){
        T1 key=it->first;

        _used.erase(it->second.second);
        _used.push_front(key);
        it->second.second=_used.begin();

    }
public:
    LRUCache(int capacity):_capacity(capacity){}
    int capacity(){
        return _capacity;
    }
    void setCapacity(int capacity){
        _capacity=capacity;
    }

    void put(const T1& key,const T2& value){
        auto it=_cache.find(key);
        if(it==_cache.end()){
            if(_cache.size()==_capacity){
                _cache.erase(_used.back());
                _used.pop_back();
            }
            _used.push_front(key);
        }else{
            touch(it);
        }
        _cache[key]={value,_used.begin()};
    }
    bool contain(const T1& key){
        return _cache.find(key)!=_cache.end();
    }

    T2 get(const T1 &key){
        auto it=_cache.find(key);
        if(it==_cache.end()){
            std::cerr<<"key not found"<<std::endl;
            return T2();
        }
        touch(it);
        return it->second.first;

    }



};


#endif
