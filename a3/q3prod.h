#ifndef __Q3PROD_H__
#define __Q3PROD_H__

extern MPRNG mprng;

_Task Producer {
  BoundedBuffer<int> &buffer;
  const int produce;
  const int delay;
  void main();

 public:
  Producer(BoundedBuffer<int> & buffer, const int produce, const int delay);
};

Producer::Producer(BoundedBuffer<int> &buffer, const int produce, const int delay)
    : buffer(buffer), produce(produce), delay(delay) {}

void Producer::main() {
  for (int i = 1; i <= produce; ++i) {
    yield(mprng(delay - 1)); // yield between 0 and delay - 1 times
    buffer.insert(i); // produce value
  }
}

#endif
