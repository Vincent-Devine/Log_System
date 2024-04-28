# Log System
Log system write in C++

## Table of Content
- [Add to your project](#add-to-your-project)
- [Functionnality](#functionnality)
- [Technologie](#techonologie)
- [Credit](#credit)

## Add to your project
```bash
$git submodule add git@github.com:Vincent-Devine/Log_System.git
```

## Functionnality
### Write log on file
```cpp
#include "Log.hpp"

// Start of your program
Log::OpenFile("logfile.txt");

// ...

// End of your program
Log::CloseFile();
```

### Logging
Call define ```LOG``` to write to the log file and console the log message with log level, date, file, line and function.
```cpp
#include "Log.hpp"

LOG("Info log", LogLevel::Info);
LOG("Validation log", LogLevel::Validation);
LOG("Warning log", LogLevel::Warning);
LOG("Critical log", LogLevel::Critical);
```

Result: <br>
![png](./Screenshot/log_result.png)

### Assertion
```cpp
#include "Log.hpp"

Assertion(true, "gg, true == true");
```

## Technologie
- **C++ 20**

## Credit
Vincent DEVINE