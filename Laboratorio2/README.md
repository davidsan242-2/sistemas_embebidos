# Laboratorio No. 2

UNIVERSIDAD DEL VALLE\
FACULTAD DE INGENIERÍA\
ESCUELA DE INGENIERÍA ELÉCTRICA Y ELECTRÓNICA\
DISEÑO E IMPLEMENTACIÓN DE SISTEMAS EMBEBIDOS\
DOCENTE: BREYNER POSSO

### Integrantes
**Andrés Felipe Marín Ramírez: 202502859**\
**David Esteban Sandoval Flórez: 202507595**\
**Fecha: 2025-12-12**

[Enlace al documento del laboratorio en formato PDF](ESDI_2025-2_Lab-1_v2.pdf)

[Challenge-1](./Challenge-1)\
[Challenge-2](./Challenge-2)

### Instrucciones de despliegue

1. Adicionar boards de ESP32 en entorno de Arduino

- Digirise al menú Archivo/Preferencias
- Colocar la siguiente URL en el campo de Gestores de boards adicionales: https://dl.espressif.com/dl/package_esp32_index.json
- Dirigirse luego al menú Tools/Board/Boards manager y buscar ESP32
- Instalar ESP32 by Espressif Systems

2. Configurar la placa ESP32 S3 DevKit C
- Dirigirse luego al menú Tools/Board/ESP32 y escoger ESP32S2 Dev Module
- En el menú Tools escoger puerto serial /dev/ttyACM0
- En el menú Tools/USB CDC On Boot escoger la opción Enabled.

3. Subir los sketches
- Abrir el sketch del challenge respectivo en File/Open
- Dar click en el botón Upload de la barra de herramientas
- Abrir la consola serial en Tools/Serial Monitor
