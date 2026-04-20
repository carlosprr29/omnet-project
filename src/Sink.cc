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
    simsignal_t totalDelaySignal; // Para ver la evolución en el tiempo

  protected:
    virtual void initialize() override {
        numReceived = 0;
        totalDelaySignal = registerSignal("totalDelay");
    }

    virtual void handleMessage(cMessage *msg) override {
        numReceived++;

        // Aquí puedes medir métricas (opcional)
        simtime_t delay = simTime() - msg->getCreationTime();

        // 2. Emitir señal (Mejor que recordScalar dentro de handleMessage)
        emit(totalDelaySignal, delay);

        // 3. LOG para depuración (Opcional, ver en consola)
        EV << "Paquete recibido. Delay: " << delay << "s. Total: " << numReceived << endl;

        delete msg;
    }

    virtual void finish() override {
        recordScalar("Paquetes totales recibidos", numReceived);
    }
};

Define_Module(Sink);
