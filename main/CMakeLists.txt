idf_component_register(
    SRCS "app_main.c" "bl0942_driver.c" "rainmaker_params.c" "relay_control.c" "power_monitor.c"
    INCLUDE_DIRS "."
    REQUIRES esp_rainmaker app_ota nvs_flash driver esp_wifi esp_netif
)