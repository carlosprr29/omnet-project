/*
 * Server.h
 *
 *  Created on: Apr 3, 2026
 *      Author: salas
 */

#ifndef SRC_SERVER_H_
#define SRC_SERVER_H_

#include <omnetpp/csimplemodule.h>

class Server: public omnetpp::cSimpleModule {
public:
    Server();
    virtual ~Server();
};
private:
    long totalServices1; // Contador acumulativo

#endif /* SRC_SERVER_H_ */
