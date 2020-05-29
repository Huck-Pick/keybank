# keybank
keybank es un pequeño gestor de contraseñas de al menos 512 bits de seguridad, escrito en c++ para usar desde la terminal de Windows. Cifra las llaves basandose en 6 mapas caóticos descritos en el artículo "Integrated chaotic systems for image encryption" ayudado por un hash de 256 bits.

# Características básicas
```sh
> keybank init
```
- Crea un nuevo usuario
    - Pide un nombre de usuario
        - Identificador unico
    - Pide una contraseña local
        - Usada para cifrar el sitio y correo ligado a la contraseña
    - Pide una contraseña maestra
        - Usada para cifrar la constraseña ligada al correo

```sh
> keybank <NOMBRE_USUARIO> <OPCION>
```
- Opciones
    - `add` 
        - Añade nueva llave al usuario
    - `rm`   
        - Borra una llave del usuario
    - `edit`   
        - edita una llave del usuario
    - `clip`    
        - Copia a portapapeles por 30 seg una llave almacenada
    - `edit_user`    
        - Edita: nombre de usuario, contraseña local y contraseña maestra
    - `rm_user`    
        - Borra usuario (No se puede deshacer)
```sh
> keybank
```
- despliega el menú de opciones.
    
# Recursos
   - g++ 9.2.0
   - Windows.h
# Instalacion
```sh
> git clone https://github.com/Huck-Pick/keybank.git
> cd .\sourse\
> g++ -o .\keybank.exe .\keybank.cpp
> xcopy .\keybank.exe ..\keybank\
```
Despues deben de copiar la carpeta de `keybank` a `C:\Program Files\` , y añadir un nuevo path a las variables de entorno de usuario, para ello van a: 

`Editar variables de entorno del sistema >  variables de entorno > variables de usuario > path > nuevo` 

y escriben `C:\Program Files\keybank\` 

Listo! podran ejecutar el programa desde cualquier directorio en terminal windows.