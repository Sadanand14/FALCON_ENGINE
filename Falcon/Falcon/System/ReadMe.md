
## Logging in Falcon

Whenever you want to include logs in your code, you need to include `System/Log.h`.

### Using Logging macros,

We have different macros based on severity and whether it is coming from engine or Game.

Following is in ascending order of severity,
`INFO, TRACE, WARN, ERROR, FATAL`

Following are the examples on how to use the loggers,

```cpp
 FL_ENGINE_INFO("INFO: This is a log without paramter");
 
 FL_GAME_INFO(" INFO: I am passing a parameter {0}",param);
 
 FL_ENGIENE_WARNING ("WARNING: I can take in as many parameters you want {0}, {1}, {2}", param0,param1,param2);
```
### Cautions while using logging systen,
1. Make sure you pass the parameters in correct order and number.
2. If you are passing in a custom class, make sure it has a toString like method which returns c style string. 

### Logging style,
Consider following things for writing logs,
1. Add "INFO:" , "WARN:" before statements to make it easier to understand. (This helps event each of them has a diffent coloring scheme).
2. Make sure you pass relevent data to failure logs and in try-catch blocks.
