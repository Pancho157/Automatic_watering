# Tabla de contenidos

1. [Instalation](#instalation)
1. [Conections](#conections)
1. [Modules](#modules)
1. [Usage](#usage)

## Instalation

---

El estudiante deberá desarrollar la navegabilidad básica de la aplicación, demostrando que la app permite ver el catálogo, y navegar a un detalle

## Conections

---

Sensor N°1: D4
Sensor N°2: D4

Rele: D6

Control del rele activado por sensor N°2: D7

Display LCD (con I2C): 

- SDA: A4 
- SCL: A5

After button: D1
Before button: D2
Select button: D3

(Los botones se conectan a gnd y vcc, sacando un cable que verifica cuando se cierra el circuito de los mismos y dicho cable se conecta a los puertos mencionados anteriormente)

<img src="./src/images/paraElReadme/funcionalidadBasica.gif" width="900" />

## Modules

---

1. Rutas a configurar

   - ‘/’ navega a <ItemListContainer />

   - ‘/category/:id’ <ItemListContainer />

   - ‘/item/:id’ navega a <ItemDetailContainer />

2. Links a configurar

   - Clickear en el brand debe navegar a ‘/’

   - Clickear un Item.js debe navegar a /item/:id

   - Clickear en una categoría del navbar debe navegar a /category/:categoryId

Para finalizar deberá integrar los parámetros de tus async-mocks para reaccionar a :itemId y :categoryId ¡utilizando efectos y los hooks de parámetros que vimos en clase! Si te encuentras en una categoría deberías poder detectar la navegación a otra categoría y volver a cargar los productos que correspondan a dicha categoría

## Usage

---

El error que presenta el gif se debe a que se inspeccionó la página para mostrar que la misma es responsive
<img src="./src/images/paraElReadme/funcionalidadBasica.gif" width="900" />
