#include <iostream>
#include <cstdio>

#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"

#include "application/application.hpp"

int main()
{
    FILE *fp = fopen("config.json", "rb");
    char buffer[65536];
    rapidjson::FileReadStream is(fp, buffer, sizeof(buffer));
    rapidjson::Document doc;
    doc.ParseStream(is);
    fclose(fp);

    auto app = Application(doc, 1900, 1050);

    app.set_time_scale(100);
    app.set_satellites_scale(20);
    app.run();
    return 0;
}
