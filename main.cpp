#include <string>
#include <vector>
#include <queue>
#include <iostream>

struct Tree {
  Tree* left;
  Tree* right;
  std::string value;
  float eval;
  Tree() : left(nullptr), right(nullptr), 
           value(""), eval(0.0f) {}
  Tree(std::string v) : left(nullptr), right(nullptr), 
                 value(v), eval(0.0f) {}
  Tree(char c, Tree* l, Tree* r): left(l), right(r), 
                                  value(std::string(1,c)), eval(0.0f) {}
};

float evaluate(Tree* root);
Tree* levelInsert(std::string value, std::queue<Tree*>& q, Tree* root);
Tree* levelBuildTree(std::vector<std::string> &exps); //for tree contruction
Tree* postFixBuildTree(std::vector<std::string> &stExprs);
void inorder(Tree* root); // for spot check testing
void freeTree(Tree * root); // for memory clean up

float evaluate(Tree* root) {
  if(root == nullptr) {
        return 0.0f;
    }
    
    evaluate(root->right);
    evaluate(root->left);
    switch(root->value[0]) {// char
      case '+':
        root->eval = root->left->eval + root->right->eval;
        break;
      case '-':
        if (root->value.length() > 1) {
          std::string::size_type sz;
          root->eval = std::stof (root->value,&sz);
          break;
        }
        root->eval = root->left->eval - root->right->eval;
        break;
      case '/':
        root->eval = root->left->eval / root->right->eval;
        break;
      case '*':
        root->eval = root->left->eval * root->right->eval;
        break;
      default:
        std::string::size_type sz;
        root->eval = std::stof (root->value,&sz);
    }
    return root->eval;
}

Tree* levelInsert(std::string value, std::queue<Tree*>& q, Tree* root) { 
    Tree* node = new Tree();
    node->value = value;
    Tree*  curr = q.front();
    if (root == nullptr) {
        root = node; 
    } else if (curr->left == nullptr) {
        curr->left = node; 
    } else { 
        curr->right = node; 
        q.pop(); 
    }
    q.push(node); 
    return root; 
}

Tree* postFixBuildTree(std::vector<std::string> &stExprs) {
    std::vector<Tree*> stack;
    for(int i = 0; i < stExprs.size(); i++) {
        switch(stExprs[i][0]) {
            case '+':
            case '-': {
              if (stExprs[i].length() > 1) {
                stack.push_back(new Tree(stExprs[i]));
                break;
              }
            }
            case '/':
            case '*': {
                Tree* var1 = stack.back();
                stack.pop_back();
                Tree* var2 = stack.back();
                stack.pop_back();
                stack.push_back(new Tree(stExprs[i][0],var1,var2));
                break;
            }
            default: {
                stack.push_back(new Tree(stExprs[i]));
            }
        }
    }
    return stack[0];
}

Tree* levelBuildTree(std::vector<std::string> &exps) {
    Tree* root = nullptr; 
    std::queue<Tree*> q; 
    for (int i = 0; i < exps.size(); i++) {
      root = levelInsert(exps[i], q,root);
    }
    return root; 
}

void inorder(Tree* root) {
  if(root == nullptr) {
    return;
  }
  inorder(root->left);
  std::cout << root->value << " ";
  inorder(root->right);
}

 void freeTree(Tree * root) {
     if (root == nullptr) { return; }
     freeTree(root->left);
     freeTree(root->right);
     delete root;
 }

// How to evaluate an infix expression ?
// 2 + 5 - 10 / ( 1 - (3 / 5) )
//          -              | 0
//       +        /        | 1
//     2   5   10   -      | 2
//                1    /   | 3
//                   3   5 | 4

// 1. Assume this express is in form of a tree.
//    Write data structure for this Tree and connect the 
//.   nodes to represent a parse-tree.
// 2. Evaluate this tree to get a numerical result
// 
int main() {
  std::vector<std::string> exps{"-", "+", "/", "2", "5", "10", "-"};//,"1", "/", "3", "5"};
  Tree* root = levelBuildTree(exps);
  Tree* node = new Tree();
  Tree* node2 = new Tree();
  Tree* node3 = new Tree();
  Tree* node4 = new Tree();
  node->value = "1";
  node2->value = "/";
  node3->value = "3";
  node4->value = "5";
  node2->left = node3;
  node2->right = node4;
  root->right->right->left = node;
  root->right->right->right = node2;
  inorder(root);
  std:: cout << "\n";
  std:: cout << "eval: " << evaluate(root) << std::endl;
  freeTree(root);
  
   // (1 +3) * ((7+ -1)*5) = 4 * (6*5) =4*30 = 120
  std::vector<std::string> exps2{"1", "3", "+", "5", "7", "-1", "+", "*", "*"};
  root =  postFixBuildTree(exps2);
  inorder(root);
  std:: cout << "\n";
  std:: cout << "eval: " << evaluate(root) << std::endl;
  freeTree(root);

  return 0;
}
