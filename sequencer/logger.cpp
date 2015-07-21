#include "logger.h"

using namespace std;
using namespace std::chrono;

void Logger::restart() {
    ofstream of(fname, ofstream::trunc);
}

const string Logger::fname("gl.log");
