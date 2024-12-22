//
// Created by Владислав Отвагин on 13.12.2024.
//

#ifndef TID_H
#define TID_H
#include "sem_analyzer.h"
extern std::vector<Token> tokens;
extern std::string project;
extern Bor keywords;


struct parametr {
    parametr(std::string type_, std::string id_): type(type_), id(id_) {}
    bool operator==(const parametr& other) const {
        return type == other.type;
    }
    bool operator!=(const parametr& other) const {
        return !(*this == other);
    }
    std::string type;
    std::string id;
    std::string value;
};


class Tid {
public:
    void push_id(parametr& x) {
        for (parametr& i : parametrs) {
            if (i.id == x.id && i.type == x.type) {
                throw std::string ("the id already exists");

            }
        }
        parametrs.push_back(x);
    }

    std::pair <std::string, bool>  check_id(const std::string& id) {
        bool flag = 0;
        for (const parametr& i : parametrs) {
            if (i.id == id) {
                flag = 1;
                return std::make_pair(i.type, 1);
            }
        }
        if (flag == 0) {
            //std::cout << "ERROR found";
            return std::make_pair("", 0);
        }

    }

    std::string get_val_id(const std::string& id){
        for (const parametr& i : parametrs) {
            if (i.id == id) {
                return i.value;
            }
        }
    }

    void set_val_id(const std::string& id, std::string& val) {
        for (parametr& i : parametrs) {
            if (i.id == id) {
                i.value = (val);
            }
        }
    }

private:
    std::vector<parametr> parametrs;
};

class Tree_Tid {
public:
    ~Tree_Tid() {
        for (auto* scope_ : scopes_) {
            delete scope_;
        }
    }
    void create_scope() {
        Tree_Tid* new_scope = new Tree_Tid();
        new_scope->parent_ = cur_scope_;
        cur_scope_->scopes_.push_back(new_scope);
        cur_scope_ = new_scope;
    }

    void exit_scope() {
        if (cur_scope_ && cur_scope_->parent_) {
            cur_scope_ = cur_scope_->parent_;
        }
    }

    void push_id(parametr& x) {
        cur_scope_->scope.push_id(x);
    }
    Tree_Tid* cur_scope_ = this;

    std::pair <std::string, bool> check_id(const std::string& id) {
        Tree_Tid* scope = cur_scope_;
        std::string ans;
        while (scope) {
            ans = scope->scope.check_id(const_cast<std::string&>(id)).first;
            if (!ans.empty()) {
                return std::make_pair(ans, 1);
            }
            scope = scope->parent_;
        }
        return std::make_pair("", 0);;
    }

private:
    Tid scope;
    Tree_Tid* parent_ = nullptr;
    std::vector<Tree_Tid*> scopes_;
};



#endif //TID_H