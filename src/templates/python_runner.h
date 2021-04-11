#ifndef PYTHON_RUNNER_H
#define PYTHON_RUNNER_H

#ifdef PYTHON_SCRIPTING
#include "pyembed.h"
#endif

#include <map>
#include <string>
#include <set>

struct python_runner
{

    PyObject* pymodule = nullptr;
    PyObject *main = nullptr;

    std::string buffer;

    explicit python_runner()
    {
        PyImport_AppendInittab("emb", emb::PyInit_emb);
        Py_Initialize();

        pymodule = PyImport_ImportModule("emb");
        main = PyImport_AddModule("__main__");
    }

    std::string run(std::map<std::string, std::string>& kps, std::string between, const std::vector<std::string>& all_variables)
    {
        std::string buffer;
        emb::stdout_write_type write = [&buffer] (std::string s) { buffer += s; };
        emb::set_stdout(write);

        // generate a list of assignments to kp/kv values from the kps vector
        std::set<std::string> set_variables;
        for(const auto& [kp, kv] : kps)
        {
            std::string cmd = kp + "='" + kv + "'\n";
            between = cmd + between;
            set_variables.insert(kp);
        }

        // here gather all the other variables that might have been in the template
        for(const auto& v : all_variables)
        {
            if(set_variables.count(v) == 0)
            {
                std::string cmd = v + "=''\n";
                between = cmd + between;
                set_variables.insert(v);
            }
        }

        // now gather all the structure definitions

        //std::cerr << "Trying to run:" << std::endl<< between << std::endl;

        PyRun_SimpleString(between.c_str());
        emb::reset_stdout();

        PyObject *globals = PyModule_GetDict(main);
        if(globals)
        {
            for(const auto& v : set_variables)
            {
                PyObject *a = PyDict_GetItemString(globals, v.c_str());
                if(a)
                {
                    PyObject* temp_bytes = PyUnicode_AsEncodedString( a, "UTF-8", "strict" );
                    if (temp_bytes)
                    {
                        std::string r = PyBytes_AS_STRING( temp_bytes );
                        kps[v] = r;

                        Py_DECREF( temp_bytes );
                    }
                    Py_DECREF(a);
                }
            }

            Py_DECREF(globals);
        }

        return buffer;
    }

    virtual ~python_runner()
    {
        Py_DECREF(main);
        Py_DECREF(pymodule);
        Py_Finalize();
    }

};

#endif // PYTHON_RUNNER_H
