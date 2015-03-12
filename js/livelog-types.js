var LiveLog_Enable = true;

var LiveLog_GlobalTypes = {
    "LOG_DEBUG":    {"id": 0},
    "LOG_ERROR":    {"id": 1},
    "LOG_INFO":     {"id": 2},
    "LOG_WARNING":  {"id": 3}
};

var LiveLog_Types = {
    "LOG_STD_OUTPUT": {
        "id": 0,
        "description": "Console styled outputs from standard output",
        "option_defaults": {
            "DISPLAY": true,
            "STACK": false,
            "BREAKPOINT": false
        }
    },
    "LOG_STD_ERR_OUTPUT": {
        "id": 1,
        "description": "Console styled outputs from the error output",
        "option_defaults": {
            "DISPLAY": true,
            "STACK": false,
            "BREAKPOINT": true
        }
    }
};