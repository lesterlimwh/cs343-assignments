#ifndef Q3BUFFER_H
#define Q3BUFFER_H

#include <iostream>
#include <queue>

using namespace std;

#ifdef BUSY  // busy waiting implementation

template <typename T>
class BoundedBuffer {
  uOwnerLock bufferLock;
  uCondLock prodLock;
  uCondLock consLock;

  queue<T> container; // stores buffer values
  const unsigned int size; // buffer size
 public:
  BoundedBuffer(const unsigned int size = 10);
  void insert(T elem);
  T remove();
};

template <typename T>
BoundedBuffer<T>::BoundedBuffer(const unsigned int size) : size(size) {}

template <typename T>
void BoundedBuffer<T>::insert(T elem) {
  bufferLock.acquire();

  // wait for consumer to consume while buffer full
  while (container.size() >= size) {
    prodLock.wait(bufferLock);
  }

  container.push(elem);
  consLock.signal(); // signal to cons that buffer has values

  bufferLock.release();
}

template <typename T>
T BoundedBuffer<T>::remove() {
  bufferLock.acquire();

  // wait for producer to produce while buffer is empty
  while (container.size() <= 0) {
    consLock.wait(bufferLock);
  }

  T val = container.front();
  container.pop();

  prodLock.signal(); // signal to prod that buffer has space

  bufferLock.release();

  return val;
}
#endif  // BUSY

#ifdef NOBUSY  // no busy waiting implementation

template <typename T>
class BoundedBuffer {
  // locks
  uOwnerLock bufferLock;
  uCondLock bargeLock;
  uCondLock prodLock;
  uCondLock consLock;

  // signals
  bool prodSignal = false;
  bool consSignal = false;
  bool bufferSignal = false;

  queue<T> container; // stores buffer values
  const unsigned int size; // buffer size
 public:
  BoundedBuffer(const unsigned int size = 10);
  void insert(T elem);
  T remove();
};

template <typename T>
BoundedBuffer<T>::BoundedBuffer(const unsigned int size) : size(size) {}

template <typename T>
void BoundedBuffer<T>::insert(T elem) {
  bufferLock.acquire();
  bool barged = false;

  // wait if barging
  if (prodSignal || consSignal || bufferSignal) {
    bargeLock.wait(bufferLock);
    bufferSignal = false;
    barged = true;
  }

  // wait for consumer to consume if buffer is full
  if (container.size() >= size) {
    // signal barge lock so others don't need to wait if barge is not empty
    if (barged && !bargeLock.empty()) {
      bufferSignal = true;
      bargeLock.signal();
    }
    prodLock.wait(bufferLock);
    prodSignal = false;
  }

  container.push(elem);

  // signal consumers or barge
  if (!consLock.empty()) {
    consSignal = true;
    consLock.signal();
  } else if (!bargeLock.empty()) {
    bufferSignal = true;
    bargeLock.signal();
  }

  bufferLock.release();
}

template <typename T>
T BoundedBuffer<T>::remove() {
  bufferLock.acquire();

  bool barged = false;

  // wait so we don't barge
  if (consSignal || prodSignal || bufferSignal) {
    bargeLock.wait(bufferLock);
    bufferSignal = false;
    barged = true;
  }

  // wait for producer to produce
  if (container.size() <= 0) {
    // signal barge lock so others don't need to wait if barge is not empty
    if (barged && !bargeLock.empty()) {
      bufferSignal = true;
      bargeLock.signal();
    }

    consLock.wait(bufferLock);
    consSignal = false;
  }

  T val = container.front();
  container.pop();

  // signal consumers or barge
  if (!prodLock.empty()) {
    prodSignal = true;
    prodLock.signal();
  } else if (!bargeLock.empty()) {
    bufferSignal = true;
    bargeLock.signal();
  }

  bufferLock.release();

  return val;
}
#endif  // NOBUSY

#endif  // include guard
