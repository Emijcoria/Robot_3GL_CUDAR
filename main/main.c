#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"


#define LED1 2


uint8_t led_level = 0;
uint8_t count = 0;
static const char *tag = "Main";


esp_err_t init_motors(void);
esp_err_t init_teclas(void);

const int teclaspin[8] = {4, 5, 16, 17, 18, 19, 21, 22}; //GPIOs de ESP32 
const int M1STEPDIR[3] = {0,0,0}; //X  pasos/(adelante/atras)/enable
const int M2STEPDIR[3] = {0,0,0}; //Y  pasos/(horario/antihorario)/enable    EL ENABLE ES COMUN ENTRE LOS TRES
const int M3STEPDIR[3] = {0,0,0}; //Z  pasos/(arriba/abajo)/enable




void app_main(void)
{

    init_motors();
    init_teclas();

    while(1){
        
        vTaskDelay(200 / portTICK_PERIOD_MS); //200ms delay
        
        select = -1;
        for (int i = 0; i<8;i++){
        
            if(gpio_get_level(teclaspin[i]) == 0){
                select = teclaspin[i];
                break; //toma el primero que encuentra


            }

        }

        ESP_LOGI(tag, "Tecla presionada %d\n", select);
    



        switch(select){
        
        case 4:
            ESP_LOGI(tag, "Tecla 1 presionada\n");

            gpio_set_level(M1STEPDIR[2], 0); //EN
            gpio_set_level(M1STEPDIR[1], 1); //DIR
            gpio_set_level(M1STEPDIR[0], 1); //STEP
            gpio_set_level(M1STEPDIR[0], 0);
            gpio_set_level(M1STEPDIR[2], 1); //EN



            break;
        case 5:
            ESP_LOGI(tag, "Tecla 2 presionada\n");
            break;
        case 16:
            ESP_LOGI(tag, "Tecla 3 presionada\n");
            break;
        case 17:
            ESP_LOGI(tag, "Tecla 4 presionada\n");
            break;
        case 18:
            ESP_LOGI(tag, "Tecla 5 presionada\n");
            break;
        case 19:
            ESP_LOGI(tag, "Tecla 6 presionada\n");
            break;
        case 21:
            ESP_LOGI(tag, "Tecla 7 presionada\n");
            break;
        case 22:
            ESP_LOGI(tag, "Tecla 8 presionada\n");
            break;
        default:
            ESP_LOGW(tag, "Tecla no reconocida\n");
            break;
        }

       
        
       
    }

}


esp_err_t mov_motor(gpio_num_t step_pin, gpio_num_t dir_pin, gpio_num_t en_pin, int steps, int direction){
    
    gpio_set_level(en_pin, 0); //Habilitar motor
    gpio_set_level(dir_pin, direction); //Establecer dirección

    for (int i = 0; i < steps; i++){
        gpio_set_level(step_pin, 1); //Paso
        vTaskDelay(10 / portTICK_PERIOD_MS); //Pequeña demora entre pasos
        gpio_set_level(step_pin, 0);
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }

    gpio_set_level(en_pin, 1); //Deshabilitar motor
    return ESP_OK;
}

esp_err_t init_motors(void){

  for (int i = 0; i < 3; i++){

    gpio_reset_pin(M1STEPDIR[i]);
    gpio_set_direction(M1STEPDIR[i], GPIO_MODE_OUTPUT);

    gpio_reset_pin(M2STEPDIR[i]);
    gpio_set_direction(M2STEPDIR[i], GPIO_MODE_OUTPUT);

    gpio_reset_pin(M3STEPDIR[i]);
    gpio_set_direction(M3STEPDIR[i], GPIO_MODE_OUTPUT);
  }

  return ESP_OK;
}

esp_err_t init_teclas(void){

  for (int i = 0; i < 8; i++) {

    gpio_reset_pin(teclaspin[i]);
    gpio_set_direction(teclaspin[i], GPIO_MODE_INPUT);
  }

  return ESP_OK;
}

