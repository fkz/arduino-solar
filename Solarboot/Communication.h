#include <WProgram.h>

/**
 Eine Klasse zur Vereinfachung der Kommunikation zwischen Solarboot und Fernbedienung
*/
class Communication
{
  public:
    Communication ();
    bool testConnection (int timeout = 2000);
};