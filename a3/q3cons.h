#ifndef __Q3CONS_H__
#define __Q3CONS_H__

extern MPRNG mprng;

_Task Consumer {
  BoundedBuffer<int> &buffer;
  const int delay;
  const int sentinel;
  int &sum;
  void main();

 public:
  Consumer(BoundedBuffer<int> &buffer, const int delay, const int sentinel, int &sum);
};

Consumer::Consumer(BoundedBuffer<int> &buffer, const int delay, const int sentinel, int &sum)
    : buffer(buffer), delay(delay), sentinel(sentinel), sum(sum) {}

void Consumer::main() {
  int val; // stores consuming value

  for (;;) {
    yield(mprng(delay - 1)); // yield between 0 and delay - 1 times
    val = buffer.remove(); // consume value
    if (val == sentinel) { // complete on SENTINEL read
      break;
    } else {
      sum += val;
    }
  }
}

#endif
