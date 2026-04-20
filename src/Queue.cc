/*
 * Queue.cpp
 *
 *  Created on: Apr 3, 2026
 *      Author: salas
 */

#include <omnetpp.h>
using namespace omnetpp;

class Queue : public cSimpleModule {
  private:
    cQueue queue;          // 👈 cola real
    bool busy;
    simsignal_t qlenSignal; // 👈 señal tamaño cola
  protected:
    virtual void initialize() override {
            qlenSignal = registerSignal("qlen");
            busy = false;
        }

    virtual void handleMessage(cMessage *msg) override {

            /// 🔹 Si es señal del servidor (pide trabajo)
            // if (strcmp(msg->getName(), "pull") == 0) {
            // 🔹 Si viene del servidor (pull)
            //if (msg->arrivedOn("in", 1)) {
            // queue.insert(msg);

            // Si hay mensajes, envía uno (el servidor controla su ritmo)
            //if (!queue.isEmpty()) {
            //    cMessage *job = (cMessage *) queue.pop();
            //    send(job, "out");
            //    busy = true;
            //}

            //emit(qlenSignal, queue.getLength());
        //}

        // 1. Si el mensaje viene del servidor avisando que terminó (un "ACK" o "Pull")
             if (msg->isSelfMessage() || strcmp(msg->getName(), "puedes_enviar") == 0) {
                 busy = false;
                 delete msg; // Borramos el aviso

                 // Si tenemos algo en espera, lo enviamos ahora
                 if (!queue.isEmpty()) {
                     enviarAlServidor();
                 }
              }
              // 2. Si es un paquete que llega de los sources
              else {
                  queue.insert(msg);
                  emit(qlenSignal, queue.getLength());

                  // Solo enviamos si el servidor no está ocupado
                  if (!busy) {
                      enviarAlServidor();
                     }
                 }
             }

             void enviarAlServidor() {
                 if (!queue.isEmpty()) {
                     cMessage *job = (cMessage *) queue.pop();
                     busy = true; // Marcamos como ocupado hasta que nos avise
                     send(job, "out");
                     emit(qlenSignal, queue.getLength());
                 }
    }

};
Define_Module(Queue);
