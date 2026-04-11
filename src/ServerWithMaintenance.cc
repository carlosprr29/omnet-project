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
    int serviceCounter = 0;
    int Y;
    int m;
    bool inMaintenance = false;

    cMessage *endService;
    cMessage *endMaintenance;
    cMessage *currentJob = nullptr;

  protected:
    virtual void initialize() override {
        Y = par("Y");
        m = par("m");

        endService = new cMessage("endService");
        endMaintenance = new cMessage("endMaintenance");

        send(new cMessage("pull"), "out");
    }

    virtual void handleMessage(cMessage *msg) override {

        if (msg == endService) {
            send(currentJob, "out");
            currentJob = nullptr;

            serviceCounter++;

            if (serviceCounter == Y) {
                inMaintenance = true;
                serviceCounter = 0;
                scheduleAt(simTime() + SimTime(m), endMaintenance);

            }
            else {

                send(new cMessage("pull"), "out");
                  }

        } else if (msg == endMaintenance) {
            inMaintenance = false;
            send(new cMessage("pull"), "out");

        } else {
            if (!inMaintenance && currentJob == nullptr) {
                currentJob = msg;
                scheduleAt(simTime() + SimTime(1), endService);
            } else {
                // si está ocupado o en mantenimiento → descarta o cola (simplificado)
                delete msg;

            }
        }
    }
};

Define_Module(ServerWithMaintenance);
