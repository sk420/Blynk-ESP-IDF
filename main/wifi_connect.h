#ifndef _WIFI_CONNECT_H_
#define _WIFI_CONNECT_H_

#ifdef __cplusplus
extern "C" {
#endif

void wifi_begin(void);
void get_device_service_name(char *service_name, size_t max, const char *prefix);
void wifi_reset(void);
bool wifi_is_connected(void);

#ifdef __cplusplus
}
#endif
#endif // _WIFI_CONNECT_H_