#ifndef COMMAND_PROCESSOR_H
#define COMMAND_PROCESSOR_H

#include "MetroSystem.h"

class CommandProcessor
{
private:
    MetroSystem& system;

    bool processLine(const char* line);

public:
    CommandProcessor(MetroSystem& metroSystem);

    bool processFile(const char* filename);
};

#endif