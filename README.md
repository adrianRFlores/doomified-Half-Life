# doomified-Half-Life
En honor al 25 aniversario de Half-Life, se hizo un raycaster al estilo Doom/Wolfenstein con la estética y los sonidos de Half-Life.
Para este minijuego se asume control de Gordon Freeman, con el objetivo de escapar de los laboratorios de Black Mesa luego de la cascada de resonancia.

Aunque no hay enemigos, Gordon cuenta con 2 armas:
1. Su icónico crowbar
2. Una pistola simple

## Controles

- El juego inicia con un menú principal, el cual es posible cerrar al presionar la tecla Enter.
- Para voltear a los lados, se pueden utilizar las teclas a y d. Además, el movimiento del mouse en el eje horizontal también se puede utilizar para ver a los lados.
- Para avanzar o retroceder, se utiliza w o s respectivamente.
- Si se cuenta con un control, el juego reconoce un Joystick para que asuma el rol del mouse, funcionando de manera idéntica. Además, el movimiento se puede realizar con el D-Pad del control, si es que posee uno.
- Para cerrar el juego, se puede presionar la tecla Escape en cualquier momento.

## Videos de funcionamiento

### Gameplay general, con música y efectos de sonido

[Ver en Youtube](https://youtu.be/VGf3H2xAtrs)

### Funcionamiento con control

![control](https://github.com/adrianRFlores/doomified-Half-Life/assets/84111818/7852dadc-ecfc-43c2-ad41-95bcede753a9)

### Funcionamiento con mouse

![mouse](https://github.com/adrianRFlores/doomified-Half-Life/assets/84111818/6152a98d-d403-4eef-a803-99fc389a81ad)

## Características

- Soporte para control
- Cámara con movimiento hacia adelante y hacia atrás, con rotación
- Música de fondo
- Efectos de Sonido
- Animación de sprites
- Pantalla de bienvenida

## Compilar
Utilizar MinGW con ``mingw32-make.exe`` dentro del directorio del Makefile. SDL, SDL_image, SDL_Mixer y GLM vienen empacados dentro de la carpeta src.
``mingw32-pthreads-w32`` debe estar instalado en MinGW para poder utilizar threading.
