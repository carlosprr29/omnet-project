/*
 * ServerWithMaintenance.cpp
 *
 *  Created on: Apr 3, 2026
 *      Author: salas
 */

#include <omnetpp.h>
using namespace omnetpp;

class ServerWithMaintenance : public cSimpleModule {
  private:
    int serviceCounter;
    int Y;
    int m;
    bool inMaintenance;
    bool busy;

    cMessage *endService;
    cMessage *endMaintenance;
    cMessage *currentJob;

  protected:
    virtual void initialize() override {
        Y = par("Y");
        m = par("m");

        serviceCounter = 0;
        inMaintenance = false;
        busy = false;
        currentJob = nullptr;

        endService = new cMessage("endService");
        endMaintenance = new cMessage("endMaintenance");

        // Avisar a la cola al arrancar que estamos listos
        send(new cMessage("puedes_enviar"), "out_queue");
    }

    virtual void handleMessage(cMessage *msg) override {
        // 1. FIN DE SERVICIO
        if (msg == endService) {
            send(currentJob, "out");
            currentJob = nullptr;
            busy = false;

            serviceCounter++;

            if (serviceCounter >= Y) {
                inMaintenance = true;
                serviceCounter = 0;
                scheduleAt(simTime() + SimTime(m), endMaintenance);
                //return;

            } else {
                // No hay mantenimiento, pedimos el siguiente
                send(new cMessage("puedes_enviar"), "out_queue");
            }
        }
        // 🔹 FIN DE MANTENIMIENTO
        else if (msg == endMaintenance) {
            inMaintenance = false;
            // Ahora que terminó el mantenimiento, avisamos a la cola
            send(new cMessage("puedes_enviar"), "out_queue");
        }
        // 🔹 NUEVO TRABAJO
        else {
             if (inMaintenance || busy) {
                 throw cRuntimeError("Error: La cola envió un paquete pero el servidor no está disponible.");
             }

             currentJob = msg;
             busy = true;

             // ⏱️ 1 time slot de servicio
             scheduleAt(simTime() + 1, endService);
          }
    }

    virtual void finish() override {
        cancelAndDelete(endService);
        cancelAndDelete(endMaintenance);
        if (currentJob) delete currentJob;
    }

};

Define_Module(ServerWithMaintenance);
