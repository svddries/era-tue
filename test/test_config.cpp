#include <era-tue/configuration/writer.h>
#include <era-tue/configuration/reader.h>
#include <era-tue/configuration/parser.h>
#include <era-tue/configuration/emitter.h>


#include <iostream>

// ----------------------------------------------------------------------------------------------------

void showValue(configuration::Reader& r, const std::string& key)
{
    int v;
    if (r.value(key, v))
        std::cout << key << " = " << v << std::endl;
    else
        std::cout << "Unknown key: '" << key << "'." << std::endl;
}

// ----------------------------------------------------------------------------------------------------

int main(int argc, char **argv)
{
    if (argc > 1)
    {
        configuration::Data data;
        configuration::Parser p(data);
        p.readFile(argv[1]);

        configuration::emit(data, std::cout, configuration::EMIT_JSON);

        return 0;
    }

    configuration::Data data;
    configuration::Writer w(data);

    w.writeGroup("test");
    w.setValue("a", 5);
    w.setValue("b", 123);

    w.writeArray("array");

    for(int i = 0; i < 12; ++i)
    {
        w.addArrayItem();
        w.setValue("a", i);
        w.setValue("b", i * 2);
        w.endArrayItem();
    }

    w.endArray();

    w.endGroup();

    std::cout << "----------" << std::endl;

    configuration::emit(data, std::cout, configuration::EMIT_JSON | configuration::EMIT_MINIMAL);

    std::cout << "----------" << std::endl;

    // - - - - - - -

    configuration::Reader r(data);

    if (!r.readGroup("test"))
        std::cout << "Unknown group" << std::endl;

    if (r.readArray("array"))
    {
        while(r.nextArrayItem())
        {
            showValue(r, "a");
            showValue(r, "b");
        }
        r.endArray();
    }
    else
    {
        std::cout << "Unknown array" << std::endl;
    }

    showValue(r, "b");

    return 0;
}
