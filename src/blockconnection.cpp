/**
 * Course ICP @ FIT VUT Brno, 2018
 * ICP 2018 Project - blockeditor
 *
 * @author David Hás, xhasda00
 * @author Ksenia Bolshakova, xbolsh00
 *
 * @brief A representation of a blockconnection.
 * @file blockconnection.cpp
 *
 *
 */

#include "blockconnection.h"

BlockConnection::BlockConnection(Line* line, bool isFirstP, Port* firstPort, Port* secondPort)
{
    this->line = line;
    this->isFirstP = isFirstP;
    this->firstPort = firstPort;
    this->secondPort = secondPort;
}

bool BlockConnection::isFirstPoint()
{
    return isFirstP;
}

Line* BlockConnection::getLine()
{
    return line;
}

Port* BlockConnection::getFirstPort()
{
    return firstPort;
}

Port* BlockConnection::getSecondPort()
{
    return secondPort;
}
