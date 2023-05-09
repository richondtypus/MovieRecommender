#ifndef TREEMULTIMAP_INCLUDED
#define TREEMULTIMAP_INCLUDED
#include <vector>
#include <iostream> //remember to delete later
using namespace std;

template <typename KeyType, typename ValueType>
class TreeMultimap
{
  public:
    class Iterator
    {
      public:
        Iterator() : m_vec(nullptr), index(0) {}
        Iterator(vector<ValueType>* v) :m_vec(v), index(0) {}
        ValueType& get_value() const
        {
            return m_vec->at(index);
        }

        bool is_valid() const
        {
            if (m_vec == nullptr || index>=m_vec->size()) {
                return false;
            }
            return true;
        }

        void advance()
        {
            index++;
        }

      private:
          vector<ValueType>* m_vec;
          ValueType m_val;
          int index;
    };

    TreeMultimap()
    {
        root = nullptr;
    }

    ~TreeMultimap()
    {
        deleteTree(root);
    }


    void insert(const KeyType& key, const ValueType& value)
    {
        insertHelper(root, key, value);
    }

    

    Iterator find(const KeyType& key) const
    {
        return findHelper(root, key);
    }

  private:
      struct Node {
          Node* left;
          Node* right;
          KeyType key;
          vector <ValueType>* values;
          Node() :left(nullptr), right(nullptr) {}
          Node(KeyType k, ValueType v) :left(nullptr), right(nullptr), key(k), values(new vector<ValueType>) {
              values->push_back(v);
          }
          ~Node() {
              delete values;
              values = nullptr;
          }

      };

      struct Node* root;

      void insertHelper(Node*& current, const KeyType& k, const ValueType& v) {
          //if no nodes exist, create root
          if (current == nullptr) {
              current = new Node(k, v);
              return;
          }
          if (current->key == k) {
              current->values->push_back(v);
          }
          else {
              if (k > current->key) {
                  insertHelper(current->right, k, v);
              }
              else {
                  insertHelper(current->left, k, v);
              }
          }
      }

      void deleteTree(struct Node* node) {
          if (node != nullptr) {
              deleteTree(node->left);
              deleteTree(node->right);
              delete node;
          }
      }

      Iterator findHelper(Node* node, const KeyType& k) const {
          if (node == nullptr) {
              return Iterator();
          }
          else {
              if (node->key == k) {
                  return Iterator(node->values);
              }
              if (k > node->key) {
                  return findHelper(node->right, k);
              }
              else {
                  return findHelper(node->left, k);
              }
          }
      }
};

#endif // TREEMULTIMAP_INCLUDED
