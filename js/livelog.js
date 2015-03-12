// LiveLog ----------------------------
var LiveLog_es_client;
var LiveLog_es_index;
var LiveLog_es_inited = false;

var LiveLog_init = function(table, onInitDone) {
    if(LiveLog_Enable == false) {
        onInitDone();
        return;
    }

    LiveLog_es_client = new $.es.Client({
        hosts: 'localhost:9200'
    });
    LiveLog_es_index = "logstash-emscripten-" + Date.now();
    LiveLog_es_client.indices.create({
        "index": LiveLog_es_index
    }, function(error, response, status) {
        LiveLog_es_client.indices.putMapping({
            "index": LiveLog_es_index,
            "type": "log",
            "body": {
                "log": {
                    "_index" : { "enabled" : true },
                    "properties": {
                        "logMessage": {"type": "string", "store": true},
                        "@timestamp": {"type": "date", "store": true},
                        "logType": {"type": "string", "store": true},
                        "logName": {"type": "string", "store": true},
                        "references": {"type": "object", "store": true, "dynamic": true},
                        "stack": {"type": "boolean", "store": true},
                        "instance": {"type": "string", "store": true}
                    }
                }
            }
        }, function(error, response, status) {
            LiveLog_es_inited = true;
            onInitDone();
        });
    });
};

var LiveLog_push = function(builder) {
    if(!LiveLog_Enable) return;

    var fun = function() {
        var log_type = LiveLog_getLogType(builder.log_id);
        var global_type = LiveLog_getGlobalType(builder.global_type);

        if(!log_type.option_defaults.DISPLAY) return;
        if(log_type.option_defaults.BREAKPOINT) debugger;

        LiveLog_es_client.create({
            "index": LiveLog_es_index,
            "type": "log",
            "body": {
                "logMessage": builder.msg,
                "@timestamp": moment().format('YYYY-MM-DDTHH:mm:ss.SSSZZ'),
                "logType": global_type.key,
                "logName": log_type.key,
                "references": builder.parsed_refs,
                "stack": log_type.option_defaults.STACK,
                "instance": LiveLog_es_index
            }
        }, function (error, response, status) {
            //debugger;
        });
    };
    if(LiveLog_es_inited) {
        fun();
    } else {
        console.log("ERROR: Elasticsearch.js is not ready yet but tried to push a log");
        debugger;
    }
};

var LiveLog_getLogType = function(id) {
    for(var key in LiveLog_Types) {
        if(LiveLog_Types.hasOwnProperty(key)) {
            if (LiveLog_Types[key].id == id) {
                LiveLog_Types[key].key = key;
                return LiveLog_Types[key];
            }
        }
    }
};

var LiveLog_getGlobalType = function(id) {
    for(var key in LiveLog_GlobalTypes) {
        if(LiveLog_GlobalTypes.hasOwnProperty(key)) {
            if (LiveLog_GlobalTypes[key].id == id) {
                LiveLog_GlobalTypes[key].key = key;
                return LiveLog_GlobalTypes[key];
            }
        }
    }
};

// Builder ------------------
var live_log_instance;

function LiveLogBuilder() {
}

LiveLogBuilder.prototype.init = function(id, global_type) {
    this.log_id = id;
    this.global_type = global_type;
    this.refs = [];
    this.parsed_refs = [];
    this.options = [];
    this.msg = LiveLog_getLogType(id).description;
};

LiveLogBuilder.prototype.setMessage = function(msg) {
    this.msg = Pointer_stringify(msg);
};

LiveLogBuilder.prototype.addReferenceObject = function(name, obj_json) {
    this.refs[this.refs.length] = { 'name': Pointer_stringify(name), 'value': Pointer_stringify(obj_json) };
    this.parsed_refs = $.extend({}, this.parsed_refs, JSON.parse(this.refs[this.refs.length-1].value));
};


LiveLogBuilder.prototype.push = function() {
    LiveLog_push(this);
};
