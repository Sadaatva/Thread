#include <iostream>
#include <mutex>
#include <vector>
#include <queue>
#include <thread>


class threadSafeQ {
public:    
    std::queue<int> rawQ;
    std::mutex m;
    int& retrieveAndDelete() {
        int front_value = 0;
        m.lock();
        if (!rawQ.empty()) {
            front_value = rawQ.front();
            rawQ.pop();
        }
        m.unlock();
        return front_value;
    }

    threadSafeQ(std::queue<int> rawQ) {
        this -> rawQ = rawQ;
    }

    void print() {
        m.lock();
        while (!rawQ.empty()) {
            std::cout << rawQ.front() << " ";
            rawQ.pop();
        }
        std::cout << "\n";
        m.unlock();
    }

    void push(int val) {
        m.lock();
        rawQ.push(val);
        m.unlock();
    }
};

int main() {  
    std::queue<int> example;
  
    for (int i = 1; i <= 10; i++) {
        example.push(i);
    }

    threadSafeQ s(example);

    std::thread funcTest1(&threadSafeQ::retrieveAndDelete, &s);
    std::thread funcTest2(&threadSafeQ::push, &s, 13);

    if (funcTest1.joinable()) {
        funcTest1.join();
    }

    if (funcTest2.joinable()) {
        funcTest2.join();
    }

    s.print();
    return 0;
}   
