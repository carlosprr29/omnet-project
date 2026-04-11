/*
 * Server.cpp
 *
 *  Created on: Apr 3, 2026
 *      Author: salas
 */

#include <omnetpp.h>
using namespace omnetpp;

class Server  : public cSimpleModule {
  private:
    cMessage *endService;
    cMessage *currentJob = nullptr;
    bool busy = false;

    simsignal_t delaySignal1;
    simsignal_t delaySignal2;

    int count1 = 0;
    int count2 = 0;
    simsignal_t throughput1;
    simsignal_t throughput2;

  protected:
    virtual void initialize() override {
        endService = new cMessage("endService");
        // 👇 registrar señal
        delaySignal1 = registerSignal("delay1");
        delaySignal2 = registerSignal("delay2");

        throughput1 = registerSignal("throughput1");
        throughput2 = registerSignal("throughput2");

        send(new cMessage("pull"), "in");
    }

    virtual void handleMessage(cMessage *msg) override {

        // 🔹 Si termina el servicio
        if (msg == endService) {
            // 👇 calcular delay
            simtime_t delay = simTime() - currentJob->getCreationTime();

            // distinguir clase (ejemplo simple)
            if (strcmp(currentJob->getName(), "class1") == 0)
            {
                emit(delaySignal1, delay);
                count1++;
                emit(throughput1, count1);
            }
            else
            {
                emit(delaySignal2, delay);
                count2++;
                emit(throughput2, count2);
            }

            send(currentJob, "out");
            currentJob = nullptr;
            busy = false;

            send(new cMessage("pull"), "in");
        }

        // 🔹 Si llega un nuevo paquete
        else {
            if (!busy) {
                currentJob = msg;
                busy = true;

                scheduleAt(simTime() + 1, endService);
            } else {
                // Si está ocupado, descartamos o podrías encolar
                delete msg;  // opción simple
            }
        }
    }
};

Define_Module(Server);
