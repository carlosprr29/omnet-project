/*
 * Source.cpp
 *
 *  Created on: Apr 3, 2026
 *      Author: salas
 */

#include <omnetpp.h>
using namespace omnetpp;

class Source : public cSimpleModule {
  private:
    cMessage *msg;
    double p; // para geométrica
    double q; // para Poisson
    bool isGeometric; // para distinguir source1 y source2

  protected:
    virtual void initialize() override {
        msg = new cMessage("generate");

        // Detectar qué source es
        if (strcmp(getName(), "source1") == 0) {
            isGeometric = true;
            p = par("p");
        } else {
            isGeometric = false;
            q = par("q");
        }

        scheduleAt(simTime(), msg);
    }

    virtual void handleMessage(cMessage *msg) override {
        // Enviar paquete
        cMessage *job = new cMessage("job");
        send(job, "out");

        simtime_t interArrival;

        if (isGeometric) {
            // Geométrica (en tiempo discreto)
            int k = geometric(p);  // número de slots
            interArrival = k;      // 1 slot por unidad
        } else {
            // Poisson → exponencial en tiempo continuo
            interArrival = exponential(1.0 / q);
        }

        scheduleAt(simTime() + interArrival, msg);
    }
};

Define_Module(Source);
