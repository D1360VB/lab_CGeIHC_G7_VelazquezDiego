# Repositorio del Laboratorio de Computación Gráfica e Interacción Humano-Computadora

Este proyecto es parte del laboratorio de **Computación Gráfica e Interacción Humano-Computadora**.  
Consiste en el repositorio de las distintas prácticas y previos hechos en distintas bibliotecas gráficas en C++.

---
## 🚀 Requisitos

- g++ (o cualquier compilador con soporte para C++11 o superior)
- OpenGL
- GLFW
- GLEW

En Debian/Ubuntu puedes instalarlos con:

```bash
sudo apt update
sudo apt install build-essential libglfw3-dev libglew-dev libgl1-mesa-dev
```

*A partir del previo 6, se descarga adicionalmente:
```bash
sudo apt install libassimp-dev libsoil-dev
```

🔧 Compilación
```bash
g++ <programa_a_compilar>.cpp -o <ejecutable_de_programa_a_compilar> -lglfw -lGLEW -lGL
```

*Para compilar a partir del previo 6:
```bash
g++ Carga_de_Modelos.cpp -Iinclude -o <ejecutable_de_programa_a_compilar> -lglfw -lGLEW -lGL -lSOIL -lassimp
```

▶️ Ejecución
```bash
./<ejecutable_de_programa>
```
---

## Capturas de resultado de las distintas prácticas


Practica 02 - Resultado

<img width="814" height="600" alt="Screenshot From 2025-08-24 23-19-53" src="https://github.com/user-attachments/assets/bb51d34e-8dcc-4a2d-8425-6644c9866331" />

