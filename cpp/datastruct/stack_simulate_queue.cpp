#include <iostream>
#include <stack>

using namespace std;

template<class T>
struct MyQueue
{
  stack<T> s1;
  stack<T> s2;

  void push(T &t)
  {
    s1.push(t);
  }

  T front()
  {
    if (s2.empty()){
      if( s1.size() == 0) throw;
      if(!s1.empty()){
        s2.push(s1.top());
        s1.pop();
      }
    }
    return s2.top();
  }

  void pop()
  {
    if (s2.empty()){
      while(!s1.empty()){
        s2.push(s1.top());
        s1.pop();
      }
    }else{
      s2.pop();
    }
  }
};

int main(int argc, char ** argv)
{
  MyQueue<int> mq;
  int i = 0;
  for (i = 0; i < 10; i++){
   mq.push(i);
  }

  for (i = 0; i < 10; i++){
    cout << mq.front() << endl;
    mq.pop();
  }
  return 0;
}

