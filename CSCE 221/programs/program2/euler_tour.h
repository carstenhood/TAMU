/* euler_tour.h:
 *  * A C++ tmplate which implements an Euler tour.
 */
#ifndef EULERTOUR_H
#define EULERTOUR_H

template <typename E>
class binary_tree;

template <typename E>
class euler_tour {
    
  protected:
    typedef binary_tree<E> Tree;
    typedef typename Tree::NodePtr NodePtr; // a position in the tree
    
  public:
    void euler_tour_traversal(NodePtr _p);
  protected:
    virtual void visit_external(NodePtr _p) {}
    virtual void visit_left(NodePtr _p) {}
    virtual void visit_below(NodePtr _p) {}
    virtual void visit_right(NodePtr _p) {}
};

//do the tour
template <typename E>
void euler_tour<E>::euler_tour_traversal(NodePtr _p) {
    if (_p == NULL)
        return;
    if (_p->is_external())
        visit_external(_p);
    else {
        visit_left(_p);
        euler_tour_traversal(_p->get_left_child());
        visit_below(_p);
        euler_tour_traversal(_p->get_right_child());
        visit_right(_p);
    }
}

//uses external and bottom visits to implement an inorder tour
template<typename E>
class recursive_inorder : public euler_tour<E>
{
    std::deque<typename euler_tour<E>::NodePtr>& d;
    
  public:
    recursive_inorder(std::deque<typename euler_tour<E>::NodePtr>& _de)
    : d(_de)
    {};
    
    virtual void visit_external(typename euler_tour<E>::NodePtr _p) {
        d.push_back(_p);
    }
    
    virtual void visit_below(typename euler_tour<E>::NodePtr _p) {
        d.push_back(_p);
    }
};

//uses external and left visits to implement a preorder tour
template<typename E>
class recursive_preorder : public euler_tour<E>
{
    std::deque<typename euler_tour<E>::NodePtr>& d;
    
  public:
    recursive_preorder(std::deque<typename euler_tour<E>::NodePtr>& _de)
    : d(_de)
    {};
    
    virtual void visit_external(typename euler_tour<E>::NodePtr _p) {
        d.push_back(_p);
    }
    
    //perform left visit action in a preorder traversal
    virtual void visit_left(typename euler_tour<E>::NodePtr _p) {
        d.push_back(_p);
    }
};

//uses external and right visits to implement a postorder tour
template<typename E>
class recursive_postorder : public euler_tour<E>
{
    std::deque<typename euler_tour<E>::NodePtr>& d;
    
public:
    recursive_postorder(std::deque<typename euler_tour<E>::NodePtr>& _de)
    : d(_de)
    {};
    
    virtual void visit_external(typename euler_tour<E>::NodePtr _p) {
        d.push_back(_p);
    }
    
    //perform right visit action in a postorder traversal
    virtual void visit_right(typename euler_tour<E>::NodePtr _p) {
        d.push_back(_p);
    }
};


#endif
