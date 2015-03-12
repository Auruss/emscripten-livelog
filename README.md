Emscripten live logging using kibana
====================================

Example look:
-------------
![Example](http://youtrack.arenaclash.net:8080/_persistent/real_log.jpg?file=108-0&v=0&c=true&rw=1920&rh=1040&u=1425733935216 "Example kibana screenshot")

Combine the power of native programs with existing web technology
-----------------------------------------------------------------
Kibana is a very powerful logging frontend, even on live environments it is very useful for errors for example. You shouldn't use direct elasticsearch connection on live systems though.

Overhead
-----------
I myself am using it even for logging fps stats every 100ms and I couldn't see any issues with that.
It depends on the integration how ever:

- C++ -> Setup the reflection for each type once and after that the overhead is very low.


TODOs
-----
- Custom loggers other than elasticsearch/kibana
- Automatic reflection for the C++ integration (probably using 3rd party tools like cling)