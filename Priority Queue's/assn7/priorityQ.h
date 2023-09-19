// ******************************************************
// Name: Kamila Kinel
// NSHE id: 5005951543
// Assignment number: assingment7
// Description: Program that reads into files to get
//              information and uses priorityQs
//              to create a create a sort of simulated
//              gameplay of Resident Evil 3: Nemesis
// ******************************************************

#ifndef __PRIORITYQ_H__
#define __PRIORITYQ_H__

#include <vector>
#include <unordered_map>

template <class t1, class t2>
class priorityQ {
    public:
        priorityQ();
        void push_back(const t1&, const t2&);
        void pop_front();
        void increaseKey(const t1&, const t2&);
        t2 get_front_priority() const;
        t1 get_front_key() const;
        bool isEmpty() const;

    private:
        // each element has a key id and value
        struct priorityType {
            t1 key;
            t2 priority;
        };
        void bubbleUp(std::size_t);
        void bubbleDown(std::size_t);
        std::vector<priorityType> heapArray;
        std::unordered_map<t1, std::size_t> itemToPQ;
};

template <class t1, class t2>
priorityQ<t1, t2>::priorityQ() {

}

/**
 * nserts a new priotityType
*  object to the back of heapArray, maps this key to the last index of the heapArray and then calls
*  bubbleUp( heapArray.size() - 1 )
 *
 * @param key name 
 * @param value our "priority"
 * @return N/A
 */
template <class t1, class t2>
void priorityQ<t1, t2>::push_back(const t1& key, const t2& value){
    priorityType p = priorityType();
    p.key = key;
    p.priority = value;
    heapArray.push_back(p);
    itemToPQ[key] = heapArray.size() - 1;
    bubbleUp(heapArray.size() - 1);
}

/**
 * 
 *
 * @param key
 * @param value
 * @return N/A
 */
template <class t1, class t2>
void priorityQ<t1, t2>::increaseKey(const t1& key, const t2& value){
    std::size_t i = itemToPQ[key];  //
    // priorityType p = priorityType();
    // p.key = key;
    // p.priority = value;
    heapArray[i].priority += value;
    bubbleUp(i);
}

/**
 * assigns the last element of heapArray and assigns it to
*   the root position, updates the itemToPQ map accordingly
 *
 * @return N/A
 */
template <class t1, class t2>
void priorityQ<t1, t2>::pop_front(){
    priorityType p = heapArray[heapArray.size() - 1];
    heapArray[0] = p;
    itemToPQ[p.key] = 0;
    heapArray.pop_back();
    bubbleDown(0);
}

/**
 *
 * @return the priority field of the root element
 */
template <class t1, class t2>
t2 priorityQ<t1, t2>::get_front_priority() const{
    if (isEmpty()) return t2();
    return heapArray[0].priority;
}

template <class t1, class t2>
t1 priorityQ<t1, t2>::get_front_key() const{
    if (isEmpty()) return nullptr;
    return heapArray[0].key;
}

template <class t1, class t2>
bool priorityQ<t1, t2>::isEmpty() const{
    return heapArray.size() == 0;
}

template <class t1, class t2>
void priorityQ<t1, t2>::bubbleUp(std::size_t index){
    std::size_t parentIndex = (index - 1) / 2;
    if (parentIndex < 0 || parentIndex >= heapArray.size()) return;
    if (heapArray[parentIndex].priority < heapArray[index].priority){
        //swap
        std::size_t i = parentIndex;
        priorityType tmp = heapArray[i];
        heapArray[i] = heapArray[index];
        heapArray[index] = tmp;
        itemToPQ[heapArray[i].key] = i;
        itemToPQ[heapArray[index].key] = index;
        bubbleUp(i);
    }
}

/**
 *function that performs the standard
*  bubble down procedure, using the parameter index, compute the left and right children indices and
*  compare their value fields and swap the parent with the correct child and continue until you cannot
*  bubble down any furthe
 * @return N/A

 */
template <class t1, class t2>
void priorityQ<t1, t2>::bubbleDown(std::size_t index){
    // creates indexes
    std::size_t leftChildIndex = (index * 2) + 1;
    std::size_t rightChildIndex = (index * 2) + 2;
    if (leftChildIndex < 0 || leftChildIndex >= heapArray.size()) leftChildIndex = -1;
    if (rightChildIndex < 0 || rightChildIndex >= heapArray.size()) rightChildIndex = -1;
    if (leftChildIndex == -1 && rightChildIndex == -1) return;
    std::size_t i = -1;

    // check that both indexes arent -1
    if (leftChildIndex != -1 && rightChildIndex != -1) {
        if (heapArray[index].priority < heapArray[leftChildIndex].priority || heapArray[index].priority < heapArray[rightChildIndex].priority){
            // If right child > left, we set right to i
            if (heapArray[leftChildIndex].priority < heapArray[rightChildIndex].priority){
                i = rightChildIndex;
            } else {
                i = leftChildIndex;
            }
        }
    } else if (leftChildIndex != -1){
        // if left> curr, left = i
        if (heapArray[index].priority < heapArray[leftChildIndex].priority){
            i = leftChildIndex;
        }
    } else {
        // if right> curr, right = i
        if (heapArray[index].priority < heapArray[rightChildIndex].priority){
            i = rightChildIndex;
        }
    }
    if (i != -1){
        // swap
        priorityType tmp = heapArray[i];
        heapArray[i] = heapArray[index];
        heapArray[index] = tmp;
        itemToPQ[heapArray[i].key] = i;
        itemToPQ[heapArray[index].key] = index;
        bubbleDown(i);
    }
}

#endif  // __PRIORITYQ_H__
