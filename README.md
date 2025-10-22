# Como compilar

## Requisitos

1. Linux/WSL (opicional, mas facilita)
2. C++ compiler (g++ ou Clang)
3. Cmake (use snap pra instalar, tem a versão mais atualizada)

## Compilação

Dentro do diretório do projeto:
```bash
mkdir -p build && cd build
cmake ..
cmake --build . -j
./chromasnake
```
