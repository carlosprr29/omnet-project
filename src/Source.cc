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
    cMessage *generateEvent;

    double p; // geométrica
    double q; // poisson
    bool isGeometric;

  protected:
    virtual void initialize() override {
        generateEvent = new cMessage("generate");

        if (strcmp(getName(), "source1") == 0) {
            isGeometric = true;
            p = par("p");
        } else {
            isGeometric = false;
            q = par("q");
        }

        // empezar en t=0
        scheduleAt(simTime(), generateEvent);
    }

    virtual void handleMessage(cMessage *msg) override {

        int numArrivals = 0;

        if (isGeometric) {
            // número de llegadas en el slot
            numArrivals = geometric(p);
        } else {
            // número de llegadas en el slot
            numArrivals = poisson(q);
        }

        // generar esos usuarios
        for (int i = 0; i < numArrivals; i++) {
            cMessage *job = new cMessage("job");

            // marcar clase
            if (isGeometric)
                job->setKind(1); // clase 1
            else
                job->setKind(2); // clase 2

            send(job, "out");
        }

        // siguiente slot
        scheduleAt(simTime() + 1, generateEvent);
    }

    virtual void finish() override {
        cancelAndDelete(generateEvent);
    }
};

Define_Module(Source);
