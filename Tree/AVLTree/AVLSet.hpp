#include "../../templete.hpp"

struct AVLset{
    struct Node{
        Node* left;
        Node* right;
        ll info;
    };
    int size=0;

private:
    Node* erase_internal(Node* t, ll x) {
        if (t == nullptr) {
            assert(false && "Element not found");
        }

        if (x < t->info) {
            t->left = erase_internal(t->left, x);
        } 
        else if (x > t->info) {
            t->right = erase_internal(t->right, x);
        } 
        else {
            size--; 

            if (t->left == nullptr) {
                Node* next = t->right;
                delete t;
                return next;
            } 
            if (t->right == nullptr) {
                Node* next = t->left;
                delete t;
                return next;
            }

            Node* min_node = t->right;
            while (min_node->left != nullptr) {
                min_node = min_node->left;
            }

            t->info = min_node->info;
            
            size++; 
            t->right = erase_internal(t->right, min_node->info);
        }
        return t;
    }

    Node* root=nullptr;


    void insert(ll x){
        Node* now=root;
        while(1){
            if(now == nullptr){
                Node* rootNode =new Node{
                    nullptr,
                    nullptr,
                    x
                };
                root = rootNode;
                size++;
                break;
            }

            else if(now -> info == x){
                break;
            }

            else if(x > now -> info){
                if(now -> right == nullptr){
                    Node* initNode =new Node{
                        nullptr,
                        nullptr,
                        x
                    };
                    now -> right = initNode;
                    size++;
                    break;
                }
                else{
                    now = now -> right;
                }
            }

            else{
                if(now -> left == nullptr){
                    Node* initNode =new Node{
                        nullptr,
                        nullptr,
                        x
                    };
                    size++;
                    now -> left = initNode;
                    break;
                }
                else{
                    now = now -> left;
                }
            }
        }
    }

    void erase(ll x){
        root = erase_internal(root, x);
    }

    bool count(ll x){
        Node* now=root;
        while(1){
            if(now == nullptr) return false;

            if(now -> info == x) return true;

            if(x > now -> info){
                now = now -> right;
            }

            else{
                now = now -> left;
            }
        }
    }

    int getSize(){
        return size;
    }
};