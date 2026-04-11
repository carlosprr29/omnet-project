 /*
 * Sink.cpp
 *
 *  Created on: Apr 3, 2026
 *      Author: salas
 */

#include <omnetpp.h>
using namespace omnetpp;

class Sink : public cSimpleModule
{
  private:
    int numReceived = 0;

  protected:
    virtual void initialize() override {
        numReceived = 0;
    }

    virtual void handleMessage(cMessage *msg) override {
        numReceived++;

        // Aquí puedes medir métricas (opcional)
        simtime_t delay = simTime() - msg->getCreationTime();
        recordScalar("delay", delay);

        delete msg;
    }

    virtual void finish() override {
        recordScalar("Paquetes recibidos", numReceived);
    }
};

Define_Module(Sink);
