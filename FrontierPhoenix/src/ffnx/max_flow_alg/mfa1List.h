//file: mfa1List.h

/*  This class implements a generic template linked list used by both the DR-Planner and the Solver. */

/*Copyright (C) June 22, 2001 Meera Sitharam

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
http://www.gnu.org/copyleft/gpl.html

You should have received a copy of the GNU General Public License
in the documentation index in the documentation
folder of the FRONTIER-gnu directory; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#ifndef MFA_LIST_H
#define MFA_LIST_H

#include <stddef.h>

namespace ffnx::mfa {

    int min(int a, int b) {
        if (a < b) return a;
        else return b;
    }


    template<class Type>
    class List;  //a list of  Vertex, Edge, or Cluster

    template<class Type>
    class oneNode {
        friend class List<Type>;

    private:
        Type elem;       //Type can be Vertex, Edge, or Cluster
        oneNode<Type> *prev;
        oneNode<Type> *next;

        oneNode() {
            prev = NULL;
            next = NULL;
        }

        inline oneNode(const Type &elm) {
            elem = elm;
            prev = NULL;
            next = NULL;
        }
    };

    template<class Type>
    class List {
    protected:
        int length;
        oneNode<Type> *first, *last; //first->second-> ... ->last
    public:
        inline List() {
            length = 0;
            first = last = NULL;
        }

        List(const List<Type> &);     // copy constructor
        ~List();

        void append(const Type &);

        Type &pop();

        Type &retrieve(int) const;

        Type *addrByIndex(int);

        Type &getByIndex(int pos);

        void deleteIndex(int pos);

        void deleteElem(Type &);

        void erase(oneNode<Type> *__position);

        void makeEmpty();

        bool hasElem(Type &);

        inline int returnLen() const { return length; }

        void changeElem(int, Type &);

        bool empty() { if (length == 0) return true; else return false; }

        List<Type> &operator=(const List<Type> &);
    };

    template<class Type>
    List<Type>::List(const List<Type> &ls) {
        int i;
        oneNode<Type> *ls_pt = ls.first;
        length = ls.length;
        for (i = 0; i < length; i++) {
            oneNode<Type> *newNode = new oneNode<Type>(ls_pt->elem);
            if (i == 0) { first = last = newNode; }
            else {
                newNode->prev = last;
                last->next = newNode;
                last = newNode;
            }
        }
    }

    template<class Type>
    List<Type>::~List() {
        if (!empty()) {
            oneNode<Type> *current = first, *temp;
            while (current != last) {
                temp = current;
                current = current->next;
                delete temp;
            }
            delete last;
            length = 0;
        }
    }

    template<class Type>
    void List<Type>::append(const Type &elm) {
        oneNode<Type> *newNode = new oneNode<Type>(elm);
        if (length == 0) { first = last = newNode; }
        else {
            newNode->prev = last;
            last->next = newNode;
            last = newNode;
        }
        length++;
    }

    template<class Type>
    Type &List<Type>::pop() {
        oneNode<Type> *poped;
        if (length <= 0) {
            oneNode<Type> *dummy = new oneNode<Type>;
            return dummy->elem;
        }
        if (length == 1) {
            poped = first;
            first = last = NULL;
            length = 0;
            return poped->elem;
        }
        if (length > 1) {
            poped = first;
            first = first->next;
            first->prev = NULL;
            length--;
            return poped->elem;
        }
    }

    template<class Type>
    Type &List<Type>::retrieve(int pos) const {
        int i;
        oneNode<Type> *current;
        if (pos > length || length <= 0) {
            oneNode<Type> *dummy = new oneNode<Type>;
            return dummy->elem;
        }
        current = first;
        if (length > 1)
            for (i = 1; i < pos; i++)
                current = current->next;
        return current->elem;
    }

    template<class Type>
    bool List<Type>::hasElem(Type &elm) {
        oneNode<Type> *current;
        current = first;
        for (int i = 0; i < length; i++) {
            if (current->elem == elm) return true;
            else current = current->next;
        }
        return false;
    }

    template<class Type>
    Type *List<Type>::addrByIndex(int pos) {
        int i;
        oneNode<Type> *current;
        if (pos > length || length <= 0) return NULL;
        current = first;
        if (length > 1)
            for (i = 1; i < pos; i++)
                current = current->next;
        return &(current->elem);
    }

    template<class Type>
    void List<Type>::deleteElem(Type &elm) {
        oneNode<Type> *current;
        if (length <= 0) return;
        current = first;
        while ((!((current->elem) == elm)) && (current != last))
            current = current->next;
        if ((current->elem) == elm) {
            if (current == first) first = current->next;
            else if (current == last) last = current->prev;
            else {
                (current->prev)->next = current->next;
                (current->next)->prev = current->prev;
            }
            delete current;
            length--;
        }
    }

    template<class Type>
    Type &List<Type>::getByIndex(int pos) {
        int i;
        oneNode<Type> *current;
        if (pos > length || length <= 0) return current->elem;
        current = first;
        if (length == 1) {
            first = last = NULL;
        } else {
            for (i = 1; i < pos; i++)
                current = current->next;
            if (current == first) first = current->next;
            else if (current == last) last = current->prev;
            else {
                (current->prev)->next = current->next;
                (current->next)->prev = current->prev;
            }
        }
        length--;
        return current->elem;
    }

    template<class Type>
    void List<Type>::deleteIndex(int pos) {
        int i;
        oneNode<Type> *current;
        if (pos > length || length <= 0) return;
        current = first;
        if (length == 1) {
            first = last = NULL;
        } else {
            for (i = 1; i < pos; i++)
                current = current->next;
            if (current == first) first = current->next;
            else if (current == last) last = current->prev;
            else {
                (current->prev)->next = current->next;
                (current->next)->prev = current->prev;
            }
        }
        delete current;
        length--;
    }

    template<class Type>
    void List<Type>::erase(oneNode<Type> *_position) {
        if (_position == NULL || length <= 0) return;
        if (_position == first) first = _position->next;
        else if (_position == last) last = _position->prev;
        else {
            (_position->prev)->next = _position->next;
            (_position->next)->prev = _position->prev;
        }
        delete _position;
        length--;
    }

    template<class Type>
    void List<Type>::makeEmpty() {
        oneNode<Type> *current;
        if (length == 0) return;
        current = first;
        while (current != last) {
            current = current->next;
            delete first;
            first = current;
        }
        delete current;
        first = last = NULL;
        length = 0;
    }

    template<class Type>
    void List<Type>::changeElem(int pos, Type &newE) {
        int i;
        oneNode<Type> *current;
        current = first;
        if (length > 1)
            for (i = 1; i < pos; i++)
                current = current->next;
        current->elem = newE;
    }

    template<class Type>
    List<Type> &List<Type>::operator=(const List<Type> &ls) {
        if (this != &ls) {
            this->makeEmpty();
            int len, len1 = length, len2 = ls.length;
            oneNode<Type> *__first1 = first;
            oneNode<Type> *__first2 = ls.first;

            len = min(len1, len2);
            while (len > 0) {
                __first1->elem = __first2->elem;
                __first1 = __first1->next;
                __first2 = __first2->next;
                len--;
            }

            len = len2 - len1;

            if (len2 > len1) {
                while (len > 0) {
                    append(__first2->elem);
                    __first2 = __first2->next;
                    len--;
                }
            }
            if (len2 < len1) {
                while (len < 0) {
                    erase(__first1);
                    __first1 = __first1->next;
                    len++;
                }
            }
        }
        return *this;
    }
}

#endif
