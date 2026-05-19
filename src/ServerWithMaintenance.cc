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
    int totalServices;
    int Y;
    int m;
    bool inMaintenance;
    bool busy;

    cMessage *endService;
    cMessage *endMaintenance;
    cMessage *currentJob;

    // --- SEÑALES PARA ESTADÍSTICAS ---
    simsignal_t delaySignal2;
    simsignal_t throughputSignal2;

  protected:
    virtual void initialize() override {
        Y = par("Y");
        m = par("m");

        serviceCounter = 0;
        totalServices = 0;
        inMaintenance = false;
        busy = false;
        currentJob = nullptr;

        endService = new cMessage("endService");
        endMaintenance = new cMessage("endMaintenance");

        // Registrar señales (deben coincidir con el nombre en el .ned)
        delaySignal2 = registerSignal("delay2");
        throughputSignal2 = registerSignal("throughput2");

        // Avisar a la cola al arrancar que estamos listos
        send(new cMessage("puedes_enviar"), "out_queue");
        //cMessage *avisoInicial = new cMessage("puedes_enviar");
        //scheduleAt(simTime(), avisoInicial);
    }

    virtual void handleMessage(cMessage *msg) override {
        // 1. FIN DE SERVICIO
        if (msg == endService) {
            // 1. CALCULAR ESTADÍSTICAS ANTES DE ENVIAR
            simtime_t delay = simTime() - currentJob->getCreationTime();
            emit(delaySignal2, delay);         // Para Residence Time
            totalServices++;
            emit(throughputSignal2, totalServices); // <--- Emitimos el total acumulado

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
        // --- CÁLCULO DEL THROUGHPUT REAL ---
        // Tiempo total de simulación menos el periodo de warmup
        simtime_t totalSimulationTime = simTime() - getSimulation()->getWarmupPeriod();

        if (totalSimulationTime > 0) {
              // Paquetes totales / Tsbiempo total útil de simulación
              double throughputReal = (double)totalServices / totalSimulationTime.dbl();

              // Esto guarda directamente el valor final en tu archivo .sca sin promediar unos
              recordScalar("throughput2", throughputReal);
        }

        cancelAndDelete(endService);
        cancelAndDelete(endMaintenance);
        if (currentJob) delete currentJob;
    }

};

Define_Module(ServerWithMaintenance);
