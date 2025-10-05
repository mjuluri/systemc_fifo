#include <systemc>
using namespace sc_core;
using namespace std;

// ---------------- PRODUCER ----------------
SC_MODULE(Producer) {
    sc_fifo_out<int> out; // output port (to FIFO)

    SC_CTOR(Producer) {
        SC_THREAD(produce);
    }

    void produce() {
        for (int i = 1; i <= 5; ++i) {
            wait(5, SC_NS); // simulate delay
            cout << "[Producer] Writing data: " << i
                 << " at " << sc_time_stamp() << endl;
            out.write(i); // send data to FIFO
        }
    }
};

// ---------------- CONSUMER ----------------
SC_MODULE(Consumer) {
    sc_fifo_in<int> in; // input port (from FIFO)

    SC_CTOR(Consumer) {
        SC_THREAD(consume);
    }

    void consume() {
        while (true) {
            int data = in.read(); // blocking read
            cout << "[Consumer] Received data: " << data
                 << " at " << sc_time_stamp() << endl;
            wait(8, SC_NS); // simulate processing delay
        }
    }
};

// ---------------- TOP LEVEL ----------------
int sc_main(int, char*[]) {
    sc_fifo<int> fifo(3); // capacity = 3 elements

    Producer prod("Producer");
    Consumer cons("Consumer");

    prod.out(fifo);
    cons.in(fifo);

    sc_start(60, SC_NS); // simulate for 60 ns
    return 0;
}

