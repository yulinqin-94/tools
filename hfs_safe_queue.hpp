#pragma once

//单个读，单个写，多线程安全，无锁
template<typename T>
class cqueue {
private:
  T *data;
  int n;
  int beg, end;

public:
  cqueue(int n):n(n),beg(1),end(0) {
    data = new T[n];
  }

  ~cqueue() {
    if(data) {
      delete data;
      data = NULL;
    }
  }

  bool push(T e) {
    int k = beg;
    if(end+1 < n) {
      if(end+1 == k-1 || (end+1 == n-1 && k == 0))
	      return false;
      else {
        data[end+1] = e;
        end++;
        return true;
      }
    }
    else {
      if(1 == k)
	      return false;
      else {
        data[0] = e;
        end = 0;
        return true;
      }
    }

    return true;
  }

  bool pop(T &e) {
    int k = end;
    if(k == beg - 1 || (k == n-1 && 0 == beg))
      return false;

    e = data[beg];
    if(beg+1 == n)
      beg = 0;
    else
      beg++;
    return true;
  }
};
