/**
 * Course ICP @ FIT VUT Brno, 2018
 * ICP 2018 Project - blockeditor
 *
 * @author David Hás, xhasda00
 * @author Ksenia Bolshakova, xbolsh00
 *
 * @brief An abstract class of a blockconnection.
 * @file blockconnection.h
 *
 *
 */

#ifndef BLOCKCONNECTION_H
#define BLOCKCONNECTION_H

#include <QGraphicsLineItem>
#include "line.h"

class Port;
/**
 * @brief The BlockConnection class contains the information about block's connections.
 */
class BlockConnection
{
public:
    /**
     * @brief BlockConnection is a constructor.
     * @param line is a line between two blocks.
     * @param isFirstP is a bool value, what contains the information if it is a first port or not.
     * @param firstPort is a first port.
     * @param secondPort is a second port.
     */
    BlockConnection(Line* line, bool isFirstP, Port* firstPort, Port* secondPort);
    /**
     * @brief isFirstPoint returns the information if it is a first port or not.
     * @return bool value. True is if it's a first port. False if it is second port.
     */
    bool isFirstPoint();
    /**
     * @brief getLine returns the connection between blocks.
     * @return the line.
     */
    Line* getLine();
    /**
     * @brief getFirstPort returns the first connected port.
     * @return first connected port.
     */
    Port* getFirstPort();
    /**
     * @brief getSecondPort returns the second connected port.
     * @return second connected port.
     */
    Port* getSecondPort();
private:
    Line* line;
    bool isFirstP;
    Port* firstPort;
    Port* secondPort;
};

#endif // BLOCKCONNECTION_H
