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

    cMessage *currentJob = nullptr;
    bool busy = false;

    simsignal_t delaySignal1;
    simsignal_t delaySignal2;

    //int count1 = 0;
    //int count2 = 0;
    simsignal_t throughput1;
    simsignal_t throughput2;

  protected:
    virtual void initialize() override {
        endServiceEvent = new cMessage("endService");
        //nextJobEvent = new cMessage("nextJob");

        // 👇 registrar señal
        delaySignal1 = registerSignal("delay1");
        delaySignal2 = registerSignal("delay2");

        throughput1 = registerSignal("throughput1");
        throughput2 = registerSignal("throughput2");

        //scheduleAt(simTime(), nextJobEvent);
        // Al inicio, el servidor está libre.
        // Opcional: Avisar a la cola que ya puede enviar el primero.
        scheduleAt(simTime(), new cMessage("puedes_enviar"));
    }

    virtual void handleMessage(cMessage *msg) override {

        // 1. Si el servidor se envía a sí mismo el aviso de "puedes enviar" inicial
        if (msg->isSelfMessage() && strcmp(msg->getName(), "puedes_enviar") == 0) {
            send(msg, "out_queue"); // Enviamos el mensaje físico a la cola
            return;
        }

        // 🔹 Si termina el servicio
        if (msg == endServiceEvent) {
            // 👇 calcular delay
            simtime_t delay = simTime() - currentJob->getCreationTime();

            // distinguir clase (ejemplo simple)
            if (currentJob->getKind() == 1)
            {
                emit(delaySignal1, delay);
                //count1++;
                emit(throughput1, 1);
            }
            else
            {
                emit(delaySignal2, delay);
                //count2++;
                emit(throughput2, 1);
            }

            send(currentJob, "out");
            currentJob = nullptr;
            busy = false;

            //scheduleAt(simTime(), nextJobEvent);
            // 📢 ¡CLAVE!: Avisar a la cola que estamos libres para el siguiente
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
