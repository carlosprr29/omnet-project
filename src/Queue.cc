/*
 * Queue.cpp
 *
 *  Created on: Apr 3, 2026
 *      Author: salas
 */

#include <omnetpp.h>
using namespace omnetpp;

class Queue : public cSimpleModule {
  private:
    cQueue queue;          // 👈 cola real

    simsignal_t qlenSignal; // 👈 señal tamaño cola
  protected:
    virtual void initialize() override {
            qlenSignal = registerSignal("qlen");
        }

    virtual void handleMessage(cMessage *msg) override {

            /// 🔹 Si es señal del servidor (pide trabajo)
            if (strcmp(msg->getName(), "pull") == 0) {

                if (!queue.isEmpty()) {
                    cMessage *job = (cMessage *) queue.pop();
                    send(job, "out");
                }

                delete msg;
            } else {
                queue.insert(msg); // guardar en cola
            }

            // 👇 emitir tamaño de cola
            emit(qlenSignal, queue.getLength());
        }

};

Define_Module(Queue);
