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
    ///cQueue *queueRef;   // referencia indirecta no usada directamente en OMNeT++


    cMessage *endServiceEvent;
    //cMessage *nextJobEvent;
    int totalServices1;
    cMessage *currentJob;
    bool busy;

    simsignal_t delaySignal1;
    //simsignal_t delaySignal2;

    //int count1 = 0;
    //int count2 = 0;
    simsignal_t throughputSignal1;
    //simsignal_t throughput2;

  protected:
    virtual void initialize() override {
        endServiceEvent = new cMessage("endService");
        //nextJobEvent = new cMessage("nextJob");
        currentJob = nullptr;
        busy = false;
        totalServices1 = 0;
        // 👇 registrar señal
        delaySignal1 = registerSignal("delay1");
        //delaySignal2 = registerSignal("delay2");

        throughputSignal1 = registerSignal("throughput1");
        //throughput2 = registerSignal("throughput2");

        //scheduleAt(simTime(), nextJobEvent);
        // Al inicio, el servidor está libre.
        // Opcional: Avisar a la cola que ya puede enviar el primero.
        //scheduleAt(simTime(), new cMessage("puedes_enviar"));
        send(new cMessage("puedes_enviar"), "out_queue");
    }

    virtual void handleMessage(cMessage *msg) override {

        // 1. Si el servidor se envía a sí mismo el aviso de "puedes enviar" inicial
        if (msg == endServiceEvent) {
            // Calcular y emitir Residence Time (Tiempo de residencia) [cite: 32]
            simtime_t delay = simTime() - currentJob->getCreationTime();
            emit(delaySignal1, delay);

            // Emitir señal de Throughput [cite: 33]
            totalServices1++;
            emit(throughputSignal1, totalServices1);

            send(currentJob, "out");
            currentJob = nullptr;
            busy = false;

            // 📢 CLAVE: Pedir el siguiente paquete a la cola tras quedar libre
            send(new cMessage("puedes_enviar"), "out_queue");
        }

        // 🔹 Si llega un nuevo paquete
        else {
            if (busy) {
                // 🚨 Esto NO debería ocurrir si la cola está bien
                throw cRuntimeError("Server received job while busy");
            }

            currentJob = msg;
            busy = true;

            // ⏱️ servicio = 1 time slot
            scheduleAt(simTime() + 1, endServiceEvent);
        }
    }

    virtual void finish() override {
        cancelAndDelete(endServiceEvent);
        //cancelAndDelete(nextJobEvent);
        if (currentJob) delete currentJob;
    }
};

Define_Module(Server);
