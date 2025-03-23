# Instalación

```
sudo apt update

sudo apt install git -y

```

# Iniciar rama desde un commit antiguo (hay que utilizar su identificador, que se encuentra en github)

```

git branch nueva_rama identificador_del_commit

git branch

git checkout nueva_rama

```

# Inicio en git y asociarlo a un repositorio de GitHub

```

git config --global user.name "jorge"

git config --global user.email "j.avilaorero@gmail.com"

git init (en la carpeta que queremos hacer repositorio local)

git add . (añade al repo local todo lo que hay en esa carpeta)

git commit -m "first commit"

git branch -M main

git remote add origin https://github.com/JorgeAvila102/panter_ws.git

git push -u origin main

```


# Añadir, commit y push de nuevos archivos

```

git status

git add nombre_archivo (que queremos subir) ( . para subir todos los archivos)

git commit -m "comentario"

git push -u origin main

```

# Crear nueva rama

```
git branch nombre_rama

```

# Eliminar rama

```
git branch -D nombre_rama

```


# Ver ramas

```
git branch

```
# Cambiar de rama

```
git checkout rama_destino

```

# Diferencia entre dos ramas

```
git diff rama1 rama2

```

# Fusionar codigo de rama2 a la main

```
git branch (para ver que estamos en la main)

git merge rama2 (pone en la main lo que hay en la rama2)

git push origin main
```
