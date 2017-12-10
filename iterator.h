#ifndef ITERATOR_H
#define ITERATOR_H

#include "struct.h"
#include "list.h"
using namespace std;
#include <vector>
#include <stack>
#include <queue>
#include <iostream>
#include <algorithm>

template <class T>
class Iterator{
public:
    virtual void first() = 0;
    virtual void next() = 0;
    virtual T currentItem() const = 0;
    virtual bool isDone() const = 0;
};

template <class T>
class NullIterator: public Iterator <T>{
public:
    NullIterator(Term *n){}
    void first(){};
    void next(){};
    T currentItem() const{
        return nullptr;
    }
    bool isDone() const{
        return true;
    }

};

template <class T>
class StructIterator :public Iterator<T>{
public:
    friend class Struct;
    void first(){
        _index = 0;
    }

    T currentItem() const{
        return _s->args(_index);
    }

    bool isDone() const{
        return _index >= _s->arity();
    }

    void next(){
        _index++;
    }
private:
    StructIterator(Struct *s): _index(0), _s(s) {}
    int _index;
    Struct *_s;
};

template <class T>
class ListIterator :public Iterator<T>{
public:
    ListIterator(List *list):_index(0), _list(list){}

    void first(){
        _index = 0;
    }

    T currentItem() const{
        return _list->args(_index);
    }

    bool isDone() const{
        return _index >= _list->arity();
    }

    void next(){
        _index++;
    }
private:
    int _index;
    List *_list;
};

template <class T>
class DFSIterator : public Iterator<T>{
public:
    DFSIterator(T term):_term(term),_index(0){}

    void first(){
        _index = 0;
        T term;
        stack.push(_term);

        while(!stack.empty()){
            term = stack.top();
            stack.pop();
            if(_term != term){
                _dfs.push_back(term);
            }

            Iterator <T>* it = term->createIterator();

            re.clear();
            for(it->first(); !it->isDone(); it->next()){
                re.push_back(it->currentItem());
            }
            int i = re.size()-1;
            while(i != -1){
                stack.push(re[i]);
                i--;
            }
        }
    }
    void next(){
        _index++;
    }
    T currentItem() const{
        return _dfs[_index];
    }
    bool isDone() const{
        return _index >= _dfs.size();
    }
private:
    std::vector<T> _dfs;
    std::stack<T> stack;
    std::vector<T> re;
    T _term;
    int _index;
};

template <class T>
class BFSIterator : public Iterator<T>{
public:
    BFSIterator(T term):_term(term),_index(0){}

    void first(){
        _index = 0;
        T term;
        queue.push(_term);

        while(!queue.empty()){
            term = queue.front();
            queue.pop();
            if(_term != term){
                _bfs.push_back(term);
            }

            Iterator <T>* it = term->createIterator();
            for(it->first(); !it->isDone(); it->next()){
                queue.push(it->currentItem());
            }
        }
    }
    void next(){
        _index++;
    }
    T currentItem() const{
        return _bfs[_index];
    }
    bool isDone() const{
        return _index >= _bfs.size();
    }
private:
    std::vector<T> _bfs;
    std::queue<T> queue;
    T _term;
    int _index;
};

#endif
