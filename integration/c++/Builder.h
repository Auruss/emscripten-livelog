#pragma once

#include <vector>
#include <sstream>

#include <Common/LiveLog/Types.h>
#include <Common/LiveLog/Reflection.h>

namespace Common {
    namespace LiveLog {

        class Builder {

        public:
            Builder(int log_id, int global_id);
            ~Builder();

            void setMessage(const char *format, ...);

            void addRefObj(const char* name, ReflObject *refl, void* data_ptr);

            void push();

        private:
            void jsonify(ReflObject *refl, std::stringstream& str, const char* name, char* data);
            void jsonify_value(const char* type, const char* second_type, char* data, std::stringstream& str);

            int _log_id;
            const char *_message;

            std::vector<const char *> _refs;

        };

    }
}