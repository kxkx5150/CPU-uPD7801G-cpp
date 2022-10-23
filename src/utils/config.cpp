#include <stdlib.h>
#include "config.h"
#include "../fileio.h"


config_t config;

void init_config()
{
    config.version = CONFIG_VERSION;

    for (int i = 0; i < 8; i++) {
#ifdef USE_CART
        _tcscpy(config.recent_cart[i], (""));
#endif
#ifdef USE_FD1
        for (int j = 0; j < 4; j++)
            _tcscpy(config.recent_disk[j][i], (""));
#endif
#ifdef USE_DATAREC
        _tcscpy(config.recent_datarec[i], (""));
#endif
#ifdef USE_MEDIA
        _tcscpy(config.recent_media[i], (""));
#endif
    }
    config.cpu_power = 0;
#ifdef USE_FD1
    config.ignore_crc = false;
#endif
#ifdef USE_SCANLINE
    config.scan_line = false;
#endif
#ifdef USE_MONITORYPE
    config.monitor_type = 0;
#endif
    config.sound_frequency = 3;
    config.sound_latency   = 0;
}

void load_config()
{
    // init_config();

    // char app_path[MAX_PATH], config_path[MAX_PATH];
    // GetModuleFileName(NULL, app_path, MAX_PATH);
    // int pt = _tcslen(app_path);
    // while (app_path[pt] != '\\')
    //     pt--;
    // app_path[pt + 1] = '\0';

    // _stprintf(config_path, ("%s%s.cfg"), app_path, (CONFIG_NAME));


    // FILEIO *fio = new FILEIO();
    // if (fio->Fopen(config_path, FILEIO_READ_BINARY)) {
    //     fio->Fread((void *)&config, sizeof(config), 1);
    //     fio->Fclose();
    // }
    // delete fio;

    // if (config.version != CONFIG_VERSION)
    //     init_config();
}

void save_config()
{
    char app_path[MAX_PATH], config_path[MAX_PATH];
    // GetModuleFileName(NULL, app_path, MAX_PATH);
    int pt = _tcslen(app_path);
    while (app_path[pt] != '\\')
        pt--;
    app_path[pt + 1] = '\0';
    _stprintf(config_path, ("%s%s.cfg"), app_path, (CONFIG_NAME));

    FILEIO *fio = new FILEIO();
    if (fio->Fopen(config_path, FILEIO_WRITE_BINARY)) {
        fio->Fwrite((void *)&config, sizeof(config), 1);
        fio->Fclose();
    }
    delete fio;
}
