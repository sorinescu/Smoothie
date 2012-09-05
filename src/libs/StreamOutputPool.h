/*
      This file is part of Smoothie (http://smoothieware.org/). The motion control part is heavily based on Grbl (https://github.com/simen/grbl).
      Smoothie is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
      Smoothie is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
      You should have received a copy of the GNU General Public License along with Smoothie. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef STREAMOUTPUTPOOL_H
#define STREAMOUTPUTPOOL_H

#include <cstdio>
#include <cstdarg>

#include "libs/StreamOutput.h"

class StreamOutputPool
{
public:
    StreamOutputPool(){
        msg_size = 0;
    }

    void printf(const smt_string format, ...){
        // Make the message
        va_list args;
        va_start(args, format);

        if (!msg_size){
            msg_size = format.size() + 1;
            message = new char[msg_size];
        }

        int size = vsnprintf(message, msg_size, format.c_str(), args);
        if (size >= msg_size)
        {
            delete[] message;
            msg_size = size * 2;
            message = new char[msg_size];
        }

        vsprintf(message, format.c_str(), args);
        va_end(args);

        // Dispatch to all
        for (unsigned int i=0; i < this->streams.size(); i++)
            this->streams.at(i)->printf(message);
    }

    void append_stream(StreamOutput* stream){
        this->streams.push_back(stream);
    }

    smt_vector<StreamOutput*>::type streams;

protected:
    char *message;
    int msg_size;
};

#endif
