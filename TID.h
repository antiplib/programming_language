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
    Tid(Tid* parent = nullptr) : parent_(parent) {}
    void push_id(parametr& pr) {
        for (parametr& i : parametrs) {
            if (i.id == pr.id && i.type == pr.type) {
                throw std::string ("the id already exists");
            }
        }
        parametrs.push_back(pr);
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
    Tid* parent_;
private:
    std::vector <parametr> parametrs;

};


class Tree_Tid {
public:
    Tree_Tid() : current_scope_(new Tid()) {}

    ~Tree_Tid() {
        for (auto scope : scopes_) {
            delete scope;
        }
        delete current_scope_;
    }

    void create_scope() {
        Tid* new_scope = new Tid(current_scope_);
        scopes_.push_back(new_scope);
        current_scope_ = new_scope;
    }


    void exit_scope() {
        if (!scopes_.empty()) {
            current_scope_ = scopes_.back()->parent_;
            scopes_.pop_back();
        } else {
            throw std::string ("No more scopes to exit.");
        }
    }
    std::pair <std::string, bool> check_id(const std::string& id) {
        Tid* scope = current_scope_;
        bool flag = 0;
        while (scope) {
            std::string type;
            bool fl;
            if (scope->check_id(id).second == 0) {
                //std::cout << "ERROR2" << std::endl;
                return std::make_pair("", 0);
            } else {
                type = scope->check_id(id).first;
            }

            if (!type.empty()) {
                flag = 1;
                return std::make_pair(type, 1);
            }
            scope = scope->parent_;
        }
        if (flag == 0) {
            return std::make_pair("", 0);
        }
        //throw ("id is not found");
    }
    void push_id(parametr& pr) {
        current_scope_->push_id(pr);
    }


    Tid* current_scope_;

private:
    // Tid* current_scope_;
    std::vector<Tid*> scopes_;
};



#endif //TID_H
