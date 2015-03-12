#include <sstream>
#include <cstring>

#include <emscripten/emscripten.h>

#include <Emscripten/LiveLog/Builder.h>

#include <cstdio>

using namespace Emscripten::LiveLog;

Builder::Builder(int log_id, int global_id) {
    _log_id = log_id;
    _message = NULL;

    EM_ASM_INT({
        live_log_instance = new LiveLogBuilder();
        live_log_instance.init($0, $1);
    }, log_id, global_id);


}

Builder::~Builder() {
    if(_message != NULL) {
        delete [] _message;
    }
}

void Builder::setMessage(const char *format, ...) {
    va_list argList;

    char* buffer = new char[256];
    va_start(argList, format);
    vsprintf(buffer, format, argList);
    va_end(argList);

    _message = buffer;

    EM_ASM_ARGS({
        live_log_instance.setMessage($0);
    }, _message);
}

void Builder::addRefObj(const char* name, ReflObject *refl, void* data_ptr) {
    std::stringstream str;
    str << "{";
    jsonify(refl, str, name, (char*)data_ptr);
    str << "}";

    EM_ASM_ARGS({
        live_log_instance.addReferenceObject($0,$1);
    }, name, str.str().c_str());
}

void Builder::push() {
    EM_ASM({
        live_log_instance.push();
    });
}


void Builder::jsonify(ReflObject *refl, std::stringstream& str, const char* name, char* data) {
    str << "\"" << name << "\": {";
    for(auto iter = refl->Meta.begin(); iter != refl->Meta.end(); iter++) {
        str << "\"" << iter->name << "\": ";
        if(iter->mem == LIVELOG_REFL_SCALAR || iter->mem == LIVELOG_REFL_ARRAY) {
           jsonify_value(iter->type_name, iter->second_type_name, data + iter->offset, str);
        }
        /*else if (iter->mem == LIVELOG_REFL_ARRAY) {
            str << "\"{array}\"";
        }*/

        if((iter + 1) == refl->Meta.end()) {
            if(refl->Groups.empty()) break;
        }
        str << ",";
    }

    for(auto giter = refl->Groups.begin(); giter != refl->Groups.end(); giter++) {
        jsonify(giter->obj, str, giter->name, data + giter->offset);

        if((giter + 1) != refl->Groups.end()) {
            str << ",";
        }
    }

    str << "}";
}

void Builder::jsonify_value(const char* type, const char* second_type, char* data, std::stringstream& str) {
    if(std::strcmp(typeid(int).name(), type) == 0) {
        int* pval = (int*)(data);
        str << *pval;
    } else if(std::strcmp(typeid(float).name(), type) == 0) {
        float* pval = (float*)(data);
        str << *pval;
    } else if (std::strcmp(typeid(std::string).name(), type) == 0) {
        std::string* pval = (std::string*)data;
        str << "\"" << *pval << "\"";
    } else {
        if(second_type != NULL) {
            if(std::strcmp(typeid(std::string).name(), second_type) == 0) {
                std::vector<std::string>* vec = (std::vector<std::string>*) data;
                int count = vec->size();
                str << "{ \"count\": " << count << ", \"array\": [";
                int i = 0;
                for(auto iter = vec->begin(); iter != vec->end(); iter++) {
                    jsonify_value(second_type, NULL, (char*)&*iter, str);

                    if((iter + 1) != vec->end()) {
                        str << ",";
                    }
                }
                str << "]}";
            }
        }
        else {
            str << "\"{unknown type: " << type << "}\"";
        }
    }
}

