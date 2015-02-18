#include <era-tue/configuration/writer.h>
#include <era-tue/configuration/reader.h>

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
    configuration::Data data;
    configuration::Writer w(data);

    w.writeGroup("test");
    w.setValue("a", 5);
    w.setValue("b", 123);
    w.endGroup();

    w.writeArray("array");

    for(int i = 0; i < 12; ++i)
    {
        w.addArrayItem();
        w.setValue("a", i);
        w.setValue("b", i * 2);
        w.endArrayItem();
    }

    w.endArray();

    // - - - - - - -

    configuration::Reader r(data);

    if (!r.readArray("array"))
        std::cout << "Unknown array" << std::endl;

    while(r.nextArrayItem())
    {
        showValue(r, "a");
    }
    r.endArray();

    if (!r.readGroup("test"))
        std::cout << "Unknown group" << std::endl;

    showValue(r, "b");

    return 0;
}
